/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:57:58 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/22 08:29:41 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H__
#define FT_STRING_H__

#include "../libft.h"

int ft_arrlen(char **array);
size_t ft_strlen(char *string);
int ft_memcmp(void *pointer1, void *pointer2, size_t len);
void *ft_memcpy(void *destination, void *source, size_t len);
void *ft_memmove(void *destination, void *source, size_t len);
void *ft_memset(void *pointer, int c, size_t len);
void ft_strcpy(char *destination, char *source);
void ft_strncpy(char *destination, char *source, int size);
char *ft_charjoin(char *string, char c);
char *ft_strchr(char *string, int c);
int ft_strcmp(char *string1, char *string2);
int ft_strncmp(char *string1, char *string2, size_t len);
char *ft_strdup(char *string);
char *ft_strtrim(char *str, char *set);
char *ft_strjoin0(char *string1, char *string2);
char *ft_strjoin1(char *string1, char *string2);
char *ft_strjoin2(char *string1, char *string2, char *string3);
size_t ft_strlcat(char *destination, char *source, size_t size);
size_t ft_strlcpy(char *destination, char *source, size_t size);
char *ft_strrchr(char *string, int c);
char *ft_strstr(char *from, char *to_find);
char *ft_substr(char *string, unsigned int start, size_t len);
char **ft_split(char *string, char *spliter);
char *convert_base(long long number, char *base);

#endif