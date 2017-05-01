src_files = $(wildcard *.cpp)

include_dirs = $(dir $(wildcard ../*/)) $(dir $(wildcard ../../interface/))

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

alloc:
	$(CXX) -g -std=c++14 -c alloc_first_fit.cpp -o $(dir_intermediate)/alloc.o $(include_flags)

free:
	$(CXX) -g -std=c++14 -c free_first_fit.cpp -o $(dir_intermediate)/free.o $(include_flags)

memorypartition:
	$(CXX) -g -std=c++14 -c memory_p1.cpp -o $(dir_intermediate)/memory_p1.o $(include_flags)

memorymanager:
	$(CXX) -g -std=c++14 -c memory_manager_p1t_g1_ff.cpp -o $(dir_intermediate)/memory_manager_p1t_g1_ff.o $(include_flags)
