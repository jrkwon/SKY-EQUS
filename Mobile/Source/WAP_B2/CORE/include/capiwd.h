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
#ifndef CAPIWD_H
#define CAPIWD_H


#include "tapicmmn.h"


IMPORT_C VOID    WDc_receivedRequest (const CHAR *data, UINT16 dataLength, UINT16 destinationPort, UINT16 sourcePort);


IMPORT_C VOID    WDc_errorRequest (UINT8 message, UINT16 destinationPort);


#endif
