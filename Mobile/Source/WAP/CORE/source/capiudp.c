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

#include "capiudp.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    UDPc_receivedRequest (const CHAR *data, UINT16 dataLength, const CHAR *destination, UINT8 destLength, const CHAR *source, UINT8 sourceLength, UINT16 destinationPort, UINT16 sourcePort)
{
   

    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;

CLNTa_log(1,0,"receive tid1 :%x tid2 :%x type:%x",data[1],data[2],data[0]);
    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_data, *_destination, *_source;
        char    *strData, *strDestination, *strSource;
        _data = strData = OctetString2HexString(data, dataLength);
        _destination = strDestination = OctetString2HexString(destination, destLength);
        _source = strSource = OctetString2HexString(source, sourceLength);
        if (!_data)
        {
            _data = cszNULL;
        }
        if (!_destination)
        {
            _destination = cszNULL;
        }
        if (!_source)
        {
            _source = strSource;
        }
        CLNTa_log(0, 0, "UDPc_receivedRequest\nconst CHAR * \t data \t %s\nUINT16 \t dataLength \t %u\nconst CHAR * \t destination \t %s\nUINT8 \t destLength \t %u\nconst CHAR * \t source \t %s\nUINT8 \t sourceLength \t %u\nUINT16 \t destinationPort \t %u\nUINT16 \t sourcePort \t %u\n", _data, dataLength, _destination, (int)destLength, _source, (int)sourceLength, (int)destinationPort, (int)sourcePort);
        OSConnectorFree(strData);
        OSConnectorFree(strDestination);
        OSConnectorFree(strSource);
    }
#endif

    pMsg->Id = eUDPc_receivedRequest;

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
    pCHAR = NULL;
    if (destLength)
    {
        if (destination)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(destLength);
            if (!pCHAR)
                return;
            memcpy(pCHAR, destination, destLength);
        }
        else
        {
            destLength = 0;
        }
    }
    pMsg->pCHAR[1] = pCHAR;
    pMsg->vUINT8[0] = destLength;
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
    pMsg->pCHAR[2] = pCHAR;
    pMsg->vUINT8[1] = sourceLength;
    pMsg->vUINT16[1] = destinationPort;
    pMsg->vUINT16[2] = sourcePort;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    UDPc_errorRequest (UINT8 message, UINT16 destinationPort)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "UDPc_errorRequest\nUINT8 \t message \t %u\nUINT16 \t destinationPort \t %u\n", (int)message, (int)destinationPort);
    }
#endif

    pMsg->Id = eUDPc_errorRequest;

    pMsg->vUINT8[0] = message;
    pMsg->vUINT16[0] = destinationPort;

    bSendMsg = SendMsg(pMsg);
}
