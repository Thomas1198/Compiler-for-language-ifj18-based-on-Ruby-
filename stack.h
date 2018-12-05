/**
 * @file stack.h
 * @author Tomas Dorda (xdorda00)
 * @brief Implementation of stack
 */



#ifndef PROJECT_STACK_H
#define PROJECT_STACK_H

#include <stdlib.h>

#include "error.h"
#include "symtable.h"
#include "token.h"
#include "parser.h"

/**
 * @struct Representation of tack item
 */
typedef struct stack_item {
    struct tToken *data;      /// pointer to token
    set_type set;
    data_type type;
    bool is_null;
    struct stack_item *next; /// pointer to next item
} Sitem;


/**
 * @struct Representation of stack
 */
typedef struct tStack{
    Sitem *top; /// Pointer to stack item on top of stack.
} Symstack;


/**
 * @brief Initializes stack.
 *
 * @param stack Pointer to stack.
 */
void stack_init(Symstack *stack);


/**
 * @brief Pushes symbol to stack
 *
 * @param stack pointer to stack.
 * @param token token to be pushed
 * @param set
 * @param type
 */
bool stack_push(Symstack *stack, struct tToken *token, set_type set, data_type type);


/**
 * @brief Pops more times.
 *
 * @param stack Pointer to stack.
 * @param count number of items to be popped
 */
void stack_pop_count(Symstack *stack, int count);


/**
 * @brief Inserts symbol after top terminal.
 *
 * @param stack pointer to stack
 * @param token token to be pushed
 * @param set set type
 * @param type data type
 * @return true if successful
 */
bool stack_insert_after_top(Symstack *stack, struct tToken *token, set_type set, data_type type);


/**
 * @brief Pops top symbol from stack.
 *
 * @param stack Pointer to stack.
 * @return top item or NULL
 */
bool stack_pop(Symstack *stack);

/**
 * @brief Gets top symbol from stack.
 *
 * @param stack Pointer to stack.
 * @return top symbol or NULL
 */
Sitem *stack_get_top_term(Symstack *stack);

/**
 * @brief Checks if stack is empty.
 *
 * @param stack Pointer to stack.
 * @return True if empty, false if its not empty
 */
bool stack_is_empty(Symstack *stack);


/**
 * @brief Frees resources.
 *
 * @param stack pointer to stack.
 */
void stack_free(Symstack *stack);

/**
 * @brief Gets top token.
 *
 * @param stack pointer to stack.
 */
struct tToken *get_top(Symstack *stack);

#endif //PROJECT_STACK_H