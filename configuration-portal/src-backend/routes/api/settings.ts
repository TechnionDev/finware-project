import express from "express";
import { getSettings, updateSettings } from "../../util-managers/SettingsManager";

export default function settingsRouterFactory(financeAccountsController) {
  const settingsRouter = express.Router();

  settingsRouter.get("/get", getSettings);
  settingsRouter.put("/update", (req, res) => updateSettings(req, res, financeAccountsController));

  return settingsRouter;
};
