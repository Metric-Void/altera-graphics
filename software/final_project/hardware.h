/*
 * hardware.h
 *
 *  Created on: Apr 30, 2020
 *      Author: leezi
 */

#include"system.h"
#ifndef HARDWARE_H_
#define HARDWARE_H_

// Graphics
#define palette_ptr 		((volatile int*)	COLOR_PALETTE_0_BASE)
#define spriteram_ptr		((volatile int*) 	SPRITE_RAM_0_BASE)
#define tiletable_ptr		((volatile int*)	TILE_TABLE_0_BASE)

// OTG Interface.
#define otg_hpi_address		((volatile int*) 	OTG_HPI_ADDRESS_BASE)
#define otg_hpi_data		((volatile int*)	OTG_HPI_DATA_BASE)
#define otg_hpi_r			((volatile char*)	OTG_HPI_R_BASE)
#define otg_hpi_cs			((volatile char*)	OTG_HPI_CS_BASE) //FOR SOME REASON CS BASE BEHAVES WEIRDLY MIGHT HAVE TO SET MANUALLY
#define otg_hpi_w			((volatile char*)	OTG_HPI_W_BASE)
#define otg_hpi_reset		((volatile char*)	OTG_HPI_RESET_BASE)

#endif /* HARDWARE_H_ */
