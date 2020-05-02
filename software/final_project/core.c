/*
 * core.c
 *
 *  Created on: May 2, 2020
 *      Author: leezi
 */
#include "core.h"
#include "hardware.h"

void game_tick() {
	palette_ptr[0b00000] = 0x000000;	// Pure black
	palette_ptr[0b00001] = 0xFFFFFF;	// Pure white
	palette_ptr[0b00010] = 0xFFFFFF;	// Pure white
	palette_ptr[0b00011] = 0xFFFFFF;	// Pure white

	tiletable_ptr[0x00100010] = 0b00000000001;
}
