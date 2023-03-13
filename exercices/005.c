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

char *heap_pointer(char *str)
{
    int len = ft_strlen(str);
    char *res = malloc(len + 4);
    memset(res, 0, len + 4);
    int i = 2;
    int j = 0;
    res[0] = '(';
    res[1] = '*';
    while (i < len + 2)
    {
        res[i] = str[i - 2];
        i++;
    }
    res[i] = ')';
    return (res);
}

char *stack_pointer(char *str, char *size)
{
    // if (!size)
    //     size = "0";
    int len = ft_strlen(str) + ft_strlen(size);
    char *res = malloc((len + 3) * sizeof(char));
    memset(res, 0, len + 3);
    int i = 0;
    int j = 0;
    while (str && str[i])
    {
        res[i] = str[i];
        i++;
    }
    res[i] = '[';
    i++;
    j = 0;
    while (size && size[j])
    {
        res[i + j] = size[j];
        j++;
    }
    res[i + j] = ']';
    return (res);
}

char *function(char *str, char *return_type)
{
    int len = ft_strlen(str) + ft_strlen(return_type) + 2;
    char *res = malloc((len + 3) * sizeof(char));
    memset(res, 0, (len + 3) * sizeof(char));
    strcpy(res, return_type);
    res[ft_strlen(return_type)] = ' ';
    strcpy(res + ft_strlen(return_type) + 1 , str);
    res[ft_strlen(res)] = '(';
    res[ft_strlen(res)] = ')';
    return (res);
}

char *structure(char *str)
{
    int len = ft_strlen(str) + ft_strlen("struct ") + 1;
    char *res = malloc(len * sizeof(char));
    memset(res, 0, len * sizeof(char));
    strcpy(res, "struct ");
    res[ft_strlen("struct ")] = ' ';
    strcpy(res + ft_strlen("struct "), str);
    return (res);
}

int is_number(char *str)
{
    int i = 0;
    while (str && str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int main(void)
{
    char *str;
    int valid;
    char **arr;
    int i = 0;
    int j = 0;
    char *res;
    int there_is_number;
    int n = -1;

    // printf("-> %d\n", word_counter("acbdcde", "acd"));
    // printf("-> %s\n", add_parentises("x", '*'));

    // exit(0);
    char *argument[] = {INT, CHAR, LONG, FLOAT, DOUBLE, SIGNED, UNSIGNED, SHORT, CONST, NULL};
    while (1)
    {
        valid = 0;
        write(1, "> ", 2);
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
                if (strncmp(arr[i], "array", ft_strlen("array") - 1) == 0)
                {
                    j = i;
                    while (arr[j] && !is_number(arr[j]))
                        j++;
                    // if (arr[j] == NULL)
                    //     arr[j] = "0";
                    // printf("stack pointer with size %s\n", arr[j]);
                    str = stack_pointer(str, arr[j]);
                }
                if (strncmp(arr[i], "pointer", ft_strlen("pointer") - 1) == 0)
                {
                    // printf("heap pointer with size %s\n", arr[j]);
                    str = heap_pointer(str);
                }
                if (strncmp(arr[i], "function", ft_strlen("function") - 1) == 0)
                {
                    // printf("function\n");
                    char *return_type = NULL;
                    j = i;
                    while (arr[j] && strncmp(arr[j], "return", ft_strlen("return") - 1))
                        j++;
                    if (arr[j] && strncmp(arr[j], "return", ft_strlen("return") - 1) == 0)
                    {
                        i = j + 1;
                        return_type = arr[j + 1];
                        // printf("\'%s\' function that return \'%s\'\n", str, return_type);
                    }
                    str = function(str, return_type);
                }
                if (strncmp(arr[i], "struct", ft_strlen("struct") - 1) == 0)
                {
                }
                i++;
            }
            printf("> %s\n", str);
        }
        // if (valid)
        //     printf("valid data type\n");
        // else
        //     printf("not valid data type\n");
    }
}