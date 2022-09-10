#include <U8g2_for_Adafruit_GFX.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h>

void reverseheb(char* string);
void blockUntilPress();

enum alignment { LEFT, RIGHT, CENTER };
#define Black GxEPD_BLACK
#define LightGrey GxEPD_BLACK
#define Grey GxEPD_BLACK
class GraphBuilder {
 public:
  GraphBuilder(GxEPD_Class& display_, U8G2_FOR_ADAFRUIT_GFX& u8g2_);

/* (C) D L BIRD
    This function will draw a graph on a ePaper/TFT/LCD display using data from an array containing data to be graphed.
    The variable 'max_readings' determines the maximum number of data elements for each array. Call it with the following parametric data:
    x_pos-the x axis top-left position of the graph
    y_pos-the y-axis top-left position of the graph, e.g. 100, 200 would draw the graph 100 pixels along and 200 pixels down from the top-left of the screen
    width-the width of the graph in pixels
    height-height of the graph in pixels
    Y1_Max-sets the scale of plotted data, for example 5000 would scale all data to a Y-axis of 5000 maximum
    data_array1 is parsed by value, externally they can be called anything else, e.g. within the routine it is called data_array1, but externally could be temperature_readings
    auto_scale-a logical value (TRUE or FALSE) that switches the Y-axis autoscale On or Off
    barchart_on-a logical value (TRUE or FALSE) that switches the drawing mode between barhcart and line graph
    barchart_colour-a sets the title and graph plotting colour
    If called with Y!_Max value of 500 and the data never goes above 500, then autoscale will retain a 0-500 Y scale, if on, the scale increases/decreases to match the data.
    auto_scale_margin, e.g. if set to 1000 then autoscale increments the scale by 1000 steps.
*/
  void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min,
                 float Y1Max, String title, float DataArray[], int readings,
                 boolean auto_scale, boolean barchart_mode);

 private:
  GxEPD_Class& display;
  U8G2_FOR_ADAFRUIT_GFX& u8g2;
  void fillCircle(int x, int y, int r, uint16_t color);

  void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color);

  void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color);

  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

  void drawCircle(int x0, int y0, int r, uint8_t color);

  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                    int16_t y2, uint16_t color);

  void drawPixel(int x, int y, uint8_t color);

  void setFont(const uint8_t* font);

  void edp_update();


  void drawString(int x, int y, String text, alignment align);

};