/**
 * @file token.c
 * @author
 * @brief
 */

#include "token.h"

void init_token(struct tToken *token) {
    token->set_type_of_token = UNDEFINED_SET;
    token->data_type_of_token = UNDEFINED;
    token->defined = false;
    token->value.i = 0;
    token->value.d = 0.0;
    token->value.f = 0.0;
    token->funkce = false;
    token->content_string = dynamic_string_init();

}