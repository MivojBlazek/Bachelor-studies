#
# @file Makefile
#
#	IFJ Projekt 2023
#
# @author <xblaze38> Michal Blažek
# @author <xmicha94> Kryštof Michálek
# @author <xsapik02> Matyáš Sapík
# @author <xzavod16> Ondřej Závodský
#

CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror
CFLAGSDEBUG= -std=c99 -pedantic -Wall -Wextra -g

all: main

main: main.c parser.c scanner.c stack.c symtable.c code_generator.c expression.c
	$(CC) $(CFLAGS) main.c parser.c scanner.c stack.c symtable.c code_generator.c expression.c -o ifj2023

scanner: clean main.c scanner.c
	$(CC) $(CFLAGSDEBUG) main.c scanner.c -o scanner

parser: clean main.c parser.c
	$(CC) $(CFLAGSDEBUG) main.c parser.c expression.c stack.c code_generator.c -o parser

symtable: clean main.c parser.c scanner.c stack.c symtable.c code_generator.c expression.c
	$(CC) $(CFLAGSDEBUG) main.c parser.c scanner.c stack.c symtable.c code_generator.c expression.c -o symtable

stack: clean main.c stack.c
	$(CC) $(CFLAGSDEBUG) main.c stack.c -o stack

clean:
	rm -f *.o scanner parser symtable stack ifj2023