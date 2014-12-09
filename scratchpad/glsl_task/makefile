build_dir := ./build
inc_en := ./../../src/enCode
inc_core := ./../../src/core

$(shell mkdir -p $(build_dir))

test_rendertask_mac:
	g++ -g -DMAC_OS -std=c++11 main.cpp ../../src/gl/GLTexture.cpp -I/usr/local/include -I../../src/gl -I../../src/file -I$(inc_core) -I$(inc_en) -pthread -lGLEW -framework GLUT -framework OpenGL -framework Cocoa -L/usr/local/lib -o $(build_dir)/test_rendertask

test_rendertask:
        g++ -g -std=c++11 main.cpp ../../src/gl/GLTexture.cpp -I../../src/gl -I../../src/f\
ile -I$(inc_core) -I$(inc_en) -pthread -L/usr/lib/nvidia-340 -lGL -lGLU -lGLEW -lglut -o $(build_di\
r)/test_rendertask

