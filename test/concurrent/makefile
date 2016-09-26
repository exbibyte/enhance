#include directories start---
inc_folder_basic := $(dir $(wildcard ../../src/*/))
inc_folder_test := ../

#common basic include folders
inc_folder_basic := $(sort $(dir $(wildcard ../../src/*/)))

include_dirs = $(inc_folder_basic) $(inc_folder_test)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_concurrent = $(wildcard ../../src/core/intermediate/*.o)

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

