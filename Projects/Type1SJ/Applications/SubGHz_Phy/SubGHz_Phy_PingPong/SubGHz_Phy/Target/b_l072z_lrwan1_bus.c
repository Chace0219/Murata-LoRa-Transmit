/**
  ******************************************************************************
  * @file           : b_l072z_lrwan1_bus.c
  * @brief          : source file for the BSP BUS IO driver
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
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
#include "b_l072z_lrwan1_bus.h"
#include "b_l072z_lrwan1_errno.h"
#include "stm32l0xx_hal.h"

#define TIMEOUT_DURATION 1000
/** @addtogroup BSP
  * @{
  */
__weak HAL_StatusTypeDef MX_SPI2_Init(SPI_HandleTypeDef *hspi);

/** @addtogroup NUCLEO_L073RZ
  * @{
  */

/** @defgroup NUCLEO_L073RZ_BUS NUCLEO_L073RZ BUS
  * @{
  */

/** @defgroup NUCLEO_L073RZ_Private_Variables BUS Private Variables
  * @{
  */
SPI_HandleTypeDef hspi2;
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
static uint32_t IsSPI2MspCbValid = 0;
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup NUCLEO_L073RZ_Private_FunctionPrototypes  Private Function Prototypes
  * @{
  */

static void SPI2_MspInit(SPI_HandleTypeDef *spiHandle);
static void SPI2_MspDeInit(SPI_HandleTypeDef *spiHandle);
static uint32_t SPI_GetPrescaler(uint32_t clock_src_hz, uint32_t baudrate_mbps);

/**
  * @}
  */

/** @defgroup NUCLEO_L073RZ_LOW_LEVEL_Private_Functions NUCLEO_L073RZ LOW LEVEL Private Functions
  * @{
  */

/** @defgroup NUCLEO_L073RZ_BUS_Exported_Functions NUCLEO_L073RZ_BUS Exported Functions
  * @{
  */
/* BUS IO driver over SPI Peripheral */
/*******************************************************************************
                            BUS OPERATIONS OVER SPI
  *******************************************************************************/
/**
  * @brief  Initializes SPI HAL.
  * @retval None
  * @retval BSP status
  */
int32_t BSP_SPI2_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hspi2.Instance  = SPI2;
  if (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_RESET)
  {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
    /* Init the SPI Msp */
    SPI2_MspInit(&hspi2);
#else
    if (IsSPI2MspCbValid == 0U)
    {
      if (BSP_SPI2_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif

    /* Init the SPI */
    if (MX_SPI2_Init(&hspi2) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  DeInitializes SPI HAL.
  * @retval None
  * @retval BSP status
  */
int32_t BSP_SPI2_DeInit(void)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
  SPI2_MspDeInit(&hspi2);
#endif

  if (HAL_SPI_DeInit(&hspi2) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  Write Data through SPI BUS.
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI2_Send(uint8_t *pData, uint16_t len)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;

  if (HAL_SPI_Transmit(&hspi2, pData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}

/**
  * @brief  Receive Data from SPI BUS
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t  BSP_SPI2_Recv(uint8_t *pData, uint16_t len)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;

  if (HAL_SPI_Receive(&hspi2, pData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}

/**
  * @brief  Send and Receive data to/from SPI BUS (Full duplex)
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI2_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;

  if (HAL_SPI_TransmitReceive(&hspi2, pTxData, pRxData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP SPI2 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_SPI2_RegisterDefaultMspCallbacks(void)
{

  __HAL_SPI_RESET_HANDLE_STATE(&hspi2);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi2, HAL_SPI_MSPINIT_CB_ID, SPI2_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi2, HAL_SPI_MSPDEINIT_CB_ID, SPI2_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsSPI2MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP SPI2 Bus Msp Callback registering
  * @param Callbacks     pointer to SPI2 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_SPI2_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_SPI_RESET_HANDLE_STATE(&hspi2);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi2, HAL_SPI_MSPINIT_CB_ID, Callbacks->pMspSpiInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi2, HAL_SPI_MSPDEINIT_CB_ID, Callbacks->pMspSpiDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsSPI2MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/**
  * @brief  Return system tick in ms
  * @retval Current HAL time base time stamp
  */
int32_t BSP_GetTick(void)
{
  return HAL_GetTick();
}

/* SPI2 init function */

__weak HAL_StatusTypeDef MX_SPI2_Init(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef ret = HAL_OK;
  hspi->Instance = SPI2;
  hspi->Init.Mode = SPI_MODE_MASTER;
  hspi->Init.Direction = SPI_DIRECTION_2LINES;
  hspi->Init.DataSize = SPI_DATASIZE_8BIT;
  hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi->Init.NSS = SPI_NSS_SOFT;
  /* SPI2 is on APB2 for L0x3 -> HAL_RCC_GetPCLK2Freq */
  hspi->Init.BaudRatePrescaler = SPI_GetPrescaler(HAL_RCC_GetPCLK2Freq(), RADIO_SPI_BAUDRATE);
  hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi->Init.TIMode = SPI_TIMODE_DISABLE;
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

static void SPI2_MspInit(SPI_HandleTypeDef *spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
  /* Enable Peripheral clock */
  __HAL_RCC_SPI2_CLK_ENABLE();

  /**SPI2 GPIO Configuration
  PA5     ------> SPI2_SCK
  PA6     ------> SPI2_MISO
  PA7     ------> SPI2_MOSI
    */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = BUS_SPI2_MOSI_GPIO_AF;
  GPIO_InitStruct.Pin = BUS_SPI2_MOSI_GPIO_PIN;
  HAL_GPIO_Init(BUS_SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Alternate = BUS_SPI2_MISO_GPIO_AF;
  GPIO_InitStruct.Pin = BUS_SPI2_MISO_GPIO_PIN;
  HAL_GPIO_Init(BUS_SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Alternate = BUS_SPI2_SCK_GPIO_AF;
  GPIO_InitStruct.Pin = BUS_SPI2_SCK_GPIO_PIN;
  HAL_GPIO_Init(BUS_SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);


  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
}

static void SPI2_MspDeInit(SPI_HandleTypeDef *spiHandle)
{
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
  /* Peripheral clock disable */
  __HAL_RCC_SPI2_CLK_DISABLE();

  /**SPI2 GPIO Configuration
  PA5     ------> SPI2_SCK
  PA6     ------> SPI2_MISO
  PA7     ------> SPI2_MOSI
    */
  HAL_GPIO_DeInit(BUS_SPI2_MOSI_GPIO_PORT, BUS_SPI2_MOSI_GPIO_PIN);
  HAL_GPIO_DeInit(BUS_SPI2_MISO_GPIO_PORT, BUS_SPI2_MISO_GPIO_PIN);
  HAL_GPIO_DeInit(BUS_SPI2_SCK_GPIO_PORT, BUS_SPI2_SCK_GPIO_PIN);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
}

static uint32_t SPI_GetPrescaler(uint32_t clock_src_hz, uint32_t baudrate_mbps)
{
  uint32_t divisor = 0;
  uint32_t spi_clk = clock_src_hz;
  uint32_t presc = 0;

  static const uint32_t baudrate[] =
  {
    SPI_BAUDRATEPRESCALER_2,
    SPI_BAUDRATEPRESCALER_4,
    SPI_BAUDRATEPRESCALER_8,
    SPI_BAUDRATEPRESCALER_16,
    SPI_BAUDRATEPRESCALER_32,
    SPI_BAUDRATEPRESCALER_64,
    SPI_BAUDRATEPRESCALER_128,
    SPI_BAUDRATEPRESCALER_256,
  };

  while (spi_clk > baudrate_mbps)
  {
    presc = baudrate[divisor];
    if (++divisor > 7)
    {
      break;
    }

    spi_clk = (spi_clk >> 1);
  }

  return presc;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
