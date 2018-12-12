/**
 * @file token_list_test.c
 * @author Tomas Dorda (xdorda00)
 * @brief Token list test
 */

#include "token_list.h"

void print_token(struct tToken *token, char *activity) {
    printf("ACTIVITY: %s \n", activity);
    printf("Dynamic_string *content_string: \"%s\"\n", token->content_string->str);
    printf("union value_union value: %d\n", token->value.i);
    printf("bool defined: %s\n", token->defined ? "true" : "false");
    printf("data_type data_type_of_token: %d\n", token->data_type_of_token);
    printf("set_type set_type_of_token: %d\n\n", token->set_type_of_token);
}
void init_token(struct tToken *token, Dynamic_string *string) {
    token->set_type_of_token = UNDEFINED_SET;
    token->data_type_of_token = UNDEFINED;
    token->defined = false;
    token->value.i = 0;
    token->content_string = string;
}


int main(){
    struct tToken ta,tb,tc,td,te;
    Dynamic_string sa,sb,sc,sd,se;

    dynamic_string_init(&sa);
    dynamic_string_add_const_str(&sa, "sa");
    dynamic_string_init(&sb);
    dynamic_string_add_const_str(&sb, "sb");
    dynamic_string_init(&sc);
    dynamic_string_add_const_str(&sc, "sc");
    dynamic_string_init(&sd);
    dynamic_string_add_const_str(&sd, "sd");
    dynamic_string_init(&se);
    dynamic_string_add_const_str(&se, "se");

    init_token(&ta,&sa);
    print_token(&ta,"inicializace tokenu");
    init_token(&tb,&sb);
    print_token(&tb,"inicializace tokenu");
    init_token(&tc,&sc);
    init_token(&td,&sd);
    init_token(&te,&se);

    tDList list;
    DLInitList(&list);

    DLInsertLast(&list, ta);
    DLInsertLast(&list, tb);
    DLInsertLast(&list, tc);
    DLInsertLast(&list, td);
    DLInsertLast(&list, te);

    puts("Linearni seznam:\n");

    DLFirst(&list);

    printf("-%s\n", list.Act->token.content_string->str);
    DLSucc(&list);
    printf("-%s\n", list.Act->token.content_string->str);
    DLSucc(&list);
    printf("-%s\n", list.Act->token.content_string->str);
    DLSucc(&list);
    printf("-%s\n", list.Act->token.content_string->str);
    DLSucc(&list);
    printf("-%s\n", list.Act->token.content_string->str);
    DLSucc(&list);

    DLDisposeList(&list);

    return 0;
}
