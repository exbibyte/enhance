inc_folder_basic := $(dir $(wildcard ../../impl/*/)) $(dir $(wildcard ../../interface/*/))
inc_folder_test := ../

include_dirs = $(inc_folder_basic) $(inc_folder_test)

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_concurrent = $(wildcard ../../impl/concurrent/intermediate/*.o)

object_files = $(objects_concurrent)

dir_build = build
$(shell mkdir -p $(dir_build))

targets := $(patsubst %.cpp, %, $(wildcard *.cpp))

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
