# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/21 13:34:10 by ledelbec          #+#    #+#              #
#    Updated: 2024/04/21 15:48:20 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCES=\
	src/main.c

CC=clang
CFLAGS=-Wall -Wextra -pthread -g
NAME=philo

OBJECTS=$(SOURCES:.c=.o)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)
