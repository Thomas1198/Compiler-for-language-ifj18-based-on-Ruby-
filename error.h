/**
 * @file error.h
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */
#ifndef PROJECT_ERROR_H
#define PROJECT_ERROR_H

#include <stdio.h>
#include <stdlib.h>


enum ERROR_CODE {
    SCANNER_ERROR = 1,         // chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
    SYNTAX_ERROR = 2,          // chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu
    PROG_SEM_ERROR = 3,        // sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné, atp.
    TYPE_SEM_ERROR = 4,        // sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
    FUNCTION_ERROE = 5,        // sémantická chyba v programu – špatný počet parametrů u volání funkce
    OTHER_SEM_ERROR = 6,       // ostatní sémantické chyby
    ZERO_DIVISION_ERROR = 9,   // běhová chyba dělení nulou
    INTERNAL_ERROR = 99        // interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.)
};

/**
 * @brief Prints error message with error code.
 * @param err_code error number
 * @param msg Message to print.
 */
void ErrorPrint(enum ERROR_CODE err_code, const char *msg);


#endif //PROJECT_ERROR_H
