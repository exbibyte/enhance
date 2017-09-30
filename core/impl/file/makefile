src_files = $(wildcard *.cpp)

include_dirs := $(dir $(wildcard ../*/)) $(dir $(wildcard ../../interface/*/))

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_math := $(wildcard ../math/intermediate/*.o)

objects := $(patsubst %.cpp,%.o,$(src_files))

dir_intermediate := ./intermediate

$(shell mkdir -p $(dir_intermediate))

%.o: %.cpp
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

.PHONY: all

all: $(objects)

.PHONY: clean

clean:
	rm -f $(dir_intermediate)/*.o 

