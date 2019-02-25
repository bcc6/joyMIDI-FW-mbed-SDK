#ifndef SLIDEROTARY_H
#define SLIDEROTARY_H

#include "mbed.h"

class SlideRotary {
public:
  SlideRotary(AnalogIn &adc) : _adc(adc) {}
  float read() { return _adc.read(); }

private:
  AnalogIn &_adc;
};

#endif