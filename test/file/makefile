src_files := ../../src/file/file_md5_mesh.cpp
dir_build = build

$(shell mkdir -p $(dir_build))

.PHONY: all
all:
	g++ -g -std=c++11 $(src_files) test_file_md5_mesh.cpp -o $(dir_build)/test_file_md5_mesh -I../../src/file
