/**
 * @file symtable_test.c
 * @author Tomas Dorda (xdorda00)
 * @brief Symtable test
 */

#include <stdio.h>

#include "symtable.h"
#include "dynamic_string.h"

void print_token(struct tToken *token, char *activity) {
    if (token == NULL || activity == NULL)
        return;
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


int main() {
    struct tToken ta, tb, tc;
    Dynamic_string *sa, *sb, *sc;

    sa = dynamic_string_init();
    dynamic_string_add_const_str(sa, "sa");
    sb = dynamic_string_init();
    dynamic_string_add_const_str(sb, "sb");
    sc = dynamic_string_init();
    dynamic_string_add_const_str(sc, "sc");

    init_token(&ta, sa);
    init_token(&tb, sb);
    init_token(&tc, sc);


    Titem *symtable[MAX_TABLE_SIZE];
    symtable_create(&symtable);

    symtable_insert(&symtable, &ta);
    symtable_insert(&symtable, &tb);
    symtable_insert(&symtable, &tc);

    puts("Tabulka symbolu:\n");

    print_token(symtable_get(&symtable, sa), "get token from table");
    print_token(symtable_get(&symtable, sb), "get token from table");
    print_token(symtable_get(&symtable, sc), "get token from table");

    puts("Tabulka symbolu - odebrani sb:\n");
    symtable_remove(&symtable, sb);

    print_token(symtable_get(&symtable, sa), "get token from table");
    print_token(symtable_get(&symtable, sb), "get token from table");
    print_token(symtable_get(&symtable, sc), "get token from table");

    symtable_destroy(&symtable);
    return 0;
}
