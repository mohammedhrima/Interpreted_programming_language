#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define INT "int"
#define CHAR "char"
#define LONG "long"
#define FLOAT "float"
#define DOUBLE "double"
#define SIGNED "signed"
#define UNSIGNED "unsigned"
#define SHORT "short"
#define CONST "const"

int ft_strlen(char *str)
{
    int i = 0;
    while (str && str[i])
        i++;
    return (i);
}

char *strjoin(char *str, char c)
{
    int i = 0;

    char *res = malloc(ft_strlen(str) + 2);
    while (str && str[i])
    {
        res[i] = str[i];
        i++;
    }
    res[i] = c;
    res[i + 1] = '\0';
    return (res);
}

char *readline(void)
{
    char *res;
    char c;

    res = NULL;
    while (read(STDIN_FILENO, &c, 1) && c != '\n')
    {
        char *tmp = strjoin(res, c);
        free(res);
        res = tmp;
    }
    return (res);
}

int ft_strcmp(char *str1, char *str2)
{
    int i = 0;
    while (str1 && str2 && str1[i] && str1[i] == str2[i])
        i++;
    return (str1[i] - str2[i]);
}

int main(void)
{
    char *str;
    int valid;

    char *argument[] = {INT, CHAR, LONG, FLOAT, DOUBLE, SIGNED, UNSIGNED, SHORT, CONST, NULL};
    while (1)
    {
        valid = 0;
        str = readline();
        if (str == NULL)
            valid = 0;
        else
        {
            int i = 0;
            while (argument[i])
            {
                if (ft_strcmp(argument[i], str) == 0)
                {
                    valid = 1;
                    break;
                }
                i++;
            }
        }
        if (valid)
            printf("valid data type\n");
        else
            printf("not valid data type\n");
    }
}


