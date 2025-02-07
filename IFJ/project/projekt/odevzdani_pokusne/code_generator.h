/**
 * @file code_generator.h
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
 * @author <xsapik02> Matyáš Sapík
 * @author <xzavod16> Ondřej Závodský
*/

#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "scanner.h"
#include "expression.h"

/** Kody instrukci */
typedef enum
{
    MOVE,
    CREATEFRAME,
    PUSHFRAME,
    POPFRAME,
    DEFVAR,
    CALL,
    RETURN,

    PUSHS,
    POPS,
    CLEARS,

    ADD,
    SUB,
    MUL,
    DIV,
    IDIV,
    ADDS,
    SUBS,
    MULS,
    DIVS,
    IDIVS,

    LT,
    GT,
    EQ,
    LTS,
    GTS,
    EQS,

    AND,
    OR,
    NOT,
    ANDS,
    ORS,
    NOTS,

    INT2FLOAT,
    FLOAT2INT,
    INT2CHAR,
    STRI2INT,
    INT2FLOATS,
    FLOAT2INTS,
    INT2CHARS,
    STRI2INTS,

    READ,
    WRITE,

    CONCAT,
    STRLEN,
    GETCHAR,
    SETCHAR,

    TYPE,

    LABEL,
    JUMP,
    JUMPIFEQ,
    JUMPIFNEQ,
    JUMPIFEQS,
    JUMPIFNEQS,
    EXIT,

    BREAK,
    DPRINT,
    
    NO_INSTRUCTION
}InstructionName;

/** Radek jedne instrukce */
typedef struct
{
    InstructionName name;
    Token op[3];
}Instruction;

/** Funkce vytvori prazdnou instrukci */
void instructionEmpty(Instruction *instr);

/** Funkce ziska z pravidla potrebne jmeno instrukce */
InstructionName getInstrName(precedenceTableRulesStruct ruleStruct);

/** Funkce urci pocet operandu potrebnych pro danou instrukci */
int getNumberOfOperands(InstructionName instrName);

/** Funkce vrati jmeno instrukce misto jejiho kodu */
char *instructionCode2Name(InstructionName code);

/** Funkce vygeneruje urcitou instrukci */
int generateInstruction(Instruction instr);

#endif // _CODE_GENERATOR_H_