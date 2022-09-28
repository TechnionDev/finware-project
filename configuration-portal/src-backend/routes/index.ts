import express from "express";
import { checkSessionKey } from "../util-managers/SettingsManager";
import apiRouterFactory from "./api";
import { StatusCodes } from 'http-status-codes';
import path from "path";


export default function routerFactory({ financeAccountsController, bluetoothController }) {
    const router = express.Router();
    router.get('/', async (req, res, next) => {
        if (await checkSessionKey(req.cookies['sessionKey'])) {
            next();
        } else {
            // Redirect to /login
            res.status(StatusCodes.FORBIDDEN).redirect('/login');
        }
    });
    router.use("/api", apiRouterFactory({ financeAccountsController, bluetoothController }));

    return router;
};