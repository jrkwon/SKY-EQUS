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
#ifndef _SI_RE_H
#define _SI_RE_H


#include "cmmnrsrc.h"
#include "si_bpi.h"
#include "si_ops.h"
#include "si_frame.h"
#include "si_fs.h"
#include "si_var.h"


typedef struct
{
	BYTE*					BP;
	UINT32				BPend; /* (BP offset points to the byte after the last byte of the BP */
	pstructBPInfo	BPI;
	pstructOpS		OpS;
	pstructFS			FS;
	UINT32				IP; /* BP offset */
	pstructFrame	currF;
} structRuntimeEnv, *pstructRuntimeEnv;


pstructRuntimeEnv RE_NewWithArgString( BYTE* BP, UINT32 BPlen, INT16 IANAcharset, BYTE* funcname, /* funcname must be UTF8 encoded */
		WCHAR* argString, WCHAR* refererUrl, WCHAR *baseUrl, enumErrorCode *errCode );
pstructRuntimeEnv RE_NewWithArgOpS( BYTE* BP, UINT32 BPlen, INT16 IANAcharset, BYTE* funcname,
		pstructOpS* ops, WCHAR* refererUrl, WCHAR *baseUrl, enumErrorCode *errCode );
/* the ops given is taken over by the re (NOT COPIED!!!) */
VOID RE_Delete( pstructRuntimeEnv *pThis);

pstructVar RE_GetResult( pstructRuntimeEnv thisx ); /* pops the top operand from the OpS */

BOOL RE_ReadInstruction( pstructRuntimeEnv thisx, BYTE* result );
BOOL RE_Read8bOperand( pstructRuntimeEnv thisx, UINT8* result );
BOOL RE_Read16bOperand( pstructRuntimeEnv thisx, UINT16* result );

BOOL RE_Jump( pstructRuntimeEnv thisx, INT32 offset );
pstructVar RE_Pop( pstructRuntimeEnv thisx );

/* _SI_RE_H */
#endif
