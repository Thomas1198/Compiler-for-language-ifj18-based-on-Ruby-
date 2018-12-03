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
        ErrorPrint(INTERNAL_ERROR, "[Stack.c][stack_push]");

    new_item->data = token;
    new_item->atribute = 0;//TODO
    new_item->reduce = 0;//TODO
    new_item->next = stack->top;

    stack->top = new_item;
}


struct tToken *stack_pop(Symstack *stack) {
    if (!stack_is_empty(stack)) {
        Sitem *tmp = stack->top;
        struct tToken *top_token = stack->top->data;
        stack->top = tmp->next;
        free(tmp);
        return top_token;
    }
    return NULL;
}

Sitem *stack_get_top_item(Symstack *stack) {
    Sitem *tmp=stack->top;

    while (tmp!=NULL){
        if (tmp->next == NULL)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}


bool stack_is_empty(Symstack *stack) {
    if (stack->top != NULL)
        return true;
    return false;
}


void stack_free(Symstack *stack) {
  //  while (stack_pop(stack));
}