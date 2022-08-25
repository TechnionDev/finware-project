const express = require("express");
import services from './services.json';
import bankTest from './CreditCardsManager';
// /*
import BluetoothManager from './BluetoothManager';

const btManager = new BluetoothManager();
btManager.init('myDevice', services, (pincode) => {
  console.log(`Received pincode in index.ts: ${pincode}`)
  return Promise.resolve(0);
}).then(() => {

  console.log("BT initialized");
  btManager.startAdvertising();
});
// */

const PORT = process.env.PORT || 3001;

const app = express();

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});

app.get("/api", (req, res) => {
  res.json({ message: "NEW MESSAGE 4" });
});

app.get("/api/bt-state", (req, res) => {
  res.json({ connection: btManager.connectionStatus, passcode: btManager.passcode});
  // res.json({ connection: "ADVERTISING", passcode: null });
});

app.post("/api/bt/reset", (req, res) => {
  btManager.resetConnection();
  res.json({ status: "SUCCESS" });
});

app.get("/api/credit-cards", (req, res) => {
  bankTest((scrapeResult, total) => {
    res.json({ ...scrapeResult, "total": total });
  });
});