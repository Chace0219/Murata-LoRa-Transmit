/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "lis2dh12_reg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define WATER_SENSE_INT_PIN_Pin GPIO_PIN_1
#define WATER_SENSE_INT_PIN_GPIO_Port GPIOA
#define WATER_SENSE_INT_PIN_EXTI_IRQn EXTI0_1_IRQn

#define BUTTON_INT_PIN_Pin GPIO_PIN_5
#define BUTTON_INT_PIN_GPIO_Port GPIOA
#define BUTTON_INT_PIN_EXTI_IRQn EXTI4_15_IRQn

#define LISDH12_INT1_PIN_Pin GPIO_PIN_7
#define LISDH12_INT1_PIN_GPIO_Port GPIOA
#define LISDH12_INT1_PIN_EXTI_IRQn EXTI4_15_IRQn

#define LISDH12_INT2_PIN_Pin GPIO_PIN_6
#define LISDH12_INT2_PIN_GPIO_Port GPIOA
#define LISDH12_INT2_PIN_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1<<RTC_N_PREDIV_S)-1)
#define RTC_PREDIV_A ((1<<(15-RTC_N_PREDIV_S))-1)
#define USART_BAUDRATE 115200

/* Pin mapping */

/* Definition for UARTx clock resources */
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define DMAMUX_CLK_ENABLE()              __HAL_RCC_DMAMUX1_CLK_ENABLE()


/* Definition for USARTx's NVIC */
#define USARTx_Priority 0
#define USARTx_DMA_Priority 0

#ifdef USE_USART1
#define USARTx_RX_Pin GPIO_PIN_10
#define USARTx_RX_GPIO_Port GPIOA
#define USARTx_TX_Pin GPIO_PIN_9
#define USARTx_TX_GPIO_Port GPIOA
#define USARTx_TX_AF                     GPIO_AF4_USART1
#define USARTx_RX_AF                     GPIO_AF4_USART1
#define USARTx_IRQn                      USART1_IRQn
/* Definition for USARTx's DMA Request */
#define USARTx_TX_DMA_REQUEST             DMA_REQUEST_3
/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL             DMA1_Channel2
/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA1_Channel2_3_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA1_Channel2_3_IRQHandler

#else
#define USARTx_RX_Pin GPIO_PIN_3
#define USARTx_RX_GPIO_Port GPIOA
#define USARTx_TX_Pin GPIO_PIN_2
#define USARTx_TX_GPIO_Port GPIOA
#define USARTx_TX_AF                     GPIO_AF4_USART2
#define USARTx_RX_AF                     GPIO_AF4_USART2
#define USARTx_IRQn                      USART2_IRQn
/* Definition for USARTx's DMA Request */
#define USARTx_TX_DMA_REQUEST             DMA_REQUEST_4
/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL             DMA1_Channel7
/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA1_Channel4_5_6_7_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA1_Channel4_5_6_7_IRQHandler

#endif

#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()



/* USER CODE BEGIN Private defines */
extern I2C_HandleTypeDef hi2c1;
extern stmdev_ctx_t dev_ctx;;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
