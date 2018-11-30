#include "parser.h"

int run_parser(FILE *source_code) {
    tDList token_list;
    int error_code;

    DLInitList(&token_list);

    symtable_create(&hTable);

    first_run(&token_list, source_code);


    if ((error_code = parsing(token_list)) != 0) {
        //TODO uvolneni pameti
        exit(error_code);
    }


    DLDisposeList(&token_list);
    return 0;
}

void first_run(tDList *token_list, FILE *source_code) {
    struct tToken token_actual;


    while (((token_actual = get_token(source_code))).set_type_of_token != CHAR_EOF) {
        DLInsertLast(&(*token_list), token_actual);

        //TODO zastínění proměné
        if (is_set_type(token_actual, IDENTIFIER_NAME)) {

            if (symtable_get(&hTable, token_actual.content_string) == NULL) {
                symtable_insert(&hTable, &token_list->Last->token);
            }
        }
    }
}

bool is_set_type(struct tToken token, set_type set_type1) {
    return (token.set_type_of_token == set_type1);
}


int parsing(tDList token_list) {
    struct tToken token_actual;
    int err_code;
    if (token_list.First == NULL) {
        //TODO zkontrovlovat chybovou hlášku
        return 0;
    }
    DLFirst(&token_list);


    do {
        token_actual = token_list.Act->token;

        switch (token_actual.set_type_of_token) {
            case CHAR_EOL: {
                break;
            }
            case KEY_WORD_DEF: {

                if ((err_code = parse_def(&token_list)) != 0) {
                    return err_code;
                }

                break;
            }

            case KEY_WORD_IF: {

                if ((err_code = parse_if(&token_list)) != 0) {
                    return err_code;
                }
                break;
            }

            case KEY_WORD_WHILE: {

                if ((err_code = parse_while(&token_list)) != 0) {
                    return err_code;
                }
                break;
            }

            case KEY_WORD_END: {

                if ((err_code = parse_end(&token_list)) != 0) {
                    return err_code;
                }

                break;
            }

            case IDENTIFIER_NAME: {

                if ((err_code = parse_identifier(&token_list)) != 0) {
                    return err_code;
                }

                break;
            }

            default: {
                return SYNTAX_ERROR;
            }
        }

    } while ((token_list.Act = token_list.Act->rptr) != NULL);

    if (end_req != 0) {
        return SYNTAX_ERROR;
    }

    return 0;
}

int parse_end(tDList *token_list) {
    end_req--;

    if (end_req < 0) {
        return SYNTAX_ERROR;
    }


    return check_end_of_line(&(*token_list));
}

int parse_def(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    end_req++;

    try_next_token_list_p(token_actual, token_list);

    check_set_type(token_actual, IDENTIFIER_NAME);


    try_next_token_list_p(token_actual, token_list);

    check_set_type(token_actual, CHAR_LEFT_BRACKET);

    if ((err_code = parse_def_arguments_with_bracket(&(*token_list))) != 0) {
        return err_code;
    }


    return check_end_of_line(&*(token_list));
}

int parse_def_arguments_with_bracket(tDList *token_list) {
    struct tToken token_actual;
    bool comma = false;

    while (true) {
        try_next_token_list_p(token_actual, token_list);

        if (comma && !is_set_type(token_actual, IDENTIFIER_NAME)) {
            return SYNTAX_ERROR;
        }

        if (is_set_type(token_actual, CHAR_RIGHT_BRACKET)) {
            return 0;
        }

        if (is_set_type(token_actual, IDENTIFIER_NAME)) {
            comma = false;
            continue;
        }
        if (is_set_type(token_actual, CHAR_COMMA)) {
            comma = true;
            continue;
        } else {
            return SYNTAX_ERROR;
        }
    }
}


int check_end_of_line(tDList *token_list) {
    struct tToken token_actual;
    try_next_token_list_p(token_actual, token_list);

    if (is_set_type(token_actual, CHAR_SEMICOLON)) {
        if (is_set_type(token_actual, CHAR_EOL)) {
            return 0;
        } else {
            return SYNTAX_ERROR;
        }
    } else if (is_set_type(token_actual, CHAR_EOL)) {
        return 0;
    } else {
        return SYNTAX_ERROR;
    }

}

int parse_if(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    end_req++;


    if ((err_code = parse_condition(token_list)) != 0) {
        return err_code;
    }

    try_next_token_list_p(token_actual, token_list);

    if (!is_set_type(token_actual, KEY_WORD_THEN)) {
        return SYNTAX_ERROR;
    }
    return check_end_of_line(&(*token_list));
}

int parse_while(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    end_req++;


    if ((err_code = parse_condition(token_list)) != 0) {
        return err_code;
    }

    try_next_token_list_p(token_actual, token_list);

    if (!is_set_type(token_actual, KEY_WORD_DO)) {
        return SYNTAX_ERROR;
    }
    return check_end_of_line(&(*token_list));

}

int parse_condition(tDList *token_list) {

    int err_code;
    struct tToken token_actual;

    try_next_token_list_p(token_actual, token_list);

    if (!(is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, LITERAL_NAME) ||
          is_set_type(token_actual, LITERAL_STRING))) {
        return SYNTAX_ERROR;
    }

    try_next_token_list_p(token_actual, token_list);

    if (!(is_set_type(token_actual, CHAR_DOUBLEEQ) || is_set_type(token_actual, CHAR_LEQ) ||
          is_set_type(token_actual, CHAR_GEQ) || is_set_type(token_actual, CHAR_LT) ||
          is_set_type(token_actual, CHAR_GT) || is_set_type(token_actual, KEY_WORD_NOT))) {
        return SYNTAX_ERROR;
    }

    try_next_token_list_p(token_actual, token_list);

    if (!(is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, LITERAL_NAME) ||
          is_set_type(token_actual, LITERAL_STRING))) {
        return SYNTAX_ERROR;
    }

    return 0;
}

int parse_identifier(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    try_next_token_list_p(token_actual, token_list);

    if (is_set_type(token_actual, CHAR_EQUALS)) {
        return parse_assign_value(&(*token_list));
    }
    if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, LITERAL_NAME) ||
        is_set_type(token_actual, LITERAL_STRING)) {


        //TODO odebrat true
        if (token_list->Act->lptr->token.funkce || true) {
            token_list->Act = token_list->Act->lptr;
            return parse_call_function(&(*token_list));
        }

        return SYNTAX_ERROR;

        //TODO nahradit || &&
    } else if (is_set_type(token_actual, CHAR_EOL) ||
               token_list->Act->lptr->token.funkce) {
        token_list->Act = token_list->Act->lptr;
        return check_end_of_line(&(*token_list));
    }

    return SYNTAX_ERROR;

}

int parse_assign_value(tDList *token_list) {
    int err_code, br_count = 0;
    struct tToken token_actual;
    bool exp_value = false, exp_ar = false;

    while (true) {
        try_next_token_list_p(token_actual, token_list);

        if (is_set_type(token_actual, CHAR_LEFT_BRACKET)) {
            br_count++;

            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = true;
            exp_ar = false;
        } else if (is_set_type(token_actual, CHAR_RIGHT_BRACKET)) {
            br_count--;

            if (exp_value) {
                return SYNTAX_ERROR;
            }

            exp_value = false;
            exp_ar = true;
            if (br_count < 0) {
                return SYNTAX_ERROR;
            }
        } else if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, LITERAL_NAME) ||
                   is_set_type(token_actual, LITERAL_STRING)) {

            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = false;
            exp_ar = true;


        } else if (aritmetic_char) {

            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = true;
            exp_ar = false;
        } else if (is_set_type(token_actual, CHAR_EOL) || is_set_type(token_actual, SEMICOLON)) {
            break;
        } else {
            return SYNTAX_ERROR;
        }
    }
    if (br_count != 0) {
        return SYNTAX_ERROR;
    }
    token_list->Act = token_list->Act->lptr;

    return check_end_of_line(token_list);

}

int parse_call_function(tDList *token_list) {

    struct tToken token_actual;
    bool comma = false;

    while (true) {
        try_next_token_list_p(token_actual, token_list);

        if (comma && !(is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, LITERAL_NAME) ||
                       is_set_type(token_actual, LITERAL_STRING))) {
            return SYNTAX_ERROR;
        }

        if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, LITERAL_NAME) ||
            is_set_type(token_actual, LITERAL_STRING)) {

            comma = false;
            continue;
        } else if (is_set_type(token_actual, CHAR_COMMA)) {
            comma = true;
            continue;
        } else if (is_set_type(token_actual, CHAR_EOL)) {
            token_list->Act = token_list->Act->lptr;
            return check_end_of_line(&(*token_list));
        } else {
            return SYNTAX_ERROR;
        }
    }


}