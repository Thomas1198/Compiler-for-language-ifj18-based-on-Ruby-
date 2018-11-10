/**
 * @file error.h
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */

#include <stdio.h>

enum ERROR_CODE {
    SCANNER_ERROR = 1,         // Lexical analysis error
    SYNTAX_ERROR = 2,          // Syntax analysis error
    PROG_SEM_ERROR = 3,        // Semantic error in program (undefined function/variable,...)
    TYPE_SEM_ERROR = 4,        // Semantic error of type compatibility in arithmetic, string, relational,... expressions
    OTHER_SEM_ERROR = 6,       // Other semantic errors
    ZERO_DIVISION_ERROR = 9;   // Runtime error zero division
    INTERNAL_ERROR = 99        // Internal errors (opening files, allocating memory,...)
};

/**
 * @brief Prints error message with coresponding error code.
 * @param err_code error number
 * @param msg Message to print.
 * @param ... Any other parameter (adding functionality of printf)
 */
void ErrorPrint(enum ERROR_CODES err_code, const char *msg);
