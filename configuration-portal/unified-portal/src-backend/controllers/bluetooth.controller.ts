import BLEManager from "../util-managers/BLEManager";
import NodeBleHost from "ble-host";
const AttErrors = NodeBleHost.AttErrors;


class BluetoothController {
    private resolvePairingRequest;
    private rejectPairingRequest;
    private bleManager;

    public totalAmount: number = 0;

    constructor() {
        const SERVICES = [
            {
                "uuid": "22222222-3333-4444-5555-666666666666",
                "characteristics": [
                    {
                        "uuid": "22222222-3333-4444-5555-666666666667",
                        "properties": [
                            "read"
                        ],
                        onRead: (connection, callback) => {
                            callback(AttErrors.SUCCESS, this.totalAmount.toString());
                        }
                    }
                ]
            }
        ]
        this.bleManager = new BLEManager();

        // this.bleManager.init("myDevice", SERVICES, (pincode) => {
        //     console.log(`Received pincode in index.ts: ${pincode}`)
        //     return new Promise((resolve, reject) => {
        //         this.resolvePairingRequest = resolve;
        //         this.rejectPairingRequest = reject;
        //     });
        // }).then(() => {
        //     console.log("BT initialized");
        //     this.bleManager.startAdvertising();
        // });

        this.bleManager.init("myDevice", SERVICES, () => {
            console.log(`passcodeHandler Invoked`);
            return new Promise((resolve, reject) => {
                this.resolvePairingRequest = resolve;
                this.rejectPairingRequest = reject;
            });
        }).then(() => {
            console.log("BT initialized");
            this.bleManager.startAdvertising();
        });
    }

    fetchState(req, res) {
        res.json({ connection: this.bleManager.connectionStatus, passkey: this.bleManager.passkey });
    }

    reset(req, res) {
        this.bleManager.resetConnection();
        res.json({ status: "SUCCESS" });
    }

    acceptPairing(req, res) {
        this.resolvePairingRequest(req.body.passkey);
        res.json({ status: "SUCCESS" });
    }

    rejectPairing(req, res) {
        this.rejectPairingRequest();
        this.bleManager.resetConnection();
        res.json({ status: "SUCCESS" });
    }

}

export default BluetoothController;