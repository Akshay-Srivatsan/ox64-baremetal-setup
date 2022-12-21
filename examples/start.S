.section ".text.boot"

.globl _start
_start:
  la sp, __stack_top__
.option push
.option norelax
  la gp, __global_pointer$
.option pop
  call _cstart
halt:
  nop
  j halt

.globl put32
.globl PUT32
put32:
PUT32:
  fence rw,rw
  sw a1, 0(a0)
  fence rw,rw
  ret

.globl get32
.globl GET32
get32:
GET32:
  fence rw,rw
  lw a1, 0(a0)
  ret
