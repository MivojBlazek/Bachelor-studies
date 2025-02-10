/**
 * @file ipk24chat-client.h
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#ifndef _IPK24CHAT_CLIENT_H_
#define _IPK24CHAT_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <signal.h>

#define MAX_LINE_LENGTH 1400
#define COMMANDS_COUNT 4
#define BUFFER_SIZE 1400


/** Structure of transmission items */
typedef struct transmissionItemsStruct
{
    int socketFD;
    char buffer[BUFFER_SIZE];
    fd_set readSocks;
    struct timeval timeout;
    struct hostent *server;
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

/** Enum of all possible commands in input */
typedef enum inputCommandsEnum
{
    AUTH = 0,
    JOIN,
    RENAME,
    HELP
} inputCommands;

/** Enum of server message types */
typedef enum serverMsgTypeEnum
{
    MSG = 0,
    ERR,
    REPLY
} serverMsgType;

/** Enum of types of messages */
typedef enum msgTypeEnum
{
    MSG_CONFIRMS = 0x00,
    MSG_REPLY = 0x01,
    MSG_AUTH = 0x02,
    MSG_JOIN = 0x03,
    MSG_MSG = 0x04,
    MSG_ERR = 0xFE,
    MSG_BYE = 0xFF,
    MESSAGE_ARRIVED_WITHOUT_ASKING = 0x69
} msgType;

/** Enum of types of messages from server */
typedef enum msgReturnTypeEnum
{
    RETURN_POSITIVE_REPLY = 0,
    RETURN_NEGATIVE_REPLY = 1,
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
    uint16_t messageID;
    char *messageContent;
    unsigned int contentSize;
} message;

/** Struct of program arguments and messageID that increments */
typedef struct programArgumentsStruct
{
    transportProtocolType transportProtocol;
    char *serverAddress;
    int port;
    int timeout;
    int retransmissions;
    uint16_t messageID;
    char *displayName;
    bool auth;
    int socketFD;
    uint16_t originalMessageID;
} programArguments;


/** Function prints help */
void printHelp();

/** Function prints help for supported commands */
void printHelpCommands();

/** Function finds out if the word consists of only alphanumeric characters */
bool isAlphaNumericWord(const char *word);

/** Function finds out if the word has only printable characters */
bool isPrintableWord(const char *word);

/** Function finds out if the string has only printable characters or spaces */
bool isPrintableWithSpaces(const char *string);

/** Function change user displayed name */
void renameUser(char *inputLine, programArguments *prgArgs);

/** Function to join user to channel */
void join(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems);

/** Function to authenticate user */
void authenticate(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems);

/** Function sends normal message */
void sendMessage(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems);

/** Function process line from stdin (command) */
void processCommandLine(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems);

/** Function initializes default program arguments */
void initDefaultProgramArguments(programArguments *prgArgs);

/** Function handles Ctrl+C and Ctrl+D */
void controlCorD();

#endif //_IPK24CHAT_CLIENT_H_
