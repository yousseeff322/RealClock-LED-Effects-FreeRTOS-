#include "main.h"





void RTC_voidShowTimeDate(void)
{

	RTC_TimeTypeDef   Local_RTCTime ;
	RTC_DateTypeDef   Local_RTCDate  ;

	char* Local_timeFormat ;

	static char ShowTime [50];
	static char ShowDate [50];

	char* Local_pTime = ShowTime ;
	char* Local_pDate = ShowDate ;

	/* Get the RTC Current time And Date  */

	(void)HAL_RTC_GetTime(&hrtc , &Local_RTCTime , RTC_FORMAT_BIN) ;

	(void)HAL_RTC_GetDate(&hrtc , &Local_RTCDate , RTC_FORMAT_BIN) ;

	/* Prepare The Format */
	if (Local_RTCTime.TimeFormat == RTC_HOURFORMAT12_AM)
	{
		Local_timeFormat = "AM" ;

	}
	else
	{
		Local_timeFormat = "PM" ;

	}
	/*Convert The Time Format Into String HH:MM:SS [AM/PM]*/
	(void) sprintf((char * )ShowTime , "%s:\t%02d:%02d:%02d [%s]" , "\nCurrent Time & Date" , Local_RTCTime.Hours , Local_RTCTime.Minutes,Local_RTCTime.Seconds , Local_timeFormat) ;
	/*Print */
	(void)	xQueueSend(Q_Print , &Local_pTime , portMAX_DELAY) ;

	/*Convert The Date Format Into String Date    DAY/MONTH/YEAR*/
	(void) sprintf((char * )ShowDate , "\t%02d:%02d:%02d" ,  Local_RTCDate.Date, Local_RTCDate.Month, (2000+Local_RTCDate.Year) ) ;
	/*Print */
	(void) xQueueSend(Q_Print , &Local_pDate , portMAX_DELAY) ;
}



void RTC_ConfigDate(uint8_t Copy_u8Day , uint8_t Copy_u8Month , uint8_t Copy_u8Year )
{
	RTC_DateTypeDef   Local_RTCDate  ;

	Local_RTCDate.Date = Copy_u8Day ;
	Local_RTCDate.Month = Copy_u8Month ;
	Local_RTCDate.Year = Copy_u8Year ;

	HAL_RTC_SetDate(&hrtc, &Local_RTCDate, RTC_FORMAT_BIN) ;

}



void RTC_ConfigTime(uint8_t Copy_u8Hours , uint8_t Copy_u8Minute , uint8_t Copy_u8Second )
{
	RTC_TimeTypeDef   Local_RTCTime ;

	Local_RTCTime.Hours = Copy_u8Hours ;
	Local_RTCTime.Minutes = Copy_u8Minute ;
	Local_RTCTime.Seconds = Copy_u8Second ;

	HAL_RTC_SetTime(&hrtc, &Local_RTCTime, RTC_FORMAT_BIN) ;

}

uint8_t RTC_u8VIsRTCTimeConfigValid(uint8_t Copy_u8Hours , uint8_t Copy_u8Minute , uint8_t Copy_u8Second )
{
	uint8_t VaildState ;
		if( (Copy_u8Hours > 12u)  || (Copy_u8Minute >59u )|| (Copy_u8Second >59u) || (Copy_u8Hours < 0u)  || (Copy_u8Minute <0u )|| (Copy_u8Second <0u))
		{
			VaildState = 0u ;
		}
		else
		{
			VaildState = 1u ;
		}

		return VaildState ;
}

uint8_t RTC_u8VIsRDateConfigValid(uint8_t Copy_u8Day , uint8_t Copy_u8Month , uint8_t Copy_u8Year )
{
	uint8_t VaildState ;
		if( (Copy_u8Day > 31u)  || (Copy_u8Month >12u )|| (Copy_u8Year >99u) || (Copy_u8Year < 0u)  || (Copy_u8Month <0u )|| (Copy_u8Day <0u))
		{
			VaildState = 0u ;
		}
		else
		{
			VaildState = 1u ;
		}

		return VaildState ;
}
