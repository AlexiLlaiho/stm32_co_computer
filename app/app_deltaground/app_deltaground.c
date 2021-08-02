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
extern int16_t dlt_lat, dlt_lon;
extern uint32_t pulseWidth;
/******************************************************************************/

/********* Private variables **************************************************/
bool tr_enable = false;
uint8_t *qgnd;
TimerHandle_t xPeriodTimer;
BaseType_t xPeriodTimerState;
uint8_t step = 0;
/******************************************************************************/

/******** Private functions ***************************************************/
void vTimChangeMotionCallback(xTimerHandle xTimer);
/******************************************************************************/

void StartDeltaGroundTask(void const *argument)
{
	xPeriodTimer = xTimerCreate("Timer_1", 	20000/portTICK_RATE_MS, pdTRUE, ( void * ) 0, vTimChangeMotionCallback);
	if(xPeriodTimer == NULL)
	{
		asm("nop");
	}

	for (;;)
	{
		if ((pulseWidth > 1800) && (tr_enable == false))
		{
			if (xTimerStart(xPeriodTimer, 0) != pdPASS) {
				asm("nop");
			}
			tr_enable = true;
			dlt_lat = -15;
			step++;
		}
		else if (pulseWidth < 1000)
		{
			if (xTimerStop(xPeriodTimer, 0) != pdPASS) {
				asm("nop");
			}
			tr_enable = false;
			dlt_lat = 0;
			dlt_lon = 0;
		}
	vTaskDelay(10);
	}
}

void vTimChangeMotionCallback(xTimerHandle xTimer)
{
	if(step == 1)
	{
		dlt_lon = 15;
		step++;
	}
	else if (step == 2)
	{
		dlt_lat = 0;
		step++;
	}
	else if (step == 3)
	{
		dlt_lon = 0;
		step = 0;
	}
}
