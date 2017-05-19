src_files = $(wildcard *.cpp)

include_dirs_impl := $(dir $(wildcard ../*/))
include_dirs_interface := $(dir $(wildcard ../../interface/*/))
include_dirs_gl := gl/

include_dirs := $(include_dirs_gl) $(include_dirs_impl) $(include_dirs_interface)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects := $(patsubst %.cpp,%.o, $(src_files))
objects_gl = $(wildcard gl/intermediate/*.o)

dir_intermediate := ./intermediate

$(shell mkdir -p $(dir_intermediate))

%.o: %.cpp
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

.PHONY: build_gl

build_gl:
	$(MAKE) -C ./gl/ all

.PHONY: clean_gl

clean_gl:
	$(MAKE) -C ./gl/ clean

.PHONY: all

all: build_gl $(objects)

.PHONY: clean

clean: clean_gl
	rm -f $(dir_intermediate)/*.o 

