import express from "express";
import { BluetoothController } from "../../controllers";

export default function bluetoothRouterFactory(bluetoothController: BluetoothController) {
  const bluetoothRouter = express.Router();

  bluetoothRouter.get("/current-state", bluetoothController.fetchState);
  bluetoothRouter.post("/reset", bluetoothController.fetchState);
  bluetoothRouter.post("/accept", bluetoothController.acceptPairing);
  bluetoothRouter.post("/reject", bluetoothController.rejectPairing);


  return bluetoothRouter;
};

