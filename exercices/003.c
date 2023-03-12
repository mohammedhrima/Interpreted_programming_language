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

char *add_parentises(char *str, char c)
{
    int len = ft_strlen(str);
    if (c)
        len++;
    char *res = malloc(len + 3);
    memset(res,0, len + 3);
    int i = 0;
    int j = 0;
    res[i++] = '(';
    if (c)
        res[i++] = c;
    while (i < len + 1)
    {
        res[i] = str[j];
        // printf("%s, copy -> %c\n", res, str[j]);
        i++;
        j++;
    }
    res[i] = ')';
    res[i + 1] = '\0';
    return (res);
}

int word_counter(char *str, char *to_find)
{
    int i = 0;
    int j = 0;
    int c = 0;
    while (str && str[i])
    {
        if (str[i] == to_find[0])
        {
            j = 0;
            while (to_find && to_find[j] && to_find[j] == str[i + j])
                j++;
            if (to_find[j] == '\0')
                c++;
        }
        i++;
    }
    return (c);
}

int main(void)
{
    char *str;
    int valid;
    char **arr;
    int i = 0;
    char *res;

    // printf("-> %d\n", word_counter("acbdcde", "acd"));
    // printf("-> %s\n", add_parentises("x", '*'));

    // exit(0);
    char *argument[] = {INT, CHAR, LONG, FLOAT, DOUBLE, SIGNED, UNSIGNED, SHORT, CONST, NULL};
    while (1)
    {
        valid = 0;
        str = readline();

        if (str == NULL)
            valid = 0;
        else
        {
            arr = split(str);
            str = arr[0];
            i = 1;
            while (arr && arr[i])
            {
                if (arr[i][0] == 'p' && strstr(arr[i], "pointer"))
                {
                    str = add_parentises(str, '*');
                    printf("-> %s\n", str);
                }
                i++;
            }
        }
        // if (valid)
        //     printf("valid data type\n");
        // else
        //     printf("not valid data type\n");
    }
}