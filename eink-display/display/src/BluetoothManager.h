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
#include "pageManager.h"
#include <ArduinoJson.h>
// The remote service we wish to connect to.

static BLEUUID serviceUUID("2c82b713-f76a-4696-98eb-d92f9f233f40");
// The characteristic of the remote service we are interested in.

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static char *LOG_TAG = "INFO";

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
	/**
	  * Called for each advertising BLE server.
	  */
	void onResult(BLEAdvertisedDevice advertisedDevice) {
		Serial.print("BLE Advertised Device found: ");
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
		manager.showPassKey(pass_key);
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

typedef std::map<std::string, int> cardsSpending;

class BluetoothManager {
	BLERemoteService *pRemoteService{};
	std::string requestService(const std::string& serviceName);
	std::map<std::string, std::string> ServiceName2ServiceUuid {
		{"BankInfo", "5b278f16-4460-47e1-919e-2d50d7d0a55d"},
		{"RefreshRate", "49dc2b22-2dc4-4a66-afee-d7782b9b81cd"},
		{"Goal", "8f71bd04-89f7-4290-b90f-ac1265f5f127"},
		{"DaysLeft", "c27c1205-9ccb-4d1f-999f-0b9cfabf1d09"}
	};
  public:
	BluetoothManager();
	cardsSpending getBankInfo();
	int getRefreshRate();
	int getGoal();
	int getDaysLeft();
	bool connectToServer(BLEAddress pAddress);
};

#endif //DISPLAY_SRC_BLUETOOTHMANAGER_H_
