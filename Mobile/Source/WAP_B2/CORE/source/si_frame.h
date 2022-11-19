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
#ifndef _SI_FRAME_H
#define _SI_FRAME_H


#include "cmmnrsrc.h"
#include "si_var.h"
#include "si_bpi.h"
#include "si_ops.h"


typedef struct
{
	UINT8				funcIndex;				/* the index in the function pool. used to get more info from the BPinfo */
	pstructVar	*variables;		/* first function arguments then local variables */
	UINT32			returnIP;					/* is set when pushed on the FS */
	UINT16			returnSP;				/* is set when created (to SP - nbrOfArgs) */
} structFrame, *pstructFrame;


pstructFrame Frame_New( UINT8 funcIndex, pstructBPInfo BPI, pstructOpS OpS );
VOID Frame_Delete( pstructFrame *pThis, pstructBPInfo BPI );

/* _SI_FRAME_H */
#endif

