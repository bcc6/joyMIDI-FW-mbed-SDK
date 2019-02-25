#ifndef USBGAMEPAD_H
#define USBGAMEPAD_H

#include "USBHID.h"

#define REPORT_ID_GAMEPAD  5

class USBGamepad : public USBHID {
 public:
  /**
   *   Constructor
   *
   * @param vendor_id Your vendor_id
   * @param product_id Your product_id
   * @param product_release Your product_release
   */
  USBGamepad(uint16_t vendor_id = 0x045E/* 0x1234 */, uint16_t product_id = 0x028E/* 0xDEAD */,
             uint16_t product_release = 0x0001, int waitForConnect = true)
      : USBHID(0, 0, vendor_id, product_id, product_release, false),
        _x(0),
        _y(0),
        _rx(0),
        _ry(0),
        _z(0),
        _buttons(0),
        _hat(0) {
    connect(waitForConnect);
  };

  /**
   * Write state of the gamepad
   *
   * @param x x-axis position
   * @param y y-axis position
   * @param rx x-rotation position
   * @param ry y-rotation position
   * @param z z-axis position
   * @param buttons buttons state
   * @param hat hat state 0(CENTER), 1(N), 2(NE), 3(E), 4(SE), ..., 8(NW)
   * @returns true if there is no error, false otherwise
   */
  bool update(int16_t x, int16_t y, int16_t rx, int16_t ry, int16_t z,
              uint16_t buttons, uint8_t hat);

  /**
   * Write state of the gamepad
   *
   * @returns true if there is no error, false otherwise
   */
  bool update();

  /**
   * Move the cursor to (x, y, z)
   *
   * @param x x-axis position
   * @param y y-axis position
   * @param z z-axis position
   * @returns true if there is no error, false otherwise
   */
  bool move(int16_t x, int16_t y, int16_t z);

  /**
   * Rotate the x/y-axis
   *
   * @param rx x-axis rotation
   * @param ry y-axis rotation
   * @returns true if there is no error, false otherwise
   */
  bool rotate(int16_t rx, int16_t ry);

  /**
   * Press one or several buttons
   *
   * @param buttons buttons state
   * @returns true if there is no error, false otherwise
   */
  bool buttons(uint16_t buttons);

  /**
   * Press hat
   *
   * @param hat hat state
   * @returns true if there is no error, false otherwise
   */
  bool hat(uint8_t hat);

  /**
   * To define the report descriptor. Warning: this method has to store the
   * length of the report descriptor in reportLength.
   *
   * @returns pointer to the report descriptor
   */
  virtual const uint8_t* reportDesc();

 private:
  int16_t _x;
  int16_t _y;
  int16_t _rx;
  int16_t _ry;
  int16_t _z;
  uint16_t _buttons;  // 10 bits
  uint8_t _hat;       // 4 bits
};

#endif