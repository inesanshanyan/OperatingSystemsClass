#include <stdio.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int first = 10;
    int second = 20;

    printf("Before swap: first = %d, second = %d\n", first, second);

    swap(&first, &second);

    printf("After swap: first = %d, second = %d\n", first, second);

    return 0;
}