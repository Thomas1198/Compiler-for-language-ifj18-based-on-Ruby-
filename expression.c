
#include "expression.h"

#define PREC_TAB_SIZE 8

#define GENERATE_CODE(_callback, ...)								\
	if (!_callback(__VA_ARGS__)) return INTERNAL_ERROR

Symstack stack;

/**
 * @enum Precedence table sings.
 */
typedef enum
{
    S,    /// < SHIFT
    E,    /// = EQUAL
    R,    /// > REDUCE
    N     /// # ERROR
} Prec_table_signs;

/**
 * @enum Precedence table indexes.
 */
typedef enum
{
    I_PLUS_MINUS,		/// 0 +-
    I_MUL_DIV,			/// 1 */
    I_IDIV,				/// 2 \ /
    I_REL_OP,			/// 3 r
    I_LEFT_BRACKET,		/// 4 (
    I_RIGHT_BRACKET,	/// 5 )
    I_DATA,				/// 6 i
    I_DOLLAR			/// 7 $
} Prec_table_index_enum;

int prec_table[PREC_TAB_SIZE][PREC_TAB_SIZE] =
        {
            //	|+- | */| \ | r | ( | ) | i | $ |
                { R , S , S , R , S , R , S , R }, /// +-
                { R , R , R , R , S , R , S , R }, /// */
                { R , S , R , R , S , R , S , R }, /// \ /
                { S , S , S , N , S , R , S , R }, /// r (relation operators) = <> <= < >= >
                { S , S , S , S , S , E , S , N }, /// (
                { R , R , R , R , N , R , N , R }, /// )
                { R , R , R , R , N , R , N , R }, /// i (id, int, double, string)
                { S , S , S , S , S , N , S , N }  /// $
        };


/**
 * @enum Rules used for parsing.
 */
typedef enum
{
    NT_EQ_NT,		/// E -> E = E
    NT_NEQ_NT,		/// E -> E <> E
    NT_LEQ_NT,		/// E -> E <= E
    NT_LTN_NT,		/// E -> E < E
    NT_MEQ_NT,		/// E -> E => E
    NT_MTN_NT,		/// E -> E > E
    NT_PLUS_NT,		/// E -> E + E
    NT_MINUS_NT,	/// E -> E - E
    NT_IDIV_NT,		/// E -> E \ E
    NT_MUL_NT,		/// E -> E * E
    NT_DIV_NT,		/// E -> E / E
    LBR_NT_RBR,		/// E -> (E)
    OPERAND,		/// E -> i
    NOT_A_RULE		/// rule doesn't exist
} Prec_tab_rules;


/**
 * Function converts symbol to precedence table index.
 *
 * @param symbol Symbol to be converted.
 * @return Returns precedence table index.
 */
static Prec_table_index_enum prec_table_index(set_type sign)
{
    switch (sign)
    {
        case CHAR_OPERATOR_PLUS:
        case CHAR_OPERATOR_MINUS:
            return I_PLUS_MINUS;

        case CHAR_OPERATOR_MUL:
        case CHAR_OPERATOR_DIV:
            return I_MUL_DIV;

        case IDIV:
            return I_IDIV;

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
        case DOUBLE:
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
static Prec_tab_rules test_rule(int num, Sitem *op1, Sitem *op2, Sitem *op3)
{
    switch (num)
    {
        case 1:
            // rule E -> i
            if (op1->data->set_type_of_token == IDENTIFIER_NAME || op1->data->set_type_of_token == CHAR_INTEGER || op1->data->set_type_of_token == NUMBER_DOUBLE || op1->data->set_type_of_token == LITERAL_STRING)
                return OPERAND;

            return NOT_A_RULE;

        case 3:
            // rule E -> (E)
            if (op1->data->set_type_of_token == CHAR_LEFT_BRACKET && op2->data->set_type_of_token == NON_TERM && op3->data->set_type_of_token == CHAR_RIGHT_BRACKET)
                return LBR_NT_RBR;

            if (op1->data->set_type_of_token == NON_TERM && op3->data->set_type_of_token == NON_TERM) //TODO non term by mohlo znamenat ze je to nejaka blbost?
            {
                switch (op2->data->set_type_of_token)
                {
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

                        // rule E -> E \ E
                    case IDIV:      //TODO
                        return NT_IDIV_NT;

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
                    case LESS_THAN:
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
        if (!stack_is_empty(&stack)) {
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
static int check_semantics(Prec_rules_enum rule, Symbol_stack_item* op1, Symbol_stack_item* op2, Symbol_stack_item* op3, Data_type* final_type)
{
    bool retype_op1_to_double = false;
    bool retype_op3_to_double = false;
    bool retype_op1_to_integer = false;
    bool retype_op3_to_integer = false;

    if (rule == OPERAND)
    {
        if (op1->data_type == TYPE_UNDEFINED)
            return SEM_ERR_UNDEFINED_VAR;

        if (op1->data_type == TYPE_BOOL)
            return SEM_ERR_TYPE_COMPAT;
    }

    if (rule == LBR_NT_RBR)
    {
        if (op2->data_type == TYPE_UNDEFINED)
            return SEM_ERR_UNDEFINED_VAR;
    }

    if (rule != OPERAND && rule != LBR_NT_RBR)
    {
        if (op1->data_type == TYPE_UNDEFINED || op3->data_type == TYPE_UNDEFINED)
            return SEM_ERR_UNDEFINED_VAR;

        if (op1->data_type == TYPE_BOOL || op3->data_type == TYPE_BOOL)
            return SEM_ERR_TYPE_COMPAT;
    }

    switch (rule)
    {
        case OPERAND:
            *final_type = op1->data_type;
            break;

        case LBR_NT_RBR:
            *final_type = op2->data_type;
            break;

        case NT_PLUS_NT:
        case NT_MINUS_NT:
        case NT_MUL_NT:
            if (op1->data_type == TYPE_STRING && op3->data_type == TYPE_STRING && rule == NT_PLUS_NT)
            {
                *final_type = TYPE_STRING;
                break;
            }

            if (op1->data_type == TYPE_INT && op3->data_type == TYPE_INT)
            {
                *final_type = TYPE_INT;
                break;
            }

            if (op1->data_type == TYPE_STRING || op3->data_type == TYPE_STRING)
                return SEM_ERR_TYPE_COMPAT;

            *final_type = TYPE_DOUBLE;

            if (op1->data_type == TYPE_INT)
                retype_op1_to_double = true;

            if (op3->data_type == TYPE_INT)
                retype_op3_to_double = true;

            break;

        case NT_DIV_NT:
            *final_type = TYPE_DOUBLE;

            if (op1->data_type == TYPE_STRING || op3->data_type == TYPE_STRING)
                return SEM_ERR_TYPE_COMPAT;

            if (op1->data_type == TYPE_INT)
                retype_op1_to_double = true;

            if (op3->data_type == TYPE_INT)
                retype_op3_to_double = true;

            break;

        case NT_IDIV_NT:
            *final_type = TYPE_INT;

            if (op1->data_type == TYPE_STRING || op3->data_type == TYPE_STRING)
                return SEM_ERR_TYPE_COMPAT;

            if (op1->data_type == TYPE_DOUBLE)
                retype_op1_to_integer = true;

            if (op3->data_type == TYPE_DOUBLE)
                retype_op3_to_integer = true;

            break;

        case NT_EQ_NT:
        case NT_NEQ_NT:
        case NT_LEQ_NT:
        case NT_LTN_NT:
        case NT_MEQ_NT:
        case NT_MTN_NT:
            *final_type = TYPE_BOOL;

            if (op1->data_type == TYPE_INT && op3->data_type == TYPE_DOUBLE)
                retype_op1_to_double = true;

            else if (op1->data_type == TYPE_DOUBLE && op3->data_type == TYPE_INT)
                retype_op3_to_double = true;

            else if (op1->data_type != op3->data_type)
                return SEM_ERR_TYPE_COMPAT;

            break;

        default:
            break;
    }

    if (retype_op1_to_double)
    {
        GENERATE_CODE(generate_stack_op2_to_double);
    }

    if (retype_op3_to_double)
    {
        GENERATE_CODE(generate_stack_op1_to_double);
    }

    if (retype_op1_to_integer)
    {
        GENERATE_CODE(generate_stack_op2_to_integer);
    }

    if (retype_op3_to_integer)
    {
        GENERATE_CODE(generate_stack_op1_to_integer);
    }

    return SYNTAX_OK;
}


/**
 * Function reduces symbols after STOP symbol if rule for reducing is found.
 *
 * @param data Pointer to table.
 * @return Given exit code.
 */
static int reduce_by_rule(PData* data)
{
    (void) data;

    int result;

    Sitem* op1 = NULL;
    Sitem* op2 = NULL;
    Sitem* op3 = NULL;
    data_type final_type;
    Prec_tab_rules rule_for_code_gen;
    bool found = false;

    int count = num_of_symbols_after_stop(&found);


    if (count == 1 && found)
    {
        op1 = stack.top;
        rule_for_code_gen = test_rule(count, op1, NULL, NULL);
    }
    else if (count == 3 && found)
    {
        op1 = stack.top->next->next;
        op2 = stack.top->next;
        op3 = stack.top;
        rule_for_code_gen = test_rule(count, op1, op2, op3);
    }
    else
        return SYNTAX_ERROR;

    if (rule_for_code_gen == NOT_A_RULE)
    {
        return SYNTAX_ERROR;
    }
    else
    {
        if (rule_for_code_gen == NT_PLUS_NT && final_type == TYPE_STRING)
        {
            GENERATE_CODE(generate_concat_stack_strings);
        }
        else GENERATE_CODE(generate_stack_operation, rule_for_code_gen);

        symbol_stack_pop_count(&stack, count + 1);
        stack_push(&stack, NON_TERM, final_type);
    }

    return 0;
}


int expression(PData* data)
{
    int result = SYNTAX_ERROR;

    stack_init(&stack);

    if (!stack_push(&stack, DOLLAR, TYPE_UNDEFINED))
        ErrorPrint(INTERNAL_ERROR);

    Sitem* top_stack_terminal;
    set_type actual_symbol;

    bool success = false;

    do
    {
        actual_symbol = prec_table_index(&data->token);
        top_stack_terminal = symbol_stack_top_terminal(&stack);

        if (top_stack_terminal == NULL)
            FREE_RESOURCES_RETURN(ERROR_INTERNAL);

        switch (prec_table[prec_table_index(top_stack_terminal->data->set_type_of_token)][prec_table_index(actual_symbol)])
        {
            case S:
                if (!symbol_stack_insert_after_top_terminal(&stack, STOP, TYPE_UNDEFINED))
                    FREE_RESOURCES_RETURN(ERROR_INTERNAL);

                if(!symbol_stack_push(&stack, actual_symbol, get_data_type(&data->token, data)))
                    FREE_RESOURCES_RETURN(ERROR_INTERNAL);

                if (actual_symbol == IDENTIFIER || actual_symbol == INT_NUMBER || actual_symbol == DOUBLE_NUMBER || actual_symbol == STRING)
                {
                    GENERATE_CODE(generate_push, data->token);
                }

                if ((result = get_next_token(&data->token)))
                    FREE_RESOURCES_RETURN(result);
                break;

            case E:
                symbol_stack_push(&stack, actual_symbol, get_data_type(&data->token, data));

                if ((result = get_next_token(&data->token)))
                    FREE_RESOURCES_RETURN(result);
                break;

            case R:
                if ((result = reduce_by_rule(data)))
                    FREE_RESOURCES_RETURN(result);
                break;

            case N:
                if (actual_symbol == DOLLAR && top_stack_terminal->symbol == DOLLAR)
                    success = true;
                else
                    FREE_RESOURCES_RETURN(SYNTAX_ERR);
                break;
        }
    } while (!success);

    Sitem* final_non_terminal = stack_get_top_item(&stack);
    if (final_non_terminal == NULL)
        ErrorPrint(INTERNAL_ERROR);
    if (final_non_terminal->symbol != NON_TERM)
        ErrorPrint(INTERNAL_ERROR);

    if (data->lhs_id != NULL)
    {
        char *frame = "LF";
        if (data->lhs_id->global) frame = "GF";

        switch (data->lhs_id->type)
        {
            case TYPE_INT:
                if (final_non_terminal->data_type == TYPE_STRING)
                    FREE_RESOURCES_RETURN(SEM_ERR_TYPE_COMPAT);

                GENERATE_CODE(generate_save_expression_result, data->lhs_id->identifier, final_non_terminal->data_type, TYPE_INT, frame);
                break;

            case TYPE_DOUBLE:
                if (final_non_terminal->data_type == TYPE_STRING)
                    FREE_RESOURCES_RETURN(SEM_ERR_TYPE_COMPAT);

                GENERATE_CODE(generate_save_expression_result, data->lhs_id->identifier, final_non_terminal->data_type, TYPE_DOUBLE, frame);
                break;

            case TYPE_STRING:
                if (final_non_terminal->data_type != TYPE_STRING)
                    FREE_RESOURCES_RETURN(SEM_ERR_TYPE_COMPAT);

                GENERATE_CODE(generate_save_expression_result, data->lhs_id->identifier, TYPE_STRING, TYPE_STRING, frame);
                break;

            case TYPE_UNDEFINED:
                GENERATE_CODE(generate_save_expression_result, data->lhs_id->identifier, final_non_terminal->data_type, TYPE_UNDEFINED, frame);
                break;

            case TYPE_BOOL:
                if (final_non_terminal->data_type != TYPE_BOOL)
                    FREE_RESOURCES_RETURN(SEM_ERR_TYPE_COMPAT);

                GENERATE_CODE(generate_save_expression_result, data->lhs_id->identifier, final_non_terminal->data_type, TYPE_BOOL, frame);
                break;

            default:
                break;
        }
    }

    FREE_RESOURCES_RETURN(0);
}
