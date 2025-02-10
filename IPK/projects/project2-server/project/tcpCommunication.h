/**
 * @file tcpCommunication.h
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#ifndef _TCP_COMMUNICATION_H_
#define _TCP_COMMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "ipk24chat-server.h"
#include "users.h"

struct transmissionItemsStruct;


/** Function process message from client */
int messageFromClientTCP(char *buffer, user *newUser, struct sockaddr_in clientAddress);

/** Function create a TCP message and returns it */
void createMessageTCP(message *msg, msgType type, char *displayName, char *messageContent);

/** Function to send data to client */
void TCPSend(int socket, char *messageToSend, int messageSize);

/** Function to receive data from client */
void TCPRecv(int socket, char *buffer);

/** Function sets connection with client */
void TCPsetup(programArguments *prgArgs, transmissionItems *connectionItems);

#endif //_TCP_COMMUNICATION_H_
