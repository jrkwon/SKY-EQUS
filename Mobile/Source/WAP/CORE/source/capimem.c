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

#include "capimem.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    MEMc_initCache (UINT32 cacheSize, UINT32 restoredSize)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_initCache\nUINT32 \t cacheSize \t %u\nUINT32 \t restoredSize \t %u\n", cacheSize, restoredSize);
#endif

    pMsg->Id = eMEMc_initCache;

    pMsg->vUINT32[0] = cacheSize;
    pMsg->vUINT32[1] = restoredSize;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MEMc_prepareCache (UINT32 availablePersistentMemory)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_prepareCache\nUINT32 \t availablePersistentMemory \t %u\n", availablePersistentMemory);
#endif

    pMsg->Id = eMEMc_prepareCache;

    pMsg->vUINT32[0] = availablePersistentMemory;

    bSendMsg = SendMsg(pMsg);
}
