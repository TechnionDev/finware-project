#include "pageManager.h"

#include "esp_adc_cal.h"
#include "roboto12.h"
#include "roboto16.h"
#include "roboto20.h"
#include "roboto26b.h"
#include "roboto45b.h"

PageManager::PageManager() {
  Serial.begin(115200);
  // framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT
  // / 2); if (!framebuffer) Serial.println("Memory alloc failed!");
  // memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  // framebuffer = (uint8_t*)malloc(EPD_WIDTH * EPD_HEIGHT / 2);
  Serial.println("Initializing display");
  epd_init();
  epd_poweron();
}

#define CIRCLES_MARGIN 20
void PageManager::printPageMenu(int pageNum, int totalPages) {
  int radius = 20;
  short x = 20 + radius / 2;
  short y = 20 + radius / 2;
  for (int i = 0; i < totalPages; ++i) {
    if (pageNum == i) {
      fillCircle(x, y, radius, Black, framebuffer);
    } else {
      // fillCircle(x, y, radius, 215, framebuffer);
      drawCircle(x, y, radius, 0x66, framebuffer);
      drawCircle(x, y, radius - 1, 0x66, framebuffer);
      drawCircle(x, y, radius - 2, 0x66, framebuffer);
    }
    if (i != totalPages - 1) {
      drawFastHLine(x + radius, y-1, CIRCLES_MARGIN, 0x22, framebuffer);
      drawFastHLine(x + radius, y, CIRCLES_MARGIN, 0x22, framebuffer);
      drawFastHLine(x + radius, y+1, CIRCLES_MARGIN, 0x22, framebuffer);
    }
    x += 2 * radius + CIRCLES_MARGIN;
  }
  this->currentCourser = x;
}

#define PADDING_DAYS_LEFT 36
#define FONT_EPD_HEIGHT (u8g2.getFontAscent() - u8g2.getFontDescent())
#define HCENTER(containerw, str) \
  ((containerw - u8g2.getUTF8Width(str.c_str())) / 2)
#define VCENTER(containerh, str) ((containerh + (FONT_EPD_HEIGHT)) / 2)

void PageManager::printDaysLeft(int daysLeftNum) {
  String daysLeft = String(daysLeftNum) +
                    (daysLeftNum == 1 ? " Day left" : " Days left") + "  |  ";

  int batteryMargin = 117;
  drawString(EPD_WIDTH - (PADDING_DAYS_LEFT / 2) - batteryMargin,
             (PADDING_DAYS_LEFT / 2) - 6, daysLeft, RIGHT, BOTTOM, &Roboto16,
             framebuffer);
  drawRect(EPD_WIDTH - (PADDING_DAYS_LEFT)-getTextWidth(daysLeft, &Roboto16) -
               batteryMargin,
           0,
           10 + PADDING_DAYS_LEFT + getTextWidth(daysLeft, &Roboto16) +
               batteryMargin,
           getTextHeight(daysLeft, &Roboto16) + PADDING_DAYS_LEFT, Black,
           framebuffer);
}

int vref = 1100;
void PageManager::DrawBattery(int x, int y) {
  uint8_t percentage = 100;
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
      ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
    vref = adc_chars.vref;
  }
  float voltage = analogRead(36) / 4096.0 * 6.566 * (vref / 1000.0);
  if (voltage > 1) {  // Only display if there is a valid reading
    Serial.println("\nVoltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) +
                 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage +
                 632041.7303;
    if (voltage >= 4.20) percentage = 100;
    if (voltage <= 3.20) percentage = 0;  // orig 3.5
    drawRect(x + 25, y - 14, 40, 15, Black, framebuffer);
    fillRect(x + 65, y - 10, 4, 7, Black, framebuffer);
    fillRect(x + 27, y - 12, 36 * percentage / 100.0, 11, Black, framebuffer);
    drawString(x + 78, y - 6, String(percentage) + "%", LEFT, &Roboto12,
               framebuffer);
  }
}

void PageManager::printTotalSum(int totalSum) {
  /*   u8g2.setFont(u8g2_font_inr30_mr);
    u8g2.setCursor(HCENTER(EPD_WIDTH, String(totalSum)),
                   VCENTER(EPD_HEIGHT, String(totalSum)) - 5);
    u8g2.print((std::to_string(totalSum)).c_str()); */
  drawString(EPD_WIDTH / 2, EPD_HEIGHT / 2, "₪" + String(totalSum), CENTER,
             CENTER, &Roboto45B, framebuffer);
}

void PageManager::printProgressAndGoal(int totalSum, int monthlyGoal) {
  int barWidth = 0.84 * EPD_WIDTH;
  int barX = (EPD_WIDTH - barWidth) / 2;
  int barY = EPD_HEIGHT - 80;
  int barSize = 3;
  int indicatorRadius = 19;
  int goalIndicatorHeight = 30;
  double goalPositionPercent = 0.666667;

  fillRect(barX, barY, barWidth, barSize, 0x9A, framebuffer);
  double progressPercent =
      ((totalSum / (double)monthlyGoal) * goalPositionPercent);
  int progIndicatorX = barX + (progressPercent * barWidth);
  int progIndicatorY = barY + (barSize / 2);
  int goalX = barX + (barWidth * goalPositionPercent);
  int goalY = barY - (goalIndicatorHeight / 2) - (barSize / 2) + 3;
  fillRect(goalX, goalY, 2, goalIndicatorHeight + barSize, Black, framebuffer);

  if (progressPercent <= goalPositionPercent) {
    fillCircle(progIndicatorX, progIndicatorY, indicatorRadius + 6, White,
               framebuffer);
    fillCircle(progIndicatorX, progIndicatorY, indicatorRadius, Black,
               framebuffer);

    const FontProperties fp = {15, 0x00, 0, 0};
    int dollarX = progIndicatorX - 9;
    int dollarY = progIndicatorY + 12;
    write_mode(&Roboto16, "$", &dollarX, &dollarY, framebuffer, WHITE_ON_BLACK,
               &fp);
    // drawString(progIndicatorX, progIndicatorY, "$", CENTER, CENTER,
    // &Roboto16,
    //            framebuffer);
  } else {
    int warningY = barY - 8;
    int warningHeight = 11 + barSize;
    fillTriangle(progIndicatorX, warningY - 2, progIndicatorX - 10,
                 warningY + warningHeight, progIndicatorX + 10,
                 warningY + warningHeight, White, framebuffer);
    fillTriangle(progIndicatorX, warningY, progIndicatorX - 8,
                 warningY + warningHeight, progIndicatorX + 8,
                 warningY + warningHeight, White, framebuffer);
    /*     u8g2.setCursor(progIndicatorX - 3, warningY + warningHeight);
        u8g2.setForegroundColor(GxEPD_WHITE);
        u8g2.setBackgroundColor(GxEPD_BLACK);
        u8g2.print("!");
        u8g2.setForegroundColor(GxEPD_BLACK);
        u8g2.setBackgroundColor(GxEPD_WHITE); */
  }

  /*   u8g2.setFont(u8g2_font_6x10_mr);
    u8g2.setCursor(goalX - (u8g2.getUTF8Width(monthlyGoalStr.c_str()) / 2) + 1,
                   goalY + goalIndicatorHeight + barSize + FONT_EPD_HEIGHT);
    u8g2.print(monthlyGoalStr.c_str()); */
  String monthlyGoalStr = "Budget: " + String(monthlyGoal);
  drawString(goalX, goalY + goalIndicatorHeight / 2 + 20, monthlyGoalStr,
             CENTER, BOTTOM, &Roboto16, framebuffer);
}

void PageManager::printCardSpending(const std::map<std::string, int> &cardMap) {
  /*   u8g2.setFont(u8g2_font_9x15_tr);
    u8g2.setCursor(HCENTER(EPD_WIDTH, String("Credit Cards")), 15);
    u8g2.print("Credit Cards"); */
  drawString(EPD_WIDTH / 2, 15, String("Credit Cards"), CENTER, &FiraSans,
             framebuffer);

  // u8g2.setFont(u8g2_font_10x20_tr);
  // int y = 42;
  // for (const auto &it : cardMap) {
  //   u8g2.setCursor(3, y);
  //   u8g2.println((it.first + ":").c_str());
  //   u8g2.setCursor(180, y);
  //   u8g2.println(std::to_string(it.second).c_str());
  //   y += 25;
  // }
}
void resetDisplay() { epd_clear(); }

void PageManager::showSumPage(int totalSum, int daysLeft, int monthlyGoal) {
  Serial.println("Printing Total Sum page");
  resetDisplay();
  printPageMenu(0, 3);
  printDaysLeft(daysLeft);
  printTotalSum(totalSum);
  printProgressAndGoal(totalSum, monthlyGoal);
  DrawBattery(EPD_WIDTH - 150, 42);
  epd_update(framebuffer);
}

void PageManager::showCardSpendingPage(
    const std::map<std::string, int> &cardMap) {
  Serial.println("Printing Card Spending page");
  resetDisplay();
  printPageMenu(1, 3);
  printCardSpending(cardMap);
  epd_update(framebuffer);
}

void PageManager::showPassKey(uint32_t pass_key) {
  showTitle(String(pass_key), String("Enter passkey:"));
}

void PageManager::showTitle(String title, String subtitle, int delayAfter) {
  resetDisplay();
  /*   u8g2.setFont(u8g2_font_9x15_tr);
    u8g2.setCursor(HCENTER(EPD_WIDTH, subtitle), 20);
    u8g2.print(subtitle.c_str()); */
  drawString(EPD_WIDTH / 2, 20, subtitle, CENTER, &Roboto26B, framebuffer);

  /*   u8g2.setFont(u8g2_font_fub20_tr);
    u8g2.setCursor(HCENTER(EPD_WIDTH, title), VCENTER(EPD_HEIGHT, title));
    u8g2.print(title.c_str()); */
  drawString(EPD_WIDTH / 2, EPD_HEIGHT / 2, title, CENTER, &Roboto26B,
             framebuffer);
  epd_update(framebuffer);

  delay(delayAfter);
}

#define MAX_DATA_POINTS 31
void PageManager::showGraphPage(String cycleStartDate, String cycleEndDate,
                                int daysInCycle, JsonArray dataPoints) {
  Serial.println("Printing Monthly Spending Graph page");
  resetDisplay();
  printPageMenu(2, 3);
  float dataPointsArr[MAX_DATA_POINTS];
  copyArray(dataPoints, dataPointsArr);

  DrawGraph(30, 24, 215, 80, -1, -1, "Monthly Spending", dataPointsArr,
            dataPoints.size(), daysInCycle, cycleStartDate, cycleEndDate, true,
            true, framebuffer);
  epd_update(framebuffer);
}