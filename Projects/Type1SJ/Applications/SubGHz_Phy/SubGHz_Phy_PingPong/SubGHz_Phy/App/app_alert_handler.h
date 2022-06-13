
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_ALERT_HANDLER_H
#define APP_ALERT_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


enum PunchAlert {
	PANIC_ALERT,
	WET_ALERT,
	DROP_EVENT,
	NO_ALERT,
};

extern enum PunchAlert current_alert;

void alert_proc(void);
void transition_alert(enum PunchAlert target);
bool in_alert(enum PunchAlert alert);

extern bool panic_alert_triggered;
extern uint32_t panic_alert_trigger_time;
extern bool wet_alert_triggered;
extern uint32_t wet_alert_trigger_time;
extern bool drop_event_triggered;
extern uint32_t drop_event_trigger_time;

extern uint8_t alert_state;
extern uint8_t pre_alert_state;

extern bool drop_led_blinked;

#ifdef __cplusplus
}
#endif

#endif /* APP_ALERT_HANDLER_H */

