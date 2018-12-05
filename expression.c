/**
 * @file expressions.c
 * @author Tomas Dorda (xdorda00)
 * @brief Expressions evaluation
 */

#include "expression.h"

#define FREE_RESOURCES_RETURN(return_code)        \
    do {                                        \
        stack_free(&stack);                        \
        return return_code;                        \
    } while(0)


#define GENERATE_CODE(_callback, ...)                                \
    (_callback(__VA_ARGS__))

// Precedence table
int prec_table[PREC_TAB_SIZE][PREC_TAB_SIZE] =
        {
                //	|+- | */| r | ( | ) | i | $ |
                {R, S, S, S, R, S, R}, /// +-
                {R, R, R, S, R, S, R}, /// */
                {S, S, S, S, R, S, R}, /// r (relation operators) = <> <= < >= >
                {S, S, S, S, E, S, N}, /// (
                {R, R, R, N, R, N, R}, /// )
                {R, R, R, N, R, N, R}, /// i (id, int, double, string)
                {S, S, S, S, N, S, N}  /// $
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
 * @brief tests if symbols in parameters are valid according to rules.
 * @param num number of symbols
 * @param op1 Symbol 1.
 * @param op2 Symbol 2.
 * @param op3 Symbol 3.
 * @return rule or not rule
 */
static Prec_tab_rules test_rule(int num, Sitem *op1, Sitem *op2, Sitem *op3) {
    switch (num) {
        case 1:         // rule E -> i
            if (op1->set == IDENTIFIER_NAME || op1->set == CHAR_INTEGER ||
                op1->set == CHAR_DOUBLE || op1->set == LITERAL_STRING)
                return OPERAND;

            return NOT_A_RULE;

        case 3:         // E -> (E)
            if (op1->set == CHAR_LEFT_BRACKET && op2->set == NON_TERM &&
                op3->set == CHAR_RIGHT_BRACKET)
                return LBR_NT_RBR;

            if (op1->set == NON_TERM && op3->set == NON_TERM) {
                switch (op2->set) {
                    case CHAR_OPERATOR_PLUS:    // E -> E + E
                        return NT_PLUS_NT;

                    case CHAR_OPERATOR_MINUS:   // E -> E - E
                        return NT_MINUS_NT;

                    case CHAR_OPERATOR_MUL:     // E -> E * E
                        return NT_MUL_NT;

                    case CHAR_OPERATOR_DIV:     // E -> E / E
                        return NT_DIV_NT;

                    case CHAR_EQUALS:           // E -> E = E
                        return NT_EQ_NT;

                    case CHAR_NEQ:              // E -> E <> E
                        return NT_NEQ_NT;

                    case CHAR_LEQ:              // E -> E <= E
                        return NT_LEQ_NT;

                    case CHAR_LT:               // E -> E < E
                        return NT_LTN_NT;

                    case CHAR_GEQ:              // E -> E >= E
                        return NT_MEQ_NT;

                    case CHAR_GT:               // E -> E > E
                        return NT_MTN_NT;

                    default:
                        return NOT_A_RULE;
                }
            }
            return NOT_A_RULE;
    }
    return NOT_A_RULE;
}


/**
 * @brief counts number of symbols after stop on stack
 * @param stop_found points to bool variable (true if found otherwise to false)
 * @return number of symbols after stop symbol
 */
static int num_of_symbols_after_stop(bool *stop_found) {
    Sitem *tmp = stack.top;
    int count = 0;

    while (tmp != NULL) {
        if (tmp->set != STOP) {
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
 * @brief checks of operands semantics
 * @param rule Pointer to table.
 * @param op1 Symbol 1
 * @param op2 Symbol 2
 * @param op3 Symbol 3
 * @param final_type Sets data type which will be after executing rule.
 * @return Given exit code.
 */
static int check_semantics(Prec_tab_rules rule, Sitem *op1, Sitem *op2, Sitem *op3, data_type *final_type) {
    bool retype_op1_to_double = false;
    bool retype_op3_to_double = false;

    if (rule == OPERAND) {
        if (op1->set == UNDEFINED)
            return PROG_SEM_ERROR;

        if (op1->type == BOOLEAN)
            return PROG_SEM_ERROR;
    }

    if (rule == LBR_NT_RBR) {
        if (op2->type == UNDEFINED)
            return PROG_SEM_ERROR;
    }

    if (rule != OPERAND && rule != LBR_NT_RBR) {
        if (op1->type == UNDEFINED || op3->type == UNDEFINED)
            return PROG_SEM_ERROR;

        if (op1->type == BOOLEAN || op3->type == BOOLEAN)
            return PROG_SEM_ERROR;
    }

    switch (rule) {
        case OPERAND:
            *final_type = op1->type;
            break;

        case LBR_NT_RBR:
            *final_type = op2->type;
            break;

        case NT_PLUS_NT:
        case NT_MINUS_NT:
        case NT_MUL_NT:
            if (op1->type == STRING_DT && op3->type == STRING_DT &&
                rule == NT_PLUS_NT) {
                *final_type = STRING_DT;
                break;
            }

            if (op1->type == INT && op3->type == INT) {
                *final_type = INT;
                break;
            }

            if (op1->type == STRING_DT || op3->type == STRING_DT)
                return PROG_SEM_ERROR;

            *final_type = FLOAT;

            if (op1->type == INT)
                retype_op1_to_double = true;

            if (op3->type == INT)
                retype_op3_to_double = true;

            break;

        case NT_DIV_NT:
            *final_type = INT;

            if (op1->type == STRING_DT || op3->type == STRING_DT)
                return PROG_SEM_ERROR;

            if (op3->is_null)
                exit(ZERO_DIVISION_ERROR);

            if (op1->type == INT && op3->type == INT)
                break;

            *final_type = FLOAT;
            if (op1->type == FLOAT)
                retype_op3_to_double = true;

            if (op3->type == FLOAT)
                retype_op1_to_double = true;
            break;

        case NT_EQ_NT:
        case NT_NEQ_NT:
        case NT_LEQ_NT:
        case NT_LTN_NT:
        case NT_MEQ_NT:
        case NT_MTN_NT:
            *final_type = BOOLEAN;

            if (op1->type == FLOAT && op3->type == INT)
                retype_op3_to_double = true;

            else if (op1->type == INT && op3->type == FLOAT)
                retype_op1_to_double = true;

            else if (op1->type != op3->type)
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

    return 0;
}


/**
 * @brief Reduces symbols after STOP
 * @param data pointer to table
 * @return exit code
 */
static int reduce_by_rule(struct tToken *save_location) {

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
                if(final_type == FLOAT)
                    generate_divs();
                else
                    generate_idivs();
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
    }
    bool is_null_tmp = stack.top->is_null;
    stack_pop_count(&stack, count + 1);
    stack_push(&stack, NULL, NON_TERM, final_type);
    save_location->data_type_of_token = final_type;
    stack.top->is_null =is_null_tmp;
    return 0;
}

int expression(tDList *list, struct tToken *save_location) {
    struct tToken *tmp;
    if ((tmp = symtable_get(&hTable, save_location->content_string)) != NULL)
        save_location = tmp;

    int result;

    stack_init(&stack);

    if (!stack_push(&stack, NULL, DOLLAR, UNDEFINED))
        ErrorPrint(INTERNAL_ERROR, "[expressions.c][expression()]");

    Sitem *top_stack_terminal;
    set_type actual_symbol;
    struct tToken *actual_token;

    bool success = false;
    DLFirst(list);

    do {
        if (list->Act != NULL) {
            actual_symbol = list->Act->token.set_type_of_token;
            if ((actual_token = symtable_get(&hTable, list->Act->token.content_string)) == NULL)
                actual_token = &list->Act->token;
        } else
            actual_symbol = DOLLAR;
        top_stack_terminal = stack_get_top_term(&stack);

        if (top_stack_terminal == NULL)
            FREE_RESOURCES_RETURN(INTERNAL_ERROR);

        switch (prec_table[prec_table_index(top_stack_terminal->set)][prec_table_index(
                actual_symbol)]) {
            case S:
                if (!stack_insert_after_top(&stack, NULL, STOP, UNDEFINED))
                    FREE_RESOURCES_RETURN(INTERNAL_ERROR);

                if (!stack_push(&stack, actual_token, actual_symbol, actual_token->data_type_of_token))
                    FREE_RESOURCES_RETURN(INTERNAL_ERROR);

                if (actual_symbol == IDENTIFIER_NAME) {
                    GENERATE_CODE(generate_push_var, *actual_token);

                } else if (actual_symbol == CHAR_INTEGER || actual_symbol == CHAR_DOUBLE ||
                           actual_symbol == LITERAL_STRING) {
                    GENERATE_CODE(generate_push_value, *actual_token);
                }
                DLSucc(list);

                break;

            case E:
                stack_push(&stack, actual_token, actual_symbol, actual_token->data_type_of_token);
                DLSucc(list);
                break;

            case R:
                if ((result = reduce_by_rule(save_location)))
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

    Sitem *final_non_terminal = stack.top;
    if (final_non_terminal == NULL)
        ErrorPrint(INTERNAL_ERROR, "[expressions]");
    if (final_non_terminal->set != NON_TERM)
        ErrorPrint(INTERNAL_ERROR, "[expressions]");

    if (save_location != NULL) {
        generate_pops();
        generate_clear_stack();
        if(save_location->data_type_of_token != BOOLEAN && save_location->set_type_of_token != KEY_WORD_END)
            GENERATE_CODE(generate_save_result_to_var, *save_location);
    }

    FREE_RESOURCES_RETURN(0);
}

