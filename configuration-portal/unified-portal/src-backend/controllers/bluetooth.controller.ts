import BLEManager from "../util-managers/BLEManager";
import NodeBleHost from "ble-host";
const AttErrors = NodeBleHost.AttErrors;

enum UUIDS {
    SERVICE_MAIN = "2c82b713-f76a-4696-98eb-d92f9f233f40",
    CHAR_BANK_INFO = "5b278f16-4460-47e1-919e-2d50d7d0a55d",
    CHAR_REFRESH_RATE = "49dc2b22-2dc4-4a66-afee-d7782b9b81cd",
    CHAR_GOAL = "8f71bd04-89f7-4290-b90f-ac1265f5f127",
    CHAR_DAYS_LEFT = "c27c1205-9ccb-4d1f-999f-0b9cfabf1d09"
};

class GATTInformation {
    public bankInfo: object;
    public refreshRate: number;
    public goal: number;
    public cycleStartDate: Date;

    constructor({ refreshRate, goal, cycleStartDate }) {
        this.refreshRate = refreshRate;
        this.goal = goal;
        this.cycleStartDate = cycleStartDate;
    }

    private services = [
        {
            "uuid": UUIDS.SERVICE_MAIN,
            "characteristics": [
                this.computeCharacteristic(UUIDS.CHAR_BANK_INFO, "bankInfo", "object"),
                this.computeCharacteristic(UUIDS.CHAR_REFRESH_RATE, "refreshRate", "number"),
                this.computeCharacteristic(UUIDS.CHAR_GOAL, "goal", "number"),
                {
                    "uuid": UUIDS.CHAR_DAYS_LEFT,
                    "properties": ["read"],
                    onRead: (_, callback) => {
                        return this.cycleStartDate; // todo this is super wrong... fix to correctly diff the next cycle with current date
                    }
                }
            ]
        }
    ];

    computeCharacteristic(uuid, propertyName, propType) {
        let onRead;
        switch (propType) {
            case "string":
                onRead = (_, callback) => {
                    callback(AttErrors.SUCCESS, this[propertyName]);
                }
                break;
            case "number":
                onRead = (_, callback) => {
                    callback(AttErrors.SUCCESS, JSON.stringify({ value: this[propertyName] }));
                }
                break;
            case "object":
                onRead = (_, callback) => {
                    callback(AttErrors.SUCCESS, JSON.stringify(this[propertyName]));
                }
                break;
        }

        return {
            "uuid": uuid,
            "properties": ["read"],
            onRead
        };
    }

    getServices() {
        return this.services;
    }
};

class BluetoothController {
    private resolvePairingRequest;
    private rejectPairingRequest;
    private bleManager;

    public gattInformation: GATTInformation;
    public totalAmount: number = 0;

    constructor({ refreshRate, goal, cycleStartDate }) {
        this.gattInformation = new GATTInformation({ refreshRate, goal, cycleStartDate });
        const SERVICES = this.gattInformation.getServices();
        console.log(SERVICES);

        this.bleManager = new BLEManager();
        this.bleManager.init("Finware", SERVICES, this.passkeyHandler.bind(this)).then(() => {
            this.bleManager.startAdvertising();
            console.log("BLE initialized");
        });
    }

    private passkeyHandler() {
        console.log(`passkeyHandler Invoked`);
        return new Promise((resolve, reject) => {
            this.resolvePairingRequest = resolve;
            this.rejectPairingRequest = reject;
        });
    }

    public fetchState(req, res) {
        res.json({ connection: this.bleManager.connectionStatus, passkey: this.bleManager.passkey });
    }

    public reset(req, res) {
        this.bleManager.resetConnection();
        res.json({ status: "SUCCESS" });
    }

    public acceptPairing(req, res) {
        this.resolvePairingRequest(req.body.passkey);
        res.json({ status: "SUCCESS" });
    }

    public rejectPairing(req, res) {
        this.rejectPairingRequest();
        this.bleManager.resetConnection();
        res.json({ status: "SUCCESS" });
    }

}

export default BluetoothController;