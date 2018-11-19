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

int parsing(tDList token_list) {
    struct tToken token_actual;
    tDLElemPtr act;
    int err_code;
    if (token_list.First == NULL) {
        //TODO zkontrovlovat chybovou hlášku
        return 0;
    }
    act = token_list.First;

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
    if (token_list->Act->rptr == NULL) {
        //TODO zkontrovlovat chybovou hlášku
        return 2;
    }
    struct tToken token_actual = token_list->Act->rptr->token;

    if (token_actual.set_type_of_token != IDENTIFIER_NAME) {
        return 2;
    }

    if ((token_list->Act = token_list->Act->rptr) == NULL) {
        //TODO zkontrovlovat chybovou hlášku
        return 2;
    }

    token_actual = token_list->Act->token;

    if (token_actual.set_type_of_token != CHAR_LEFT_BRACKET) {
        return 2;
    }

    if ((token_list->Act = token_list->Act->rptr) == NULL) {
        //TODO zkontrovlovat chybovou hlášku
        return 2;
    }

    if((err_code=parse_def_arguments(&token_list))!=0){
        return err_code;
    }



}

int parse_def_arguments(tDList *token_list) {
    return 0;
}