#include "main.h"


static void TurnOnEvenLeds(void) ;
static void TurnOnOddLeds(void) ;
static void LEDControl (uint8_t Copy_u8Num) ;

GPIO_TypeDef* LEDPort  = LEDS_PORT ;
uint16_t LEDPin[8] = {LED1_PIN , LED2_PIN , LED3_PIN , LED4_PIN ,LED5_PIN, LED6_PIN, LED7_PIN , LED8_PIN} ;


void LEDEffectStart (uint8_t Copy_u8LedEffNum)
{
	/* Stop All Sw Timers and animations */
	LEDEffectStop() ;
	xTimerStart ( HandleLedTimer[Copy_u8LedEffNum-1] , portMAX_DELAY) ;

}

void LEDEffectStop(void)
{
	uint8_t local_u8Counter ;
	for(local_u8Counter = 0u ; local_u8Counter < 4u ; local_u8Counter++)
	{
	xTimerStop( HandleLedTimer[local_u8Counter] , portMAX_DELAY) ;
	}
}


void ledEffectAction1(void)
{
    uint8_t Local_u8Counter;
    for (Local_u8Counter = 0u; Local_u8Counter < 8; Local_u8Counter++)
    {
        HAL_GPIO_TogglePin(LEDPort, LEDPin[Local_u8Counter]);
    }
}


void ledEffectAction2(void)
{
	static uint8_t Local_u8Flag = 0u ;

	if(Local_u8Flag == 0u )
	{

		TurnOnEvenLeds() ;
	}
	else
	{
		TurnOnOddLeds() ;

	}
		Local_u8Flag ^=1u ;
}

/* Turn on even-numbered LEDs (1, 3, 5, 7) */
static void TurnOnEvenLeds(void)
{
    HAL_GPIO_WritePin(LEDPort, LED1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LEDPort, LED2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEDPort, LED3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LEDPort, LED4_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEDPort, LED5_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LEDPort, LED6_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEDPort, LED7_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LEDPort, LED8_PIN, GPIO_PIN_RESET);
}

/* Turn on odd-numbered LEDs (2, 4, 6, 8) */
static void TurnOnOddLeds(void)
{
    HAL_GPIO_WritePin(LEDPort, LED1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEDPort, LED2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LEDPort, LED3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEDPort, LED4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LEDPort, LED5_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEDPort, LED6_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LEDPort, LED7_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEDPort, LED8_PIN, GPIO_PIN_SET);
}

/* LED Effect 3: Shift Left */
void ledEffectAction3(void)
{
    static uint8_t Local_u8Num = 0u;
    LEDControl(1 << Local_u8Num); // Shift left by Local_u8Num
    Local_u8Num++;
    Local_u8Num %= 8; // Keep the value between 0 and 7
}

/* LED Effect 4: Shift Right */
void ledEffectAction4(void)
{
    static uint8_t Local_u8Num = 0u;
    LEDControl(1 << (7 - Local_u8Num)); // Shift right by (7 - Local_u8Num)
    Local_u8Num++;
    Local_u8Num %= 8; // Keep the value between 0 and 7
}

/* Control the LEDs by setting them based on the given bitmask */
static void LEDControl(uint8_t Copy_u8Num)
{
    uint8_t Local_u8Counter;
    for (Local_u8Counter = 0u; Local_u8Counter < 8u; Local_u8Counter++)
    {
        /* Write the value of each LED based on the corresponding bit in Copy_u8Num */
        HAL_GPIO_WritePin(LEDPort, LEDPin[Local_u8Counter], (Copy_u8Num & (1 << Local_u8Counter)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}
