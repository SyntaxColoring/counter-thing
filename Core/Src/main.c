/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdatomic.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
// Cleared when there is a button press to acknowledge. Set when it's been acknowledged.
volatile atomic_flag button_flag = ATOMIC_FLAG_INIT;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

struct full_digit_segs
{
	_Bool a;
	_Bool b;
	_Bool c;
	_Bool d;
	_Bool e;
	_Bool f;
	_Bool g;
};

struct display_segs
{
	_Bool hundreds; // Our display can only show 1 or nothing for the hundreds digit.
	struct full_digit_segs tens;
	struct full_digit_segs ones;
};

static struct full_digit_segs encode_digit(unsigned digit, _Bool encode_zero_as_blank) {
	switch (digit)
	{
	case 0:
		return encode_zero_as_blank
			? (struct full_digit_segs){}
			: (struct full_digit_segs){.a=1,.b=1,.c=1,.d=1,.e=1,.f=1};
	case 1:
		return (struct full_digit_segs){.b=1,.c=1};
	case 2:
		return (struct full_digit_segs){.a=1,.b=1,.d=1,.e=1,.g=1};
	case 3:
		return (struct full_digit_segs){.a=1,.b=1,.c=1,.d=1,.g=1};
	case 4:
		return (struct full_digit_segs){.b=1,.c=1,.f=1,.g=1};
	case 5:
		return (struct full_digit_segs){.a=1,.c=1,.d=1,.f=1,.g=1};
	case 6:
		return (struct full_digit_segs){.a=1,.c=1,.d=1,.e=1,.f=1,.g=1};
	case 7:
		return (struct full_digit_segs){.a=1,.b=1,.c=1};
	case 8:
		return (struct full_digit_segs){.a=1,.b=1,.c=1,.d=1,.e=1,.f=1,.g=1};
	case 9:
		return (struct full_digit_segs){.a=1,.b=1,.c=1,.d=1,.f=1,.g=1};
	default:
		return (struct full_digit_segs){};
	}
}

static struct display_segs encode_number(unsigned n) {
	const unsigned ones = n % 10;
	const unsigned tens = n % 100 / 10;
	const unsigned hundreds = n % 1000 / 100;

	const _Bool blank_tens_zero = !hundreds;

	return (struct display_segs){
		.ones=encode_digit(ones, 0),
		.tens=encode_digit(tens, blank_tens_zero),
		.hundreds=!!hundreds,
	};
}

static void write_display_pins(struct display_segs display_segs, _Bool phase) {
	HAL_GPIO_WritePin(COM_GPIO_Port, COM_Pin, phase);

	// Hundreds digit:
	HAL_GPIO_WritePin(SEG_1BC_GPIO_Port, SEG_1BC_Pin, display_segs.hundreds ^ phase);

	// Tens digit:
	HAL_GPIO_WritePin(SEG_2A_GPIO_Port, SEG_2A_Pin, display_segs.tens.a ^ phase);
	HAL_GPIO_WritePin(SEG_2B_GPIO_Port, SEG_2B_Pin, display_segs.tens.b ^ phase);
	HAL_GPIO_WritePin(SEG_2C_GPIO_Port, SEG_2C_Pin, display_segs.tens.c ^ phase);
	HAL_GPIO_WritePin(SEG_2D_GPIO_Port, SEG_2D_Pin, display_segs.tens.d ^ phase);
	HAL_GPIO_WritePin(SEG_2E_GPIO_Port, SEG_2E_Pin, display_segs.tens.e ^ phase);
	HAL_GPIO_WritePin(SEG_2F_GPIO_Port, SEG_2F_Pin, display_segs.tens.f ^ phase);
	HAL_GPIO_WritePin(SEG_2G_GPIO_Port, SEG_2G_Pin, display_segs.tens.g ^ phase);

	// Ones digit:
	HAL_GPIO_WritePin(SEG_3A_GPIO_Port, SEG_3A_Pin, display_segs.ones.a ^ phase);
	HAL_GPIO_WritePin(SEG_3B_GPIO_Port, SEG_3B_Pin, display_segs.ones.b ^ phase);
	HAL_GPIO_WritePin(SEG_3C_GPIO_Port, SEG_3C_Pin, display_segs.ones.c ^ phase);
	HAL_GPIO_WritePin(SEG_3D_GPIO_Port, SEG_3D_Pin, display_segs.ones.d ^ phase);
	HAL_GPIO_WritePin(SEG_3E_GPIO_Port, SEG_3E_Pin, display_segs.ones.e ^ phase);
	HAL_GPIO_WritePin(SEG_3F_GPIO_Port, SEG_3F_Pin, display_segs.ones.f ^ phase);
	HAL_GPIO_WritePin(SEG_3G_GPIO_Port, SEG_3G_Pin, display_segs.ones.g ^ phase);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	atomic_flag_clear(&button_flag);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  atomic_flag_test_and_set(&button_flag);
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
  MX_GPIO_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  _Bool phase = 0;
  unsigned count = 0;
  unsigned ticks = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (ticks >= 32)
	  {
		  ticks = 0;
		  count++;
	  }

	  if (count > 199) count = 0;

	  const _Bool button_pressed_since_last_loop = !atomic_flag_test_and_set(&button_flag);
	  if (button_pressed_since_last_loop)
	  {
		  ticks = 0;
		  count = 0;
	  }

	  write_display_pins(encode_number(count), phase);

	  ticks++;
	  phase = !phase;

	  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_BYPASS;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 64, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SEG_3A_Pin|SEG_3B_Pin|SEG_3C_Pin|SEG_3D_Pin
                          |SEG_3F_Pin|SEG_1BC_Pin|COM_Pin|SEG_2D_Pin
                          |SEG_2E_Pin|SEG_2G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG_3E_Pin|SEG_3G_Pin|SEG_2F_Pin|SEG_2A_Pin
                          |SEG_2B_Pin|SEG_2C_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SEG_3A_Pin SEG_3B_Pin SEG_3C_Pin SEG_3D_Pin
                           SEG_3F_Pin SEG_1BC_Pin COM_Pin SEG_2D_Pin
                           SEG_2E_Pin SEG_2G_Pin */
  GPIO_InitStruct.Pin = SEG_3A_Pin|SEG_3B_Pin|SEG_3C_Pin|SEG_3D_Pin
                          |SEG_3F_Pin|SEG_1BC_Pin|COM_Pin|SEG_2D_Pin
                          |SEG_2E_Pin|SEG_2G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG_3E_Pin SEG_3G_Pin SEG_2F_Pin SEG_2A_Pin
                           SEG_2B_Pin SEG_2C_Pin */
  GPIO_InitStruct.Pin = SEG_3E_Pin|SEG_3G_Pin|SEG_2F_Pin|SEG_2A_Pin
                          |SEG_2B_Pin|SEG_2C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
