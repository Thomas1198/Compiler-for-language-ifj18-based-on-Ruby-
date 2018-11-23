/**
 * @brief Code generator
 * @file code_generator.c
 * @author Jan Osolsobe (xosols00)
 */

#include "code_generator.h"

Dynamic_string gen_code; ///string to generated code


bool generate_file_head()
{
    ADD_COMMENT("Start of program\n");
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
    ADD_COMMENT("Start of main\n");

    ADD_INSTRUCTION("LABEL $$main");
    ADD_INSTRUCTION("CREATFRAME");
    ADD_INSTRUCTION("PUSHFRAME");

    return true;
}

bool generate_main_end()
{
    ADD_COMMENT("End of main\n");

    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("CLEARS");

    return true;
}

bool generate_function_call(char *name)
{
    ADD_COMMENT("Function call\n");

    ADD_CODE("CALL $"); ADD_CODE(name); ADD_CODE("\n");

    return true;
}

bool generate_function_before_par()
{
    ADD_INSTRUCTION("CREATEFRAME");
    return true;
}

bool generate_function_par(struct tToken param, int index)
{
    ADD_CODE("DEFVAR LF@"); ADD_CODE(param.content_string->str); ADD_CODE("/n");
    ADD_CODE("MOVE LF@"); ADD_CODE(param.content_string->str); ADD_CODE("LF@"); ADD_INTIGER(index);

    return true;
}

bool generate_function_start(char *name)
{
    ADD_COMMENT("\n Start of function"); ADD_CODE(name); ADD_CODE("\n");

    ADD_CODE("LABEL $"); ADD_CODE(name); ADD_CODE("\n");
    ADD_INSTRUCTION("PUSHFRAME");

    return true;
}

bool generate_function_end(char *name)
{
    ADD_COMMENT("\n End of function"); ADD_CODE(name); ADD_CODE("\n");

    ADD_CODE("LABEL $"); ADD_CODE(name); ADD_CODE("%return\n");
    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("RETURN");
    return true;
}
