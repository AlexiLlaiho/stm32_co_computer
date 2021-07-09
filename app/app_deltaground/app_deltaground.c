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
uint8_t delta_packet[] = {'D', 'e', 'l', 't', 'a', '=', 0, 0};
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
				if(pgndres < 0)
					delta_packet[6] = '-';
				else
					delta_packet[6] = '+';
				delta_packet[7] = (uint8_t)pgndres;
//				tr_enable = true;
			}
		}
		if(tr_enable == true)
			HAL_UART_Transmit_IT(&huart3, delta_packet, 8);
	}
}
