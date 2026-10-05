#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = malloc(10 * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter 10 integers: ");
    for (int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }

    int *resized = realloc(arr, 5 * sizeof(int));

    if (resized == NULL) {
        printf("Memory reallocation failed\n");
        free(arr);
        exit(EXIT_FAILURE);
    }

    arr = resized;

    printf("Array after resizing: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);

    return 0;
}
