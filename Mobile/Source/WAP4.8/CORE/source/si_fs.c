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
#include "si_fs.h"


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
pstructFS FS_New( VOID )
{
	pstructFS	fs = NEWSTRUCT( structFS );

	if (fs != NULL) {
		f_lh_newhead( &(fs->head) );

		if (fs->head == NULL) {
			DEALLOC( &fs );
		}
	}

	return fs;
}


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
VOID FS_Delete( pstructFS *pThis, pstructBPInfo bpi )
{
	t_link				link;
	pstructFrame	aFrame;

	if (*pThis != NULL) {

		while ( ! f_lh_empty((*pThis)->head) ) {
			link = f_lh_last((*pThis)->head);
			aFrame = (pstructFrame) link->data;
			Frame_Delete( &aFrame, bpi );
			f_lh_disposelink( &link );
		}
		f_lh_disposehead( &((*pThis)->head) );

		DEALLOC( pThis );
	}
}


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
VOID FS_Push( pstructFS thisx, pstructFrame *pFrame, UINT32 returnIP )
{
	t_link	newEl = NULL;

	if ((thisx != NULL) && (*pFrame != NULL)) {
		f_lh_newlink( &newEl );
		if (newEl != NULL) {
			newEl->data = *pFrame;
			(*pFrame)->returnIP = returnIP;
			*pFrame = NULL;
			f_lh_into( &newEl, &(thisx->head) );
		}
	}
}


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
pstructFrame FS_Pop( pstructFS thisx )
{
	t_link				link;
	pstructFrame	res = NULL;

	link = f_lh_last(thisx->head);
	if (link != NULL) {
		res = (pstructFrame) link->data;
		f_lh_disposelink( &link );
	}

	return res;
}

