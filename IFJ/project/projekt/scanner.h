/**
 * @file scanner.h
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
*/

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdbool.h>

/** Typ tokenu */
typedef enum 
{
    NOT_NIL = 0,        // 0: !
    NIL_CONDITION,      // 1: ??
    RELATION_COMPARE,   // 2: ==,!=,<,>,<=,>=
    PLUS_MINUS,         // 3: +,-
    MULTIPLY_DIVIDE,    // 4: *,/
    LEFT_BRACKET,       // 5: (
    RIGHT_BRACKET,      // 6: )
    IDENTIFIER,         // 7: i

    DOLLAR,             // 8: $
    E_TOKEN,            // 9: E

    LF_TYPE,            // 10: LF
    RG_TYPE,            // 11: RG
    EQ_TYPE,            // 12: EQ
    UN_TYPE,            // 13: UN

    NO_TYPE,            // 14: no type

    INT,                // 15: 123
    DOUBLE,             // 16: 123.123
    DOUBLE_EXPONENT,    // 17: 123.123e-123
    STRING,             // 18: ""
    MORE_LINES_STRING,  // 19: """\n xxx \n"""

    UNDERLINE,          // 20: _

    KEYWORD,            // 21: Double, else, func, if, Int, let, nil, return, String, var, while
    KEYWORD_NIL,        // 22: Double?, Int?, String?
    LEFT_CURLY_B,       // 23: {
    RIGHT_CURLY_B,      // 24: }
    COMMA,              // 25: ,
    COLON,              // 26: :
    ARROW,              // 27: ->
    ASSIGN,             // 28: =

    LINE_COMMENT,       // 29: //   .//! scanner by nemel davat
    BLOCK_COMMENT,      // 30: /**/ .//! scanner by nemel davat
    EOL_TYPE,           // 31: \n
    EOF_TYPE            // 32: EOF
}tokenType;

/** Typ tokenu pro rozliseni PLUS, MINUS atd */
typedef enum
{
    PLUS = 0,               // 0: +
    MINUS,                  // 1: -
    MULTIPLY,               // 2: *
    DIVIDE,                 // 3: /
    EQUAL,                  // 4: ==
    NOT_EQUAL,              // 5: !=
    LESS,                   // 6: <
    MORE,                   // 7: >
    LESS_EQUAL,             // 8: <=
    MORE_EQUAL,             // 9: >=
    NO_SECOND_TYPE,         // 10: no type

    ID_INT,                 // 11: IDENTIFIER INT
    ID_DOUBLE,              // 12: IDENTIFIER DOUBLE
    ID_STRING               // 13: IDENTIFIER STRING
}tokenSecondType;

/** Pocet klicovych slov */
#define KEYWORD_COUNT 11

/** Atributy tokenu */
typedef union
{
    int i;
    double d;
    char *s;
}tokenAttrib;

/** Struktura tokenu */
typedef struct
{
    tokenType type;
    tokenSecondType secondType;
    tokenAttrib attribute;
}Token;

/** Stavy */
typedef enum
{
    START,                      // START STATE

    EOL_STATE,                  // OTHER STATES
    EOF_STATE,
    UNDERLINE_STATE,
    COMMA_STATE,
    COLON_STATE,
    ID_STATE,
    ID_QUESTIONMARK,
    LEFT_CURLY_B_STATE,
    RIGHT_CURLY_B_STATE,
    
    INT_STATE,                  // DATA_TYPES
    INT_DOT,
    DOUBLE_STATE,
    DOUBLE_E,
    DOUBLE_E_SIGN,
    DOUBLE_EXP_STATE,

    STRING_START,               // STRING
    STRING_BACKSLASH,
    STRING_ESCAPE_U,
    STRING_ESCAPE_U_BEGIN,
    STRING_STATE,
    STRING_ALMOST_MORE_LINE,
    STRING_MORE_LINE,
    STRING_THREE2END,
    STRING_TWO2END,
    STRING_ONE2END,
    STRING_MORE_LINES_END,

    LT_STATE,                   // OPERATORS
    LEQ_STATE,
    GT_STATE,
    GEQ_STATE,
    MINUS_STATE,
    ARROW_STATE,
    PLUS_STATE,
    MULTIPLY_STATE,
    LEFT_BRACKET_STATE,
    RIGHT_BRACKET_STATE,
    NOT_NIL_STATE,
    NOT_EQ_STATE,
    ALMOST_NIL_COND_STATE,
    NIL_COND_STATE,
    ASSIGNMENT_STATE,
    EQ_STATE,
    DIVIDE_STATE,

    LINE_COMMENT_START,         // COMMENTS
    LINE_COMMENT_END,
    BLOCK_COMMENT_START,
    BLOCK_COMMENT_ENDING,
    BLOCK_COMMENT_END
}State;


/** Funkce zjisti, zda vysledny IDENTIFIER neni nahodou nejaky KEYWORD */
void isKeyword(Token *token);

/** Funkce vytvori token pro parser, pripadne vraci cislo erroru */
int getToken(Token *token, int *lineCount);

#endif //_SCANNER_H_