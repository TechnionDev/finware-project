#ifndef DISPLAY_SRC_PAGEMANAGER_H_
#define DISPLAY_SRC_PAGEMANAGER_H_
#define LILYGO_T5_V213

#include <Arduino.h>
#include <ArduinoJson.h>

#include <map>

#include "epd_driver.h"
#include "firesans.h"
#include "utils.h"

class PageManager {
 private:
  short currentCourser = 0;
  void resetDisplay();
  void printPageMenu(int pageNum, int totalPages);
  void printDaysLeft(int daysLeft);
  void printTotalSum(int totalSum);
  void printProgressAndGoal(int totalSum, int monthlyGoal);
  void printCardSpending(const std::map<std::string, int>& cardMap);

 public:
  uint8_t* framebuffer;
  explicit PageManager();
  void showSumPage(int totalSum, int daysLeft, int monthlyGoal);
  void showCardSpendingPage(const std::map<std::string, int>& cardMap);
  void showTopFivePurchase();
  void showPassKey(uint32_t pass_key);
  void showGraphPage(String cycleStartDate, String cycleEndDate,
                     int daysInCycle, JsonArray dataPoints);
  void showTitle(String title, String subtitle, int delayAfter = 0);
  void DrawBattery(int x, int y);
};

#endif  // DISPLAY_SRC_PAGEMANAGER_H_
