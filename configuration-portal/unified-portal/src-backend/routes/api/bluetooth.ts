import express from "express";
import { BluetoothController } from "../../controllers";

export default function bluetoothRouterFactory(bluetoothController: BluetoothController) {
  const bluetoothRouter = express.Router();

  bluetoothRouter.get("/current-state", bluetoothController.fetchState.bind(bluetoothController));
  bluetoothRouter.post("/reset", bluetoothController.fetchState.bind(bluetoothController));
  bluetoothRouter.post("/accept", bluetoothController.acceptPairing.bind(bluetoothController));
  bluetoothRouter.post("/reject", bluetoothController.rejectPairing.bind(bluetoothController));


  return bluetoothRouter;
};

