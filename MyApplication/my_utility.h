#ifndef MY_UTILITY_H
#define MY_UTILITY_H

#include "mbed.h"

#define TO_PERCENT(n) ((int)(n * 100))
#define TO_PERMILL(n) ((int)(n * 1000))

float map(float in, float in_min, float in_max, float out_min, float out_max);

#endif