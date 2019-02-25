#ifndef BMA253_H
#define BMA253_H

#include "mbed.h"

#define BMA_REG_BGW_CHIPID    0x00
#define BMA_REG_ACCD_X_LSB    0x02
#define BMA_REG_ACCD_X_MSB    0x03
#define BMA_REG_ACCD_Y_LSB    0x04
#define BMA_REG_ACCD_Y_MSB    0x05
#define BMA_REG_ACCD_Z_LSB    0x06
#define BMA_REG_ACCD_Z_MSB    0x07
#define BMA_REG_ACCD_TEMP     0x08
#define BMA_REG_INT_STATUS_0  0x09
#define BMA_REG_INT_STATUS_1  0x0A
#define BMA_REG_INT_STATUS_2  0x0B
#define BMA_REG_INT_STATUS_3  0x0C
#define BMA_REG_FIFO_STATUS   0x0E
#define BMA_REG_PMU_RANGE     0x0F
#define BMA_REG_PMU_BW        0x10
#define BMA_REG_PMU_LPW       0x11
#define BMA_REG_PMU_LOW_POWER 0x12
#define BMA_REG_ACCD_HBW      0x13
#define BMA_REG_BGW_SOFTRESET 0x14
#define BMA_REG_INT_EN_0      0x16
#define BMA_REG_INT_EN_1      0x17
#define BMA_REG_INT_EN_2      0x18
#define BMA_REG_INT_MAP_0     0x19
#define BMA_REG_INT_MAP_1     0x1A
#define BMA_REG_INT_MAP_2     0x1B
#define BMA_REG_INT_SRC       0x1E
#define BMA_REG_INT_OUT_CTRL  0x20
#define BMA_REG_INT_RST_LATCH 0x21
#define BMA_REG_INT_0         0x22
#define BMA_REG_INT_1         0x23
#define BMA_REG_INT_2         0x24
#define BMA_REG_INT_3         0x25
#define BMA_REG_INT_4         0x26
#define BMA_REG_INT_5         0x27
#define BMA_REG_INT_6         0x28
#define BMA_REG_INT_7         0x29
#define BMA_REG_INT_8         0x2A
#define BMA_REG_INT_9         0x2B
#define BMA_REG_INT_A         0x2C
#define BMA_REG_INT_B         0x2D
#define BMA_REG_INT_C         0x2E
#define BMA_REG_INT_D         0x2F
#define BMA_REG_FIFO_CONFIG_0 0x30
#define BMA_REG_PMU_SELF_TEST 0x32
#define BMA_REG_TRIM_NVM_CTRL 0x33
#define BMA_REG_BGW_SPI3_WDT  0x34
#define BMA_REG_OFC_CTRL      0x36
#define BMA_REG_OFC_SETTING   0x37
#define BMA_REG_OFC_OFFSET_X  0x38
#define BMA_REG_OFC_OFFSET_Y  0x39
#define BMA_REG_OFC_OFFSET_Z  0x3A
#define BMA_REG_TRIM_GP0      0x3B
#define BMA_REG_TRIM_GP1      0x3C
#define BMA_REG_FIFO_CONFIG_1 0x3E
#define BMA_REG_FIFO_DATA     0x3F

enum BMA_RangeSensitivity {
  BMA_RANGE_2G_SENS  = 1024,
  BMA_RANGE_4G_SENS  = 512,
  BMA_RANGE_8G_SENS  = 256,
  BMA_RANGE_16G_SENS = 128,
};

class BMA253 {
 public:

  /** Create a BMA253
   *
   *  @param i2c I2C object
   *  @param i2c_addr I2C address
   */
  BMA253(I2C &i2c, int i2c_addr = (0x18 << 1));

  /**
   * Set the frequency of the I2C interface
   *
   * @param hz The bus frequency in hertz
   */
  void speed(int hz);

  /** Set sensitivity
   *
   * @param s BMA_RangeSensitivity
   */
  void set_sensitivity(int16_t s);

  /** Get device ID
   *
   * @returns ID(0xFA)
   */
  uint8_t get_device_id();

  /** Get raw data
   *
   * @param xyz Output X, Y and Z raw data, respectively range [-2048, 2047]
   */
  void get_raw(int16_t *xyz);

  /** Get vector
   *
   * @param xyz Output X, Y and Z vector, unit: G
   */
  void get_vector(float *xyz);

  /** calibrate
   *
   * @param xyz Input X, Y and Z raw data
   */
  void calibrate(int16_t *xyz);

 private:
  I2C &_i2c;

  uint8_t _i2c_addr;
  int16_t _sensitivity;
  int16_t _calib_xyz[3];

  void _reg_write(char reg, char val);
  void _reg_read(char reg, char *val, int len);
};

#endif