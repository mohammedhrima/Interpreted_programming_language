#include <stdio.h>
#include <unistd.h>

void ft_putchar(char c)
{
    write(1, &c, sizeof(char));
}

int ft_isspace(int c)
{
    return (c == ' ' || c == '\n' || c == '\t');
}
int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

long ft_atoi(char *str)
{
    long res = 0;
    long sign = 1;
    int i = 0;

    while (ft_isspace(str[i]))
        i++;
    if (str[i] == '+')
        i++;
    else if (str[i] == '-')
    {
        i++;
        sign = -1;
    }
    while (ft_isdigit(str[i]))
    {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    return sign * res;
}

double ft_atof(char *str)
{
    double res = 0.0;
    double sign = 1.0;
    double fraction = 0.1;
    int i = 0;

    while (ft_isspace(str[i]))
        i++;
    if (str[i] == '+')
        i++;
    else if (str[i] == '-')
    {
        i++;
        sign = -1.0;
    }
    while (ft_isdigit(str[i]))
    {
        res = res * 10.0 + (str[i] - '0');
        i++;
    }
    if (str[i] == '.')
    {
        i++;
        while (ft_isdigit(str[i]))
        {
            res = res + fraction * (str[i] - '0');
            fraction *= 0.1;
            i++;
        }
    }
    return sign * res;
}

void ft_putnbr(long num)
{
    if (num < 0)
    {
        ft_putchar('-');
        num = -num;
    }
    if (num < 10)
        ft_putchar(num + '0');
    else
    {
        ft_putnbr(num / 10);
        ft_putnbr(num % 10);
    }
}

void ft_putfloat(double num, int decimal_places)
{
    if (num < 0.0)
    {
        ft_putchar('-');
        num = -num;
    }
    long int_part = (long)num;
    double float_part = num - (double)int_part;

    ft_putnbr(int_part);
    if (decimal_places > 0)
    {
        ft_putchar('.');
        int i = 0;
        while (i < decimal_places)
        {
            float_part *= 10;
            long digit = (long)float_part;
            ft_putnbr(digit);
            float_part -= (double)digit;
            i++;
        }
    }
}

int main()
{
    long n = ft_atoi("100.00025");
    printf("> %d\n", n);
//    ft_putfloat(n, 6);

    return 0;
}
