#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Please provide 1 argument\n");
        exit(-1);
    }

    char *arg = argv[1];
    // printf("arg: %s\n\n", arg);
    printf(".intel_syntax noprefix\n");
    printf(".global _main\n");
    printf("_main:\n");
    int i = 0;
    printf("    mov rax, %d\n", atoi(arg + i));
    i++;
    while (arg[i])
    {
        // while(arg[i] == ' ')
        //     i++;
        if (arg[i] == '+')
            printf("    add rax, %d\n", atoi(arg + i + 1));
        else if (arg[i] == '-')
            printf("    sub rax, %d\n", atoi(arg + i + 1));
        i++;
    }
    printf("    ret\n");
    return 0;
}
