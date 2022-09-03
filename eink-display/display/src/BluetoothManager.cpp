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
	BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
	if (pRemoteService == nullptr) {
		Serial.print("Failed to find our service UUID: ");
		Serial.println(serviceUUID.toString().c_str());
		return false;
	}
	Serial.println(" - Found our service");


	// Obtain a reference to the characteristic in the service of the remote BLE server.
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
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */

BluetoothManager::BluetoothManager() {

}

cardsSpending BluetoothManager::getBankInfo() {
	DynamicJsonDocument doc(200);
	auto output = requestService("BankInfo");
	//TODO:: remove json
	output = "{\"hapoalim\": 5000,\"max\": 7000}";
	DeserializationError error = deserializeJson(doc, output);
	if (error) {
		Serial.print("deserializeJson() failed: ");
		Serial.println(error.c_str());
		return {};
	}
	cardsSpending bankInfo;
	JsonObject root = doc.as<JsonObject>();
	for (JsonPair kv : root) {
		bankInfo[kv.key().c_str()] = kv.value().as<int>();
	}
	return bankInfo;
}

int BluetoothManager::getRefreshRate() {
//	int refreshRate = std::stoi(requestService("RefreshRate"));
	int refreshRate = 100000;
	return refreshRate;
}
int BluetoothManager::getGoal() {
//	int Goal = std::stoi(requestService("Goal"));//TODO:: uncomment
	int Goal = 15000;//TODO:: remove
	return Goal;
}
int BluetoothManager::getDaysLeft() {
	Serial.println("getDaysLeft");
	//int DaysLeft = std::stoi(requestService("DaysLeft"));//TODO:: uncomment
	Serial.println("out of getDaysLeft");
	int DaysLeft = 15;//TODO:: remove
	return DaysLeft;
}

std::string BluetoothManager::requestService(const std::string &serviceName) {
	std::map<std::string, std::string> ServiceName2ServiceUuid {
		{"BankInfo", "5b278f16-4460-47e1-919e-2d50d7d0a55d"},
		{"RefreshRate", "49dc2b22-2dc4-4a66-afee-d7782b9b81cd"},
		{"Goal", "8f71bd04-89f7-4290-b90f-ac1265f5f127"},
		{"DaysLeft", "c27c1205-9ccb-4d1f-999f-0b9cfabf1d09"}
	};
	auto serviceUuid = ServiceName2ServiceUuid[serviceName];
	//TODO::complete ble call for service


	return "";
}
