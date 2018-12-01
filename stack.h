/**
 * @file stack.h
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */

#include <stdlib.h>

#ifndef PROJECT_STACK_H
#define PROJECT_STACK_H


#include <stdbool.h>

#include "error.h"
#include "symtable.h"
#include "token.h"


/**
 * @struct Stack item representation.
 */
typedef struct stack_item {
    struct tToken *data;      /// pointer to token
    struct stack_item *next; /// pointer to next item
} Sitem;

/**
 * @struct Stack representation.
 */
typedef struct {
    Sitem *top; /// Pointer to stack item on top of stack.
}Symstack;


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
void stack_push(Symstack *stack, struct tToken *token);


/**
 * Pops top symbol from stack.
 *
 * @param stack Pointer to stack.
 * @return True if successful else false.
 */
bool stack_pop(Symstack *stack);


/**
 * Returns top symbol.
 *
 * @param stack Pointer to stack.
 * @return Pointer to symbol on top of stack.
 */
Sitem *symbol_stack_top(Symstack *stack);


/**
 * Frees resources used for stack.
 *
 * @param stack Pointer to stack.
 */
void stack_free(Symstack *stack);


#endif //PROJECT_STACK_H