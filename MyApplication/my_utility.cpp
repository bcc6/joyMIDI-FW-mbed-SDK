#include "my_utility.h"

float map(float in, float in_min, float in_max, float out_min, float out_max) {
  if (in <= in_min) return out_min;
  if (in >= in_max) return out_max;

  return (in - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}