//
// Created by Dani Bondar on 29/08/2022.
//

#ifndef DISPLAY_SRC_BLUETOOTHMANAGER_H_
#define DISPLAY_SRC_BLUETOOTHMANAGER_H_
#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>

#include "pageManager.h"
// The remote service we wish to connect to.

static BLEUUID serviceUUID("2c82b713-f76a-4696-98eb-d92f9f233f40");
// The characteristic of the remote service we are interested in.

static BLEAddress *pServerAddress;
// connected, doConnect and authed should be consistent between deepSleeps, therefor need to be saved in RTC memory
static boolean doConnect = false;
static boolean connected = false;
static char LOG_TAG[] = "INFO";

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() &&
        advertisedDevice.getServiceUUID().equals(serviceUUID)) {
      Serial.println(" - Found the RaspberryPi!");
      advertisedDevice.getScan()->stop();

      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
    }
  }
};


class BluetoothManager {
  PageManager pageManager;
  BLERemoteService *pRemoteService{};
  std::string requestService(const std::string &serviceName);
  std::map<std::string, std::string> ServiceName2ServiceUuid{
      {"BankInfo", "5b278f16-4460-47e1-919e-2d50d7d0a55d"},
      {"RefreshRate", "49dc2b22-2dc4-4a66-afee-d7782b9b81cd"},
      {"Goal", "8f71bd04-89f7-4290-b90f-ac1265f5f127"},
      {"DaysLeft", "c27c1205-9ccb-4d1f-999f-0b9cfabf1d09"},
      {"GraphData", "b8ed4639-8e38-4d0f-9ad6-5e46544f171a"}};

 public:
  BluetoothManager(PageManager &pageManager);
  void updateBankInfoBuffer(char BankInfoBuffer[]);
  cardsSpending getBankInfo(const char BankInfoBuffer[]);
  int getRefreshRate();
  int getGoal();
  int getDaysLeft();
  void updateJsonDocBuffer(char jsonDocBuffer[]);
  DynamicJsonDocument getGraphData(const char jsonDocBuffer[]);
  bool connectToServer(BLEAddress pAddress);
};

void waitForAuth();
#endif  // DISPLAY_SRC_BLUETOOTHMANAGER_H_
