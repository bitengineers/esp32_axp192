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

#ifdef __cplusplus
}
#endif // __cplusplu
