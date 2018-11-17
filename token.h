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
typedef struct {
    Dynamic_string *content_string; ///identifier (key)
    union value;	                /// Data type of symbol / return type of function
    bool defined;	                /// Defined if current function was defined
    enum data_type;                 /// Data type (int,char...)
    enum set_type;                  /// To which set token belogs (sign, variable...)
} Token;

