src_files := $(wildcard ./src/*.cpp)
src_folder_math := ./src/math
src_folder_file := ./src/file
src_folder_ui := ./src/ui
src_folder_en := ./src/enCode
src_folder_core := ./src/core
src_folder_transition := ./src/transition
src_folder_graph := ./src/graph
src_folder_polymesh := ./src/polymesh
src_folder_datatransform := ./src/DataTransform
src_folder_datatransform_pass := ./src/DataTransform/Pass
src_folder_gl := ./src/gl

src_folder_test_file := ./test/file
src_folder_test_datatransform := ./test/DataTransform
src_folder_test_gl := ./test/gl
src_folder_test_core := ./test/core
src_folder_test_transition := ./test/transition

inc_folder_math := ./src/math
inc_folder_file := ./src/file
inc_folder_ui := ./src/ui
inc_folder_catch := ./test
inc_folder_en := ./src/enCode
inc_folder_core := ./src/core
inc_folder_transition := ./src/transition
inc_folder_graph := ./src/graph
inc_folder_polymesh := ./src/polymesh
inc_folder_datatransform := ./src/DataTransform
inc_folder_datatransform_pass := ./src/DataTransform/Pass
inc_folder_gl := ./src/gl

build_dir := ./build
lib:= -L/usr/lib/nvidia-340 -lGL -lGLU -lGLEW -lglut
libjemalloc := -ljemalloc
libsfml := -lsfml-graphics -lsfml-window -lsfml-system

$(shell mkdir -p $(build_dir))

.PHONY: all

all: test_vec test_quat test_dualquat test_dualscalar test_Mat test_enTable test_CircularBuffer test_BufferPool test_slerp test_sclerp test_ThreadPoolCircularBuffer test_enThreadPool test_enTPCommon test_TransMatrix test_PolyMesh test_GraphDirected test_WingedEdge test_Trie test_Obj2PolyMesh test_StreamChannel test_StreamManager test_StreamInterface

parsing:
	test_ParsePolyMesh test_DataTransformPolyMesh_osx

graphics_mac:
	test_GLSceneManager_osx
	test_GLSceneManagerV2_osx	

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
	g++ -std=c++0x ./test/core/test_BufferPool.cpp -I$(inc_folder_catch) -I$(inc_folder_core) -o $(build_dir)/test_bufferpool

test_Thread:
	g++ -std=c++0x -O3 ./test/core/test_Thread.cpp -pthread -I$(inc_folder_core) -o $(build_dir)/test_thread

test_Octree:
	g++ -std=c++0x -O3 ./test/math/test_Octree.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -o $(build_dir)/test_octree

test_ThreadPool:
	g++ -std=c++0x -O0 -g -Wall ./test/core/test_ThreadPool.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_catch) -o $(build_dir)/test_threadpool

test_Sfml:
	g++ -std=c++0x -g ./test/other/test_sfml.cpp $(libsfml) -o $(build_dir)/test_sfml

test_ConfigNode:
	g++ -std=c++0x -g ./test/core/test_cconfignode.cpp $(src_folder_core)/ConfigNode.cpp -I$(inc_folder_core) -o $(build_dir)/test_cconfignode

test_Clock:
	g++ -std=c++0x -g ./test/core/test_Clock.cpp $(src_folder_core)/Clock.cpp -I$(inc_folder_core) -I$(inc_folder_catch) -o $(build_dir)/test_clock

test_ThreadPoolCircularBuffer:
	g++ -std=c++0x -g ./test/core/test_ThreadPoolCircularBuffer.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_catch) -o $(build_dir)/test_threadpoolcircularbuffer

test_enThreadPool:
	g++ -std=c++0x -g ./test/enCode/test_enThreadPool.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_en) -I$(inc_folder_catch) -o $(build_dir)/test_enthreadpool

test_enTPCommon:
	g++ -std=c++0x -g ./test/enCode/test_enTPCommon.cpp -pthread -I$(inc_folder_core) -I$(inc_folder_en) -I$(inc_folder_catch) -o $(build_dir)/test_entpcommon

test_TransMatrix:
	g++ -std=c++0x -g -O0 -pthread $(src_folder_test_transition)/TestTransMatrix.cpp -I$(inc_folder_transition) -I$(inc_folder_catch) -o $(build_dir)/test_TransMatrix

test_PolyMesh:
	g++ -std=c++0x -g -O0 ./test/polymesh/test_PolyMesh.cpp -pthread $(src_folder_math)/Vec.cpp $(src_folder_polymesh)/PolyMesh.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -I$(inc_folder_polymesh) -I$(inc_folder_transition) -o $(build_dir)/test_PolyMesh

test_PolyMeshInterface:
	g++ -std=c++0x -g -O0 ./test/polymesh/test_PolyMeshInterface.cpp -pthread $(src_folder_math)/Vec.cpp $(src_folder_polymesh)/PolyMeshInterface.cpp $(src_folder_polymesh)/PolyMeshGraph.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -I$(inc_folder_polymesh) -o $(build_dir)/test_PolyMeshInterface

test_GraphDirected:
	g++ -std=c++0x -g -O0 ./test/graph/TestGraphDirected.cpp -pthread -I$(inc_folder_catch) -I$(inc_folder_graph) -o $(build_dir)/test_GraphDirected

test_WingedEdge:
	g++ -std=c++0x -g -O0 ./test/polymesh/test_WingedEdge.cpp -pthread $(src_folder_math)/Vec.cpp $(src_folder_polymesh)/WingedEdge.cpp -I$(inc_folder_catch) -I$(inc_folder_math) -I$(inc_folder_polymesh) -o $(build_dir)/test_WingedEdge

test_WindowManager:
	g++ -std=c++0x -g -DGLFW_INCLUDE_GLCOREARB ./test/ui/test_WindowManager.cpp -pthread $(src_folder_ui)/WindowManagerGlfw.cpp -I/usr/local/include -I$(inc_folder_ui) -I$(inc_folder_math) -I$(inc_folder_polymesh) -I$(inc_folder_graph) -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework Core\
Video -L/usr/local/lib -o $(build_dir)/test_WindowManager

test_Trie:
	g++ -std=c++0x -g -O0 ./test/graph/test_Trie.cpp -pthread -I$(inc_folder_catch) -I$(inc_folder_graph) -o $(build_dir)/test_Trie

test_ParsePolyMesh_osx:
	bison -d $(src_folder_file)/testyac_PolyMesh.y -o $(src_folder_file)/testyac_PolyMesh.tab.c
	flex -o $(src_folder_file)/testlex_PolyMesh.yy.c $(src_folder_file)/testlex_PolyMesh.l
	g++ -std=c++0x $(src_folder_file)/testyac_PolyMesh.tab.c $(src_folder_file)/testlex_PolyMesh.yy.c $(src_folder_test_file)/test_ParsePolyMesh.cpp -I$(inc_folder_file) -ll -o $(build_dir)/test_ParsePolyMesh

test_ParsePolyMesh:
	bison -d $(src_folder_file)/testyac_PolyMesh.y -o $(src_folder_file)/testyac_PolyMesh.tab.c
	flex -o $(src_folder_file)/testlex_PolyMesh.yy.c $(src_folder_file)/testlex_PolyMesh.l
	g++ -std=c++0x $(src_folder_file)/testyac_PolyMesh.tab.c $(src_folder_file)/testlex_PolyMesh.yy.c $(src_folder_test_file)/test_ParsePolyMesh.cpp -I$(inc_folder_file) -lfl -o $(build_dir)/test_ParsePolyMesh

test_Filter_ParseNode_osx:
	bison -d $(src_folder_file)/testyac_PolyMesh.y -o $(src_folder_file)/testyac_PolyMesh.tab.c
	flex -o $(src_folder_file)/testlex_PolyMesh.yy.c $(src_folder_file)/testlex_PolyMesh.l
	g++ -std=c++0x -g $(src_folder_file)/testyac_PolyMesh.tab.c $(src_folder_file)/testlex_PolyMesh.yy.c $(src_folder_test_file)/test_Filter_ParseNode.cpp $(src_folder_file)/Filter_ParseNode.cpp $(src_folder_file)/Filter_ParsePolyMesh.cpp -I$(inc_folder_file) -ll -o $(build_dir)/test_Filter_ParseNode

test_Filter_ParseNode_v2_osx:
	bison -d $(src_folder_file)/bison_PolyMesh.y -o $(src_folder_file)/bison_PolyMesh.tab.c
	flex --header-file=$(src_folder_file)/Flex_PolyMesh.h -o $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/flex_PolyMesh.l
	g++ -std=c++0x -g $(src_folder_file)/bison_PolyMesh.tab.c $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_test_file)/test_Filter_ParseNode_v2.cpp $(src_folder_file)/Filter_ParseNode.cpp $(src_folder_file)/Filter_ParsePolyMesh.cpp -I$(inc_folder_file) -ll -o $(build_dir)/test_Filter_ParseNode_v2

test_DataTransformPolyMesh_osx:
	bison -d $(src_folder_file)/bison_PolyMesh.y -o $(src_folder_file)/bison_PolyMesh.tab.c
	flex --header-file=$(src_folder_file)/Flex_PolyMesh.h -o $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/flex_PolyMesh.l
	g++ -std=c++0x -g $(src_folder_file)/bison_PolyMesh.tab.c $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/Filter_ParseNode.cpp $(src_folder_file)/Filter_ParsePolyMesh.cpp $(src_folder_datatransform)/DataTransformPass.cpp $(src_folder_datatransform)/DataTransformMetaInfo.cpp $(src_folder_datatransform)/DataTransformMetaInfoCombiner.cpp $(src_folder_datatransform)/DataTransformDriver.cpp $(src_folder_datatransform_pass)/PassParsePolyMesh.cpp $(src_folder_test_datatransform)/test_DataTransformPolyMesh.cpp -I$(inc_folder_file) -I$(inc_folder_datatransform) -I$(inc_folder_datatransform_pass) -ll -o $(build_dir)/test_DataTransformPolyMesh

test_DataTransformPolyMeshArray_osx:
	bison -d $(src_folder_file)/bison_PolyMesh.y -o $(src_folder_file)/bison_PolyMesh.tab.c
	flex --header-file=$(src_folder_file)/Flex_PolyMesh.h -o $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/flex_PolyMesh.l
	g++ -std=c++0x -g $(src_folder_file)/bison_PolyMesh.tab.c $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/Filter_ParseNode.cpp $(src_folder_file)/Filter_ParsePolyMesh.cpp $(src_folder_file)/PolyMesh_Data_Arrays.cpp $(src_folder_datatransform)/DataTransformPass.cpp $(src_folder_datatransform)/DataTransformMetaInfo.cpp $(src_folder_datatransform)/DataTransformMetaInfoCombiner.cpp $(src_folder_datatransform)/DataTransformDriver.cpp $(src_folder_datatransform_pass)/PassConvertPolyMeshDataStructToArray.cpp $(src_folder_datatransform_pass)/PassParsePolyMesh.cpp $(src_folder_test_datatransform)/test_DataTransformPolyMeshArray.cpp -I$(inc_folder_gl) -I$(inc_folder_file) -I$(inc_folder_datatransform) -I$(inc_folder_datatransform_pass) -ll -o $(build_dir)/test_DataTransformPolyMeshArray

test_GLSceneManager_osx:
	g++ -g -DGLFW_INCLUDE_GLCOREARB -std=c++0x $(src_folder_gl)/GLSceneManager.cpp $(src_folder_ui)/WindowManagerGlfw.cpp $(src_folder_gl)/GLSLProgram.cpp $(src_folder_gl)/GLHelper.cpp $(src_folder_gl)/GLTexture.cpp $(src_folder_file)/textfile.cpp $(src_folder_file)/PPM.cpp $(src_folder_polymesh)/WingedEdge.cpp $(src_folder_math)/Vec.cpp $(src_folder_test_gl)/test_GLSceneManager.cpp -I/usr/local/include -I$(inc_folder_file) -I$(inc_folder_gl) -I$(inc_folder_ui) -I$(inc_folder_graph) -I$(inc_folder_polymesh) -I$(inc_folder_math) -I$(inc_folder_en) -I$(inc_folder_core) -pthread -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -L/usr/local/lib -o $(build_dir)/test_GLSceneManager

test_GLSceneManagerV2_osx:
	bison -d $(src_folder_file)/bison_PolyMesh.y -o $(src_folder_file)/bison_PolyMesh.tab.c
	flex --header-file=$(src_folder_file)/Flex_PolyMesh.h -o $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/flex_PolyMesh.l
	g++ -g -DGLFW_INCLUDE_GLCOREARB -std=c++0x $(src_folder_file)/bison_PolyMesh.tab.c $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/Filter_ParseNode.cpp $(src_folder_file)/Filter_ParsePolyMesh.cpp $(src_folder_file)/PolyMesh_Data_Arrays.cpp $(src_folder_datatransform)/DataTransformPass.cpp $(src_folder_datatransform)/DataTransformMetaInfo.cpp $(src_folder_datatransform)/DataTransformMetaInfoCombiner.cpp $(src_folder_datatransform)/DataTransformDriver.cpp $(src_folder_datatransform_pass)/PassConvertPolyMeshDataStructToArray.cpp $(src_folder_datatransform_pass)/PassParsePolyMesh.cpp $(src_folder_gl)/GLSceneManager.cpp $(src_folder_ui)/WindowManagerGlfw.cpp $(src_folder_gl)/GLSLProgram.cpp $(src_folder_gl)/GLHelper.cpp $(src_folder_gl)/GLTexture.cpp $(src_folder_file)/textfile.cpp $(src_folder_file)/PPM.cpp $(src_folder_polymesh)/WingedEdge.cpp $(src_folder_math)/Vec.cpp $(src_folder_core)/Clock.cpp $(src_folder_test_gl)/test_GLSceneManagerV2.cpp -I/usr/local/include -I$(inc_folder_file) -I$(inc_folder_datatransform) -I$(inc_folder_datatransform_pass) -I$(inc_folder_gl) -I$(inc_folder_ui) -I$(inc_folder_graph) -I$(inc_folder_polymesh) -I$(inc_folder_math) -I$(inc_folder_en) -I$(inc_folder_core) -ll -pthread -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -L/usr/local/lib -o $(build_dir)/test_GLSceneManagerV2

test_GLSceneManagerV3_osx:
	bison -d $(src_folder_file)/bison_PolyMesh.y -o $(src_folder_file)/bison_PolyMesh.tab.c
	flex --header-file=$(src_folder_file)/Flex_PolyMesh.h -o $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/flex_PolyMesh.l
	g++ -g -DGLFW_INCLUDE_GLCOREARB -std=c++1y $(src_folder_file)/bison_PolyMesh.tab.c $(src_folder_file)/flex_PolyMesh.yy.c $(src_folder_file)/Filter_ParseNode.cpp $(src_folder_file)/Filter_ParsePolyMesh.cpp $(src_folder_file)/PolyMesh_Data_Arrays.cpp $(src_folder_datatransform)/DataTransformPass.cpp $(src_folder_datatransform)/DataTransformMetaInfo.cpp $(src_folder_datatransform)/DataTransformMetaInfoCombiner.cpp $(src_folder_datatransform)/DataTransformDriver.cpp $(src_folder_datatransform_pass)/PassConvertPolyMeshDataStructToArray.cpp $(src_folder_datatransform_pass)/PassParsePolyMesh.cpp $(src_folder_gl)/GLSceneManager.cpp $(src_folder_ui)/WindowManagerGlfw.cpp $(src_folder_gl)/GLSLProgram.cpp $(src_folder_gl)/GLHelper.cpp $(src_folder_gl)/GLTexture.cpp $(src_folder_gl)/GLRenderPassShadowMap.cpp $(src_folder_file)/textfile.cpp $(src_folder_file)/PPM.cpp $(src_folder_polymesh)/WingedEdge.cpp $(src_folder_math)/Vec.cpp $(src_folder_math)/RenderMeshOrientation.cpp $(src_folder_core)/Clock.cpp $(src_folder_test_gl)/test_GLSceneManagerV3.cpp -I/usr/local/include -I$(inc_folder_file) -I$(inc_folder_datatransform) -I$(inc_folder_datatransform_pass) -I$(inc_folder_gl) -I$(inc_folder_ui) -I$(inc_folder_graph) -I$(inc_folder_polymesh) -I$(inc_folder_math) -I$(inc_folder_en) -I$(inc_folder_core) -ll -pthread -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -L/usr/local/lib -o $(build_dir)/test_GLSceneManagerV3

test_Obj2PolyMesh:
	g++ -std=c++0x -g $(src_folder_file)/Obj2PolyMesh.cpp -o $(build_dir)/test_Obj2PolyMesh

test_StreamChannel:
	$(CXX) -std=c++11 -g $(src_folder_core)/StreamChannel.cpp $(src_folder_test_core)/test_StreamChannel.cpp -I$(inc_folder_catch) -I$(inc_folder_core) -o $(build_dir)/test_StreamChannel

test_StreamManager:
	$(CXX) -std=c++11 -g $(src_folder_core)/StreamManager.cpp $(src_folder_core)/StreamChannel.cpp $(src_folder_test_core)/test_StreamManager.cpp -I$(inc_folder_catch) -I$(inc_folder_core) -o $(build_dir)/test_StreamManager

test_StreamInterface:
	$(CXX) -std=c++11 -g $(src_folder_core)/StreamManager.cpp $(src_folder_core)/StreamChannel.cpp $(src_folder_core)/StreamInterface.cpp $(src_folder_test_core)/test_StreamInterface.cpp -I$(inc_folder_catch) -I$(inc_folder_core) -o $(build_dir)/test_StreamInterface
