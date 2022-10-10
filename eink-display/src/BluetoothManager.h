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

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    boolean *doConnectPtr;
    char *bleServerAddrStr;

   public:
    MyAdvertisedDeviceCallbacks(boolean *doConnectPtr, char *bleServerAddrStr);
    /**
     * Called for each advertising BLE server.
     */
    void onResult(BLEAdvertisedDevice advertisedDevice);
};

class BluetoothManager {
    PageManager &pageManager;
    BLERemoteService *pRemoteService{};
    std::string requestService(const std::string &serviceName);
    std::map<std::string, std::string> ServiceName2ServiceUuid{
        {"BankInfo", "5b278f16-4460-47e1-919e-2d50d7d0a55d"},
        {"RefreshRate", "49dc2b22-2dc4-4a66-afee-d7782b9b81cd"},
        {"BaseSpending", "f0e6f8b6-0c2d-4b2d-8a4f-1c4e4b4f4d0b"},
        {"Goal", "8f71bd04-89f7-4290-b90f-ac1265f5f127"},
        {"DaysLeft", "c27c1205-9ccb-4d1f-999f-0b9cfabf1d09"},
        {"GraphData", "b8ed4639-8e38-4d0f-9ad6-5e46544f171a"}};

   public:
    BluetoothManager(PageManager &pageManager);
    void updateBankInfoBuffer(char BankInfoBuffer[]);
    cardsSpending getBankInfo(const char BankInfoBuffer[]);
    int getRefreshRate();
    int getBaseSpending();
    int getGoal();
    int getDaysLeft();
    void updateJsonDocBuffer(char jsonDocBuffer[]);
    DynamicJsonDocument getGraphData(const char jsonDocBuffer[]);
    bool connectToServer(BLEAddress &pAddress);
};

boolean waitForAuth();
boolean isAuthedFailed();
#endif  // DISPLAY_SRC_BLUETOOTHMANAGER_H_
