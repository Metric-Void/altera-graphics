//-------------------------------------------------------------------------
//    Color_Mapper.sv                                                    --
//    Stephen Kempf                                                      --
//    3-1-06                                                             --
//                                                                       --
//    Modified by David Kesler  07-16-2008                               --
//    Translated by Joe Meng    07-07-2013                               --
//    Modified by Po-Han Huang  10-06-2017                               --
//                                                                       --
//    Fall 2017 Distribution                                             --
//                                                                       --
//    For use with ECE 385 Lab 8                                         --
//    University of Illinois ECE Department                              --
//-------------------------------------------------------------------------

// color_mapper: Decide which color to be output to VGA for each pixel.
module  color_mapper ( input        [9:0] DrawX, DrawY,       // Current pixel coordinates
							  input        [1:0] extend_color,
							  input        [7:0] export_pattern,
                       output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output
                     );
    
    logic [7:0] Red, Green, Blue;
    
    // Output colors to VGA
    assign VGA_R = Red;
    assign VGA_G = Green;
    assign VGA_B = Blue;
    
    
//    always_comb          //color correpond to 00,01,10,11 can be separately defined for each pattern
//    begin
//			case (export_pattern)      //determine the shape code first
//				2'h00: case (extend_color)      //define color code for each shape
//					2'b00: begin
//							 Red = 8'hff;
//							 Green = 8'hff;
//							 Blue = 8'hff;
//					end
//					endcase
//				endcase
//    end 
    
endmodule
