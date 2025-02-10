# Changelog

All notable changes to this project will be documented in this file.

## 28.02.2024

* Added files that project need
* Argument parsing
* HELP command added

## 29.02.2024

* AUTH command added
* message structure added

## 04.03.2024

* JOIN command added
* normal message processing added
* RENAME command added
* UDP communication added
* authentication is required when joining channel or sending message

## 05.03.2024

* handle 1 server message

## 06.03.2024

* Makefile debug with -g created
* Sending AUTH, JOIN and normal messages to server
* Processing server CONFIRM and REPLY messages

## 07.03.2024

* Processing BYE and ERR messages from server
* Processing normal messages from server
* Added ERR, BYE and unknown message processing after JOIN and AUTH commands

## 08.03.2024

* Added timeout for confirmation messages
* Added timeout (5s) for replies
* In open transmission can read and send JOIN and MSG messages

## 09.03.2024

* Removed reply timeout, now just waiting for answer
* Fixed bugs where the program could unintendedly jump into wrong branches
* Numeric bytes are for example \x00 instead of \x30

## 11.03.2024

* Divided UDP setup from communication
* Fixed bugs with sending only a part of message
* Fixed CONFIRM message bugs
* UDP CONFIRM messages change little endian to big endian
* Added Ctrl+C and Ctrl+D behavior
* Added function to correctly end client after error

## 13.03.2024

* Added behavior for UDP after incorrect credentials during authentication
* Added TCP transport protocol support
* Fixed behavior in UDP after receiving ERR, BYE or unknown message

## 14.03.2024

* Fixed TCP client output
* Fixed TCP memory leaks in C-c situation
* Updated cases where program should exit to safely end communication instead
* Added unknown message behavior to UDP authentication

## 15.03.2024

* Changed gethostbyname() to getaddrinfo()
* Fixed UDP joining message where client expected reply but there was message before reply

## 19.03.2024

* Added documentation to README.md

## 26.03.2024

* Fixed some memory leaks
* Added LICENSE