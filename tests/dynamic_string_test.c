/**
 * @file dynamic_string_test.c
 * @author Tomas Dorda (xdorda00)
 * @brief Dynamic string interface test
 */

#include <stdio.h>
#include "../dynamic_string.h"

void print_strukture(Dynamic_string *s, char *activity) {
    printf("ACTIVITY: %s \n", activity)
    printf("char *str: \"%s\"\n", s->str);
    printf("unsigned int length: %d\n", s->length);
    printf("unsigned int alloc_size: %d\n\n", s->alloc_size);
}

int main(){
    Dynamic_string test;
    Dynamic_string new;

    dynamic_string_init(&test);
    print_strukture(&test, "INIT");

    dynamic_string_add_const_str(&test,"1234567");
    print_strukture(&test,"add const str");

    dynamic_string_add_char(&test,'8');
    print_strukture(&test,"add char");

    printf("COMPARE: %d\n", dynamic_string_cmp_const_str(&test, "12345678"));

    dynamic_string_init(&new);

    dynamic_string_copy(&test,&new);
    print_strukture(&new,"COPY");

    dynamic_string_clear(&test);
    print_strukture(&test,"clear");

    dynamic_string_free(&test);
    dynamic_string_free(&new);
    print_strukture(&new,"free test");

    return 0;
}