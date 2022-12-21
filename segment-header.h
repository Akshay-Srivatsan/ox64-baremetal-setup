#pragma once
#include <stdint.h>

struct segment_header {
  uint32_t destaddr;
  uint32_t len;
  uint32_t rsvd;
  uint32_t crc32;
};
