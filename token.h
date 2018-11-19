/**
 * @file token.h
 * @author
 * @brief
 */

#include <stdbool.h>

#include "dynamic_string.h"

/**
 * @struct Token representation
 */
/*
typedef enum {
    KEY_WORD_DEF,
    KEY_WORD_DO,
    KEY_WORD_ELSE,
    KEY_WORD_END,
    KEY_WORD_IF,
    KEY_WORD_NOT,
    KEY_WORD_NIL,
    KEY_WORD_THEN,
    KEY_WORD_WHILE,

    LITERAL_INT,
    LITERAL_FLOAT,
    LITERAL_STRING,

    IDENTIFIRE_NAME,

    OPERATOR_ASSINMENT,

    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLICATION,
    OPERATOR_DIVISION,

    OPERATOR_IS_LOWER,
    OPERATOR_IS_BIGGER,
    OPERATOR_IS_LOWER_OR_EQUAL,
    OPERATOR_IS_BIGGER_OR_EQUAL,
    OPERATOR_IS_EQUAL,
    OPERATOR_IN_NOT_EQUAL,

    CHAR_LEFT_BRACKET,
    CHAR_RIGTH_BRACKET,

    CHAR_SEMICOLON,
    CHAR_EOL,
    CHAR_EOF,


} set_type;*/
typedef enum {
	CHAR_OPERATOR_PLUS,
	CHAR_OPERATOR_MINUS,
	CHAR_OPERATOR_DIV,
	CHAR_OPERATOR_MUL,
	CHAR_ASSIGN,
    CHAR_EOL,
    IDENTIFIER_NAME,
    CHAR_LEFT_BRACKET,
    CHAR_RIGTH_BRACKET,
    LITERAL_NAME,
    KEY_WORD_FOLLOWED_BY_EOL,
    KEY_WORD_FOLLOWED_BY_CONDITION,
    KEY_WORD_NIL,
    KEY_WORD_DEF,
	EMPTY,
	CHAR_SEMICOLON,
	CHAR_COMMA,
	CHAR_EOF,


}set_type;

typedef enum {
    INT,
    FLOAT,
    STRING,
    BOOLEAN,
    UNDEFINED,
}data_type;

typedef struct tToken {
    Dynamic_string *content_string; ///identifier (key)
    union value;                    /// Data type of symbol / return type of function
    bool defined;                    /// Defined if current function was defined
    enum data_type data_type_of_token;            /// Data type (int,char...)
    enum set_type set_type_of_token;                  /// To which set token belogs (sign, variable...)
};


