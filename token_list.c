/**
 * @file token_list.c
 * @author
 * @brief
 */

#include "token_list.h"

void DLInitList (tDLList *L) {
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
    while (L->First != NULL) {
        tDLElemPtr tmp = L->First;
        L->First = L->First->rptr;  //presmerovani zacatku na prvek napravo
        free(tmp);
    }

    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
    tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
    if (newElement == NULL)
        DLError();
    else {
        newElement->data = val;
        newElement->lptr = NULL;
        newElement->rptr = L->First;

        if (L->First == NULL)
            L->Last = newElement; //jednina polozka => je zaroven i konec
        else
            L->First->lptr = newElement;

        L->First = newElement;
    }
}

void DLInsertLast(tDLList *L, int val) {
    tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
    if (newElement == NULL)
        DLError();
    else {
        newElement->data = val;
        newElement->lptr = L->Last;
        newElement->rptr = NULL;

        if (L->First == NULL)
            L->First = newElement;  //jednina polozka => je zaroven i zacatek
        else
            L->Last->rptr = newElement;

        L->Last = newElement;
    }
}

void DLFirst (tDLList *L) {
    L->Act = L->First;
}

void DLLast (tDLList *L) {
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
    if (L->First != NULL)
        *val = L->First->data;
    else
        DLError();
}

void DLCopyLast (tDLList *L, int *val) {
    if (L->First != NULL)
        *val = L->Last->data;
    else
        DLError();
}

void DLDeleteFirst (tDLList *L) {

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

void DLDeleteLast (tDLList *L) {
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

void DLPostDelete (tDLList *L) {
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

void DLPreDelete (tDLList *L) {
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

void DLPostInsert (tDLList *L, int val) {
    if (L->Act != NULL) {
        tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
        if (newElement == NULL)
            DLError();
        else {
            newElement->data = val;
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

void DLPreInsert (tDLList *L, int val) {
    if (L->Act != NULL) {
        tDLElemPtr newElement = malloc(sizeof(struct tDLElem));
        if (newElement == NULL)
            DLError();
        else {
            newElement->data = val;
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

void DLCopy (tDLList *L, int *val) {
    if (L->Act != NULL)
        *val = L->Act->data;
    else
        DLError();
}

void DLActualize (tDLList *L, int val) {
    if (L->Act != NULL)
        L->Act->data = val;
}

void DLSucc (tDLList *L) {
    if (L->Act != NULL)
        L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
    if (L->Act != NULL)
        L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
    return L->Act != NULL;
}