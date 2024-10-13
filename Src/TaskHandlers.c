#include "main.h"

#define HH_STATE 	0
#define MM_STATE 	1
#define SS_STATE 	2


#define DAY_STATE 		0
#define MONTH_STATE 	1
#define YEAR_STATE 		2






static void  ProcessCommand (Command_t  *Copy_Command)  ;
static void  ExtractCommand (Command_t  *Copy_Command) ;
static uint8_t u8GetNumbers (const char* Copy_ASCINum  ,uint8_t Length ) ;

const char* MsgInvelid = "\n************************************************\n"
		"\t////Invelid Option//////\t\n"
		"*************************************************" ;



void MenuTaskHandler (void *pv)
{
	Command_t *Local_Cmd ;
	uint32_t Local_u32CmdAddress ;

	const char* MsgMenu = "\r===============================================\n"
			"                     MENU                      \n"
			"===============================================\n"
			"LED Effect                             ----> 0 \n"
			"Data And Time                          ----> 1 \n"
			"EXIT                                   ----> 2 \n"
			"Enter Your Choice Here :         " ;


	while(1)
	{
		xQueueSend(Q_Print , (void *)&MsgMenu , portMAX_DELAY ) ;
		xTaskNotifyWait(0 , 0 , &Local_u32CmdAddress , portMAX_DELAY) ;

		Local_Cmd  = (Command_t * )Local_u32CmdAddress ;

		/*Command Length should be 1 */
		if (Local_Cmd ->Lengtht == 1 )
		{
			switch (Local_Cmd->PayLoad[0])
			{
			case '0' :
				CurrentState = LedEffectState ;
				xTaskNotify(HandleLedTask , 0 , eNoAction) ;
				break ;
			case '1' :
				CurrentState = RTCMenuState ;
				xTaskNotify(HandleRTCTask , 0 , eNoAction) ;
				break ;

			case '2' :
				/*exit */
				break ;
			default  :
				xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
				continue ;
			}
		}
		else {
			/* INVAILED ENTERY */
			xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
			continue ;
		}

		/*Wait to run again when some other task notify it /// same handle of action of exit  */
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY)  ;
	}
}

void LedTaskHandler (void *pv)
{
	uint32_t Local_u32CmdAddress ;
	Command_t *Local_Cmd ;

	const char* MsgLED  = "\r===============================================\n"
			"                     LED EFFECT                \n"
			"===============================================\n"
			"NONE                 	     		----> NO \n"
			"LED Effect (1)                 	        ----> E1 \n"
			"LED Effect (2)                 	        ----> E2 \n"
			"LED Effect (3)                 	        ----> E3 \n"
			"LED Effect (4)                 	        ----> E4 \n"
			"Enter Your Choice Here :      				   " ;

	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY) ;
		xQueueSend(Q_Print , (void *)&MsgLED , portMAX_DELAY ) ;

		/* Wait for Command */
		xTaskNotifyWait(0 , 0 , &Local_u32CmdAddress , portMAX_DELAY) ;

		Local_Cmd  = (Command_t * )Local_u32CmdAddress ;

		/*Command Length should be 2 */
		if (Local_Cmd ->Lengtht == 2 )
		{
			if(strcmp((char *)Local_Cmd->PayLoad , "NO") == 0)
			{
				LEDEffectStop() ;


			}

			else if(strcmp((char *)Local_Cmd->PayLoad , "E1") == 0)
			{
				LEDEffectStart(1) ;

			}
			else if(strcmp((char *)Local_Cmd->PayLoad , "E2") == 0)
			{
				LEDEffectStart(2) ;

			}
			else if(strcmp((char *)Local_Cmd->PayLoad , "E3") == 0)
			{
				LEDEffectStart(3) ;

			}
			else if(strcmp((char *)Local_Cmd->PayLoad , "E4") == 0)
			{
				LEDEffectStart(4) ;

			}
			else {

				/* INVAILED ENTERY */
				xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;

			}


		}
		else {
			/* INVAILED ENTERY */
			xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
		}
		/*update State */
		CurrentState = MainMenuState ;
		/*Notify Menu Task */
		xTaskNotify(HandleMenuTask , 0 , eNoAction ) ;
		/*Wait to run again when some other task notify it /// same handle of action of exit  */
		//xTaskNotifyWait(0,0,NULL,portMAX_DELAY)  ;

	}
}
void CMDTaskHandler (void *pv)
{
	BaseType_t Local_ReturnValue ;

	Command_t Cmd ;
	while(1)
	{
		/* implement notfiy wait */
		Local_ReturnValue = xTaskNotifyWait(0,0 ,NULL ,portMAX_DELAY) ;

		if (Local_ReturnValue == pdTRUE )
		{
			ProcessCommand(&Cmd) ;

		}
	}
}

static void  ProcessCommand (Command_t  *Copy_Command)
{
	ExtractCommand(Copy_Command) ;

	switch (CurrentState)
	{

	case MainMenuState :
		/*Notify main menu */
		xTaskNotify(HandleMenuTask , (uint32_t) Copy_Command , eSetValueWithOverwrite ) ;
		break ;

	case LedEffectState :
		/*Notify led  */
		xTaskNotify(HandleLedTask , (uint32_t) Copy_Command , eSetValueWithOverwrite ) ;
		break ;

	case RTCMenuState :
	case RTCTimerConfigState :
	case RTCTDateConfigState :
		/*Notify RTC  */
		xTaskNotify(HandleRTCTask , (uint32_t) Copy_Command , eSetValueWithOverwrite ) ;
		break ;

	}


}

static void  ExtractCommand (Command_t  *Copy_Command)
{
	UBaseType_t Local_numOfMessage ;
	uint8_t  counter ;
	Local_numOfMessage = uxQueueMessagesWaiting(Q_Data) ;

	for ( counter = 0u ; counter < Local_numOfMessage ; counter++)
	{
		xQueueReceive(Q_Data,(void * ) &Copy_Command->PayLoad[counter], 0) ;

		if ((char)Copy_Command->PayLoad[counter] =='\r')
		{
			break ;
		}
	}
	/* PUT NULL CHAR AT end of command to make it string  */
	Copy_Command->PayLoad[counter] = '\0';

	/* Save Length */
	Copy_Command->Lengtht = counter  ;
}

void RTCTaskHandler (void *pv)
{

	Command_t *Local_Cmd ;
	uint32_t Local_u32CmdAddress ;

	uint8_t Local_u8RTCCfgState = 0 ;

	uint8_t Local_u8Hours , Local_u8Minute , Local_u8Sec ;
	uint8_t Local_u8Day , Local_u8Month , Local_u8Year ;


	const char* MsgRTC1 =     "===============================================\n"
			"                   RTC  MENU                      \n"
			"===============================================\n" ;


	const char* MsgRTC2 = 	  "\nConfigure Time                  ---->0\n"
			"\nConfigure Date                  ---->1\n"
			"\nEnable Reporting                ---->2\n"
			"Enter Your Choice Here :         " ;


	const char* MsgRTCHoursConfig = " Enter Hour (1-12) : " ;
	const char* MsgRTCMintusConfig =" Enter Minutes (0-59) : " ;
	const char* MsgRTCSecondConfig = " Enter Second (0-59) : " ;

	const char* MsgRTCDayConfig = " Enter Day (1-31) : " ;
	const char* MsgRTCMonthConfig = "Enter Month (1-12) : " ;
	const char* MsgRTCYearConfig = " Enter Year (0-99) : " ;

	const char* MsgRTCConfigSuccess = "Configuration Successful\n" ;
	const char* MsgRTCEnableReporting = "Enable Time Date Reporting (y/n) : " ;

	while(1)
	{
		/*Notify Wait  (wait till Someone Notify me )*/
		xTaskNotifyWait(0 ,0, NULL ,portMAX_DELAY) ;
		/*Print The Menu + show the current Date and time */
		xQueueSend(Q_Print ,(void * ) &MsgRTC1 , portMAX_DELAY) ;
		RTC_voidShowTimeDate() ;

		xQueueSend(Q_Print ,(void * ) &MsgRTC2 , portMAX_DELAY) ;



		/*Loop until MainMenuState is Invoked*/
		while (CurrentState != MainMenuState)
		{
			xTaskNotifyWait(0 , 0 , &Local_u32CmdAddress , portMAX_DELAY) ;
			Local_Cmd  = (Command_t * )Local_u32CmdAddress ;
			switch (CurrentState)
			{
			case RTCMenuState :
				if(Local_Cmd->Lengtht == 1)
				{
					switch (Local_Cmd->PayLoad[0])
					{
					case '0' :
						CurrentState = RTCTimerConfigState ;
						xQueueSend(Q_Print, (void * )&MsgRTCHoursConfig, portMAX_DELAY) ;
						break ;


					case '1' :
						CurrentState = RTCTDateConfigState ;
						xQueueSend(Q_Print, (void * )&MsgRTCDayConfig, portMAX_DELAY) ;
						break ;


					case '2' :
						CurrentState = RTCReportState ;
						xQueueSend(Q_Print, (void * )&MsgRTCEnableReporting, portMAX_DELAY) ;
						break ;


					case '3' :
						CurrentState = MainMenuState ;
						break ;


					default :
						// invailed
						CurrentState = MainMenuState ;
						xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
						break ;
					}


				}
				else
				{
					// invailed
					CurrentState = MainMenuState ;
					xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
				}
				break ;


			case RTCTimerConfigState:
				if(Local_Cmd->Lengtht <= 2)
				{

					/*GET HH MM SS and config with invailed entry */
					switch(Local_u8RTCCfgState)
					{
					case HH_STATE :
						Local_u8Hours = u8GetNumbers((char*) Local_Cmd->PayLoad, Local_Cmd->Lengtht) ;
						xQueueSend(Q_Print , (void * )&MsgRTCMintusConfig , portMAX_DELAY ) ;
						Local_u8RTCCfgState = MM_STATE ;
						break ;
					case MM_STATE :
						Local_u8Minute = u8GetNumbers((char*) Local_Cmd->PayLoad, Local_Cmd->Lengtht) ;
						xQueueSend(Q_Print , (void * )&MsgRTCSecondConfig , portMAX_DELAY ) ;
						Local_u8RTCCfgState = SS_STATE ;
						break ;
					case SS_STATE :
						Local_u8Sec = u8GetNumbers((char*) Local_Cmd->PayLoad, Local_Cmd->Lengtht) ;
						if (RTC_u8VIsRTCTimeConfigValid(Local_u8Hours , Local_u8Minute , Local_u8Sec))
						{
							RTC_ConfigTime(Local_u8Hours, Local_u8Minute, Local_u8Sec) ;
							xQueueSend(Q_Print , (void * )&MsgRTCConfigSuccess , portMAX_DELAY ) ;
							RTC_voidShowTimeDate() ;
							CurrentState = MainMenuState ;

						}
						else
						{
							//invailed
							CurrentState = MainMenuState ;
							xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
						}
						Local_u8RTCCfgState = 0 ;
						break ;

					}
				}
				else
				{
					// invailed
					CurrentState = MainMenuState ;
					xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
				}
				break ;

			case RTCTDateConfigState:
				if(Local_Cmd->Lengtht <= 2)
				{

					/*GET dat month year and config with invailed entry */
					switch(Local_u8RTCCfgState)
					{
					case DAY_STATE :
						Local_u8Day = u8GetNumbers((char*) Local_Cmd->PayLoad, Local_Cmd->Lengtht) ;
						xQueueSend(Q_Print , (void * )&MsgRTCMonthConfig , portMAX_DELAY ) ;
						Local_u8RTCCfgState = MONTH_STATE ;
						break ;
					case MONTH_STATE :
						Local_u8Month = u8GetNumbers((char*) Local_Cmd->PayLoad, Local_Cmd->Lengtht) ;
						xQueueSend(Q_Print , (void * )&MsgRTCYearConfig , portMAX_DELAY ) ;
						Local_u8RTCCfgState = YEAR_STATE ;
						break ;
					case YEAR_STATE :
						Local_u8Year = u8GetNumbers((char*) Local_Cmd->PayLoad, Local_Cmd->Lengtht) ;
						if (RTC_u8VIsRDateConfigValid(Local_u8Day , Local_u8Month , Local_u8Year))
						{
							RTC_ConfigDate(Local_u8Day, Local_u8Month, Local_u8Year) ;
							xQueueSend(Q_Print , (void * )&MsgRTCConfigSuccess , portMAX_DELAY ) ;
							RTC_voidShowTimeDate() ;
							CurrentState = MainMenuState ;

						}
						else
						{
							//invailed
							CurrentState = MainMenuState ;
							xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
						}
						Local_u8RTCCfgState = 0 ;
						break ;

					}
				}
				else
				{
					// invailed
					CurrentState = MainMenuState ;
					xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;
				}
				break ;

			case RTCReportState:
				/*Start Reporting Timer*/
				break;

			case Exit_From_RTC:	break;
					default:
						/*Invalid Option*/
						CurrentState = MainMenuState;
						xQueueSend(Q_Print , (void *)&MsgInvelid , portMAX_DELAY ) ;

						break;

			}
			/* wait for command */
		}

		xTaskNotify(HandleMenuTask , 0 ,eNoAction) ;
	}
}
void PrintTaskHandler (void *pv)
{
	uint32_t* Local_pv32Msg ;

	while(1)
	{
		xQueueReceive(Q_Print, &Local_pv32Msg , portMAX_DELAY) ;
		HAL_UART_Transmit(&huart2, (uint8_t*)Local_pv32Msg, strlen((char*)Local_pv32Msg), HAL_MAX_DELAY) ;

	}
}

static uint8_t u8GetNumbers (const char* Copy_ASCINum  ,uint8_t Copy_u8Length )
{
	uint8_t Local_u8DesimalNum ;
	if(Copy_u8Length ==2  )
	{
		Local_u8DesimalNum = ((Copy_ASCINum[0] - '0') *  10) + (Copy_ASCINum[1] - '0');

	}
	else {

		Local_u8DesimalNum = (Copy_ASCINum[0] - '0');
	}


	return Local_u8DesimalNum ;
}
