# QMTECH Wukong XC7A100T top-level pins.
# DDR3 pin constraints are emitted by the MIG IP (from boards/wukong/mig.prj),
# so only the board oscillator and reset button live here.
# Pins from ChinaQMTECH/QM_XC7A100T_WUKONG_BOARD examples.

# 50 MHz board oscillator
set_property -dict {PACKAGE_PIN M21 IOSTANDARD LVCMOS33} [get_ports sys_clk]
create_clock -name sys_clk -period 20.000 [get_ports sys_clk]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets -of_objects [get_ports sys_clk]]

# Active-low reset push-button
set_property -dict {PACKAGE_PIN H7 IOSTANDARD LVCMOS33} [get_ports sys_rst_n]
