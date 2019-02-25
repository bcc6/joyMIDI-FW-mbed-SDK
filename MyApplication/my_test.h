#ifndef MY_TEST_H
#define MY_TEST_H

#include "mbed.h"
#include "my_debug.h"
#include "USBMIDI.h"
#include "USBJoystick.h"
#include "MAFilter.h"
#include "SlideRotary.h"
#include "FSR.h"
#include "Joystick.h"
#include "RGBLed.h"
#include "LEDs.h"
#include "Keys.h"
#include "QEI.h"
#include "BMA253.h"
#include "SSD1306.h"

void test_console(USBMIDI *um, USBJoystick *uj,
                  SlideRotary *pot, FSR *fsr, Joystick *joy,
                  RGBLed *rgb, LEDs *leds, Keys *keys, QEI *jwa, QEI *jwb,
                  BMA253 *accel, SSD1306 *oled);

#endif
