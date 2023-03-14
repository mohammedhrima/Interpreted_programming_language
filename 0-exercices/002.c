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
    while (read(STDIN_FILENO, &c, 1) && c != '\n' && c != ';')
    {
        char *tmp = strjoin(res, c);
        free(res);
        res = tmp;
    }
    return (res);
}

char **split(char *str)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int len = 0;
    char **res = NULL;
    while (str && str[i] && str[i] == ' ')
        i++;
    while (str && str[i])
    {
        while (str[i] && str[i] != ' ')
            i++;
        len++;
        while (str[i] && str[i] == ' ')
            i++;
    }
    res = malloc(sizeof(char *) * (len + 1));
    i = 0;
    j = 0;
    while (j < len)
    {
        while (str && str[i] && str[i] == ' ')
            i++;
        k = i;
        while (str && str[k] && str[k] != ' ')
            k++;
        res[j] = malloc((k - i + 2) * sizeof(char));
        l = 0;
        while (i < k)
        {
            res[j][l] = str[i];
            i++;
            l++;
        }
        res[j][l] = '\0';
        j++;
    }
    res[j] = NULL;
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
    char **arr;
    int i = 0;

#if 1
    char *argument[] = {INT, CHAR, LONG, FLOAT, DOUBLE, SIGNED, UNSIGNED, SHORT, CONST, NULL};
    while (1)
    {
        valid = 0;
        str = readline();
        arr = split(str);
        if (str == NULL)
            valid = 0;
        else
        {
            valid = 0;
            int i = 0;
            while (argument[i])
            {
                int j = 0;
                while (arr[j])
                {
                    // printf("compaire \'%s\' to \'%s\'\n", argument[i], arr[j]);
                    if (ft_strcmp(argument[i], arr[j]) == 0)
                    {
                        valid = 1;
                        break;
                    }
                    j++;
                }
                if (valid)
                    break;
                i++;
            }
        }
        if (valid)
            printf("valid data type\n");
        else
            printf("not valid data type\n");
    }
#endif
}