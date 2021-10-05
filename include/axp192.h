#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

  typedef enum {
    AXP192_MODE_A,
    AXP192_MODE_B
  } axp192_switch_mode_t;

  typedef struct _axp192_status {
  } axp192_status_t;

  typedef enum {
    AXP192_VOL_4_1,
    AXP192_VOL_4_15,
    AXP192_VOL_4_2,
    AXP192_VOL_4_36
  } axp192_chg_vol_t;

  typedef enum {
    AXP192_CHG_END_CUR_10PER,
    AXP192_CHG_END_CUR_15PER,
  } axp192_chg_end_cur_t;

  typedef enum {
    AXP192_CHG_CUR_100,
    AXP192_CHG_CUR_190,
    AXP192_CHG_CUR_280,
    AXP192_CHG_CUR_360,
    AXP192_CHG_CUR_450,
    AXP192_CHG_CUR_550,
    AXP192_CHG_CUR_630,
    AXP192_CHG_CUR_700,
  } axp192_chg_cur_t;

  typedef enum {
    AXP192_ADC_SAMPLERATE_25,
    AXP192_ADC_SAMPLERATE_50,
    AXP192_ADC_SAMPLERATE_100,
    AXP192_ADC_SAMPLERATE_200,
  } axp192_adc_samplerate_t;

  typedef enum {
    AXP192_TS_OUT_CUR_20U,
    AXP192_TS_OUT_CUR_40U,
    AXP192_TS_OUT_CUR_60U,
    AXP192_TS_OUT_CUR_80U,
  } axp192_ts_out_cur_t;

  typedef enum {
    AXP192_TS_FUNCTION_BATT_TEMP,
    AXP192_TS_FUNCTION_EXT_IN,
  } axp192_ts_func_t;

  typedef enum {
    AXP192_TS_OUT_MODE_OFF,
    AXP192_TS_OUT_MODE_CHARGING,
    AXP192_TS_OUT_MODE_ADCSAMPLING,
    AXP192_TS_OUT_MODE_ON,
  } axp192_ts_out_mode_t;

  typedef enum {
    AXP192_GPIO_OUTPUT,
    AXP192_GPIO_INPUT,
    AXP192_GPIO_OUTPUT_PWM,
    AXP192_GPIO_INPUT_ADC,
    AXP192_GPIO_OUTPUT_LOW,
    AXP192_GPIO_FLOATING
  } axp192_gpio_func_t;

  void axp192_init(void);
  void axp192_deinit(void);

  void axp192_read_status(void);
  bool axp192_is_overheat(void);
  bool axp192_is_charging(void);
  bool axp192_has_bat(void);
  bool axp192_is_bat_activated(void);
  bool axp192_has_enough_charge_current(void);
  axp192_switch_mode_t axp192_get_mode(void);

  // 0x12 POWER_OUTPUT Register
  void axp192_exten(bool on);
  void axp192_dcdc1(bool on);
  void axp192_dcdc2(bool on);
  void axp192_dcdc3(bool on);
  void axp192_ldo2(bool on);
  void axp192_ldo3(bool on);

  // 0x33 CHRG_CONTROL Register
  void axp192_chgen(bool on);
  void axp192_chg_set_target_vol(axp192_chg_vol_t vol);
  void axp192_chg_set_end_current(axp192_chg_end_cur_t cur);
  void axp192_chg_set_current(axp192_chg_cur_t cur);

  void axp192_chg_ext_pass_en(bool on);

  // 0x78 0x79 Battery voltage
  uint16_t axp192_batt_vol_get(void);
  // 0x7a 7b Battery charge current
  uint16_t axp192_batt_chrg_cur_get(void);
  // 0x7c 7d Battery discharge current
  uint16_t axp192_batt_dischrg_cur_get(void);

  // 0x82 ADC Enable 1
  void axp192_adc_batt_vol_en(bool on);
  void axp192_adc_batt_cur_en(bool on);
  void axp192_adc_ac_vol_en(bool on);
  void axp192_adc_ac_cur_en(bool on);
  void axp192_adc_vbus_vol_en(bool on);
  void axp192_adc_vbus_cur_en(bool on);
  void axp192_adc_aps_en(bool on);
  void axp192_adc_ts_pin_en(bool on);

  // 0x83 ADC Enable 2
  void axp192_adc_temp_en(bool on);
  void axp192_adc_gpio0_en(bool on);
  void axp192_adc_gpio1_en(bool on);
  void axp192_adc_gpio2_en(bool on);
  void axp192_adc_gpio3_en(bool on);

  // 0x84 ADC samplerate, TS pin control
  void axp192_adc_samplerate_set(axp192_adc_samplerate_t rate);
  void axp192_ts_cur_set(axp192_ts_out_cur_t output);
  void axp192_ts_function_set(axp192_ts_func_t func);
  void axp192_ts_output_en(axp192_ts_out_mode_t mode);

  void axp192_gpio1_change(axp192_gpio_func_t func);
  void axp192_gpio2_change(axp192_gpio_func_t func);

#ifdef __cplusplus
}
#endif // __cplusplus
