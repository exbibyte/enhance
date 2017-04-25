src_dir :=  ./impl
test_dir :=  ./test

build_test_ds:
	$(MAKE) -C $(test_dir)/ds all
build_src_ds:
	$(MAKE) -C $(src_dir)/ds all

build_src_memory:
	$(MAKE) -C $(src_dir)/memory all

build_test_dsc:
	$(MAKE) -C $(test_dir)/dsc all
build_src_dsc:
	$(MAKE) -C $(src_dir)/dsc all

build_src_math:
	$(MAKE) -C $(src_dir)/math all

build_test_hash:
	$(MAKE) -C $(test_dir)/hash all
build_src_hash:
	$(MAKE) -C $(src_dir)/hash all

builds: build_src_ds build_src_memory build_src_dsc build_src_math build_src_hash

tests: build_test_ds build_test_dsc build_test_hash

.PHONY: all
all: builds tests


.PHONY: clean
clean:
	for dir in $(dir $(src_dir)/*/) $(dir $(test_dir)/*/); do \
	  ($(MAKE) -C $$dir clean);\
	done
