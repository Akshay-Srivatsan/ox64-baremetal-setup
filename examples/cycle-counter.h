#pragma once
#include <stddef.h>

#define CYCLES_PER_SECOND 320000000

static size_t cycle_cnt_read(void) {
  size_t count;
  asm volatile("rdcycle %0" : "=r"(count));
  return count;
}
