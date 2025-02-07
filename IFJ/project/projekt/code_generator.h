/**
 * @file code_generator.h
 * 
 * IFJ Projekt 2023
 * 
 * @author <xmicha94> Kryštof Michálek
*/

#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_
#include "parser.h"

/* Funkce pro generovani urciteho kodu v IFJcode23 */
void generate_code(parserData *data, int x, char *id, int i, int i2, double d, char *s);

#endif // _CODE_GENERATOR_H_