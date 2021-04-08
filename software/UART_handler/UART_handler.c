#include "UART_handler.h"


char status, nsIndicator, ewIndicator, mode;
double courseOverGround_2;
float utcTime, latitude, longitude, speedOverGround, courseOverGround;
int32_t date;
uint32_t CheckSum;
uint8_t buffer[50];
uint8_t raw_buff[] = {0, 0, 0, 0, 0, 0, 0, 0, 0x0A};


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
	uint8_t n=sprintf(buffer, "%.4f,%.4f,%.2f,%.1f\n", latitude, longitude, speedOverGround, courseOverGround);

	return buffer;
}

uint8_t* raw_data_packet(uint8_t *size)
{
	double fractpart = 0, intpart = 0;
	uint32_t tdata = 0;

	fractpart = modf((double) latitude, &intpart);
	fractpart *= 100000;
	tdata = (uint32_t) fractpart;
	raw_buff[3] = (uint8_t) tdata;
	raw_buff[2] = (uint8_t) (tdata >> 8);
	raw_buff[1] = (uint8_t) (tdata >> 16);
	raw_buff[0] = (uint8_t) (tdata >> 24);

	fractpart = modf((double) longitude, &intpart);
	fractpart *= 100000;
	tdata = (uint32_t) fractpart;
	raw_buff[7] = (uint8_t) tdata;
	raw_buff[6] = (uint8_t) (tdata >> 8);
	raw_buff[5] = (uint8_t) (tdata >> 16);
	raw_buff[4] = (uint8_t) (tdata >> 24);

	*size = sizeof(raw_buff) / sizeof(raw_buff[0]);

	return raw_buff;
}



