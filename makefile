core_dir =  ./core
kernel_dir =  ./kernel

build_core:
	$(MAKE) -C $(core_dir) all

build_kernel:
	$(MAKE) -C $(kernel_dir) all

builds: build_core build_kernel

.PHONY: all
all: builds


removetemp0:
	find -name '*~' | xargs rm -f

removetemp1:
	find -name '*#' | xargs rm -f

removetemp: removetemp0 removetemp1

.PHONY: clean
clean: removetemp
	for dir in $(dir ./*/ ); do \
	  ($(MAKE) -C $$dir clean);\
	done

