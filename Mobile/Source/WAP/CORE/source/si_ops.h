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
/***********************
File: si_ops.h + c
Author: Klas Hermodsson (KHN)

Description: (OpS - Operand Stack)
The OpS is the WMLScript interpreter's operand stack.
Each RE (runtime environment, see si_re) has an OpS.
I.e. there is one OpS per script file. 

To stop functions within a script file of popping more 
operands from the OpS than it has pushed on the stack, a 
stack pointer (SP) is keeping track of this for each 
function context (see si_frame).



************************/


#include "cmmnrsrc.h"
#include "si_var.h"
#include "si_list.h"


typedef struct
{
	t_head head;
} structOpS, *pstructOpS;


/*==========================================
	OpS_New
============================================

---Purpose: 
To create and initialise an operand stack.
      
---Params:

               
---Return:
pstructOpS		the created OpS	
NULL					something went wrong

------------------------------------------------------------------------*/
pstructOpS OpS_New( VOID );


/*==========================================
	OpS_NewFromOpS
============================================

---Purpose: 
To create and initialise an operand stack. The created OpS is initialised by moving entries from
another OpS.
      
---Params:
src						The OpS to move from
nbrToCopy			how many entries from the top of the stack to be moved.
								If -1 is passed in this argument all entries from src are moved.
               
---Return:
pstructOpS		the created OpS	
NULL					something went wrong

------------------------------------------------------------------------*/
pstructOpS OpS_NewFromOpS( pstructOpS src, INT16 nbrToMove ); /* nbrToMove == -1 means the whole OpS */

/*==========================================
	OpS_NewFromArgString
============================================

---Purpose: 

	
---Params:

               
---Return:


------------------------------------------------------------------------*/
pstructOpS OpS_NewFromArgString( WCHAR* argString );

/*==========================================
	OpS_Delete
============================================

---Purpose: 
      
---Params:
               
---Return:

------------------------------------------------------------------------*/
VOID OpS_Delete( pstructOpS *pThis );

/*==========================================
	OpS_Push
============================================

---Purpose: 
To push a var on an OpS.
	
---Params:
thisx			the OpS to operate on
pVar			the var to be push.
						The reference will afterwards be set to NULL to avoid
						misuse of the reference.
	
---Return:

------------------------------------------------------------------------*/
VOID OpS_Push( pstructOpS thisx, pstructVar *pVar );

/*==========================================
	OpS_Pop
============================================

---Purpose: 
To pop a var from an OpS.
	
---Params:
thisx					the OpS to operate on
	
---Return:
pstructVar		the top stack element
NULL					the stack is empty 

------------------------------------------------------------------------*/
pstructVar OpS_Pop( pstructOpS thisx );

/*==========================================
	OpS_GetTopIndex
============================================

---Purpose: 
To get the index of the top element 
(i.e. how many elements are in the stack).
Example: If one element is is the stack the topindex is 1.
If the stack is empty zero is returned.
	
---Params:
thisx					the OpS to operate on
	
---Return:
UINT16				the index of the top stack element

------------------------------------------------------------------------*/
UINT16 OpS_GetTopIndex( pstructOpS thisx );

/*==========================================
	OpS_ClearUntilIndex
============================================

---Purpose: 
To pop stack entries until there is <index> entries left in the stack.
(Needed for cleaning up the OpS after function return statement)
	
---Params:
thisx					the OpS to operate on
index					which index the top entry in the stack must have after 
								the operation is done
	
---Return:
TRUE					the operation was successful
FALSE					the operation failed (e.g. less than <index> entries in the stack)

------------------------------------------------------------------------*/
BOOL OpS_ClearUntilIndex( pstructOpS thisx, UINT16 index );

/* _SI_OPS_H */
#endif
