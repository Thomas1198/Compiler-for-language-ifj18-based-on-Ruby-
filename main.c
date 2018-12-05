/**
 * @file main.c
 * @author Tomas Dorda (xdorda00), Jakub Konetzny, Jan Osolsobe, Patrik Strnad
 */

#include <stdio.h>

#include "parser.h"
#include "error.h"

int main(int argc, char *argv[]) {

    FILE *source_code;

    if (argc == 1) {
        source_code = stdin;
    } else if (argc == 2) {
        if ((source_code = fopen(argv[1], "r")) == NULL) {
            return INTERNAL_ERROR;
        }
    } else {
        return INTERNAL_ERROR;
    }

    run_parser(source_code);

    fclose(source_code);

    return 0;
}