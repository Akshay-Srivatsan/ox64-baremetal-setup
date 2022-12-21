#pragma once
#include <stdio.h>

#define ABORT(...)                                                             \
  {                                                                            \
    fflush(stdout);                                                            \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(EXIT_FAILURE);                                                        \
  }

#define IO_ABORT(fn) ABORT("\nError in `%s`: %s\n", fn, strerror(errno))

#define TRY_IO(fn, ...)                                                        \
  ({                                                                           \
    int ret = fn(__VA_ARGS__);                                                 \
    if (ret < 0) {                                                             \
      IO_ABORT(#fn);                                                           \
    }                                                                          \
    ret;                                                                       \
  })
