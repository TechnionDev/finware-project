/*
                        Based on Neil Kolban example for IDF:
   https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
                        Ported to Arduino ESP32 by Evandro Copercini
*/
#include <ArduinoJson.h>
// #include <U8g2_for_Adafruit_GFX.h>
#include "epd_driver.h"
#include "firesans.h"

// #include "BluetoothManager.h"
#include "pageManager.h"
#include "utils.h"

// static GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
// static GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
// static U8G2_FOR_ADAFRUIT_GFX u8g2;
uint8_t* framebuffer;
PageManager pageManager;
// static BluetoothManager blm(pageManager);
/* 
void initFrameBuffer() {

  framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2,
                                             MALLOC_CAP_SPIRAM);
  if (!framebuffer) Serial.println("Memory alloc failed!");
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
} */

// uint8_t* framebuffer;
void setup() {
  // initFrameBuffer();
  Serial.begin(115200);
  framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) Serial.println("Memory alloc failed!");
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  pageManager.framebuffer = framebuffer;
  pinMode(GPIO_NUM_39, INPUT);
  Serial.println("Starting Finware application");
  // epd_init();
  // epd_poweron();
  // delay(1500);
  // initEpd();

  // fillRect(000, 000, 500, 500, 0x00);
  // epd_update();
  // epd_draw_grayscale_image(epd_full_screen(),
  //                          framebuffer);  // Update the screen

  int x = 100;
  int y = 100;
  delay(1000);

// drawString(100, 100 , "Test String", CENTER, framebuffer);
// epd_update(framebuffer);
  pageManager.showSumPage(3123, 10, 2800);
    // epd_clear();
/*    epd_clear();
  drawString(0, 0, "BOTTOM", LEFT, BOTTOM, &FiraSans, framebuffer);
  drawString(0,0, "CENTER", LEFT, CENTER, &FiraSans, framebuffer);
  drawString(0, 0, "TOP", LEFT, TOP, &FiraSans, framebuffer);
  epd_update(framebuffer);  */

  // drawString(EPD_WIDTH / 2, EPD_HEIGHT / 2, "THIS TESTING", CENTER, &FiraSans, framebuffer);
  // epd_update(framebuffer);
  // pageManager.showTitle(String("Test"), "");
  return;

  /*   Serial.println("Scanning Bluetooth for the raspberryPi");
    pageManager.showTitle("Scanning...", "", 1000);
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
    Serial.println("Setup done"); */
}

void loop() {
  return;
  /*   if (doConnect) {
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
    } */
}
