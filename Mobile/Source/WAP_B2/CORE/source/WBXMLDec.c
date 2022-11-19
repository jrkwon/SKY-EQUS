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
		000306	JPR		Call stack reduction implemented

   ================================================================== */

#include "WBXMLDec.h"
#include "WBXMLElm.h"
#include "aapiclnt.h"
#include "WMLParse.h"
#include "trnscode.h"

/* WTA specific */
#ifdef CONFIG_WTA
	#include "WTAChDec.h"
#endif

/* PUSH specific */
#ifdef CONFIG_PUSH
	#include "P_SIDec.h"
	#include "P_SLDec.h"
	#include "P_CODec.h"
#endif



/* ------------------------------------------
	Call Stack Struct and functions
   ------------------------------------------ */

typedef struct tWBXMLCALLSTACK
{
	pELEMENTTYPE pContent;
	pTEXTELEMENT pNewText;
	BYTE *pbToken;
	BYTE bToken;
	BYTE bLiteralInfo;
	BYTE bEndTest;
	UINT32 iMBUINT;
	UINT32 iPos;
	BOOL fLiteral;
	pELEMENTTYPE pContentList;
	BYTE* pbCheck;
} WBXMLCALLSTACK, *pWBXMLCALLSTACK;


pWBXMLCALLSTACK WBXML_CreateCallStack (void)
{
	pWBXMLCALLSTACK pStack=NEWSTRUCT(WBXMLCALLSTACK);

	/* Set default values */
	if ( pStack !=NULL )
	{
		pStack->pContent=NULL;
		pStack->pNewText=NULL;
		pStack->pbToken=NULL;
		pStack->bToken=0;
		pStack->bLiteralInfo=0;
		pStack->bEndTest=0;
		pStack->iMBUINT=0;
		pStack->iPos=0;
		pStack->fLiteral=FALSE;
		pStack->pContentList=NULL;
		pStack->pbCheck=NULL;
	}

	return pStack;
}


/* -----------------------------------------------
				  Help functions.
   ----------------------------------------------- */
void WBXML_StepByte (pWBXMLDECODESTR pDecStr) 
{
	pDecStr->pbCurByte++;
}

WCHAR *WBXML_CopyCharToWChar (pWBXMLDECODESTR pDecStr, CHAR* src)
{
	WCHAR *pchTempValue=NULL;

	/* ASSERT: pDecStr!=NULL
			   src!=NULL
	*/

	pchTempValue=w_cstr2wstr(src);

	if (pchTempValue==NULL)
	{
		/* Could not create copy - out of memory. */
		pDecStr->fAbort=TRUE;
	}

	return pchTempValue;
}


/* -----------------------------------------------
	       Functions for the string table.
   ----------------------------------------------- */
/* -----------------------------------------------
	Returns a pointer to the string at the specified 
	location in the string table. When the parsing 
	is complete, the string table and the strings are 
	removed.
	Input: Pointer to WBXMLDECODESTR and Location 
		  (UINT32)
	Output: Pointer to WCHAR (NOT COPIED), NULL if 
			error.
   ----------------------------------------------- */
WCHAR *WBXML_GetStrTabVal (pWBXMLDECODESTR pDecStr, UINT32 iLoc) 
{
	BOOL fCont=TRUE;
	pSTRINGTABSTR pStrTemp=pDecStr->pStrTab;

	while ((pStrTemp!=NULL)&&(fCont))
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
		return pStrTemp->pchString;
	}
	else 
	{
		return NULL;
	}
}

void WBXML_DelStrTab (pWBXMLDECODESTR pDecStr) 
{
	pSTRINGTABSTR pStrTemp=NULL;

	while (pDecStr->pStrTab!=NULL) 
	{
		DEALLOC(&(pDecStr->pStrTab->pchString));
		pStrTemp=pDecStr->pStrTab;
		pDecStr->pStrTab=pDecStr->pStrTab->pNext;
		DEALLOC(&pStrTemp);
	}
}

/* -----------------------------------------------
				WBXML Parse Functions.
   ----------------------------------------------- */

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
		pDecStr->iDTD=0;
		pDecStr->iAttrCodePage=0;
		pDecStr->iTagCodePage=0;
		pDecStr->iError=0;
		pDecStr->fAbort=FALSE;	
		pDecStr->iViewID=0;
		pDecStr->pStrTab=NULL;
		pDecStr->pAppSpec=NULL;
	}

	return pDecStr;
}

void WBXML_DeleteWBXMLDecStr (pWBXMLDECODESTR *ppDecStr)
{
	if (*ppDecStr!=NULL)
	{
		WBXML_DelStrTab (*ppDecStr);
		DEALLOC(ppDecStr);
	}
}

WCHAR *WBXML_GetSTR_I (pWBXMLDECODESTR pDecStr) 
{
	INT16 iCharset=pDecStr->iCharset;
	INT32 iLength=0;
	UINT32 iBytes=0;
	WCHAR *pchResult=NULL;

	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		/* Calculate length */
		iLength=Iana2Unicode_calcLen (pDecStr->pbCurByte, iCharset, TRUE,
						pDecStr->pbEnd-pDecStr->pbCurByte, &iBytes);

		/* Check result */
		if (iLength<0)
		{
			/* Not OK. Interpret as iso-latin-1 */
			CLNTa_error (pDecStr->iViewID,ERR_WAE_WML_CONTENT_CHARSET_ERROR,
								ERRTYPE_INFORMATION);
			pDecStr->iError|=ERROR_Text_Encoding;

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
				CLNTa_error (pDecStr->iViewID,ERR_WAE_WML_CONTENT_CHARSET_ERROR,
								ERRTYPE_INFORMATION);
				pDecStr->fAbort=TRUE;
			}
		}
		else
		{
			/* Out of memory */
			pDecStr->fAbort=TRUE;
		}
	}

	/* Return string */
	return pchResult;
}


/* ( inline | tableref | extension string ) */
WCHAR *WBXML_ParseGlobalString (pWBXMLDECODESTR pDecStr)
{
	BYTE *pbTemp=NULL;
	UINT32 iStringTabLoc=0;
	UINT32 iPos=0;

	pbTemp=pDecStr->pbCurByte;

	/* Check for string termination */
	if (pbTemp<pDecStr->pbEnd)
	{
		switch (*pbTemp) 
		{
		case GLOBAL_STR_I:
		case GLOBAL_EXT_I_0:
		case GLOBAL_EXT_I_1:
		case GLOBAL_EXT_I_2:

			/* Inline string */
			WBXML_StepByte(pDecStr);			
			return (WBXML_GetSTR_I(pDecStr));

		case GLOBAL_STR_T:
		case GLOBAL_EXT_T_0:
		case GLOBAL_EXT_T_1:
		case GLOBAL_EXT_T_2:

			/* String table reference */
			WBXML_StepByte(pDecStr);
			
			if (pDecStr->pbCurByte<pDecStr->pbEnd)
			{
				ReadMBUInt32(&iStringTabLoc,pDecStr->pbCurByte,
								(UINT32)(pDecStr->pbEnd-pDecStr->pbCurByte),&iPos);
					
				pDecStr->pbCurByte+=iPos;

				return CreateStringCopy(WBXML_GetStrTabVal(pDecStr,iStringTabLoc));
			}
			break;
		case GLOBAL_EXT_0:
		case GLOBAL_EXT_1:
		case GLOBAL_EXT_2:
			/* Single byte document-type-specific-data */
			WBXML_StepByte(pDecStr);
			break;
		}
	}
	else
	{
		/* Fatal error */
		pDecStr->fAbort=TRUE;
	}
	return NULL;
}


BOOL WBXML_CheckIfText (pWBXMLDECODESTR pDecStr)
{
	/* Check if ( string | extension | entity ) */
	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		switch (*pDecStr->pbCurByte)
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
	}
	return FALSE;
}

/* Returns TRUE if Attribute Value. That is NOT
   Attribute Start Token or Global token END or 
   LITERAL or end of instream.  */
BOOL WBXML_CheckIfAttributeValue (pWBXMLDECODESTR pDecStr)
{
	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		switch (*pDecStr->pbCurByte)
		{
		case GLOBAL_SWITCH_PAGE:
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
			return TRUE;

		case GLOBAL_END:
		case GLOBAL_LITERAL:
		case GLOBAL_LITERAL_C:
		case GLOBAL_LITERAL_A:
		case GLOBAL_LITERAL_AC:
			return FALSE;
		}

		if (*pDecStr->pbCurByte>127)
		{
			/* Not Attribute Start Token */
			return TRUE;
		}
	}
	return FALSE;
}

/* ( string | extension | entity ) */
pTEXTELEMENT WBXML_ParseGlobalText (pWBXMLDECODESTR pDecStr) 
{
	pTEXTELEMENT pNewText=NULL;
	WCHAR *pchString=NULL;
	UINT32 iEntity=0;
	UINT32 iPos=0;
	BYTE *pbTemp=NULL;
	BOOL fCont=TRUE;
	enum TextType iType;

	while ((pDecStr->pbCurByte<pDecStr->pbEnd)&&(fCont))
	{
		/* Store all consecutive strings, extensions or entities. */
		
		/* Check if ( string | extension | entity ) */
		if (WBXML_CheckIfText(pDecStr))
		{
			if (*pDecStr->pbCurByte==GLOBAL_ENTITY)
			{
				WBXML_StepByte(pDecStr);

				if (pDecStr->pbCurByte<pDecStr->pbEnd)
				{	
					iPos=0;
					if (ReadMBUInt32(&iEntity,pDecStr->pbCurByte,
								pDecStr->pbEnd-pDecStr->pbCurByte,&iPos))
					{
						/*	Add to text. */
						if (!Text_AddEntity(&pNewText,iEntity))
						{
							/* Fatal error */
							pDecStr->fAbort=TRUE;
						}

						pDecStr->pbCurByte+=iPos;
					}
					else 
					{
						/* Fatal error. Error in encoding of MBUINT32. */
						pDecStr->fAbort=TRUE;
					}
				}
			}
			else
			{
				pbTemp=pDecStr->pbCurByte;

				/* Get string */
				pchString=WBXML_ParseGlobalString(pDecStr);
				
				if (pchString!=NULL) 
				{
					/* Check if extension. Add to text */
					iType=WBXML_GetTextType(*pbTemp);
					
					if (iType!=T_String) 
					{
						if (!Text_AddExtension(&pNewText,pchString,iType))
						{
							/* Out of memory. Fatal error. */
							pDecStr->fAbort=TRUE;
						}
					} 
					else 
					{
						if (!Text_AddString(&pNewText,pchString))
						{
							/* Out of memory. Fatal error. */
							pDecStr->fAbort=TRUE;
						}
					}
				} 
				else 
				{
					/* Out of memory. Fatal error. Or string not
					   found in string table. */
					pDecStr->iError|=ERROR_String_Table_Reference;
					fCont=FALSE;
				}
			}
		}
		else
		{
			/* Not text - quit */
			fCont=FALSE;
		}
	}

	return pNewText;
}

pTEXTELEMENT WBXML_ParseAttributeValue (pWBXMLDECODESTR pDecStr)
{
	BOOL fCont=TRUE;
	BYTE *pbCheck=NULL;
	pTEXTELEMENT pNewText=NULL;
	pTEXTELEMENT pAttrText=NULL;
	WCHAR *pchString=NULL;

	/* Get all attribute values */
	while ((pDecStr->pbCurByte<pDecStr->pbEnd)&&(fCont))
	{
		pbCheck=pDecStr->pbCurByte;

		if (WBXML_CheckIfAttributeValue(pDecStr))
		{
			/* Get attribute value */

			/* Switch page */
			if (*pDecStr->pbCurByte==GLOBAL_SWITCH_PAGE)
			{
				WBXML_StepByte(pDecStr);

				/* Switch Attribute Code Page */
				if (pDecStr->pbCurByte<pDecStr->pbEnd)
				{
					pDecStr->iAttrCodePage=*pDecStr->pbCurByte;
					WBXML_StepByte(pDecStr);
				}
			}
			else if (WBXML_CheckIfText(pDecStr))
			{
				/* Get global text */
				pNewText=WBXML_ParseGlobalText (pDecStr);

				/* Append to pNewText */
				Text_AppendText(&pAttrText,pNewText);
			}
			else 
			{
				/* Get application specific value (check code page in function) */
				switch (pDecStr->iDTD)
				{
				case DTD_WML:
					pchString=WML_ParseAttributeValue (pDecStr);
					break;
				
				/* WTA specific */
				#ifdef CONFIG_WTA
				case DTD_CHANNEL:
					pchString=Ch_ParseAttributeValue (pDecStr);
					break;
				#endif

				/* PUSH specific */
				#ifdef CONFIG_PUSH
				case DTD_PUSH_SI:
					pchString=Si_ParseAttributeValue (pDecStr);
					break;
				case DTD_PUSH_SL:
					pchString=Sl_ParseAttributeValue (pDecStr);
					break;
				case DTD_PUSH_CO:
					pchString=Co_ParseAttributeValue (pDecStr);
					break;
				#endif
				}

				/* Add to pAttrText */
				if (!Text_AddString(&pAttrText,pchString))
				{
					/* Out of memory. Fatal error. */
					pDecStr->fAbort=TRUE;
				}

				if (pbCheck==pDecStr->pbCurByte)
				{
					/* Not moved - step! */
					WBXML_StepByte(pDecStr);
				}
			}
		}
		else
		{
			/* Quit */
			fCont=FALSE;
		}
	}

	return pAttrText;
}

void WBXML_ParseAttributes (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pElement)
{
	BOOL fCont=TRUE;
	BYTE bCheck=0;
	UINT32 iMBUINT=0;
	UINT32 iPos=0;

	/* Get all attributes */
	while ((pDecStr->pbCurByte<pDecStr->pbEnd)&&(fCont))
	{
		bCheck=*pDecStr->pbCurByte;
		WBXML_StepByte(pDecStr);

		/* End */
		switch (bCheck)
		{
		case GLOBAL_END:
			fCont=FALSE;		
			break;

		/* Switch page */
		case GLOBAL_SWITCH_PAGE:
			/* Switch Attribute Code Page */
			if (pDecStr->pbCurByte<pDecStr->pbEnd)
			{
				pDecStr->iAttrCodePage=*pDecStr->pbCurByte;
				WBXML_StepByte(pDecStr);
			}
			break;

		/* Literal */
		case GLOBAL_LITERAL:
			if (pDecStr->pbCurByte<pDecStr->pbEnd)
			{
				/* Get Attribute name index */
				iPos=0;

				if (ReadMBUInt32(&iMBUINT,pDecStr->pbCurByte,
							pDecStr->pbEnd-pDecStr->pbCurByte,&iPos))
				{
					pDecStr->pbCurByte+=iPos;
				}
				else 
				{
					/* Fatal error. Error in encoding of MBUINT32. */
					pDecStr->fAbort=TRUE;
				}

				switch (pDecStr->iDTD)
				{
			 	case DTD_WML:	
					/* Get token name and convert to byte */
					bCheck=WML_ConvertAttributeToByte (pDecStr, iMBUINT);
					break;

				/* WTA specific */
				#ifdef CONFIG_WTA
			 	case DTD_CHANNEL:	
					bCheck=Ch_ConvertAttributeToByte (pDecStr, iMBUINT);
					break;
				#endif

				/* PUSH specific */
				#ifdef CONFIG_PUSH
				case DTD_PUSH_SI:
					bCheck=Si_ConvertAttributeToByte (pDecStr, iMBUINT);
					break;
				case DTD_PUSH_SL:
					bCheck=Sl_ConvertAttributeToByte (pDecStr, iMBUINT);
					break;
				case DTD_PUSH_CO:
					bCheck=Co_ConvertAttributeToByte (pDecStr, iMBUINT);
					break;
				#endif
				}
			}
		/* Other */
		default:
			/* Get application specific value (check code page in function) */
			switch (pDecStr->iDTD)
			{
			case DTD_WML:
				WML_ParseAttribute (pDecStr,pElement,bCheck);
				break;

			/* WTA specific */
			#ifdef CONFIG_WTA
			case DTD_CHANNEL:
				Ch_ParseAttribute (pDecStr,pElement,bCheck);
				break;
			#endif

			/* PUSH specific */
			#ifdef CONFIG_PUSH
			case DTD_PUSH_SI:
				Si_ParseAttribute (pDecStr,pElement,bCheck);
				break;
			case DTD_PUSH_SL:
				Sl_ParseAttribute (pDecStr,pElement,bCheck);
				break;
			case DTD_PUSH_CO:
				Co_ParseAttribute (pDecStr,pElement,bCheck);
				break;
			#endif

			}
			break;
		}
	}
}

/* Application specific modifications */
void XML_AppSpecParseMod (pWBXMLDECODESTR pDecStr, 
								pELEMENTTYPE *ppElement, pELEMENTTYPE pParent)
{
	switch (pDecStr->iDTD)
	{
	case DTD_WML:
		WML_ModifyParse (pDecStr,ppElement,pParent);
		break;

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		/* NO CHANGES NEEDED FOR CHANNEL 
		Ch_ModifyParse (pDecStr,ppElement,pParent);
		*/
		break;
	#endif

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
	case DTD_PUSH_SL:
	case DTD_PUSH_CO:
		/* NO CHANGES NEEDED FOR PUSH */
		break;

	#endif

	}
}

/* ------------------------------------------------
	Parses the instream for ONE element. Creates the 
	appropriate element	and parses the attributes 
	and the content.

	Element:
	( stag [ 1*attribute END ] [ *content END ] )

	Content:
	( element | string | extension | entity | pi 
	| opaque )

	Input: Pointer to WBXMLDECODESTR and parent 
		   element.
	Output: Pointer to an element or NULL

	THIS FUNCTION USES A STRUCT (tWBXMLCALLSTACK)
	INSTEAD OF LOCAL VARIABLES. THIS SAVES MEMORY
	ON THE CALL STACK.
   ------------------------------------------------*/
pELEMENTTYPE WBXML_ParseElement (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pParent)
{
	pELEMENTTYPE pNewElement=NULL;

	/* Create locla call stack (to save memory on "real" call stack. */
	pWBXMLCALLSTACK pStack=WBXML_CreateCallStack();
	
	if (pStack!=NULL)
	{
		if (pDecStr->pbCurByte<pDecStr->pbEnd)
		{
			pStack->pbToken=pDecStr->pbCurByte;
			pStack->bToken=*(pStack->pbToken);

			switch (*(pStack->pbToken))
			{
			/* SWITCH PAGE */
			case GLOBAL_SWITCH_PAGE:
				WBXML_StepByte(pDecStr);

				/* Switch Tag Code Page */
				if (pDecStr->pbCurByte<pDecStr->pbEnd)
				{
					pDecStr->iTagCodePage=*pDecStr->pbCurByte;
					WBXML_StepByte(pDecStr);
				}
				break;
			
			/* END */
			case GLOBAL_END:

				/* Step */
				WBXML_StepByte(pDecStr);

				/* End of element */
				break;

			/* PI */
			case GLOBAL_PI:
				
				WBXML_StepByte(pDecStr);
				WBXML_ParseAttributes(pDecStr,NULL);
				break;

			/* Opaque */
			case GLOBAL_OPAQUE:

				WBXML_StepByte(pDecStr);

				/* Step past data */
				if (pDecStr->pbCurByte<pDecStr->pbEnd)
				{
					pStack->iPos=0;
					if (ReadMBUInt32(&(pStack->iMBUINT),pDecStr->pbCurByte,
								pDecStr->pbEnd-pDecStr->pbCurByte,&(pStack->iPos)))
					{
						pDecStr->pbCurByte+=(pStack->iPos)+(pStack->iMBUINT);
					}
					else 
					{
						/* Fatal error. Error in encoding of MBUINT32. */
						pDecStr->fAbort=TRUE;
					}
				}

				break;

			/* TEXT */
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

				/* Create text element */
				pNewElement=XML_CreateElement(Type_Text,pDecStr->iDTD,pDecStr->iViewID);

				if (pNewElement!=NULL)
				{
					pStack->pNewText=WBXML_ParseGlobalText(pDecStr);

					if (pStack->pNewText!=NULL)
					{
						pNewElement->pElm=pStack->pNewText;
					}
					else
					{
						DEALLOC(&pNewElement);
					}
				}

				break;

			/* LITERAL */
			case GLOBAL_LITERAL:
			case GLOBAL_LITERAL_C:
			case GLOBAL_LITERAL_A:
			case GLOBAL_LITERAL_AC:

				pStack->fLiteral=TRUE;

				/* Get 'mask' */
				pStack->bLiteralInfo=(BYTE)((pStack->bToken)&BOTH);

				/* Step */
				WBXML_StepByte(pDecStr);

				if (pDecStr->pbCurByte<pDecStr->pbEnd)
				{
					/* Get Tag name index */
					pStack->iPos=0;

					if (ReadMBUInt32(&(pStack->iMBUINT),pDecStr->pbCurByte,
								pDecStr->pbEnd-pDecStr->pbCurByte,&(pStack->iPos)))
					{
						pDecStr->pbCurByte+=pStack->iPos;
					}
					else 
					{
						/* Fatal error. Error in encoding of MBUINT32. */
						pDecStr->fAbort=TRUE;
					}

					/* Get token name and convert to byte */
					switch (pDecStr->iDTD)
					{
					case DTD_WML:	
						/* Get token name and convert to byte */
						pStack->bToken=WML_ConvertTagToByte (pDecStr, pStack->iMBUINT);
						break;

					/* WTA specific */
					#ifdef CONFIG_WTA
					case DTD_CHANNEL:	
						pStack->bToken=Ch_ConvertTagToByte (pDecStr, pStack->iMBUINT);
						break;
					#endif

					/* PUSH specific */
					#ifdef CONFIG_PUSH
					case DTD_PUSH_SI:
						pStack->bToken=Si_ConvertTagToByte (pDecStr, pStack->iMBUINT);
						break;
					case DTD_PUSH_SL:
						pStack->bToken=Sl_ConvertTagToByte (pDecStr, pStack->iMBUINT);
						break;
					case DTD_PUSH_CO:
						pStack->bToken=Co_ConvertTagToByte (pDecStr, pStack->iMBUINT);
						break;
					#endif

					}
					
					/* Add 'mask' */
					(pStack->bToken)|=(pStack->bLiteralInfo);
				}
				else
				{
					/* Error */
					break;
				}
				
			/* Element */
			default:

				if (!(pStack->fLiteral))
				{
					/* Step */
					WBXML_StepByte(pDecStr);
				}

				/* Create element */
				pNewElement=XML_CreateElement((UINT8)((pStack->bToken)&MASK),
															pDecStr->iDTD,pDecStr->iViewID);

				/* Parse attributes */
				if (((pStack->bToken)&ATTR)&&(!pDecStr->fAbort))
				{
					WBXML_ParseAttributes(pDecStr,pNewElement);
				}

				/* Parse content - one element a time */
				if ((pStack->bToken)&CONT)
				{
					/* WML-specific - section 12.4 in the wml specification. */

					if (pDecStr->pbCurByte<pDecStr->pbEnd)
					{
						pStack->bEndTest=*pDecStr->pbCurByte;
					}

					while ((pStack->bEndTest!=GLOBAL_END)&&(!pDecStr->fAbort))
					{
						pStack->pbCheck=pDecStr->pbCurByte;

						/* WML-specific - section 12.4 in the wml specification. */
						if ((pNewElement==NULL)&&(pDecStr->iDTD==DTD_WML))
						{
							pStack->pContent=WBXML_ParseElement (pDecStr,pParent);
						}
						else
						{
							pStack->pContent=WBXML_ParseElement (pDecStr,pNewElement);
						}

						if (pDecStr->pbCurByte<pDecStr->pbEnd)
						{
							pStack->bEndTest=*pDecStr->pbCurByte;
						}

						if (pNewElement!=NULL)
						{						
							/* Store content - if it is not stored it is deleted */
							XML_AddContent (pNewElement,pStack->pContent,pDecStr->iDTD);
							pStack->pContent=NULL;
						}

						/* WML-specific - section 12.4 in the wml specification. */
						else if (pDecStr->iDTD==DTD_WML)
						{
							/* pNewElement is NULL. Store content in a list and
							   return content instead. */
							WML_AddContentToEnd (&(pStack->pContentList),pStack->pContent);
						}

						/* Check if the byte stream pointer has moved */
						if (pDecStr->pbCurByte==pStack->pbCheck)
						{
							/* NOT MOVED - Quit */
							pDecStr->fAbort=TRUE;
						}
					}

					/* WML-specific - section 12.4 in the wml specification. */
					if ((pNewElement==NULL)&&(pDecStr->iDTD==DTD_WML))
					{
						/* Store content list as element. */
						pNewElement=pStack->pContentList;
					}

					/* Step past END-token */
					WBXML_StepByte(pDecStr);
				}

				if (pNewElement!=NULL)
				{
					/* Application specific modifications */
					XML_AppSpecParseMod (pDecStr,&pNewElement,pParent);
				}

				/* Nothing ? */
				if (pStack->pbToken==pDecStr->pbCurByte)
				{
					/* Not moved - set error code */
					pDecStr->iError|=ERROR_Unknown_Token;

					/* Step */
					WBXML_StepByte(pDecStr);
				}
			}
		}
		else
		{
			/* Error in instream */
			pDecStr->fAbort=TRUE;
		}
	}

	/* Delete Local Stack */
	DEALLOC (&pStack);

	/* Return element */
	return pNewElement;
}

/* Check publicID */
BOOL WBXML_CheckPublicIDWChar (pWBXMLDECODESTR pDecStr, WCHAR *pchId)
{
	switch (pDecStr->iDTD)
	{
	case DTD_WML:
		return WML_CheckPublicIDWChar (pDecStr, pchId);

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		return Ch_CheckPublicIDWChar (pDecStr, pchId);
	#endif	

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
		return Si_CheckPublicIDWChar (pDecStr, pchId);
		break;
	case DTD_PUSH_SL:
		return Sl_CheckPublicIDWChar (pDecStr, pchId);
		break;
	case DTD_PUSH_CO:
		return Co_CheckPublicIDWChar (pDecStr, pchId);
		break;
	#endif

	}
	return FALSE;
}

BOOL WBXML_CheckPublicIDInt (pWBXMLDECODESTR pDecStr, UINT8 iId)
{
	switch (pDecStr->iDTD)
	{
	case DTD_WML:
		return WML_CheckPublicIDInt (pDecStr, iId);

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		return Ch_CheckPublicIDInt (pDecStr, iId);
	#endif

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
		return Si_CheckPublicIDInt (pDecStr, iId);
		break;
	case DTD_PUSH_SL:
		return Sl_CheckPublicIDInt (pDecStr, iId);
		break;
	case DTD_PUSH_CO:
		return Co_CheckPublicIDInt (pDecStr, iId);
		break;
	#endif

	}
	return FALSE;
}

/* Set decode charset */
void WBXML_SetDecodeCharset (pWBXMLDECODESTR pDecStr, INT16 iCharset)
{
	switch (pDecStr->iDTD)
	{
	case DTD_WML:
		WML_SetParserCharset (pDecStr, iCharset);
		break;

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		Ch_SetParserCharset (pDecStr, iCharset);
		break;
	#endif

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
		Si_SetParserCharset (pDecStr, iCharset);
		break;
	case DTD_PUSH_SL:
		Sl_SetParserCharset (pDecStr, iCharset);
		break;
	case DTD_PUSH_CO:
		Co_SetParserCharset (pDecStr, iCharset);
		break;
	#endif

	}
}

BOOL WBXML_ParseProlog (pWBXMLDECODESTR pDecStr)
{
	UINT32 iPublicID=0;
	UINT32 iIndex=0;
	UINT32 iStrTabLen=0;
	UINT32 iPos=0;
	UINT32 iCharset=0;
	BOOL fPublicIDisString=FALSE;
	pSTRINGTABSTR pStrTemp=NULL;
	pSTRINGTABSTR pStrPrev=NULL;
	BYTE *pbStrTabEnd=NULL;
	BYTE *pbStrTabStart=NULL;
	WCHAR *pchPublicID=NULL;

	/* Version number */
	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		/* Check version number */
		if (*pDecStr->pbCurByte!=WBXML_VERSION_NUMBER)
		{
			/* Wrong WBXML version number. Send warning to the application. */
			CLNTa_error (pDecStr->iViewID,ERR_WAE_WML_CONTENT_VERSION_WARNING,
							ERRTYPE_INFORMATION);
		}

		WBXML_StepByte (pDecStr);
	}

	/* Parse public ID */
	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		if (*pDecStr->pbCurByte==0) 
		{
			WBXML_StepByte (pDecStr);

			if (pDecStr->pbCurByte<pDecStr->pbEnd)
			{
				iPos=0;
				/* Encoded as string. Get location. Get the
				   string later, when the string table is parsed. */
				if (ReadMBUInt32(&iPublicID,pDecStr->pbCurByte,
							pDecStr->pbEnd-pDecStr->pbCurByte,&iPos))
				{
					pDecStr->pbCurByte+=iPos;

					fPublicIDisString=TRUE;
				} 
				else
				{
					/* Error in encoding of publicID */
					return FALSE;
				}
			}
		}
		else
		{
			/* Get PublicID as integer. */
			if (ReadMBUInt32(&iPublicID,pDecStr->pbCurByte,
						pDecStr->pbEnd-pDecStr->pbCurByte,&iPos))
			{
				pDecStr->pbCurByte+=iPos;
			} 
			else
			{
				/* Error in encoding of publicID */
				return FALSE;
			}
		}
	}

	/* Charset */
	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		/* WAP 1.1 */

		/* Get Charset. */
		iPos=0;

		if (ReadMBUInt32(&iCharset,pDecStr->pbCurByte,
					pDecStr->pbEnd-pDecStr->pbCurByte,&iPos))
		{
			pDecStr->pbCurByte+=iPos;
		} 
		else
		{
			/* Error in encoding of Charset */
			return FALSE;
		}

		/* Set charset */
		WBXML_SetDecodeCharset (pDecStr, (INT16) iCharset);
	}

	/* String Table */

	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		/* Get string table length. */
		iPos=0;

		if (ReadMBUInt32(&iStrTabLen,pDecStr->pbCurByte,
					pDecStr->pbEnd-pDecStr->pbCurByte,&iPos))
		{
			pDecStr->pbCurByte+=iPos;

			/* Set min (pDecStr->pbEnd,pDecStr->pbCurByte+iStrTabLen) */
			pbStrTabEnd=pDecStr->pbCurByte+iStrTabLen;
			pbStrTabStart=pDecStr->pbCurByte;

			if (pbStrTabEnd>pDecStr->pbEnd)
			{
				/* String table longer than instream */
				return FALSE;
			}
		} 
		else
		{
			/* Error in encoding of string table length */
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	while (pDecStr->pbCurByte<pbStrTabEnd)
	{
		/* Parse the instream and copy the string
		   table into the memory. */
		
		/* Create new String table element */
		pStrTemp=NEWSTRUCT(STRINGTABSTR);

		if (pStrTemp!=NULL) 
		{
			pStrTemp->pchString=WBXML_GetSTR_I (pDecStr);
			pStrTemp->iOffset=iIndex;

			if (pStrTemp->pchString==NULL)
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
			pDecStr->fAbort=TRUE;

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
		pchPublicID=CreateStringCopy(WBXML_GetStrTabVal(pDecStr,iPublicID));

		if (WBXML_CheckPublicIDWChar (pDecStr,pchPublicID))
		{
			/* OK */
			DEALLOC(&pchPublicID);
		}
		else
		{
			DEALLOC(&pchPublicID);

			/* Delete string table */
			WBXML_DelStrTab (pDecStr);

			return FALSE;
		}
	}
	else
	{
		if (!WBXML_CheckPublicIDInt (pDecStr,(UINT8)iPublicID))
		{
			/* Not OK */

			/* Delete string table */
			WBXML_DelStrTab (pDecStr);
			
			return FALSE;
		}
	}

	/* OK! */
	return TRUE;
}
