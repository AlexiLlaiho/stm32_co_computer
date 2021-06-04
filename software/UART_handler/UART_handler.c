#include "UART_handler.h"

struct GPS_Packet
{
	float utcTime;
	float latitude;
	char nsIndicator;
	float longitude;
	char ewIndicator;
	uint8_t solutiontype;
	uint8_t satelitenum;
	float hdop;
	float hightsealevel;
	char hslindicator;
	float geoidheight;
	char geoindicator;
	uint32_t CheckSum;
} in_gps_pck;

uint8_t buffer[75];
uint8_t raw_buff[] = {0, 0, 0, 0, 0, 0, 0, 0, 0x0A};

uint8_t calc_checksum(uint8_t *s);


void GPRMS_Analyze(uint8_t *Data_from_GPS) /* */
{
      sscanf (Data_from_GPS, "$GPGGA,%f,%f,%c,%f,%c,%u,%u,%f,%f,%c,%f,%c,,*%X",
              &in_gps_pck.utcTime,
              &in_gps_pck.latitude,
              &in_gps_pck.nsIndicator,
              &in_gps_pck.longitude,
              &in_gps_pck.ewIndicator,
              &in_gps_pck.solutiontype,
              &in_gps_pck.satelitenum,
              &in_gps_pck.hdop,
              &in_gps_pck.hightsealevel,
              &in_gps_pck.hslindicator,
              &in_gps_pck.geoidheight,
			  &in_gps_pck.geoindicator,
			  &in_gps_pck.CheckSum
              );
}

uint8_t *coordinates_packet(uint8_t *size, uint8_t *data_frm_gcs)
{
	float delta = 0.0;
	if(*(data_frm_gcs) == '!')
		delta = 0.0;
	else
	{
		delta = *(data_frm_gcs);
		delta *= 0.001;
	}
	sprintf(buffer, "$GPGGA,%.2f,%.4f,%c,%.4f,%c,%u,%u,%.1f,%.2f,%c,%.3f,%c,,*",
			in_gps_pck.utcTime,
			in_gps_pck.latitude - delta,
			in_gps_pck.nsIndicator,
			in_gps_pck.longitude,
			in_gps_pck.ewIndicator,
			in_gps_pck.solutiontype,
			in_gps_pck.satelitenum,
			in_gps_pck.hdop,
			in_gps_pck.hightsealevel,
			in_gps_pck.hslindicator,
			in_gps_pck.geoidheight,
			in_gps_pck.geoindicator
			);
	in_gps_pck.CheckSum = calc_checksum(buffer);
	*size = sprintf (buffer, "$GPGGA,%.2f,%.4f,%c,%.4f,%c,%u,%u,%.1f,%.2f,%c,%.3f,%c,,*%X",
			in_gps_pck.utcTime,
			in_gps_pck.latitude - delta,
			in_gps_pck.nsIndicator,
			in_gps_pck.longitude,
			in_gps_pck.ewIndicator,
			in_gps_pck.solutiontype,
			in_gps_pck.satelitenum,
			in_gps_pck.hdop,
			in_gps_pck.hightsealevel,
			in_gps_pck.hslindicator,
			in_gps_pck.geoidheight,
			in_gps_pck.geoindicator,
			in_gps_pck.CheckSum
			);
	return buffer;
}

uint8_t* raw_data_packet(uint8_t *size)
{
	double fractpart = 0, intpart = 0;
	uint32_t tdata = 0;

	fractpart = modf((double) in_gps_pck.latitude, &intpart);
	fractpart *= 100000;
	tdata = (uint32_t) fractpart;
	raw_buff[3] = (uint8_t) tdata;
	raw_buff[2] = (uint8_t) (tdata >> 8);
	raw_buff[1] = (uint8_t) (tdata >> 16);
	raw_buff[0] = (uint8_t) (tdata >> 24);

	fractpart = modf((double) in_gps_pck.longitude, &intpart);
	fractpart *= 100000;
	tdata = (uint32_t) fractpart;
	raw_buff[7] = (uint8_t) tdata;
	raw_buff[6] = (uint8_t) (tdata >> 8);
	raw_buff[5] = (uint8_t) (tdata >> 16);
	raw_buff[4] = (uint8_t) (tdata >> 24);

	*size = sizeof(raw_buff) / sizeof(raw_buff[0]);

	return raw_buff;
}

uint8_t calc_checksum(uint8_t *s)
{
	uint8_t result;
	result = 0;
	s++; // Skip dollar sign
	while ((*s != '*') && (*s != '\0'))
		result ^= *s++;
	return result;
}



