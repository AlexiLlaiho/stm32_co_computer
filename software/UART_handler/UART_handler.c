#include "UART_handler.h"


char status, nsIndicator, ewIndicator, mode;
double courseOverGround_2;
float utcTime, latitude, longitude, speedOverGround, courseOverGround;
int32_t date;
uint32_t CheckSum;
uint8_t buffer[50];


void GPRMS_Analyze(uint8_t *Data_from_GPS) /* */
{
      sscanf (Data_from_GPS, "$GPRMC,%f,%c,%f,%c,%f,%c,%f,%f,%d,,,%c*%X",
              &utcTime,
              &status,
              &latitude,
              &nsIndicator,
              &longitude,
              &ewIndicator,
              &speedOverGround,
              &courseOverGround,
              &date,
              &mode,
              &CheckSum
              );
}

uint8_t *coordinates_packet()
{
	uint8_t n=sprintf(buffer, "%f,%f,%f,%f\n", latitude, longitude, speedOverGround, courseOverGround);

	return buffer;
}

