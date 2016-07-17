#include directories start---
inc_folder_basic := $(dir $(wildcard ../../src/*/))
inc_folder_test := ../
inc_folder_kernel := ../../src/kernel
inc_folder_kernel_component := ../../src/kernel/component

#get list of subdirectories in ../modules/
inc_folder_module_subdirs := $(sort $(dir $(wildcard ../../src/modules/*/)))

include_dirs = $(inc_folder_basic) $(inc_folder_module_subdirs) $(inc_folder_test) $(inc_folder_kernel_component) $(inc_folder_kernel)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

specification_flags = -pthread

objects_kernel := $(wildcard ../../src/kernel/intermediate/*.o)
objects_modules := $(wildcard ../../src/modules/**/intermediate/*.o)

objects_gl := $(wildcard ../../src/gl/intermediate/*.o)
objects_render := $(wildcard ../../src/render/intermediate/*.o)
objects_math := $(wildcard ../../src/math/intermediate/*.o)
objects_file := $(wildcard ../../src/file/intermediate/*.o)

target := test_enEngineKernel0

object_files := $(objects_kernel) $(objects_modules) $(objects_gl) $(objects_render) $(objects_math) $(objects_file)

dir_build := ./build

libs =  -lGL -lGLEW -lgl3w `pkg-config --static --libs glfw3`

pkg_config_flags = `pkg-config --cflags glfw3`

$(shell mkdir -p $(dir_build))

.PHONY: all

all:
	$(CXX) -g -std=c++14 $(pkg_config_flags) -o $(dir_build)/$(target) $(target).cpp $(object_files) $(include_flags) $(specification_flags) $(libs)

.PHONY: clean

clean:
	rm -f -r ./build 

