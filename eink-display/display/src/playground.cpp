#include <Arduino.h>
/*
			Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
			Ported to Arduino ESP32 by Evandro Copercini
*/

#include "BluetoothManager.h"
#define LILYGO_T5_V213
// #define LILYGO_T5_V22
// #define LILYGO_T5_V24
// #define LILYGO_T5_V28
// #define LILYGO_T5_V102
// #define LILYGO_T5_V266
// #define LILYGO_EPD_DISPLAY_102
// #define LILYGO_EPD_DISPLAY_154


#include <boards.h>
#include <GxEPD.h>

#if defined(LILYGO_T5_V102) || defined(LILYGO_EPD_DISPLAY_102)
#include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w
#elif defined(LILYGO_T5_V266)
#include <GxDEPG0266BN/GxDEPG0266BN.h>    // 2.66" b/w   form DKE GROUP
#elif defined(LILYGO_T5_V213)
#include <GxDEPG0213BN/GxDEPG0213BN.h>    // 2.13" b/w  form DKE GROUP
#else
// #include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w
// #include <GxGDEW0154Z04/GxGDEW0154Z04.h>  // 1.54" b/w/r 200x200
// #include <GxGDEW0154Z17/GxGDEW0154Z17.h>  // 1.54" b/w/r 152x152
// #include <GxGDEH0154D67/GxGDEH0154D67.h>  // 1.54" b/w
// #include <GxDEPG0150BN/GxDEPG0150BN.h>    // 1.51" b/w   form DKE GROUP
// #include <GxDEPG0266BN/GxDEPG0266BN.h>    // 2.66" b/w   form DKE GROUP
// #include <GxDEPG0290R/GxDEPG0290R.h>      // 2.9" b/w/r  form DKE GROUP
// #include <GxDEPG0290B/GxDEPG0290B.h>      // 2.9" b/w    form DKE GROUP
// #include <GxGDEW029Z10/GxGDEW029Z10.h>    // 2.9" b/w/r  form GoodDisplay
// #include <GxGDEW0213Z16/GxGDEW0213Z16.h>  // 2.13" b/w/r form GoodDisplay
// #include <GxGDE0213B1/GxGDE0213B1.h>      // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEH0213B72/GxGDEH0213B72.h>  // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEH0213B73/GxGDEH0213B73.h>  // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEM0213B74/GxGDEM0213B74.h>  // 2.13" b/w  form GoodDisplay 4-color
// #include <GxGDEW0213M21/GxGDEW0213M21.h>  // 2.13"  b/w Ultra wide temperature , form GoodDisplay
// #include <GxDEPG0213BN/GxDEPG0213BN.h>    // 2.13" b/w  form DKE GROUP
// #include <GxGDEW027W3/GxGDEW027W3.h>      // 2.7" b/w   form GoodDisplay
// #include <GxGDEW027C44/GxGDEW027C44.h>    // 2.7" b/w/r form GoodDisplay
// #include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w   form GoodDisplay
// #include <GxDEPG0750BN/GxDEPG0750BN.h>    // 7.5" b/w   form DKE GROUP
#endif

#include GxEPD_BitmapExamples
#include "pageManager.h"
// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

void LilyGo_logo();

void setup() {
	Serial.begin(115200);
//	Serial.println("Starting Arduino BLE Client application...");
//	BLEDevice::init("");
//
//	// Retrieve a Scanner and set the callback we want to use to be informed when we
//	// have detected a new device.  Specify that we want active scanning and start the
//	// scan to run for 30 seconds.
//	BLEScan *pBLEScan = BLEDevice::getScan();
//	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//	pBLEScan->setActiveScan(true);
//	pBLEScan->start(30);

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
	display.setRotation(1); //todo::move to setup>

	//LilyGo_logo();
	Serial.println("setup done");

} // End of setup.


// This is the Arduino main loop function.
void loop() {
	pageManager manager(display);
	Serial.println("req print:");
	//manager.showSumPage(9999,1,10000);
	manager.showCardSpendingPage({});
	delay(100000);
//	BluetoothManager blm;
//	// If the flag "doConnect" is true then we have scanned for and found the desired
//	// BLE Server with which we wish to connect.  Now we connect to it.  Once we are
//	// connected we set the connected flag to be true.
//
//	display.setRotation(1); //todo::move to setup>
//	display.fillScreen(GxEPD_WHITE);
//	display.setTextColor(GxEPD_BLACK);
//	display.setFont(&FreeMonoBold9pt7b);
//	display.setCursor(0, 0);
//	display.println("Gur Noob");
////    display.setFont(&FreeMonoBold12pt7b);
////    display.println("Dani Noob");
//	display.setFont(&FreeMonoBold18pt7b);
//	display.println("Thank you very much Tom!");
//	display.update();
//	delay(100000);
//
//	if (doConnect == true) {
//		if (blm.connectToServer(*pServerAddress)) {
//			Serial.println("We are now connected to the BLE Server.");
//			connected = true;
//		} else {
//			Serial.println("We have failed to connect to the server; there is nothin more we will do.");
//		}
//		doConnect = false;
//	}
//
//	// If we are connected to a peer BLE Server, update the characteristic each time we are reached
//	// with the current time since boot.
//	if (connected) {
//		String newValue = "Time since boot: " + String(millis() / 1000);
//		Serial.println("Setting new characteristic value to \"" + newValue + "\"");
//
//		// Set the characteristic's value to be the array of bytes that is actually a string.
//		pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
//	}
//
//	delay(1000); // Delay a second between loops.
} // End of loop


void LilyGo_logo(void)
{
	display.fillScreen(GxEPD_WHITE);
	display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
	display.update();
}