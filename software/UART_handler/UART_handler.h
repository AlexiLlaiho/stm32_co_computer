/*!
 *******************************************************************************
 * @file    .h
 * @author
 * @version V1.0.0.0
 * @date    20.01.2018
 * @brief
 * @details
 *******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT </center></h2>
 *******************************************************************************
 */

#ifndef __UART_HANDLER_H
#define __UART_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/****** Public functions *******************************************************/
uint8_t *start_stop(uint8_t *data_for_analyze);
uint8_t *start_stop_lite(uint8_t *dt_fr_analyze);
void GPS_Analyze(uint8_t *Data_from_GPS);
uint8_t *coordinates_packet(uint8_t *size, int16_t data_frm_gcs, int16_t data2_frm_gcs);
uint8_t *raw_data_packet(uint8_t *size);
int16_t delta_minus_gps(uint8_t *dltgnd);
/*******************************************************************************/

#endif
