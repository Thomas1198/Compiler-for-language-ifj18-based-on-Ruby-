/**
 * @file token_list.c
 * @author
 * @brief
 */

#include "token_list.h"

void DLInitList (tDList *L) {
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDList *L) {
    while (L->First != NULL) {
        tDLElemPtr tmp = L->First;
        L->First = L->First->rptr;
        dynamic_string_free(tmp->token.content_string);
        free(tmp);
    }

    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDList *L, struct tToken val) {
    tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
    if (newElement == NULL)
        ErrorPrint(INTERNAL_ERROR,"Error in token_list.c");

    else {
        newElement->token = val;
        newElement->lptr = NULL;
        newElement->rptr = L->First;

        if (L->First == NULL)
            L->Last = newElement;
        else
            L->First->lptr = newElement;

        L->First = newElement;
    }
}

void DLInsertLast(tDList *L, struct tToken val) {
    tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
    if (newElement == NULL)
        ErrorPrint(INTERNAL_ERROR,"Error in token_list.c");
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

void DLFirst (tDList *L) {
    L->Act = L->First;
}

void DLLast (tDList *L) {
    L->Act = L->Last;
}

void DLCopyFirst (tDList *L, struct tToken *val) {
    if (L->First != NULL)
        *val = L->First->token;
    else
        ErrorPrint(INTERNAL_ERROR,"Error in token_list.c");
}

void DLCopyLast (tDList *L, struct tToken *val) {
    if (L->First != NULL)
        *val = L->Last->token;
    else
        ErrorPrint(INTERNAL_ERROR,"Error in token_list.c");
}

void DLDeleteFirst (tDList *L) {

    if(L->First != NULL) {
        if (L->First == L->Act)
            L->Act = NULL;

        tDLElemPtr tmp = L->First;

        if (L->First == L->Last) {
            L->First = NULL;
            L->Last = NULL;
        }
        else {
            L->First = L->First->rptr;
            L->First->lptr = NULL;
        }

        free(tmp);
    }
}

void DLDeleteLast (tDList *L) {
    if(L->First != NULL) {
        if (L->Last == L->Act)
            L->Act = NULL;

        tDLElemPtr tmp = L->Last;

        if (L->First == L->Last) {
            L->First = NULL;
            L->Last = NULL;
        }
        else {
            L->Last = L->Last->lptr;
            L->Last->rptr = NULL;
        }

        free(tmp);
    }
}

void DLPostDelete (tDList *L) {
    if (L->First != NULL && L->Act != NULL && L->Act != L->Last) {
        tDLElemPtr tmp = L->Act->rptr;
        L->Act->rptr = tmp->rptr;
        if (tmp == L->Last)
            L->Last = L->Act;
        else
            tmp->rptr->lptr = L->Act;

        free(tmp);
    }
}

void DLPreDelete (tDList *L) {
    if (L->First != NULL && L->Act != NULL && L->Act != L->First) {
        tDLElemPtr tmp = L->Act->lptr;
        L->Act->lptr = tmp->lptr;
        if (tmp == L->First)
            L->First = L->Act;
        else
            tmp->lptr->rptr = L->Act;

        free(tmp);
    }
}

void DLPostInsert (tDList *L, struct tToken val) {
    if (L->Act != NULL) {
        tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
        if (newElement == NULL)
            ErrorPrint(INTERNAL_ERROR,"Error in token_list.c");
        else {
            newElement->token = val;
            newElement->lptr = L->Act;
            newElement->rptr = L->Act->rptr;

            L->Act->rptr = newElement;

            if (L->Act == L->Last)
                L->Last = newElement;
            else
                newElement->rptr->lptr = newElement;
        }
    }
}

void DLPreInsert (tDList *L, struct tToken val) {
    if (L->Act != NULL) {
        tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
        if (newElement == NULL)
            ErrorPrint(INTERNAL_ERROR,"Error in token_list.c");
        else {
            newElement->token = val;
            newElement->lptr = L->Act->lptr;
            newElement->rptr = L->Act;

            L->Act->lptr = newElement;

            if (L->Act == L->First)
                L->First = newElement;
            else
                newElement->lptr->rptr = newElement;
        }
    }
}

void DLCopy (tDList *L, struct tToken *val) {
    if (L->Act != NULL)
        *val = L->Act->token;
    else
        ErrorPrint(INTERNAL_ERROR,"Error in token_list.c");
}

void DLActualize (tDList *L, struct tToken val) {
    if (L->Act != NULL)
        L->Act->token = val;
}

void DLSucc (tDList *L) {
    if (L->Act != NULL)
        L->Act = L->Act->rptr;
}


void DLPred (tDList *L) {
    if (L->Act != NULL)
        L->Act = L->Act->lptr;
}

int DLActive (tDList *L) {
    return L->Act != NULL;
}