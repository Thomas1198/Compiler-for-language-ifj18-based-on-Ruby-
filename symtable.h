/**
 * @file symtable.h
 * @author
 * @brief
 */




#ifndef PROJECT_SYMTABLE_H
#define PROJECT_SYMTABLE_H

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "dynamic_string.h"
#include "token.h"



#define MAX_TABLE_SIZE 27457 //TODO change the number

/**
 * @struct Symbol table item representation
 */
typedef struct htab_item {
    struct tToken *data;      /// pointer to token
    struct htab_item *next; /// pointer to next item
} Titem;


// Symbol table
typedef Titem *Symtable[MAX_TABLE_SIZE];


/**
 * @brief initialise symbol table
 *
 * @param tab pointer on table
 */
void symtable_create(Symtable *tab);

/**
 * @brief Inserts item to table
 *
 * @param table pointer to table.
 * @param token token
 */
void symtable_insert(Symtable *table, struct tToken *token);

/**
 * @brief finds symbol and returns its data
 *
 * @param table pointer to table
 * @param key function identifier or variable
 * @return NULL if not existing symbol, pointer to data of symbol if successfully found.
 */
struct tToken *symtable_get(Symtable *table, Dynamic_string *key);


/**
 * @brief removes symbol
 *
 * @param table pointer to table
 * @param key function identifier or variable
 * @return true if successful, false if unsuccessful.
 */
void symtable_remove(Symtable *table, Dynamic_string *key);

/**
 * @brief destroys table and frees resources
 *
 * @param table pointer to table
 */
void symtable_destroy(Symtable *table);

#endif //PROJECT_SYMTABLE_H