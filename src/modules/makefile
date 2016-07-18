sub_modules := $(sort $(dir $(wildcard */) ))

.PHONY: sub_modules

all:
	for dir in $(sub_modules); do \
	  ($(MAKE) -C $$dir);\
	done

