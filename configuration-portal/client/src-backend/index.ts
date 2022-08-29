const express = require("express");
import mongoose from 'mongoose';
import bankTest from './managers/FinancialAccountsManager';

import { CreditCardCtl } from "./controllers"
import FinancialBE from "./models/FinancialBE";
import path from "path";

mongoose.connect('mongodb://127.0.0.1:27017/finware');

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
        onRead: function(connection, callback) {
          callback(totalAmount);
        }
      }
    ]
  }
]

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
app.use(express.static(path.resolve(__dirname, '../build')));

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});

app.post("/api/financial-account/scrape", async (req, res) => {
  totalAmount = await CreditCardCtl.scrape();
  console.log("Total amount: ", totalAmount);
  res.json({ amount: totalAmount });
});

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

app.get("/api/financial-account", async (req, res) => {
  res.json(await CreditCardCtl.fetchCreditCards());
});


app.post("/api/financial-account/create", CreditCardCtl.createFinancialAccount);

// All other GET requests not handled before will return our React app
app.get('*', (req, res) => {
  res.sendFile(path.resolve(__dirname, '../build/', 'index.html'));
});