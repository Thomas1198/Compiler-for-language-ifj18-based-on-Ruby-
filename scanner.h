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

/**
 * @brief loads characters from the input file and decides whats the next token.
 *
 * @param the input file with RB code
 * @return a fully initialized and recognized next token.
 */
struct tToken get_token(FILE *source_file);

/**
 * @brief takes an integer to process if it's valid
 *
 * @param input token that we're working with
 * @return a processed token with a correct integer. If invalid, it exits the function and outputs an error.
 */
struct tToken process_integer(struct tToken token);


/**
 * @brief takes a float to process if it's valid
 *
 * @param input token that we're working with
 * @return a processed token with a correct float number. If invalid, it exits the function and outputs an error.
 */
struct tToken process_decimal(struct tToken token);

/**
 * @brief compares an input string with existing keywords to decide whether it is a keyword or an identifier name.
 *
 * @param input token that we're working with
 * @return either a keyword token if the comparison matches up, or an identifier_name token.
 */
struct tToken process_identifier(struct tToken token);

/**
 * @brief decides if the input string is a multiline comment start, end, or just an ordinary string.
 *
 * @param input token that we're working with
 * @return 1,2 or 3, depending on the comparison result. 
 */
int process_commentary(struct tToken token);

#endif //PROJECT_SCANNER_H