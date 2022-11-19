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

#include "capisms.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    SMSc_sentRequest (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "SMSc_sentRequest\nVOID\n");
    }
#endif

    pMsg->Id = eSMSc_sentRequest;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    SMSc_receivedRequest (const CHAR *source, UINT8 sourceLength, const CHAR *data, UINT8 dataLength)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_source, *_data;
        char    *strSource;
        char    *strData;
        _data = strData = OctetString2HexString(data, dataLength);
        _source = strSource = OctetString2HexString(source, sourceLength);
        if (!_data)
        {
            _data = cszNULL;
        }
        if (!_source)
        {
            _source = cszNULL;
        }
        CLNTa_log(0, 0, "SMSc_receivedRequest\nconst CHAR * \t source \t %s\nUINT8 \t sourceLength \t %u\nconst CHAR * \t data \t %s\nUINT8 \t dataLength \t %u\n", _source, (int)sourceLength, _data, (int)dataLength);
        OSConnectorFree(strSource);
        OSConnectorFree(strData);
    }
#endif

    pMsg->Id = eSMSc_receivedRequest;

    pCHAR = NULL;
    if (sourceLength)
    {
        if (source)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(sourceLength);
            if (!pCHAR)
                return;
            memcpy(pCHAR, source, sourceLength);
        }
        else
        {
            sourceLength = 0;
        }
    }
    pMsg->pCHAR[0] = pCHAR;
    pMsg->vUINT8[0] = sourceLength;
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
    pMsg->pCHAR[1] = pCHAR;
    pMsg->vUINT8[1] = dataLength;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    SMSc_receivedError (UINT8 message)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "SMSc_receivedError\nUINT8 \t message \t %u\n", message);
    }
#endif

    pMsg->Id = eSMSc_receivedError;

    pMsg->vUINT8[0] = message;

    bSendMsg = SendMsg(pMsg);
}
