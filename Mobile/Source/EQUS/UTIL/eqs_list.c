/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqs_list.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-10-07 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

void EQS_API InitList(LIST_T *pList, void (*pfnDestroy)(void *pData))
{
    pList->nSize = 0;
    pList->pHead = pList->pTail = NULL;
    pList->pfnDestroy = pfnDestroy;
}

void EQS_API DestroyList(LIST_T *pList)
{
    void *pData;

    /* remove each element */
    while(GET_LIST_SIZE(pList) > 0)
    {
        RemoveListElement(pList, NULL, (void **)&pData);
        if(pList->pfnDestroy != NULL)
        {
            /* 
            ** call a user defined function to free dynamically 
            ** allocated data 
            */
            pList->pfnDestroy(pData);
        }
    }
    /* clear the list structure */
    memset(pList, 0, sizeof(pList));
}

void EQS_API RemoveListElement(LIST_T* pList, ELEMENT_T *pElement, void **ppData)
{
    ELEMENT_T *pOldElement;

    /* check an empty list */
    if(GET_LIST_SIZE(pList) == 0)
        return;

    /* remove the element from the list */
    if(pElement == NULL) /* remove from the head of the list */
    {
        *ppData = pList->pHead->pData;
        pOldElement = pList->pHead;
        pList->pHead = pList->pHead->pNext;

        if(GET_LIST_SIZE(pList) == 0)
            pList->pTail = NULL;
    }
    else /* remove from somewhere other than the head */
    {
        if(GET_LIST_NEXT(pElement) == NULL)
            return;

        *ppData = pElement->pNext->pData;
        pOldElement = pElement->pNext;
        pElement->pNext = pElement->pNext->pNext;

        if(pElement->pNext == NULL)
            pList->pTail = pElement;
    }
    /* free the storage allocated by teh abstract data type */
    EQS_Free((void**)(&pOldElement));

    /* decrease the size of the list */
    pList->nSize--;
}

void EQS_API InsertListElement(LIST_T *pList, ELEMENT_T *pElement, void *pData)
{
    ELEMENT_T *pNewElement;

    /* allocate storage for the element */
    pNewElement = (ELEMENT_T *)EQS_Malloc(sizeof(ELEMENT_T));
    //ASSERT(pNewElement);

    /* insert the element into the list */
    pNewElement->pData = (void *)pData;
    
    if(pElement == NULL) /* insert at the head of the list */
    {
        if(GET_LIST_SIZE(pList) == 0)
            pList->pTail = pNewElement;
        pNewElement->pNext = pList->pHead;
        pList->pHead = pNewElement;
    }
    else /* insert somewhere other than at the head */
    {
        if(GET_LIST_NEXT(pElement) == NULL)
            pList->pTail = pNewElement;
        pNewElement->pNext = pElement->pNext;
        pElement->pNext = pNewElement;
    }
    /* increase the size of the list */
    pList->nSize++;
}



