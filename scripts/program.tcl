# Needs the board powered and the JTAG cable plugged into the workstation.

open_hw_manager
connect_hw_server
open_hw_target
current_hw_device [lindex [get_hw_devices] 0]
set_property PROGRAM.FILE \
  build_vivado/rasterizer_fpga.runs/impl_1/system_wrapper.bit [current_hw_device]
program_hw_devices [current_hw_device]
puts "program.tcl: device programmed."
