/**
  ******************************************************************************
  * @file    stm32_lpm_if.c
  * @author  MCD Application Team
  * @brief   Low layer function to enter/exit low power modes (stop, sleep)
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

/* Includes ------------------------------------------------------------------*/
#include "sys_debug.h"
#include "stm32_lpm_if.h"
#include "usart_if.h"
#include "radio_board_if.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief Power driver callbacks handler
  */
const struct UTIL_LPM_Driver_s UTIL_PowerDriver =
{
  PWR_EnterSleepMode,
  PWR_ExitSleepMode,

  PWR_EnterStopMode,
  PWR_ExitStopMode,

  PWR_EnterOffMode,
  PWR_ExitOffMode,
};

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions --------------------------------------------------------*/

void PWR_EnterOffMode(void)
{
  /* USER CODE BEGIN EnterOffMode_1 */

  /* USER CODE END EnterOffMode_1 */
}

void PWR_ExitOffMode(void)
{
  /* USER CODE BEGIN ExitOffMode_1 */

  /* USER CODE END ExitOffMode_1 */
}

void PWR_EnterStopMode(void)
{
  UTILS_ENTER_CRITICAL_SECTION();

  Sx_Board_IoDeInit();

  UTIL_ADV_TRACE_IoDeInit();//@Murata DeInit the trace io when enter stop mode

  /*clear wake up flag*/
  SET_BIT(PWR->CR, PWR_CR_CWUF);

  UTILS_EXIT_CRITICAL_SECTION();

  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

void PWR_ExitStopMode(void)
{
  /* Disable IRQ while the MCU is not running on HSI */

  UTILS_ENTER_CRITICAL_SECTION();

  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSI */
  __HAL_RCC_HSI_ENABLE();

  /* Wait till HSI is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET) {}

  /* Enable PLL */
  __HAL_RCC_PLL_ENABLE();
  /* Wait till PLL is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET) {}

  /* Select PLL as system clock source */
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);

  /* Wait till PLL is used as system clock source */
  while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK) {}

  /* initializes the peripherals */
  Sx_Board_IoInit();
  UTIL_ADV_TRACE_IoInit();//@Murata Init the trace io when exit stop mode

  UTILS_EXIT_CRITICAL_SECTION();
}

void PWR_EnterSleepMode(void)
{
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void PWR_ExitSleepMode(void)
{
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private Functions Definition -----------------------------------------------*/
/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

