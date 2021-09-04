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

  void axp192_init(void);
  void axp192_deinit(void);

  void axp192_read_status(void);
  bool axp192_is_overheat(void);
  bool axp192_is_charging(void);
  bool axp192_has_bat(void);
  bool axp192_is_bat_activated(void);
  bool axp192_has_enough_charge_current(void);
  axp192_switch_mode_t axp192_get_mode(void);

  void axp192_exten(bool on);
  void axp192_dcdc1(bool on);
  void axp192_dcdc2(bool on);
  void axp192_dcdc3(bool on);
  void axp192_ldo2(bool on);
  void axp192_ldo3(bool on);

#ifdef __cplusplus
}
#endif // __cplusplu
