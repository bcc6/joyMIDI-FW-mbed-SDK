#ifndef SSD1306_H
#define SSD1306_H

#include "mbed.h"
#include "ssd1306_config.h"

#define SSD1306_IS_COMMAND 0x00
#define SSD1306_IS_DATA 0x40
#define SSD1306_IS_NOT_LAST 0x80
#define SSD1306_IS_LAST 0x00

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_SETPAGESTARTADDRESS 0xB0

#define SSD1306_MEMORYMODE 0x20

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

/**
 * SSD1306. A class for interacting with SSD1306 controlled 128x64 cheap OLED displays
 * 
 * Example of use:
 * @code
 * #include "mbed.h"
 * #include "ssd1306.h"
 * 
 * SSD1306 lcd (I2C_SDA, I2C_SCL); // assumes default I2C address of 0x78
 * AnalogIn knob (A0);             // potentiometer to analog pin A0
 *
 * int main()
 * {
 *   lcd.speed (SSD1306::Medium);  // set working frequency
 *   lcd.init();                   // initialize SSD1306
 *   lcd.cls();                    // clear frame buffer
 *   lcd.locate (3,1);             // set text cursor to line 3, column 1
 *   lcd.printf ("Hello, world!"); // print to frame buffer
 *   lcd.line (  6, 22, 114, 22, SSD1306::Normal); //
 *   lcd.line (114, 22, 114, 33, SSD1306::Normal); // Surrounds text with 
 *   lcd.line (114, 33,   6, 33, SSD1306::Normal); // a rectangle
 *   lcd.line (  6, 33,   6, 22, SSD1306::Normal); //
 *   lcd.fill (0, 0);              // fills screen outside rectangle
 *   lcd.redraw();                 // updates actual display transferring frame buffer over I2C bus
 *   while (1) {
 *     unsigned char level = 255*knob.read();  // reads pot. Scales to 0-255
 *     lcd.set_contrast (level);               // set contrast
 *     wait_ms (20);                           // waits a little to prevent excesive I2C traffic
 *   }
 * }
 * @endcode
 */

class SSD1306 {
 public:
  /**
   * PlotStyle. Defines how pixels being plotted interact with existing pixels
   * on the screen
   */
  enum PlotStyle {
    Normal,  /**< The point is set on the display */
    Inverse, /**< The point is erased on the display */
    Xor      /**< The point is erased on the display if it was already set, otherwise, it is set */
  };

  /**
   * I2CSpeed. Defines the speed of the I2C bus
   */
  enum I2CSpeed {
    Slow,   /**< I2C frequency is set to 100 kHz */
    Medium, /**< I2C frequency is set to 400 kHz */
    Fast    /**< I2C frequency is set to 1 MHz. Use it only with short connections to host */
  };

  /**
   * Creates an instance of a SSD1306 driver, specifying I2C pins to use
   *
   * @param sda I2C data line pin
   * @param scl I2C clock line pin
   */
  SSD1306(PinName sda, PinName scl, char ssd1306_addr = 0x78);

  /**
   * Creates an instance of a SSD1306 driver using an existing I2C object
   *
   * @param busi2c I2C object
   * @param ssd1306_addr I2C addr of SSD1306 controller
   */
  SSD1306(I2C &busi2c, char ssd1306_addr = 0x78);

  /**
   * Set the frequency of the I2C interface
   *
   * @param hz The bus frequency in hertz
   */
  void speed(I2CSpeed spd);

  /**
   * Initialize the SSD1306 controller
   */
  int init(void);

  /**
   * Print a character at the current text position. Updates the text position
   * and forces a scroll if at the end of the screen
   *
   * @param c ASCII code of the character to print (8-bit ASCII code, use
   * original IBM code page 437. No control codes.
   * @param refresh (optional) Force an actual screen redraw after scrolling
   */
  void putchar(char c, bool refresh = false);

  /**
   * Prints a NUL terminated string
   *
   * @param s C-style string (NUL terminated) to print.
   * @param refresh (optional) Force an actual screen redraw after the operation
   */
  void puts(char *s, bool refresh = false);

  /**
   * printf interface to SSD1306 controller
   *
   * @param fmt Format string.
   */
  void printf(const char *fmt, ...);

  /**
   * Change the text position
   *
   * @param row Text row (0-7) to print the next character
   * @param column Text column (0-15) to print the next character
   */
  void locate(char row, char column);

  /**
   * Redraws the physical display, sending the content of the display memory to
   * the SSD1306 controller using the I2C bus
   */
  void redraw(void);

  /**
   * Only redraw the selected partial
   *
   * @param start_page Start page (0-7)
   * @param start_col Start column, (0-127)
   * @param num_page Number of page to be update (1-8)
   * @param num_col Number of column to be update (1-128)
   */
  void redraw_partial(char start_page, char start_col, char num_page, char num_col);

  /**
   * Changes the contrast (actually, pixel brightness) of the screen
   *
   * @param v Contrast level (0-255)
   */
  void set_contrast(char v);

  /**
   * Clear screen and optionally, fills it with a predefined picture (in 128x64
   * OLED format, see display datasheet). Also resets the text position.
   *
   * @param bkground If not NULL, should point to a 1024 byte buffer with the
   * image to load to the display
   * @param refresh (optional) Force an actual screen redraw after the operation
   */
  void cls(char *bkground = NULL, bool refresh = false);

  /**
   * Fills with an Icon in 32x32 format
   *
   * @param icon Icon data 128bytes
   */
  void icon32x32(char *icon);

  /**
   * Plots a pixel.
   *
   * @param x x-coordinate (0-127) of the pixel. X coordinates go left to right.
   * @param y y-coordinate (0-63) of the pixel. Y coordinates go up to down.
   * @param mode. Plot style mode: Normal (pixel is drawn), Inverse (pixel is
   * erased), or Xor (pixel is erased if background position is already set,
   * otherwise is drawn)
   * @param refresh (optional) Force an actual screen redraw after plotting
   */
  void plot(char x, char y, PlotStyle mode, bool refresh = false);

  /**
   * Returns the state of a pixel coordinate from screen
   *
   * @param x x-coordinate (0-127) of the pixel. X coordinates go left to right.
   * @param y y-coordinate (0-63) of the pixel. Y coordinates go up to down.
   * @return true if the pixel was set, or false otherwise
   */
  bool point(char x, char y);

  /**
   * Draws a line using the Bresenham algorithm
   *
   * @param x0 x-coordinate (0-127) of the starting point
   * @param y0 y-coordinate (0-63) of the starting point
   * @param x1 x-coordinate (0-127) of the ending point
   * @param y1 y-coordinate (0-63) of the ending point
   * @param mode. Plot style mode: Normal (pixel is drawn), Inverse (pixel is
   * erased), or Xor (pixel is erased if background position is already set,
   * otherwise is drawn)
   * @param refresh (optional) Force an actual screen redraw after drawing the
   * line
   */
  void line(char x0, char y0, char x1, char y1, PlotStyle mode, bool refresh = false);

  /**
   * Draws a circle
   *
   * @param x0 x-coordinate (0-127) of the center point
   * @param y0 y-coordinate (0-63) of the center point
   * @param r radius of the circle
   * @param mode. Plot style mode: Normal (pixel is drawn), Inverse (pixel is
   * erased), or Xor (pixel is erased if background position is already set,
   * otherwise is drawn)
   * @param refresh (optional) Force an actual screen redraw after drawing the
   * shape
   */
  void circle(char x0, char y0, char r, PlotStyle mode, bool refresh = false);

  /**
   * Flood fills an area surrounded by the screen edge and/or a closed shape.
   *
   * @param x x-coordinate (0-127) of a point inside the area to fill, not
   * plotted
   * @param y y-coordinate (0-63) of a point inside the area to fill, not
   * plotted
   * @param refresh (optional) Force an actual screen redraw after the operation
   */
  void fill(char x, char y, bool refresh = false);

 protected:
  I2C *bus;                 // I2C object
  char ssd1306_i2c_addr;    // I2C address of SSD1306
  char *fb;                 // Frame buffer
  int idxfb;                // Index of frame buffer
  char *xb;                 // Transfer buffer

  int command(char c);
  int data(char d);
  int command_data(char c, char c_or_d, char lastitem);
};

#endif
