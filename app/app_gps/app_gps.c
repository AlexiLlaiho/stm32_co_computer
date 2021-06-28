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

void StartDefaultTask(void const *argument)
{
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	extern QueueHandle_t xRGPSQueue;
	extern uint8_t buffer[];
	extern uint8_t rgcs_data[];
	extern UART_HandleTypeDef huart3;
	uint8_t qgps = 0;

	for (;;)
	{
		if (xRGPSQueue != NULL)
		{
			if( xQueueReceive( xRGPSQueue, &(qgps), (TickType_t ) 10 ) == pdPASS )
			{
				if(start_stop(&qgps) == true)
				{
//					GPS_Analyze(buffer);
//					uint8_t lenght = 0;
//					uint8_t *p_coordinates_packet = coordinates_packet(&lenght, rgcs_data);
//					HAL_UART_Transmit_IT(&huart3, p_coordinates_packet, lenght);
				}
			}
		}
//		osDelay(1);
	}
}
