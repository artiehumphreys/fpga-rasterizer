REMOTE=workstation
REMOTE_DIR=~/fpga-rasterizer

VIVADO_SETTINGS=/tools/Xilinx/2025.2/Vivado/settings64.sh
VITIS_SETTINGS=/tools/Xilinx/2025.2/Vitis/settings64.sh

SOURCE_TOOLS=source $(VIVADO_SETTINGS) && source $(VITIS_SETTINGS)

setup:
	ssh $(REMOTE) 'echo "source $(VIVADO_SETTINGS)" >> ~/.bash_profile && echo "source $(VITIS_SETTINGS)" >> ~/.bash_profile'

sync:
	rsync -avz --exclude 'build/' ./ $(REMOTE):$(REMOTE_DIR)/

build: sync
	ssh $(REMOTE) '$(SOURCE_TOOLS) && cd $(REMOTE_DIR) && v++ -c --mode hls --config scripts/hls_config.cfg --work_dir build/'
