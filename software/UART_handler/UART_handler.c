#include "UART_handler.h"

/**************** Private variables **********************************/
bool start_byte = false, end_byte = false;
bool st_rdlt_byte = false, en_rdlt_byte = false;
float delta = 0.0;
uint8_t temp = 0, rgps_i, buff_numb = 0;
uint8_t msg_t = 0; // if 1 - RMC's message type or 2 is equivalent GGA
uint8_t buffer[85], buffer1[85], buffer2[85], dlt_gnd_buff[4];
uint8_t raw_buff[] = {0, 0, 0, 0, 0, 0, 0, 0, 0x0A};
uint8_t *p_current_buff;
int16_t dlt_mns_gnd = 0;
uint8_t rdlt_i = 0;

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
/*********************************************************************/

/**************** Private functions **********************************/
uint8_t calc_checksum(uint8_t *s);
uint8_t* switch_buff(uint8_t buff_nmb, uint8_t item_buff, uint8_t *data_to_buff, bool now_change);
/*********************************************************************/

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

uint8_t *coordinates_packet(uint8_t *size, int16_t data_frm_gcs)
{
	delta = data_frm_gcs * 0.001;

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

uint8_t *start_stop(uint8_t *data_for_analyze)
{
	if (*(data_for_analyze) == '$')
	{
		rgps_i = 0;
		start_byte = true;
		end_byte = false;
		switch_buff(buff_numb, rgps_i, data_for_analyze, end_byte);
		++rgps_i;
	}
	else if ((start_byte) && (*(data_for_analyze) != '\n'))
	{
		switch_buff(buff_numb, rgps_i, data_for_analyze, end_byte);
		++rgps_i;
	}
	else if ((start_byte) && (*(data_for_analyze ) == '\n'))
	{
		start_byte = false;
		end_byte = true;
		p_current_buff = switch_buff(buff_numb, rgps_i, data_for_analyze, end_byte);
		rgps_i = 0;
		buff_numb++;
	}
	if(buff_numb > 2)
		buff_numb = 0;

	return p_current_buff;
}

/*
 *
 */
uint8_t *start_stop_lite(uint8_t *dt_fr_analyze)
{
	if (*(dt_fr_analyze) == '>')
	{
		rdlt_i = 0;
		st_rdlt_byte = true;
		en_rdlt_byte = false;
	}
	else if ((st_rdlt_byte) && (*(dt_fr_analyze) != '\n'))
	{
		dlt_gnd_buff[rdlt_i] = *(dt_fr_analyze);
		++rdlt_i;
	}
	else if ((st_rdlt_byte) && (*(dt_fr_analyze ) == '\n'))
	{
		st_rdlt_byte = false;
		en_rdlt_byte = true;
		rdlt_i = 0;
	}
	if(en_rdlt_byte == true)
		return dlt_gnd_buff;
	else
		return 0;
}

uint8_t *switch_buff(uint8_t buff_nmb, uint8_t item_buff, uint8_t *data_to_buff, bool now_change)
{
	if (buff_nmb == 0)
	{
		buffer[item_buff] = *data_to_buff;
		if (now_change == true)
		{
			return buffer;
		}
	}
	else if (buff_nmb == 1)
	{
		buffer1[item_buff] = *data_to_buff;
		if (now_change == true)
		{
			return buffer1;
		}
	}
	else if (buff_nmb == 2)
	{
		buffer2[item_buff] = *data_to_buff;
		if (now_change == true)
		{
			return buffer2;
		}
	}
}

/*
 * example of input buffer: exb[] = {"-", "9","9","9"}
 * uint8_t *dltgnd - this is a pointer to the buffer
 * return int16_t  - this is a result of calculation incoming data
*/
int16_t delta_minus_gps(uint8_t *dltgnd)
{
	int8_t a = 0, b = 0, c = 0, d = 0;

	if(*(dltgnd) == '-')
	{
		a = (*(dltgnd + 1) - 48) * 100;
		b = (*(dltgnd + 2) - 48) * 10;
		c = *(dltgnd + 3) - 48;
		dlt_mns_gnd = (a + b + c) * (-1);
	}
	else
	{
		a = (*(dltgnd) - 48) * 1000;
		b = (*(dltgnd + 1) - 48) * 100;
		c = (*(dltgnd + 2) - 48) * 10;
		d = *(dltgnd + 3) - 48;
		dlt_mns_gnd = (a + b + c + d);
	}
	return dlt_mns_gnd;
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



