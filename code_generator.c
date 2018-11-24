/**
 * @brief Code generator
 * @file code_generator.c
 * @author Jan Osolsobe (xosols00)
 */

#include "code_generator.h"

Dynamic_string *gen_code; ///string to generated code


void generate_file_head()
{
    ADD_COMMENT("Start of program\n");
    ADD_INSTRUCTION(".IFJcode18");

    ADD_INSTRUCTION("JUMP $$main");
}

void generate_build_in_functions()
{
    ADD_INSTRUCTION(PRINT);

    ADD_INSTRUCTION(INPUTS);
    ADD_INSTRUCTION(INPUTI);
    ADD_INSTRUCTION(INPUTF);

    ADD_INSTRUCTION(LENGTH);
    ADD_INSTRUCTION(SUBSTR);
    ADD_INSTRUCTION(ASC);
    ADD_INSTRUCTION(CHR);
}

void generator_start()
{

    gen_code=dynamic_string_init();

    generate_file_head();
    generate_build_in_functions();
}

void generator_clear()
{
    dynamic_string_clear(gen_code);
}

void write_code()
{
    printf ("%s", gen_code->str);
}

void generate_main_start()
{
    ADD_COMMENT("Start of main\n");

    ADD_INSTRUCTION("LABEL $$main");
    ADD_INSTRUCTION("CREATFRAME");
    ADD_INSTRUCTION("PUSHFRAME");
}

void generate_main_end()
{
    ADD_COMMENT("End of main\n");

    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("CLEARS");
}

void generate_function_call(struct tToken function)
{
    ADD_COMMENT("Function call\n");

    ADD_CODE("CALL $"); ADD_CODE(function.content_string); ADD_CODE("\n");

}

void generate_function_before_par()
{
    ADD_INSTRUCTION("CREATEFRAME");
}

void generate_function_par(struct tToken param, int index)
{
    ADD_CODE("DEFVAR LF@"); ADD_CODE("%"); ADD_INTEGER(index); ADD_CODE("\n");
    ADD_CODE("MOVE LF@");  ADD_CODE("%"); ADD_INTEGER(index); ADD_CODE("  LF@%"); ADD_CODE(param.content_string->str); ADD_CODE("\n");

}

void generate_function_start(struct tToken function)
{
    ADD_COMMENT("Start of function "); ADD_CODE(function.content_string); ADD_CODE("\n");

    ADD_CODE("LABEL $"); ADD_CODE(function.content_string); ADD_CODE("\n");
    ADD_INSTRUCTION("PUSHFRAME");

}

void generate_function_end()
{
    ADD_COMMENT("End of function "); ADD_CODE(name); ADD_CODE("\n");

    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("RETURN");
}
