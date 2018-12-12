
/**
 * @brief Code generator test
 * @file generator_test_2.c
 * @author Jan Osolsobe (xosols00)
 */

#include "code_generator.h"

void init_token(struct tToken *token, Dynamic_string *string) {
    token->set_type_of_token = UNDEFINED_SET;
    token->data_type_of_token = UNDEFINED;
    token->defined = false;
    token->value.i = 0;
    token->content_string = string;
}

int main()
{
    struct tToken temp;
    Dynamic_string dyn_str;
    dynamic_string_init(&dyn_str);
    init_token(&temp, &dyn_str);
    dynamic_string_add_const_str(temp.content_string, "Hello world");
    char fun[]= "hello";

    generator_start();
    generate_main_start();
    generate_function_before_par();
    generate_function_par(temp,1);
    generate_function_call(fun);

    generate_function_start(fun);
    generate_function_end(fun);


    generate_main_end();
    write_code();

    dynamic_string_free(temp.content_string);
    generator_clear();
    return 0;
}
