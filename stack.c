/**
 * @file stack.c
 * @author Tomas Dorda
 * @brief
 */

#include "stack.h"


void stack_init(Symstack *stack) {
    stack->top = NULL;
}

bool is_token_null(struct tToken *token){
    switch (token->data_type_of_token) {
        case INT:
            return token->value.i == 0;
        case FLOAT:
            return token->value.f == 0;
        default:
            return false;
    }
}


bool stack_push(Symstack *stack, struct tToken *token, set_type set, data_type type) {
    Sitem *new_item = (Sitem *) malloc(sizeof(Sitem));

    if (new_item == NULL)
        return false;

    if (token != NULL) {
        new_item->data = token;
        new_item->set = token->set_type_of_token;
        new_item->type =token->data_type_of_token;
        new_item->is_null = is_token_null(token);
    } else {
        new_item->data = NULL;
        new_item->set = set;
        new_item->type = type;
        new_item->is_null = false;
    }

    new_item->next = stack->top;

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
                new_item->is_null = is_token_null(token);
            } else {
                new_item->data = NULL;
                new_item->set = set;
                new_item->type = type;
                new_item->is_null = false;
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

Sitem *stack_get_top_term(Symstack *stack) {

    for (Sitem *tmp = stack->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->set < STOP)
            return tmp;
    }

    return NULL;
}

struct tToken *get_top(Symstack *stack) {

    return stack->top->data;
}

bool stack_is_empty(Symstack *stack) {
    if (stack->top != NULL)
        return false;
    return true;
}


void stack_free(Symstack *stack) {
    while (stack_pop(stack));
}