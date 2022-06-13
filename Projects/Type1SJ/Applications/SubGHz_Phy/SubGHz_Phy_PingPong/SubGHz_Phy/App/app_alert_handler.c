/**
 *
 */
#include "stm32l0xx_hal.h"

#include "platform.h"
#include "app_global_def.h"
#include "app_alert_handler.h"
#include "app_radio_handler.h"
#include "app_led_state.h"
#include "subghz_phy_app.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

bool panic_alert_triggered = false;
uint32_t panic_alert_trigger_time = 0;
bool wet_alert_triggered = false;
uint32_t wet_alert_trigger_time = 0;
bool drop_event_triggered = false;
uint32_t drop_event_trigger_time = 0;

uint8_t alert_state = 0;
uint8_t pre_alert_state = 0;

bool drop_led_blinked = false;

enum PunchAlert current_alert = NO_ALERT;
enum PunchAlert prev_alert = NO_ALERT;
static uint32_t transition_time = 0;

void alert_proc(void){

	/** wet alert is triggered and kept for 15s */
	if(wet_alert_triggered) {
		static uint32_t next_packet_offset = 50;
		static bool sf7_sent = false;

		if(HAL_GPIO_ReadPin(WATER_SENSE_INT_PIN_GPIO_Port, WATER_SENSE_INT_PIN_Pin) && (HAL_GetTick() - wet_alert_trigger_time) > 15000){
			wet_alert_triggered = false;
			sf7_sent = false;
		}

		if(HAL_GetTick() - wet_alert_trigger_time < 15000) {
			static uint32_t packet_sent_time = 0;

			if(HAL_GetTick() - packet_sent_time > (400 + next_packet_offset)) {
				uint16_t adc_for_wet = 0;
				// adc_for_wet = get_wet_adc();
				// configure_extint_wet();
		#if WATER_ALERT_FEATURE
				uint8_t packetCount = generateWaterAlert(adc_for_wet);
				sf7_sent = !sf7_sent;
			#if LORA_FEATURE
				configure_radio(sf7_sent ? 7 : 9, sf7_sent ? 250 : 125);
				lora_xmit_packet(buffer, packetCount);
			#endif
		#endif
				packet_sent_time = HAL_GetTick();

				/** */
				// uint32_t random_seed = HAL_GetTick();
				next_packet_offset = random() % 10 * 10;
				/** */
			}
		}
	}

	/** panic alert is triggered and kept for 5s */
	if(panic_alert_triggered) {
		static uint32_t button_alert_xmit_time = 0;
		static uint32_t next_packet_offset = 50;
		static bool sf7_sent = false;
		if(HAL_GetTick() - button_alert_xmit_time >= (500 + next_packet_offset)){
			button_alert_xmit_time = HAL_GetTick();
			#if BUTTON_ALERT_FEATURE
			uint8_t packetCount = generateButonAlert(01);
			sf7_sent = !sf7_sent;
			#if LORA_FEATURE
			configure_radio(sf7_sent ? 7 : 9, sf7_sent ? 250 : 125);
			lora_xmit_packet(buffer, packetCount);
			#endif
			#endif

			/** */
			// uint32_t random_seed = HAL_GetTick();
			next_packet_offset = random() % 10 * 10;
			/** */
		}

		if(HAL_GetTick() - panic_alert_trigger_time > 5000){
			panic_alert_triggered = false;
			sf7_sent = false;
		}
	}

	if(wet_alert_triggered && charging_state == NOCHARGING){
		if(in_alert(NO_ALERT)){
			transition_alert(WET_ALERT);
		}
	}
	else if(panic_alert_triggered){
		if(in_alert(NO_ALERT)){
			transition_alert(PANIC_ALERT);
		}
	}

	if(wet_alert_triggered == false
		&& panic_alert_triggered == false
		&& drop_event_triggered == false){
		if(!in_alert(NO_ALERT))
			transition_alert(NO_ALERT);
		}
}

void transition_alert(enum PunchAlert target_alert){
	current_alert = target_alert;
	transition_time = HAL_GetTick();
}

bool in_alert(enum PunchAlert alert){
	return current_alert == alert;
}
