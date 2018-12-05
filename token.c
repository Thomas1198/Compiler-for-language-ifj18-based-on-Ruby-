/**
 * @file token.c
 * @author Tomas Dorda (xdorda00)
 * @brief Representation of token and token functions
 */


#include "token.h"

void init_token(struct tToken *token) {
    token->set_type_of_token = UNDEFINED_SET;
    token->data_type_of_token = UNDEFINED;
    token->defined = false;
    token->value.i = 0;
    token->value.f = 0.0;
    token->content_string = dynamic_string_init();
    token->par_count = 0;
    token->more_params=false;
    token->build_id=false;
    token->assigned=false;
    token->funkce = false;
}