/**
 * @file error.c
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */

 #include "error.h"

 void ErrorPrint(enum ERROR_CODES err_code, const char *msg) {
 	fprintf(stderr, "ERROR (%d): %s", err_code, msg);
 }
