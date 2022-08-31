//
// Created by Dani Bondar on 29/08/2022.
//
#define LILYGO_T5_V213
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
// FreeFonts from Adafruit_GFX

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <utility>
#include "pageManager.h"
#define HEADER_SIZE 16
pageManager::pageManager(GxDEPG0213BN &Display) : display(Display) {
}
void pageManager::setupPageMenu(int pageNum, int totalPages) {
	short circleSize = HEADER_SIZE;
	short x = 9;
	short y = 9;
	for (int i = 0; i < totalPages; ++i) {
		if (pageNum == i) {
			//display.drawBitmap(fullCircle, x, y, circleSize, circleSize, GxEPD_BLACK, 0);
			display.fillCircle(x,y,8,GxEPD_BLACK);
		} else {
			//display.drawBitmap(emptyCircle, x, y, circleSize, circleSize, GxEPD_BLACK, 0);
			display.drawCircle(x,y,8,GxEPD_BLACK);
		}
		x += circleSize;
	}
	this->currentCourser = x;
}

void pageManager::printTitle(const std::string& title) {
	display.setFont(&FreeMonoBold12pt7b);
	display.setCursor(this->currentCourser+8,15); // magic numbers for nicer layout on screen
	display.print(title.c_str());
}

void pageManager::printDaysLeft(int daysLeft) {
	display.drawRoundRect(display.getCursorX()+5, 0,93,60,10,GxEPD_BLACK);

	display.setCursor(display.getCursorX()+11, 13);
	display.setFont(&FreeMono9pt7b);
	display.print("Days To:");

	display.setCursor(185, 45);
	display.setFont(&FreeMonoBold12pt7b);
	display.print(std::to_string(daysLeft).c_str());
}

void pageManager::printTotalSum(int totalSum) {
	if (totalSum>=10000){
		display.setCursor(8, 60);
	} else{
		display.setCursor(30, 60);
	}
	display.setFont(&FreeMonoBold24pt7b);
	display.print(std::to_string(totalSum).c_str());
}

void pageManager::showSumPage(int totalSum, int daysLeft, int monthlyGoal) {
	Serial.println("Starting printing first page");
	display.fillScreen(GxEPD_WHITE);
	display.setTextColor(GxEPD_BLACK);
	display.setBackgroundColor(GxEPD_WHITE);
	setupPageMenu(0,3);
	printTitle("Total:");
	printDaysLeft(30);
	printTotalSum(5000);
	Serial.println("end print");
	display.update();
	Serial.println("end update");

}

