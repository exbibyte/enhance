src_files := $(wildcard ./src/*.cpp)
src_math_folder := ./src/math
src_file_folder := ./src/file
src_ui_folder := ./src/ui
src_en_folder := ./src/enCode
src_core_folder := ./src/core

inc_math := ./src/math
inc_file := ./src/file
inc_ui := ./src/ui
inc_catch := ./test
build_dir := ./build

$(shell mkdir -p $(build_dir))

.PHONY: All

All: test_TransMatrix	

test_TransMatrix:
	g++ -std=c++11 -g -O3 -pthread ./Transition/TestTransMatrix.cpp -ITransition -I. -o $(build_dir)/test_TransMatrix