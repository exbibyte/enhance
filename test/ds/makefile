inc_folder_basic := $(dir $(wildcard ../../impl/*/))
inc_folder_test := ../

include_dirs = $(inc_folder_basic) $(inc_folder_test)

src_files = $(wildcard *.cpp)

objects_memory = $(wildcard ../../impl/memory/intermediate/*.o)
objects_ds = $(wildcard ../../impl/ds/intermediate/*.o)

object_files := $(objects_memory) $(objects_ds)

include_flags = $(foreach d, $(include_dirs), -I$d)

dir_build = build

$(shell mkdir -p $(dir_build))

targets := $(patsubst %.cpp,%,$(src_files))

% : %.cpp
	$(CXX) -std=c++11 -g -Wall -o $(dir_build)/$@ $< $(object_files) $(include_flags)

.PHONY: all
all: $(targets)

.PHONY: clean
clean:
	rm -f -r ./build 

testcase_excludes := ""

testcases:
	@for i in $(filter-out $(testcase_excludes), $(wildcard $(dir_build)/*) ); do \
		./$$i; \
	done \
