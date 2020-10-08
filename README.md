# altera-graphics
A memory-mapped graphics adapter for Altera FPGA and NIOS.

This was originally the final project for ECE 385.
It has been put here since the final project is free-topic, and no academic integrity is involved.

## Hardware Part
The hardware consists of a Nios/II(f) processor IP and a text-mode color graphics adapter. The graphics adapter is memory-mapped onto Nios memory bus.  
The graphics adapter support 80x40 text, with 4 colors for each character.
Common ASCII characters has been baked into hardware, but the driver can also provide new characters, fonts, sprites, patterns, etc. in the form of bitmaps.
An easy way to provide a new pattern can be found in ```software/final_project/core.c```

## Software Part
The software is located in ```software/final_project``` and ```software/final_project_bsp```.
It is a scratch PacMan game intended to demonstract the functionality of underlying hardware.  
The game is controlled with a USB keyboard. It is not perfect but good enough for demoing the graphics adapter.
