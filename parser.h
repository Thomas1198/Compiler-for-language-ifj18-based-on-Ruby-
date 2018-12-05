/**
 * @file    parser.h
 * @author  Jakub Konetzný(xkonet01)
 * @brief   parser
 */

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
                        is_set_type(token_actual, CHAR_GT) || is_set_type(token_actual, CHAR_NEQ))


int end_req, lable,if_count;

bool func,b_else;

struct tToken *act_fun, *inputs, *inputi, *inputf, *print,*length,*subsrt,*ord,*chr,*end;

Symtable hTable;

tDList token_list;

struct tStack *lables_stack;

/**
 * @brief   function to run parsing
 *
 * @param   source code
 * @return  error value
 */
int run_parser(FILE *source_code);


/**
 * @brief   adding tokens to list plus adding identificators to table
 *
 * @param   list of tokens
 * @param   source code
 * @return  error value
 */
int tokens_to_list(tDList *token_list, FILE *source_code);

/**
 * @brief   parsing whole wist line by line
 *
 * @param   list of tokens
 * @return  error value
 */
int parsing(tDList token_list);


/**
 * @brief   parsing deklaration of function
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_def(tDList *token_list);


/**
 * @brief   parsing arguments of function
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_def_arguments(tDList *token_list);


/**
 * @brief   parsing end of line
 *
 * @param   list of tokens
 * @return  error value
 */
int check_end_of_line(tDList *token_list);


/**
 * @brief   compare set type of token with set type
 *
 * @param   token
 * @param   set type of token
 * @return  bool
 */
bool is_set_type(struct tToken token, set_type set_type1);


/**
 * @brief   parsing line begining with if
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_if(tDList *token_list);


/**
 * @brief   parsing line begining with end
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_end(tDList *token_list);


/**
 * @brief   parsing line begining with while
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_while(tDList *token_list);


/**
 * @brief   parsing condition
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_condition(tDList *token_list, int set);

/**
 * @brief   parsing line begining with identifire(variable or function)
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_identifier(tDList *token_list);


/**
 * @brief   parsing line begining with else
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_assign_value(tDList *token_list);


/**
 * @brief   parsing function call
 *
 * @param   list of tokens
 * @param   count of parametres
 * @return  error value
 */
int parse_call_function(tDList *token_list, int count);


/**
 * @brief   parsing line begining with else
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_condition_expr(tDList *token_list, int set, tDList *tmp_list);


/**
 * @brief   parsing constant value assignment
 *
 * @param   list of tokens
 * @return  error value.
 */
int assign_value_const(tDList *token_list);

/**
 * @brief   initialize build in function
 */
void init_build_in();


/**
 * @brief   delete build in function
 */
void free_build_in();


/**
 * @brief   parsing line begining with else
 *
 * @param   list of tokens
 * @return  error value
 */
int parse_else(tDList *token_list);

#endif //PROJECT_PARSER_H