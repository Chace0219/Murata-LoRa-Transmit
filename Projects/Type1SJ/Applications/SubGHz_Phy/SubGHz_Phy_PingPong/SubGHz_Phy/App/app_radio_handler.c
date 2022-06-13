/**
 *
 */
#include "stm32l0xx_hal.h"
#include "platform.h"
#include "main.h"
#include "sys_app.h"
#include "subghz_phy_app.h"
#include "app_radio_handler.h"
#include "app_configurations.h"

char buffer[128];

uint8_t generateDropPacket(uint8_t gravity, uint8_t duration, uint16_t m, int16_t vector_sum){
	buffer[0] = setting.unit_id / 256;
	buffer[1] = setting.unit_id % 256;
	buffer[2] = 0x83;
	buffer[3] = gravity;
	buffer[4] = 0;
	for(uint8_t idx = 0; idx < 4; idx++)
		buffer[4] += buffer[idx];
	return 5;
}

uint8_t generateLandDetection(uint8_t fall_probability, uint8_t duration, int16_t imapct) {
	buffer[0] = setting.unit_id / 256;
	buffer[1] = setting.unit_id % 256;
	buffer[2] = 0x84;
	buffer[3] = fall_probability;
	buffer[4] = duration;
	buffer[5] = (uint16_t)imapct / 256;
	buffer[6] = (uint16_t)imapct % 256;
	return 7;
}

uint8_t generateWaterAlert(uint16_t adc){
	/*
		Water Alert Packet:
        ex: 2045 81
        - UnitID : 0x2045
        - EventType: Water alert event
	*/
	buffer[0] = setting.unit_id / 256;
	buffer[1] = setting.unit_id % 256;
	buffer[2] = 0x81;
	buffer[3] = 0;
	for(uint8_t idx = 0; idx < 3; idx++)
		buffer[3] += buffer[idx];
	return 4;
	/*
	buffer[3] = adc / 256;
	buffer[4] = adc % 256;
	return 5;
	*/
}

uint8_t generateButonAlert(uint8_t buttonType){
	/*
		Button Alert Packet:
		ex: 2045 82 01
		- UnitID : 0x2045
		- EventType: Button alert
		- Reason: 01-short button, 02-long button, you can define more reasons for button alert
	*/
	buffer[0] = setting.unit_id / 256;
	buffer[1] = setting.unit_id % 256;
	buffer[2] = 0x82;
	buffer[3] = buttonType;
	buffer[4] = 0;
	for(uint8_t idx = 0; idx < 4; idx++)
		buffer[4] += buffer[idx];
	return 5;
}

uint8_t generateHeartBeat(
	bool i2c_responsive, uint8_t battery,
	uint16_t button_counter, uint16_t wet_counter,
	uint8_t fall_probability,
	uint16_t drop_counter, uint8_t alert_state,
	uint16_t wet_adc, bool wet_dio, int8_t charging){

	buffer[0] = setting.unit_id / 256;
	buffer[1] = setting.unit_id % 256;
	buffer[2] = 0x00;
	buffer[3] = battery;
	buffer[4] = button_counter / 256;
	buffer[5] = button_counter % 256;
	buffer[6] = wet_counter / 256;
	buffer[7] = wet_counter % 256;
	buffer[8] = drop_counter / 256;
	buffer[9] = drop_counter % 256;
	buffer[10] = i2c_responsive;
	buffer[11] = alert_state;

	buffer[12] = wet_adc / 256;
	buffer[13] = wet_adc % 256;

	buffer[14] = wet_dio;
	buffer[15] = (uint8_t)charging;
	buffer[16] = fall_probability;

	buffer[17] = 0;
	for(uint8_t idx = 0; idx < 16; idx++)
		buffer[17] += buffer[idx];
	return 18;
}
