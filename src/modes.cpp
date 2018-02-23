#include "modes.h"

void displayFStop(Adafruit_SSD1306 display, int x, int y, uint16_t fstop, int* x1, int* y1) {
  // f/1.1 or f/11
  char text[6];
	
  if(fstop < 100) {
    sprintf(text, "f/%i.%i", fstop / 10, fstop % 10);
  } else {
    sprintf(text, "f/%i", fstop / 10);
  }
	
  int16_t _x1, _y1;
  uint16_t w, h;
  display.getTextBounds(text, x, y, &_x1, &_y1, &w, &h);  

  *x1 = _x1 + w;
  *y1 = _y1 + h;
  
  display.setCursor(_x1, _y1);
  display.print(text);
}

void displayExposure(Adafruit_SSD1306 display, int x, int y, int32_t exposure, int* x1, int* y1) {
  // 1/99999 or 30" or 1.0"
  char text[8];
	
  if(exposure <= -100) {
    sprintf(text, "%i\"", -exposure / 10);
  } else if(exposure <= 0) {
    sprintf(text, "%i.%i\"", -exposure / 10, -exposure % 10);
  } else {
    sprintf(text, "1/%i", exposure);
  }
	
  int16_t _x1, _y1;
  uint16_t w, h;
  display.getTextBounds(text, x, y, &_x1, &_y1, &w, &h);  

  *x1 = _x1 + w;
  *y1 = _y1 + h;
  
  display.setCursor(_x1, _y1);
  display.print(text);
}

void displayISO(Adafruit_SSD1306 display, int x, int y, uint16_t iso, int* x1, int* y1) {
  // ISO9999
  char text[8];
	
  sprintf(text, "ISO%i", iso);
  	
  int16_t _x1, _y1;
  uint16_t w, h;
  display.getTextBounds(text, x, y, &_x1, &_y1, &w, &h);  

  *x1 = _x1 + w;
  *y1 = _y1 + h;
  
  display.setCursor(_x1, _y1);
  display.print(text);
}

// ISO
ISOMode::ISOMode (uint16_t fstop, int32_t exposure) {
  this->fstop = fstop;
  this->exposure = exposure;
}

void ISOMode::displayHeader(Adafruit_SSD1306 display, int x, int y) {
  char * modeText = "I";

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(modeText, 0, 0, &x1, &y1, &w, &h);  
  
  int offset = x + 2;
	
  display.setCursor(offset, y + 1);
  display.print(modeText);
    
  offset += w;
     
  offset += 1;
  display.drawLine(offset, y, offset, y + 8, WHITE);
  offset += 2;
  
  int fstopY;
  displayFStop(display, offset, y + 1, this->fstop, &offset, &fstopY);
  
  offset += 1;
  display.drawLine(offset, y, offset, y + 8, WHITE);
  offset += 2;
  
  int exposureY;
  displayExposure(display, offset, y + 1, this->exposure, &offset, &exposureY);
  
  display.display();
  
  // TODO main display
  display.setTextSize(1);
  display.setCursor(5, 20);
  display.print("ISO");

  display.setTextSize(4);
  display.setCursor(25, 20);
  display.print("1600");
  display.display();
}

// Exposure
ExposureMode::ExposureMode (uint16_t fstop, uint16_t iso) {
  this->fstop = fstop;
  this->iso = iso;
}

void ExposureMode::displayHeader(Adafruit_SSD1306 display, int x, int y) {
  char * modeText = "S";

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(modeText, 0, 0, &x1, &y1, &w, &h);  
  
  int offset = x + 2;
	
  display.setCursor(offset, y + 1);
  display.print(modeText);
    
  offset += w;
  
  offset += 1;
  display.drawLine(offset, y, offset, y + 8, WHITE);
  offset += 2;
  
  int isoY;
  displayISO(display, offset, y + 1, this->iso, &offset, &isoY);
  
  offset += 1;
  display.drawLine(offset, y, offset, y + 8, WHITE);
  offset += 2;
  
  int fstopY;
  displayFStop(display, offset, y + 1, this->fstop, &offset, &fstopY);
    
  display.display();
  
  // TODO main display
  display.setTextSize(2);
  display.setCursor(5, 20);
  display.print("1/"); 

  display.setTextSize(4);
  display.setCursor(30, 20);
  display.print("1000");
  display.display();
}

// FStop
FStopMode::FStopMode (int32_t exposure, uint16_t iso) {
  this->exposure = exposure;
  this->iso = iso;
}

void FStopMode::displayHeader(Adafruit_SSD1306 display, int x, int y) {
  char * modeText = "f";

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(modeText, 0, 0, &x1, &y1, &w, &h);  
  
  int offset = x + 2;
	
  display.setCursor(offset, y + 1);
  display.print(modeText);
    
  offset += w;
  
  offset += 1;
  display.drawLine(offset, y, offset, y + 8, WHITE);
  offset += 2;
  
  int isoY;
  displayISO(display, offset, y + 1, this->iso, &offset, &isoY);
  
  offset += 1;
  display.drawLine(offset, y, offset, y + 8, WHITE);
  offset += 2;
  
  int fstopY;
  displayExposure(display, offset, y + 1, this->exposure, &offset, &fstopY);
    
  display.display();
  
  // TODO main display
  display.setTextSize(2);
  display.setCursor(5, 20);
  display.print("f/");
  
  display.setTextSize(4);
  display.setCursor(30, 20);
  display.print("8.0");
  display.display();
}
