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
#include "CmmnRsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    MEMc_initCache (memSizeType cacheSize, memSizeType restoredSize)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_initCache\nmemSizeType \t cacheSize \t %u\nmemSizeType \t restoredSize \t %u\n", cacheSize, restoredSize);
#endif

    pMsg->Id = eMEMc_initCache;

    pMsg->memSize[0] = cacheSize;
    pMsg->memSize[1] = restoredSize;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MEMc_prepareCache (memSizeType availablePersistentMemory)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_prepareCache\nmemSizeType \t availablePersistentMemory \t %u\n", availablePersistentMemory);
#endif

    pMsg->Id = eMEMc_prepareCache;

    pMsg->memSize[0] = availablePersistentMemory;

    bSendMsg = SendMsg(pMsg);
}
