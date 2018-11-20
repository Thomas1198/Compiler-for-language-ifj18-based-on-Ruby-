#include "parser.h"

int run_parser(FILE *source_code) {
    tDList token_list;
    int error_code;

    first_run(&token_list, source_code);
    if ((error_code = parsing(token_list)) != 0) {
        //TODO uvolneni pameti
        exit(error_code);
    }

}

void first_run(tDList *token_list, FILE *source_code) {
    struct tToken token_actual;

    while ((token_actual = get_token(source_code)).content_string != NULL) {
        DLInsertLast(&token_list, token_actual);
    }
}

inline bool is_set_type(struct tToken token, set_type set_type1) {
    return (token.set_type_of_token == set_type1);
}


int parsing(tDList token_list) {
    struct tToken token_actual;
    tDLElemPtr act;
    int err_code;
    if (token_list.First == NULL) {
        //TODO zkontrovlovat chybovou hlášku
        return 0;
    }
    act = token_list.First;
    DLFirst(&token_list);

    /*typedef enum {
	CHAR_OPERATOR_PLUS,
	CHAR_OPERATOR_MINUS,
	CHAR_OPERATOR_DIV,
	CHAR_OPERATOR_MUL,
	CHAR_ASSIGN,
    CHAR_EOL,
    IDENTIFIER_NAME,
    CHAR_LEFT_BRACKET,
    CHAR_RIGTH_BRACKET,
    LITERAL_NAME,
    KEY_WORD_FOLLOWED_BY_EOL,
    KEY_WORD_FOLLOWED_BY_CONDITION,
    KEY_WORD_NIL,
    KEY_WORD_DEF,
	EMPTY,
	CHAR_SEMICOLON,
	CHAR_COMMA,
	CHAR_EOF,


}set_type;*/

    do {
        token_actual = act->token;

        switch (token_actual.set_type_of_token) {
            case CHAR_EOL: {
                break;
            }
            case KEY_WORD_DEF: {

                if ((err_code = parse_def(&act)) != 0) {
                    return err_code;
                }

                break;
            }
            case KEY_WORD_FOLLOWED_BY_CONDITION: {


                break;
            }
            case IDENTIFIER_NAME: {


                break;
            }
            case LITERAL_NAME: {


                break;
            }
            default: {
                return 2;
            }
        }

    } while ((act = act->rptr) != NULL);


    return 0;
}


int parse_def(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    try_next_token_list(token_actual,token_list);

    check_set_type(token_actual, IDENTIFIER_NAME);

    try_next_token_list(token_actual,token_list);

    check_set_type(token_actual, CHAR_LEFT_BRACKET);

    if((err_code=parse_def_arguments(&token_list))!=0){
        return err_code;
    }

    return check_end_of_line(&token_list) ;
}

int parse_def_arguments(tDList *token_list) {
    struct tToken token_actual;
    bool comma=false;

    while (true) {
        try_next_token_list(token_actual,token_list);

        if(comma){
            return 2;
        }

        if (is_set_type(token_actual, CHAR_RIGHT_BRACKET)) {
            return 0;
        }

        comma=false;

        if (is_set_type(token_actual, IDENTIFIER_NAME)) {
            try_next_token_list(token_actual,token_list);

            if (is_set_type(token_actual, CHAR_COMMA)) {
                comma=true;
                continue;
            } else{
                continue;
            }
        } else {
            return 2;
        }

    }
}

int check_end_of_line(tDList *token_list){
    struct tToken token_actual;
    try_next_token_list(token_actual,token_list);

    if (is_set_type(token_actual, SEMICOLON)) {
         if(is_set_type(token_actual, EOL)){
             return 0;
         } else{
             return 2;
         }
    } else if(is_set_type(token_actual, EOL)) {
        return 0;
    } else{
        return 2;
    }
}