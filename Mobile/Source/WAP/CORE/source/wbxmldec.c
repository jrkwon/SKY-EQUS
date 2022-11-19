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
/* ==================================================================
	FILE: WBXMLDec.c
  
	General WBXML-decoder.

	Rev history:
		990302	JPR		Created
		990322	JPR		Modified 
		990323	JPR		WAP 1.1
		990326	JPR		Corrections in Prolog-decoding
		990330	JPR		Common string-decoding functions used
		990406	JPR		Corrections
		990427	JPR		Supports new CLNTa_error functions
		990610	JPR		Removal of unreferenced variables
		990812	JPR		Updated to support channel decoding
		990827	JPR		Extended WML-parsing
		991109	JPR		TR 173. Correction in WBXML_ParseElement
		991116	JPR		Updated to support PUSH Content-types

  		991124	JPR		Redesign of Element Handling, Text Handling,
						and Error Handling (Consult separate document).
						Updates for WAP 1.2 are also included.
		991126	JPR		Application specific parts and calls to 
						CLNTa_error removed
		991208	JPR		ViewId removed
		991222	JPR		Charset handling changed
		000104	JPR		Change in DTD_CreateElement
		000211	JPR		Correction in function WBXML_DecodeContent
						and WBXML_DecodeAttributeValue (OPAQUE)
		000303	JPR		Function WBXML_GetContentType added
		000510	JPR		Call stack reduction implemented
		000526	JPR		Function WBXML_CheckPublicID modified
		000808	JPR		Correction in code page handling
		000814	JPR		Functions WBXML_StepPastAllPIs and 
									WBXML_StepPastOnePI added
		000925	JPR		Correction in WBXML_DecodeProlog (WBXML 1.3 added)

   ================================================================== */

#include "wbxmldec.h"
#include "wbxmlelm.h"
#include "trnscode.h"


/*========================================================================
	Call Stack Structs and functions
==========================================================================*/
typedef struct tWBXMLCONTENTCALLSTACK
{
	BOOL fCont;
	BOOL fPiEnd;
	pELEMENTTYPE pElement;
	BYTE bToken;
	UINT32 iLength;
	BYTE* pbData;
} WBXMLCONTENTCALLSTACK, *pWBXMLCONTENTCALLSTACK;


pWBXMLCONTENTCALLSTACK WBXML_CreateContentCallStack (void)
{
	pWBXMLCONTENTCALLSTACK pStack=NEWSTRUCT(WBXMLCONTENTCALLSTACK);

	/* Set default values */
	if ( pStack !=NULL )
	{
		pStack->fCont=TRUE;
		pStack->fPiEnd=FALSE;
		pStack->pElement=NULL;
		pStack->bToken=0;
		pStack->iLength=0;
		pStack->pbData=NULL;
	}

	return pStack;
}

typedef struct tWBXMLELEMENTCALLSTACK
{
	pELEMENTTYPE pContent;
	BYTE bToken;
	BYTE bTagInfo;
	UINT16 iTag;
	UINT32 iMBUINT;
} WBXMLELEMENTCALLSTACK, *pWBXMLELEMENTCALLSTACK;


pWBXMLELEMENTCALLSTACK WBXML_CreateElementCallStack (void)
{
	pWBXMLELEMENTCALLSTACK pStack=NEWSTRUCT(WBXMLELEMENTCALLSTACK);

	/* Set default values */
	if ( pStack !=NULL )
	{
		pStack->pContent=NULL;
		pStack->bToken=0;
		pStack->bTagInfo=0;
		pStack->iTag=Type_Literal;
		pStack->iMBUINT=0;
	}

	return pStack;
}

/*========================================================================*/


void WBXML_StepByte (pWBXMLDECODESTR pDecStr) 
{
	pDecStr->pbCurByte++;
}


/*========================================================================
	WBXML_GetStrTabVal
==========================================================================*/
WCHAR* WBXML_GetStrTabVal (pWBXMLDECODESTR pDecStr, UINT32 iLoc) 
{
	BOOL fCont=TRUE;
	pSTRINGTABSTR pStrTemp=pDecStr->pStrTab;

	while ( (pStrTemp!=NULL) && (fCont) )
	{
		if (iLoc==pStrTemp->iOffset)
		{
			fCont=FALSE;
		}
		else
		{
			pStrTemp=pStrTemp->pNext;
		}
	}

	if (pStrTemp!=NULL)
	{
		return pStrTemp->pwchString;
	}

	/* Error - WBXML_Error_StringTable */
	(pDecStr->iDecodeResult)|=(WBXML_Error_StringTable);

	return NULL;
}

/*========================================================================
	WBXML_CreateWBXMLDecStr
==========================================================================*/
pWBXMLDECODESTR WBXML_CreateWBXMLDecStr (void)
{
	pWBXMLDECODESTR pDecStr=NULL;

	/* Allocate memory */
	pDecStr=NEWSTRUCT(WBXMLDECODESTR);

	if (pDecStr!=NULL)
	{
		pDecStr->pbCurByte=NULL;
		pDecStr->pbEnd=NULL;
		pDecStr->iCharset=IANA_CHARSET_INVALID;
		pDecStr->iContentLevel=0;
		pDecStr->bContentType=0;
		pDecStr->iVersion=0;
		pDecStr->iAttrCodePage=0;
		pDecStr->iTagCodePage=0;
		pDecStr->iDecodeResult=0;
		pDecStr->pStrTab=NULL;
		pDecStr->pAppSpec=NULL;
	}

	return pDecStr;
}


void WBXML_DelStrTab (pWBXMLDECODESTR pDecStr) 
{
	pSTRINGTABSTR pStrTemp=NULL;

	while (pDecStr->pStrTab!=NULL) 
	{
		DEALLOC(&(pDecStr->pStrTab->pwchString));
		pStrTemp=pDecStr->pStrTab;
		pDecStr->pStrTab=pDecStr->pStrTab->pNext;
		DEALLOC(&pStrTemp);
	}
}


/*========================================================================
	WBXML_DeleteWBXMLDecStr
==========================================================================*/
void WBXML_DeleteWBXMLDecStr (pWBXMLDECODESTR *ppDecStr)
{
	if (*ppDecStr!=NULL)
	{
		WBXML_DelStrTab (*ppDecStr);
		DEALLOC(ppDecStr);
	}
}


/*========================================================================
	WBXML_CopyCharToWChar
==========================================================================*/
WCHAR* WBXML_CopyCharToWChar (pWBXMLDECODESTR pDecStr, CHAR* src)
{
	WCHAR *pchTempValue=NULL;

	/* ASSERT: pDecStr!=NULL
			   src!=NULL
	*/

	pchTempValue=w_cstr2wstr(src);
	pDecStr=pDecStr;

	#ifndef HAS_SETJMP
	if (pchTempValue==NULL)
	{
		/* Could not create copy - out of memory. */
		pDecStr->iDecodeResult|=WBXML_Error_OutOfMemory;	
	}
	#endif

	return pchTempValue;
}

/*========================================================================
	WBXML_GetSTR_I
==========================================================================*/
WCHAR *WBXML_GetSTR_I (pWBXMLDECODESTR pDecStr) 
{
	INT16 iCharset=pDecStr->iCharset;
	INT32 iLength=0;
	UINT32 iBytes=0;
	WCHAR *pchResult=NULL;

	if ( pDecStr->pbCurByte < pDecStr->pbEnd )
	{
		/* Calculate length */
		iLength=Iana2Unicode_calcLen (pDecStr->pbCurByte, iCharset, TRUE,
						pDecStr->pbEnd-pDecStr->pbCurByte, &iBytes);

		/* Check result */
		if (iLength<0)
		{
			/* Not OK. Interpret as iso-latin-1 */
			pDecStr->iDecodeResult|=WBXML_Warning_UnknownCharset;	
			iCharset=IANA_CHARSET_LATIN1;

			Iana2Unicode_calcLen (pDecStr->pbCurByte, iCharset, TRUE,
						pDecStr->pbEnd-pDecStr->pbCurByte, &iBytes);

			iLength=iBytes;
		}

		/* Create result buffer */
		pchResult=NEWARRAY(WCHAR,iLength+1);

		/* Convert string */
		if (pchResult!=NULL)
		{
			/* Set termination character */
			pchResult[iLength]=0;
		
			if (Iana2Unicode_convert(pDecStr->pbCurByte, iCharset, iBytes,
						pchResult, iLength))
			{
				/* Step past string */
				pDecStr->pbCurByte+=iBytes+Iana2Unicode_getNullTermByteLen(iCharset);
			}
			else
			{
				/* Error */
				DEALLOC(&pchResult);

				/* Charset error */
				pDecStr->iDecodeResult|=WBXML_Error_CharsetEncoding;	
			}
		}
		else
		{
			/* Out of memory */
			pDecStr->iDecodeResult|=WBXML_Error_OutOfMemory;	
		}
	}
	else
	{
		/* Data stream error */
		pDecStr->iDecodeResult|=WBXML_Error_DataStream;
	}

	/* Return string */
	return pchResult;
}


/*========================================================================
	WBXML_GetMBUINT32
==========================================================================*/
UINT32 WBXML_GetMBUINT32 (pWBXMLDECODESTR pDecStr)
{
	UINT32 iPos=0;
	UINT32 iResult=0;

	if ( pDecStr->pbCurByte < pDecStr->pbEnd )
	{	
		if (ReadMBUInt32(&iResult,pDecStr->pbCurByte,
					pDecStr->pbEnd-pDecStr->pbCurByte,&iPos))
		{
			/* Step forward */
			pDecStr->pbCurByte+=iPos;
		}
		else 
		{
			/* Fatal error. Error in encoding of MBUINT32. */
			pDecStr->iDecodeResult|=WBXML_Error_MbuintEncoding;	
		}
	}
	else
	{
		/* Instream error */
		pDecStr->iDecodeResult|=WBXML_Error_DataStream;	
	}
	return iResult;
}


/*========================================================================
	WBXML_CheckIfGlobalText
==========================================================================*/
BOOL WBXML_CheckIfGlobalText (BYTE bToken)
{
	/* Check if ( string | extension | entity ) */
	switch (bToken)
	{
	case GLOBAL_ENTITY:
	case GLOBAL_STR_I:
	case GLOBAL_EXT_I_0:
	case GLOBAL_EXT_I_1:
	case GLOBAL_EXT_I_2:
	case GLOBAL_EXT_T_0:
	case GLOBAL_EXT_T_1:
	case GLOBAL_EXT_T_2:
	case GLOBAL_STR_T:
	case GLOBAL_EXT_0:
	case GLOBAL_EXT_1:
	case GLOBAL_EXT_2:
		return TRUE;
	}
	return FALSE;
}


/*========================================================================
	WBXML_DecodeGlobalText
==========================================================================*/
pELEMENTTYPE WBXML_DecodeGlobalText (pWBXMLDECODESTR pDecStr)
{
	pELEMENTTYPE pElement=NULL;
	WCHAR *pwchString=NULL;
	BYTE bToken=0;
	UINT32 iMBUINT32=0;

	if ( pDecStr->pbCurByte < pDecStr->pbEnd )
	{
		bToken=*pDecStr->pbCurByte;
		WBXML_StepByte(pDecStr);

		switch (bToken)
		{
		/* Entity */
		case GLOBAL_ENTITY:

			/* Get mbuint32 */
			iMBUINT32=WBXML_GetMBUINT32 (pDecStr);

			/* Create element */
			pElement=XML_CreateEntity (iMBUINT32);

			break;

		/* Inline string */
		case GLOBAL_STR_I:

			/* Get inline string */
			pwchString=WBXML_GetSTR_I(pDecStr);

			/* Create inline text element */
			pElement=XML_CreateInlineText(&pwchString);
			break;

		/* Extension with inline string */
		case GLOBAL_EXT_I_0:
		case GLOBAL_EXT_I_1:
		case GLOBAL_EXT_I_2:

			/* Get inline string */
			pwchString=WBXML_GetSTR_I(pDecStr);

			/* Create extension element */
			pElement=XML_CreateExt(&pwchString, bToken);
			break;

		case GLOBAL_STR_T:

			/* Get mbuint32 */
			iMBUINT32=WBXML_GetMBUINT32 (pDecStr);

			/* Create string table text element */
			pElement=XML_CreateTableText(iMBUINT32);

			break;

		case GLOBAL_EXT_T_0:
		case GLOBAL_EXT_T_1:
		case GLOBAL_EXT_T_2:

			/* Get mbuint32 */
			iMBUINT32=WBXML_GetMBUINT32 (pDecStr);

			/* Get copy of string table element */
			pwchString=CreateStringCopy(WBXML_GetStrTabVal(pDecStr,iMBUINT32));

			/* Create extension element */
			pElement=XML_CreateExt(&pwchString, bToken);

			break;

		case GLOBAL_EXT_0:
		case GLOBAL_EXT_1:
		case GLOBAL_EXT_2:

			pwchString=NULL;

			/* Create extension element */
			pElement=XML_CreateExt(&pwchString, bToken);

			break;
		}
	}
	else
	{
		/* Instream error */
		pDecStr->iDecodeResult|=WBXML_Error_DataStream;	
	}

	return pElement;
}

/*========================================================================
	WBXML_DecodeAttributeValue
==========================================================================*/
pELEMENTTYPE WBXML_DecodeAttributeValue (pWBXMLDECODESTR pDecStr)
{
	BOOL fCont=TRUE;
	BYTE bToken=NULL;
	pELEMENTTYPE pAttrVal=NULL;
	pELEMENTTYPE pAttrPart=NULL;

	/* Get all attribute value parts */
	while ( (pDecStr->pbCurByte < pDecStr->pbEnd ) && (fCont) )
	{
		pAttrPart=NULL;
		bToken=*pDecStr->pbCurByte;

		/* Switch page */
		if (bToken==GLOBAL_SWITCH_PAGE)
		{
			WBXML_StepByte(pDecStr);

			/* Switch Attribute Code Page */
			if ( pDecStr->pbCurByte < pDecStr->pbEnd )
			{
				pDecStr->iAttrCodePage=*pDecStr->pbCurByte;
				WBXML_StepByte(pDecStr);
			}
		}

		/* End */
		else if (bToken==GLOBAL_END)
		{
			fCont=FALSE;
		}

		/* ( string | extension | entity ) */
		else if (WBXML_CheckIfGlobalText(bToken))
		{
			/* Get Text-element */
			pAttrPart=WBXML_DecodeGlobalText (pDecStr);
		}

		/* OPAQUE */
		else if (bToken==GLOBAL_OPAQUE)
		{
			UINT32 iLength=0;
			WBXML_StepByte(pDecStr);

			/* Get length (mbuint32) */
			iLength=WBXML_GetMBUINT32 (pDecStr);

			/* Copy data */
			if ( pDecStr->pbCurByte+iLength < pDecStr->pbEnd )
			{
				BYTE* pbData=NEWARRAY (BYTE,iLength);

				#ifndef HAS_SETJMP
				if (pbData!=NULL)
				{
				#endif
		
				/* Copy */
				B_COPYSTRINGN (pbData,pDecStr->pbCurByte,iLength);

				/* Create opaque element */
				pAttrPart=XML_CreateOpaque (&pbData, iLength);

				#ifndef HAS_SETJMP
				}
				else
				{
					/* Out of memory */
					pDecStr->iDecodeResult|=WBXML_Error_DataStream;
				}
				#endif
			}
			else
			{
				/* Instream error */
				pDecStr->iDecodeResult|=WBXML_Error_DataStream;
			}

			/* Step past data */
			pDecStr->pbCurByte+=iLength;
		}

		/* ATTRVALUE ( value 128 or greater ) */
		else if (bToken>127)
		{
			UINT16 iAttrVal=(UINT16)((UINT16)bToken + (UINT16)(256 * pDecStr->iAttrCodePage));
			WCHAR *pwchString=NULL;

			WBXML_StepByte(pDecStr);

			/* Get application specific value */
			pwchString=pDecStr->DTD_GetAttributeValue (iAttrVal);

			/* Add to pAttrPart */
			if (pwchString!=NULL)
			{
				pAttrPart=XML_CreateInlineText (&pwchString);
			}
			else
			{
				/* Error in encoding - unknown attribute value */
				pDecStr->iDecodeResult|=WBXML_Warning_UnknownAttributeValue;	
			}
		}
		
		else
		{
			/* End of attribute value - quit */
			fCont=FALSE;
		}

		/* Add attribute part to end of attribute value */
		XML_AddElementToEnd (&pAttrVal, pAttrPart);
	}

	return pAttrVal;
}


/*========================================================================
	WBXML_DecodeAttributes
==========================================================================*/
void WBXML_DecodeAttributes (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pElement)
{
	BOOL fCont=TRUE;
	BYTE bToken=0;
	UINT16 iAttribute=0;
	pELEMENTTYPE pAttrVal=NULL;
	UINT32 iMBUINT=0;

	/* Decode all attributes and the values */
	while ( (pDecStr->pbCurByte < pDecStr->pbEnd) && (fCont) )
	{
		bToken=*pDecStr->pbCurByte;

		/* Get the correct attribute value */
		iAttribute=(UINT16)(bToken + (256 * pDecStr->iAttrCodePage));
		WBXML_StepByte(pDecStr);

		switch (bToken)
		{

		/* End */
		case GLOBAL_END:
			fCont=FALSE;		
			break;

		/* Switch page */
		case GLOBAL_SWITCH_PAGE:

			/* Switch Attribute Code Page */
			if ( pDecStr->pbCurByte < pDecStr->pbEnd )
			{
				pDecStr->iAttrCodePage=*pDecStr->pbCurByte;
				WBXML_StepByte(pDecStr);
			}
			break;

		/* Literal */
		case GLOBAL_LITERAL:
			
			/* Get mbuint32 */
			iMBUINT=WBXML_GetMBUINT32 (pDecStr);
			
			/* Get string from string table and call function 
			   to convert it to the correct token */
			iAttribute=pDecStr->DTD_LiteralAttributeToToken 
								(WBXML_GetStrTabVal (pDecStr, iMBUINT));

			/* No break - continue into default */

		/* Other */
		default:

			/* Get attribute value */
			pAttrVal=WBXML_DecodeAttributeValue(pDecStr);

			/* Store attribute value */
			if (!pDecStr->DTD_StoreAttributeValue (pDecStr, pElement, 
													iAttribute, &pAttrVal))
			{
				/* Illegal attribute - warning */
				pDecStr->iDecodeResult|=WBXML_Warning_IllegalAttribute;	
			}

			/* Delete attribute value (list of elements) */
			XML_DeleteElementList(&pAttrVal,pDecStr);

			break;
		}
	}
}


/*========================================================================
	WBXML_DecodeContent
==========================================================================*/
pELEMENTTYPE WBXML_DecodeContent (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pParent)
{	
	pELEMENTTYPE pContentList=NULL;
	pWBXMLCONTENTCALLSTACK pStack=WBXML_CreateContentCallStack();

	if (pStack!=NULL)
	{
		/* Get all elements on this level */
		while ( ( pDecStr->pbCurByte < pDecStr->pbEnd ) && ( pStack->fCont ) && 
				( pDecStr->iDecodeResult < WBXML_FatalError ))
		{
			pStack->bToken=*pDecStr->pbCurByte;
			pStack->pElement=NULL;

			/* END */
			if (pStack->bToken==GLOBAL_END)
			{
				WBXML_StepByte(pDecStr);
				pStack->fCont=FALSE;
			}

			/* GLOBAL TEXT ( entity | string | extension ) */
			else if (WBXML_CheckIfGlobalText (pStack->bToken))
			{
				pStack->pElement=WBXML_DecodeGlobalText (pDecStr);			
			}

			/* PI */
			else if (pStack->bToken==GLOBAL_PI)
			{
				pStack->fPiEnd=FALSE;

				/* Find END */
				while ( (pDecStr->pbCurByte < pDecStr->pbEnd) && 
						(!pStack->fPiEnd) )
				{
					if ( *(pDecStr->pbCurByte)==GLOBAL_END )
					{
						/* END found - Step past and exit loop. */
						pDecStr->pbCurByte++;
						pStack->fPiEnd=TRUE;
					}
					else
					{
						pDecStr->pbCurByte++;
					}
				}
			}

			/* OPAQUE */
			else if (pStack->bToken==GLOBAL_OPAQUE)
			{
				WBXML_StepByte(pDecStr);

				/* Get length (mbuint32) */
				pStack->iLength=WBXML_GetMBUINT32 (pDecStr);

				/* Copy data */
				if ( pDecStr->pbCurByte+pStack->iLength < pDecStr->pbEnd )
				{
					pStack->pbData=NEWARRAY (BYTE,pStack->iLength);

					#ifndef HAS_SETJMP
					if (pStack->pbData!=NULL)
					{
					#endif

					/* Copy */
					B_COPYSTRINGN (pStack->pbData,pDecStr->pbCurByte,pStack->iLength);

					/* Create opaque element */
					pStack->pElement=XML_CreateOpaque (&(pStack->pbData), pStack->iLength);

					#ifndef HAS_SETJMP
					}
					else
					{
						/* Out of memory */
						pDecStr->iDecodeResult|=WBXML_Error_DataStream;
					}
					#endif
				}
				else
				{
					/* Instream error */
					pDecStr->iDecodeResult|=WBXML_Error_DataStream;
				}

				/* Step past data */
				pDecStr->pbCurByte+=(pStack->iLength);
			}

			/* ELEMENT */
			else
			{
				pStack->pElement=WBXML_DecodeElement (pDecStr, pParent);
			}

			/* Add element to content list */
			XML_AddElementToEnd (&pContentList, (pStack->pElement));

			/* Check if fatal error */
			if ( pDecStr->iDecodeResult > WBXML_FatalError )
			{
				pStack->fCont=FALSE;
			}
		}
	}
	else
	{
		/* Out of memory */
		pDecStr->iDecodeResult|=WBXML_Error_OutOfMemory;
	}

	/* Delete local stack */
	DEALLOC(&pStack);

	return pContentList;
}


/*========================================================================
	WBXML_StepPastOnePI
==========================================================================*/
void WBXML_StepPastOnePI (pWBXMLDECODESTR pDecStr)
{
	BOOL fPiEnd=FALSE;

	/* Check if PI found */
	if ( (pDecStr->pbCurByte < pDecStr->pbEnd) && (*pDecStr->pbCurByte==GLOBAL_PI) )
	{
		/* Step */
		pDecStr->pbCurByte++;

		/* Find END */
		while ( (pDecStr->pbCurByte < pDecStr->pbEnd) && 
				(!fPiEnd) )
		{
			if ( *(pDecStr->pbCurByte)==GLOBAL_END )
			{
				/* END found - Step past and exit loop. */
				pDecStr->pbCurByte++;
				fPiEnd=TRUE;
			}
			else
			{
				pDecStr->pbCurByte++;
			}
		}
	}
}

/*========================================================================
	WBXML_StepPastAllPIs
==========================================================================*/
void WBXML_StepPastAllPIs (pWBXMLDECODESTR pDecStr)
{
	/* Check if PI found */
	while ( (pDecStr->pbCurByte < pDecStr->pbEnd) && (*pDecStr->pbCurByte==GLOBAL_PI) )
	{
		/* Step past PI */
		WBXML_StepPastOnePI (pDecStr);
	}
}

/*========================================================================
	WBXML_DecodeElement
==========================================================================*/
pELEMENTTYPE WBXML_DecodeElement (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pParent)
{
	pELEMENTTYPE pNewElement=NULL;
	pWBXMLELEMENTCALLSTACK pStack=WBXML_CreateElementCallStack();

	if (pStack!=NULL)
	{
		/* Check if SWITCH PAGE */
		if ( pDecStr->pbCurByte < pDecStr->pbEnd )
		{
			if ( *pDecStr->pbCurByte==GLOBAL_SWITCH_PAGE )
			{
				WBXML_StepByte(pDecStr);

				if ( pDecStr->pbCurByte < pDecStr->pbEnd )
				{
					/* Switch Tag Code Page */
					pDecStr->iTagCodePage=*pDecStr->pbCurByte;
					WBXML_StepByte(pDecStr);
				}
			}
		}
			
		/* Get tag */
		if ( pDecStr->pbCurByte < pDecStr->pbEnd )
		{
			pStack->bToken=*pDecStr->pbCurByte;

			/* Step */
			WBXML_StepByte(pDecStr);

			switch (pStack->bToken)
			{
			/* Illegal */
			case GLOBAL_SWITCH_PAGE:
			case GLOBAL_END:
			case GLOBAL_ENTITY:
			case GLOBAL_STR_I:
			case GLOBAL_EXT_I_0:
			case GLOBAL_EXT_I_1:
			case GLOBAL_EXT_I_2:
			case GLOBAL_PI:
			case GLOBAL_EXT_T_0:
			case GLOBAL_EXT_T_1:
			case GLOBAL_EXT_T_2:
			case GLOBAL_STR_T:
			case GLOBAL_EXT_0:
			case GLOBAL_EXT_1:
			case GLOBAL_EXT_2:
			case GLOBAL_OPAQUE:

				/* Error in WBXML-encoding - fatal */
				pDecStr->iDecodeResult|=WBXML_Error_Encoding;
				break;

			/* LITERAL */
			case GLOBAL_LITERAL:
			case GLOBAL_LITERAL_C:
			case GLOBAL_LITERAL_A:
			case GLOBAL_LITERAL_AC:

				/* Get 'mask' */
				pStack->bTagInfo=(BYTE)((pStack->bToken)&BOTH);

				/* Get mbuint32 */
				pStack->iMBUINT=WBXML_GetMBUINT32 (pDecStr);

				/* Get string from string table and call function 
				   to convert it to the correct token */
				pStack->iTag=pDecStr->DTD_LiteralTagToToken 
										(WBXML_GetStrTabVal (pDecStr, pStack->iMBUINT));
				break;

			/* Element */
			default:

				/* Get 'mask' */
				pStack->bTagInfo=(BYTE)((pStack->bToken)&BOTH);

				/* Get tag */
				pStack->iTag=(UINT16)(((pStack->bToken)&MASK) + (256 * pDecStr->iTagCodePage));

				break;
			}
		}
		else
		{
			/* Instream error */
			pDecStr->iDecodeResult|=WBXML_Error_DataStream;	
		}

		/* Decode element */
		if ( pDecStr->iDecodeResult <= WBXML_FatalError )
		{
			/* Create Element */
			pNewElement=pDecStr->DTD_CreateElement(pDecStr,pStack->iTag);

			/* Decode attributes (if any) */
			if ((pStack->bTagInfo)&ATTR)
			{
				WBXML_DecodeAttributes (pDecStr, pNewElement);
			}

			/* Decode content (if any) */
			if ((pStack->bTagInfo)&CONT)
			{
				/* Get content */
				pStack->pContent=WBXML_DecodeContent (pDecStr, pNewElement);

				/* Store content in element */
				if (!pDecStr->DTD_AddContent (pDecStr,pNewElement, &(pStack->pContent)))
				{
					/* Content not allowed here - set warning */
					pDecStr->iDecodeResult|=WBXML_Warning_IllegalElement;	
				}

				/* Delete content (only if not stored) */
				XML_DeleteElementList (&(pStack->pContent),pDecStr);
			}

			if (pNewElement!=NULL)
			{
				/* Application specific modifications */
				pDecStr->DTD_AppSpecParseMod (pDecStr,&pNewElement,pParent);
			}
		}
	}
	else
	{
		/* Out of memory */
		pDecStr->iDecodeResult|=WBXML_Error_OutOfMemory;
	}

	/* Delete local stack */
	DEALLOC(&pStack);

	/* Return element */
	return pNewElement;
}


/* Set decode charset */
void WBXML_SetDecodeCharset (pWBXMLDECODESTR pDecStr, INT16 iCharset)
{
	/* Set charset : -1 indicates unknown charset. If the charset is
	   already set (e.g., WSP-level) and differ from the value
	   specified in the content, the definition in the content will 
	   be used. If none is defined, UTF-8 will be set as default.	
	*/
	if (iCharset!=IANA_CHARSET_INVALID)
	{
		pDecStr->iCharset=iCharset;
		return;
	}

	/* Check if charset already defined */
	if (pDecStr->iCharset!=IANA_CHARSET_INVALID)
	{
		/* Keep pre-defined charset */
		return;
	}

	/* Default charset is UTF-8 */
	pDecStr->iCharset=IANA_CHARSET_UTF8;
}

BOOL WBXML_DecodeProlog (pWBXMLDECODESTR pDecStr)
{
	UINT32 iPublicIDRef=0;
	UINT8 iPublicID=0;
	UINT32 iIndex=0;
	UINT32 iStrTabLen=0;
	BOOL fPublicIDisString=FALSE;
	pSTRINGTABSTR pStrTemp=NULL;
	pSTRINGTABSTR pStrPrev=NULL;
	BYTE *pbStrTabEnd=NULL;
	BYTE *pbStrTabStart=NULL;
	WCHAR *pchPublicID=NULL;

	/* Version number */
	if ( pDecStr->pbCurByte < pDecStr->pbEnd )
	{
		/* Check version number */
		if (! ((*pDecStr->pbCurByte==WBXML_VERSION_NUMBER_1_1) ||
		       (*pDecStr->pbCurByte==WBXML_VERSION_NUMBER_1_2) ||
					 (*pDecStr->pbCurByte==WBXML_VERSION_NUMBER_1_3)))
		{
			/* Wrong WBXML version number. Set warning. */
			pDecStr->iDecodeResult|=WBXML_Warning_Version;	
		}

		WBXML_StepByte (pDecStr);
	}

	/* Parse public ID */
	if ( pDecStr->pbCurByte < pDecStr->pbEnd )
	{
		if (*pDecStr->pbCurByte==0) 
		{
			WBXML_StepByte (pDecStr);

			/* Encoded as string. Get location. Get the
			   string later, when the string table is parsed. */
			iPublicIDRef=WBXML_GetMBUINT32 (pDecStr);

			fPublicIDisString=TRUE;
		}
		else
		{
			/* Get PublicID as integer. */
			iPublicID=(UINT8)WBXML_GetMBUINT32 (pDecStr);
		}
	}

	/* Charset */
	if ( pDecStr->pbCurByte < pDecStr->pbEnd )
	{
		UINT32 iMBUINT=0;
		INT16 iCharset=IANA_CHARSET_INVALID;

		/* Get Charset. */
		iMBUINT=WBXML_GetMBUINT32 (pDecStr);

		/* Charset value of 0 inplies IANA_CHARSET_INVALID */
		if (iMBUINT!=0)
		{
			iCharset=(INT16)(iMBUINT);
		}

		/* Set charset */
		WBXML_SetDecodeCharset (pDecStr, iCharset);
	}

	/* String Table */
	if ( pDecStr->pbCurByte < pDecStr->pbEnd )
	{
		/* Get string table length. */
		iStrTabLen=WBXML_GetMBUINT32 (pDecStr);

		/* Set min (pDecStr->pbEnd,pDecStr->pbCurByte+iStrTabLen) */
		pbStrTabEnd=pDecStr->pbCurByte+iStrTabLen;
		pbStrTabStart=pDecStr->pbCurByte;

		if ( pbStrTabEnd > pDecStr->pbEnd )
		{
			/* String table longer than instream */
			pDecStr->iDecodeResult|=WBXML_Error_DataStream;
			return FALSE;
		}
	}

	while ( pDecStr->pbCurByte < pbStrTabEnd )
	{
		/* Parse the instream and copy the string
		   table into the memory. */
		
		/* Create new String table element */
		pStrTemp=NEWSTRUCT(STRINGTABSTR);

		if (pStrTemp!=NULL) 
		{
			pStrTemp->pwchString=WBXML_GetSTR_I (pDecStr);
			pStrTemp->iOffset=iIndex;

			if (pStrTemp->pwchString==NULL)
			{
				/* Quit */
				pDecStr->pbCurByte=pbStrTabEnd;
			}
		
			/* Add to the end of the list. */
			pStrTemp->pNext=NULL;
			if (pDecStr->pStrTab==NULL)
			{
				pDecStr->pStrTab=pStrTemp;	
			}
			else
			{
				pStrPrev->pNext=pStrTemp;
			}
			pStrPrev=pStrTemp;
		} 
		else 
		{
			/* Could not create element. Fatal error. */
			pDecStr->iDecodeResult|=WBXML_Error_OutOfMemory;	

			/* Delete string table */
			WBXML_DelStrTab (pDecStr);

			return FALSE;
		}

		/* Update index */
		iIndex=pDecStr->pbCurByte-pbStrTabStart;
	}

	/* Check public ID */
	if (fPublicIDisString)
	{
		/* Get publicID as string */
		pchPublicID=WBXML_GetStrTabVal(pDecStr,iPublicIDRef);

		if (pDecStr->DTD_CheckPublicID (pDecStr,pchPublicID,&iPublicID))
		{
			/* Store iPublicID in DTD */
			pDecStr->iVersion=iPublicID;
		}
		else
		{
			pDecStr->iDecodeResult|=WBXML_Error_PublicID;	

			/* Delete string table */
			WBXML_DelStrTab (pDecStr);
			return FALSE;
		}
	}
	else
	{
		if (pDecStr->DTD_CheckPublicID (pDecStr,NULL,&iPublicID))
		{
			/* Store iPublicID in DTD */
			pDecStr->iVersion=iPublicID;
		}
		else
		{
			/* Not OK */
			pDecStr->iDecodeResult|=WBXML_Error_PublicID;	

			/* Delete string table */
			WBXML_DelStrTab (pDecStr);
			
			return FALSE;
		}
	}

	/* OK! */
	return TRUE;
}


/*========================================================================
	WBXML_CheckPublicID
==========================================================================
	Examines the PublicID and returns the corresponding WSP-content type.

	Input: pWBXMLDECODESTR (not NULL), WCHAR*, and UINT8
	Output: TRUE if valid ID, FALSE otherwise.
==========================================================================*/
BOOL WBXML_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	pDecStr=pDecStr;

	/* Use string */
	if (pwchPId!=NULL)
	{
		UINT32 iLength=0;

		const WCHAR pwchPublic_ID_base[]={'-','/','/','W','A','P','F','O','R','U','M',								   '/','/','D','T','D',' ',0};
		const WCHAR pwchPublic_ID_wml10[]={'W','M','L',' ','1','.','0','/','/','E','N',0};
		const WCHAR pwchPublic_ID_wta10[]={'W','T','A',' ','1','.','0','/','/','E','N',0};
		const WCHAR pwchPublic_ID_wml11[]={'W','M','L',' ','1','.','1','/','/','E','N',0};
		const WCHAR pwchPublic_ID_si10[]={'S','I',' ','1','.','0','/','/','E','N',0};
		const WCHAR pwchPublic_ID_sl10[]={'S','L',' ','1','.','0','/','/','E','N',0};
		const WCHAR pwchPublic_ID_co10[]={'C','O',' ','1','.','0','/','/','E','N',0};
		const WCHAR pwchPublic_ID_ch11[]={'C','H','A','N','N','E','L',' ','1','.','1','/','/','E','N',0};
		const WCHAR pwchPublic_ID_wml12[]={'W','M','L',' ','1','.','2','/','/','E','N',0};
		const WCHAR pwchPublic_ID_wml13[]={'W','M','L',' ','1','.','3','/','/','E','N',0};
		const WCHAR pwchPublic_ID_prov10[]={'P','R','O','V',' ','1','.','0','/','/','E','N',0};
		const WCHAR pwchPublic_ID_wtawml12[]={'W','T','A','-','W','M','L',' ','1','.','2','/','/','E','N',0};
		const WCHAR pwchPublic_ID_ch12[]={'C','H','A','N','N','E','L',' ','1','.','2','/','/','E','N',0};

		WCHAR* pwchTemp=NULL;

		/* Get length of pwchPId string */
		iLength=(UINT32) (w_strlen(pwchPId));

		/* Length must be greater than 18 */
		if ( iLength > 18 )
		{
			/* Check base */
			if (CompareWideStringNoCase (pwchPId,(WCHAR*)pwchPublic_ID_base,17))
			{
				pwchTemp=pwchPId+17;

				/* Base OK - check other part */
				if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_wml10,iLength-17))
				{
					/* wml 1.0 --> unknown (not supported) */
					*piPId=0;
					return FALSE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_wta10,iLength-17))
				{
					/* wta event --> unknown (not supported) */
					*piPId=0;
					return FALSE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_wml11,iLength-17))
				{
					/* wml 1.1 */
					*piPId=0x14;
					return TRUE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_si10,iLength-17))
				{
					/* si 1.0 */
					*piPId=0x2E;
					return TRUE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_sl10,iLength-17))
				{
					/* sl 1.0 */
					*piPId=0x30;
					return TRUE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_co10,iLength-17))
				{
					/* co 1.0 */
					*piPId=0x32;
					return TRUE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_ch11,iLength-17))
				{
				#ifdef CONFIG_WTA
					/* channel 1.1 */
					*piPId=0x16;
					return TRUE;
				#else
					/* channel 1.1 --> not supported */
					*piPId=0x0;
					return FALSE;
				#endif
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_wml12,iLength-17))
				{
					/* wml 1.2 */
					*piPId=0x14;
					return TRUE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_wml13,iLength-17))
				{
					/* wml 1.3 */
					*piPId=0x14;
					return TRUE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_prov10,iLength-17))
				{
					/* provisioning --> not supported */
					*piPId=0;
					return FALSE;
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_wtawml12,iLength-17))
				{
				#ifdef CONFIG_WTA
					/* wta-wml 1.2 --> 0x75 is an internal number */
					*piPId=0x75;
					return TRUE;
				#else
					/* wta-wml 1.2 --> not supported */
					*piPId=0x0;
					return FALSE;
				#endif
				}
				else if (CompareWideStringNoCase (pwchTemp,(WCHAR*)pwchPublic_ID_ch12,iLength-17))
				{
				#ifdef CONFIG_WTA
					/* channel 1.2 */
					*piPId=0x16;
					return TRUE;
				#else
					/* channel 1.2 --> not supported */
					*piPId=0x0;
					return FALSE;
				#endif
				}
			}
		}
	}

	/* Use integer */
	else
	{
		switch (*piPId)
		{

/* unknown --> use wml 1.2 */
/*		case 0x01:
			Unknown 
			*piPId=0;
			return TRUE;
*/
		case 0x02:
			/* wml 1.0 --> unknown (not supported) */
			*piPId=0x0;
			return FALSE;
		case 0x03:
			/* wta event --> unknown (not supported) */
			*piPId=0;
			return FALSE;
		case 0x04:
			/* wml 1.1 */
			*piPId=0x14;
			return TRUE;
		case 0x05:
			/* si 1.0 */
			*piPId=0x2E;
			return TRUE;
		case 0x06:
			/* sl 1.0 */
			*piPId=0x30;
			return TRUE;
		case 0x07:
			/* co 1.0 */
			*piPId=0x32;
			return TRUE;
		case 0x08:
			/* channel 1.1 */
			*piPId=0x16;
			return TRUE;
		case 0x09:
			/* wml 1.2 */
			*piPId=0x14;
			return TRUE;
		}
	}

	/* Other --> assume wml */
	*piPId=0x14;
	return FALSE;
}


BOOL WBXML_GetContentType (BYTE* pbData, BYTE* pbEnd, BYTE* pbContentType)
{
	pWBXMLDECODESTR pDecStr=NULL;
	BOOL fResult=FALSE;

	if (pbData<pbEnd)
	{
		/* Create tWBXMLDECODESTR and init the attributes */
		pDecStr=WBXML_CreateWBXMLDecStr();

		if (pDecStr!=NULL)
		{
			/* Set the function pointers */
			pDecStr->DTD_CreateElement=NULL;
			pDecStr->DTD_DeleteElement=NULL;
			pDecStr->DTD_AddContent=NULL;
			pDecStr->DTD_GetContent=NULL;
			pDecStr->DTD_AppSpecParseMod=NULL;
			pDecStr->DTD_StoreAttributeValue=NULL;
			pDecStr->DTD_GetAttributeValue=NULL;
			pDecStr->DTD_LiteralAttributeToToken=NULL;
			pDecStr->DTD_LiteralTagToToken=NULL;
			pDecStr->DTD_CheckPublicID=WBXML_CheckPublicID;

			/* Store data in the decode struct */
			pDecStr->bContentType=0; /* Unknown */
			pDecStr->iCharset=0;
			pDecStr->iContentLevel=0;
			pDecStr->pbCurByte=pbData;
			pDecStr->pbEnd=pbEnd;
			pDecStr->iDecodeResult=0;		
			pDecStr->pAppSpec=NULL;
			
			/* Parse prolog */
			fResult = WBXML_DecodeProlog(pDecStr);

			*pbContentType=(BYTE)pDecStr->iVersion;

			/* Delete decode struct */
			WBXML_DeleteWBXMLDecStr(&pDecStr);
		}
	}

	return fResult;
}
