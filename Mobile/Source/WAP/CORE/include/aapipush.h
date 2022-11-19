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
#ifndef AAPIPUSH_H
#define AAPIPUSH_H


#include "wipprdct.h"

#if defined CONFIG_PUSH

#include "tapicmmn.h"


#define PUSH_SI_LOW_PRIO    1
#define PUSH_SI_MEDIUM_PRIO 2
#define PUSH_SI_HIGH_PRIO   3

#define PUSH_STATUS_NON_LOADED   4
#define PUSH_STATUS_LOADED       5

#define PUSH_SL_CACHE_PRIO  1
#define PUSH_SL_LOW_PRIO    2
#define PUSH_SL_HIGH_PRIO   3




VOID    PUSHa_newSIReceived (INT16 id, UINT8 priority);

VOID    PUSHa_SIinfo (INT16 id, UINT8 status, UINT32 created, UINT32 expires, const WCHAR *message, BOOL expired, const CHAR *url, UINT8 priority, const CHAR *initURL);

VOID    PUSHa_newSLReceived (INT16 id, UINT8 priority);

VOID    PUSHa_SLinfo (INT16 id, UINT8 status, const CHAR *url, UINT8 priority, const CHAR *initURL);

VOID    PUSHa_messagechange (INT16 id, UINT8 change);

VOID    PUSHa_requestConnection (UINT8 siaId, UINT16 stackMode, UINT8 accessType, const CHAR *address, UINT8 addresslen);

#endif


#endif
