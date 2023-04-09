#include <stdio.h>

int main(void)
{
    char *str = "You look gorgeous";
    int i = 0;
    while(str[i])
        printf("%d\n", str[i++]);
}