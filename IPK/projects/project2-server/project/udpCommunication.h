/**
 * @file udpCommunication.h
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#ifndef _UDP_COMMUNICATION_H_
#define _UDP_COMMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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


/** Function process UDP message from client */
int messageFromClientUDP(char *buffer, message *confirmMessage, user *newUser, struct sockaddr_in clientAddress, uint16_t *refMessageID, bool printRECV);

/** Function create a UDP message and returns it */
void createMessageUDP(message *newMsg, msgType type, char *displayName, char *messageContent, uint16_t messageID, uint16_t refMessageID);

/** Function to send data to server */
void UDPSend(int socket, char *messageToSend, int messageSize, struct sockaddr *address, socklen_t addressSize);

/** Function to receive data from server */
void UDPRecv(int socket, char *buffer, struct sockaddr *address, socklen_t addressSize);

/** Function sets connection with server */
void UDPsetup(programArguments *prgArgs, transmissionItems *connectionItems);

#endif //_UDP_COMMUNICATION_H_
