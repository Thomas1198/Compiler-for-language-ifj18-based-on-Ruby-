//
// Created by kony on 1.12.18.
//

#include "expression.h"

#define LS  -1
#define GR   1
#define EQ   0
#define ER  -99

#define num_of_tokens 14

////////////////////////////////////////////// *    /   +   -   (   )   i   $   <   >  <=  >=  ==  !=
int SA_table[num_of_tokens][num_of_tokens] = {
/*                                    " * " */{GR, GR, GR, GR, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                    " / " */
                                              {GR, GR, GR, GR, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                    " + " */
                                              {LS, LS, GR, GR, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                    " - " */
                                              {LS, LS, GR, GR, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                    " ( " */
                                              {LS, LS, LS, LS, LS, EQ, LS, ER, LS, LS, LS, LS, LS, LS},
/*                                    " ) " */
                                              {GR, GR, GR, GR, ER, GR, ER, GR, GR, GR, GR, GR, GR, GR},
/*                                    " i " */
                                              {GR, GR, GR, GR, ER, GR, ER, GR, GR, GR, GR, GR, GR, GR},
/*                                    " $ " */
                                              {LS, LS, LS, LS, LS, ER, LS, ER, LS, LS, LS, LS, LS, LS},
/*                                    " < " */
                                              {LS, LS, LS, LS, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                    " > " */
                                              {LS, LS, LS, LS, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                   " <= " */
                                              {LS, LS, LS, LS, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                   "  >=" */
                                              {LS, LS, LS, LS, LS, GR, LS, GR, GR, GR, GR, GR, GR, GR},
/*                                   " == " */
                                              {LS, LS, LS, LS, LS, GR, LS, GR, LS, LS, LS, LS, GR, GR},
/*                                   " != " */
                                              {LS, LS, LS, LS, LS, GR, LS, GR, LS, LS, LS, LS, GR, GR}};

int get_int_for_table(struct tToken token) {
    switch (token.set_type_of_token) {
        case CHAR_OPERATOR_MUL:
            return 0;
        case CHAR_OPERATOR_DIV:
            return 1;
        case CHAR_OPERATOR_PLUS:
            return 2;
        case CHAR_OPERATOR_MINUS:
            return 3;
        case CHAR_LEFT_BRACKET:
            return 4;
        case CHAR_RIGHT_BRACKET:
            return 5;
        case IDENTIFIER_NAME:
            return 6;
        case END_PRECE:
            return 7;
        case CHAR_LT:
            return 8;
        case CHAR_GT:
            return 9;
        case CHAR_LEQ:
            return 10;
        case CHAR_GEQ:
            return 11;
        case CHAR_DOUBLEEQ:
            return 12;
        case KEY_WORD_NOT:
            return 13;
        default:
            exit(2);
    }
}

int proces_expression(tDList *token_list) {

    Symstack *stack;

    stack = (Symstack *) malloc(sizeof(stack));

    stack_init(stack);
    struct tToken *a, *b, *tmp,*left,*right;

    tmp = malloc(sizeof(struct tToken));
    init_token(tmp);

    left = malloc(sizeof(struct tToken));
    init_token(left);

    right = malloc(sizeof(struct tToken));
    init_token(right);


    right->set_type_of_token=CHAR_GT;
    left->set_type_of_token=CHAR_LT;

    tmp->set_type_of_token = END_PRECE;
    stack_push(stack, tmp);

    DLFirst(token_list);
    b=token_list->Act;

    do {

        a=stack_get_top(stack);
        if(b!=NULL){
        *b=token_list->Act->token;
    }
        switch (SA_table[get_int_for_table(*a)][get_int_for_table(*b)]) {
            case EQ: {

                stack_push(stack,b);
                token_list->Act=token_list->Act->rptr;

                break;
            }
            case LS : {

                stack_push(stack,left);
                stack_push(stack,b);
                token_list->Act=token_list->Act->rptr;

                break;
            }
            case GR: {
                break;
            }
            default: {
                exit(99);
            }

        }
    } while (b->set_type_of_token == END_PRECE && stack_get_top(stack)->set_type_of_token == END_PRECE);


    return 0;
}


