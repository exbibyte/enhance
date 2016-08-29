#include directories start---

inc_folder_basic = $(dir $(wildcard ../../src/*/))
inc_fold_imgui = ../../src/ui/imgui
inc_folder_test = ../

include_dirs = $(inc_folder_imgui) $(inc_folder_basic) $(inc_folder_test)

#include directories end---

#src files start---

src_files = $(wildcard *.cpp)

#src files end---

include_flags = $(foreach d, $(include_dirs), -I$d)

dir_build = build

objects_ui := $(wildcard ../../src/ui/intermediate/*.o)
objects_math := $(wildcard ../../src/math/intermediate/*.o)

objects = $(objects_ui) $(objects_math)

libs =  -lGL -lGLEW -lgl3w `pkg-config --static --libs glfw3`

pkg_config_flags = `pkg-config --cflags glfw3`

$(shell mkdir -p $(dir_build))

targets := $(patsubst %.cpp,%,$(src_files))

% : %.cpp
	$(CXX) -g -std=c++14 -o $(dir_build)/$@ $< $(include_flags)

.PHONY: all
all: $(targets)

.PHONY: clean
clean:
	rm -f -r ./build 

test_trackball:
	$(CXX) -g -std=c++14 $(pkg_config_flags) -o $(dir_build)/test_trackball test_trackball.cpp $(objects) $(include_flags) $(libs)
