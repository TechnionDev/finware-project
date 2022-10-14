#include "pageManager.h"

PageManager::PageManager(U8G2_FOR_ADAFRUIT_GFX &u8g2_, GxDEPG0213BN &display_,
                         GraphBuilder &gb_)
    : u8g2(u8g2_), display(display_), gb(gb_) {
    logm("Initializing display");
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init();  // enable diagnostic output on Serial
    display.fillScreen(GxEPD_WHITE);
    display.setRotation(3);
    this->u8g2.begin(display);
    this->u8g2.setFont(
        u8g2_font_9x15_tr);                      // select u8g2 font from here:
                                                 // https://github.com/olikraus/u8g2/wiki/fntlistall
    this->u8g2.setFontMode(1);                   // use u8g2 transparent mode (this is default)
    this->u8g2.setFontDirection(0);              // left to right (this is default)
    this->u8g2.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
    this->u8g2.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
}

#define CIRCLES_MARGIN 6
#define CIRCLES_RADIUS 6
void PageManager::printPageMenu(int pageNum, int totalPages) {
    short x = 9;
    short y = 9;
    for (int i = 0; i < totalPages; ++i) {
        if (pageNum == i) {
            display.fillCircle(x, y, CIRCLES_RADIUS, GxEPD_BLACK);
        } else {
            display.drawCircle(x, y, CIRCLES_RADIUS, GxEPD_BLACK);
        }
        if (i != totalPages - 1) {
            display.drawFastHLine(x + CIRCLES_RADIUS, y, CIRCLES_MARGIN, GxEPD_BLACK);
        }
        x += 2 * CIRCLES_RADIUS + CIRCLES_MARGIN;
    }
    this->currentCourser = x;
}

#define PADDING_DAYS_LEFT 8
#define FONT_HEIGHT() (this->u8g2.getFontAscent() - this->u8g2.getFontDescent())
#define HCENTER(containerw, str) \
    ((containerw - this->u8g2.getUTF8Width(str.c_str())) / 2)
#define VCENTER(containerh, str) ((containerh + (FONT_HEIGHT())) / 2)
#define BATTERY_GRAPHICS_DEFAULT_WIDTH 25
#define BATTERY_DEFAULT_Y (PADDING_DAYS_LEFT - 1)
#define BATTERY_DEFAULT_X (this->display.width() - BATTERY_GRAPHICS_DEFAULT_WIDTH)

int PageManager::getBatteryPercentage() {
    logm("Checking bat percentage");
    uint32_t percentage = 100;
    const static int table[11] = {
        3000, 3650, 3700, 3740, 3760, 3795,
        3840, 3910, 3980, 4070, 4150};
    uint16_t v1 = analogRead(BAT_TEST);
    int voltage = ((float)analogRead(35) / 4200.0) * 7.05 * 1000;

    for (int i = 0; i < 11; i++) {
        if (voltage < table[i]) {
            percentage = i * 10 - (10UL * (int)(table[i] - voltage)) /
                                      (int)(table[i] - table[i - 1]);
            break;
        }
    }
    Serial.print("Battery percentage: ");
    logm(percentage);

    return percentage;
}

void PageManager::printDaysLeft(int daysLeftNum) {
    this->u8g2.setFont(u8g2_font_9x15_mr);
    String daysLeft =
        String(daysLeftNum) + (daysLeftNum == 1 ? " Day left" : " Days left");
    // daysLeft += " | " + String(this->getBatteryPercentage()) + "%";
    const int strWidth = this->u8g2.getUTF8Width(daysLeft.c_str());
    const int rectX = (display.width() - strWidth - PADDING_DAYS_LEFT) / 2;
    const int rectHeight = FONT_HEIGHT() + PADDING_DAYS_LEFT;
    this->u8g2.setCursor(HCENTER(display.width(), daysLeft),
                         0 + FONT_HEIGHT() + (PADDING_DAYS_LEFT / 2) - 2);
    this->u8g2.print(daysLeft);
    // display.drawRect(rectX, 0, strWidth + PADDING_DAYS_LEFT,
    //                  rectHeight, GxEPD_BLACK);
}

void PageManager::drawBattery(int x, int y, double scale) {
    logm("Drawing battery");
    uint8_t percentage = this->getBatteryPercentage();
    const int mainBattWidth = 40 * scale;
    const int tipOffsetX = mainBattWidth;
    const int tipOffsetY = 6 * scale;
    const int tipWidth = 4 * scale;
    const int tipHeight = 7 * scale;
    const int graphicsWidth = tipOffsetX + tipWidth;
    const int graphicsHeight = 15s * scale;
    const int chargeMargin = 1;
    if (x == -1) {
        // Align screen center
        x = display.width() / 2 - graphicsWidth / 2;
    }
    // Battery main
    gb.drawRect(x, y,
                mainBattWidth, graphicsHeight,
                Black);
    // Battery tip fill
    gb.fillRect(x + tipOffsetX, y + tipOffsetY,
                tipHeight, tipWidth,
                Black);
    // Battery main fill
    gb.fillRect(x + chargeMargin, y + chargeMargin,
                mainBattWidth * percentage / 100.0 - 2 * chargeMargin, graphicsHeight - 2 * chargeMargin,
                Black);
}

void PageManager::drawWarningTriangle(int x, int y) {
    this->u8g2.setFont(u8g2_font_7x13_tr);
    int warningHeight = 14;
    display.fillTriangle(x, y - 2, x - 10,
                         y + warningHeight, x + 10,
                         y + warningHeight, GxEPD_WHITE);
    display.fillTriangle(x, y, x - 8,
                         y + warningHeight, x + 8,
                         y + warningHeight, GxEPD_BLACK);
    this->u8g2.setCursor(x - 3, y + warningHeight);
    this->u8g2.setForegroundColor(GxEPD_WHITE);
    this->u8g2.setBackgroundColor(GxEPD_BLACK);
    this->u8g2.print("!");
    this->u8g2.setForegroundColor(GxEPD_BLACK);
    this->u8g2.setBackgroundColor(GxEPD_WHITE);
}

void PageManager::printTotalSum(int totalSum) {
    this->u8g2.setFont(u8g2_font_inr30_mr);
    this->u8g2.setCursor(HCENTER(display.width(), String(totalSum)),
                         VCENTER(display.height(), String(totalSum)) - 5);
    this->u8g2.print((std::to_string(totalSum)).c_str());
}

void PageManager::printProgressAndGoal(int totalSum, int monthlyGoal) {
    if (monthlyGoal == 0) {
        monthlyGoal = 1;
    }

    int barWidth = 0.95 * display.width();
    int barX = (display.width() - barWidth) / 2;
    int barY = display.height() - 23;
    const int barSize = 3;
    const int indicatorRadius = 7;
    const int goalIndicatorHeight = 10;
    const double goalPositionPercent = 0.7;
    const int indicatorOffsetYpx = 3;

    display.fillRect(barX, barY, barWidth, barSize, GxEPD_BLACK);
    double progressPercent =
        ((totalSum / (double)monthlyGoal) * goalPositionPercent);
    int progIndicatorX = barX + min((int)(progressPercent * barWidth), barWidth);
    int progIndicatorY = barY + (barSize / 2);
    int goalX = barX + (barWidth * goalPositionPercent);
    int goalY = barY - (goalIndicatorHeight / 2) - (barSize / 2) + indicatorOffsetYpx;
    display.fillRect(goalX, goalY, 2, goalIndicatorHeight + barSize, GxEPD_BLACK);
    // Start bar indicator
    display.fillRect(barX, barY, 2, goalIndicatorHeight + barSize, GxEPD_BLACK);
    // End bar indicator
    display.fillRect(barX + barWidth - barSize / 2, barY, 2, goalIndicatorHeight + barSize, GxEPD_BLACK);

    if (progressPercent <= goalPositionPercent) {
        this->u8g2.setFont(u8g2_font_8x13B_tr);
        display.fillCircle(progIndicatorX, progIndicatorY, indicatorRadius + 2,
                           GxEPD_WHITE);
        display.fillCircle(progIndicatorX, progIndicatorY, indicatorRadius,
                           GxEPD_BLACK);
        this->u8g2.setCursor(progIndicatorX + 1 - indicatorRadius +
                                 HCENTER(indicatorRadius * 2, String("$")),
                             progIndicatorY - 1 - indicatorRadius +
                                 VCENTER(indicatorRadius * 2, String("$")));
        this->u8g2.setForegroundColor(GxEPD_WHITE);
        this->u8g2.setBackgroundColor(GxEPD_BLACK);
        this->u8g2.print("$");
        this->u8g2.setForegroundColor(GxEPD_BLACK);
        this->u8g2.setBackgroundColor(GxEPD_WHITE);

    } else {
        this->drawWarningTriangle(progIndicatorX, barY - 8);
        // this->u8g2.setFont(u8g2_font_7x13_tr);
        // int warningY = barY - 8;
        // int warningHeight = 11 + barSize;
        // display.fillTriangle(progIndicatorX, warningY - 2, progIndicatorX - 10,
        //                      warningY + warningHeight, progIndicatorX + 10,
        //                      warningY + warningHeight, GxEPD_WHITE);
        // display.fillTriangle(progIndicatorX, warningY, progIndicatorX - 8,
        //                      warningY + warningHeight, progIndicatorX + 8,
        //                      warningY + warningHeight, GxEPD_BLACK);
        // this->u8g2.setCursor(progIndicatorX - 3, warningY + warningHeight);
        // this->u8g2.setForegroundColor(GxEPD_WHITE);
        // this->u8g2.setBackgroundColor(GxEPD_BLACK);
        // this->u8g2.print("!");
        // this->u8g2.setForegroundColor(GxEPD_BLACK);
        // this->u8g2.setBackgroundColor(GxEPD_WHITE);
    }

    this->u8g2.setFont(u8g2_font_6x10_mr);
    String monthlyGoalStr = "Budget: " + String(monthlyGoal);
    this->u8g2.setCursor(goalX - (this->u8g2.getUTF8Width(monthlyGoalStr.c_str()) / 2) + 1,
                         goalY + goalIndicatorHeight + barSize + FONT_HEIGHT());
    this->u8g2.print(monthlyGoalStr.c_str());
}

void PageManager::printCardSpending(const cardsSpending &cardMap) {
    this->u8g2.setFont(u8g2_font_9x15_tr);
    this->u8g2.setCursor(HCENTER(display.width(), String("Credit Cards")), 15);
    this->u8g2.print("Credit Cards");
    this->u8g2.setFont(u8g2_font_10x20_tr);
    int y = 42;
    for (const auto &it : cardMap) {
        this->u8g2.setCursor(3, y);
        this->u8g2.println((it.first + ":").c_str());
        this->u8g2.setCursor(180, y);
        this->u8g2.println(std::to_string(it.second).c_str());
        y += 25;
    }
}
void resetDisplay(GxDEPG0213BN &display) {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setBackgroundColor(GxEPD_WHITE);
}

void PageManager::showSumPage(int totalSum, int daysLeft, int monthlyGoal) {
    logm("Printing Total Sum page");
    resetDisplay(display);
    printPageMenu(0, 3);
    printDaysLeft(daysLeft);
    printTotalSum(totalSum);
    printProgressAndGoal(totalSum, monthlyGoal);
    this->drawBattery(BATTERY_DEFAULT_X, BATTERY_DEFAULT_Y);
    display.update();
}

void PageManager::showCardSpendingPage(const cardsSpending &cardMap) {
    resetDisplay(display);
    printPageMenu(1, 3);
    printCardSpending(cardMap);
    this->drawBattery(BATTERY_DEFAULT_X, BATTERY_DEFAULT_Y);
    display.update();
}

void PageManager::showPassKey(uint32_t pass_key) {
    showTitle(String(pass_key), String("Enter passkey:"));
}

void PageManager::showTitle(String title, String subtitle, int delayAfter) {
    resetDisplay(display);
    this->u8g2.setFont(u8g2_font_9x15_tr);
    this->u8g2.setCursor(HCENTER(display.width(), subtitle), 20);
    this->u8g2.print(subtitle.c_str());

    this->u8g2.setFont(u8g2_font_fub20_tr);
    this->u8g2.setCursor(HCENTER(display.width(), title),
                         VCENTER(display.height(), title));
    this->u8g2.print(title.c_str());
    this->drawBattery(-1, display.height() - 30, 1.2);
    display.update();

    delay(delayAfter);
}

#define MAX_DATA_POINTS 31
void PageManager::showGraphPage(String cycleStartDate, String cycleEndDate,
                                int daysInCycle, JsonArray dataPoints) {
    logm("Printing Monthly Spending Graph page");
    resetDisplay(display);
    printPageMenu(2, 3);
    float dataPointsArr[MAX_DATA_POINTS];
    copyArray(dataPoints, dataPointsArr);

    gb.DrawGraph(30, 24, 215, 80, -1, -1, "Monthly Spending", dataPointsArr,
                 dataPoints.size(), daysInCycle, cycleStartDate, cycleEndDate,
                 true, true);
    this->drawBattery(BATTERY_DEFAULT_X, BATTERY_DEFAULT_Y);
    display.update();
}
