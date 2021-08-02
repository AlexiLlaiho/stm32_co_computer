/*!
 *******************************************************************************
 * @file    .c
 * @author	Gorbunov
 * @version V0.0.1
 * @date    24.06.2021
 * @brief
 * @details
 *******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT </center></h2>
 *******************************************************************************
 */
#include "app_gps.h"
extern QueueHandle_t xRGPSQueue;
extern uint8_t buffer[];
extern UART_HandleTypeDef huart3;
extern uint8_t delta_packet[];
int16_t dlt_lat = 0, dlt_lon = 0;
uint8_t *qgps;

void StartDefaultTask(void const *argument)
{
	for (;;)
	{
		if (xRGPSQueue != NULL)
		{
			if( xQueueReceive( xRGPSQueue, &(qgps), (TickType_t ) 2 ) == pdPASS )
			{
					GPS_Analyze(qgps);
					uint8_t lenght = 0;
					uint8_t *p_coordinates_packet = coordinates_packet(&lenght, dlt_lat, dlt_lon);
					HAL_UART_Transmit_IT(&huart3, p_coordinates_packet, lenght);
			}
		}
	}
}
