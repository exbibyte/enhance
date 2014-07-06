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
inc_en := ./src/enCode
inc_core := ./src/core

build_dir := ./build
lib:= -lGL -lGLU -lGLEW -lglut
libjemalloc := -ljemalloc

$(shell mkdir -p $(build_dir))

.PHONY: all

all: test_vec test_quat test_dualquat test_dualscalar test_mat test_enTable test_circularbuffer test_bufferpool test_slerp test_sclerp

test_vec:
	g++ -std=c++0x ./test/math/catch_vec.cpp $(src_math_folder)/Vec.cpp -I$(inc_catch) -I$(inc_math) $(lib) -o $(build_dir)/test_vec	

test_quat:
	g++ -std=c++0x ./test/math/catch_quat.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp -I$(inc_catch) -I$(inc_math) -o $(build_dir)/test_quat	

test_dualquat:
	g++ -std=c++0x ./test/math/catch_dualquat.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_math_folder)/DualScalar.cpp $(src_math_folder)/DualQuat.cpp -I$(inc_catch) -I$(inc_math) -o $(build_dir)/test_dualquat	

test_lex:
	g++ -std=c++0x ./test/file/testlex.cpp $(src_file_folder)/Lex.cpp -I$(inc_catch) -I$(inc_file) $(lib) -o $(build_dir)/test_lex	

md5demo:
	g++ -std=c++0x ./test/MD5Demo.cpp $(src_file_folder)/Lex.cpp $(src_file_folder)/MD5Model.cpp $(src_file_folder)/PPM.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_ui_folder)/Trackball.cpp -I$(inc_catch) -I$(inc_file) -I$(inc_math) -I$(inc_ui) $(lib) -o $(build_dir)/md5demo

test_slerp:
	g++ -std=c++0x ./test/math/test_slerp.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_ui_folder)/Trackball.cpp -I$(inc_file) -I$(inc_catch) -I$(inc_math) -I$(inc_ui) $(lib) -o $(build_dir)/test_slerp

test_dualscalar:
	g++ -std=c++0x ./test/math/catch_dualscalar.cpp $(src_math_folder)/DualScalar.cpp -I$(inc_catch) -I$(inc_math) -o $(build_dir)/test_dualscalar

test_sclerp:
	g++ -std=c++0x -g ./test/math/test_sclerp.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_math_folder)/DualScalar.cpp $(src_math_folder)/DualQuat.cpp $(src_ui_folder)/Trackball.cpp -I$(inc_file) -I$(inc_catch) -I$(inc_math) -I$(inc_ui) $(lib) -o $(build_dir)/test_sclerp

test_mat:
	g++ -std=c++0x ./test/math/catch_mat.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Mat.cpp -I$(inc_catch) -I$(inc_math) -o $(build_dir)/test_mat

test_ik:
	g++ -std=c++0x ./test/test_ik.cpp $(src_math_folder)/Vec.cpp $(src_math_folder)/Quat.cpp $(src_math_folder)/DualScalar.cpp $(src_math_folder)/DualQuat.cpp $(src_ui_folder)/Trackball.cpp -I$(inc_catch) -I$(inc_file) -I$(inc_math) -I$(inc_ui) $(lib) -o $(build_dir)/test_ik

test_enTable:
	g++ -std=c++0x -g ./test/enCode/test_enTable.cpp -I$(inc_catch) -I$(inc_en) -o $(build_dir)/test_enTable

test_std_thread:
	g++ -std=c++0x ./test/other/test_std_thread.cpp -pthread -o $(build_dir)/test_std_thread

test_circularbuffer:
	g++ -std=c++0x ./test/core/test_CircularBuffer.cpp -I$(inc_catch) -I$(inc_core) -o $(build_dir)/test_circularbuffer

test_circularbuffer_threadsafe:
	g++ -std=c++0x ./test/core/test_CircularBuffer_threadsafe.cpp -I$(inc_catch) -I$(inc_core) -o $(build_dir)/test_circularbuffer_threadsafe

test_bufferpool:
	g++ -std=c++0x ./test/core/test_BufferPool.cpp -I$(inc_catch) -I$(inc_core) $(libjemalloc) -o $(build_dir)/test_bufferpool

test_enThread:
	g++ -std=c++0x -O3 ./test/enCode/test_enThread.cpp -pthread -I$(inc_core) -I$(inc_en) -o $(build_dir)/test_enthread

test_enThreadPool:
	g++ -std=c++11 -O3 ./test/enCode/test_enThreadPool.cpp -pthread -I$(inc_core) -I$(inc_en) -o $(build_dir)/test_enthreadpool

test_Octree:
	g++ -std=c++0x -O3 ./test/math/test_Octree.cpp -I$(inc_catch) -I$(inc_math) -o $(build_dir)/test_octree

test_ThreadPool:
	g++ -std=c++11 -O3 ./test/core/test_ThreadPool.cpp -pthread -I$(inc_core) -o $(build_dir)/test_threadpool
