import express from "express";
import path from "path";
import mongoose from "mongoose";
import routerFactory from "./routes";

import FinancialBE from "./models/FinancialBE";
import { FinanceAccountsController } from "./controllers"
import { BluetoothController } from "./controllers"



const bluetoothController = new BluetoothController();
const financeAccountsController = new FinanceAccountsController(bluetoothController);

mongoose.connect("mongodb://127.0.0.1:27017/finware");



const PORT = process.env.PORT || 3001;

const app = express();
app.use(express.json());
app.use(express.static(path.resolve(__dirname, "../build")));
app.use("/", routerFactory({ financeAccountsController, bluetoothController }));

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});




// All other GET requests not handled before will return our React app
app.get("*", (req, res) => {
  res.sendFile(path.resolve(__dirname, "../build/", "index.html"));
});