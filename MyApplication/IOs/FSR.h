#ifndef FSR_H
#define FSR_H

#include "mbed.h"
#include "my_utility.h"

#define DEFAULT_CALIB_GAIN    120 // 1.2
#define DEFAULT_CALIB_IN_MIN  20  // 0.2
#define DEFAULT_CALIB_IN_MAX  80  // 0.8

class FSR {
public:
  FSR(AnalogIn &adc) : _adc(adc) {
    _calib[0] = DEFAULT_CALIB_GAIN;
    _calib[1] = DEFAULT_CALIB_IN_MIN;
    _calib[2] = DEFAULT_CALIB_IN_MAX;
  }

  float raw() {
    return _adc.read(); 
  }

  float read() {
    float r = raw();
    float gain   = _calib[0] / 100.0f;
    float in_min = _calib[1] / 100.0f;
    float in_max = _calib[2] / 100.0f;

    r = map(r*gain, in_min, in_max, 0.0f, 1.0f);
    return r;
  }

  void calibrate(uint8_t *c) {
    memcpy(_calib, c, sizeof(_calib));
  }

private:
  AnalogIn &_adc;
  uint8_t _calib[3];
};

#endif