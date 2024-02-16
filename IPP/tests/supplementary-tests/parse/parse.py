# hlavicka parse.py

#///TODO lexikalni analyza
#///TODO syntakticka analyza
#TODO generovani kodu v xml


import sys


# argument parsing
import argparse

try:
    argParser = argparse.ArgumentParser(description='Description')
    args = argParser.parse_args()
    if vars(args):
        print(args.accumulate(args.integers))
    else:
        pass
except SystemExit as err:
    if err.code > 0:
        sys.exit(10)


#### lexical analyzer
        
import re

# splits file content into words
inputFile = sys.stdin.read()
inputFile = re.sub(r'#', ' # ', inputFile)  # makes spaces around '#'
words = re.findall(r'\S+|\n', inputFile)    # splits into words without witespaces and with '\n'

tokens = []
comment = False

# deletes comments
for word in words:
    if word == '\n':
        tokens.append(word)
        comment = False
    elif word == '#' or comment:
        comment = True
    else:
        tokens.append(word)

tokens.append('\0')
#print(tokens) #!DEBUG

tokenTypes = {
    '.IPPcode24':   'PROGRAM',
    '\n':           'NEW_LINE',
    '\0':           'END_OF_FILE',

    'int':          'DATA_TYPE',
    'string':       'DATA_TYPE',
    'bool':         'DATA_TYPE',

    'MOVE':         'COMMAND',
    'CREATEFRAME':  'COMMAND',
    'PUSHFRAME':    'COMMAND',
    'POPFRAME':     'COMMAND',
    'DEFVAR':       'COMMAND',
    'CALL':         'COMMAND',
    'RETURN':       'COMMAND',

    'PUSHS':        'COMMAND',
    'POPS':         'COMMAND',

    'ADD':          'COMMAND',
    'SUB':          'COMMAND',
    'MUL':          'COMMAND',
    'IDIV':         'COMMAND',
    'LT':           'COMMAND',
    'GT':           'COMMAND',
    'EQ':           'COMMAND',
    'AND':          'COMMAND',
    'OR':           'COMMAND',
    'NOT':          'COMMAND',
    'INT2CHAR':     'COMMAND',
    'STRI2INT':     'COMMAND',

    'READ':         'COMMAND',
    'WRITE':        'COMMAND',

    'CONCAT':       'COMMAND',
    'STRLEN':       'COMMAND',
    'GETCHAR':      'COMMAND',
    'SETCHAR':      'COMMAND',

    'TYPE':         'COMMAND',

    'LABEL':        'COMMAND',
    'JUMP':         'COMMAND',
    'JUMPIFEQ':     'COMMAND',
    'JUMPIFNEQ':    'COMMAND',
    'EXIT':         'COMMAND',

    'DPRINT':       'COMMAND',
    'BREAK':        'COMMAND',
}

# recognize type of tokens
pairs = []
for item in tokens:
    tokenType = tokenTypes.get(item, '-')
    if tokenType == '-':
        if item.startswith('GF@'):
            item = item[len('GF@'):]
            tokenType = 'GF'
        elif item.startswith('LF@'):
            item = item[len('LF@'):]
            tokenType = 'LF'
        elif item.startswith('TF@'):
            item = item[len('TF@'):]
            tokenType = 'TF'
        elif item.startswith('bool@'):
            item = item[len('bool@'):]
            tokenType = 'BOOL'
        elif item.startswith('string@'):
            item = item[len('string@'):]
            tokenType = 'STRING'
        elif item.startswith('int@'):
            item = item[len('int@'):]
            tokenType = 'INT'
        elif item.startswith('nil@'):
            item = item[len('nil@'):]
            tokenType = 'NIL'

    # still could be command with lowercase
    if tokenType == '-':
        tmpItem = item.upper()
        tokenType = tokenTypes.get(tmpItem, 'LABEL')
        if tokenType != 'LABEL':
            item = tmpItem

    pairs.append((item, tokenType))


# printing tokens with their types #! DEBUG
#for token, tokenType in pairs:
#    print(f"({repr(token)}, {repr(tokenType)})")
#print('\n') #! DEBUG

#### end of lexical analyzer


#### code generator

import xml.etree.ElementTree as eTree

# header generator
def programGen():
    programArgs = {
        'language': 'IPPcode24',
        'hello2':   'world2'
    }
    header = eTree.Element("program", programArgs)
    return header

# instruction generator
def instructionGen(header):
    instrArgs = {
        'order': '1',
        'hello': 'world'
    }
    instruction = eTree.SubElement(header, "instruction", instrArgs)
    instruction.text = 'Text inside'

#### end of code generator


#### syntax analyzer

# checks header in IPPcode24 code
currentToken = pairs.pop(0)
while currentToken[1] == 'NEW_LINE':
    currentToken = pairs.pop(0)
if currentToken[1] != 'PROGRAM':
    sys.exit(21)    #! missing .IPPcode24 in code
currentToken = pairs.pop(0)
if currentToken[1] == 'END_OF_FILE':
    sys.exit(0)    #! returns 0 as there is only header
if currentToken[1] != 'NEW_LINE':
    sys.exit(21)    #! missing new line after .IPPcode24

# switching to tokens array
tokens.clear()
for token, tokenType in pairs:
    tokens.append((token, tokenType))

#! DEBUG
# for token, tokenType in tokens:
#     print(f"({repr(token)}, {repr(tokenType)})")
# print('\n') #! DEBUG


# <prog> -> <command> <enter> <prog>
# <prog> -> \0
def prog(currentToken):
    if currentToken[1] == 'COMMAND':
        currentToken = command(currentToken)
        currentToken = enter(currentToken)
        prog(currentToken)
        return 0
    elif currentToken[1] == 'END_OF_FILE':
        return 0
    elif currentToken[1] == 'PROGRAM':
        sys.exit(23) #! too many headers
    else:
        sys.exit(22) #! no command or EOF

# <enter> -> \n <another_enter>
def enter(currentToken):
    if currentToken[1] not in ('NEW_LINE', 'END_OF_FILE'):
        sys.exit(23) #! missing new line
    if currentToken[1] == 'END_OF_FILE':
        return currentToken # returns currentToken (\0) for prog
    currentToken = tokens.pop(0)
    currentToken = another_enter(currentToken)
    return currentToken

# <another_enter> -> <enter>
# <another_enter> -> epsilon
def another_enter(currentToken):
    if currentToken[1] == 'NEW_LINE':
        currentToken = enter(currentToken)
        return currentToken
    else:
        return currentToken

# <command> -> commands
# <command> -> epsilon
def command(currentTokenFirst):
    currentToken = tokens.pop(0)
    match currentTokenFirst[0]:
        case 'MOVE':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'CREATEFRAME':
            return currentToken
        case 'PUSHFRAME':
            return currentToken
        case 'POPFRAME':
            return currentToken
        case 'DEFVAR':
            currentToken = var(currentToken)
            return currentToken
        case 'CALL':
            currentToken = label(currentToken)
            return currentToken
        case 'RETURN':
            return currentToken
        case 'PUSHS':
            currentToken = symb(currentToken)
            return currentToken
        case 'POPS':
            currentToken = var(currentToken)
            return currentToken
        case 'ADD':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'SUB':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'MUL':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'IDIV':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'LT':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'GT':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'EQ':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'AND':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'OR':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'NOT':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'INT2CHAR':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'STRI2INT':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'READ':
            currentToken = var(currentToken)
            currentToken = data_type(currentToken)
            return currentToken
        case 'WRITE':
            currentToken = symb(currentToken)
            return currentToken
        case 'CONCAT':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'STRLEN':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'GETCHAR':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'SETCHAR':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'TYPE':
            currentToken = var(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'LABEL':
            currentToken = label(currentToken)
            return currentToken
        case 'JUMP':
            currentToken = label(currentToken)
            return currentToken
        case 'JUMPIFEQ':
            currentToken = label(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'JUMPIFNEQ':
            currentToken = label(currentToken)
            currentToken = symb(currentToken)
            currentToken = symb(currentToken)
            return currentToken
        case 'EXIT':
            currentToken = symb(currentToken)
            return currentToken
        case 'DPRINT':
            currentToken = symb(currentToken)
            return currentToken
        case 'BREAK':
            return currentToken
        case _:
            return currentToken #??? what is the purpose of this

# <var> -> ...
def var(currentToken):
    if currentToken[1] not in ('GF', 'LF', 'TF'):
        sys.exit(23) #! missing variable
    currentToken = tokens.pop(0)
    return currentToken

# <symb> -> ...
def symb(currentToken):
    if currentToken[1] not in ('GF', 'LF', 'TF', 'BOOL', 'STRING', 'INT'):
        sys.exit(23) #! missing variable or constant
    currentToken = tokens.pop(0)
    return currentToken

# <label> -> ...
def label(currentToken):
    if currentToken[1] != 'LABEL':
        sys.exit(23) #! missing label
    currentToken = tokens.pop(0)
    return currentToken

# <data_type> -> ...
def data_type(currentToken):
    if currentToken[1] != 'DATA_TYPE':
        sys.exit(23) #! missing data type
    currentToken = tokens.pop(0)
    return currentToken


# main

# header gen
header = programGen()
#TODO instructionGen(header)

currentToken = tokens.pop(0)
currentToken = another_enter(currentToken)

result = prog(currentToken)
print(result) #! DEBUG

#! DEBUG
for token, tokenType in tokens:
    print(f"({repr(token)}, {repr(tokenType)})")
print('\n') #! DEBUG

# code gen
xmlTree = eTree.ElementTree(header)
xmlTree.write('output.xml')

#### end of syntax analyzer
