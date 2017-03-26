src_dir = ../../src/fileformat
src_dir_math = ../../src/math
src_files = $(src_dir)/file_md5_mesh.cpp $(src_dir)/file_md5_anim.cpp $(src_dir)/file_md5_skel.cpp $(src_dir)/file_md5_calc_mesh.cpp
src_files_math = $(src_dir_math)/Quat.cpp $(src_dir_math)/Vec.cpp
dir_build = build
inc_dir = ../../src/fileformat
inc_dir_math = ../../src/math

$(shell mkdir -p $(dir_build))

.PHONY: all
all:
	g++ -g -std=c++11 $(src_files) $(src_files_math) test_file_md5_mesh.cpp -o $(dir_build)/test_file_md5_mesh -I$(inc_dir) -I$(inc_dir_math)
	g++ -g -std=c++11 $(src_files) $(src_files_math) test_file_md5_anim.cpp -o $(dir_build)/test_file_md5_anim -I$(inc_dir) -I$(inc_dir_math)
	g++ -g -std=c++11 $(src_files) $(src_files_math) test_file_md5_skel.cpp -o $(dir_build)/test_file_md5_skel -I$(inc_dir) -I$(inc_dir_math)
	g++ -g -std=c++11 $(src_files) $(src_files_math) test_file_md5_calc_mesh.cpp -o $(dir_build)/test_file_md5_calc_mesh -I$(inc_dir) -I$(inc_dir_math)
