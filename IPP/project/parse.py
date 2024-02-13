# hlavicka parse.py

#///TODO lexikalni analyza
#TODO syntakticka analyza
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
    pairs.append((item, tokenType))

# printing tokens with their types #! DEBUG
#for token, tokenType in pairs:
#    print(f"({repr(token)}, {repr(tokenType)})")
#print('\n') #! DEBUG

#### end of lexical analyzer

#### syntax analyzer

# checks header in IPPcode24 code
currentToken = pairs.pop(0)
while currentToken[1] == 'NEW_LINE':
    currentToken = pairs.pop(0)
if currentToken[1] != 'PROGRAM':
    sys.exit(21)    #! missing .IPPcode24 in code
currentToken = pairs.pop(0)
if currentToken[1] != 'NEW_LINE':
    sys.exit(21)    #! missing new line after .IPPcode24

#! DEBUG
#for token, tokenType in pairs:
#    print(f"({repr(token)}, {repr(tokenType)})")
#print('\n') #! DEBUG

# getting rid of new line characters
tokens.clear()
for token, tokenType in pairs:
    if tokenType != 'NEW_LINE':
        tokens.append((token, tokenType))

#! DEBUG
for token, tokenType in tokens:
    print(f"({repr(token)}, {repr(tokenType)})")
print('\n') #! DEBUG

# rest of syntax analysis

# <prog> -> <command> <prog>
# <prog> -> \0
def prog(currentToken):
    if currentToken[1] == 'COMMAND':
        currentToken = tokens.pop(0)
        ret = command(currentToken)
        if ret > 0:
            sys.exit(100)   #TODO RETURN
        currentToken = tokens.pop(0)
        return prog(currentToken)
    elif currentToken[1] == 'END_OF_FILE':
        return 0

# <command> -> commands
# <command> -> epsilon
def command(currentToken):
    match currentToken[0]:
        case 'MOVE':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'CREATEFRAME':
            currentToken = tokens.pop(0)
            return 0
        case 'PUSHFRAME':
            currentToken = tokens.pop(0)
            return 0
        case 'POPFRAME':
            currentToken = tokens.pop(0)
            return 0
        case 'DEFVAR':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'CALL':
            ret = label(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'RETURN':
            currentToken = tokens.pop(0)
            return 0
        case 'PUSHS':
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'POPS':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'ADD':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'SUB':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'MUL':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'IDIV':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'LT':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'GT':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'EQ':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'AND':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'OR':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'NOT':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'INT2CHAR':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'STRI2INT':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'READ':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = data_type(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'WRITE':
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'CONCAT':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'STRLEN':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'GETCHAR':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'SETCHAR':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'TYPE':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'LABEL':
            ret = label(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'JUMP':
            ret = label(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'JUMPIFEQ':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'JUMPIFNEQ':
            ret = var(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'EXIT':
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'DPRINT':
            ret = symb(currentToken)
            if ret > 0:
                sys.exit(100)   #TODO RETURN
            currentToken = tokens.pop(0)
            return 0
        case 'BREAK':
            currentToken = tokens.pop(0)
            return 0
        case _:
            return 0


# <var> -> ...
def var(currentToken):
    return 0 #! DEBUG
    # if #another token is:
       #TODO inside var

# <symb> -> ...
def symb(currentToken):
    return 0 #! DEBUG
    # if #another token is:
       #TODO inside symb

# <label> -> ...
def label(currentToken):
    return 0 #! DEBUG
    # if #another token is:
       #TODO inside label

# <data_type> -> ...
def data_type(currentToken):
    return 0 #! DEBUG
    # if #another token is:
       #TODO inside data_type


# main
currentToken = tokens.pop(0)
result = prog(currentToken)
print(result) #! DEBUG

#! DEBUG
for token, tokenType in tokens:
    print(f"({repr(token)}, {repr(tokenType)})")
print('\n') #! DEBUG

#### end of syntax analyzer
