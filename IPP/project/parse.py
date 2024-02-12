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

inputFile = sys.stdin.read()
inputFile = re.sub(r'#', ' # ', inputFile)  # makes spaces around '#'
words = re.findall(r'\S+|\n', inputFile)    # splits into words without witespaces and with '\n'

tokens = []
comment = False

for word in words:  # deletes comments
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

pairs = []
for item in tokens: # recognize type of tokens
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

# printing tokens with their types
for token, tokenType in pairs:
    print(f"({repr(token)}, {repr(tokenType)})")

#### end of lexical analyzer

#### syntax analyzer



#### end of syntax analyzer
