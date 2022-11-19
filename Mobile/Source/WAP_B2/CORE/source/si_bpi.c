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
/*********
History
-------
990526 (KHN) Corrigendum WAG-WMLScript-28 implemented.
               Access control changes. Since access control is by default
               disabled, there is no need for a certain pragma for this and it
               has therefore been removed.
990726 (KHN) Error in BPI_New
               First the IANAcharset parameter was used to update the BPI struct's
               charset parameter. Then the BPI struct was updated and also the
               charset parameter. Then finally a charset check was made but not
               with the BPI struct as it should but instead with the IANAcharset
               parameter. This error has now been corrected.
990824 (KHN) Correction of BPI_AccessCheck:
               Now the function properly handles relative paths.
               An access path that is not relative is resolved and then the path
               is extracted.
**********/

#include "si_bpi.h"

#include "si_types.h"
#include "URL.h"
#include "WMLIF.h"
#include "trnscode.h"

/******* private functions *******/


BOOL ParseAndCheckVersion( pstructBPInfo bpi, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
	BOOL	allOk = TRUE;
	BYTE	version;
	BYTE	majorVersion;
	BYTE	minorVersion;

	if ( ReadByte( &version, BP, BPend, pos ) )
	{
		majorVersion = (BYTE)( (version >> 4) + 1 );
		minorVersion = (BYTE)( (version & 0x0F) );

		allOk = (BOOL)( ( majorVersion == SI_MAJOR_VERSION ) && (minorVersion <= SI_MINOR_VERSION) );
	}
	else allOk = FALSE;

	if (allOk)
	{
		bpi->version = version;
	}

	return allOk;
}



BOOL ParseAndCheckSize( BYTE* BP, UINT32 BPend, UINT32 *pos )
{
	BOOL		allOk = TRUE;
	UINT32	size = 0;

	if ( ReadMBUInt32( &size, BP, BPend, pos ) )
	{
		allOk = (BOOL)( ( size == (UINT32)(BPend - *pos) ) );
	}
	else allOk = FALSE;

	return allOk;
}


BOOL ParseAndCheckConstant( pstructBPInfo bpi, pstructConstInfo pInfo, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
	BOOL		allOk = TRUE;
	UINT32	strSize;
	UINT8		constType;

  bpi = bpi;	/* to get rid of compilation warning */

	if ( ReadUInt8( &constType, BP, BPend, pos ) )
	{
		allOk = (BOOL)( constType < CONST_TYPE_MAX );
	}
	else allOk = FALSE;

	if (allOk)
  {
/*  Modified by GBU,000103  added cast */
/*
 		pInfo->type = constType;
*/
        pInfo->type = (enumConstType)constType;
		pInfo->pos = *pos;

		switch(constType)
    {
            case CONST_TYPE_INT8:
/*  Modified by GBU,000103  removed one unused parameter */
/*
				allOk = StepBytes( sizeof(INT8), BP, BPend, pos );
*/
                allOk = StepBytes( sizeof(INT8), BPend, pos );
                break;


            case CONST_TYPE_INT16:
/*  Modified by GBU,000103  removed one unused parameter */
/*
				allOk = StepBytes( sizeof(INT16), BP, BPend, pos );
*/
                allOk = StepBytes( sizeof(INT16), BPend, pos );
                break;


            case CONST_TYPE_INT32:
/*  Modified by GBU,000103  removed one unused parameter */
/*
				allOk = StepBytes( sizeof(INT32), BP, BPend, pos );
*/
                allOk = StepBytes( sizeof(INT32), BPend, pos );
                break;


            case CONST_TYPE_FLOAT32:
/*  Modified by GBU,000103  removed one unused parameter */
/*
				allOk = StepBytes( sizeof(FLOAT32), BP, BPend, pos );
*/
                allOk = StepBytes( sizeof(FLOAT32), BPend, pos );
                break;

			case CONST_TYPE_UTF8:
			case CONST_TYPE_EXT_CHARSET:
				allOk = ReadMBUInt32( &strSize, BP, BPend, pos );
				if (allOk)
                {
/*  Modified by GBU,000103  removed one unused parameter */
/*
					allOk = StepBytes( strSize, BP, BPend, pos );
*/
                    allOk = StepBytes( strSize, BPend, pos );
                }
				break;
		}
	}

	return allOk;
}


BOOL ParseAndCheckConstantPool( pstructBPInfo bpi, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
	BOOL		allOk = TRUE;
	UINT16	nbrOfConsts = 0;
	UINT16	charSet = 0;
  UINT16	i;
	pstructConstInfo			pInfo = NULL;

	allOk = ReadMBUInt16( &nbrOfConsts, BP, BPend, pos );
	if (allOk)
  {
		bpi->nbrOfConstants = nbrOfConsts;
	}

	allOk = ReadMBUInt16( &charSet, BP, BPend, pos );
	if (allOk && (charSet != 0))
  {
  	/* if charSet is 0, it means that the encoding of
  	the CONST_TYPE_EXT_CHARSET is given in the WSP header.
    If no encoding is given there either, the
    default charSet will be assumed */
		bpi->characterSet = charSet;
	}

	if (allOk && (nbrOfConsts > 0))
  {
		bpi->constants = NEWARRAY( structConstInfo, nbrOfConsts );

		i = 0;
		pInfo = bpi->constants;
		while ( allOk && (i < nbrOfConsts) )
    {
			allOk = ParseAndCheckConstant( bpi, pInfo, BP, BPend, pos );
			i++;
			pInfo++;
		}
	}
	else bpi->constants = NULL;

	return allOk;
}


BOOL ParseAndCheckPragma( pstructBPInfo bpi, pstructPragmaInfo pInfo, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
	BOOL							allOk = TRUE;
	UINT8							pragmaType;
	UINT16						aParam;
	UINT8							nbrOfParams;
	UINT8							i;
	enumConstType			constType;


	if ( ReadUInt8( &pragmaType, BP, BPend, pos ) )
	{
		allOk = ((BOOL)( pragmaType < PRAGMA_MAX ));
	}
	else allOk = FALSE;

	if (allOk) {

/*  Modified by GBU,000103  added cast */
/*
		pInfo->type = pragmaType;
*/
        pInfo->type = (enumPragmaType)pragmaType;
        pInfo->pos = *pos;
#ifdef WAPCORE_FOR_WARNING
		nbrOfParams = 0;
#endif //#ifdef WAPCORE_FOR_WARNING

		switch(pragmaType) {
			case PRAGMA_ACCESS_DOMAIN:
			case PRAGMA_ACCESS_PATH:
				nbrOfParams = 1;
				break;
			case PRAGMA_UA_PROP:
				nbrOfParams = 2;
				break;
			case PRAGMA_UA_PROP_SCHEME:
				nbrOfParams = 3;
				break;
		}

		i = 0;
		while ( (i<nbrOfParams) && allOk )
    {
			allOk = ReadMBUInt16( &aParam, BP, BPend, pos );
			if (allOk)
      {
				if ( BPI_GetConstantType( bpi, aParam, &constType ) )
        {
					allOk = (BOOL)((constType >= CONST_TYPE_UTF8) &&
          	(constType <= CONST_TYPE_EXT_CHARSET));
					/* each pragma has a set of params, each param is a string constant
					a check must be made to ensure the param which is an index into the constant pool
					is a legal string constant */
				}
				else allOk = FALSE;
			}
			i++;
		}

	}

	return allOk;
}


BOOL ParseAndCheckPragmaPool( pstructBPInfo bpi, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
	BOOL									allOk = TRUE;
	UINT16								nbrOfPragmas = 0;
	UINT16								i;
	pstructPragmaInfo			pInfo = NULL;

	allOk = ReadMBUInt16( &nbrOfPragmas, BP, BPend, pos );

	if (allOk) {
		bpi->nbrOfPragmas = nbrOfPragmas;
	}

	if (allOk && nbrOfPragmas) {
		bpi->pragmas = NEWARRAY( structPragmaInfo, nbrOfPragmas );

		i = 0;
		pInfo = bpi->pragmas;
		while ( allOk && (i < nbrOfPragmas) ) {
			allOk = ParseAndCheckPragma( bpi, pInfo, BP, BPend, pos );
			i++;
			pInfo++;
		}
	}
	else bpi->pragmas = NULL;

	return allOk;
}


BOOL CheckFunctionName( BYTE* funcName, UINT8 strLen )
/* checks that a UTF8 encoded string is a valid function name */
{
	UINT8		i;
	BOOL		ok = TRUE;

	ok =  ((BOOL)( ( (funcName[0] >= 'a') && (funcName[0] <= 'z') )  ||
				( (funcName[0] >= 'A') && (funcName[0] <= 'Z') )  || (funcName[0] == '_') ));

	i = 1;
	while ((i < strLen) && ok) {
		ok =  (BOOL)( ( (funcName[i] >= 'a') && (funcName[i] <= 'z') )  ||
					( (funcName[i] >= 'A') && (funcName[i] <= 'Z') )  || 
					(funcName[i] == '_') ||
					( (funcName[i] >= '0') && (funcName[i] <= '9') ) );
		i++;
	}

	return ok;
}


BOOL ParseAndCheckFunctionPool( pstructBPInfo bpi, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
	BOOL		allOk = TRUE;
	UINT8		nbrOfFunc;
	UINT8		nbrOfPubFunc;		/* number of funcs in func name list */
	UINT8		funcIndex;
	UINT8		nameSize;
	UINT8		nbrOfArgs;
	UINT8		nbrOfLocVars;
	UINT32	funcSize;
	UINT8		i;

	allOk = ReadUInt8( &nbrOfFunc, BP, BPend, pos );
	if (allOk) {
		allOk = (BOOL)( nbrOfFunc > 0 );
	}

	if (allOk) {

		bpi->nbrOfFunctions = nbrOfFunc;
		bpi->functions = NEWARRAY( structFuncInfo, nbrOfFunc );

		/* Start of function name list */

		/* first start with setting funcNamePos of the functions to 0 */
		i = 0;
		while (i < nbrOfFunc) {
			bpi->functions[i].funcNamePos = 0;
			i++;
		}

		allOk = ReadUInt8( &nbrOfPubFunc, BP, BPend, pos );
		if (allOk) {
			allOk = (BOOL)( (nbrOfPubFunc > 0) && (nbrOfPubFunc <= nbrOfFunc) );
		}

		if (allOk) {
			i = 0;
			while ((i < nbrOfPubFunc) && allOk) {
				allOk = ReadUInt8( &funcIndex, BP, BPend, pos );
				if (allOk) {
					allOk = (BOOL)( funcIndex < nbrOfFunc );
				}
				if (allOk) {
					bpi->functions[funcIndex].funcNamePos = *pos;
					/* the funcNamePos is used by Accesscheck to search for the function with a certain function name */

					allOk = ReadUInt8( &nameSize, BP, BPend, pos );
				}

                if (allOk)  {
/*  Modified by GBU,000103  removed one unused parameter */
/*
					allOk = StepBytes( nameSize, BP, BPend, pos );
*/
                    allOk = StepBytes( nameSize, BPend, pos );
                    /* step the bytes to see that the BP doesn't end abruptly */
				}
				if (allOk) {
					allOk = CheckFunctionName( (BP + *pos - nameSize), nameSize );
					/* check that it is a legal func name string (UTF8 encoded) */
				}
				i++;
			}
		}

		if (allOk) {

			/* Start of functions section (func prologue and and code array)  */

			i = 0;
			while ((i < nbrOfFunc) && allOk) {
				allOk = ReadUInt8( &nbrOfArgs, BP, BPend, pos );
				if (allOk) {
					bpi->functions[i].nbrOfArgs = nbrOfArgs;

					allOk = ReadUInt8( &nbrOfLocVars, BP, BPend, pos );
				}
				if (allOk) {
					bpi->functions[i].nbrOfVars = nbrOfLocVars;

					allOk = (BOOL)( (nbrOfArgs + nbrOfLocVars) <= 256 );
				}
				if (allOk) {
					allOk = ReadMBUInt32( &funcSize, BP, BPend, pos );
				}
				if (allOk) {
					bpi->functions[i].startPos = *pos;
					bpi->functions[i].endPos = *pos + funcSize;

/*  Modified by GBU,000103  removed one unused parameter */
/*
					allOk = StepBytes( funcSize, BP, BPend, pos );
*/
                    allOk = StepBytes( funcSize, BPend, pos );
                }
				i++;
			}
		}
	}

	return allOk;
}


/******* end private functions **************/



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
VOID BPI_Delete( pstructBPInfo *pThis )
{
	if (*pThis != NULL) {
		DEALLOC( &( (*pThis)->constants ) );
		DEALLOC( &( (*pThis)->pragmas ) );
		DEALLOC( &( (*pThis)->functions ) );

		DEALLOC(pThis);
	}
}


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
pstructBPInfo BPI_New( BYTE* BP, UINT32 BPlen, INT16 IANAcharset )
{
	UINT32					pos = 0;
	BOOL						allOk	= TRUE;
	pstructBPInfo		bpi = NEWSTRUCT(structBPInfo);


	bpi->constants = NULL;
	bpi->pragmas = NULL;
	bpi->functions = NULL;
	bpi->characterSet = IANAcharset;
		/* a bytecode pacakage has one string encodein (UTF8, UCS2 or UCS4) */

	allOk = ParseAndCheckVersion( bpi, BP, BPlen, &pos );

	if (allOk) {
		allOk = ParseAndCheckSize( BP, BPlen, &pos );
	}

	if (allOk) {
		allOk = ParseAndCheckConstantPool( bpi, BP, BPlen, &pos );
    if (bpi->characterSet < 1)
    {
	  	/* if charSet is 0, it means that the encoding of
	  	the CONST_TYPE_EXT_CHARSET is given in the WSP header.
	    If no encoding is given there either, the
	    default charSet will be assumed (IANA_CHARSET_LATIN1).
			!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      This is a deliberate deviation from the WMLS 1.1 spec which
      says UTF-8 as the default.
      The reason for this deviation is that assuming ISO-Latin1 will never
      result in faulty strings since all values of a BYTE* string is a valid
      ISO-LATIN1 string which is not the case with UTF-8. We believe that
      this will make the interpreter more robust.
      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      */
    	bpi->characterSet = IANA_CHARSET_LATIN1;
    }
	}
	if (allOk) {
		allOk = Iana2Unicode_canConvert( bpi->characterSet );
    	/* if transcoding is not supported, verification error will occur */
	}

	if (allOk) {
		allOk = ParseAndCheckPragmaPool( bpi, BP, BPlen, &pos );
	}

	if (allOk) {
		allOk = ParseAndCheckFunctionPool( bpi, BP, BPlen, &pos );
	}

	if ( ! allOk )
	{
		if (bpi)
		{
			BPI_Delete( &bpi ); /* ... and bpi = NULL */
		}
	}

	return bpi;	
}


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
		UINT8 nbrOfArgs, WCHAR* refererUrl, WCHAR *baseUrl, UINT8* funcIndexResult )
{
	/*
  By default Access control is disabled
  (i.e. if no access pragmas exist there will be no access checks against
  domain or path)
   */

	BOOL				accessAllowed = TRUE;
	UINT8				funcIndex;
	UINT8				BPIfuncNameLen;	/* length of the funcName in the BPI */
	UINT32			paramFuncNameLen;	/* length of the funcName parameter to this function */
	UINT8				i;
	UINT32			bpPos;
	pstructVar	accessDomain = NULL;
	pstructVar	accessPath = NULL;
	UINT16			cIndex;
	UINT32			BPpos;


	/* check access pragmas */
	i = 0;
	while (i < thisx->nbrOfPragmas)
	{
		switch (thisx->pragmas[i].type)
		{
			case PRAGMA_ACCESS_DOMAIN:
				if (accessDomain != NULL)
				{	/* there can only be one access domain pragma */
					Var_Delete( &accessDomain );
					Var_Delete( &accessPath );
					return ERR_WAE_WMLS_VERIFICATION;
				}
				BPpos = thisx->pragmas[i].pos;
				ReadMBUInt16( &cIndex, BP, BPend, &BPpos );
				accessDomain = BPI_GetConstant( thisx, BP, BPend, cIndex );
				break;
			case PRAGMA_ACCESS_PATH:
				if (accessPath != NULL)
				{	/* there can only be one access path pragma */
					Var_Delete( &accessDomain );
					Var_Delete( &accessPath );
					return ERR_WAE_WMLS_VERIFICATION;
				}
				BPpos = thisx->pragmas[i].pos;
				ReadMBUInt16( &cIndex, BP, BPend, &BPpos );
				accessPath = BPI_GetConstant( thisx, BP, BPend, cIndex );
				break;
		}

		i++;
	}

  /* perform the accesscheck */
	if ((accessDomain != NULL) || (accessPath != NULL))
	{
    BOOL fSlask;
  	BYTE *pbDomain = NULL;
    BYTE *pbPath = NULL;
    BYTE *pbRefUrl = wip_wchar2byte( refererUrl, &fSlask );

		if (accessDomain != NULL)
		{
			pbDomain = wip_wchar2byte( accessDomain->val.theString, &fSlask );
    }
		else
    {
			WCHAR *pwchTemp = NULL;

			w_GetHost (baseUrl, &pwchTemp);
      pbDomain = wip_wchar2byte( pwchTemp, &fSlask );
      DEALLOC( &pwchTemp );
    }
		if (accessPath != NULL)
		{
			/* 990824 (KHN) Access supports relative paths so the
         path must be resolved against the baseURL and then do getpath */
			if (accessPath->val.theString[0] != '/')
      { /* ...then it is a relative path and must be resolved */
				WCHAR* pwchTemp = NULL;
				WCHAR* pwchTemp2 = NULL;

        if (w_Resolve (baseUrl, accessPath->val.theString, &pwchTemp) &&
        		w_GetPath(pwchTemp, &pwchTemp2))
        {
					pbPath = wip_wchar2byte( pwchTemp2, &fSlask );
        }
        else
        {	/* either the resolve or getpath failed, then just try with the path given */
					pbPath = wip_wchar2byte( accessPath->val.theString, &fSlask );
        }
	      DEALLOC( &pwchTemp );
	      DEALLOC( &pwchTemp2 );
      }
      else
      { /* absolute path */
				pbPath = wip_wchar2byte( accessPath->val.theString, &fSlask );
      }
    }

    accessAllowed = URL_CheckAccess( pbRefUrl, pbDomain, pbPath );
    DEALLOC( &pbDomain );
    DEALLOC( &pbPath );
    DEALLOC( &pbRefUrl );
	}
	Var_Delete( &accessDomain );
	Var_Delete( &accessPath );

  /* perform the check for the called function and the number of parameters */
	if (accessAllowed)
	{
		if (funcName == NULL)
		{
			return ERR_WAE_WMLS_NULL;
		}
		paramFuncNameLen = 0;
		while (funcName[paramFuncNameLen] != 0)  /* not NULL */
		{
			paramFuncNameLen++;
		}
		/* the length is now calculated */

		/* now check to see if function to be called exists and that the correct number of arguments are supplied */
		funcIndex = 0;
		while (funcIndex < thisx->nbrOfFunctions)
		{
			if (thisx->functions[funcIndex].funcNamePos != 0)
			{	/* if a named function (public), then perform name comparisson */
				/* first get length of funcName */
				bpPos = thisx->functions[funcIndex].funcNamePos;
				if (!ReadUInt8( &BPIfuncNameLen, BP, BPend, &bpPos ))
				{
					return ERR_WAE_WMLS_VERIFICATION;
				}
				if (BPIfuncNameLen == paramFuncNameLen)
				{	/* compare */
					i = 0;
					while ((i < paramFuncNameLen) && (BP[bpPos]== funcName[i]))
					{
						i++;
						bpPos++;
					}
					if (i == paramFuncNameLen)
					{	/* a match and the function is done */
						/* now check if nbrOfArgs match */
						if (nbrOfArgs == thisx->functions[funcIndex].nbrOfArgs)
						{	/* the correct function is found */
							*funcIndexResult = funcIndex;
							return ERR_WAE_WMLS_NONE;
						}
						else
						{	/* incorrect nbrOfArgs */
							return ERR_WAE_WMLS_FUNC_ARGS;
						}
					}
				}
			}

			funcIndex++;
		}

		return ERR_WAE_WMLS_EXTERNAL; /* if this line is reached, there was no matching function */
	}
	else
	{
		return ERR_WAE_WMLS_ACCESS;
	}
}


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
		pUA userAgent )
{
	pstructVar			stringVar = NULL;
	WCHAR						*wchProperty = NULL;
	WCHAR						*wchContent = NULL;
	WCHAR						*wchScheme = NULL;
  UINT16					pragmaIndx;
  UINT16					constantIndx;
  UINT32					BPpos;
  enumPragmaType	type;


  for (pragmaIndx = 0; pragmaIndx < thisx->nbrOfPragmas; pragmaIndx++)
  {
		type = thisx->pragmas[ pragmaIndx ].type;
		if ( (type == PRAGMA_UA_PROP_SCHEME) || (type == PRAGMA_UA_PROP) )
    {
			BPpos = thisx->pragmas[ pragmaIndx ].pos;

			ReadMBUInt16( &constantIndx, BP, BPlen, &BPpos );
      stringVar = BPI_GetConstant( thisx, BP, BPlen, constantIndx );
      wchProperty = stringVar->val.theString;
			stringVar->val.theString = NULL;
      stringVar->theStringLen = 0;
      Var_Delete( &stringVar );

			ReadMBUInt16( &constantIndx, BP, BPlen, &BPpos );
      stringVar = BPI_GetConstant( thisx, BP, BPlen, constantIndx );
      wchContent = stringVar->val.theString;
			stringVar->val.theString = NULL;
      stringVar->theStringLen = 0;
      Var_Delete( &stringVar );

			if (type == PRAGMA_UA_PROP_SCHEME)
      {
				ReadMBUInt16( &constantIndx, BP, BPlen, &BPpos );
	      stringVar = BPI_GetConstant( thisx, BP, BPlen, constantIndx );
	      wchScheme = stringVar->val.theString;
				stringVar->val.theString = NULL;
	      stringVar->theStringLen = 0;
	      Var_Delete( &stringVar );
      }

			UApragmaFromScript( userAgent, &wchProperty, &wchContent, &wchScheme );

			wchProperty = NULL;
			wchContent = NULL;
			wchScheme = NULL;
    }
  }
}


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
BOOL BPI_GetConstantType( pstructBPInfo thisx, UINT16 constIndex, enumConstType* constType )
{
	if (thisx != NULL) {
		if (constIndex < (thisx->nbrOfConstants)) {
			*constType = thisx->constants[constIndex].type;
			return TRUE;
		}
	}

	return FALSE; /* if this line is reached, the operation failed */
}



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
pstructVar BPI_GetConstant( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, UINT16 constIndex )
{
	pstructVar			result = NULL;
	enumConstType		constType;
	UINT32					constPos;
	INT8						val_i8;
	INT16						val_i16;
	INT32						val_i32;
#ifdef HAS_FLOAT
	FLOAT32					val_f32;
#endif

	UINT32					strByteLen;	/* the length of the string in number of bytes  */
	INT32						strLetterLen;	/* the number of letters/characters in the string */
	BYTE*						strStart = NULL;
	WCHAR*					ucs2Start;

	if ( BPI_GetConstantType( thisx, constIndex, &constType )	) {
		/* a valid constant */

		constPos = thisx->constants[constIndex].pos;

		switch (constType) {

			case CONST_TYPE_INT8:
				result = Var_New();
				ReadInt8( &val_i8, BP, BPend, &constPos );
				Var_AssignInt( result, val_i8 );
				break;

			case CONST_TYPE_INT16:
				result = Var_New();
				ReadInt16( &val_i16, BP, BPend, &constPos );
				Var_AssignInt( result, val_i16 );
				break;

			case CONST_TYPE_INT32:
				result = Var_New();
				ReadInt32( &val_i32, BP, BPend, &constPos );
				Var_AssignInt( result, val_i32 );
				break;


			case CONST_TYPE_FLOAT32:
				result = Var_New();
#ifdef HAS_FLOAT
				ReadFloat32( &val_f32, BP, BPend, &constPos );
				Var_AssignFloat( result, val_f32 );
#else
				Var_AssignInvalid( result );
#endif
				break;


			case CONST_TYPE_EXT_CHARSET:	/* string encoding determined by the BPI->characterSet entry */
			case CONST_TYPE_UTF8:		/* variable length string encoding */
				result = Var_New();
				ReadMBUInt32( &strByteLen, BP, BPend, &constPos );
				strStart	= BP + constPos;
				if (constType == CONST_TYPE_EXT_CHARSET)
        {
					strLetterLen = Iana2Unicode_calcLen( strStart, thisx->characterSet,
          		FALSE, strByteLen, &strByteLen );
        }
        else
        {	/* UTF8 */
					strLetterLen = Iana2Unicode_calcLen( strStart, IANA_CHARSET_UTF8,
          		FALSE, strByteLen, &strByteLen );
        }
				if (strLetterLen >= 0) {
					if (strLetterLen > 0)
          {
						Var_NewString( result, strLetterLen );
						ucs2Start = result->val.theString;
						if (constType == CONST_TYPE_EXT_CHARSET)
	          {
							if ( ! Iana2Unicode_convert( strStart, thisx->characterSet,
	            		strByteLen, ucs2Start, sizeof(WCHAR)*strLetterLen) )
		          {
								Var_AssignInvalid( result );
							}
						}
	          else
	          {	/* UTF8 */
							if ( ! Iana2Unicode_convert( strStart, IANA_CHARSET_UTF8, strByteLen,
	            		ucs2Start, sizeof(WCHAR)*strLetterLen) )
		          {
								Var_AssignInvalid( result );
							}
	          }
          }
          /* else the empty string will be the result */
				}
				else { /* incorrect encoded string, how and which error code should be given??? */
					Var_AssignInvalid( result );
				}
				break;


			case CONST_TYPE_ES:
				result = Var_New();
				break;

		}
	}

	return result;
}


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
BYTE* BPI_GetRawUTF8Constant( pstructBPInfo thisx, BYTE* BP, UINT32 BPend, UINT16 constIndex ) 
{
	enumConstType		constType;
	UINT32					constPos;
	UINT32					strLen;
	BYTE*						result = NULL;
	BYTE*						utf8Pos;


	if ( BPI_GetConstantType( thisx, constIndex, &constType ) && (constType == CONST_TYPE_UTF8)	) {
		/* a valid UTF8 string constant */

		constPos = thisx->constants[constIndex].pos;
		ReadMBUInt32( &strLen, BP, BPend, &constPos );
		utf8Pos	= BP + constPos;

		result = NEWARRAY( BYTE, strLen+1 ); /* +1 to include a NULL */
		result[strLen] = 0; /* now NULL terminated */
		memcpy( result, utf8Pos , strLen );

		return result;
	}
	else {
		return NULL;
	}
}
