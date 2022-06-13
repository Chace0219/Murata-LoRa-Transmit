
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_BOARD_TESTING_H
#define APP_BOARD_TESTING_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

enum
{
    STATE_BOARD_TESTING_IDLE,
    STATE_BOARD_TESTING_BUTTON_PRESSED,
    STATE_BOARD_TESTING_WATER_DETECTION,
    STATE_BOARD_TESTING_FREE_FALL_DETECTION,
};

void app_board_test_init(void);
void app_board_test_process();

#ifdef __cplusplus
}
#endif

#endif /* LIS2DH12_REGS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
