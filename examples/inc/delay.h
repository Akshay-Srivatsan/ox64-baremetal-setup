#pragma once
#include <stdbool.h>

#include "cycle-counter.h"

static void delay_ncycles(size_t ncycles) {
  size_t start = cycle_cnt_read();
  while (true) {
    if ((cycle_cnt_read() - start) >= ncycles)
      break;
  }
}
static void delay_us(size_t us) {
  delay_ncycles(CYCLES_PER_SECOND / 1000000 * us);
}

static void delay_ms(size_t ms) {
  delay_us(ms * 1000);
}

static void delay_sec(size_t sec) {
  delay_ms(sec * 1000);
}
