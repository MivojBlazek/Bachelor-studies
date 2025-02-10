/**
 * @file ipk24chat-server.c
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "users.h"
#include "udpCommunication.h"
#include "tcpCommunication.h"
#include "ipk24chat-server.h"

int UDPSet;
user *activeUsers;
int *numOfUsers;
pthread_mutex_t mutex;
programArguments *prgArgs;

/** Function prints help */
void printHelp()
{
    printf("Usage: ./ipk24chat-server [-l <server_listening_IP_address>] [-p <server_listening_port>] [-d <timeout>] [-r <retransmissions>] [-h]\n");
    printf("Options:\n");
    printf("  -l <server_listening_IP_address>\tServer listening IP address for welcome sockets (optional, default: 0.0.0.0)\n");
    printf("  -p <server_listening_port>\t\tServer listening port for welcome sockets (optional, default: 4567)\n");
    printf("  -d <timeout>\t\t\t\tUDP confirmation timeout (optional, default: 250)\n");
    printf("  -r <retransmissions>\t\t\tMaximum number of UDP retransmissions (optional, default: 3)\n");
    printf("  -h\t\t\t\t\tPrints program help output and exits\n");
}

/** Function finds out if the word consists of only alphanumeric characters */
bool isAlphaNumericWord(const char *word, int TCPorUDP)
{
    for (unsigned int i = 0; i < strlen(word); i++)
    {
        if (!isalnum(word[i]) && word[i] != '-')
        {
            if ((word[i] == '\r' || word[i] == '\n') && TCPorUDP == TCP && (strlen(word) - i) <= 2)
            {
                continue;
            }
            return false;
        }
    }
    return true;
}

/** Function finds out if the word has only printable characters */
bool isPrintableWord(const char *word, int TCPorUDP)
{
    for (unsigned int i = 0; i < strlen(word); i++)
    {
        if (word[i] < 0x21 || word[i] > 0x7E)
        {
            if ((word[i] == '\r' || word[i] == '\n') && TCPorUDP == TCP && (strlen(word) - i) <= 2)
            {
                continue;
            }
            return false;
        }
    }
    return true;
}

/** Function finds out if the string has only printable characters or spaces */
bool isPrintableWithSpaces(const char *string, int TCPorUDP)
{
    for (unsigned int i = 0; i < strlen(string); i++)
    {
        if (string[i] < 0x20 || string[i] > 0x7E)
        {
            if ((string[i] == '\r' || string[i] == '\n') && TCPorUDP == TCP && (strlen(string) - i) <= 2)
            {
                continue;
            }
            return false;
        }
    }
    return true;
}

/** Function ends communication with client */
void tryToExitUDP(transmissionItems *connectionItems, user *client, bool exists)
{
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL) { exit(2); }
    int arrivingReply, confirmExpected;
    int retransmissions;
    if (exists)
    {
        // user exists
        createMessageUDP(msg, MSG_ERR, "Server", "Invalid message arrived!", prgArgs->messageID++, 0);
        
        // wait for confirmation message
        retransmissions = 0;
        do
        {
            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
            {
                printf("SENT %s:%d | ERR\n", str,  ntohs(connectionItems->serverAddress.sin_port));
            }
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
            free(msg);
            pthread_mutex_lock(&mutex); //! LOCK
            removeUser(client->id, activeUsers, *numOfUsers);
            (*numOfUsers)--;
            pthread_mutex_unlock(&mutex); //! UNLOCK
            return;
        }
        
        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        
        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, client, connectionItems->serverAddress, NULL, false);
        if (confirmExpected >= 0)
        {
            //! expected confirm message
            free(msg);
            pthread_mutex_lock(&mutex); //! LOCK
            removeUser(client->id, activeUsers, *numOfUsers);
            (*numOfUsers)--;
            pthread_mutex_unlock(&mutex); //! UNLOCK
            return;
        }

        // create message to broadcast it
        char *messageContent = (char *)malloc(sizeof(char) * (strlen(client->displayName) + strlen(" has left ") + strlen(client->channel) + strlen(".") + 1));
        if (messageContent == NULL) { exit(2); }
        strcpy(messageContent, client->displayName);
        strcat(messageContent, " has left ");
        strcat(messageContent, client->channel);
        strcat(messageContent, ".");
        createMessageUDP(msg, MSG_MSG, "Server", messageContent, prgArgs->messageID++, 0);
        free(messageContent);

        // broadcast message about leaving channel
        pthread_mutex_lock(&mutex); //! LOCK
        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (usersInSameChannel == NULL) { exit(2); }
        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (protocolsOfUsers == NULL) { exit(2); }
        int numberOfReceivers = findUsersInSameChannel(client->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
        for (int i = 0; i < numberOfReceivers; i++)
        {
            if (protocolsOfUsers[i] == UDP)
            {
                UDPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
            }
            else // TCP
            {
                message *newMsg = (message *)malloc(sizeof(message));
                if (newMsg == NULL) { exit(2); }
                UDPtoTCP(newMsg, msg);
                TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
                free(newMsg);
            }
        }
        free(protocolsOfUsers);
        free(clientsAddressesInTheSameChannel);
        free(usersInSameChannel);
        UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        pthread_mutex_unlock(&mutex); //! UNLOCK
        
        createMessageUDP(msg, MSG_BYE, NULL, NULL, prgArgs->messageID++, 0);
        // wait for confirmation message
        retransmissions = 0;
        do
        {
            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
            {
                printf("SENT %s:%d | BYE\n", str,  ntohs(connectionItems->serverAddress.sin_port));
            }
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
            free(msg);
            pthread_mutex_lock(&mutex); //! LOCK
            removeUser(client->id, activeUsers, *numOfUsers);
            (*numOfUsers)--;
            pthread_mutex_unlock(&mutex); //! UNLOCK
            return;
        }
        
        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        
        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, client, connectionItems->serverAddress, NULL, false);
        if (confirmExpected >= 0)
        {
            //! expected confirm message
            free(msg);
            pthread_mutex_lock(&mutex); //! LOCK
            removeUser(client->id, activeUsers, *numOfUsers);
            (*numOfUsers)--;
            pthread_mutex_unlock(&mutex); //! UNLOCK
            return;
        }
        
        // remove user from activeUsers
        pthread_mutex_lock(&mutex); //! LOCK
        removeUser(client->id, activeUsers, *numOfUsers);
        (*numOfUsers)--;
        pthread_mutex_unlock(&mutex); //! UNLOCK
    }
    else
    {
        createMessageUDP(msg, MSG_ERR, "Server", "User is not authenticated!", prgArgs->messageID++, 0);

        retransmissions = 0;
        do
        {
            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
            {
                printf("SENT %s:%d | ERR\n", str,  ntohs(connectionItems->serverAddress.sin_port));
            }

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
            free(msg);
            return;
        }
        
        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        
        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, client, connectionItems->serverAddress, NULL, true);
        if (confirmExpected >= 0)
        {
            //! expected confirm message
            free(msg);
            return;
        }
    }
    free(msg);
}

/** Function converts TCP message to UDP format */
void TCPtoUDP(message *newMsg, const char *TCPMsg)
{
    // always it is MSG_MSG
    char *pointerToTCPMessage = (char *)malloc(sizeof(char) * (strlen(TCPMsg) - 9));
    if (pointerToTCPMessage == NULL) { exit(2); }
    strcpy(pointerToTCPMessage, TCPMsg + 9);
    char displayName[21];
    char messageContent[1401];
    char *ISpos = strstr(pointerToTCPMessage, " IS ");
    if (ISpos != NULL)
    {
        int displayNameLength = ISpos - pointerToTCPMessage;
        strncpy(displayName, pointerToTCPMessage, displayNameLength);
        displayName[displayNameLength] = '\0';

        char *messageStart = ISpos + 4; // " IS "
        strcpy(messageContent, messageStart);
    }
    int messageLength = strlen(messageContent);
    messageContent[messageLength - 1] = '\0';
    messageContent[messageLength - 2] = '\0';

    newMsg->contentSize = strlen(displayName) + strlen(messageContent) + 5; // x04/x0FE | x00 x00 | dName | x00 | content | x00
    newMsg->messageContent = (char *)malloc(sizeof(char) * newMsg->contentSize);
    if (newMsg->messageContent == NULL) { exit(2); }
    int pos = 0;

    newMsg->messageContent[pos++] = '\x04';
    newMsg->messageContent[pos++] = (char)((prgArgs->messageID >> 8) & 0xFF);
    newMsg->messageContent[pos++] = (char)(prgArgs->messageID & 0xFF);
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
    prgArgs->messageID++;
}

/** Function converts UDP message to TCP format */
void UDPtoTCP(message *newMsg, const message *UDPMsg)
{
    // always it is MSG_MSG
    char *pointerToUDPMessage = UDPMsg->messageContent + 3;
    char displayName[21];
    char messageContent[1401];
    strcpy(displayName, pointerToUDPMessage);
    pointerToUDPMessage += strlen(pointerToUDPMessage) + 1;
    strcpy(messageContent, pointerToUDPMessage);

    newMsg->contentSize = strlen(displayName) + strlen(messageContent) + 15 + 1; // MSG_FROM_ displayName _IS_ messageContent\r\n\0
    newMsg->messageContent = (char *)malloc(sizeof(char) * newMsg->contentSize);
    if (newMsg->messageContent == NULL) { exit(2); }
    sprintf(newMsg->messageContent, "MSG FROM %s IS %s\r\n", displayName, messageContent);
}

/** Function authenticates new UDP user */
void authUDP(transmissionItems *connectionItems, message *msg, user *newUser, uint16_t refMessageID)
{
    int arrivingReply, confirmExpected;
    int retransmissions = 0;
    // AUTH arrived and user doesn't exist
    if (authenticationSuccessful(newUser))
    {
        newUser->channel = (char *)malloc(sizeof(char) * 8); // Default
        if (newUser->channel == NULL) { exit(2); }
        strcpy(newUser->channel, "Default");
        newUser->socket = connectionItems->socket;
        newUser->protocol = UDP;
        newUser->clientAddress = connectionItems->serverAddress;
        char strId[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, strId, sizeof(strId)))
        {
            newUser->id = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
            if (newUser->id == NULL) { exit(2); }
            sprintf(newUser->id, "%s:%d", strId,  ntohs(connectionItems->serverAddress.sin_port));
        }

        pthread_mutex_lock(&mutex); //! LOCK
        addUserToActive(newUser, activeUsers, *numOfUsers);
        (*numOfUsers)++;
        pthread_mutex_unlock(&mutex); //! UNLOCK

        // send REPLY
        createMessageUDP(msg, MSG_REPLY_OK, NULL, "Authentication successful!", prgArgs->messageID++, refMessageID);

        // wait for confirmation message
        retransmissions = 0;
        do
        {
            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
            {
                printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
            }

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
            tryToExitUDP(connectionItems, newUser, true);
            return;
        }
        
        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        
        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, newUser, connectionItems->serverAddress, &refMessageID, true);
        if (confirmExpected >= 0)
        {
            //! expected confirm message
            tryToExitUDP(connectionItems, newUser, true);
            return;
        }

        char *messageContent = (char *)malloc(sizeof(char) * (strlen(newUser->displayName) + strlen(" has joined Default.") + 1));
        if (messageContent == NULL) { exit(2); }
        strcpy(messageContent, newUser->displayName);
        strcat(messageContent, " has joined Default.");
        createMessageUDP(msg, MSG_MSG, "Server", messageContent, prgArgs->messageID++, 0);
        free(messageContent);

        // broadcast this message
        pthread_mutex_lock(&mutex); //! LOCK
        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (usersInSameChannel == NULL) { exit(2); }
        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (protocolsOfUsers == NULL) { exit(2); }
        int numberOfReceivers = findUsersInSameChannel(newUser->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
        for (int i = 0; i < numberOfReceivers; i++)
        {
            if (protocolsOfUsers[i] == UDP)
            {
                UDPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
            }
            else // TCP
            {
                message *newMsg = (message *)malloc(sizeof(message));
                if (newMsg == NULL) { exit(2); }
                UDPtoTCP(newMsg, msg);
                TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
                free(newMsg);
            }
        }
        free(protocolsOfUsers);
        free(clientsAddressesInTheSameChannel);
        free(usersInSameChannel);
        UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        pthread_mutex_unlock(&mutex); //! UNLOCK
    }
    else
    {
        // send !REPLY
        createMessageUDP(msg, MSG_REPLY_NOK, NULL, "Authentication failed!", prgArgs->messageID++, refMessageID);

        // wait for confirmation message
        retransmissions = 0;
        do
        {
            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
            char str[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
            {
                printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
            }
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
            tryToExitUDP(connectionItems, newUser, true);
            return;
        }
        
        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        
        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, newUser, connectionItems->serverAddress, &refMessageID, true);
        if (confirmExpected >= 0)
        {
            //! expected confirm message
            tryToExitUDP(connectionItems, newUser, true);
            return;
        }
    }
}

/** Function joins UDP user to channel */
void joinUDP(transmissionItems *connectionItems, message *msg, user *existingUser, uint16_t refMessageID)
{
    // get size of message in connectionItems->buffer
    char *pointerToUDPMessage = connectionItems->buffer + 3;
    char channelID[21];
    char displayName[21];
    strcpy(channelID, pointerToUDPMessage);
    pointerToUDPMessage += strlen(pointerToUDPMessage) + 1;
    strcpy(displayName, pointerToUDPMessage);

    msg->contentSize = strlen(channelID) + strlen(displayName) + 5;
    msg->messageContent = (char *)malloc(sizeof(char) * msg->contentSize);
    if (msg->messageContent == NULL) { exit(2); }
    memcpy(msg->messageContent, connectionItems->buffer, msg->contentSize);

    existingUser->channel = (char *)malloc(sizeof(char) * (strlen(channelID) + 1));
    if (existingUser->channel == NULL) { exit(2); }
    existingUser->displayName = (char *)malloc(sizeof(char) * (strlen(displayName) + 1));
    if (existingUser->displayName == NULL) { exit(2); }
    strcpy(existingUser->channel, channelID);
    strcpy(existingUser->displayName, displayName);


    // send REPLY
    message *msgReply = (message *)malloc(sizeof(message));
    if (msgReply == NULL) { exit(2); }
    createMessageUDP(msgReply, MSG_REPLY_OK, NULL, "Join successful!", prgArgs->messageID++, refMessageID);
                            
    // wait for confirmation message
    int arrivingReply, confirmExpected;
    int retransmissions = 0;
    do
    {
        UDPSend(connectionItems->socket, msgReply->messageContent, msgReply->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
        {
            printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
        }
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
        tryToExitUDP(connectionItems, existingUser, true);
        return;
    }
    
    UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
    
    confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, existingUser, connectionItems->serverAddress, &refMessageID, true);
    if (confirmExpected >= 0)
    {
        //! expected confirm message
        tryToExitUDP(connectionItems, existingUser, true);
        return;
    }


    // create message to broadcast it
    char *messageContent = (char *)malloc(sizeof(char) * (strlen(existingUser->displayName) + strlen(" has joined ") + strlen(existingUser->channel) + strlen(".") + 1));
    if (messageContent == NULL) { exit(2); }
    strcpy(messageContent, existingUser->displayName);
    strcat(messageContent, " has joined ");
    strcat(messageContent, existingUser->channel);
    strcat(messageContent, ".");
    createMessageUDP(msg, MSG_MSG, "Server", messageContent, prgArgs->messageID++, 0);
    free(messageContent);

    // broadcast message about leaving channel
    pthread_mutex_lock(&mutex); //! LOCK
    int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
    if (usersInSameChannel == NULL) { exit(2); }
    struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
    if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
    int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
    if (protocolsOfUsers == NULL) { exit(2); }
    int numberOfReceivers = findUsersInSameChannel(existingUser->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
    for (int i = 0; i < numberOfReceivers; i++)
    {
        if (protocolsOfUsers[i] == UDP)
        {
            UDPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
        }
        else // TCP
        {
            message *newMsg = (message *)malloc(sizeof(message));
            if (newMsg == NULL) { exit(2); }
            UDPtoTCP(newMsg, msg);
            TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
            free(newMsg);
        }
    }
    free(protocolsOfUsers);
    free(clientsAddressesInTheSameChannel);
    free(usersInSameChannel);
    UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
    pthread_mutex_unlock(&mutex); //! UNLOCK
}

/** Function processes arrived message */
void msgUDP(transmissionItems *connectionItems, user *existingUser)
{
    // get size of message in connectionItems->buffer
    char *pointerToUDPMessage = connectionItems->buffer + 3;
    char displayName[21];
    char messageContent[1401];
    strcpy(displayName, pointerToUDPMessage);
    pointerToUDPMessage += strlen(pointerToUDPMessage) + 1;
    strcpy(messageContent, pointerToUDPMessage);

    int messageSize = strlen(displayName) + strlen(messageContent) + 5;

    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL) { exit(2); }
    msg->contentSize = messageSize;
    msg->messageContent = (char *)malloc(sizeof(char) * messageSize);
    if (msg->messageContent == NULL) { exit(2); }
    memcpy(msg->messageContent, connectionItems->buffer, messageSize);

    // forward message in connectionItems->buffer to clients in same channel
    pthread_mutex_lock(&mutex); //! LOCK
    int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
    if (usersInSameChannel == NULL) { exit(2); }
    struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
    if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
    int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
    if (protocolsOfUsers == NULL) { exit(2); }
    int numberOfReceivers = findUsersInSameChannel(existingUser->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
    for (int i = 0; i < numberOfReceivers; i++)
    {
        if (protocolsOfUsers[i] == UDP)
        {
            UDPSend(usersInSameChannel[i], connectionItems->buffer, messageSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
        }
        else // TCP
        {
            message *newMsg = (message *)malloc(sizeof(message));
            if (newMsg == NULL) { exit(2); }
            UDPtoTCP(newMsg, msg);
            TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
            free(newMsg);
        }
    }
    free(msg->messageContent);
    free(msg);
    free(protocolsOfUsers);
    free(clientsAddressesInTheSameChannel);
    free(usersInSameChannel);
    pthread_mutex_unlock(&mutex); //! UNLOCK
}

/** Function representing error state of FSM (trying to end communication) */
void errorStateTCP(transmissionItems *connectionItems, user *client)
{
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL) { exit(2); }

    char *messageContent = (char *)malloc(sizeof(char) * (strlen(client->displayName) + strlen(" has left ") + strlen(client->channel) + strlen(".") + 1));
    if (messageContent == NULL) { exit(2); }
    strcpy(messageContent, client->displayName);
    strcat(messageContent, " has left ");
    strcat(messageContent, client->channel);
    strcat(messageContent, ".");
    createMessageTCP(msg, MSG_MSG, "Server", messageContent);
    free(messageContent);

    // broadcast this message
    pthread_mutex_lock(&mutex); //! LOCK
    int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
    if (usersInSameChannel == NULL) { exit(2); }
    struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
    if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
    int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
    if (protocolsOfUsers == NULL) { exit(2); }
    int numberOfReceivers = findUsersInSameChannel(client->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
    for (int i = 0; i < numberOfReceivers; i++)
    {
        if (protocolsOfUsers[i] == TCP)
        {
            TCPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize);
        }
        else // UDP
        {
            message *newMsg = (message *)malloc(sizeof(message));
            if (newMsg == NULL) { exit(2); }
            TCPtoUDP(newMsg, msg->messageContent);
            UDPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
            free(newMsg);
        }
    }
    free(protocolsOfUsers);
    free(clientsAddressesInTheSameChannel);
    free(usersInSameChannel);
    TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
    pthread_mutex_unlock(&mutex); //! UNLOCK

    createMessageTCP(msg, MSG_BYE, NULL, NULL);
    TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
    
    free(msg);
    pthread_mutex_lock(&mutex); //! LOCK
    removeUser(client->id, activeUsers, *numOfUsers);
    (*numOfUsers)--;
    pthread_mutex_unlock(&mutex); //! UNLOCK
    close(connectionItems->socket);
    pthread_cancel(pthread_self());
}

/** Function representing open state of FSM (communication) */
void openStateTCP(transmissionItems *connectionItems, user *client)
{
    msgReturnType msgReturnValue;
    message msg;

    while (true)
    {
        user *clientBeforeJoin = (user *)malloc(sizeof(user));
        if (clientBeforeJoin == NULL) { exit(2); }
        memcpy(clientBeforeJoin, client, sizeof(user));
        TCPRecv(connectionItems->socket, connectionItems->buffer);
        msgReturnValue = messageFromClientTCP(connectionItems->buffer, client, connectionItems->serverAddress);
        if (msgReturnValue != RETURN_JOIN)
        {
            free(clientBeforeJoin);
        }

        switch (msgReturnValue)
        {
            case RETURN_AUTH:
                // client has been already authenticated
                createMessageTCP(&msg, MSG_ERR, "Server", "Already authenticated!");
                TCPSend(connectionItems->socket, msg.messageContent, msg.contentSize);
                errorStateTCP(connectionItems, client);
                break;
            case RETURN_JOIN:
            {
                // client left channel
                char *messageContent = (char *)malloc(sizeof(char) * (strlen(client->displayName) + strlen(" has left ") + strlen(clientBeforeJoin->channel) + strlen(".") + 1));
                if (messageContent == NULL) { exit(2); }
                strcpy(messageContent, client->displayName);
                strcat(messageContent, " has left ");
                strcat(messageContent, clientBeforeJoin->channel);
                strcat(messageContent, ".");
                createMessageTCP(&msg, MSG_MSG, "Server", messageContent);
                free(messageContent);

                // broadcast this message                    
                user *meAfter = (user *)malloc(sizeof(user));
                if (meAfter == NULL) { exit(2); }
                meAfter->channel = (char *)malloc(sizeof(char) * strlen(client->channel));
                if (meAfter->channel == NULL) { exit(2); }
                strcpy(meAfter->channel, client->channel);

                pthread_mutex_lock(&mutex); //! LOCK
                user *me = (user *)malloc(sizeof(user));
                if (me == NULL) { exit(2); }
                me = findUser(client->id, activeUsers, *numOfUsers);
                me->channel = (char *)malloc(sizeof(char) * strlen(clientBeforeJoin->channel));
                if (me->channel == NULL) { exit(2); }
                strcpy(me->channel, clientBeforeJoin->channel);

                int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (usersInSameChannel == NULL) { exit(2); }
                struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (protocolsOfUsers == NULL) { exit(2); }
                int numberOfReceivers = findUsersInSameChannel(client->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                for (int i = 0; i < numberOfReceivers; i++)
                {
                    if (protocolsOfUsers[i] == TCP)
                    {
                        TCPSend(usersInSameChannel[i], msg.messageContent, msg.contentSize);
                    }
                    else // UDP
                    {
                        message *newMsg = (message *)malloc(sizeof(message));
                        if (newMsg == NULL) { exit(2); }
                        TCPtoUDP(newMsg, msg.messageContent);
                        UDPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                        free(newMsg);
                    }
                }
                free(protocolsOfUsers);
                free(clientsAddressesInTheSameChannel);
                free(usersInSameChannel);
                TCPSend(connectionItems->socket, msg.messageContent, msg.contentSize);
                pthread_mutex_unlock(&mutex); //! UNLOCK

                free(me->channel);
                me->channel = (char *)malloc(sizeof(char) * strlen(meAfter->channel));
                if (me->channel == NULL) { exit(2); }
                strcpy(me->channel, meAfter->channel);
                free(meAfter->channel);
                free(meAfter);

                free(clientBeforeJoin);

                // send REPLY
                message msgReply;
                createMessageTCP(&msgReply, MSG_REPLY_OK, NULL, "Join successful!");
                TCPSend(connectionItems->socket, msgReply.messageContent, msgReply.contentSize);
                char str[INET_ADDRSTRLEN];
                if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                {
                    printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
                }

                message msg2;
                // client joined different channel
                char *messageContent2 = (char *)malloc(sizeof(char) * (strlen(client->displayName) + strlen(" has joined ") + strlen(client->channel) + strlen(".") + 1));
                if (messageContent2 == NULL) { exit(2); }
                strcpy(messageContent2, client->displayName);
                strcat(messageContent2, " has joined ");
                strcat(messageContent2, client->channel);
                strcat(messageContent2, ".");
                createMessageTCP(&msg2, MSG_MSG, "Server", messageContent2);
                free(messageContent2);

                // broadcast this message
                pthread_mutex_lock(&mutex); //! LOCK
                int *usersInSameChannel2 = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (usersInSameChannel2 == NULL) { exit(2); }
                struct sockaddr_in *clientsAddressesInTheSameChannel2 = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                if (clientsAddressesInTheSameChannel2 == NULL) { exit(2); }
                int *protocolsOfUsers2 = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (protocolsOfUsers2 == NULL) { exit(2); }
                int numberOfReceivers2 = findUsersInSameChannel(client->id, usersInSameChannel2, clientsAddressesInTheSameChannel2, protocolsOfUsers2, activeUsers, *numOfUsers);
                for (int i = 0; i < numberOfReceivers2; i++)
                {
                    if (protocolsOfUsers2[i] == TCP)
                    {
                        TCPSend(usersInSameChannel2[i], msg2.messageContent, msg2.contentSize);
                    }
                    else // UDP
                    {
                        message *newMsg = (message *)malloc(sizeof(message));
                        if (newMsg == NULL) { exit(2); }
                        TCPtoUDP(newMsg, msg2.messageContent);
                        UDPSend(usersInSameChannel2[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                        free(newMsg);
                    }
                }
                free(usersInSameChannel2);
                TCPSend(connectionItems->socket, msg2.messageContent, msg2.contentSize);
                pthread_mutex_unlock(&mutex); //! UNLOCK
                break;
            }
            case RETURN_MSG:
                // client send message -> forward to other clients
                pthread_mutex_lock(&mutex); //! LOCK
                int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (usersInSameChannel == NULL) { exit(2); }
                struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (protocolsOfUsers == NULL) { exit(2); }
                int numberOfReceivers = findUsersInSameChannel(client->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                for (int i = 0; i < numberOfReceivers; i++)
                {

                    if (protocolsOfUsers[i] == TCP)
                    {
                        TCPSend(usersInSameChannel[i], connectionItems->buffer, strlen(connectionItems->buffer) + 1);
                    }
                    else // UDP
                    {
                        message *newMsg = (message *)malloc(sizeof(message));
                        if (newMsg == NULL) { exit(2); }
                        TCPtoUDP(newMsg, connectionItems->buffer);
                        UDPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                        free(newMsg);
                    }
                }
                free(protocolsOfUsers);
                free(clientsAddressesInTheSameChannel);
                free(usersInSameChannel);
                pthread_mutex_unlock(&mutex); //! UNLOCK
                break;
            case RETURN_ERR:
            {
                char *messageContent = (char *)malloc(sizeof(char) * (strlen(client->displayName) + strlen(" has left ") + strlen(client->channel) + strlen(".") + 1));
                if (messageContent == NULL) { exit(2); }
                strcpy(messageContent, client->displayName);
                strcat(messageContent, " has left ");
                strcat(messageContent, client->channel);
                strcat(messageContent, ".");
                createMessageTCP(&msg, MSG_MSG, "Server", messageContent);
                free(messageContent);

                // broadcast this message
                pthread_mutex_lock(&mutex); //! LOCK
                int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (usersInSameChannel == NULL) { exit(2); }
                struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (protocolsOfUsers == NULL) { exit(2); }
                int numberOfReceivers = findUsersInSameChannel(client->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                for (int i = 0; i < numberOfReceivers; i++)
                {
                    if (protocolsOfUsers[i] == TCP)
                    {
                        TCPSend(usersInSameChannel[i], msg.messageContent, msg.contentSize);
                    }
                    else // UDP
                    {
                        message *newMsg = (message *)malloc(sizeof(message));
                        if (newMsg == NULL) { exit(2); }
                        TCPtoUDP(newMsg, msg.messageContent);
                        UDPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                        free(newMsg);
                    }
                }
                free(protocolsOfUsers);
                free(clientsAddressesInTheSameChannel);
                free(usersInSameChannel);
                TCPSend(connectionItems->socket, msg.messageContent, msg.contentSize);

                removeUser(client->id, activeUsers, *numOfUsers);
                (*numOfUsers)--;
                pthread_mutex_unlock(&mutex); //! UNLOCK

                createMessageTCP(&msg, MSG_BYE, NULL, NULL);
                TCPSend(connectionItems->socket, msg.messageContent, msg.contentSize);
                close(connectionItems->socket);
                pthread_cancel(pthread_self());
                break;
            }
            case RETURN_BYE:
            {
                char *messageContent = (char *)malloc(sizeof(char) * (strlen(client->displayName) + strlen(" has left ") + strlen(client->channel) + strlen(".") + 1));
                if (messageContent == NULL) { exit(2); }
                strcpy(messageContent, client->displayName);
                strcat(messageContent, " has left ");
                strcat(messageContent, client->channel);
                strcat(messageContent, ".");
                createMessageTCP(&msg, MSG_MSG, "Server", messageContent);
                free(messageContent);

                // broadcast this message
                pthread_mutex_lock(&mutex); //! LOCK
                int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (usersInSameChannel == NULL) { exit(2); }
                struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                if (protocolsOfUsers == NULL) { exit(2); }
                int numberOfReceivers = findUsersInSameChannel(client->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                for (int i = 0; i < numberOfReceivers; i++)
                {
                    if (protocolsOfUsers[i] == TCP)
                    {
                        TCPSend(usersInSameChannel[i], msg.messageContent, msg.contentSize);
                    }
                    else // UDP
                    {
                        message *newMsg = (message *)malloc(sizeof(message));
                        if (newMsg == NULL) { exit(2); }
                        TCPtoUDP(newMsg, msg.messageContent);
                        UDPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                        free(newMsg);
                    }
                }
                free(protocolsOfUsers);
                free(clientsAddressesInTheSameChannel);
                free(usersInSameChannel);
                TCPSend(connectionItems->socket, msg.messageContent, msg.contentSize);

                removeUser(client->id, activeUsers, *numOfUsers);
                (*numOfUsers)--;
                pthread_mutex_unlock(&mutex); //! UNLOCK

                close(connectionItems->socket);
                pthread_cancel(pthread_self());
                break;
            }
            case RETURN_UNKNOWN:
                createMessageTCP(&msg, MSG_ERR, "Server", "Invalid message arrived!");
                TCPSend(connectionItems->socket, msg.messageContent, msg.contentSize);
                errorStateTCP(connectionItems, client);
                break;
            default:
                exit(1);
                break;
        }
    }
}

/** Function representing auth state of FSM (there was invalid authentication) */
void authStateTCP(transmissionItems *connectionItems)
{
    msgReturnType msgReturnValue;
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL) { exit(2); }
    user newUser;

    TCPRecv(connectionItems->socket, connectionItems->buffer);
    msgReturnValue = messageFromClientTCP(connectionItems->buffer, &newUser, connectionItems->serverAddress);
    if (msgReturnValue != RETURN_AUTH)
    {
        if (msgReturnValue == RETURN_BYE)
        {
            close(connectionItems->socket);
            pthread_cancel(pthread_self());
        }
        createMessageTCP(msg, MSG_REPLY_NOK, NULL, "Authentication expected!");
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
        {
            printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
        }
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
        authStateTCP(connectionItems);
        return;
    }
    // check if authentication was successful
    if (authenticationSuccessful(&newUser))
    {
        newUser.channel = (char *)malloc(sizeof(char) * 8); // Default
        if (newUser.channel == NULL) { exit(2); }
        strcpy(newUser.channel, "Default");
        newUser.socket = connectionItems->socket;
        newUser.protocol = TCP;
        newUser.clientAddress = connectionItems->serverAddress;
        char strId[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, strId, sizeof(strId)))
        {
            newUser.id = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
            if (newUser.id == NULL) { exit(2); }
            sprintf(newUser.id, "%s:%d", strId,  ntohs(connectionItems->serverAddress.sin_port));
        }

        pthread_mutex_lock(&mutex); //! LOCK
        addUserToActive(&newUser, activeUsers, *numOfUsers);
        (*numOfUsers)++;
        pthread_mutex_unlock(&mutex); //! UNLOCK

        // send REPLY
        createMessageTCP(msg, MSG_REPLY_OK, NULL, "Authentication successful!");
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
        {
            printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
        }
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);

        char *messageContent = (char *)malloc(sizeof(char) * (strlen(newUser.displayName) + strlen(" has joined Default.") + 1));
        if (messageContent == NULL) { exit(2); }
        strcpy(messageContent, newUser.displayName);
        strcat(messageContent, " has joined Default.");
        createMessageTCP(msg, MSG_MSG, "Server", messageContent);
        free(messageContent);

        // broadcast this message
        pthread_mutex_lock(&mutex); //! LOCK
        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (usersInSameChannel == NULL) { exit(2); }
        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (protocolsOfUsers == NULL) { exit(2); }
        int numberOfReceivers = findUsersInSameChannel(newUser.id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
        for (int i = 0; i < numberOfReceivers; i++)
        {
            if (protocolsOfUsers[i] == TCP)
            {
                TCPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize);
            }
            else // UDP
            {
                message *newMsg = (message *)malloc(sizeof(message));
                if (newMsg == NULL) { exit(2); }
                TCPtoUDP(newMsg, msg->messageContent);
                UDPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                free(newMsg);
            }
        }
        free(protocolsOfUsers);
        free(clientsAddressesInTheSameChannel);
        free(usersInSameChannel);
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
        pthread_mutex_unlock(&mutex); //! UNLOCK

        openStateTCP(connectionItems, &newUser);
    }
    else
    {
        // send !REPLY
        createMessageTCP(msg, MSG_REPLY_NOK, NULL, "Authentication failed!");
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
        {
            printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
        }
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
        authStateTCP(connectionItems);
    }
    free(msg);
}

/** Function representing accept state of FSM (waiting for client authentication) */
void acceptStateTCP(transmissionItems *connectionItems)
{
    // first message is already in buffer
    msgReturnType msgReturnValue;
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL) { exit(2); }
    user newUser;

    msgReturnValue = messageFromClientTCP(connectionItems->buffer, &newUser, connectionItems->serverAddress);
    if (msgReturnValue != RETURN_AUTH)
    {
        createMessageTCP(msg, MSG_REPLY_NOK, NULL, "Authentication expected!");
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
        {
            printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
        }
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
        authStateTCP(connectionItems);
        return;
    }
    // check if authentication was successful
    if (authenticationSuccessful(&newUser))
    {
        newUser.channel = (char *)malloc(sizeof(char) * 8); // Default
        if (newUser.channel == NULL) { exit(2); }
        strcpy(newUser.channel, "Default");
        newUser.socket = connectionItems->socket;
        newUser.protocol = TCP;
        newUser.clientAddress = connectionItems->serverAddress;
        char strId[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, strId, sizeof(strId)))
        {
            newUser.id = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
            if (newUser.id == NULL) { exit(2); }
            sprintf(newUser.id, "%s:%d", strId,  ntohs(connectionItems->serverAddress.sin_port));
        }

        pthread_mutex_lock(&mutex); //! LOCK
        addUserToActive(&newUser, activeUsers, *numOfUsers);
        (*numOfUsers)++;
        pthread_mutex_unlock(&mutex); //! UNLOCK

        // send REPLY
        createMessageTCP(msg, MSG_REPLY_OK, NULL, "Authentication successful!");
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
        {
            printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
        }
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);

        char *messageContent = (char *)malloc(sizeof(char) * (strlen(newUser.displayName) + strlen(" has joined Default.") + 1));
        if (messageContent == NULL) { exit(2); }
        strcpy(messageContent, newUser.displayName);
        strcat(messageContent, " has joined Default.");
        createMessageTCP(msg, MSG_MSG, "Server", messageContent);
        free(messageContent);

        // broadcast this message
        pthread_mutex_lock(&mutex); //! LOCK
        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (usersInSameChannel == NULL) { exit(2); }
        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
        if (protocolsOfUsers == NULL) { exit(2); }
        int numberOfReceivers = findUsersInSameChannel(newUser.id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
        for (int i = 0; i < numberOfReceivers; i++)
        {
            if (protocolsOfUsers[i] == TCP)
            {
                TCPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize);
            }
            else // UDP
            {
                message *newMsg = (message *)malloc(sizeof(message));
                if (newMsg == NULL) { exit(2); }
                TCPtoUDP(newMsg, msg->messageContent);
                UDPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize, (struct sockaddr *)&clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                free(newMsg);
            }
        }
        free(protocolsOfUsers);
        free(clientsAddressesInTheSameChannel);
        free(usersInSameChannel);
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
        pthread_mutex_unlock(&mutex); //! UNLOCK

        openStateTCP(connectionItems, &newUser);
    }
    else
    {
        // send !REPLY
        createMessageTCP(msg, MSG_REPLY_NOK, NULL, "Authentication failed!");
        char str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
        {
            printf("SENT %s:%d | REPLY\n", str,  ntohs(connectionItems->serverAddress.sin_port));
        }
        TCPSend(connectionItems->socket, msg->messageContent, msg->contentSize);
        authStateTCP(connectionItems);
    }
    free(msg);
}

/** Function initializes default program arguments */
void initDefaultProgramArguments()
{
    prgArgs->listeningIP = (char *)malloc(sizeof(char) * 16);
    if (prgArgs->listeningIP == NULL) { exit(2); }
    prgArgs->listeningIP = "0.0.0.0";
    prgArgs->port = 4567;
    prgArgs->timeout = 250;
    prgArgs->retransmissions = 3;
    prgArgs->messageID = 0;
}

/** Function handles Ctrl+C and Ctrl+D */
void controlCorD()
{
    // send BYE to all users and end communication
    message byeMessage;
    pthread_mutex_lock(&mutex); //! LOCK
    for (int i = 0; i < *numOfUsers; i++)
    {
        if (activeUsers[i].protocol == TCP)
        {
            createMessageTCP(&byeMessage, MSG_BYE, NULL, NULL);
            TCPSend(activeUsers[i].socket, byeMessage.messageContent, byeMessage.contentSize);
        }
        else // UDP
        {
            createMessageUDP(&byeMessage, MSG_BYE, NULL, NULL, prgArgs->messageID++, 0);
            UDPSend(activeUsers[i].socket, byeMessage.messageContent, byeMessage.contentSize, (struct sockaddr *) &activeUsers[i].clientAddress, sizeof(activeUsers[i].clientAddress));
        }
        close(activeUsers[i].socket);
    }
    pthread_mutex_unlock(&mutex); //! UNLOCK

    free(prgArgs);
    shmdt(activeUsers);
    shmdt(numOfUsers);
    shmctl(shmget(SHM_KEY_ONE, sizeof(user) * MAX_USERS, 0), IPC_RMID, NULL);
    shmctl(shmget(SHM_KEY_TWO, sizeof(int), 0), IPC_RMID, NULL);
    pthread_mutex_destroy(&mutex);
    exit(0);
}

/** Function that is called after creating a new thread representing client */
void *clientHandlerTCP(void *arg)
{
    transmissionItems *connectionItems = malloc(sizeof(transmissionItems));
    if (connectionItems == NULL) { exit(2); }
    memcpy(connectionItems, arg, sizeof(transmissionItems));

    TCPRecv(connectionItems->socket, connectionItems->buffer);
    acceptStateTCP(connectionItems);
    free(connectionItems);
    return NULL;
}

/** Function that is called after creating a new thread representing UDP clients */
void *clientHandlerUDP(void *arg)
{
    transmissionItems *connectionItems = malloc(sizeof(transmissionItems));
    if (connectionItems == NULL) { exit(2); }
    memcpy(connectionItems, arg, sizeof(transmissionItems));

    fd_set readSockets;
    FD_ZERO(&readSockets);
    while (true)
    {
        FD_SET(connectionItems->socket, &readSockets);

        int activityOnSocket = select(FD_SETSIZE, &readSockets, NULL, NULL, NULL);
        if (activityOnSocket == -1)
        {
            exit(3); //! select failed
        }
        else if (activityOnSocket > 0)
        {
            if (FD_ISSET(connectionItems->socket, &readSockets)) // UDP
            {
                // message from UDP arrived
                int arrivingReply, confirmExpected;
                int retransmissions = 0;
                message *msg = (message *)malloc(sizeof(message));
                if (msg == NULL) { exit(2); }
                user newUser;
                uint16_t refMessageID = 0;
                UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                int msgReturnValue = messageFromClientUDP(connectionItems->buffer, msg, &newUser, connectionItems->serverAddress, &refMessageID, false);
                if (msgReturnValue >= 0)
                {
                    createMessageUDP(msg, MSG_CONFIRMS, NULL, NULL, 0, refMessageID);
                    UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                    char str[INET_ADDRSTRLEN];
                    if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                    {
                        printf("SENT %s:%d | CONFIRM\n", str,  ntohs(connectionItems->serverAddress.sin_port));
                    }
                }
                switch (msgReturnValue)
                {
                    case RETURN_AUTH:
                    {
                        char str[INET_ADDRSTRLEN];
                        char *newId = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
                        if (newId == NULL) { exit(2); }
                        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                        {
                            sprintf(newId, "%s:%d", str,  ntohs(connectionItems->serverAddress.sin_port));
                        }
                        user *existingUserWithSameId = findUser(newId, activeUsers, *numOfUsers);
                        free(newId);

                        if (existingUserWithSameId == NULL)
                        {
                            // new user
                            authUDP(connectionItems, msg, &newUser, refMessageID);
                        }
                        else
                        {
                            // user already exists
                            createMessageUDP(msg, MSG_ERR, "Server", "Already authenticated!", prgArgs->messageID++, 0);
                            
                            // wait for confirmation message
                            retransmissions = 0;
                            do
                            {
                                UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                                char str[INET_ADDRSTRLEN];
                                if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                                {
                                    printf("SENT %s:%d | ERR\n", str,  ntohs(connectionItems->serverAddress.sin_port));
                                }
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
                                tryToExitUDP(connectionItems, &newUser, true);
                            }
                            
                            UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                            
                            confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, &newUser, connectionItems->serverAddress, &refMessageID, false);
                            if (confirmExpected >= 0)
                            {
                                //! expected confirm message
                                tryToExitUDP(connectionItems, &newUser, true);
                            }
                            
                            createMessageUDP(msg, MSG_BYE, NULL, NULL, prgArgs->messageID++, 0);
                            // wait for confirmation message
                            retransmissions = 0;
                            do
                            {
                                UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                                char str[INET_ADDRSTRLEN];
                                if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                                {
                                    printf("SENT %s:%d | BYE\n", str,  ntohs(connectionItems->serverAddress.sin_port));
                                }
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
                                tryToExitUDP(connectionItems, &newUser, true);
                            }
                            
                            UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                            
                            confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, &newUser, connectionItems->serverAddress, &refMessageID, false);
                            if (confirmExpected >= 0)
                            {
                                //! expected confirm message
                                tryToExitUDP(connectionItems, &newUser, true);
                            }
                            
                            // remove user from activeUsers
                            free(msg);
                            pthread_mutex_lock(&mutex); //! LOCK
                            removeUser(newId, activeUsers, *numOfUsers);
                            (*numOfUsers)--;
                            pthread_mutex_unlock(&mutex); //! UNLOCK
                        }
                        break;
                    }
                    case RETURN_JOIN:
                    {
                        char str[INET_ADDRSTRLEN];
                        char *newId = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
                        if (newId == NULL) { exit(2); }
                        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                        {
                            sprintf(newId, "%s:%d", str,  ntohs(connectionItems->serverAddress.sin_port));
                        }
                        user *existingUser = findUser(newId, activeUsers, *numOfUsers);
                        free(newId);

                        if (existingUser == NULL)
                        {
                            //! user doesn't exist
                            tryToExitUDP(connectionItems, &newUser, false);
                        }

                        // create message to broadcast it
                        char *messageContent = (char *)malloc(sizeof(char) * (strlen(newUser.displayName) + strlen(" has left ") + strlen(existingUser->channel) + strlen(".") + 1));
                        if (messageContent == NULL) { exit(2); }
                        strcpy(messageContent, newUser.displayName);
                        strcat(messageContent, " has left ");
                        strcat(messageContent, existingUser->channel);
                        strcat(messageContent, ".");
                        createMessageUDP(msg, MSG_MSG, "Server", messageContent, prgArgs->messageID++, 0);
                        free(messageContent);

                        // broadcast message about leaving channel
                        pthread_mutex_lock(&mutex); //! LOCK
                        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (usersInSameChannel == NULL) { exit(2); }
                        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (protocolsOfUsers == NULL) { exit(2); }
                        int numberOfReceivers = findUsersInSameChannel(existingUser->id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                        for (int i = 0; i < numberOfReceivers; i++)
                        {
                            if (protocolsOfUsers[i] == UDP)
                            {
                                UDPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                            }
                            else // TCP
                            {
                                message *newMsg = (message *)malloc(sizeof(message));
                                if (newMsg == NULL) { exit(2); }
                                UDPtoTCP(newMsg, msg);
                                TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
                                free(newMsg);
                            }
                        }
                        free(protocolsOfUsers);
                        free(clientsAddressesInTheSameChannel);
                        free(usersInSameChannel);
                        UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                        pthread_mutex_unlock(&mutex); //! UNLOCK


                        existingUser->channel = (char *)malloc(sizeof(char) * strlen(newUser.channel));
                        existingUser->displayName = (char *)malloc(sizeof(char) * strlen(newUser.displayName));
                        strcpy(existingUser->channel, newUser.channel);
                        strcpy(existingUser->displayName, newUser.displayName);

                        joinUDP(connectionItems, msg, existingUser, refMessageID);
                        break;
                    }
                    case RETURN_MSG:
                    {
                        char str[INET_ADDRSTRLEN];
                        char *newId = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
                        if (newId == NULL) { exit(2); }
                        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                        {
                            sprintf(newId, "%s:%d", str,  ntohs(connectionItems->serverAddress.sin_port));
                        }
                        user *existingUser = findUser(newId, activeUsers, *numOfUsers);
                        free(newId);

                        if (existingUser == NULL)
                        {
                            //! user doesn't exist
                            tryToExitUDP(connectionItems, &newUser, false);
                        }
                        msgUDP(connectionItems, &newUser);
                        break;
                    }
                    case RETURN_ERR:
                    {
                        char str[INET_ADDRSTRLEN];
                        char *newId = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
                        if (newId == NULL) { exit(2); }
                        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                        {
                            sprintf(newId, "%s:%d", str,  ntohs(connectionItems->serverAddress.sin_port));
                        }
                        user *existingUser = findUser(newId, activeUsers, *numOfUsers);
                        free(newId);

                        if (existingUser == NULL)
                        {
                            //! user doesn't exist
                            tryToExitUDP(connectionItems, &newUser, false);
                        }

                        // create message to broadcast it
                        char *messageContent = (char *)malloc(sizeof(char) * (strlen(newUser.displayName) + strlen(" has left ") + strlen(newUser.channel) + strlen(".") + 1));
                        if (messageContent == NULL) { exit(2); }
                        strcpy(messageContent, newUser.displayName);
                        strcat(messageContent, " has left ");
                        strcat(messageContent, newUser.channel);
                        strcat(messageContent, ".");
                        createMessageUDP(msg, MSG_MSG, "Server", messageContent, prgArgs->messageID++, 0);
                        free(messageContent);

                        // broadcast message about leaving channel
                        pthread_mutex_lock(&mutex); //! LOCK
                        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (usersInSameChannel == NULL) { exit(2); }
                        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (protocolsOfUsers == NULL) { exit(2); }
                        int numberOfReceivers = findUsersInSameChannel(newUser.id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                        for (int i = 0; i < numberOfReceivers; i++)
                        {
                            if (protocolsOfUsers[i] == UDP)
                            {
                                UDPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                            }
                            else // TCP
                            {
                                message *newMsg = (message *)malloc(sizeof(message));
                                if (newMsg == NULL) { exit(2); }
                                UDPtoTCP(newMsg, msg);
                                TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
                                free(newMsg);
                            }
                        }
                        free(protocolsOfUsers);
                        free(clientsAddressesInTheSameChannel);
                        free(usersInSameChannel);
                        UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                        pthread_mutex_unlock(&mutex); //! UNLOCK

                        // send BYE to user
                        createMessageUDP(msg, MSG_BYE, NULL, NULL, prgArgs->messageID++, 0);

                        // wait for confirmation message
                        retransmissions = 0;
                        do
                        {
                            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                            char str[INET_ADDRSTRLEN];
                            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                            {
                                printf("SENT %s:%d | BYE\n", str,  ntohs(connectionItems->serverAddress.sin_port));
                            }
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
                            tryToExitUDP(connectionItems, &newUser, true);
                            return NULL;
                        }
                        
                        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                        
                        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, &newUser, connectionItems->serverAddress, &refMessageID, false);
                        if (confirmExpected >= 0)
                        {
                            //! expected confirm message
                            tryToExitUDP(connectionItems, &newUser, true);
                        }
                        
                        // remove user from activeUsers
                        free(msg);
                        pthread_mutex_lock(&mutex); //! LOCK
                        removeUser(newId, activeUsers, *numOfUsers);
                        (*numOfUsers)--;
                        pthread_mutex_unlock(&mutex); //! UNLOCK
                        break;
                    }
                    case RETURN_BYE:
                    {
                        char str[INET_ADDRSTRLEN];
                        char *newId = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
                        if (newId == NULL) { exit(2); }
                        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                        {
                            sprintf(newId, "%s:%d", str,  ntohs(connectionItems->serverAddress.sin_port));
                        }
                        user *existingUser = findUser(newId, activeUsers, *numOfUsers);
                        free(newId);

                        if (existingUser == NULL)
                        {
                            //! user doesn't exist
                            tryToExitUDP(connectionItems, &newUser, false);
                        }

                        // create message to broadcast it
                        char *messageContent = (char *)malloc(sizeof(char) * (strlen(newUser.displayName) + strlen(" has left ") + strlen(newUser.channel) + strlen(".") + 1));
                        if (messageContent == NULL) { exit(2); }
                        strcpy(messageContent, newUser.displayName);
                        strcat(messageContent, " has left ");
                        strcat(messageContent, newUser.channel);
                        strcat(messageContent, ".");
                        createMessageUDP(msg, MSG_MSG, "Server", messageContent, prgArgs->messageID++, 0);
                        free(messageContent);

                        // broadcast message about leaving channel
                        pthread_mutex_lock(&mutex); //! LOCK
                        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (usersInSameChannel == NULL) { exit(2); }
                        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (protocolsOfUsers == NULL) { exit(2); }
                        int numberOfReceivers = findUsersInSameChannel(newUser.id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                        for (int i = 0; i < numberOfReceivers; i++)
                        {
                            if (protocolsOfUsers[i] == UDP)
                            {
                                UDPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                            }
                            else // TCP
                            {
                                message *newMsg = (message *)malloc(sizeof(message));
                                if (newMsg == NULL) { exit(2); }
                                UDPtoTCP(newMsg, msg);
                                TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
                                free(newMsg);
                            }
                        }
                        free(protocolsOfUsers);
                        free(clientsAddressesInTheSameChannel);
                        free(usersInSameChannel);
                        UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                        pthread_mutex_unlock(&mutex); //! UNLOCK

                        free(msg);
                        pthread_mutex_lock(&mutex); //! LOCK
                        removeUser(newUser.id, activeUsers, *numOfUsers);
                        (*numOfUsers)--;
                        pthread_mutex_unlock(&mutex); //! UNLOCK
                        break;
                    }
                    case RETURN_UNKNOWN:
                    {
                        char str[INET_ADDRSTRLEN];
                        char *newId = (char *)malloc(sizeof(char) * (INET_ADDRSTRLEN + 1 + 5 + 1)); // address:port\0
                        if (newId == NULL) { exit(2); }
                        if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                        {
                            sprintf(newId, "%s:%d", str,  ntohs(connectionItems->serverAddress.sin_port));
                        }
                        user *existingUser = findUser(newId, activeUsers, *numOfUsers);
                        free(newId);

                        if (existingUser == NULL)
                        {
                            //! user doesn't exist
                            tryToExitUDP(connectionItems, &newUser, false);
                        }
                        // user already exists
                        createMessageUDP(msg, MSG_ERR, "Server", "Invalid message arrived!", prgArgs->messageID++, 0);
                        
                        // wait for confirmation message
                        retransmissions = 0;
                        do
                        {
                            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                            char str[INET_ADDRSTRLEN];
                            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                            {
                                printf("SENT %s:%d | ERR\n", str,  ntohs(connectionItems->serverAddress.sin_port));
                            }
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
                            tryToExitUDP(connectionItems, &newUser, true);
                        }
                        
                        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                        
                        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, &newUser, connectionItems->serverAddress, &refMessageID, false);
                        if (confirmExpected >= 0)
                        {
                            //! expected confirm message
                            tryToExitUDP(connectionItems, &newUser, true);
                        }

                        // create message to broadcast it
                        char *messageContent = (char *)malloc(sizeof(char) * (strlen(newUser.displayName) + strlen(" has left ") + strlen(newUser.channel) + strlen(".") + 1));
                        if (messageContent == NULL) { exit(2); }
                        strcpy(messageContent, newUser.displayName);
                        strcat(messageContent, " has left ");
                        strcat(messageContent, newUser.channel);
                        strcat(messageContent, ".");
                        createMessageUDP(msg, MSG_MSG, "Server", messageContent, prgArgs->messageID++, 0);
                        free(messageContent);

                        // broadcast message about leaving channel
                        pthread_mutex_lock(&mutex); //! LOCK
                        int *usersInSameChannel = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (usersInSameChannel == NULL) { exit(2); }
                        struct sockaddr_in *clientsAddressesInTheSameChannel = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * (*numOfUsers));
                        if (clientsAddressesInTheSameChannel == NULL) { exit(2); }
                        int *protocolsOfUsers = (int *)malloc(sizeof(int) * (*numOfUsers));
                        if (protocolsOfUsers == NULL) { exit(2); }
                        int numberOfReceivers = findUsersInSameChannel(newUser.id, usersInSameChannel, clientsAddressesInTheSameChannel, protocolsOfUsers, activeUsers, *numOfUsers);
                        for (int i = 0; i < numberOfReceivers; i++)
                        {
                            if (protocolsOfUsers[i] == UDP)
                            {
                                UDPSend(usersInSameChannel[i], msg->messageContent, msg->contentSize, (struct sockaddr *) &clientsAddressesInTheSameChannel[i], sizeof(clientsAddressesInTheSameChannel[i]));
                            }
                            else // TCP
                            {
                                message *newMsg = (message *)malloc(sizeof(message));
                                if (newMsg == NULL) { exit(2); }
                                UDPtoTCP(newMsg, msg);
                                TCPSend(usersInSameChannel[i], newMsg->messageContent, newMsg->contentSize);
                                free(newMsg);
                            }
                        }
                        free(protocolsOfUsers);
                        free(clientsAddressesInTheSameChannel);
                        free(usersInSameChannel);
                        UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                        pthread_mutex_unlock(&mutex); //! UNLOCK
                        
                        createMessageUDP(msg, MSG_BYE, NULL, NULL, prgArgs->messageID++, 0);
                        // wait for confirmation message
                        retransmissions = 0;
                        do
                        {
                            UDPSend(connectionItems->socket, msg->messageContent, msg->contentSize, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                            char str[INET_ADDRSTRLEN];
                            if(inet_ntop(AF_INET, &connectionItems->serverAddress.sin_addr, str, sizeof(str)))
                            {
                                printf("SENT %s:%d | BYE\n", str,  ntohs(connectionItems->serverAddress.sin_port));
                            }
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
                            tryToExitUDP(connectionItems, &newUser, true);
                        }
                        
                        UDPRecv(connectionItems->socket, connectionItems->buffer, (struct sockaddr *) &connectionItems->serverAddress, sizeof(connectionItems->serverAddress));
                        
                        confirmExpected = messageFromClientUDP(connectionItems->buffer, msg, &newUser, connectionItems->serverAddress, &refMessageID, false);
                        if (confirmExpected >= 0)
                        {
                            //! expected confirm message
                            tryToExitUDP(connectionItems, &newUser, true);
                        }
                        
                        // remove user from activeUsers
                        free(msg);
                        pthread_mutex_lock(&mutex); //! LOCK
                        removeUser(newId, activeUsers, *numOfUsers);
                        (*numOfUsers)--;
                        pthread_mutex_unlock(&mutex); //! UNLOCK
                        break;
                    }
                    default:
                        // RETURN_CONFIRM
                        break;
                }
            }
        }
    }
    free(connectionItems);
    return NULL;
}

/** Main function of IPK server */
int main(int argc, char *argv[])
{
    signal(SIGINT, controlCorD);
    signal(SIGQUIT, controlCorD);
    
    // mutex initialization
    pthread_mutex_init(&mutex, NULL);

    // users array
    int shmid = shmget(SHM_KEY_ONE, sizeof(user) * MAX_USERS, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        exit(1); //! shmget error
    }
    activeUsers = (user *)shmat(shmid, NULL, 0);
    if (activeUsers == (user *)(-1))
    {
        exit(1); //! shmat error
    }

    // users position
    int shmid2 = shmget(SHM_KEY_TWO, sizeof(int), IPC_CREAT | 0666);
    if (shmid2 == -1)
    {
        exit(1); //! shmget error
    }
    numOfUsers = (int *)shmat(shmid2, NULL, 0);
    if (numOfUsers == (int *)(-1))
    {
        exit(1); //! shmat error
    }
    *numOfUsers = 0;

    prgArgs = (programArguments *)malloc(sizeof(programArguments));
    if (prgArgs == NULL) { exit(2); }
    initDefaultProgramArguments();
    UDPSet = 0;

    // Argument parsing
    for (int i = 1; i < argc; i += 2)
    {
        if (!strcmp(argv[i], "-l"))
        {
            prgArgs->listeningIP = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-p"))
        {
            prgArgs->port = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-d"))
        {
            prgArgs->timeout = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-r"))
        {
            prgArgs->retransmissions = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-h"))
        {
            printHelp();
            return 0;
        }
    }

    transmissionItems *threadArg = (transmissionItems *)malloc(sizeof(transmissionItems));
    if (threadArg == NULL) { exit(2); }
    transmissionItems *UDPconnItems = (transmissionItems *)malloc(sizeof(transmissionItems));
    if (UDPconnItems == NULL) { exit(2); }
    TCPsetup(prgArgs, threadArg);
    UDPsetup(prgArgs, UDPconnItems);

    fd_set readSockets;
    FD_ZERO(&readSockets);
    while (true)
    {
        FD_SET(prgArgs->welcomeSocket, &readSockets);
        FD_SET(UDPconnItems->socket, &readSockets);

        int activityOnSocket = select(FD_SETSIZE, &readSockets, NULL, NULL, NULL);
        if (activityOnSocket == -1)
        {
            exit(3); //! select failed
        }
        else if (activityOnSocket > 0)
        {
            if (FD_ISSET(prgArgs->welcomeSocket, &readSockets)) // TCP
            {
                threadArg->socket = accept(prgArgs->welcomeSocket, threadArg->address, &threadArg->addressSize);
                if (threadArg->socket < 0)
                {
                    //! accept failed
                    continue;
                }

                pthread_t tid;
                if (pthread_create(&tid, NULL, clientHandlerTCP, threadArg) != 0)
                {
                    exit(1); //! pthread_create failed
                }
                pthread_detach(tid);
            }
            else if (FD_ISSET(UDPconnItems->socket, &readSockets) && !UDPSet) // UDP
            {
                UDPSet = 1;

                pthread_t tid;
                if (pthread_create(&tid, NULL, clientHandlerUDP, UDPconnItems) != 0)
                {
                    exit(1); //! pthread_create failed
                }
                pthread_detach(tid);
            }
        }
    }
    free(prgArgs);
    free(threadArg);
    free(UDPconnItems);
    shmdt(activeUsers);
    shmdt(numOfUsers);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
