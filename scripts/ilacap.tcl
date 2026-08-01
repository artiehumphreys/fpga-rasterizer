open_hw_manager
connect_hw_server
open_hw_target
set dev [lindex [get_hw_devices] 0]
current_hw_device $dev
set_property PROGRAM.FILE build_vivado/rasterizer_fpga.runs/impl_1/system_wrapper.bit $dev
set_property PROBES.FILE  build_vivado/rasterizer_fpga.runs/impl_1/system_wrapper.ltx $dev
program_hw_devices $dev
refresh_hw_device $dev

set ila [lindex [get_hw_ilas -of_objects $dev] 0]
puts "ilacap: ila = $ila"
puts "ilacap: probes ="
foreach p [get_hw_probes -of_objects $ila] { puts "   $p" }

set tv [lindex [get_hw_probes -of_objects $ila -regexp {.*ready.*[Tt][Vv]alid}] 0]
set_property TRIGGER_COMPARE_VALUE eq1'b1 $tv
set_property CONTROL.TRIGGER_POSITION 64 $ila
set_property CONTROL.DATA_DEPTH 4096 $ila
run_hw_ila $ila

set fired [wait_on_hw_ila -timeout 10 $ila]
puts "ilacap: trigger fired = $fired (0/false = ready TVALID never asserted)"
write_hw_ila_data -csv_file ila.csv -force [upload_hw_ila_data $ila]
puts "ilacap: wrote ila.csv"
