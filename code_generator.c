/**
 * @brief Code generator
 * @file code_generator.c
 * @author Jan Osolsobe (xosols00)
 */

#include "code_generator.h"


Dynamic_string *gen_code_main; ///string to generated code
Dynamic_string *gen_code_function; ///string to generated code
bool is_main = false;

void generate_file_head() {
    ADD_COMMENT("Start of program\n");
    ADD_INSTRUCTION(".IFJcode18");


    ADD_INSTRUCTION("DEFVAR GF@%tmp1");
    ADD_INSTRUCTION("DEFVAR GF@%tmp2");

    ADD_INSTRUCTION("DEFVAR GF@%result");
    ADD_INSTRUCTION("MOVE GF@%result nil@nil");
    ADD_INSTRUCTION("DEFVAR GF@%return");

    ADD_INSTRUCTION("JUMP $$main");
}

void generate_build_in_functions() {
    ADD_INSTRUCTION(PRINT);

    ADD_INSTRUCTION(INPUTS);
    ADD_INSTRUCTION(INPUTI);
    ADD_INSTRUCTION(INPUTF);

    ADD_INSTRUCTION(LENGTH);
    ADD_INSTRUCTION(SUBSTR);
    ADD_INSTRUCTION(ORD);
    ADD_INSTRUCTION(CHR);
}

void generator_start() {

    gen_code_main = dynamic_string_init();
    gen_code_function = dynamic_string_init();


    generate_file_head();
    generate_build_in_functions();
}

void generator_clear() {
    dynamic_string_clear(gen_code_main);
    dynamic_string_clear(gen_code_function);

}

void write_code() {
    printf("%s", gen_code_function->str);
    printf("%s", gen_code_main->str);
}

void generate_main_start() {
    is_main = true;
    ADD_COMMENT("Start of main\n");

    ADD_INSTRUCTION("LABEL $$main");
    ADD_INSTRUCTION("CREATEFRAME");
    ADD_INSTRUCTION("PUSHFRAME");
}

void generate_main_end() {
    ADD_COMMENT("End of main\n");

    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("CLEARS");
    is_main = false;
}

void generate_function_call(struct tToken function) {
    ADD_COMMENT("Function call\n");

    ADD_CODE("CALL $");
    ADD_CODE(function.content_string->str);
    ADD_CODE("\n");

}

void generate_function_before_par() {
    ADD_INSTRUCTION("CREATEFRAME");
}

void generate_function_par_def(struct tToken param, int index) {
    ADD_CODE("DEFVAR LF@");
    ADD_CODE("%");
    ADD_CODE(param.content_string->str);
    ADD_CODE("\n");
    ADD_CODE("MOVE LF@");
    ADD_CODE("%");
    ADD_CODE(param.content_string->str);
    ADD_CODE("  LF@%");
    ADD_INTEGER(index);
    ADD_CODE("\n");
}

void generate_function_start(struct tToken function) {
    ADD_COMMENT("Start of function ");
    ADD_CODE(function.content_string->str);
    ADD_CODE("\n");

    ADD_CODE("LABEL $");
    ADD_CODE(function.content_string->str);
    ADD_CODE("\n");
    ADD_INSTRUCTION("PUSHFRAME");

}

void generate_function_end(struct tToken function) {
    ADD_COMMENT("End of function ");
    ADD_CODE(function.content_string->str);
    ADD_CODE("\n");

    ADD_CODE("LABEL $");
    ADD_CODE(function.content_string->str);
    ADD_CODE("%return\n");
    ADD_INSTRUCTION("POPFRAME");
    ADD_INSTRUCTION("RETURN");
}


void generate_function_return_val_assign(struct tToken var) {
    ADD_CODE("MOVE LF@%");
    ADD_CODE(var.content_string->str);
    ADD_CODE(" GF@%return\n");
}

void generate_value(struct tToken var) {
    char string[20];
    switch (var.data_type_of_token) {
        case INT:
            sprintf(string, "%d", var.value.i);
            ADD_CODE("int@");
            ADD_CODE(string);
            ADD_CODE("\n");
            return;
        case FLOAT:
            sprintf(string, "%a", var.value.f);
            ADD_CODE("float@");
            ADD_CODE(string);
            ADD_CODE("\n");
            return;
        case STRING_DT:
            ADD_CODE("string@");
            ADD_CODE(var.content_string->str);
            ADD_CODE("\n");
            return;

        case NIL:
            ADD_CODE("nil@nil");
            return;
        case UNDEFINED:
            if (var.set_type_of_token == IDENTIFIER_NAME) {
                ADD_CODE("LF@%");
                ADD_CODE(var.content_string->str);
            } else
                ErrorPrint(INTERNAL_ERROR,
                           "[code_generator.c][generate_value] datatype of token nepatri ani do jednoho statu");
break;
        default:

            ErrorPrint(INTERNAL_ERROR,
                       "[code_generator.c][generate_value] datatype of token nepatri ani do jednoho statu");
    }
}

void generate_function_pass_par(struct tToken par, int index) {
    ADD_CODE("DEFVAR TF@%");
    ADD_INTEGER(index);
    ADD_CODE("\n");

    ADD_CODE("MOVE TF@%");
    ADD_INTEGER(index);
    ADD_CODE(" ");
    generate_value(par);
    ADD_CODE("\n");
}

void generate_function_ret(struct tToken function) {
    ADD_INSTRUCTION("MOVE GF@%return GF@%result");
    ADD_CODE("JUMP $");
    ADD_CODE(function.content_string->str);
    ADD_CODE("%return\n");
}

void generate_var_decl(struct tToken var) {
    ADD_CODE("DEFVAR LF@%");
    ADD_CODE(var.content_string->str);
    ADD_CODE("\n");
}



void generate_label(int label_index) {
    ADD_CODE("LABEL $");
    ADD_CODE("%");
    ADD_INTEGER(label_index);

}

void generate_if_head() {
    ADD_INSTRUCTION("\n# If Then");
}

void generate_if_start(int label_index) {
    ADD_CODE("JUMPIFEQ $");
    ADD_CODE("%");
    ADD_INTEGER(label_index);
    ADD_CODE("_else GF@%result bool@false\n");
}


void generate_if_else_part(int label_index) {
    ADD_CODE("JUMP $");
    ADD_CODE("%");
    ADD_INTEGER(label_index);
    ADD_CODE("\n");

    ADD_INSTRUCTION("# Else");

    generate_label(label_index);
    ADD_CODE("_else \n");


}


void generate_if_end(int label_index) {
    ADD_INSTRUCTION("# End If");
    generate_label(label_index);
    ADD_CODE("\n");

}


void generate_while_head(int label_index) {
    ADD_INSTRUCTION("\n# Do While");

    generate_label(label_index);
    ADD_CODE("\n");

}


void generate_while_start(int label_index) {
    ADD_CODE("JUMPIFEQ $");
    ADD_CODE("%");
    ADD_INTEGER(label_index);
    ADD_CODE("_end");
    ADD_CODE(" GF@%result bool@false");
    ADD_CODE("\n");
}


void generate_while_end(int label_index) {
    ADD_CODE("JUMP $");
    ADD_CODE("%");
    ADD_INTEGER(label_index);
    ADD_CODE("\n");

    ADD_INSTRUCTION("# Loop");

    generate_label(label_index);
    ADD_CODE("_end\n");

}


void generate_push_var(struct tToken var) {
    ADD_CODE("PUSHS LF@%"); ADD_CODE(var.content_string->str); ADD_CODE("\n");

}
void generate_push_value(struct tToken var) {
    ADD_CODE("PUSHS ");
    generate_value(var);
    ADD_CODE("\n");
}

void generate_adds() {
    ADD_INSTRUCTION("ADDS");
}

void generate_subs() {
    ADD_INSTRUCTION("SUBS");
}

void generate_muls() {
    ADD_INSTRUCTION("MULS");
}

void generate_divs() {
    ADD_INSTRUCTION("DIVS");
}

void generate_idivs() {
    ADD_INSTRUCTION("POPS GF@%tmp1");
    ADD_INSTRUCTION("INT2FLOATS");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
    ADD_INSTRUCTION("INT2FLOATS");
    ADD_INSTRUCTION("DIVS");
    ADD_INSTRUCTION("FLOAT2INTS");
}


void generate_eqs() {
    ADD_INSTRUCTION("EQS");
}

void generate_gts() {
    ADD_INSTRUCTION("GTS");
}

void generate_lts() {
    ADD_INSTRUCTION("LTS");
}

void generate_not_eqs() {
    ADD_INSTRUCTION("EQS");
    ADD_INSTRUCTION("NOTS");
}

void generate_gt_eqs() {
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

void generate_ls_eqs() {
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

void generate_pops() {
    ADD_INSTRUCTION("POPS GF@%result");
}

void generate_save_result_to_var(struct tToken var) {
    ADD_CODE("MOVE LF@%");
    ADD_CODE(var.content_string->str);
    ADD_CODE(" GF@%result\n");
}

void generate_clear_stack() {
    ADD_INSTRUCTION("CLEARS");
}

void generate_stack_op1_to_double() {
    ADD_INSTRUCTION("INT2FLOATS");
}

void generate_stack_op2_to_double() {
    ADD_INSTRUCTION("POPS GF@%tmp1");
    ADD_INSTRUCTION("INT2FLOATS");
    ADD_INSTRUCTION("PUSHS GF@%tmp1");
}


void generate_concats() {
    ADD_INSTRUCTION("POPS GF@%tmp2");
    ADD_INSTRUCTION("POPS GF@%tmp1");
    ADD_INSTRUCTION("CONCAT GF@%result GF@%tmp1 GF@%tmp2");
    ADD_INSTRUCTION("PUSHS GF@%result");
}