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
libsfml := -lsfml-graphics -lsfml-window -lsfml-system

$(shell mkdir -p $(build_dir))

.PHONY: all

all: test_vec test_quat test_dualquat test_dualscalar test_mat test_enTable test_CircularBuffer test_BufferPool test_slerp test_sclerp

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

test_CircularBuffer:
	g++ -std=c++0x ./test/core/test_CircularBuffer.cpp -I$(inc_catch) -I$(inc_core) -o $(build_dir)/test_circularbuffer

test_CircularBufferThreadSafe:
	g++ -std=c++0x ./test/core/test_CircularBufferThreadSafe.cpp -I$(inc_catch) -I$(inc_core) -o $(build_dir)/test_circularbufferthreadsafe

test_BufferPool:
	g++ -std=c++0x ./test/core/test_BufferPool.cpp -I$(inc_catch) -I$(inc_core) $(libjemalloc) -o $(build_dir)/test_bufferpool

test_Thread:
	g++ -std=c++0x -O3 ./test/core/test_Thread.cpp -pthread -I$(inc_core) -o $(build_dir)/test_thread

test_Octree:
	g++ -std=c++0x -O3 ./test/math/test_Octree.cpp -I$(inc_catch) -I$(inc_math) -o $(build_dir)/test_octree

test_ThreadPool:
	g++ -std=c++11 -O0 -g -Wall ./test/core/test_ThreadPool.cpp -pthread -I$(inc_core) -I$(inc_catch) -o $(build_dir)/test_threadpool

test_Sfml:
	g++ ./test/other/test_sfml.cpp $(libsfml) -o $(build_dir)/test_sfml

test_ConfigNode:
	g++ -std=c++11 -g ./test/core/test_cconfignode.cpp $(src_core_folder)/ConfigNode.cpp -I$(inc_core) -o $(build_dir)/test_cconfignode

test_Clock:
	g++ -std=c++11 -g ./test/core/test_Clock.cpp $(src_core_folder)/Clock.cpp -I$(inc_core) -I$(inc_catch) -o $(build_dir)/test_clock

test_ThreadPoolCircularBuffer:
	g++ -std=c++11 -g ./test/core/test_ThreadPoolCircularBuffer.cpp -pthread -I$(inc_core) -I$(inc_catch) -o $(build_dir)/test_threadpoolcircularbuffer

test_enThreadPool:
	g++ -std=c++11 -g ./test/enCode/test_enThreadPool.cpp -pthread -I$(inc_core) -I$(inc_en) -I$(inc_catch) -o $(build_dir)/test_enthreadpool

test_enTPCommon:
	g++ -std=c++11 -g ./test/enCode/test_enTPCommon.cpp -pthread -I$(inc_core) -I$(inc_en) -I$(inc_catch) -o $(build_dir)/test_entpcommon
