/*!
 *******************************************************************************
 * @file    .c
 * @author  Gorbunov
 * @version V0.0.1
 * @date    25.06.2021
 * @brief
 * @details
 *******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT </center></h2>
 *******************************************************************************
 */
#include "app_pulse.h"

void StartLedTask(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
	for (;;)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
		osDelay(250);
	}
}
