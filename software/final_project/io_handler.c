//io_handler.c
#include "io_handler.h"
#include <stdio.h>

void IO_init(void)
{
	*otg_hpi_reset = 0xFF;
	*otg_hpi_cs = 0xFF;
	*otg_hpi_r = 0xFF;
	*otg_hpi_w = 0xFF;
	*otg_hpi_address = 0;
	*otg_hpi_data = 0;
	// Reset OTG chip
	*otg_hpi_cs = 0;
	*otg_hpi_reset = 0;
	*otg_hpi_reset = 0xFF;
	*otg_hpi_cs = 0xFF;
}

void IO_write(alt_u8 Address, alt_u16 Data)
{
//*************************************************************************//
//									TASK								   //
//*************************************************************************//
//							Write this function							   //
//*************************************************************************//
	*otg_hpi_address = Address;
	*otg_hpi_data = Data;
	*otg_hpi_cs = 0;
	*otg_hpi_w = 0;
	*otg_hpi_w = 0xFF;
	*otg_hpi_cs = 0xFF;
	//printf("Wrote %x to HPI %d\n", Data, Address);
}

alt_u16 IO_read(alt_u8 Address)
{
	alt_u16 temp;
//*************************************************************************//
//									TASK								   //
//*************************************************************************//
//							Write this function							   //
//*************************************************************************//
	*otg_hpi_address = Address;
	*otg_hpi_cs = 0;
	*otg_hpi_r = 0;
	temp = *otg_hpi_data;
	*otg_hpi_r = 0xFF;
	*otg_hpi_cs = 0xFF;
	//printf("%x\n",temp);
	//printf("Read %x from HPI %d\n", temp, Address);
	return temp;
}
