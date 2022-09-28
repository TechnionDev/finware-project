import BLEManager from "../util-managers/BLEManager";
import NodeBleHost from "ble-host";
import { getCycleStartDate, getCycleEndDate, getCycleDayCount, datediff } from "./utils"
import Settings from "../models/Settings";

const AttErrors = NodeBleHost.AttErrors;

enum UUIDS {
    SERVICE_MAIN = "2c82b713-f76a-4696-98eb-d92f9f233f40",
    CHAR_BANK_INFO = "5b278f16-4460-47e1-919e-2d50d7d0a55d",
    CHAR_REFRESH_RATE = "49dc2b22-2dc4-4a66-afee-d7782b9b81cd",
    CHAR_GOAL = "8f71bd04-89f7-4290-b90f-ac1265f5f127",
    CHAR_DAYS_LEFT = "c27c1205-9ccb-4d1f-999f-0b9cfabf1d09",
    CHAR_GRAPH_DATA = "b8ed4639-8e38-4d0f-9ad6-5e46544f171a"
};

class GATTInformation {
    public bankInfo: object = {};
    public refreshRate: number;
    public goal: number;
    public cycleStartDay: number;
    public graphData: any;

    constructor({ refreshRate, goal}) {
        this.refreshRate = refreshRate;
        this.goal = goal;
    }

    private services = [
        {
            "uuid": UUIDS.SERVICE_MAIN,
            "characteristics": [
                this.computeCharacteristic(UUIDS.CHAR_BANK_INFO, "bankInfo", "object"),
                {
                    "uuid": UUIDS.CHAR_GOAL,
                    "properties": ["read"],
                    "readPerm": "encrypted-mitm-sc",
                    onRead: async (_, callback) => {
                        let settings = await Settings.findOneAndUpdate({}, {}, { upsert: true, new: true });
                        callback(AttErrors.SUCCESS, JSON.stringify({ value: settings.expense_budget }));
                    }
                },
                {
                    "uuid": UUIDS.CHAR_REFRESH_RATE,
                    "properties": ["read"],
                    "readPerm": "encrypted-mitm-sc",
                    onRead: async (_, callback) => {
                        let settings = await Settings.findOneAndUpdate({}, {}, { upsert: true, new: true });
                        callback(AttErrors.SUCCESS, JSON.stringify({ value: settings.display_refresh_frequency_minutes }));
                    }
                },
                {
                    "uuid": UUIDS.CHAR_DAYS_LEFT,
                    "properties": ["read"],
                    "readPerm": "encrypted-mitm-sc",
                    onRead: async (_, callback) => {
                        let settings = await Settings.findOneAndUpdate({}, {}, { upsert: true, new: true });
                        callback(AttErrors.SUCCESS, JSON.stringify({ value: datediff(new Date(), getCycleEndDate(settings.month_cycle_start_day)) }));
                    }
                },
                {
                    "uuid": UUIDS.CHAR_GRAPH_DATA,
                    "properties": ["read"],
                    "readPerm": "encrypted-mitm-sc",
                    onRead: async (_, callback) => {
                        let settings = await Settings.findOneAndUpdate({}, {}, { upsert: true, new: true });
                        this.graphData["cycleStartDate"] = getCycleStartDate(settings.month_cycle_start_day).toLocaleString(undefined, { month: "short", day: "numeric" });
                        this.graphData["cycleEndDate"] = getCycleEndDate(settings.month_cycle_start_day).toLocaleString(undefined, { month: "short", day: "numeric" });
                        this.graphData["daysInCycle"] = getCycleDayCount(settings.month_cycle_start_day);
                        this.graphData.data = this.graphData?.data.map(x => Math.round(x));
                        callback(AttErrors.SUCCESS, JSON.stringify(this.graphData));
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
            "readPerm": "encrypted-mitm-sc",
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

    constructor({ refreshRate, goal }) {
        this.gattInformation = new GATTInformation({ refreshRate, goal});
        const SERVICES = this.gattInformation.getServices();
        console.log(SERVICES);

        this.bleManager = new BLEManager();
        this.bleManager.init("Finware", SERVICES, this.passkeyHandler.bind(this)).then(() => {
            this.bleManager.startAdvertising();
            console.log("BLE initialized");
        });

        // setInterval(() => console.log("Gatt information: ", this.gattInformation), 15000);
        setTimeout(() => console.log("Gatt information: ", this.gattInformation), 15000);
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