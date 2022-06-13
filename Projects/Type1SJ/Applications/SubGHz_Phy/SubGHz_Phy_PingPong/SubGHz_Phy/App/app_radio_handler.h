
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_RADIO_HANDLER_H
#define APP_RADIO_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

extern char buffer[128];

uint8_t generateDropPacket(uint8_t gravity, uint8_t duration, uint16_t m, int16_t vector_sum);
uint8_t generateWaterAlert(uint16_t adc);
uint8_t generateButonAlert(uint8_t buttonType);
uint8_t generateHeartBeat(bool i2c_responsive, uint8_t battery, uint16_t button_counter,
	uint16_t wet_counter, uint8_t fall_probability, uint16_t drop_counter, uint8_t alert_state, uint16_t wet_adc, bool wet_dio, int8_t charging);
uint8_t generateLandDetection(uint8_t fall_probability, uint8_t duration, int16_t imapct);

#ifdef __cplusplus
}
#endif

#endif /* APP_RADIO_HANDLER_H */

