/**
 * @file code_generator.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
 * @author <xsapik02> Matyáš Sapík
 * @author <xzavod16> Ondřej Závodský
*/

#include "code_generator.h"
#include <stdio.h>

// TODO cely code_generator je nefunkcni (pravdepodobne to stejne bude implementace parseru)

/** Funkce vytvori prazdnou instrukci */
void instructionEmpty(Instruction *instr)
{
    instr->name = NO_INSTRUCTION;

    for (int i = 0; i < 3; i++)
    {
        instr->op[i].type = NO_TYPE;
        instr->op[i].secondType = NO_SECOND_TYPE;
        instr->op[i].attribute.d = 0;
        instr->op[i].attribute.i = 0;
        instr->op[i].attribute.s = "";
    }
}

/** Funkce ziska z pravidla potrebne jmeno instrukce */
InstructionName getInstrName(precedenceTableRulesStruct ruleStruct)
{
    //TODO
    switch (ruleStruct.rule)
    {
        case E_PLUS_E:
            return ADD;

        case OPERAND:
            return MOVE;

        case NO_RULE:
        default:
            return NO_INSTRUCTION;
    }
    return NO_INSTRUCTION;
}

/** Funkce urci pocet operandu potrebnych pro danou instrukci */
int getNumberOfOperands(InstructionName instrName)
{
    //TODO
    switch (instrName)
    {
        case MOVE:
            return 2;

        case ADD:
            return 3;
        
        default:
            return 0;
    }
    return 0;
}

/** Nazvy instrukci */
char *instrNameFromCode[57] = // sizeof(InstructionName) jakoby, ale nejde to takhle napsat
{
    "MOVE",
    "CREATEFRAME",
    "PUSHFRAME",
    "POPFRAME",
    "DEFVAR",
    "CALL",
    "RETURN",

    "PUSHS",
    "POPS",
    "CLEARS",

    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "IDIV",
    "ADDS",
    "SUBS",
    "MULS",
    "DIVS",
    "IDIVS",

    "LT",
    "GT",
    "EQ",
    "LTS",
    "GTS",
    "EQS",

    "AND",
    "OR",
    "NOT",
    "ANDS",
    "ORS",
    "NOTS",

    "INT2FLOAT",
    "FLOAT2INT",
    "INT2CHAR",
    "STRI2INT",
    "INT2FLOATS",
    "FLOAT2INTS",
    "INT2CHARS",
    "STRI2INTS",

    "READ",
    "WRITE",

    "CONCAT",
    "STRLEN",
    "GETCHAR",
    "SETCHAR",

    "TYPE",

    "LABEL",
    "JUMP",
    "JUMPIFEQ",
    "JUMPIFNEQ",
    "JUMPIFEQS",
    "JUMPIFNEQS",
    "EXIT",

    "BREAK",
    "DPRINT",
    
    "NO_INSTRUCTION"
};

/** Funkce vrati jmeno instrukce misto jejiho kodu */
char *instructionCode2Name(InstructionName code)
{
    char *name = instrNameFromCode[code];
    return name;
}

/** Funkce vygeneruje urcitou instrukci */
int generateInstruction(Instruction instr)
{
    //TODO
    char *name = instructionCode2Name(instr.name);
    fprintf(stderr, "                               INSTRUCTION: %s %s %s %s\n", name, instr.op[0].attribute.s, instr.op[1].attribute.s, instr.op[2].attribute.s);
    return 0;
}
