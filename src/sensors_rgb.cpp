#include <Wire.h>
#include "sensors_rgb.h"

uint8_t read8(TCS3472_REGISTER reg);
uint16_t read16(TCS3472_REGISTER reg);
void write8(TCS3472_REGISTER reg, uint8_t value);

SensorRGB::SensorRGB(SensorMux *mux, uint8_t mux_channel)
{
  this->_mux = mux;
  this->_mux_channel = mux_channel;
}

void SensorRGB::begin()
{
  this->setParams(0xff, TCS3472_CONTROL_AGAIN60);
  this->waitForReady();
}

void calculateColourTemperature(uint16_t c, uint16_t r, uint16_t g, uint16_t b, float *lux, float *temp)
{
  // "Calculating Color Temperature and Illuminance using the TAOS TCS3414CS Digital Color Sensor" Joe Smith, Feb 2009

  float X = -0.14282 * r + 1.54924 * g + -0.95641 * b;
  float Y = -0.32466 * r + 1.57837 * g + -0.73191 * b;
  float Z = -0.68202 * r + 0.77073 * g + 0.56332 * b;

  float x = X / (X + Y + Z);
  float y = Y / (X + Y + Z);

  float n = (x - 0.3320) / (0.1858 - y);

  *lux = Y;
  *temp = 449 * (n * n * n) + 3525 * (n * n) + 6823.3 * n + 5520.33;
}

ReadingRGB SensorRGB::read()
{
  ReadingRGB reading;

  this->_mux->single(this->_mux_channel);

  delay((256.0f - this->_atime) * 2.4f);

  uint16_t raw_c = read16(TCS3472_REGISTER_CL);
  uint16_t raw_r = read16(TCS3472_REGISTER_RL);
  uint16_t raw_g = read16(TCS3472_REGISTER_GL);
  uint16_t raw_b = read16(TCS3472_REGISTER_BL);

  Serial.println(raw_c);

  reading.r = raw_r;
  reading.g = raw_g;
  reading.b = raw_b;

  calculateColourTemperature(raw_c, raw_r, raw_g, raw_b, &reading.lux, &reading.temp);

  // "Lux and CCT Calculations using ams Color Sensors" Aug 2013
  float ir = 0; // ((float)raw_r + (float)raw_g + (float)raw_b - (float)raw_c) / 2;

  float c = raw_c - ir;
  float r = raw_r - ir;
  float g = raw_g - ir;
  float b = raw_b - ir;

  Serial.print(c);
  Serial.print(" ");
  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.println(b);

  // float G = 0.136 * r + 1 * g + -0.444 * b;

  float gains[4] = {1.0f, 4.0f, 16.0f, 60.0f};
  float ATIME_ms = (256.0f - this->_atime) * 2.4f;
  float AGAINx = gains[this->_control & 0x03];

  float T = 1;
  float GA = 1 / T;
  float DF = 310;

  float CPL = (ATIME_ms * AGAINx) / (GA * DF);

  // Serial.println(G);
  // Serial.println(CPL);
  // // reading.lux = G / CPL;
  // reading.temp = 3810.0f * ((float)b / r) + 1391.0f;

  // // temp

  float X = -0.14282 * (r / CPL) + 1.54924 * (g / CPL) + -0.95641 * (b / CPL);
  float Y = -0.32466 * (r / CPL) + 1.57837 * (g / CPL) + -0.73191 * (b / CPL);
  float Z = -0.68202 * (r / CPL) + 0.77073 * (g / CPL) + 0.56332 * (b / CPL);

  float x = X / (X + Y + Z);
  float y = Y / (X + Y + Z);

  float n = (x - 0.3320) / (0.1858 - y);

  reading.lux = Y;
  reading.temp = 449 * (n * n * n) + 3525 * (n * n) + 6823.3 * n + 5520.33;

  return reading;
}

void SensorRGB::setParams(TCS3472_TIMING atime, TCS3472_CONTROL_FLAGS control)
{
  this->_mux->single(this->_mux_channel);

  this->disable();
  write8(TCS3472_REGISTER_TIMING, atime);
  write8(TCS3472_REGISTER_CONTROL, control);
  this->enable();

  this->_atime = atime;
  this->_control = control;
}
void SensorRGB::waitForReady()
{
  while (!(read8(TCS3472_REGISTER_STATUS) & TCS3472_STATUS_AVALID))
  {
    delay(5);
  }
}

void SensorRGB::enable()
{
  write8(TCS3472_REGISTER_ENABLE, TCS3472_ENABLE_PON);
  // "A minimum interval of 2.4 ms must pass after PON is asserted before an RGBC can be initiated"
  delay(3);
  write8(TCS3472_REGISTER_ENABLE, TCS3472_ENABLE_PON | TCS3472_ENABLE_AEN);
}
void SensorRGB::disable()
{
  write8(TCS3472_REGISTER_ENABLE, 0);
}

uint8_t read8(TCS3472_REGISTER reg)
{
  Wire.beginTransmission(TCS3472_ADDR);
  Wire.write(TCS3472_COMMAND | TCS3472_COMMAND_TYPE_REPEAT | reg);
  Wire.endTransmission();

  Wire.requestFrom(TCS3472_ADDR, 1);
  uint16_t a = Wire.read();

  return a;
}
uint16_t read16(TCS3472_REGISTER reg)
{
  Wire.beginTransmission(TCS3472_ADDR);
  Wire.write(TCS3472_COMMAND | TCS3472_COMMAND_TYPE_BLOCK | reg);
  Wire.endTransmission();

  Wire.requestFrom(TCS3472_ADDR, 2);
  uint16_t a = Wire.read();
  uint16_t b = Wire.read();

  return (b << 8) | a;
}
void write8(TCS3472_REGISTER reg, uint8_t value)
{
  Wire.beginTransmission(TCS3472_ADDR);
  Wire.write(TCS3472_COMMAND | TCS3472_COMMAND_TYPE_REPEAT | reg);
  Wire.write(value);
  Wire.endTransmission();
}