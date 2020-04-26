/*****************************************************
Color Palette.
A total of 8 palettes. Each palette can carry 4 colors.
======================================================
Address Definition:

Address contains 3 bits. [2:0] indicates the palette
Data contains 24 bits. [23:16] Red, [15:8] Green, [7:0] Blue.
******************************************************/

module color_palette(
	input logic CLK_50, CLK_100, RESET,
	input logic [2:0] AVL_ADDR,
	input logic [31:0] AVL_WRITEDATA,
	input logic [3:0] AVL_BYTE_EN,
	output logic [31:0] AVL_READDATA,
	input logic AVL_WRITE, AVL_READ, AVL_CS,
	
	// Exports.
	input logic [2:0] palette,
	input logic [1:0] color_index,
	output logic [23:0] rgb
);

// 8 palettes, each containing 4 colors, each color contains 24 bits.
logic [23:0] regfile[8][4];

always @(posedge CLK_100) begin
	if(AVL_CS) begin
		if(AVL_WRITE) begin
			regfile[AVL_ADDR][7:0] = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0] : regfile[AVL_ADDR][7:0];
			regfile[AVL_ADDR][15:8] = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0] : regfile[AVL_ADDR][15:8];
			regfile[AVL_ADDR][23:16] = AVL_BYTE_EN[0]? AVL_WRITEDATA[7:0] : regfile[AVL_ADDR][23:16];
		end else if (AVL_READ) begin
			AVL_READDATA = regfile[AVL_ADDR];
		end
	end
end

always_comb begin
	rgb = regfile[palette][color_index];
end

endmodule
