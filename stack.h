/**
 * @file stack.h
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */



#ifndef PROJECT_STACK_H
#define PROJECT_STACK_H

#include <stdlib.h>

#include "error.h"
#include "symtable.h"
#include "token.h"
#include "parser.h"

/**
 * @struct Stack item representation.
 */
typedef struct stack_item {
    struct tToken *data;      /// pointer to token
    set_type set;
    data_type type;
    bool is_null;
    struct stack_item *next; /// pointer to next item
} Sitem;


/**
 * @struct Stack representation.
 */
typedef struct tStack{
    Sitem *top; /// Pointer to stack item on top of stack.
} Symstack;


/**
 * Initializes stack.
 *
 * @param stack Pointer to stack.
 */
void stack_init(Symstack *stack);


/**
 * Pushes symbol to stack and sets its data type.
 *
 * @param stack Pointer to stack.
 * @param token token to be pushed.
 */
bool stack_push(Symstack *stack, struct tToken *token, set_type set, data_type type);


/**
 * Function pops stack more times.
 *
 * @param stack Pointer to stack.
 * @param count How many times stack will be popped.
 */
void stack_pop_count(Symstack *stack, int count);


/**
 * Function inserts symbol after top terminal.
 *
 * @param stack Pointer to stack.
 * @param symbol Symbol to be pushed.
 * @param type Data type to be set.
 * @return True if successfull else false.
 */
bool stack_insert_after_top(Symstack *stack, struct tToken *token, set_type set, data_type type);


/**
 * Pops top symbol from stack.
 *
 * @param stack Pointer to stack.
 * @return Top item or NULL if it does not exist
 */
bool stack_pop(Symstack *stack);

/**
 * Returns top symbol from stack.
 *
 * @param stack Pointer to stack.
 * @return Top item or NULL if it does not exist
 */
Sitem *stack_get_top_term(Symstack *stack);

/**
 * Checks if is stack empty.
 *
 * @param stack Pointer to stack.
 * @return True if empty, false if nonempty
 */
bool stack_is_empty(Symstack *stack);


/**
 * Frees resources used for stack.
 *
 * @param stack Pointer to stack.
 */
void stack_free(Symstack *stack);

struct tToken *get_top(Symstack *stack);

#endif //PROJECT_STACK_H