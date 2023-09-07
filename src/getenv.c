#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

// Declare a function pointer for the original getenv
static char *(*original_getenv)(const char *name) = NULL;

// Our hook for getenv
char *getenv(const char *name) {
    // Initialize the original getenv function if it's not already
    if (!original_getenv) {
        original_getenv = dlsym(RTLD_NEXT, "getenv");
    }

    printf("[ :::::::::::::: Start of getenv Hook :::::::::::::: ]\n");

    // Log the hook
    printf("HOOK: getenv hooked!\n");
//    print_call_stack();

    // Log the name of the environment variable being fetched
    if (name) {
        printf("HOOK: Fetching environment variable: %s\n", name);
    } else {
        printf("HOOK: Fetching environment variable: NULL\n");
    }

    // Call the original getenv function
    char *result = original_getenv(name);

    // Log the result
    if (result) {
        printf("HOOK: Value of environment variable: %s\n", result);
    } else {
        printf("HOOK: Environment variable not found.\n");
    }

    printf("[ :::::::::::::: End of getenv Hook :::::::::::::: ]\n");

    return result;
}
