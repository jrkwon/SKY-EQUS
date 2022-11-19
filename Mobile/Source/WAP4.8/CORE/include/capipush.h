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
#ifndef CAPIPUSH_H
#define CAPIPUSH_H


#include "wipprdct.h"

#if defined CONFIG_PUSH

#include "tapicmmn.h"

#define PUSH_SHOW_ALL   1
#define PUSH_SHOW_EXP   2
#define PUSH_SHOW_NON_EXP   3
#define PUSH_SHOW_LOADED   4
#define PUSH_SHOW_NON_LOADED   5

#define PUSH_DEL_ALL   1
#define PUSH_DEL_EXP   2
#define PUSH_DEL_NON_EXP   3
#define PUSH_DEL_LOADED   4
#define PUSH_DEL_NON_LOADED   5


IMPORT_C VOID   PUSHc_loadSI (UINT8 objectId, INT16 id);

IMPORT_C VOID   PUSHc_deleteSI (INT16 id, UINT8 selection);

IMPORT_C VOID   PUSHc_getSIinfo (INT16 id, UINT8 selection);

IMPORT_C VOID   PUSHc_loadSL (UINT8 objectId, INT16 id);

IMPORT_C VOID   PUSHc_deleteSL (INT16 id, UINT8 selection);

IMPORT_C VOID   PUSHc_getSLinfo (INT16 id, UINT8 selection);

IMPORT_C VOID	PUSHc_checkIfAnyUnloadedMsg (VOID);

IMPORT_C VOID	PUSHc_changeStatus (INT16 id, UINT8 status);

IMPORT_C VOID   PUSHc_requestConnectionDone( UINT8 siaId, UINT8 channelId, BOOL success);

IMPORT_C VOID   PUSHc_removePushMessage(INT8 type);
#endif


#endif
