#ifndef modes_h
#define modes_h

#include "Arduino.h"
#include "Adafruit_GFX/Adafruit_GFX.h"
#include "Adafruit_SSD1306/Adafruit_SSD1306.h"

class Mode {
public:  
  virtual void displayHeader(Adafruit_SSD1306 display, int x, int y) = 0;
};

class ISOMode : public Mode {
public:
  ISOMode(uint16_t fstop, int32_t exposure);

  virtual void displayHeader(Adafruit_SSD1306 display, int x, int y);
private:
  uint16_t fstop;
  int32_t exposure;
};

class ExposureMode : public Mode {
public:
  ExposureMode(uint16_t fstop, uint16_t iso);

  virtual void displayHeader(Adafruit_SSD1306 display, int x, int y);
private:
  uint16_t fstop;
  uint16_t iso;
};

class FStopMode : public Mode {
public:
  FStopMode(int32_t exposure, uint16_t iso);

  virtual void displayHeader(Adafruit_SSD1306 display, int x, int y);
private:
  int32_t exposure;
  uint16_t iso;
};

#endif
