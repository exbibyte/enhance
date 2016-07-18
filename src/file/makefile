#source files---

src_files = $(subst Obj2PolyMesh.cpp,,$(wildcard *.cpp))
#src_files_yy = $(wildcard *.yy.c )

#source files end---

#include directories start---
inc_folder_core := ../core ../render ../gl ../file ../asset ../math

include_dirs = $(inc_folder_core)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects := $(patsubst %.cpp,%.o,$(src_files))
#objects_yy := $(patsubst %.c,%.o,$(src_files_yy))

dir_intermediate := ./intermediate

$(shell mkdir -p $(dir_intermediate))

%.o: %.cpp
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

flex_PolyMesh.yy.o: flex_PolyMesh.yy.c
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

bison_PolyMesh.tab.o: bison_PolyMesh.tab.c
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

.PHONY: all

all: $(objects) flex_PolyMesh.yy.o bison_PolyMesh.tab.o

.PHONY: clean

clean:
	rm -f $(dir_intermediate)/*.o 

