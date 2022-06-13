
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_LED_STATE_H
#define APP_LED_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define BOOST_RISE_TIME 1
#define BOOST_FALL_TIME 1

enum red_led_state {
	RED_OFF,
	RED_ALERT_ON,
	RED_HEARTBEAT
};

void turn_red_LED(bool turnOn);
void red_led_proc(void);
void heartbeat_red_blink(void);

extern bool red_blink;

#ifdef __cplusplus
}
#endif

#endif /* APP_LED_STATE_H */

