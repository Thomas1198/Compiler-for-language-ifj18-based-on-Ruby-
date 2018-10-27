/**
 * @file error.h
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */
#ifndef IFJ_ERROR_CODES_H
#define IFJ_ERROR_CODES_H

#include <stdarg.h>
#include <stdio.h>

enum ERROR_CODES {

  //TODO
	// ERROR_SCANNER = 1, // Lexical analysis error
	// ERROR_SYNTAX = 2, // Syntax error
	// ERROR_PROG_SEM = 3, // Semantic error in program (undefined function/variable,...)
	// ERROR_TYPE_SEM = 4, // Semantic error of type compatibility in arithmetic, string, relational,... expressions
	// ERROR_OTHER_SEM = 6, // Other semantic errors */
	// ERROR_INTERNAL = 99 // Internal errors (opening files, allocating memory,...)

};

/**
 * @brief Prints error message with coresponding error code.
 * @param err_code error number
 * @param msg Message to print.
 * @param ... Any other parameter (adding functionality of printf)
 */
void printError(enum ERROR_CODES err_code, const char *msg, ...);
