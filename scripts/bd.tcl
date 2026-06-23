# scripts/bd.tcl — build the QMTECH Wukong block design for the rasterizer.
# System: 50MHz osc -> clk_wiz -> MIG (DDR3). JTAG-to-AXI + rasterizer kernel
# share an AXI SmartConnect crossbar reaching DDR3 and the kernel's control regs.

set part      xc7a100t-fgg676-1
set proj_dir  build_vivado
set proj_name rasterizer_fpga
set bd_name   system

file delete -force $proj_dir
create_project $proj_name $proj_dir -part $part

# Make the HLS-packaged kernel available
set_property ip_repo_paths build/hls/impl/ip [current_project]
update_ip_catalog

create_bd_design $bd_name

# DDR3 controller, configured from our saved MIG project file.
set mig [create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series mig_7series_0]
set_property CONFIG.XML_INPUT_FILE [pwd]/boards/wukong/mig.prj $mig

# Clock wizard: 50 MHz board clock -> 166.666 MHz that the MIG wants.
set clk [create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz clk_wiz_0]
set_property -dict {
  CONFIG.PRIM_IN_FREQ               {50.000}
  CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {166.666}
} $clk

create_bd_cell -type ip -vlnv xilinx.com:ip:jtag_axi jtag_axi_0

create_bd_cell -type ip -vlnv xilinx.com:hls:rasterizer rasterizer_0

create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect smartconnect_0
create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset proc_sys_reset_0

# AXI data paths through the crossbar
set_property -dict {CONFIG.NUM_SI {2} CONFIG.NUM_MI {2}} [get_bd_cells smartconnect_0]
# two masters IN: JTAG (control + readback), kernel (framebuffer writes)
connect_bd_intf_net [get_bd_intf_pins jtag_axi_0/M_AXI]         [get_bd_intf_pins smartconnect_0/S00_AXI]
connect_bd_intf_net [get_bd_intf_pins rasterizer_0/m_axi_gmem0] [get_bd_intf_pins smartconnect_0/S01_AXI]
# two slaves OUT: DDR3, kernel control registers
connect_bd_intf_net [get_bd_intf_pins smartconnect_0/M00_AXI]   [get_bd_intf_pins mig_7series_0/S_AXI]
connect_bd_intf_net [get_bd_intf_pins smartconnect_0/M01_AXI]   [get_bd_intf_pins rasterizer_0/s_axi_control]

# clocks
connect_bd_net [get_bd_pins clk_wiz_0/clk_out1] [get_bd_pins mig_7series_0/sys_clk_i]
# MIG's user clock is the single AXI clock domain for everything else
set uiclk [get_bd_pins mig_7series_0/ui_clk]
connect_bd_net $uiclk [get_bd_pins smartconnect_0/aclk]
connect_bd_net $uiclk [get_bd_pins jtag_axi_0/aclk]
connect_bd_net $uiclk [get_bd_pins rasterizer_0/ap_clk]
connect_bd_net $uiclk [get_bd_pins proc_sys_reset_0/slowest_sync_clk]

# resets
connect_bd_net [get_bd_pins mig_7series_0/mmcm_locked] [get_bd_pins proc_sys_reset_0/dcm_locked]
set arstn [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
connect_bd_net $arstn [get_bd_pins smartconnect_0/aresetn]
connect_bd_net $arstn [get_bd_pins jtag_axi_0/aresetn]
connect_bd_net $arstn [get_bd_pins rasterizer_0/ap_rst_n]
connect_bd_net $arstn [get_bd_pins mig_7series_0/aresetn]

make_bd_intf_pins_external [get_bd_intf_pins mig_7series_0/DDR3]

create_bd_port -dir I -type clk -freq_hz 50000000 sys_clk
connect_bd_net [get_bd_ports sys_clk] [get_bd_pins clk_wiz_0/clk_in1]

# proc_sys_reset ext_reset_in defaults active-low — matches sys_rst_n.
# ACTIVE_LOW renames clk_wiz's reset pin to resetn.
set_property CONFIG.RESET_TYPE {ACTIVE_LOW} [get_bd_cells clk_wiz_0]
create_bd_port -dir I -type rst sys_rst_n
connect_bd_net [get_bd_ports sys_rst_n] [get_bd_pins mig_7series_0/sys_rst]
connect_bd_net [get_bd_ports sys_rst_n] [get_bd_pins proc_sys_reset_0/ext_reset_in]
connect_bd_net [get_bd_ports sys_rst_n] [get_bd_pins clk_wiz_0/resetn]

# map DDR3 + control registers into the address space
assign_bd_address

# validate, save, wrap, add constraints, set top
validate_bd_design
save_bd_design
make_wrapper -files [get_files $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd] -top
add_files -norecurse $proj_dir/$proj_name.gen/sources_1/bd/$bd_name/hdl/${bd_name}_wrapper.v
add_files -fileset constrs_1 constraints/wukong_top.xdc
set_property top ${bd_name}_wrapper [current_fileset]
update_compile_order -fileset sources_1

puts "bd.tcl: block design built and validated."
