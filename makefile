src_files := $(wildcard ./src/*.cpp)
src_folder_math := ./src/math
src_folder_file := ./src/file
src_folder_ui := ./src/ui
src_folder_en := ./src/enCode
src_folder_core := ./src/core
src_folder_transition := ./src/transition
src_folder_graph := ./src/graph
src_folder_polymesh := ./src/polymesh

inc_folder_math := ./src/math
inc_folder_file := ./src/file
inc_folder_ui := ./src/ui
inc_folder_catch := ./test
inc_folder_en := ./src/enCode
inc_folder_core := ./src/core
inc_folder_transition := ./src/transition
inc_folder_graph := ./src/graph
inc_folder_polymesh := ./src/polymesh

build_dir := ./build
lib:= -L/usr/lib/nvidia-340 -lGL -lGLU -lGLEW -lglut
libjemalloc := -ljemalloc
libsfml := -lsfml-graphics -lsfml-window -lsfml-system

$(shell mkdir -p $(build_dir))

.PHONY: all

all: test_vec test_quat test_dualquat test_dualscalar test_Mat test_enTable test_CircularBuffer test_BufferPool test_slerp test_sclerp

test_vec:
	g++ -std=c++0x ./test/math/catch_vec.cpp $(src_folder_math)/Vec.cpp -I$(inc_folder_catch) -I$(inc_folder_math) $(lib) -o $(build_dir)/test_vec	

test_quat:
	g++ -std=c++0x ./test/math/catch_quat.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/Quat.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -o $(build_dir)/test_quat	

test_dualquat:
	g++ -std=c++0x ./test/math/catch_dualquat.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/Quat.cpp $(src_folder_math)/DualScalar.cpp $(src_folder_math)/DualQuat.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -o $(build_dir)/test_dualquat	

test_lex:
	g++ -std=c++0x ./test/file/testlex.cpp $(src_folder_file)/Lex.cpp -I$(inc_folder_catch) -I$(inc_folder_file) $(lib) -o $(build_dir)/test_lex	

md5demo:
	g++ -std=c++0x ./test/MD5Demo.cpp $(src_folder_file)/Lex.cpp $(src_folder_file)/MD5Model.cpp $(src_folder_file)/PPM.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/Quat.cpp $(src_folder_ui)/Trackball.cpp -I$(inc_folder_catch) -I$(inc_folder_file) -I$(inc_folder_math) -I$(inc_folder_ui) $(lib) -o $(build_dir)/md5demo

test_slerp:
	g++ -std=c++0x ./test/math/test_slerp.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/Quat.cpp $(src_folder_ui)/Trackball.cpp -I$(inc_folder_file) -I$(inc_folder_catch) -I$(inc_folder_math) -I$(inc_folder_ui) $(lib) -o $(build_dir)/test_slerp

test_dualscalar:
	g++ -std=c++0x ./test/math/catch_dualscalar.cpp $(src_folder_math)/DualScalar.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -o $(build_dir)/test_dualscalar

test_sclerp:
	g++ -std=c++0x -g ./test/math/test_sclerp.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/Quat.cpp $(src_folder_math)/DualScalar.cpp $(src_folder_math)/DualQuat.cpp $(src_folder_ui)/Trackball.cpp -I$(inc_folder_file) -I$(inc_folder_catch) -I$(inc_folder_math) -I$(inc_folder_ui) $(lib) -o $(build_dir)/test_sclerp

test_Mat:
	g++ -std=c++0x ./test/math/catch_mat.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/Mat.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -o $(build_dir)/test_Mat

test_ik:
	g++ -std=c++0x ./test/test_ik.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/Quat.cpp $(src_folder_math)/DualScalar.cpp $(src_folder_math)/DualQuat.cpp $(src_folder_ui)/Trackball.cpp -I$(inc_folder_catch) -I$(inc_folder_file) -I$(inc_folder_math) -I$(inc_folder_ui) $(lib) -o $(build_dir)/test_ik

test_enTable:
	g++ -std=c++0x -g ./test/enCode/test_enTable.cpp -I$(inc_folder_catch) -I$(inc_folder_en) -o $(build_dir)/test_enTable

test_std_thread:
	g++ -std=c++0x ./test/other/test_std_thread.cpp -pthread -o $(build_dir)/test_std_thread

test_CircularBuffer:
	g++ -std=c++0x ./test/core/test_CircularBuffer.cpp -I$(inc_folder_catch) -I$(inc_folder_core) -o $(build_dir)/test_circularbuffer

test_CircularBufferThreadSafe:
	g++ -std=c++0x ./test/core/test_CircularBufferThreadSafe.cpp -I$(inc_folder_catch) -I$(inc_folder_core) -o $(build_dir)/test_circularbufferthreadsafe

test_BufferPool:
	g++ -std=c++0x ./test/core/test_BufferPool.cpp -I$(inc_folder_catch) -I$(inc_folder_core) $(libjemalloc) -o $(build_dir)/test_bufferpool

test_Thread:
	g++ -std=c++0x -O3 ./test/core/test_Thread.cpp -pthread -I$(inc_folder_core) -o $(build_dir)/test_thread

test_Octree:
	g++ -std=c++0x -O3 ./test/math/test_Octree.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -o $(build_dir)/test_octree

test_ThreadPool:
	g++ -std=c++11 -O0 -g -Wall ./test/core/test_ThreadPool.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_catch) -o $(build_dir)/test_threadpool

test_Sfml:
	g++ ./test/other/test_sfml.cpp $(libsfml) -o $(build_dir)/test_sfml

test_ConfigNode:
	g++ -std=c++11 -g ./test/core/test_cconfignode.cpp $(src_folder_core)/ConfigNode.cpp -I$(inc_folder_core) -o $(build_dir)/test_cconfignode

test_Clock:
	g++ -std=c++11 -g ./test/core/test_Clock.cpp $(src_folder_core)/Clock.cpp -I$(inc_folder_core) -I$(inc_folder_catch) -o $(build_dir)/test_clock

test_ThreadPoolCircularBuffer:
	g++ -std=c++11 -g ./test/core/test_ThreadPoolCircularBuffer.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_catch) -o $(build_dir)/test_threadpoolcircularbuffer

test_enThreadPool:
	g++ -std=c++11 -g ./test/enCode/test_enThreadPool.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_en) -I$(inc_folder_catch) -o $(build_dir)/test_enthreadpool

test_enTPCommon:
	g++ -std=c++11 -g ./test/enCode/test_enTPCommon.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_en) -I$(inc_folder_catch) -o $(build_dir)/test_entpcommon

test_TransMatrix:
	g++ -std=c++11 -g -O3 -pthread $(src_folder_transition)/TestTransMatrix.cpp -I$(inc_folder_transition) -I$(inc_folder_catch) -o $(build_dir)/test_TransMatrix

test_PolyMesh:
	g++ -std=c++11 -g -O0 ./test/polymesh/test_PolyMesh.cpp -pthread $(src_folder_math)/Vec.cpp $(src_folder_polymesh)/PolyMesh.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -I$(inc_folder_polymesh) -I$(inc_folder_transition) -o $(build_dir)/test_PolyMesh

test_GraphDirected:
	g++ -std=c++11 -g -O0 ./test/graph/TestGraphDirected.cpp -pthread -I$(inc_folder_catch) -I$(inc_folder_graph) -o $(build_dir)/test_GraphDirected