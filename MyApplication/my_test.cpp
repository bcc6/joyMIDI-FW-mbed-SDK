#include "my_test.h"

CircularBuffer<char, 20> _log;
uint8_t _sysex[] = {0xF0, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0xF7};

void _onNoteOff(uint8_t c, uint8_t n, uint8_t v) {
  if (c == 1 && n == 23 && v == 0)
    _log.push(0x70);  // NoteOn(Velocity=0)
  else if (c == 1 && n == 23 && v == 45)
    _log.push(0x80);
  else
    _log.push(0x00);
}
void _onNoteOn(uint8_t c, uint8_t n, uint8_t v) {
  if (c == 1 && n == 23 && v == 45)
    _log.push(0x90);
  else
    _log.push(0x00);
}
void _onPolyAftertouch(uint8_t c, uint8_t n, uint8_t p) {
  if (c == 1 && n == 23 && p == 45)
    _log.push(0xA0);
  else
    _log.push(0x00);
}
void _onControlChange(uint8_t c, uint8_t f, uint8_t v) {
  if (c == 1 && f == 23 && v == 45)
    _log.push(0xB0);
  else
    _log.push(0x00);
}
void _onProgramChange(uint8_t c, uint8_t p) {
  if (c == 1 && p == 23)
    _log.push(0xC0);
  else
    _log.push(0x00);
}
void _onChannelAftertouch(uint8_t c, uint8_t p) {
  if (c == 1 && p == 23)
    _log.push(0xD0);
  else
    _log.push(0x00);
}
void _onPitchBend(uint8_t c, uint16_t a) {
  if (c == 1 && a == 2345)
    _log.push(0xE0);
  else
    _log.push(0x00);
}
void _onSystemExclusive(uint8_t *s, uint16_t l, bool n) {
  if (l != sizeof(_sysex)) {
    _log.push(0x00);
    return;
  }
  for (int i = 0; i < l; i++) {
    if (s[i] != _sysex[i]) {
      _log.push(0x00);
      return;
    }
  }
  _log.push(0xF0);
}
void _onTimeCodeQuarterFrame(uint8_t t) {
  if (t == 123)
    _log.push(0xF1);
  else
    _log.push(0x00);
}
void _onSongPositionPointer(uint16_t p) {
  if (p == 12345)
    _log.push(0xF2);
  else
    _log.push(0x00);
}
void _onSongSelect(uint8_t s) {
  if (s == 123)
    _log.push(0xF3);
  else
    _log.push(0x00);
}
void _onTuneRequest() { _log.push(0xF6); }
void _onTimingClock() { _log.push(0xF8); }
void _onStart() { _log.push(0xFA); }
void _onContinue() { _log.push(0xFB); }
void _onStop() { _log.push(0xFC); }
void _onActiveSensing() { _log.push(0xFE); }
void _onReset() { _log.push(0xFF); }

void _attach_callback(USBMIDI *um) {
  /* Channel Voice Messages */
  um->attachNoteOff(_onNoteOff);
  um->attachNoteOn(_onNoteOn);
  um->attachPolyAftertouch(_onPolyAftertouch);
  um->attachControlChange(_onControlChange);
  um->attachProgramChange(_onProgramChange);
  um->attachChannelAftertouch(_onChannelAftertouch);
  um->attachPitchBend(_onPitchBend);
  /* System Common Messages */
  um->attachSystemExclusive(_onSystemExclusive);
  um->attachTimeCodeQuarterFrame(_onTimeCodeQuarterFrame);
  um->attachSongPositionPointer(_onSongPositionPointer);
  um->attachSongSelect(_onSongSelect);
  um->attachTuneRequest(_onTuneRequest);
  /* System Real-Time Messages */
  um->attachTimingClock(_onTimingClock);
  um->attachStart(_onStart);
  um->attachContinue(_onContinue);
  um->attachStop(_onStop);
  um->attachActiveSensing(_onActiveSensing);
  um->attachReset(_onReset);
}

void _send_message(USBMIDI *um) {
  /* Channel Voice Messages */
  um->sendNoteOn(1, 23, 0), wait_ms(2);  // Equivalent to NoteOff()
  um->sendNoteOff(1, 23, 45), wait_ms(2);
  um->sendNoteOn(1, 23, 45), wait_ms(2);
  um->sendPolyAftertouch(1, 23, 45), wait_ms(2);
  um->sendControlChange(1, 23, 45), wait_ms(2);
  um->sendProgramChange(1, 23), wait_ms(2);
  um->sendChannelAftertouch(1, 23), wait_ms(2);
  um->sendPitchBend(1, 2345), wait_ms(2);
  /* System Common Messages */
  um->sendSystemExclusive(_sysex, sizeof(_sysex), false), wait_ms(2);
  um->sendTimeCodeQuarterFrame(123), wait_ms(2);
  um->sendSongPositionPointer(12345), wait_ms(2);
  um->sendSongSelect(123), wait_ms(2);
  um->sendTuneRequest(), wait_ms(2);
  /* System Real-Time Messages */
  um->sendTimingClock(), wait_ms(2);
  um->sendStart(), wait_ms(2);
  um->sendContinue(), wait_ms(2);
  um->sendStop(), wait_ms(2);
  um->sendActiveSensing(), wait_ms(2);
  um->sendReset(), wait_ms(2);
}

void _verify_message(uint8_t *msg, int msg_len) {
  for (int i = 0; i < msg_len; i++) {
    if (_log.empty()) {
      DEBUG_OUT("FAIL: DataEmpty != %02Xh\n", msg[i]);
      while (1)
        ;
    } else {
      char data = 0;
      _log.pop(data);

      if (data != msg[i]) {
        DEBUG_OUT("FAIL: %02Xh != %02Xh\n", data, msg[i]);
        while (1)
          ;
      }
    }
  }
}

void console_usbmidi(USBMIDI *um) {
  int times = 0;

  DEBUG_OUT("Plug USB to start >\n");
  um = new USBMIDI();
  wait_ms(100);
  _attach_callback(um);

  DEBUG_OUT("Config Host MIDI I/O to loopback then press \'SPACE\' >\n");
  PRESS(' ');

  while (1) {
    /* Clear log */
    _log.reset();

    /* Send message */
    _send_message(um);
    wait_ms(20);

    /* Verify message */
    uint8_t msg[] = {0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0xF1,
                     0xF2, 0xF3, 0xF6, 0xF8, 0xFA, 0xFB, 0xFC, 0xFE, 0xFF};
    _verify_message(msg, sizeof(msg));

    if ((++times % 100) == 0) {
      DEBUG_OUT("PASS: %d times\n", times);
    }
  }  // End of while loop
}

void console_usbjoystick(USBJoystick *uj) {
  DEBUG_OUT("Plug USB to start >\n");
  uj = new USBJoystick();
  wait_ms(100);

  uint16_t i = 0;
  int16_t throttle = 0;
  int16_t rudder = 0;
  int16_t x = 0;
  int16_t y = 0;
  int32_t radius = 120;
  int32_t angle = 0;
  uint32_t buttons = 0;
  uint8_t  hat = 0;

  while (1) {
      // Basic Joystick
      throttle = (i >> 8) & 0xFF; // value -128 .. 127
      rudder   = (i >> 8) & 0xFF; // value -128 .. 127
      buttons  = (i >> 8) & 0x0F; // value    0 .. 15, one bit per button
      hat      = (i >> 8) & 0x07; // value    0 .. 7 or 8 for neutral
      i++;

      x = cos((double)angle*3.14/180.0)*radius;  // value -128 .. 127
      y = sin((double)angle*3.14/180.0)*radius;  // value -128 .. 127
      angle += 3;

      uj->update(throttle, rudder, x, y, buttons, hat);

      wait(0.001);
  }
}

void console_slide_rotary(SlideRotary *pot) {
  while (1) {
    float v = pot->read();

    DEBUG_OUT("%d%%\n", TO_PERCENT(v));
    wait(1);
  }
}

void console_fsr(FSR *fsr) {
  while (1) {
    float v = fsr->read();
    int   n = v * 30;

    DEBUG_OUT("%3d%% ", TO_PERCENT(v));
    for(int i=0; i<n; i++) {
      DEBUG_OUT("*");
    }
    DEBUG_OUT("\n");

    wait_ms(50);
  }
}

void console_joystick(Joystick *joy) {
  Vector2D c = joy->get_raw();
  float xmin = 0.0f, xmid = c.x, xmax = 1.0f;
  float ymin = 0.0f, ymid = c.y, ymax = 1.0f;
  joy->calibrate(xmin, xmid, xmax, ymin, ymid, ymax);
  DEBUG_OUT("Cal: X(%d%% %d%% %d%%) Y(%d%% %d%% %d%%)\n",
    TO_PERCENT(xmin), TO_PERCENT(xmid), TO_PERCENT(xmax),
    TO_PERCENT(ymin), TO_PERCENT(ymid), TO_PERCENT(ymax));

  while (1) {
    Vector2D r = joy->get_raw();
    Vector2D v = joy->get_vector();
    Vector2D m = joy->get_mapped_vector();
    Polar2D p = joy->get_polar();
    Direction d = joy->get_direction();

    DEBUG_OUT("Raw(%d%% %d%%) ", TO_PERCENT(r.x), TO_PERCENT(r.y));
    DEBUG_OUT("Vec(%d%% %d%%) ", TO_PERCENT(v.x), TO_PERCENT(v.y));
    DEBUG_OUT("map(%d%% %d%%) ", TO_PERCENT(m.x), TO_PERCENT(m.y));
    DEBUG_OUT("Pol(%d%% %ddeg) ", TO_PERCENT(p.r), (int)p.t);
    DEBUG_OUT("Dir(%d) ", d);
    DEBUG_OUT("Pressed(%d)\n", joy->is_pressed());

    wait(1);
  }
}

void console_rgbled(RGBLed *rgb) {
  Colors colors[] = {COLOR_RED,  COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN,
                     COLOR_BLUE, COLOR_INDIGO, COLOR_WHITE,  COLOR_CYAN};

  while (1) {
    /* RGB PWM */
    for (int i = 0; i < 3; i++) {
      if (i == 0) DEBUG_OUT("PWM R\n");
      if (i == 1) DEBUG_OUT("PWM G\n");
      if (i == 2) DEBUG_OUT("PWM B\n");

      for (int j = 0; j <= 255; j++) {
        if (i == 0) rgb->write(j << 16);
        if (i == 1) rgb->write(j << 8);
        if (i == 2) rgb->write(j << 0);
        wait_ms(4);
      }

      for (int j = 255; j >= 0; j--) {
        if (i == 0) rgb->write(j << 16);
        if (i == 1) rgb->write(j << 8);
        if (i == 2) rgb->write(j << 0);
        wait_ms(4);
      }
    }

    /* Colors */
    for (int i = 0; i < 8; i++) {
      rgb->write(colors[i]);
      switch(i){
        case 0: DEBUG_OUT("RED\n");    break;
        case 1: DEBUG_OUT("ORANGE\n"); break;
        case 2: DEBUG_OUT("YELLOW\n"); break;
        case 3: DEBUG_OUT("GREEN\n");  break;
        case 4: DEBUG_OUT("BLUE\n");   break;
        case 5: DEBUG_OUT("INDIGO\n"); break;
        case 6: DEBUG_OUT("WHITE\n");  break;
        case 7: DEBUG_OUT("CYAN\n\n"); break;
      }
      wait(1);
    }
  }
}

void console_btnleds(LEDs *leds) {
  uint32_t data = 1;
  while(1) {
    DEBUG_OUT("%05X\n", data);

    leds->write(data);
    data <<= 1;
    if (data == (1<<20)) data = 1;
    wait(0.5);
  }
}

void console_keys(Keys *keys) {
  while (1) {
    int k = keys->read();
    if (k) DEBUG_OUT("%05X\n", k);
    wait(0.2);
  }
}

void console_wheel(QEI *jwa, QEI *jwb) {
  while(1) {
    int a = jwa->getPulses();
    int b = jwb->getPulses();
    DEBUG_OUT("L:%3d  ", a);
    DEBUG_OUT("R:%3d\n", b);
    wait(0.2);
  }
}

void console_accel(BMA253 *accel) {
  int16_t raw[3];
  accel->set_sensitivity(BMA_RANGE_2G_SENS);
  accel->get_raw(raw);
  accel->calibrate(raw);
  DEBUG_OUT("Calib(%d %d %d)\n", raw[0], raw[1], raw[2]);

  while (1) {
    float vec[3];
    accel->get_vector(vec);
    DEBUG_OUT("XYZ(%3d%% %3d%% %3d%%)\n", TO_PERCENT(vec[0]), TO_PERCENT(vec[1]), TO_PERCENT(vec[2]));
    wait(0.2);
  }
}

void console_oled(SSD1306 *oled) {
  oled->init();
  oled->cls(NULL, 1);

  DEBUG_OUT("Press \'SPACE\' to start >\n");
  PRESS(' ');

  while(1) {
    oled->cls(NULL, 1);
    wait(0.5);

    oled->locate(0, 0);
    for(int i=0; i<256; i++){
      oled->putchar(i);

      if((i+1)%64 == 0) {
        oled->redraw();
        wait(2);
        oled->locate(0, 0);
      }
    }
  }
}

void test_console(USBMIDI *um, USBJoystick *uj,
                  SlideRotary *pot, FSR *fsr, Joystick *joy,
                  RGBLed *rgb, LEDs *leds, Keys *keys, QEI *jwa, QEI *jwb,
                  BMA253 *accel, SSD1306 *oled)
{
  int select = 0;

  while (1) {
    DEBUG_OUT("\n");
    DEBUG_OUT("============================\n");
    DEBUG_OUT("* Test Console              \n");
    DEBUG_OUT("============================\n");
    DEBUG_OUT("* ( 0) USB MIDI\n");
    DEBUG_OUT("* ( 1) USB Joystick\n");
    DEBUG_OUT("*\n");
    DEBUG_OUT("* (10) Slide/Rotary\n");
    DEBUG_OUT("* (11) FSR\n");
    DEBUG_OUT("* (12) Joystick\n");
    DEBUG_OUT("* (13) RGB LED\n");
    DEBUG_OUT("* (14) Button LED\n");
    DEBUG_OUT("* (15) Keys\n");
    DEBUG_OUT("* (16) Jog wheel\n");
    DEBUG_OUT("*\n");
    DEBUG_OUT("* (20) Acclerometer\n");
    DEBUG_OUT("* (21) OLED\n");
    DEBUG_OUT("============================\n");

    while (1) {
      DEBUG_OUT("> ");
      DEBUG_IN("%d", &select);
      if ((select >= 0 && select <= 1) || (select >= 10 && select <= 16) ||
          (select >= 20 && select <= 21)) {
        DEBUG_OUT("\n");
        break;
      } else {
        DEBUG_OUT("\n");
      }
    }

    switch (select) {
      case 0:
        console_usbmidi(um);
        break;
      case 1:
        console_usbjoystick(uj);
        break;

      case 10:
        console_slide_rotary(pot);
        break;
      case 11:
        console_fsr(fsr);
        break;
      case 12:
        console_joystick(joy);
        break;
      case 13:
        console_rgbled(rgb);
        break;
      case 14:
        console_btnleds(leds);
        break;
      case 15:
        console_keys(keys);
        break;
      case 16:
        console_wheel(jwa, jwb);
        break;

      case 20:
        console_accel(accel);
        break;
      case 21:
        console_oled(oled);
        break;
    }

    DEBUG_OUT("\n\n");
  }  // End of While loop
}
