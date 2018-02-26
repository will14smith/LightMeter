#ifndef sensors_rgb_h
#define sensors_rgb_h

#include "Arduino.h"
#include "sensors_mux.h"

#define TCS3472_ADDR 0x29

#define TCS3472_COMMAND 0x80
#define TCS3472_COMMAND_TYPE_REPEAT 0x00
#define TCS3472_COMMAND_TYPE_BLOCK 0x20

enum TCS3472_REGISTER
{
  TCS3472_REGISTER_ENABLE = 0x00,
  TCS3472_REGISTER_TIMING = 0x01,
  TCS3472_REGISTER_CONTROL = 0x0F,
  TCS3472_REGISTER_STATUS = 0x13,

  TCS3472_REGISTER_CL = 0x14,
  TCS3472_REGISTER_CH = 0x15,
  TCS3472_REGISTER_RL = 0x16,
  TCS3472_REGISTER_RH = 0x17,
  TCS3472_REGISTER_GL = 0x18,
  TCS3472_REGISTER_GH = 0x19,
  TCS3472_REGISTER_BL = 0x1A,
  TCS3472_REGISTER_BH = 0x1B,
};

enum TCS3472_ENABLE_FLAGS
{
  TCS3472_ENABLE_AIEN = 0x10,
  TCS3472_ENABLE_WEN = 0x08,
  TCS3472_ENABLE_AEN = 0x02,
  TCS3472_ENABLE_PON = 0x01
};

typedef uint8_t TCS3472_TIMING;

enum TCS3472_CONTROL_FLAGS
{
  TCS3472_CONTROL_AGAIN1 = 0x00,
  TCS3472_CONTROL_AGAIN4 = 0x01,
  TCS3472_CONTROL_AGAIN16 = 0x02,
  TCS3472_CONTROL_AGAIN60 = 0x03,
};

enum TCS3472_STATUS_FLAGS
{
  TCS3472_STATUS_AINT = 0x10,
  TCS3472_STATUS_AVALID = 0x01
};

struct ReadingRGB
{
  float lux;

  uint16_t r;
  uint16_t g;
  uint16_t b;

  float temp;
};

class SensorRGB
{
public:
  SensorRGB(SensorMux *mux, uint8_t mux_channel);

  void begin();

  ReadingRGB read();

private:
  void setParams(TCS3472_TIMING atime, TCS3472_CONTROL_FLAGS control);
  void waitForReady();

  void enable();
  void disable();

  SensorMux *_mux;
  uint8_t _mux_channel;

  TCS3472_TIMING _atime;
  TCS3472_CONTROL_FLAGS _control;
};

#endif