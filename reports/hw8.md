**File I/O Report**

The programs in [homeworks/hw8](../homeworks/hw8) use `open`, `read`, `write`, `lseek`, and `ftruncate`. File data is not moved with `fread` or `fwrite`. When a path or a log line has to come from the user, it is read with `read(0, ...)`.

**Assignment 1**

[assignment1.c](../homeworks/hw8/assignment1.c) copies one file to another. It asks for the source path, then the destination path. The source is opened with `O_RDONLY`. The destination is opened with `O_WRONLY|O_CREAT|O_TRUNC` and mode `0644`, so the file is created if it does not exist and emptied if it does.

The copy loop reads up to 4096 bytes and writes them until `read` returns 0. That return value is EOF. `write` is allowed to store only part of the buffer, so the inner loop continues until every byte from that read has been written. The program then prints how many bytes were copied. If `open`, `read`, `write`, or `close` fails, `perror` prints the reason and the program stops.

```
Enter source file path: src.txt
Enter destination file path: dst.txt
Copied 18 bytes
```

**Assignment 2**

[assignment2.c](../homeworks/hw8/assignment2.c) uses the fixed name `data.txt`. It creates the file, writes `ABCDEFGHIJKLMNOPQRSTUVWXYZ` (26 bytes), and closes it. The file is opened again with `O_RDWR`. `lseek(fd, 0, SEEK_END)` reports the size, which is 26. `ftruncate(fd, 10)` keeps the first 10 bytes. A second seek to the end reports 10. The offset is moved back to 0 and the remaining bytes are printed.

```
Current size: 26
New size: 10
ABCDEFGHIJ
```

`ftruncate` changes the length. It does not rewind the file, so the read has to seek to the start first.

**Assignment 3**

[assignment3.c](../homeworks/hw8/assignment3.c) asks for a path and opens that file read-only. `lseek(fd, 0, SEEK_END)` gives the size. Starting at the last byte, the program seeks to that position, reads one byte, and writes it to standard output. It repeats this until the first byte has been printed, then writes a newline. An empty file has size 0, so the loop does not run and only the newline is printed.

A file that contains `abc` prints `cba` and then a newline.

**Assignment 4**

[assignment4.c](../homeworks/hw8/assignment4.c) opens `log.txt` with `O_WRONLY|O_CREAT|O_APPEND` and mode `0644`. One line is read from standard input with `read`, not `fgets`. The written text is `PID=<pid>: ` plus that line. A newline is added only when the typed line did not already end with one. After the write, `lseek(fd, 0, SEEK_CUR)` prints the new offset.

`O_APPEND` does not freeze the offset at 0. It only forces each `write` to begin at the current end of the file. When the write finishes, the kernel moves the offset to the byte just past the new data. `SEEK_CUR` reads that offset, so it grows on every append.

```
Enter a log line: hello
File offset: 16
```

A second run with `second` printed offset 33. The file then held:

```
PID=4498: hello
PID=4500: second
```

The PID changes every run. The offset grows by the number of bytes just appended.

**Assignment 5**

[assignment5.c](../homeworks/hw8/assignment5.c) creates `sparse.bin`, writes the 5 bytes `START`, then calls `lseek(fd, 1024*1024, SEEK_CUR)`. That skips 1 MiB from the current position without writing zeros. It then writes `END` and closes the file. Reopening it and seeking to the end reports the logical size:

```
Apparent size: 1048584
```

That is `5 + 1048576 + 3`, which is greater than 1,048,581. The gap is a hole. The filesystem does not store a megabyte of zero bytes for it. `du` counts allocated blocks, so on a Linux filesystem the disk use stays small. A test under `/tmp` showed `du` at 8K while `lseek` still reported 1048584. A Windows drive mounted in WSL may allocate the hole anyway, so `du` can look large there even though the apparent size is the same.

**Assignment 6**

[assignment6.c](../homeworks/hw8/assignment6.c) writes lines `1` through `10` into `numbers.txt`, one number per line. It reopens the file with `O_RDWR` and scans until it has seen three newlines. The offset at that point is the start of line 4. The offset is found by counting newlines, not by hard-coding it.

Line 4 is `4\n`, which is 2 bytes. The replacement is `100\n`, which is 4 bytes, so the rest of the file has to move. The program reads everything after the start of line 4 into a buffer, seeks back, writes `100\n`, then writes the saved bytes that followed the old line. `ftruncate` drops any old tail that would be left if the new text were shorter. The printed file is:

```
1
2
3
100
5
6
7
8
9
10
```

**Assignment 7**

[assignment7.c](../homeworks/hw8/assignment7.c) asks for two paths and opens both read-only. It reads both files in 4096-byte chunks and compares them byte by byte. The first mismatch is printed as `Files differ at byte N`, where `N` starts at 0, and the program exits with status 1. If one file ends while the other still has bytes, and everything before that matched, the difference is the length of the shorter file. Equal files, including two empty files, print `Files are identical` and exit with status 0.

`abc` and `abd` differ at byte 2. `abc` and `abcd` differ at byte 3.

**How to run**

```bash
cd homeworks/hw8
gcc -Wall -Wextra -std=c11 -o assignment1 assignment1.c
./assignment1
```

The other six files compile the same way. Assignments 2, 4, 5, and 6 create `data.txt`, `log.txt`, `sparse.bin`, and `numbers.txt` in the current directory.

Github link: [https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw8](https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw8)
