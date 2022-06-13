/**
 *
 */
#include "stm32l0xx_hal.h"
#include "platform.h"
#include "app_global_def.h"
#include "app_state_machine.h"
#include "app_production_logic.h"
#include "app_alert_handler.h"
#include "app_radio_handler.h"
#include "app_configurations.h"
#include "app_led_state.h"

#include "platform.h"
#include "main.h"
#include "sys_app.h"

#include "subghz_phy_app.h"

#define BOOTING_STATE_TIME_MS 2000

static bool i2c_responsive = false;
static double battery = 200;
static uint16_t button_counter = 0;
static uint16_t wet_counter = 0;
static uint16_t drop_counter = 0;
static uint8_t fall_duration = 0;
static uint16_t fall_probability = 0;
static int16_t impact = 0;
static int16_t m = 0;

static bool button_pressed = false;
int8_t charging_state = 0;
// static uint32_t charge_pulse_time = 0;
// static bool entered_in_charging = false;

static int16_t check_free_fall(void);
static uint32_t heartbeat_interval = 0;
static uint32_t heartbeat_time = 0;

static int16_t data_raw_acceleration[3];
static float acceleration_mg[3];

/** */
void enter_booting(void){
}

void loop_booting(void){
}

void exit_booting(void){
}
/** */

void app_production_logic_init(void){

	(void)init_new_state(SYSTEM_BOOTING_STATE, NULL, NULL, NULL);
	(void)init_new_state(SYSTEM_NORMAL_STATE, NULL, NULL, NULL);
	(void)init_new_state(SYSTEM_IDLE_STATE, NULL, NULL, NULL);
	(void)init_new_state(SYSTEM_SLEEP_STATE, NULL, NULL, NULL);
	application_transition_state(SYSTEM_BOOTING_STATE);

	// Configure LoRa module
	#if LORA_FEATURE
		configure_radio(9, 125);
	#else
		power_off_radio();
	#endif


	#if RGBLED_FEATURE
	set_all_pixels(0x00, 0x00, 0x00);
	#endif

	load_config();

	// Send heartbeat message and blink red LED
	application_transition_state(SYSTEM_NORMAL_STATE);

	/** calculate heartbeat interval using random function */
	heartbeat_interval = (random() % 5 * 1000) + HEARTBEAT_INTERVAL;
	/** calculate heartbeat interval using random function */
}

/**
 * Check if device is in free fall state
 */
static int16_t check_free_fall(void) {

	int16_t vector_sum = 0;

	lis2dh12_reg_t reg;
	/* Read output only if new value available */
	lis2dh12_xl_data_ready_get(&dev_ctx, &reg.byte);

	if (reg.byte) {
		/* Read accelerometer data */
		memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
		lis2dh12_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
		acceleration_mg[0] =
		lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[0]);
		acceleration_mg[1] =
		lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[1]);
		acceleration_mg[2] =
		lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[2]);

		APP_LOG(1, VLEVEL_L, "Acceleration [mg]:%d\t%d\t%d\r\n",
				  (int16_t)acceleration_mg[0], (int16_t)acceleration_mg[1], (int16_t)acceleration_mg[2]);

		int16_t x, y, z;
		x = data_raw_acceleration[0];
		y = data_raw_acceleration[1];
		z = data_raw_acceleration[2];

		static uint32_t total_of_vector = 0;
		static uint32_t avg_cnt = 0;
		static uint32_t avg_of_vector = 0;

		const uint8_t fall_probability_max = 75;
		const uint8_t fall_probability_base = 20;
		// const uint8_t fall_probability_min = 0;

		// calculation of vector sum for acceleration
		vector_sum = sqrt(x * x + y * y + z * z); //
		total_of_vector += vector_sum;
		avg_cnt ++;
		if(avg_cnt > 100){
			avg_of_vector = total_of_vector / avg_cnt;
			total_of_vector = 0;
			avg_cnt = 0;
		}

		#if TEST_FORCE_FALL
		static uint32_t force_fall_time = 0;
		static uint32_t next_force_interval = 15000;

		if(HAL_GetTick() - force_fall_time >= (45000 + next_force_interval))
		{
			printf("Manual free fall happened \r\n");
			force_fall_time = HAL_GetTick();
			if(fall_probability == 0)
				fall_probability = 22;
			uint32_t random_seed = HAL_GetTick();
			next_force_interval = ((uint16_t)rand_r(&random_seed) % 5 * 1500);
		}
		#endif

		int16_t falling_gravity_threshold = avg_of_vector * 1 / 2; // ex: 1g
		if(vector_sum < falling_gravity_threshold){ // weightlessness situation
			if (fall_duration < 255)
				fall_duration++;
			fall_probability += ((1300 * fall_duration) / vector_sum);
			if(fall_probability < fall_probability_base){
				fall_probability = fall_probability_base;
			}
			if(fall_probability > fall_probability_max){
				fall_probability = fall_probability_max;
			}
		}
		else{
			if(fall_probability > 0)
			{
				if(impact < vector_sum){
					impact = vector_sum;
				}

			}
			else{
				fall_duration = 0;
				impact = 0;
			}
		}

	}

	return vector_sum;
}


/** */
void app_production_logic_process(){
	app_state_machine_proc();

	/** Customized procedures */
	red_led_proc();
	alert_proc();
	/** Customized procedures */

	alert_state = (panic_alert_triggered << PANIC_ALERT) | (wet_alert_triggered << WET_ALERT) | (drop_event_triggered << DROP_EVENT);
	if(pre_alert_state != alert_state){
		pre_alert_state = alert_state;
		if(alert_state == 0){
			uint16_t adc_for_wet = 0;
			// configure_wet_adc();
			// adc_for_wet = get_wet_adc();
			heartbeat_time = HAL_GetTick();
			// configure_extint_wet();
			bool wet_pin_dio = false;
			wet_pin_dio = HAL_GPIO_ReadPin(WATER_SENSE_INT_PIN_GPIO_Port, WATER_SENSE_INT_PIN_Pin);
			#if HEARTBEAT_FEATURE
			uint8_t packetCount = generateHeartBeat(i2c_responsive, (uint16_t)battery, button_counter, wet_counter, fall_probability, drop_counter, alert_state, adc_for_wet, wet_pin_dio, charging_state);
			#if LORA_FEATURE
			lora_xmit_packet(buffer, packetCount);
			#endif
			#endif
		}
	}

	if(application_in_state(SYSTEM_NORMAL_STATE)) {

//		/** check if charging has finished */
//		// for more than 1500ms, no charging pulse, it means that not charging or fully charged
//		if((HAL_GetTick() / 1000 - charge_pulse_time) > 2000)
//		{
//			if(port_pin_get_input_level(BATTCHARGEPIN)){
//				if(charging_state != NOCHARGING){
//					charging_state = NOCHARGING;
//
//					#if RGBLED_FEATURE
//						// draw black color for RGB LEDs when charging has finished
//						port_pin_set_output_level(LED_PWR_PIN, LOW);
//						set_all_pixels(0, 0, 0);
//						draw_pixels();
//						delay_ms(BOOST_RISE_TIME);
//						port_pin_set_output_level(LED_DATA_PIN, LOW);
//						delay_ms(BOOST_FALL_TIME);
//						port_pin_set_output_level(LED_PWR_PIN, HIGH);
//					#endif
//				}
//			}
//			else{
//				if(charging_state != FULLCHARGED){
//					charging_state = FULLCHARGED;
//				}
//			}
//		}
//
//		// entered charging/fault state, it should set LEDs with
//		if(entered_in_charging){
//			entered_in_charging =  false;
//
//		#if RGBLED_FEATURE
//			// draw black color for RGB LEDs and turn on booster when charging has started
//			port_pin_set_output_level(LED_PWR_PIN, LOW);
//			set_all_pixels(0, 0, 0);
//			draw_pixels();
//		#endif
//
//		}
//
//		/** while no alert, if charging pulse pin should be matched with red LED while charging */


		if(charging_state == INCHARGING || charging_state == CHARGE_FAULT)
		{
			if(in_alert(NO_ALERT) || in_alert(DROP_EVENT))
			{
				// pulsing charge LED at only battery is more than threshold stable
				// turn_red_LED(!port_pin_get_input_level(BATTCHARGEPIN));

				/*
				if(battery >= (BATTERY_NORMAL_THRESHOLD / 16))
				turn_red_LED(!port_pin_get_input_level(BATTCHARGEPIN));
				else
				turn_red_LED(false);
				*/
			}
		}
		else{
			if(in_alert(NO_ALERT) || in_alert(DROP_EVENT))
			{
				turn_red_LED(false);
			}
		}

		/** every 15s, it should xmit heartbeat packet by switching spreading factor */
		if((HAL_GetTick() - heartbeat_time) > heartbeat_interval && in_alert(NO_ALERT)) {

			/** calculate next heartbeat interval using random function */
			heartbeat_interval = ((random() % 5) * 1000) + HEARTBEAT_INTERVAL;
			/** calculate next heartbeat interval using random function */

			if(charging_state == NOCHARGING){
				heartbeat_red_blink();
			}

			uint16_t adc_for_wet = 0;
//			adc_for_wet = get_wet_adc();
//			configure_extint_wet();

			bool wet_pin_dio = false;
			wet_pin_dio = HAL_GPIO_ReadPin(WATER_SENSE_INT_PIN_GPIO_Port, WATER_SENSE_INT_PIN_Pin);
			// if(battery >= (BATTERY_NORMAL_THRESHOLD / 16))
			{
				#if HEARTBEAT_FEATURE
				uint8_t packetCount = generateHeartBeat(i2c_responsive, (uint16_t)battery, button_counter, wet_counter, fall_probability, drop_counter, alert_state, adc_for_wet, wet_pin_dio, charging_state);
				#if LORA_FEATURE

				/** swapping spread factors between SF_7 and SF_9 */
				static uint8_t sf_factor = 9;
				if(sf_factor == 9){
					sf_factor = 7;
					configure_radio(sf_factor, 250);
				}
				else{
					sf_factor = 9;
					configure_radio(sf_factor, 125);
				}
				/** swapping spread factors between SF_7 and SF_9 */

				// printf("heartbeat battery: %d, sensor: %s, charging: %d\r\n", (uint8_t)battery, (i2c_responsive == true ? "ok" : "false"), (uint8_t)charging_state);
				lora_xmit_packet(buffer, packetCount);
				#endif
				#endif
			}
			heartbeat_time = HAL_GetTick();
		}

		// Checking accel
		static uint32_t accel_check_time = 0;
		if(HAL_GetTick() - accel_check_time > 25) {
			accel_check_time = HAL_GetTick();
			int16_t vector_sum = check_free_fall();
			if(fall_probability > 0){ // BIG_FALL_PROBABILITY
				static uint16_t drop_event_packet_count = 0;
				if(!drop_event_triggered)
				{
					drop_event_packet_count = 0;
					drop_event_triggered = true;
					drop_event_trigger_time = HAL_GetTick();

					if(in_alert(NO_ALERT)){
						transition_alert(DROP_EVENT);
					}
				}

				// send drop/fall event packet only when wet alert is not triggered
				if(!wet_alert_triggered)
				{
					static uint32_t packet_sent_time = 0;
					static uint32_t next_packet_offset = 200;
					if(HAL_GetTick() - packet_sent_time > (300 + next_packet_offset))
					{
						static uint8_t sf_packet_count = 0;
						drop_event_packet_count++;
						packet_sent_time = HAL_GetTick();
					#if DROP_EVANT_FEATURE
						uint8_t packetCount = generateDropPacket(fall_probability, fall_duration, m, vector_sum);
						fall_probability --;
						#if LORA_FEATURE

						switch(sf_packet_count){
							case 0:
							case 1:
								configure_radio(7, 250);
							break;

							case 2:
								configure_radio(9, 125);
							break;
						}
						lora_xmit_packet(buffer, packetCount);
						#endif
					#endif
						sf_packet_count++;
						if(sf_packet_count > 2)
							sf_packet_count = 0;
						/** */
						next_packet_offset = random() % 10 * 40;
						/** */
					}
				}
			}
			else{
				drop_event_triggered = false;
				drop_led_blinked = false;
			}
		}

		// Process button de-bounce
		static bool buttonState = false;             // the current reading from the input pin
		static bool lastButtonState = false;   // the previous reading from the input pin
		static uint32_t lastDebounceTime = 0;  // the last time the output pin was toggled
		const uint32_t debounceDelay = 25;    // the debounce time; increase if the output flickers

		// read the state of the switch into a local variable:
		bool reading = HAL_GPIO_ReadPin(BUTTON_INT_PIN_GPIO_Port, BUTTON_INT_PIN_Pin);
		// If the switch changed, due to noise or pressing:
		if (reading != lastButtonState) {
			// reset the de bounce timer
			lastDebounceTime = HAL_GetTick();
		}
		if ((HAL_GetTick() - lastDebounceTime) > debounceDelay) {
			if (reading != buttonState) {
				buttonState = reading;
				// only toggle the LED if the new button state is HIGH
				if (buttonState == true) {
					// Button released
				}
				else if(buttonState == false){
					// Panic button has triggered (Alert!!!)
					transition_alert(PANIC_ALERT);
					panic_alert_triggered = true;
					panic_alert_trigger_time = HAL_GetTick();
				}
			}
		}
		lastButtonState = reading;
	}
}

void water_detection_interrupt_callback(){
	// ignore wet pin interrupt when it is plugged to charger
	if(charging_state != NOCHARGING)
		return;

	wet_counter++;
	if(wet_alert_triggered == false){

		// starts to send water alert packets
		#if WATER_ALERT_FEATURE
		uint8_t packetCount = generateWaterAlert(0);
		#if LORA_FEATURE
			configure_radio(7, 250);
			lora_xmit_packet(buffer, packetCount);
		#endif
		#endif

		wet_alert_triggered = true;
		wet_alert_trigger_time = HAL_GetTick();

		// transition to panic alert
		transition_alert(WET_ALERT);
	}
}

void button_clicked_interrupt_callback(){
	button_counter++;
	if(button_pressed == false){
		button_pressed = true;
	}
}

