# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/24 05:24:40 by mhrima            #+#    #+#              #
#    Updated: 2023/08/26 02:36:04 by mhrima           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -fsanitize=address -g3 #-Wall -Wextra -Werror
RM = rm -rf
OBJDIR = objects
LIB = Libft/libft.a
EXEC = mini 

SRCS = main.c global.c debug.c tokenize.c parsing.c interpret.c
OBJS = $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.c=.o)))

.PHONY: all lib clean fclean re

all: lib $(EXEC)

lib:
	@if [ ! -f $(LIB) ]; then \
		make -C Libft; \
	fi

$(OBJS): $(SRCS) header.h
$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -lm -o $(EXEC)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJDIR)
	make -C Libft clean

fclean: clean
	$(RM) $(EXEC)
	make -C Libft fclean

re: fclean all

#cc -c main.c -o objects/main.o
#cc objects/main.o -L./Libft -l:libft.a -lm -o exe