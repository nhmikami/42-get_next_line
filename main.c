#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(int ac, char **av)
{
    (void)ac;
    int fd1 = open(av[1], O_RDONLY);
    printf("%s", get_next_line(fd1));
    printf("%s", get_next_line(fd1));
    printf("%s", get_next_line(fd1));
    printf("%s", get_next_line(fd1));
    printf("%s", get_next_line(fd1));
    // int fd2 = open(av[2], O_RDONLY);
    // printf("%s", get_next_line(fd2));

    close(fd1);
}