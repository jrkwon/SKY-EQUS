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

#include "capiussd.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    USSDc_receivedResultProcessRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;

	if (pMsg == NULL)
		return;

#if     defined LOG_EXTERNAL
    {
        const char  *_data;
        char    *strData;
        _data = cszNULL;
        if (stringLength)
        {
            if (data)
            {
                _data = strData = OctetString2HexString(data, stringLength);
            }
        }
        CLNTa_log(0, 0, "USSDc_receivedResultProcessRequest\nUINT8 \t dataCodingScheme \t %u\nconst CHAR * \t data \t %s\nINT8 \t stringLength \t %i\n", dataCodingScheme, _data, (int)stringLength);
        OSConnectorFree(strData);
    }
#endif

    pMsg->Id = eUSSDc_receivedResultProcessRequest;

    pMsg->vUINT8[0] = dataCodingScheme;
    pCHAR = NULL;
    if (stringLength)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(stringLength);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, stringLength);
        }
        else
        {
            stringLength = 0;
        }
    }
    pMsg->vUINT8[1] = stringLength;
    pMsg->pCHAR[0] = pCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    USSDc_receivedError (UINT8 message, BOOL isKept)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

	if (pMsg == NULL)
		return;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "USSDc_receivedError\nUINT8 \t message \t %u\nBOOL \t isKept \t %u\n", (int)message, (int)isKept);
#endif

    pMsg->Id = eUSSDc_receivedError;

    pMsg->vUINT8[0] = message;
    pMsg->vBOOL = isKept;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    USSDc_receivedInvokeRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;

	if (pMsg == NULL)
		return;

#if     defined LOG_EXTERNAL
    {
        const char  *_data;
        char    *strData;
        _data = cszNULL;
        if (stringLength)
        {
            if (data)
            {
                _data = strData = OctetString2HexString(data, stringLength);
            }
        }
        CLNTa_log(0, 0, "USSDc_receivedInvokeRequest\nUINT8 dataCodingScheme \t %u\nconst CHAR * \t data \t %s\nINT8 \t stringLength \t %i\n", (int)dataCodingScheme, _data, (int)stringLength);
        OSConnectorFree(strData);
    }
#endif

    pMsg->Id = eUSSDc_receivedInvokeRequest;

    pMsg->vUINT8[0] = dataCodingScheme;
    pCHAR = NULL;
    if (stringLength)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(stringLength);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, stringLength);
        }
        else
        {
            stringLength = 0;
        }
    }
    pMsg->vUINT8[0] = stringLength;
    pMsg->pCHAR[0] = pCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    USSDc_receivedRelease (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

	if (pMsg == NULL)
		return;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "USSDc_receivedRelease\nVOID\n");
#endif

    pMsg->Id = eUSSDc_receivedRelease;

    bSendMsg = SendMsg(pMsg);
}
