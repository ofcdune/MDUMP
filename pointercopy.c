#include "mdump.h"

int ptrcpy(void) {

    unsigned int test = 0x88;
    unsigned int *test_ptr = &test;

    unsigned char buffer[8];

    memcpy(buffer, &test_ptr, 8);

    void *ptr = NULL;
    unsigned char *helper;

    helper = (unsigned char *) &ptr;

    printf("%p\n", ptr);

    helper[0] =  buffer[0];
    helper[1] =  buffer[1];
    helper[2] =  buffer[2];
    helper[3] =  buffer[3];
    helper[4] =  buffer[4];
    helper[5] =  buffer[5];
    helper[6] =  buffer[6];
    helper[7] =  buffer[7];

    puts("here 2");

    printf("%p\n", ptr);

    printf("%2x\n", * (unsigned int *) ptr);

    return 0;
}

void *pointer_copy(void *restrict root_node, unsigned char offset) {
    void *ptr = NULL;
    unsigned char *helper, *helper2;

    helper = (unsigned char *) &ptr;
    helper2 = (unsigned char *) root_node;

    int helper2_index = offset;

    int i;
    for (i = 0; i < 8; i++) {
        helper[i] = helper2[helper2_index++];
    }

    return ptr;
}