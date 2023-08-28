#include "ft_stdlib.h"

char *ft_readline(int fd)
{
    char *res = NULL;
    char c = 0;

    while (1)
    {
        int n = read(fd, &c, sizeof(char));
        if (n <= 0)
            break;
        if (c == '\n' || c == '\0')
            break;
        res = ft_charjoin(res, c);
    }
    return res;
}