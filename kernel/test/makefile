inc_folder_core_impl = $(dir $(wildcard ../../core/impl/*/))
inc_folder_core_interface = $(dir $(wildcard ../../core/interface/*/))
inc_folder_kernel_interface = $(dir $(wildcard ../../kernel/interface/*/))
inc_folder_kernel_impl = $(dir $(wildcard ../../kernel/impl/*/))
inc_folder_test = ../
inc_folder_gl = ../../impl/render/gl/

include_dirs = $(inc_folder_test) $(inc_folder_core_impl) $(inc_folder_core_interface) $(inc_folder_kernel_impl) $(inc_folder_kernel_interface) $(inc_folder_gl)

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_core := $(wildcard ../../core/impl/*/intermediate/*.o)
objects_kernel := $(wildcard ../../kernel/impl/intermediate/*.o)
objects_render = $(wildcard ../../core/impl/render/intermediate/*.o)

object_files = $(objects_kernel) $(objects_core)

dir_build = build
$(shell mkdir -p $(dir_build))

#target_excludes := test_lock_reader_writer
target_excludes := 

targets := $(filter-out $(target_excludes), $(patsubst %.cpp, %, $(wildcard *.cpp)) )

libs := -pthread -lGL -lGLEW -lgl3w `pkg-config --static --libs glfw3`

pkg_config_flags = `pkg-config --cflags glfw3`

%: %.cpp
	$(CXX) -DDEBUG_INFO -g -std=c++14 -o $(dir_build)/$@ $(pkg_config_flags) $^ $(object_files) $(include_flags) $(libs)

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
