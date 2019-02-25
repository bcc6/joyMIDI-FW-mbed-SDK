
#include "Joystick.h"

Joystick::Joystick(AnalogIn &x, AnalogIn &y, DebounceIn &sw)
    : _x(x),
      _y(y),
      _sw(sw),
      _xmin(0.0f),
      _xmid(0.5f),
      _xmax(1.0f),
      _ymin(0.0f),
      _ymid(0.5f),
      _ymax(1.0f) {}

void Joystick::calibrate(float xmin, float xmid, float xmax, float ymin, float ymid, float ymax) {
  _xmin = xmin;
  _xmid = xmid;
  _xmax = xmax;
  _ymin = ymin;
  _ymid = ymid;
  _ymax = ymax;
}

Vector2D Joystick::get_raw() {
  Vector2D r = {_x, _y};
  return r;
}

Vector2D Joystick::get_vector() {
  Vector2D r = get_raw();
  /* Convert range [0.0, 1.0] to [-1.0, 1.0] */
  float x, y;
  if (r.x <= _xmid) {
    x = map(r.x, _xmin, _xmid, -1.0f, 0.0f);
  } else {
    x = map(r.x, _xmid, _xmax, 0.0f, 1.0f);
  }
  if (r.y <= _ymid) {
    y = map(r.y, _ymin, _ymid, -1.0f, 0.0f);
  } else {
    y = map(r.y, _ymid, _ymax, 0.0f, 1.0f);
  }
  Vector2D v = {-x, -y};  // Inverse X and Y-axis
  return v;
}

Vector2D Joystick::get_mapped_vector() {
  Vector2D v = get_vector();
  /* http://mathproofs.blogspot.tw/2005/07/mapping-square-to-circle.html */
  float x = v.x * sqrt(1.0f - pow(v.y, 2.0f) / 2.0f);
  float y = v.y * sqrt(1.0f - pow(v.x, 2.0f) / 2.0f);
  Vector2D m = {x, y};
  return m;
}

Polar2D Joystick::get_polar() {
  Vector2D m = get_mapped_vector();
  /* North(0 degrees), clockwise */
  float x = m.y;
  float y = m.x;
  float radius = sqrt(x * x + y * y);  // pythagoras
  float theta = RAD2DEG * atan2(y, x);
  /* Convert theta -180~180 to 0~360 */
  if (theta < 0.0f) {
    theta += 360.0f;
  }
  /* Noise floor */
  if (radius < RADIUS_NOISE_FLOOR) {
    radius = 0.0f;
    theta = -1.0f;
  }
  Polar2D p = {radius, theta};
  return p;
}

Direction Joystick::get_direction() {
  float theta = get_polar().t;
  Direction d;
  if (theta < 0.0f) {
    d = CENTER;
  } else if (theta < 22.5f) {
    d = N;
  } else if (theta < 67.5f) {
    d = NE;
  } else if (theta < 112.5f) {
    d = E;
  } else if (theta < 157.5f) {
    d = SE;
  } else if (theta < 202.5f) {
    d = S;
  } else if (theta < 247.5f) {
    d = SW;
  } else if (theta < 292.5f) {
    d = W;
  } else if (theta < 337.5f) {
    d = NW;
  } else {
    d = N;
  }
  return d;
}

bool Joystick::is_pressed() { return _sw ? false : true; }