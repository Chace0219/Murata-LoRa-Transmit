/**
  ******************************************************************************
  * @file           : b_l072z_lrwan1_bus.h
  * @brief          : header file for the BSP BUS IO driver
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NUCLEO_L073RZ_BUS_H
#define __NUCLEO_L073RZ_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef USE_HAL_SPI_REGISTER_CALLBACKS
#define USE_HAL_SPI_REGISTER_CALLBACKS 0
#endif
#ifndef USE_HAL_I2C_REGISTER_CALLBACKS
#define USE_HAL_I2C_REGISTER_CALLBACKS 0
#endif

/* Needed for SPI_GetPrescaler */
#define RADIO_SPI_BAUDRATE                  10000000U /* 16M Sigfox, 10M Lora */

/* Ticket https://intbugzilla.st.com/show_bug.cgi?id=54043 */
#define BUS_SPI2_SCK_GPIO_PIN            GPIO_PIN_13
#define BUS_SPI2_MISO_GPIO_PIN           GPIO_PIN_14
#define BUS_SPI2_MOSI_GPIO_PIN           GPIO_PIN_15
#define BUS_SPI2_SCK_GPIO_PORT           GPIOB
#define BUS_SPI2_MISO_GPIO_PORT          GPIOB
#define BUS_SPI2_MOSI_GPIO_PORT          GPIOB
#define BUS_SPI2_SCK_GPIO_AF             GPIO_AF0_SPI2
#define BUS_SPI2_MOSI_GPIO_AF            GPIO_AF0_SPI2
#define BUS_SPI2_MISO_GPIO_AF            GPIO_AF0_SPI2
#define BUS_SPI2_SCK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI2_MOSI_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI2_MISO_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()


#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pSPI_CallbackTypeDef  pMspSpiInitCb;
  pSPI_CallbackTypeDef  pMspSpiDeInitCb;
} BSP_SPI_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* BUS IO driver over SPI Peripheral */
int32_t BSP_SPI2_Init(void);
int32_t BSP_SPI2_DeInit(void);
int32_t BSP_SPI2_Send(uint8_t *pData, uint16_t len);
int32_t BSP_SPI2_Recv(uint8_t *pData, uint16_t len);
int32_t BSP_SPI2_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len);

int32_t BSP_GetTick(void);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_BUS_RegisterDefaultMspCallbacks(void);
int32_t BSP_BUS_RegisterMspCallbacks(BSP_BUS_Cb_t *Callbacks);
#endif /* ((USE_HAL_I2C_REGISTER_CALLBACKS == 1) || (USE_HAL_SPI_REGISTER_CALLBACKS == 1)) */

#ifdef __cplusplus
}
#endif

#endif /* __NUCLEO_L073RZ_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
