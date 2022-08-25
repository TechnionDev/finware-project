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

export default class BluetoothManager {
    private manager: any = null;
    private deviceName: string = null;
    private passcodeHandler: Function = null;
    private btConnection = null;

    public passcode: string = null;
    public connectionStatus: Connection = null

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
        conn.on('disconnect', () => {
            this.btConnection = null;
            this.startAdvertising()
        }); // restart advertising after disconnect

        console.log('Connection established!');

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
            callback({ ioCap: IOCapabilities.DISPLAY_YES_NO, bondingFlags: 1, mitm: true });
        });

        conn.smp.on('passkeyExchange', (associationModel, passcode, callback) => {
            console.log('Security in passkeyExchange: ', conn.smp.currentEncryptionLevel);

            if (associationModel == AssociationModels.NUMERIC_COMPARISON) {
                console.log(`NUMERIC_COMPARISON got code: ${passcode}:`);
                this.passcode = passcode;
                this.passcodeHandler(passcode)
                    .then(() => {
                        callback();
                    })
                    .catch(() => {
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

    public init(deviceName: string, services: object[], passcodeHandler: (pinCode: string) => Promise<any>,) {
        this.deviceName = deviceName;
        this.passcodeHandler = passcodeHandler;

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

