/**
 * @file token.h
 * @author Tomas Dorda (xdorda00)
 * @brief Representation of token and token functions
 */


#ifndef PROJECT_TOKEN_H
#define PROJECT_TOKEN_H


#include <stdbool.h>

#include "dynamic_string.h"

/**
 * @struct Token representation
 */

typedef enum {
    CHAR_OPERATOR_PLUS,
    CHAR_OPERATOR_MINUS,
    CHAR_OPERATOR_DIV,
    CHAR_OPERATOR_MUL,
    CHAR_EQUALS,
    CHAR_DOUBLEEQ,
    CHAR_EOL,
    IDENTIFIER_NAME,
    CHAR_LEFT_BRACKET,
    CHAR_RIGHT_BRACKET,
    KEY_WORD_DEF,
    KEY_WORD_DO,
    KEY_WORD_ELSE,
    KEY_WORD_END,
    KEY_WORD_IF,
    KEY_WORD_NOT,
    KEY_WORD_NIL,
    KEY_WORD_THEN,
    KEY_WORD_WHILE,
    CHAR_COMMA,
    CHAR_EOF,
    LITERAL_STRING,
    CHAR_NEQ,
    CHAR_LEQ,
    CHAR_GEQ,
    CHAR_LT,
    CHAR_GT,
    CHAR_INTEGER,
    CHAR_DOUBLE,
    UNDEFINED_SET,
    NONE,
    DOLLAR,
    STOP,
    NON_TERM
} set_type;

typedef enum {
    INT,
    FLOAT,
    STRING_DT,
    BOOLEAN,
    NIL,
    UNDEFINED,
} data_type;

union value_union {
    int i;
    float f;
};

struct tToken {
    Dynamic_string *content_string; ///identifier (key)
    union value_union value;
    bool defined;
    data_type data_type_of_token;  /// Data type (int,char...)
    set_type set_type_of_token;    /// Which set token belongs to (sign, variable...)
    bool funkce;
    int par_count;
    bool more_params;
    bool build_id;
    bool assigned;
};

/**
 * @brief initialise token
 *
 * @param tab pointer on table
 */
void init_token(struct tToken *token);

#endif //PROJECT_TOKEN_H