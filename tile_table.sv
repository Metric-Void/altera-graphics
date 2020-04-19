module tile_tabele (
	input logic Clk,
	input logic [9:0] DrawX, DrawY,
	output logic [1:0] extend_color,
	output logic [7:0] export_pattern
	);

	logic [7:0] tiles [80][30];
	logic [10:0] fr_addr;
	logic [10:0] fr_row;
	logic [7:0] data;
	logic [2:0] ex_color;
	logic [7:0] curr_pattern;			//Assume using all 2 bits of the hex number in font_rom
	assign extend_color = ex_color;  //Output color to color_mapper
	assign export_pattern = curr_pattern;    //Output desired pattern to color_mapper
	
	font_rom from(.addr(fr_addr), .data(data));
	
	always_ff @(posedge Clk) begin
		tile_x = DrawX / 8;
		tile_y = DrawY / 16;
		fr_row = tiles[tile_x][tile_y] * 16 + (DrawY - tile_y * 16);
		fr_addr = fr_row;
		curr_pattern = fr_addr / 16;   //finding the corresponding pattern in font_rom
		curr_color = data[DrawX - tile_x * 8];
		
		ex_color = 2'b00;         //define deflat value
		if (curr_color == 1'b0) begin //extend to 2 bits
			ex_color = 2'b00;
		end
		else begin 
			ex_color = 2'b11;
		end
		
	end
	
endmodule 
