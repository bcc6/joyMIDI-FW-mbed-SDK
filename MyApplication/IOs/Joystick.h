#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "mbed.h"
#include "my_utility.h"
#include "DebounceIn.h"

#define RADIUS_NOISE_FLOOR (0.1f)
#define RAD2DEG (57.2957795131f)

enum Direction {
  CENTER,  // 0
  N,       // 1
  NE,      // 2
  E,       // 3
  SE,      // 4
  S,       // 5
  SW,      // 6
  W,       // 7
  NW       // 8
};

struct Vector2D {
  float x;
  float y;
};

struct Polar2D {
  float r;  // Radius (Magnitude)
  float t;  // Theta (Angle)
};

class Joystick {
 public:
  /** Create a Joystick
   *
   *  @param x X-axis
   *  @param y Y-axis
   *  @param sw Switch
   */
  Joystick(AnalogIn &x, AnalogIn &y, DebounceIn &sw);

  /** calibrate the linear
   *
   * @param xmin The mininal value of X-axis (Default 0.0)
   * @param xmid The middle  value of X-axis (Default 0.5)
   * @param xmax The maximum value of X-axis (Default 1.0)
   * @param ymin The mininal value of Y-axis (Default 0.0)
   * @param ymid The middle  value of Y-axis (Default 0.5)
   * @param ymax The maximum value of Y-axis (Default 1.0)
   */
  void calibrate(float xmin, float xmid, float xmax, float ymin, float ymid, float ymax);

  /** Get raw data
   *
   * @returns Vector2D
   *    X  range [-1.0, 1.0]
   *    Y  range [-1.0, 1.0]
   */
  Vector2D get_raw();

  /** Get vector coord
   *
   * @returns Vector2D
   *    X  range [-1.0, 1.0]
   *    Y  range [-1.0, 1.0]
   */
  Vector2D get_vector();

  /** Get mapped-vector coord (Square to circle)
   *
   * @returns Vector2D
   *    X  range [-1.0, 1.0]
   *    Y  range [-1.0, 1.0]
   */
  Vector2D get_mapped_vector();

  /** Get polar coord
   *
   * @returns Polar2D
   *    r range [0.0, 1.0]
   *    t range [0.0, 360.0], -1 mean center, North(0 degrees), clockwise
   */
  Polar2D get_polar();

  /** Get direction
   *
   * @returns enum Direction
   */
  Direction get_direction();

  /** Check if the switch pressed
   *
   * @returns
   *    true pressed
   */
  bool is_pressed();

 private:
  AnalogIn &_x;
  AnalogIn &_y;
  DebounceIn &_sw;

  float _xmin, _xmid, _xmax;
  float _ymin, _ymid, _ymax;
};

#endif