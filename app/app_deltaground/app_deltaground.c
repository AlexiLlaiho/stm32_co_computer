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
bool step_done = false;
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
void stepper_func(int16_t *coordinate_name, uint8_t num_moving, char direction);
bool change_direction(int16_t *what_is_changing, char where_is_changing);
void reset_variables(void);
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
		}
		else if (pulseWidth < 1000)
		{
			if (xTimerStop(xPeriodTimer, 0) != pdPASS) {
				asm("nop");
			}
			tr_enable = false;
			reset_variables();
		}
	vTaskDelay(10);
	}
}

void vTimChangeMotionCallback(xTimerHandle xTimer)
{
	if (step == 0){
		stepper_func(&dlt_lat, 8, 'N');
	}
	else if (step == 1)	{
		stepper_func(&dlt_lat, 8, 'B');
	}
	else if (step == 2)	{
		stepper_func(&dlt_lat, 8, 'S');
	}
	else if (step == 3)	{
		stepper_func(&dlt_lat, 8, 'B');
	}
	else if (step == 4)	{
		stepper_func(&dlt_lon, 8, 'W');
	}
	else if (step == 5)	{
		stepper_func(&dlt_lon, 8, 'B');
	}
	else if (step == 6)	{
		stepper_func(&dlt_lon, 8, 'E');
	}
	else if (step == 7)	{
		stepper_func(&dlt_lon, 8, 'B');
	}
}

void stepper_func(int16_t *coordinate_name, uint8_t num_moving, char direction)
{
	if((change_direction(coordinate_name, direction) == true) &&  (wait_time < req_time))
		wait_time++;
	if(wait_time == req_time)
	{
		step_done = false;
		if(step < (num_moving - 1))
			step++;
		else
			step = 0;

		wait_time = 0;
	}
}

bool change_direction(int16_t *what_is_changing, char where_is_changing)
{
	if (((where_is_changing == 'N') || (where_is_changing == 'E')) && ( *(what_is_changing) > (dst_range * (-1))) )
	{
		(*(what_is_changing))--;
	}
	else if (((where_is_changing == 'N') || (where_is_changing == 'E')) && ( *(what_is_changing) == (dst_range * (-1))) )
	{
		step_done = true;
	}

	if (((where_is_changing == 'S') || (where_is_changing == 'W')) && ( *(what_is_changing) < dst_range) )
	{
		(*(what_is_changing))++;
	}
	else if (((where_is_changing == 'S') || (where_is_changing == 'W')) && ( *(what_is_changing) == dst_range) )
	{
		step_done = true;
	}

	if ((where_is_changing == 'B') && (*(what_is_changing) != 0))
	{
		if(*(what_is_changing) > 0)
			(*(what_is_changing))--;
		else
			(*(what_is_changing))++;
	}
	else if ((where_is_changing == 'B') && (*(what_is_changing) == 0))
	{
		step_done = true;
	}

	return step_done;
}

void reset_variables(void)
{
	dlt_lat = 0;
	dlt_lon = 0;
	step = 0;
	wait_time = 0;
}
