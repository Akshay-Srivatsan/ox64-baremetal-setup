#!/usr/bin/env python3
from bootheader import bootheader_cfg_keys

max_size = 0
for k in bootheader_cfg_keys:
    v = bootheader_cfg_keys[k]
    end = int(v["offset"]) + int(v["bitlen"])/8
    if max_size < end:
        max_size = int(end)

bits = [''] * 8*max_size

for k in bootheader_cfg_keys:
    v = bootheader_cfg_keys[k]
    offset = int(v["offset"])
    pos = int(v["pos"])
    bitlen = int(v["bitlen"])

    start = offset*8 + pos
    for i in range(0, bitlen):
        bits[start + i ] = k


print("#pragma once")
print("#include <stdint.h>")
print("struct boot_header {")

i = 0
while i < len(bits):
    start = i
    key = bits[start]
    end = start
    while end < len(bits) and bits[end] == bits[start]:
        end += 1
    length = end - start
    t = "unsigned";
    natural_width = False
    if length <= 8:
        t = "uint8_t";
        natural_width = length == 8
    elif length <= 16:
        t = "uint16_t";
        natural_width = length == 16
    elif length <= 32:
        t = "uint32_t";
        natural_width = length == 32
    elif length <= 64:
        t = "uint64_t";
        natural_width = length == 64
    if natural_width and key != "":
        print(f"  {t} {key};")
    else:
        print(f"  {t} {key} : {length};")
    i = end

print("} __attribute__((packed));");

print(f'_Static_assert(sizeof(struct boot_header) == {max_size}, "boot header must be {max_size} bytes long");')

