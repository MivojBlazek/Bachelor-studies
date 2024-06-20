CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror
CFLAGSMATH=-lm
CFLAGSDEBUG=-g
NAME=test

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME) $(CFLAGSDEBUG) $(CFLAGSMATH)

clean:
	rm -f $(NAME)
