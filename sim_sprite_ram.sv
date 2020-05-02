// Sprite RAM tester.

module sprite_ram_test();

`timescale 1ns/1ps

logic CLK_50 = 0;
logic CLK_100 = 0;

logic reset = 0;

logic [7:0] index_line;
logic [3:0] line_line;
logic [15:0] data_line;
logic AVL_READ, AVL_WRITE, AVL_CS;
logic [3:0] AVL_BYTE_EN;
logic [8:0] AVL_ADDR;
logic [31:0] AVL_WRITEDATA;
logic [31:0] AVL_READDATA;
logic RESET;

sprite_ram testee(
	.CLK_50,
	.CLK_100,
	.get_index(index_line),		// Sprite index
	.get_line(line_line),		// Defines which lines to get
	.get_data(data_line),		// Resulting data.
	.*
);

always begin
	#5 CLK_100 = ~CLK_100;
end

always begin
	#10 CLK_50 = ~CLK_50;
end

initial begin
	// Font ROM test.
	index_line = 8'b00000001;
	line_line = 4'd7;
	
	#20 line_line = 4'd3;
	#20 line_line = 4'd0;
	
	// Font RAM test.
	// Write something to 100000000
	AVL_ADDR = 9'h00;
	AVL_WRITEDATA = 32'hCFFCDFFD;
	AVL_CS = 1'b1;
	#10 AVL_WRITE = 1'b1;
		 AVL_BYTE_EN = 4'hF;
	#20 AVL_WRITE = 1'b0;
		 AVL_CS = 1'b0;
	
	index_line = 8'b10000000;
	line_line = 4'd0;
	#20 line_line = 4'd1;
end

endmodule
