REMOTE=workstation
REMOTE_DIR=~/fpga-rasterizer

CXX=c++
CXXFLAGS=-std=c++23 -Wall -Wextra -O2 -Isrc -Ihost
HOST_SRC=host/main.cpp src/rasterizer.cpp
HOST_BIN=build/rasterizer

VIVADO_SETTINGS=/tools/Xilinx/2025.2/Vivado/settings64.sh
VITIS_SETTINGS=/tools/Xilinx/2025.2/Vitis/settings64.sh

SOURCE_TOOLS=source $(VIVADO_SETTINGS) && source $(VITIS_SETTINGS)

native: $(HOST_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(HOST_SRC) -o $(HOST_BIN)

run: native
	./$(HOST_BIN)

setup:
	ssh $(REMOTE) 'echo "source $(VIVADO_SETTINGS)" >> ~/.bash_profile && echo "source $(VITIS_SETTINGS)" >> ~/.bash_profile'

sync:
	rsync -avz --exclude 'build/' ./ $(REMOTE):$(REMOTE_DIR)/

build: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && v++ -c --mode hls --config scripts/hls_config.cfg --work_dir build/'

csim: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && vitis-run --mode hls --csim --config scripts/hls_config.cfg --work_dir build/'

# C/RTL co-simulation: verifies generated RTL matches the C model.
cosim: build
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && vitis-run --mode hls --cosim --config scripts/hls_config.cfg --work_dir build/'

bd: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && vivado -mode batch -source scripts/bd.tcl'

bit: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && vivado -mode batch -source scripts/bit.tcl'

program: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && vivado -mode batch -source scripts/program.tcl'

firstlight: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && vivado -mode batch -source scripts/firstlight.tcl'

ilacap: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && vivado -mode batch -source scripts/ilacap.tcl'
