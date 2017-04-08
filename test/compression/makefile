#include directories start---

inc_folder_basic := $(dir $(wildcard ../../src/*/))
inc_folder_test := ../

include_dirs = $(inc_folder_basic) $(inc_folder_test)

#include directories end---

#src files start---

src_files = $(wildcard *.cpp)

#src files end---

include_flags = $(foreach d, $(include_dirs), -I$d)

dir_build = build

$(shell mkdir -p $(dir_build))

targets := $(patsubst %.cpp,%,$(src_files))

% : %.cpp
	$(CXX) -g -std=c++14 -o $(dir_build)/$@ $< $(include_flags)

.PHONY: all
all: $(targets)

.PHONY: clean
clean:
	rm -f -r ./build 

testcase_excludes := $(dir_build)/

testcases:
	@for i in $(filter-out $(testcase_excludes), $(wildcard $(dir_build)/*) ); do \
		./$$i; \
	done \
