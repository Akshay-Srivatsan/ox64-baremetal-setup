# Ox64 Baremetal

This code provides a minimal setup for running code baremetal on the
[Ox64][ox64] RISC-V dev board, powered by a BL808 CPU. Instead of using the SDK
provided by Bouffalo Labs, this code compiles a raw RISC-V binary and sends it
to the ROM bootloader.

## `ox-flash`

While the idea was to build a flashing tool for the Ox64, for now it's just
a bootloading tool. The BL808 bootrom seemingly has no way of actually
_flashing_ anything, just loading code to RAM and letting it run. The standard
boot process sends over a binary called `eflash_loader`, which performs the
actual flashing. This loader _could_ send that, but it could also send any
other arbitrary binary over, including a custom flashing stub.

### Usage

```
Usage: ox-flash [OPTION...]
ox-flash -- A flashing tool for the bl808 chip
used on the Ox64 development board.

  -a, --addr=ADDRESS         The RAM address to load at (default 0x22020000).
  -b, --baud=BAUD            The baud rate to use.
  -l, --linger               Keep the serial port open and print any bytes
                             received.
  -p, --port=SERIALPORT      The serial port to use.
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```

This probably only works on Linux as-is, but it might work on macOS too with
a few modifications. It definitely won't work on Windows without
a compatibility layer like WSL.

### Running Binaries

- Install the `riscv64-unknown-elf-gcc` toolchain from your package manager.
  - `riscv32-unknown-elf-gcc` would also work
- Run `make` to build `ox-flash` and the example.
- Put your Ox64 in boot mode, connected through the Picoprobe or a compatible
  serial adapter.
- Run `./ox-flash --port /dev/ttyACM0 --linger examples/1-hello.bin`,
  substituting `/dev/ttyACM0` for whatever the relevant port is called on your
  computer.

## Bootloading Sequence

The ROM bootloader on the BL808 is similar to the bootloader on the BL602. As
a result, the [BL602 ISP protocol documentation][bl602isp] was very useful for
figuring out the boot sequence. However, there were a few differences from the
BL602, for which I referenced [smaeul/bouffalo-loader][bouffalo-loader].

The sequence was as follows:

- The PC configures its serial port as 8n1, at any reasonable baud rate.
- The PC writes a stream of 0x55 ('U') to the serial port for at least
  5 milliseconds. This lets the Ox64 determine the baud rate of the
  connection.
- The PC writes a sequence of bytes to signal that it's ready to begin.
  - This is _not_ present on the BL602, so existing flashing/loading tools don't
    work.
- In response, the BL808 responds "OK".
- The PC sends over a "boot header." This isn't documented, but
  [bflb-mcu-tool][bflb-mcu-tool] has Python code to generate this, which was
  copied into `bootheader.py` here. Using the Python code, we can create
  a matching C struct.
  - The boot header includes multiple CRC32s of itself (one for the flash
    config section, one for the clock config section, and one for the whole
    thing). It also includes a SHA-256 hash of the entire image being sent
    over, _including_ the section headers.
- In response, the BL808 responds "OK".
- The PC sends over a "segment header" describing a segment to load. My loader
  uses a single contiguous segment for everything, but it is possible to send
  over multiple discontiguous segments too.
- The Ox64 responds "OK" and sends back the same header. It's unclear why, but
  it might have something to do with the optional encryption?
- The PC sends over a sequence of "packets", each of which have at most 4092
  bytes of data (there's a 4-byte header, and the max size is 4096 bytes).
- The Ox64 responds "OK" to each.
- The PC requests that the Ox64 check the sent image.
- The Ox64 responds "OK". Presumably it checks the SHA-256 hash here.
- The PC requests that the Ox64 execute the binary it just sent.
- The Ox64 responds "OK", then transfers execution to the base address of the
  image.
  - _Unlike_ the standard flashing/boot sequence, there's no partition tables
    or fs image here. The data is copied directly to RAM, then executed.

Under the standard flashing sequence, the loaded program (`eflash_loader`)
would then continue communicating with the PC-side program to transfer the real
binary and write it to flash. However, for now this code doesn't do much---I
just wanted to make sure I could get my own code running on the board without
jumping through a million hoops.

## Why not use the SDK?

The biggest reason is that the SDK (like many vendor SDKs) does way too much
behind the scenes. That complexity comes with tradeoffs: for example, the
official SDK requires using a custom build of gcc to use vendor-specific RISC-V
extensions. While the binaries here are much less functional (in fact, they're
nearly trivial), they're compiled using a standard RISC-V toolchain.

I also don't like the Bouffalo Labs flashing tool; again, it seems to do way
too much stuff I don't care about (installing the `boot2` closed-source
bootloader, setting up partitions, etc.), when all I want to do is execute my
code on the chip. In addition, the GUI doesn't work on Wayland, and the
command-line version is almost completely undocumented.

Of course, not using the SDK has all the usual downsides. Literally the only
thing this code can do so far is write to the UART (it can't even configure it!
it relies on the bootloader to have done that). It's also unclear how much
_can_ be implemented without the SDK; I'd assume that the Wi-Fi and BLE are
essentially going to be nonexistent without the official setup.

## Hardware Setup

There's something weird about the UART config on the BL808, so a lot of
USB-serial adapters seem to have trouble with it. What did work was a Raspberry
Pi Pico running [a fork of picoprobe][picoprobefork]. I have pins 4 and 5 on
the Pico connected to pins 14 and 15 on the Ox64 (the corner with the
microUSB/SD card, under the "BOOT" button), which are pins the bootrom uses as
its UART. To start the Ox64 bootrom, connect it to power while holding the
"BOOT" button.

## CPU Info

The code herein runs on the CPU called `m0` or `MCU` in the docs, which is the
E907 32-bit CPU. It seems like it should be possible to also send code to the
64-bit `d0` (or `MM`) CPU, but I haven't tested that yet.

## Docs/Links

- [Ox64 Wiki][ox64]
- [Pine64 Discord][pine64discord]
- [BL808 Reference Manual][bl808rm]
- [Bouffalo Loader (the same thing but in Python)][bouffalo-loader]
- [BL602 ISP Protocol][bl602isp]
- [Bouffalo Labs MCU Tool][bflb-mcu-tool]

[bl808rm]: https://files.pine64.org/doc/datasheet/ox64/BL808_RM_en_1.0(open).pdf
[bl602isp]: https://raw.githubusercontent.com/bouffalolab/bl_docs/main/BL602_ISP/en/BL602_ISP_protocol.pdf
[bouffalo-loader]: https://github.com/smaeul/bouffalo-loader
[ox64]: https://wiki.pine64.org/wiki/Ox64
[pine64discord]: https://discord.com/invite/pine64
[picoprobefork]: https://github.com/sanjay900/picoprobe
[bflb-mcu-tool]: https://pypi.org/project/bflb-mcu-tool/
