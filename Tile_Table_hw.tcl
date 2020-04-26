# TCL File Generated by Component Editor 19.1
# Sun Apr 26 17:06:17 CDT 2020
# DO NOT MODIFY


# 
# Tile_Table "Tile Table Custom IP" v1.0
# Allen Lee 2020.04.26.17:06:17
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module Tile_Table
# 
set_module_property DESCRIPTION ""
set_module_property NAME Tile_Table
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP "ECE 385 Final Project"
set_module_property AUTHOR "Allen Lee"
set_module_property DISPLAY_NAME "Tile Table Custom IP"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL tile_table
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file tile_table.sv SYSTEM_VERILOG PATH tile_table.sv TOP_LEVEL_FILE

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL tile_table
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file tile_table.sv SYSTEM_VERILOG PATH tile_table.sv


# 
# parameters
# 


# 
# display items
# 


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
# connection point RESET
# 
add_interface RESET reset end
set_interface_property RESET associatedClock clock_100
set_interface_property RESET synchronousEdges DEASSERT
set_interface_property RESET ENABLED true
set_interface_property RESET EXPORT_OF ""
set_interface_property RESET PORT_NAME_MAP ""
set_interface_property RESET CMSIS_SVD_VARIABLES ""
set_interface_property RESET SVD_ADDRESS_GROUP ""

add_interface_port RESET RESET reset Input 1


# 
# connection point avalon_slave
# 
add_interface avalon_slave avalon end
set_interface_property avalon_slave addressUnits WORDS
set_interface_property avalon_slave associatedClock clock_100
set_interface_property avalon_slave associatedReset RESET
set_interface_property avalon_slave bitsPerSymbol 8
set_interface_property avalon_slave burstOnBurstBoundariesOnly false
set_interface_property avalon_slave burstcountUnits WORDS
set_interface_property avalon_slave explicitAddressSpan 0
set_interface_property avalon_slave holdTime 0
set_interface_property avalon_slave linewrapBursts false
set_interface_property avalon_slave maximumPendingReadTransactions 0
set_interface_property avalon_slave maximumPendingWriteTransactions 0
set_interface_property avalon_slave readLatency 0
set_interface_property avalon_slave readWaitTime 1
set_interface_property avalon_slave setupTime 0
set_interface_property avalon_slave timingUnits Cycles
set_interface_property avalon_slave writeWaitTime 0
set_interface_property avalon_slave ENABLED true
set_interface_property avalon_slave EXPORT_OF ""
set_interface_property avalon_slave PORT_NAME_MAP ""
set_interface_property avalon_slave CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave AVL_ADDR address Input 13
add_interface_port avalon_slave AVL_READ read Input 1
add_interface_port avalon_slave AVL_WRITE write Input 1
add_interface_port avalon_slave AVL_CS chipselect Input 1
add_interface_port avalon_slave AVL_BYTE_EN byteenable Input 4
add_interface_port avalon_slave AVL_READDATA readdata Output 32
add_interface_port avalon_slave AVL_WRITEDATA writedata Input 32
set_interface_assignment avalon_slave embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave embeddedsw.configuration.isPrintableDevice 0


# 
# connection point conduit_sprite_ram_index
# 
add_interface conduit_sprite_ram_index conduit end
set_interface_property conduit_sprite_ram_index associatedClock clock_100
set_interface_property conduit_sprite_ram_index associatedReset RESET
set_interface_property conduit_sprite_ram_index ENABLED true
set_interface_property conduit_sprite_ram_index EXPORT_OF ""
set_interface_property conduit_sprite_ram_index PORT_NAME_MAP ""
set_interface_property conduit_sprite_ram_index CMSIS_SVD_VARIABLES ""
set_interface_property conduit_sprite_ram_index SVD_ADDRESS_GROUP ""

add_interface_port conduit_sprite_ram_index get_index export Output 8


# 
# connection point conduit_sprite_ram_line
# 
add_interface conduit_sprite_ram_line conduit end
set_interface_property conduit_sprite_ram_line associatedClock clock_100
set_interface_property conduit_sprite_ram_line associatedReset RESET
set_interface_property conduit_sprite_ram_line ENABLED true
set_interface_property conduit_sprite_ram_line EXPORT_OF ""
set_interface_property conduit_sprite_ram_line PORT_NAME_MAP ""
set_interface_property conduit_sprite_ram_line CMSIS_SVD_VARIABLES ""
set_interface_property conduit_sprite_ram_line SVD_ADDRESS_GROUP ""

add_interface_port conduit_sprite_ram_line get_line export Output 4


# 
# connection point conduit_sprite_ram_data
# 
add_interface conduit_sprite_ram_data conduit end
set_interface_property conduit_sprite_ram_data associatedClock clock_100
set_interface_property conduit_sprite_ram_data associatedReset RESET
set_interface_property conduit_sprite_ram_data ENABLED true
set_interface_property conduit_sprite_ram_data EXPORT_OF ""
set_interface_property conduit_sprite_ram_data PORT_NAME_MAP ""
set_interface_property conduit_sprite_ram_data CMSIS_SVD_VARIABLES ""
set_interface_property conduit_sprite_ram_data SVD_ADDRESS_GROUP ""

add_interface_port conduit_sprite_ram_data get_data export Input 16


# 
# connection point conduit_palette_select
# 
add_interface conduit_palette_select conduit end
set_interface_property conduit_palette_select associatedClock clock_100
set_interface_property conduit_palette_select associatedReset RESET
set_interface_property conduit_palette_select ENABLED true
set_interface_property conduit_palette_select EXPORT_OF ""
set_interface_property conduit_palette_select PORT_NAME_MAP ""
set_interface_property conduit_palette_select CMSIS_SVD_VARIABLES ""
set_interface_property conduit_palette_select SVD_ADDRESS_GROUP ""

add_interface_port conduit_palette_select palette export Output 3


# 
# connection point conduit_palette_index
# 
add_interface conduit_palette_index conduit end
set_interface_property conduit_palette_index associatedClock clock_100
set_interface_property conduit_palette_index associatedReset RESET
set_interface_property conduit_palette_index ENABLED true
set_interface_property conduit_palette_index EXPORT_OF ""
set_interface_property conduit_palette_index PORT_NAME_MAP ""
set_interface_property conduit_palette_index CMSIS_SVD_VARIABLES ""
set_interface_property conduit_palette_index SVD_ADDRESS_GROUP ""

add_interface_port conduit_palette_index color_index export Output 2


# 
# connection point conduit_palette_rgb
# 
add_interface conduit_palette_rgb conduit end
set_interface_property conduit_palette_rgb associatedClock clock_100
set_interface_property conduit_palette_rgb associatedReset RESET
set_interface_property conduit_palette_rgb ENABLED true
set_interface_property conduit_palette_rgb EXPORT_OF ""
set_interface_property conduit_palette_rgb PORT_NAME_MAP ""
set_interface_property conduit_palette_rgb CMSIS_SVD_VARIABLES ""
set_interface_property conduit_palette_rgb SVD_ADDRESS_GROUP ""

add_interface_port conduit_palette_rgb rgb export Input 24


# 
# connection point conduit_color_mapper_x
# 
add_interface conduit_color_mapper_x conduit end
set_interface_property conduit_color_mapper_x associatedClock clock_100
set_interface_property conduit_color_mapper_x associatedReset RESET
set_interface_property conduit_color_mapper_x ENABLED true
set_interface_property conduit_color_mapper_x EXPORT_OF ""
set_interface_property conduit_color_mapper_x PORT_NAME_MAP ""
set_interface_property conduit_color_mapper_x CMSIS_SVD_VARIABLES ""
set_interface_property conduit_color_mapper_x SVD_ADDRESS_GROUP ""

add_interface_port conduit_color_mapper_x DrawX export Input 10


# 
# connection point conduit_color_mapper_y
# 
add_interface conduit_color_mapper_y conduit end
set_interface_property conduit_color_mapper_y associatedClock clock_100
set_interface_property conduit_color_mapper_y associatedReset RESET
set_interface_property conduit_color_mapper_y ENABLED true
set_interface_property conduit_color_mapper_y EXPORT_OF ""
set_interface_property conduit_color_mapper_y PORT_NAME_MAP ""
set_interface_property conduit_color_mapper_y CMSIS_SVD_VARIABLES ""
set_interface_property conduit_color_mapper_y SVD_ADDRESS_GROUP ""

add_interface_port conduit_color_mapper_y DrawY export Input 10


# 
# connection point conduit_color_mapper_r
# 
add_interface conduit_color_mapper_r conduit end
set_interface_property conduit_color_mapper_r associatedClock clock_100
set_interface_property conduit_color_mapper_r associatedReset RESET
set_interface_property conduit_color_mapper_r ENABLED true
set_interface_property conduit_color_mapper_r EXPORT_OF ""
set_interface_property conduit_color_mapper_r PORT_NAME_MAP ""
set_interface_property conduit_color_mapper_r CMSIS_SVD_VARIABLES ""
set_interface_property conduit_color_mapper_r SVD_ADDRESS_GROUP ""

add_interface_port conduit_color_mapper_r VGA_RED export Output 8


# 
# connection point conduit_color_mapper_g
# 
add_interface conduit_color_mapper_g conduit end
set_interface_property conduit_color_mapper_g associatedClock clock_100
set_interface_property conduit_color_mapper_g associatedReset RESET
set_interface_property conduit_color_mapper_g ENABLED true
set_interface_property conduit_color_mapper_g EXPORT_OF ""
set_interface_property conduit_color_mapper_g PORT_NAME_MAP ""
set_interface_property conduit_color_mapper_g CMSIS_SVD_VARIABLES ""
set_interface_property conduit_color_mapper_g SVD_ADDRESS_GROUP ""

add_interface_port conduit_color_mapper_g VGA_GREEN export Output 8


# 
# connection point conduit_color_mapper_b
# 
add_interface conduit_color_mapper_b conduit end
set_interface_property conduit_color_mapper_b associatedClock clock_100
set_interface_property conduit_color_mapper_b associatedReset RESET
set_interface_property conduit_color_mapper_b ENABLED true
set_interface_property conduit_color_mapper_b EXPORT_OF ""
set_interface_property conduit_color_mapper_b PORT_NAME_MAP ""
set_interface_property conduit_color_mapper_b CMSIS_SVD_VARIABLES ""
set_interface_property conduit_color_mapper_b SVD_ADDRESS_GROUP ""

add_interface_port conduit_color_mapper_b VGA_BLUE export Output 8

