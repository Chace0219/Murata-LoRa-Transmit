/**
 *
 */
#include "stm32l0xx_hal.h"
#include "platform.h"
#include "main.h"
#include "sys_app.h"

#include "app_global_def.h"
#include "app_led_state.h"
#include "app_alert_handler.h"

/** external variables */
bool red_blink = false;

/** static variables */
static uint32_t led_transition_time = 0;
static uint8_t curr_red_state = RED_OFF;

/** */
void turn_red_LED(bool turnOn){
#if MAINLED_FEATURE
	LED_On(RED_LED);
#endif
	red_blink = turnOn;
}

/************************************************************************/
/*
	RGB LED action
	- for heartbeat, it needs to blink for 25ms with white color
	- for wet alert, 350ms off, 50ms on with blue color (0, 0, 127), blinking for 15s
	- for button alert, 750ms off, 50ms on with red (127, 0, 0), blinking for 5s
	- for drop event, 950ms off, 50ms on with yellow (31, 31, 0), blinking only once
*/
/************************************************************************/

void red_led_proc(void){
	switch(curr_red_state){
		case RED_OFF:
			if(in_alert(WET_ALERT) && charging_state == NOCHARGING){
				if(HAL_GetTick() - led_transition_time > 350){
					turn_red_LED(true);
					#if RGBLED_FEATURE
					port_pin_set_output_level(LED_PWR_PIN, LOW);
					set_all_pixels(0, 0, 127);
					delay_ms(BOOST_RISE_TIME);
					draw_pixels();
					#endif
					curr_red_state = RED_ALERT_ON;
					led_transition_time = HAL_GetTick();
				}
			}
			else if(in_alert(PANIC_ALERT)){
				if(HAL_GetTick() - led_transition_time > 750){
					turn_red_LED(true);
				#if RGBLED_FEATURE
					port_pin_set_output_level(LED_PWR_PIN, LOW);
					set_all_pixels(127, 0, 0);
					delay_ms(BOOST_RISE_TIME);
					draw_pixels();
				#endif
					curr_red_state = RED_ALERT_ON;
					led_transition_time = HAL_GetTick();
				}
			}
			else if(in_alert(DROP_EVENT)){
				if(HAL_GetTick() - led_transition_time > 950){
					if(!drop_led_blinked){
					#if RGBLED_FEATURE
						// blinking yellow color
						port_pin_set_output_level(LED_PWR_PIN, LOW);
						set_all_pixels(31, 31, 0);
						delay_ms(BOOST_RISE_TIME);
						draw_pixels();
					#endif
						drop_led_blinked = true;
					}
					curr_red_state = RED_ALERT_ON;
					led_transition_time = HAL_GetTick();
				}
			}
		break;

		case RED_ALERT_ON:
			if(in_alert(WET_ALERT)){
				if(HAL_GetTick() - led_transition_time > 50){
					turn_red_LED(false);
				#if RGBLED_FEATURE
					set_all_pixels(0, 0, 0);
					draw_pixels();
					port_pin_set_output_level(LED_DATA_PIN, LOW);
					delay_ms(BOOST_FALL_TIME);
					port_pin_set_output_level(LED_PWR_PIN, HIGH);
				#endif
					curr_red_state = RED_OFF;
					led_transition_time = HAL_GetTick();
				}
			}
			else if(in_alert(PANIC_ALERT)){
				if(HAL_GetTick() - led_transition_time > 50){
					turn_red_LED(false);
					#if RGBLED_FEATURE
					set_all_pixels(0, 0, 0);
					draw_pixels();
					port_pin_set_output_level(LED_DATA_PIN, LOW);
					delay_ms(BOOST_FALL_TIME);
					port_pin_set_output_level(LED_PWR_PIN, HIGH);
					#endif
					curr_red_state = RED_OFF;
					led_transition_time = HAL_GetTick();
				}
			}
			else if(in_alert(DROP_EVENT)){
				// turn off with 100ms
				if(HAL_GetTick() - led_transition_time > 50){

				#if RGBLED_FEATURE
					set_all_pixels(0, 0, 0);
					draw_pixels();
					port_pin_set_output_level(LED_DATA_PIN, LOW);
					delay_ms(BOOST_FALL_TIME);
					port_pin_set_output_level(LED_PWR_PIN, HIGH);
				#endif
					curr_red_state = RED_OFF;
					led_transition_time = HAL_GetTick();
				}
			}
			else{
				turn_red_LED(false);
			#if RGBLED_FEATURE
				set_all_pixels(0, 0, 0);
				draw_pixels();
				port_pin_set_output_level(LED_DATA_PIN, LOW);
				delay_ms(BOOST_FALL_TIME);
				port_pin_set_output_level(LED_PWR_PIN, HIGH);
			#endif
				curr_red_state = RED_OFF;
				led_transition_time = HAL_GetTick();
			}
		break;

		case RED_HEARTBEAT:
			if(HAL_GetTick() - led_transition_time > 25){
				curr_red_state = RED_OFF;
				turn_red_LED(false);
			#if RGBLED_FEATURE
				set_all_pixels(0, 0, 0);
				draw_pixels();
				port_pin_set_output_level(LED_DATA_PIN, LOW);
				delay_ms(BOOST_FALL_TIME);
				port_pin_set_output_level(LED_PWR_PIN, HIGH);
			#endif
				led_transition_time = HAL_GetTick();
			}
		break;

		default:
		break;
	}
}

void heartbeat_red_blink(void){
	if(in_alert(NO_ALERT)){

	#if RGBLED_FEATURE
		// blinking white color
		port_pin_set_output_level(LED_PWR_PIN, LOW);
		set_all_pixels(31, 31, 31);
		delay_ms(BOOST_RISE_TIME);
		draw_pixels();
	#endif
		led_transition_time = HAL_GetTick();
		curr_red_state = RED_HEARTBEAT;
	}
}
