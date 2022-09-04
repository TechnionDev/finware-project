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
	//TODO::SWITCH TO BOND
//	pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
	pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM);

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
		Serial.println("hiiiii");
		bankInfo[kv.key().c_str()] = -kv.value().as<int>();
	}

	for (const auto& it : bankInfo){
		Serial.println(it.first.c_str());
		Serial.println(it.second);
	}
	return bankInfo;
}

int BluetoothManager::getRefreshRate() {
	Serial.println("RefreshRate");
	DynamicJsonDocument doc(200);
	auto output = requestService("RefreshRate");
	DeserializationError error = deserializeJson(doc, output);
	if (error) {
		Serial.print("deserializeJson() failed: ");
		Serial.println(error.c_str());
		return {};
	}
	int refreshRate = doc["value"];
	Serial.println(refreshRate);
	Serial.println("out of RefreshRate");
	return refreshRate;
}
int BluetoothManager::getGoal() {
	Serial.println("getGoal");
	DynamicJsonDocument doc(200);
	auto output = requestService("Goal");
	DeserializationError error = deserializeJson(doc, output);
	if (error) {
		Serial.print("deserializeJson() failed: ");
		Serial.println(error.c_str());
		return {};
	}
	int Goal = doc["value"];
	Serial.println("out of Goal");

	return Goal;
}

int BluetoothManager::getDaysLeft() {
	Serial.println("getDaysLeft");
	DynamicJsonDocument doc(200);
	auto output = requestService("DaysLeft");
	DeserializationError error = deserializeJson(doc, output);
	if (error) {
		Serial.print("deserializeJson() failed: ");
		Serial.println(error.c_str());
		return {};
	}
	int DaysLeft = doc["value"];
	Serial.println("out of getDaysLeft");
	return DaysLeft;
}

std::string BluetoothManager::requestService(const std::string &serviceName) {
	Serial.println(("requesting this service " + serviceName + ": ").c_str());
	auto serviceUuid = ServiceName2ServiceUuid[serviceName];
	BLERemoteCharacteristic *pRemoteCharacteristic = this->pRemoteService->getCharacteristic(BLEUUID(serviceUuid));
	auto res =  pRemoteCharacteristic->readValue();

	Serial.println(("this is the " + serviceName + ": ").c_str());
	Serial.println(res.c_str());
	return res;
}
