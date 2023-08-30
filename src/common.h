#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <ctype.h>

#define MAX_STACK_FRAMES 64

void print_call_stack(void);
void print_hex_data(void *data, size_t len);
void print_ascii_data(void *data, size_t len);

#endif // COMMON_H
