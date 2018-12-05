#ifndef PROJECT_EXPRESSION_H
#define PROJECT_EXPRESSION_H

#include "token.h"
#include "token_list.h"
#include "stack.h"
#include "error.h"
#include "code_generator.h"
#include "error.h"
#include "symtable.h"
#define PREC_TAB_SIZE 7


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
    NT_MUL_NT,		/// E -> E * E
    NT_DIV_NT,		/// E -> E / E
    LBR_NT_RBR,		/// E -> (E)
    OPERAND,		/// E -> i
    NOT_A_RULE		/// rule doesn't exist
} Prec_tab_rules;


/**
 * @enum Precedence table indexes.
 */
typedef enum
{
    I_PLUS_MINUS,		/// 0 +-
    I_MUL_DIV,			/// */
    I_REL_OP,			/// r
    I_LEFT_BRACKET,		/// (
    I_RIGHT_BRACKET,	/// )
    I_DATA,				/// i
    I_DOLLAR			/// $
} Prec_table_index_enum;


/**
 * Implementation of <expression> rule. Parses expressions.
 *
 * @param data Pointer to parser's internal data.
 * @return Given exit code.
 */
int expression(tDList *list, struct tToken *save_location);

#endif //PROJECT_EXPRESSION_H
