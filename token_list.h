/**
 * @file token_list.h
 * @author Tomas Dorda (xdorda00)
 * @brief Token list functions and representation
 */



#ifndef PROJECT_TOKEN_LIST_H
#define PROJECT_TOKEN_LIST_H

#include "token.h"
#include "error.h"


/**
 * @struct List element representation
 */
typedef struct tDLElem {
    struct tToken token;
    struct tDLElem *lptr;
    struct tDLElem *rptr;
} *tDLElemPtr;

/**
 * @struct List representation
 */
typedef struct {
    tDLElemPtr First;
    tDLElemPtr Act;
    tDLElemPtr Last;
} tDList;


/**
 * @brief Initicialization of list
 * @param L list of token
 */
void DLInitList(tDList *L);

/**
 * @brief delate list of token
 * @param L list of token
 */
void DLDisposeList(tDList *L);

/**
 * @brief insert token to end
 * @param L list of token
 * @param val token to insert
 */
void DLInsertLast(tDList *L, struct tToken val);

/**
 * @brief Activation first
 * @param L list of token
 */
void DLFirst(tDList *L);

/**
 * @brief Succrrement of list
 * @param L list of token
 */
void DLSucc(tDList *L);


#endif //PROJECT_TOKEN_LIST_H