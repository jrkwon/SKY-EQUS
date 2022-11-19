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
#ifndef _SI_FS_H
#define _SI_FS_H

#include "si_list.h"
#include "si_frame.h"
#include "si_bpi.h"

typedef struct
{
	t_head head;
} structFS, *pstructFS;


pstructFS FS_New( VOID );
VOID FS_Delete( pstructFS *pThis, pstructBPInfo bpi );

VOID FS_Push( pstructFS thisx, pstructFrame *pFrame, UINT32 returnIP );
pstructFrame FS_Pop( pstructFS thisx );


/* _SI_FS_H */
#endif
