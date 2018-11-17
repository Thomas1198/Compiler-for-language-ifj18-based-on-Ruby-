/**
 * @file symtable.h
 * @author
 * @brief
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"


/**
 * @struct Item data representation
 */
typedef struct {
    Dynamic_string *content_string; ///identifier (key)
    union value;	                /// Data type of symbol / return type of function
    bool defined;	                /// Defined if current function was defined
    enum data_type;                 /// Data type (int,char...)
    enum set_type;                  /// To which set token belogs (sign, variable...)
} Token;

/**
 * @struct Symbol table item representation
 */
typedef struct htab_listitem {
    Dynamic_string *key; /// identifier
    Token data; /// data
    struct htab_listitem *next; /// pointer to next item
} Titem;
