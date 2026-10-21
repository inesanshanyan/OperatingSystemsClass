**Data Alignment Report**

**Assignment 1**

The program declares an `int`, a `char`, a `double`, and a `short`, then prints `sizeof` and the address of each variable.

On this machine the sizes and alignment requirements are:

```
char   size = 1  alignment = 1
short  size = 2  alignment = 2
int    size = 4  alignment = 4
double size = 8  alignment = 8
```

Alignment means the address of a value should be a multiple of that number. A `double` should start at an address divisible by 8, an `int` by 4, and a `short` by 2. A `char` can start anywhere.

One run printed:

```
int    i = 0x7ffee691eee8
char   c = 0x7ffee691eee5
double d = 0x7ffee691eef8
short  s = 0x7ffee691eee6
```

The absolute addresses change every time the program runs, because these are local variables on the stack. What stays the same is the alignment. Dividing each address by the type's alignment leaves a remainder of 0, so every variable is placed on a legal boundary.

The compiler also did not keep declaration order. Sorted by address, this run was `char`, then `short`, then `int`, then `double`. There can also be unused bytes between them. Those gaps are not part of the variables. They exist so the next variable can start at an address its type is allowed to use.

**Assignment 2**

The three fields by themselves are `1 + 4 + 8 = 13` bytes. The struct is larger than that because the compiler inserts padding.

Order 1 is `char`, `int`, `double`:

```
sizeof = 16
a offset = 0
b offset = 4
c offset = 8
```

`char` is at offset 0. The next field is an `int`, which needs an offset divisible by 4, so 3 unused bytes are inserted. The `double` then starts at offset 8, which is divisible by 8. The total is 16, a multiple of 8, so an array of this struct would also keep every `double` aligned.

Order 2 puts `double` first, then `int`, then `char`:

```
sizeof = 16
c offset = 0
b offset = 8
a offset = 12
```

This order does not reduce the size. It is still 16. The padding just moves. `double` and `int` no longer need bytes between them, but `char` ends at offset 13, and the struct alignment is still 8. The compiler adds 3 bytes at the end so the size is a multiple of 8.

Order 3, `char` then `double` then `int`, does increase the size:

```
sizeof = 24
a offset = 0
c offset = 8
b offset = 16
```

`double` cannot start at offset 1, so 7 bytes are inserted after the `char`. The `int` fits at offset 16, and 4 more bytes are added at the end to reach a multiple of 8. The same 13 bytes of data now occupy 24 bytes.

Changing the field order can increase the struct size when a large field is forced to skip ahead to its next alignment boundary. It does not always reduce the size. For the two orders in the assignment, the size stayed 16 because both layouts already waste only 3 bytes, and 16 is the smallest multiple of 8 that can hold 13 bytes.

**Assignment 3**

The same field order, `char`, `int`, `double`, is compiled four times. `#pragma pack(n)` tells the compiler that a field's alignment cannot be larger than `n`. `#pragma pack()` after each struct restores the normal rules, so one experiment does not change the next one.

Without packing:

```
sizeof = 16  alignment = 8
a offset = 0
b offset = 4
c offset = 8
padding before b = 3, before c = 0, after c = 0
```

This is the same layout as order 1 in assignment 2. Three padding bytes make the `int` start at a multiple of 4.

With `#pragma pack(1)`:

```
sizeof = 13  alignment = 1
a offset = 0
b offset = 1
c offset = 5
padding before b = 0, before c = 0, after c = 0
```

Packing at 1 removes every padding byte. The struct is exactly 13 bytes, and its own alignment drops to 1. In the sample run, `a` was at `0x7ffc6b7ffdb5`, so `b` started at an odd address plus one, which is not divisible by 4, and `c` was not divisible by 8. The data is packed tightly, but the `int` and `double` are no longer naturally aligned. On x86-64 that is allowed, but those accesses can be slower. On some other architectures an unaligned access is not allowed at all.

With `#pragma pack(2)`:

```
sizeof = 14  alignment = 2
a offset = 0
b offset = 2
c offset = 6
padding before b = 1, before c = 0, after c = 0
```

Each field is aligned to the smaller of its normal alignment and 2. The `int` would normally want 4, but packing limits that to 2, so one padding byte is enough. The `double` starts at offset 6. That is a multiple of 2, not of 8. The size is 14.

With `#pragma pack(4)`:

```
sizeof = 16  alignment = 4
a offset = 0
b offset = 4
c offset = 8
padding before b = 3, before c = 0, after c = 0
```

The offsets match the unpacked struct, so the size stays 16. The difference is the alignment of the struct itself: 4 instead of 8. The compiler only promises that the struct starts at a multiple of 4. Offset 8 is a multiple of 8 only if the struct itself starts at a multiple of 8. Under `pack(4)` that is no longer guaranteed, so the `double` can end up only 4-byte aligned.

Packing trades space for alignment. `pack(1)` uses the least memory and ignores the normal boundaries. `pack(2)` is in between. `pack(4)` does not shrink this particular field order, but it weakens the alignment promise for `double`. The normal layout uses 3 extra bytes so every field, and every later element in an array of the struct, stays on its natural boundary.

GitHub link: <https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw6>
