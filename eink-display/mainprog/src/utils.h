#ifndef UTILS_H_
#define UTILS_H_

#include <Arduino.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <epd_driver.h>

#define LISTEN_FOR_CLICKS_TIMEOUT 15000
#define HIDE_SHOW_BUTTON GPIO_NUM_34
#define NEXT_BUTTON GPIO_NUM_39
#define BUTTON_CLICK_TIMEOUT -1

#define White 0xFF
#define LightGrey 0xBB
#define Grey 0x88
#define DarkGrey 0x44
#define Black 0x00

enum alignment { LEFT, RIGHT, CENTER, TOP, BOTTOM };
void reverseheb(char* string);
boolean blockUntilPress(unsigned long startTime);
int listenForButtonClick(int timeout);

/* (C) D L BIRD
    This function will draw a graph on a ePaper/TFT/LCD display using data
   from an array containing data to be graphed. The variable 'max_readings'
   determines the maximum number of data elements for each array. Call it with
   the following parametric data: x_pos-the x axis top-left position of the
   graph y_pos-the y-axis top-left position of the graph, e.g. 100, 200 would
   draw the graph 100 pixels along and 200 pixels down from the top-left of
   the screen width-the width of the graph in pixels height-height of the
   graph in pixels Y1_Max-sets the scale of plotted data, for example 5000
   would scale all data to a Y-axis of 5000 maximum data_array1 is parsed by
   value, externally they can be called anything else, e.g. within the routine
   it is called data_array1, but externally could be temperature_readings
    auto_scale-a logical value (TRUE or FALSE) that switches the Y-axis
   autoscale On or Off barchart_on-a logical value (TRUE or FALSE) that
   switches the drawing mode between barhcart and line graph barchart_colour-a
   sets the title and graph plotting colour If called with Y!_Max value of 500
   and the data never goes above 500, then autoscale will retain a 0-500 Y
   scale, if on, the scale increases/decreases to match the data.
    auto_scale_margin, e.g. if set to 1000 then autoscale increments the scale
   by 1000 steps.
*/
void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, int Y1Min,
               int Y1Max, String title, float DataArray[], int readings,
               int max_data_points, String x_start_title, String x_end_title,
               boolean auto_scale, boolean barchart_mode, uint8_t *framebuffer);
void fillCircle(int x, int y, int r, uint8_t color, uint8_t *framebuffer);

void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color,
                   uint8_t *framebuffer);

void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color,
                   uint8_t *framebuffer);

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color,
              uint8_t *framebuffer);

void drawCircle(int x0, int y0, int r, uint8_t color, uint8_t *framebuffer);

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color,
              uint8_t *framebuffer);

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color,
              uint8_t *framebuffer);

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                  int16_t y2, uint16_t color, uint8_t *framebuffer);

void drawPixel(int x, int y, uint8_t color, uint8_t *framebuffer);

// void setFont(const uint8_t* font, uint8_t *framebuffer);

void epd_update(uint8_t *framebuffer);

int getTextWidth(String text, const GFXfont *font);

int getTextHeight(String text, const GFXfont *font);

void drawString(int x, int y, String text, alignment align, const GFXfont *font,
                uint8_t *framebuffer);

void drawString(int x, int y, String text, alignment align,
                uint8_t *framebuffer);

void drawString(int x, int y, String text, alignment align, alignment alignv,
                const GFXfont *font, uint8_t *framebuffer);

void initEpd(uint8_t *framebuffer);

static void remove_all_bonded_devices() {
  int dev_num = esp_ble_get_bond_device_num();

  esp_ble_bond_dev_t *dev_list =
      (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
  esp_ble_get_bond_device_list(&dev_num, dev_list);
  for (int i = 0; i < dev_num; i++) {
    esp_ble_remove_bond_device(dev_list[i].bd_addr);
  }

  free(dev_list);
}

#endif
