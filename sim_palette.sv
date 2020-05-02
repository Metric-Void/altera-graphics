
`timescale 1ns/1ps

module sim_palette();

logic CLK_50, CLK_100, RESET;
logic [4:0] AVL_ADDR;
logic [31:0] AVL_WRITEDATA;
logic [3:0] AVL_BYTE_EN;
logic [31:0] AVL_READDATA;
logic AVL_WRITE, AVL_READ, AVL_CS;

logic [2:0] palette;
logic [1:0] color_index;
logic [23:0] rgb;

color_palette testee(
	.*
);

always begin
	#5 CLK_100 = ~CLK_100;
end

always begin
	#10 CLK_50 = ~CLK_50;
end

initial begin
	CLK_50 = 0;
	CLK_100 = 0;
end

initial begin
	// Write something to palette #0
	AVL_CS= 1'b1;
	AVL_BYTE_EN = 4'hF;

	#10	// Color 0 = black.
	AVL_ADDR = 5'b00000;
	AVL_WRITEDATA = 32'h00000000;	// Black
	#10 AVL_WRITE = 1;
	#10 AVL_WRITE = 0;

	#10	// Color 01 = Red.
	AVL_ADDR = 5'b00001;
	AVL_WRITEDATA = 32'h00FF0000; // Red
	#10 AVL_WRITE = 1;
	#10 AVL_WRITE = 0;

	#10	// Color 10 = Green
	AVL_ADDR = 5'b00010;
	AVL_WRITEDATA = 32'h0000FF00; // Green
	#10 AVL_WRITE = 1;
	#10 AVL_WRITE = 0;

	#10	// Color 11 = Black
	AVL_ADDR = 5'b0011;
	AVL_WRITEDATA = 32'h00FFFFFF; // Black
	#10 AVL_WRITE = 1;
	#10 AVL_WRITE = 0;

	// Now try to retrieve colors.
	# 20 // Color 0:0.
	palette = 3'b000;
	color_index = 2'b00;
	#50 color_index = 2'b01;
	#50 color_index = 2'b10;
	#50 color_index = 2'b11;
end

endmodule
