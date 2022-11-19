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
	FILE: WBXMLElm.c
  
	General WBXML-decoder. Element handler.
	Source file.

	Rev history:
		990302	JPR		Created
		990309	JPR		Modified
		990512	JPR		Correction
		990813	JPR		Additions to support channel decoding
		991116	JPR		Additions to support PUSH Content Types

   ================================================================== */

#include "WBXMLElm.h"

/* ------------------------------------------
   Allocates memory for a new string and copies
   the specified string into the memory.
   Input: WCHAR *
   Output: WCHAR *, NULL if error
   ------------------------------------------*/
WCHAR *CreateStringCopy (WCHAR *pchString) 
{
	WCHAR *pchTempText=NULL;

	if (pchString!=NULL) 
	{
		pchTempText=NEWARRAY(WCHAR,STRINGLENGTH(pchString)+1);
		if (pchTempText!=NULL) 
		{
			COPYSTRING(pchTempText,pchString);
		} 
	}
	return pchTempText;
}

/* ----------------------------------------------
    Functions for handling the TEXTELEMENT       
   ---------------------------------------------- */

pELEMENTTYPE XML_CreateTextElement (pTEXTELEMENT *ppText)
{
	pELEMENTTYPE pNewElement=NULL;

	/* Create super element (ViewID not needed) */
	pNewElement=XML_CreateElement (Type_Text,0,0);

	if (pNewElement!=NULL)
	{
		/* Add TEXTELEMENT */
		pNewElement->pElm=*ppText;
	}
	else
	{
		/* Delete TEXTELEMENT */
		Text_DeleteText(ppText);
	}

	return pNewElement;
}

pTEXTELEMENT Text_CreateText (void) 
{
	pTEXTELEMENT pText=NULL;

	pText=NEWSTRUCT(TEXTELEMENT);
	if (pText!=NULL) 
	{
		pText->iEntity=0;
		pText->pchTextData=NULL;
		pText->pNextText=NULL;
	}
	return pText;
}

/* Deletes a text */
void Text_DeleteText (pTEXTELEMENT *ppText)
{
	pTEXTELEMENT pTemp=*ppText;
	pTEXTELEMENT pOld=NULL;

	while (pTemp!=NULL)
	{
		pOld=pTemp;
		DEALLOC(&pOld->pchTextData);
		pTemp=pTemp->pNextText;
		DEALLOC(&pOld);
	}
}

BOOL Text_AddString (pTEXTELEMENT *ppText, WCHAR *pchTextData)
{
	pTEXTELEMENT pNewText=NULL;
	pTEXTELEMENT pTemp=*ppText;
	
	if (pchTextData!=NULL) 
	{
		pNewText=Text_CreateText();

		/* Check if created */
		if (pNewText!=NULL) 
		{
			pNewText->iType=T_String;
			pNewText->pchTextData=pchTextData;
			
			if (*ppText==NULL)
			{
				*ppText=pNewText;				
			}
			else 
			{
				while (pTemp->pNextText!=NULL)
				{
					pTemp=pTemp->pNextText;
				}
				pTemp->pNextText=pNewText;
			}
			return TRUE;
		} 
		else
		{
			/* Not created - remove pchTextData */
			DEALLOC(&pchTextData);
		}
	}
	return FALSE;
}

BOOL Text_AddHeadString (pTEXTELEMENT *ppText, WCHAR *pchTextData)
{
	pTEXTELEMENT pNewText=NULL;
	
	if (pchTextData!=NULL) 
	{
		pNewText=Text_CreateText();

		/* Check if created */
		if (pNewText!=NULL) 
		{
			pNewText->iType=T_String;
			pNewText->pchTextData=pchTextData;
			pNewText->pNextText=*ppText;
			*ppText=pNewText;	
			
			return TRUE;
		} 
		else
		{
			/* Not created - remove pchTextData */
			DEALLOC(&pchTextData);
		}
	}
	return FALSE;
}


BOOL Text_AddEntity (pTEXTELEMENT *ppText, UINT32 iEntity)
{
	pTEXTELEMENT pNewText=NULL;
	pTEXTELEMENT pTemp=*ppText;
	
	pNewText=Text_CreateText();

	/* Check if created */
	if (pNewText!=NULL) 
	{
		pNewText->iType=T_Entity;
		pNewText->pchTextData=NULL;
		pNewText->iEntity=iEntity;
		
		if (*ppText==NULL)
		{
			*ppText=pNewText;				
		}
		else 
		{
			while (pTemp->pNextText!=NULL)
			{
				pTemp=pTemp->pNextText;
			}
			pTemp->pNextText=pNewText;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL Text_AddExtension (pTEXTELEMENT *ppText, WCHAR *pchData, enum TextType iExtType)
{
	pTEXTELEMENT pNewText=NULL;
	pTEXTELEMENT pTemp=*ppText;
 	
	if (pchData!=NULL) 
	{
		pNewText=Text_CreateText();

		if (pNewText!=NULL) 
		{
			pNewText->iType=iExtType;
			pNewText->pchTextData=pchData;

			if (*ppText==NULL)
			{
				*ppText=pNewText;				
			}
			else 
			{
				while (pTemp->pNextText!=NULL)
				{
					pTemp=pTemp->pNextText;
				}
				pTemp->pNextText=pNewText;
			}
			return TRUE;		
		}  
		else
		{
			/* Not created - remove pchTextData */
			DEALLOC(&pchData);
		}
	}
	return FALSE;
}


UINT32 Text_GetLength (pTEXTELEMENT pText)
{
	pTEXTELEMENT pTemp=pText;
	UINT32 iLength=0;

	/* ASSERT:	pText!=NULL
	*/

	while (pTemp!=NULL)
	{
		if (pTemp->iType==T_String)
		{
			/* pchTextData is not NULL - operation ok */
			iLength+=(STRINGLENGTH(pTemp->pchTextData));
		}
		else if (pTemp->iType==T_Entity)
		{
			/* Add one WCHAR */
			iLength++;
		}

		/* Ignore extensions */

		pTemp=pTemp->pNextText;
	}

	return iLength;
}

WCHAR *Text_GetString(pTEXTELEMENT pText, UINT32 iStrLen)
{
	WCHAR *pchNewString=NULL;
	WCHAR *pchTempString=NULL;
	pTEXTELEMENT pTemp=pText;
	WCHAR pchEntity[2];

	/* ASSERT:	pText!=NULL
	*/

	if (iStrLen>0)
	{
		/* Allocate memory for string and termination character. */
		pchNewString=NEWARRAY(WCHAR,(iStrLen+1));

		if (pchNewString!=NULL)
		{
			/* Set termination character first */
			pchNewString[0]=W_EndOfText;
			pchTempString=pchNewString;

			while (pTemp!=NULL)
			{
				if (pTemp->iType==T_String)
				{
					CONCATSTRING(pchTempString,pTemp->pchTextData);
				}
				else if (pTemp->iType==T_Entity)
				{
					/* Convert entity to string and add to text. */
					pchEntity[1]=W_EndOfText;
					
					/* Cannot be represented in USC-2 
					   Character (HEX 7F) is shown instead. */
					if (pTemp->iEntity>65535)
					{
						pchEntity[0]=(WCHAR)('\x7F');
					}
					else
					{
						pchEntity[0]=(WCHAR)(pTemp->iEntity);
					}

					/* Add entity */
					CONCATSTRING(pchTempString,pchEntity);
				}
				pTemp=pTemp->pNextText;
			}
		}
	}

	return pchNewString;
}

WCHAR *Text_GetText (pTEXTELEMENT pText)
{
	WCHAR *pchString=NULL;
	UINT32 iStrLen=0;

	if (pText!=NULL)
	{
		/* Get text length */
		iStrLen=Text_GetLength (pText);

		/* Copy Text */
		pchString=Text_GetString (pText,iStrLen);
	}

	return pchString;
}


WCHAR *Text_ToString (pTEXTELEMENT *ppText)
{
	WCHAR *pchString=NULL;
	UINT32 iStrLen=0;

	if (*ppText!=NULL)
	{
		/* Get text length */
		iStrLen=Text_GetLength (*ppText);

		/* Copy Text */
		pchString=Text_GetString (*ppText,iStrLen);

		/* Remove text element */
		Text_DeleteText (ppText);
	}

	return pchString;
}

void Text_AppendText (pTEXTELEMENT *ppText, pTEXTELEMENT pNewText)
{
	pTEXTELEMENT pTemp=*ppText;

	if (pNewText!=NULL)
	{
		if (pTemp==NULL)
		{
			/* pNewText is the first element */
			*ppText=pNewText;
		}
		else
		{
			/* Find end */
			while (pTemp->pNextText!=NULL)
			{
				pTemp=pTemp->pNextText;
			}

			/* Add new text */
			pTemp->pNextText=pNewText;	
		}
	}
}

enum TextType WBXML_GetTextType (BYTE bByte) 
{	
	switch (bByte)
	{
	case GLOBAL_EXT_I_0:
	case GLOBAL_EXT_T_0:
	case GLOBAL_EXT_0:
		return T_Ext_0;
	case GLOBAL_EXT_I_1:
	case GLOBAL_EXT_T_1:
	case GLOBAL_EXT_1:
		return T_Ext_1;
	case GLOBAL_EXT_I_2:
	case GLOBAL_EXT_T_2:
	case GLOBAL_EXT_2:
		return T_Ext_2;
	}
	return T_String;
}

/* --------------------------------------------- */


/* The global types are constructed here and the application
   specific types are created in the specific files. */
pELEMENTTYPE XML_CreateElement (UINT8 iType, UINT8 iDTD, UINT8 iViewID)
{
	pELEMENTTYPE pElement=NULL;

	/* Global types */
	switch (iType)
	{
	case Type_Text:
		/* Text - content is created when needed */
		pElement=NEWSTRUCT(ELEMENTTYPE);

		if (pElement!=NULL) 
		{
			pElement->pNextElement=NULL;
			pElement->iType=iType;
			pElement->pElm=NULL;
		}
		return pElement;
	}

	/* Application specific types */
	switch (iDTD) 
	{
	case DTD_WML:
		pElement=WML_CreateElement(iType, iViewID);
		break;

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		pElement=Ch_CreateElement(iType, iViewID);
		break;
	#endif

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
		pElement=Si_CreateElement(iType, iViewID);
		break;
	case DTD_PUSH_SL:
		pElement=Sl_CreateElement(iType, iViewID);
		break;
	case DTD_PUSH_CO:
		pElement=Co_CreateElement(iType, iViewID);
		break;
	#endif
	
	}

	return pElement;
}

void XML_DeleteElement (pELEMENTTYPE pElement, UINT8 iDTD)
{
	switch (iDTD) 
	{
	case DTD_WML:
		WML_DeleteElement(pElement);
		break;

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		Ch_DeleteElement(pElement);
		break;
	#endif

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
		Si_DeleteElement(pElement);
		break;
	case DTD_PUSH_SL:
		Sl_DeleteElement(pElement);
		break;
	case DTD_PUSH_CO:
		Co_DeleteElement(pElement);
		break;
	#endif
	
	}
}

void XML_AddContent (pELEMENTTYPE pElement, pELEMENTTYPE pContent, UINT8 iDTD)
{
	switch (iDTD) 
	{
	case DTD_WML:
		WML_AddContent(pElement,pContent);
		break;

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		Ch_AddContent(pElement,pContent);
		break;
	#endif

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
		Si_AddContent(pElement,pContent);
		break;
	case DTD_PUSH_SL:
		Sl_AddContent(pElement,pContent);
		break;
	case DTD_PUSH_CO:
		Co_AddContent(pElement,pContent);
		break;
	#endif
	
	}
}

pELEMENTTYPE XML_GetContent (pELEMENTTYPE pElement,UINT8 iDTD)
{
	switch (iDTD) 
	{
	case DTD_WML:
		return WML_GetContent(pElement);

	/* WTA specific */
	#ifdef CONFIG_WTA
	case DTD_CHANNEL:
		return Ch_GetContent(pElement);
	#endif

	/* PUSH specific */
	#ifdef CONFIG_PUSH
	case DTD_PUSH_SI:
		return Si_GetContent(pElement);
	case DTD_PUSH_SL:
		return Sl_GetContent(pElement);
	case DTD_PUSH_CO:
		return Co_GetContent(pElement);
	#endif
	}
	return NULL;
}

pELEMENTTYPE XML_GetNextElement (pELEMENTTYPE pElementObj) 
{
	if (pElementObj!=NULL)
	{
		return pElementObj->pNextElement;
	}
	return NULL;
}

