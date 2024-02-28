# IPP project
#
# file: parse.py
# author: Michal Blažek (xblaze38)

import sys

# argument parsing
import argparse

try:
    argParser = argparse.ArgumentParser(description='This program is a compiler from the IPPcode24 language to the XML language')
    args = argParser.parse_args()
    if vars(args):
        print(args.accumulate(args.integers))
    else:
        pass
except SystemExit as err:
    if '-h' in sys.argv or '--help' in sys.argv:
        sys.exit(0)
    sys.exit(10)


#### lexical analyzer
        
import re

# splits file content into words
inputFile = sys.stdin.read()
inputFile = re.sub(r'#', ' # ', inputFile)  # makes spaces around '#'
words = re.findall(r'\S+|\n', inputFile)    # splits into words without whitespaces and with '\n'

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

tokenTypes = {
    '.IPPCODE24':   'PROGRAM',
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
            for character in item:
                if not (character.isalnum() or character in '_-$&%*!?'):
                    sys.exit(23) #! invalid character in variable name
            if item[0].isdigit():
                sys.exit(23) #! variable name starts with digit 
            tokenType = 'GF'
        elif item.startswith('LF@'):
            item = item[len('LF@'):]
            for character in item:
                if not (character.isalnum() or character in '_-$&%*!?'):
                    sys.exit(23) #! invalid character in variable name
            if item[0].isdigit():
                sys.exit(23) #! variable name starts with digit 
            tokenType = 'LF'
        elif item.startswith('TF@'):
            item = item[len('TF@'):]
            for character in item:
                if not (character.isalnum() or character in '_-$&%*!?'):
                    sys.exit(23) #! invalid character in variable name
            if item[0].isdigit():
                sys.exit(23) #! variable name starts with digit 
            tokenType = 'TF'
        elif item.startswith('bool@'):
            item = item[len('bool@'):]
            if item not in ('false', 'true'):
                sys.exit(23) #! false or true only
            tokenType = 'BOOL'
        elif item.startswith('string@'):
            item = item[len('string@'):]
            wrongEscapeSeq = re.finditer(r'\\(\D|\d{1}\D|\d{2}\D|$|\d{1}$|\d{2}$)', item)
            for match in wrongEscapeSeq:
                sys.exit(23) #! invalid escape sequence in string
            tokenType = 'STRING'
        elif item.startswith('int@'):
            item = item[len('int@'):]
            if len(item) == 0:
                sys.exit(23) #! empty int@
            if not (item.isdigit() or ((item[0] == '-' or item[0] == '+') and item[1:].isdigit())):
                if (item[0] == '0' and item[1] == 'x' and all(hexChars.isdigit() or hexChars.lower() in 'abcdef' for hexChars in item[2:])) or ((item[0] == '-' or item[0] == '+') and item[1] == '0' and item[2] == 'x' and all(hexChars.isdigit() or hexChars.lower() in 'abcdef' for hexChars in item[3:])): # hex numbers
                    pass
                elif (item[0] == '0' and item[1] == 'o' and all(hexChars.isdigit() and hexChars not in '89' for hexChars in item[2:])) or ((item[0] == '-' or item[0] == '+') and item[1] == '0' and item[2] == 'o' and all(hexChars.isdigit() and hexChars not in '89' for hexChars in item[3:])): # oct numbers
                    pass
                else:
                    sys.exit(23) #! integers only
            tokenType = 'INT'
        elif item.startswith('nil@'):
            item = item[len('nil@'):]
            if item != 'nil':
                sys.exit(23) #! nil only
            tokenType = 'NIL'

    # still could be command with lowercase
    itemCaseSensitive = item
    if tokenType == '-':
        tokenType = tokenTypes.get(item.upper(), 'LABEL')
        if tokenType != 'LABEL':
            item = itemCaseSensitive

    pairs.append((item, tokenType))

#### end of lexical analyzer


#### code generator

import xml.dom.minidom as mnD

xmlFile = mnD.Document()

# header generator
def headerGen():
    header = xmlFile.createProcessingInstruction('xml', 'version="1.0" encoding="UTF-8"')
    xmlFile.appendChild(header)

# program generator
def programGen():
    program = xmlFile.createElement('program')
    program.setAttribute('language', 'IPPcode24')
    xmlFile.appendChild(program)
    return program

# instruction generator
def instructionGen(order, opcode):
    instruction = xmlFile.createElement('instruction')
    instruction.setAttribute('order', str(order))
    instruction.setAttribute('opcode', opcode)
    return instruction, order

# instruction argument generator
def instrArgGen(instruction, argNum, type, argText):
    arg = xmlFile.createElement(argNum)
    arg.setAttribute('type', type)
    arg.appendChild(xmlFile.createTextNode(argText))
    instruction.appendChild(arg)

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
    print('<?xml version="1.0" encoding="UTF-8"?>')
    print('<program language="IPPcode24">')
    print('</program>')
    sys.exit(0)    #! returns 0 as there is only header
if currentToken[1] != 'NEW_LINE':
    sys.exit(21)    #! missing new line after .IPPcode24

# switching to tokens array
tokens.clear()
for token, tokenType in pairs:
    tokens.append((token, tokenType))


# <prog> -> <command> <enter> <prog>
# <prog> -> \0
def prog(currentToken, order):
    if currentToken[1] == 'COMMAND':
        currentToken, order = command(currentToken, order)
        currentToken = enter(currentToken)
        prog(currentToken, order)
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
def command(currentTokenFirst, order):
    currentToken = tokens.pop(0)
    match currentTokenFirst[0].upper():
        case 'MOVE' | 'NOT' | 'INT2CHAR' | 'STRLEN' | 'TYPE':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'CREATEFRAME' | 'PUSHFRAME' | 'POPFRAME' | 'RETURN' | 'BREAK':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'DEFVAR' | 'POPS':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'CALL' | 'LABEL' | 'JUMP':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            label(currentToken)
            instrArgGen(instr, 'arg1', 'label', currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'PUSHS' | 'WRITE' | 'EXIT' | 'DPRINT':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg1', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'ADD' | 'SUB' | 'MUL' | 'IDIV' | 'LT' | 'GT' | 'EQ' | 'AND' | 'OR' | 'STRI2INT' | 'CONCAT' | 'GETCHAR' | 'SETCHAR':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg3', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'READ':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            data_type(currentToken)
            instrArgGen(instr, 'arg2', 'type', currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'JUMPIFEQ' | 'JUMPIFNEQ':
            instr, order = instructionGen(order, currentTokenFirst[0].upper())
            label(currentToken)
            instrArgGen(instr, 'arg1', 'label', currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg3', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case _:
            sys.exit(23) #! unknown opcode

# <var> -> ...
def var(currentToken):
    if currentToken[1] not in ('GF', 'LF', 'TF'):
        sys.exit(23) #! missing variable

# <symb> -> ...
def symb(currentToken):
    if currentToken[1] not in ('GF', 'LF', 'TF', 'BOOL', 'STRING', 'INT', 'NIL'):
        sys.exit(23) #! missing variable or constant
    if currentToken[1] in ('GF', 'LF', 'TF'):
        return 'var', currentToken[1] + '@' + currentToken[0]
    else:
        return currentToken[1].lower(), currentToken[0]

# <label> -> ...
def label(currentToken):
    if currentToken[1] not in ('LABEL', 'DATA_TYPE', 'COMMAND'):
        sys.exit(23) #! missing label
    for character in currentToken[0]:
        if not (character.isalnum() or character in '_-$&%*!?'):
            sys.exit(23) #! invalid character in label
    if currentToken[0][0].isdigit():
        sys.exit(23) #! label starts with digit 

# <data_type> -> ...
def data_type(currentToken):
    if currentToken[1] != 'DATA_TYPE':
        sys.exit(23) #! missing data type


# main

# header gen
headerGen()
program = programGen()
order = 1

currentToken = tokens.pop(0)
currentToken = another_enter(currentToken)
result = prog(currentToken, order)

xmlFile = xmlFile.toprettyxml()
xmlFileSplit = xmlFile.split('\n')
xmlFile = '\n'.join(xmlFileSplit[1:])

print(xmlFile, file=sys.stdout)
sys.exit(result)

#### end of syntax analyzer
