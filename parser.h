
#ifndef PROJECT_PARSER_H
#define PROJECT_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token_list.h"
#include "error.h"
#include "symtable.h"


#define check_set_type(token, set_type)do{\
    if ((token).set_type_of_token != (set_type)) {\
        return SYNTAX_ERROR;\
    }\
}while(false)

#define try_next_token_list_p(token_actual, token_list)do{\
    if (((token_list)->Act = token_list->Act->rptr) == NULL) {\
        return SYNTAX_ERROR;\
    }\
    (token_actual)=(token_list)->Act->token;\
}while(false)

int end_req;

int if_count;

Symtable hTable;

int run_parser(FILE *source_code);

void first_run(tDList *token_list, FILE *source_code);

int parsing(tDList token_list);

int parse_def(tDList *token_list);

int parse_def_arguments(tDList *token_list);

int check_end_of_line(tDList *token_list);

bool is_set_type(struct tToken token, set_type set_type1);

int parse_if(tDList *token_list);

int parse_end(tDList *token_list);

int parse_while(tDList *token_list);

int parse_condition(tDList *token_list);

int parse_identifier(tDList *token_list);

int parse_assign_value(tDList *token_list);

int parse_call_function(tDList *token_list);

#endif //PROJECT_PARSER_H