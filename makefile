src_files := $(wildcard ./src/*.cpp)
src_math_folder := ./src/math
src_file_folder := ./src/file
src_ui_folder := ./src/ui

inc_math := ./src/math
inc_file := ./src/file
inc_ui := ./src/ui

build_dir := ./build
lib:= -lGL -lGLU -lGLEW -lglut

$(shell mkdir -p $(build_dir))


test_vec:
	g++ -std=c++0x ./test/catch_vec.cpp $(src_math_folder)/Vec.cpp -I$(inc_math) $(lib) -o $(build_dir)/test_vec	

test_quat:
	g++ -std=c++0x ./test/catch_quat.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp -I$(inc_math) -o $(build_dir)/test_quat	

test_dualquat:
	g++ -std=c++0x ./test/catch_dualquat.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_math_folder)/DualScalar.cpp $(src_math_folder)/DualQuat.cpp -I$(inc_math) -o $(build_dir)/test_dualquat	

test_lex:
	g++ -std=c++0x ./test/testlex.cpp $(src_file_folder)/Lex.cpp -I$(inc_file) $(lib) -o $(build_dir)/test_lex	

md5demo:
	g++ -std=c++0x ./test/MD5Demo.cpp $(src_file_folder)/Lex.cpp $(src_file_folder)/MD5Model.cpp $(src_file_folder)/PPM.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_ui_folder)/Trackball.cpp -I$(inc_file) -I$(inc_math) -I$(inc_ui) $(lib) -o $(build_dir)/md5demo

test_slerp:
	g++ -std=c++0x ./test/test_slerp.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_ui_folder)/Trackball.cpp -I$(inc_file) -I$(inc_math) -I$(inc_ui) $(lib) -o $(build_dir)/test_slerp

test_dualscalar:
	g++ -std=c++0x ./test/catch_dualscalar.cpp $(src_math_folder)/DualScalar.cpp -I$(inc_math) -o $(build_dir)/test_dualscalar

test_sclerp:
	g++ -std=c++0x ./test/test_sclerp.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_math_folder)/DualScalar.cpp $(src_math_folder)/DualQuat.cpp $(src_ui_folder)/Trackball.cpp -I$(inc_file) -I$(inc_math) -I$(inc_ui) $(lib) -o $(build_dir)/test_sclerp

test_mat:
	g++ -std=c++0x ./test/catch_mat.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Mat.cpp -I$(inc_math) -o $(build_dir)/test_mat

