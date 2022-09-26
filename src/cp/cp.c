#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_buf_SIZE 200


int main(int argc, char *argv[])
{
        char buf[MAX_buf_SIZE];
        int fd1, fd2;
        int count = 1;
        int status;

        if(argc != 3)
        {
            printf("Please enter src & dist files\n");
            exit(EXIT_FAILURE);
        }

        fd1 = open(argv[1], O_RDONLY, 0644);
        fd2 = open(argv[2], O_WRONLY|O_CREAT, 0644);

        if (fd1 == -1 || fd2 == -1)
        {
            exit(EXIT_FAILURE);
        }  

        do
        {
            count = read(fd1, buf, MAX_buf_SIZE);
            if (count == -1)
            {
                close(fd1);
                close(fd2);
                exit(EXIT_FAILURE);
            }  
            status = write(fd2, buf, count);
            if (status == -1)
            {
                close(fd1);
                close(fd2);
                exit(EXIT_FAILURE);
            } 
        }
        while (count != 0);

        close(fd1);
        close(fd2);

        return 0;
}
