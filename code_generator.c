/**
 * @brief Code generator
 * @file code_generator.c
 * @author Jan Osolsobe (xosols00)
 */


#include <ctype.h>

#include "dynamic_string.h"
#include "code_generator.h"

#define ADD_INSTRUCTION(instruction)
    if (!dynamic_string_add_const_str(&gen_code, (instruction "\n"))) return false

#define ADD_COMMENT(comment)
    if (!dynamic_string_add_const_str(&gen_code, ( "# "comment "\n"))) return false

#define ADD_CODE(code)
    if (!dynamic_string_add_const_str(&gen_code, (code))) return false

#define ADD_INTIGER(code) ///convert string to int
    do{
        char string[20];
        sprintf(string, "%d", code);
        ADD_CODE(string);
    } while (0)


Dynamic_string gen_code; ///string to generated code


bool generate_file_head()
{
    ADD_COMMENT("Start of program");
    ADD_INSTRUCTION(".IFJcode18");

    ADD_INSTRUCTION("JUMP $$main")

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

    ADD_INTIGER("42")

    return true;
}

bool generate_main_end()
{
    ADD_COMMENT("End of main");

    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("CLEARS");
}