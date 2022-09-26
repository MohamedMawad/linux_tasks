#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_buf_SIZE 200

int main(int argc, char **argv)
{
    char buf[MAX_buf_SIZE] = {0};
    char *ret_ptr = NULL;

    ret_ptr = getcwd(buf, MAX_buf_SIZE);

    if (ret_ptr == NULL)
    {
        printf("Error: pathname size exceeds buf size bytes \n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", ret_ptr);

	return 0;

}
