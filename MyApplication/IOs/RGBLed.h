#ifndef RGBLED_H
#define RGBLED_H

#include "mbed.h"

enum Colors {
  COLOR_RED = 0xFF0000,
  COLOR_ORANGE = 0xFF7F00,
  COLOR_YELLOW = 0xFFFF00,
  COLOR_GREEN = 0x00FF00,
  COLOR_BLUE = 0x0000FF,
  COLOR_INDIGO = 0x4B0082,
  COLOR_WHITE = 0xFFFFFF,
  COLOR_CYAN = 0x00FFFF,
};

class RGBLed {
 public:
  /** Create a RGBLed
   *
   *  @param r_pin PwmOut pin to LED Red
   *  @param g_pin PwmOut pin to LED Green
   *  @param b_pin PwmOut pin to LED Blue
   */
  RGBLed(PwmOut &r_pin, PwmOut &g_pin, PwmOut &b_pin, float brightness = 0.5);

  /** set LED brightness
   *
   *  @param brightness Range 0.0-1.0
   */
  inline void set_brightness(float brightness) { _brightness = brightness; }

  /** Write color (1.0, 1.0, 1.0)
   *
   * @param r Range 0.0-1.0
   * @param g Range 0.0-1.0
   * @param b Range 0.0-1.0
   */
  void write(float r, float g, float b);

  /** Write color (255, 255, 255)
   *
   * @param r Range 0-255
   * @param g Range 0-255
   * @param b Range 0-255
   */
  void write(int r, int g, int b);

  /** Write color (0xFFFFFF)
   *
   * @param rgb Range 0x000000-0xFFFFFF
   */
  void write(int rgb);

  /** Write color (ColorName)
   *
   * @param color enum Colors
   */
  void write(Colors color);

 private:
  PwmOut &_r_pin;
  PwmOut &_g_pin;
  PwmOut &_b_pin;

  float _brightness;
};

#endif