#include <stdio.h>
#define SIZE 128 // max push back number

/*
- Pbackbuf: is push-back stack
- Pbackp  : is the stack pointer. the stack grow down so a push is *--Pbackp=c and a pop is: c=*Pbackp++
- get()   : evaluates yo the next input character, either popping it off the stack (if it's not empty)
            or by calling  getc().
- unget(c): pushes c back. It evaluates to c if succeful, or to -1 if the pushback stack was full.
*/

int ft_strlen(char *str)
{
    int i = 0;
    while (str && str[i])
        i++;
    return (i);
}

int Pbackbuf[SIZE];
int *Pbackp = &Pbackbuf[SIZE];

#define get(stream) (Pbackp < &Pbackbuf[SIZE] ? *Pbackp++ : getc(stream))
#define unget(c) (Pbackp <= Pbackbuf ? -1 : (*--Pbackp = (c)))

void ungets(char *start, int n)
{
    /*
    Push back the last n characters of string by working backwards
    through the string.
    */
    char *p = start + ft_strlen(start); // Find the end of string
    while (--p >= start && --n >= 0)
    {
        if (unget(*p) == -1)
            fprintf(stderr, "Pushback-stack overflow \n");
    }
}