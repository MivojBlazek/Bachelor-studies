CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
NAME=proj2

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME) -lpthread

clean:
	rm -f $(NAME)
