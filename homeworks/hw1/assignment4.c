#include <stdio.h>

int main() {
    int number = 10;
    int *ptr = &number;
    int **double_ptr = &ptr;

    printf("Value using pointer: %d\n", *ptr);
    printf("Value using double-pointer: %d\n", **double_ptr);

    return 0;
}