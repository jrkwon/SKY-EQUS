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
#ifndef AAPICLNT_H
#define AAPICLNT_H


#include "tapicmmn.h"
#include "wipprdct.h"
#include "confvars.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "tapiclnt.h"
#endif

VOID    CLNTa_terminated (VOID);

UINT32  CLNTa_currentTime (VOID);

VOID    CLNTa_setTimer (UINT32 timeInterval);

VOID    CLNTa_resetTimer (VOID);

VOID	CLNTa_requestConnection( UINT8 objectId );

VOID	CLNTa_setupConnection( UINT8 objectId, UINT8 channelID );

VOID	CLNTa_closeConnection( UINT8 objectId, UINT8 channelID );

VOID    CLNTa_getFile (UINT8 fileId, const CHAR *URL);

VOID    CLNTa_callFunction (UINT8 functionId, const CHAR *device, const CHAR *function, const CHAR *attributes);

VOID    CLNTa_nonSupportedScheme (UINT8 viewId, const CHAR *URL);

VOID    CLNTa_content (UINT8 urlID, const CHAR *data, UINT16 length, BOOL moreData, const CHAR *contentType, UINT32 totalSize, INT16 errorNo);

VOID    CLNTa_acknowledgePostContent (UINT8 urlID);

VOID    CLNTa_error (UINT8 viewId, INT16 errorNo, UINT8 errorType);

VOID    CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);

#ifdef USE_PROPRIETARY_WMLS_LIBS

BOOL    CLNTa_hasWMLSLibFunc (UINT16 libNbr, UINT8 funcNbr, UINT8 *nbrOfParams);

WMLSvar* CLNTa_callWMLSLibFunc (UINT16 invokeId, UINT16 libNbr, UINT8 funcNbr, const WMLSvar * const *params, BOOL *pSeparateResponse);

#endif

#endif
