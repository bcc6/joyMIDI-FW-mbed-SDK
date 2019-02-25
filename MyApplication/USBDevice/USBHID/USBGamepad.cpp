#include "stdint.h"
#include "USBGamepad.h"

bool USBGamepad::update(int16_t x, int16_t y, int16_t rx, int16_t ry, int16_t z,
                        uint16_t buttons, uint8_t hat) {
  _x = x;
  _y = y;
  _rx = rx;
  _ry = ry;
  _z = z;
  _buttons = buttons;
  _hat = hat;

  return update();
}

bool USBGamepad::update() {
  HID_REPORT report;

  report.data[0] = (_x >> 0) & 0xFF;
  report.data[1] = (_x >> 8) & 0xFF;
  report.data[2] = (_y >> 0) & 0xFF;
  report.data[3] = (_y >> 8) & 0xFF;
  report.data[4] = (_rx >> 0) & 0xFF;
  report.data[5] = (_rx >> 8) & 0xFF;
  report.data[6] = (_ry >> 0) & 0xFF;
  report.data[7] = (_ry >> 8) & 0xFF;
  report.data[8] = (_z >> 0) & 0xFF;
  report.data[9] = (_z >> 8) & 0xFF;
  uint16_t hat_buttons = ((uint16_t)_hat << 10) | _buttons;  // 4 + 10 bits
  report.data[10] = (hat_buttons >> 0) & 0xFF;
  report.data[11] = (hat_buttons >> 8) & 0x3F;
  report.data[12] = 0x00;  // Padding
  report.data[13] = 0x00;  // Padding
  report.length = 14;

  return send(&report);
}

bool USBGamepad::move(int16_t x, int16_t y, int16_t z) {
   _x = x;
   _y = y;
   _z = z;
   return update();
}

bool USBGamepad::rotate(int16_t rx, int16_t ry) {
   _rx = rx;
   _ry = ry;
   return update();
}

bool USBGamepad::buttons(uint16_t buttons) {
   _buttons = buttons;
   return update();
}

bool USBGamepad::hat(uint8_t hat) {
   _hat = hat;
   return update();
}

const uint8_t * USBGamepad::reportDesc() {    

         /* Refer to wired XBox360 gamepad */

         static uint8_t reportDescriptor[] = {
              USAGE_PAGE(1), 0x01,                  // Generic Desktop
              USAGE(1), 0x05,                       // Usage (Gamepad)
              COLLECTION(1), 0x01,                  // Application

                COLLECTION(1), 0x00,                // Physical
                  USAGE(1), 0x30,                   // X
                  USAGE(1), 0x31,                   // Y
                  LOGICAL_MINIMUM(1), 0x00,         // 0
                  LOGICAL_MAXIMUM(2), 0xFF, 0xFF,   // -1
                  // PHYSICAL_MINIMUM(1), 0x00,        // 0
                  // PHYSICAL_MAXIMUM(2), 0xFF, 0xFF,  // -1
                  REPORT_SIZE(1), 0x10,             // 16 bit
                  REPORT_COUNT(1), 0x02,            // 2 field
                  INPUT(1), 0x02,                   // Data,Var,Abs
                END_COLLECTION(0),

                COLLECTION(1), 0x00,                // Physical
                  USAGE(1), 0x33,                   // Rx
                  USAGE(1), 0x34,                   // Ry
                  LOGICAL_MINIMUM(1), 0x00,         // 0
                  LOGICAL_MAXIMUM(2), 0xFF, 0xFF,   // -1
                  // PHYSICAL_MINIMUM(1), 0x00,        // 0
                  // PHYSICAL_MAXIMUM(2), 0xFF, 0xFF,  // -1
                  REPORT_SIZE(1), 0x10,             // 16 bit
                  REPORT_COUNT(1), 0x02,            // 2 field
                  INPUT(1), 0x02,                   // Data,Var,Abs
                END_COLLECTION(0),

                COLLECTION(1), 0x00,                // Physical
                  USAGE(1), 0x32,                   // Z
                  LOGICAL_MINIMUM(1), 0x00,         // 0
                  LOGICAL_MAXIMUM(2), 0xFF, 0xFF,   // -1
                  // PHYSICAL_MINIMUM(1), 0x00,        // 0
                  // PHYSICAL_MAXIMUM(2), 0xFF, 0xFF,  // -1
                  REPORT_SIZE(1), 0x10,             // 16 bit
                  REPORT_COUNT(1), 0x01,            // 1 field
                  INPUT(1), 0x02,                   // Data,Var,Abs
                END_COLLECTION(0),

                USAGE_PAGE(1), 0x09,                // Button
                USAGE_MINIMUM(1), 0x01,             // Button 1
                USAGE_MAXIMUM(1), 0x0A,             // Button 10
                REPORT_SIZE(1), 0x01,               // 1 bit
                REPORT_COUNT(1), 0x0A,              // 10 field
                INPUT(1), 0x02,                     // Data,Var,Abs

                USAGE_PAGE(1), 0x01,                // Generic Desktop
                USAGE(1), 0x39,                     // Usage (Hat Switch)
                LOGICAL_MINIMUM(1), 0x01,           // 1
                LOGICAL_MAXIMUM(1), 0x08,           // 8
                // PHYSICAL_MINIMUM(1), 0x00,          // 0
                // PHYSICAL_MAXIMUM(2), 0x3B, 0x10,    // 4155 ???
                // UNIT(2), 0x0E, 0x00,                // Unit ???
                REPORT_SIZE(1), 0x04,               // 4 bit
                REPORT_COUNT(1), 0x01,              // 1 field
                INPUT(1), 0x42,                     // Data,Var,Abs,Null

                REPORT_SIZE(1), 0x02,               // 2 bit
                REPORT_COUNT(1), 0x01,              // 1 field
                INPUT(1), 0x03,                     // Cnst,Var,Abs
                
                REPORT_SIZE(1), 0x08,               // 8 bit
                REPORT_COUNT(1), 0x02,              // 2 field
                INPUT(1), 0x03,                     // Cnst,Var,Abs
      };

      reportLength = sizeof(reportDescriptor);
      return reportDescriptor;
}
