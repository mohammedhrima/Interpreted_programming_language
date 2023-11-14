# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/24 05:24:40 by mhrima            #+#    #+#              #
#    Updated: 2023/11/14 13:25:35 by mhrima           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = #-Wall -Wextra -Werror -fsanitize=address -g3
AR = ar rcs
RM = rm -rf
OBJDIR = objects

# List of subdirectories
SUBDIRS = ctype stdlib string printf memory stdio

# Collect source files from subdirectories
SRCS0 = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))
OBJS0 = $(addprefix $(OBJDIR)/,$(notdir $(SRCS0:.c=.o)))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS0)
	$(AR) $(NAME) $(OBJS0)

$(OBJS0): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: */%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all