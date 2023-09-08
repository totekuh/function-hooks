#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // Log the name of the environment variable being fetched
    if (name) {
        printf("HOOK: Fetching environment variable: %s\n", name);
    } else {
        printf("HOOK: Fetching environment variable: NULL\n");
    }

    char *ld_preload_var_name = "LD_PRELOAD";
    if (strcmp(name, ld_preload_var_name) == 0) {
        printf("HOOK: LD_PRELOAD query detected. Pretending it's not defined.\n");
        printf("[ :::::::::::::: End of getenv Hook :::::::::::::: ]\n");
        return NULL;
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
