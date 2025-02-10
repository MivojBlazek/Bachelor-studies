#
# @file Makefile
#
#	IPK project 1
#
# @author <xblaze38> Michal Blažek
#

CC=gcc
CFLAGS= -std=gnu17 -pedantic -Wall -Wextra
CFLAGSDEBUG= -std=gnu17 -pedantic -Wall -Wextra -g

main: ipk24chat-client.c
	$(CC) $(CFLAGS) ipk24chat-client.c udpCommunication.c tcpCommunication.c -o ipk24chat-client

debug: ipk24chat-client.c
	$(CC) $(CFLAGSDEBUG) ipk24chat-client.c udpCommunication.c tcpCommunication.c -o ipk24chat-client

clean:
	rm -f *.o ipk24chat-client