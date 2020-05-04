
module final_project_soc (
	clk_clk,
	clk_100_export_clk,
	hpi_addr_export,
	hpi_cs_export,
	hpi_data_in_port,
	hpi_data_out_port,
	hpi_r_export,
	hpi_reset_export,
	hpi_w_export,
	pll_200_st_clk,
	reset_reset_n,
	sdram_clk_clk,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	tt_cm_b_export,
	tt_cm_g_export,
	tt_cm_r_export,
	tt_cm_x_export,
	tt_cm_y_export,
	vga_clk_25_clk,
	vga_vs_export);	

	input		clk_clk;
	output		clk_100_export_clk;
	output	[1:0]	hpi_addr_export;
	output		hpi_cs_export;
	input	[15:0]	hpi_data_in_port;
	output	[15:0]	hpi_data_out_port;
	output		hpi_r_export;
	output		hpi_reset_export;
	output		hpi_w_export;
	output		pll_200_st_clk;
	input		reset_reset_n;
	output		sdram_clk_clk;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[31:0]	sdram_wire_dq;
	output	[3:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	output	[7:0]	tt_cm_b_export;
	output	[7:0]	tt_cm_g_export;
	output	[7:0]	tt_cm_r_export;
	input	[9:0]	tt_cm_x_export;
	input	[9:0]	tt_cm_y_export;
	output		vga_clk_25_clk;
	input		vga_vs_export;
endmodule
