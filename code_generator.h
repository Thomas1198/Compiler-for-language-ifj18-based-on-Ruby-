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
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>


#define ADD_INSTRUCTION(instruction) \
    dynamic_string_add_const_str(gen_code, (instruction "\n"))

#define ADD_COMMENT(comment) \
    dynamic_string_add_const_str(gen_code, ( "# "comment ))

#define ADD_CODE(code) \
    dynamic_string_add_const_str(gen_code, (code))

#define ADD_INTEGER(code) \
    do{ \
      char string[20]; \
      sprintf(string, "%d", code); \
      ADD_CODE(string); \
    } while (0)


/// Build-functions

///Print/

#define PRINT \
    "\n # Build-in function" \
    "\n # PRINT" \
    "\n LABEL $print" \
    "\n PUSHFRAME" \
    "\n WRITE LF@%1" \
	"\n POPFRAME" \
	"\n RETURN"

#define INPUTS \
    "\n #INPUTS" \
    "\n LABEL $inputs" \
    "\n PUSHFRAME" \
    "\n READ LF@input string" \
	"\n POPFRAME" \
	"\n RETURN"

#define INPUTI \
    "\n #INPUTS" \
    "\n LABEL $inputi" \
    "\n PUSHFRAME" \
    "\n READ LF@input int" \
	"\n POPFRAME" \
	"\n RETURN"

#define INPUTF \
    "\n #INPUTS" \
    "\n LABEL $inputf" \
    "\n PUSHFRAME" \
    "\n READ LF@input float" \
	"\n POPFRAME" \
	"\n RETURN"

#define LENGTH \
    "\n #LENGTH" \
    "\n LABEL $length" \
    "\n PUSHFRAME" \
    "\n DEFVAR LF@%return" \
    "\n STRLEN LF@%return LF@%0" \
	"\n POPFRAME" \
	"\n RETURN"

#define SUBSTR \
    "\n #SUBSTR" \
    "\n LABEL $substr"  // TODO do it ! DO NOT PROCRASTINATE !

#define ASC \
    "\n #ASC" \
    "\n LABEL $asc"  // TODO do it ! DO NOT PROCRASTINATE !

#define CHR \
    "\n #CHR" \
    "\n LABEL $chr"  // TODO do it ! DO NOT PROCRASTINATE !



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
 */
void generate_function_before_par();

/**
 * @brief Generate pass parameter
 * @param param token with parametr
 * @param index index
 */
void generate_function_par( struct tToken param, int index);

/**
 * @brief Generate function start
 * @param name Name of function
 */
void generate_function_start(struct tToken function);

/**
 * @brief Generate function end
 */
void generate_function_end();



#endif //PROJECT_CODE_GENERATOR_H
