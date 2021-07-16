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

bool tr_enable = false;
extern QueueHandle_t xGRDDTQueue;
extern UART_HandleTypeDef huart3;
uint8_t *qgnd;
uint8_t delta_packet[] = {'D', 'e', 'l', 't', 'a', '=', '0', '0','0','0'};
extern int16_t pgndres;

void StartDeltaGroundTask(void const *argument)
{
	for (;;)
	{
		if (xGRDDTQueue != NULL)
		{
			if (xQueueReceive(xGRDDTQueue, &(qgnd), (TickType_t) 2) == pdPASS)
			{
				pgndres = delta_minus_gps(qgnd);
//				for(uint8_t local_i = 0; local_i < 4; local_i++)
//				{
//					delta_packet[local_i + 6] = *(qgnd + local_i);
//				}
			}
		}
	}
}
