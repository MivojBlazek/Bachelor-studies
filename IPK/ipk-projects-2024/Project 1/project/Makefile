#
# @file Makefile
#
#	IPK project 1
#
# @author <xblaze38> Michal Blažek
#

CC=gcc
CFLAGS= -std=c2x -pedantic -Wall -Wextra
CFLAGSDEBUG= -std=c20 -pedantic -Wall -Wextra -g

main: ipk24chat-client.c
	$(CC) $(CFLAGS) ipk24chat-client.c -o ipk24chat-client

clean:
	rm -f *.o ipk24chat-client