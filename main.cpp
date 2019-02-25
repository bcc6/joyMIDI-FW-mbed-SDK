#include "mbed.h"
#include "my_debug.h"
#include "my_test.h"

DEBUG_SERIAL(PA_2, PA_3, 115200);

USBMIDI     *usbmidi;
USBJoystick *usbjoy;

AnalogIn    slide_rotary_pot(PB_1);
SlideRotary pot(slide_rotary_pot);

AnalogIn    fsr_in(PB_4);
FSR         fsr(fsr_in);

AnalogIn    joystick_x(PB_3);
AnalogIn    joystick_y(PB_2);
DebounceIn  joystick_sw(PB_6);
Joystick    joy(joystick_x, joystick_y, joystick_sw);

PwmOut rgbled_r(PC_3);
PwmOut rgbled_g(PC_2);
PwmOut rgbled_b(PC_1);
RGBLed rgb(rgbled_r, rgbled_g, rgbled_b);

DigitalOut leds_sh(PE_10);
DigitalOut leds_st(PE_11);
DigitalOut leds_ds(PE_12);
LEDs leds(leds_sh, leds_st, leds_ds);

// DigitalOut keys_ce(PB_7);
DigitalOut keys_ck(PD_1);
DigitalOut keys_pl(PD_2);
DigitalIn  keys_si(PD_3);
Keys keys(keys_ck, keys_pl, keys_si);

QEI jwa(PA_0, PA_1, NC, 12, QEI::X2_ENCODING);  // Jog-Wheel Left
QEI jwb(PD_0, PB_5, NC, 12, QEI::X2_ENCODING);  // Jog-Wheel Right

class I2C2 : public I2C {
 public:
  I2C2(PinName sda, PinName scl) : I2C(sda, scl) {
    frequency(400000);
    start();
  };
};
I2C2          i2c(PE_0, PC_4);
BMA253        accel(i2c, (0x18 << 1));
SSD1306       oled(i2c, 0x78);


//=================================================================================
// Redefine
void mbed_die(void) {
  // This will be triggered while mbed-os goes wrong.
  // Call flow:　error() -> exit(1) -> mbed_die()
  while (1) {
    for (int i = 0; i < 4; ++i) {
      rgb.write(COLOR_RED);
      wait_ms(150);
      rgb.write(0);
      wait_ms(150);
    }
    for (int i = 0; i < 4; ++i) {
      rgb.write(COLOR_RED);
      wait_ms(400);
      rgb.write(0);
      wait_ms(400);
    }
  }
}


//=================================================================================
// Main
int main(void) {
  DEBUG_OUT("\n%s\n", MBED_CONF_APP_WELCOME_STRING);
  DEBUG_OUT("mbed-os-%d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

  /* Test functions */
#if (MBED_CONF_APP_RUN_MODE == 2)
  test_console(usbmidi, usbjoy, &pot, &fsr, &joy, &rgb, &leds, &keys, &jwa, &jwb, &accel, &oled);
  while(1);
#endif

  /* main loop */
  while (1) {
    // do what ever you want
  }
}
