#include "common.h"

void print_call_stack(void) {
    void *buffer[MAX_STACK_FRAMES];
    char **stack_strings;
    int nptrs;

    nptrs = backtrace(buffer, MAX_STACK_FRAMES);
    stack_strings = backtrace_symbols(buffer, nptrs);

    if (stack_strings == NULL) {
        perror("backtrace_symbols");
        exit(1);
    }

    printf("HOOK: Call stack:\n");
    for (int i = 1; i < nptrs; i++) { // Start from 1 to skip the current frame
        printf("  %s\n", stack_strings[i]);
    }

    free(stack_strings);
}

void print_hex_data(void *data, size_t len) {
    unsigned char *byte_data = (unsigned char *)data;
    for (size_t i = 0; i < len; i++) {
        printf("\\x%02x", byte_data[i]);
    }
    printf("\n");
}

void print_ascii_data(void *data, size_t len) {
    unsigned char *byte_data = (unsigned char *)data;
    for (size_t i = 0; i < len; i++) {
        printf("%c", isprint(byte_data[i]) ? byte_data[i] : '.');
    }
    printf("\n");
}
