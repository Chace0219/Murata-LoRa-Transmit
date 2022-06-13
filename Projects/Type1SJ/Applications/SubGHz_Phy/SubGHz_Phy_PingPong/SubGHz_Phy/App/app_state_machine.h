
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_STATE_MACHINE_H
#define APP_STATE_MACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/** */
struct application_state{
	uint8_t state_index;
	void (*on_enter_proc)(void);
	void (*loop_proc)(void);
	void (*on_exit_proc)(void);
	struct application_state* next;
};

void init_new_state(uint8_t index, void (*on_enter_proc)(void), void (*loop_proc)(void), void (*on_exit_proc)(void));
void app_state_machine_proc();
void application_transition_state(uint8_t new_state_index);
bool application_in_state(uint8_t state_index);
uint32_t time_in_current_state();

#ifdef __cplusplus
}
#endif

#endif /* LIS2DH12_REGS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
