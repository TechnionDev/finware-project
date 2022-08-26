#include <Arduino.h>
/*
      Based on Neil Kolban example for IDF:
   https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
      Ported to Arduino ESP32 by Evandro Copercini
*/
#define LILYGO_T5_V213

#include <GxDEPG0213BN/GxDEPG0213BN.h>  // 2.13" b/w  form DKE GROUP
#include <GxEPD.h>
#include <boards.h>

#include GxEPD_BitmapExamples
// FreeFonts from Adafruit_GFX
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#include <iostream>
#include <bits/stdc++.h>

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

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

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
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  pinMode(GPIO_NUM_39, INPUT);
  SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
  display.init();  // enable diagnostic output on Serial
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
    delay(100000);
  }

  delay(1000);  // Delay a second between loops.
}

void displaymsg(const char *msg) {
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold12pt7b);
  display.setCursor(0, 45);
  display.println(msg);
  display.update();
}
