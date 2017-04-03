#source files---

src_files := $(filter-out obj2polymesh.cpp, $(wildcard *.cpp))

#source files end---

#include directories start---

inc_folder_basic := $(dir $(wildcard ../../src/*/))

include_dirs = $(inc_folder_basic)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects =  $(patsubst %.cpp,%.o,$(src_files))

dir_intermediate := ./intermediate

dir_build = build

$(shell mkdir -p $(dir_build))

$(shell mkdir -p $(dir_intermediate))

obj2polymesh:
	$(CXX) -g -std=c++14 obj2polymesh.cpp -o $(dir_build)/obj2polyMesh

%.o: %.cpp
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

.PHONY: all

all: $(objects) $(obj2polymesh)

.PHONY: clean

clean:
	rm -f $(dir_intermediate)/*.o 


