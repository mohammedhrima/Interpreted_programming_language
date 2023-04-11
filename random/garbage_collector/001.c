#include <stdlib.h>

typedef struct header {
    unsigned int size;
    struct header *next;
} header_t;

int main(void)
{
    char *str = malloc(1000);
}