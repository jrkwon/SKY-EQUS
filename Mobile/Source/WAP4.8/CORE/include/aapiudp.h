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
#ifndef AAPIUDP_H
#define AAPIUDP_H


#include "tapicmmn.h"


VOID    UDPa_sendRequest (const CHAR *data, UINT16 dataLength, const CHAR *destination, UINT8 destLength, const CHAR *source, UINT8 sourceLength, UINT16 destinationPort, UINT16 sourcePort, UINT8 objectId, UINT8 channelId);


#endif
