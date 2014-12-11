build_dir := ./build
inc_en := ./../../src/enCode
inc_core := ./../../src/core

$(shell mkdir -p $(build_dir))

test_shadow_mac:
	g++ -g -DGLFW_INCLUDE_GLCOREARB -DMAC_OS -std=c++11 main.cpp ../../src/gl/GLTexture.cpp ../../src/gl/GLSLProgram.cpp ../../src/gl/GLHelper.cpp ../../src/file/textfile.cpp -I/usr/local/include -I../../src/gl -I../../src/file -I$(inc_core) -I$(inc_en) -pthread -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -L/usr/local/lib -o $(build_dir)/test_shadow

test_shadow:
	g++ -g -std=c++11 main.cpp ../../src/gl/GLTexture.cpp -I../../src/gl -I../../src/file -L/usr/lib/nvidia-340 -lGL -lGLU -lGLEW -lglut -o test_shadow
