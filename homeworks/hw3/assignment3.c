#include <stdio.h>
#include <stdlib.h>

void cleanup1() {
    printf("Cleanup function 1 called\n");
}

void cleanup2() {
    printf("Cleanup function 2 called\n");
}

int main() {
    if (atexit(cleanup1) != 0) {
        printf("Error: failed to register cleanup1\n");
        return 1;
    }

    if (atexit(cleanup2) != 0) {
        printf("Error: failed to register cleanup2\n");
        return 1;
    }

    printf("Main function is running\n");

    exit(0);
}