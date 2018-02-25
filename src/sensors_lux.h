#ifndef sensors_lux_h
#define sensors_lux_h

#include "Arduino.h"
#include "sensors_mux.h"

struct ReadingLux
{
  int32_t value;
};

class SensorLux
{
public:
  SensorLux(SensorMux *mux, uint8_t mux_channel);

  void begin();

  ReadingLux read();

private:
  SensorMux *_mux;
  uint8_t _mux_channel;
};

#endif