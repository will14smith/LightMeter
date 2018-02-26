#ifndef sensors_lux_h
#define sensors_lux_h

#include "Arduino.h"
#include "sensors_mux.h"

#define TSL2591_ADDR 0x29

#define TSL2591_ENABLE_REG 0x00
#define TSL2591_ENABLE_AEN (1 << 1) // Enable ALS function
#define TSL2591_ENABLE_PON (1 << 0) // Enable oscillator for timers & ADC

#define TSL2591_CONTROL_REG 0x01
#define TSL2591_CONTROL_AGAIN_LOW (0b00 << 4)
#define TSL2591_CONTROL_AGAIN_MEDIUM (0b01 << 4)
#define TSL2591_CONTROL_AGAIN_HIGH (0b10 << 4)
#define TSL2591_CONTROL_AGAIN_MAX (0b11 << 4)
#define TSL2591_CONTROL_ATIME_100 (0b000 << 0)
#define TSL2591_CONTROL_ATIME_200 (0b001 << 0)
#define TSL2591_CONTROL_ATIME_300 (0b010 << 0)
#define TSL2591_CONTROL_ATIME_400 (0b011 << 0)
#define TSL2591_CONTROL_ATIME_500 (0b100 << 0)
#define TSL2591_CONTROL_ATIME_600 (0b101 << 0)

#define TSL2591_STATUS_REG 0x13
#define TSL2591_STATUS_AVALID_MASK 0x1 // Is ALS valid

#define TSL2591_C0_DATA_L_REG 0x14
#define TSL2591_C0_DATA_H_REG 0x15
#define TSL2591_C1_DATA_L_REG 0x16
#define TSL2591_C1_DATA_H_REG 0x17

struct ReadingLux
{
  int16_t raw;
  float lux;
};

class SensorLux
{
public:
  SensorLux(SensorMux *mux, uint8_t mux_channel);

  void begin();

  void agc();
  ReadingLux read();

private:
  SensorMux *_mux;
  uint8_t _mux_channel;

  uint8_t _gain;
};

#endif