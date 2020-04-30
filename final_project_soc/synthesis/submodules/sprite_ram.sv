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
	input 	logic [10:0] AVL_ADDR,
	input 	logic [31:0] AVL_WRITEDATA,
	output	logic [31:0] AVL_READDATA,
	
	// Export data.
	input		logic [7:0] get_index,		// Sprite index
	input		logic [3:0] get_line,		// Defines which lines to get
	output	logic [15:0] get_data		// Resulting data.
);
	
	// Internal register for extra sprites.
	logic [255:0] regfile[128];
	
	// Interfacing part.
	// These code implements the Avalon-MM interfae. It allows extra sprites to be written here.
	// Font rom cannot be written to or read from.
	wire [7:0] ADDR_LOW;
	wire [6:0] regaddr;
	assign ADDR_LOW = AVL_ADDR[7:0];		// 0..255. Address without sprite line.
	assign regaddr = AVL_ADDR[6:0];	// ADDR_LOW-128. Get the RAM address.
	
	wire [2:0] sprite_line;
	assign sprite_line = AVL_ADDR[10:8];

	always @(posedge CLK_100) begin
		if(AVL_CS) begin
			if(AVL_WRITE == 1'b1) begin
				if(ADDR_LOW > 127) begin
				   // Packed array index must be constant.
					// Who made this rule should get C++ Primer to hit his head. 
					// [(7 + {sprite_line, 2'bXX} << 3):({sprite_line, 2'bXX} << 3)] is the general formula here.
					case(sprite_line)  
						 3'b000: begin
							  regfile[regaddr][7:0]    = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][7:0];
							  regfile[regaddr][15:8]   = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][15:8];
							  regfile[regaddr][23:16]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][23:16];
							  regfile[regaddr][31:24]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][31:24];
						 end
						 3'b001: begin
							  regfile[regaddr][39:32]  = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][39:32];
							  regfile[regaddr][47:40]  = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][47:40];
							  regfile[regaddr][55:48]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][55:48];
							  regfile[regaddr][63:56]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][63:56];
						 end
						 3'b010: begin
							  regfile[regaddr][71:64]  = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][71:64];
							  regfile[regaddr][79:72]  = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][79:72];
							  regfile[regaddr][87:80]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][87:80];
							  regfile[regaddr][95:88]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][95:88];
						 end
						 3'b011: begin
							  regfile[regaddr][103:96]   = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][103:96] ;
							  regfile[regaddr][111:104]  = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][111:104];
							  regfile[regaddr][119:112]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][119:112];
							  regfile[regaddr][127:120]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][127:120];
						 end
						 3'b100: begin
							  regfile[regaddr][135:128]  = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][135:128];
							  regfile[regaddr][143:136]  = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][143:136];
							  regfile[regaddr][151:144]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][151:144];
							  regfile[regaddr][159:152]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][159:152];
						 end
						 3'b101: begin
							  regfile[regaddr][167:160]  = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][167:160];
							  regfile[regaddr][175:168]  = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][175:168];
							  regfile[regaddr][183:176]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][183:176];
							  regfile[regaddr][191:184]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][191:184];
						 end
						 3'b110: begin
							  regfile[regaddr][199:192]  = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][199:192];
							  regfile[regaddr][207:200]  = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][207:200];
							  regfile[regaddr][215:208]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][215:208];
							  regfile[regaddr][223:216]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][223:216];
						 end
						 3'b111: begin
							  regfile[regaddr][231:224]  = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0]   : regfile[regaddr][231:224];
							  regfile[regaddr][239:232]  = AVL_BYTE_EN[1]? AVL_WRITEDATA[15:8]  : regfile[regaddr][239:232];
							  regfile[regaddr][247:240]  = AVL_BYTE_EN[2]? AVL_WRITEDATA[23:16] : regfile[regaddr][247:240];
							  regfile[regaddr][255:248]  = AVL_BYTE_EN[3]? AVL_WRITEDATA[31:24] : regfile[regaddr][255:248];
						 end
					endcase
				end // Otherwise do nothing. Other addresses cannot be written.
			end else if(AVL_READ == 1'b1) begin
				if(regaddr>127) begin
					case(sprite_line)
						3'b000: AVL_READDATA = regfile[regaddr][31:0];
						3'b001: AVL_READDATA = regfile[regaddr][63:32];
						3'b010: AVL_READDATA = regfile[regaddr][95:64];
						3'b011: AVL_READDATA = regfile[regaddr][127:96];
						3'b100: AVL_READDATA = regfile[regaddr][159:128];
						3'b101: AVL_READDATA = regfile[regaddr][191:160];
						3'b110: AVL_READDATA = regfile[regaddr][223:192];
						3'b111: AVL_READDATA = regfile[regaddr][255:224];
					endcase
				end else begin // Disallow reading from Font ROM.
					AVL_READDATA = 32'b0;
				end
			end
		end
	end
	
	wire [31:0] get_data_p;
	
	logic [7:0] raw_rom;
	wire [15:0] rom_expand;
	
	// logic [10:0] from_addr = {get_index[6:0], get_line};
	
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
	
	always_comb begin
		// Being lazy here. Copy and re-switch
		if(get_index[7]) begin		// High Index. Sprite RAM.
			case(get_line[2:0])
				3'b000: get_data_p = regfile[get_index[6:0]][31:0];
				3'b001: get_data_p = regfile[get_index[6:0]][63:32];
				3'b010: get_data_p = regfile[get_index[6:0]][95:64];
				3'b011: get_data_p = regfile[get_index[6:0]][127:96];
				3'b100: get_data_p = regfile[get_index[6:0]][159:128];
				3'b101: get_data_p = regfile[get_index[6:0]][191:160];
				3'b110: get_data_p = regfile[get_index[6:0]][223:192];
				3'b111: get_data_p = regfile[get_index[6:0]][255:224];
			endcase
			if(get_line[3]) begin
				// High. Rear bits
				get_data = get_data_p[31:16];
			end else begin
				// Low. Front bits.
				get_data = get_data_p[15:0];
			end
		end else begin			// Low index. Font ROM.
			get_data_p = 32'bX;
			get_data = rom_expand;
		end
	end
endmodule
