#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"

#include "axp192.h"

#define TAG             "AXP192"
#define AXP192_I2C      I2C_NUM_0
#define AXP192_I2C_ADDR 0x68
#define AXP192_I2C_CLK  400000
#define AXP192_SDA      21
#define AXP192_SCL      22

#define AXP192_REG_POWER_OUTPUT   0x12
#define AXP192_REG_CHG_CONTROL    0x33

static uint8_t status[0];
static void axp192_i2c_write(uint8_t reg, uint8_t value);
static void axp192_i2c_read(uint8_t reg, uint8_t *value);
static void _axp192_reg_update(uint8_t reg, uint8_t value, uint8_t bit);
static void _axp192_reg_toggle(uint8_t reg, bool on, uint8_t bit);


void axp192_init(void)
{
  i2c_config_t i2c_config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = AXP192_SDA,
    .scl_io_num = AXP192_SCL,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = AXP192_I2C_CLK
  };
  ESP_ERROR_CHECK(i2c_param_config(AXP192_I2C, &i2c_config));
  ESP_ERROR_CHECK(i2c_driver_install(AXP192_I2C, I2C_MODE_MASTER, 0, 0, 0));
  axp192_read_status();
  if (axp192_has_bat()) {
    ESP_LOGI(TAG, "has battery");
  }
  if (axp192_is_charging()) {
    ESP_LOGI(TAG, "is charging state");
  }
  if (axp192_has_enough_charge_current()) {
    ESP_LOGI(TAG, "enough charge current");
  }
  if (axp192_is_bat_activated()) {
    ESP_LOGI(TAG, "is battery activated");
  }
}

void axp192_deinit(void)
{
  ESP_ERROR_CHECK(i2c_driver_delete(AXP192_I2C));
}

static void axp192_i2c_write(uint8_t reg, uint8_t value)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, AXP192_I2C_ADDR | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, reg, true);
  i2c_master_write_byte(cmd, value, true);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(AXP192_I2C, cmd, pdMS_TO_TICKS(10));
  i2c_cmd_link_delete(cmd);
}

static void axp192_i2c_read(uint8_t reg, uint8_t *value)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, AXP192_I2C_ADDR | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, reg, true);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, AXP192_I2C_ADDR | I2C_MASTER_READ, true);
  i2c_master_read_byte(cmd, value, I2C_MASTER_LAST_NACK);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(AXP192_I2C, cmd, pdMS_TO_TICKS(10));
  i2c_cmd_link_delete(cmd);
}

void axp192_read_status(void)
{
  axp192_i2c_read(0x01,status);
}

bool axp192_is_overheat(void)
{
  if (status[0] & 0x80) {
    return true;
  }
  return false;
}

bool axp192_is_charging(void)
{
  if (status[0] & 0x40) {
    return true;
  }
  return false;
}

bool axp192_has_bat(void)
{
  if (status[0] & 0x20) {
    return true;
  }
  return false;
}

bool axp192_is_bat_activated(void)
{
  if (status[0] & 0x08) {
    return true;
  }
  return false;
}

bool axp192_has_enough_charge_current(void)
{
  if (status[0] & 0x04) {
    return true;
  }
  return false;
}

axp192_switch_mode_t axp192_get_mode(void)
{
  if (status[0] & 0x02) {
    return AXP192_MODE_B;
  }
  return AXP192_MODE_A;
}

static void _axp192_reg_update(uint8_t reg, uint8_t value, uint8_t bit)
{
  uint8_t reg_value[2];
  axp192_i2c_read(reg, reg_value);
  value = value & bit;
  reg_value[1] = reg_value[0] | value;
  axp192_i2c_write(reg, reg_value[1]);
}

static void _axp192_reg_toggle(uint8_t reg, bool on, uint8_t bit)
{
  uint8_t reg_value[2];
  axp192_i2c_read(reg, reg_value);
  if (on) {
    reg_value[1] = reg_value[0] | bit;
  } else {
    reg_value[1] = reg_value[0] ^ bit;
  }
  if (reg_value[0] != reg_value[1]) {
    axp192_i2c_write(reg, reg_value[1]);
  }
}

void axp192_exten(bool on)
{
  _axp192_reg_toggle(AXP192_REG_POWER_OUTPUT, on, 0x40);
}

void axp192_dcdc1(bool on)
{
  _axp192_reg_toggle(AXP192_REG_POWER_OUTPUT, on, 0x01);
}

void axp192_dcdc2(bool on)
{
  _axp192_reg_toggle(AXP192_REG_POWER_OUTPUT, on, 0x10);
}

void axp192_dcdc3(bool on)
{
  _axp192_reg_toggle(AXP192_REG_POWER_OUTPUT, on, 0x02);
}

void axp192_ldo2(bool on)
{
  _axp192_reg_toggle(AXP192_REG_POWER_OUTPUT, on, 0x04);
}

void axp192_ldo3(bool on)
{
  _axp192_reg_toggle(AXP192_REG_POWER_OUTPUT, on, 0x08);
}

void axp192_chgen(bool on)
{
  _axp192_reg_toggle(AXP192_REG_CHG_CONTROL, on, 0x80);
}

void axp192_chg_set_target_vol(axp192_chg_vol_t vol)
{
  uint8_t value = 0x00;
  switch(vol) {
  case AXP192_VOL_4_1:
    value = 0x00;
    break;
  case AXP192_VOL_4_15:
    value = 0x20;
    break;
  case AXP192_VOL_4_36:
    value = 0x60;
    break;
  case AXP192_VOL_4_2:
  default:
    value = 0x40;
    break;
  }
  _axp192_reg_update(AXP192_REG_CHG_CONTROL, value, 0x60);
}

void axp192_chg_set_end_current(axp192_chg_end_cur_t cur)
{
  uint8_t value = 0x00;
  switch(cur) {
  case AXP192_CHG_END_CUR_10PER:
    value = 0x00;
    break;
  case AXP192_CHG_END_CUR_15PER:
    value = 0x10;
  default:
    break;
  }
  _axp192_reg_update(AXP192_REG_CHG_CONTROL, value, 0x10);
}

void axp192_chg_set_current(axp192_chg_cur_t cur)
{
  uint8_t value = 0x00;
  switch(cur) {
  case AXP192_CHG_CUR_100:
    value = 0x00;
    break;
  case AXP192_CHG_CUR_190:
    value = 0x01;
    break;
  case AXP192_CHG_CUR_280:
    value = 0x02;
    break;
  case AXP192_CHG_CUR_360:
    value = 0x03;
    break;
  case AXP192_CHG_CUR_450:
    value = 0x04;
    break;
  case AXP192_CHG_CUR_550:
    value = 0x05;
    break;
  case AXP192_CHG_CUR_630:
    value = 0x06;
    break;
  case AXP192_CHG_CUR_700:
    value = 0x07;
    break;
  }
  _axp192_reg_update(AXP192_REG_CHG_CONTROL, value, 0x07);
}

// 0x78 0x79 Battery voltage
uint16_t axp192_batt_get_vol(void)
{
  uint8_t v;
  uint16_t rtn = 0x0000;
  axp192_i2c_read(0x78, &v);
  rtn = v << 4;
  v = 0x00;
  axp192_i2c_read(0x79, &v);
  rtn |= v;
  return rtn;
}

// 0x7a 0x7b Battery charge current
uint16_t axp192_batt_get_chrg_current(void)
{
  uint8_t v;
  uint16_t rtn = 0x0000;
  axp192_i2c_read(0x7a, &v);
  rtn = (v << 5);
  v = 0x00;
  axp192_i2c_read(0x7b, &v);
  rtn |= v;
  return rtn;
}

// 0x7c 0x7d Battery discharge current
uint16_t axp192_batt_get_dischrg_current(void)
{
  uint8_t v;
  uint16_t rtn = 0x0000;
  axp192_i2c_read(0x7c, &v);
  rtn = (v << 5);
  v = 0x00;
  axp192_i2c_read(0x7d, &v);
  rtn |= v;
  return rtn;
}
