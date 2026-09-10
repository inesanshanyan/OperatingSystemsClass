#include <stdio.h>

int main() {
    char str[] = "Hello";
    char *ptr = str;

    printf("String: ");

    while (*ptr != '\0') {
        printf("%c", *ptr);
        ptr++;
    }

    printf("\n");

    ptr = str;

    int length = 0;

    while (*ptr != '\0') {
        length++;
        ptr++;
    }

    printf("Number of characters: %d\n", length);

    return 0;
}