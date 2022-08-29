"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express = require("express");
const mongoose_1 = __importDefault(require("mongoose"));
const controllers_1 = require("./controllers");
const path_1 = __importDefault(require("path"));
mongoose_1.default.connect('mongodb://127.0.0.1:27017/finware');
let totalAmount = 0;
const SERVICES = [
    {
        "uuid": "22222222-3333-4444-5555-666666666666",
        "characteristics": [
            {
                "uuid": "22222222-3333-4444-5555-666666666667",
                "properties": [
                    "read"
                ],
                onRead: function (connection, callback) {
                    callback(totalAmount);
                }
            }
        ]
    }
];
/*
import BluetoothManager from './managers/BluetoothManager';

const btManager = new BluetoothManager();
let resolvePairingRequest;
let rejectPairingRequest;

btManager.init('myDevice', services, (pincode) => {
  console.log(`Received pincode in index.ts: ${pincode}`)
  return new Promise((resolve, reject) => {
    resolvePairingRequest = resolve;
    rejectPairingRequest = reject;
  });
}).then(() => {
  console.log("BT initialized");
  btManager.startAdvertising();
});
*/
const PORT = process.env.PORT || 3000;
const app = express();
app.use(express.json());
app.use(express.static(path_1.default.resolve(__dirname, '../build')));
app.listen(PORT, () => {
    console.log(`Server listening on ${PORT}`);
});
app.post("/api/financial-account/scrape", (req, res) => __awaiter(void 0, void 0, void 0, function* () {
    totalAmount = yield controllers_1.CreditCardCtl.scrape();
    console.log("Total amount: ", totalAmount);
    res.json({ amount: totalAmount });
}));
app.get("/api", (req, res) => {
    res.json({ message: "NEW MESSAGE 4" });
});
app.get("/api/bt-state", (req, res) => {
    // res.json({ connection: btManager.connectionStatus, passkey: btManager.passkey });
    res.json({ connection: "ADVERTISING", passcode: null });
});
app.post("/api/bt/reset", (req, res) => {
    // btManager.resetConnection();
    res.json({ status: "SUCCESS" });
});
app.post("/api/bt/accept", (req, res) => {
    // resolvePairingRequest();
    res.json({ status: "SUCCESS" });
});
app.post("/api/bt/reject", (req, res) => {
    // rejectPairingRequest();
    // btManager.resetConnection();
    res.json({ status: "SUCCESS" });
});
app.get("/api/financial-account", (req, res) => __awaiter(void 0, void 0, void 0, function* () {
    res.json(yield controllers_1.CreditCardCtl.fetchCreditCards());
}));
app.post("/api/financial-account/create", controllers_1.CreditCardCtl.createFinancialAccount);
// All other GET requests not handled before will return our React app
app.get('*', (req, res) => {
    res.sendFile(path_1.default.resolve(__dirname, '../build/', 'index.html'));
});
