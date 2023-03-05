SRC = $(wildcard src/*.c)
ASM = $(wildcard src/*.S)
SHARED = $(patsubst src/%.c,obj/%.o,$(SRC)) $(patsubst src/%.S,obj/%.o,$(ASM))

TESTS=$(wildcard tests/[0-9]*.c)
BIN=$(patsubst tests/%.c,out/%.bin,$(TESTS))
MEMMAP=memmap.ld

RV=riscv64-unknown-elf
CC=$(RV)-gcc
AS=$(RV)-as
LD=$(RV)-ld
OBJCOPY=$(RV)-objcopy
OBJDUMP=$(RV)-objdump

CFLAGS += -ffreestanding -nostdlib -nostartfiles -Wall -Werror -Wno-unused-function -march=rv32imafc -mabi=ilp32f -Iinc
ASFLAGS += -ffreestanding -nostdlib -nostartfiles -Wall -Werror -Wno-unused-function -march=rv32imafc -mabi=ilp32f -Iinc
LDFLAGS += -march=rv32imafc -mabi=ilp32f -nostdlib -nostartfiles

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	rm -rf obj out **/*.d

ifneq ($(MAKECMDGOALS),clean)
include $(wildcard **/*.d)
endif

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $< $(CFLAGS) -c -o $@ -MMD -MF $(@:.o=.d)

obj/%.o: tests/%.c
	mkdir -p obj
	$(CC) $< $(CFLAGS) -c -o $@ -MMD -MF $(@:.o=.d)

obj/%.o: src/%.S
	mkdir -p obj
	$(CC) $< $(ASFLAGS) -c -o $@ -MMD -MF $(@:.o=.d)

obj/%.elf: obj/%.o $(SHARED) $(MEMMAP)
	mkdir -p obj
	$(CC) $(LDFLAGS) -T$(MEMMAP) $(SHARED) $< -o $@

out/%.bin out/%.list: obj/%.elf
	mkdir -p out
	$(OBJCOPY) $< -Obinary $@
	$(OBJDUMP) $< -D > $(@:.bin=.list)

.PRECIOUS: obj/%.o obj/%.elf out/%.list
.SUFFIXES:
