import express from "express";
import { getSettings, updateSettings } from "../../util-managers/SettingsManager";

export default function settingsRouterFactory() {
  const settingsRouter = express.Router();

  // TODO: Add authentication for this router
  settingsRouter.get("/get", getSettings);
  settingsRouter.put("/update", updateSettings);

  return settingsRouter;
};
