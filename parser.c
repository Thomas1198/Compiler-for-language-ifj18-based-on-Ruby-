/**
 * @file    parser.c
 * @author  Jakub Konetzný(xkonet01)
 * @brief   parser
 */

#include "parser.h"


int run_parser(FILE *source_code) {

    int error_code;

    DLInitList(&token_list);

    symtable_create(&hTable);
    lables_stack = (struct tStack *) malloc(sizeof(struct tStack));

    stack_init(lables_stack);

    lables_stack = (struct tStack *) malloc(sizeof(struct tStack));

    func = false;

    generator_start();
    init_build_in();

    if ((error_code = tokens_to_list(&token_list, source_code)) != 0) {
        generator_clear();
        free_build_in();
        exit(error_code);
    }


    if ((error_code = parsing(token_list)) != 0) {
        generator_clear();
        free_build_in();
        exit(error_code);
    }
    write_code();
    generator_clear();
    free_build_in();
    DLDisposeList(&token_list);
    return 0;
}

int tokens_to_list(tDList *token_list, FILE *source_code) {
    struct tToken token_actual, *tmp = NULL, *tmp_func = NULL;
    bool function_par = false, function_var = false, end = true;
    int end_count = 0;


    while (((token_actual = get_token(source_code))).set_type_of_token != CHAR_EOF) {

        DLInsertLast(&(*token_list), token_actual);


        if (is_set_type(token_actual, IDENTIFIER_NAME)) {

            if (function_par) {
                tmp_func->par_count++;
                dynamic_string_add_const_str(token_actual.content_string, "_var_");
                dynamic_string_add_const_str(token_actual.content_string, tmp_func->content_string->str);

                if (symtable_get(&hTable, token_actual.content_string) != NULL) {
                    return PROG_SEM_ERROR;
                }

                token_list->Last->token.defined = true;
            }
            if (function_var) {

                tmp = symtable_get(&hTable, token_actual.content_string);
                if (tmp == NULL) {

                    dynamic_string_add_const_str(token_actual.content_string, "_var_");
                    dynamic_string_add_const_str(token_actual.content_string, tmp_func->content_string->str);
                }
            }

            if ((tmp = symtable_get(&hTable, token_actual.content_string)) == NULL) {
                symtable_insert(&hTable, &token_list->Last->token);
                tmp = symtable_get(&hTable, token_actual.content_string);
            }
            if (token_list->Last->lptr != NULL) {
                if (is_set_type(token_list->Last->lptr->token, KEY_WORD_DEF)) {
                    if (tmp->defined) {
                        return PROG_SEM_ERROR;
                    }

                    end_count++;
                    tmp->funkce = true;
                    tmp->defined = true;
                    tmp->par_count = 0;
                    if (end) {
                        tmp_func = tmp;
                        end = false;
                        function_par = true;
                    }


                }

                if (is_set_type(token_list->Last->lptr->token, KEY_WORD_IF) ||
                    is_set_type(token_list->Last->lptr->token, KEY_WORD_WHILE)) {
                    end_count++;
                }
            }
        }

        if (is_set_type(token_actual, CHAR_EQUALS)) {
            if (token_list->Last->lptr != NULL) {
                tmp = &token_list->Last->lptr->token;

            }

            if ((tmp = symtable_get(&hTable, tmp->content_string)) == NULL) {
                symtable_insert(&hTable, &token_list->Last->lptr->token);

                if (is_set_type(token_list->Last->lptr->token, IDENTIFIER_NAME)) {
                    token_list->Last->lptr->token.defined = true;
                }
            } else {
                if (is_set_type(*tmp, IDENTIFIER_NAME)) {
                    tmp->defined = true;
                }
            }

        }
        if (is_set_type(token_actual, KEY_WORD_END)) {
            end_count--;
            if (end_count == 0) {
                function_var = false;
                end = true;
            }
        }

        if (is_set_type(token_actual, CHAR_EOL)) {
            if (function_par) {
                function_var = true;
            }
            function_par = false;

        }

    }
    return 0;
}

void init_build_in() {
    inputf = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(inputf);
    inputf->par_count = 0;
    inputf->funkce = true;
    inputf->defined = true;
    inputf->build_id = true;
    dynamic_string_add_const_str(inputf->content_string, "inputf");
    inputf->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, inputf);

    inputi = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(inputi);
    inputi->par_count = 0;
    inputi->funkce = true;
    inputi->defined = true;
    inputi->build_id = true;
    dynamic_string_add_const_str(inputi->content_string, "inputi");
    inputi->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, inputi);

    inputs = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(inputs);
    inputs->par_count = 0;
    inputs->funkce = true;
    inputs->defined = true;
    inputs->build_id = true;
    dynamic_string_add_const_str(inputs->content_string, "inputs");
    inputs->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, inputs);

    print = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(print);
    print->more_params = true;
    print->funkce = true;
    print->defined = true;
    print->build_id = true;
    dynamic_string_add_const_str(print->content_string, "print");
    print->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, print);


    length = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(length);
    length->funkce = true;
    length->defined = true;
    length->par_count = 1;
    length->build_id = true;
    dynamic_string_add_const_str(length->content_string, "length");
    length->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, length);

    subsrt = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(subsrt);
    subsrt->funkce = true;
    subsrt->defined = true;
    subsrt->par_count = 3;
    subsrt->build_id = true;
    dynamic_string_add_const_str(subsrt->content_string, "subsrt");
    subsrt->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, subsrt);


    ord = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(ord);
    ord->funkce = true;
    ord->defined = true;
    ord->par_count = 2;
    ord->build_id = true;
    dynamic_string_add_const_str(ord->content_string, "ord");
    ord->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, ord);

    chr = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(chr);
    chr->funkce = true;
    chr->defined = true;
    chr->par_count = 1;
    chr->build_id = true;
    dynamic_string_add_const_str(chr->content_string, "chr");
    chr->set_type_of_token = IDENTIFIER_NAME;
    symtable_insert(&hTable, chr);

    end = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(end);
    chr->set_type_of_token = KEY_WORD_END;

}

bool is_set_type(struct tToken token, set_type set_type1) {
    return (token.set_type_of_token == set_type1);
}

int parsing(tDList token_list) {
    struct tToken token_actual, *fnc_token;
    int err_code = 0, exp_end = 0;
    bool fce = false, main = false, main_end = false;
    if (token_list.First == NULL) {
        return 0;
    }
    DLFirst(&token_list);


    do {
        token_actual = token_list.Act->token;


        if (!fce && !main) {
            if (token_list.Act->rptr != NULL) {
                fnc_token = &token_list.Act->rptr->token;
            }
            if (!is_set_type(token_actual, KEY_WORD_DEF)) {
                generate_main_start();
                main = true;
                main_end = true;
            }
        }


        switch (token_actual.set_type_of_token) {
            case CHAR_EOL: {
                break;
            }
            case KEY_WORD_DEF: {
                exp_end++;
                fce = true;


                if (main_end) {
                    generate_main_end();
                    main_end = false;
                }


                if (token_list.Act->rptr != NULL) {
                    fnc_token = &token_list.Act->rptr->token;
                    generate_function_start(*fnc_token);
                } else {
                    return SYNTAX_ERROR;
                }

                if ((err_code = parse_def(&token_list)) != 0) {
                    return err_code;
                }

                break;
            }

            case KEY_WORD_IF: {
                if_count++;
                if ((err_code = parse_if(&token_list)) != 0) {
                    return err_code;
                }
                use_if=true;
                exp_end++;
                break;
            }

            case KEY_WORD_WHILE: {

                if ((err_code = parse_while(&token_list)) != 0) {
                    return err_code;
                }
                exp_end++;
                break;
            }

            case KEY_WORD_END: {

                exp_end--;

                if (fce && exp_end == 0) {
                    fce = false;
                }

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
            case CHAR_INTEGER: {

            }
            case STRING_DT: {

            }
            case LITERAL_STRING: {

                if ((err_code = assign_value_const(&token_list) != 0)) {
                    return err_code;
                }
                break;
            }

            case KEY_WORD_ELSE: {

                if ((err_code = parse_else(&token_list) != 0)) {
                    printf("%d", err_code);
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
    if (main_end) {
        generate_main_end();
    }

    return 0;
}

int parse_end(tDList *token_list) {
    end_req--;



        struct tToken *tmp = get_top(lables_stack);

    if (end_req < 0) {
        return SYNTAX_ERROR;
    }

    if (!b_else && use_if) {
        generate_if_else_part(tmp->value.i);
    }
    b_else = false;
    if (end_req == 0 && func) {
        generate_function_ret(*act_fun);
        generate_function_end(*act_fun);
        func = false;
    } else {


        if (is_set_type(*tmp, KEY_WORD_IF)) {

            generate_if_end(tmp->value.i);
            use_if=false;

        } else if (is_set_type(*tmp, KEY_WORD_WHILE)) {
            generate_while_end(tmp->value.i);
        } else {
            exit(99);
        }
        stack_pop(lables_stack);
    }


    return check_end_of_line(&(*token_list));
}

int parse_def(tDList *token_list) {
    int err_code;
    struct tToken token_actual;

    func = true;

    act_fun = &token_list->Act->rptr->token;

    end_req++;

    try_next_token_list_p(token_actual, token_list);

    act_fun = &token_list->Act->token;

    check_set_type(token_actual, IDENTIFIER_NAME);


    try_next_token_list_p(token_actual, token_list);

    check_set_type(token_actual, CHAR_LEFT_BRACKET);

    if ((err_code = parse_def_arguments(&(*token_list))) != 0) {
        return err_code;
    }


    return check_end_of_line(&*(token_list));
}

int parse_def_arguments(tDList *token_list) {
    struct tToken token_actual;
    int i = 0;
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
            generate_function_par_def(token_actual, i);
            i++;
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

    if (is_set_type(token_actual, CHAR_EOL)) {
        return 0;
    } else {
        return SYNTAX_ERROR;
    }

}

int parse_if(tDList *token_list) {
    int err_code;

    generate_if_head();

    end_req++;
    err_code = parse_condition(token_list, KEY_WORD_THEN);
    if (err_code != 0) {
        return err_code;
    }

    struct tToken *tmp;

    tmp = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(tmp);

    tmp->value.i = lable++;
    tmp->set_type_of_token = KEY_WORD_IF;
    stack_push(lables_stack, tmp, KEY_WORD_IF, INT);


    generate_if_start(tmp->value.i);

    return 0;
}

int parse_while(tDList *token_list) {
    int err_code;

    generate_while_head(lable);

    end_req++;
    err_code = parse_condition(token_list, KEY_WORD_DO);
    if (err_code != 0) {
        return 0;
    }


    struct tToken *tmp;

    tmp = (struct tToken *) malloc(sizeof(struct tToken));
    init_token(tmp);

    tmp->value.i = lable++;
    tmp->set_type_of_token = KEY_WORD_WHILE;
    stack_push(lables_stack, tmp, KEY_WORD_WHILE, INT);

    generate_while_start(tmp->value.i);

    return 0;

}

int parse_condition(tDList *token_list, int set) {
    int errcode;

    tDList *tmp_list;
    tmp_list = (tDList *) malloc(sizeof(tDList));

    DLInitList(tmp_list);

    if ((errcode = parse_condition_expr(token_list, NONE, tmp_list)) != 0) {
        free(tmp_list);
        return errcode;
    }

    if ((errcode = parse_condition_expr(token_list, set, tmp_list)) != 0) {
        free(tmp_list);
        return errcode;
    }


    if ((errcode = expression(tmp_list, end)) != 0) {
        return errcode;
    }

    free(tmp_list);
    return check_end_of_line(token_list);
}

int parse_identifier(tDList *token_list) {

    struct tToken token_actual, *tmp;

    try_next_token_list_p(token_actual, token_list);

    //přiřazení
    if (is_set_type(token_actual, CHAR_EQUALS)) {
        return parse_assign_value(token_list);
    }
    //funkce s parametry
    if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, CHAR_INTEGER) ||
        is_set_type(token_actual, CHAR_DOUBLE) ||
        is_set_type(token_actual, LITERAL_STRING) || is_set_type(token_actual, CHAR_LEFT_BRACKET)) {

        tmp = symtable_get(&hTable, token_list->Act->lptr->token.content_string);


        if (!tmp) {
            return SYNTAX_ERROR;
        }

        if (tmp->funkce || tmp->more_params) {


            token_list->Act = token_list->Act->lptr;
            return parse_call_function(&(*token_list), tmp->par_count);

        } else {
            return PROG_SEM_ERROR;
        }



        //funkce bez parametru
    } else if (is_set_type(token_actual, CHAR_EOL)) {

        tmp = symtable_get(&hTable, token_list->Act->lptr->token.content_string);

        if (!tmp) {
            return SYNTAX_ERROR;
        }

        if ((tmp->funkce && tmp->par_count == 0) || tmp->more_params) {
            token_list->Act = token_list->Act->lptr;
            generate_function_before_par();
            generate_function_call(token_list->Act->token);
            return check_end_of_line(&(*token_list));
        } else {
            return FUNCTION_ERROE;
        }


    }

    return SYNTAX_ERROR;

}

int parse_assign_value(tDList *token_list) {
    int br_count = 0, errcode = 0;
    struct tToken token_actual, *tmp = NULL, *value;
    bool exp_value = false, exp_ar = false;
    tDList tmp_list;

    DLInitList(&tmp_list);


    tmp = symtable_get(&hTable, token_list->Act->lptr->token.content_string);

    if (tmp != NULL) {
        if (!tmp->assigned) {
            tmp->assigned = true;
            generate_var_decl(token_list->Act->lptr->token);
        }
    }

    value = &token_list->Act->lptr->token;

    while (true) {
        try_next_token_list_p(token_actual, token_list);

        tmp = symtable_get(&hTable, token_actual.content_string);

        if (tmp != NULL) {
            if (tmp->funkce) {
                generate_function_before_par();
                errcode = parse_call_function(token_list, token_actual.par_count);


                generate_function_return_val_assign(*value);
                return errcode;
            }
        }
        if (is_set_type(token_actual, CHAR_LEFT_BRACKET)) {
            br_count++;
            DLInsertLast(&(tmp_list), token_actual);
            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = true;
            exp_ar = false;
        } else if (is_set_type(token_actual, CHAR_RIGHT_BRACKET)) {
            br_count--;
            DLInsertLast(&(tmp_list), token_actual);
            if (exp_value) {
                return SYNTAX_ERROR;
            }

            exp_value = false;
            exp_ar = true;
            if (br_count < 0) {
                return SYNTAX_ERROR;
            }
        } else if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, CHAR_INTEGER) ||
                   is_set_type(token_actual, CHAR_DOUBLE) ||
                   is_set_type(token_actual, LITERAL_STRING)) {

            tmp = symtable_get(&hTable, token_actual.content_string);

            if (tmp != NULL && !tmp->defined) {
                return PROG_SEM_ERROR;
            }

            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = false;
            exp_ar = true;

            DLInsertLast(&(tmp_list), token_actual);


        } else if (aritmetic_char) {

            DLInsertLast(&(tmp_list), token_actual);

            if (!exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = true;
            exp_ar = false;
        } else if (is_set_type(token_actual, CHAR_EOL)) {
            break;
        } else {
            return SYNTAX_ERROR;
        }
    }
    if (br_count != 0 || exp_value) {
        return SYNTAX_ERROR;
    }
    token_list->Act = token_list->Act->lptr;


    errcode = expression(&tmp_list, value);
    if (errcode != 0) {
        return errcode;
    }

    return 0;

}

int parse_call_function(tDList *token_list, int count) {

    struct tToken token_actual, *tmp, *func;
    bool comma = false, exp_bra = false, before_par = true;
    int par_count = 0, i = 0;


    if (token_list->Act->token.par_count > 0) {
        generate_function_before_par();
    }

    func = symtable_get(&hTable, token_list->Act->token.content_string);


    //TODO výraz jako parametr

    while (true) {
        try_next_token_list_p(token_actual, token_list);

        if (comma && !(is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, CHAR_INTEGER) ||
                       is_set_type(token_actual, CHAR_DOUBLE) ||
                       is_set_type(token_actual, LITERAL_STRING))) {
            return SYNTAX_ERROR;
        }

        if (is_set_type(token_actual, CHAR_LEFT_BRACKET)) {
            exp_bra = true;
            continue;
        }

        if (is_set_type(token_actual, CHAR_RIGHT_BRACKET) && exp_bra) {
            try_next_token_list_p(token_actual, token_list);
            if (is_set_type(token_actual, CHAR_EOL)) {
                if (!func->more_params) {
                    generate_function_call(*func);
                }
                return 0;
            }
        }

        if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, CHAR_INTEGER) ||
            is_set_type(token_actual, CHAR_DOUBLE) ||
            is_set_type(token_actual, LITERAL_STRING)) {

            if (before_par && !func->more_params) {
                before_par = false;
                generate_function_before_par();
            }


            if (func->more_params) {
                generate_function_before_par();
                generate_function_pass_par(token_actual, 0);
                generate_function_call(*func);
            } else {
                generate_function_pass_par(token_actual, i);
            }

            i++;

            tmp = symtable_get(&hTable, token_actual.content_string);

            if (tmp != NULL && !tmp->defined) {
                return PROG_SEM_ERROR;
            }

            par_count++;
            comma = false;
            continue;
        } else if (is_set_type(token_actual, CHAR_COMMA)) {
            comma = true;
            continue;
        } else if (is_set_type(token_actual, CHAR_EOL)) {
            token_list->Act = token_list->Act->lptr;

            if (par_count != count && !func->more_params) {
                return FUNCTION_ERROE;
            }

            if (exp_bra) {
                return SYNTAX_ERROR;
            }


            if (!func->more_params) {
                generate_function_call(*func);
            }

            return check_end_of_line(&(*token_list));
        } else {
            return SYNTAX_ERROR;
        }
    }


}

int parse_condition_expr(tDList *token_list, int set, tDList *tmp_list) {
    int br_count = 0;
    struct tToken token_actual, *tmp;
    bool exp_value = false, exp_ar = false;

    while (true) {
        try_next_token_list_p(token_actual, token_list);

        if (is_set_type(token_actual, CHAR_LEFT_BRACKET)) {
            br_count++;
            DLInsertLast(tmp_list, token_actual);

            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = true;
            exp_ar = false;
        } else if (is_set_type(token_actual, CHAR_RIGHT_BRACKET)) {
            br_count--;
            DLInsertLast(tmp_list, token_actual);
            if (exp_value) {
                return SYNTAX_ERROR;
            }

            exp_value = false;
            exp_ar = true;
            if (br_count < 0) {
                return SYNTAX_ERROR;
            }
        } else if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, CHAR_INTEGER) ||
                   is_set_type(token_actual, CHAR_DOUBLE) ||
                   is_set_type(token_actual, LITERAL_STRING)) {

            DLInsertLast(tmp_list, token_actual);
            tmp = symtable_get(&hTable, token_actual.content_string);

            if (tmp != NULL && !tmp->defined) {
                return PROG_SEM_ERROR;
            }

            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = false;
            exp_ar = true;


        } else if (aritmetic_char) {
            DLInsertLast(tmp_list, token_actual);
            if (!exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = true;
            exp_ar = false;
        } else if (is_set_type(token_actual, CHAR_EOL) ||
                   is_set_type(token_actual, set)) {
            break;
        } else if (reational_char) {
            DLInsertLast(tmp_list, token_actual);
            break;
        } else {
            return SYNTAX_ERROR;
        }
    }
    if (br_count != 0 || exp_value) {
        return SYNTAX_ERROR;
    }


    return 0;
}


int assign_value_const(tDList *token_list) {
    int br_count = 0, errcode = 0;
    struct tToken token_actual, *tmp = NULL;
    bool exp_value = false, exp_ar = false, first = false;
    tDList tmp_list;

    DLInitList(&tmp_list);

    token_actual = token_list->Act->token;

    while (true) {

        if (first) {
            try_next_token_list_p(token_actual, token_list);
        }
        first = true;
        if (token_actual.set_type_of_token == IDENTIFIER_NAME) {
            tmp = symtable_get(&hTable, token_actual.content_string);
        }
        if (tmp != NULL) {
            if (tmp->funkce) {
                errcode = parse_call_function(token_list, token_actual.par_count);

                return errcode;
            }
        }
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
        } else if (is_set_type(token_actual, IDENTIFIER_NAME) || is_set_type(token_actual, CHAR_INTEGER) ||
                   is_set_type(token_actual, CHAR_DOUBLE) ||
                   is_set_type(token_actual, LITERAL_STRING)) {

            tmp = symtable_get(&hTable, token_actual.content_string);

            if (tmp != NULL && !tmp->defined) {
                return PROG_SEM_ERROR;
            }

            if (exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = false;
            exp_ar = true;

            DLInsertLast(&(tmp_list), token_actual);


        } else if (aritmetic_char) {

            DLInsertLast(&(tmp_list), token_actual);

            if (!exp_ar) {
                return SYNTAX_ERROR;
            }

            exp_value = true;
            exp_ar = false;
        } else if (is_set_type(token_actual, CHAR_EOL)) {
            break;
        } else {
            return SYNTAX_ERROR;
        }


    }
    if (br_count != 0 || exp_value) {

        exit(SYNTAX_ERROR);

    }

    errcode = expression(&tmp_list, end);
    if (errcode != 0) {
        exit(errcode);
    }

    return 0;

}


void free_build_in() {
    free(inputf);
    free(inputi);
    free(inputs);
    free(print);
    free(length);
    free(subsrt);
    free(ord);
    free(chr);
    free(end);
}

int parse_else(tDList *token_list) {
    struct tToken token_actual;
    struct tToken *tmp = get_top(lables_stack);

    b_else = true;

    generate_if_else_part(tmp->value.i);

    try_next_token_list_p(token_actual, token_list);
    if (!is_set_type(token_actual, CHAR_EOL)) {
        exit(2);
    }
    return 0;
}