import express from "express";
import path from "path";
import mongoose from "mongoose";
import routerFactory from "./routes";
// Import https and fs
import https from "https";
import fs from "fs";

import { FinancialAccountsController } from "./controllers"
import { BluetoothController } from "./controllers"
import { CronJob } from 'cron';
import Settings from "./models/Settings";


mongoose.connect("mongodb://127.0.0.1:27017/finware").then(async () => {
    console.log('MongoDB database connected successfully');
    // Get current settings document (create one if missing)
    let settings = await Settings.findOneAndUpdate({}, {}, { upsert: true, new: true });


    const bluetoothController = new BluetoothController({ refreshRate: settings.display_refresh_frequency_minutes, goal: settings.expense_budget, cycleStartDay: settings.month_cycle_start_day }); // TODO: populate with actual data
    const financeAccountsController = new FinancialAccountsController(bluetoothController);

    new CronJob(
        '0 */5 * * * *',
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

    if (process.env.NODE_ENV?.includes("development")) {
        app.listen(PORT, () => {
            console.log(`Server listening on ${PORT}`);
        });
    } else {
        const httpsOptions = {
            key: fs.readFileSync(path.resolve(__dirname, '../security/cert.key')),
            cert: fs.readFileSync(path.resolve(__dirname, '../security/cert.pem'))
        }
        const server = https.createServer(httpsOptions, app)
            .listen(PORT, () => {
                console.log('HTTPS Server listening on ' + PORT);
                console.log('Env is:', process.env.NODE_ENV);
            })
            ;
    }


    // All other GET requests not handled before will return our React app
    app.get("*", (_req, res) => {
        res.sendFile(path.resolve(__dirname, "../build/", "index.html"));
    });

    app.use((err: Error, _req, res, next) => {
        if (res.headersSent) {
            return next(err)
        }
        res.status(500)
        res.json({ error: err.stack });
    });
}).catch(err => {
    console.log('Failed to connect to MongoDB:\n', err);
});