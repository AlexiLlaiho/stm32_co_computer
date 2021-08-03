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
uint8_t wait_time = 0;
uint8_t req_time = 21;
int8_t dst_range = 21;
/******************************************************************************/

/******** Private functions ***************************************************/
void vTimChangeMotionCallback(xTimerHandle xTimer);
/******************************************************************************/

void StartDeltaGroundTask(void const *argument)
{
	xPeriodTimer = xTimerCreate("Timer_1", 	1000/portTICK_RATE_MS, pdTRUE, ( void * ) 0, vTimChangeMotionCallback);
	if(xPeriodTimer == NULL){
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
			dlt_lat = -1;
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
	if (step == 0)
	{

	}
	else if (step == 1)
	{

	}
	else if (step == 2)
	{

	}
	else if (step == 3)
	{

	}
}

void stepper_func(int16_t *coordinate_name, uint8_t num_moving)
{
	if (*(coordinate_name) > (dst_range * (-1)))
		(*(coordinate_name))--;
	else if (wait_time < req_time)
		wait_time++;
	else
	{
		if(step < num_moving)
			step++;
		else
			step = 0;

		wait_time = 0;
	}
}
