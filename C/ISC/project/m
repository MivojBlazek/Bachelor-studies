################################################################################
# @file     Makefile
# @author   Karel Ondrej, BUT
################################################################################

################################################################################
# MACROS
################################################################################

# slozka s *.c soubory
SOURCE.DIR=src
# slozka s *.h soubory
INCLUDE.DIR=include
# slozka s vygenerovanymi *.o a *.d soubory
BUILD.DIR=build

# seznam vsech *.c souboru
SOURCES=$(wildcard $(SOURCE.DIR)/*.c)
# seznam vsech *.h souboru - not used
HEADERS=$(wildcard $(INCLUDE.DIR)/*.h)
# seznam vsech *.o souboru, ktere se maji vygenerovat
OBJECTS=$(SOURCES:$(SOURCE.DIR)/%.c=$(BUILD.DIR)/%.o)
# seznam vsech *.d souboru, ktere se maji vygenerovat
DEPENDS=$(OBJECTS:%.o=%.d)				

# nazev tohoto osouboru
MAKEFILE=$(lastword $(MAKEFILE_LIST))
# nazev hlavniho programu, ktery se ma vygenerovat
EXECUTABLE=main
# nazev programu pro testy
TEST=test
# vsechny programy
TARGETS=$(EXECUTABLE) $(TEST)

# prekladac
CC=gcc
# argumenty prekladu
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic -g -I$(INCLUDE.DIR) -MD

################################################################################
# Commands
################################################################################

# vychozi cil
default: all

# preloz vsechny soubory
all: $(TARGETS)

# spusti hlavni program
run: run_$(EXECUTABLE)

# spusti soubor, napr. run_test spusti soubor ./test
# argumenty programu muzou by specifikovany pomoci -e "ARGS=--help"
# example: make run_main -e "ARGS=--help"
run_%: %
	./$< $(ARGS)

# smaz vsechny vygenerovane soubory
# $(RM) je implicitni makro => RM=rm -f
clean:
	$(RM) $(TARGETS) $(OBJECTS) $(DEPENDS)
	rmdir $(BUILD.DIR)

# nekontroluj existenci cilovych souboru u techto cilu
.PHONY: all clean default run run_%

################################################################################
# Automatic generated files/folders
################################################################################

# hlavny spustitelny soubor
# addprefix prida adresar $(BUILD.DIR)/ pred soubory main.o functions.o
$(EXECUTABLE): $(addprefix $(BUILD.DIR)/, main.o functions.o)
	$(CC) $(CFLAGS) -o $@ $^

# testy
$(TEST): $(addprefix $(BUILD.DIR)/, test.o functions.o)
	$(CC) $(CFLAGS) -o $@ $^

# vlozi zavislosti z *.d souboru
# diky '-' na zacatku nevadi, pokud nektery soubor chybi
-include $(DEPENDS)

# vytvory *.o soubory ve slozce $(BUILD.DIR)
# u cilu za znakem | se kontroluje zda cil existuje, nicmene ne zda se zmenil
# pouziva se to prevazne u adresaru, kde staci, ze existuje
$(BUILD.DIR)/%.o: $(SOURCE.DIR)/%.c | $(BUILD.DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# vytvor adresar pro generovane soubory
$(BUILD.DIR):
	mkdir -p $@

################################################################################
# END
################################################################################
