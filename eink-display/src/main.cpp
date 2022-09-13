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

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR bool show = true;
RTC_DATA_ATTR int currPage = 0;


RTC_DATA_ATTR char BankInfoBuffer[1024];
RTC_DATA_ATTR char jsonDocBuffer[1024];
RTC_DATA_ATTR int daysLeft = 0;
RTC_DATA_ATTR int goal = 0;
RTC_DATA_ATTR int refrashRate = 0;
RTC_DATA_ATTR int totalSum = 0;


#define BUTTON_PIN_BITMASK 0x8004
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */
#define HIDE_SHOW_BUTTON GPIO_NUM_15
#define NEXT_BUTTON GPIO_NUM_2

esp_sleep_wakeup_cause_t print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    default : 
      Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason);
      wakeup_reason = ESP_SLEEP_WAKEUP_UNDEFINED;
      break;
  }
  return wakeup_reason;
}

int getGPIOPIN(){
  uint64_t GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  int pgioPin = (log(GPIO_reason))/log(2);
  Serial.println(pgioPin, 0);
  return pgioPin;
}

void setup() {
  Serial.begin(115200);
  pinMode(GPIO_NUM_2, INPUT);
  Serial.println("Starting Finware application");
   ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_wakeup_cause_t wakeup_reason = print_wakeup_reason();
  getGPIOPIN();

  
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  
  // ESP_SLEEP_WAKEUP_UNDEFINED is for reset button
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED ){
    Serial.println("Scanning Bluetooth for the raspberryPi");
    pageManager.showTitle("Scanning...", "", 1000);
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
    Serial.println("Setup done");
  }
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
    
    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT1){
      //the cause of the weakup was refresh or restart, therefor need to fetch the date from the BLE
      blm.updateBankInfoBuffer(BankInfoBuffer);
      auto bankInfo = blm.getBankInfo(BankInfoBuffer);
      blm.updateJsonDocBuffer(jsonDocBuffer);
      daysLeft = blm.getDaysLeft();
      goal = blm.getGoal();
      refrashRate = blm.getRefreshRate();
      esp_sleep_enable_timer_wakeup(refrashRate * uS_TO_S_FACTOR);
      for (const auto& it : bankInfo) {
        totalSum += it.second;
      }
      pageManager.showSumPage(totalSum, daysLeft, goal);
    }else{
      if (getGPIOPIN()==HIDE_SHOW_BUTTON){
        show =! show; 
        show? pageManager.showSumPage(totalSum, daysLeft, goal): display.eraseDisplay();
      }
      else{
        auto startTime = millis();
        while (millis() - startTime < NEXT_BUTTON_TIME_INTERVAL) {
          //TODO:: need to finish this while loop
          currPage ++;
          cardsSpending bankInfo;
          switch (currPage % 3)
          {
            case 0:
              pageManager.showSumPage(totalSum, daysLeft, goal);
              break;
            case 1:
              bankInfo = blm.getBankInfo(BankInfoBuffer);
              pageManager.showCardSpendingPage(bankInfo);
              break;
            case 2:
              auto doc = blm.getGraphData(jsonDocBuffer);
              pageManager.showGraphPage(doc["cycleStartDate"], doc["cycleEndDate"],
                                              doc["daysInCycle"], doc["data"]);
              break;
          }
          blockUntilPress(startTime);
          }
      }
    }
    Serial.println("Going to sleep now");
    delay(1000);
    esp_deep_sleep_start();
  }
}
