/**
 * @brief Code generator
 * @file code_generator.h
 * @author Jan Osolsobe (xosols00)
 */

#ifndef PROJECT_CODE_GENERATOR_H
#define PROJECT_CODE_GENERATOR_H

#include "dynamic_string.h"
#include "token.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

#define ADD_INSTRUCTION(instruction) do{\
    if(is_main)\
    {\
        dynamic_string_add_const_str(gen_code_main, (instruction "\n"));\
    }\
    else\
    {\
        dynamic_string_add_const_str(gen_code_function, (instruction"\n"));\
    } \
    }while(false)

#define ADD_COMMENT(comment) do{\
    if(is_main)\
    {\
        dynamic_string_add_const_str(gen_code_main, ("# "comment));\
    }\
    else\
    {\
        dynamic_string_add_const_str(gen_code_function, ("# "comment));\
    } \
    }while(false)

#define ADD_CODE(code) do{\
    if(is_main)\
    {\
        dynamic_string_add_const_str(gen_code_main, (code));\
    }\
    else\
    {\
        dynamic_string_add_const_str(gen_code_function, (code));\
    } \
    }while(false)

#define ADD_INTEGER(code) \
    do{ \
      char string[20]; \
      sprintf(string, "%d", code); \
      ADD_CODE(string); \
    } while (0)


/// Build-functions

///Print
#define PRINT \
    "\n # Build-in function" \
    "\n # PRINT" \
    "\n LABEL $print" \
    "\n PUSHFRAME" \
    "\n WRITE LF@%0" \
    "\n MOVE GF@%return nil@nil" \
    "\n POPFRAME" \
    "\n RETURN"

/// Input
#define INPUTS \
    "\n #INPUTS" \
    "\n LABEL $inputs" \
    "\n READ GF@%return string" \
    "\n RETURN"

#define INPUTI \
    "\n #INPUTI" \
    "\n LABEL $inputi" \
    "\n READ GF@%return int" \
    "\n RETURN"

#define INPUTF \
    "\n #INPUTF" \
    "\n LABEL $inputf" \
    "\n READ GF@%return float" \
    "\n RETURN"

///Length
#define LENGTH \
    "\n #LENGTH" \
    "\n LABEL $length" \
    "\n PUSHFRAME" \
    "\n STRLEN GF@%return LF@%0" \
    "\n POPFRAME" \
    "\n RETURN"

/// Substring
#define SUBSTR \
    "\n #SUBSTR" \
    "\n LABEL $substr" \
    "\n PUSHFRAME" \
    "\n MOVE GF@%return string@" \
    "\n DEFVAR LF@length_str" \
    "\n CREATEFRAME" \
    "\n DEFVAR TF@%0" \
    "\n MOVE TF@%0 LF@%0" \
    "\n CALL $length" \
    "\n MOVE LF@length_str TF@%retval" \
    "\n DEFVAR LF@ret_cond"    \
    "\n LT LF@ret_cond LF@length_str int@0" \
    "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
    "\n EQ LF@ret_cond LF@length_str int@0" \
    "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
    "\n LT LF@ret_cond LF@%1 int@0" \
    "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
    "\n EQ LF@ret_cond LF@%1 int@0" \
    "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
    "\n GT LF@ret_cond LF@%1 LF@length_str"    \
    "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
    "\n EQ LF@ret_cond LF@%2 int@0"    \
    "\n JUMPIFEQ $substr$return LF@ret_cond bool@true" \
    "\n DEFVAR LF@max_n" \
    "\n MOVE LF@max_n LF@length_str" \
    "\n SUB LF@max_n LF@max_n LF@%1" \
    "\n ADD LF@max_n LF@max_n int@1" \
    "\n DEFVAR LF@edit_n_cond" \
    "\n LT LF@edit_n_cond LF@%2 int@0" \
    "\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" \
    "\n GT LF@edit_n_cond LF@%2 LF@max_n" \
    "\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true" \
    "\n JUMP $substr$process" \
    "\n LABEL $substr$edit_n" \
    "\n MOVE LF@%2 LF@max_n" \
    "\n LABEL $substr$process" \
    "\n DEFVAR LF@index" \
    "\n MOVE LF@index LF@%1"    \
    "\n SUB LF@index LF@index int@1" \
    "\n DEFVAR LF@char"    \
    "\n DEFVAR LF@process_loop_cond" \
    "\n LABEL $substr$process_loop"    \
    "\n GETCHAR LF@char LF@%0 LF@index" \
    "\n CONCAT GF@%return GF@%return LF@char" \
    "\n ADD LF@index LF@index int@1" \
    "\n SUB LF@%2 LF@%2 int@1" \
    "\n GT LF@process_loop_cond LF@%2 int@0" \
    "\n JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true" \
    "\n LABEL $substr$return" \
    "\n POPFRAME" \
    "\n RETURN"

///Ord
#define ORD \
    "\n #ORD" \
    "\n LABEL $ord" \
    "\n PUSHFRAME" \
    "\n MOVE GF@%return int@0" \
    "\n DEFVAR LF@cond_length" \
    "\n LT LF@cond_length LF@%1 int@1" \
    "\n JUMPIFEQ $ord$return LF@cond_length bool@true" \
    "\n DEFVAR LF@length_str" \
    "\n CREATEFRAME" \
    "\n DEFVAR TF@%0" \
    "\n MOVE TF@%0 LF@%0" \
    "\n CALL $length" \
    "\n MOVE LF@length_str GF@%return" \
    "\n GT LF@cond_length LF@%1 LF@length_str" \
    "\n JUMPIFEQ $ord$return LF@cond_length bool@true" \
    "\n SUB LF@%1 LF@%1 int@1" \
    "\n STRI2INT GF@%return LF@%0 LF@%1" \
    "\n LABEL $ord$return" \
    "\n POPFRAME" \
    "\n RETURN"

/// Chr
#define CHR \
    "\n #CHR" \
    "\n LABEL $chr" \
    "\n PUSHFRAME" \
    "\n DEFVAR GF@%return" \
    "\n MOVE GF@%return string@" \
    "\n LT LF@cond_range LF@%0 int@0" \
    "\n JUMPIFEQ $chr$return LF@cond_range bool@true" \
    "\n GT LF@cond_range LF@%0 int@255" \
    "\n JUMPIFEQ $chr$return LF@cond_range bool@true" \
    "\n INT2CHAR GF@%return LF@%0" \
    "\n LABEL $chr$return" \
    "\n POPFRAME" \
    "\n RETURN" \
    "\n"


/**
 * @brief Generate file header
 */
void generate_file_head();

/**
 * @brief Generate build in functions
 */
void generate_build_in_functions();

/**
 * @brief Inicialization  of generator
 */

void generator_start();

/**
 * @brief Clear resources
 */
void generator_clear();

/**
 * @brief Write final generated code to stdout
 */
void write_code();

/**
 * @brief Generate start of main function
 */
void generate_main_start();

/**
 * @brief Generate end of main function
 */
void generate_main_end();

/**
 * @brief Generate function call
 * @param name Name of function
 */
void generate_function_call(struct tToken function);

/**
 * @brief Generate frame to load parameters
 * Vlozeni ramce na pred vlozenim parametru parametry
 */
void generate_function_before_par();

/**
 * @brief Generate pass parameter
 * @param param token with parametr
 * @param index index
 * Vlozeni do funkce, po function_start, pokud ma parametry
 */
void generate_function_par_def(struct tToken param, int index);

/**
 * @brief Generate function start
 * @param name Name of function
 *
 */
void generate_function_start(struct tToken function);

/**
 * @brief Generate function end
 * @param function Token with function
 */
void generate_function_end(struct tToken function);


/**
 * @brief Generate defaulte value assign
 * @param var tToken with variable
 * Plní navratovou hodnotu
 */
void generate_function_return_val_assign(struct tToken var);

/**
 * @brief Generate value for variable
 * @param var tToken with variable
 * Nepouživej!
 */
void generate_value(struct tToken var);

/**
 * @brief Generate function pass param
 * @param var tToken with variable
 * @param index index of parameter
 *
 */
void generate_function_pass_par(struct tToken par, int index);

/**
 * @brief Generate variable declaretion
 * @param var tToken with variable
 * Vlozeni parametru pred volanim funkce
 */

void generate_var_decl(struct tToken var);

/**
 * @brief generate label
 * @param function tToken with function
 * @param label_index index of label
 * @param label_deep deep of label
 * Nepouzivej
 */

void generate_label(struct tToken function, int label_index, int label_deep);

/**
 * @brief generate if head
 */
void generate_if_head();

/**
 * @brief generate if start
 * @param function tToken with function
 * @param label_index index of label
 * @param label_deep deep of label
 */
void generate_if_start(struct tToken function, int label_index, int label_deep);

/**
 * @brief generate if else
 * @param function tToken with function
 * @param label_index index of label
 * @param label_deep deep of label
 */
void generate_if_else_part(struct tToken function, int label_index, int label_deep);

/**
 * @brief generate if end
 * @param function tToken with function
 * @param label_index index of label
 * @param label_deep deep of label
 */
void generate_if_end(struct tToken function, int label_index, int label_deep);

/**
 * @brief generate while end
 * @param function tToken with function
 * @param label_index index of label
 * @param label_deep deep of label
 */
void generate_while_head(struct tToken function, int label_index, int label_deep);

/**
 * @brief generate while start
 * @param function tToken with function
 * @param label_index index of label
 * @param label_deep deep of label
 */
void generate_while_start(struct tToken function, int label_index, int label_deep);

/**
 * @brief generate while end
 * @param function tToken with function
 * @param label_index index of label
 * @param label_deep deep of label
 */
void generate_while_end(struct tToken function, int label_index, int label_deep);


/**
 * @brief Generate push variable on stack
 * @param var tToken with variable
 * vloz na zasobnik hodnotu promene
 */
void generate_push_var(struct tToken var);

/**
 * @brief Generate push value on stack
 * @param var tToken with variable
 * vloz na zasobnik primou hodnotu
 */
void generate_push_value(struct tToken var);

/**
 * @brief add operands in stack
 */
void generate_adds();

/**
 * @brief sub operands in stack
 */
void generate_subs();

/**
 * @brief mul operands in stack
 */
void generate_muls();

/**
 * @brief div operands in stack
 */
void generate_divs();

/**
 * @brief idiv operands in stack
 */
void generate_idivs();

/**
 * @brief eq operands in stack
 */
void generate_eqs();

/**
 * @brief gt operands in stack
 */
void generate_gts();

/**
 * @brief lt operands in stack
 */
void generate_lts();

/**
 * @brief not equal in stack
 */
void generate_not_eqs();

/**
 * @brief equal or greater operands in stack
 */
void generate_gt_eqs();

/**
 * @brief equal or less operands in stack
 */
void generate_ls_eqs();

/**
 * @brief pop stack to global frame
 * ulozi konecnou promenou do globalni funkce
 */
void generate_pops();

/**
 * @brief Save result to variable
 * @param var Token with variable
 * z globalni funkce do promene
 */
void generate_save_result_to_var(struct tToken var);

/**
 * @brief clear stack
 * vymaze stack, volat vzdy pred prvnim vlozenim do stacku pri vypoctu
 */
void generate_clear_stack();




/// Funkce pro prevod mezi dat.typama v stacku
/**
 * @brief convert first operand to double
 */
void generate_stack_op1_to_double();

/**
 * @brief convert second operand to intiger
 */
void generate_stack_op1_to_integer();

/**
 * @brief convert second operand to double
 */
void generate_stack_op2_to_double();

/**
 * @brief convert second operand to intiger
 */
void generate_stack_op2_to_integer();

void generate_concats();

void generate_function_ret(struct tToken function);


#endif //PROJECT_CODE_GENERATOR_H
