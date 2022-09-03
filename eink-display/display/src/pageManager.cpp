//
// Created by Dani Bondar on 29/08/2022.
//
#include "pageManager.h"

pageManager::pageManager(GxDEPG0213BN &Display) : display(Display) {
}
void pageManager::setupPageMenu(int pageNum, int totalPages) {
	short x = 9;
	short y = 9;
	for (int i = 0; i < totalPages; ++i) {
		if (pageNum == i) {
			//display.drawBitmap(fullCircle, x, y, circleSize, circleSize, GxEPD_BLACK, 0);
			display.fillCircle(x, y, 8, GxEPD_BLACK);
		} else {
			//display.drawBitmap(emptyCircle, x, y, circleSize, circleSize, GxEPD_BLACK, 0);
			display.drawCircle(x, y, 8, GxEPD_BLACK);
		}
		x += 21;
	}
	this->currentCourser = x;
}

void pageManager::printDaysLeft(int daysLeft) {
	display.setFont(&FreeMono9pt7b);
	if (daysLeft == 1) {
		display.drawRect(120, 0, 130, 25, GxEPD_BLACK);
		display.setCursor(125, 15);
		display.print((std::to_string(daysLeft) + " Day left").c_str());
	} else {
		display.drawRect(110, 0, 140, 25, GxEPD_BLACK);
		display.setCursor(115, 15);
		display.print((std::to_string(daysLeft) + " Days left").c_str());
	}
}

void pageManager::printTotalSum(int totalSum) {
	if(totalSum <100){
		display.setCursor(80, 70);
	}
	else if (totalSum >= 10000) {
		display.setCursor(58, 70);
	} else {
		display.setCursor(70, 70);
	}
	display.setFont(&FreeMonoBold24pt7b);
	display.print((std::to_string(totalSum)).c_str());
}

void pageManager::printProgressBar(int totalSum, int monthlyGoal) {
	display.drawRoundRect(5, 90, 150, 30, 6, GxEPD_BLACK);
	int ratio = 0;
	if (totalSum >= monthlyGoal) {
		ratio = 12;
	} else {
		ratio = floor(((double(totalSum) / monthlyGoal) * 12));
	}
	Serial.println(std::to_string(ratio).c_str());
	int x = 9;
	for (int i = 0; i < ratio; i++) {
		display.fillRect(x, 92, 10, 26, GxEPD_BLACK);
		x += 10 + 2;
	}
}

void pageManager::printMonthlyGoal(int monthlyGoal) {
	display.setFont(&FreeMono9pt7b);
	display.setCursor(180, 95);
	display.print("Goal");
	display.drawFastHLine(173, 99, 54, GxEPD_BLACK);
	if(monthlyGoal<1000){
		display.setCursor(186, 115);
		display.print(std::to_string(monthlyGoal).c_str());
	}else{
		display.setCursor(173, 115);
		display.print(std::to_string(monthlyGoal).c_str());
	}
}

void pageManager::printCardSpending(const std::map<std::string, int> &cardMap) {
	int y = 44;
	display.setFont(&FreeMonoBold12pt7b);
	for (const auto& it: cardMap) {
		display.setCursor(3, y);
		display.println((it.first + ":").c_str());
		display.setCursor(150,y);
		display.println(std::to_string(it.second).c_str());
		y+=25;
	}
}

void pageManager::showSumPage(int totalSum, int daysLeft, int monthlyGoal) {
	Serial.println("Starting printing first page");
	display.setRotation(1);
	display.fillScreen(GxEPD_WHITE);
	display.setTextColor(GxEPD_BLACK);
	display.setBackgroundColor(GxEPD_WHITE);
	setupPageMenu(0, 3);
	printDaysLeft(daysLeft);
	printTotalSum(totalSum);
	printProgressBar(totalSum, monthlyGoal);
	printMonthlyGoal(monthlyGoal);
	Serial.println("end print");
	display.update();
	Serial.println("end update");
}
void pageManager::showCardSpendingPage(const std::map<std::string, int> &cardMap) {
	Serial.println("Starting printing spending page");
	display.fillScreen(GxEPD_WHITE);
	display.setTextColor(GxEPD_BLACK);
	display.setBackgroundColor(GxEPD_WHITE);
	setupPageMenu(1, 3);
	printCardSpending(cardMap);
	Serial.println("end print");
	display.update();
	Serial.println("end update");
}
void pageManager::showPassKey(uint32_t pass_key) {
	Serial.println("Hi im showing the passkey");
	display.setRotation(1);
	display.fillScreen(GxEPD_WHITE);
	display.setTextColor(GxEPD_BLACK);
	display.setBackgroundColor(GxEPD_WHITE);
	display.setFont(&FreeMonoBold12pt7b);
	display.setCursor(0,15);
	display.println("Enter this passkey in configuration portal:");
	display.println(pass_key);
	display.update();
}




