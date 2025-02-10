/**
 * @file udpCommunication.c
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "udpCommunication.h"


/** Function tries to exit correctly */
void tryToExitUDP(programArguments *prgArgs, transmissionItems *connectionItems)
{
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL)
    {
        exit(2);
    }

    // sending ERR message
    createMessageUDP(msg, MSG_ERR, NULL, NULL, NULL, prgArgs->displayName, "Invalid message! Trying to exit connection.", prgArgs->messageID++);
    fprintf(stderr, "ERR: Invalid message! Trying to exit connection.\n");
    int messageSize = msg->contentSize + 3;
    char *messageToSend = (char *)malloc(sizeof(char) * messageSize);
    if (messageToSend == NULL)
    {
        exit(2);
    }
    messageToSend[0] = (char)msg->type;
    messageToSend[1] = msg->messageID & 0xFF;
    messageToSend[2] = msg->messageID >> 8;
    for (unsigned i = 0; i < msg->contentSize; i++)
    {
        messageToSend[3 + i] = msg->messageContent[i];
    }

    int arrivingReply, confirmExpected;
    int retransmissions = 0;
    // wait for confirmation message
    do
    {
        UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
        arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
        if (arrivingReply == -1)
        {
            exit(3); //! select error
        }
        retransmissions++;
    } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
    if (!arrivingReply)
    {
        //! after n retransmissions still no answer
        fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
    }
    
    retransmissions = 0;
    UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

    // confirm is processed
    confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
    if (confirmExpected >= 0)
    {
        //! expected confirm message
        fprintf(stderr, "ERR: Expected confirm message!\n");
    }

    // sending BYE message
    createMessageUDP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL, prgArgs->messageID++);
    messageSize = msg->contentSize + 3;
    messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
    if (messageToSend == NULL)
    {
        exit(2);
    }
    messageToSend[0] = (char)msg->type;
    messageToSend[1] = msg->messageID & 0xFF;
    messageToSend[2] = msg->messageID >> 8;
    for (unsigned i = 0; i < msg->contentSize; i++)
    {
        messageToSend[3 + i] = msg->messageContent[i];
    }

    // wait for confirmation message
    do
    {
        UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
        arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
        if (arrivingReply == -1)
        {
            exit(3); //! select error
        }
        retransmissions++;
    } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
    if (!arrivingReply)
    {
        //! after n retransmissions still no answer
        fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
    }
    
    retransmissions = 0;
    UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

    // confirm is processed
    confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
    if (confirmExpected >= 0)
    {
        //! expected confirm message
        fprintf(stderr, "ERR: Expected confirm message!\n");
    }

    free(msg->messageContent);
    free(msg);
    free(messageToSend);
    close(connectionItems->socketFD);
    exit(0);
}

/** Function handles UDP messages from server */
int messageFromServerUDP(char *buffer, message *confirmMessage, programArguments *prgArgs)
{
    uint16_t originalMessageID = confirmMessage->messageID;
    char byte1 = (unsigned char)buffer[1];
    char byte2 = (unsigned char)buffer[2];
    uint16_t messageID = (byte1 << 8) | byte2;
    char *content = (buffer + 3);

    switch ((unsigned char)buffer[0])
    {
        case MSG_CONFIRMS:
            return (-(int)messageID) - 1;
            break;
        case MSG_REPLY:
        { //? due to the declaration after label (gcc with -pedantic gave warning)
            char success = (unsigned char)content[0];
            char refMsgByte1 = (unsigned char)content[1];
            char refMsgByte2 = (unsigned char)content[2];
            uint16_t refMessageID = (refMsgByte1 << 8) | refMsgByte2;

            // messageID check
            if (prgArgs->originalMessageID)
            {
                originalMessageID = prgArgs->originalMessageID;
                prgArgs->originalMessageID = 0;
            }

            if (originalMessageID != refMessageID)
            {
                //! reply has malformed refMessageID
                return RETURN_UNKNOWN;
            }
            
            content += 3;
            if (success == '\0')
            {
                fprintf(stderr, "Failure: %s\n", content);
                createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, NULL, NULL, NULL, messageID);
                return RETURN_NEGATIVE_REPLY;
            }

            fprintf(stderr, "Success: %s\n", content);
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, NULL, NULL, NULL, messageID);
            return RETURN_POSITIVE_REPLY;
            break;
        }
        case MSG_MSG:
        {
            char displayName[21];
            char messageContent[1401];
            strcpy(displayName, content);
            content += strlen(content) + 1;
            strcpy(messageContent, content);
          
            printf("%s: %s\n", displayName, messageContent);
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, NULL, NULL, NULL, messageID);
            return RETURN_MSG;
            break;
        }
        case MSG_ERR:
        {
            char displayName[21];
            char messageContent[1401];
            strcpy(displayName, content);
            content += strlen(content) + 1;
            strcpy(messageContent, content);
            
            fprintf(stderr, "ERR FROM %s: %s\n", displayName, messageContent);
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, NULL, NULL, NULL, messageID);
            return RETURN_ERR;
            break;
        }
        case MSG_BYE:
            createMessageUDP(confirmMessage, MSG_CONFIRMS, NULL, NULL, NULL, NULL, NULL, messageID);
            return RETURN_BYE;
            break;
        default:
            return RETURN_UNKNOWN;
            break;
    }
    return 0;
}

/** Function create a UDP message and returns it */
void createMessageUDP(message *newMsg, msgType type, char *username, char *channelID, char *secret, char *displayName, char *messageContent, uint16_t messageID)
{
    char *content = NULL;
    unsigned int contentLength = 0;
    unsigned int pos = 0;

    switch (type)
    {
        case MSG_CONFIRMS:
            content = (char *)malloc(sizeof(char));
            if (content == NULL)
            {
                exit(2);
            }
            content[0] = '\0';
            break;
        case MSG_AUTH:
            contentLength = strlen(username) + strlen(secret) + strlen(displayName) + 3;
            content = (char *)malloc(sizeof(char) * contentLength);
            if (content == NULL)
            {
                exit(2);
            }

            for (unsigned int i = 0; i < strlen(username); i++)
            {
                content[pos++] = username[i];
            }
            content[pos++] = '\0';

            for (unsigned int i = 0; i < strlen(displayName); i++)
            {
                content[pos++] = displayName[i];
            }
            content[pos++] = '\0';

            for (unsigned int i = 0; i < strlen(secret); i++)
            {
                content[pos++] = secret[i];
            }
            content[pos++] = '\0';

            break;
        case MSG_JOIN:
            contentLength = strlen(channelID) + strlen(displayName) + 2;
            content = (char *)malloc(sizeof(char) * contentLength);
            if (content == NULL)
            {
                exit(2);
            }

            for (unsigned int i = 0; i < strlen(channelID); i++)
            {
                content[pos++] = channelID[i];
            }
            content[pos++] = '\0';

            for (unsigned int i = 0; i < strlen(displayName); i++)
            {
                content[pos++] = displayName[i];
            }
            content[pos++] = '\0';
            break;
        case MSG_MSG:
        case MSG_ERR:
            contentLength = strlen(displayName) + strlen(messageContent) + 2;
            content = (char *)malloc(sizeof(char) * contentLength);
            if (content == NULL)
            {
                exit(2);
            }

            for (unsigned int i = 0; i < strlen(displayName); i++)
            {
                content[pos++] = displayName[i];
            }
            content[pos++] = '\0';

            for (unsigned int i = 0; i < strlen(messageContent); i++)
            {
                content[pos++] = messageContent[i];
            }
            content[pos++] = '\0';
            break;
        case MSG_BYE:
            content = (char *)malloc(sizeof(char));
            if (content == NULL)
            {
                exit(2);
            }
            content[0] = '\0';
            break;
        default:
            exit(1);
            break;
    }
    newMsg->type = type;
    newMsg->messageID = messageID;
    newMsg->contentSize = contentLength;
    newMsg->messageContent = content;
}

/** Function to send data to server */
void UDPSend(int socketFD, char *messageToSend, int messageSize, struct sockaddr *address, socklen_t addressSize)
{
    int bytesTX = sendto(socketFD, messageToSend, messageSize, 0, address, addressSize);
    if (bytesTX < 0)
    {
        exit(3); //! error sendto
    }
}

/** Function to receive data from server */
void UDPRecv(int socketFD, char *buffer, struct sockaddr *address, socklen_t addressSize)
{
    int bytesRX = recvfrom(socketFD, buffer, BUFFER_SIZE, 0, address, &addressSize);
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
    int socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    prgArgs->socketFD = socketFD;
    connectionItems->socketFD = socketFD;
    if (socketFD <= 0)
    {
        exit(3);
    }
    FD_ZERO(&connectionItems->readSocks);
    FD_SET(socketFD, &connectionItems->readSocks);


    struct addrinfo hints, *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;

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
            break;
        }
    }

    if (rp == NULL)
    {
        exit(53); //! connection failure
    }

    freeaddrinfo(result);
}

/** Function provides UDP communication with server */
void UDPcommunication(programArguments *prgArgs, message *msg, transmissionItems *connectionItems)
{
    // msg processing
    char *messageToSend;
    int messageSize;
    if (msg->type != MESSAGE_ARRIVED_WITHOUT_ASKING)
    {
        messageSize = msg->contentSize + 3;
        messageToSend = (char *)malloc(sizeof(char) * messageSize);
        if (messageToSend == NULL)
        {
            exit(2);
        }
        messageToSend[0] = (char)msg->type;
        messageToSend[1] = msg->messageID >> 8;
        messageToSend[2] = msg->messageID & 0xFF;
        for (unsigned i = 0; i < msg->contentSize; i++)
        {
            messageToSend[3 + i] = msg->messageContent[i];
        }
    }


    int arrivingReply, confirmExpected;
    int retransmissions = 0;
    msgReturnType msgReturnValue;
    switch (msg->type)
    {
        case MESSAGE_ARRIVED_WITHOUT_ASKING:
            UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);
            msgReturnValue = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);

            // creating confirm message and sending it
            messageSize = msg->contentSize + 3;
            messageToSend = (char *)malloc(sizeof(char) * messageSize);
            if (messageToSend == NULL)
            {
                exit(2);
            }
            messageToSend[0] = (char)msg->type;
            messageToSend[1] = msg->messageID >> 8;
            messageToSend[2] = msg->messageID & 0xFF;
            for (unsigned i = 0; i < msg->contentSize; i++)
            {
                messageToSend[3 + i] = msg->messageContent[i];
            }

            UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
            
            // if server send ERR, BYE or UNKNOWN message
            if (msgReturnValue == RETURN_ERR)
            {
                createMessageUDP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL, prgArgs->messageID++);
                messageSize = msg->contentSize + 3;
                messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
                if (messageToSend == NULL)
                {
                    exit(2);
                }
                messageToSend[0] = (char)msg->type;
                messageToSend[1] = msg->messageID & 0xFF;
                messageToSend[2] = msg->messageID >> 8;
                for (unsigned i = 0; i < msg->contentSize; i++)
                {
                    messageToSend[3 + i] = msg->messageContent[i];
                }

                // wait for confirmation message
                do
                {
                    UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                    arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                    if (arrivingReply == -1)
                    {
                        exit(3); //! select error
                    }
                    retransmissions++;
                } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
                if (!arrivingReply)
                {
                    //! after n retransmissions still no answer
                    fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                    return;
                }
                
                retransmissions = 0;
                UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

                // confirm is processed
                confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
                if (confirmExpected >= 0)
                {
                    //! expected confirm message
                    tryToExitUDP(prgArgs, connectionItems);
                }

                free(msg->messageContent);
                free(msg);
                free(messageToSend);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_BYE)
            {
                free(msg->messageContent);
                free(msg);
                free(messageToSend);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_UNKNOWN)
            {
                // sending ERR message
                createMessageUDP(msg, MSG_ERR, NULL, NULL, NULL, prgArgs->displayName, "Unknown message type!", prgArgs->messageID++);
                fprintf(stderr, "ERR: Unknown message type!\n");
                messageSize = msg->contentSize + 3;
                messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
                if (messageToSend == NULL)
                {
                    exit(2);
                }
                messageToSend[0] = (char)msg->type;
                messageToSend[1] = msg->messageID & 0xFF;
                messageToSend[2] = msg->messageID >> 8;
                for (unsigned i = 0; i < msg->contentSize; i++)
                {
                    messageToSend[3 + i] = msg->messageContent[i];
                }

                // wait for confirmation message
                do
                {
                    UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                    arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                    if (arrivingReply == -1)
                    {
                        exit(3); //! select error
                    }
                    retransmissions++;
                } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
                if (!arrivingReply)
                {
                    //! after n retransmissions still no answer
                    fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                    return;
                }
                
                retransmissions = 0;
                UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

                // confirm is processed
                confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
                if (confirmExpected >= 0)
                {
                    //! expected confirm message
                    tryToExitUDP(prgArgs, connectionItems);
                }

                // sending BYE message
                createMessageUDP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL, prgArgs->messageID++);
                messageSize = msg->contentSize + 3;
                messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
                if (messageToSend == NULL)
                {
                    exit(2);
                }
                messageToSend[0] = (char)msg->type;
                messageToSend[1] = msg->messageID & 0xFF;
                messageToSend[2] = msg->messageID >> 8;
                for (unsigned i = 0; i < msg->contentSize; i++)
                {
                    messageToSend[3 + i] = msg->messageContent[i];
                }

                // wait for confirmation message
                do
                {
                    UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                    arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                    if (arrivingReply == -1)
                    {
                        exit(3); //! select error
                    }
                    retransmissions++;
                } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
                if (!arrivingReply)
                {
                    //! after n retransmissions still no answer
                    fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                    return;
                }
                
                retransmissions = 0;
                UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

                // confirm is processed
                confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
                if (confirmExpected >= 0)
                {
                    //! expected confirm message
                    tryToExitUDP(prgArgs, connectionItems);
                }

                free(msg->messageContent);
                free(msg);
                free(messageToSend);
                close(connectionItems->socketFD);
                exit(0);
            }
            break;
        case MSG_AUTH:
            // wait for confirmation message
            do
            {
                UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                if (arrivingReply == -1)
                {
                    exit(3); //! select error
                }
                retransmissions++;
            } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
            if (!arrivingReply)
            {
                //! after n retransmissions still no answer
                fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                return;
            }

            retransmissions = 0;
            UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

            // confirm is processed
            confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
            if (confirmExpected >= 0)
            {
                //! expected confirm message
                tryToExitUDP(prgArgs, connectionItems);
            }
            
            // wait for reply after auth command
            UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);
            // reply arrived

            msgReturnValue = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
            if (msgReturnValue != RETURN_NEGATIVE_REPLY)
            {
                prgArgs->auth = true;
            }

            // msg processing
            messageSize = msg->contentSize + 3;
            messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
            if (messageToSend == NULL)
            {
                exit(2);
            }
            messageToSend[0] = (char)msg->type;
            messageToSend[1] = msg->messageID >> 8;
            messageToSend[2] = msg->messageID & 0xFF;
            for (unsigned i = 0; i < msg->contentSize; i++)
            {
                messageToSend[3 + i] = msg->messageContent[i];
            }

            UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);

            // if there was ERR message from server
            if (msgReturnValue == RETURN_ERR)
            {
                createMessageUDP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL, prgArgs->messageID++);
                messageSize = msg->contentSize + 3;
                messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
                if (messageToSend == NULL)
                {
                    exit(2);
                }
                messageToSend[0] = (char)msg->type;
                messageToSend[1] = msg->messageID & 0xFF;
                messageToSend[2] = msg->messageID >> 8;
                for (unsigned i = 0; i < msg->contentSize; i++)
                {
                    messageToSend[3 + i] = msg->messageContent[i];
                }

                // wait for confirmation message
                do
                {
                    UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                    arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                    if (arrivingReply == -1)
                    {
                        exit(3); //! select error
                    }
                    retransmissions++;
                } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
                if (!arrivingReply)
                {
                    //! after n retransmissions still no answer
                    fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                    return;
                }
                
                retransmissions = 0;
                UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

                // confirm is processed
                confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
                if (confirmExpected >= 0)
                {
                    //! expected confirm message
                    tryToExitUDP(prgArgs, connectionItems);
                }

                free(msg->messageContent);
                free(msg);
                free(messageToSend);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_BYE)
            {
                free(msg->messageContent);
                free(msg);
                free(messageToSend);
                close(connectionItems->socketFD);
                exit(0);
            }
            else if (msgReturnValue == RETURN_UNKNOWN)
            {
                // sending ERR message
                createMessageUDP(msg, MSG_ERR, NULL, NULL, NULL, prgArgs->displayName, "Unknown message type!", prgArgs->messageID++);
                fprintf(stderr, "ERR: Unknown message type!\n");
                messageSize = msg->contentSize + 3;
                messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
                if (messageToSend == NULL)
                {
                    exit(2);
                }
                messageToSend[0] = (char)msg->type;
                messageToSend[1] = msg->messageID & 0xFF;
                messageToSend[2] = msg->messageID >> 8;
                for (unsigned i = 0; i < msg->contentSize; i++)
                {
                    messageToSend[3 + i] = msg->messageContent[i];
                }

                // wait for confirmation message
                do
                {
                    UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                    arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                    if (arrivingReply == -1)
                    {
                        exit(3); //! select error
                    }
                    retransmissions++;
                } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
                if (!arrivingReply)
                {
                    //! after n retransmissions still no answer
                    fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                    return;
                }
                
                retransmissions = 0;
                UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

                // confirm is processed
                confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
                if (confirmExpected >= 0)
                {
                    //! expected confirm message
                    tryToExitUDP(prgArgs, connectionItems);
                }

                // sending BYE message
                createMessageUDP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL, prgArgs->messageID++);
                messageSize = msg->contentSize + 3;
                messageToSend = (char *)realloc(messageToSend, sizeof(char) * messageSize);
                if (messageToSend == NULL)
                {
                    exit(2);
                }
                messageToSend[0] = (char)msg->type;
                messageToSend[1] = msg->messageID & 0xFF;
                messageToSend[2] = msg->messageID >> 8;
                for (unsigned i = 0; i < msg->contentSize; i++)
                {
                    messageToSend[3 + i] = msg->messageContent[i];
                }

                // wait for confirmation message
                do
                {
                    UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                    arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                    if (arrivingReply == -1)
                    {
                        exit(3); //! select error
                    }
                    retransmissions++;
                } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
                if (!arrivingReply)
                {
                    //! after n retransmissions still no answer
                    fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                    return;
                }
                
                retransmissions = 0;
                UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

                // confirm is processed
                confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
                if (confirmExpected >= 0)
                {
                    //! expected confirm message
                    tryToExitUDP(prgArgs, connectionItems);
                }

                free(msg->messageContent);
                free(msg);
                free(messageToSend);
                close(connectionItems->socketFD);
                exit(0);
            }
            break;
        case MSG_JOIN:
        case MSG_MSG:
            if (msg->type == MSG_JOIN)
            {
                prgArgs->originalMessageID = prgArgs->messageID - 1;
            }
            // wait for confirmation message
            do
            {
                UDPSend(connectionItems->socketFD, messageToSend, messageSize, connectionItems->address, connectionItems->addressSize);
                arrivingReply = select(FD_SETSIZE, &connectionItems->readSocks, NULL, NULL, &connectionItems->timeout);
                if (arrivingReply == -1)
                {
                    exit(3); //! select error
                }
                retransmissions++;
            } while (!arrivingReply && retransmissions <= prgArgs->retransmissions);
            if (!arrivingReply)
            {
                //! after n retransmissions still no answer
                fprintf(stderr, "ERR: After %d retransmissions still no reply!\n", prgArgs->retransmissions);
                return;
            }
            
            retransmissions = 0;
            UDPRecv(connectionItems->socketFD, connectionItems->buffer, connectionItems->address, connectionItems->addressSize);

            // confirm is processed
            confirmExpected = messageFromServerUDP(connectionItems->buffer, msg, prgArgs);
            if (confirmExpected >= 0)
            {
                //! expected confirm message
                tryToExitUDP(prgArgs, connectionItems);
            }
            break;
        default:
            exit(1);
            break;
    }
}
