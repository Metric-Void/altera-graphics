# TCL File Generated by Component Editor 19.1
# Sat May 02 17:46:14 CDT 2020
# DO NOT MODIFY


# 
# Color_Palette "Color Palette Custom IP" v1.0
# Allen Lee 2020.05.02.17:46:14
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module Color_Palette
# 
set_module_property DESCRIPTION ""
set_module_property NAME Color_Palette
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP "ECE 385 Final Project"
set_module_property AUTHOR "Allen Lee"
set_module_property DISPLAY_NAME "Color Palette Custom IP"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL color_palette
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file color_palette.sv SYSTEM_VERILOG PATH color_palette.sv TOP_LEVEL_FILE

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL color_palette
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file color_palette.sv SYSTEM_VERILOG PATH color_palette.sv
add_fileset_file sim_palette.sv SYSTEM_VERILOG PATH sim_palette.sv


# 
# parameters
# 


# 
# display items
# 


# 
# connection point avalon_slave_0
# 
add_interface avalon_slave_0 avalon end
set_interface_property avalon_slave_0 addressUnits WORDS
set_interface_property avalon_slave_0 associatedClock clock_100
set_interface_property avalon_slave_0 associatedReset reset
set_interface_property avalon_slave_0 bitsPerSymbol 8
set_interface_property avalon_slave_0 burstOnBurstBoundariesOnly false
set_interface_property avalon_slave_0 burstcountUnits WORDS
set_interface_property avalon_slave_0 explicitAddressSpan 0
set_interface_property avalon_slave_0 holdTime 0
set_interface_property avalon_slave_0 linewrapBursts false
set_interface_property avalon_slave_0 maximumPendingReadTransactions 0
set_interface_property avalon_slave_0 maximumPendingWriteTransactions 0
set_interface_property avalon_slave_0 readLatency 0
set_interface_property avalon_slave_0 readWaitStates 0
set_interface_property avalon_slave_0 readWaitTime 0
set_interface_property avalon_slave_0 setupTime 0
set_interface_property avalon_slave_0 timingUnits Cycles
set_interface_property avalon_slave_0 writeWaitTime 0
set_interface_property avalon_slave_0 ENABLED true
set_interface_property avalon_slave_0 EXPORT_OF ""
set_interface_property avalon_slave_0 PORT_NAME_MAP ""
set_interface_property avalon_slave_0 CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave_0 SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave_0 AVL_ADDR address Input 5
add_interface_port avalon_slave_0 AVL_BYTE_EN byteenable Input 4
add_interface_port avalon_slave_0 AVL_CS chipselect Input 1
add_interface_port avalon_slave_0 AVL_READ read Input 1
add_interface_port avalon_slave_0 AVL_READDATA readdata Output 32
add_interface_port avalon_slave_0 AVL_WRITE write Input 1
add_interface_port avalon_slave_0 AVL_WRITEDATA writedata Input 32
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock_100
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset RESET reset Input 1


# 
# connection point clock_50
# 
add_interface clock_50 clock end
set_interface_property clock_50 clockRate 50000000
set_interface_property clock_50 ENABLED true
set_interface_property clock_50 EXPORT_OF ""
set_interface_property clock_50 PORT_NAME_MAP ""
set_interface_property clock_50 CMSIS_SVD_VARIABLES ""
set_interface_property clock_50 SVD_ADDRESS_GROUP ""

add_interface_port clock_50 CLK_50 clk Input 1


# 
# connection point clock_100
# 
add_interface clock_100 clock end
set_interface_property clock_100 clockRate 100000000
set_interface_property clock_100 ENABLED true
set_interface_property clock_100 EXPORT_OF ""
set_interface_property clock_100 PORT_NAME_MAP ""
set_interface_property clock_100 CMSIS_SVD_VARIABLES ""
set_interface_property clock_100 SVD_ADDRESS_GROUP ""

add_interface_port clock_100 CLK_100 clk Input 1


# 
# connection point conduit_color_index
# 
add_interface conduit_color_index conduit end
set_interface_property conduit_color_index associatedClock clock_100
set_interface_property conduit_color_index associatedReset reset
set_interface_property conduit_color_index ENABLED true
set_interface_property conduit_color_index EXPORT_OF ""
set_interface_property conduit_color_index PORT_NAME_MAP ""
set_interface_property conduit_color_index CMSIS_SVD_VARIABLES ""
set_interface_property conduit_color_index SVD_ADDRESS_GROUP ""

add_interface_port conduit_color_index color_index export Input 2


# 
# connection point conduit_palette_select
# 
add_interface conduit_palette_select conduit end
set_interface_property conduit_palette_select associatedClock clock_100
set_interface_property conduit_palette_select associatedReset reset
set_interface_property conduit_palette_select ENABLED true
set_interface_property conduit_palette_select EXPORT_OF ""
set_interface_property conduit_palette_select PORT_NAME_MAP ""
set_interface_property conduit_palette_select CMSIS_SVD_VARIABLES ""
set_interface_property conduit_palette_select SVD_ADDRESS_GROUP ""

add_interface_port conduit_palette_select palette export Input 3


# 
# connection point conduit_rgb
# 
add_interface conduit_rgb conduit end
set_interface_property conduit_rgb associatedClock clock_100
set_interface_property conduit_rgb associatedReset reset
set_interface_property conduit_rgb ENABLED true
set_interface_property conduit_rgb EXPORT_OF ""
set_interface_property conduit_rgb PORT_NAME_MAP ""
set_interface_property conduit_rgb CMSIS_SVD_VARIABLES ""
set_interface_property conduit_rgb SVD_ADDRESS_GROUP ""

add_interface_port conduit_rgb rgb export Output 24

