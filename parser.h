
#ifndef PROJECT_PARSER_H
#define PROJECT_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token_list.h"
#include "error.h"
#include "symtable.h"
#include "code_generator.h"
#include "expression.h"
#include "stack.h"


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

#define aritmetic_char  (is_set_type(token_actual,CHAR_OPERATOR_PLUS)||is_set_type(token_actual,CHAR_OPERATOR_DIV)||\
                        is_set_type(token_actual,CHAR_OPERATOR_MUL)||is_set_type(token_actual,CHAR_OPERATOR_MINUS))

#define reational_char (is_set_type(token_actual, CHAR_DOUBLEEQ) || is_set_type(token_actual, CHAR_LEQ) ||\
                        is_set_type(token_actual, CHAR_GEQ) || is_set_type(token_actual, CHAR_LT) ||\
                        is_set_type(token_actual, CHAR_GT) || is_set_type(token_actual, KEY_WORD_NOT))


int end_req;

int if_count;

struct tToken *act_fun;

Symtable hTable;

int run_parser(FILE *source_code);

int first_run(tDList *token_list, FILE *source_code);

int parsing(tDList token_list);

int parse_def(tDList *token_list);

int parse_def_arguments_with_bracket(tDList *token_list);

int check_end_of_line(tDList *token_list);

bool is_set_type(struct tToken token, set_type set_type1);

int parse_if(tDList *token_list);

int parse_end(tDList *token_list);

int parse_while(tDList *token_list);

int parse_condition(tDList *token_list, int set);

int parse_identifier(tDList *token_list);

int parse_assign_value(tDList *token_list);

int parse_call_function(tDList *token_list, int count);

int parse_condition_expr(tDList *token_list, int set);


#endif //PROJECT_PARSER_H