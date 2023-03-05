#include "lib.h"

void kmain(void) {
  uart_init(115200);
  printk("Hello, World!\n");
  printk("Hex: %x\n", 0xdeadbeef);
  printk("Int: %d\n", -1);
  printk("UInt: %u\n", -1);
  printk("Char: %c\n", 'a');
  printk("String: %s\n", "hi");
  printk("Pointer: %p\n", &kmain);
  printk("Boolean: %b\n", 5);
}
