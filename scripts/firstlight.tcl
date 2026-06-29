# make program and read the framebuffer back from DDR3.
# Register map: xrasterizer_hw.h. AXI map (from jtag_axi): control @0x0, DDR3 @0x80000000.

open_hw_manager
connect_hw_server
open_hw_target
current_hw_device [lindex [get_hw_devices] 0]
refresh_hw_device -update_hw_probes false [current_hw_device]

set axi [lindex [get_hw_axis] 0]
puts "firstlight: using hw_axi $axi"

proc axi_wr {axi addr data} {
  delete_hw_axi_txn -quiet t
  create_hw_axi_txn t $axi -address $addr -data $data -type write
  run_hw_axi t
}
proc axi_rd {axi addr} {
  delete_hw_axi_txn -quiet t
  create_hw_axi_txn t $axi -address $addr -type read
  run_hw_axi t
  return [get_property DATA [get_hw_axi_txns t]]
}

# framebuffer pointer (fb_mem, 64-bit) = DDR3 base
axi_wr $axi 00000010 80000000; # fb_mem[31:0]
axi_wr $axi 00000014 00000000; # fb_mem[63:32]

axi_wr $axi 00000000 00000001

set ctrl ffffffff
for {set i 0} {$i < 200} {incr i} {
  set ctrl [axi_rd $axi 00000000]
  if {[expr {0x$ctrl & 0x2}]} break
  after 10
}
if {![expr {0x$ctrl & 0x2}]} {
  error "firstlight: kernel never asserted ap_done (ctrl=0x$ctrl)"
}
puts "firstlight: ap_done asserted (ctrl=0x$ctrl)"

# read first 8 words of DDR3 (top-left pixels, RGBA little-endian per word)
delete_hw_axi_txn -quiet r_fb
create_hw_axi_txn r_fb $axi -address 80000000 -len 8 -type read
run_hw_axi r_fb
puts "firstlight: DDR3\[0x80000000\]+32B = [get_property DATA [get_hw_axi_txns r_fb]]"
