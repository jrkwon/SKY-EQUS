/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqs_list.h
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
** ============================================================================
*/

#ifndef _EQS_LIST_H_
#define _EQS_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif
    
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
#define GET_LIST_SIZE(pList)            ((pList)->nSize)

#define GET_LIST_HEAD(pList)            ((pList)->pHead)
#define GET_LIST_TAIL(pList)            ((pList)->pTail)

#define GET_LIST_DATA(pElement)         ((pElement)->pData)
#define GET_LIST_NEXT(pElement)         ((pElement)->pNext)

#define IS_LIST_HEAD(pList, pElement)   ((pList)->pHead == (pElement) \
                                        ? TRUE : FALSE)
#define IS_LIST_TAIL(pElement)          ((pElement)->pNext == NULL \
                                        ? TRUE : FALSE)

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagELEMENT_T       ELEMENT_T;
typedef struct tagLIST_T          LIST_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
/* a structure for an element */
struct tagELEMENT_T {
    void            *pData;
    ELEMENT_T         *pNext;
};

/* a structure for a linked list */

struct tagLIST_T {
    UINT8           nSize;
    void            (*pfnDestroy)(void *pData);
    ELEMENT_T       *pHead;
    ELEMENT_T       *pTail;
};

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void EQS_API InitList(LIST_T *pList, void (*pfnDestroy)(void *pData));
extern void EQS_API DestroyList(LIST_T *pList);
extern void EQS_API InsertListElement(LIST_T *pList, ELEMENT_T *pElement, 
                                      void *pData);
extern void EQS_API RemoveListElement(LIST_T* pList, ELEMENT_T *pElement, 
                                      void **ppData);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_LIST_H_ */
