#include "mdump.h"

void custom_traversal_1(struct test *restrict root) {

    size_t resulting = 19;

    int fd;
    if ((fd = open("test.mdump", O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IREAD | S_IWRITE)) == -1) {
        fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
        exit(1);
    }

    unsigned char structure[9] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STRUCTP};
    unsigned char *ptr2 = mdump_dump_struct(root, structure, sizeof(*root), resulting);

    if (write(fd, ptr2, resulting) == -1) {
        fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
        close(fd);
        exit(1);
    }

    close(fd);

    if ((fd = open("test.mdump", O_APPEND | O_WRONLY | O_BINARY, S_IREAD | S_IWRITE)) == -1) {
        fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
        exit(1);
    }

    while (root->next != NULL) {
        root = root->next;
        ptr2 = mdump_dump_struct(root, structure, sizeof(*root), resulting);

        if (write(fd, ptr2, resulting) == -1) {
            fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
            close(fd);
            exit(1);
        }
    }
}


void custom_traversal_2(struct rb_root *restrict root) {
    size_t resulting = 28;

    int fd;
    if ((fd = open("test.mdump", O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IREAD | S_IWRITE)) == -1) {
        fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
        exit(1);
    }

    unsigned char structure[18] = {
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            STRUCTP, STRUCTP
    };

    unsigned char *ptr2 = mdump_dump_struct(root->ptr, structure, sizeof(*root->ptr), resulting);

    if (write(fd, ptr2, resulting) == -1) {
        fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
        close(fd);
        exit(1);
    }

    close(fd);

    if ((fd = open("test.mdump", O_APPEND | O_WRONLY | O_BINARY, S_IREAD | S_IWRITE)) == -1) {
        fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
        exit(1);
    }

    unsigned int i;
    for (i = 0; i < root->size; i++) {
        rb_wind_fw(root, 1);

        ptr2 = mdump_dump_struct(root->ptr, structure, sizeof(*root->ptr), resulting);

        if (write(fd, ptr2, resulting) == -1) {
            fprintf(stderr, "Failed to open file '%s'\n", "test.mdump");
            close(fd);
            exit(1);
        }
    }
}


int main(void) {

    struct rb_root *root = rb_new();
    rb_insert(root, 0, "stone");
    rb_insert(root, 1, "dune");
    rb_insert(root, 2, "coal");
    rb_insert(root, 3, "xyz");
    rb_insert(root, 4, "lol");
    rb_insert(root, 5, "lel");
    rb_insert(root, 6, "lal");
    rb_insert(root, 7, "lul");

    rb_delete(root, 3);

    custom_traversal_2(root);

    return 0;
}
