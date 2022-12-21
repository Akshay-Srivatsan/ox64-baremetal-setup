#pragma once

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "error.h"

static speed_t speed_for_baud(unsigned baud) {
  switch (baud) {
  case 0:
    return B0;
  case 50:
    return B50;
  case 75:
    return B75;
  case 110:
    return B110;
  case 134:
    return B134;
  case 150:
    return B150;
  case 200:
    return B200;
  case 300:
    return B300;
  case 600:
    return B600;
  case 1200:
    return B1200;
  case 1800:
    return B1800;
  case 2400:
    return B2400;
  case 4800:
    return B4800;
  case 9600:
    return B9600;
  case 19200:
    return B19200;
  case 38400:
    return B38400;
  case 57600:
    return B57600;
  case 115200:
    return B115200;
  case 230400:
    return B230400;
  case 460800:
    return B460800;
  case 500000:
    return B500000;
  case 576000:
    return B576000;
  case 921600:
    return B921600;
  case 1000000:
    return B1000000;
  case 1152000:
    return B1152000;
  case 1500000:
    return B1500000;
  case 2000000:
    return B2000000;
  default:
    ABORT("Invalid baud rate: %d\n", baud);
  }
}

static void set_8n1(int fd, int baud) {
  struct termios tio;
  tcgetattr(fd, &tio);
  speed_t speed = speed_for_baud(baud);
  cfsetispeed(&tio, speed);
  cfsetospeed(&tio, speed);
  tio.c_cflag &= ~PARENB;
  tio.c_cflag &= ~CSTOPB;
  tio.c_cflag &= ~CSIZE;
  tio.c_cflag |= CS8;
  tio.c_cflag &= ~CRTSCTS;
  tio.c_cflag |= CREAD | CLOCAL;
  tio.c_cc[VMIN] = 0;
  tio.c_cc[VTIME] = 20; // 1-second timeout
  cfmakeraw(&tio);
  TRY_IO(tcflush, fd, TCIFLUSH);
  TRY_IO(tcsetattr, fd, TCSANOW, &tio);
}

static void write_exact(int fd, const unsigned char *data, size_t n_bytes) {
  int response = TRY_IO(write, fd, data, n_bytes);
  if (response != n_bytes) {
    IO_ABORT("write_exact");
  }
}

static void read_exact(int fd, unsigned char *data, size_t n_bytes) {
  int response = TRY_IO(read, fd, data, n_bytes);
  if (response != n_bytes) {
    IO_ABORT("read_exact");
  }
}
