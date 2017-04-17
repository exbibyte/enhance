#include directories start---

inc_folder_basic := $(dir $(wildcard ../../src/*/))
inc_folder_test := ../

include_dirs = $(inc_folder_basic) $(inc_folder_test)

#include directories end---

#src files start---

src_files = $(wildcard *.c)

#src files end---

objects_memory = $(wildcard ../../src/memory/intermediate/*.o)
objects_ds = $(wildcard ../../src/ds/intermediate/*.o)

object_files := $(objects_memory) $(objects_ds)

include_flags = $(foreach d, $(include_dirs), -I$d)

dir_build = build

$(shell mkdir -p $(dir_build))

targets := $(patsubst %.c,%,$(src_files))

% : %.c
	clang -g -Wall -o $(dir_build)/$@ $< $(object_files) $(include_flags)

.PHONY: all
all: $(targets)

.PHONY: clean
clean:
	rm -f -r ./build 

testcase_excludes := ""

testcases:
	@for i in $(filter-out $(testcase_excludes), $(wildcard $(dir_build)/*) ); do \
		./$$i; \
	done \
