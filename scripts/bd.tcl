set part xc7a100t-fgg676-1
set proj_dir build_vivado
set proj_name rasterizer_fpga
set bd_name system

file delete -force $proj_dir
create_project $proj_name $proj_dir -part $part

set_property ip_repo_paths {rasterizer_config scanout_config} [current_project]
update_ip_catalog

source scripts/bd_vivado.tcl

make_wrapper -files [get_files $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd] -top
add_files -norecurse $proj_dir/$proj_name.gen/sources_1/bd/$bd_name/hdl/${bd_name}_wrapper.v
add_files -fileset constrs_1 constraints/wukong_top.xdc
set_property top ${bd_name}_wrapper [current_fileset]
update_compile_order -fileset sources_1

puts "bd.tcl: block design built and validated."
