/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SEG_3A_Pin GPIO_PIN_4
#define SEG_3A_GPIO_Port GPIOA
#define SEG_3B_Pin GPIO_PIN_5
#define SEG_3B_GPIO_Port GPIOA
#define SEG_3C_Pin GPIO_PIN_6
#define SEG_3C_GPIO_Port GPIOA
#define SEG_3D_Pin GPIO_PIN_7
#define SEG_3D_GPIO_Port GPIOA
#define SEG_3E_Pin GPIO_PIN_0
#define SEG_3E_GPIO_Port GPIOB
#define SEG_3G_Pin GPIO_PIN_1
#define SEG_3G_GPIO_Port GPIOB
#define SEG_3F_Pin GPIO_PIN_8
#define SEG_3F_GPIO_Port GPIOA
#define SEG_1BC_Pin GPIO_PIN_9
#define SEG_1BC_GPIO_Port GPIOA
#define COM_Pin GPIO_PIN_10
#define COM_GPIO_Port GPIOA
#define SEG_2D_Pin GPIO_PIN_11
#define SEG_2D_GPIO_Port GPIOA
#define SEG_2E_Pin GPIO_PIN_12
#define SEG_2E_GPIO_Port GPIOA
#define SEG_2G_Pin GPIO_PIN_15
#define SEG_2G_GPIO_Port GPIOA
#define SEG_2F_Pin GPIO_PIN_3
#define SEG_2F_GPIO_Port GPIOB
#define SEG_2A_Pin GPIO_PIN_4
#define SEG_2A_GPIO_Port GPIOB
#define SEG_2B_Pin GPIO_PIN_5
#define SEG_2B_GPIO_Port GPIOB
#define SEG_2C_Pin GPIO_PIN_6
#define SEG_2C_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_7
#define BUTTON_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
