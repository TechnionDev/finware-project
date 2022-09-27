#ifndef DISPLAY_SRC_PAGEMANAGER_H_
#define DISPLAY_SRC_PAGEMANAGER_H_
#define LILYGO_T5_V213

#include <Arduino.h>
#include <ArduinoJson.h>

#include <map>

#include "epd_driver.h"
#include "utils.h"

#define NO_DIFF_YET 99999999

typedef std::map<std::string, int> cardsSpending;

class PageManager {
 private:
  short currentCourser = 0;
  void resetDisplay();
  void printPageMenu(int pageNum, int totalPages);
  void printDaysLeft(int daysLeft);
  void printTotalSum(int totalSum, int sumDiff);
  void printProgressAndGoal(int totalSum, int monthlyGoal);
  void printCardSpending(const std::map<std::string, int>& cardMap);
  void printCardSpendingOpt2(const std::map<std::string, int>& cardMap);

 public:
  uint8_t* framebuffer;
  explicit PageManager();
  void showSumPage(int totalSum, int daysLeft, int monthlyGoal, int sumDiff,
                   const std::map<std::string, int>& cardMap);
  void showCardSpendingPage(const std::map<std::string, int>& cardMap);
  void showTopFivePurchase();
  void showPassKey(uint32_t pass_key);
  void showGraphPage(String cycleStartDate, String cycleEndDate, int daysLeft,
                     int daysInCycle, JsonArray dataPoints);
  void showTitle(String title, String subtitle, int delayAfter = 0);
  void DrawBattery(int x, int y);
};

#endif  // DISPLAY_SRC_PAGEMANAGER_H_
