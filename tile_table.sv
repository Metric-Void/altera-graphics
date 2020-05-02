/***********************************************
Tile Table
================================================
* Avalon-MM interface description.
The tile to write to/read from is indicated by 0xABCD.
0xAB = height index (0..30)
0xDE = width index (0..80)
Since 30 = 1E = 0001 1110, 12:0 will be enough. (13 bits)

Data is 11 bits. [7:0] indicates pattern index, [10:8] indicates palette.

DrawX is horizontal (Width), DrawY is vertical(height).
***********************************************/
module tile_table (
	input logic CLK_100, CLK_50, RESET,
	// Avalon-MM interface.
	input		logic	[12:0]	AVL_ADDR,
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
	// logic [10:0] tiles [2560];
	logic [11:0] addr_change;
	assign addr_change = {AVL_ADDR[6:0], AVL_ADDR[12:8]};
	
	// tile_x = 0..80. Assign 0..127 (7 bits)
	logic [6:0] tile_x;
	// tile_y = 0..30. Assign 0..32 (5 bits)
	logic [4:0] tile_y;
	
	logic [11:0] get_addr_change;
	assign get_addr_change = {tile_x, tile_y};
	
	// Remaining x value should between 0..7. Assign 3 bits.
	logic [2:0] remain_x;
	
	// Remaining y value should between 0..15. Assign 4 bits.
	logic [3:0] remain_y;
	
	// The code below is identical to
	// tile_x = DrawX / 8;
	// tile_y = DrawY / 16;
	// remain_x = DrawX mod 8;
	// remain_y = DrawY mod 16;
	assign tile_x = DrawX[9:3];
	assign remain_x = DrawX[2:0];
	assign tile_y = DrawY[8:4];		// Highest bit of DrawY should be 0.
	assign remain_y = DrawY[3:0];
	
	assign get_line = remain_y;
	
	// Cache for color.
	logic [1:0] sprite_line_read[8];
	
	tile_table_dpram ttram0(
		.clk(CLK_100),
		.addr1(addr_change),
		.be1(AVL_BYTE_EN[1:0]),
		.data_in1(AVL_WRITEDATA),
		.data_out1(AVL_READDATA),
		.we1(AVL_WRITE),
		
		.addr2(get_addr_change),
		.be2(2'b11),
		.data_out2({palette, get_index}),
		.we2(1'b0)
	);
	
	// This block handles the Avalon-MM interface.
	always_comb begin
		// Get the corresponding pattern from sprite_ram.
		// get_index = tiles[get_addr_change][7:0];	// The current pattern.
		
		sprite_line_read[0] = get_data[1:0];
		sprite_line_read[1] = get_data[3:2];
		sprite_line_read[2] = get_data[5:4];
		sprite_line_read[3] = get_data[7:6];
		sprite_line_read[4] = get_data[9:8];
		sprite_line_read[5] = get_data[11:10];
		sprite_line_read[6] = get_data[13:12];
		sprite_line_read[7] = get_data[15:14];
		
		// Now get_data is filled with a line. Check remain_x. Get the color
		// Each pixel occupy 2 bits.
		// palette = tiles[get_addr_change][10:8];
		color_index = sprite_line_read[7-remain_x];
		
		VGA_RED = rgb[23:16];
		VGA_GREEN = rgb[15:8];
		VGA_BLUE = rgb[7:0];
	end
	
endmodule 