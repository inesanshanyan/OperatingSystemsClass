#include <stdio.h>

int main() {
    int numbers[5] = {6, 9, 76, 100, 46};
    int length = 5; // 
    int *ptr = numbers;

    printf("Original array:\n");

    for (int i = 0; i < length; i++) {
        printf("%d ", *(ptr + i));
    }

    printf("\n");

    for (int i = 0; i < length; i++) {
        *(ptr + i) = *(ptr + i) + 10;
    }

    printf("Modified array using pointer:\n");

    for (int i = 0; i < length; i++) {
        printf("%d ", *(ptr + i));
    }

    printf("\n");

    printf("Modified array using array name:\n");

    for (int i = 0; i < length; i++) {
        printf("%d ", numbers[i]);
    }

    printf("\n");

    return 0;
}