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
    token->par_count = 0;
    token->more_params = false;
}

void copy_token(struct tToken *dst, struct tToken *src) {
    dst->set_type_of_token = src->set_type_of_token;
    dst->data_type_of_token = src->data_type_of_token;
    dst->defined = src->defined;
    dst->value.i = src->value.i;
    dst->value.d = src->value.d;
    dst->value.f = src->value.f;
    dst->funkce = src->funkce;
    dst->content_string = dynamic_string_init();
    dynamic_string_add_const_str(dst->content_string, dst->content_string->str);
    dst->par_count = src->par_count;
    dst->more_params = src->more_params;
}