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

#define BUTTON_PIN_BITMASK 0x8004
#define uS_TO_M_FACTOR \
  (1000000 * 60) /* Conversion factor for micro seconds to seconds */
#define HIDE_SHOW_BUTTON GPIO_NUM_15
#define NEXT_BUTTON GPIO_NUM_2
#define SCAN_TIMEOUT_SEC 60
#define DEFAULT_REFRESH_RATE_MIN 30

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
RTC_DATA_ATTR uint64_t refreshRate = DEFAULT_REFRESH_RATE_MIN;
RTC_DATA_ATTR int totalSum = 0;

esp_sleep_wakeup_cause_t print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("Wakeup caused by external signal using RTC_CNTL");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by timer");
      break;
    default:
      Serial.printf("Wakeup was not caused by deep sleep, wakeup_cause: %d\n",
                    wakeup_reason);
      wakeup_reason = ESP_SLEEP_WAKEUP_UNDEFINED;
      break;
  }
  return wakeup_reason;
}

int getGPIOPIN() {
  uint64_t GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  int pgioPin = (log(GPIO_reason)) / log(2);
  Serial.println(pgioPin, 0);
  return pgioPin;
}

void setup() {
  Serial.begin(115200);
  pinMode(NEXT_BUTTON, INPUT);
  pinMode(HIDE_SHOW_BUTTON, INPUT);
  Serial.println("Starting Finware application");
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_wakeup_cause_t wakeup_reason = print_wakeup_reason();
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  // ESP_SLEEP_WAKEUP_UNDEFINED is for reset button
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED ||
      wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println("Scanning bluetooth to find the RaspberryPi");
    pageManager.showTitle("Scanning...", "Searching for RaspberryPi", 1000);
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(SCAN_TIMEOUT_SEC);  // Scan forever
    if (!doConnect) {
      Serial.println("RPi was not found, going back to sleep");
      pageManager.showTitle("RPi Wasn't Found", "Will retry later");
      esp_deep_sleep(refreshRate * uS_TO_M_FACTOR);
    }
    Serial.println("RPi was found, Setup done");
  }
}

void showPage(int page) {
  cardsSpending bankInfo;
  switch (page % 3) {
    case 0:
      pageManager.showSumPage(totalSum, daysLeft, goal);
      break;
    case 1:
      bankInfo = blm.getBankInfo(BankInfoBuffer);
      pageManager.showCardSpendingPage(bankInfo);
      break;
    case 2:
      auto doc = blm.getGraphData(jsonDocBuffer);
      doc["daysInCycle"],
          pageManager.showGraphPage(doc["cycleStartDate"], doc["cycleEndDate"],
                                    doc["daysInCycle"], doc["data"]);
      break;
  }
}


void refreshDataAndDisplay() {
  if (!connected) {
    Serial.println(
        "Attempted to refresh display data when there is no bluetooth "
        "connection, ignoring");
    return;
  }
  blm.updateBankInfoBuffer(BankInfoBuffer);
  auto bankInfo = blm.getBankInfo(BankInfoBuffer);
  blm.updateJsonDocBuffer(jsonDocBuffer);
  daysLeft = blm.getDaysLeft();
  goal = blm.getGoal();
  refreshRate = blm.getRefreshRate();
  totalSum = 0;
  for (const auto& it : bankInfo) {
    totalSum += it.second;
  }
  showPage(currPage);
}

boolean connectBT() {
  doConnect = false;
  if (blm.connectToServer(*pServerAddress)) {
    Serial.println("We are now connected to the BLE Server.");
    connected = waitForAuth();
    return connected;
  }

  Serial.println(
      "We have failed to connect to the server; there is nothin more we "
      "will do.");

  return false;
}

void handleNextButtonClick() {
  currPage++;
  showPage(currPage);
}

void handleShowHideButtonClick() {
  show = !show;
  show ? showPage(currPage) : display.eraseDisplay();
}

void handleClick(int gpiopin) {
  if (gpiopin == HIDE_SHOW_BUTTON) {
    handleShowHideButtonClick();
  } else if (gpiopin == NEXT_BUTTON) {
    handleNextButtonClick();
  } else {
    Serial.printf(
        "Cannot handle button click, gpio pin number: %d is not implemented",
        gpiopin);
  }
}

void handleWakeupClick() { handleClick(getGPIOPIN()); }

void loop() {
  Serial.printf("Running loop. doConnect: %s | connected: %s\n",
                doConnect ? "true" : "false", connected ? "true" : "false");
  if (doConnect) {
    boolean success = connectBT();
    if (!success) {
      Serial.println("Could not connect to RaspberryPi, going to sleep");

      if (!isAuthedFailed()) { 
        //  We have not asynchronisely shown error title to screen
        pageManager.showTitle("Pairing Error", "Will retry later");
      }
      esp_deep_sleep(refreshRate * uS_TO_M_FACTOR);

      return;
    }
  }

  esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();
  if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT1) {
    handleWakeupClick();
    auto lastPressTime = millis();
    while (millis() - lastPressTime < LISTEN_FOR_CLICKS_TIMEOUT) {
      int gpiopin = listenForButtonClick(LISTEN_FOR_CLICKS_TIMEOUT);
      if (gpiopin == BUTTON_CLICK_TIMEOUT) {
        break;
      }

      handleClick(gpiopin);
      lastPressTime = millis();
    }
  } else if (wakeup_cause == ESP_SLEEP_WAKEUP_TIMER ||
             wakeup_cause == ESP_SLEEP_WAKEUP_UNDEFINED) {
    refreshDataAndDisplay();
  }

  Serial.println("Finished loop, going to sleep");
  esp_deep_sleep(refreshRate * uS_TO_M_FACTOR);
}
