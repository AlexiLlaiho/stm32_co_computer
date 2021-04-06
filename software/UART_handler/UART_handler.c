#include "UART_handler.h"


char status, nsIndicator, ewIndicator, mode;
double courseOverGround_2;
float utcTime, latitude, longitude, speedOverGround, courseOverGround;
int32_t date;
uint32_t CheckSum;


void GPRMS_Analyze(uint8_t *Data_from_GPS) /* */
{
  char *str;
//  str = strstr((char *)Data_from_GPS, "$GPRMC");

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

