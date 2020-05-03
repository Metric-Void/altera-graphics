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

// Internal function. Can be used but no need.
void writePaletteHelper(uint8_t paletteId, uint8_t colorIndex, int rgbColor) {
	uint8_t paletteAddr = ((paletteId & 0x7) << 2) | (colorIndex & 0x3);
	palette_ptr[paletteAddr] = rgbColor & 0x00FFFFFF;
}

// Write an entire palette to the palette storage.
void writePalette(Palette toWrite) {
	writePaletteHelper(toWrite.paletteId, 0, toWrite.colors[0]);
	writePaletteHelper(toWrite.paletteId, 1, toWrite.colors[1]);
	writePaletteHelper(toWrite.paletteId, 2, toWrite.colors[2]);
	writePaletteHelper(toWrite.paletteId, 3, toWrite.colors[3]);
}

// " " - 00, "@" - 01. "#" - 10, "*" - 11.
void parseSpriteLine(Sprite *sprite, uint8_t linenum, char data[8]) {
	uint16_t result_line;
	for(uint8_t i=0; i<8; i++) {
		switch(data[i]) {
			case '*': {
				result_line = (result_line<<2) | 0b11;
				break;
			}
			case '#': {
				result_line = (result_line<<2) | 0b10;
				break;
			}
			case '@': {
				result_line = (result_line<<2) | 0b01;
				break;
			}
			default: {
				result_line = (result_line<<2) | 0b00;
			}
		}
	}
	sprite -> lines[linenum] = result_line;
}

// Parses all 128 characters in a custom sprite
void parseSprite(Sprite *target, char datas[16][8]) {
	for(int i=0; i<16; i++) {
		parseSpriteLine(target,i,datas[i]);
	}
}

void writeSprite(Sprite* target) {
	for(int i=0; i<8; i+=1) {
		int prepare_dualine = (((int)(target->lines[2*i]) & 0xFFFF)<< 16) | (((int)(target->lines[2*i+1]) & 0xFFFF));
		printf("%08x\n",prepare_dualine);
		spriteram_ptr[((target->sprite_id & 0x3F) << 3) | (i&0x7)] = prepare_dualine;
	}
}
