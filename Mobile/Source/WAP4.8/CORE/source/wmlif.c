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
/*========================================================================
	FILE: wmlif.c

	Source file for the interface between WAE and the WML-structure.
	Includes function to initiate the parse of a WML-deck and using
	the structure created during the parse.

	Rev history:
		980728	JPR		Created
		990208	JPR		Corrections
		990315	JPR		Major changes. Handles the WML-specific parts 
						of the startup and closing down of the parser.
		990316	JPR		Variable handling moved here from WMLElm.h
		990406	JPR		Corrections
		990409	JPR		Access handling modified
		990412	JPR		Url.h used instead of Url_Lib.h
		990413	JPR		WML_HandleMMIAction corrected
		990420	JPR		WML_EndParser updated. Card/Deck-shadowing
						on do elements corrected.
		990421	JPR		WML_UpdateAllSelect corrected. Now handles p
						and fieldset.
		990427	JPR		Corrections in History functions
		990517	JPR		WML/WBXML Version handling updated and 
						extended error handling.
		990528	JPR		History functions updated. Paragraph updates
		990106	JPR		Navigation corrections (setvar element)
		990610	JPR		Trim text functions updated.
		990709	JPR		Store Timer function corrected.
		990823	JPR		Correction in WML_EndParserHelp
		990826	JPR		Function WML_NewContext added.
		990827	JPR		Correction in WML_ParseNextElement
		991020	JPR		WTA additions included
		991109	JPR		TR 173. Correction in WML_ParseNextElement
		991124	JPR		History functions moved to History.c

		991208	JPR		Changes to support new generic WBXML-decoder,
						PARALLELL_PARSE_ENABLED removed.
		000110	IPN		Removed pVisitedHosts from WML_CreateUserAgent.
		000111	JPR		DeleteHostList function call removed.
		000113	JPR		Correction in WML_GetNextElement
		000117	JPR		Function WML_CreateUserAgent modified
		000203	JPR		WML_GetNextElement corrected
		000209	JPR		Correction in functions WML_GetAssembleLength and
						WML_UpdateSelect
		000808	JPR		Updated to support WTA-WML (WTA 1.2.1). Correction in
						Code page handling.
		000810	JPR		Default wrap mode added to card. Switch page corrected
		000925	JPR		Correction to support post with correct charset
		010119	NKE		Updated WML_StartParser and WML_ParseNextElement for
						the new decoder in wbxmldec.c.
		010125	NKE		Updated WML_StartParser.
		010516  IPN     Changes in WML_GetString, new escape function.
		                Changes in WML_GetStringLength, new escape function.

==========================================================================*/

#include "confvars.h"
#include "cmmnrsrc.h"
#include "wmlparse.h"
#include "wmlif.h"
#include "hdrutil.h"
#include "url.h"
#include "waectype.h"
#include "history.h"
#include "wbxmldec.h"
#include "wbxmlelm.h"
#include "xml_conv.h"
#include "wmlevent.h"

/*========================================================================
	WML_CreateUserAgent
==========================================================================*/
pUA WML_CreateUserAgent (void) 
{
	pUA pUser;

	pUser=NEWSTRUCT(UA);
	if (pUser!=NULL)
	{
		pUser->iUserAgentMode=User_Agent_WML;
		pUser->pDeck=NULL;
		pUser->pDecStr=NULL;
		pUser->pbURLBase=NULL;

		pUser->pEventVariables=NULL;

		/* This attribute is shared with the template.
		   DO NOT DEALLOCATE when the user agent is
		   deleted. Handled when the TEMPLATE element
		   is deleted. */
		pUser->pTemplateEvents=NULL;

		pUser->iViewID=0;
		pUser->fLockMMI=FALSE;
		pUser->fUserInputMode=FALSE;
		pUser->pHistoryList=NULL;
		pUser->iElmInHistory=0;
		pUser->iMaxElmInHistory=10;
		pUser->pCurrentCard=NULL;
		pUser->iCurrentWrapMode=P_Unknown;
		pUser->pVARTABLE=NULL;
		pUser->iTextEncodFormat=IANA_CHARSET_INVALID;
		pUser->pWMLDeckData=NULL;

        #ifdef TASKFAILURE_REPARSE_RAWDATA
			pUser->iOldPIVersion=0;
			pUser->iOldContentTypeByte=0;
			pUser->iOldCharset=0;
			pUser->pOldRawWMLDeckData = NULL;
			pUser->iOldRawWMLDeckDataLen =0;

			pUser->iTempPIVersion=0;
			pUser->iTempContentTypeByte=0;
			pUser->iTempCharset=0;
			pUser->pTempRawWMLDeckData = NULL;
			pUser->iTempRawWMLDeckDataLen =0;
        #endif
	}
	return pUser;
}


/*========================================================================
	WML_DeleteUserAgent
==========================================================================*/
void WML_DeleteUserAgent (pUA *ppUser)
{
	if (*ppUser!=NULL)
	{
		/* Delete variable table */
		WML_EmptyVarTable (*ppUser);

		/* Delete temporary variable list */
		WML_DeleteEventVariables (*ppUser);

		/* Delete deck */
		WML_EmptyDECK(*ppUser);

		/* Delete history */ 
		WML_EmptyHistoryList(*ppUser);
		
		/* Dealloc pbURLBase */
		DEALLOC(&((*ppUser)->pbURLBase));

		/* Delete Decode Struct if not already deleted. */
		WBXML_DeleteWBXMLDecStr(&((*ppUser)->pDecStr));
		
        #ifdef TASKFAILURE_REPARSE_RAWDATA
			if( (*ppUser)->pOldRawWMLDeckData == (*ppUser)->pTempRawWMLDeckData ){
				DEALLOC( & ((*ppUser)->pOldRawWMLDeckData) );
				(*ppUser)->pTempRawWMLDeckData = NULL;
			}
			else{
				DEALLOC( & ((*ppUser)->pOldRawWMLDeckData) );
				DEALLOC( & ((*ppUser)->pTempRawWMLDeckData) );
			}
        #endif

		DEALLOC (ppUser);
	}
}

/*========================================================================
	WML_StartParser
==========================================================================*/
BOOL WML_StartParser (pUA pUser, BYTE *pbStream, UINT32 iNbrOfBytes,
					  BYTE bContentType, UINT8 iContentLevel)
{
	pWBXMLDECODESTR pDecStr=NULL;

	if (pUser!=NULL)
	{
		pUser->iCurrentWrapMode = P_Wrap;

		/* Check if decode struct exists */
		if (pUser->pDecStr!=NULL)
		{
			/* Remove old deck */
			WML_EmptyDECK(pUser);

			/* Clear string table */
			WBXML_DelStrTab(pUser->pDecStr);

			/* Use old decode struct */
			pDecStr=pUser->pDecStr;
		}
		else
		{
			/* Create tWBXMLDECODESTR and init the attributes */
			pDecStr=WBXML_CreateWBXMLDecStr();

			if (pDecStr!=NULL)
			{
				/* Set the function pointers */
				pDecStr->DTD_CreateElement           = WML_CreateElement;
				pDecStr->DTD_DeleteElement           = WML_DeleteElement;
				pDecStr->DTD_ValidContent            = WML_ValidContent;
				pDecStr->DTD_GetContent              = WML_GetContent;
				pDecStr->DTD_AppSpecParseMod         = WML_ModifyParse;
				pDecStr->DTD_StoreAttributeValue     = WML_StoreAttributeValue;
				pDecStr->DTD_GetAttributeValue       = WML_GetAttributeValue;
				pDecStr->DTD_LiteralAttributeToToken = WML_LiteralAttributeToToken;
				pDecStr->DTD_LiteralTagToToken       = WML_LiteralTagToToken;
				pDecStr->DTD_CheckPublicID           = WML_CheckPublicID;
			}
		}

		if (pDecStr!=NULL)
		{
			/* Init values */
			pDecStr->pbCurByte=pbStream;
			pDecStr->pbEnd=pbStream+iNbrOfBytes;
			pDecStr->iCharset=pUser->iTextEncodFormat;
			pDecStr->iContentLevel=iContentLevel;
			pDecStr->bContentType=bContentType;
			pDecStr->iDecodeResult=0;
			pDecStr->pAppSpec=NULL;
			pDecStr->iTagCodePage=0;
			pDecStr->iAttrCodePage=0;
			pDecStr->iVersion=0;
			pDecStr->pStrTab=NULL;
			
			WBXML_InitDecode(pDecStr, FALSE);

			/* Connect the structs */
			pDecStr->pAppSpec=pUser;
			pUser->pDecStr=pDecStr;

			if (WBXML_DecodeProlog(pDecStr))
			{
				/* To support post with correct charset */
				pUser->iTextEncodFormat = pDecStr->iCharset;
				return TRUE;
			}
		}
	}

	return FALSE;
}



/*========================================================================
	WML_EndParser
==========================================================================*/

void WML_FixTemplateDo (pUA pUser)
{
	pELEMENTTYPE pTemplate=NULL;
	pELEMENTTYPE pCard=NULL;
	pELEMENTTYPE pBack=NULL;
	pELEMENTTYPE pCardDO=NULL;
	pELEMENTTYPE pContent=NULL;
	WCHAR *pchTName=NULL;
	WCHAR *pchCName=NULL;
	BOOL fCont=TRUE;
	BOOL fOverridden=FALSE;

	/* Get TEMPLATE */
	pContent=WML_GetContent(pUser->pDeck);
	while ((pContent!=NULL)&&(fCont))
	{
		if (pContent->iType==Type_TEMPLATE)
		{
			pTemplate=pContent;
			fCont=FALSE;
		}
		else
		{
			pContent=pContent->pNextElement;
		}
	}

	if (pTemplate==NULL)
	{
		return;
	}

	/* TEMPLATE found */

	/* Check if the TEMPLATE contains any DO element */
	pContent=WML_GetContent(pTemplate);
	fCont=FALSE;
	while ((pContent!=NULL)&&(!fCont))
	{
		if (pContent->iType==Type_DO)
		{
			fCont=TRUE;
		}
		else
		{
			pContent=pContent->pNextElement;
		}
	}

	/* fCont is TRUE if the TEMPLATE contains 
	   at least one DO element */
	if (fCont)
	{
		pCard=WML_GetContent(pUser->pDeck);

		/* Add TEMPLATE DO for every CARD */
		while (pCard!=NULL)
		{
			if (pCard->iType==Type_CARD)
			{
				/* CARD found. Examine content of TEMPLATE. Add
				   the DO elements that exist on TEMPLATE 
				   level and are not overridden on the CARD. */

				pContent=WML_GetContent(pTemplate);

				/* Find all DO on TEMPLATE */
				while (pContent!=NULL)
				{
					if (pContent->iType==Type_DO)
					{
						/* TEMPLATE DO found - Get Name */
						pchTName=((DOELEMENT*)(pContent))->pchName;

						if (pchTName==NULL)
						{
							/* Set Name as Type */
							pchTName=((DOELEMENT*)(pContent))->pchType;
						}

						if (pchTName!=NULL)
						{
							/* Get CARD DO */
							pCardDO=WML_GetContent(pCard);
							fOverridden=FALSE;
							pBack=NULL;

							/* Compare with all CARD-level DO elements */
							while ((pCardDO!=NULL)&&(!fOverridden))
							{
								if (pCardDO->iType==Type_DO)
								{
									/* Card DO found - Get Card DO Name */
									pchCName=((DOELEMENT*)(pCardDO))->pchName;

									if (pchCName==NULL)
									{
										/* Set Name as Card DO Type */
										pchTName=((DOELEMENT*)(pCardDO))->pchType;
									}

									if (pchCName!=NULL)
									{
										/* Compare the names */
										if (COMPARESTRING(pchTName,pchCName)==0)
										{
											/* Equal - overridden by the card */
											fOverridden=TRUE;
										}
									}

									pCardDO=pCardDO->pNextElement;
								}

								/* Wap 1.1 specific addition
								   (Check p contents) */
								else if (pCardDO->iType==Type_P)
								{
									/* Pointer back to this level */
									pBack=pCardDO;

									pCardDO=WML_GetContent(pCardDO);
								}

								/* Get next possible DO */
								else
								{
									pCardDO=pCardDO->pNextElement;

									if (pCardDO==NULL)
									{
										pCardDO=pBack;
										pBack=NULL;

										if (pCardDO!=NULL)
										{
											pCardDO=pCardDO->pNextElement;
										}
									}
								}
							}

							/* If the TEMPLATE DO is not overridden - add to card. */
							if (!fOverridden)
							{
								/* Add to card */
								WML_AddTemplateDO(pCard,pContent);
							}
						}
					}

					/* Get next TEMPLATE content */
					pContent=pContent->pNextElement;
				}
			}

			/* Get next card */
			pCard=pCard->pNextElement;
		}
	}
}


BOOL WML_EndParser (pUA pUser)
{
	BOOL fDecodeOK=FALSE;

	if (pUser==NULL) 
	{
		return FALSE;
	}

	/* Check if the parse was ok. */
	if (pUser->pDecStr!=NULL)
	{
		fDecodeOK=(pUser->pDecStr->iDecodeResult<=WBXML_FatalError);
	}

	if (!fDecodeOK)
	{
		/* Parse not ok - delete deck */
		XML_DeleteElement (&(pUser->pDeck),pUser->pDecStr);

		/* Clear string table */
		WBXML_DelStrTab(pUser->pDecStr);

		return FALSE;
	}

	/* Decoding OK, Fix TEMPLATE DO */
	WML_FixTemplateDo (pUser);

	return TRUE;
}


/*========================================================================
	WML_AbortParser
==========================================================================*/
void WML_AbortParser (pUA pUser)
{
	/* Set unknown error and call WML_EndParser */
	if (pUser!=NULL)
	{
		if (pUser->pDecStr!=NULL)
		{
			/* Set unknown error */
			pUser->pDecStr->iDecodeResult|=WBXML_Error_Unknown;
		}

		WML_EndParser (pUser);
	}
}



/*========================================================================
	WML_EmptyDECK
==========================================================================*/
void WML_EmptyDECK (pUA pUser)
{
	if (pUser!=NULL)
	{
		XML_DeleteElement(&(pUser->pDeck),pUser->pDecStr);
		pUser->pDeck=NULL;

		/* Remove the eventtable. (Deleted in TEMPLATE
		   element, set to NULL) */
		pUser->pTemplateEvents=NULL;
	}
}



/*========================================================================

	VARIABLE TABLE FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_SetVariable 
==========================================================================*/
BOOL WML_SetVariable (pUA pUser, WCHAR *pchName, WCHAR *pchValue, BOOL fWTA)
{
	pVARTABELM pVARTABTemp=pUser->pVARTABLE;
	pVARTABELM pVARTABTempPar=NULL;
	WCHAR *pchTempValue;

	if (pchName!=NULL) 
	{
		/* Check if valid name */
		if (WML_CheckVariableSyntax (pchName,fWTA))
		{
			/* Empty string with nullterm is equal to NULL */
			if (pchValue!=NULL)
			{
				if (*pchValue==0)
				{
					pchValue=NULL;
				}
			}

			/* Check if the variable already exists. */
			while (pVARTABTemp!=NULL) 
			{
				if (COMPARESTRING(pVARTABTemp->pchName,pchName)==0) 
				{
					/* Var name exists - delete the old value 
					   and add the new value */

					/* Check new value */
					if (pchValue!=NULL)
					{
						pchTempValue=CreateStringCopy(pchValue);

						/* Check if value copied. */
						if (pchTempValue!=NULL)
						{
							/* Change value */
							DEALLOC(&(pVARTABTemp->pchValue));
							pVARTABTemp->pchValue=pchTempValue;
							return TRUE;
						}
						else
						{
							/* Could not change value */
							return FALSE;
						}
					}
					else
					{
						/* Remove variable from list */
						DEALLOC(&(pVARTABTemp->pchName));
						DEALLOC(&(pVARTABTemp->pchValue));

						if (pVARTABTempPar==NULL)
						{
							pUser->pVARTABLE=pVARTABTemp->pNextVar;
						}
						else
						{
							pVARTABTempPar->pNextVar=pVARTABTemp->pNextVar;
						}

						DEALLOC(&pVARTABTemp);
						return TRUE;
					}
				}
				if (pVARTABTempPar==NULL)
				{
					pVARTABTempPar=pUser->pVARTABLE;
				}
				else
				{
					pVARTABTempPar=pVARTABTemp;
				}
				pVARTABTemp=pVARTABTemp->pNextVar;
			}

			/* Create new variable and add to variable table. */
			pVARTABTemp=NEWSTRUCT(VARTABELM);
			if (pVARTABTemp!=NULL) 
			{
				pVARTABTemp->pchName=CreateStringCopy(pchName);
				pVARTABTemp->pchValue=CreateStringCopy(pchValue);
				pVARTABTemp->pNextVar=NULL;

				/* Check if really created. */
				if (pVARTABTemp->pchName!=NULL)
				{
					/* Check value. */
					if (pchValue!=NULL) 
					{
						if (pVARTABTemp->pchValue!=NULL) 
						{
							/* Add to list. */
							if (pUser->pVARTABLE==NULL)
							{
								pUser->pVARTABLE=pVARTABTemp;
							}
							else
							{
								pVARTABTempPar->pNextVar=pVARTABTemp;
							}
							return TRUE;
						}
					}
					
				}
				/* Error - Delete element. */
				DEALLOC(&pVARTABTemp->pchName);
				DEALLOC(&pVARTABTemp->pchValue);
				DEALLOC(&pVARTABTemp);
			}
		}
		else
		{
			/* Syntax error */
			return FALSE;
		}
	}

	/* ERROR - return FALSE */
	return FALSE;
}


/*========================================================================
	WML_GetVariable 
==========================================================================*/
WCHAR *WML_GetVariable (pUA pUser, WCHAR *pchName)
{
	pVARTABELM pVARTABTemp=pUser->pVARTABLE;

	if (pchName!=NULL) 
	{
		while (pVARTABTemp!=NULL) 
		{
			if (COMPARESTRING(pchName,pVARTABTemp->pchName)==0)
			{
				return CreateStringCopy(pVARTABTemp->pchValue);
			}
			pVARTABTemp=pVARTABTemp->pNextVar;
		}
	}
	return NULL;
}


/*========================================================================
	WML_EmptyVarTable
==========================================================================*/
void WML_EmptyVarTable (pUA pUser)
{
	pVARTABELM pVARTABLE;
	pVARTABELM pVARTABLETemp;

	pVARTABLETemp=pUser->pVARTABLE;
	while (pVARTABLETemp!=NULL) 
	{
		pVARTABLE=pVARTABLETemp;
		DEALLOC(&(pVARTABLE->pchName));
		DEALLOC(&(pVARTABLE->pchValue));
		pVARTABLETemp=pVARTABLETemp->pNextVar;
		DEALLOC(&pVARTABLE);
	}
	pUser->pVARTABLE=NULL;
}


/*========================================================================
	WML_DeleteEventVariables
==========================================================================*/
void WML_DeleteEventVariables (pUA pUser)
{
	pVARTABELM pVarList=NULL;
	pVARTABELM pTempVar=NULL;

	/* ASSERT:	pUser!=NULL
	*/

	pVarList=pUser->pEventVariables;

	while (pVarList!=NULL)
	{
		pTempVar=pVarList;
		pVarList=pVarList->pNextVar;

		/* Remove element */
		DEALLOC(&pTempVar->pchName);
		DEALLOC(&pTempVar->pchValue);
		DEALLOC(&pTempVar);
	}

	pUser->pEventVariables=NULL;
}


/*========================================================================
	WML_AddEventVariablesToContext
==========================================================================*/
void WML_AddEventVariablesToContext (pUA pUser)
{
	pVARTABELM pTempVar=NULL;

	/* ASSERT:	pUser!=NULL
	*/

	pTempVar=pUser->pEventVariables;

	while (pTempVar!=NULL)
	{
		/* Set variable */
		WML_SetVariable (pUser, pTempVar->pchName, pTempVar->pchValue, 
									(pUser->iUserAgentMode==User_Agent_WTA));

		pTempVar=pTempVar->pNextVar;
	}

	/* Delete list */
	WML_DeleteEventVariables (pUser);
}


/*========================================================================
	WML_CheckVariableSyntax
==========================================================================*/
/* ( "_" | alpha ) *[ "_" | alpha | digit )

  if WTA is enabled:
   ( "_" | alpha | digit ) *[ "_" | alpha | digit )	
*/

BOOL WML_CheckVariableSyntax (WCHAR *pchVariable, BOOL fWTA)
{
	fWTA=fWTA;

	if (pchVariable!=NULL)
	{
		if (*pchVariable!=0)
		{
			if ((*pchVariable==WC('_'))||
				((*pchVariable < 0x100) && wae_isalpha(*pchVariable))

			/* WTA specific */
			#ifdef CONFIG_WTA
				||
				/* digit */
				((fWTA)&&(*pchVariable < 0x100) && wae_isdigit(*pchVariable))
			#endif

				)
			{
				pchVariable++;

				while (*pchVariable!=0)
				{
					if (((*pchVariable < 0x100) && wae_isalpha(*pchVariable))||
						((*pchVariable < 0x100) && wae_isdigit(*pchVariable))||
						(*pchVariable==WC('_')))
					{
						pchVariable++;
					}
					else
					{
						return FALSE;
					}
				}

				/* Has at least one char, and loop OK */
				return TRUE;
			}
		}
	}
	return FALSE;
}




/*========================================================================

	SELECT ELEMENT FUNCTIONS

=========================================================================*/

/* Help function for CalculateResultLength */
UINT16 WML_GetNumberLength (UINT16 iNumber)
{
	UINT16 iResult=1;

	while (iNumber>=10)
	{
		iNumber/=10;
		iResult++;
	}
	return iResult;
}

/* Calculates the length of the string to be constructed
   when making a ';'-separated list from a NUMBERSTRUCT. */
UINT16 WML_CalculateResultLength (pNUMBERSTRUCT pOptions)
{
	UINT16 iResult=0;

	while (pOptions!=NULL)
	{
		iResult+=WML_GetNumberLength(pOptions->iIndex);
		
		/* Add space for ';' */
		iResult++;

		pOptions=pOptions->pNext;
	}

	if (iResult>0)
	{
		/* Remove one ';' */
		iResult--;
	}

	return iResult;
}

/* Help function for ConvertNumber2String */
void WML_SetOneNumber (UINT16 iNumber, WCHAR **ppchString)
{
	UINT16 iDiv=0;
	UINT16 iMod=0;

	if (iNumber>0)
	{
		iDiv=(UINT16)(iNumber/10);
		iMod=(UINT16)(iNumber%10);

		WML_SetOneNumber(iDiv,ppchString);

		**ppchString=(UINT16)(WC('1')+iMod-1);

		(*ppchString)++;
	}
}

/* Assembles the numbers in the NUMBERSTRUCT to a ';'-
   separated string. (E.g 1 5 13  -> "1;5;13")			*/
void WML_ConvertNumber2String (pNUMBERSTRUCT pOptions, WCHAR *pchString)
{
	UINT16 iTemp=0;
	WCHAR *pchTemp=NULL;

	pchTemp=pchString;

	while (pOptions!=NULL)
	{
		iTemp=pOptions->iIndex;
		WML_SetOneNumber(iTemp,&pchTemp);

		if (pOptions->pNext!=NULL)
		{
			/* Set ';' character */
			*pchTemp=WC(';');
			pchTemp++;
		}
		pOptions=pOptions->pNext;
	}

	/* Set termination character */
	*pchTemp=0;
}

/* Help function for AssembleKeyString. 
   Calculates the length for the result string
   (excluding the termination character). */
UINT32 WML_GetAssembleLength (pUA pUser, pELEMENTTYPE pElement, BOOL *fError)
{
	UINT32 iLength=0;
	UINT32 iThis=0;
	pELEMENTTYPE pContent=NULL;
	pOPTIONELEMENT pOption=NULL;

	/* Step through all elements on this level */
	while (pElement!=NULL)
	{
		/* Check OPTGROUP or OPTION */
		if (pElement->iType==Type_OPTGROUP)
		{
			pContent=WML_GetContent(pElement);
			iLength+=WML_GetAssembleLength(pUser, pContent, fError);
		}
		else if (pElement->iType==Type_OPTION)
		{
			pOption=(OPTIONELEMENT*)(pElement);
			if (pOption->fSelected)
			{
				iThis=WML_GetStringLength(pUser,pOption->pValue);

				/* Don't add empty strings */
				if (iThis>0)
				{
					/* Add length for data and ';'-character,*/
					iLength+=iThis+1;
				}
			}
		}
		else
		{
			/* Error! */
			*fError=TRUE;
		}
		pElement=pElement->pNextElement;
	}
	return iLength;
}

/* Help function for AssembleKeyString */
void WML_AddSelectedOptions (pUA pUser, pELEMENTTYPE pElement, WCHAR **ppchResult)
{
	WCHAR *pchTemp=NULL;
	pELEMENTTYPE pContent=NULL;
	pOPTIONELEMENT pOption=NULL;

	/* Step through all elements on this level */
	while (pElement!=NULL)
	{
		/* Check OPTGROUP or OPTION */
		if (pElement->iType==Type_OPTGROUP)
		{
			pContent=WML_GetContent(pElement);
			WML_AddSelectedOptions(pUser, pContent, ppchResult);
		}
		else if (pElement->iType==Type_OPTION)
		{
			pOption=(OPTIONELEMENT*)(pElement);
			if (pOption->fSelected)
			{
				/* Add option VALUE */
				pchTemp=WML_GetString(pUser,pOption->pValue);

				/* Don't add empty values */
				if (pchTemp!=NULL)
				{
					if (*pchTemp!=0)
					{
						/* Copy string */
						COPYSTRING(*ppchResult,pchTemp);

						/* Update ppchResult pointer */
						*ppchResult+=STRINGLENGTH(pchTemp);

						/* Add ';'-character */
						*(*ppchResult)++=WC(';');
					}

					/* Deallocate temp pointer */
					DEALLOC (&pchTemp);
				}
			}
		}
		pElement=pElement->pNextElement;
	}
}

/* Assembles the values in the selected OPTION elements 
  (those with the fSelected flag set) and returns a';'-
  separated string. (E.g. 1 3 4 -> "[Value of option 1];
  [Value of option 3];[Value of option 4]".) */
WCHAR *WML_AssembleKeyString (pUA pUser, pELEMENTTYPE pElement)
{
	UINT32 iLength=0;
	BOOL fError=FALSE;
	WCHAR *pchResult=NULL;
	WCHAR *pchResultTemp=NULL;

	if (pElement!=NULL)
	{
		if ((pElement->iType==Type_OPTGROUP)||(pElement->iType==Type_OPTION))
		{
			/* Get result length. The length includes one ';' more
			   than it should, but no termination character, which
			   makes the length accurate. */
			iLength=WML_GetAssembleLength(pUser, pElement, &fError);

			if ((iLength>0)&&(!fError))
			{
				pchResult=NEWARRAY(WCHAR,iLength);

				if (pchResult!=NULL)
				{
					pchResultTemp=pchResult;

					/* Add the selected options to pchResult. */
					WML_AddSelectedOptions (pUser,pElement,&pchResultTemp);

					/* Insert termination character instead of 
					   last ';'-character. */
					*--pchResultTemp=0;
				}
			}
		}
	}
	return pchResult;
}

pELEMENTTYPE WML_GetFirstSelectedOption(pELEMENTTYPE pElement, BOOL *pfFound)
{
	pELEMENTTYPE pContent=NULL;
	pELEMENTTYPE pResult=NULL;
	pOPTIONELEMENT pOption=NULL;

	/* Step through all elements on this level */
	while ((pElement!=NULL)&&(!*pfFound))
	{
		/* Check OPTGROUP or OPTION */
		if (pElement->iType==Type_OPTGROUP)
		{
			pContent=WML_GetContent(pElement);
			pResult=WML_GetFirstSelectedOption(pContent,pfFound);
		}
		else if (pElement->iType==Type_OPTION)
		{
			pOption=(OPTIONELEMENT*)(pElement);
			if (pOption->fSelected)
			{
				*pfFound=TRUE;
				return pElement;
			}
		}
		pElement=pElement->pNextElement;
	}
	return pResult;
}


/* Sets the variables to the correct values according to 
   the pOptions-list. */
void WML_SetSelectVariables (pUA pUser, pELEMENTTYPE pSelect, pNUMBERSTRUCT pOptions)
{
	pSELECTELEMENT pSelectElm=NULL;
	pELEMENTTYPE pContent=NULL;
	pELEMENTTYPE pOption=NULL;
	pOPTIONELEMENT pOptionElm=NULL;
	WCHAR *pchResult=NULL;
	WCHAR *pchTemp=NULL;
	UINT16 iLength=0;
	BOOL fFound=FALSE;

	if ((pSelect!=NULL)&&(pOptions!=NULL))
	{
		if (pSelect->iType==Type_SELECT)
		{
			pSelectElm=(SELECTELEMENT*)(pSelect);

			/* Single choice NAME */
			if ((pSelectElm->pchName!=NULL)&&(!pSelectElm->fMultiple))
			{
				/* Set one option */
				pContent=WML_GetContent(pSelect);

				/* Get first selected option. */
				pOption=WML_GetFirstSelectedOption(pContent, &fFound);

				if (pOption!=NULL)
				{
					/* Get the value. */
					pOptionElm=(OPTIONELEMENT*)(pOption);
					pchResult=WML_GetString(pUser,pOptionElm->pValue);

					if (pchResult!=NULL)
					{
						/* Set the key variable to the value. */
						WML_SetVariable(pUser,pSelectElm->pchName,pchResult,FALSE);

						DEALLOC(&pchResult);
					}
				}
			}

			/* Multiple choice NAME */
			else if ((pSelectElm->pchName!=NULL)&&(pSelectElm->fMultiple))
			{
				/* Add all options */
				pContent=WML_GetContent(pSelect);

				if (pContent!=NULL)
				{
					/* Get value */
					pchResult=WML_AssembleKeyString (pUser,pContent);

					if (pchResult!=NULL)
					{
						/* Set variable */
						WML_SetVariable(pUser,pSelectElm->pchName,pchResult,FALSE);

						DEALLOC(&pchResult);
					}
				}
			}

			/* Single choice INAME */
			if ((pSelectElm->pchIName!=NULL)&&(!pSelectElm->fMultiple))
			{
				{
					/* Pick first element in list */
					iLength=WML_GetNumberLength(pOptions->iIndex);

					pchResult=NEWARRAY(WCHAR,iLength+1);

					if (pchResult!=NULL)
					{
						pchTemp=pchResult;

						/* Set variable */
						if (pOptions!=NULL)
						{
							WML_SetOneNumber(pOptions->iIndex,&pchTemp);

							/* Set termination character */
							*pchTemp=0;
						}

						WML_SetVariable(pUser,pSelectElm->pchIName,pchResult,FALSE);

						DEALLOC(&pchResult);
					}
				}
			}

			/* Multiple choice INAME */
			else if ((pSelectElm->pchIName!=NULL)&&(pSelectElm->fMultiple))
			{
				{
					/* Pick ALL elements in list */
					iLength=WML_CalculateResultLength (pOptions);
	
					pchResult=NEWARRAY(WCHAR,iLength+1);

					if (pchResult!=NULL)
					{
						/* Set variable */
						WML_ConvertNumber2String(pOptions,pchResult);

						WML_SetVariable(pUser,pSelectElm->pchIName,pchResult,FALSE);

						DEALLOC(&pchResult);
					}
				}
			}
		}
	}
	else if (pSelect!=NULL)
	{
		/* pOptions NULL -> set name to empty string
		   and iname to 0 if the select element is 
		   multiple select. */

		if (pSelect->iType==Type_SELECT)
		{
			pSelectElm=(SELECTELEMENT*)(pSelect);

			if (pSelectElm->fMultiple)
			{
				/* Set name to empty string (remove variable) */
				WML_SetVariable(pUser,pSelectElm->pchName,NULL,FALSE);

				/* Set iname to 0 (string) */
				pchResult=NEWARRAY(WCHAR,2);
				if (pchResult!=NULL)
				{
					pchResult[0]=(WCHAR)('0');
					pchResult[1]=0;

					WML_SetVariable(pUser,pSelectElm->pchIName,pchResult,FALSE);
					DEALLOC(&pchResult);
				}
				WML_SetVariable(pUser,pSelectElm->pchIName,NULL,FALSE);
			}
		}
	}
}

/* Add the element after the specified element */
void WML_AddNumberToList(pNUMBERSTRUCT pElement, pNUMBERSTRUCT *ppAfter)
{
	if (pElement!=NULL)
	{
		if (*ppAfter==NULL)
		{
			*ppAfter=pElement;
		}
		else
		{
			(*ppAfter)->pNext=pElement;
		}
	}
}

/*  All selected options are found and a list NUMBERSTRUCT
	with the indices is returned. Indata should be
	a pointer to an option or optgroup. */
pNUMBERSTRUCT WML_GetSelectedOptions (pELEMENTTYPE pElement, pNUMBERSTRUCT *ppAfter, 
								  BOOL *pfError, UINT16 *piIndex)
{
	pELEMENTTYPE pContent=NULL;
	pNUMBERSTRUCT pResult=NULL;
	pOPTIONELEMENT pOption=NULL;
	pNUMBERSTRUCT pTemp=NULL;

	/* Step through all elements on this level */
	while ((pElement!=NULL)&&(!*pfError))
	{
		/* Check OPTGROUP or OPTION */
		if (pElement->iType==Type_OPTGROUP)
		{
			pContent=WML_GetContent(pElement);
			pTemp=WML_GetSelectedOptions(pContent,ppAfter,pfError,piIndex);

			if (pTemp!=NULL)
			{
				if (pResult==NULL)
				{
					pResult=pTemp;
				}
			}
		}
		else if (pElement->iType==Type_OPTION)
		{
			pOption=(OPTIONELEMENT*)(pElement);
			if (pOption!=NULL)
			{
				(*piIndex)++;

				if (pOption->fSelected)
				{
					/* Create new NUMBERSTRUCT */
					pTemp=NEWSTRUCT(NUMBERSTRUCT);

					if (pTemp!=NULL)
					{
						pTemp->iIndex=*piIndex;
						pTemp->pNext=NULL;
					}
					else
					{
						/* Out of memory */
						*pfError=TRUE;
					}

					/* Add to list */
					WML_AddNumberToList(pTemp,ppAfter);

					if (pResult==NULL)
					{
						pResult=pTemp;
					}
					*ppAfter=pTemp;
				}
			}
		}
		pElement=pElement->pNextElement;
	}
	return pResult;
}


/* Returns the number of options in a SELECT element 
   (including those in optgroups). */
UINT16 WML_CountOptions (pELEMENTTYPE pElement, BOOL *fError)
{
	UINT16 iNbr=0;
	pELEMENTTYPE pContent=NULL;

	/* Step through all elements on this level */
	while (pElement!=NULL)
	{
		/* Check OPTGROUP or OPTION */
		if (pElement->iType==Type_OPTGROUP)
		{
			pContent=WML_GetContent(pElement);
			iNbr+=WML_CountOptions(pContent, fError);
		}
		else if (pElement->iType==Type_OPTION)
		{
			iNbr++;
		}
		else 
		{
			/* Error! */
			*fError=TRUE;
		}
		pElement=pElement->pNextElement;
	}
	return iNbr;
}

pNUMBERSTRUCT WML_CreateNUMBERSTRUCT (void)
{
	pNUMBERSTRUCT pTemp=NULL;

	pTemp=NEWSTRUCT(NUMBERSTRUCT);
	if (pTemp!=NULL)
	{
		pTemp->iIndex=0;
		pTemp->pNext=NULL;
	}
	return pTemp;
}

void WML_DeleteNUMBERSTRUCTList (pNUMBERSTRUCT pNumber)
{
	pNUMBERSTRUCT pTemp=NULL;

	while (pNumber!=NULL)
	{
		pTemp=pNumber;
		pNumber=pNumber->pNext;
		DEALLOC(&pTemp);
	}
}

/* Helpnction for ValidateIndex. */
void WML_AddIndexNumber (pNUMBERSTRUCT *ppList, UINT16 iIndex, UINT16 iMax)
{
	pNUMBERSTRUCT pTemp=NULL;
	pNUMBERSTRUCT pBefore=NULL;
	pNUMBERSTRUCT pAfter=NULL;
	pNUMBERSTRUCT pIndex=NULL;
	BOOL fInsert=FALSE;

	if ((iIndex>0)&&(iIndex<=iMax))
	{
		pTemp=*ppList;

		if (*ppList==NULL)
		{
			fInsert=TRUE;
		}

		/* Find where to insert element */
		while ((!fInsert)&&(pTemp!=NULL))
		{
			pAfter=pTemp->pNext;

			if (iIndex<pTemp->iIndex)
			{
				pAfter=pTemp;
				fInsert=TRUE;
			}
			else if (iIndex>pTemp->iIndex)
			{
				pBefore=pTemp;
				pAfter=pTemp->pNext;
				pTemp=pTemp->pNext;
				if (pAfter!=NULL)
				{
					if (iIndex<pAfter->iIndex)
					{
						fInsert=TRUE;
					}
				}
				else
				{
					fInsert=TRUE;
				}
			}
			else
			{
				/* Equal - don't insert */
				fInsert=FALSE;
				pTemp=NULL;
			}
		}

		if (fInsert)
		{
			pIndex=NEWSTRUCT(NUMBERSTRUCT);
			if (pIndex!=NULL)
			{
				pIndex->iIndex=iIndex;
				pIndex->pNext=NULL;
			}

			if (*ppList==NULL)
			{
				*ppList=pIndex;
			}
			else
			{
				pIndex->pNext=pAfter;

				if (pBefore!=NULL)
				{
					pBefore->pNext=pIndex;
				}
				else
				{
					/* First in list */
					*ppList=pIndex;
				}
			}
		}
	}
}

/* Function for ordering the indices in a SELECT element.
   The function sorts the numbers and removes duplicate 
   indices. It also checks if the number is a valid option. 
   The result is stored in a NUMBERSTRUCT. */
pNUMBERSTRUCT WML_ValidateIndex (WCHAR *pchIndex, UINT16 iMax)
{
	pNUMBERSTRUCT pResult=NULL;
	WCHAR chChar;
	BOOL fError=FALSE;
	BOOL fStarted=FALSE;
	BOOL fStopped=FALSE;
	UINT16 iIndex=0;

	if (pchIndex!=NULL)
	{
		chChar=*pchIndex;

		while (chChar!=0)
		{
			if (chChar==' ')
			{
				if (fStarted)
				{
					fStopped=TRUE;	
				}
			}
			else if (chChar=='.')
			{
				/* Decimal index - not allowed */
				fError=TRUE;
				iIndex=0;
			}
			else if ((chChar>=WC('0'))&&(chChar<=WC('9')))
			{
				if (!fStopped)
				{
					iIndex=(UINT16)((iIndex*10)+chChar-WC('0'));
					fStarted=TRUE;
				}
				else 
				{
					fError=TRUE;
				}
			}
			else if (chChar==WC(';'))
			{
				if ((fStarted)&&(!fError))
				{
					WML_AddIndexNumber(&pResult,iIndex,iMax);
				}
	
				iIndex=0;
				fError=FALSE;
				fStarted=FALSE;
				fStopped=FALSE;
			}
			else
			{
				fError=TRUE;
			}
			pchIndex++;
			chChar=*pchIndex;
		}

		/* End of text. Add last index. */
		if ((fStarted)&&(!fError))
		{
			WML_AddIndexNumber(&pResult,iIndex,iMax);
		}
	}
	return pResult;
}

/* Help function for FindIndexNumber. */
UINT16 WML_FindOption (pUA pUser, pELEMENTTYPE pElement, BOOL *pfError, 
				   BOOL *pfFound, WCHAR *pchString)
{
	UINT16 iNbr=0;
	pELEMENTTYPE pContent=NULL;
	pOPTIONELEMENT pOption=NULL;
	WCHAR *pchTemp=NULL;

	/* Step through all elements on this level */
	while ((pElement!=NULL)&&(!*pfFound))
	{
		/* Check OPTGROUP or OPTION */
		if (pElement->iType==Type_OPTGROUP)
		{
			pContent=WML_GetContent(pElement);
			iNbr+=WML_FindOption(pUser,pContent,pfError,pfFound,pchString);
		}
		else if (pElement->iType==Type_OPTION)
		{
			iNbr++;

			/* Get value */
			pOption=(OPTIONELEMENT*)(pElement);

			if (pOption!=NULL)
			{
				pchTemp=WML_GetString(pUser,pOption->pValue);
				if (pchTemp!=NULL)
				{
					/* Remove trailing and leading blanks (?)
					pchTrimTemp=TrimString(pchTemp); */

					if (pchTemp!=NULL)
					{
						/* Compare strings */
						if (COMPARESTRING(pchTemp,pchString)==0)
						{
							*pfFound=TRUE;
						}
					}
					DEALLOC(&pchTemp);
				}
			}
		}
		else
		{
			/* Error! */
			*pfError=TRUE;
		}
		pElement=pElement->pNextElement;
	}
	return iNbr;
}

/* Returns the number of the option with the value
   equal of to that of the indata. (The strings are
   compared.) The function also removes an item from
   a ';'-separated list if necessary.) */
UINT16 WML_FindIndexNumber (pUA pUser, WCHAR *pchString, UINT16 iStrLen, 
						pELEMENTTYPE pOption)
{
	WCHAR *pchTestString=NULL;
	UINT16 iIndex=0;
	BOOL fError=FALSE;
	BOOL fFound=FALSE;

	pchTestString=NEWARRAY(WCHAR,iStrLen+1);
	
	if (pchTestString!=NULL)
	{
		COPYSTRINGN(pchTestString,pchString,iStrLen);
		*(pchTestString+(iStrLen))=0;

		/* Remove trailing and leading blanks (?)
		pchTrimString=TrimString(pchTestString);*/

		if (pchTestString!=NULL)
		{
			/* Find in values in the options */
			iIndex=WML_FindOption(pUser,pOption,&fError,&fFound,pchTestString);
			
			if ((fError)||(!fFound))
			{
				iIndex=0;
			}
		}
		DEALLOC(&pchTestString);
	}
	return iIndex;
}

/* All options indicated in the pchIndex is located and a list
   of the indices is returned (NUMBERSTRUCT). If fMultiple is
   set the different options are found (i.e. "option1;option2"
   means "option1" and "option2" should be set. If fMultiple
   is not set "option1;option2" is considered as ONE item) */
pNUMBERSTRUCT WML_SetKeyIndex (pUA pUser, WCHAR *pchIndex, BOOL fMultiple, 
						   pELEMENTTYPE pOption, UINT16 iMax)
{
	pNUMBERSTRUCT pResult=NULL;
	WCHAR chChar;
	WCHAR *pchStart=NULL;
	UINT16 iIndex=0;

	if (pchIndex!=NULL)
	{
		chChar=*pchIndex;
		pchStart=pchIndex;

		if (fMultiple)
		{
			while (chChar!=0)
			{
				if (chChar==WC(';'))
				{
					iIndex=WML_FindIndexNumber (pUser,pchStart,(UINT16)(pchIndex-pchStart),pOption);
					WML_AddIndexNumber (&pResult, iIndex, iMax);
					pchStart=pchIndex+1;
				}
				pchIndex++;
				chChar=*pchIndex;
			}

			/* End of text. Check last index. */
			iIndex=WML_FindIndexNumber (pUser,pchStart,(UINT16)(pchIndex-pchStart),pOption);
			WML_AddIndexNumber (&pResult, iIndex, iMax);
		}
		else
		{
			/* Find end of string */
			while (chChar!=0)
			{
				pchIndex++;
				chChar=*pchIndex;
			}

			/* Treat whole string as one value, even if it 
			   contains ';'.								*/
			iIndex=WML_FindIndexNumber (pUser,pchStart,(UINT16)(pchIndex-pchStart),pOption);
			WML_AddIndexNumber (&pResult, iIndex, iMax);
		}
	}
	return pResult;
}

/*========================================================================
	WML_SetSelectDefault
==========================================================================*/

/* Sets the default value for a SELECT element. */
BOOL WML_SetSelectDefault (pELEMENTTYPE pSelect, pUA pUser)
{
	pSELECTELEMENT pSelectElm=NULL;
	pELEMENTTYPE pContent=NULL;
	UINT16 iNbrOfOptions=0;
	BOOL fError=FALSE;
	pNUMBERSTRUCT pDefaultOption=NULL;
	pNUMBERSTRUCT pSelectedTemp=NULL;
	WCHAR *pchOptions=NULL;
	WCHAR *pchTempText=NULL;
	UINT16 iNbr1=0;
	UINT16 iNbr2=0;

	if (pUser!=NULL) 
	{
		if (pSelect!=NULL) 
		{
			if (pSelect->iType==Type_SELECT) 
			{
				pSelectElm=(SELECTELEMENT*)(pSelect);

				pContent=WML_GetContent(pSelect);

				/* Count the options */
				iNbrOfOptions=WML_CountOptions (pContent,&fError);

				/* Determine the default option indices */
				if (pSelectElm->pchIName!=NULL)
				{
					/* IName */

					/* Get variable */
					pchOptions=WML_GetVariable(pUser,pSelectElm->pchIName);

					if (pchOptions!=NULL)
					{
						/* Get options */
						pDefaultOption=WML_ValidateIndex(pchOptions,iNbrOfOptions);

						DEALLOC(&pchOptions);
					}
				}
				if ((pDefaultOption==NULL)&&(pSelectElm->pIValue!=NULL))
				{
					/* IDefault */
					pchTempText=WML_GetString(pUser,pSelectElm->pIValue);

					if (pchTempText!=NULL)
					{
						/* Get options */
						pDefaultOption=WML_ValidateIndex(pchTempText,iNbrOfOptions);
	
						DEALLOC(&pchTempText);
					}
				}
				if ((pDefaultOption==NULL)&&(pSelectElm->pchName!=NULL))
				{
					/* Key */
					pchOptions=WML_GetVariable(pUser,pSelectElm->pchName);

					if (pchOptions!=NULL)
					{
						/* Get options */
						pDefaultOption=WML_SetKeyIndex(pUser,pchOptions,
									pSelectElm->fMultiple, pContent,iNbrOfOptions);

						DEALLOC(&pchOptions);
					}
				}
				if ((pDefaultOption==NULL)&&(pSelectElm->pValue!=NULL))
				{
					/* Default */
					pchTempText=WML_GetString(pUser,pSelectElm->pValue);

					if (pchTempText!=NULL)
					{
						/* Get options */
						pDefaultOption=WML_SetKeyIndex(pUser,pchTempText,
										pSelectElm->fMultiple,pContent,iNbrOfOptions);
	
						DEALLOC(&pchTempText);
					}
				}
				if ((pDefaultOption==NULL)&&(pSelectElm->fMultiple))
				{
					/* Multi-choice */
				}
				if ((pDefaultOption==NULL)&&(!pSelectElm->fMultiple))
				{
					/* Single-choice */
					if (iNbrOfOptions>0)
					{
						pDefaultOption=WML_CreateNUMBERSTRUCT ();
						if (pDefaultOption!=NULL)
						{
							pDefaultOption->iIndex=1;
						}
					}
				}

				/* pre-select options */
				pSelectedTemp=pDefaultOption;

				WML_PreSelectOptions (pContent,&pSelectedTemp,&iNbr1,&iNbr2);

				/* Initialise variables */
				WML_SetSelectVariables(pUser, pSelect, pDefaultOption);

				WML_DeleteNUMBERSTRUCTList (pDefaultOption);
				return TRUE;
			}
		}
	}
	return FALSE;
}


/*========================================================================
	WML_UpdateSelect
==========================================================================*/

/* 	Updates the variables in a specified SELECT. */
BOOL WML_UpdateSelect (pELEMENTTYPE pSelect, pUA pUser)
{
	pELEMENTTYPE pContent=NULL;
	pNUMBERSTRUCT pSelected=NULL;
	pNUMBERSTRUCT pTemp=NULL;
	BOOL fError=FALSE;
	UINT16 iIndex=0;

	if (pUser!=NULL) 
	{
		if (pSelect!=NULL) 
		{
			if (pSelect->iType==Type_SELECT) 
			{
				pContent=WML_GetContent(pSelect);

				if (pContent!=NULL)
				{
					if ((pContent->iType==Type_OPTGROUP)||
							(pContent->iType==Type_OPTION))
					{
						pSelected=WML_GetSelectedOptions (pContent,&pTemp,&fError,&iIndex);
						
						if (!fError)
						{
							/* Set variables */
							WML_SetSelectVariables(pUser, pSelect, pSelected);

							WML_DeleteNUMBERSTRUCTList(pSelected);

							return TRUE;
						}
						else
						{
							WML_DeleteNUMBERSTRUCTList(pSelected);
						}
					}
				}
			}
		}
	}
	return FALSE;
}

/* Help function for WML_UpdateAllSelect */
BOOL WML_UpdateOneLevelSelect (pELEMENTTYPE pContent, pUA pUser)
{
	BOOL fResult=TRUE;

	/* Find all select elements */
	while ((pContent!=NULL)&&(fResult))
	{
		if (pContent->iType==Type_SELECT)
		{
			/* Update select */
			if (!WML_UpdateSelect(pContent,pUser))
			{
				/* Error */
				return FALSE;
			}
		}

		/* Go down one level */
		else if ((pContent->iType==Type_FIELDSET)||(pContent->iType==Type_P))
		{
			fResult=WML_UpdateOneLevelSelect (WML_GetContent(pContent), pUser);
		}

		/* Get next element on level */
		pContent=pContent->pNextElement;
	}

	return fResult;
}

/*========================================================================
	WML_UpdateAllSelect
==========================================================================*/
BOOL WML_UpdateAllSelect (pELEMENTTYPE pCard, pUA pUser)
{
	BOOL fResult=FALSE;
	pELEMENTTYPE pContent=NULL;

	if (pCard!=NULL)
	{
		/* Check if card */
		if (pCard->iType==Type_CARD)
		{
			/* Get Card level content */
			pContent=WML_GetContent(pCard);

			/* Update all select on card level and below */
			fResult=WML_UpdateOneLevelSelect (pContent, pUser);
		}
	}

	return fResult;
}

/*========================================================================
	WML_PreSelectOptions
==========================================================================*/
/* Sets the 'SELECTED'-field in all the option elements
   indicated in the NUMBERSTRUCT to TRUE, all other to
   FALSE. */   
void WML_PreSelectOptions(pELEMENTTYPE pElement, pNUMBERSTRUCT *ppDefaultOption,
					  UINT16 *piFindNbr, UINT16 *piThisNbr)
{
	pOPTIONELEMENT pOption=NULL;
	pELEMENTTYPE pContent=NULL;

	if (*piThisNbr==0)
	{
		if (ppDefaultOption!=NULL)
		{
			/* Only if first time */
			if (*ppDefaultOption!=NULL)
			{
				*piFindNbr=(*ppDefaultOption)->iIndex;
				*ppDefaultOption=(*ppDefaultOption)->pNext;
				(*piThisNbr)++;
			}
		}
	}

	while (pElement!=NULL)
	{
		if (pElement->iType==Type_OPTION)
		{
			pOption=(OPTIONELEMENT*)(pElement);
			if ((*piFindNbr==*piThisNbr)&&(*piFindNbr!=0))
			{
				/* Select this option */
				pOption->fSelected=TRUE;

				if (ppDefaultOption!=NULL)
				{
					if (*ppDefaultOption!=NULL)
					{
						*piFindNbr=(*ppDefaultOption)->iIndex;
						*ppDefaultOption=(*ppDefaultOption)->pNext;
					}
				}
			}
			else
			{
				/* De-select this option */
				pOption->fSelected=FALSE;
			}
			(*piThisNbr)++;
		}
		else if (pElement->iType==Type_OPTGROUP)
		{
			pContent=WML_GetContent(pElement);
			WML_PreSelectOptions(pContent,ppDefaultOption,piFindNbr,piThisNbr);
		}
		pElement=pElement->pNextElement;
	}
}



/*========================================================================

	INPUT ELEMENT FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_SetInputKeyValue
==========================================================================*/
BOOL WML_SetInputKeyValue (pUA pUser, pELEMENTTYPE pElementObj, WCHAR *pchValue) 
{
	pINPUTELEMENT pInput;
	WCHAR *pchName;

	if (pUser!=NULL) 
	{
		if (pElementObj!=NULL) 
		{
			if (pElementObj->iType==Type_INPUT) 
			{
				/* Get the key name */
				pInput=(INPUTELEMENT*)(pElementObj);
				pchName=pInput->pchName;

				/* Set the value in the var table */
				if (WML_SetVariable(pUser,pchName,pchValue,FALSE))
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/*========================================================================
	WML_SetInputKeyValue
==========================================================================*/
WCHAR* WML_GetInputKeyValue (pUA pUser, pELEMENTTYPE pElement)
{
	pINPUTELEMENT pInput;
	WCHAR *pchName;
	WCHAR *pchValue=NULL;

	if (pUser!=NULL) 
	{
		if (pElement!=NULL) 
		{
			if (pElement->iType==Type_INPUT) 
			{
				pInput=(INPUTELEMENT*)(pElement);

				/* Get the Key value */
				pchName=pInput->pchName;

				/* Get COPY of the value */
				pchValue=WML_GetVariable(pUser,pchName);

				/* Check if NULL, if so get the default value */
				if (pchValue==NULL)
				{
					pchValue=WML_GetString(pUser,pInput->pValue);
		
					if (pchValue!=NULL)
					{
						WML_SetInputKeyValue(pUser,pElement,pchValue);
					}
				}
			}
		}
	}
	return pchValue;
}




/*========================================================================

	ACTION ELEMENT FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_UpdateEventVariables
==========================================================================*/
void WML_UpdateEventVariables (pUA pUser, pELEMENTTYPE pAction)
{
	pELEMENTTYPE pContent=NULL;
	pSETVARELEMENT pVar=NULL;
	WCHAR *pchVarName=NULL;
	WCHAR *pchVarValue=NULL;

	pVARTABELM pLastInList=NULL;
	pVARTABELM pTempVar=NULL;

	/* Check if the action includes variables to be set. 
	   The VAR element can exist in GO, PREV and REFRESH.
	   If a VAR element is found, set it.*/
	if (pAction!=NULL) 
	{
		if ((pAction->iType==Type_GO)||
			(pAction->iType==Type_PREV)||
			(pAction->iType==Type_REFRESH)) 
		{
			/* Get the content of the action element. */
			pContent=WML_GetContent(pAction);

			/* Empty old variable list */
			WML_DeleteEventVariables (pUser);

			/* Check for VAR element. */
			while (pContent!=NULL) 
			{
				if (pContent->iType==Type_SETVAR) 
				{
					pVar=(SETVARELEMENT*)(pContent);
					
					/* Set the variable. */
					pchVarName=WML_GetString(pUser,pVar->pName);
					pchVarValue=WML_GetString(pUser,pVar->pValue);

					/* Check if valid variable name. */
					if (WML_CheckVariableSyntax(pchVarName,FALSE))
					{
						/* Create struct */
						pTempVar=NEWSTRUCT(VARTABELM);

						if (pTempVar!=NULL)
						{
							/* Add values to struct */
							pTempVar->pchName=pchVarName;
							pTempVar->pchValue=pchVarValue;
							pTempVar->pNextVar=NULL;

							/* Add struct to list */
							if (pLastInList!=NULL)
							{
								pLastInList->pNextVar=pTempVar;
								pLastInList=pTempVar;
							}
							else
							{
								pUser->pEventVariables=pTempVar;
								pLastInList=pTempVar;
							}
						}
					}
					else
					{
						/* Not valid values - remove */
						DEALLOC(&pchVarName);
						DEALLOC(&pchVarValue);
					}
				}
				pContent=pContent->pNextElement;
			}
		}
	}
}


/*========================================================================
	WML_HandleEvent
==========================================================================*/
pELEMENTTYPE WML_HandleEvent (pUA pUser, pELEMENTTYPE pElement, INT8 iEvent, 
							  BOOL fChangeVars)
{
	pELEMENTTYPE pAction=NULL;

	if (pElement!=NULL) 
	{
		if (pElement->iType==Type_CARD)
		{
			/* Check CARD level Event */
			pAction=WML_GetEventAction(iEvent,
					((CARDELEMENT*)(pElement))->pEventTab);

			if (pAction==NULL) 
			{
				if (pUser!=NULL) 
				{
					/* Check TEMPLATE level Event */
					pAction=WML_GetEventAction (iEvent,pUser->pTemplateEvents);
				}
			}

			if (fChangeVars)
			{
				/* Update variables, if any */
				WML_UpdateEventVariables (pUser, pAction);
			}
		}
	}

	return pAction;
}


/*========================================================================
	WML_HandleMMIAction
==========================================================================*/
pELEMENTTYPE WML_HandleMMIAction (pUA pUser, pELEMENTTYPE pElement, BOOL fChangeVars)
{
	pELEMENTTYPE pAction=NULL;
	pELEMENTTYPE pContent=NULL;

	if (pUser!=NULL)
	{
		if (pElement!=NULL)
		{
			/* do */
			if (pElement->iType==Type_DO)
			{
				/* Get Action */
				pAction=WML_GetContent(pElement);
			}

			/* option */
			else if (pElement->iType==Type_OPTION)
			{
				/* The action is always an attribute 
				   (or moved there during the parse) */
				pAction=((OPTIONELEMENT*)(pElement))->pOnpick;
			}

			/* anchor */
			else if (pElement->iType==Type_ANCHOR)
			{
				/* Get the event in an anchor element */
				pContent=WML_GetContent(pElement);

				/* Find the action. */
				while (pContent!=NULL)
				{
					if ((pContent->iType==Type_GO)||
							(pContent->iType==Type_PREV)||
							(pContent->iType==Type_REFRESH))
					{
						/* Action found. */
						pAction=pContent;

						/* Quit search. */
						pContent=NULL;
					}
					pContent=WML_GetNextElement(pContent);
				}
			}

			/* a */
			else if (pElement->iType==Type_A)
			{
				/* Return the element, the action is the eleemnt.
				   Href is an attribute in a. */
				pAction=pElement;
			}

			if (fChangeVars)
			{
				/* Update variables, if any */
				WML_UpdateEventVariables (pUser, pAction);
			}
		}
	}

	return pAction;
}




/*========================================================================

	TIMER ELEMENT FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_GetTimer
==========================================================================*/
UINT32 WML_GetTimer (pUA pUser, pELEMENTTYPE pTimer)
{
	WCHAR *pchValue;
	pTIMERELEMENT pTimerElm;
	UINT32 iResult=0;

	if (pUser!=NULL) 
	{
		if (pTimer!=NULL) 
		{
			if (pTimer->iType==Type_TIMER) 
			{
				pTimerElm=(TIMERELEMENT*)(pTimer);

				/* Check value of KEY */
				pchValue=WML_GetVariable(pUser,pTimerElm->pchName);

				/* If NULL, get default value. */
				if (pchValue==NULL)
				{
					pchValue=WML_GetString(pUser,pTimerElm->pValue);
				}

				/* Convert to UINT32 */
				if (pchValue!=NULL) 
				{
					if (Convert_StringToUINT32(&pchValue,&iResult))
					{
						return iResult;
					}
				}
			}
		}
	}
	return 0;
}


/*========================================================================
	WML_StoreTimer
==========================================================================*/
/* Help function for StoreTimer */
void WML_SetOneUINT32Number (UINT32 iNumber, WCHAR **ppchString)
{
	UINT32 iDiv=0;
	UINT32 iMod=0;

	if (iNumber>0)
	{
		iDiv=iNumber/10;
		iMod=iNumber%10;

		WML_SetOneUINT32Number(iDiv,ppchString);

		**ppchString=(UINT16)(WC('1')+(UINT16)iMod-1);

		(*ppchString)++;
	}
}

BOOL WML_StoreTimer (pUA pUser, pELEMENTTYPE pTimer, UINT32 iValue)
{
	pTIMERELEMENT pTimerElm;
	WCHAR *pchValue=NULL;
	WCHAR *pchValueTemp=NULL;

	if ((pTimer!=NULL)&&(pUser!=NULL))
	{
		if (pTimer->iType==Type_TIMER) 
		{
			pTimerElm=(TIMERELEMENT*)(pTimer);

			/* Allocate memory for string 
			   (an UINT32 is 10 digits at most) */
			pchValue=NEWARRAY(WCHAR,11);

			if (pchValue!=NULL)
			{
				pchValueTemp=pchValue;

				if (iValue==0)
				{
					*pchValueTemp++=(UINT16)(WC('0'));
				}
				else
				{
					WML_SetOneUINT32Number(iValue,&pchValueTemp);
				}

				/* Set termination character */
				*pchValueTemp=0;

				/* Set variable. fWTA is FALSE - cannot be set from here */
				if (WML_SetVariable(pUser,pTimerElm->pchName,pchValue,FALSE))
				{
					DEALLOC(&pchValue);
					return TRUE;
				}

				DEALLOC(&pchValue);
			}
		}
	}
	return FALSE;
}


/*========================================================================

	WML ELEMENT FUNCTIONS 

=========================================================================*/

/*========================================================================
	WML_GetNextElement
==========================================================================*/
pELEMENTTYPE WML_GetNextElement (pELEMENTTYPE pElement)
{
	BOOL fText=TRUE;

	/* If start element is NOT text, get the first follwoing
	   element. */
	if ((pElement!=NULL)&&(!isTypeWMLText(pElement->iType)))
	{
		pElement=pElement->pNextElement;
	}
	else
	{
		/* Continue until NULL or a non-text element id found
		   (Type_Inline_Text, Type_Table_Text, Type_Entity or
		   Type_Ext).
		*/
		while ((pElement!=NULL)&&(fText))
		{
			pElement=pElement->pNextElement;

			/* Check if text */
			if ((pElement!=NULL)&&(!isTypeWMLText(pElement->iType)))
			{
				fText=FALSE;		
			}
		}
	}

	return pElement;
}

/*========================================================================
	WML_GetCard
==========================================================================*/
pELEMENTTYPE WML_GetCard (pUA pUser, WCHAR *pchName) 
{
	pELEMENTTYPE pCont=NULL;
	pWMLELEMENT pWML=NULL;
	pELEMENTTYPE pFirstCard=NULL;
	WCHAR *pchTempName=NULL;

	if (pUser!=NULL) 
	{
		/* Get the deck */
		pCont=pUser->pDeck;

		if (pCont!=NULL) 
		{
			/* Get the first element in the deck */
			pWML=(WMLELEMENT*)(pCont);
			pCont=pWML->pContent;

			while (pCont!=NULL) 
			{
				if (pCont->iType==Type_CARD) 
				{
					if (pFirstCard==NULL)
					{
						/* First card found */
						pFirstCard=pCont;
						pUser->pCurrentCard=pCont;
					}

					pchTempName=((CARDELEMENT*)(pCont))->pchId;

					if ((pchName!=NULL) && (pchTempName!=NULL))
					{
						if (COMPARESTRING(pchName,pchTempName)==0)
						{
							return pCont;
						}
					}
				}
				pCont=pCont->pNextElement;
			}
		}
	}
	return pFirstCard;
}


/*========================================================================
	WML_GetNextCard
==========================================================================*/
pELEMENTTYPE WML_GetNextCard (pUA pUser)
{
	pELEMENTTYPE pTemp=NULL;

	if (pUser!=NULL) 
	{
		pTemp=pUser->pCurrentCard;

		if (pTemp!=NULL)
		{
			pTemp=pTemp->pNextElement;

			/* Find next CARD */
			while (pTemp!=NULL) 
			{
				/* Check if CARD */
				if (pTemp->iType==Type_CARD)
				{
					/* Next CARD found */
					pUser->pCurrentCard=pTemp;
					return pTemp;
				}

				pTemp=pTemp->pNextElement;
			}
		}
	}
	return NULL;
}


/*========================================================================
	WML_GetCardTitle
==========================================================================*/
WCHAR* WML_GetCardTitle (pUA pUser, pELEMENTTYPE pCARD)
{
	pCARDELEMENT pCardElement;

	if (pUser!=NULL)
	{
		if (pCARD!=NULL)
		{
			if (pCARD->iType==Type_CARD)
			{
				pCardElement=(CARDELEMENT*)(pCARD);
				return WML_GetString(pUser,pCardElement->pTitle);
			}
		}
	}
	return NULL;
}


/*========================================================================
	WML_GetTemplateDO
==========================================================================*/
pTEMPLATEDOTYPE WML_GetTemplateDO (pELEMENTTYPE pCARD)
{
	if (pCARD!=NULL)
	{
		if (pCARD->iType==Type_CARD)
		{
			return (((CARDELEMENT*)(pCARD))->pTemplateDO);
		}
	}
	return NULL;
}


/*========================================================================
	WML_GetNextTemplateDO
==========================================================================*/
pTEMPLATEDOTYPE WML_GetNextTemplateDO (pTEMPLATEDOTYPE pTemplateDO)
{
	if (pTemplateDO!=NULL)
	{
		return (pTemplateDO->pNext);
	}
	return NULL;
}


/*========================================================================
	WML_GetDOFromTemplate
==========================================================================*/
pELEMENTTYPE WML_GetDOFromTemplate (pTEMPLATEDOTYPE pTemplateDO)
{
	if (pTemplateDO!=NULL)
	{
		return pTemplateDO->pDO;
	}
	return NULL;
}



/*========================================================================
	WML_CheckAccessElement
==========================================================================*/
BOOL WML_CheckAccessElement (pELEMENTTYPE pAccess, BYTE *pbURL, BYTE *pbDefaultDomain)
{
	pACCESSELEMENT pAccessElm=NULL;
	BOOL fResult=FALSE;
	BOOL fOverflow=FALSE;
	BYTE *pbTempDomain=NULL;
	BYTE *pbPath=NULL;
	BYTE *pbDomain=NULL;

	if (pAccess!=NULL)
	{
		pAccessElm=(pACCESSELEMENT)pAccess;

		if (pAccessElm!=NULL)
		{
			/* Get domain */
			if (pAccessElm->pchDomain!=NULL)
			{
				/* Convert host to byte*. Dealloc pbTempDomain 
				   after comparision (not pbDomain) */

				pbTempDomain=wip_wchar2byte(pAccessElm->pchDomain,&fOverflow);
				pbDomain=pbTempDomain;

				if ((pbTempDomain==NULL)||(fOverflow))
				{
					/* Error */
					DEALLOC(&pbTempDomain);
					return FALSE;
				}
			}
			else
			{
				pbDomain=pbDefaultDomain;
			}

			/* Get path */
			if (pAccessElm->pchPath!=NULL)
			{
				/* Convert path to byte*. Dealloc pbPath
				   after comparision. */
				pbPath=wip_wchar2byte(pAccessElm->pchPath,&fOverflow);

				if ((pbPath==NULL)||(fOverflow))
				{
					/* Error */
					DEALLOC(&pbTempDomain);
					DEALLOC(&pbPath);
					return FALSE;
				}
			}

			/* Path and Domain specified - check access */
			fResult=URL_CheckAccess (pbURL, pbDomain, pbPath);

			/* DEALLOC temporary strings */
			DEALLOC(&pbTempDomain);
			DEALLOC(&pbPath);
		}
	}
	return fResult;
}

/*========================================================================
	WML_NewContext
==========================================================================*/
void WML_NewContext (pUA pUser)
{
	if (pUser!=NULL)
	{
		/* Empty variable table */
		WML_EmptyVarTable (pUser);

		/* Remove all history entries but the newest */
		WML_EmptyHistoryNewContext (pUser);
	}
}



/*========================================================================
	UApragmaFromScript
==========================================================================*/
void UApragmaFromScript(pUA userAgent, WCHAR** ppwchPropertyName, 
						WCHAR** ppwchContent, WCHAR** ppwchScheme )
{
/*  Added by GBU,990702,    to get rid of IAR warning */
    userAgent = userAgent;

	/* no UA pragma handling at this time */
	DEALLOC( ppwchPropertyName );
	DEALLOC( ppwchContent );
	DEALLOC( ppwchScheme  );
}





/*========================================================================
	WML_IsPEmpty
==========================================================================*/
BOOL WML_IsPEmpty (pUA pUser, pELEMENTTYPE pPElement)
{ 
	pELEMENTTYPE pContent=NULL;
	WCHAR *pchText=NULL;

	if ((pPElement==NULL)||(pUser==NULL))
	{
		/* NO ELEMENT - EMPTY (?) */
		return TRUE;
	}

	if (pPElement->iType!=Type_P)
	{
		/* Not p element */
		return FALSE;
	}

	/* Get p content */
	pContent=WML_GetContent(pPElement);

	while (pContent!=NULL)
	{
		if (isTypeWMLText(pContent->iType))
		{
			/* Text found, check if only white space */
			pchText=WML_GetString (pUser, pContent);

			if (pchText!=NULL)
			{
				if (!(WML_CheckIfOnlyWhiteSpace(pchText)))
				{
					/* Not empty */
					DEALLOC(&pchText);
					return FALSE;
				}
				DEALLOC(&pchText);
			}
		}
		else
		{
			/* Other element found, return FALSE  
			   (p element not empty) */
			return FALSE;
		}

		/* Get next content */
		pContent=WML_GetNextElement(pContent);
	}

	/* p element is empty */
	return TRUE;
}



/*========================================================================
	WML_TrimTextWCHAR
	WML_TrimTextBYTE
==========================================================================*/
BOOL WML_TrimTextWCHAR (WCHAR *pchText, UINT8 iMode)
{
	WCHAR *pchRead=NULL;
	WCHAR *pchWrite=NULL;
	WCHAR *pchLastChar=NULL;
	BOOL fSpaceFound=FALSE;
	BOOL fResult=FALSE;

	if (pchText==NULL)
	{
		return FALSE;
	}

	pchWrite=pchText;
	pchRead=pchText;

	/* Trim left? */
	if ((iMode)&(TRIMTEXT_LEFT))
	{
		BOOL fSpaceFound=TRUE;

		/* Find first non-white space char */
		while ((*pchRead!=0)&&(fSpaceFound))
		{
			if (isXMLWhiteSpace(*pchRead))
			{
				/* Do not step write pos */
				pchRead++;
			}
			else
			{
				/* Not white space */
				fSpaceFound=FALSE;
			}
		}
	}

	pchLastChar=pchWrite;

	/* Remove insignificant white spaces */
	while (*pchRead!=0)
	{
		if (isXMLWhiteSpace(*pchRead))
		{
			/* All white spaces -> x20 */
			*pchRead=(WCHAR)('\x20');

			if (fSpaceFound)
			{
				/* Do not step write pos */
				pchRead++;
			}
			else
			{
				fSpaceFound=TRUE;
				*pchWrite++=*pchRead++;
			}
		}
		else
		{
			pchLastChar=pchWrite;

			/* Not white space - write */
			*pchWrite++=*pchRead++;
			fSpaceFound=FALSE;
		}
	}

	/* Trim right? */
	if ((iMode)&(TRIMTEXT_RIGHT))
	{
		if (isXMLWhiteSpace(*pchLastChar))
		{
			*pchLastChar=0;
		}
		else if (!*pchLastChar++==0)
		{
			*pchLastChar=0;
		}
	}
	else
	{
		/* Check if last char is white space.
		   (For not LEFT)
		*/
		if (isXMLWhiteSpace(*pchLastChar))
		{
			if (!((iMode)&(TRIMTEXT_LEFT)))
			{
				fResult=TRUE;
			}
		}
		else
		{
			/* Find last character in string */
			if (pchLastChar<pchWrite)
			{
				pchLastChar++;
			}

			/* Check if last char is white space. */
			if (isXMLWhiteSpace(*pchLastChar))
			{
				fResult=TRUE;
			}
		}
	}

	/* Set new termination character */
	*pchWrite=0;

	return fResult;
}


/* Byte version */
BOOL WML_TrimTextBYTE (BYTE *pbText, UINT8 iMode)
{
	BYTE *pbRead=NULL;
	BYTE *pbWrite=NULL;
	BYTE *pbLastChar=NULL;
	BOOL fSpaceFound=FALSE;
	BOOL fResult=FALSE;

	if (pbText==NULL)
	{
		return FALSE;
	}

	pbWrite=pbText;
	pbRead=pbText;

	/* Trim left? */
	if ((iMode)&(TRIMTEXT_LEFT))
	{
		BOOL fSpaceFound=TRUE;

		/* Find first non-white space char */
		while ((*pbRead!=0)&&(fSpaceFound))
		{
			if (B_isXMLWhiteSpace(*pbRead))
			{
				/* Do not step write pos */
				pbRead++;
			}
			else
			{
				/* Not white space */
				fSpaceFound=FALSE;
			}
		}
	}

	pbLastChar=pbWrite;

	/* Remove insignificant white spaces */
	while (*pbRead!=0)
	{
		if (B_isXMLWhiteSpace(*pbRead))
		{
			/* All white spaces -> x20 */
			*pbRead=(BYTE)('\x20');

			if (fSpaceFound)
			{
				/* Do not step write pos */
				pbRead++;
			}
			else
			{
				fSpaceFound=TRUE;
				*pbWrite++=*pbRead++;
			}
		}
		else
		{
			pbLastChar=pbWrite;

			/* Not white space - write */
			*pbWrite++=*pbRead++;
			fSpaceFound=FALSE;
		}
	}

	/* Trim right? */
	if ((iMode)&(TRIMTEXT_RIGHT))
	{
		if (B_isXMLWhiteSpace(*pbLastChar))
		{
			*pbLastChar=0;
		}
		else if (!*pbLastChar++==0)
		{
			*pbLastChar=0;
		}
	}
	else
	{
		/* Check if last char is white space.
		   (For not LEFT)
		*/
		if (B_isXMLWhiteSpace(*pbLastChar))
		{
			if (!((iMode)&(TRIMTEXT_LEFT)))
			{
				fResult=TRUE;
			}
		}
		else
		{
			/* Find last character in string */
			if (pbLastChar<pbWrite)
			{
				pbLastChar++;
			}

			/* Check if last char is white space. */
			if (B_isXMLWhiteSpace(*pbLastChar))
			{
				fResult=TRUE;
			}
		}
	}

	/* Set new termination character */
	*pbWrite=0;

	return fResult;
}


/*========================================================================
	WML_GetTextEncodForm 
==========================================================================*/
INT16 WML_GetTextEncodForm (pUA pUser) 
{
	if (pUser!=NULL)
	{
		return (pUser->iTextEncodFormat);
	}
	return 0;
}




/*========================================================================
	WML_GetString
==========================================================================*/
WCHAR* WML_GetString (pUA pUser, pELEMENTTYPE pElement)
{
	UINT32 iLength=0;
	WCHAR* pwchString=NULL;
	WCHAR* pwchTemp=NULL;
	WCHAR* pwchExt=NULL;
	WCHAR* pwchExtEscaped=NULL;
	WCHAR pwchEntity[2];
	pELEMENTTYPE pTemp=NULL;
	BOOL fText=TRUE;

	/* Get length -> >0 implies pElement NOT NULL*/
	iLength=WML_GetStringLength(pUser,pElement);

	if (iLength==0)
	{
		return NULL;
	}

	/* Allocate memory for string plus termination character */
	pwchString=NEWARRAY(WCHAR,iLength+1);

	/* Out of memory? */
//	#ifndef HAS_SETJMP
	if (pwchString==NULL)
	{
		CLNTa_log(1,0,"fatal error");
		return NULL;
	}
//	#endif

	/* Set termination character */
	*pwchString=0;

	/* Get all string parts */
	pTemp=pElement;

	while ((pTemp!=NULL)&&(fText))
	{
		/* Inline */
		if (pTemp->iType==Type_Inline_Text)
		{
			CONCATSTRING(pwchString,((pI_TEXTELEMENT)pTemp)->pwchTextData);
		}

		/* String Table */
		else if (pTemp->iType==Type_Table_Text)
		{
			/* Get string from string table */
			pwchTemp=CreateStringCopy(WBXML_GetStrTabVal 
							(pUser->pDecStr,((pT_TEXTELEMENT)pTemp)->iTableRef));
			
			if (pwchTemp!=NULL)
			{
				CONCATSTRING(pwchString,pwchTemp);
				DEALLOC(&pwchTemp);
			}
		}

		/* Entity */
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
			CONCATSTRING(pwchString,pwchEntity);
		}

		/* Extensions */
		else if (pTemp->iType==Type_Ext_0)
		{
			pwchExt=WML_GetVariable(pUser,((pEXT_ELEMENT)pTemp)->pwchName);

			/* URL-escape text. */
			pwchExtEscaped=w_wmlVariableEscape(pwchExt);

			if (pwchExtEscaped!=NULL)
			{
				CONCATSTRING(pwchString,pwchExtEscaped);
			}

			DEALLOC(&pwchExt);
			DEALLOC(&pwchExtEscaped);
		}
		else if (pTemp->iType==Type_Ext_1)
		{
			pwchExt=WML_GetVariable(pUser,((pEXT_ELEMENT)pTemp)->pwchName);

			/* URL-unescape text. */
			pwchExtEscaped=w_UnescapeString(pwchExt);

			if (pwchExtEscaped!=NULL)
			{
				CONCATSTRING(pwchString,pwchExtEscaped);
			}

			DEALLOC(&pwchExt);
			DEALLOC(&pwchExtEscaped);
		}
		else if (pTemp->iType==Type_Ext_2)
		{
			pwchExt=WML_GetVariable(pUser,((pEXT_ELEMENT)pTemp)->pwchName);

			/* NO URL-escaping! */
			if (pwchExt!=NULL)
			{
				CONCATSTRING(pwchString,pwchExt);
				DEALLOC(&pwchExt);
			}
		}

		else
		{
			/* Not text */
			fText=FALSE;
		}

		pTemp=pTemp->pNextElement;
	}

	return pwchString;
}


/*========================================================================
	WML_GetStringLength
==========================================================================*/
UINT32 WML_GetStringLength (pUA pUser, pELEMENTTYPE pElement)
{
	pELEMENTTYPE pTemp=pElement;
	UINT32 iLength=0;
	BOOL fText=TRUE;
	WCHAR* pwchTempText=NULL;
	WCHAR* pwchEscText=NULL;

	/* Get all concecutive text parts */
	while ((pTemp!=NULL)&&(fText))
	{
		if (pTemp->iType==Type_Inline_Text)
		{
			/* pchTextData is not NULL - operation ok */
			iLength+=(STRINGLENGTH(((pI_TEXTELEMENT)pTemp)->pwchTextData));
		}
		else if (pTemp->iType==Type_Table_Text)
		{
			/* Get string from string table */
			pwchTempText=WBXML_GetStrTabVal (pUser->pDecStr,
									((pT_TEXTELEMENT)pTemp)->iTableRef);
			
			if (pwchTempText!=NULL)
			{
				/* Calculate length */
				iLength+=(STRINGLENGTH(pwchTempText));
			}
		}
		else if (pTemp->iType==Type_Entity)
		{
			/* Add one WCHAR */
			iLength++;
		}

		/* If a variable, get it and calculate length
		   and take care of the URL-escaping. */
		else if (pTemp->iType==Type_Ext_0) 
		{
			pwchTempText=WML_GetVariable(pUser,((pEXT_ELEMENT)pTemp)->pwchName);

			/* URL-escape text. */
			pwchEscText=w_wmlVariableEscape(pwchTempText);
			if (pwchEscText!=NULL)
			{
				iLength=iLength+(STRINGLENGTH(pwchEscText));
			}
			DEALLOC(&pwchTempText);
			DEALLOC(&pwchEscText);
		}
		else if (pTemp->iType==Type_Ext_1)
		{
			pwchTempText=WML_GetVariable(pUser,((pEXT_ELEMENT)pTemp)->pwchName);

			/* URL-unescape text. */
			pwchEscText=w_UnescapeString(pwchTempText);
			if (pwchTempText!=NULL)
			{
				iLength=iLength+(STRINGLENGTH(pwchTempText));
			}
			DEALLOC(&pwchTempText);
			DEALLOC(&pwchEscText);
		}
		else if (pTemp->iType==Type_Ext_2) 
		{
			/* NO URL-escaping! */
			pwchTempText=WML_GetVariable(pUser,((pEXT_ELEMENT)pTemp)->pwchName);
			if (pwchTempText!=NULL)
			{
				iLength=iLength+(STRINGLENGTH(pwchTempText));
			}
			DEALLOC(&pwchTempText);
		}
		else
		{
			/* Not text */
			fText=FALSE;
		}

		pTemp=pTemp->pNextElement;
	}

	return iLength;
}
