/**
 * @file ipk24chat-client.c
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "udpCommunication.h"
#include "tcpCommunication.h"
#include "ipk24chat-client.h"

struct transmissionItemsStruct *globalConnectionItems;
programArguments *globalPrgArgs;

/** Function prints help */
void printHelp()
{
    printf("Usage: ./ipk24chat-client -t <tcp|udp> -s <server_address> [-p <server_port>] [-d <timeout>] [-r <retransmissions>] [-h]\n");
    printf("Options:\n");
    printf("  -t <tcp|udp>\t\tTransport protocol used for connection\n");
    printf("  -s <server_address>\tIP address or hostname of the server\n");
    printf("  -p <server_port>\tServer port (optional, default: 4567)\n");
    printf("  -d <timeout>\t\tUDP confirmation timeout (optional, default: 250)\n");
    printf("  -r <retransmissions>\tMaximum number of UDP retransmissions (optional, default: 3)\n");
    printf("  -h\t\t\tPrints program help output and exits\n");
}

/** Function prints help for supported commands */
void printHelpCommands()
{
    printf("--------------------------------------------------------------------------------------------\n");
    printf("Supported local commands:\n");
    printf("  /auth {Username} {Secret} {DisplayName}\tAuthenticates user with provided credentials\n");
    printf("  /join {ChannelID}\t\t\t\tJoins the specified channel\n");
    printf("  /rename {DisplayName}\t\t\t\tChanges the display name of the user\n");
    printf("  /help\t\t\t\t\t\tPrints out supported local commands\n");
    printf("--------------------------------------------------------------------------------------------\n");
}

/** Function finds out if the word consists of only alphanumeric characters */
bool isAlphaNumericWord(const char *word)
{
    for (unsigned int i = 0; i < strlen(word); i++)
    {
        if (!isalnum(word[i]) && word[i] != '-')
        {
            return false;
        }
    }
    return true;
}

/** Function finds out if the word has only printable characters */
bool isPrintableWord(const char *word)
{
    for (unsigned int i = 0; i < strlen(word); i++)
    {
        if (word[i] < 0x21 || word[i] > 0x7E)
        {
            return false;
        }
    }
    return true;
}

/** Function finds out if the string has only printable characters or spaces */
bool isPrintableWithSpaces(const char *string)
{
    for (unsigned int i = 0; i < strlen(string); i++)
    {
        if (string[i] < 0x20 || string[i] > 0x7E)
        {
            return false;
        }
    }
    return true;
}

/** Function change user displayed name */
void renameUser(char *inputLine, programArguments *prgArgs)
{
    // collect and check data from input
    char *newLine = strchr(inputLine, '\n');
    if (newLine != NULL)
    {
        *newLine = '\0';
    }

    if (inputLine != NULL && isPrintableWord(inputLine) && strlen(inputLine) <= 20)
    {
        strcpy(prgArgs->displayName, inputLine);
        prgArgs->displayName[sizeof(prgArgs->displayName) - 1] = '\0';
    }
    else
    {
        //! invalid characters or length
        fprintf(stderr, "ERR: Displayed name contains incompatible characters or it is too long!\n");
        return;
    }
}

/** Function to join user to channel */
void join(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems)
{
    // collect and check data from input
    char *newLine = strchr(inputLine, '\n');
    if (newLine != NULL)
    {
        *newLine = '\0';
    }

    char channelID[21];
    if (inputLine != NULL && isAlphaNumericWord(inputLine) && strlen(inputLine) <= 20)
    {
        strcpy(channelID, inputLine);
        channelID[sizeof(channelID) - 1] = '\0';
    }
    else
    {
        //! invalid characters or length
        fprintf(stderr, "ERR: Channel ID contains incompatible characters or it is too long!\n");
        return;
    }

    // create a message with provided data
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL)
    {
        exit(2);
    }

    if (prgArgs->transportProtocol == UDP)
    {
        // create message
        createMessageUDP(msg, MSG_JOIN, NULL, channelID, NULL, prgArgs->displayName, NULL, prgArgs->messageID++);

        // send the message
        UDPcommunication(prgArgs, msg, connectionItems);
    }
    else // TCP
    {
        // create message
        createMessageTCP(msg, MSG_JOIN, NULL, channelID, NULL, prgArgs->displayName, NULL);

        // send the message
        TCPcommunication(prgArgs, msg, connectionItems);
    }
    free(msg->messageContent);
    free(msg);
}

/** Function to authenticate user */
void authenticate(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems)
{
    // collect and check data from input
    char *word;

    char username[21];
    char secret[129];
    char displayName[21];
    word = strtok(inputLine, " ");
    if (word != NULL && isAlphaNumericWord(word) && strlen(word) <= 20)
    {
        strcpy(username, word);
        username[sizeof(username) - 1] = '\0';
    }
    else
    {
        //! invalid characters or length
        fprintf(stderr, "ERR: Username contains incompatible characters or it is too long!\n");
        return;
    }
    word = strtok(NULL, " ");
    if (word != NULL && isAlphaNumericWord(word) && strlen(word) <= 128)
    {
        strcpy(secret, word);
        secret[sizeof(secret) - 1] = '\0';
    }
    else
    {
        //! invalid characters or length
        fprintf(stderr, "ERR: Secret contains incompatible characters or it is too long!\n");
        return;
    }
    word = strtok(NULL, "\n");
    if (word != NULL && isPrintableWord(word) && strlen(word) <= 20)
    {
        strcpy(displayName, word);
        displayName[sizeof(displayName) - 1] = '\0';
        strcpy(prgArgs->displayName, displayName);
    }
    else
    {
        //! invalid characters or length
        fprintf(stderr, "ERR: Displayed name contains incompatible characters or it is too long!\n");
        return;
    }

    // create a message with provided data
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL)
    {
        exit(2);
    }

    if (prgArgs->transportProtocol == UDP)
    {
        // setup connection to server
        UDPsetup(prgArgs, connectionItems);

        // create message
        createMessageUDP(msg, MSG_AUTH, username, NULL, secret, prgArgs->displayName, NULL, prgArgs->messageID++);

        // send the message
        UDPcommunication(prgArgs, msg, connectionItems);
    }
    else // TCP
    {
        // setup connection to server
        TCPsetup(prgArgs, connectionItems);

        // create message
        createMessageTCP(msg, MSG_AUTH, username, NULL, secret, prgArgs->displayName, NULL);

        // send the message
        TCPcommunication(prgArgs, msg, connectionItems);
    }
    free(msg->messageContent);
    free(msg);
}

/** Function process line from stdin (command) */
void processCommandLine(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems)
{
    char *commands[COMMANDS_COUNT] = {"/auth ", "/join ", "/rename ", "/help"};
    for (int cmd = 0; cmd < COMMANDS_COUNT; cmd++)
    {
        if (!strncmp(inputLine, commands[cmd], strlen(commands[cmd])))
        {
            memmove(inputLine, inputLine + strlen(commands[cmd]), strlen(inputLine) - strlen(commands[cmd]) + 1);
            switch (cmd)
            {
                case AUTH:
                    if (prgArgs->auth)
                    {
                        fprintf(stderr, "ERR: Already authenticated!\n");
                        return;
                    }
                    authenticate(inputLine, prgArgs, connectionItems);
                    break;
                case JOIN:
                    if (prgArgs->auth)
                    {
                        join(inputLine, prgArgs, connectionItems);
                    }
                    else
                    {
                        fprintf(stderr, "ERR: User is not authenticated!\n");
                        return;
                    }
                    break;
                case RENAME:
                    renameUser(inputLine, prgArgs);
                    break;
                case HELP:
                    printHelpCommands();
                    break;
            }
        }
    }
}

/** Function sends normal message */
void sendMessage(char *inputLine, programArguments *prgArgs, struct transmissionItemsStruct *connectionItems)
{
    // collect and check data from input
    char *newLine = strchr(inputLine, '\n');
    if (newLine != NULL)
    {
        *newLine = '\0';
    }

    char messageContent[1401];
    if (inputLine != NULL && isPrintableWithSpaces(inputLine) && strlen(inputLine) <= 1400)
    {
        strcpy(messageContent, inputLine);
        messageContent[sizeof(messageContent) - 1] = '\0';
    }
    else
    {
        //! invalid characters or length
        fprintf(stderr, "ERR: Message contains incompatible characters or it is too long!\n");
        return;
    }

    // create a message with provided data
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL)
    {
        exit(2);
    }

    if (prgArgs->transportProtocol == UDP)
    {
        // create message
        createMessageUDP(msg, MSG_MSG, NULL, NULL, NULL, prgArgs->displayName, messageContent, prgArgs->messageID++);

        // send the message
        UDPcommunication(prgArgs, msg, connectionItems);
    }
    else // TCP
    {
        // create message
        createMessageTCP(msg, MSG_MSG, NULL, NULL, NULL, prgArgs->displayName, messageContent);

        // send the message
        TCPcommunication(prgArgs, msg, connectionItems);
    }
    free(msg->messageContent);
    free(msg);
}

/** Function initializes default program arguments */
void initDefaultProgramArguments(programArguments *prgArgs)
{
    prgArgs->port = 4567;
    prgArgs->timeout = 250;
    prgArgs->retransmissions = 3;
    prgArgs->messageID = 0;
    prgArgs->displayName = (char *)malloc(sizeof(char) * 21);
    if (prgArgs->displayName == NULL)
    {
        exit(2);
    }
    prgArgs->auth = false;
    prgArgs->originalMessageID = 0;
}

/** Function handles Ctrl+C and Ctrl+D */
void controlCorD()
{
    message *msg = (message *)malloc(sizeof(message));
    if (msg == NULL)
    {
        exit(2);
    }

    if (globalPrgArgs->transportProtocol == UDP)
    {
        createMessageUDP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL, globalPrgArgs->messageID++);

        char *messageToSend = (char *)malloc(sizeof(char) * 3);
        if (messageToSend == NULL)
        {
            exit(2);
        }

        unsigned int messageSize = 3;
        messageToSend[0] = (char)msg->type;
        messageToSend[1] = msg->messageID & 0xFF;
        messageToSend[2] = msg->messageID >> 8;

        int arrivingReply;
        int retransmissions = 0;
        do
        {
            UDPSend(globalConnectionItems->socketFD, messageToSend, messageSize, globalConnectionItems->address, globalConnectionItems->addressSize);
            arrivingReply = select(FD_SETSIZE, &globalConnectionItems->readSocks, NULL, NULL, &globalConnectionItems->timeout);
            if (arrivingReply == -1)
            {
                exit(3);
            }
            retransmissions++;
        } while (!arrivingReply && retransmissions <= globalPrgArgs->retransmissions);
        free(messageToSend);
    }
    else // TCP
    {
        createMessageTCP(msg, MSG_BYE, NULL, NULL, NULL, NULL, NULL);
        TCPSend(globalConnectionItems->socketFD, msg->messageContent, msg->contentSize);
    }
    free(msg->messageContent);
    free(msg);
    free(globalPrgArgs->displayName);
    close(globalConnectionItems->socketFD);
    exit(0);
}

/** Main function of IPK project */
int main(int argc, char *argv[])
{
    signal(SIGINT, controlCorD);
    signal(SIGQUIT, controlCorD);

    programArguments prgArgs;
    globalPrgArgs = &prgArgs;
    initDefaultProgramArguments(&prgArgs);

    // Argument parsing
    for (int i = 1; i < argc; i += 2)
    {
        if (!strcmp(argv[i], "-t"))
        {
            if (!strcmp(argv[i + 1], "udp"))
            {
                prgArgs.transportProtocol = UDP;
            }
            else if (!strcmp(argv[i + 1], "tcp"))
            {
                prgArgs.transportProtocol = TCP;
            }
            else
            {
                exit(1); //! invalid argument
            }
        }
        else if (!strcmp(argv[i], "-s"))
        {
            prgArgs.serverAddress = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-p"))
        {
            prgArgs.port = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-d"))
        {
            prgArgs.timeout = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-r"))
        {
            prgArgs.retransmissions = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-h"))
        {
            printHelp();
            return 0;
        }
    }

    // Processing stdin
    char stdinInput[MAX_LINE_LENGTH];
    
    struct transmissionItemsStruct connectionItems;
    globalConnectionItems = &connectionItems;

    fd_set readSocks;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while (true)
    {
        FD_ZERO(&readSocks);
        FD_SET(STDIN_FILENO, &readSocks);
        FD_SET(prgArgs.socketFD, &readSocks);
        if (!prgArgs.auth)
        {
            if (fgets(stdinInput, MAX_LINE_LENGTH, stdin) == NULL)
            {
                break;
            }
            if (stdinInput[0] == '/')
            {
                // command
                processCommandLine(stdinInput, &prgArgs, &connectionItems);
            }
            else
            {
                // message
                fprintf(stderr, "ERR: User is not authenticated!\n");
            }
        }
        else
        {
            // auth successful -> JOIN, MSG or receiving messages from server
            int activityOnSocket = select(FD_SETSIZE, &readSocks, NULL, NULL, &timeout);
            if (activityOnSocket == -1)
            {
                exit(3);
            }
            else if (activityOnSocket > 0)
            {
                if (FD_ISSET(prgArgs.socketFD, &readSocks))
                {
                    message *msg = (message *)malloc(sizeof(message));
                    if (msg == NULL)
                    {
                        exit(2);
                    }

                    msg->type = MESSAGE_ARRIVED_WITHOUT_ASKING;
                    if (prgArgs.transportProtocol == UDP)
                    {
                        UDPcommunication(&prgArgs, msg, &connectionItems);
                    }
                    else // TCP
                    {
                        TCPcommunication(&prgArgs, msg, &connectionItems);
                    }
                    free(msg->messageContent);
                    free(msg);
                }
                else if (FD_ISSET(STDIN_FILENO, &readSocks))
                {
                    if (fgets(stdinInput, MAX_LINE_LENGTH, stdin) == NULL)
                    {
                        if (feof(stdin))
                        {
                            controlCorD(SIGINT);
                            exit(0);
                        }
                        else
                        {
                            exit(1);
                        }
                    }
                    if (stdinInput[0] == '/')
                    {
                        // command
                        processCommandLine(stdinInput, &prgArgs, &connectionItems);
                    }
                    else
                    {
                        // message
                        sendMessage(stdinInput, &prgArgs, &connectionItems);
                    }
                }
            }
        }
    }
    return 0;
}
