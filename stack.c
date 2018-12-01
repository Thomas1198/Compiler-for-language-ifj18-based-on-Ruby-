/**
 * @file stack.c
 * @author Tomas Dorda
 * @brief
 */

#include "stack.h"


void stack_init(Symstack *stack) {
    stack->top = NULL;
}


void stack_push(Symstack *stack, struct tToken *token) {
    Sitem *new_item = (Sitem *) malloc(sizeof(Sitem));

    if (new_item == NULL)
        ErrorPrint(INTERNAL_ERROR,"[Stack.c][stack_push]");

    new_item->data = token;
    new_item->next = stack->top;

    stack->top = new_item;
}


bool stack_pop(Symstack *stack) {
    if (stack->top != NULL) {
        Sitem *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
        return true;
    }
    return false;
}


Sitem *symbol_stack_top(Symstack *stack) {
    return stack->top;
}


void stack_free(Symstack *stack) {
    while (stack_pop(stack));
}