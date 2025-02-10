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
#include "ipk24chat-client.h"

struct transmissionItemsStruct;


/** Function handles TCP messages from server */
int messageFromServerTCP(char *buffer);

/** Function create a TCP message and returns it */
void createMessageTCP(message *msg, msgType type, char *username, char *channelID, char *secret, char *displayName, char *messageContent);

/** Function to send data to server */
void TCPSend(int socketFD, char *messageToSend, int messageSize);

/** Function to receive data from server */
void TCPRecv(int socketFD, char *buffer);

/** Function sets connection with server */
void TCPsetup(programArguments *prgArgs, transmissionItems *connectionItems);

/** Function provides UDP communication with server */
void TCPcommunication(programArguments *prgArgs, message *msg, transmissionItems *connectionItems);

#endif //_TCP_COMMUNICATION_H_
