#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <esp_task_wdt.h>

#include "epd_driver.h"
#include "esp_adc_cal.h"
#include "firasans.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logo.h"

#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>

#define BATT_PIN 36
#define SD_MISO 12
#define SD_MOSI 13
#define SD_SCLK 14
#define SD_CS 15

void displaymsg(const char *msg);
void blockUnillAccept() {
  int last = 1;
  int now = digitalRead(GPIO_NUM_39);
  while (now == 1) {
    if (now != last) {
      Serial.printf("Now: %d, Last:%d\n", now, last);
    }
    last = now;
    delay(1);
    now = digitalRead(GPIO_NUM_39);
  }
}

// The remote service we wish to connect to.
static BLEUUID serviceUUID("22222222-3333-4444-5555-666666666666");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("22222222-3333-4444-5555-666666666667");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static char *LOG_TAG = "INFO";


class MySecurity : public BLESecurityCallbacks {
  uint32_t onPassKeyRequest() { return 123456; }
  void onPassKeyNotify(uint32_t pass_key) {
    Serial.printf("onPassKeyNotify \n");
    ESP_LOGE(LOG_TAG, "The passkey Notify number:%d", pass_key);
  }
  bool onConfirmPIN(uint32_t pass_key) {
    char buf[100];
    Serial.printf("onConfirmPIN \n");
    Serial.printf("The passkey YES/NO number:%d", pass_key);

    sprintf(buf, "Passkey: %06d", pass_key);
    displaymsg(buf);
    blockUnillAccept();
    return true;
  }
  bool onSecurityRequest() {
    Serial.printf("onSecurityRequest \n");
    ESP_LOGI(LOG_TAG, "Security Request");
    return true;
  }
  void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl) {
    if (auth_cmpl.success) {
      ESP_LOGI(LOG_TAG, "remote BD_ADDR:");
      esp_log_buffer_hex(LOG_TAG, auth_cmpl.bd_addr, sizeof(auth_cmpl.bd_addr));
      ESP_LOGI(LOG_TAG, "address type = %d", auth_cmpl.addr_type);
      displaymsg("Connected");
    }
    ESP_LOGI(LOG_TAG, "pair status = %s",
             auth_cmpl.success ? "success" : "fail");
  }
};

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                           uint8_t *pData, size_t length, bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
}
static void __attribute__((unused)) remove_all_bonded_devices(void) {
  int dev_num = esp_ble_get_bond_device_num();

  esp_ble_bond_dev_t *dev_list =
      (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
  esp_ble_get_bond_device_list(&dev_num, dev_list);
  for (int i = 0; i < dev_num; i++) {
    esp_ble_remove_bond_device(dev_list[i].bd_addr);
  }

  free(dev_list);
}
bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
  BLEDevice::setSecurityCallbacks(new MySecurity());

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM);
  pSecurity->setCapability(ESP_IO_CAP_IO);
  pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  BLEClient *pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  // Connect to the remove BLE Server.
  pClient->connect(pAddress, BLE_ADDR_TYPE_PUBLIC);

  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE
  // server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic");

  // Read the value of the characteristic.
  std::string value = pRemoteCharacteristic->readValue();
  Serial.print("The characteristic value was: ");
  Serial.println(value.c_str());
  return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we
 * are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are
    // looking for.
    if (advertisedDevice.haveServiceUUID() &&
        advertisedDevice.getServiceUUID().equals(serviceUUID)) {
      //
      Serial.print("Found our device!  address: ");
      advertisedDevice.getScan()->stop();

      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;

    }  // Found our server
  }    // onResult
};     // MyAdvertisedDeviceCallbacks

void setup() {
  char buf[128];

  Serial.begin(115200);

  /*
   * SD Card test
   * Only as a test SdCard hardware, use example reference
   * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD/examples
   * * */
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);
  pinMode(GPIO_NUM_39, INPUT);
  epd_init();

  framebuffer =
      (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) {
    Serial.println("alloc memory failed !!!");
    while (1)
      ;
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

  Rect_t area = {
      .x = 230,
      .y = 20,
      .width = logo_width,
      .height = logo_height,
  };

  epd_poweron();
  epd_clear();
  epd_poweroff();

  int cursor_x = 200;
  int cursor_y = 250;

  const char *string1 = "➸ 16 color grayscale  😀 \n";
  const char *string2 = "➸ Use with 4.7\" EPDs 😍 \n";
  const char *string3 = "➸ High-quality font rendering ✎🙋";
  const char *string4 = "➸ ~630ms for full frame draw 🚀\n";
  const char *string5 = "I LOVE THE SCOOPOT";

  epd_poweron();

  writeln((GFXfont *)&FiraSans, string5, &cursor_x, &cursor_y, NULL);

  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  // Retrieve a Scanner and set the callback we want to use to be informed when
  // we have detected a new device.  Specify that we want active scanning and
  // start the scan to run for 30 seconds.
  displaymsg("Press button to start scanning");
  blockUnillAccept();
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}  // End of setup.

// This is the Arduino main loop function.
int last = 0;
void loop() {
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
      Serial.println(
          "We have failed to connect to the server; there is nothin more we "
          "will do.");
    }
    doConnect = false;
  }

  if (connected) {
    String newValue = "Time since boot: " + String(millis() / 1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    std::string totalValue = pRemoteCharacteristic->readValue();
    displaymsg(totalValue.c_str());
    delay(100000);
  }

  delay(1000);  // Delay a second between loops.
}

void displaymsg(const char *msg) {

}

uint8_t *framebuffer;
int vref = 1100;

void setup() {
  char buf[128];

  Serial.begin(115200);

  /*
   * SD Card test
   * Only as a test SdCard hardware, use example reference
   * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD/examples
   * * */
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);
  bool rlst = SD.begin(SD_CS);
  if (!rlst) {
    Serial.println("SD init failed");
    snprintf(buf, 128, "➸ No detected SdCard");
  } else {
    snprintf(buf, 128, "➸ Detected SdCard insert:%.2f GB",
             SD.cardSize() / 1024.0 / 1024.0 / 1024.0);
  }

  // Correct the ADC reference voltage
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
      ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
    vref = adc_chars.vref;
  }

  epd_init();

  framebuffer =
      (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) {
    Serial.println("alloc memory failed !!!");
    while (1)
      ;
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

  Rect_t area = {
      .x = 230,
      .y = 20,
      .width = logo_width,
      .height = logo_height,
  };

  epd_poweron();
  epd_clear();
  epd_draw_grayscale_image(area, (uint8_t *)logo_data);
  epd_poweroff();

  int cursor_x = 200;
  int cursor_y = 250;

  const char *string1 = "➸ 16 color grayscale  😀 \n";
  const char *string2 = "➸ Use with 4.7\" EPDs 😍 \n";
  const char *string3 = "➸ High-quality font rendering ✎🙋";
  const char *string4 = "➸ ~630ms for full frame draw 🚀\n";
  const char *string5 = "I LOVE THE SCOOPOT";

  epd_poweron();

  writeln((GFXfont *)&FiraSans, string5, &cursor_x, &cursor_y, NULL);
  delay(500);
  return;
  cursor_x = 200;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, string1, &cursor_x, &cursor_y, NULL);
  delay(500);
  cursor_x = 200;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, string2, &cursor_x, &cursor_y, NULL);
  delay(500);
  cursor_x = 200;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, string3, &cursor_x, &cursor_y, NULL);
  delay(500);
  cursor_x = 200;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, string4, &cursor_x, &cursor_y, NULL);
  delay(500);

  epd_poweroff();
}

void loop() {
  // When reading the battery voltage, POWER_EN must be turned on
  epd_poweron();
  delay(10);  // Make adc measurement more accurate
  uint16_t v = analogRead(BATT_PIN);
  float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
  String voltage = "➸ Voltage: " + String(battery_voltage) + "V";
  Serial.println(voltage);

  Rect_t area = {
      .x = 200,
      .y = 460,
      .width = 320,
      .height = 50,
  };

  int cursor_x = 200;
  int cursor_y = 500;
  epd_clear_area(area);
  writeln((GFXfont *)&FiraSans, (char *)voltage.c_str(), &cursor_x, &cursor_y,
          NULL);

  // There are two ways to close

  // It will turn off the power of the ink screen, but cannot turn off the blue
  // LED light. epd_poweroff();

  // It will turn off the power of the entire
  //  POWER_EN control and also turn off the blue LED light
  epd_poweroff_all();
  delay(1000000);
}
