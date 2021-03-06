#include <SPI.h>
#include <Wire.h>
#include "src/Adafruit_GFX/Adafruit_GFX.h"
#include "src/Adafruit_SSD1306/Adafruit_SSD1306.h"
#include "src/modes.h"
#include "src/sensors_mux.h"
#include "src/sensors_lux.h"
#include "src/sensors_rgb.h"

// If using software SPI (the default case):
#define OLED_MOSI   3
#define OLED_CLK    4
#define OLED_DC     5
#define OLED_CS     2
#define OLED_RESET  1
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

SensorMux mux;
SensorLux lux(&mux, 0);
SensorRGB rgb(&mux, 1);

/*
 * ISO values are encoded as: X is ISO X
 * Exposure values are encoded as: +X is 1/X, -X is (X/10)"
 * FStop values are encoded as: X is f/(X/10)
 */

void setup() {
  Serial.begin(9600);

  Wire.begin();
  mux.begin();
  lux.begin();
  rgb.begin();

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  displayHeader();
  displayFooter();

  Mode* mode = new ISOMode(220, 6400);
  mode->displayHeader(display, 0,0);
}

void loop() {
  displayFooter();
  delay(100);
}

#define HEADER_HEIGHT 10
void displayHeader() {  
  display.drawLine(0, HEADER_HEIGHT-1, display.width()-1, HEADER_HEIGHT-1, WHITE);

  drawBatteryText(114, 1, 95);
  drawBattery(115, 1, 95);

  display.display();
}

void floatToString(char *buffer, float f, uint8_t whole_len, uint8_t frac_len) {
  int32_t whole = (int32_t)f;
  uint32_t frac = (uint32_t)((f - whole) * pow(10, frac_len));

  sprintf(buffer, "%i.%i", whole, frac);
}

#define FOOTER_HEIGHT 10
#define FOOTER_TOP (SSD1306_LCDHEIGHT - FOOTER_HEIGHT)
void displayFooter() {
  display.fillRect(0, FOOTER_TOP, 127, FOOTER_TOP+FOOTER_HEIGHT-1, BLACK);


  ReadingLux reading1 = lux.read();
  ReadingRGB reading2 = rgb.read();
  // text = 1234K or 12,345,678lx
  char text[26];
  
  display.drawLine(0, FOOTER_TOP-1, display.width()-1, FOOTER_TOP-1, WHITE);

  floatToString(text, reading2.temp, 10, 3);

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);  
  
  int offset = 2;
  
  display.setCursor(offset, FOOTER_TOP + 2);
  display.print(text);
    
  offset += w;
     
  offset += 1;
  display.drawLine(offset, FOOTER_TOP + 1, offset, FOOTER_TOP + FOOTER_HEIGHT - 1, WHITE);
  offset += 2;

  floatToString(text, reading1.lux, 10, 3);

  display.setCursor(offset, FOOTER_TOP + 2); 
  display.print(text);
  
  display.display();
}

// drawn with (x,y) at top-right
void drawBatteryText(int x, int y, int percent) {
  // 3 numbers + % + null
  char text [5];
  sprintf(text, "%i%%", percent);

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  display.setCursor(x-w,y);
  display.print(text);
}

// drawn with (x,y) at top-left
void drawBattery(int x, int y, int percent) { 
  display.drawLine(x + 1, y, x + 10, y, WHITE);
  display.drawLine(x + 1, y + 6, x + 10, y + 6, WHITE);
  display.drawLine(x + 11, y, x + 11, y + 6, WHITE);
  display.drawLine(x + 1, y + 1, x + 1, y + 5, WHITE);
  display.drawLine(x, y + 2, x, y + 4, WHITE);
  
  if(percent >= 20) { display.drawLine(x + 9, y + 2, x + 9, y + 4, WHITE); }
  if(percent >= 40) { display.drawLine(x + 7, y + 2, x + 7, y + 4, WHITE); }
  if(percent >= 60) { display.drawLine(x + 5, y + 2, x + 5, y + 4, WHITE); }
  if(percent >= 80) { display.drawLine(x + 3, y + 2, x + 3, y + 4, WHITE); }
}

