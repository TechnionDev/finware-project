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
