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

flex_PolyMesh.yy.c: flex_PolyMesh.l
	flex --outfile=flex_PolyMesh.yy.c $<

flex_PolyMesh.yy.o: flex_PolyMesh.yy.c
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

bison_PolyMesh.tab.c: bison_PolyMesh.y
	bison -d -v bison_PolyMesh.y

bison_PolyMesh.tab.o: bison_PolyMesh.tab.c
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

%.o: %.cpp bison_PolyMesh.tab.o flex_PolyMesh.yy.o
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

.PHONY: all

all: $(objects)

.PHONY: clean

clean:
	rm -f $(dir_intermediate)/*.o 

obj2polymesh:
	$(CXX) -g -std=c++14 Obj2PolyMesh.cpp -o Obj2PolyMesh
