#include "ft_stdlib.h"

char *ft_readline(int fd)
{
    char *res = NULL;
    char *c = ft_calloc(2, sizeof(char));
    char *tmp;

    while (1)
    {
        int n = read(fd, c, sizeof(char));
        if (n <= 0)
            break;
        tmp = ft_strjoin0(res, c);
        res = tmp;
        if (c[0] == '\n' || c[0] == '\0')
            break;
    }
    if(tmp)
        tmp[ft_strlen(tmp) - 1] = '\0'; // replace newline with /0
    return res;
}