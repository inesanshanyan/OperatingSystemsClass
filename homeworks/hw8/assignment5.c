#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int fd;
    off_t size;

    fd = open("sparse.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (write(fd, "START", 5) != 5) {
        perror("write");
        close(fd);
        return 1;
    }

    if (lseek(fd, 1024 * 1024, SEEK_CUR) < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    if (write(fd, "END", 3) != 3) {
        perror("write");
        close(fd);
        return 1;
    }

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    fd = open("sparse.bin", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    /*
     * The 1 MiB gap is a hole, not stored zero bytes. lseek reports the
     * logical size, which includes the hole. du reports allocated blocks,
     * so it stays small: only START and END occupy disk space.
     */
    printf("Apparent size: %lld\n", (long long)size);

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    return 0;
}
