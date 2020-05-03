# Create Clocks
create_clock -name {Clk} -period 20.000 -waveform { 0.000 10.000 } [get_ports {CLOCK_50}]

# derive_pll_clocks -create_base_clocks

#create_generated_clock 
#-name {fp_system|sdram_pll|sd1|pll|clk[0]} 
#-source [get_pins {lab9_qsystem|altpll_0|sd1|pll|inclk[0]}] 
#-duty_cycle 50.000 
#-multiply_by 1 -phase -54.000 
#-master_clock {CLOCK_50} [get_pins {lab9_qsystem|altpll_0|sd1|pll|clk[0]}] 

create_generated_clock -name {fp_system|sdram_pll|sd1|pll7|clk[0]} -source [get_pins {fp_system|sdram_pll|sd1|pll7|inclk[0]}] -duty_cycle 50.000 -multiply_by 1 -phase 0.00 -master_clock {Clk}   [get_pins {fp_system|sdram_pll|sd1|pll7|clk[0]}]
create_generated_clock -name {fp_system|sdram_pll|sd1|pll7|clk[1]} -source [get_pins {fp_system|sdram_pll|sd1|pll7|inclk[0]}] -duty_cycle 50.000 -divide_by 1   -phase 0.00 -master_clock {Clk} [get_pins {fp_system|sdram_pll|sd1|pll7|clk[1]}]
create_generated_clock -name {fp_system|sdram_pll|sd1|pll7|clk[2]} -source [get_pins {fp_system|sdram_pll|sd1|pll7|inclk[0]}] -duty_cycle 50.000 -multiply_by 2 -phase 0.00 -master_clock {Clk}   [get_pins {fp_system|sdram_pll|sd1|pll7|clk[2]}]
create_generated_clock -name {fp_system|sdram_pll|sd1|pll7|clk[3]} -source [get_pins {fp_system|sdram_pll|sd1|pll7|inclk[0]}] -duty_cycle 50.000 -multiply_by 2 -phase -54.00 -master_clock {Clk} [get_pins {fp_system|sdram_pll|sd1|pll7|clk[3]}]

# Constrain the input I/O path
set_input_delay -clock {Clk} -max 3 [all_inputs]
set_input_delay -clock {Clk} -min 2 [all_inputs]

# Constrain the output I/O path
set_output_delay -clock {Clk} 2 [all_outputs]

