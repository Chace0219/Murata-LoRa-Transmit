/**
 *
 */
#include "app_board_testing.h"
#include "app_state_machine.h"
#include "stm32l0xx_hal.h"

#include "platform.h"
#include "main.h"
#include "sys_app.h"

#include "subghz_phy_app.h"

/** */
void enter_testing_idle(void){

}

void loop_testing_idle(void){
	static uint32_t checked_lisdh12_chip_time = 0;
	const uint8_t chip_address = 0x19;
	if(HAL_GetTick() - checked_lisdh12_chip_time >= 2000){
		HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(chip_address << 1), 3, 5);
		if (ret != HAL_OK) /* No ACK Received At That Address */
		{
			printf("No ACK Received At %02X\r\n", chip_address << 1);
			// Only green LED
			LED_Off(MAIN_LED);
			LED_Off(RED_LED);
			LED_On(GREEN_LED);
			LED_Off(BLUE_LED);
		}
		else if(ret == HAL_OK)
		{
			printf("Found a Device At %02X\r\n", chip_address << 1);

			// Both white LED
			LED_On(MAIN_LED);
			LED_On(RED_LED);
			LED_On(GREEN_LED);
			LED_On(BLUE_LED);
		}
		checked_lisdh12_chip_time = HAL_GetTick();
	}
}

void exit_testing_idle(void){

}
/** */


/** */
void enter_testing_button_press(void){
	LED_Off(MAIN_LED);
	LED_On(RED_LED);
	LED_Off(GREEN_LED);
	LED_Off(BLUE_LED);
}

void loop_testing_button_press(void){

}

void exit_testing_button_press(void){
	LED_Off(MAIN_LED);
	LED_Off(RED_LED);
	LED_Off(GREEN_LED);
	LED_Off(BLUE_LED);
}
/** */

/** */
void enter_testing_water_detection(void){
	LED_Off(MAIN_LED);
	LED_Off(RED_LED);
	LED_Off(GREEN_LED);
	LED_On(BLUE_LED);
}

void loop_testing_water_detection(void){

}

void exit_testing_water_detection(void){
	LED_Off(MAIN_LED);
	LED_Off(RED_LED);
	LED_Off(GREEN_LED);
	LED_Off(BLUE_LED);
}
/** */

/** */
void enter_testing_free_fall_detection(void){
	LED_Off(MAIN_LED);
	LED_On(RED_LED);
	LED_On(GREEN_LED);
	LED_Off(BLUE_LED);
}

void loop_testing_free_fall_detection(void){
	if(time_in_current_state() > 4000){
		application_transition_state(STATE_BOARD_TESTING_IDLE);
	}
}

void exit_testing_free_fall_detection(void){
	LED_Off(MAIN_LED);
	LED_Off(RED_LED);
	LED_Off(GREEN_LED);
	LED_Off(BLUE_LED);
}
/** */

void app_board_test_init(void){
	(void)init_new_state(STATE_BOARD_TESTING_IDLE,
			enter_testing_idle, loop_testing_idle, exit_testing_idle);
	(void)init_new_state(STATE_BOARD_TESTING_BUTTON_PRESSED,
			enter_testing_button_press, loop_testing_button_press, exit_testing_button_press);
	(void)init_new_state(STATE_BOARD_TESTING_WATER_DETECTION,
			enter_testing_water_detection, loop_testing_water_detection, exit_testing_water_detection);
	(void)init_new_state(STATE_BOARD_TESTING_FREE_FALL_DETECTION,
			enter_testing_free_fall_detection, loop_testing_free_fall_detection, exit_testing_free_fall_detection);
	application_transition_state(STATE_BOARD_TESTING_IDLE);
}

void app_board_test_process(){
	static uint32_t lora_send_time = 0;
	if(HAL_GetTick() - lora_send_time >= 2000){
		lora_send_time = HAL_GetTick();
		static uint32_t index = 0;
		char buffer[64];
		sprintf(buffer, "Testing LoRa, %04d", index);
		lora_xmit_packet(buffer, strlen(buffer));
		index++;
	}
	app_state_machine_proc();
}
