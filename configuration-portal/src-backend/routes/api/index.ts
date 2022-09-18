import express from "express";
import financialRouterFactory from "./financial-account";
import bluetoothRouterFactory from "./bluetooth";
import settingsRouterFactory from "./settings";


export default function apiRouterFactory({ financeAccountsController, bluetoothController }) {
    const apiRouter = express.Router();
    apiRouter.use("/financial-account", financialRouterFactory(financeAccountsController));
    apiRouter.use("/bluetooth", bluetoothRouterFactory(bluetoothController));
    apiRouter.use("/settings", settingsRouterFactory());
    // TODO: Make 404 page instead of always returning the main app page

    return apiRouter;
};
