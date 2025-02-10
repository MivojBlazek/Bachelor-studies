/**
 * @file users.c
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "users.h"


/** Array of allowed users (who can log in) */
user allowedUsers[] = {
    {
        .username = "admin",
        .secret = "admin"
    },
    {
        .username = "xblaze38",
        .secret = "secret"
    },
    {
        .username = "test",
        .secret = "TEST"
    }
};

/** Function checks if user is authenticated successfully */
bool authenticationSuccessful(const user *newUser)
{
    for (unsigned int i = 0; i < sizeof(allowedUsers) / sizeof(allowedUsers[0]); i++)
    {
        if (!strcmp(newUser->username, allowedUsers[i].username) && !strcmp(newUser->secret, allowedUsers[i].secret))
        {
            return true;
        }
    }
    return false;
}

/** Function to add user to the list */
void addUserToActive(const user *newUser, user *activeUsers, int numOfUsers)
{
    if (numOfUsers >= MAX_USERS)
    {
        exit(1); //! maximum users reached
    }
    activeUsers[numOfUsers] = *newUser;
}

/** Function to remove user from the list */
void removeUser(char *id, user *activeUsers, int numOfUsers)
{
    for (int i = 0; i < numOfUsers; i++)
    {
        if (!strcmp(activeUsers[i].id, id))
        {
            for (int j = i; j < MAX_USERS - 1; j++)
            {
                activeUsers[j] = activeUsers[j + 1];
            }
            return;
        }
    }
}

/** Function finds user from activeUsers according to id and returns it */
user *findUser(char *id, user *activeUsers, int numOfUsers)
{
    for (int i = 0; i < numOfUsers; i++)
    {
        if (!strcmp(activeUsers[i].id, id))
        {
            return &activeUsers[i];
        }
    }
    return NULL;
}

/** Function finds other users in the same channel as the user, returns size of new array */
int findUsersInSameChannel(char *id, int *socketsOfUsersInSameChannel, struct sockaddr_in *clientsAddressesInTheSameChannel, int *protocolsOfUsers, user *activeUsers, int numOfUsers)
{
    user *me = findUser(id, activeUsers, numOfUsers);
    if (me == NULL)
    {
        exit(1); //! my socket doesn't exist
    }
    
    int countOfSameChannel = 0;
    for (int i = 0; i < numOfUsers; i++)
    {
        if (strcmp(activeUsers[i].id, id) && !strcmp(activeUsers[i].channel, me->channel))
        {
            socketsOfUsersInSameChannel[countOfSameChannel] = activeUsers[i].socket;
            clientsAddressesInTheSameChannel[countOfSameChannel] = activeUsers[i].clientAddress;
            protocolsOfUsers[countOfSameChannel++] = activeUsers[i].protocol;
        }
    }
    return countOfSameChannel;
}
