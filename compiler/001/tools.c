#include "header.h"

int ft_strlen(char *str)
{
    int i = 0;
    while (str && str[i])
        i++;
    return i;
}

void ft_putchar(int fd, char c)
{
    write(fd, &c, sizeof(char));
}

void ft_putstr(int fd, char *str)
{
    int i = 0;
    while (str && str[i])
        ft_putchar(fd, str[i++]);
}

void ft_putnbr(int fd, long num)
{
    if (num < 0)
    {
        ft_putchar(fd, '-');
        num = -num;
    }
    if (num < 10)
        ft_putchar(fd, num + '0');
    else
    {
        ft_putnbr(fd, num / 10);
        ft_putnbr(fd, num % 10);
    }
}

void print_space(int fd, int line_long)
{
    int i = 0;
    while (i < line_long)
    {
        ft_putchar(fd, ' ');
        i++;
    }
}
int len_long(long num)
{
    int res = 0;
    if (num < 0)
    {
        res++;
        num = -num;
    }
    if (num >= 0)
        res++;
    while (num >= 10)
    {
        num /= 10;
        res++;
    }
    return (res);
}

void ft_printf(int fd, char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    int i = 0;
    while (fmt && fmt[i])
    {
        if (fmt[i] == '%')
        {
            i++;
            int space = 0;
            while (isdigit(fmt[i]))
            {
                space = 10 * space + fmt[i] - '0';
                i++;
            }
            if (fmt[i] == 'd')
            {
                long num = va_arg(ap, long);
                space -= len_long(num);
                print_space(fd, space);
                ft_putnbr(fd, num);
            }
            if (fmt[i] == 'c')
            {
                space--;

                int c = va_arg(ap, int);
                ft_putchar(fd, c);
            }
            if (fmt[i] == 's')
            {
                char *str = va_arg(ap, char *);
                space -= ft_strlen(str);
                print_space(fd, space);
                ft_putstr(fd, str);
            }
        }
        else
            ft_putchar(fd, fmt[i]);
        i++;
    }
    va_end(ap);
    if (fd == STDERR_FILENO)
        exit(1);
}

void ft_strncpy(char *dest, char *src, int size)
{
    if (dest == NULL || src == NULL)
        ft_printf(STDERR_FILENO, "receive NULL in strncpy\n");
    int len = ft_strlen(dest);
    int i = 0;
    while (src[i] && i < size)
    {
        dest[len + i] = src[i];
        i++;
    }
}

void ft_strcpy(char *dest, char *src)
{
    if (dest == NULL || src == NULL)
        ft_printf(STDERR_FILENO, "receive NULL in strcpy\n");
    int len = ft_strlen(dest);
    int i = 0;
    while (src[i])
    {
        dest[len + i] = src[i];
        i++;
    }
}

char *ft_strdup(char *str)
{
    char *res = calloc(ft_strlen(str), sizeof(char));
    ft_strcpy(res, str);
    return (res);
}

char *strjoin(char *string1, char *string2)
{
    char *res = calloc(ft_strlen(string1) + ft_strlen(string2) + 1, sizeof(char));
    if (res == NULL)
        ft_printf(STDERR_FILENO, "malloc failed in strjoin");
    if (string1)
        ft_strcpy(res, string1);
    if (string2)
        ft_strcpy(res + ft_strlen(res), string2);
    return res;
}

char *readline(int fd)
{
    int len = 0;
    char *res = NULL;
    char *c = calloc(2, sizeof(char));
    char *tmp;

    while (1)
    {
        int n = read(fd, c, sizeof(char));
        if (n <= 0)
            break;
        tmp = strjoin(res, c);
        free(res);
        res = tmp;
        if (c[0] == '\n' || c[0] == '\0')
            break;
    }
    return res;
}

void handle_signal(int signum)
{
    exit(0);
}