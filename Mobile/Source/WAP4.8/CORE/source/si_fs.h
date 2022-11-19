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
/***********************
File: si_fs.h + c
Author: Klas Hermodsson (KHN)

Description: (FS - Frame Stack)
The FS is a stack of frames (function contexts, see si_frame). 
The FS is used as a function call stack where the current frame is
pushed when a function call is made. A frame is popped and 
made the current function context when a function call is done.

NOTE! Since each SI (see si_si) handles only one script file and 
has a FS then the FS only consists of local function context within
the script file (BP). If a call is made to a function within another
BP then a new SI is created and it has its own FS.

The functions below only interact with the structFS itself. 
It does not change other structs. The rest of the logic which must 
be carried out when pushing and poping a frame is handled by the
modules that use this module.

************************/

#include "si_list.h"
#include "si_frame.h"
#include "si_bpi.h"

typedef struct
{
	t_head head;
} structFS, *pstructFS;


/*==========================================
	FS_New
============================================

---Purpose: 
To create and init a new frame stack.
	
---Params:

	
---Return:
pstructFS			the operation was successful
NULL					the operation failed

------------------------------------------------------------------------*/
pstructFS FS_New( VOID );

/*==========================================
	FS_Delete
============================================

---Purpose: 
To deallocate the FS and its components
The BPI is needed in order to dispose each frame 
(the frame must know how many vars (args+local vars)) it has 
so they can be properly deallocated.
	
---Params:
pThis					the FS to be deallocated
bpi						the BPI that the frames in the FS are based on
	
---Return:

------------------------------------------------------------------------*/
VOID FS_Delete( pstructFS *pThis, pstructBPInfo bpi );

/*==========================================
	FS_Push
============================================

---Purpose: 
Push a frame on a frame stack.
The returnIp is set.

The frame reference is then set to NULL so that the 
reference is not used later by mistake.
	
---Params:
thisx					the frame stack on which to push the frame
pFrame				the frame to push
IP						current IP so that the returnIP of the frame can be set.
	
---Return:

------------------------------------------------------------------------*/
VOID FS_Push( pstructFS thisx, pstructFrame *pFrame, UINT32 returnIP );


/*==========================================
	FS_Pop
============================================

---Purpose: 
Pop a frame from a frame stack
	
---Params:
thisx					the frame stack from which to pop a frame
	
---Return:
pstructFS			the popped stack entry
NULL					the stack was empty

------------------------------------------------------------------------*/
pstructFrame FS_Pop( pstructFS thisx );


/* _SI_FS_H */
#endif
