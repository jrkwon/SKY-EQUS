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
#ifndef _SI_OPS_H
#define _SI_OPS_H


#include "cmmnrsrc.h"
#include "si_var.h"
#include "si_list.h"


typedef struct
{
	t_head head;
} structOpS, *pstructOpS;


pstructOpS OpS_New( VOID );
pstructOpS OpS_NewFromArgString( WCHAR* argString );
pstructOpS OpS_NewFromOpS( pstructOpS src, INT16 nbrToMove ); /* nbrToMove == -1 means the whole OpS */
VOID OpS_Delete( pstructOpS *pThis );

VOID OpS_Push( pstructOpS thisx, pstructVar *pVar );
pstructVar OpS_Pop( pstructOpS thisx );

UINT16 OpS_GetTopIndex( pstructOpS thisx );
BOOL OpS_ClearUntilIndex( pstructOpS thisx, UINT16 index );

/* _SI_OPS_H */
#endif
