import HciSocket from 'hci-socket';
import NodeBleHost from 'ble-host';

const COMPARISON_FAILED = 0xC;

const BleManager = NodeBleHost.BleManager;
const AdvertisingDataBuilder = NodeBleHost.AdvertisingDataBuilder;
const HciErrors = NodeBleHost.HciErrors;
const AttErrors = NodeBleHost.AttErrors;
const IOCapabilities = NodeBleHost.IOCapabilities;
const AssociationModels = NodeBleHost.AssociationModels;
const SmpErrors = NodeBleHost.SmpErrors;

enum Connection {
    CONNECTED = "CONNECTED",
    PAIRING = "PAIRING",
    ADVERTISING = "ADVERTISING"
}

export default class BLEManager {
    private manager: any;
    private deviceName: string;
    private passkeyHandler: Function;
    private btConnection;

    public passkey: string;
    public connectionStatus: Connection;

    private initAdvData(manager, services) {
        const advDataBuffer = new AdvertisingDataBuilder()
            .addFlags(['leGeneralDiscoverableMode', 'brEdrNotSupported'])
            .addLocalName(/*isComplete*/ true, this.deviceName)
            .add128BitServiceUUIDs(/*isComplete*/ true, services.map((service) => service.uuid))
            .build();
        manager.setAdvertisingData(advDataBuffer);
    }

    private newPairingHandler(status, conn) {
        if (status != HciErrors.SUCCESS) {
            // Advertising could not be started for some controller-specific reason, try again after 10 seconds
            setTimeout(this.startAdvertising, 10000);
            return;
        }

        this.btConnection = conn;
        conn.on('disconnect', (errorCode) => {
            console.log("Bluetooth was disconnected");
            this.btConnection = null;
            this.startAdvertising();
        }); // restart advertising after disconnect

        console.log('Connection established!');
        console.log(conn);
        console.log(conn.smp);

        this.connectionStatus = Connection.PAIRING;
        conn.smp.sendSecurityRequest(/*bond*/ true, /*mitm*/ true, /*sc*/ true, /*keypress*/ false);

        conn.smp.on('encrypt', (status, currentEncryptionLevel) => {
            if (status != HciErrors.SUCCESS) {
                console.log('Could not start encryption due to ' + HciErrors.toString(status));
                return;
            }
            this.connectionStatus = Connection.CONNECTED;
            console.log('The encryption process is now complete!');
            console.log('MITM protection: ' + currentEncryptionLevel.mitm);
            console.log('LE Secure Connections used: ' + currentEncryptionLevel.sc);
        });

        // Without this event handler the I/O capabilities will be no input, no output
        conn.smp.on('pairingRequest', (req, callback) => {
            console.log("Pairing request");
            callback({ ioCap: IOCapabilities.KEYBOARD_ONLY, bondingFlags: 1, mitm: true, sc: true});
        });

        conn.smp.on('passkeyExchange', (associationModel, passkey, callback) => {
            console.log('Security in passkeyExchange: ', conn.smp.currentEncryptionLevel);

            if (associationModel == AssociationModels.PASSKEY_ENTRY_RSP_INPUTS) {
                console.log(`PASSKEY_ENTRY_RSP_INPUTS got code: ${passkey}:`);
                this.passkey = passkey;
                this.passkeyHandler()
                    .then((passkey) => {
                        console.log("passkey entry returned to blemanager, now calling callback(passkey)");
                        callback(passkey);
                    })
                    .catch(() => {
                        console.log("PASS_KEY entry failed at the RPi");
                        conn.smp.sendPairingFailed(COMPARISON_FAILED);
                    });
            }
        });

        conn.smp.on('pairingComplete', (resultObject) => {
            this.connectionStatus = Connection.CONNECTED;
            console.log('The pairing process is now complete!');
            console.log('MITM protection: ' + conn.smp.currentEncryptionLevel.mitm);
            console.log('LE Secure Connections used: ' + conn.smp.currentEncryptionLevel.sc);
            // Put logic here, e.g. read a protected characteristic
        });

        conn.smp.on('pairingFailed', (reason, isErrorFromRemote) => {
            console.log('Pairing failed with reason ' + SmpErrors.toString(reason));
        });

    }

    public init(deviceName: string, services: object[], passkeyHandler: (pinCode: string) => Promise<any>,) {
        this.deviceName = deviceName;
        this.passkeyHandler = passkeyHandler;

        var transport = new HciSocket(); // connects to the first hci device on the computer, for example hci0
        var options = {
            // optional properties go here
        };

        const initPromise = new Promise((resolve, reject) => {
            BleManager.create(transport, options, (err, manager) => {
                if (err) {
                    reject(err);
                    return;
                }
                this.manager = manager;
                manager.gattDb.addServices(services);
                this.initAdvData(manager, services);
                manager.gattDb.setDeviceName(deviceName);

                resolve("SUCCESS");
            });
        });

        return initPromise;
    }

    public startAdvertising() {
        if (this.connectionStatus != Connection.ADVERTISING) {
            this.connectionStatus = Connection.ADVERTISING;
            this.manager.startAdvertising({ /*options*/ }, this.newPairingHandler.bind(this));
            console.log("Started advertising");
        }
    }

    public stopAdvertising() {
        // TODO
    }

    public disconnect() {
        this.btConnection && this.btConnection.disconnect(); // This will also start readvertising
        this.btConnection = null;
    }

    public resetConnection() {
        this.disconnect();
        this.startAdvertising();
    }
}

