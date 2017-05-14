inc_folder_basic = $(dir $(wildcard ../../impl/*/))
inc_folder_interface = $(dir $(wildcard ../../interface/*/))
inc_folder_test = ../

include_dirs = $(inc_folder_basic) $(inc_folder_test) $(inc_folder_interface)

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_render = $(wildcard ../../impl/render/intermediate/*.o)

object_files = $(objects_render)

dir_build = build
$(shell mkdir -p $(dir_build))

#target_excludes := test_lock_reader_writer
target_excludes := 

targets := $(filter-out $(target_excludes), $(patsubst %.cpp, %, $(wildcard *.cpp)) )

libs := -pthread

%: %.cpp
	$(CXX) -g -std=c++14 -o $(dir_build)/$@ $^ $(object_files) $(include_flags) $(libs)

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
