//
// Created by Dani Bondar on 29/08/2022.
//

#include "BluetoothManager.h"

// The characteristic of the remote service we are interested in.
static BLEUUID serviceUUID("2c82b713-f76a-4696-98eb-d92f9f233f40");
static boolean authed = false;
static boolean authedFailed = false;
class MySecurity : public BLESecurityCallbacks {
   private:
    PageManager pageManager;

   public:
    MySecurity(PageManager pageManager) : pageManager(pageManager) {}

    uint32_t onPassKeyRequest() {
        logm("onPassKeyRequest (shouldn't reach here in the Finware project)");
        return 0;
    }

    void onPassKeyNotify(uint32_t pass_key) {
        logm("onPassKeyNotify");
        pageManager.showPassKey(pass_key);
    }
    bool onConfirmPIN(uint32_t pass_key) {
        logm("onConfirmPIN (shouldn't reach here in the Finware project)");
        return false;
    }

    bool onSecurityRequest() {
        logm("onSecurityRequest");
        return true;
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl) {
        if (auth_cmpl.success) {
            authed = true;
            authedFailed = false;
            logm("Bluetooth pairing finished successfully!");
        } else {
            authedFailed = true;
            authed = false;
            logf("Bluetooth pairing encountered an error: %d\n", auth_cmpl.fail_reason);  // I think 102 is connection was closed by slave
            pageManager.showTitle("Pairing Error", String("Error: " + String(auth_cmpl.fail_reason)) + ". Retry later");
        }
    }
};

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                           uint8_t *pData, size_t length, bool isNotify) {}

bool BluetoothManager::connectToServer(BLEAddress &pAddress, esp_ble_addr_type_t addrType) {
    logf("Forming a connection to %s", pAddress.toString().c_str());

    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
    BLEDevice::setSecurityCallbacks(new MySecurity(pageManager));

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM);

    pSecurity->setCapability(ESP_IO_CAP_OUT);
    pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    BLEClient *pClient = BLEDevice::createClient();
    logm(" - Created client");

    logf(" - Connecting with address type %d", addrType, BLE_ADDR_TYPE_PUBLIC);
    if (pClient->connect(pAddress, BLE_ADDR_TYPE_PUBLIC) == false) {
        logm("Failed to connect to server");
        return false;
    }
    logm(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    this->pRemoteService = pClient->getService(serviceUUID);
    if (this->pRemoteService == nullptr) {
        logf("Failed to find our service UUID: %s", serviceUUID.toString().c_str());
        return false;
    }

    // Make sure we can find all characteristics UUIDs
    for (const auto &it : this->ServiceName2ServiceUuid) {
      BLERemoteCharacteristic *pRemoteCharacteristic =
          this->pRemoteService->getCharacteristic(BLEUUID(it.second));
      if (pRemoteCharacteristic == nullptr) {
        Serial.print("Failed to find our characteristic UUID: ");
        logm(it.second.c_str());
        return false;
      }
      logm((" - Found this characteristic: " + it.first).c_str());
    }
    logm("Found all characteristics!");
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
        logf("deserializeJson() failed: %s", error.c_str());
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
        logf("deserializeJson() failed: %s", error.c_str());
        return {};
    }
    int refreshRate = doc["value"];
    return refreshRate;
}

int BluetoothManager::getBaseSpending() {
    DynamicJsonDocument doc(200);
    auto output = requestService("BaseSpending");
    DeserializationError error = deserializeJson(doc, output);
    if (error) {
        logf("deserializeJson() failed: %s", error.c_str());
        return {};
    }
    int baseSpending = doc["value"];
    return baseSpending;
}

int BluetoothManager::getGoal() {
    DynamicJsonDocument doc(200);
    auto output = requestService("Goal");
    DeserializationError error = deserializeJson(doc, output);
    if (error) {
        logf("deserializeJson() failed: %s", error.c_str());
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
        logf("deserializeJson() failed: %s", error.c_str());
        return {};
    }
    int DaysLeft = doc["value"];
    return DaysLeft;
}

bool BluetoothManager::getShouldShowBEWarning() {
    DynamicJsonDocument doc(200);
    auto output = requestService("ShouldShowBEWarning");
    DeserializationError error = deserializeJson(doc, output);
    if (error) {
        logf("deserializeJson() failed: %s", error.c_str());
        return {};
    }
    return doc["value"];
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
        logf("deserializeJson() failed: %s", error.c_str());
        return doc;
    }
    return doc;
}

std::string BluetoothManager::requestService(const std::string &serviceName) {
    try {
    logf("Requesting characteristic '%s'....", serviceName.c_str());
    auto serviceUuid = ServiceName2ServiceUuid[serviceName];
    // logf("Service UUID is '%s'", serviceUuid.c_str());
    // logf("pRemoteService: %p", this->pRemoteService);
    BLERemoteCharacteristic *pRemoteCharacteristic = this->pRemoteService->getCharacteristic(BLEUUID(serviceUuid));
    auto res = pRemoteCharacteristic->readValue();

    logf(" - Got characteristic value %s: %s", serviceName.c_str(), res.c_str());
    return res;
    } catch (std::exception &e) {
        logf("Error in requestService: %s", e.what());
        return "Error";
    }
}

boolean waitForAuth() {
    while (!authed && !authedFailed) {
        delay(100);
    }

    return authed;
}

boolean isAuthedFailed() {
    return authedFailed;
}
void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() &&
        advertisedDevice.getServiceUUID().equals(serviceUUID)) {
        logf("- Found RPi: %s", advertisedDevice.toString().c_str());
        advertisedDevice.getScan()->stop();
        *(this->bleServerAddrType) = advertisedDevice.getAddressType();
        strncpy(this->bleServerAddrStr, advertisedDevice.getAddress().toString().c_str(), 18);
        *(this->doConnectPtr) = true;
    } else {
        logf("- Device: %s", advertisedDevice.toString().c_str());
    }
}

MyAdvertisedDeviceCallbacks::MyAdvertisedDeviceCallbacks(boolean *doConnectPtr,
                                                         char *bleServerAddrStr,
                                                         esp_ble_addr_type_t *bleServerAddrType)
    : doConnectPtr(doConnectPtr),
      bleServerAddrStr(bleServerAddrStr),
      bleServerAddrType(bleServerAddrType) {}