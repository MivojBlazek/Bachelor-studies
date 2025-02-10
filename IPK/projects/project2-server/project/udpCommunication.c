/**
 * @file udpCommunication.c
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "udpCommunication.h"


/** Function process UDP message from client */
int messageFromClientUDP(char *buffer, message *confirmMessage, user *newUser, struct sockaddr_in clientAddress, uint16_t *refMessageID, bool printRECV)
{
    char byte1 = (unsigned char)buffer[1];
    char byte2 = (unsigned char)buffer[2];
    uint16_t messageID = (byte1 << 8) | byte2;
    char *content = (buffer + 3);

    char tmp[1500];
    switch ((unsigned char)buffer[0])
    {
        case MSG_CONFIRMS:
        {
            char str[INET_ADDRSTRLEN];
            if (printRECV)
            {
                if (inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
                {
                    printf("RECV %s:%d | CONFIRM\n", str,  ntohs(clientAddress.sin_port));
                }
            }
            return (-(int)messageID) - 1;
            break;
        }
        case MSG_AUTH:
        {
            *refMessageID = messageID;

            char username[21];
            char displayName[21];
            char secret[21];
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 20 || !isAlphaNumericWord(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(username, content);
            content += strlen(content) + 1;
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 20 || !isPrintableWord(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(displayName, content);
            content += strlen(content) + 1;
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 128 || !isAlphaNumericWord(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(secret, content);

            newUser->username = (char *)malloc(sizeof(char) * (strlen(username) + 1));
            if (newUser->username == NULL) { exit(2); }
            newUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
            if (newUser->displayName == NULL) { exit(2); }
            newUser->secret = (char *)malloc(sizeof(char) * (strlen(secret) + 1));
            if (newUser->secret == NULL) { exit(2); }
            strcpy(newUser->username, username);
            strcpy(newUser->displayName, displayName);
            strcpy(newUser->secret, secret);

            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
            {
                printf("RECV %s:%d | AUTH\n", str,  ntohs(clientAddress.sin_port));
            }
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, 0, messageID);
            return RETURN_AUTH;
            break;
        }
        case MSG_JOIN:
        {
            *refMessageID = messageID;
            
            char channelID[21];
            char displayName[21];
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 20 || !isAlphaNumericWord(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(channelID, content);
            content += strlen(content) + 1;
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 20 || !isPrintableWord(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(displayName, content);

            newUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
            if (newUser->displayName == NULL) { exit(2); }
            newUser->channel = (char *)malloc(sizeof(char) * (strlen(channelID) + 1));
            if (newUser->channel == NULL) { exit(2); }
            strcpy(newUser->displayName, displayName);
            strcpy(newUser->channel, channelID);

            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
            {
                printf("RECV %s:%d | JOIN\n", str,  ntohs(clientAddress.sin_port));
            }
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, 0, messageID);
            return RETURN_JOIN;
            break;
        }
        case MSG_MSG:
        {
            char displayName[21];
            char messageContent[1401];
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 20 || !isPrintableWord(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(displayName, content);
            content += strlen(content) + 1;
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 1400 || !isPrintableWithSpaces(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(messageContent, content);

            newUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
            if (newUser->displayName == NULL) { exit(2); }
            strcpy(newUser->displayName, displayName);
          
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
            {
                printf("RECV %s:%d | MSG\n", str,  ntohs(clientAddress.sin_port));
            }
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, 0, messageID);
            return RETURN_MSG;
            break;
        }
        case MSG_ERR:
        {
            char displayName[21];
            char messageContent[1401];
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 20 || !isPrintableWord(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(displayName, content);
            content += strlen(content) + 1;
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, content);
            if (strlen(tmp) > 1400 || !isPrintableWithSpaces(tmp, UDP))
            {
                return RETURN_UNKNOWN;
            }
            strcpy(messageContent, content);

            newUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
            if (newUser->displayName == NULL) { exit(2); }
            strcpy(newUser->displayName, displayName);
            
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
            {
                printf("RECV %s:%d | ERR\n", str,  ntohs(clientAddress.sin_port));
            }
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, 0, messageID);
            return RETURN_ERR;
            break;
        }
        case MSG_BYE:
        {
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
            {
                printf("RECV %s:%d | BYE\n", str,  ntohs(clientAddress.sin_port));
            }
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, 0, messageID);
            return RETURN_BYE;
            break;
        }
        default:
            return RETURN_UNKNOWN;
            break;
    }
    return 0;
}

/** Function create a UDP message and returns it */
void createMessageUDP(message *newMsg, msgType type, char *displayName, char *messageContent, uint16_t messageID, uint16_t refMessageID)
{
    unsigned int pos = 0;
    unsigned int contentLength = 0;
    switch (type)
    {
        case MSG_CONFIRMS:
            contentLength = 3; // x00 | x00 x00
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }
            newMsg->messageContent[0] = '\x00';
            newMsg->messageContent[1] = (char)((refMessageID >> 8) & 0xFF);
            newMsg->messageContent[2] = (char)(refMessageID & 0xFF);
            break;
        case MSG_REPLY_OK:
        case MSG_REPLY_NOK:
            contentLength = strlen(messageContent) + 7; // x01 | x00 x00 | x01/x00 | x00 x00 | content | x00
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }
            newMsg->messageContent[0] = '\x01';
            newMsg->messageContent[1] = (char)((messageID >> 8) & 0xFF);
            newMsg->messageContent[2] = (char)(messageID & 0xFF);
            if (type == MSG_REPLY_OK)
            {
                newMsg->messageContent[3] = '\x01';
            }
            else
            {
                newMsg->messageContent[3] = '\x00';
            }
            newMsg->messageContent[4] = (char)((refMessageID >> 8) & 0xFF);
            newMsg->messageContent[5] = (char)(refMessageID & 0xFF);

            pos = 6;
            for (unsigned int i = 0; i < strlen(messageContent); i++)
            {
                newMsg->messageContent[pos++] = messageContent[i];
            }
            newMsg->messageContent[pos++] = '\0';
            break;
        case MSG_MSG:
        case MSG_ERR:
            contentLength = strlen(displayName) + strlen(messageContent) + 5;
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }

            if (type == MSG_MSG)
            {
                newMsg->messageContent[pos++] = '\x04';
            }
            else
            {
                newMsg->messageContent[pos++] = '\xFE';
            }
            newMsg->messageContent[pos++] = (char)((messageID >> 8) & 0xFF);
            newMsg->messageContent[pos++] = (char)(messageID & 0xFF);
            for (unsigned int i = 0; i < strlen(displayName); i++)
            {
                newMsg->messageContent[pos++] = displayName[i];
            }
            newMsg->messageContent[pos++] = '\0';

            for (unsigned int i = 0; i < strlen(messageContent); i++)
            {
                newMsg->messageContent[pos++] = messageContent[i];
            }
            newMsg->messageContent[pos++] = '\0';
            break;
        case MSG_BYE:
            contentLength = 3; // xFF | x00 x00
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }
            newMsg->messageContent[0] = '\xFF';
            newMsg->messageContent[1] = (char)((messageID >> 8) & 0xFF);
            newMsg->messageContent[2] = (char)(messageID & 0xFF);
            break;
        default:
            exit(1);
            break;
    }
    newMsg->type = type;
    newMsg->contentSize = contentLength;
}

/** Function to send data to server */
void UDPSend(int socket, char *messageToSend, int messageSize, struct sockaddr *address, socklen_t addressSize)
{
    int bytesTX = sendto(socket, messageToSend, messageSize, 0, address, addressSize);
    if (bytesTX < 0)
    {
        exit(3); //! error sendto
    }
}

/** Function to receive data from server */
void UDPRecv(int socket, char *buffer, struct sockaddr *address, socklen_t addressSize)
{
    int bytesRX = recvfrom(socket, buffer, BUFFER_SIZE, 0, address, &addressSize);
    if (bytesRX < 0)
    {
        exit(3); //! error recvfrom
    }
}

/** Function sets connection with server */
void UDPsetup(programArguments *prgArgs, transmissionItems *connectionItems)
{
    // timeout setup
    connectionItems->timeout.tv_sec = prgArgs->timeout / 1000;
    connectionItems->timeout.tv_usec = (prgArgs->timeout % 1000) * 1000;

    // socket setup
    int welcomeSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (welcomeSocket <= 0)
    {
        exit(3);
    }
    int enable = 1;
    setsockopt(welcomeSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    FD_ZERO(&connectionItems->readSocks);
    FD_SET(welcomeSocket, &connectionItems->readSocks);

    // binding socket
    memset(&connectionItems->serverAddress, 0, sizeof(connectionItems->serverAddress));
    connectionItems->serverAddress.sin_family = AF_INET;

    uint32_t IP;
    struct in_addr ipv4Addr;
    if (inet_pton(AF_INET, prgArgs->listeningIP, &ipv4Addr) != 1)
    {
        IP = 0;
    }
    IP = ntohl(ipv4Addr.s_addr);
    connectionItems->serverAddress.sin_addr.s_addr = htonl(IP);
    connectionItems->serverAddress.sin_port = htons(prgArgs->port);

    connectionItems->address = (struct sockaddr *) &connectionItems->serverAddress;
    connectionItems->addressSize = sizeof(connectionItems->serverAddress);

    if (bind(welcomeSocket, connectionItems->address, connectionItems->addressSize) < 0)
    {
        exit(3);
    }

    connectionItems->socket = welcomeSocket;
}
