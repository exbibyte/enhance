#include directories start---
inc_folder_core := ../../src/core
inc_folder_test := ../
inc_folder_kernel := ../../src/kernel
inc_folder_kernel_component := ../../src/kernel/component
inc_folder_modules_subdir := $(wildcard ../../src/modules/**/)

#get list of subdirectories in ../modules/
inc_folder_module_subdirs := $(sort $(dir $(wildcard ./*/)))

include_dirs = $(inc_folder_core) $(inc_folder_module_subdirs) $(inc_folder_test) $(inc_folder_kernel_component) $(inc_folder_kernel) $(inc_folder_modules_subdir)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_kernel := $(wildcard ../../src/kernel/intermediate/*.o)
objects_modules := $(wildcard ../../src/modules/**/intermediate/*.o)

target := test_enEngineKernel0

object_files := $(objects_kernel) $(objects_modules)

dir_build := ./build

$(shell mkdir -p $(dir_build))

.PHONY: all

all:
	$(CXX) -std=c++14 -o $(dir_build)/$(target) $(target).cpp $(object_files) $(include_flags)

.PHONY: clean

clean:
	rm -f -r ./build 

