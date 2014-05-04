src_files := $(wildcard ./src/*.cpp)
inc_dir := ./src
build_dir := ./build
lib:=

$(shell mkdir -p $(build_dir))


g++ -std=c++0x $(src_files) -I$(inc_dir) $(lib) -o $(build_dir)/	
