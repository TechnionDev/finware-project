import express from "express";
import cookieParser from "cookie-parser";
import path from "path";
import mongoose from "mongoose";
import routerFactory from "./routes";
// Import https and fs
import https from "https";
import fs from "fs";
import logger from "morgan";

import { FinancialAccountsController } from "./controllers"
import { BluetoothController } from "./controllers"
import { CronJob } from 'cron';
import Settings from "./models/Settings";

function requireHTTPS(req, res, next) {
    // The 'x-forwarded-proto' check is for Heroku
    if (!req.secure && req.get('x-forwarded-proto') !== 'https' && !process.env.NODE_ENV?.toLowerCase().includes("dev")) {
        return res.redirect('https://' + req.get('host') + req.url);
    }
    next();
}

mongoose.connect("mongodb://127.0.0.1:27017/finware").then(async () => {
    console.log('MongoDB database connected successfully');
    // Get current settings document (create one if missing)
    let settings = await Settings.findOneAndUpdate({}, {}, { upsert: true, new: true });


    const bluetoothController = new BluetoothController(); // TODO: populate with actual data
    const financeAccountsController = new FinancialAccountsController(bluetoothController);

    new CronJob(
        `0 0 4 * * *`, // Once every 24 hours at 04:00:00
        financeAccountsController.updateFinancialData.bind(financeAccountsController),
        null,
        true, /* start */
        null,
        null,
        true /* runOnInit */
    );

    const PORT = process.env.PORT || 80;
    const HTTPS_PORT = process.env.HTTPS_PORT || 443;

    const app = express();
    app.use(logger(':remote-addr - :remote-user [:date[clf]] ":method :url HTTP/:http-version" :status - :response-time ms'));
    app.use(express.json());
    app.use(cookieParser());
    app.use(requireHTTPS);
    app.use("/", routerFactory({ financeAccountsController, bluetoothController }));
    app.use(express.static(path.resolve(__dirname, "../build")));

    if (!process.env.NODE_ENV?.includes("development")) {
        const httpsOptions = {
            key: fs.readFileSync(path.resolve(__dirname, '../security/cert.key')),
            cert: fs.readFileSync(path.resolve(__dirname, '../security/cert.pem'))
        }
        const server = https.createServer(httpsOptions, app)
            .listen(HTTPS_PORT, () => {
                console.log('HTTPS Server listening on ' + HTTPS_PORT);
                console.log('Env is:', process.env.NODE_ENV);
            })
            ;
    }

    app.listen(PORT, () => {
        console.log(`Server listening on ${PORT}`);
    });

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
    console.log("Failed to start up the server, error:", err);
    process.exit(1);
});
