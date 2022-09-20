//
// Created by Dani Bondar on 29/08/2022.
//

#include "BluetoothManager.h"

static boolean authed = false;
static boolean authedFailed = false;
class MySecurity : public BLESecurityCallbacks {
 private:
  PageManager& pageManager;

 public:
  MySecurity(PageManager& pageManager) : pageManager(pageManager) {}

  uint32_t onPassKeyRequest() { return 0; }

  void onPassKeyNotify(
      uint32_t pass_key) {  // TODO: prepend passkey with zeroes...
    Serial.printf("onPassKeyNotify called, passkey: %d\n", pass_key);
    pageManager.showPassKey(pass_key);
  }
  bool onConfirmPIN(uint32_t pass_key) { return false; }

  bool onSecurityRequest() {
    Serial.printf("onSecurityRequest \n");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl) {
    if (auth_cmpl.success) {
      authed = true;
      Serial.println("Bluetooth pairing finished successfully!");
    } else {
      authedFailed = true;
      Serial.printf(
          "Bluetooth pairing encountered an error: %d\n",
          auth_cmpl
              .fail_reason);  // I think 102 is connection was closed by slave
      pageManager.showTitle(
          "Pairing Error",
          String("Error: " + String(auth_cmpl.fail_reason)) + ". Retry later", 2500);
    }
  }
};

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                           uint8_t *pData, size_t length, bool isNotify) {}

bool BluetoothManager::connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
  BLEDevice::setSecurityCallbacks(new MySecurity(pageManager));

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);

  pSecurity->setCapability(ESP_IO_CAP_OUT);
  pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  BLEClient *pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  bool res = pClient->connect(pAddress, BLE_ADDR_TYPE_PUBLIC);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  this->pRemoteService = pClient->getService(serviceUUID);
  if (this->pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }
  Serial.println("Successfully got our service by UUID");

  // TODO: i dont think finding all the characteristics is important, think
  // about this more
  /*
  for (const auto &it : this->ServiceName2ServiceUuid) {
    BLERemoteCharacteristic *pRemoteCharacteristic =
        this->pRemoteService->getCharacteristic(BLEUUID(it.second));
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(it.second.c_str());
      return false;
    }
    Serial.println((" - Found this characteristic: " + it.first).c_str());
  }
  Serial.println("Found all characteristics!");
  */
  return true;
}

/**
 * Scan for BLE servers and find the first one that advertises the service we
 * are looking for.
 */
BluetoothManager::BluetoothManager(PageManager &pageManager)
    : pageManager(pageManager) {}

void BluetoothManager::updateBankInfoBuffer(char BankInfoBuffer[]) {
  strcpy(BankInfoBuffer, requestService("BankInfo").c_str());
}

cardsSpending BluetoothManager::getBankInfo(const char BankInfoBuffer[]) {
  // deserializeJson will alter the char array that is given, thus we need to
  // copy the BankInfoBuffer to a tmp buffer
  //  char tmp [1024];
  //  strcpy(tmp,BankInfoBuffer);
  DynamicJsonDocument doc(200);
  DeserializationError error = deserializeJson(doc, BankInfoBuffer);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return {};
  }
  cardsSpending bankInfo;
  JsonObject root = doc.as<JsonObject>();
  for (JsonPair kv : root) {
    bankInfo[kv.key().c_str()] = -kv.value().as<int>();
  }
  return bankInfo;
}

int BluetoothManager::getRefreshRate() {
  DynamicJsonDocument doc(200);
  auto output = requestService("RefreshRate");
  DeserializationError error = deserializeJson(doc, output);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return {};
  }
  int refreshRate = doc["value"];
  return refreshRate;
}
int BluetoothManager::getGoal() {
  DynamicJsonDocument doc(200);
  auto output = requestService("Goal");
  DeserializationError error = deserializeJson(doc, output);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return {};
  }
  int Goal = doc["value"];

  return Goal;
}

int BluetoothManager::getDaysLeft() {
  DynamicJsonDocument doc(200);
  auto output = requestService("DaysLeft");
  DeserializationError error = deserializeJson(doc, output);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return {};
  }
  int DaysLeft = doc["value"];
  return DaysLeft;
}

void BluetoothManager::updateJsonDocBuffer(char jsonDocBuffer[]) {
  strcpy(jsonDocBuffer, requestService("GraphData").c_str());
}

DynamicJsonDocument BluetoothManager::getGraphData(const char jsonDocBuffer[]) {
  // deserializeJson will alter the char array that is given, thus we need to
  // copy the BankInfoBuffer to a tmp buffer
  //  char tmp [1024];
  //  strcpy(tmp,jsonDocBuffer);
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, jsonDocBuffer);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return doc;
  }
  return doc;
}

std::string BluetoothManager::requestService(const std::string &serviceName) {
  Serial.println(("Requesting characteristic " + serviceName + "....").c_str());
  auto serviceUuid = ServiceName2ServiceUuid[serviceName];
  BLERemoteCharacteristic *pRemoteCharacteristic =
      this->pRemoteService->getCharacteristic(BLEUUID(serviceUuid));
  auto res = pRemoteCharacteristic->readValue();

  Serial.print((" - Got characteristic value" + serviceName + ": ").c_str());
  Serial.println(res.c_str());
  return res;
}

boolean waitForAuth() {
  while (!authed && !authedFailed) {
    delay(500);
  }

  if (authedFailed) {
    return false;
  }
  return true;
}

boolean isAuthedFailed() { return authedFailed; }