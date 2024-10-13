/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include  <string.h>
#include "timers.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct {

	uint8_t PayLoad[10] ;
	uint8_t Lengtht ;

}Command_t ;

typedef enum {

	MainMenuState ,
	LedEffectState ,
	RTCMenuState ,
	RTCTimerConfigState ,
	RTCTDateConfigState ,
	RTCReportState ,
	Exit_From_RTC
}State_t ;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

extern State_t CurrentState ;

extern TaskHandle_t  HandleMenuTask ,HandleLedTask ,  HandleCmdTask  , HandleRTCTask , HandlePrintTask;
extern QueueHandle_t   Q_Print , Q_Data ;
extern TimerHandle_t  HandleLedTimer[4] ;


extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef huart2;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void LEDEffectStart (uint8_t Copy_u8LedEffNum);
void LEDEffectStop(void);

void ledEffectAction1(void);
void ledEffectAction2(void);
void ledEffectAction3(void);
void ledEffectAction4(void);
void RTC_voidShowTimeDate(void) ;
void RTC_ConfigDate(uint8_t Copy_u8Day , uint8_t Copy_u8Month , uint8_t Copy_u8Year ) ;
void RTC_ConfigTime(uint8_t Copy_u8Hours , uint8_t Copy_u8Minute , uint8_t Copy_u8Second ) ;
uint8_t RTC_u8VIsRDateConfigValid(uint8_t Copy_u8Day , uint8_t Copy_u8Month , uint8_t Copy_u8Year ) ;
uint8_t RTC_u8VIsRTCTimeConfigValid(uint8_t Copy_u8Hours , uint8_t Copy_u8Minute , uint8_t Copy_u8Second );



/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

#define LEDS_PORT    GPIOB

#define LED1_PIN	GPIO_PIN_0
#define LED2_PIN	GPIO_PIN_1
#define LED3_PIN	GPIO_PIN_2
#define LED4_PIN	GPIO_PIN_3
#define LED5_PIN	GPIO_PIN_4
#define LED6_PIN	GPIO_PIN_5
#define LED7_PIN	GPIO_PIN_6
#define LED8_PIN	GPIO_PIN_7


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
