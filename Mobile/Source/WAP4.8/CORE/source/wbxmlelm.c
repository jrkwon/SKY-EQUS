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
	FILE: wbxmlelm.c
  
	General WBXML-decoder. Element handler.	Source file.

	Rev history:
		990302	JPR		Created
		990309	JPR		Modified
		990512	JPR		Correction
		990813	JPR		Additions to support channel decoding
		991116	JPR		Additions to support PUSH Content Types

  		991125	JPR		Redesign of Element Handling, Text Handling,
						and Error Handling (Consult separate document).
						Application specific parts removed
		991202	JPR		XML_AddHeadString, FindStringBinarySearch, and 
						functions for creating and deleting empty 
						elements added.
		991206	JPR		Minor corrections
		000113	JPR		OPAQUE changed to XMLOPAQUE
		000120	JPR		Function FindStringBinarySearch corrected
		000131	JPR		Functions XML_AddContent and XML_GetContent added.
		000207	JPR		Memory leak removed in XML_AddHeadString
		001213	IPN		Memory leak removed in XML_BuildString
		010119	NKE		Updated XML_DeleteElement for the new decoder in wbxmldec.c.
   ================================================================== */

#include "wbxmlelm.h"
#include "wbxmldec.h"

/*========================================================================
	CreateStringCopy
==========================================================================*/
WCHAR* CreateStringCopy (WCHAR *pchString) 
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


/*========================================================================
	FindStringBinarySearch
==========================================================================*/
INT16 FindStringBinarySearch (const WCHAR* pwchString, 
							  const WCHAR* const ppwchStringTab[], UINT16 iTabLen)
{
	BOOL fCont=TRUE;
	INT8 iCheck=0;

	UINT16 iIndex=(UINT16)(iTabLen-1);
	UINT16 iMin=0;
	UINT16 iMax=(UINT16)(iTabLen-1);

	if (pwchString!=NULL)
	{
		/* Binary search */
		while ( (fCont) )
		{
			/* Check this index */
			iCheck=COMPARESTRING (pwchString,(ppwchStringTab[iIndex]));

			if (iCheck==0)
			{
				/* Found */
				return (INT16)(iIndex);
			}

			if (iMin>=iMax)
			{
				fCont=FALSE;
			}

			/* Find next element to check */
			if (iCheck<0)
			{
				/* Search left subtree */
				iMax=iIndex;
				iIndex=(UINT16)(iMin+((iIndex-iMin)/2));
			}
			else
			{
				/* Search right subtree */
				iMin=(UINT16)(iIndex+1);
				iIndex=(UINT16)(iIndex+((iMax-iIndex)/2));
			}
		}
	}

	/* Not found */
	return -1;
}


/*========================================================================

	Text Element functions

=========================================================================*/

/*========================================================================
	XML_GetStringLength
==========================================================================*/
UINT32 XML_GetStringLength (pELEMENTTYPE pStartElement, pWBXMLDECODESTR pDecStr)
{
	pELEMENTTYPE pTemp=pStartElement;
	UINT32 iLength=0;

	/* ASSERT:	pText!=NULL
	*/

	while (pTemp!=NULL)
	{
		/* Check type (Type_Inline_Text, Type_Table_Text or Type_Entity) */

		if (pTemp->iType==Type_Inline_Text)
		{
			/* pchTextData is not NULL - operation ok */
			iLength+=(STRINGLENGTH(((pI_TEXTELEMENT)pTemp)->pwchTextData));
		}
		else if (pTemp->iType==Type_Table_Text)
		{
			/* Get string from string table */
			WCHAR* pwchTemp=WBXML_GetStrTabVal (pDecStr,
									((pT_TEXTELEMENT)pTemp)->iTableRef);
			
			if (pwchTemp!=NULL)
			{
				/* Calculate length */
				iLength+=(STRINGLENGTH(pwchTemp));
			}
		}
		else if (pTemp->iType==Type_Entity)
		{
			/* Add one WCHAR */
			iLength++;
		}
		else
		{
			/* Not text element - quit */
			return iLength;
		}

		pTemp=pTemp->pNextElement;
	}

	return iLength;
}


/*========================================================================
	XML_GetString
==========================================================================*/

/* Help function to XML_GetString and XML_ConvertToString */
WCHAR* XML_BuildString (pELEMENTTYPE pStartElement, pWBXMLDECODESTR pDecStr,
						UINT32 iLength, pELEMENTTYPE* ppLastElement)
{
	WCHAR *pwchString=NULL;
	WCHAR *pwchTemp=NULL;
	pELEMENTTYPE pTemp=pStartElement;
	pELEMENTTYPE pLastUsed=NULL;
	WCHAR pwchEntity[2];

	if (iLength>0)
	{
		/* Allocate memory for string and termination character. */
		pwchString=NEWARRAY(WCHAR,(iLength+1));

		#ifndef HAS_SETJMP
		if (pwchString!=NULL)
		{
		#endif
		
		/* Set termination character first */
		pwchString[0]=0;
		pwchTemp=pwchString;

		while (pTemp!=NULL)
		{
			if (pTemp->iType==Type_Inline_Text)
			{
				CONCATSTRING(pwchTemp,((pI_TEXTELEMENT)pTemp)->pwchTextData);
			}
			else if (pTemp->iType==Type_Table_Text)
			{
				/* Get string from string table */
				WCHAR* pwchTableText=CreateStringCopy(WBXML_GetStrTabVal 
								(pDecStr,((pT_TEXTELEMENT)pTemp)->iTableRef));
				
				if (pwchTemp!=NULL)
				{
					CONCATSTRING(pwchTemp,pwchTableText);
				}
				DEALLOC(&pwchTableText);
			}
			else if (pTemp->iType==Type_Entity)
			{
				/* Convert entity to string and add to text. */
				pwchEntity[1]=0;
				
				/* Cannot be represented in USC-2 
				   Character (HEX 7F) is shown instead. */
				if (((pENTITY)pTemp)->iEntity>65535)
				{
					pwchEntity[0]=(WCHAR)('\x7F');
				}
				else
				{
					pwchEntity[0]=(WCHAR)(((pENTITY)pTemp)->iEntity);
				}

				/* Add entity */
				CONCATSTRING(pwchTemp,pwchEntity);
			}
			else
			{
				/* Set ppLastElement */
				*ppLastElement=pLastUsed;

				return pwchString;
			}

			pLastUsed=pTemp;
			pTemp=pTemp->pNextElement;
		}

		#ifndef HAS_SETJMP
		}
		#endif
	}

	/* All elements used, set ppLastElement to last element used. */
	*ppLastElement=pLastUsed;

	return pwchString;
}


WCHAR* XML_GetString (pELEMENTTYPE pStartElement, pWBXMLDECODESTR pDecStr)
{
	pELEMENTTYPE pDummy=NULL;
	WCHAR* pwchString=NULL;

	/* Build string */
	pwchString=XML_BuildString (pStartElement,pDecStr,XML_GetStringLength 
							   (pStartElement,pDecStr), &pDummy);

	return pwchString;
}


/*========================================================================
	XML_ConvertToString
==========================================================================*/
WCHAR* XML_ConvertToString (pELEMENTTYPE* ppStartElement, pWBXMLDECODESTR pDecStr)
{
	pELEMENTTYPE pLastUsed=NULL;
	pELEMENTTYPE pDeleteList=NULL;
	WCHAR* pwchString=NULL;

	if (ppStartElement!=NULL)
	{
		/* Build string */
		pwchString=XML_BuildString (*ppStartElement,pDecStr,XML_GetStringLength 
								   (*ppStartElement,pDecStr), &pLastUsed);
	}
	
	if (pLastUsed!=NULL)
	{
		pDeleteList=*ppStartElement;

		/* Move past all used text elements */
		*ppStartElement=pLastUsed->pNextElement;

		pLastUsed->pNextElement=NULL;

		/* Delete list */
		XML_DeleteElementList (&pDeleteList, pDecStr);
	}

	return pwchString;
}


/*========================================================================
	XML_AddHeadString
==========================================================================*/
void XML_AddHeadString (pELEMENTTYPE* ppStartElement, const CHAR* pchString,
						pWBXMLDECODESTR pDecStr)
{
	WCHAR* pwchWideString=NULL;
	pELEMENTTYPE pNewElement=NULL;

	pDecStr=pDecStr;

	if (pchString!=NULL)
	{
		/* Copy string */
		pwchWideString=wip_byte2wchar (pchString);

		#ifndef HAS_SETJMP
		if (pwchWideString==NULL)
		{
			/* Out of memory */
			pDecStr->iDecodeResult|=WBXML_Error_OutOfMemory;
			return;
		}
		#endif

		/* Create inline string element */
		pNewElement=XML_CreateInlineText (&pwchWideString);

		/* Add first in element list */
		XML_AddElementToHead (ppStartElement,pNewElement);
	}
}





/*========================================================================

	Functions for handling Global Elements

=========================================================================*/

/*========================================================================
	XML_CreateEmptyElement
==========================================================================*/
pELEMENTTYPE XML_CreateEmptyElement (UINT16 iType)
{
	/* Create super element */
	pELEMENTTYPE pElement=NEWSTRUCT(ELEMENTTYPE);

	#ifndef HAS_SETJMP
	if (pElement!=NULL)
	{
	#endif

		/* Set type */
		pElement->iType=iType;
		pElement->pNextElement=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	/* Return */
	return pElement;
}


/*========================================================================
	XML_DeleteEmptyElement
==========================================================================*/
void XML_DeleteEmptyElement (pELEMENTTYPE* ppElement)
{
	if (ppElement!=NULL)
	{
		DEALLOC(ppElement);
	}
}


/*========================================================================
	XML_AddContent
==========================================================================*/
void XML_AddContent (pELEMENTTYPE pElement, pELEMENTTYPE* ppContent)
{
	pELEMENTTYPE pContentList=NULL;
	pELEMENTTYPE pTemp=NULL;

	/* Cast element to Type_ContentElement. This type can be used
	   since all elements with content has the content attribute
	   specified as their third field.
	*/
	pContentList=((pCONTENTELEMENT)(pElement))->pContent;

	/* Add content */
	if (pContentList==NULL)
	{
		/* First element */
		((pCONTENTELEMENT)(pElement))->pContent=*ppContent;
	}
	else
	{
		pTemp=pContentList;

		/* Find last element */
		while ((pTemp->pNextElement)!=NULL)
		{
			pTemp=(pTemp->pNextElement);
		}

		/* Add element last to list */
		pTemp->pNextElement=*ppContent;
	}

	*ppContent=NULL;
}


/*========================================================================
	XML_GetContent
==========================================================================*/
pELEMENTTYPE XML_GetContent (pELEMENTTYPE pElement)
{
	return ((pCONTENTELEMENT) (pElement))->pContent;
}


/*========================================================================
	XML_CreateInlineText
==========================================================================*/
pELEMENTTYPE XML_CreateInlineText (WCHAR** ppwchString)
{
	/* Check if data defined */
	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Create element */
		pI_TEXTELEMENT pElement=NEWSTRUCT(I_TEXTELEMENT);

		#ifndef HAS_SETJMP
		if (pElement!=NULL)
		{
		#endif

			/* Store attributes */
			pElement->iType=Type_Inline_Text;
			pElement->pNextElement=NULL;
			pElement->pwchTextData=*ppwchString;
			*ppwchString=NULL;

		#ifndef HAS_SETJMP
		}
		else
		{
			/* Delete string */
			DEALLOC (ppwchString);
		}
		#endif

		/* Cast to super type and return */
		return (pELEMENTTYPE) pElement;
	}
	return NULL;
}


/*========================================================================
	XML_CreateTableText
==========================================================================*/
pELEMENTTYPE XML_CreateTableText (UINT32 iTableRef)
{
	/* Create element */
	pT_TEXTELEMENT pElement=NEWSTRUCT(T_TEXTELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL)
	{
	#endif

		/* Store attributes */
		pElement->iType=Type_Table_Text;
		pElement->pNextElement=NULL;
		pElement->iTableRef=iTableRef;

	#ifndef HAS_SETJMP
	}
	#endif

	/* Cast to super type and return */
	return (pELEMENTTYPE) pElement;
}


/*========================================================================
	XML_DeleteText
==========================================================================*/
void XML_DeleteText (pELEMENTTYPE* ppText)
{
	if ((ppText!=NULL)&&(*ppText!=NULL))
	{
		if ((*ppText)->iType==Type_Inline_Text)
		{
			/* Remove the string */
			DEALLOC(& ((pI_TEXTELEMENT)(*ppText))->pwchTextData);

			DEALLOC(ppText);
		}
		else if ((*ppText)->iType==Type_Table_Text)
		{
			DEALLOC(ppText);
		}
	}
}


/*========================================================================
	XML_CreateExt
==========================================================================*/
pELEMENTTYPE XML_CreateExt (WCHAR** ppwchString, BYTE bType)
{
	UINT16 iType=0;
	pEXT_ELEMENT pElement=NULL;

	/* Check if data defined */
	if (ppwchString!=NULL)
	{
		/* Get type */
		switch (bType)
		{
		case GLOBAL_EXT_I_0:
		case GLOBAL_EXT_T_0:
		case GLOBAL_EXT_0:
			iType=Type_Ext_0;
			break;
		case GLOBAL_EXT_I_1:
		case GLOBAL_EXT_T_1:
		case GLOBAL_EXT_1:
			iType=Type_Ext_1;
			break;
		case GLOBAL_EXT_I_2:
		case GLOBAL_EXT_T_2:
		case GLOBAL_EXT_2:
			iType=Type_Ext_2;
			break;
		default:
			return NULL;
		}

		/* Create element */
		pElement=NEWSTRUCT(EXT_ELEMENT);

		#ifndef HAS_SETJMP
		if (pElement!=NULL)
		{
		#endif
			
			/* Store attributes */
			pElement->iType=iType;
			pElement->pNextElement=NULL;
			pElement->pwchName=*ppwchString;
			*ppwchString=NULL;

		#ifndef HAS_SETJMP
		}
		else
		{
			/* Delete string */
			DEALLOC (ppwchString);
		}
		#endif

		/* Cast to super type and return */
		return (pELEMENTTYPE) pElement;
	}
	return NULL;
}


/*========================================================================
	XML_DeleteExt
==========================================================================*/
void XML_DeleteExt (pELEMENTTYPE* ppExt)
{
	if ((ppExt!=NULL)&&(*ppExt!=NULL))
	{
		if( ((*ppExt)->iType==Type_Ext_0) ||
			((*ppExt)->iType==Type_Ext_1) ||
			((*ppExt)->iType==Type_Ext_2) )
		{
			/* Remove the string */
			DEALLOC(& ((pEXT_ELEMENT)(*ppExt))->pwchName);

			DEALLOC(ppExt);
		}
	}
}


/*========================================================================
	XML_CreateEntity
==========================================================================*/
pELEMENTTYPE XML_CreateEntity (UINT32 iEntity)
{
	/* Create element */
	pENTITY pElement=NEWSTRUCT(ENTITY);

	#ifndef HAS_SETJMP
	if (pElement!=NULL)
	{
	#endif
		
		/* Store attributes */
		pElement->iType=Type_Entity;
		pElement->pNextElement=NULL;
		pElement->iEntity=iEntity;

	#ifndef HAS_SETJMP
	}
	#endif

	/* Cast to super type and return */
	return (pELEMENTTYPE) pElement;
}


/*========================================================================
	XML_DeleteEntity
==========================================================================*/
void XML_DeleteEntity (pELEMENTTYPE* ppEntity)
{
	if ((ppEntity!=NULL)&&(*ppEntity!=NULL))
	{
		if ((*ppEntity)->iType==Type_Entity)
		{
			DEALLOC(ppEntity);
		}
	}
}


/*========================================================================
	XML_CreateOpaque
==========================================================================*/
pELEMENTTYPE XML_CreateOpaque (BYTE** ppbData, UINT32 iLength)
{
	/* Check if data defined */
	if (ppbData!=NULL)
	{
		/* Create element */
		pXMLOPAQUE pElement=NEWSTRUCT(XMLOPAQUE);

		#ifndef HAS_SETJMP
		if (pElement!=NULL)
		{
		#endif
			
			/* Store attributes */
			pElement->iType=Type_Opaque;
			pElement->pNextElement=NULL;
			pElement->iLength=iLength;
			pElement->pbData=*ppbData;
			*ppbData=NULL;

		#ifndef HAS_SETJMP
		}
		#endif

		/* Cast to super type and return */
		return (pELEMENTTYPE) pElement;
	}
	return NULL;
}


/*========================================================================
	XML_DeleteOpaque
==========================================================================*/
void XML_DeleteOpaque (pELEMENTTYPE* ppOpaque)
{
	if ((ppOpaque!=NULL)&&(*ppOpaque!=NULL))
	{
		if ((*ppOpaque)->iType==Type_Opaque)
		{
			/* Delete data */
			DEALLOC(& ((pXMLOPAQUE)(*ppOpaque))->pbData);

			DEALLOC(ppOpaque);
		}
	}
}


/*========================================================================
	XML_GetNextElement
==========================================================================*/
pELEMENTTYPE XML_GetNextElement (pELEMENTTYPE pElement)
{
	if (pElement!=NULL)
	{
		return pElement->pNextElement;
	}
	return NULL;
}


/*========================================================================
	XML_DeleteElement
==========================================================================*/
void XML_DeleteElement (pELEMENTTYPE* ppElement, pWBXMLDECODESTR pDecStr)
{
	if (*ppElement!=NULL)
	{
		/* Check type */
		switch ((*ppElement)->iType)
		{

		/* Global elements */
		case Type_Inline_Text:
		case Type_Table_Text:
			XML_DeleteText (ppElement);
			break;
		case Type_Ext_0:
		case Type_Ext_1:
		case Type_Ext_2:
			XML_DeleteExt (ppElement);
			break;
		case Type_Entity:
			XML_DeleteEntity (ppElement);
			break;
		case Type_Opaque:
			XML_DeleteOpaque (ppElement);
			break;
		case Type_ContentElement:
			XML_DeleteElementList(& ((pCONTENTELEMENT) (*ppElement))->pContent, pDecStr);
			DEALLOC(ppElement);
			break;

		/* Application specific elements */
		default:
			pDecStr->DTD_DeleteElement(pDecStr,ppElement);
			break;
		}
	}
}


/*========================================================================
	XML_DeleteElementList
==========================================================================*/
void XML_DeleteElementList (pELEMENTTYPE* ppElement, pWBXMLDECODESTR pDecStr)
{
	if (ppElement!=NULL)
	{
		/* Get first element */
		pELEMENTTYPE pTemp=NULL;

		/* Delete all elements on this level */
		while (*ppElement!=NULL)
		{
			pTemp=*ppElement;

			*ppElement=(*ppElement)->pNextElement;

			XML_DeleteElement(&pTemp,pDecStr);
		}
	}
}


/*========================================================================
	XML_AddElementToHead
==========================================================================*/
void XML_AddElementToHead (pELEMENTTYPE* ppElementList, pELEMENTTYPE pElement)
{
	if ((pElement!=NULL)&&(ppElementList!=NULL))
	{
		/* Add first in existing list */
		pElement->pNextElement=*ppElementList;

		*ppElementList=pElement;	
	}
}


/*========================================================================
	XML_AddElementToEnd
==========================================================================*/
void XML_AddElementToEnd (pELEMENTTYPE* ppElementList, pELEMENTTYPE pElement)
{
	if ((pElement!=NULL)&&(ppElementList!=NULL))
	{
		pElement->pNextElement=NULL;

		if (*ppElementList==NULL)
		{
			/* List is empty */
			*ppElementList=pElement;
		}
		else
		{
			pELEMENTTYPE pTemp=*ppElementList;

			/* Find last element */
			while (pTemp->pNextElement!=NULL)
			{
				pTemp=pTemp->pNextElement;
			}

			/* Append element to list */
			pTemp->pNextElement=pElement;
		}
	}
}
