//
// Created by Dani Bondar on 29/08/2022.
//

#include "BluetoothManager.h"

static void notifyCallback(
	BLERemoteCharacteristic *pBLERemoteCharacteristic,
	uint8_t *pData,
	size_t length,
	bool isNotify) {
	Serial.print("Notify callback for characteristic ");
	Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
	Serial.print(" of data length ");
	Serial.println(length);
}

bool BluetoothManager::connectToServer(BLEAddress pAddress) {
	Serial.print("Forming a connection to ");
	Serial.println(pAddress.toString().c_str());

	BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
	BLEDevice::setSecurityCallbacks(new MySecurity());

	BLESecurity *pSecurity = new BLESecurity();
	pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
	pSecurity->setCapability(ESP_IO_CAP_OUT);
	pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
	BLEClient *pClient = BLEDevice::createClient();
	Serial.println(" - Created client");

	// Connect to the remove(remote?) BLE Server.
	pClient->connect(pAddress, BLE_ADDR_TYPE_PUBLIC);
	Serial.println(" - Connected to server");

	// Obtain a reference to the service we are after in the remote BLE server.
	this->pRemoteService = pClient->getService(serviceUUID);
	if (this->pRemoteService == nullptr) {
		Serial.print("Failed to find our service UUID: ");
		Serial.println(serviceUUID.toString().c_str());
		return false;
	}
	Serial.println(" - Found our service");


	// Obtain a reference to the characteristic in the service of the remote BLE server.
	for (const auto &it: this->ServiceName2ServiceUuid) {
		BLERemoteCharacteristic *pRemoteCharacteristic = this->pRemoteService->getCharacteristic(BLEUUID(it.second));
		if (pRemoteCharacteristic == nullptr) {
			Serial.print("Failed to find our characteristic UUID: ");
			Serial.println(it.second.c_str());
			return false;
		}
		Serial.println((" - Found this characteristic: " + it.first).c_str());
	}
	Serial.println("Found all characteristics!");
	return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */

BluetoothManager::BluetoothManager() = default;

cardsSpending BluetoothManager::getBankInfo() {
	DynamicJsonDocument doc(200);
	auto output = requestService("BankInfo");
	DeserializationError error = deserializeJson(doc, output);
	if (error) {
		Serial.print("deserializeJson() failed: ");
		Serial.println(error.c_str());
		return {};
	}
	cardsSpending bankInfo;
	JsonObject root = doc.as<JsonObject>();
	for (JsonPair kv: root) {
		bankInfo[kv.key().c_str()] = kv.value().as<int>();
	}
	return bankInfo;
}

int BluetoothManager::getRefreshRate() {
	int refreshRate = std::stoi(requestService("RefreshRate"));
	return refreshRate;
}
int BluetoothManager::getGoal() {
	int Goal = std::stoi(requestService("Goal"));
	return Goal;
}
int BluetoothManager::getDaysLeft() {
	Serial.println("getDaysLeft");
	int DaysLeft = std::stoi(requestService("DaysLeft"));
	Serial.println("out of getDaysLeft");
	return DaysLeft;
}

std::string BluetoothManager::requestService(const std::string &serviceName) {
	auto serviceUuid = ServiceName2ServiceUuid[serviceName];
	BLERemoteCharacteristic *pRemoteCharacteristic = this->pRemoteService->getCharacteristic(BLEUUID(serviceUuid));
	auto res =  pRemoteCharacteristic->readValue();


	return res;
}
