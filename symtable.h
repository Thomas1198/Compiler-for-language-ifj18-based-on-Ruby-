/**
 * @file symtable.h
 * @author
 * @brief
 */

#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"
#include "token.h"

#define MAX_TABLE_SIZE 27457 //TODO change the number

/**
 * @struct Symbol table item representation
 */
typedef struct htab_item {
    Dynamic_string *key;     /// identifier
    struct tToken data;      /// data
    struct htab_item *next; /// pointer to next item
} Titem;


// Symbol table
typedef Titem* Symtable[MAX_TABLE_SIZE];


/**
 * @brief initialise symbol table
 *
 * @param tab pointer on table
 */
void symtable_create(Symtable *tab);

/**
 * @brief Appends item to table
 *
 * @param table pointer to table.
 * @param key function identifier or variable
 * @return NULL if error or item is existing else returns pointer to added item.
 */
struct tToken *symtable_insert(Symtable *table, const char *key);

/**
 * @brief finds symbol and returns its data
 *
 * @param table pointer to table
 * @param key function identifier or variable
 * @return NULL if not existing symbol, pointer to data of symbol if successfully found.
 */
struct tToken *symtable_get(Symtable *table, const char *key);


/**
 * @brief removes symbol
 *
 * @param table pointer to table
 * @param key function identifier or variable
 * @return true if successful, false if unsuccessful.
 */
bool symtable_remove(Symtable *table, const char *key);

/**
 * @brief frees resources
 *
 * @param table pointer to table
 */
void symtable_destroy(Symtable *table);