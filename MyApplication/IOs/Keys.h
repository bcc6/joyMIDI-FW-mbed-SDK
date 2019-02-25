#ifndef KEYS_H
#define KEYS_H

#include "mbed.h"

#define KEY_SW(n) (1UL << (n - 1))  // n range 1-20

static const int    KEYS_NUMBER_OF_CHIPS = 3;
static const int    KEYS_DATA_BITS = 8;
static const int    KEYS_NUMBERS = 20;
static const int8_t KEYS_MAP[24] = {
    // Mapping SWn (Key number in schematic) to bit_n, Value '-1' means skip
    // SWn  13  14  15  16  17  18  19  20
            13, 12,  4,  5,  6,  7, 15, 14,
    // SWn   5   6   7   8   9  10  11  12
            17, 16,  8,  9, 10, 11, 19, 18,
    // SWn   1   2   3   4  n/a n/a n/a n/a
             0,  3,  1,  2, -1, -1, -1, -1
};

static const int DEBOUNCE_us = 1000;
static const int DEBOUNCE_SAMPLES = 10;


// 74HC165: 8-bit parallel-in/serial out shift register
class Keys {
 public:
  /** Create a Keys object
   *
   * @param ck Clock (LOW-to-HIGH edge-triggered), aka CP
   * @param pl Parallel load (active LOW), aka nPL
   * @param si Serial input, aka Q7
   */
  Keys(DigitalOut &ck, DigitalOut &pl, DigitalIn &si);

  /** Read
   *
   * @returns
   *    Bit 0-19 correspond to key1-key20, '1' mean Pressed
   */
  inline int read() { return _keys; }

 private:
  DigitalOut &_ck;
  DigitalOut &_pl;
  DigitalIn &_si;

  Ticker _ticker;
  uint8_t _counter[KEYS_NUMBERS];
  volatile int _keys;

  int _read();
  void _keys_process();
};

#endif