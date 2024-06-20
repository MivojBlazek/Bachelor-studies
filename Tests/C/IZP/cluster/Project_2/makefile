CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -DNDEBUG
CFLAGSMATH=-lm
NAME=project2_final

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME) -g $(CFLAGSMATH)

clean:
	rm -f $(NAME)
