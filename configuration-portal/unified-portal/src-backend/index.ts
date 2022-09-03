import express from "express";
import path from "path";
import mongoose from "mongoose";
import routerFactory from "./routes";

import { FinancialAccountsController } from "./controllers"
import { BluetoothController } from "./controllers"
import { CronJob } from 'cron';
import Settings from "./models/Settings";


mongoose.connect("mongodb://127.0.0.1:27017/finware").then(async () => {
    console.log('MongoDB database connected successfully');
    let settings = await Settings.findOne({});


    const bluetoothController = new BluetoothController({ refreshRate: settings.display_refresh_frequency_minutes, goal: settings.expense_budget, cycleStartDate: settings.month_cycle_start_day }); // TODO: populate with actual data
    const financeAccountsController = new FinancialAccountsController(bluetoothController);

    new CronJob(
        '0 * * * *',
        financeAccountsController.updateFinancialData.bind(financeAccountsController),
        null,
        true, /* start */
        null,
        null,
        true /* runOnInit */
    );

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
}).catch(err => {
    console.log('Failed to connect to MongoDB:\n', err);
});
