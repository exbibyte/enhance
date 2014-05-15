src_files := $(wildcard ./src/*.cpp)
src_math_folder := ./src/math
src_file_folder := ./src/file

inc_math := ./src/math
inc_file := ./src/file

build_dir := ./build
lib:=

$(shell mkdir -p $(build_dir))


test_math:
	g++ -std=c++0x ./test/catch_math.cpp $(src_math_folder)/Vec.cpp -I$(inc_math) $(lib) -o $(build_dir)/test_math	

test_quat:
	g++ -std=c++0x ./test/catch_quat.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp -I$(inc_math) $(lib) -o $(build_dir)/test_quat	

test_lex:
	g++ -std=c++0x ./test/testlex.cpp $(src_file_folder)/Lex.cpp -I$(inc_file) $(lib) -o $(build_dir)/test_lex	
