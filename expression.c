#include "expression.h"

// Precedence table
int prec_table[PREC_TAB_SIZE][PREC_TAB_SIZE] =
        {
                //	|+- | */| r | ( | ) | i | $ |
                { R , S , S , S , R , S , R }, /// +-
                { R , R , R , S , R , S , R }, /// */
                { S , S , S , S , R , S , R }, /// r (relation operators) = <> <= < >= >
                { S , S , S , S , E , S , N }, /// (
                { R , R , R , N , R , N , R }, /// )
                { R , R , R , N , R , N , R }, /// i (id, int, double, string)
                { S , S , S , S , N , S , N }  /// $
        };

Symstack stack;

/**
 * Function converts symbol to precedence table index.
 *
 * @param symbol Symbol to be converted.
 * @return Returns precedence table index.
 */
static Prec_table_index_enum prec_table_index(set_type type) {
    switch (type) {
        case CHAR_OPERATOR_PLUS:
        case CHAR_OPERATOR_MINUS:
            return I_PLUS_MINUS;

        case CHAR_OPERATOR_MUL:
        case CHAR_OPERATOR_DIV:
            return I_MUL_DIV;

        case CHAR_EQUALS:
        case CHAR_NEQ:
        case CHAR_LEQ:
        case CHAR_LT:
        case CHAR_GEQ:
        case CHAR_GT:
            return I_REL_OP;

        case CHAR_LEFT_BRACKET:
            return I_LEFT_BRACKET;

        case CHAR_RIGHT_BRACKET:
            return I_RIGHT_BRACKET;

        case IDENTIFIER_NAME:
        case CHAR_INTEGER:
        case CHAR_DOUBLE:
        case LITERAL_STRING:
            return I_DATA;

        default:
            return I_DOLLAR;
    }
}


/**
 * Function tests if symbols in parameters are valid according to rules.
 *
 * @param num Number of valid symbols in parameter.
 * @param op1 Symbol 1.
 * @param op2 Symbol 2.
 * @param op3 Symbol 3.
 * @return NOT_A_RULE if no rule is found or returns rule which is valid.
 */
static Prec_tab_rules test_rule(int num, Sitem *op1, Sitem *op2, Sitem *op3) {
    switch (num) {
        case 1:
            // rule E -> i
            if (op1->data->set_type_of_token == IDENTIFIER_NAME || op1->data->set_type_of_token == CHAR_INTEGER ||
                op1->data->set_type_of_token == NUMBER_DOUBLE || op1->data->set_type_of_token == LITERAL_STRING)
                return OPERAND;

            return NOT_A_RULE;

        case 3:
            // rule E -> (E)
            if (op1->data->set_type_of_token == CHAR_LEFT_BRACKET && op2->data->set_type_of_token == NON_TERM &&
                op3->data->set_type_of_token == CHAR_RIGHT_BRACKET)
                return LBR_NT_RBR;

            if (op1->data->set_type_of_token == NON_TERM &&
                op3->data->set_type_of_token == NON_TERM) //TODO non term by mohlo znamenat ze je to nejaka blbost?
            {
                switch (op2->data->set_type_of_token) {
                    // rule E -> E + E
                    case CHAR_OPERATOR_PLUS:
                        return NT_PLUS_NT;

                        // rule E -> E - E
                    case CHAR_OPERATOR_MINUS:
                        return NT_MINUS_NT;

                        // rule E -> E * E
                    case CHAR_OPERATOR_MUL:
                        return NT_MUL_NT;

                        // rule E -> E / E
                    case CHAR_OPERATOR_DIV:
                        return NT_DIV_NT;

                        // rule E -> E = E
                    case CHAR_EQUALS:
                        return NT_EQ_NT;

                        // rule E -> E <> E
                    case CHAR_NEQ:
                        return NT_NEQ_NT;

                        // rule E -> E <= E
                    case CHAR_LEQ:
                        return NT_LEQ_NT;

                        // rule E -> E < E
                    case CHAR_LT:
                        return NT_LTN_NT;

                        // rule E -> E >= E
                    case CHAR_GEQ:
                        return NT_MEQ_NT;

                        // rule E -> E > E
                    case CHAR_GT:
                        return NT_MTN_NT;

                        // invalid operator
                    default:
                        return NOT_A_RULE;
                }
            }
            return NOT_A_RULE;
    }
    return NOT_A_RULE;
}


/**
 * Function function counts number of symbols after stop symbol on stack.
 *
 * @param stop_found Pointer to bool variable which will be changed to true if stop was found else to false.
 * @return Number of charatcters after stop symbol. Is valid only when stop_found was set to true.
 */
static int num_of_symbols_after_stop(bool *stop_found) {
    Sitem *tmp = stack_get_top_item(&stack);
    int count = 0;

    while (tmp != NULL) {
        if (tmp->set != STOP) {    //TODO
            *stop_found = false;
            count++;
        } else {
            *stop_found = true;
            break;
        }

        tmp = tmp->next;
    }

    return count;
}

/**
 * Function checks semantics of operands according to rule.
 *
 * @param rule Pointer to table.
 * @param op1 Symbol 1.
 * @param op2 Symbol 2.
 * @param op3 Symbol 3.
 * @param final_type Sets data type which will be after executing rule.
 * @return Given exit code.
 */
static int check_semantics(Prec_tab_rules rule, Sitem *op1, Sitem *op2, Sitem *op3, data_type *final_type) {
    bool retype_op1_to_double = false;
    bool retype_op3_to_double = false;
    bool retype_op1_to_integer = false;
    bool retype_op3_to_integer = false;

    if (rule == OPERAND) {
        if (op1->data->data_type_of_token == UNDEFINED)
            return PROG_SEM_ERROR;

        if (op1->data->data_type_of_token == BOOLEAN)
            return PROG_SEM_ERROR;
    }

    if (rule == LBR_NT_RBR) {
        if (op2->data->data_type_of_token == UNDEFINED)
            return PROG_SEM_ERROR;
        //TODO kontrola zda to neni boolean
    }

    if (rule != OPERAND && rule != LBR_NT_RBR) {
        if (op1->data->data_type_of_token == UNDEFINED || op3->data->data_type_of_token == UNDEFINED)
            return PROG_SEM_ERROR;

        if (op1->data->data_type_of_token == BOOLEAN || op3->data->data_type_of_token == BOOLEAN)
            return PROG_SEM_ERROR;
    }

    switch (rule) {
        case OPERAND:
            *final_type = op1->data->data_type_of_token;
            break;

        case LBR_NT_RBR:
            *final_type = op2->data->data_type_of_token;
            break;

        case NT_PLUS_NT:
        case NT_MINUS_NT:
        case NT_MUL_NT:
            if (op1->data->data_type_of_token == STRING_DT && op3->data->data_type_of_token == STRING_DT &&
                rule == NT_PLUS_NT) {
                *final_type = STRING_DT;
                break;
            }

            if (op1->data->data_type_of_token == INT && op3->data->data_type_of_token == INT) {
                *final_type = INT;
                break;
            }

            if (op1->data->data_type_of_token == STRING_DT || op3->data->data_type_of_token == STRING_DT)
                return PROG_SEM_ERROR;

            *final_type = FLOAT;

            if (op1->data->data_type_of_token == INT)
                retype_op1_to_double = true;

            if (op3->data->data_type_of_token == INT)
                retype_op3_to_double = true;

            break;

        case NT_DIV_NT:
            *final_type = INT;

            if (op1->data->data_type_of_token == STRING_DT || op3->data->data_type_of_token == STRING_DT)
                return PROG_SEM_ERROR;

            if (op1->data->data_type_of_token == INT && op3->data->data_type_of_token == INT)
                break;
/*TODO
 * Operátor / značí dělení dvou číselných operandů. Je-li alespoň jeden operand Float,
je výsledek dělení Float, jinak je operátor proveden jako celočíselné dělení a výsledkem
je Integer. Při dělení nulou dochází k běhové14 chybě 9.

Pro relační operátory <, >, <=, >=, ==, != platí, že výsledkem porovnání je pravdi-
vostní hodnota a že mají stejnou sémantiku jako v jazyce Ruby. Tyto operátory pracují s ope-
randy stejného typu, a to Integer, Float nebo String. Je-li jeden operand Integer a

druhý Float, je operand typu Integer konvertován na Float. U řetězců se porovnání
provádí lexikograficky. Operátory == a != umožňují porovnávat i operandy různých typů
(včetně nil), kdy, nedojde-li k implicitní konverzi mezi Integer a Float, je výsledek
nepravda. Bez rozšíření BOOLOP není s výsledkem porovnání možné dále pracovat a lze
jej využít pouze u příkazů if a while.
 */
            *final_type = FLOAT;
            if (op1->data->data_type_of_token == FLOAT)
                retype_op3_to_double = true;

            if (op3->data->data_type_of_token == FLOAT)
                retype_op1_to_double = true;
            break;

        case NT_EQ_NT:
        case NT_NEQ_NT:
        case NT_LEQ_NT:
        case NT_LTN_NT:
        case NT_MEQ_NT:
        case NT_MTN_NT:
            *final_type = BOOLEAN;

            if (op1->data->data_type_of_token == FLOAT && op3->data->data_type_of_token == INT)
                retype_op3_to_double = true;

            else if (op1->data->data_type_of_token == INT && op3->data->data_type_of_token == FLOAT)
                retype_op1_to_double = true;

            else if (op1->data->data_type_of_token != op3->data->data_type_of_token)
                return PROG_SEM_ERROR;

            break;

        default:
            break;
    }

    if (retype_op1_to_double) {
        GENERATE_CODE(generate_stack_op2_to_double);
    }

    if (retype_op3_to_double) {
        GENERATE_CODE(generate_stack_op1_to_double);
    }

    if (retype_op1_to_integer) {
        GENERATE_CODE(generate_stack_op2_to_integer);
    }

    if (retype_op3_to_integer) {
        GENERATE_CODE(generate_stack_op1_to_integer);
    }

    return 0;
}


/**
 * Function reduces symbols after STOP symbol if rule for reducing is found.
 *
 * @param data Pointer to table.
 * @return Given exit code.
 */
static int reduce_by_rule() {

    int result;

    Sitem *op1 = NULL;
    Sitem *op2 = NULL;
    Sitem *op3 = NULL;
    data_type final_type;
    Prec_tab_rules rule_for_code_gen;
    bool found = false;

    int count = num_of_symbols_after_stop(&found);


    if (count == 1 && found) {
        op1 = stack.top;
        rule_for_code_gen = test_rule(count, op1, NULL, NULL);
    } else if (count == 3 && found) {
        op1 = stack.top->next->next;
        op2 = stack.top->next;
        op3 = stack.top;
        rule_for_code_gen = test_rule(count, op1, op2, op3);
    } else
        return SYNTAX_ERROR;

    if (rule_for_code_gen == NOT_A_RULE)
        return SYNTAX_ERROR;


    if ((result = check_semantics(rule_for_code_gen, op1, op2, op3, &final_type)))
        return result;

    if (rule_for_code_gen == NT_PLUS_NT && final_type == STRING_DT) {
        GENERATE_CODE(generate_concats);
    } else {
        switch (rule_for_code_gen) {
                case NT_PLUS_NT:
                    generate_adds();
                break;

                case NT_MINUS_NT:
                    generate_subs();
                break;

                case NT_MUL_NT:
                    generate_muls();
                break;

                case NT_DIV_NT:
                    generate_divs();
                break;

                case NT_EQ_NT:
                    generate_eqs();
                break;

                case NT_NEQ_NT:
                    generate_not_eqs();
                break;

                case NT_LEQ_NT:
                    generate_ls_eqs();
                break;

                case NT_LTN_NT:
                    generate_lts();
                break;

                case NT_MEQ_NT:
                    generate_gt_eqs();
                break;

                case NT_MTN_NT:
                    generate_gts();
                break;

                default:
                    break;

        }

        stack_pop_count(&stack, count + 1);
        stack_push(&stack, NULL, NON_TERM, final_type);
    }
    return 0;
}

//pridat kontrolu deleni nulou
    int expression(tDList *list, struct tToken *save_location) {
        int result = SYNTAX_ERROR;

        stack_init(&stack);

        if (!stack_push(&stack, NULL, DOLLAR, UNDEFINED))
            ErrorPrint(INTERNAL_ERROR, "[expressions.c][expression()]");

        Sitem *top_stack_terminal;
        set_type actual_symbol;

        bool success = false;
        DLFirst(list);

        do {
            if (list->Act != NULL)
                actual_symbol = list->Act->token.set_type_of_token;
            else
                actual_symbol = DOLLAR;
            top_stack_terminal = stack_get_top_item(&stack);

            if (top_stack_terminal == NULL)
                FREE_RESOURCES_RETURN(INTERNAL_ERROR);

            switch (prec_table[prec_table_index(top_stack_terminal->set)][prec_table_index(
                    actual_symbol)]) {
                case S:
                    if (!stack_insert_after_top(&stack, NULL, STOP, UNDEFINED))
                        FREE_RESOURCES_RETURN(INTERNAL_ERROR);

                    if (!stack_push(&stack, &list->Act->token, actual_symbol, list->Act->token.data_type_of_token))
                        FREE_RESOURCES_RETURN(INTERNAL_ERROR);

                    if (actual_symbol == IDENTIFIER_NAME) {
                        GENERATE_CODE(generate_push_var, list->Act->token);

                    } else if (actual_symbol == CHAR_INTEGER || actual_symbol == CHAR_DOUBLE ||
                               actual_symbol == LITERAL_STRING) {
                        GENERATE_CODE(generate_push_value, list->Act->token);
                    }
                    DLSucc(list);

                    break;

                case E:
                    stack_push(&stack, &list->Act->token, actual_symbol, list->Act->token.data_type_of_token);
                    DLSucc(list);
                    break;

                case R:
                    if ((result = reduce_by_rule()))
                        FREE_RESOURCES_RETURN(result);
                    break;

                case N:
                    if (actual_symbol == DOLLAR && top_stack_terminal->set == DOLLAR)
                        success = true;
                    else
                        FREE_RESOURCES_RETURN(SYNTAX_ERROR);
                    break;
            }
        } while (!success);

        Sitem *final_non_terminal = stack_get_top_item(&stack);
        if (final_non_terminal == NULL)
            ErrorPrint(INTERNAL_ERROR, "[expressions]");
        if (final_non_terminal->set != NON_TERM)
            ErrorPrint(INTERNAL_ERROR, "[expressions]");

        //TODO koknrola final non terminal data type a save loc data type

        generate_pops();
        if (save_location != NULL) {

            switch (save_location->data_type_of_token) {
                case INT:
                    if (final_non_terminal->type == STRING_DT)
                        FREE_RESOURCES_RETURN(PROG_SEM_ERROR);
                    GENERATE_CODE(generate_save_result_to_var, *save_location);
                    break;

                case FLOAT:
                    if (final_non_terminal->type == STRING_DT)
                        FREE_RESOURCES_RETURN(PROG_SEM_ERROR);
                    GENERATE_CODE(generate_save_result_to_var, *save_location);
                    break;

                case STRING_DT:
                    if (final_non_terminal->type != STRING_DT)
                        FREE_RESOURCES_RETURN(PROG_SEM_ERROR);
                    GENERATE_CODE(generate_save_result_to_var, *save_location);
                    break;

                case UNDEFINED:
                    GENERATE_CODE(generate_save_result_to_var, *save_location);
                    break;

                case BOOLEAN:
                    if (final_non_terminal->type != BOOLEAN)
                        FREE_RESOURCES_RETURN(PROG_SEM_ERROR);
                    GENERATE_CODE(generate_save_result_to_var, *save_location);
                    break;

                default:
                    break;
            }
        }

        FREE_RESOURCES_RETURN(0);
    }

