#include "sensors_lux.h"

SensorLux::SensorLux(SensorMux *mux, uint8_t mux_channel)
{
  this->_mux = mux;
  this->_mux_channel = mux_channel;
}

void SensorLux::begin()
{
  this->_mux->single(this->_mux_channel);
  // TODO setup device
}

ReadingLux SensorLux::read()
{
  this->_mux->single(this->_mux_channel);
  // TODO read
  ReadingLux reading;

  reading.value = 100;

  return reading;
}
