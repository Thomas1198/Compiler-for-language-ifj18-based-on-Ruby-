/**
 * @file scanner.h
 * @author Patrik Strnad (xstrna11)
 * @brief Scanner, tokenize input
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

typedef enum {
    START,
    COMMENTARY,
    KEYWORD,
    NUMBER,
    NUMBER_DOUBLE,
    NUMBER_DEC,
    NUMBER_EXP,
    NUMBER_EXP_DONE,
    NUMBER_EXP_SIGN,
    STRING_START,
    LESS_THAN,
    GREATER_THAN,
    EOL,
    EQUALS,
    EXCLAMATION,
    STARTCHUNKCOMMENTARY,
    STARTCHUNKCOMMENTARYCONTINUE,
    MIGHTBECOMMENT,
    KEYWORDLONGER
} SCANNER_STATE;


struct tToken get_token(FILE *source_file);

struct tToken process_integer(struct tToken token);

struct tToken process_decimal(struct tToken token);

struct tToken process_identifier(struct tToken token);

int process_commentary(struct tToken token);

#endif //PROJECT_SCANNER_H