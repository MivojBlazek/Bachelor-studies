/**
 * @file ipk24chat-server.h
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#ifndef _IPK24CHAT_SERVER_H_
#define _IPK24CHAT_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <signal.h>

#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_LINE_LENGTH 1400
#define BUFFER_SIZE 1400

#define SHM_KEY_ONE 1234
#define SHM_KEY_TWO 1111

typedef struct userStruct user;


/** Structure of transmission items */
typedef struct transmissionItemsStruct
{
    int socket;
    char buffer[BUFFER_SIZE];
    fd_set readSocks;
    struct timeval timeout;
    struct sockaddr_in serverAddress;
    struct sockaddr *address;
    socklen_t addressSize;
} transmissionItems;

/** Enum of transport protocols */
typedef enum transportProtocolTypeEnum
{
    UDP = 0,
    TCP
} transportProtocolType;

/** Enum of types of messages */
typedef enum msgTypeEnum
{
    MSG_CONFIRMS = 0x00,
    MSG_REPLY_OK = 0x01,
    MSG_AUTH = 0x02,
    MSG_JOIN = 0x03,
    MSG_MSG = 0x04,
    MSG_ERR = 0xFE,
    MSG_BYE = 0xFF,
    MSG_REPLY_NOK = 0x10,
} msgType;

/** Enum of types of messages from server */
typedef enum msgReturnTypeEnum
{
    RETURN_AUTH = 0,
    RETURN_JOIN = 1,
    RETURN_ERR = 2,
    RETURN_BYE = 3,
    RETURN_MSG = 4,
    RETURN_UNKNOWN = 99
    // RETURN_CONFIRMS -> messageID with minus and add -1
} msgReturnType;

/** Message struct */
typedef struct messageStruct
{
    uint8_t type;
    char *messageContent;
    unsigned int contentSize;
} message;

/** Structure of program arguments */
typedef struct programArgumentsStruct
{
    char *listeningIP;
    uint16_t port;
    uint16_t timeout;
    uint8_t retransmissions;
    int welcomeSocket;
    uint16_t messageID;
} programArguments;

/** Function prints help */
void printHelp();

/** Function finds out if the word consists of only alphanumeric characters */
bool isAlphaNumericWord(const char *word, int TCPorUDP);

/** Function finds out if the word has only printable characters */
bool isPrintableWord(const char *word, int TCPorUDP);

/** Function finds out if the string has only printable characters or spaces */
bool isPrintableWithSpaces(const char *string, int TCPorUDP);

/** Function ends communication with client */
void tryToExitUDP(transmissionItems *connectionItems, user *client, bool exists);

/** Function converts TCP message to UDP format */
void TCPtoUDP(message *newMsg, const char *TCPMsg);

/** Function converts UDP message to TCP format */
void UDPtoTCP(message *newMsg, const message *UDPMsg);

/** Function authenticates new UDP user */
void authUDP(transmissionItems *connectionItems, message *msg, user *newUser, uint16_t refMessageID);

/** Function joins UDP user to channel */
void joinUDP(transmissionItems *connectionItems, message *msg, user *existingUser, uint16_t refMessageID);

/** Function processes arrived message */
void msgUDP(transmissionItems *connectionItems, user *existingUser);

/** Function representing error state of FSM (trying to end communication) */
void errorState(transmissionItems *connectionItems, user *client);

/** Function representing open state of FSM (communication) */
void openState(transmissionItems *connectionItems, user *client);

/** Function representing auth state of FSM (there was invalid authentication) */
void authState(transmissionItems *connectionItems);

/** Function representing accept state of FSM (waiting for client authentication) */
void acceptState(transmissionItems *connectionItems);

/** Function initializes default program arguments */
void initDefaultProgramArguments();

/** Function handles Ctrl+C and Ctrl+D */
void controlCorD();

/** Function that is called after creating a new thread representing client */
void *clientHandlerTCP(void *arg);

/** Function that is called after creating a new thread representing UDP clients */
void *clientHandlerUDP(void *arg);

#endif //_IPK24CHAT_SERVER_H_
