/**
 * @file tcpCommunication.c
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "tcpCommunication.h"


/** Function handles TCP messages from server */
int messageFromServerTCP(char *buffer)
{
    if (strncmp(buffer, "ERR FROM ", 9) == 0)
    {
        buffer += 9;
        char displayName[21];
        char messageContent[1401];
        char *ISpos = strstr(buffer, " IS ");
        if (ISpos != NULL)
        {
            int displayNameLength = ISpos - buffer;
            if (displayNameLength > 20)
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }
            strncpy(displayName, buffer, displayNameLength);
            displayName[displayNameLength] = '\0';

            char *messageStart = ISpos + 4; // " IS "
            if (strlen(messageStart) > 1400)
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
        messageContent[messageLength - 1] = '\0';
        messageContent[messageLength - 2] = '\0';
        fprintf(stderr, "ERR FROM %s: %s\n", displayName, messageContent);
        return RETURN_ERR;
    }
    else if (strncmp(buffer, "MSG FROM ", 9) == 0)
    {
        buffer += 9;
        char displayName[21];
        char messageContent[1401];
        char *ISpos = strstr(buffer, " IS ");
        if (ISpos != NULL)
        {
            int displayNameLength = ISpos - buffer;
            if (displayNameLength > 20)
            {
                //! invalid characters or length
                return RETURN_UNKNOWN;
            }
            strncpy(displayName, buffer, displayNameLength);
            displayName[displayNameLength] = '\0';

            char *messageStart = ISpos + 4; // " IS "
            if (strlen(messageStart) > 1400)
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
        messageContent[messageLength - 1] = '\0';
        messageContent[messageLength - 2] = '\0';
        printf("%s: %s\n", displayName, messageContent);
        return RETURN_MSG;
    }
    else if (strncmp(buffer, "REPLY OK IS ", 12) == 0)
    {
        buffer += 12;
        int bufferLength = strlen(buffer);
        buffer[bufferLength - 1] = '\0';
        buffer[bufferLength - 2] = '\0';
        fprintf(stderr, "Success: %s\n", buffer);
        return RETURN_POSITIVE_REPLY;
    }
    else if (strncmp(buffer, "REPLY NOK IS ", 13) == 0)
    {
        buffer += 13;
        int bufferLength = strlen(buffer);
        buffer[bufferLength - 1] = '\0';
        buffer[bufferLength - 2] = '\0';
        fprintf(stderr, "Failure: %s\n", buffer);
        return RETURN_NEGATIVE_REPLY;
    }
    else if (strcmp(buffer, "BYE\r\n") == 0)
    {
        return RETURN_BYE;
    }
    else
    {
        return RETURN_UNKNOWN;
    }
    return 0;
}

/** Function create a TCP message and returns it */
void createMessageTCP(message *newMsg, msgType type, char *username, char *channelID, char *secret, char *displayName, char *messageContent)
{
    unsigned int contentLength = 0;

    switch (type)
    {
        case MSG_AUTH:
            contentLength = strlen(username) + strlen(secret) + strlen(displayName) + 18 + 1; // AUTH_ _AS_ _USING_ \r\n
            newMsg->messageContent = (char *)realloc(newMsg->messageContent, sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL)
            {
                exit(2);
            }

            strcpy(newMsg->messageContent, "AUTH ");
            strcat(newMsg->messageContent, username);
            strcat(newMsg->messageContent, " AS ");
            strcat(newMsg->messageContent, displayName);
            strcat(newMsg->messageContent, " USING ");
            strcat(newMsg->messageContent, secret);
            strcat(newMsg->messageContent, "\r\n");
            break;
        case MSG_JOIN:
            contentLength = strlen(channelID) + strlen(displayName) + 11 + 1; // JOIN_ _AS_ \r\n
            newMsg->messageContent = (char *)realloc(newMsg->messageContent, sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL)
            {
                exit(2);
            }

            strcpy(newMsg->messageContent, "JOIN ");
            strcat(newMsg->messageContent, channelID);
            strcat(newMsg->messageContent, " AS ");
            strcat(newMsg->messageContent, displayName);
            strcat(newMsg->messageContent, "\r\n");
            break;
        case MSG_MSG:
        case MSG_ERR:
            contentLength = strlen(displayName) + strlen(messageContent) + 15 + 1; // ERR(MSG)_FROM_ _IS_ \r\n
            newMsg->messageContent = (char *)realloc(newMsg->messageContent, sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL)
            {
                exit(2);
            }

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
            newMsg->messageContent = (char *)realloc(newMsg->messageContent, sizeof(char) * contentLength);
            if (newMsg->messageContent == NULL)
            {
                exit(2);
            }
            strcpy(newMsg->messageContent, "BYE\r\n");
            break;
        default:
            exit(1);
            break;
    }
    newMsg->type = type;
    newMsg->messageID = 0;
    newMsg->contentSize = contentLength;
}

/** Function to send data to server */
void TCPSend(int socketFD, char *messageToSend, int messageSize)
{
    int bytesTX = send(socketFD, messageToSend, messageSize - 1, 0);
    if (bytesTX < 0)
    {
        exit(3); //! error send
    }
}

/** Function to receive data from server */
void TCPRecv(int socketFD, char *buffer)
{
    // clear buffer array
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = '\0';
    }
    int bytesRX = recv(socketFD, buffer, BUFFER_SIZE, 0);
    if (bytesRX < 0)
    {
        exit(3); //! error recv
    }
}

/** Function sets connection with server */
void TCPsetup(programArguments *prgArgs, transmissionItems *connectionItems)
{
    // socket setup
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    prgArgs->socketFD = socketFD;
    connectionItems->socketFD = socketFD;
    if (socketFD <= 0)
    {
        exit(3);
    }

    struct addrinfo hints, *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_TCP;

    // convert port number to char *
    char portNumber[20];
    sprintf(portNumber, "%d", prgArgs->port);

    int status = getaddrinfo(prgArgs->serverAddress, portNumber, &hints, &result);
    if (status)
    {
        exit(3);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            memcpy(&connectionItems->serverAddress, ipv4, sizeof(connectionItems->serverAddress));

            connectionItems->address = (struct sockaddr *) &connectionItems->serverAddress;
            connectionItems->addressSize = sizeof(connectionItems->serverAddress);
            if (connect(connectionItems->socketFD, connectionItems->address, connectionItems->addressSize) != -1)
            {
                break;
            }
        }
    }

    if (rp == NULL)
    {
        exit(53); //! connection failure
    }

    freeaddrinfo(result);
}

/** Function provides UDP communication with server */
void TCPcommunication(programArguments *prgArgs, message *msg, transmissionItems *connectionItems)
{
    msgReturnType msgReturnValue;
    switch (msg->type)
    {
        case MESSAGE_ARRIVED_WITHOUT_ASKING:
            TCPRecv(connectionItems->socketFD, connectionItems->buffer);
            msgReturnValue = messageFromServerTCP(connectionItems->buffer);
            if (msgReturnValue == RETURN_ERR)
            {
                // ERR after MSG -> send BYE and end
                createMessageTCP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL);
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
                
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_BYE)
            {
                // BYE after MSG
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_UNKNOWN)
            {
                // UNKNOWN message after MSG -> send ERR and BYE and end
                createMessageTCP(msg, MSG_ERR, NULL, NULL, NULL, prgArgs->displayName, "Unknown message type!");
                fprintf(stderr, "ERR: Unknown message type!\n");
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);

                createMessageTCP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL);
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
                
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            break;
        case MSG_AUTH:
            TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
            TCPRecv(connectionItems->socketFD, connectionItems->buffer);
            msgReturnValue = messageFromServerTCP(connectionItems->buffer);
            if (msgReturnValue == RETURN_NEGATIVE_REPLY)
            {
                // NOK REPLY after AUTH
                break;
            }
            else if (msgReturnValue == RETURN_ERR)
            {
                // ERR after AUTH -> send BYE and end
                createMessageTCP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL);
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
                
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_UNKNOWN)
            {
                // UNKNOWN message after MSG -> send ERR and BYE and end
                createMessageTCP(msg, MSG_ERR, NULL, NULL, NULL, prgArgs->displayName, "Unknown message type!");
                fprintf(stderr, "ERR: Unknown message type!\n");
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);

                createMessageTCP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL);
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
                
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            // positive REPLY
            prgArgs->auth = true;
            break;
        case MSG_JOIN:
            TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
            TCPRecv(connectionItems->socketFD, connectionItems->buffer);
            msgReturnValue = messageFromServerTCP(connectionItems->buffer);

            if (msgReturnValue == RETURN_ERR)
            {
                // ERR after JOIN -> send BYE and end
                createMessageTCP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL);
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
                
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_BYE)
            {
                // BYE after JOIN
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_UNKNOWN)
            {
                // UNKNOWN message after JOIN -> send ERR and BYE and end
                createMessageTCP(msg, MSG_ERR, NULL, NULL, NULL, prgArgs->displayName, "Unknown message type!");
                fprintf(stderr, "ERR: Unknown message type!\n");
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);

                createMessageTCP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL);
                TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
                
                free(msg->messageContent);
                free(msg);
                free(prgArgs->displayName);
                close(connectionItems->socketFD);
                exit(0);
            }
            break;
        case MSG_MSG:
            TCPSend(connectionItems->socketFD, msg->messageContent, msg->contentSize);
            break;
        default:
            exit(1);
            break;
    }
}
