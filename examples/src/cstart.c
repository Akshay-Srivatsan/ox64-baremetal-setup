void _cstart(void) {
  extern char __bss_start__, __bss_end__;
  for (char *c = &__bss_start__; c < &__bss_end__; c++) {
    *c = 0;
  }

  void kmain(void);
  kmain();
}
