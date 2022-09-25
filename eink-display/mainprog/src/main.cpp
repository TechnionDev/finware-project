
#include <ArduinoJson.h>

#include "BluetoothManager.h"
#include "driver/rtc_io.h"
#include "epd_driver.h"
#include "pageManager.h"
#include "utils.h"

#define BUTTON_PIN_BITMASK 0x8004
#define uS_TO_M_FACTOR \
  (1000000 * 60) /* Conversion factor for micro seconds to seconds */

#define SCAN_TIMEOUT_SEC 60
#define DEFAULT_REFRESH_RATE_MIN 30

uint8_t* framebuffer;
PageManager pageManager;
static BluetoothManager blm(pageManager);

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
    case ESP_SLEEP_WAKEUP_EXT0:
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
  esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();

  Serial.print("GPIO that triggered the wake up: GPIO ");
  if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT0) {
    return HIDE_SHOW_BUTTON;
    Serial.println(HIDE_SHOW_BUTTON);
  } else if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT1) {
    return NEXT_BUTTON;
    Serial.println(NEXT_BUTTON);
  } else {
    Serial.println("Unknown");
    return -1;
  }
}
void initDisplay(){
  framebuffer =
      (uint8_t*)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) Serial.println("Memory alloc failed!");
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  pageManager.framebuffer = framebuffer;
  epd_init();
  epd_poweron();

}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Finware application");
  pinMode(NEXT_BUTTON, INPUT);
  pinMode(HIDE_SHOW_BUTTON, INPUT);
  initDisplay();

  esp_sleep_wakeup_cause_t wakeup_reason = print_wakeup_reason();
  esp_sleep_enable_ext0_wakeup(HIDE_SHOW_BUTTON, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_39, ESP_EXT1_WAKEUP_ALL_LOW);

  // ESP_SLEEP_WAKEUP_UNDEFINED is for reset button
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED ||
      wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println("Scanning bluetooth to find the RaspberryPi");
    pageManager.showTitle("Scanning...", "Searching for RaspberryPi", 1000);
    BLEDevice::init("Finware");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(SCAN_TIMEOUT_SEC);
    delay(1000);
    if (!doConnect) {
      Serial.println("RPi was not found, going back to sleep");
      pageManager.showTitle("RPi Wasn't Found", "Will retry later", 2500);
      esp_deep_sleep(refreshRate * uS_TO_M_FACTOR);
    }
    Serial.println("RPi was successfully found, Setup done");
  }
}

void showPage(int page) {
  cardsSpending bankInfo;
  switch (page % 3) {
    case 0:
      bankInfo = blm.getBankInfo(BankInfoBuffer);
      pageManager.showSumPage(totalSum, daysLeft, goal, bankInfo);
      break;
    case 1:
      auto doc = blm.getGraphData(jsonDocBuffer);
      doc["daysInCycle"],
          pageManager.showGraphPage(doc["cycleStartDate"], doc["cycleEndDate"],
                                    daysLeft, doc["daysInCycle"], doc["data"]);
      break;
  }
}

void refreshDataAndDisplay() {
  Serial.println("Refreshing data and printing it to display");
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
  show ? showPage(currPage) : epd_clear();
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
  int next_button = digitalRead(NEXT_BUTTON);
  int hide_button = digitalRead(HIDE_SHOW_BUTTON);
  int startTime = millis();

  delay(1000);

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
  if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT0 ||
      wakeup_cause == ESP_SLEEP_WAKEUP_EXT1) {
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