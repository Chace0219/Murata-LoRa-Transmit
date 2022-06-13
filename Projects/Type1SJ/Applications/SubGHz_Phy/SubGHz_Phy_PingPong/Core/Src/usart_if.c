/**
  ******************************************************************************
  * @file    usart_if.c
  * @author  MCD Application Team
  * @brief   Configuration of UART MX driver interface for hyperterminal communication
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
#include "usart_if.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* Uart Handle */

#ifdef USE_USART1
#define UartHandle husart1
extern UART_HandleTypeDef husart1;
#else
#define UartHandle husart2
extern UART_HandleTypeDef husart2;
#endif

uint8_t charRx;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief Trace driver callbacks handler
  */
const UTIL_ADV_TRACE_Driver_s UTIL_TraceDriver =
{
  vcom_Init,
  vcom_DeInit,
  vcom_ReceiveInit,
  vcom_Trace_DMA,
  vcom_IoInit,//@Murata Add for low power mode
  vcom_IoDeInit,//@Murata Add for low power mode
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
/**
  * @brief  TX complete callback
  * @return none
  */
static void (*TxCpltCallback)(void *);
/**
  * @brief  RX complete callback
  * @param  rxChar ptr of chars buffer sent by user
  * @param  size buffer size
  * @param  error errorcode
  * @return none
  */
static void (*RxCpltCallback)(uint8_t *rxChar, uint16_t size, uint8_t error);

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions --------------------------------------------------------*/

UTIL_ADV_TRACE_Status_t vcom_Init(void (*cb)(void *))
{
  /* USER CODE BEGIN vcom_Init_1 */

  /* USER CODE END vcom_Init_1 */
  TxCpltCallback = cb;
#ifdef USE_USART1
    MX_USART1_Init();
#else
    MX_USART2_Init();
#endif

  return UTIL_ADV_TRACE_OK;
  /* USER CODE BEGIN vcom_Init_2 */

  /* USER CODE END vcom_Init_2 */
}

UTIL_ADV_TRACE_Status_t vcom_DeInit(void)
{
  /* USER CODE BEGIN vcom_DeInit_1 */

  /* USER CODE END vcom_DeInit_1 */
  /* ##-1- Reset peripherals ################################################## */

#ifdef USE_USART1
  __HAL_RCC_USART1_FORCE_RESET();
  __HAL_RCC_USART1_RELEASE_RESET();
#else
  __HAL_RCC_USART2_FORCE_RESET();
  __HAL_RCC_USART2_RELEASE_RESET();
#endif

  /* ##-2- MspDeInit ################################################## */
  HAL_UART_MspDeInit(&UartHandle);

  /* ##-3- Disable the NVIC for DMA ########################################### */
  /* temporary while waiting CR 50840: MX implementation of  MX_DMA_DeInit() */
  /* For the time being user should change manually the channel according to the MX settings */
  /* USER CODE BEGIN 1 */
#ifdef USE_USART1
    HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);

#else
    HAL_NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);
#endif

  return UTIL_ADV_TRACE_OK;
  /* USER CODE END 1 */
  /* USER CODE BEGIN vcom_DeInit_2 */

  /* USER CODE END vcom_DeInit_2 */
}
UTIL_ADV_TRACE_Status_t vcom_IoInit(void)
{
    /* at that point, MCU has been awoken. Wake Up based on RXNE flag successful */
    HAL_UARTEx_DisableStopMode(&UartHandle);

    /* Disable the UART Wake UP from stop mode Interrupt */
    __HAL_UART_DISABLE_IT(&UartHandle, UART_IT_WUF);

    /* enable dma */
    __HAL_DMA_ENABLE(UartHandle.hdmatx);

    /* set tx2 queue params to default value */
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    /* Enable GPIO TX/RX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin       = USARTx_TX_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;

    HAL_GPIO_Init(USARTx_TX_GPIO_Port, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = USARTx_RX_Pin;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;

    HAL_GPIO_Init(USARTx_RX_GPIO_Port, &GPIO_InitStruct);
		
    return UTIL_ADV_TRACE_OK;
}


/******************************************************************************
 * @fn      vcom_IoDeInit
 *
 * @brief   DeInit uart IOs.
 *
 * @param   None
 *
 * @return  None
 *****************************************************************************/

UTIL_ADV_TRACE_Status_t vcom_IoDeInit(void)
{
    /* make sure that no UART transfer is on-going */
    while (__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);

    /* make sure that UART is ready to receive
    * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
    while (__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);

    /* disable dma */
    __HAL_DMA_DISABLE(UartHandle.hdmatx);

    /*Set wakeUp event on start bit*/
    UART_WakeUpTypeDef wakeup =
    {
        .WakeUpEvent = UART_WAKEUP_ON_STARTBIT
    };

    if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, wakeup) != HAL_OK)
    {
        Error_Handler();
    }

    /* Enable the UART Wake UP from stop mode Interrupt */
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);

    /* enable MCU wake-up by UART */
    HAL_UARTEx_EnableStopMode(&UartHandle);

    /* Disable USARTx */
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();

    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
  
    GPIO_InitStructure.Pin =  USARTx_TX_Pin ;
    HAL_GPIO_Init(  USARTx_TX_GPIO_Port, &GPIO_InitStructure );
		
    return UTIL_ADV_TRACE_OK;
}

void vcom_Trace(uint8_t *p_data, uint16_t size)
{
  /* USER CODE BEGIN vcom_Trace_1 */

  /* USER CODE END vcom_Trace_1 */
  HAL_UART_Transmit(&UartHandle, p_data, size, 1000);
  /* USER CODE BEGIN vcom_Trace_2 */

  /* USER CODE END vcom_Trace_2 */
}

UTIL_ADV_TRACE_Status_t vcom_Trace_DMA(uint8_t *p_data, uint16_t size)
{
  /* USER CODE BEGIN vcom_Trace_DMA_1 */

  /* USER CODE END vcom_Trace_DMA_1 */
  HAL_UART_Transmit_DMA(&UartHandle, p_data, size);
  return UTIL_ADV_TRACE_OK;
  /* USER CODE BEGIN vcom_Trace_DMA_2 */

  /* USER CODE END vcom_Trace_DMA_2 */
}

UTIL_ADV_TRACE_Status_t vcom_ReceiveInit(void (*RxCb)(uint8_t *rxChar, uint16_t size, uint8_t error))
{
  /* USER CODE BEGIN vcom_ReceiveInit_1 */

  /* USER CODE END vcom_ReceiveInit_1 */
  UART_WakeUpTypeDef WakeUpSelection;

  /*record call back*/
  RxCpltCallback = RxCb;

  /*Set wakeUp event on start bit*/
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;

  HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection);

  /* Make sure that no UART transfer is on-going */
  while (__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);

  /* Make sure that UART is ready to receive)   */
  while (__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);

  /* Enable USART interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);

  /*Enable wakeup from stop mode*/
  HAL_UARTEx_EnableStopMode(&UartHandle);

  /*Start LPUART receive on IT*/
  HAL_UART_Receive_IT(&UartHandle, &charRx, 1);

  return UTIL_ADV_TRACE_OK;
  /* USER CODE BEGIN vcom_ReceiveInit_2 */

  /* USER CODE END vcom_ReceiveInit_2 */
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* USER CODE BEGIN HAL_UART_TxCpltCallback_1 */

  /* USER CODE END HAL_UART_TxCpltCallback_1 */
  /* buffer transmission complete*/
  TxCpltCallback(NULL);
  /* USER CODE BEGIN HAL_UART_TxCpltCallback_2 */

  /* USER CODE END HAL_UART_TxCpltCallback_2 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* USER CODE BEGIN HAL_UART_RxCpltCallback_1 */

  /* USER CODE END HAL_UART_RxCpltCallback_1 */
  if ((NULL != RxCpltCallback) && (HAL_UART_ERROR_NONE == UartHandle->ErrorCode))
  {
    RxCpltCallback(&charRx, 1, 0);
  }
  HAL_UART_Receive_IT(UartHandle, &charRx, 1);
  /* USER CODE BEGIN HAL_UART_RxCpltCallback_2 */

  /* USER CODE END HAL_UART_RxCpltCallback_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private Functions Definition -----------------------------------------------*/

/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
