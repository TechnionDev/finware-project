/*
                        Based on Neil Kolban example for IDF:
   https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
                        Ported to Arduino ESP32 by Evandro Copercini
*/
#include <U8g2_for_Adafruit_GFX.h>

#include "BluetoothManager.h"
#include "pageManager.h"
#include "utils.h"

static GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
static GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
static U8G2_FOR_ADAFRUIT_GFX u8g2;
static PageManager pageManager(u8g2, display);
static BluetoothManager blm(pageManager);

void setup() {
  Serial.begin(115200);
  pinMode(GPIO_NUM_39, INPUT);
  Serial.println("Starting Arduino BLE Client application...");
  delay(1000);
  // pageManager.showSumPage(100, 100, 100);

  GraphBuilder gb(display, u8g2);
  float data[] = {350, 500, 310, 400, 200, 300, 250, 260, 700};
  gb.DrawGraph(30, 24, 215, 77, 250, 700, "Spending Graph", data, 9, true, false);  
  display.update();

  return;

  /*
  *** This is a working hebrew printing
  char test[] = "סער הכי חכם";
  reverseheb(test);
  display.setRotation(1);
  Serial.printf("WIDTH: %d\n", display.width());
  Serial.printf("HEIGHT: %d\n", display.height());
  u8g2.begin(display);
  display.setTextColor(GxEPD_BLACK);
  u8g2.setFontMode(1);       // use u8g2 transparent mode (this is default)
  u8g2.setFontDirection(0);  // left to right (this is default)
  u8g2.setForegroundColor(GxEPD_BLACK);   // apply Adafruit GFX color
  u8g2.setBackgroundColor(GxEPD_WHITE);   // apply Adafruit GFX color
  u8g2.setFont(u8g2_font_cu12_t_hebrew);  // select u8g2 font from here:
  // u8g2.setCursor(display.width() - u8g2.getUTF8Width(test), 0);
  u8g2.setCursor(display.width() - 120, 50);
  Serial.println(u8g2.getUTF8Width(test));
  Serial.println(test);
  u8g2.println(test);
  */

  delay(2000); // For some reason the esp gets stuck without this..???
  Serial.println("0");
  BLEDevice::init("");
  Serial.println("1");
  BLEScan* pBLEScan = BLEDevice::getScan();
  Serial.println("2");
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  Serial.println("3");
  pBLEScan->setActiveScan(true);
  Serial.println("4");
  pBLEScan->start(30);
  Serial.println("5");
  Serial.println("Setup done");
}

// This is the Arduino main loop function.
void loop() {
  return;
  Serial.println("in loop");
  // If the flag "doConnect" is true then we have scanned for and found the
  // desired BLE Server with which we wish to connect.  Now we connect to it.
  // Once we are connected we set the connected flag to be true.
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

  // If we are connected to a peer BLE Server, update the characteristic each
  // time we are reached with the current time since boot.
  if (connected) {
    int totalSum = 0;
    for (const auto& it : blm.getBankInfo()) {
      totalSum += it.second;
    }
    // pageManager.showCardSpendingPage(blm.getBankInfo());
    pageManager.showSumPage(totalSum, blm.getDaysLeft(), blm.getGoal());
    delay(blm.getRefreshRate());  // Delay a second between loops.
  }
  if (!connected) {
    delay(100000);
  }
}  // End of loop
