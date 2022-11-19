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
#ifndef TAPICLNT_H
#define TAPICLNT_H


#include "wiptrgt.h"
#include "wipprdct.h"

typedef struct
{
	UINT8	type;
	INT32	intVal;
	BOOL	boolVal;
	FLOAT32 floatVal;
	INT16	stringIANAcharset;
	UINT32	stringLengthInBytes;
	CHAR*	stringVal;
} WMLSvar;

#endif //TAPICLNT_H
