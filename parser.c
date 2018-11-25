#include "parser.h"

int run_parser(FILE *source_code) {
    tDList token_list;
    int error_code;

    DLInitList(&token_list);

    symtable_create(&hTable);

    first_run(&token_list, source_code);

    generator_start();

    if ((error_code = parsing(token_list)) != 0) {
        //TODO uvolneni pameti
        exit(error_code);
    }
    //TODO předělat na tvrdo vložený konec
generate_main_end();

    DLDisposeList(&token_list);
    return 0;
}

void first_run(tDList *token_list, FILE *source_code) {
    struct tToken token_actual;

    while (((token_actual = get_token(source_code))).set_type_of_token != CHAR_EOF) {
        DLInsertLast(&(*token_list), token_actual);

        if (is_set_type(token_actual, IDENTIFIER_NAME)) {
            //TODO vložení do tabulky
            if (symtable_get(&hTable, token_actual.content_string) == NULL) {

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

    /*

     CHAR_OPERATOR_PLUS,
	CHAR_OPERATOR_MINUS,
	CHAR_OPERATOR_DIV,
	CHAR_OPERATOR_MUL,
	CHAR_ASSIGN,
    CHAR_EOL,
    IDENTIFIER_NAME,
    CHAR_LEFT_BRACKET,
    CHAR_RIGHT_BRACKET,
    LITERAL_NAME,
    KEY_WORD_DEF,
    KEY_WORD_DO,
    KEY_WORD_ELSE,
    KEY_WORD_END,
    KEY_WORD_IF,
    KEY_WORD_NOT,
    KEY_WORD_NIL,
    KEY_WORD_THEN,
    KEY_WORD_WHILE,
	EMPTY,
	CHAR_SEMICOLON,
	CHAR_COMMA,
	CHAR_EOF,

     */

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
                //TODO natvrdo přidaný začatek mainu předělat
                generate_main_start();

                break;
            }

            case IDENTIFIER_NAME: {

                if ((err_code = parse_identifier(&token_list)) != 0) {
                    return err_code;
                }

                break;
            }
            case LITERAL_NAME: {


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

    //TODO pokud je funkce
    generate_function_end();

    return check_end_of_line(&(*token_list));
}

int parse_def(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    end_req++;

    try_next_token_list_p(token_actual, token_list);

    check_set_type(token_actual, IDENTIFIER_NAME);

    generate_function_start(token_actual);

    try_next_token_list_p(token_actual, token_list);

    check_set_type(token_actual, CHAR_LEFT_BRACKET);

    if ((err_code = parse_def_arguments(&(*token_list))) != 0) {
        return err_code;
    }

    //TODO pro parametry
    //generate_function_before_par();

    return check_end_of_line(&*(token_list));
}

int parse_def_arguments(tDList *token_list) {
    struct tToken token_actual;
    bool comma = false;

    while (true) {
        try_next_token_list_p(token_actual, token_list);

        if (comma) {
            return SYNTAX_ERROR;
        }

        if (is_set_type(token_actual, CHAR_RIGHT_BRACKET)) {
            return 0;
        }

        comma = false;

        if (is_set_type(token_actual, IDENTIFIER_NAME)) {
            try_next_token_list_p(token_actual, token_list);

            if (is_set_type(token_actual, CHAR_COMMA)) {
                comma = true;
                continue;
            } else {
                continue;
            }
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

    try_next_token_list_p(token_actual, token_list);

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

    try_next_token_list_p(token_actual, token_list);

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
    //TODO dopsat tělo funkce
    return 0;
}

int parse_identifier(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    try_next_token_list_p(token_actual, token_list);

    if (is_set_type(token_actual, CHAR_ASSIGN)) {
        return parse_assign_value(&(*token_list));
    }
    if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, LITERAL_NAME) ||
        is_set_type(token_actual, LITERAL_STRING)) {
        return parse_call_function(&(*token_list));
    } else{

        generate_function_call(token_list->Act->lptr->token);
        return 0;
    }

    //TODO bez parametru funkce

}

int parse_assign_value(tDList *token_list) {

    //TODO dodělat až pojede HW
    return 0;

}

int parse_call_function(tDList *token_list) {
    int err_code;
    struct tToken token_actual = token_list->Act->token;
    //TODO více paramettů!!!!!
    //TODO
    //TODO
    //TODO

    //TODO více paramertů

        generate_function_before_par();
        generate_function_par(token_actual, 0);
        token_actual=token_list->Act->lptr->token;
        generate_function_call(token_actual);
    return check_end_of_line(&(*token_list));

}