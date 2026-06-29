open_hw_manager
connect_hw_server
open_hw_target
current_hw_device [lindex [get_hw_devices] 0]
refresh_hw_device -update_hw_probes false [current_hw_device]
set axi [lindex [get_hw_axis] 0]

proc axi_wr {axi addr data} { delete_hw_axi_txn -quiet t; create_hw_axi_txn t $axi -address $addr -data $data -type write; run_hw_axi t }
proc axi_rd {axi addr} { delete_hw_axi_txn -quiet t; create_hw_axi_txn t $axi -address $addr -type read; run_hw_axi t; return [get_property DATA [get_hw_axi_txns t]] }

axi_wr $axi 80000000 deadbeef
axi_wr $axi 80000004 cafef00d
puts "ddrtest: 0x80000000 = 0x[axi_rd $axi 80000000] (expect deadbeef)"
puts "ddrtest: 0x80000004 = 0x[axi_rd $axi 80000004] (expect cafef00d)"
