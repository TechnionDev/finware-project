/*
                        Based on Neil Kolban example for IDF:
   https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
                        Ported to Arduino ESP32 by Evandro Copercini
*/
#include <ArduinoJson.h>
#include <U8g2_for_Adafruit_GFX.h>

#include "BluetoothManager.h"
#include "pageManager.h"
#include "utils.h"

static GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
static GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
static U8G2_FOR_ADAFRUIT_GFX u8g2;
static GraphBuilder gb(display, u8g2);
static PageManager pageManager(u8g2, display, gb);
static BluetoothManager blm(pageManager);

void setup() {
  Serial.begin(115200);
  pinMode(GPIO_NUM_39, INPUT);
  Serial.println("Starting Finware application");

  Serial.println("Scanning Bluetooth for the raspberryPi");
  pageManager.showTitle("Scanning...", "", 1000);
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
  Serial.println("Setup done");
}

void loop() {
  if (doConnect) {
    if (blm.connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
      Serial.println(
          "We have failed to connect to the server; there is nothin more we "
          "will do.");
    }
    doConnect = false;
  }
  waitForAuth();
  if (connected) {
    auto bankInfo = blm.getBankInfo();
    DynamicJsonDocument doc = blm.getGraphData();
    int daysLeft = blm.getDaysLeft();
    int goal = blm.getGoal();

    int totalSum = 0;
    for (const auto& it : bankInfo) {
      totalSum += it.second;
    }

    while (true) {
      pageManager.showSumPage(totalSum, daysLeft, goal);
      blockUntilPress();
      pageManager.showCardSpendingPage(bankInfo);
      blockUntilPress();
      pageManager.showGraphPage(doc["cycleStartDate"], doc["cycleEndDate"],
                                doc["daysInCycle"], doc["data"]);
      blockUntilPress();
    }

    delay(blm.getRefreshRate() * 1000);
  }
}
