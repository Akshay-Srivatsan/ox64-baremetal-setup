SRC=$(wildcard *.c)

CFLAGS += -Werror -Wall -Wno-packed-bitfield-compat -lz -lssl -lcrypto -MMD

.PHONY: all
all: ox-flash examples

.PHONY: examples
examples:
	$(MAKE) -C examples

.PHONY: clean
clean:
	rm -f ox-flash *.d boot-header.h
	$(MAKE) -C examples clean

ifneq ($(MAKECMDGOALS),clean)
	include $(wildcard *.d)
endif

boot-header.h: dump-struct.py $(wildcard *.py)
	./$< > $@

ox-flash: ox-flash.c boot-header.h
	$(CC) $< $(CFLAGS) -o $@
