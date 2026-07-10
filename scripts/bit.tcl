open_project build_vivado/rasterizer_fpga.xpr

launch_runs synth_1 -jobs 4
wait_on_run synth_1

launch_runs impl_1 -to_step write_bitstream -jobs 4
wait_on_run impl_1

if {[get_property PROGRESS [get_runs impl_1]] ne "100%"} {
  error "bit.tcl: impl_1 did not finish (check timing/DRC in the run log)."
}

puts "bit.tcl: bitstream at build_vivado/rasterizer_fpga.runs/impl_1/system_wrapper.bit"
