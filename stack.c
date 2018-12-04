/**
 * @file stack.c
 * @author Tomas Dorda
 * @brief
 */

#include "stack.h"


void stack_init(Symstack *stack) {
    stack->top = NULL;
}


bool stack_push(Symstack *stack, struct tToken *token, set_type set, data_type type) {
    Sitem *new_item = (Sitem *) malloc(sizeof(Sitem));

    if (new_item == NULL)
        return false;

    if (token != NULL) {
        new_item->data = token;
        new_item->set = token->set_type_of_token;
        new_item->type =token->data_type_of_token;
        new_item->next = stack->top;
    } else {
        new_item->data = NULL;
        new_item->set = set;
        new_item->type = type;
    }

    stack->top = new_item;
    return true;
}


void stack_pop_count(Symstack *stack, int count) {
    for (int i = 0; i < count; i++)
        stack_pop(stack);
}

bool stack_insert_after_top(Symstack *stack, struct tToken *token, set_type set, data_type type) {
    Sitem *previous = NULL;

    for (Sitem *tmp = stack->top; tmp != NULL; tmp = tmp->next) {
        if (tmp->set < STOP) {
            Sitem *new_item = (Sitem *) malloc(sizeof(Sitem));

            if (new_item == NULL)
                return false;

            if (token != NULL) {
                new_item->data = token;
                new_item->set = token->set_type_of_token;
                new_item->type =token->data_type_of_token;
                new_item->next = stack->top;
            } else {
                new_item->data = NULL;
                new_item->set = set;
                new_item->type = type;
            }

            if (previous == NULL) {
                new_item->next = stack->top;
                stack->top = new_item;
            } else {
                new_item->next = previous->next;
                previous->next = new_item;
            }

            return true;
        }

        previous = tmp;
    }

    return false;
}


bool stack_pop(Symstack *stack) {
    if (!stack_is_empty(stack)) {
        Sitem *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
        return true;
    }
    return false;
}

Sitem *stack_get_top_item(Symstack *stack) {
    Sitem *tmp = stack->top;

    while (!stack_is_empty(stack)) {
        if (tmp->set < STOP)
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
    while (stack_pop(stack));
}