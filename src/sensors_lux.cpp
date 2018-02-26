#include <Wire.h>
#include "sensors_lux.h"

uint8_t read8(uint8_t reg);
uint32_t read32(uint8_t reg);
void write8(uint8_t reg, uint8_t value);

#define ATIME TSL2591_CONTROL_ATIME_100
#define ATIME_MS 100

#define GA 1
#define LUX_DF 408.0
#define LUX_COEFB 1.64
#define LUX_COEFC 0.59
#define LUX_COEFD 0.86

uint8_t gain_lookup[4] = {TSL2591_CONTROL_AGAIN_LOW, TSL2591_CONTROL_AGAIN_MEDIUM, TSL2591_CONTROL_AGAIN_HIGH, TSL2591_CONTROL_AGAIN_MAX};
float gain_multipliers[4] = {1.0, 24.5, 400.0, 9550};

SensorLux::SensorLux(SensorMux *mux, uint8_t mux_channel)
{
  this->_mux = mux;
  this->_mux_channel = mux_channel;
  this->_gain = 2;
}

void set_gain(uint8_t gain)
{
  write8(TSL2591_ENABLE_REG, 0);
  write8(TSL2591_CONTROL_REG, gain_lookup[gain] | ATIME);
  write8(TSL2591_ENABLE_REG, TSL2591_ENABLE_AEN | TSL2591_ENABLE_PON);

  // wait until ALS is valid
  while (!(read8(TSL2591_STATUS_REG) & TSL2591_STATUS_AVALID_MASK))
  {
    delay(110);
  }
}

void SensorLux::begin()
{
  this->_mux->single(this->_mux_channel);
  set_gain(this->_gain);
}

void SensorLux::agc()
{
  this->_mux->single(this->_mux_channel);

  uint16_t raw_ch0 = read32(TSL2591_C0_DATA_L_REG) & 0xffff;
  if (raw_ch0 < 100)
  {
    if (this->_gain == 3)
    {
      return;
    }

    this->_gain++;
    set_gain(this->_gain);
  }

  if (raw_ch0 > 10000)
  {
    if (this->_gain == 0)
    {
      return;
    }

    this->_gain--;
    set_gain(this->_gain);
  }
}

ReadingLux SensorLux::read()
{
  this->_mux->single(this->_mux_channel);

  uint32_t raw = read32(TSL2591_C0_DATA_L_REG);
  uint16_t ch0 = raw & 0xffff;
  uint16_t ch1 = (raw >> 16) & 0xffff;
  Serial.println(ch0);
  Serial.println(ch1);
  Serial.println("   ");

  float gain_multiplier = gain_multipliers[this->_gain];
  float cpl = (ATIME_MS * gain_multiplier) / (GA * LUX_DF);

  float lux1 = ((float)ch0 - (LUX_COEFB * (float)ch1)) / cpl;
  float lux2 = ((LUX_COEFC * (float)ch0) - (LUX_COEFD * (float)ch1)) / cpl;
  float lux = lux1 > lux2 ? lux1 : lux2;

  ReadingLux reading;

  reading.raw = raw;
  reading.lux = lux;

  return reading;
}

uint8_t read8(uint8_t reg)
{
  Wire.beginTransmission(TSL2591_ADDR);
  Wire.write(0xA0 | reg);
  Wire.endTransmission();

  Wire.requestFrom(TSL2591_ADDR, 1);
  uint16_t a = Wire.read();

  return a;
}
uint32_t read32(uint8_t reg)
{
  Wire.beginTransmission(TSL2591_ADDR);
  Wire.write(0xA0 | reg);
  Wire.endTransmission();

  Wire.requestFrom(TSL2591_ADDR, 4);
  uint16_t a = Wire.read();
  uint16_t b = Wire.read();
  uint16_t c = Wire.read();
  uint16_t d = Wire.read();

  return (d << 24) | (c << 16) | (b << 8) | a;
}

void write8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(TSL2591_ADDR);
  Wire.write(0xA0 | reg);
  Wire.write(value);
  Wire.endTransmission();
}