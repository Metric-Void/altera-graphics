//io_handler.c
#include "io_handler.h"
#include <stdio.h>

void IO_init(void)
{
	printf("Check A");
	*otg_hpi_reset = 1;
	printf("Check B");
	*otg_hpi_cs = 1;
	printf("Check C");
	*otg_hpi_r = 1;
	printf("Check D");
	*otg_hpi_w = 1;
	printf("Check E");
	*otg_hpi_address = 0;
	printf("Check F");
	*otg_hpi_data = 0;
	// Reset OTG chip
	*otg_hpi_cs = 0;
	printf("Check G");
	*otg_hpi_reset = 0;
	printf("Check H");
	*otg_hpi_reset = 1;
	printf("Check I");
	*otg_hpi_cs = 1;
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
	*otg_hpi_w = 1;
	*otg_hpi_cs = 1;
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
	*otg_hpi_r = 1;
	*otg_hpi_cs = 1;
	//printf("%x\n",temp);
	//printf("Read %x from HPI %d\n", temp, Address);
	return temp;
}
