#include "utils.h"

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

void reverseheb(char* string) {
  int i, j;
  char* temp;
  //
  j = strlen(string);
  temp = (char*)malloc(j + 1);
  temp[j] = 0;
  for (i = 0; string[i]; i++) {
    if (string[i] == (char)0xD7) {
      temp[j - i - 1] = string[i + 1];
      temp[j - i - 2] = (char)0xD7;
      i++;
    } else {
      if ((string[i] < '0' || string[i] > '9') &&
          (string[i] < 'A' || string[i] > 'Z') &&
          (string[i] < 'a' || string[i] > 'z'))
        temp[j - i - 1] = string[i];
      else {
        int m, k;
        k = i;
        while ((string[i] >= '0' && string[i] <= '9') ||
               (string[i] >= 'A' && string[i] <= 'Z') ||
               (string[i] >= 'a' && string[i] <= 'z')) {
          i++;
        }
        i--;
        for (m = 0; m < (i - k + 1); m++) {
          temp[j - (k + m) - 1] = string[i - m];
        }
      }
    }
  }
  strcpy(string, temp);
  free(temp);
}

boolean blockUntilPress(unsigned long startTime) {
  int next_button = digitalRead(NEXT_BUTTON);
  int hide_button = digitalRead(HIDE_SHOW_BUTTON);
  while (hide_button == 0 && next_button == 0) {
    if (millis() - startTime > LISTEN_FOR_CLICKS_TIMEOUT) {
      return false;
    }
    delay(1);
    next_button = digitalRead(NEXT_BUTTON);
    hide_button = digitalRead(HIDE_SHOW_BUTTON);
  }
  return true;
}

int listenForButtonClick(int timeout) {
  int next_button = 0;
  int hide_button = 0;
  int startTime = millis();
  while (millis() - startTime < timeout) {
    delay(1);
    next_button = digitalRead(NEXT_BUTTON);
    hide_button = digitalRead(HIDE_SHOW_BUTTON);

    if (next_button) {
      return NEXT_BUTTON;
    } else if (hide_button) {
      return HIDE_SHOW_BUTTON;
    }
  }

  return BUTTON_CLICK_TIMEOUT;
}

GraphBuilder::GraphBuilder(GxEPD_Class& display_, U8G2_FOR_ADAFRUIT_GFX& u8g2_)
    : display(display_), u8g2(u8g2_) {}

void GraphBuilder::drawString(int x, int y, String text, alignment align,
                              const uint8_t font[] = u8g2_font_6x10_tr) {
  int16_t x1,
      y1;  // the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT) x = x - w;
  if (align == CENTER) x = x - w / 2;
  int cursor_y = y + h;
  u8g2.setCursor(x, cursor_y);
  u8g2.setFont(font);
  u8g2.print(text.c_str());
}

void GraphBuilder::fillCircle(int x, int y, int r, uint16_t color) {
  display.fillCircle(x, y, r, color);
}

void GraphBuilder::drawFastHLine(int16_t x0, int16_t y0, int length,
                                 uint16_t color) {
  display.drawLine(x0, y0, x0 + length, y0, color);
}

void GraphBuilder::drawFastVLine(int16_t x0, int16_t y0, int length,
                                 uint16_t color) {
  display.drawLine(x0, y0, x0, y0 + length, color);
}

void GraphBuilder::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                            uint16_t color) {
  display.drawLine(x0, y0, x1, y1, color);
}

void GraphBuilder::drawCircle(int x0, int y0, int r, uint8_t color) {
  display.drawCircle(x0, y0, r, color);
}

void GraphBuilder::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
                            uint16_t color) {
  display.drawRect(x, y, w, h, color);
}

void GraphBuilder::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                            uint16_t color) {
  display.fillRect(x, y, w, h, color);
}

void GraphBuilder::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color) {
  display.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void GraphBuilder::drawPixel(int x, int y, uint8_t color) {
  display.drawPixel(x, y, color);
}

void GraphBuilder::setFont(const uint8_t* font) { u8g2.setFont(font); }

void GraphBuilder::edp_update() { display.update(); }

void GraphBuilder::DrawGraph(int x_pos, int y_pos, int gwidth, int gheight,
                             int Y1Min, int Y1Max, String title,
                             float DataArray[], int readings,
                             int max_data_points, String x_start_title,
                             String x_end_title, boolean auto_scale,
                             boolean barchart_mode) {
#define auto_scale_margin \
  0  // Sets the autoscale increment, so axis steps up fter a change of e.g. 3
#define y_minor_axis 5  // 5 y-axis division markers
  int maxYscale = -10000;
  int minYscale = 10000;
  int last_x, last_y;
  float x2, y2;
  if (auto_scale == true) {
    for (int i = 0; i < readings; i++) {
      if (DataArray[i] >= maxYscale) maxYscale = DataArray[i];
      if (DataArray[i] <= minYscale) minYscale = DataArray[i];
    }
    // Auto scale the graph and round to the
    // nearest value defined, default was Y1Max
    maxYscale = round(maxYscale + auto_scale_margin);
    Y1Max = round(maxYscale);
    // Auto scale the graph and round to the
    // nearest value defined, default was Y1Min
    if (minYscale != 0) {
      minYscale = round(minYscale - auto_scale_margin);
    }
    Y1Min = round(minYscale);
  }
  Y1Min = barchart_mode ? 0 : Y1Min - ((int)Y1Min % 200);
  Y1Max = Y1Max + (100 - ((int)Y1Max % 100)) + 100;

  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[0], Y1Min, Y1Max)) /
                       (Y1Max - Y1Min) * gheight;
  drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, Grey);
  drawString(x_pos - 20 + gwidth / 2, y_pos - 18, title, CENTER,
             u8g2_font_9x15_tr);
  for (int gx = 0; gx < readings; gx++) {
    // x2 = x_pos + gx * gwidth / (readings - 1) -
    x2 = x_pos + gx * gwidth / (max_data_points - 1) -
         1;  // max_readings is the global variable that sets the maximum data
             // that can be plotted
    y2 = y_pos +
         (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) *
             gheight +
         1;
    if (barchart_mode) {
      fillRect(x2 + 2, y2, (gwidth / max_data_points) - 1,
               y_pos + gheight - y2 + 2, Black);
    } else {
      drawLine(last_x, last_y - 1, x2, y2 - 1,
               Black);  // Two lines for hi-res display
      drawLine(last_x, last_y, x2, y2, Black);
    }
    last_x = x2;
    last_y = y2;
  }

  // Draw the Y-axis scale
#define number_of_dashes 20
  for (int spacing = 0; spacing <= y_minor_axis; spacing++) {
    for (int j = 0; j < number_of_dashes;
         j++) {  // Draw dashed graph grid lines
      if (spacing < y_minor_axis)
        drawFastHLine((x_pos + 3 + j * gwidth / number_of_dashes),
                      y_pos + (gheight * spacing / y_minor_axis),
                      gwidth / (2 * number_of_dashes), Grey);
    }

    drawString(
        x_pos - 3, y_pos + ((gheight * spacing) / y_minor_axis) - 3,
        String((int)(Y1Max - (Y1Max - Y1Min) / y_minor_axis * spacing + 0.01)),
        RIGHT);
  }

  drawString(x_pos, y_pos + gheight + 4, x_start_title, LEFT);
  drawString(x_pos + gwidth, y_pos + gheight + 4, x_end_title, RIGHT);

  for (int i = 0; i < 3; i++) {
    if (i < 2)
      drawFastVLine(x_pos + gwidth / 3 * i + gwidth / 3, y_pos, gheight,
                    LightGrey);
  }
}
