#ifndef HEADER_H
#define HEADER_H

#define STDIN STDIN_FILENO
#define STDOUT STDOUT_FILENO
#define STDERR STDERR_FILENO


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>

int pos;
int line;
char *text;

char *readline(int fd);
void ft_printf(int fd, char *fmt, ...);
int ft_strlen(char *str);
void handle_signal(int signum);
void ft_strcpy(char *dest, char *src);
void ft_strncpy(char *dest, char *src, int size);
void print_space(int fd, int line_long);
int ft_isalpha(int c);
int ft_isdigit(int c);

#endif