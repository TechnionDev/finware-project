const express = require("express");
import services from './services.json';
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
  // res.json({ connection: "ADVERTISING" });
});