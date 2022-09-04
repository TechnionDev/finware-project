/*
			Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
			Ported to Arduino ESP32 by Evandro Copercini
*/
#include "pageManager.h"
#include "BluetoothManager.h"

BluetoothManager blm;

void setup() {
	Serial.begin(115200);
	Serial.println("Starting Arduino BLE Client application...");
	BLEDevice::init("");

	// Retrieve a Scanner and set the callback we want to use to be informed when we
	// have detectesd a new device.  Specify that we want active scanning and start the
	// scan to run for 30 seconds.
	BLEScan *pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
	pBLEScan->setActiveScan(true);
	pBLEScan->start(30);

#if defined(LILYGO_EPD_DISPLAY_102)
	pinMode(EPD_POWER_ENABLE, OUTPUT);
	digitalWrite(EPD_POWER_ENABLE, HIGH);
#endif /*LILYGO_EPD_DISPLAY_102*/
#if defined(LILYGO_T5_V102)
	pinMode(POWER_ENABLE, OUTPUT);
	digitalWrite(POWER_ENABLE, HIGH);
#endif /*LILYGO_T5_V102*/

	SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
	display.init(); // enable diagnostic output on Serial
	display.setRotation(1);
	Serial.println("setup done");

} // End of setup.


// This is the Arduino main loop function.
void loop() {
	Serial.println("in loop");
	// If the flag "doConnect" is true then we have scanned for and found the desired
	// BLE Server with which we wish to connect.  Now we connect to it.  Once we are
	// connected we set the connected flag to be true.
	if (doConnect) {
		if (blm.connectToServer(*pServerAddress)) {
			Serial.println("We are now connected to the BLE Server.");
			connected = true;
		} else {
			Serial.println("We have failed to connect to the server; there is nothin more we will do.");
		}
		doConnect = false;
	}

	// If we are connected to a peer BLE Server, update the characteristic each time we are reached
	// with the current time since boot.
	if (connected) {
		int totalSum = 0;
		for (const auto& it: blm.getBankInfo()) {
			totalSum += it.second;
		}
		manager.showSumPage(totalSum,blm.getDaysLeft(), blm.getGoal());
	}

	delay(blm.getRefreshRate()); // Delay a second between loops.
} // End of loop
