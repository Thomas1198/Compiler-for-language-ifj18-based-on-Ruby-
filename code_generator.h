/**
 * @brief Code generator
 * @file code_generator.h
 * @author Jan Osolsobe (xosols00)
 */

#ifndef PROJECT_CODE_GENERATOR_H
#define PROJECT_CODE_GENERATOR_H

#include <stdio.h>
#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"

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
