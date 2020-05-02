/*************************************************
Sprite RAM.
==================================================
Description:
This module is an expander for font_rom. It allows 256 sprites for the system.

First 128 sprites come from font_rom. They are not modifiable, but can be read.
0 and 1 will be converted to 00 and 11. 

Other 128 can be read and written, and can be given colors. Each sprite can have 4 colors defined by the palette.

Each sprite is 8 pixel in width and 16 pixel in height.
When being written, each sprite is split into 4 chunks of 32 bits.

Definition of AVL_ADDR:
Bits [7:0] represents 0-255. Represents sprite number 0-255.
Bits [10:8] represents four parts of the sprite. 000 -> first 2 rows, 111 -> last 2 rows, etc.

Illustration:
         // code x14
[00]        8'b0000000000000000, // 0           (LSB) (Internal 0:15)
[00]        8'b0000000000000000, // 1
[00]        8'b0011111111111111, // 2  *******
[00]        8'b1111001111001111, // 3 ** ** **  (MSB)
[01]        8'b1111001111001111, // 4 ** ** **
[01]        8'b1111001111001111, // 5 ** ** **
[01]        8'b0011111111001111, // 6  **** **
[01]        8'b0000001111001111, // 7    ** **
[10]        8'b0000001111001111, // 8    ** **
[10]        8'b0000001111001111, // 9    ** **
[10]        8'b0000001111001111, // a    ** **
[10]        8'b0000001111001111, // b    ** **
[11]        8'b0000000000000000, // c
[11]        8'b0000000000000000, // d
[11]        8'b0000000000000000, // e
[11]        8'b0000000000000000, // f                  (Internal 241:256)

-------------------------------------------------

Author: Zixi Li (zixili2@illinois.edu)
*************************************************/
module sprite_ram(
	input		logic CLK_50, CLK_100, RESET,
	
	// AVL Interface.
	input		logic AVL_READ, AVL_WRITE, AVL_CS,
	input 	logic [3:0] AVL_BYTE_EN,
	input 	logic [8:0] AVL_ADDR,
	input 	logic [31:0] AVL_WRITEDATA,
	output	logic [31:0] AVL_READDATA,
	
	// Export data.
	input		logic [7:0] get_index,		// Sprite index
	input		logic [3:0] get_line,		// Defines which lines to get
	output	logic [15:0] get_data		// Resulting data.
);
	
	wire [31:0] get_data_p;
	
	logic [7:0] raw_rom;
	logic [15:0] rom_expand;
	
	// logic [10:0] from_addr = {get_index[6:0], get_line};
	
	// Font rom and its bit expansion.
	font_rom from0(
		.addr({get_index[6:0], get_line}),
		.data(raw_rom)
	);
	
	always_comb begin
		for(int i=0; i<=7;i++) begin
			rom_expand[i*2] = raw_rom[i];
			rom_expand[i*2+1] = raw_rom[i];
		end
	end
	
	// Interface with dpram.
	logic [8:0] hw_addr_transform;
	logic [32:0] hw_dataraw;
	logic [3:0] hw_be;
	
	always_comb begin
		if(get_line[0] == 0) begin
			// Getting the front part of 32 bits.
			hw_be = 4'b1100;
			hw_addr_transform = {get_index[5:0], get_line[3:1]};
			get_data = get_index[7]? hw_dataraw[31:16]: rom_expand;
		end else begin
			hw_be = 4'b0011;
			hw_addr_transform = {get_index[5:0], get_line[3:1]};
			get_data = get_index[7]? hw_dataraw[15:0]: rom_expand;
		end
	end
	
	sprite_dpram regfile (
		.clk(CLK_100),
		
		// First port is dedicated to AVL interface.
		.addr1(AVL_ADDR),
		.be1(AVL_BYTE_EN),
		.data_in1(AVL_WRITEDATA),
		.data_out1(AVL_READDATA),
		.we1(AVL_WRITE),
		
		// Second dedicated to hardware part reading.
		.addr2(hw_addr_transform),
		.be2(hw_be),
		.data_out2(hw_dataraw),
		.we2(1'b0)
	);
endmodule
