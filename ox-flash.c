#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <zlib.h>

#include "argparse.h"
#include "headers.h"
#include "io.h"

const char *argp_program_version = "ox-flash 0.0";
const char *argp_program_bug_address = "<srivatsan@cs.stanford.edu>";

void check_ok(int fd) {
  unsigned char ok[2] = {0, 0};
  read_exact(fd, ok, 2);
  printf("%s\n", ok);
  if (memcmp(ok, "OK", 2)) {
    printf("Error: didn't see 'OK' response.\n");
    printf("Response: [0x%x, 0x%x]\n", ok[0], ok[1]);
    exit(1);
  }
}

int main(int argc, char **argv) {
  struct arguments arguments;
  arguments.baud = 115200;
  arguments.addr = 0x22020000;
  arguments.port = NULL;
  arguments.firmware = NULL;
  arguments.linger = false;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  printf("Using baud: %d\n", arguments.baud);
  printf("Using port: %s\n", arguments.port);
  printf("Using addr: 0x%x\n", arguments.addr);
  printf("Using file: %s\n", arguments.firmware);
  printf("\n");

  printf("Opening file... ");
  int file = TRY_IO(open, arguments.firmware, O_RDONLY);
  printf("success! (%d)\n", file);
  struct stat stat;
  TRY_IO(fstat, file, &stat);
  size_t size = stat.st_size;
  printf("File is %ld bytes.\n", size);
  printf("Reading file... ");
  unsigned char *image = malloc(size);
  read_exact(file, image, size);
  printf("success!\n");
  TRY_IO(close, file);
  printf("\n");

  printf("Opening port... ");
  int port = TRY_IO(open, arguments.port, O_RDWR | O_NOCTTY);
  printf("success! (%d)\n", port);

  printf("Setting baud rate... ");
  set_8n1(port, arguments.baud);
  printf("success!\n");
  printf("\n");

  // Protocol from
  // https://raw.githubusercontent.com/bouffalolab/bl_docs/main/BL602_ISP/en/BL602_ISP_protocol.pdf
  // and https://github.com/smaeul/bouffalo-loader/

  // Handshake

  // 1. 5ms of 0x55
  // 10 bits per byte, <baud> bits per second, 0.005 seconds
  unsigned sync_length = 0.005 * arguments.baud / 10 + 1;
  printf("Sending sync of length %d.\n", sync_length);
  unsigned char *sync = malloc(sync_length);
  memset(sync, 0x55, sync_length);
  write_exact(port, sync, sync_length);

  printf("Sending end-of-sync sequence.\n");
  usleep(300000);
  // 2. for the bl808 only (not the bl602), send an "end-of-sync" sequence
  const unsigned char end_of_sync[12] = {0x50, 0x00, 0x08, 0x00, 0x38, 0xF0,
                                         0x00, 0x20, 0x00, 0x00, 0x00, 0x18};
  write_exact(port, end_of_sync, sizeof(end_of_sync));
  usleep(10000);

  printf("Waiting for OK response... ");
  fflush(stdout);
  check_ok(port);
  printf("\n");

  uint32_t start_addr = arguments.addr;
  struct segment_header segment_header = make_segment_header(start_addr, size);

  // Load Boot Header
  printf("Sending boot header.\n");
  struct boot_header header = default_boot_header();
  header.m0_boot_entry = arguments.addr;
  update_header_hash(&header, &segment_header, image, size);
  update_header_crcs(&header);

  printf("Boot header is %lu bytes.\n", sizeof(header));
  uint16_t length = sizeof(header);
  uint8_t *length_bytes = (uint8_t *)&length;
  unsigned char load_boot_header[4] = {0x11, 0x00, length_bytes[0],
                                       length_bytes[1]};
  write_exact(port, load_boot_header, 4);
  write_exact(port, (unsigned char *)&header, sizeof(header));
  printf("Waiting for OK response... ");
  fflush(stdout);
  check_ok(port);
  printf("\n");

  printf("Sending segment header.\n");
  printf("Segment Address: 0x%x\n", segment_header.destaddr);
  printf("Segment Length: %d\n", segment_header.len);
  unsigned char load_segment_header[4] = {0x17, 0x00,
                                          sizeof(struct segment_header), 0x00};
  write_exact(port, load_segment_header, sizeof(load_segment_header));
  write_exact(port, (const unsigned char *)&segment_header,
              sizeof(struct segment_header));
  printf("Waiting for OK response... ");
  fflush(stdout);
  check_ok(port);
  usleep(10000);
  printf("Reading back segment header... ");
  uint16_t response_length;
  read_exact(port, (unsigned char *)&response_length, 2);
  if (response_length != sizeof(struct segment_header)) {
    ABORT("Error: invalid segment header length in response.\n");
  }
  struct segment_header received_segment_header;
  read_exact(port, (unsigned char *)&received_segment_header,
             sizeof(struct segment_header));
  if (memcmp(&segment_header, &received_segment_header,
             sizeof(struct segment_header))) {
    ABORT("Error: response header didn't match our sent header.\n");
  }
  printf("OK\n");
  printf("\n");

  size_t bytes_per_packet = 4096 - 4;
  size_t packets = (size + bytes_per_packet - 1) / bytes_per_packet;
  printf("Going to send %lu bytes (%lu packet%s).\n", size, packets,
         (packets == 1) ? "" : "s");
  for (size_t i = 0; i < packets; i++) {
    printf("Packet %lu:\n", i);
    size_t offset = i * bytes_per_packet;
    printf("\tOffset: %lu\n", offset);
    size_t length = MIN(bytes_per_packet, size - offset);
    printf("\tLength: %lu\n", length);
    printf("\tWriting segment data... ");
    unsigned char load_segment_data[4] = {0x18, 0x00, length & 0xff,
                                          (length >> 8) & 0xff};
    write_exact(port, load_segment_data, sizeof(load_segment_data));
    write_exact(port, &image[offset], length);
    usleep(30000);
    check_ok(port);
  }
  printf("\n");

  printf("Checking sent image.\n");
  unsigned char check_image[4] = {0x19, 0x00, 0x00, 0x00};
  write_exact(port, check_image, sizeof(check_image));
  printf("Waiting for OK response... ");
  fflush(stdout);
  check_ok(port);
  printf("\n");

  printf("Running sent image.\n");
  unsigned char run_image[4] = {0x1A, 0x00, 0x00, 0x00};
  write_exact(port, run_image, sizeof(run_image));
  printf("Waiting for OK response... ");
  fflush(stdout);
  check_ok(port);
  printf("\n");

  printf("Done!\n");
  if (!arguments.linger) {
    exit(EXIT_SUCCESS);
  }
  printf("---- BEGIN OUTPUT ----\n");

  while (true) {
    char c;
    int x = read(port, &c, 1);
    if (x == 1) {
      write(STDOUT_FILENO, &c, 1);
    }
    if (x < 1) {
      exit(EXIT_SUCCESS);
    }
  }
}
