core_dir =  ./core
kernel_dir =  ./kernel

build_core:
	$(MAKE) -C $(core_dir) all

build_kernel:
	$(MAKE) -C $(kernel_dir) all

builds: build_core build_kernel

.PHONY: all
all: builds

.PHONY: clean
clean:
	for dir in $(dir ./ ); do \
	  ($(MAKE) -C $$dir clean);\
	done
