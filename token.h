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

typedef enum {
    KEY_WORD, DATA_TYPE, IDENTIFIRE, OPERATOR, LITERAL, EOL_DT,
} data_type;

typedef struct tToken {
    Dynamic_string *content_string; ///identifier (key)
    union value;                    /// Data type of symbol / return type of function
    bool defined;                    /// Defined if current function was defined
    data_type data_type_of_token;                 /// Data type (int,char...)
    enum set_type;                  /// To which set token belogs (sign, variable...)
};


