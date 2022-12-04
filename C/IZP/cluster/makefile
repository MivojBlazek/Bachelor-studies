CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -DNDEBUG
CFLAGSMATH=-lm
NAME=clusterl

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME) -g $(CFLAGSMATH)

clean:
	rm -f $(NAME)
