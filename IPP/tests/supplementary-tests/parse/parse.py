# IPP project
#
# file: parse.py
# author: Michal Blažek (xblaze38)

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
    match currentTokenFirst[0]:
        case 'MOVE':
            instr, order = instructionGen(order, 'MOVE')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'CREATEFRAME':
            instr, order = instructionGen(order, 'CREATEFRAME')
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'PUSHFRAME':
            instr, order = instructionGen(order, 'PUSHFRAME')
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'POPFRAME':
            instr, order = instructionGen(order, 'POPFRAME')
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'DEFVAR':
            instr, order = instructionGen(order, 'DEFVAR')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'CALL':
            instr, order = instructionGen(order, 'CALL')
            label(currentToken)
            instrArgGen(instr, 'arg1', 'label', currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'RETURN':
            instr, order = instructionGen(order, 'RETURN')
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'PUSHS':
            instr, order = instructionGen(order, 'PUSHS')
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg1', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'POPS':
            instr, order = instructionGen(order, 'POPS')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'ADD':
            instr, order = instructionGen(order, 'ADD')
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
        case 'SUB':
            instr, order = instructionGen(order, 'SUB')
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
        case 'MUL':
            instr, order = instructionGen(order, 'MUL')
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
        case 'IDIV':
            instr, order = instructionGen(order, 'IDIV')
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
        case 'LT':
            instr, order = instructionGen(order, 'LT')
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
        case 'GT':
            instr, order = instructionGen(order, 'GT')
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
        case 'EQ':
            instr, order = instructionGen(order, 'EQ')
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
        case 'AND':
            instr, order = instructionGen(order, 'AND')
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
        case 'OR':
            instr, order = instructionGen(order, 'OR')
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
        case 'NOT':
            instr, order = instructionGen(order, 'NOT')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'INT2CHAR':
            instr, order = instructionGen(order, 'INT2CHAR')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'STRI2INT':
            instr, order = instructionGen(order, 'STRI2INT')
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
            instr, order = instructionGen(order, 'READ')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            data_type(currentToken)
            instrArgGen(instr, 'arg2', 'type', currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'WRITE':
            instr, order = instructionGen(order, 'WRITE')
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg1', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'CONCAT':
            instr, order = instructionGen(order, 'CONCAT')
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
        case 'STRLEN':
            instr, order = instructionGen(order, 'STRLEN')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'GETCHAR':
            instr, order = instructionGen(order, 'GETCHAR')
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
        case 'SETCHAR':
            instr, order = instructionGen(order, 'SETCHAR')
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
        case 'TYPE':
            instr, order = instructionGen(order, 'TYPE')
            var(currentToken)
            instrArgGen(instr, 'arg1', 'var', currentToken[1] + '@' + currentToken[0])
            currentToken = tokens.pop(0)
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg2', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'LABEL':
            instr, order = instructionGen(order, 'LABEL')
            label(currentToken)
            instrArgGen(instr, 'arg1', 'label', currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'JUMP':
            instr, order = instructionGen(order, 'JUMP')
            label(currentToken)
            instrArgGen(instr, 'arg1', 'label', currentToken[0])
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'JUMPIFEQ':
            instr, order = instructionGen(order, 'JUMPIFEQ')
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
        case 'JUMPIFNEQ':
            instr, order = instructionGen(order, 'JUMPIFNEQ')
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
        case 'EXIT':
            instr, order = instructionGen(order, 'EXIT')
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg1', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'DPRINT':
            instr, order = instructionGen(order, 'DPRINT')
            varOrConst, argText = symb(currentToken)
            instrArgGen(instr, 'arg1', varOrConst, argText)
            currentToken = tokens.pop(0)
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case 'BREAK':
            instr, order = instructionGen(order, 'BREAK')
            program.appendChild(instr)
            order += 1
            return currentToken, order
        case _:
            # instr, order = instructionGen(order, 'MOVE')
            # program.appendChild(instr)
            # order += 1
            return currentToken, order #??? what is the purpose of this

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
    if currentToken[1] != 'LABEL':
        sys.exit(23) #! missing label

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
