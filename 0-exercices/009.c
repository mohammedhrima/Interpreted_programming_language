#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define INT "int"
#define CHAR "char"
#define LONG "long"
#define FLOAT "float"
#define DOUBLE "double"
#define SIGNED "signed"
#define UNSIGNED "unsigned"
#define SHORT "short"
#define CONST "const"
void putspaces(int len);
int ft_strlen(char *str)
{
    // printf("len of %s\n", str);
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

char **split(char *str, char c)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int len = 0;
    char **res = NULL;
    while (str && str[i] && str[i] == c)
        i++;
    while (str && str[i])
    {
        while (str[i] && str[i] != c)
            i++;
        len++;
        while (str[i] && str[i] == c)
            i++;
    }
    res = malloc(sizeof(char *) * (len + 1));
    i = 0;
    j = 0;
    while (j < len)
    {
        while (str && str[i] && str[i] == c)
            i++;
        k = i;
        while (str && str[k] && str[k] != c)
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

void ft_strcpy(char *src, char *dest)
{
    int i = 0;
    int j = 0;
    while (src && src[i])
        i++;
    while (dest && dest[j])
    {
        src[i] = dest[j];
        i++;
        j++;
    }
}

char *heap_pointer(char *str)
{
    int len = ft_strlen("(*") + ft_strlen(str) + ft_strlen(")") + 1;
    char *res = malloc(len * sizeof(char));
    memset(res, 0, len * sizeof(char));
    ft_strcpy(res, "(*");
    ft_strcpy(res + ft_strlen(res), str);
    res[ft_strlen(res)] = ')';
    return (res);
}

char *stack_pointer(char *str, char *size)
{
    // printf("stack pointer\n-> str : %s | size : %s \n", str,size);
    int len = ft_strlen(str) + ft_strlen("[") + ft_strlen(size) + ft_strlen("]") + 2;
    char *res = malloc(len * sizeof(char));
    memset(res, 0, len * sizeof(char));
    ft_strcpy(res, str);
    // printf("0. res after strcpy: %s\n", res);
    res[ft_strlen(res)] = '[';
    ft_strcpy(res + ft_strlen(res), size);
    // printf("1. res after strcpy: %s\n", res);

    res[ft_strlen(res)] = ']';
    return (res);
}

char *function(char *str)
{
    int len = ft_strlen(" ") + ft_strlen(str) + ft_strlen("()") + 1;
    char *res = malloc(len * sizeof(char));
    memset(res, 0, len * sizeof(char));
    ft_strcpy(res, str);
    res[ft_strlen(res)] = '(';
    res[ft_strlen(res)] = ')';
    return (res);
}

char *structure(char *str, char *type)
{
    // printf("\'%s\' is struct of type \'%s\'\n", str, type);
    int len = ft_strlen("struct ") + ft_strlen(type) + ft_strlen(" ") + ft_strlen(str) + 1;
    char *res = malloc(len * sizeof(char));
    memset(res, 0, len * sizeof(char));
    ft_strcpy(res, "struct ");
    ft_strcpy(res + ft_strlen(res), type);
    res[ft_strlen(res)] = ' ';
    ft_strcpy(res + ft_strlen(res), str);
    return (res);
}

// int is_digit

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

char *set_data_type(char *str, char *type)
{
    // printf("in set data type : \'%s\' of type \'%s\'\n", str, type);
    int len = ft_strlen(type) + ft_strlen(" ") + ft_strlen(str) + 1;
    char *res = malloc(len * sizeof(char));
    memset(res, 0, len * sizeof(char));
    ft_strcpy(res, type);
    res[ft_strlen(res)] = ' ';
    ft_strcpy(res + ft_strlen(res), str);
    return (res);
}

// itoa
int lvl(int num)
{
    int i;

    i = 0;
    if (num >= 0 && num < 10)
        return (1);
    if (num / 10 > 0)
        i++;
    while (num / 10 > 0)
    {
        num /= 10;
        i++;
    }
    return (i);
}

char *create_int(int len, int n, char *res, int neg)
{
    while (len >= neg)
    {
        res[len] = n % 10 + '0';
        n /= 10;
        len--;
    }
    return (res);
}

char *ft_itoa(int n)
{
    char *res;
    int len;
    int neg;

    neg = 0;
    if (n == -2147483648)
        return (strdup("-2147483648"));
    if (n < 0)
    {
        neg = 1;
        n = -n;
    }
    len = lvl(n);
    if (neg)
        len++;
    res = (char *)malloc((len + 1) * sizeof(char));
    if (res == NULL)
        return (NULL);
    res[len] = '\0';
    if (neg)
        res[0] = '-';
    return (create_int(len - 1, n, res, neg));
}

char *increment_decrement(int number, char *str)
{
    int num = atoi(str);
    strstr(str, "++") && num++;
    strstr(str, "--") && num--;
    // printf("number is %d\n", num);
    return (ft_itoa(num));
}

void putcharacter(char c)
{
    write(1, &c, sizeof(char));
}

void putstring(char *str, char end, char *exclude)
{
    int i = 0;
    while (str && str[i])
    {
        if (exclude == NULL || !strrchr(exclude, str[i]))
            putcharacter(str[i]);
        else
            putspaces(1);
        i++;
    }
    putcharacter(end);
}

int arrlen(char **array)
{
    int i = 0;
    while (array && array[i])
        i++;
    return (i);
}

void putspaces(int len)
{
    while (len--)
        putcharacter(' ');
}

int main(void)
{
    putstring("\n============= start =============\n", 0, 0);
    char *str;
    int valid;
    char **arr0;
    char **arr1;
    char **arr2;
    char **arr3;
    int line_lenght = 40;

    int i = 0;
    int j = 0;

    char *argument[] = {INT, CHAR, LONG, FLOAT, DOUBLE, SIGNED, UNSIGNED, SHORT, CONST, NULL};
    while (1)
    {
        valid = 0;
        putstring("\n> ", 0, 0);
        str = readline();
        if (strncmp(str, "clear", ft_strlen("clear") - 1) == 0)
            system("clear");
        else if (strncmp(str, "exit", ft_strlen("exit") - 1) == 0)
            exit(0);
        // esif (str == NULL)
        //     valid = 0;
        else
        {
            // printf("\n%10s\n", str);
            // int spaces = 2;
            putspaces((line_lenght - ft_strlen(str)) / 2);
            putstring(str, 0, 0); // print string
            putspaces((line_lenght - ft_strlen(str)) / 2);
            putcharacter('\n');
#if 1
            arr0 = split(str, '*');
            i = 0;
            line_lenght /= 2;
            // spaces++;
            while (arr0[i])
            {
                putspaces((line_lenght - ft_strlen(arr0[i])) / 2);
                putstring(arr0[i], 0, "()");
                putspaces((line_lenght - ft_strlen(arr0[i])) / 2);
                i++;
            }
            putcharacter('\n');

            i = 0;
            line_lenght /= 2;
            while (arr0[i])
            {
                arr1 = split(arr0[i], '+');
                j = 0;
                while (arr1[j])
                {
                    putspaces((line_lenght - ft_strlen(arr1[j])) / 2);
                    putstring(arr1[j], 0, "()");
                    putspaces((line_lenght - ft_strlen(arr1[j])) / 2);
                    j++;
                }
                // putcharacter(' ');
                i++;
            }
#endif
            putchar('\n');
            // putstring("*", ' ', 0);
            // if (strcmp("clear", str))
            //     printf("%s\n", str);
        }
        // if (valid)
        //     printf("valid data type\n");
        // else
        //     printf("not valid data type\n");
    }
}