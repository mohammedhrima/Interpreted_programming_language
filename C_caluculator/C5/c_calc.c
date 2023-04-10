#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <xlocale.h>
#include <unistd.h>
#include <stdlib.h>

int pos = 0;
int add_sub(char *str);

int integer(char *str)
{
    int left = 0;
    if (str[pos] == '(')
    {
        pos++;
        left = add_sub(str);
        if (str[pos] != ')')
        {
            int curr_pos = pos;
            pos += 2; // 2 for '> '
            while (pos-- > 0)
                printf(" ");
            printf("^\n");
            printf("Error syntax expected \')\' in pos %d\n", curr_pos);
            exit(0);
        }
        pos++;
        return left;
    }
    if (isdigit(str[pos]))
    {
        while (isdigit(str[pos]))
        {
            left = 10 * left + str[pos] - '0';
            pos++;
        }
    }
    return left;
}

int mul_sub(char *str)
{
    int left = integer(str);
    while (str[pos] && strchr("*/", str[pos]))
    {
        if (str[pos] == '*')
        {
            pos++;
            left *= integer(str);
        }
        if (str[pos] == '/')
        {
            pos++;
            left /= integer(str);
        }
    }
    return left;
}

int add_sub(char *str)
{
    int left = mul_sub(str);
    while (str[pos] && strchr("+-", str[pos]))
    {
        if (str[pos] == '+')
        {
            pos++;
            left += mul_sub(str);
        }
        if (str[pos] == '-')
        {
            pos++;
            left -= mul_sub(str);
        }
    }
    return left;
}

int main(void)
{
    char str[100];
    while (1)
    {
        memset(str, 0, 100);
        printf("> ");
        scanf("%s", str);
        int res = add_sub(str);
        pos = 0;
        printf("res: %d\n", res);
    }
}
