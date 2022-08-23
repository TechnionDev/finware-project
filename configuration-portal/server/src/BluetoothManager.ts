import HciSocket from 'hci-socket';
import NodeBleHost from 'ble-host';

const COMPARISON_FAILED = 0xC;

const BleManager = NodeBleHost.BleManager;
const AdvertisingDataBuilder = NodeBleHost.AdvertisingDataBuilder;
const HciErrors = NodeBleHost.HciErrors;
const AttErrors = NodeBleHost.AttErrors;

export default class BluetoothManager {
    manager: any = null;
    deviceName: string = null;
    passcode: string = null;
    passcodeHandler: Function = null;
    // TODO: add state (connected / advertising / ?)

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
        conn.on('disconnect', ()=>{this.startAdvertising()}); // restart advertising after disconnect

        console.log('Connection established!');
        console.log('Security: ', conn.smp.currentEncryptionLevel);

        const IOCapabilities = NodeBleHost.IOCapabilities;
        const AssociationModels = NodeBleHost.AssociationModels;
        const SmpErrors = NodeBleHost.SmpErrors;

        conn.smp.sendSecurityRequest(/*bond*/ true, /*mitm*/ true, /*sc*/ true, /*keypress*/ false);

        // Without this event handler the I/O capabilities will be no input, no output
        conn.smp.on('pairingRequest', function (req, callback) {
            callback({ ioCap: IOCapabilities.DISPLAY_YES_NO, bondingFlags: 1, mitm: true });
        });

        conn.smp.on('passkeyExchange', (associationModel, passcode, callback) => {
            console.log('Security in passkeyExchange: ', conn.smp.currentEncryptionLevel);

            if (associationModel == AssociationModels.NUMERIC_COMPARISON) {
                console.log(`NUMERIC_COMPARISON got code: ${passcode}:`);
                this.passcode = passcode;
                console.log("this:", this);
                this.passcodeHandler(passcode)
                    .then(() => {
                        callback();
                    })
                    .catch(() => {
                        conn.smp.sendPairingFailed(COMPARISON_FAILED);
                    });
            }
        });

        conn.smp.on('pairingComplete', function (resultObject) {
            console.log('The pairing process is now complete!');
            console.log('MITM protection: ' + conn.smp.currentEncryptionLevel.mitm);
            console.log('LE Secure Connections used: ' + conn.smp.currentEncryptionLevel.sc);
            // Put logic here, e.g. read a protected characteristic
        });

        conn.smp.on('pairingFailed', function (reason, isErrorFromRemote) {
            console.log('Pairing failed with reason ' + SmpErrors.toString(reason));
        });

    }

    public init(deviceName: string, services: object[], passcodeHandler: (pinCode: string) => Promise<any>,) {
        this.deviceName = deviceName;
        this.passcodeHandler = passcodeHandler;
        console.log("BLA:", passcodeHandler, typeof (passcodeHandler));

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
                manager.gattDb.addServices(services);
                this.initAdvData(manager, services);
                manager.gattDb.setDeviceName(deviceName);
                this.manager = manager;

                resolve("SUCCESS");
            });
        });

        return initPromise;
    }

    public startAdvertising() {
        this.manager.startAdvertising({ /*options*/ }, this.newPairingHandler.bind(this));
        console.log("Started advertising")
    }

    public stopAdvertising() {
        // TODO
    }


}


// var transport = new HciSocket(); // connects to the first hci device on the computer, for example hci0

// var options = {
//     // optional properties go here
// };

// console.log('Pre create bleManager');

// BleManager.create(transport, options, function (err, manager) {
//     // err is either null or an Error object
//     // if err is null, manager contains a fully initialized BleManager object
//     var notificationCharacteristic;

//     manager.gattDb.setDeviceName(deviceName);
//     manager.gattDb.addServices([
//         {
//             uuid: '22222222-3333-4444-5555-666666666666',
//             characteristics: [
//                 {
//                     uuid: '22222222-3333-4444-5555-666666666667',
//                     properties: ['read', 'write'],
//                     value: 'some default value' // could be a Buffer for a binary value
//                 },
//                 {
//                     uuid: '22222222-3333-4444-5555-666666666668',
//                     properties: ['read'],
//                     onRead: function (connection, callback) {
//                         callback(AttErrors.SUCCESS, new Date().toString());
//                     }
//                 },
//                 {
//                     uuid: '22222222-3333-4444-5555-666666666669',
//                     properties: ['write'],
//                     onWrite: function (connection, needsResponse, value, callback) {
//                         console.log('A new value was written:', value);
//                         callback(AttErrors.SUCCESS); // actually only needs to be called when needsResponse is true
//                     }
//                 },
//                 notificationCharacteristic = {
//                     uuid: '22222222-3333-4444-5555-66666666666A',
//                     properties: ['notify'],
//                     onSubscriptionChange: function (connection, notification, indication, isWrite) {
//                         if (notification) {
//                             // Notifications are now enabled, so let's send something
//                             notificationCharacteristic.notify(connection, 'Sample notification');
//                         }
//                     }
//                 }
//             ]
//         }
//     ]);

//     const advDataBuffer = new AdvertisingDataBuilder()
//         .addFlags(['leGeneralDiscoverableMode', 'brEdrNotSupported'])
//         .addLocalName(/*isComplete*/ true, deviceName)
//         .add128BitServiceUUIDs(/*isComplete*/ true, ['22222222-3333-4444-5555-666666666666'])
//         .build();
//     manager.setAdvertisingData(advDataBuffer);
//     // call manager.setScanResponseData(...) if scan response data is desired too
//     startAdv();

//     function startAdv() {
//         manager.startAdvertising({/*options*/ }, connectCallback);
//     }

//     function connectCallback(status, conn) {
//         if (status != HciErrors.SUCCESS) {
//             // Advertising could not be started for some controller-specific reason, try again after 10 seconds
//             setTimeout(startAdv, 10000);
//             return;
//         }
//         conn.on('disconnect', startAdv); // restart advertising after disconnect
//         console.log('Connection established!');


//         const IOCapabilities = NodeBleHost.IOCapabilities;
//         const AssociationModels = NodeBleHost.AssociationModels;
//         const SmpErrors = NodeBleHost.SmpErrors;

//         // console.log(conn.smp);
//         console.log('Security: ', conn.smp.currentEncryptionLevel);

//         conn.smp.sendSecurityRequest(/*bond*/ true, /*mitm*/ true, /*sc*/ true, /*keypress*/ false);

//         // Without this event handler the I/O capabilities will be no input, no output
//         conn.smp.on('pairingRequest', function (req, callback) {
//             callback({ ioCap: IOCapabilities.DISPLAY_YES_NO, bondingFlags: 1, mitm: true });
//             // callback({ ioCap: IOCapabilities.KEYBOARD_ONLY, bondingFlags: 1, mitm: true });
//         });

//         conn.smp.on('passkeyExchange', function (associationModel, userPasskey, callback) {
//             console.log('Security in passkeyExchange: ', conn.smp.currentEncryptionLevel);

//             if (associationModel == AssociationModels.NUMERIC_COMPARISON) {
//                 console.log(`NUMERIC_COMPARISON got code: ${userPasskey}:`);
//                 callback();
//             }
//         });

//         conn.smp.on('pairingComplete', function (resultObject) {
//             console.log('The pairing process is now complete!');
//             console.log('MITM protection: ' + conn.smp.currentEncryptionLevel.mitm);
//             console.log('LE Secure Connections used: ' + conn.smp.currentEncryptionLevel.sc);
//             // Put logic here, e.g. read a protected characteristic
//         });

//         conn.smp.on('pairingFailed', function (reason, isErrorFromRemote) {
//             console.log('Pairing failed with reason ' + SmpErrors.toString(reason));
//         });

//     }
// });
