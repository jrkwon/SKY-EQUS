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

/***********************
File: si_bpi.h + c
Author: Klas Hermodsson (KHN)

Description:
BPI stands for Bytecode Pacakage Information. A BP is the same thing as 
a compiled wmlscript file.

The BPI (structBPInfo) is a struct which itself contains a number of substructs. 
The information in the BPI struct is gotten by parsing the BP and collecting
important data from it. The BPI is there as an table of contents to the BP and mostly 
contains hhere in the BP info on functions and constants can be found.

BPI functions:
The important functions of the BPI include construction and deallocation 
of the BPI structure. There are also functions for retrieval of constant types and
values as well as checking to see if this BP has been called by a resource that
comply with access restrictions (if any).
************************/


#include "cmmnrsrc.h"
#include "si_var.h"
#include "wmldef.h"
	/* WMLDef.h is for the pUA type */


/*
The different constant types. Each constant in the constant pool of the BP must
be of one, and one only, constant type.
*/
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

/*
The different pragma types. Each pragma in the pragma pool of the BP must
be of one, and one only, pragma type.
*/
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
	UINT32	funcNamePos;	/* if the function is available outside this BP then it has a name (encoded in UTF8). */
	UINT8		nbrOfArgs;
	UINT8		nbrOfVars;
	UINT32	startPos;
	UINT32	endPos;
} structFuncInfo, *pstructFuncInfo;

typedef struct
{
	enumConstType   type;
	UINT32					pos;			/* pos to to the byte after the type (i.e. points to the first byte of the value (empty string does not need more than the type)) */
} structConstInfo, *pstructConstInfo;

typedef struct
{
	enumPragmaType  type;
	UINT32					pos;
} structPragmaInfo, *pstructPragmaInfo;

typedef struct
{
	BYTE							version;
	UINT16						nbrOfConstants;
	INT16							characterSet;
  	/* a IANA MIBenum code corresponding to the encoding used for the
    CONST_TYPE_EXT_CHARSET strings */
	pstructConstInfo	constants; /* array of structConstInfo */
	UINT16						nbrOfPragmas;
	pstructPragmaInfo	pragmas; /* array of structPragmaInfo */
	UINT8							nbrOfFunctions;
	pstructFuncInfo		functions; /* array of structFuncInfo */
	
} structBPInfo, *pstructBPInfo;



/*==========================================
	BPI_New
============================================

---Purpose:
To create and init a BPI.
The BP will be parsed and the BPI will be filled out with all the info
that can be derived from the BP.

---Params:
BP								the bytecode package which is to be parsed
BPlen							the length (in bytes) of the BP
IANAcharset				string encoding within the BP file according to the WSP header

---Return:
pstructBPInfo			the BPI was successfully created and initialised.
NULL							the operation failed.

------------------------------------------------------------------------*/
pstructBPInfo BPI_New( BYTE* BP, UINT32 BPlen, INT16 IANAcharset ); 

/*==========================================
	BPI_Delete
============================================

---Purpose: 
To deallocate a BPI and its components.

	Note! The BPI pointer will be set to NULL after deallocation so that 
	it's not possible to use the pointer by mistake.
      
---Params:
pThis				a reference to the BPI to delete
               
---Return:


------------------------------------------------------------------------*/
VOID BPI_Delete( pstructBPInfo *pThis );

/*==========================================
	BPI_AccessCheck
============================================

---Purpose:
To check if we are allowed to execute and call a function in this bytecode package (BP).
if everything went ok the funcIndexResult parameter will contain the function index of 
the function that we want to call.
	
---Params:
basePath		used to resolve the path specified in the access path pragma
							to an absolute path

---Return:				

------------------------------------------------------------------------*/
enumErrorCode BPI_AccessCheck( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, BYTE* funcName,
		UINT8 nbrOfArgs, WCHAR* refererUrl, WCHAR *baseUrl, UINT8* funcIndexResult );


/*==========================================
	BPI_SendUApragmasToTheUA
============================================

---Purpose:
Sees to it that the UA is given the information insida any UA pragmas resdiding
in the BP file.

---Params:
thisx					the BP to operate on
BP						a pointer to the compiled script
BPlen					the length (in bytes) of BP
userAgent			the struct of the user agent that invoked this script

---Return:

------------------------------------------------------------------------*/
VOID BPI_SendUApragmasToTheUA( pstructBPInfo thisx, BYTE* BP, UINT32 BPlen,
		pUA userAgent );

/*==========================================
	BPI_GetConstantType
============================================

---Purpose:
To retrieve the constant type of a constant with the index <constIndex>.

---Params:
thisx					the BP to operate on
constIndex		the index of the constant that we want the type of
constType			after a successfull call this parameter will hold the type of the constanr

---Return:
TRUE					the call was successful and the type is found in the constType parameter above
FALSE					the operation failed

------------------------------------------------------------------------*/
BOOL BPI_GetConstantType( pstructBPInfo thisx, UINT16 constIndex, enumConstType* constType );

/*==========================================
	BPI_GetConstant
============================================

---Purpose:
To retrieve the constant and return its value as a var.
	
---Params:
thisx					the BP to operate on
constIndex		the index of the constant that we want to get the value of
	
---Return:				
pstructVar		the call was successful
NULL					the operation failed

------------------------------------------------------------------------*/
pstructVar BPI_GetConstant( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, UINT16 constIndex );

/*==========================================
	BPI_GetRawUTF8Constant
============================================

---Purpose:
To retrieve an UTF8 string constant and return it unconverted 
as an null terminated string
	
---Params:
thisx					the BP to operate on
constIndex		the index of the constant that we want to get the value of
	
---Return:				
BYTE*					a pointer to the UTF8 string constant
NULL					not an UTF8 string constant

------------------------------------------------------------------------*/
BYTE* BPI_GetRawUTF8Constant( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, UINT16 constIndex );


/* _SI_BPI_H */
#endif

