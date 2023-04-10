# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/24 05:24:40 by mhrima            #+#    #+#              #
#    Updated: 2023/02/15 11:22:54 by mhrima           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -rf

SRCS0 = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c \
		ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_putstr_fd.c ft_split.c ft_split2.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strjoin2.c ft_strjoin3.c ft_strlcat.c \
		ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strcmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c \
		ft_tolower.c ft_toupper.c get_next_line.c ft_strstr.c ft_lstadd_back.c  ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
		ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c ft_arrlen.c ft_split3.c

SRCS1 = ft_printf/ft_printf.c ft_printf/convert.c ft_printf/len.c ft_printf/others.c ft_printf/put.c ft_printf/printf_utils_handle_flag.c ft_printf/printf_utils_handle_output_1.c ft_printf/printf_utils_handle_output_2.c

SRCS2 = my_malloc_free/my_malloc.c my_malloc_free/my_free.c

OBJS0 = $(SRCS0:.c=.o)
OBJS1 = ft_printf.o convert.o len.o others.o put.o printf_utils_handle_flag.o printf_utils_handle_output_1.o printf_utils_handle_output_2.o
OBJS2 = my_malloc.o my_free.o

$(NAME): $(OBJS0) $(OBJS1) $(OBJS2)

all : $(NAME)

$(OBJS1):$(SRCS1) 
	$(CC) -c $(SRCS1)
	
$(OBJS2):$(SRCS2) 
	$(CC) -c $(SRCS2)

$(NAME) : $(OBJS0) $(OBJS1) $(OBJS2)
	$(AR) $(NAME) $(OBJS0) $(OBJS1) $(OBJS2)

bonus: $(OBJS0) $(OBJS1)
	$(AR) $(NAME) $(OBJS0) $(OBJS1) $(OBJS2)


clean : 
	$(RM) $(OBJS0) $(OBJS1) $(OBJS2)
	
fclean : clean
	$(RM) $(NAME)

re : fclean all