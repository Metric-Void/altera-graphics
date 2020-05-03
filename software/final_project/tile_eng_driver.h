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
	int paletteId;
	int colors[4];
} Palette;

typedef struct _sprite {
	uint16_t sprite_id;
	uint16_t lines[16];
} Sprite;

void drawTile(int x, int y, int paletteId, int spriteId);
void writePaletteHelper(uint8_t paletteId, uint8_t colorIndex, int rgbColor);
void writePalette(Palette* toWrite);
void parseSpriteLine(Sprite *sprite, uint8_t linenum, char data[8]);
void parseSprite(Sprite *target, char datas[16][8]);
void writeSprite(Sprite* target);
void writeText(int beginningX, int Y, char text[], int paletteId);
void clearScreen();

#endif /* TILE_ENG_DRIVER_H_ */
