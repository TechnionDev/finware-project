#ifndef DISPLAY_SRC_PAGEMANAGER_H_
#define DISPLAY_SRC_PAGEMANAGER_H_
#define LILYGO_T5_V213

#include <Arduino.h>
#include <ArduinoJson.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h>  // 2.13" b/w  form DKE GROUP
#include <GxEPD.h>
#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <boards.h>

#include "utils.h"

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include <map>

class PageManager {
 private:
  GxEPD_Class& display;
  U8G2_FOR_ADAFRUIT_GFX& u8g2;
  GraphBuilder& gb;
  short currentCourser = 0;
  void printPageMenu(int pageNum, int totalPages);
  void printDaysLeft(int daysLeft);
  void printTotalSum(int totalSum);
  void printProgressAndGoal(int totalSum, int monthlyGoal);
  void printCardSpending(const std::map<std::string, int>& cardMap);

 public:
  explicit PageManager(U8G2_FOR_ADAFRUIT_GFX& u8g2, GxEPD_Class& display,
                       GraphBuilder& gb);
  void showSumPage(int totalSum, int daysLeft, int monthlyGoal);
  void showCardSpendingPage(const std::map<std::string, int>& cardMap);
  void showTopFivePurchase();
  void showPassKey(uint32_t pass_key);
  void showGraphPage(String cycleStartDate, String cycleEndDate,
                     int daysInCycle, JsonArray dataPoints);
  void showTitle(String title, String subtitle, int delayAfter=0);
  // void showGraphPage();
};

#endif  // DISPLAY_SRC_PAGEMANAGER_H_
