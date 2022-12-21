#pragma once
#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct arguments {
  char *port;
  unsigned baud;
  unsigned addr;
  char *firmware;
  bool linger;
};

static struct argp_option options[] = {
    {"port", 'p', "SERIALPORT", 0, "The serial port to use."},
    {"baud", 'b', "BAUD", 0, "The baud rate to use."},
    {"addr", 'a', "ADDRESS", 0,
     "The RAM address to load at (default 0x22020000)."},
    {"linger", 'l', 0, 0,
     "Keep the serial port open and print any bytes received."},
    {0},
};

static char args_doc[] = "";
static char doc[] = "ox-flash -- A flashing tool for the bl808 chip used on "
                    "the Ox64 development board.";

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = (struct arguments *)state->input;
  switch (key) {
  case 'p':
    arguments->port = arg;
    break;
  case 'b':
    arguments->baud = strtol(arg, NULL, 0);
    break;
  case 'a':
    arguments->addr = strtol(arg, NULL, 0);
    break;
  case 'l':
    arguments->linger = true;
    break;
  case ARGP_KEY_ARG:
    if (arguments->firmware == NULL) {
      arguments->firmware = arg;
    } else {
      argp_usage(state);
    }
    break;
  case ARGP_KEY_END:
    if ((arguments->baud < 0) || (arguments->port == NULL) ||
        (!strcmp(arguments->port, "")) || (arguments->firmware == NULL) ||
        (!strcmp(arguments->firmware, ""))) {
      argp_usage(state);
    }
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};
