#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s2[i] && s1[i]
		&& (unsigned char)s1[i] == (unsigned char)s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}



int main() {

    bool res = (ft_strncmp("fjjfhgfdabc" + strlen("fjjfhgfdabcy") - strlen("abc"), "abc", strlen("abc")) == 0);
    printf("-> %d\n", res);
}
