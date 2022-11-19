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
#ifndef CAPIUSSD_H
#define CAPIUSSD_H


#include "tapicmmn.h"


IMPORT_C VOID    USSDc_receivedResultProcessRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength);


IMPORT_C VOID    USSDc_receivedError (UINT8 message, BOOL isKept);


IMPORT_C VOID    USSDc_receivedInvokeRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength);


IMPORT_C VOID    USSDc_receivedRelease (VOID);


#endif
