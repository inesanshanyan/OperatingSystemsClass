**Dynamic Memory Allocation Report**

**Assignment 1**

The program asks for n, then calls malloc to request a heap block large enough for n integers. malloc returns a pointer to the start of that block, or NULL if the request fails. The program checks for NULL, reads the values into the block, adds them, and prints the sum.

free(arr) tells the allocator that this block is no longer needed. The pointer itself is not erased, but it must not be used after free.

**Assignment 2**

calloc also allocates a block, but it sets every byte to zero before returning. That is why the first print shows all zeros. After the user overwrites those zeros, the program adds the values as a double and divides by n, so the average can have a decimal part. The block is then released with free.

**Assignment 3**

The program first allocates space for 10 integers and reads them. realloc then asks the allocator to change that block to 5 integers. When the size shrinks, the first 5 values are kept and the rest are dropped. realloc may return the same address or move the data to a new block.

If realloc succeeds, the old pointer must not be used, so the program stores the new pointer in arr. If it fails, the original block is still valid, so the program frees arr and exits.

**Assignment 4**

This uses two levels of allocation. malloc first creates an array of 3 char pointers. Then each string gets its own 50-byte block. The pointer array and the string data are separate heap blocks.

realloc grows only the array of pointers, from 3 to 5. It does not create the new string buffers, so the program allocates 50 bytes for each new slot and reads two more strings. At the end, each string is freed first, and then the pointer array is freed. The order matters: after free(strings), the addresses of the strings would be lost.

**Assignment 5**

The number of students is not known at compile time, so malloc creates an int array of that size. The program starts with the first grade as both the highest and the lowest, then walks through the rest of the array and updates those two values. After printing them, free returns the block to the allocator.

**Assignment 6**

my_realloc does not resize the original block in place. If ptr is NULL, it behaves like malloc. If new_size is 0, it frees the block and returns NULL. Otherwise it allocates a new block, copies the smaller of the two sizes with memcpy, frees the old block, and returns the new pointer.

The caller must pass the old size because malloc does not give that size back later. Bytes beyond the copied part are not initialized, so main sets the extra elements to 0. Unlike the real realloc, this version always copies and always uses a new block.

**Assignment 7**

Some uses need an address that is a multiple of a given alignment, here 64. malloc does not guarantee that, so aligned_malloc requests extra space: the needed size, plus alignment - 1 so the address can be rounded up, plus room for one saved pointer.

It rounds the address up with a bitmask, stores the original malloc pointer just before the aligned address, and returns the aligned pointer. aligned_free reads that saved pointer and frees the original block. A normal free on the aligned pointer would be wrong, because that address was not returned by malloc.

GitHub link: <https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw5>
