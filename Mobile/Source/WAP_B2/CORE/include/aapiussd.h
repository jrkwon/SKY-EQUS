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
#ifndef AAPIUSSD_H
#define AAPIUSSD_H


#include "tapicmmn.h"


VOID    USSDa_sendInvokeProcessRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength);


VOID    USSDa_sendResultRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength);


VOID    USSDa_sendAbort (VOID);


#endif
