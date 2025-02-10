/**
 * @file tcpCommunication.c
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "tcpCommunication.h"

/** Function process message from client */
int messageFromClientTCP(char *buffer, user *newUser, struct sockaddr_in clientAddress)
{
    if (!strncmp(buffer, "ERR FROM ", 9))
    {
        buffer += 9;
        char displayName[21];
        char messageContent[1401];
        char *ISpos = strstr(buffer, " IS ");
        if (ISpos != NULL)
        {
            int displayNameLength = ISpos - buffer;
            strncpy(displayName, buffer, displayNameLength > 20 ? 20 : displayNameLength);
            displayName[displayNameLength] = '\0';
            if (displayNameLength > 20  || !isPrintableWord(displayName, TCP))
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }

            char *messageStart = ISpos + 4; // " IS "
            if (strlen(messageStart) > 1400 || !isPrintableWithSpaces(messageStart, TCP))
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }
            strcpy(messageContent, messageStart);
        }
        else
        {
            //! malformed message (" IS " missing)
            return RETURN_UNKNOWN;
        }

        int messageLength = strlen(messageContent);
        if (messageContent[messageLength - 1] != '\n' || messageContent[messageLength - 2] != '\r')
        {
            //! malformed message (\r\n missing)
            return RETURN_UNKNOWN;
        }
        messageContent[messageLength - 1] = '\0';
        messageContent[messageLength - 2] = '\0';
        
        newUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
        if (newUser->displayName == NULL) { exit(2); }
        strcpy(newUser->displayName, displayName);

        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
        {
            printf("RECV %s:%d | ERR\n", str,  ntohs(clientAddress.sin_port));
        }
        return RETURN_ERR;
    }
    else if (!strncmp(buffer, "MSG FROM ", 9))
    {
        buffer += 9;
        char displayName[21];
        char messageContent[1401];
        char *ISpos = strstr(buffer, " IS ");
        if (ISpos != NULL)
        {
            int displayNameLength = ISpos - buffer;
            strncpy(displayName, buffer, displayNameLength > 20 ? 20 : displayNameLength);
            displayName[displayNameLength] = '\0';
            if (displayNameLength > 20  || !isPrintableWord(displayName, TCP))
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }

            char *messageStart = ISpos + 4; // " IS "
            if (strlen(messageStart) > 1400 || !isPrintableWithSpaces(messageStart, TCP))
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }
            strcpy(messageContent, messageStart);
        }
        else
        {
            //! malformed message (" IS " missing)
            return RETURN_UNKNOWN;
        }

        int messageLength = strlen(messageContent);
        if (messageContent[messageLength - 1] != '\n' || messageContent[messageLength - 2] != '\r')
        {
            //! malformed message (\r\n missing)
            return RETURN_UNKNOWN;
        }
        messageContent[messageLength - 1] = '\0';
        messageContent[messageLength - 2] = '\0';
        
        newUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
        if (newUser->displayName == NULL) { exit(2); }
        strcpy(newUser->displayName, displayName);

        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
        {
            printf("RECV %s:%d | MSG\n", str, ntohs(clientAddress.sin_port));
        }
        return RETURN_MSG;
    }
    else if (!strncmp(buffer, "JOIN ", 5))
    {
        buffer += 5;
        char channelID[21];
        char displayName[21];
        char *ASpos = strstr(buffer, " AS ");
        if (ASpos != NULL)
        {
            int channelIDLength = ASpos - buffer;
            strncpy(channelID, buffer, channelIDLength > 20 ? 20 : channelIDLength);
            channelID[channelIDLength] = '\0';
            if (channelIDLength > 20 || !isAlphaNumericWord(channelID, TCP))
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }

            char *displayNameStart = ASpos + 4; // " AS "
            if (strlen(displayNameStart) > 20 || !isPrintableWord(displayNameStart, TCP))
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }
            strcpy(displayName, displayNameStart);
        }
        else
        {
            //! malformed message (" AS " missing)
            return RETURN_UNKNOWN;
        }

        int displayNameLength = strlen(displayName);
        if (displayName[displayNameLength - 1] != '\n' || displayName[displayNameLength - 2] != '\r')
        {
            //! malformed message (\r\n missing)
            return RETURN_UNKNOWN;
        }
        displayName[displayNameLength - 1] = '\0';
        displayName[displayNameLength - 2] = '\0';

        newUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
        if (newUser->displayName == NULL) { exit(2); }
        newUser->channel = (char *)malloc(sizeof(char) * (strlen(channelID) + 1));
        if (newUser->channel == NULL) { exit(2); }
        strcpy(newUser->displayName, displayName);
        strcpy(newUser->channel, channelID);

        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
        {
            printf("RECV %s:%d | JOIN\n", str, ntohs(clientAddress.sin_port));
        }
        return RETURN_JOIN;
    }
    else if (!strncmp(buffer, "AUTH ", 5))
    {
        buffer += 5;
        char username[21];
        char displayName[21];
        char secret[129];
        char *ASpos = strstr(buffer, " AS ");
        if (ASpos != NULL)
        {
            int usernameLength = ASpos - buffer;
            strncpy(username, buffer, usernameLength > 20 ? 20 : usernameLength);
            username[usernameLength] = '\0';
            if (usernameLength > 20 || !isAlphaNumericWord(username, TCP))
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }

            char *displayNameStart = ASpos + 4; // " AS "

            char *USINGpos = strstr(buffer, " USING ");
            if (USINGpos != NULL)
            {
                int displayNameLength = USINGpos - displayNameStart;
                strncpy(displayName, displayNameStart, displayNameLength > 20 ? 20 : displayNameLength);
                displayName[displayNameLength] = '\0';
                if (displayNameLength > 20 || !isPrintableWord(displayName, TCP))
                {
                    //! invalid characters or length
                    return RETURN_UNKNOWN;
                }

                char *secretStart = USINGpos + 7; // " USING "
                if (strlen(secretStart) > 128 || !isPrintableWord(secretStart, TCP))
                {
                    //! invalid characters or length
                    return RETURN_UNKNOWN;
                }
                strcpy(secret, secretStart);
            }
            else
            {
                //! malformed message (" USING " missing)
                return RETURN_UNKNOWN;
            }
        }
        else
        {
            //! malformed message (" AS " missing)
            return RETURN_UNKNOWN;
        }

        int secretLength = strlen(secret);
        if (secret[secretLength - 1] != '\n' || secret[secretLength - 2] != '\r')
        {
            //! malformed message (\r\n missing)
            return RETURN_UNKNOWN;
        }
        secret[secretLength - 1] = '\0';
        secret[secretLength - 2] = '\0';

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
            printf("RECV %s:%d | AUTH\n", str, ntohs(clientAddress.sin_port));
        }
        return RETURN_AUTH;
    }
    else if (!strcmp(buffer, "BYE\r\n"))
    {
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &clientAddress.sin_addr, str, sizeof(str)))
        {
            printf("RECV %s:%d | BYE\n", str, ntohs(clientAddress.sin_port));
        }
        return RETURN_BYE;
    }
    else
    {
        return RETURN_UNKNOWN;
    }
    return 0;
}

/** Function create a TCP message and returns it */
void createMessageTCP(message *newMsg, msgType type, char *displayName, char *messageContent)
{
    unsigned int contentLength = 0;
    switch (type)
    {
        case MSG_REPLY_OK:
            contentLength = strlen(messageContent) + 14 + 1; // REPLY_OK_IS_ \r\n
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }
            
            strcpy(newMsg->messageContent, "REPLY OK IS ");
            strcat(newMsg->messageContent, messageContent);
            strcat(newMsg->messageContent, "\r\n");
            break;
        case MSG_REPLY_NOK:
            contentLength = strlen(messageContent) + 15 + 1; // REPLY_NOK_IS_ \r\n
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }
            
            strcpy(newMsg->messageContent, "REPLY NOK IS ");
            strcat(newMsg->messageContent, messageContent);
            strcat(newMsg->messageContent, "\r\n");
            break;
        case MSG_MSG:
        case MSG_ERR:
            contentLength = strlen(displayName) + strlen(messageContent) + 15 + 1; // ERR(MSG)_FROM_ _IS_ \r\n
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }

            if (type == MSG_MSG)
            {
                strcpy(newMsg->messageContent, "MSG FROM ");
            }
            else
            {
                strcpy(newMsg->messageContent, "ERR FROM ");
            }
            strcat(newMsg->messageContent, displayName);
            strcat(newMsg->messageContent, " IS ");
            strcat(newMsg->messageContent, messageContent);
            strcat(newMsg->messageContent, "\r\n");
            break;
        case MSG_BYE:
            contentLength = 5 + 1; // BYE\r\n
            newMsg->messageContent = (char *)malloc(sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL) { exit(2); }
            
            strcpy(newMsg->messageContent, "BYE\r\n");
            break;
        default:
            exit(1);
            break;
    }
    newMsg->type = type;
    newMsg->contentSize = contentLength;
}

/** Function to send data to client */
void TCPSend(int socket, char *messageToSend, int messageSize)
{
    int bytesTX = send(socket, messageToSend, messageSize - 1, 0);
    if (bytesTX < 0)
    {
        exit(3); //! error send
    }
}

/** Function to receive data from client */
void TCPRecv(int socket, char *buffer)
{
    // clear buffer array
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = '\0';
    }
    int bytesRX = recv(socket, buffer, BUFFER_SIZE, 0);
    if (bytesRX < 0)
    {
        exit(3); //! error recv
    }
}

/** Function sets connection with client */
void TCPsetup(programArguments *prgArgs, transmissionItems *connectionItems)
{
    // socket setup
    int welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket <= 0)
    {
        exit(3);
    }
    int enable = 1;
    setsockopt(welcomeSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

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

    // listening for incoming connections
    if (listen(welcomeSocket, MAX_USERS) < 0)
    {
        exit(3);
    }

    prgArgs->welcomeSocket = welcomeSocket;
}
