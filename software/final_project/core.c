/*
 * core.c
 *
 *  Created on: May 2, 2020
 *      Author: leezi
 */
#include "core.h"
#include "hardware.h"
#include "tile_eng_driver.h"
#include <stdint.h>

void game_tick() {
	palette_ptr[0b00000] = 0x117180;
	palette_ptr[0b00001] = 0x6EECFF;
	palette_ptr[0b00010] = 0x1BB4CC;
	palette_ptr[0b00011] = 0x21E3FF;

	Palette newPalette;
	newPalette.paletteId = 1;
	newPalette.colors[0] = 0x723E80;
	newPalette.colors[1] = 0xF3C7FF;
	newPalette.colors[2] = 0x796380;
	newPalette.colors[3] = 0xE57DFF;
	writePalette(newPalette);

	Palette newPalette2;
	newPalette2.paletteId = 2;
	newPalette2.colors[0] = 0x2F802E;
	newPalette2.colors[3] = 0x5EFF5B;
	writePalette(newPalette2);

	Palette newPalette3;
	newPalette3.paletteId = 3;
	newPalette3.colors[0] = 0x805E1B;
	newPalette3.colors[3] = 0xFFBC36;
	writePalette(newPalette3);

	Sprite wavey;
	wavey.sprite_id = 0;
	char wavey_mat[16][8] = {
			"        ",
			"        ",
			"        ",
			"@       ",
			" @     @",
			"  @   @ ",
			"   @ @  ",
			"    @   ",
			"        ",
			"    #   ",
			"   # #  ",
			"  #   # ",
			" #     #",
			"#       ",
			"        ",
			"        ",
	};

	parseSprite(&wavey, wavey_mat);
	for(int i=0;i<15;i++) {
		printf("%04x\n",wavey.lines[i]);
	}
	printf("\n\n");

	writeSprite(&wavey);
	printf("\n\n");

	for(int i=0;i<8;i++) {
		printf("%08x\n",spriteram_ptr[i]);
	}

	// spriteram_ptr[0] = 0xFFFFFFFF;
	// spriteram_ptr[1] = 0xAAAAAAAA;
	uint8_t counter = 0;
	for(int i=0; i<80; i++) {
		for(int j=0; j<30; j++) {
			drawTile(i,j,counter&0x03,128);
			counter = (counter+1) & 0x7F;
		}
	}
}
