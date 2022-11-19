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
#ifndef _SI_BPI_H
#define _SI_BPI_H

/*************************************************

si_bpi.h

This file includes the datastructure definitions and
functions that deal with the BPI (bytecode package info)

**************************************************/


#include "cmmnrsrc.h"
#include "si_var.h"
#include "WMLDef.h"
	/* WMLDef.h is for the pUA type */


enum ConstType {
	CONST_TYPE_INT8					= 0,
	CONST_TYPE_INT16				= 1,
	CONST_TYPE_INT32				= 2,
	CONST_TYPE_FLOAT32			= 3,
	CONST_TYPE_UTF8					= 4,
	CONST_TYPE_ES						= 5,
	CONST_TYPE_EXT_CHARSET	= 6,
	CONST_TYPE_MAX
};

enum PragmaType {
	PRAGMA_ACCESS_DOMAIN						= 0,
	PRAGMA_ACCESS_PATH							= 1,
	PRAGMA_UA_PROP									= 2,
	PRAGMA_UA_PROP_SCHEME						= 3,			
	PRAGMA_MAX
};

typedef enum ConstType		enumConstType;
typedef enum PragmaType		enumPragmaType;


typedef struct
{
	UINT32	funcNamePos;	/* if the function is available outside this BP then it has a name (encoded in UTF8) */
	UINT8		nbrOfArgs;
	UINT8		nbrOfVars;
	UINT32	startPos;
	UINT32	endPos;
} structFuncInfo, *pstructFuncInfo;

typedef struct
{
/*  Modified by GBU,000103  use the proper enum instead of an int */
/*
	UINT8		type;
*/
    enumConstType   type;
    UINT32          pos;			/* pos to to the byte after the type */
} structConstInfo, *pstructConstInfo;

typedef struct
{
/*  Modified by GBU,000103  use the proper enum instead of an int */
/*
	UINT8		type;
*/
    enumPragmaType  type;
    UINT32          pos;
} structPragmaInfo, *pstructPragmaInfo;

typedef struct
{
	BYTE							version;
	UINT16						nbrOfConstants;
	INT16							characterSet;
  	/* a IANA MIBenum code corresponding to the encoding used for the
    CONST_TYPE_EXT_CHARSET strings */
	pstructConstInfo	constants;
	UINT16						nbrOfPragmas;
	pstructPragmaInfo	pragmas;
	UINT8							nbrOfFunctions;
	pstructFuncInfo		functions;
	
} structBPInfo, *pstructBPInfo;

pstructBPInfo BPI_New( BYTE* BP, UINT32 BPlen, INT16 IANAcharset ); 
/*
Creates a new BPI by parsing the BP.
If something went wrong NULL is returned (ERROR_VERIFICATION).
*/
VOID BPI_Delete( pstructBPInfo *pThis );
/*
Deletes the entire BPI
*/
enumErrorCode BPI_AccessCheck( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, BYTE* funcName,
		UINT8 nbrOfArgs, WCHAR* refererUrl, WCHAR *baseUrl, UINT8* funcIndexResult );
/*
Checks to see if we are entitled to call a function with funcName. The return value
is an error code. If we are entitled to access the function the funcIndexResult is
the function index of the function we want to call.
*/

VOID BPI_SendUApragmasToTheUA( pstructBPInfo thisx, BYTE* BP, UINT32 BPlen,
		pUA userAgent );
/*
Sees to it that the UA is given the information insida any UA pragmas resdiding
in the BP file.
*/

BOOL BPI_GetConstantType( pstructBPInfo thisx, UINT16 constIndex, enumConstType* constType );
/*
To get the constant type from the provided constant index.
Returns TRUE if successful, otherwise FALSE
*/

pstructVar BPI_GetConstant( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, UINT16 constIndex );
BYTE* BPI_GetRawUTF8Constant( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, UINT16 constIndex );


/* _SI_BPI_H */
#endif
