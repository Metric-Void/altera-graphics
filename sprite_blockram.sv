module sprite_blockram(
	input CLK_100, reset,
	// Port 0: 32-bit wide.
	input [6:0] addr0_sprite_index,
	input [2:0] addr0_sprite_line,
	input [3:0] be0,
	output [31:0] data_r0,
	input [31:0] data_w0,
	input r0,w0,cs0,
	
	// Port 1: 16-bit wide.
	input [6:0] addr1_sprite_index,
	input [3:0] addr1_sprite_line,
	input [15:0] data_w1,
	output [15:0] data_r1,
	input [1:0] be1,
	input r1,w1
);

	
logic [15:0] regfile[128][16];

wire [3:0] dwidth_addr0, dwidth_addr1;

assign dwidth_addr0 = {addr0_sprite_line, 1'b0};
assign dwidth_addr1 = {addr0_sprite_line, 1'b1};

always_ff @(negedge CLK_100) begin
	// Port 0
	if(cs0) begin
		if(r0) begin
			data_r0 = {regfile[addr0_sprite_index][dwidth_addr0], regfile[addr0_sprite_index][dwidth_addr1]};
		end else if (w0) begin
			 if(be0[0]) regfile[addr0_sprite_index][dwidth_addr0][7:0] = data_w0[7:0];
			 if(be0[1]) regfile[addr0_sprite_index][dwidth_addr0][15:8] = data_w0[15:8];
			 if(be0[2]) regfile[addr0_sprite_index][dwidth_addr1][7:0] = data_w0[23:16];
			 if(be0[3]) regfile[addr0_sprite_index][dwidth_addr1][15:8] = data_w0[31:24];
		end
	end
	
	// Port 1.
	if(r1) begin
		data_r1 = regfile[addr1_sprite_index][addr1_sprite_line];
	end else if (w1) begin
		 if(be1[0]) regfile[addr1_sprite_index][addr1_sprite_line][7:0] = data_w1[7:0];
		 if(be1[1]) regfile[addr1_sprite_index][addr1_sprite_line][15:8] = data_w1[15:8];
	end
	
	// Reset
	if(reset) begin
		for(int i=0; i<=127; i++) begin
			for(int j=0; j<=15; j++) begin
				regfile[i][j] = 16'd0;
			end
		end
	end
end

endmodule
