#include "common.h"
#include <string.h>
#include <stdlib.h> // for getenv
#include <signal.h> // for raise

static void *(*original_memcpy)(void *dest, const void *src, size_t n) = NULL;

int BREAK = 0; // Set to 1 to pause, 0 to continue (disabled by default)

// Function to log content
void log_content(const void *data, size_t size, const char *description) {
    if (size > 0) {
        printf("HOOK: %s in HEX: ", description);
        print_hex_data(data, size);
        printf("HOOK: %s in ASCII: ", description);
        print_ascii_data(data, size);
    }
}

// Function to log memcpy arguments
void log_memcpy_arguments(void *dest, const void *src, size_t n) {
    printf("HOOK: memcpy arguments:\n");
    printf("      Destination address: %p\n", dest);
    printf("      Source address:      %p\n", src);
    printf("      Number of bytes:     %zu\n", n);
}

void *memcpy(void *dest, const void *src, size_t n) {
    if (!original_memcpy) {
        original_memcpy = dlsym(RTLD_NEXT, "memcpy");
    }

    // Fetch the EGG environment variable
    char *egg = getenv("EGG");
    BREAK = getenv("BREAK");

    // Start separator
    printf("[ :::::::::::::: Start of memcpy Hook :::::::::::::: ]\n");

    printf("HOOK: memcpy hooked!\n");

    // Check for EGG in source or destination
    int egg_found = 0;
    if (egg) {
        printf("HOOK: EGG is '%s'\n", egg);
        if (memmem(src, n, egg, strlen(egg)) || memmem(dest, n, egg, strlen(egg))) {
            egg_found = 1;
            printf("HOOK: EGG found!\n");
            print_call_stack();
            log_memcpy_arguments(dest, src, n);
            log_content(dest, n, "Destination content before copying");
            log_content(src, n, "Content being copied from source");
        }
    } else {
        print_call_stack();
        log_memcpy_arguments(dest, src, n);
        log_content(dest, n, "Destination content before copying");
        log_content(src, n, "Content being copied from source");
    }

    // Pause if BREAK is 1 and an egg is found or there's no egg
    if (BREAK && (egg_found || !egg)) {
        printf("Press Enter to continue...\n");
        getchar(); // Wait for Enter key
    }

    // Call the original memcpy function
    original_memcpy(dest, src, n);

    if (egg) {
        if (egg_found) {
            log_content(dest, n, "Destination content after copying");
        }
    } else {
        log_content(dest, n, "Destination content after copying");
    }

    // End separator
    printf("[ :::::::::::::: End of memcpy Hook :::::::::::::: ]\n");

    return dest;
}
