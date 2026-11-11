#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PATH_SIZE 1024
#define BUF_SIZE 4096

static int read_line(char *buf, int size) {
    int i = 0;

    while (i < size - 1) {
        char c;
        ssize_t n = read(0, &c, 1);

        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read");
            return -1;
        }
        if (n == 0 || c == '\n') {
            break;
        }
        buf[i++] = c;
    }

    buf[i] = '\0';
    return i;
}

int main(void) {
    char src_path[PATH_SIZE];
    char dst_path[PATH_SIZE];
    char buf[BUF_SIZE];
    ssize_t n;
    long long total = 0;
    int src;
    int dst;

    printf("Enter source file path: ");
    fflush(stdout);
    if (read_line(src_path, PATH_SIZE) < 0) {
        return 1;
    }

    printf("Enter destination file path: ");
    fflush(stdout);
    if (read_line(dst_path, PATH_SIZE) < 0) {
        return 1;
    }

    src = open(src_path, O_RDONLY);
    if (src < 0) {
        perror("open source");
        return 1;
    }

    dst = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst < 0) {
        perror("open destination");
        close(src);
        return 1;
    }

    while ((n = read(src, buf, sizeof(buf))) > 0) {
        ssize_t written = 0;

        while (written < n) {
            ssize_t w = write(dst, buf + written, (size_t)(n - written));
            if (w < 0) {
                if (errno == EINTR) {
                    continue;
                }
                perror("write");
                close(src);
                close(dst);
                return 1;
            }
            written += w;
            total += w;
        }
    }

    if (n < 0) {
        perror("read");
        close(src);
        close(dst);
        return 1;
    }

    if (close(src) < 0) {
        perror("close source");
        close(dst);
        return 1;
    }
    if (close(dst) < 0) {
        perror("close destination");
        return 1;
    }

    printf("Copied %lld bytes\n", total);
    return 0;
}
