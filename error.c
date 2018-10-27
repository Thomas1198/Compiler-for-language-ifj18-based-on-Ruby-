/**
 * @file error.c
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */

 #include "error.h"

 void printError(enum ERROR_CODES err_code, const char *msg, ...) {
 	fprintf(stderr, "ERROR (%d): ", err_code);
 	va_list arguments;
 	va_start(arguments, msg);
 	vfprintf(stderr, msg, arguments);
 	va_end(arguments);
 	fprintf(stderr, "\n");
 }
