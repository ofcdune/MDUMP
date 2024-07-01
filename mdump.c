#include "mdump.h"

void print_hex(const unsigned char *restrict ptr, size_t size) {

    const unsigned char hex[] = "0123456789ABCDEF";

    unsigned char mask = 0xf0;
    size_t i;
    for (i = 0; i < size; i++) {
        putchar(hex[(ptr[i] & mask) >> 4]);
        putchar(hex[ptr[i] & (mask >> 4)]);
        putchar(' ');
    }
    putchar('\n');
}

unsigned char *mdump_dump_exact(void *restrict node, const unsigned char *restrict structure, size_t struct_size, unsigned int var_count) {

    size_t resulting = var_count+(9*struct_size);

    unsigned char *buffer = (unsigned char *) calloc(resulting, sizeof(*buffer));
    if (NULL == buffer) {
       fputs("Failed to allocate buffer\n", stderr);
       exit(1);
    }

    unsigned char *ptr = node;
    unsigned char size;
    size_t index = 0, j = 0;
    unsigned int structure_index = 0;

    unsigned char i;

    while (index < resulting) {

        switch (structure[structure_index]) {

            case EMPTY:
            case CHAR:
            case UCHAR:
                size = 1;
                break;

            case SHORT:
            case USHORT:
                size = 2;
                break;

            case INT:
            case UINT:
                size = 4;
                break;

            case LONGLONG:
            case ULONGLONG:
            case CHARP:
            case UCHARP:
            case SHORTP:
            case USHORTP:
            case INTP:
            case UINTP:
            case LONGLONGP:
            case ULONGLONGP:
                size = 8;
                break;

            default:
                size = 0;
                break;
        }

        if (size == 0) {
            continue;
        }

        buffer[index++] = structure[structure_index];
        for (i = 0; i < size; i++) {
            buffer[index++] = *(unsigned char *) (ptr + j);
            memcpy(buffer + index, (ptr + j), 8);

            print_hex((ptr + j), 8);

            index += 8;
            j += 8;
        }

        structure_index++;
    }

    return buffer;
}

unsigned char *mdump_dump_struct(void *restrict root_node, const unsigned char *restrict structure, size_t struct_size, size_t resulting) {

    unsigned char *buffer = (unsigned char *) calloc(resulting, sizeof(*buffer));
    if (NULL == buffer) {
        fputs("Failed to allocate buffer\n", stderr);
        exit(1);
    }

    unsigned char *ptr = root_node;
    unsigned char size = 0;
    size_t index = 0, j = 0;
    unsigned int structure_index = 0;

    unsigned char i;

    // initialize struct header
    buffer[index++] = STRUCT;
    buffer[index++] = struct_size;

    memcpy(buffer + index, &ptr, 8);

    index += 8;

    while (index < resulting) {

        if (structure[structure_index] == EMPTY) {
            structure_index++;
            j++;
            continue;
        }

        buffer[index++] = STRUCTP;

        for (i = 0; i < 8; i++) {
            buffer[index++] = *(unsigned char *) (ptr + j);
            j++;
        }

    }

    return buffer;
}

unsigned char *mdump_dump_data_structure(void *restrict root_node, const unsigned char *restrict structure, size_t struct_size, unsigned int ptr_count) {

    size_t resulting = 10 + (9 * ptr_count);
    stack *stack_root = initialize_stack();

    push(root_node, stack_root);
    void *helper;

    unsigned char *buffer = (unsigned char *) calloc(resulting, sizeof(*buffer));
    if (NULL == buffer) {
        fputs("Failed to allocate buffer\n", stderr);
        exit(1);
    }
    size_t buffer_index = 0;

    unsigned char *buffer_resulting;

    while (!is_empty_stack(stack_root)) {
        helper = pop(stack_root);
        buffer_resulting = mdump_dump_struct(helper, structure, struct_size, resulting);
    }

    return NULL;
}
