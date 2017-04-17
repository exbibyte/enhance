#source files---

src_files = $(wildcard *.c)

#source files end---

#include directories start---
inc_folder_core := ./

include_dirs = $(inc_folder_core)

#include directories end---

include_flags = $(foreach d, $(include_dirs), -I$d)

objects := $(patsubst %.c,%.o,$(src_files))

dir_intermediate := ./intermediate

$(shell mkdir -p $(dir_intermediate))

%.o: %.c
	clang -g -Wall -c -o $(dir_intermediate)/$@ $< $(include_flags)

.PHONY: all
all: $(objects)

.PHONY: clean
clean:
	rm -f $(dir_intermediate)/*.o 

