#ifndef MDUMP_MDUMP_H
#define MDUMP_MDUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "stack/stack.h"
#include "ringbuffer/ringbuffer.h"

#ifdef __unix__
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
#elif __WIN32__ || _MSC_VER
    #include <sys\stat.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

#define VALNUL(x) if (NULL == (x)) {fprintf(stderr, #x " in function %s is NULL, exiting!\n", __func__);exit(1);}
#define NEW(type, name, amount) \
    type *name = (type *) calloc(amount, sizeof(*name)); \
    VALNUL(name)

#define EMPTY 0x00

#define CHAR 0x01
#define UCHAR 0x02
#define SHORT 0x03
#define USHORT 0x04
#define INT 0x05
#define UINT 0x06
#define LONGLONG 0x07
#define ULONGLONG 0x08

#define CHARP 0x09
#define UCHARP 0x0a
#define SHORTP 0x0b
#define USHORTP 0x0c
#define INTP 0x0d
#define UINTP 0x0e
#define LONGLONGP 0x0f
#define ULONGLONGP 0x10

#define STRUCT 0x01
#define STRUCTP 0x11

struct test {
    unsigned int size;
    struct test *next;
};

unsigned char *mdump_dump_exact(void *restrict root_node, const unsigned char *restrict structure, size_t struct_size, unsigned int var_count);
unsigned char *mdump_dump_struct(void *restrict root_node, const unsigned char *restrict structure, size_t struct_size, size_t resulting);

void print_hex(const unsigned char *restrict ptr, size_t size);

#endif
