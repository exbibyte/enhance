#source directories start---

src_folder_core := ../../core

#source directories end---

#source files---

src_files = $(wildcard *.cpp) $(wildcard **/*.cpp)

#source files end---

#include directories start---
inc_folder_core := ../../core

#get list of subdirectories in ../modules/
inc_folder_module_subdirs := $(sort $(dir $(wildcard ./../**/)))

# inc_folder_gl3w = ../../gl3w/include

include_dirs = $(inc_folder_core) $(inc_folder_module_subdirs)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects := $(patsubst %.cpp,%.o,$(src_files) )

dir_intermediate := ./intermediate

$(shell mkdir -p $(dir_intermediate))

%.o: %.cpp
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

.PHONY: all

all: $(objects)

.PHONY: clean

clean:
	rm -f $(dir_intermediate)/*.o 

