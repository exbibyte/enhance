src_dirs := ../../src/fileformat
src_files := $(src_dirs)/file_md5_mesh.cpp $(src_dirs)/file_md5_anim.cpp
dir_build = build
inc_dirs := ../../src/fileformat

$(shell mkdir -p $(dir_build))

.PHONY: all
all:
	g++ -g -std=c++11 $(src_files) test_file_md5_mesh.cpp -o $(dir_build)/test_file_md5_mesh -I$(inc_dirs)
	g++ -g -std=c++11 $(src_files) test_file_md5_anim.cpp -o $(dir_build)/test_file_md5_anim -I$(inc_dirs)
