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


/** Function tries to exit correctly */
void tryToExitUDP(programArguments *prgArgs, transmissionItems *connectionItems);

/** Function handle UDP messages from server */
int messageFromServerUDP(char *buffer, message *confirmMessage, programArguments *prgArgs);

/** Function create a UDP message and returns it */
void createMessageUDP(message *msg, msgType type, char *username, char *channelID, char *secret, char *displayName, char *messageContent, uint16_t messageID);

/** Function to send data to server */
void UDPSend(int socketFD, char *messageToSend, int messageSize, struct sockaddr *address, socklen_t addressSize);

/** Function to receive data from server */
void UDPRecv(int socketFD, char *buffer, struct sockaddr *address, socklen_t addressSize);

/** Function sets connection with server */
void UDPsetup(programArguments *prgArgs, transmissionItems *connectionItems);

/** Function provides UDP communication with server */
void UDPcommunication(programArguments *prgArgs, message *msg, transmissionItems *connectionItems);

#endif //_UDP_COMMUNICATION_H_
