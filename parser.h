#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.h"
#include "token_list.h"
#include "error.h"

int run_parser(FILE *source_code);

void first_run(tDList *token_list, FILE *source_code);

int parsing(tDList token_list);

int parse_def(tDList *token_list);

int parse_def_arguments(tDList *token_list);