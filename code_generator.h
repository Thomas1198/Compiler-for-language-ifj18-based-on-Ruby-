/**
 * @brief Code generator
 * @file code_generator.h
 * @author Jan Osolsobe (xosols00)
 */

#ifndef PROJECT_CODE_GENERATOR_H
#define PROJECT_CODE_GENERATOR_H

#include "dynamic_string.h"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

#define ADD_INSTRUCTION(instruction) \
    if (!dynamic_string_add_const_str(&gen_code, (instruction "\n"))) return false

#define ADD_COMMENT(comment) \
    if (!dynamic_string_add_const_str(&gen_code, ( "# "comment "\n"))) return false

#define ADD_CODE(code) \
    if (!dynamic_string_add_const_str(&gen_code, (code))) return false

#define ADD_INTIGER(code) \
    do{ \
      char string[20]; \
      sprintf(string, "%d", code); \
      ADD_CODE(string); \
    } while (0) \


/**
 * @brief Generate file header
 * @return True if its OK
 */
bool generate_file_head();

/**
 * @brief Inicialization  of generator
 * @return True if its OK
 */
bool generator_start();

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
 * @return True if its OK
 */
bool generate_main_start();

/**
 * @brief Generate end of main function
 * @return True if its OK
 */
bool generate_main_end();







#endif //PROJECT_CODE_GENERATOR_H
