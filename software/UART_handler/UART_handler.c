#include "UART_handler.h"

/*Private variables*/
bool start_byte = false, end_byte = false, gsb = false, geb = false;
uint8_t rgps_i = 0, rgcs_i = 0;
/*******************/

struct GPS_Packet
{
	float utcTime;
	char status;
	float latitude;
	char nsIndicator;
	float longitude;
	char ewIndicator;
	float speedOverGround;
	float courseOverGround;
	int32_t date;
	char mode;
	uint32_t CheckSum;
} in_gps_pck;

struct GGA_Packet
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
} in_gga_pck;

float delta = 0.0;
uint8_t temp = 0;
uint8_t msg_t = 0; // if 1 - RMC's message type or 2 is equivalent GGA
uint8_t buffer[75];
uint8_t raw_buff[] = {0, 0, 0, 0, 0, 0, 0, 0, 0x0A};

uint8_t calc_checksum(uint8_t *s);


void GPS_Analyze(uint8_t *Data_from_GPS) /* */
{
	temp = *(Data_from_GPS + 3);
	if(*(Data_from_GPS + 3) == 'R')
	{
      sscanf (Data_from_GPS, "$GPRMC,%f,%c,%f,%c,%f,%c,%f,%f,%d,,,%c*%X",
              &in_gps_pck.utcTime,
              &in_gps_pck.status,
              &in_gps_pck.latitude,
              &in_gps_pck.nsIndicator,
              &in_gps_pck.longitude,
              &in_gps_pck.ewIndicator,
              &in_gps_pck.speedOverGround,
              &in_gps_pck.courseOverGround,
              &in_gps_pck.date,
              &in_gps_pck.mode,
              &in_gps_pck.CheckSum
              );
      msg_t = 1;
	}
	else if((*(Data_from_GPS + 3) == 'G') && (*(Data_from_GPS + 4) == 'G'))
	{
		sscanf (Data_from_GPS, "$GPGGA,%f,%f,%c,%f,%c,%u,%u,%f,%f,%c,%f,%c,,*%X",
		    	&in_gga_pck.utcTime,
		      	&in_gga_pck.latitude,
				&in_gga_pck.nsIndicator,
				&in_gga_pck.longitude,
				&in_gga_pck.ewIndicator,
				&in_gga_pck.solutiontype,
				&in_gga_pck.satelitenum,
				&in_gga_pck.hdop,
				&in_gga_pck.hightsealevel,
				&in_gga_pck.hslindicator,
				&in_gga_pck.geoidheight,
				&in_gga_pck.geoindicator,
				&in_gga_pck.CheckSum
		        );
		msg_t = 2;
	}
}

uint8_t *coordinates_packet(uint8_t *size, uint8_t *data_frm_gcs)
{
	if(*(data_frm_gcs) == '!')
		delta = 0.0;
	else
	{
		delta = *(data_frm_gcs);
		delta *= 0.001;
	}
	if (msg_t == 1)
	{
	sprintf (buffer, "$GPRMC,%.3f,%c,%.4f,%c,%.4f,%c,%.2f,%.2f,%d,,,%c*",
 	   	   	  	  	  in_gps_pck.utcTime,
					  in_gps_pck.status,
					  in_gps_pck.latitude - delta,
					  in_gps_pck.nsIndicator,
					  in_gps_pck.longitude,
					  in_gps_pck.ewIndicator,
					  in_gps_pck.speedOverGround,
					  in_gps_pck.courseOverGround,
					  in_gps_pck.date,
					  in_gps_pck.mode
            		);
	in_gps_pck.CheckSum = calc_checksum(buffer);
	*size = sprintf (buffer, "$GPRMC,%.3f,%c,%.4f,%c,%.4f,%c,%.2f,%.2f,%d,,,%c*%X\n",
					in_gps_pck.utcTime,
					in_gps_pck.status,
					in_gps_pck.latitude - delta,
					in_gps_pck.nsIndicator,
					in_gps_pck.longitude,
					in_gps_pck.ewIndicator,
					in_gps_pck.speedOverGround,
					in_gps_pck.courseOverGround,
					in_gps_pck.date,
					in_gps_pck.mode,
					in_gps_pck.CheckSum
					);
	}
	else if(msg_t == 2)
	{
		sprintf(buffer, "$GPGGA,%.2f,%.4f,%c,%.4f,%c,%u,%u,%.1f,%.2f,%c,%.3f,%c,,*",
				in_gga_pck.utcTime,
				in_gga_pck.latitude - delta,
				in_gga_pck.nsIndicator,
				in_gga_pck.longitude,
				in_gga_pck.ewIndicator,
				in_gga_pck.solutiontype,
				in_gga_pck.satelitenum,
				in_gga_pck.hdop,
				in_gga_pck.hightsealevel,
				in_gga_pck.hslindicator,
				in_gga_pck.geoidheight,
				in_gga_pck.geoindicator
					);
			in_gps_pck.CheckSum = calc_checksum(buffer);
			*size = sprintf (buffer, "$GPGGA,%.2f,%.4f,%c,%.4f,%c,%u,%u,%.1f,%.2f,%c,%.3f,%c,,*%X\n",
					in_gga_pck.utcTime,
					in_gga_pck.latitude - delta,
					in_gga_pck.nsIndicator,
					in_gga_pck.longitude,
					in_gga_pck.ewIndicator,
					in_gga_pck.solutiontype,
					in_gga_pck.satelitenum,
					in_gga_pck.hdop,
					in_gga_pck.hightsealevel,
					in_gga_pck.hslindicator,
					in_gga_pck.geoidheight,
					in_gga_pck.geoindicator,
					in_gga_pck.CheckSum
					);

	}
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

bool start_stop(uint8_t *data_for_analyze)
{
	if (*(data_for_analyze + rgps_i) == '$') // $
	{
		start_byte = true;
		end_byte = false;
		++rgps_i;
	}
	else if ((start_byte) && (*(data_for_analyze + rgps_i) != '\n')) //*
	{
		++rgps_i;
	}
	else if ((start_byte) && (*(data_for_analyze + rgps_i) == '\n'))
	{
		start_byte = false;
		end_byte = true;
		rgps_i = 0;
	}
	return true;
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



