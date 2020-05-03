	component final_project_soc is
		port (
			clk_clk           : in    std_logic                     := 'X';             -- clk
			clk_100_clk       : out   std_logic;                                        -- clk
			hpi_addr_export   : out   std_logic_vector(1 downto 0);                     -- export
			hpi_cs_export     : out   std_logic;                                        -- export
			hpi_data_in_port  : in    std_logic_vector(15 downto 0) := (others => 'X'); -- in_port
			hpi_data_out_port : out   std_logic_vector(15 downto 0);                    -- out_port
			hpi_r_export      : out   std_logic;                                        -- export
			hpi_reset_export  : out   std_logic;                                        -- export
			hpi_w_export      : out   std_logic;                                        -- export
			reset_reset_n     : in    std_logic                     := 'X';             -- reset_n
			sdram_clk_clk     : out   std_logic;                                        -- clk
			sdram_wire_addr   : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba     : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n  : out   std_logic;                                        -- cas_n
			sdram_wire_cke    : out   std_logic;                                        -- cke
			sdram_wire_cs_n   : out   std_logic;                                        -- cs_n
			sdram_wire_dq     : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm    : out   std_logic_vector(3 downto 0);                     -- dqm
			sdram_wire_ras_n  : out   std_logic;                                        -- ras_n
			sdram_wire_we_n   : out   std_logic;                                        -- we_n
			tt_cm_b_export    : out   std_logic_vector(7 downto 0);                     -- export
			tt_cm_g_export    : out   std_logic_vector(7 downto 0);                     -- export
			tt_cm_r_export    : out   std_logic_vector(7 downto 0);                     -- export
			tt_cm_x_export    : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			tt_cm_y_export    : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			vga_clk_25_clk    : out   std_logic;                                        -- clk
			vga_vs_export     : in    std_logic                     := 'X'              -- export
		);
	end component final_project_soc;

	u0 : component final_project_soc
		port map (
			clk_clk           => CONNECTED_TO_clk_clk,           --        clk.clk
			clk_100_clk       => CONNECTED_TO_clk_100_clk,       --    clk_100.clk
			hpi_addr_export   => CONNECTED_TO_hpi_addr_export,   --   hpi_addr.export
			hpi_cs_export     => CONNECTED_TO_hpi_cs_export,     --     hpi_cs.export
			hpi_data_in_port  => CONNECTED_TO_hpi_data_in_port,  --   hpi_data.in_port
			hpi_data_out_port => CONNECTED_TO_hpi_data_out_port, --           .out_port
			hpi_r_export      => CONNECTED_TO_hpi_r_export,      --      hpi_r.export
			hpi_reset_export  => CONNECTED_TO_hpi_reset_export,  --  hpi_reset.export
			hpi_w_export      => CONNECTED_TO_hpi_w_export,      --      hpi_w.export
			reset_reset_n     => CONNECTED_TO_reset_reset_n,     --      reset.reset_n
			sdram_clk_clk     => CONNECTED_TO_sdram_clk_clk,     --  sdram_clk.clk
			sdram_wire_addr   => CONNECTED_TO_sdram_wire_addr,   -- sdram_wire.addr
			sdram_wire_ba     => CONNECTED_TO_sdram_wire_ba,     --           .ba
			sdram_wire_cas_n  => CONNECTED_TO_sdram_wire_cas_n,  --           .cas_n
			sdram_wire_cke    => CONNECTED_TO_sdram_wire_cke,    --           .cke
			sdram_wire_cs_n   => CONNECTED_TO_sdram_wire_cs_n,   --           .cs_n
			sdram_wire_dq     => CONNECTED_TO_sdram_wire_dq,     --           .dq
			sdram_wire_dqm    => CONNECTED_TO_sdram_wire_dqm,    --           .dqm
			sdram_wire_ras_n  => CONNECTED_TO_sdram_wire_ras_n,  --           .ras_n
			sdram_wire_we_n   => CONNECTED_TO_sdram_wire_we_n,   --           .we_n
			tt_cm_b_export    => CONNECTED_TO_tt_cm_b_export,    --    tt_cm_b.export
			tt_cm_g_export    => CONNECTED_TO_tt_cm_g_export,    --    tt_cm_g.export
			tt_cm_r_export    => CONNECTED_TO_tt_cm_r_export,    --    tt_cm_r.export
			tt_cm_x_export    => CONNECTED_TO_tt_cm_x_export,    --    tt_cm_x.export
			tt_cm_y_export    => CONNECTED_TO_tt_cm_y_export,    --    tt_cm_y.export
			vga_clk_25_clk    => CONNECTED_TO_vga_clk_25_clk,    -- vga_clk_25.clk
			vga_vs_export     => CONNECTED_TO_vga_vs_export      --     vga_vs.export
		);

