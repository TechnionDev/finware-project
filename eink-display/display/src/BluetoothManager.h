//
// Created by Dani Bondar on 29/08/2022.
//

#ifndef DISPLAY_SRC_BLUETOOTHMANAGER_H_
#define DISPLAY_SRC_BLUETOOTHMANAGER_H_
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Arduino.h>
// The remote service we wish to connect to.
static BLEUUID serviceUUID("22222222-3333-4444-5555-666666666666");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("22222222-3333-4444-5555-666666666667");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static char *LOG_TAG = "INFO";

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
	/**
	  * Called for each advertising BLE server.
	  */
	void onResult(BLEAdvertisedDevice advertisedDevice) {
		Serial.print("Dani BLE Advertised Device found: ");
		Serial.println(advertisedDevice.toString().c_str());

		// We have found a device, let us now see if it contains the service we are looking for.
		if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {

			//
			Serial.print("Found our device!  address: ");
			advertisedDevice.getScan()->stop();

			pServerAddress = new BLEAddress(advertisedDevice.getAddress());
			doConnect = true;

		} // Found our server
	} // onResult
}; // MyAdvertisedDeviceCallbacks

class MySecurity : public BLESecurityCallbacks {

	uint32_t onPassKeyRequest() {
		return 123456;
	}
	void onPassKeyNotify(uint32_t pass_key) {
		Serial.printf("onPassKeyNotify \n");
		ESP_LOGE(LOG_TAG, "The passkey Notify number:%d", pass_key);
	}
	bool onConfirmPIN(uint32_t pass_key) {
		Serial.printf("onConfirmPIN \n");
		Serial.printf("The passkey YES/NO number:%d", pass_key);
		vTaskDelay(5000);
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
		}
		ESP_LOGI(LOG_TAG, "pair status = %s", auth_cmpl.success ? "success" : "fail");
	}
};

class cardArray{
	//TODO:: fill class
};
class BluetoothManager {
	//TODO:: Implement class using the code in the cpp file
  public:
	BluetoothManager();
	int getTotalSpending();
	cardArray getSpendingPerCard();
	cardArray getTopFivePurchase();
	bool connectToServer(BLEAddress pAddress);
};

#endif //DISPLAY_SRC_BLUETOOTHMANAGER_H_
