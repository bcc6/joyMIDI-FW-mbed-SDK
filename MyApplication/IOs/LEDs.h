#ifndef LEDS_H
#define LEDS_H

#include "mbed.h"

static const int    LEDS_NUMBER_OF_CHIPS = 3;
static const int    LEDS_DATA_BITS = 8;
static const int    LEDS_NUMBERS = 20;
static const int8_t LEDS_MAP[24] = {
    // Mapping Dn (LED number in schematic) to bit_n, Value '-1' means skip
    // Dn   15  16  17  18  19  20  21  22
             4,  5,  6,  7, 15, 14, 13, 12,
    // Dn    6   7   8   9  10  11  12  13
             8,  9, 10, 11, 19, 18, 17, 16,
    // Dn    2   3   4   5  n/a n/a n/a n/a
             3,  0,  1,  2, -1, -1, -1, -1
};


// 74HC595: 8-bit serial-in, serial or parallel-out shift register with output latches
class LEDs {
 public:
  /** Create a LEDs object
   * @param sh Shift register clock (LOW-to-HIGH edge-triggered)
   * @param st Storage register clock (LOW-to-HIGH edge-triggered)
   * @param ds Serial data output
   */
  LEDs(DigitalOut &sh, DigitalOut &st, DigitalOut &ds) : _sh(sh), _st(st), _ds(ds) {
    _sh = 0;
    _st = 0;
    _ds = 0;

    write(0);
  }

  /** Write
   * @param Bit 0-19 correspond to LED1-LED20, '1' mean ON
   */
  void write(uint32_t data) {

    // Data & Clock
    for (int i=(LEDS_NUMBER_OF_CHIPS*LEDS_DATA_BITS-1); i>=0; i--) {
      int bit_n = LEDS_MAP[i];
      if (bit_n < 0) {
        _ds = 1;  // OFF
      } else {
        _ds = (data & (1<<bit_n)) ? 0 : 1;  // ON : OFF, Low active
      }
      asm("nop");
      _sh = 1;
      asm("nop");
      _ds = 0;
      _sh = 0;
      asm("nop");
    }

    // Latch
    _st = 1;
    asm("nop");
    _st = 0;
  }

 private:
  DigitalOut &_sh;
  DigitalOut &_st;
  DigitalOut &_ds;
};

#endif