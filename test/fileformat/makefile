src_files := ../../src/file/file_md5_mesh.cpp ../../src/file/file_md5_anim.cpp
dir_build = build

$(shell mkdir -p $(dir_build))

.PHONY: all
all:
	g++ -g -std=c++11 $(src_files) test_file_md5_mesh.cpp -o $(dir_build)/test_file_md5_mesh -I../../src/file
	g++ -g -std=c++11 $(src_files) test_file_md5_anim.cpp -o $(dir_build)/test_file_md5_anim -I../../src/file
