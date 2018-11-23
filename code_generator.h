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

#define ADD_INTIGER(code) \
    do{ \
      char string[20]; \
      sprintf(string, "%d", code); \
      ADD_CODE(string); \
    } while (0) \


/// Build-functions

///Print/
//TODO: Build in functions

#define PRINT \
    "\n # Build-in function" \
    "\n LABEL $print" \
    "\n PUSHFRAME" \
    "\n WRITE LF@%1" \
	"\n POPFRAME" \
	"\n RETURN" \



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
void generate_function_call(char *name);
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
void generate_function_start(char *name);

/**
 * @brief Generate function end
 * @param name Name of function
 */
void generate_function_end(char *name);



#endif //PROJECT_CODE_GENERATOR_H
