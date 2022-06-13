/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "main.h"
#include "app_subghz_phy.h"
#include "sys_app.h"
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lis2dh12_reg.h"

#include "app_state_machine.h"
#include "app_board_testing.h"
//#include "app_production_logic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define    BOOT_TIME_FOR_LISDH12            5 //ms
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
I2C_HandleTypeDef hi2c1;


static uint8_t whoamI;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void MX_I2C1_Init(void);
static void MX_GPIO_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static int32_t i2c1_data_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len);
static int32_t i2c1_data_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);

stmdev_ctx_t dev_ctx;
/* USER CODE END 0 */



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_SubGHz_Phy_Init();
	/* USER CODE BEGIN 2 */
	MX_I2C1_Init();
	MX_GPIO_Init();

	/* Initialize mems driver interface */
	dev_ctx.write_reg = i2c1_data_write;
	dev_ctx.read_reg = i2c1_data_read;
	dev_ctx.handle = &hi2c1;

	/* Wait sensor boot time */
	HAL_Delay(BOOT_TIME_FOR_LISDH12);

	/* Check device ID */
	lis2dh12_device_id_get(&dev_ctx, &whoamI);

	if (whoamI != LIS2DH12_ID) {
		printf("Wrong device identifier from LISDH12TR");
	}

	/* Enable Block Data Update. */
	// lis2dh12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

	/* Turn on the sensor, enable X, Y, and Z ODR = 100 Hz */
	lis2dh12_data_rate_set(&dev_ctx, LIS2DH12_ODR_100Hz);

	/* Disable high pass filter */ // High-pass filter disabled
	lis2dh12_high_pass_mode_set(&dev_ctx, LIS2DH12_NORMAL_WITH_RST);
	lis2dh12_high_pass_on_outputs_set(&dev_ctx, 0);
	lis2dh12_high_pass_int_conf_set(&dev_ctx, LIS2DH12_DISC_FROM_INT_GENERATOR);
	lis2dh12_high_pass_bandwidth_set(&dev_ctx, LIS2DH12_AGGRESSIVE);

	/* Write 40h into CTRL_REG3 */ // Interrupt activity 1 driven to INT1 pad
	lis2dh12_ctrl_reg3_t ctrl_reg3_value = {
			.i1_click = 0,
			.i1_ia1 = 1,
			.i1_ia2 = 0,
			.i1_zyxda = 0,
			.not_used_02 = 0,
			.i1_wtm = 0,
			.i1_overrun = 0,
			.not_used_01 = 0,
	};
	lis2dh12_pin_int1_config_set(&dev_ctx, &ctrl_reg3_value);

	/* Set full scale to 2g. */  // FS = ±2 g
	lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_2g);

	// /* Enable temperature sensor. */
	// lis2dh12_temperature_meas_set(&dev_ctx, LIS2DH12_TEMP_ENABLE);

	// /* Set device in continuous mode with 12 bit resol. */
	// lis2dh12_operating_mode_set(&dev_ctx, LIS2DH12_HR_12bit);

	/* Write 08h into CTRL_REG5 */ // Interrupt 1 pin latched
	lis2dh12_int1_pin_notification_mode_set(&dev_ctx, 1);

	/* Write 16h into INT1_THS */ // Set free-fall threshold = 350 mg
	lis2dh12_int1_gen_threshold_set(&dev_ctx, 0x16);

	/* Write 03h into INT1_DURATION */ // Set minimum event duration
	lis2dh12_int1_gen_duration_set(&dev_ctx, 0x10);

	/* Write 95h into INT1_CFG */ // Configure free-fall recognition
	lis2dh12_int1_cfg_t int1_cfg_value = {
			.aoi = 1,
			._6d = 0,
			.xhie = 0,
			.xlie = 1,
			.yhie = 0,
			.ylie = 1,
			.zhie = 0,
			.zlie = 1
	};
	lis2dh12_int1_gen_conf_set(&dev_ctx, &int1_cfg_value);
	/* USER CODE END 2 */

	/* Infinite loop */

	/* USER CODE BEGIN WHILE */
	app_board_test_init();
	// app_production_logic_init();

	while (1)
	{
		/* USER CODE END WHILE */
		MX_SubGHz_Phy_Process();

//		static uint32_t i2c_scanned_time = 0;
//		if(HAL_GetTick() - i2c_scanned_time >= 2000){
//			i2c_scanned_time = HAL_GetTick();
//			/*-[ I2C Bus Scanning ]-*/
//			for(uint8_t address = 1; address < 128; address++)
//			{
//				HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(address << 1), 3, 5);
//				if (ret != HAL_OK) /* No ACK Received At That Address */
//				{
//					// APP_LOG(1, VLEVEL_L, "No ACK Received At %02X\r\n", address);
//				}
//				else if(ret == HAL_OK)
//				{
//					APP_LOG(1, VLEVEL_L, "Found a Device at 0x%02X\r\n", address);
//				}
//			}
//			/*--[ Scanning Done ]--*/
//		}

		app_board_test_process();
		// app_production_logic_process();
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* Enable HSE Oscillator and Activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLLMUL_6;
  RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLLDIV_3;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set Voltage scale1 as MCU will run at 32MHz */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : PA5 */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Configure LED pins */
	LED_Init(MAIN_LED);
	LED_Init(RED_LED);
	LED_Init(GREEN_LED);
	LED_Init(BLUE_LED);
	/* Configure LED pins */

	LED_Off(MAIN_LED);
	LED_Off(RED_LED);
	LED_Off(GREEN_LED);
	LED_Off(BLUE_LED);

	/*Configure GPIO pin : WATER_SENSE_INT_PIN_Pin */
	GPIO_InitStruct.Pin = WATER_SENSE_INT_PIN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(WATER_SENSE_INT_PIN_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BUTTON_INT_PIN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(BUTTON_INT_PIN_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LISDH12_INT1_PIN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(LISDH12_INT1_PIN_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == BUTTON_INT_PIN_Pin)
    {
    	if(HAL_GPIO_ReadPin(BUTTON_INT_PIN_GPIO_Port, BUTTON_INT_PIN_Pin)){
    		printf("Button released ...");
    		if(application_in_state(STATE_BOARD_TESTING_BUTTON_PRESSED)){
    			application_transition_state(STATE_BOARD_TESTING_IDLE);
    		}
    	}
    	else{
    		printf("Button pressed ...");
    		if(application_in_state(STATE_BOARD_TESTING_IDLE)){
    			application_transition_state(STATE_BOARD_TESTING_BUTTON_PRESSED);
    		}
    	}
    }

    else if(GPIO_Pin == WATER_SENSE_INT_PIN_Pin)
    {
    	if(HAL_GPIO_ReadPin(WATER_SENSE_INT_PIN_GPIO_Port, WATER_SENSE_INT_PIN_Pin)){
    		printf("Water sensor deactivated ...");
    		if(application_in_state(STATE_BOARD_TESTING_WATER_DETECTION)){
    			application_transition_state(STATE_BOARD_TESTING_IDLE);
    		}
    	}
    	else{
    		printf("Water sensor activated ...");
    		if(application_in_state(STATE_BOARD_TESTING_IDLE)){
    			application_transition_state(STATE_BOARD_TESTING_WATER_DETECTION);
    		}
    	}
    }
    else if(GPIO_Pin == LISDH12_INT1_PIN_Pin)
    {
    	if(HAL_GPIO_ReadPin(LISDH12_INT1_PIN_GPIO_Port, LISDH12_INT1_PIN_Pin)){
    	}
    	else{
    		lis2dh12_int1_src_t int1_src_value;
    		lis2dh12_int1_gen_source_get(&dev_ctx, &int1_src_value);
    		printf("INT1 interrupt from LISDH12 ...");
    		if(!application_in_state(STATE_BOARD_TESTING_FREE_FALL_DETECTION)){
    			application_transition_state(STATE_BOARD_TESTING_FREE_FALL_DETECTION);
    		}
    	}
    }
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
static int32_t i2c1_data_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len)
{
	/* Write multiple command */
	reg |= 0x80;
	HAL_I2C_Mem_Write(handle, LIS2DH12_I2C_ADD_H, reg,
					I2C_MEMADD_SIZE_8BIT, (uint8_t*) bufp, len, 1000);
	return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
static int32_t i2c1_data_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
	/* Read multiple command */
	reg |= 0x80;
	HAL_I2C_Mem_Read(handle, LIS2DH12_I2C_ADD_H, reg,
				   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	return 0;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
