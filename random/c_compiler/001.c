#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd = open("001.asm", O_CREAT | O_TRUNC | O_RDWR);
    dup2(fd, STDOUT_FILENO);
    if (argc != 2)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    printf(".intel_syntax noprefix\n");
    printf(".globl _main\n");
    printf("_main:\n");
    printf("        mov rax, %d\n", atoi(argv[1]));
    printf("        ret\n");
    close(fd);
    return 0;
}