`timescale 1ns/1ps

module sim_tile_table();

logic CLK_100=0, CLK_50=0, RESET;

always begin
	#5 CLK_100 = ~CLK_100;
end

always begin
	#10 CLK_50 = ~CLK_50;
end

// Avalon-MM interface.
logic	[12:0]	AVL_ADDR;
logic				AVL_READ, AVL_WRITE, AVL_CS;
logic	[31:0]	AVL_WRITEDATA;
logic	[3:0]		AVL_BYTE_EN;
logic	[31:0]	AVL_READDATA;

// Connection to sprite_ram.
logic [7:0] get_index;		// Sprite index
logic [3:0] get_line;		// Defines which lines to get
logic [15:0] get_data;		// Resulting data.

// Connection to color_palette.
logic [2:0] palette;
logic [1:0] color_index;
logic [23:0] rgb;

// Connection to Color_Mapper.
logic [9:0] DrawX, DrawY;	// 10 bits allows 0..1023.
logic [7:0] VGA_RED, VGA_GREEN, VGA_BLUE;

tile_table testee_table(
	.palette(palette),	// Connected to palette
	.color_index(color_index),
	.rgb(rgb),
	.*
);

/************** Color Palette ***********************/
logic [4:0] AVL_ADDR_palette;
logic [31:0] AVL_WRITEDATA_palette;
logic [3:0] AVL_BYTE_palette_EN;
logic [31:0] AVL_READDATA_palette;
logic AVL_WRITE_palette, AVL_READ_palette, AVL_CS_palette;

// Uses alternative names for AVL-MM interface.
color_palette testee_palette(
	.CLK_50, .CLK_100,
	.AVL_ADDR(AVL_ADDR_palette),
   .AVL_WRITEDATA(AVL_WRITEDATA_palette),
   .AVL_BYTE_EN(AVL_BYTE_palette_EN),
   .AVL_READDATA(AVL_READDATA_palette),
   .AVL_WRITE(AVL_WRITE_palette),
   .AVL_READ(AVL_READ_palette),
   .AVL_CS(AVL_CS_palette),
	.palette(palette),			// Connected to testee_table.
	.color_index(color_index),	// Connected to testee_table.
	.rgb(rgb)					// Connected to testee_table.
);

/**************** Sprite RAM ***********************/
logic AVL_READ_sprite, AVL_WRITE_sprite, AVL_CS_sprite;
logic [3:0] AVL_BYTE_EN_sprite;
logic [8:0] AVL_ADDR_sprite;
logic [31:0] AVL_WRITEDATA_sprite;
logic [31:0] AVL_READDATA_sprite;

sprite_ram testee_spram(
	.CLK_50, .CLK_100,
	.get_index(get_index),		// Sprite index
	.get_line(get_line),		// Defines which lines to get
	.get_data(get_data),		// Resulting data.
	.AVL_READ(AVL_READ_sprite),
	.AVL_WRITE(AVL_WRITE_sprite),
	.AVL_ADDR(AVL_ADDR_sprite),
	.AVL_CS(AVL_CS_sprite),
	.AVL_READDATA(AVL_READDATA_sprite),
	.AVL_WRITEDATA(AVL_WRITEDATA_sprite),
	.AVL_BYTE_EN(AVL_BYTE_EN_sprite)
);

/**************** Test Proc ***********************/
initial begin
	// Write something to palette #0
	AVL_CS_palette= 1'b1;
	AVL_BYTE_palette_EN = 4'hF;

	#10	// Color 0 = black.
	AVL_ADDR_palette = 5'b00000;
	AVL_WRITEDATA_palette = 32'h00000000;	// Black
	#10 AVL_WRITE_palette = 1;
	#10 AVL_WRITE_palette = 0;

	#10	// Color 01 = Red.
	AVL_ADDR_palette = 5'b00001;
	AVL_WRITEDATA_palette = 32'h00FF0000; // Red
	#10 AVL_WRITE_palette = 1;
	#10 AVL_WRITE_palette = 0;

	#10	// Color 10 = Green
	AVL_ADDR_palette = 5'b00010;
	AVL_WRITEDATA_palette = 32'h0000FF00; // Green
	#10 AVL_WRITE_palette = 1;
	#10 AVL_WRITE_palette = 0;

	#10	// Color 11 = Black
	AVL_ADDR_palette = 5'b0011;
	AVL_WRITEDATA_palette = 32'h00FFFFFF; // Black
	#10 AVL_WRITE_palette = 1;
	#10 AVL_WRITE_palette = 0;

	// Font RAM test.
	// Write something to 100000000 sprite ram.
	AVL_ADDR_sprite = 9'h00;
	AVL_WRITEDATA_sprite = 32'hCFFCDFFD;
	AVL_CS_sprite = 1'b1;
	#10 AVL_WRITE_sprite = 1'b1;
		 AVL_BYTE_EN_sprite = 4'hF;
	#20 AVL_WRITE_sprite = 1'b0;
		 AVL_CS_sprite = 1'b0;
	
	// Tile table test.
	AVL_CS = 1'b1;
	AVL_ADDR = 13'h0000;	// (0,0).
	AVL_WRITEDATA = 1'b00000000001;	//Palette 0, Pattern 1.
	AVL_BYTE_EN = 4'hF;
	#10 AVL_WRITE = 1;
	#20 AVL_WRITE = 0;
	
	AVL_ADDR = 13'h0101;	// (1,1).
	AVL_WRITEDATA = 1'b00000000001;	//Palette 0, Pattern 1.
	AVL_BYTE_EN = 4'hF;
	#10 AVL_WRITE = 1;
	#20 AVL_WRITE = 0;
	
	AVL_ADDR = 13'h010F;	// (1,15).
	AVL_WRITEDATA = 1'b00000000001;	//Palette 0, Pattern 1.
	AVL_BYTE_EN = 4'hF;
	#10 AVL_WRITE = 1;
	#20 AVL_WRITE = 0;
	
	#50 // Try to retrieve a pixel.
	DrawX = 4;
	DrawY = 7;
	
	#50
	DrawX = 12;
	DrawY = 20;
	#10 DrawX = 13;
	#10 DrawX = 14;
	
	#50
	DrawX = 124;
	DrawY = 20;
	#10 DrawX = 125;
	#10 DrawX = 126;
end

endmodule