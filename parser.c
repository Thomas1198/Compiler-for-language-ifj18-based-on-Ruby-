#include "parser.h"

int run_parser(FILE *source_code) {
    tDList token_list;
    int error_code;

    first_run(&token_list, source_code);
    if ((error_code= parsing(token_list))!=0){
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
    if (token_list.First == NULL) {
        //TODO zkontrovlovat chybovou hlášku
        return 0;
    }
    act = token_list.First;
    do {
        token_actual = act->token;

        switch (token_actual.set_type_of_token) {
            case KEY_WORD_DEF: {
                if((act = act->rptr) == NULL){
                    return 2;
                }

                break;
            }
            case KEY_WORD_NIL: {
                break;
            }
            case KEY_WORD_FOLLOWED_BY_CONDITION: {
                break;
            }
            case KEY_WORD_FOLLOWED_BY_EOL: {
                break;
            }
            case LITERAL_NAME: {
                break;
            }
            case IDENTIFIER_NAME: {
                break;
            }
            case CHAR_EOL: {
                break;
            }
            case CHAR_LEFT_BRACKET: {
                break;
            }
            case CHAR_RIGTH_BRACKET: {
                break;
            }
            default: {
                return 2;
            }
        }
    } while ((act = act->rptr) != NULL);


    return 0;
}
