/**
 * @file users.h
 * 
 * IPK project
 * 
 * @author <xblaze38> Michal Blažek
*/

#ifndef _USERS_H_
#define _USERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include "ipk24chat-server.h"

#define MAX_USERS 42

/** Structure containing user data */
typedef struct userStruct
{
    char *username;
    char *displayName;
    char *secret;
    char *channel;
    int socket;
    struct sockaddr_in clientAddress;
    transportProtocolType protocol;
    char *id;
} user;

/** Array of allowed users (who can log in) */
extern user allowedUsers[];

/** Function checks if user is authenticated successfully */
bool authenticationSuccessful(const user *newUser);

/** Function to add user to the list */
void addUserToActive(const user *newUser, user *activeUsers, int numOfUsers);

/** Function to remove user from the list */
void removeUser(char *id, user *activeUsers, int numOfUsers);

/** Function finds user from activeUsers according to id and returns it */
user *findUser(char *id, user *activeUsers, int numOfUsers);

/** Function finds other users in the same channel as the user, returns size of new array */
int findUsersInSameChannel(char *id, int *socketsOfUsersInSameChannel, struct sockaddr_in *clientsAddressesInTheSameChannel, int *protocolsOfUsers, user *activeUsers, int numOfUsers);

#endif //_USERS_H_
