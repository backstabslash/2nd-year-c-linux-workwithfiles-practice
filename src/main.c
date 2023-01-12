#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PERMS S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
#define BUFSIZE 512

int main(int argc, char* argv[])
{
    int fdrd, fdwr;
    char buffer[BUFSIZE];
    int nbytes = 1;
    //Source file
    if (argc == 1 || strcmp(argv[1], "-") == 0)
        fdrd = STDIN_FILENO;
    else if ((fdrd = open(argv[1], O_RDONLY)) < 0)
    {
        fprintf(stderr, "Open source file %s error: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
    //Destination file   
    if (argc <= 2 || strcmp(argv[2], "-") == 0)
        fdwr = STDOUT_FILENO;
    else
    {
        umask(0);
        if ((fdwr = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, PERMS)) < 0)
        {
            fprintf(stderr, "Open destination file %s error: %s\n", argv[2], strerror(errno));
            close(fdrd);
            exit(EXIT_FAILURE);
        }
    }
    //Write file
    int size = getsize(fdrd);
    int i = 1, totalb = 0;
    while (nbytes > 0)
    {
        if (size - BUFSIZE * i < 0)
        {
            nbytes = pread(fdrd, buffer, size - BUFSIZE * (i - 1), 0);
        }
        else
        {
            nbytes = pread(fdrd, buffer, BUFSIZE, (size - BUFSIZE * i));
        }
        i++;
        if (nbytes > 0)
        {
            char reversedbuffer[nbytes];
            int j = 0;
            for (int k = nbytes - 1; k >= 0; k--)
            {
                reversedbuffer[j] = buffer[k];
                j++;
            }
            int check = pwrite(fdwr, reversedbuffer, nbytes, totalb);
            totalb += nbytes;
            if (check < 0)
            {
                printf("Write file %s error: %s\n", argv[2], strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    }
    close(fdrd);
    close(fdwr);
    return 0;
}
int getsize(int fd)
{
    off_t curpos;
    int len = 1;
    if ((curpos = lseek(fd, 0, SEEK_CUR)) >= 0)
    {
        len = lseek(fd, 0, SEEK_END);
        lseek(fd, curpos, SEEK_SET);
    }
    return len;
}

