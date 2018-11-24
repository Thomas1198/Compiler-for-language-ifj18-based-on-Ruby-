/**
 * @file scanner.h
 * @author Patrik Strnad (xstrna11)
 * @brief
 */

#ifndef PROJECT_SCANNER_H
#define PROJECT_SCANNER_H

#include <stdio.h>
#include "token.h"
#include "dynamic_string.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"

struct tToken get_token(FILE *source_code);

void initFile(FILE *f);

void set_content_string(Dynamic_string *string);

typedef enum {
    START,
    COMMENTARY,
    BACKSLASH,
    CHUNK_COMMENTARY,
    CHUNK_COMMENTARY_END,
    KEYWORD,
    NUMBER,
    NUMBER_DOUBLE,
    NUMBER_DEC,
    NUMBER_EXP,
    NUMBER_EXP_DONE,
    NUMBER_EXP_DONE_LAST,
    NUMBER_EXP_SIGN,
    STRING_START,
    STRING,
    LESS_THAN,
    GREATER_THAN,
    EOL,
    SEMICOLON,
    EQUALS,
    EXCLAMATION,
    STARTCHUNKCOMMENTARY,
    ENDCHUNKCOMMENTARY        
} SCANNER_STATE;

#endif //PROJECT_SCANNER_H