/**
 * @file token_list_test.c
 * @author Tomas Dorda (xdorda00)
 * @brief Token list test
 */

#include "../token_list.h"
#include "../token.h"

void print_token(struct tToken *token, char *activity) {
    printf("ACTIVITY: %s \n", activity);
    printf("Dynamic_string *content_string: \"%s\"\n", token->content_string->str);
    printf("union value_union value: %d\n", token->value);
    printf("bool defined: %s\n", token->defined ? "true" : "false");
    printf("data_type data_type_of_token: %d\n", token->data_type_of_token);
    printf("set_type set_type_of_token: %d\n\n", token->set_type_of_token);
}
void init_token(struct tToken *token) {
    token->set_type_of_token = UNDEFINED;
    token->data_type_of_token = UNDEFINED;
    token->defined = false;
    token->value = 0;
    token->content_string =
}

int main(){
    struct tToken a,b,c,d,e;




    return 0;
}