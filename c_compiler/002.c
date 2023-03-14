#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    // create assembly file to write to it
    int fd = open("file.asm", O_CREAT | O_TRUNC | O_RDWR);
    dup2(fd, STDOUT_FILENO);

    ////
    char *p = argv[1];
    // head of assembly file
    printf(".intel_syntax noprefix\n");
    printf(".globl _main\n");
    printf("_main:\n");
    printf("        mov rax, %ld\n", strtol(p, &p, 10));
    while (*p)
    {
        if (*p == '+')
        {
            p++;
            printf("    add rax, %ld\n", strtol(p, &p, 10));
        }
        if (*p == '-')
        {
            p++;
            printf("    sub rax, %ld\n", strtol(p, &p, 10));
        }
        fprintf(stderr, "Error bi ljaponiya %c\n", *p);
        return 1;
    }
    printf("        ret\n");
    // close assembly file
    close(fd);
    return 0;
}