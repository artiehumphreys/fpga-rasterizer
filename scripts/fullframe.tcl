open_hw_manager
connect_hw_server
open_hw_target
current_hw_device [lindex [get_hw_devices] 0]
refresh_hw_device -update_hw_probes false [current_hw_device]

set axi [lindex [get_hw_axis] 0]
puts "fullframe: using hw_axi $axi"

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
# Burst read
proc axi_rd_burst {axi addr len} {
  delete_hw_axi_txn -quiet b
  create_hw_axi_txn b $axi -address $addr -type read -len $len
  run_hw_axi b
  return [get_property DATA [get_hw_axi_txns b]]
}

proc ap_done {ctrl} { set v 0; scan $ctrl %x v; return [expr {($v >> 1) & 1}] }

# get DDR3 byte address (see firstlight.tcl)
set FB_BASE 0x80000000
set FB_W 1920
set FB_H 1080
proc px_addr {x y} { format %08x [expr {$::FB_BASE + ($y * $::FB_W + $x) * 4}] }

axi_wr $axi 00000010 80000000
axi_wr $axi 00000014 00000000
axi_rd $axi 00000000
axi_wr $axi 00000000 00000001

set ctrl ffffffff
for {set i 0} {$i < 200} {incr i} {
  set ctrl [axi_rd $axi 00000000]
  if {[ap_done $ctrl]} break
  after 10
}

if {![ap_done $ctrl]} {
  error "fullframe: kernel never asserted ap_done (ctrl=0x$ctrl)"
}
puts "fullframe: ap_done asserted (ctrl=0x$ctrl)"

# read back the whole framebuffer
set FB_BYTES [expr {$FB_W * $FB_H * 4}]
set BEATS 256; # 256 * 4 B = 1 KB/burst
set STEP [expr {$BEATS * 4}]
set REVERSE  1

set fh [open hw_fb.hex w]
set nburst [expr {$FB_BYTES / $STEP}]
for {set k 0} {$k < $nburst} {incr k} {
  set addr [format %08x [expr {$FB_BASE + $k * $STEP}]]
  set hex  [string tolower [axi_rd_burst $axi $addr $BEATS]]
  set exp  [expr {$BEATS * 8}]
  while {[string length $hex] < $exp} { set hex 0$hex };  # left-pad to full width
  set words {}
  for {set i 0} {$i < $BEATS} {incr i} {
    lappend words [string range $hex [expr {$i*8}] [expr {$i*8+7}]]
  }
  if {$REVERSE} { set words [lreverse $words] }
  foreach w $words { puts $fh $w }
  if {$k % 500 == 0} { puts "fullframe: burst $k / $nburst" }
}
close $fh
puts "fullframe: wrote hw_fb.hex ([expr {$FB_W*$FB_H}] words)"
