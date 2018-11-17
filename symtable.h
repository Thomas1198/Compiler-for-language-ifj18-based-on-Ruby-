/**
 * @file symtable.h
 * @author
 * @brief
 */
#include <stdbool.h>

#include "dynamic_string.h"


/**
 * @struct Item data representation
 */
typedef struct {
    Dynamic_string *content_string; ///identifier (key)
    union value;			/// Data type of symbol / return type of function
    bool defined;			/// Defined if current function was defined
    enum type
    char *identifier;		/// Data identifier (key).

} TData;

/**
 * @struct Symbol table item representation
 */
typedef struct htab_listitem
{
    char *key; /// identifier
    TData data; /// data
    struct htab_listitem *next; /// pointer to next item
} Titem;