#pragma once
#include <stdarg.h>

void printk(const char *format, ...);
void vprintk(const char * format, va_list arg);
