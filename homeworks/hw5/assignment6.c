#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *my_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (ptr == NULL) {
        return malloc(new_size);
    }

    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    void *new_ptr = malloc(new_size);

    if (new_ptr == NULL) {
        return NULL;
    }

    size_t copy_size = old_size < new_size ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);

    free(ptr);

    return new_ptr;
}

int main() {
    int old_n = 5;
    int *arr = malloc(old_n * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter %d integers: ", old_n);
    for (int i = 0; i < old_n; i++) {
        scanf("%d", &arr[i]);
    }

    int new_n = 8;
    int *resized = my_realloc(arr, old_n * sizeof(int), new_n * sizeof(int));

    if (resized == NULL) {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }

    arr = resized;

    for (int i = old_n; i < new_n; i++) {
        arr[i] = 0;
    }

    printf("Array after my_realloc: ");
    for (int i = 0; i < new_n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);

    return 0;
}
