/*!
 *******************************************************************************
 * @file    .c
 * @author	Gorbunov
 * @version V0.0.1
 * @date    1.07.2021
 * @brief
 * @details
 *******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT </center></h2>
 *******************************************************************************
 */
#include "app_deltaground.h"

/********* Global variables ***************************************************/
extern QueueHandle_t xGRDDTQueue;
extern UART_HandleTypeDef huart3;
extern int16_t delta_lat, delta_lon;
extern uint32_t pulseWidth;
/******************************************************************************/

/********* Private variables **************************************************/
static osTimerId Timer_1;
bool tr_enable = false;
uint8_t *qgnd;
TimerHandle_t xPeriodTimer;
uint8_t step = 0;
/******************************************************************************/

/******** Private functions ***************************************************/
void vTimChangeMotionCallback(xTimerHandle xTimer);
/******************************************************************************/

void StartDeltaGroundTask(void const *argument)
{
	xPeriodTimer = xTimerCreate("Timer_1", 1000, pdTRUE, ( void * ) 0, vTimChangeMotionCallback);
	if(xPeriodTimer == NULL){
		asm("nop");
	}
	else
		xTimerStart(Timer_1, 0);

	for (;;)
	{
		if(pulseWidth > 1800)
		{

		}
		else
		{
//			if(xTimerStop(Timer_1, 0 ) != pdPASS){
//				asm("nop");
//			}
		}
	}
}

void vTimChangeMotionCallback(xTimerHandle xTimer)
{
	if(step == 0)
	{
		delta_lat = -25;
		step++;
	}
	else if(step == 1)
	{
		delta_lon = 25;
		step++;
	}
	else if (step == 2)
	{
		delta_lat = 0;
		step++;
	}
	else if (step == 3)
	{
		delta_lon = 0;
		step = 0;
	}
}
