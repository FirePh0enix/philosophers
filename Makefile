# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/29 11:56:35 by ledelbec          #+#    #+#              #
#    Updated: 2024/03/05 11:23:19 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCES=\
	src/main.c

NAME=philo
CC=clang
CFLAGS=-MMD -g3 -Wall -Wextra # -Werror

OBJECTS=$(SOURCES:.c=.o)
DEPS=$(OBJECTS:.o=.d)

all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -pthread -o $(NAME) $(OBJECTS)

clean:
	rm -rf $(DEPS) $(OBJECTS)

fclean: clean
	rm -f philo

re: fclean all
