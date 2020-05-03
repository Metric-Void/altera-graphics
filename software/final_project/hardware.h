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

#endif /* HARDWARE_H_ */
