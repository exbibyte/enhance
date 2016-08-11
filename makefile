# kernel related starts
kernel_modules := ./src/modules
kernel := ./src/kernel
test_kernel := ./test/kernel
test_algo := ./test/algo

build_render:
	$(MAKE) -C ./src/render

build_math:
	$(MAKE) -C ./src/math

build_file:
	$(MAKE) -C ./src/file

build_datatransform:
	$(MAKE) -C ./src/DataTransform

build_ui:
	$(MAKE) -C ./src/ui

build_gl:
	$(MAKE) -C ./src/gl

build_kernel_modules: build_render build_math build_file build_datatransform build_ui build_gl
	$(MAKE) -C $(kernel_modules)

build_kernel: build_kernel_modules
	$(MAKE) -C $(kernel)

build_test_algo:
	$(MAKE) -C $(test_algo)

build_test_kernel: build_kernel
	$(MAKE) -C $(test_kernel)

.PHONY: all
all: build_test_kernel
#kernel related ends
