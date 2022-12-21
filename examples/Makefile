START=start.o cstart.o
TESTS=$(wildcard [0-9]*.c)
MEMMAP=memmap.ld

RV=riscv64-unknown-elf
CC=$(RV)-gcc
AS=$(RV)-as
LD=$(RV)-ld
OBJCOPY=$(RV)-objcopy
OBJDUMP=$(RV)-objdump

CFLAGS += -ffreestanding -nostdlib -nostartfiles -Wall -Werror -Wno-unused-function -march=rv32imafc -mabi=ilp32f
ASFLAGS += -ffreestanding -nostdlib -nostartfiles -Wall -Werror -Wno-unused-function -march=rv32imafc -mabi=ilp32f
LDFLAGS += -march=rv32imafc -mabi=ilp32f -nostdlib -nostartfiles

.PHONY: akk
all: $(TESTS:.c=.bin)

.PHONY: clean
clean:
	rm -f *.bin *.list *.o *.elf *.d

ifneq ($(MAKECMDGOALS),clean)
	include $(wildcard *.d)
endif

%.o %.d: %.c
	$(CC) $(CFLAGS) $< -c -o $@
	$(CC) $(CFLAGS) -MMD $< -c -o $@

%.o %.d: %.S
	$(CC) $(ASFLAGS) $< -c -o $@
	$(CC) $(ASFLAGS) -MMD $< -c -o $@

%.elf: %.o $(START) $(MEMMAP)
	$(CC) $(LDFLAGS) -T$(MEMMAP) $(START) $< -o $@

%.bin %.list: %.elf
	$(OBJCOPY) $< -Obinary $@
	$(OBJDUMP) $< -D > $(@:.bin=.list)

.PRECIOUS: %.o %.elf %.list
