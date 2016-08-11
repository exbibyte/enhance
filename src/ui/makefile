#source files---

src_files = $(wildcard imgui/*.cpp)

#source files end---

#include directories start---

include_dirs = ./imgui ./imgui/GL

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects = $(patsubst %.cpp,%.o,$(src_files))

dir_intermediate = ./intermediate

$(shell mkdir -p $(dir_intermediate))

gl3w.o : ./imgui/GL/gl3w.c
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$@ $< $(include_flags)

%.o: %.cpp gl3w.o
	$(CXX) -g -std=c++14 -c -o $(dir_intermediate)/$(notdir $@) $< $(include_flags)

.PHONY: all

all: $(objects)

.PHONY: clean

clean:
	rm -f $(dir_intermediate)/*.o 

