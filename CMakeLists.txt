cmake_minimum_required(VERSION 3.30)
project(fpga_rasterizer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

add_library(core INTERFACE)
target_include_directories(core INTERFACE src)

add_executable(rasterizer host/main.cpp src/rasterizer.cpp)
target_include_directories(rasterizer PRIVATE host)
target_link_libraries(rasterizer PRIVATE core)

enable_testing()
add_executable(tests tests/host.cpp src/rasterizer.cpp)
target_link_libraries(tests PRIVATE core)
