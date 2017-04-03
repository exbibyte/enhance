sub_modules := $(sort $(dir $(wildcard */) ))

.PHONY: sub_modules

.PHONY: all

all:
	for dir in $(sub_modules); do \
	  ($(MAKE) -C $$dir);\
	done

.PHONY: clean

clean:
	for dir in $(sub_modules); do \
	  ($(MAKE) -C $$dir clean);\
	done
