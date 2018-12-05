/**
 * @file token_list.c
 * @author Tomas Dorda (xdorda00)
 * @brief Token list functions and representation
 */

#include "token_list.h"

void DLInitList(tDList *L) {
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList(tDList *L) {
    while (L->First != NULL) {
        tDLElemPtr tmp = L->First;
        L->First = L->First->rptr;
        dynamic_string_free(tmp->token.content_string);
        free(tmp);
    }

    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertLast(tDList *L, struct tToken val) {
    tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
    if (newElement == NULL)
        ErrorPrint(INTERNAL_ERROR, "Error in token_list.c");
    else {
        newElement->token = val;
        newElement->lptr = L->Last;
        newElement->rptr = NULL;

        if (L->First == NULL)
            L->First = newElement;
        else
            L->Last->rptr = newElement;

        L->Last = newElement;
    }
}

void DLFirst(tDList *L) {
    L->Act = L->First;
}

void DLSucc(tDList *L) {
    if (L->Act != NULL)
        L->Act = L->Act->rptr;
}
