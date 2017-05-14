src_files = $(wildcard *.cpp)

inc_folder_core := $(dir $(wildcard ../../*/ ) )
inc_folder_gl := ./

include_dirs = $(inc_folder_core) $(inc_folder_gl)

include_flags = $(foreach d, $(include_dirs), -I$d)

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

