import express from "express";
import path from "path";
import mongoose from "mongoose";
import routerFactory from "./routes";

import { FinancialAccountsController } from "./controllers"
import { BluetoothController } from "./controllers"
import { CronJob } from 'cron';


const bluetoothController = new BluetoothController({ bankInfo: 500, refreshRate: 1000, goal: 5150, daysLeft: 15 }); // TODO: populate with actual data
const financeAccountsController = new FinancialAccountsController(bluetoothController);

new CronJob(
  '0 */15 * * * *',
  financeAccountsController.scrape.bind(financeAccountsController),
  null,
  true
);

mongoose.connect("mongodb://127.0.0.1:27017/finware").then(() => {
  console.log('MongoDB database connected successfully');
}).catch(err => {
  console.log('Failed to connect to MongoDB:\n', err);
});

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

app.use((err: Error, req, res, next) => {
  if (res.headersSent) {
    return next(err)
  }
  res.status(500)
  res.json({ error: err.stack });
});