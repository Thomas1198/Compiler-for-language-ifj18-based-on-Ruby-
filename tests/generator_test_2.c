
/**
 * @brief Code generator test
 * @file generator_test_2.c
 * @author Jan Osolsobe (xosols00)
 */

#include "../code_generator.h"
#include "../token.h"

int main()
{
    tToken temp;
    temp.content_string="Hello world";
    char fun="hello";

    generator_start();
    generate_file_head();
    generate_main_start();
    generate_function_before_par();
    generate_function_par(temp,1);
    generate_function_call(fun);

    generate_function_start(fun);
    generate_function_end(fun);


    generate_main_end();
    write_code();

    generator_clear();
    return 0;
};