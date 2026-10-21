#include <stdio.h>
#include <stdint.h>

int main(void) {
    int i = 1;
    char c = 'A';
    double d = 3.14;
    short s = 2;

    printf("Size and alignment of each type:\n");
    printf("char   size = %zu  alignment = %zu\n", sizeof(c), _Alignof(char));
    printf("short  size = %zu  alignment = %zu\n", sizeof(s), _Alignof(short));
    printf("int    size = %zu  alignment = %zu\n", sizeof(i), _Alignof(int));
    printf("double size = %zu  alignment = %zu\n", sizeof(d), _Alignof(double));

    printf("\nAddresses of the variables:\n");
    printf("int    i = %p\n", (void *)&i);
    printf("char   c = %p\n", (void *)&c);
    printf("double d = %p\n", (void *)&d);
    printf("short  s = %p\n", (void *)&s);

    const char *names[4] = {"int i", "char c", "double d", "short s"};
    void *addrs[4] = {&i, &c, &d, &s};

    for (int left = 0; left < 3; left++) {
        for (int right = left + 1; right < 4; right++) {
            if (addrs[right] < addrs[left]) {
                void *tmp_addr = addrs[left];
                const char *tmp_name = names[left];
                addrs[left] = addrs[right];
                names[left] = names[right];
                addrs[right] = tmp_addr;
                names[right] = tmp_name;
            }
        }
    }

    printf("\nSame variables ordered by address:\n");
    for (int n = 0; n < 4; n++) {
        printf("%s = %p\n", names[n], addrs[n]);
    }

    printf("\nEach address modulo the type alignment (0 means the variable is aligned):\n");
    printf("i %% %zu = %lu\n", _Alignof(int), (unsigned long)((uintptr_t)&i % _Alignof(int)));
    printf("c %% %zu = %lu\n", _Alignof(char), (unsigned long)((uintptr_t)&c % _Alignof(char)));
    printf("d %% %zu = %lu\n", _Alignof(double), (unsigned long)((uintptr_t)&d % _Alignof(double)));
    printf("s %% %zu = %lu\n", _Alignof(short), (unsigned long)((uintptr_t)&s % _Alignof(short)));

    printf("\nValues (printed so the variables are actually used): %d %c %.2f %d\n", i, c, d, s);

    return 0;
}
