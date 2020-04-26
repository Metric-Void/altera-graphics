/***********************************************
Tile Table
================================================
* Avalon-MM interface description.
The tile to write to/read from is indicated by 0xABCD.
0xAB = height index (0..30)
0xDE = width index (0..80)
Since 30 = 1E = 0001 1110, 13:0 will be enough.

Data is 11 bits. [7:0] indicates pattern index, [10:8] indicates palette.
***********************************************/
module tile_table (
	input logic CLK_100, CLK_50, RESET,
	// Avalon-MM interface.
	input		logic	[13:0]	AVL_ADDR,
	input		logic				AVL_READ, AVL_WRITE, AVL_CS,
	input		logic	[31:0]	AVL_WRITEDATA,
	input		logic	[3:0]		AVL_BYTE_EN,
	output	logic	[31:0]	AVL_READDATA,
	
	// Connection to sprite_ram.
	output		logic [7:0] get_index,		// Sprite index
	output		logic [3:0] get_line,		// Defines which lines to get
	input			logic [15:0] get_data,		// Resulting data.
	
	// Connection to color_palette.
	output logic [2:0] palette,
	output logic [1:0] color_index,
	input logic [23:0] rgb,
	
	// Connection to Color_Mapper.
	input logic [9:0] DrawX, DrawY,	// 10 bits allows 0..1023.
	output logic [7:0] VGA_RED, VGA_GREEN, VGA_BLUE
);

	// 255 total possible tiles. 0..255 (8 bits) at [7:0].
	// 8 total possible palettes, 0..7 (3 bits) at [10:8].
	logic [10:0] tiles [80][30];
	
	// tile_x = 0..80. Assign 0..127 (7 bits)
	logic [6:0] tile_x;
	// tile_y = 0..30. Assign 0..32 (5 bits)
	logic [4:0] tile_y;
	
	// Remaining x value should between 0..7. Assign 3 bits.
	logic [2:0] remain_x;
	
	// Remaining y value should between 0..15. Assign 4 bits.
	logic [3:0] remain_y;
	
	// Cache for color.
	logic [1:0] sprite_line_read[8];
	
	font_rom from(.addr(fr_addr), .data(data));
	
	// This block handles the Avalon-MM interface.
	always_ff @(posedge CLK_100) begin
		if(AVL_CS) begin
			if(AVL_WRITE) begin
				tiles[AVL_ADDR[31:16]][AVL_ADDR[15:0]][7:0] = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0] : tiles[AVL_ADDR[31:16]][AVL_ADDR[15:0]][7:0];
				tiles[AVL_ADDR[31:16]][AVL_ADDR[15:0]][10:8] = AVL_BYTE_EN[1]? AVL_WRITEDATA[10:8] : tiles[AVL_ADDR[31:16]][AVL_ADDR[15:0]][10:8];
			end else if (AVL_READ) begin
				AVL_READDATA = {{21{1'b0}}, tiles[AVL_ADDR[31:16]][AVL_ADDR[15:0]]};
			end
		end
	end
	
	// This part handles Pixel drawing.
	always_ff @(posedge CLK_100) begin
		// The code below is identical to
		// tile_x = DrawX / 8;
		// tile_y = DrawY / 16;
		// remain_x = DrawX mod 8;
		// remain_y = DrawY mod 16;
		tile_x = DrawX[9:3];
		remain_x = DrawX[2:0];
		tile_y = DrawY[8:4];		// Highest bit of DrawY should be 0.
		remain_y = DrawY[3:0];
		
		// Get the corresponding pattern from sprite_ram.
		get_index = tiles[tile_x][tile_y][7:0];	// The current pattern.
		get_line = remain_y;
		sprite_line_read = get_data;
		
		// Now get_data is filled with a line. Check remain_x. Get the color
		// Each pixel occupy 2 bits.
		palette = tiles[tile_x][tile_y][10:8];
		color_index = sprite_line_read[remain_x];
		
		VGA_RED = rgb[23:16];
		VGA_GREEN = rgb[15:8];
		VGA_BLUE = rgb[7:0];
	end
	
endmodule 
