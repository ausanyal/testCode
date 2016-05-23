#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fdin, fdout;
    char *src, *dst;
    struct stat statbuf;

    if ((fdin = open("in.file", O_RDONLY)) < 0) {
        printf("Err: in.file\n");
        return -1;
    }

    if ((fdout = open("out.file", O_RDWR | O_CREAT | O_TRUNC, FILESEC_MODE)) < 0) {
        printf("Err: in.file\n");
        return -1;
    }

    if (fstat(fdin, &statbuf) < 0) {
        printf("Err: fstat \n");
        return -1;
    }

    if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0) {
        printf("Err: lseek \n");
        return -1;

    }

    if(write(fdout, "", 1) != 1) {
        printf("Err: lseek \n");
        return -1;
    }

    if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t)-1) {
        printf("Err: fdin mmap \n");
    }

    if ((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t)-1) {
        printf("Err: fdin mmap \n");
    }

    memcpy(dst, src, statbuf.st_size - 1);
    return 0;

}
