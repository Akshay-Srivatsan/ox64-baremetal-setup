SRC=$(wildcard *.c)

CFLAGS += -Werror -Wall -Wno-packed-bitfield-compat -lz -lssl -lcrypto

.PHONY: all
all: ox-flash

.PHONY: clean
clean:
	rm -f ox-flash *.d boot-header.h

include $(wildcard *.d)

boot-header.h: dump-struct.py $(wildcard *.py)
	./$< > $@

ox-flash: ox-flash.c boot-header.h
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MMD $(CFLAGS) $< -o $@
