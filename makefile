src_files := $(wildcard ./src/*.cpp)
src_math := ./src/math/Vec.cpp
inc_math := ./src/math
inc_dir := ./src ./test
build_dir := ./build
lib:=

$(shell mkdir -p $(build_dir))


test_math:
	g++ -std=c++0x ./test/catch_math.cpp $(src_math) -I$(inc_math) $(lib) -o $(build_dir)/test_math	
