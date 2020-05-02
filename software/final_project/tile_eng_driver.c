/*
 * tile_eng_driver.c
 *
 *  Created on: May 2, 2020
 *      Author: leezi
 */

#include "hardware.h"
#include "tile_eng_driver.h"
#include <stdint.h>

void drawTile(int x, int y, int paletteId, int spriteId) {
	uint16_t tt_addr = ((y & 0xFF)<<8) | (x & 0xFF);
	uint16_t tt_data = ((paletteId & 0x7)<<8) | (spriteId & 0xFF);
	tiletable_ptr[tt_addr] = tt_data;
}

void writePaletteHelper(uint8_t paletteId, uint8_t colorIndex, int rgbColor) {
	uint8_t paletteAddr = ((paletteId & 0x7) << 2) | (colorIndex & 0x3);
	palette_ptr[paletteAddr] = rgbColor & 0x00FFFFFF;
}

void writePalette(Palette toWrite) {
	writePaletteHelper(toWrite.paletteId, 0, toWrite.colors[0]);
	writePaletteHelper(toWrite.paletteId, 1, toWrite.colors[1]);
	writePaletteHelper(toWrite.paletteId, 2, toWrite.colors[2]);
	writePaletteHelper(toWrite.paletteId, 3, toWrite.colors[3]);
}
