#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

int main(int argc, char *argv[])
{
    long long kt;
    char buf[100];
    char filename[20] = "test.txt";
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */
    if (argc >= 2)
        snprintf(filename, 20, "%s.txt", argv[1]);
    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= offset; i++) {
        kt = write(fd, write_buf, strlen(write_buf));
        printf("Writing to " FIB_DEV ", returned the sequence %lld\n", kt);
    }
    FILE *f;
    f = fopen(filename, "w");
    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        kt = read(fd, buf, 100);
        fprintf(f, "%d %lld\n", i, kt);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.\n",
               i, buf);
    }
    fprintf(f, "\n");
    fclose(f);

    for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        kt = read(fd, buf, 100);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.\n",
               i, buf);
    }

    close(fd);
    return 0;
}
