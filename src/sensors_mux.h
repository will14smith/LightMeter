#ifndef sensors_mux_h
#define sensors_mux_h

#include "Arduino.h"

class SensorMux
{
public:
  SensorMux();

  void begin();

  void single(uint8_t channel);
  void enable(uint8_t channel);
  void disable(uint8_t channel);

private:
  void set(uint8_t state);

  uint8_t _state;
};

#endif