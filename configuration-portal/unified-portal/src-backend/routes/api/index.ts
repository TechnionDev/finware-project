import express from "express";
import financialRouterFactory from "./financial-account";
import bluetoothRouterFactory from "./bluetooth";


export default function apiRouterFactory({ financeAccountsController, bluetoothController }) {
    const apiRouter = express.Router();
    apiRouter.use("/financial-account", financialRouterFactory(financeAccountsController));
    apiRouter.use("/bluetooth", bluetoothRouterFactory(bluetoothController));

    return apiRouter;
};
