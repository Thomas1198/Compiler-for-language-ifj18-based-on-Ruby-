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

    ADD_INSTRUCTION("DEFVAR GF@%tmp1");
    ADD_INSTRUCTION("DEFVAR GF@%tmp2");

    ADD_INSTRUCTION("DEFVAR GF@%result");

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

void generate_function_par_def(struct tToken param, int index)
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

void generate_function_return_val(struct tToken var)
{
    ADD_INSTRUCTION("DEFVAR LF@%retval");
    ADD_CODE("MOVE LF@retval ");
//    generate_defaul_value(var); ADD_CODE("\n");
}

void generate_function_return_val_assign(struct tToken var)
{
    ADD_CODE("MOVE LF@"); ADD_CODE(var.content_string->str); ADD_CODE(" TF@%retval\n");
}

void generate_value(struct tToken var)
{
    char string[20];
    switch (var.data_type_of_token)
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
        /*case BOOLEAN:
            sprintf(string, "%g", var.value.boolean);   //TODO REMOVE
            ADD_CODE("bool@"); ADD_CODE(string); ADD_CODE("\n");
            return;*/
        case NIL:
            ADD_CODE("nil@nil");
            return;
        case UNDEFINED:
            ErrorPrint(INTERNAL_ERROR, "[code_generator.c][generate_value] datatype of token nepatri ani do jednoho statu");
        default:
            ErrorPrint(INTERNAL_ERROR, "[code_generator.c][generate_value] datatype of token nepatri ani do jednoho statu");
    }
}

void generate_function_pass_par(struct tToken par, int index)
{
    ADD_CODE("DEFVAR TF%"); ADD_INTEGER(index); ADD_CODE("\n");

    ADD_CODE("MOVE TF@%"); ADD_INTEGER(index); ADD_CODE(" ");
    generate_value(par); ADD_CODE("\n");
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


void generate_var_pass_value(struct tToken var)
{
    ADD_CODE("MOVE LF@%"); ADD_CODE(var.content_string->str); ADD_CODE(" ");
    generate_value(var); ADD_CODE("\n");
}



void generate_label(struct tToken function, int label_index, int label_deep)
{
    ADD_CODE("LABEL $"); ADD_CODE(function.content_string->str); ADD_CODE("%"); ADD_INTEGER(label_deep);
    ADD_CODE("%"); ADD_INTEGER(label_index); ADD_CODE("\n");

}

void generate_if_head()
{
    ADD_INSTRUCTION("\n# If Then");
}

void generate_if_start(struct tToken function, int label_index, int label_deep)
{
    ADD_CODE("JUMPIFEQ $"); ADD_CODE(function.content_string->str); ADD_CODE("%"); ADD_INTEGER(label_deep);
    ADD_CODE("%"); ADD_INTEGER(label_index); ADD_CODE(" GF@%result bool@false\n");
}


void generate_if_else_part(struct tToken function, int label_index, int label_deep)
{
    ADD_CODE("JUMP $"); ADD_CODE(function.content_string->str); ADD_CODE("%"); ADD_INTEGER(label_deep);
    ADD_CODE("%"); ADD_INTEGER(label_index + 1); ADD_CODE("\n");

    ADD_INSTRUCTION("# Else");

    generate_label(function, label_index, label_deep);

}


void generate_if_end(struct tToken function, int label_index, int label_deep)
{
    ADD_INSTRUCTION("# End If");
    generate_label(function, label_index, label_deep);
}


void generate_while_head(struct tToken function, int label_index, int label_deep)
{
    ADD_INSTRUCTION("\n# Do While");

    generate_label(function, label_index, label_deep);
}


void generate_while_start(struct tToken function, int label_index, int label_deep)
{
    ADD_CODE("JUMPIFEQ $"); ADD_CODE(function.content_string->str); ADD_CODE("%"); ADD_INTEGER(label_deep);
    ADD_CODE("%"); ADD_INTEGER(label_index); ADD_CODE(" GF@%result bool@false"); ADD_CODE("\n");
}


void generate_while_end(struct tToken function, int label_index, int label_deep)
{
    ADD_CODE("JUMP $"); ADD_CODE(function.content_string->str); ADD_CODE("%"); ADD_INTEGER(label_deep);
    ADD_CODE("%"); ADD_INTEGER(label_index - 1); ADD_CODE("\n");

    ADD_INSTRUCTION("# Loop");

    generate_label(function, label_index, label_deep);
}


void generate_push_var(struct tToken var)
{
    ADD_CODE("PUSHS "); generate_value(var); ADD_CODE("\n");
}

void generate_push_value(struct tToken var)
{
   // ADD_CODE("PUSHS "); generate_value(var.value); ADD_CODE("\n");
}

void generate_adds()
{
    ADD_INSTRUCTION("ADDS");
}

void generate_subs()
{
    ADD_INSTRUCTION("SUBS");
}

void generate_muls()
{
    ADD_INSTRUCTION("MULS");
}

void generate_divs()
{
    ADD_INSTRUCTION("DIVS");
}
void generate_idivs()
{
    ADD_INSTRUCTION("IDIVS");
}


void generate_eqs()
{
    ADD_INSTRUCTION("EQS");
}

void generate_gts()
{
    ADD_INSTRUCTION("GTS");
}

void generate_lts()
{
    ADD_INSTRUCTION("LTS");
}

void generate_ands()
{
    ADD_INSTRUCTION("ANDS");
}

void generate_ors()
{
    ADD_INSTRUCTION("ORS");
}

void generate_not_eqs()
{
    ADD_INSTRUCTION("EQS")
    ADD_INSTRUCTION("NOTS");
}
void generate_gt_eqs()
{
    ADD_INSTRUCTION("POPS GF@%tmp1");
    ADD_INSTRUCTION("POPS GF@%tmp2");
    ADD_INSTRUCTION("PUSHS GF@%tmp2");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
    ADD_INSTRUCTION("GTS");
    ADD_INSTRUCTION("PUSHS GF@%tmp2");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
    ADD_INSTRUCTION("EQS");
    ADD_INSTRUCTION("ORS");
}

void generate_ls_eqs()
{
    ADD_INSTRUCTION("POPS GF@%tmp1");
    ADD_INSTRUCTION("POPS GF@%tmp2");
    ADD_INSTRUCTION("PUSHS GF@%tmp2");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
    ADD_INSTRUCTION("LTS");
    ADD_INSTRUCTION("PUSHS GF@%tmp2");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
    ADD_INSTRUCTION("EQS");
    ADD_INSTRUCTION("ORS");
}

void generate_pops()
{
    ADD_INSTRUCTION("POPS GF%result");
}

void  generate_save_result_to_var(struct tToken var)
{
    ADD_CODE("MOVE LF%"); ADD_CODE(var.content_string->str);
    ADD_CODE(" GF%result\n");
}

void generate_clear_stack()
{
    ADD_INSTRUCTION("CLEARS");
}

void generate_stack_op1_to_double()
{
    ADD_INSTRUCTION("INT2FLOATS");
}


void generate_stack_op1_to_integer()
{
    ADD_INSTRUCTION("FLOAT2R2EINTS");
}


void generate_stack_op2_to_double()
{
    ADD_INSTRUCTION("POPS GF@%tmp1");
    ADD_INSTRUCTION("INT2FLOATS");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
}


void generate_stack_op2_to_integer()
{
    ADD_INSTRUCTION("POPS GF@%tmp1");
    ADD_INSTRUCTION("FLOAT2R2EINTS");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
}