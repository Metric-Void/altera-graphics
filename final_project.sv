module final_project (
	input  logic        CLOCK_50,
	input  logic [1:0]  KEY,
	output logic [12:0] DRAM_ADDR,
	output logic [1:0]  DRAM_BA,
	output logic        DRAM_CAS_N,
	output logic        DRAM_CKE,
	output logic        DRAM_CS_N,
	inout  logic [31:0] DRAM_DQ,
	output logic [3:0]  DRAM_DQM,
	output logic        DRAM_RAS_N,
	output logic        DRAM_WE_N,
	output logic        DRAM_CLK,
	
	output logic [7:0]  VGA_R,        //VGA Red
							  VGA_G,        //VGA Green
							  VGA_B,        //VGA Blue
	output logic        VGA_CLK,      //VGA Clock
							  VGA_SYNC_N,   //VGA Sync signal
							  VGA_BLANK_N,  //VGA Blank signal
							  VGA_VS,       //VGA virtical sync signal
							  VGA_HS,       //VGA horizontal sync signal
	// CY7C67200 Interface
	inout  wire  [15:0] OTG_DATA,     //CY7C67200 Data bus 16 Bits
	output logic [1:0]  OTG_ADDR,     //CY7C67200 Address 2 Bits
	output logic        OTG_CS_N,     //CY7C67200 Chip Select
							  OTG_RD_N,     //CY7C67200 Write
							  OTG_WR_N,     //CY7C67200 Read
							  OTG_RST_N,    //CY7C67200 Reset
	input               OTG_INT       //CY7C67200 Interrupt
);

logic [15:0] hpi_data_in, hpi_data_out;
logic [1:0]  hpi_addr;
logic hpi_r, hpi_w, hpi_reset, hpi_cs;
logic [9:0] DrawX, DrawY;
logic Reset_h;

logic CLK_100;

final_project_soc fp_system (
	.clk_clk(CLOCK_50),								// Clock input
	.clk_100_clk(CLK_100),							// 100MHz clock output.
	.reset_reset_n(KEY[0]),							// Reset key
	.sdram_wire_addr(DRAM_ADDR),					// sdram_wire.addr
	.sdram_wire_ba(DRAM_BA),						// sdram_wire.ba
	.sdram_wire_cas_n(DRAM_CAS_N),				// sdram_wire.cas_n
	.sdram_wire_cke(DRAM_CKE),						// sdram_wire.cke
	.sdram_wire_cs_n(DRAM_CS_N),					// sdram.cs_n
	.sdram_wire_dq(DRAM_DQ),						// sdram.dq
	.sdram_wire_dqm(DRAM_DQM),						// sdram.dqm
	.sdram_wire_ras_n(DRAM_RAS_N),				// sdram.ras_n
	.sdram_wire_we_n(DRAM_WE_N),					// sdram.we_n
	.sdram_clk_clk(DRAM_CLK),						// Clock out to SDRAM
	.hpi_addr_export(hpi_addr),     				// hpi_addr.export
	.hpi_cs_export(hpi_cs),       				// hpi_cs.export
	.hpi_data_in_port(hpi_data_in),  			// hpi_data_in.export
	.hpi_data_out_port(hpi_data_out), 			// hpi_data_out.export
	.hpi_r_export(hpi_r),        					// hpi_r.export
	.hpi_reset_export(hpi_reset),    			// hpi_reset.export
	.hpi_w_export(hpi_w),        					// hpi_w.export
	.tt_cm_x_export(DrawX),
	.tt_cm_y_export(DrawY),
	.tt_cm_r_export(VGA_R),
	.tt_cm_g_export(VGA_G),
	.tt_cm_b_export(VGA_B),
	.vga_clk_25_clk(VGA_CLK),						// 25MHz VGA clock generated by this SoC PLL.
	.vga_vs_export(VGA_VS)
);

always_ff @(posedge CLK_100) begin
	Reset_h <= ~(KEY[0]);
end

// HPI Interface from lab 8.
hpi_io_intf hpi_io_inst(
	 .Clk(CLK_100),
	 .Reset(Reset_h),
	 // signals connected to NIOS II
	 .from_sw_address(hpi_addr),
	 .from_sw_data_in(hpi_data_in),
	 .from_sw_data_out(hpi_data_out),
	 .from_sw_r(hpi_r),
	 .from_sw_w(hpi_w),
	 .from_sw_cs(hpi_cs),
	 .from_sw_reset(hpi_reset),
	 // signals connected to EZ-OTG chip
	 .OTG_DATA(OTG_DATA),    
	 .OTG_ADDR(OTG_ADDR),    
	 .OTG_RD_N(OTG_RD_N),    
	 .OTG_WR_N(OTG_WR_N),    
	 .OTG_CS_N(OTG_CS_N),
	 .OTG_RST_N(OTG_RST_N)
);


VGA_controller screen_vga0(
	.Clk(CLOCK_50),
	.Reset(~KEY[0]),
	.VGA_HS, .VGA_VS, .VGA_CLK, .VGA_BLANK_N, .VGA_SYNC_N,
	.DrawX, .DrawY
);

endmodule
