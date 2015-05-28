build_dir := ./build
inc_en := ./../../src/enCode
inc_core := ./../../src/core
inc_ui := ./../../src/ui
inc_graph := ./../../src/graph

$(shell mkdir -p $(build_dir))

test_rendertask_mac:
	g++ -g -DGLFW_INCLUDE_GLCOREARB -std=c++11 main.cpp ../../src/ui/WindowManagerGlfw.cpp ../../src/gl/GLTexture.cpp ../../src/gl/GLSLProgram.cpp ../../src/gl/GLHelper.cpp ../../src/file/textfile.cpp ../../src/file/PPM.cpp -I/usr/local/include -I../../src/gl -I../../src/file -I$(inc_core) -I$(inc_en) -I$(inc_ui) -I$(inc_graph) -pthread -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -L/usr/local/lib -o $(build_dir)/test_rendertask

test_rendertask:
        g++ -g -std=c++11 main.cpp ../../src/gl/GLTexture.cpp -I../../src/gl -I../../src/f\
ile -I$(inc_core) -I$(inc_en) -pthread -L/usr/lib/nvidia-340 -lGL -lGLU -lGLEW -lglut -o $(build_di\
r)/test_rendertask

