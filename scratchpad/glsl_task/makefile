build_dir := ./build
inc_en := ./../../src/enCode
inc_core := ./../../src/core

$(shell mkdir -p $(build_dir))

all:
	g++ -g -std=c++11 main.cpp ../../src/gl/GLTexture.cpp -I../../src/gl -I../../src/file -I$(inc_core) -I$(inc_en) -pthread -L/usr/lib/nvidia-340 -lGL -lGLU -lGLEW -lglut -o $(build_dir)/test_rendertask
