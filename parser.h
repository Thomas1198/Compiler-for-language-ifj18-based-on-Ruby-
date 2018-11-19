#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.h"
#include "token_list.h"
#include "error.h"


#define check_set_type(token,set_type)do{\
    if (token.set_type_of_token != IDENTIFIER_NAME) {\
        return 2;\
    }\
}while(false)\

#define try_next_token_list(token_actual,token_list)do{\
    if ((token_list->Act = token_list->Act->rptr) == NULL) {\
        return 2;\
    }\
    token_actual=token_list->Act->token;\
}while(false)\

int run_parser(FILE *source_code);

void first_run(tDList *token_list, FILE *source_code);

int parsing(tDList token_list);

int parse_def(tDList *token_list);

int parse_def_arguments(tDList *token_list);

int check_end_of_line(tDList *token_list);

inline bool is_set_type(struct tToken token, set_type set_type1);