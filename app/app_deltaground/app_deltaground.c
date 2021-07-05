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

extern QueueHandle_t xGRDDTQueue;
uint8_t *qgnd;
extern int16_t *pgndres;

void StartDeltaGroundTask(void const *argument)
{
	for (;;)
	{
		if (xGRDDTQueue != NULL)
		{
			if (xQueueReceive(xGRDDTQueue, &(qgnd), (TickType_t) 2) == pdPASS)
			{
				pgndres = delta_minus_gps(qgnd);
			}
		}
	}
}
