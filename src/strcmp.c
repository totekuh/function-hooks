#define _GNU_SOURCE // Enable GNU extensions
#include <dlfcn.h>  // Must come after _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int (*original_strcmp)(const char *s1, const char *s2) = NULL;

// Function to log strcmp arguments
void log_strcmp_arguments(const char *s1, const char *s2) {
    printf("HOOK: strcmp arguments:\n");
    printf("      String1: %s\n", s1);
    printf("      String2: %s\n", s2);
}

int strcmp(const char *s1, const char *s2) {
    if (!original_strcmp) {
        original_strcmp = dlsym(RTLD_NEXT, "strcmp");
    }

    // Start separator
    printf("[ :::::::::::::: Start of strcmp Hook :::::::::::::: ]\n");

    printf("HOOK: strcmp hooked!\n");
    log_strcmp_arguments(s1, s2);

    // Capture the EGG environment variable
    char *egg = getenv("EGG");

    // Check for EGG in s1 or s2
    if (egg) {
        printf("HOOK: EGG is '%s'\n", egg);
        if (strstr(s1, egg) || strstr(s2, egg)) {
            printf("HOOK: EGG found!\n");
            // Optionally, you can print the call stack here if needed
        }
    }

    // Call the original strcmp function
    int result = original_strcmp(s1, s2);

    // Log the result
    printf("HOOK: strcmp result is %d\n", result);

    // End separator
    printf("[ :::::::::::::: End of strcmp Hook :::::::::::::: ]\n");

    return result;
}

