/**
 * @brief Code generator
 * @file code_generator.c
 * @author Jan Osolsobe (xosols00)
 */

#include "code_generator.h"
#include "dynamic_string.h"

Dynamic_string *gen_code; ///string to generated code


void generate_file_head()
{
    ADD_COMMENT("Start of program\n");
    ADD_INSTRUCTION(".IFJcode18");

    ADD_INSTRUCTION("DEFVAR GF@%tmp_op1");
    ADD_INSTRUCTION("DEFVAR GF@%tmp_op2");
    ADD_INSTRUCTION("DEFVAR GF@%tmp_op3");

    ADD_INSTRUCTION("DEFVAR GF@%exp_result");

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
    ADD_INSTRUCTION(ORD);
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

    ADD_CODE("CALL $"); ADD_CODE(function.content_string->str); ADD_CODE("\n");

}

void generate_function_before_par()
{
    ADD_INSTRUCTION("CREATEFRAME");
}

void generate_function_par_degf(struct tToken param, int index)
{
    ADD_CODE("DEFVAR LF@"); ADD_CODE("%"); ADD_INTEGER(index); ADD_CODE("\n");
    ADD_CODE("MOVE LF@");  ADD_CODE("%"); ADD_INTEGER(index); ADD_CODE("  LF@%"); ADD_CODE(param.content_string->str); ADD_CODE("\n");
}

void generate_function_start(struct tToken function)
{
    ADD_COMMENT("Start of function "); ADD_CODE(function.content_string->str); ADD_CODE("\n");

    ADD_CODE("LABEL $"); ADD_CODE(function.content_string->str); ADD_CODE("\n");
    ADD_INSTRUCTION("PUSHFRAME");

}

void generate_function_end(struct tToken function)
{
    ADD_COMMENT("End of function "); ADD_CODE(function.content_string->str); ADD_CODE("\n");

    ADD_CODE("LABEL $"); ADD_CODE(function.content_string->str); ADD_CODE("%return\n");
    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("RETURN");
}

void generate_defaul_value(struct tToken var)
{
    switch (var.data_type_of_token)
    {
        case INT:
            ADD_CODE("int@0");
            return;
        case FLOAT:
            ADD_CODE("float@0.0");
            return;
        case STRING_DT:
            ADD_CODE("string@");
            return;
        case BOOLEAN:
            ADD_CODE("bool@false");
            return;
        case NIL:
            ADD_CODE("nil@nil");
            return;
        case UNDEFINED:
            return;
    }
}

void generate_function_return_val(struct tToken var)
{
    ADD_INSTRUCTION("DEFVAR LF@%retval");
    ADD_CODE("MOVE LF@retval ");
    generate_defaul_value(var); ADD_CODE("\n");
}

void generate_function_return_val_assign(struct tToken var)
{
    ADD_CODE("MOVE LF@"); ADD_CODE(var.content_string->str); ADD_CODE(" TF@%retval\n");
}

void generete_value(struct tToken var)
{
    char string[20];
    switch (var.data_type_of_token) //TODO NEBYLY POKRYTY VSECHNY MOZNE SITUACE
    {
        case INT:
            sprintf(string, "%d", var.value.i);
            ADD_CODE("int@"); ADD_CODE(string); ADD_CODE("\n");
            return;
        case FLOAT:
            sprintf(string, "%g", var.value.d);
            ADD_CODE("float@"); ADD_CODE(string); ADD_CODE("\n");
            return;
        case STRING_DT:
            ADD_CODE("string@"); ADD_CODE(var.content_string->str); ADD_CODE("\n");
            return;
        case BOOLEAN:
            sprintf(string, "%g", var.value.boolean);   //TODO REMOVE
            ADD_CODE("bool@"); ADD_CODE(string); ADD_CODE("\n");
            return;
        case NIL:
            ADD_CODE("nil@nil");
            return;
    }
}

void generate_function_pass_par(struct tToken par, int index)
{
    ADD_CODE("DEFVAR TF%"); ADD_INTEGER(index); ADD_CODE("\n");

    ADD_CODE("MOVE TF@%"); ADD_INTEGER(index); ADD_CODE(" ");
    generete_value(par); ADD_CODE("\n");
}

void generate_function_ret(struct tToken function)
{
    ADD_INSTRUCTION("MOVE LF@%retval GF@%result");
    ADD_CODE("JUMP $"); ADD_CODE(function.content_string->str); ADD_CODE("%return\n");
}

void generate_var_decl(struct tToken var)
{
    ADD_CODE("DEFVAR LF@"); ADD_CODE(var.content_string->str); ADD_CODE("\n");
}

void generate_var_def_value(struct tToken var)
{
    ADD_CODE("MOVE LF@"); ADD_CODE(var.content_string->str); ADD_CODE(" ");
    generate_defaul_value(var); ADD_CODE("\n");
}

void generate_var_pass_value(struct tToken var)
{
    ADD_CODE("MOVE LF@%"); ADD_INTEGER(var.content_string->str); ADD_CODE(" "); //TODO prevest na int?
    generete_value(var); ADD_CODE("\n");
}
