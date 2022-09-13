#include "utils.h"

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "firesans.h"

void reverseheb(char *string) {
  int i, j;
  char *temp;
  //
  j = strlen(string);
  temp = (char *)malloc(j + 1);
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

void blockUntilPress() {
  int last = 1;
  int now = digitalRead(GPIO_NUM_39);
  while (now == 1) {
    last = now;
    delay(1);
    now = digitalRead(GPIO_NUM_39);
  }
}

const GFXfont *currentFont = &FiraSans;

void initEpd(uint8_t *framebuffer) {
  delay(1500);
  // framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2,
  // MALLOC_CAP_SPIRAM);
  // if (!framebuffer) Serial.println("Memory alloc failed!");
  // memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

void drawString(int x, int y, String text, alignment align, const GFXfont *font,
                uint8_t *framebuffer) {
  drawString(x, y, text, align, CENTER, font, framebuffer);
}

void drawString(int x, int y, String text, alignment align,
                uint8_t *framebuffer) {
  drawString(x, y, text, align, currentFont, framebuffer);
}

void drawString(int x, int y, String text, alignment align, alignment alignv,
                const GFXfont *font, uint8_t *framebuffer) {
  if (!font) {
    font = currentFont;
  }
  char *data = const_cast<char *>(text.c_str());
  int x1, y1;  // the bounds of x,y and w and h of the variable 'text' in
               // pixels.
  int w, h;
  int xx = EPD_WIDTH / 2, yy = EPD_HEIGHT / 2;
  get_text_bounds(font, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
  if (align == RIGHT) x = x - w;
  if (align == CENTER) x = x - w / 2;

  int cursor_y = y;
  if (alignv == CENTER) {
    cursor_y = y + (h / 2);
  } else if (alignv == BOTTOM) {
    cursor_y = y + h;
  }

  write_string(font, data, &x, &cursor_y, framebuffer);
}

int getTextWidth(String text, const GFXfont *font) {
  int x1, y1;
  int w, h;
  int xx = EPD_WIDTH / 2, yy = EPD_HEIGHT / 2;
  get_text_bounds(font, text.c_str(), &xx, &yy, &x1, &y1, &w, &h, NULL);
  return w;
}

int getTextHeight(String text, const GFXfont *font) {
  int x1, y1;
  int w, h;
  int xx = EPD_WIDTH / 2, yy = EPD_HEIGHT / 2;
  get_text_bounds(font, text.c_str(), &xx, &yy, &x1, &y1, &w, &h, NULL);
  return h;
}

void fillCircle(int x, int y, int r, uint8_t color, uint8_t *framebuffer) {
  epd_fill_circle(x, y, r, color, framebuffer);
}

void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color,
                   uint8_t *framebuffer) {
  epd_draw_hline(x0, y0, length, color, framebuffer);
}

void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color,
                   uint8_t *framebuffer) {
  epd_draw_vline(x0, y0, length, color, framebuffer);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color,
              uint8_t *framebuffer) {
  epd_write_line(x0, y0, x1, y1, color, framebuffer);
}

void drawCircle(int x0, int y0, int r, uint8_t color, uint8_t *framebuffer) {
  epd_draw_circle(x0, y0, r, color, framebuffer);
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color,
              uint8_t *framebuffer) {
  epd_draw_rect(x, y, w, h, color, framebuffer);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color,
              uint8_t *framebuffer) {
  epd_fill_rect(x, y, w, h, color, framebuffer);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                  int16_t y2, uint16_t color, uint8_t *framebuffer) {
  epd_fill_triangle(x0, y0, x1, y1, x2, y2, color, framebuffer);
}

void drawPixel(int x, int y, uint8_t color, uint8_t *framebuffer) {
  epd_draw_pixel(x, y, color, framebuffer);
}

// void setFont(const GFXfont *font) { currentFont = font; }

void epd_update(uint8_t *framebuffer) {
  epd_draw_grayscale_image(epd_full_screen(),
                           framebuffer);  // Update the screen
}

void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, int Y1Min,
               int Y1Max, String title, float DataArray[], int readings,
               int max_data_points, String x_start_title, String x_end_title,
               boolean auto_scale, boolean barchart_mode,
               uint8_t *framebuffer) {
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
  Y1Min = Y1Min - ((int)Y1Min % 200);
  Y1Max = Y1Max + (200 - ((int)Y1Max % 200));

  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[0], Y1Min, Y1Max)) /
                       (Y1Max - Y1Min) * gheight;
  drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, Grey, framebuffer);
  drawString(x_pos - 20 + gwidth / 2, y_pos - 18, title, CENTER, &FiraSans,
             framebuffer);
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
      fillRect(last_x + 2, y2, (gwidth / max_data_points) - 1,
               y_pos + gheight - y2 + 2, Black, framebuffer);
    } else {
      drawLine(last_x, last_y - 1, x2, y2 - 1, Black,
               framebuffer);  // Two lines for hi-res display
      drawLine(last_x, last_y, x2, y2, Black, framebuffer);
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
                      gwidth / (2 * number_of_dashes), Grey, framebuffer);
    }

    drawString(
        x_pos - 3, y_pos + ((gheight * spacing) / y_minor_axis) - 3,
        String((int)(Y1Max - (Y1Max - Y1Min) / y_minor_axis * spacing + 0.01)),
        RIGHT, framebuffer);
  }

  drawString(x_pos, y_pos + gheight + 4, x_start_title, LEFT, framebuffer);
  drawString(x_pos + gwidth, y_pos + gheight + 4, x_end_title, RIGHT,
             framebuffer);

  for (int i = 0; i < 3; i++) {
    if (i < 2)
      drawFastVLine(x_pos + gwidth / 3 * i + gwidth / 3, y_pos, gheight,
                    LightGrey, framebuffer);
  }
}