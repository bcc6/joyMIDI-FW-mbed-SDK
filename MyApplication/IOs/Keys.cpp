#include "Keys.h"

Keys::Keys(DigitalOut &ck, DigitalOut &pl, DigitalIn &si)
    : _ck(ck), _pl(pl), _si(si), _counter(), _keys(0) {
  _ck = 0;
  _pl = 1;

  _ticker.attach_us(callback(this, &Keys::_keys_process), DEBOUNCE_us);
}

int Keys::_read() {
  int val = 0;

  /* Load parallel data */
  _pl = 0;
  wait_us(1);
  _pl = 1;
  wait_us(1);

  /* Read n bits serial data */
  for (int i = 0; i < KEYS_NUMBER_OF_CHIPS; i++) {
    for (int j = (KEYS_DATA_BITS - 1); j >= 0; j--) {
      /* Read bit */
      if (_si == 0) {
        int bit_n = KEYS_MAP[i * KEYS_DATA_BITS + j];
        if (bit_n >= 0) {
          val |= (1 << bit_n);
        }
      }
      /* Next bit */
      _ck = 1;
      wait_us(1);
      _ck = 0;
    }
  }

  return val;
}

void Keys::_keys_process() {
  int keys = _read();

  /* Debounce */
  for (int n = 0; n < KEYS_NUMBERS; n++) {
    int mask = 1 << n;
    if (keys & mask) {
      /* Button n pressed */
      if (_counter[n] < DEBOUNCE_SAMPLES) _counter[n]++;
      if (_counter[n] == DEBOUNCE_SAMPLES) _keys |= (1 << n);
    } else {
      /* Button n unpressed */
      if (_counter[n] > 0) _counter[n]--;
      if (_counter[n] == 0) _keys &= ~(1 << n);
    }
  }
}
