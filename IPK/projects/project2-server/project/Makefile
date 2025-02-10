#
# @file Makefile
#
#	IPK project 2
#
# @author <xblaze38> Michal Blažek
#

CC=gcc
CFLAGS= -std=gnu17 -pedantic -Wall -Wextra -lpthread
CFLAGSDEBUG= -std=gnu17 -pedantic -Wall -Wextra -g -lpthread -Wno-format-contains-nul -Wno-format-extra-args

main: ipk24chat-server.c
	$(CC) $(CFLAGS) ipk24chat-server.c  udpCommunication.c tcpCommunication.c users.c -o ipk24chat-server

debug: ipk24chat-server.c
	$(CC) $(CFLAGSDEBUG) ipk24chat-server.c udpCommunication.c tcpCommunication.c users.c -o ipk24chat-server

clean:
	rm -f *.o ipk24chat-server