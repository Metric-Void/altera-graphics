/*
 * tile_eng_driver.h
 *
 *  Created on: May 2, 2020
 *      Author: leezi
 */

#ifndef TILE_ENG_DRIVER_H_
#define TILE_ENG_DRIVER_H_

#include <stdint.h>

typedef struct _palette {
	uint8_t paletteId;
	uint32_t colors[4];
} Palette;

void drawTile(int x, int y, int paletteId, int spriteId);
void writePaletteHelper(uint8_t paletteId, uint8_t colorIndex, int rgbColor);
void writePalette(Palette toWrite);

#endif /* TILE_ENG_DRIVER_H_ */
