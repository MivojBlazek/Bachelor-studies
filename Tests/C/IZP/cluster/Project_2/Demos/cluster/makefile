CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -DNDEBUG
CFLAGSMATH=-lm
NAME=cluster

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME) -g $(CFLAGSMATH)

clean:
	rm -f $(NAME)
