REMOTE=workstation
REMOTE_DIR=~/fpga-rasterizer

CXX=c++
CXXFLAGS=-std=c++23 -Wall -Wextra -O2 -Isrc -Ihost
HOST_SRC=host/main.cpp src/rasterizer.cpp
HOST_BIN=build/rasterizer

VIVADO_SETTINGS=/tools/Xilinx/2025.2/Vivado/settings64.sh
VITIS_SETTINGS=/tools/Xilinx/2025.2/Vitis/settings64.sh

SOURCE_TOOLS=source $(VIVADO_SETTINGS) && source $(VITIS_SETTINGS)

REMOTE_RUN=ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && $(1)'

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
	$(call REMOTE_RUN,v++ -c --mode hls --config scripts/hls_config.cfg --work_dir build/)

csim: sync
	$(call REMOTE_RUN,vitis-run --mode hls --csim --config scripts/hls_config.cfg --work_dir build/)

# C/RTL co-simulation: verifies generated RTL matches the C model.
cosim: build
	$(call REMOTE_RUN,vitis-run --mode hls --cosim --config scripts/hls_config.cfg --work_dir build/)

bd: sync
	$(call REMOTE_RUN,vivado -mode batch -source scripts/bd.tcl)

bit: sync
	$(call REMOTE_RUN,vivado -mode batch -source scripts/bit.tcl)

program: sync
	$(call REMOTE_RUN,vivado -mode batch -source scripts/program.tcl)

firstlight: sync
	$(call REMOTE_RUN,vivado -mode batch -source scripts/firstlight.tcl)

fullframe: sync
	$(call REMOTE_RUN,vivado -mode batch -source scripts/fullframe.tcl)
	scp $(REMOTE):$(REMOTE_DIR)/hw_fb.hex ./hw_fb.hex

verify: run fullframe
	diff -q golden_fb.hex hw_fb.hex && echo "hardware matches golden reference"

ilacap: sync
	$(call REMOTE_RUN,vivado -mode batch -source scripts/ilacap.tcl)
