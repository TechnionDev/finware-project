#include "pageManager.h"

PageManager::PageManager(U8G2_FOR_ADAFRUIT_GFX &u8g2_, GxDEPG0213BN &display_,
                         GraphBuilder &gb_)
    : u8g2(u8g2_), display(display_), gb(gb_) {
  Serial.println("Initializing display");
  SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
  display.init();  // enable diagnostic output on Serial
  display.fillScreen(GxEPD_WHITE);
  display.setRotation(1);
  u8g2.begin(display);
  u8g2.setFont(
      u8g2_font_9x15_tr);    // select u8g2 font from here:
                             // https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.setFontMode(1);       // use u8g2 transparent mode (this is default)
  u8g2.setFontDirection(0);  // left to right (this is default)
  u8g2.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
  u8g2.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
}

void PageManager::printPageMenu(int pageNum, int totalPages) {
  short x = 9;
  short y = 9;
  for (int i = 0; i < totalPages; ++i) {
    if (pageNum == i) {
      display.fillCircle(x, y, 8, GxEPD_BLACK);
    } else {
      display.drawCircle(x, y, 8, GxEPD_BLACK);
    }
    x += 21;
  }
  this->currentCourser = x;
}

#define PADDING_DAYS_LEFT 8
#define FONT_HEIGHT() (u8g2.getFontAscent() - u8g2.getFontDescent())
#define HCENTER(containerw, str) \
  ((containerw - u8g2.getUTF8Width(str.c_str())) / 2)
#define VCENTER(containerh, str) ((containerh + (FONT_HEIGHT())) / 2)

void PageManager::printDaysLeft(int daysLeftNum) {
  u8g2.setFont(u8g2_font_9x15_mr);
  String daysLeft =
      String(daysLeftNum) + (daysLeftNum == 1 ? " Day left" : " Days left");
  int strWidth = u8g2.getUTF8Width(daysLeft.c_str());
  int rectX = display.width() - strWidth - PADDING_DAYS_LEFT;
  u8g2.setCursor(rectX + (PADDING_DAYS_LEFT / 2),
                 0 + FONT_HEIGHT() + (PADDING_DAYS_LEFT / 2) - 2);
  u8g2.print(daysLeft);
  display.drawRect(rectX, 0, strWidth + PADDING_DAYS_LEFT,
                   FONT_HEIGHT() + PADDING_DAYS_LEFT, GxEPD_BLACK);
}

void PageManager::printTotalSum(int totalSum) {
  u8g2.setFont(u8g2_font_inr30_mr);
  u8g2.setCursor(HCENTER(display.width(), String(totalSum)),
                 VCENTER(display.height(), String(totalSum)) - 5);
  u8g2.print((std::to_string(totalSum)).c_str());
}

void PageManager::printProgressAndGoal(int totalSum, int monthlyGoal) {
  int barWidth = 0.8 * display.width();
  int barX = (display.width() - barWidth) / 2;
  int barY = display.height() - 23;
  int barSize = 3;
  int indicatorRadius = 7;
  int goalIndicatorHeight = 10;
  double goalPositionPercent = 0.666667;

  display.fillRect(barX, barY, barWidth, barSize, GxEPD_BLACK);
  double progressPercent =
      ((totalSum / (double)monthlyGoal) * goalPositionPercent);
  int progIndicatorX = barX + (progressPercent * barWidth);
  int progIndicatorY = barY + (barSize / 2);
  int goalX = barX + (barWidth * goalPositionPercent);
  int goalY = barY - (goalIndicatorHeight / 2) - (barSize / 2) + 3;
  display.fillRect(goalX, goalY, 2, goalIndicatorHeight + barSize, GxEPD_BLACK);

  if (progressPercent <= goalPositionPercent) {
    u8g2.setFont(u8g2_font_8x13B_tr);
    display.fillCircle(progIndicatorX, progIndicatorY, indicatorRadius + 2,
                       GxEPD_WHITE);
    display.fillCircle(progIndicatorX, progIndicatorY, indicatorRadius,
                       GxEPD_BLACK);
    u8g2.setCursor(progIndicatorX + 1 - indicatorRadius +
                       HCENTER(indicatorRadius * 2, String("$")),
                   progIndicatorY - 1 - indicatorRadius +
                       VCENTER(indicatorRadius * 2, String("$")));
    u8g2.setForegroundColor(GxEPD_WHITE);
    u8g2.setBackgroundColor(GxEPD_BLACK);
    u8g2.print("$");
    u8g2.setForegroundColor(GxEPD_BLACK);
    u8g2.setBackgroundColor(GxEPD_WHITE);

  } else {
    u8g2.setFont(u8g2_font_7x13_tr);
    int warningY = barY - 8;
    int warningHeight = 11 + barSize;
    display.fillTriangle(progIndicatorX, warningY - 2, progIndicatorX - 10,
                         warningY + warningHeight, progIndicatorX + 10,
                         warningY + warningHeight, GxEPD_WHITE);
    display.fillTriangle(progIndicatorX, warningY, progIndicatorX - 8,
                         warningY + warningHeight, progIndicatorX + 8,
                         warningY + warningHeight, GxEPD_BLACK);
    u8g2.setCursor(progIndicatorX - 3, warningY + warningHeight);
    u8g2.setForegroundColor(GxEPD_WHITE);
    u8g2.setBackgroundColor(GxEPD_BLACK);
    u8g2.print("!");
    u8g2.setForegroundColor(GxEPD_BLACK);
    u8g2.setBackgroundColor(GxEPD_WHITE);
  }

  u8g2.setFont(u8g2_font_6x10_mr);
  String monthlyGoalStr = "Budget: " + String(monthlyGoal);
  u8g2.setCursor(goalX - (u8g2.getUTF8Width(monthlyGoalStr.c_str()) / 2) + 1,
                 goalY + goalIndicatorHeight + barSize + FONT_HEIGHT());
  u8g2.print(monthlyGoalStr.c_str());
}

void PageManager::printCardSpending(const std::map<std::string, int> &cardMap) {
  u8g2.setFont(u8g2_font_9x15_tr);
  u8g2.setCursor(HCENTER(display.width(), String("Credit Cards")), 15);
  u8g2.print("Credit Cards");

  u8g2.setFont(u8g2_font_10x20_tr);
  int y = 42;
  for (const auto &it : cardMap) {
    u8g2.setCursor(3, y);
    u8g2.println((it.first + ":").c_str());
    u8g2.setCursor(180, y);
    u8g2.println(std::to_string(it.second).c_str());
    y += 25;
  }
}
void resetDisplay(GxDEPG0213BN &display) {
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setBackgroundColor(GxEPD_WHITE);
}

void PageManager::showSumPage(int totalSum, int daysLeft, int monthlyGoal) {
  Serial.println("Printing Total Sum page");
  resetDisplay(display);
  printPageMenu(0, 3);
  printDaysLeft(daysLeft);
  printTotalSum(totalSum);
  printProgressAndGoal(totalSum, monthlyGoal);
  display.update();
}

void PageManager::showCardSpendingPage(
    const std::map<std::string, int> &cardMap) {
  Serial.println("Printing Card Spending page");
  resetDisplay(display);
  printPageMenu(1, 3);
  printCardSpending(cardMap);
  display.update();
}

void PageManager::showPassKey(uint32_t pass_key) {
  showTitle(String(pass_key), String("Enter passkey:"));
}

void PageManager::showTitle(String title, String subtitle, int delayAfter) {
  resetDisplay(display);
  u8g2.setFont(u8g2_font_9x15_tr);
  u8g2.setCursor(HCENTER(display.width(), subtitle), 20);
  u8g2.print(subtitle.c_str());

  u8g2.setFont(u8g2_font_fub20_tr);
  u8g2.setCursor(HCENTER(display.width(), title),
                 VCENTER(display.height(), title));
  u8g2.print(title.c_str());
  display.update();

  delay(delayAfter);
}

#define MAX_DATA_POINTS 31
void PageManager::showGraphPage(String cycleStartDate, String cycleEndDate,
                                int daysInCycle, JsonArray dataPoints) {
  Serial.println("Printing Monthly Spending Graph page");
  resetDisplay(display);
  printPageMenu(2, 3);
  float dataPointsArr[MAX_DATA_POINTS];
  copyArray(dataPoints, dataPointsArr);

  gb.DrawGraph(30, 24, 215, 80, -1, -1, "Monthly Spending", dataPointsArr,
               dataPoints.size(), daysInCycle, cycleStartDate, cycleEndDate,
               true, true);
  display.update();
}