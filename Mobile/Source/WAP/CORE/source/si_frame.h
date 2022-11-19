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
/***********************
File: si_frame.h + c
Author: Klas Hermodsson (KHN)

Description:
The structFrame is where the context of a function is stored.

Contents of the structFrame:

funcIndex
This is the index into the function pool of the script file (i.e. BP).
The BPI (see si_bpi) holds a function array where this index can be 
used to access more information on the function.

variables
An array where the values of function arguments and local variables are stored.
See si_var for more info on the variable struct.

returnIP (IP - Instruction Pointer)
When a function context is put on the call stack (i.e. frame stack, see si_fs) 
due to a call to another function, then this attribute will be set with
the IP (instruction pointer) value to which the IP should resume execution
when this function regains focus.

returnSP (SP - Stack Pointer)
This attribute is used to store the depth of the operand stack (OpS, see si_ops) when 
this function context was created. This depth is used to check so that the function 
does not pop too many values from the stack causing stack underflow.
It is also used when cleaning up on the operand stack after function completion 
(the operand stack must then be cleansed from any unused values that this function has
pushed on the operand stack).

************************/


#include "cmmnrsrc.h"
#include "si_var.h"
#include "si_bpi.h"
#include "si_ops.h"


typedef struct
{
	UINT8				funcIndex;  /* the index in the function pool. used to get more info from the BPinfo */
	pstructVar	*variables; /* first function arguments then local variables */
	UINT32			returnIP;   /* is set when pushed on the FS */
	UINT16			returnSP;   /* is set when created (to SP - nbrOfArgs) */
} structFrame, *pstructFrame;


pstructFrame Frame_New( UINT8 funcIndex, pstructBPInfo BPI, pstructOpS OpS );
VOID Frame_Delete( pstructFrame *pThis, pstructBPInfo BPI );

/* _SI_FRAME_H */
#endif

