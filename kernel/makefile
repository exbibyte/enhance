src_dir :=  ./impl
test_dir :=  ./test

build_test:
	$(MAKE) -C $(test_dir) all
build_src:
	$(MAKE) -C $(src_dir) all

builds: build_src

tests: build_test

.PHONY: all
all: builds tests


.PHONY: clean
clean:
	for dir in $(dir $(src_dir)/*/) $(dir $(test_dir)/*/); do \
	  ($(MAKE) -C $$dir clean);\
	done
