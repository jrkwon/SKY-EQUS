/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
#include "wiptrgt.h"

#if     defined NO_GLOBAL_VARS
#include "userdata.h"
#endif

#include "msgqueue.h"
#include "cmmnrsrc.h"


typedef struct  stQueueMsg
{
    Msg *pFirst;
    Msg *pLast;
} QueueMsg;


#ifndef NO_GLOBAL_VARS
/*  Global queue to be used in xInEnv() */
static  QueueMsg    s_QueueMsg;
static  BOOL    s_queueIsInitialised = FALSE;
#endif


int     InitQueue(void)
{
    s_QueueMsg.pFirst = NULL;
    s_QueueMsg.pLast = NULL;

    s_queueIsInitialised = TRUE;

    return TRUE;
}


void    DeleteQueue(void)
{
    /* the memory that is consumed in the queue has already been
       deleted by the function deleteCmmnRsrcData() in CmmnRsrs.c.
       The call was done in CLNTc_terminate, were this function
       is called from.
    */
    s_QueueMsg.pFirst = NULL;
    s_QueueMsg.pLast = NULL;

    s_queueIsInitialised = FALSE;
}


int     SendMsg(Msg *pMsg)
{
    if (s_queueIsInitialised && pMsg)
    {
        pMsg->pNext = NULL;
        if (s_QueueMsg.pFirst)
        {
            s_QueueMsg.pLast->pNext = pMsg;
        }
        else
        {
            s_QueueMsg.pFirst = pMsg;
        }
        s_QueueMsg.pLast = pMsg;

        return TRUE;
    }

    if (pMsg)
    {
        OSConnectorFree(pMsg);
    }

    return FALSE;
}


Msg *   GetMsg(void)
{
    Msg *pTemp;
#ifdef WAPCORE_FOR_WARNING
	pTemp = s_QueueMsg.pFirst;
	if (pTemp)
#else
    if (pTemp = s_QueueMsg.pFirst)
#endif //WAPCORE_FOR_WARNING
    {
        s_QueueMsg.pFirst = pTemp->pNext;
        if (s_QueueMsg.pLast == pTemp)
        {
            s_QueueMsg.pLast = NULL;
        }
    }

    return pTemp;
}


Msg *   FirstMsg(void)
{
    return s_QueueMsg.pFirst;
}
