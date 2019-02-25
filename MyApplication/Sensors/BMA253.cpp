#include "BMA253.h"

BMA253::BMA253(I2C &i2c, int i2c_addr)
    : _i2c(i2c),
      _i2c_addr(i2c_addr),
      _sensitivity(BMA_RANGE_2G_SENS),
      _calib_xyz() {

  _reg_write(BMA_REG_BGW_SOFTRESET, 0xB6);  // Softreset
  wait_ms(2);
  _reg_write(BMA_REG_PMU_RANGE, 0x03);  // Range (+/-)2G
  _reg_write(BMA_REG_PMU_BW, 0x0C);     // Data filter bandwidth 125Hz (Update time 4ms)
  _reg_write(BMA_REG_PMU_LPW, 0x00);    // Power mode Normal
  _reg_write(BMA_REG_INT_EN_0, 0x00);   // Disable all interrupts
  _reg_write(BMA_REG_INT_EN_1, 0x00);
  _reg_write(BMA_REG_INT_EN_2, 0x00);
}

void BMA253::speed(int hz) { _i2c.frequency(hz); }

void BMA253::set_sensitivity(int16_t s) {
  _sensitivity = s;
  if (s == BMA_RANGE_2G_SENS)  _reg_write(BMA_REG_PMU_RANGE, 0x03);
  if (s == BMA_RANGE_4G_SENS)  _reg_write(BMA_REG_PMU_RANGE, 0x05);
  if (s == BMA_RANGE_8G_SENS)  _reg_write(BMA_REG_PMU_RANGE, 0x08);
  if (s == BMA_RANGE_16G_SENS) _reg_write(BMA_REG_PMU_RANGE, 0x0C);
  wait_ms(200);
}

uint8_t BMA253::get_device_id() {
  char id;
  _reg_read(BMA_REG_BGW_CHIPID, &id, 1);
  return id;
}

void BMA253::get_raw(int16_t *xyz) {
  char val[6];
  _reg_read(BMA_REG_ACCD_X_LSB, val, 6);

  /* Convert ADC sign-12bits to sign-16bits */
  int16_t x = ((int16_t)val[1] << 4) | ((val[0] & 0xF0) >> 4);
  if (x & 0x800) x |= 0xF000;  // Padding sign bits if negative
  int16_t y = ((int16_t)val[3] << 4) | ((val[2] & 0xF0) >> 4);
  if (y & 0x800) y |= 0xF000;  // Padding sign bits if negative
  int16_t z = ((int16_t)val[5] << 4) | ((val[4] & 0xF0) >> 4);
  if (z & 0x800) z |= 0xF000;  // Padding sign bits if negative

  xyz[0] = y;
  xyz[1] = x;
  xyz[2] = z;
}

void BMA253::get_vector(float *xyz) {
  int16_t raw_xyz[3];
  get_raw(raw_xyz);

  /* Calibration */
  raw_xyz[0] -= _calib_xyz[0];
  raw_xyz[1] -= _calib_xyz[1];
  raw_xyz[2] -= _calib_xyz[2];
  
  /* Convert raw to vector */
  xyz[0] = (float)raw_xyz[0] / (float)_sensitivity;
  xyz[1] = (float)raw_xyz[1] / (float)_sensitivity;
  xyz[2] = (float)raw_xyz[2] / (float)_sensitivity;
}

void BMA253::calibrate(int16_t *xyz) {
  _calib_xyz[0] = xyz[0];
  _calib_xyz[1] = xyz[1];
  _calib_xyz[2] = xyz[2] + _sensitivity;
}

void BMA253::_reg_write(char reg, char val) {
  char data[2];
  data[0] = reg;
  data[1] = val;
  _i2c.write(_i2c_addr, data, 2, 0);
}

void BMA253::_reg_read(char reg, char *val, int len) {
  _i2c.write(_i2c_addr, &reg, 1, 0);
  _i2c.read(_i2c_addr, val, len);
}