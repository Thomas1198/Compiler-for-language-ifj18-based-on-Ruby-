//
// Created by Jan on 21.11.2018.
//
/**
 * @brief Code generator test
 * @file generator_test_1.c
 * @author Jan Osolsobe (xosols00)
 */

#include "../code_generator.h"

int main()
{
    generator_start();
    generate_file_head();
    generate_main_start();
    generate_main_end();
    write_code();
    generator_clear();
    return 0;
};