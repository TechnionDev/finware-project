//
// Created by Dani Bondar on 29/08/2022.
//
#include <Arduino.h>
#ifndef DISPLAY_SRC_PAGEMANAGER_H_
#define DISPLAY_SRC_PAGEMANAGER_H_
#define LILYGO_T5_V213

#include <boards.h>
#include <GxEPD.h>
#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h>    // 2.13" b/w  form DKE GROUP

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <map>

class pageManager {
  private:
	GxEPD_Class display;
	short currentCourser = 0;
	void setupPageMenu(int pageNum, int totalPages);
	void printDaysLeft(int daysLeft);
	void printTotalSum(int totalSum);
	void printProgressBar(int totalSum, int monthlyGoal);
	void printMonthlyGoal(int monthlyGoal);
	void printCardSpending(const std::map<std::string,int>& cardMap);
  public:
	explicit pageManager(GxEPD_Class& display);
	void showSumPage(int totalSum, int daysLeft, int monthlyGoal);
	void showCardSpendingPage(const std::map<std::string,int>& cardMap);
	void showTopFivePurchase();


};



#endif //DISPLAY_SRC_PAGEMANAGER_H_
