#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void *aligned_malloc(size_t size, size_t alignment) {
    size_t total_size = size + alignment - 1 + sizeof(void *);
    void *raw = malloc(total_size);

    if (raw == NULL) {
        return NULL;
    }

    uintptr_t raw_addr = (uintptr_t)raw + sizeof(void *);
    uintptr_t aligned_addr = (raw_addr + alignment - 1) & ~(alignment - 1);

    ((void **)aligned_addr)[-1] = raw;

    return (void *)aligned_addr;
}

void aligned_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    void *raw = ((void **)ptr)[-1];
    free(raw);
}

int main() {
    size_t alignment = 64;
    size_t size = 10 * sizeof(int);

    int *data = aligned_malloc(size, alignment);

    if (data == NULL) {
        printf("Aligned allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Address: %p\n", (void *)data);
    printf("Aligned to %zu: %s\n", alignment,
           ((uintptr_t)data % alignment == 0) ? "yes" : "no");

    for (int i = 0; i < 10; i++) {
        data[i] = i;
    }

    printf("Values: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    aligned_free(data);

    return 0;
}
