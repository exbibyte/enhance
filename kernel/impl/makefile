inc_folder_core_impl = $(dir $(wildcard ../../core/impl/*/))
inc_folder_core_interface = $(dir $(wildcard ../../core/interface/*/))
inc_folder_kernel_interface = $(dir $(wildcard ../interface/*/))
inc_folder_test = ../

include_dirs = $(inc_folder_test) $(inc_folder_core_impl) $(inc_folder_core_interface) $(inc_folder_kernel_interface)

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_core = $(wildcard ../../core/impl/**/intermediate/*.o)

object_files = $(objects_core)

src_files = $(wildcard *.cpp)

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
