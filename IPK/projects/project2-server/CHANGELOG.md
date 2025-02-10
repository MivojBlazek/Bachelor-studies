# Changelog

All notable changes to this project will be documented in this file.

## 16.03.2024

* Added README.md
* Argument parsing
* Added TCP setup function
* Added UDP setup function
* Added Makefile

## 17.03.2024

* Added TCP message processor

## 18.03.2024

* Added credentials for successful user authentication
* Added message creator for TCP
* Added TCP states and their behavior
* Fixed issues with creating TCP message
* Added user list for active users

## 27.03.2024

* Added LICENSE
* Added active channels array
* Added operations with users in channels
* Added fork to be able process more clients at the time

## 28.03.2024

* Added pipe to send message from client to other clients

## 02.04.2024

* Added shared memory for clients to send messages to others

## 03.04.2024

* Fixed second client can communicate to first but first can only to server (using processes)

## 04.04.2024

* Fixed both clients can send and receive messages to and from each other (using threads)

## 05.04.2024

* Added TCP correct communication between clients
* Fixed issues with connecting a new client after leaving another client
* Removed channels because channel is saved to every client
* Added feature to send BYE message to all clients with C-c or C-d
* Fixed checking if data from clients has correct characters in specific parts
* Fixed cases when there should be locked shared memory and not unlocked

## 08.04.2024

* Prepared to implement UDP functionality
* Separated TCP from UDP
* UDP AUTH message works with issue with sending MSG
* TCP should forward and broadcast messages to UDP and backwards
* Fixed functions in users.c to operate with id and not socket (UDP has 1 socket for all clients)

## 09.04.2024

* Fixed issue with creating MSG message in UDP
* Fixed translation from TCP to UDP and backwards
* Fixed clients id saving
* Added functionality to all UDP messages that can client send
* Added sending REPLY after JOIN

## 10.04.2024

* Added documentation
* Added memory deallocation
* Added if after every malloc
* Fixed creating message with sprintf() warnings during compiling