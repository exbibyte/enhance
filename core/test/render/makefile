inc_folder_basic = $(dir $(wildcard ../../impl/*/))
inc_folder_interface = $(dir $(wildcard ../../interface/*/))
inc_folder_gl = ../../impl/render/gl/
inc_folder_test = ../

include_dirs = $(inc_folder_basic) $(inc_folder_test) $(inc_folder_interface) $(inc_folder_gl)

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_interface = $(wildcard ../../interface/intermediate/*.o)
objects_render = $(wildcard ../../impl/render/intermediate/*.o)
objects_render_gl = $(wildcard ../../impl/render/gl/intermediate/*.o)
objects_file = $(wildcard ../../impl/file/intermediate/*.o)
objects_algo = $(wildcard ../../impl/algo/intermediate/*.o)
objects_memory = $(wildcard ../../impl/memory/intermediate/*.o)
objects_math = $(wildcard ../../impl/math/intermediate/*.o)

object_files = $(objects_render) $(objects_render_gl) $(objects_file) $(objects_algo) $(objects_interface) $(objects_memory) $(objects_math)

dir_build = build
$(shell mkdir -p $(dir_build))

#target_excludes := test_lock_reader_writer
target_excludes := 

targets := $(filter-out $(target_excludes), $(patsubst %.cpp, %, $(wildcard *.cpp)) )

libs := -pthread -lGL `pkg-config --static --libs glfw3`

pkg_config_flags = `pkg-config --cflags glfw3`

%: %.cpp
	$(CXX) -g -std=c++14 $(pkg_config_flags) -o $(dir_build)/$@ $^ $(object_files) $(include_flags) $(libs)

.PHONY: all
all: $(targets)

.PHONY: clean
clean:
	rm -f -r ./$(dir_build)

testcase_excludes := $(dir_build)/

testcases:
	@for i in $(filter-out $(testcase_excludes), $(wildcard $(dir_build)/*) ); do \
		./$$i; \
	done \

# .PHONY: test_rendergraphscheduler
# test_rendergraphscheduler:
# 	$(CXX) -g -std=c++14 test_rendergraphscheduler.cpp -o $(dir_build)/test_rendergraphscheduler $(object_files) $(include_flags) $(libs)
