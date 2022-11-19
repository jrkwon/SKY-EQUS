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
#include "ml_typw.h" 
#include "wap.ifc"
#include "capiudp.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    UDPc_receivedRequest (const CHAR *data, UINT16 dataLength, const CHAR *destination, UINT8 destLength, const CHAR *source, UINT8 sourceLength, UINT16 destinationPort, UINT16 sourcePort, UINT8 newChannelId)
{
	XMK_SEND_TMP_VARS

    yPDef_RecUDPDatagram var;
    CHAR    *pCHAR;

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

#ifdef FEATURE_WAP
        CLNTa_log(0, 0, "UDPc_receivedRequest\nUINT16 \t dataLength \t %u\nconst CHAR * \t destination \t %s\nUINT8 \t destLength \t %u\nconst CHAR * \t source \t %s\nUINT8 \t sourceLength \t %u\nUINT16 \t destinationPort \t %u\nUINT16 \t sourcePort \t %u\nUINT8 \t newChannelId \t %u\n", dataLength, _destination, (int)destLength, _source, (int)sourceLength, (int)destinationPort, (int)sourcePort, (int)newChannelId);
#else
        CLNTa_log(0, 0, "UDPc_receivedRequest\nconst CHAR * \t data \t %s\nUINT16 \t dataLength \t %u\nconst CHAR * \t destination \t %s\nUINT8 \t destLength \t %u\nconst CHAR * \t source \t %s\nUINT8 \t sourceLength \t %u\nUINT16 \t destinationPort \t %u\nUINT16 \t sourcePort \t %u\nUINT8 \t newChannelId \t %u\n", _data, dataLength, _destination, (int)destLength, _source, (int)sourceLength, (int)destinationPort, (int)sourcePort, (int)newChannelId);
#endif
        OSConnectorFree(strData);
        OSConnectorFree(strDestination);
        OSConnectorFree(strSource);
    }
#endif

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
    var.Param1 = pCHAR;
    var.Param2 = dataLength;

	devaddr_store(&(var.Param3), (BYTE*)destination, destLength);
    devaddr_store(&(var.Param4), (BYTE*)source, sourceLength);

    var.Param5 = destinationPort;
    var.Param6 = sourcePort;
    var.Param7 = newChannelId;

    XMK_SEND_ENV( ENV,
                  RecUDPDatagram,
                  xDefaultPrioSignal,
                  sizeof( yPDef_RecUDPDatagram ),
                  &var,
                  GLOBALPID(XPTID_WDPhandler,0));
}


EXPORT_C VOID    UDPc_errorRequest (UINT8 message, UINT16 destinationPort)
{
	XMK_SEND_TMP_VARS

    yPDef_UDPErrorInd var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "UDPc_errorRequest\nUINT8 \t message \t %u\nUINT16 \t destinationPort \t %u\n", (int)message, (int)destinationPort);
    }
#endif

    var.Param2 = message;
    var.Param1 = destinationPort;

    XMK_SEND_ENV( ENV,
                  UDPErrorInd,
                  xDefaultPrioSignal,
                  sizeof( yPDef_UDPErrorInd ),
                  &var,
                  GLOBALPID(XPTID_WDPhandler,0));
}
