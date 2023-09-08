#include <stdio.h>
#include <stdlib.h>

int main() {
    char *test_var;
    char *ld_preload_var;

    // Test 1: Fetch a common environment variable like "HOME"
    test_var = getenv("HOME");
    if(test_var) {
        printf("MAIN: Value of HOME: %s\n", test_var);
    } else {
        printf("MAIN: HOME not found.\n");
    }

    // Test 2: Fetch the LD_PRELOAD environment variable
    ld_preload_var = getenv("LD_PRELOAD");
    if(ld_preload_var) {
        printf("MAIN: Value of LD_PRELOAD: %s\n", ld_preload_var);
    } else {
        printf("MAIN: LD_PRELOAD not found (as expected).\n");
    }

    return 0;
}
