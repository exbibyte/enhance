#include directories start---
inc_folder_basic := $(dir $(wildcard ../../src/*/))
inc_folder_test := ../

include_dirs = $(inc_folder_basic) $(inc_folder_test)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects_math = $(wildcard ../../src/math/intermediate/*.o)

object_files = $(objects_math)

dir_build := ./build

$(shell mkdir -p $(dir_build))

# targets := $(patsubst %.cpp, %, $(wildcard *.cpp))
targets := test_perspective test_lookat test_mat test_vec test_combinatronics test_intersection_ray_sphere test_intersection_ray_plane test_intersection_ray test_least_squares test_interp_poly_div_diff test_intersection_ray_ray

libs =  -lGL -lGLEW -lgl3w `pkg-config --static --libs glfw3`

%: %.cpp
	$(CXX) -g -std=c++14 -o $(dir_build)/$@ $^ $(object_files) $(include_flags) $(libs)

.PHONY: all
all: $(targets)

.PHONY: clean
clean:
	rm -f -r ./build 

