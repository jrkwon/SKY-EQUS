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

#include "capiwd.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    WDc_receivedRequest (const CHAR *data, UINT16 dataLength, UINT16 destinationPort, UINT16 sourcePort)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;

    if (!pMsg)
        return;

#ifdef LOG_EXTERNAL
    {
        const char  *_data;
        char    *strData;
        _data = strData = OctetString2HexString(data, dataLength);
        if (!_data)
        {
            _data = cszNULL;
        }
        CLNTa_log(0, 0, "WDc_receivedRequest\nconst CHAR * \t data \t %s\nUINT16 \t dataLength \t %u\nUINT16 \t destinationPort \t %u\nUINT16 \t sourcePort \t %u\n", _data, (int)dataLength, (int)destinationPort, (int)sourcePort);
        OSConnectorFree(strData);
    }
#endif

    pMsg->Id = eWDc_receivedRequest;

    pCHAR = NULL;
    if (dataLength)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(dataLength);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, dataLength);
        }
        else
        {
            dataLength = 0;
        }
    }
    pMsg->pCHAR[0] = pCHAR;
    pMsg->vUINT16[0] = dataLength;
    pMsg->vUINT16[1] = destinationPort;
    pMsg->vUINT16[2] = sourcePort;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    WDc_errorRequest (UINT8 message, UINT16 destinationPort)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WDc_errorRequest\nUINT8 \t message \t %u\nUINT16 \t destinationPort \t %u\n", (int)message, (int)destinationPort);
    }
#endif

    pMsg->Id = eWDc_errorRequest;

    pMsg->vUINT8[0] = message;
    pMsg->vUINT16[0] = destinationPort;

    bSendMsg = SendMsg(pMsg);
}
