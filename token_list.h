/**
 * @file token_list.h
 * @author
 * @brief
 */

#include "token.h"

typedef struct tDLElem {
    struct tToken token;
    struct tDLElem *lptr;
    struct tDLElem *rptr;
} *tDLElemPtr;

typedef struct {
    tDLElemPtr First;
    tDLElemPtr Act;
    tDLElemPtr Last;
} tToken_list;

bool add_to_token_list(tToken_list *token_list,struct tToken token);