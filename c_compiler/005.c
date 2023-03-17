#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr,"Please provide 1 argument\n");
        exit(-1);
    }
    printf(".intel_syntax noprefix\n");
    printf(".global _main\n");
    printf("_main:\n");
    printf("    mov rax, %d\n", atoi(argv[1]));
    printf("    ret\n");
    return (0);
}