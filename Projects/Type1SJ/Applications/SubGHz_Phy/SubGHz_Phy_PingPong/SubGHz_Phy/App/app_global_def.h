
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_GLOBAL_H
#define APP_GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAINLED_FEATURE true
#define RGBLED_FEATURE false

#define SLEEP_FEATURE false

#define HEARTBEAT_FEATURE true
#define IMU_FEATURE true
#define BATTERY_FUTURE true
#define LORA_FEATURE true
#define WATER_ALERT_FEATURE true
#define BUTTON_ALERT_FEATURE true
#define FALLEN_ALERT_FEATURE true
#define DROP_EVANT_FEATURE true

#define BIG_FALL_PROBABILITY 20
#define HEARTBEAT_INTERVAL 12000 // 12s

#define NOCHARGING 0
#define INCHARGING 1
#define FULLCHARGED 2
#define CHARGE_FAULT -1
extern int8_t charging_state;

#define TEST_FORCE_FALL false // true/false for enable/disable auto drop
#define BATTERY_FLAT_THRESHOLD 3200 // battery ADC threshold for shutdown
#define BATTERY_NORMAL_THRESHOLD BATTERY_FLAT_THRESHOLD + 200 // battery ADC threshold for shutdown

#ifdef __cplusplus
}
#endif

#endif /* APP_ALERT_HANDLER_H */

