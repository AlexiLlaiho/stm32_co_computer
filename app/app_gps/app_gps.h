/*!
 *******************************************************************************
 * @file    .h
 * @author  Gorbunov
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

#ifndef __APP_GPS_H
#define __APP_GPS_H

#include "main.h"
#include "cmsis_os.h"
#include "UART_handler.h"

/****** Public variables *******************************************************/
uint8_t rdgps[3];
/*******************************************************************************/

void StartDefaultTask(void const *argument);

#endif
