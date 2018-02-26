#include <Wire.h>
#include "sensors_mux.h"

#define TCAADDR 0x70

SensorMux::SensorMux() {}

void SensorMux::begin()
{
  this->set(0);
}

void SensorMux::single(uint8_t channel)
{
  this->set(1 << channel);
}
void SensorMux::enable(uint8_t channel)
{
  uint8_t newState = this->_state;

  newState |= 1 << channel;

  this->set(newState);
}
void SensorMux::disable(uint8_t channel)
{
  uint8_t newState = this->_state;

  newState &= ~(1 << channel);

  this->set(newState);
}

void SensorMux::set(uint8_t state)
{
  if (this->_state == state)
    return;

  Serial.print("setting mux state ");
  Serial.println(state);

  Wire.beginTransmission(TCAADDR);
  Wire.write(state);
  Wire.endTransmission();

  Serial.print("setting mux state - completed.");

  this->_state = state;
}