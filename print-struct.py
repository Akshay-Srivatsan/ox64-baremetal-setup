#!/usr/bin/env python3

import sys

if len(sys.argv) != 2:
    print(f"usage: {sys.argv[0]} file")
    exit(1)

from bootheader import bootheader_cfg_keys

data = None
with open(sys.argv[1], 'rb') as f:
    data = f.read()

data = bytearray(data)

for k in bootheader_cfg_keys:
    v = bootheader_cfg_keys[k]
    offset = int(v['offset'])
    assert(offset % 4 == 0)
    pos = int(v['pos'])
    bitlen = int(v['bitlen'])
    x = int.from_bytes(data[offset:offset+4], "little")
    x = (x >> pos) & ((1 << bitlen) - 1)
    print(f'.{k} = 0x{x:x},')
