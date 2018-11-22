/**
 * @brief Code generator
 * @file code_generator.c
 * @author Jan Osolsobe (xosols00)
 */

#include "code_generator.h"

Dynamic_string gen_code; ///string to generated code


bool generate_file_head()
{
    ADD_COMMENT("Start of program");
    ADD_INSTRUCTION(".IFJcode18");

    ADD_INSTRUCTION("JUMP $$main");

    return true;
}

bool generator_start()
{
    if(!dynamic_string_init(&gen_code))
    {
        return false;
    }
    if(!generate_file_head())
    {
        return false;
    }
    return true;

}

void generator_clear()
{
    dynamic_string_clear(&gen_code);
}

void write_code()
{
    printf ("%s", gen_code.str);
}

bool generate_main_start()
{
    ADD_COMMENT("Start of main");

    ADD_INSTRUCTION("LABEL $$main");
    ADD_INSTRUCTION("CREATFRAME");
    ADD_INSTRUCTION("PUSHFRAME");

    ADD_INTIGER(42);

    return true;
}

bool generate_main_end()
{
    ADD_COMMENT("End of main");

    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("CLEARS");

    return true;
}
