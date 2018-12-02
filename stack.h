/**
 * @file stack.h
 * @author Tomas Dorda (xdorda00)
 * @brief Error codes constants and function to print errors.
 */

#include <stdlib.h>

#ifndef PROJECT_STACK_H
#define PROJECT_STACK_H


#include <stdbool.h>

#include "error.h"
#include "symtable.h"
#include "token.h"

typedef enum {
    //priorita 1
    NASOBENI,       //0
    DELENI,         //1
//-----------------------------//
    //priorita 2
            SCITANI,        //2
    ODCITANI,       //3
//----------------------------//
    //priorita 3
    MENSI,           //4
    VETSI,           //5
    MENSIROVNO,      //6
    VETSIROVNO,      //7
//--------------------------//
    //priorita 4
    ROVNO,           //8 ==
    NEROVNO,         //9 !=
//---------------------------//
    //priorita 5
    LEVA,            //10 (
    PRAVA,           //11 )
//--------------------------//
    //priorita 6
    ID,             //12
//------------------------//
    KONEC,          //13 $
    CHYBA,          //14
//--------------------------//
    NETERMINAL,      //15
    OPER
} expression_type;


/**
 * @struct Stack item representation.
 */
typedef struct stack_item {
    expression_type expr;
    struct tToken *data;      /// pointer to token
    char *atribute;
    int reduce;
    int znaminko;       // < = >
    struct stack_item *next; /// pointer to next item
} Sitem;


/**
 * @struct Stack representation.
 */
typedef struct {
    Sitem *top; /// Pointer to stack item on top of stack.
} Symstack;


/**
 * Initializes stack.
 *
 * @param stack Pointer to stack.
 */
void stack_init(Symstack *stack);


/**
 * Pushes symbol to stack and sets its data type.
 *
 * @param stack Pointer to stack.
 * @param token token to be pushed.
 */
void stack_push(Symstack *stack, struct tToken *token);


/**
 * Pops top symbol from stack.
 *
 * @param stack Pointer to stack.
 * @return Top item or NULL if it does not exist
 */
Sitem *stack_pop(Symstack *stack);


/**
 * Checks if is stack empty.
 *
 * @param stack Pointer to stack.
 * @return True if empty, false if nonempty
 */
bool stack_is_empty(Symstack *stack);


/**
 * Frees resources used for stack.
 *
 * @param stack Pointer to stack.
 */
void stack_free(Symstack *stack);


#endif //PROJECT_STACK_H