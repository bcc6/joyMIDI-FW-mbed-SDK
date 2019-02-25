#include "RGBLed.h"

RGBLed::RGBLed(PwmOut &r_pin, PwmOut &g_pin, PwmOut &b_pin, float brightness)
    : _r_pin(r_pin), _g_pin(g_pin), _b_pin(b_pin), _brightness(brightness) {
  /* PWM 1KHz */
  _r_pin.period_us(1000);
  _g_pin.period_us(1000);
  _b_pin.period_us(1000);
  /* Off */
  write(0.0f, 0.0f, 0.0f);
}

void RGBLed::write(float r, float g, float b) {
  /* Common Anode */
  _r_pin = 1.0f - (r * _brightness);
  _g_pin = 1.0f - (g * _brightness);
  _b_pin = 1.0f - (b * _brightness);
}

void RGBLed::write(int r, int g, int b) {
  float fr = r / 255.0f;
  float fg = g / 255.0f;
  float fb = b / 255.0f;
  write(fr, fg, fb);
}

void RGBLed::write(int rgb) {
  int ir = (rgb & 0xFF0000) >> 16;
  int ig = (rgb & 0x00FF00) >> 8;
  int ib = (rgb & 0x0000FF) >> 0;
  write(ir, ig, ib);
}

void RGBLed::write(Colors color) {
  int rgb = color;
  write(rgb);
}
