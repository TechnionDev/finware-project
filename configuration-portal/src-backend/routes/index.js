import express from "express";
import apiRouterFactory from "./api";


export default function routerFactory({ financeAccountsController, bluetoothController }) {
    const router = express.Router();
    router.use("/api", apiRouterFactory({ financeAccountsController, bluetoothController }));

    return router;
};