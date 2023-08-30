#include "common.h"
#include <string.h>

static void *(*original_memcpy)(void *dest, const void *src, size_t n) = NULL;

void *memcpy(void *dest, const void *src, size_t n) {
    if (!original_memcpy) {
        original_memcpy = dlsym(RTLD_NEXT, "memcpy");
    }

    // Start separator
    printf("[ :::::::::::::: Start of memcpy Hook :::::::::::::: ]\n");

    // Log the hook
    printf("HOOK: memcpy hooked!\n");
    print_call_stack();

    // Log the arguments
    printf("HOOK: memcpy arguments:\n");
    printf("      Destination address: %p\n", dest);
    printf("      Source address:      %p\n", src);
    printf("      Number of bytes:     %zu\n", n);

    // Log the destination content before copying
    if (n > 0) {
        printf("HOOK: Destination content before copying in HEX: ");
        print_hex_data(dest, n);
        printf("HOOK: Destination content before copying in ASCII: ");
        print_ascii_data(dest, n);
    }

    // Log the source content
    if (n > 0) {
        printf("HOOK: Content being copied from source in HEX: ");
        print_hex_data(src, n);
        printf("HOOK: Content being copied from source in ASCII: ");
        print_ascii_data(src, n);
    }

    // Call the original memcpy function
    original_memcpy(dest, src, n);

    // Log the destination content after copying
    if (n > 0) {
        printf("HOOK: Destination content after copying in HEX: ");
        print_hex_data(dest, n);
        printf("HOOK: Destination content after copying in ASCII: ");
        print_ascii_data(dest, n);
    }

    // End separator
    printf("[ :::::::::::::: End of memcpy Hook :::::::::::::: ]\n");

    return dest;
}
