/**
  ******************************************************************************
  * @file    b-l072z-lrwan1.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on B-L072Z-LRWAN1 Discovery Kit
  *            from STMicroelectronics
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __B_L072Z_LRWAN1_H
#define __B_L072Z_LRWAN1_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stdlib.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B-L072Z-LRWAN1
  * @{
  */

/** @addtogroup B-L072Z-LRWAN1_LOW_LEVEL
  * @{
  */

/** @defgroup B-L072Z-LRWAN1_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum
{
  MAIN_LED = 0,
  RED_LED = 1,
  GREEN_LED = 2,
  BLUE_LED = 3,
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY  = BUTTON_USER
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

/**
  * @}
  */

/** @defgroup B-L072Z-LRWAN1_LOW_LEVEL_Exported_Constants
  * @{
  */



/** @addtogroup B-L072Z-LRWAN1_LOW_LEVEL_LED
  * @{
  */
#define LEDn                               4

#define MAIN_LED_PIN                       GPIO_PIN_11
#define MAIN_LED_GPIO_PORT                 GPIOA
#define MAIN_LED_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define MAIN_LED_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

#define RED_LED_PIN                       GPIO_PIN_12
#define RED_LED_GPIO_PORT                 GPIOA
#define RED_LED_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define RED_LED_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

#define GREEN_LED_PIN                     GPIO_PIN_9
#define GREEN_LED_GPIO_PORT               GPIOA
#define GREEN_LED_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define GREEN_LED_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()

#define BLUE_LED_PIN                      GPIO_PIN_10
#define BLUE_LED_GPIO_PORT                GPIOA
#define BLUE_LED_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define BLUE_LED_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)    do { \
                                                switch( __INDEX__ ) \
                                                {\
                                                  case MAIN_LED: \
                                                    MAIN_LED_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case RED_LED: \
                                                    RED_LED_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case GREEN_LED: \
                                                    GREEN_LED_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case BLUE_LED: \
                                                    BLUE_LED_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  default:\
                                                    break;\
                                                }\
                                              } while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)   do { \
                                                switch( __INDEX__ ) \
                                                {\
                                                  case MAIN_LED: \
                                                    MAIN_LED_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case RED_LED: \
                                                    RED_LED_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case GREEN_LED: \
                                                    GREEN_LED_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case BLUE_LED: \
                                                    BLUE_LED_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  default:\
                                                    break;\
                                                }\
                                              } while(0)
/**
  * @}
  */

/** @addtogroup B-L072Z-LRWAN1_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                            1

/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         GPIO_PIN_2
#define USER_BUTTON_GPIO_PORT                   GPIOB
#define USER_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define USER_BUTTON_EXTI_LINE                   GPIO_PIN_2
#define USER_BUTTON_EXTI_IRQn                   EXTI2_3_IRQn
/* Aliases */
#define KEY_BUTTON_PIN                          USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT                    USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()            USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()           USER_BUTTON_GPIO_CLK_DISABLE()
#define KEY_BUTTON_EXTI_LINE                    USER_BUTTON_EXTI_LINE
#define KEY_BUTTON_EXTI_IRQn                    USER_BUTTON_EXTI_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)     do {KEY_BUTTON_GPIO_CLK_ENABLE();  } while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    do {KEY_BUTTON__GPIO_CLK_DISABLE();} while(0)
/**
  * @}
  */




/** @defgroup B-L072Z-LRWAN1_LOW_LEVEL_Exported_Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __B_L072Z_LRWAN1_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

