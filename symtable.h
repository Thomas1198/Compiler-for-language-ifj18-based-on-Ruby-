/**
 * @file symtable.h
 * @author
 * @brief
 */

#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"
#include "token.h"

/**
 * @struct Symbol table item representation
 */
typedef struct htab_listitem {
    Dynamic_string *key;     /// identifier
    Token data;             /// data
    struct htab_listitem *next; /// pointer to next item
} Titem;
