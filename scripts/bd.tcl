set part xc7a100t-fgg676-1
set proj_dir build_vivado
set proj_name rasterizer_fpga
set bd_name system

file delete -force $proj_dir
create_project $proj_name $proj_dir -part $part

set_property ip_repo_paths {rasterizer_config scanout_config} [current_project]
update_ip_catalog

# TMDS output RTL (module-referenced by the block design as hdmi_tmds)
add_files -norecurse {
  rtl/tmds_out.vhd
  external/vhdl-hdmi-out/rtl/rgb2tmds.vhd
  external/vhdl-hdmi-out/rtl/tmds_encoder.vhd
  external/vhdl-hdmi-out/rtl/serializer.vhd
}
update_compile_order -fileset sources_1

source scripts/bd_vivado.tcl

# ensure 720p
set_property -dict [list \
  CONFIG.enable_generation {true} \
  CONFIG.GEN_VIDEO_FORMAT {RGB} \
  CONFIG.GEN_HACTIVE_SIZE {1280} \
  CONFIG.GEN_HFRAME_SIZE {1650} \
  CONFIG.GEN_HSYNC_START {1390} \
  CONFIG.GEN_HSYNC_END {1430} \
  CONFIG.GEN_HSYNC_POLARITY {High} \
  CONFIG.GEN_VACTIVE_SIZE {720} \
  CONFIG.GEN_F0_VFRAME_SIZE {750} \
  CONFIG.GEN_F0_VSYNC_VSTART {724} \
  CONFIG.GEN_F0_VSYNC_VEND {729} \
  CONFIG.GEN_VSYNC_POLARITY {High} \
] [get_bd_cells v_tc_0]
validate_bd_design
save_bd_design

make_wrapper -files [get_files $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd] -top
add_files -norecurse $proj_dir/$proj_name.gen/sources_1/bd/$bd_name/hdl/${bd_name}_wrapper.v
add_files -fileset constrs_1 constraints/wukong_top.xdc
set_property top ${bd_name}_wrapper [current_fileset]
update_compile_order -fileset sources_1

puts "bd.tcl: block design built and validated."
