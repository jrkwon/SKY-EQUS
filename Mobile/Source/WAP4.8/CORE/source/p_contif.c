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

	WAP Implementation Project

==========================================================================

    File: p_contif.c

    Description: PUSH Content Interface file.

    Author: Jens Pommer, AU-System AB

    Revision history:
    Date    Sig     Comment
    991116  JPR     First version

    000207  JPR     Updated to support new WBXML-decoder
    000208  JPR     New functions added
    000814  JPR     Handling of PI elements corrected
    010119  NKE     Updated Si_BuildStructure, Sl_BuildStructure and
                    Co_BuildStructure for the new decoder in wbxmldec.c.
    010126  NKE     Updated Si_BuildStructure

=========================================================================*/

#include "p_contif.h"
#include "pushelm.h"


/*========================================================================

	Service Indication Functions

=========================================================================*/

/*========================================================================
	Si_BuildStructure
======================================================================== */
pDECSTR Si_BuildStructure (BYTE *pbData, BYTE *pbEnd, INT16 iCharset, 
						   UINT8 iContentLevel, UINT8 iViewID)
{
	pWBXMLDECODESTR pDecStr=NULL;
	pELEMENTTYPE pStructure=NULL;
	BOOL finished;

	/* IS THIS PARAMETER NEEDED? (Error handling) */
	iViewID=iViewID;

	/* Create tWBXMLDECODESTR and init the attributes */
	pDecStr=WBXML_CreateWBXMLDecStr();

	if (pDecStr!=NULL)
	{
		/* Set the function pointers */
		pDecStr->DTD_CreateElement=Si_CreateElement;
		pDecStr->DTD_DeleteElement=Si_DeleteElement;
		pDecStr->DTD_ValidContent=Si_ValidContent;
		pDecStr->DTD_GetContent=Si_GetContent;
		pDecStr->DTD_AppSpecParseMod=Si_AppSpecParseMod;
		pDecStr->DTD_StoreAttributeValue=Si_StoreAttributeValue;
		pDecStr->DTD_GetAttributeValue=Si_GetAttributeValue;
		pDecStr->DTD_LiteralAttributeToToken=Si_LiteralAttributeToToken;
		pDecStr->DTD_LiteralTagToToken=Si_LiteralTagToToken;
		pDecStr->DTD_CheckPublicID=Si_CheckPublicID;

		/* Store data in the decode struct */
		pDecStr->bContentType='\x2d';	/* text/vnd.wap.sic */
		pDecStr->iCharset=iCharset;
		pDecStr->iContentLevel=iContentLevel;
		pDecStr->pbCurByte=pbData;
		pDecStr->pbEnd=pbEnd;
		pDecStr->iDecodeResult=0;		
		pDecStr->pAppSpec=NULL;
		
		/* Parse prolog */
		if (WBXML_DecodeProlog(pDecStr))
		{
			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);

			/* Parse channel */
			WBXML_InitDecode(pDecStr, FALSE);
			pStructure = WBXML_Decode(pDecStr, &finished);

			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);
		}

		/* Check if any (fatal) errors or warnings */
		if ( pDecStr->iDecodeResult == 0)
		{
			/* Store the structure under the application specific pointer
			   in the decode struct. */
			pDecStr->pAppSpec=pStructure;
		}
		else
		{
			/* Error - delete */
			Si_DeleteElement(pDecStr,&pStructure);

			/* Delete decode struct */
			WBXML_DeleteWBXMLDecStr(&pDecStr);
		}
	}

	/* Return the decode struct */
	return pDecStr;
}


/*========================================================================
	Si_DeleteStructure
======================================================================== */
void Si_DeleteStructure (pDECSTR *ppDecStr)
{
	if (*ppDecStr!=NULL)
	{
		/* Get the top element */
		pELEMENTTYPE pStructure=(pELEMENTTYPE)(*ppDecStr)->pAppSpec;

		/* Delete structure */
		Si_DeleteElement(*ppDecStr,&pStructure);
		(*ppDecStr)->pAppSpec=NULL;

		/* Delete decode struct */
		WBXML_DeleteWBXMLDecStr(ppDecStr);
	}
}


/*========================================================================
	Si_GetSi
======================================================================== */
pELEMENTTYPE Si_GetSi (pDECSTR pDecStr)
{
	if (pDecStr!=NULL)
	{
		return (pELEMENTTYPE)pDecStr->pAppSpec;
	}
	return NULL;
}


/*========================================================================
	Si_GetIndication
======================================================================== */
pELEMENTTYPE Si_GetIndication (pELEMENTTYPE pSi)
{
	pELEMENTTYPE pContent=NULL;

	if (pSi!=NULL)
	{
		/* Check type */
		if (pSi->iType==Si_Type_si)
		{
			pContent=Si_GetContent (pSi);

			while (pContent!=NULL)
			{
				/* Check if indication element */
				if (pContent->iType==Si_Type_indication)
				{
					/* Indication found */
					return pContent;
				}

				/* Get next */
				pContent=XML_GetNextElement (pContent);
			}
		}
	}

	/* Return NULL*/
	return NULL;
}


/*========================================================================
	Si_GetInfo
======================================================================== */
pELEMENTTYPE Si_GetInfo (pELEMENTTYPE pSi)
{
	pELEMENTTYPE pContent=NULL;

	if (pSi!=NULL)
	{
		/* Check type */
		if (pSi->iType==Si_Type_si)
		{
			pContent=Si_GetContent (pSi);

			while (pContent!=NULL)
			{
				/* Check if info element */
				if (pContent->iType==Si_Type_info)
				{
					/* Info found */
					return pContent;
				}

				/* Get next */
				pContent=XML_GetNextElement (pContent);
			}
		}
	}

	/* Return NULL */
	return NULL;
}


/*========================================================================
	Si_GetHref
======================================================================== */
BYTE *Si_GetHref (pELEMENTTYPE pIndication)
{
	if (pIndication!=NULL)
	{
		/* Check type */
		if (pIndication->iType==Si_Type_indication)
		{
			if (((SI_INDICATIONELEMENT*)(pIndication))->pbHref!=NULL)
			{
				/* Get copy of href */
				return B_CopyByteString (
					((SI_INDICATIONELEMENT *)(pIndication))->pbHref,-1);
			}
		}
	}

	return NULL;
}


/*========================================================================
	Si_GetIndicationText
======================================================================== */
WCHAR *Si_GetIndicationText (pDECSTR pDecStr, pELEMENTTYPE pIndication)
{
	pELEMENTTYPE pContent=NULL;

	if (pIndication!=NULL)
	{
		pContent=Si_GetContent(pIndication);

		/* Content is #PCDATA */
		if (pContent!=NULL)
		{
			return XML_GetString (pContent,pDecStr);
		}
	}
	
	return NULL;
}


/*========================================================================
	Si_GetSiId
======================================================================== */
WCHAR *Si_GetSiId (pELEMENTTYPE pIndication)
{
	WCHAR *pwchCopy=NULL;

	if (pIndication!=NULL)
	{
		/* Check type */
		if (pIndication->iType==Si_Type_indication)
		{
			/* Get copy of SiId */
			pwchCopy=CreateStringCopy (
				((SI_INDICATIONELEMENT *)(pIndication))->pwchSiId);

			/* return copy */
			return pwchCopy;
		}
	}

	return NULL;
}


/*========================================================================
	Si_GetCreated
======================================================================== */
UINT32 Si_GetCreated (pELEMENTTYPE pIndication)
{
	if (pIndication!=NULL)
	{
		/* Check type */
		if (pIndication->iType==Si_Type_indication)
		{
			/* Return created */
			return (((SI_INDICATIONELEMENT *)(pIndication))->iCreated);
		}
	}

	return 0;
}


/*========================================================================
	Si_GetExpires
======================================================================== */
UINT32 Si_GetExpires (pELEMENTTYPE pIndication)
{
	if (pIndication!=NULL)
	{
		/* Check type */
		if (pIndication->iType==Si_Type_indication)
		{
			/* Return Expires */
			return (((SI_INDICATIONELEMENT *)(pIndication))->iExpires);
		}
	}

	return 0;
}


/*========================================================================
	Si_GetAction
======================================================================== */
UINT8 Si_GetAction (pELEMENTTYPE pIndication)
{
	if (pIndication!=NULL)
	{
		/* Check type */
		if (pIndication->iType==Si_Type_indication)
		{
			/* Return action */
			return (((SI_INDICATIONELEMENT *)(pIndication))->iAction);
		}
	}

	/* Default value */
	return SI_SIGNAL_MEDIUM;
}


/*========================================================================
	Si_GetNextItem
======================================================================== */
pELEMENTTYPE Si_GetNextItem (pELEMENTTYPE pInfo, pELEMENTTYPE pItem)
{
    pELEMENTTYPE    pElem;

    if (pItem!=NULL)
    {
        /* Use pItem as starting point */
        pElem = pItem->pNextElement;
    }
    else
    {
        /* Use pInfo as starting point */
        pElem = Si_GetContent(pInfo);
    }

    return pElem;
}


/*========================================================================
	Si_GetClass
======================================================================== */
WCHAR *Si_GetClass (pELEMENTTYPE pItem)
{
	if (pItem!=NULL)
	{
		/* Check type */
		if (pItem->iType==Si_Type_item)
		{
			/* Get copy of Class */
			return CreateStringCopy (
				((SI_ITEMELEMENT *)(pItem))->pwchClass);
		}
	}

	return NULL;
}


/*========================================================================
	Si_GetItemText
======================================================================== */
WCHAR *Si_GetItemText (pDECSTR pDecStr, pELEMENTTYPE pItem)
{
	pELEMENTTYPE pContent=NULL;

	if (pItem!=NULL)
	{
		pContent=Si_GetContent(pItem);

		/* Content is #PCDATA */
		if (pContent!=NULL)
		{
			return XML_GetString (pContent,pDecStr);
		}
	}
	
	return NULL;
}








/*========================================================================

	Service Loading Functions

=========================================================================*/

/*========================================================================
	Sl_BuildStructure
======================================================================== */
pDECSTR Sl_BuildStructure (BYTE *pbData, BYTE *pbEnd, INT16 iCharset,
						   UINT8 iContentLevel, UINT8 iViewID)
{
	pWBXMLDECODESTR pDecStr=NULL;
	pELEMENTTYPE pStructure=NULL;
	BOOL finished;

	/* IS THIS PARAMETER NEEDED? (Error handling) */
	iViewID=iViewID;

	/* Create tWBXMLDECODESTR and init the attributes */
	pDecStr=WBXML_CreateWBXMLDecStr();

	if (pDecStr!=NULL)
	{
		/* Set the function pointers */
		pDecStr->DTD_CreateElement=Sl_CreateElement;
		pDecStr->DTD_DeleteElement=Sl_DeleteElement;
		pDecStr->DTD_ValidContent=Sl_ValidContent;
		pDecStr->DTD_GetContent=Sl_GetContent;
		pDecStr->DTD_AppSpecParseMod=Sl_AppSpecParseMod;
		pDecStr->DTD_StoreAttributeValue=Sl_StoreAttributeValue;
		pDecStr->DTD_GetAttributeValue=Sl_GetAttributeValue;
		pDecStr->DTD_LiteralAttributeToToken=Sl_LiteralAttributeToToken;
		pDecStr->DTD_LiteralTagToToken=Sl_LiteralTagToToken;
		pDecStr->DTD_CheckPublicID=Sl_CheckPublicID;

		/* Store data in the decode struct */
		pDecStr->bContentType='\x30';	/* text/vnd.wap.slc */
		pDecStr->iCharset=iCharset;
		pDecStr->iContentLevel=iContentLevel;
		pDecStr->pbCurByte=pbData;
		pDecStr->pbEnd=pbEnd;
		pDecStr->iDecodeResult=0;		
		pDecStr->pAppSpec=NULL;

		/* Parse prolog */
		if (WBXML_DecodeProlog(pDecStr))
		{
			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);

			/* Parse channel */
			WBXML_InitDecode(pDecStr, FALSE);
			pStructure = WBXML_Decode(pDecStr, &finished);

			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);
		}

		/* Check if any (fatal) errors or warnings */
		if ( pDecStr->iDecodeResult == 0 )
		{
			/* Store the structure under the application specific pointer
			   in the decode struct. */
			pDecStr->pAppSpec=pStructure;
		}
		else
		{
			/* Error - delete */
			Sl_DeleteElement(pDecStr,&pStructure);

			/* Delete decode struct */
			WBXML_DeleteWBXMLDecStr(&pDecStr);
		}
	}

	/* Return the decode struct */
	return pDecStr;
}


/*========================================================================
	Sl_DeleteStructure
======================================================================== */
void Sl_DeleteStructure (pDECSTR *ppDecStr)
{
	if (*ppDecStr!=NULL)
	{
		/* Get the top element */
		pELEMENTTYPE pStructure=(pELEMENTTYPE)(*ppDecStr)->pAppSpec;

		/* Delete structure */
		Sl_DeleteElement(*ppDecStr,&pStructure);
		(*ppDecStr)->pAppSpec=NULL;

		/* Delete decode struct */
		WBXML_DeleteWBXMLDecStr(ppDecStr);
	}
}


/*========================================================================
	Sl_GetSl
======================================================================== */
pELEMENTTYPE Sl_GetSl (pDECSTR pDecStr)
{
	if (pDecStr!=NULL)
	{
		return (pELEMENTTYPE)pDecStr->pAppSpec;
	}
	return NULL;
}


/*========================================================================
	Sl_GetHref
======================================================================== */
BYTE *Sl_GetHref (pELEMENTTYPE pSl)
{
	if (pSl!=NULL)
	{
		/* Check type */
		if (pSl->iType==Sl_Type_sl)
		{
			if (((SL_SLELEMENT *)(pSl))->pbHref!=NULL)
			{
				/* Get copy of href */
				return B_CopyByteString (((SL_SLELEMENT *)(pSl))->pbHref,-1);
			}
		}
	}

	return NULL;
}


/*========================================================================
	Sl_GetAction
======================================================================== */
UINT8 Sl_GetAction (pELEMENTTYPE pSl)
{
	if (pSl!=NULL)
	{
		/* Check type */
		if (pSl->iType==Sl_Type_sl)
		{
			/* Return action */
			return (((SL_SLELEMENT *)(pSl))->iAction);
		}
	}

	/* Default value */
	return SL_EXECUTE_LOW;
}










/*========================================================================

	Cache Operation Functions

=========================================================================*/

/*========================================================================
	Co_BuildStructure
======================================================================== */
pDECSTR Co_BuildStructure (BYTE *pbData, BYTE *pbEnd, INT16 iCharset, 
						   UINT8 iContentLevel, UINT8 iViewID)
{
	pWBXMLDECODESTR pDecStr=NULL;
	pELEMENTTYPE pStructure=NULL;
	BOOL finished;

	/* IS THIS PARAMETER NEEDED? (Error handling) */
	iViewID=iViewID;

	/* Create tWBXMLDECODESTR and init the attributes */
	pDecStr=WBXML_CreateWBXMLDecStr();

	if (pDecStr!=NULL)
	{
		/* Set the function pointers */
		pDecStr->DTD_CreateElement=Co_CreateElement;
		pDecStr->DTD_DeleteElement=Co_DeleteElement;
		pDecStr->DTD_ValidContent=Co_ValidContent;
		pDecStr->DTD_GetContent=Co_GetContent;
		pDecStr->DTD_AppSpecParseMod=Co_AppSpecParseMod;
		pDecStr->DTD_StoreAttributeValue=Co_StoreAttributeValue;
		pDecStr->DTD_GetAttributeValue=Co_GetAttributeValue;
		pDecStr->DTD_LiteralAttributeToToken=Co_LiteralAttributeToToken;
		pDecStr->DTD_LiteralTagToToken=Co_LiteralTagToToken;
		pDecStr->DTD_CheckPublicID=Co_CheckPublicID;

		/* Store data in the decode struct */
		pDecStr->bContentType='\x32';	/* text/vnd.wap.coc */
		pDecStr->iCharset=iCharset;
		pDecStr->iContentLevel=iContentLevel;
		pDecStr->pbCurByte=pbData;
		pDecStr->pbEnd=pbEnd;
		pDecStr->iDecodeResult=0;		
		pDecStr->pAppSpec=NULL;

		/* Parse prolog */
		if (WBXML_DecodeProlog(pDecStr))
		{
			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);

			/* Parse channel */
			WBXML_InitDecode(pDecStr, FALSE);
			pStructure = WBXML_Decode(pDecStr, &finished);

			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);
		}

		/* Check if any (fatal) errors or warnings */
		if ( pDecStr->iDecodeResult == 0 )
		{
			/* Store the structure under the application specific pointer
			   in the decode struct. */
			pDecStr->pAppSpec=pStructure;
		}
		else
		{
			/* Error - delete */
			Co_DeleteElement(pDecStr,&pStructure);

			/* Delete decode struct */
			WBXML_DeleteWBXMLDecStr(&pDecStr);
		}
	}

	/* Return the decode struct */
	return pDecStr;
}


/*========================================================================
	Co_DeleteStructure
======================================================================== */
void Co_DeleteStructure (pDECSTR *ppDecStr)
{
	if (*ppDecStr!=NULL)
	{
		/* Get the top element */
		pELEMENTTYPE pStructure=(pELEMENTTYPE)(*ppDecStr)->pAppSpec;

		/* Delete structure */
		Co_DeleteElement(*ppDecStr,&pStructure);
		(*ppDecStr)->pAppSpec=NULL;

		/* Delete decode struct */
		WBXML_DeleteWBXMLDecStr(ppDecStr);
	}
}


/*========================================================================
	Co_GetCo
======================================================================== */
pELEMENTTYPE Co_GetCo (pDECSTR pDecStr)
{
	if (pDecStr!=NULL)
	{
		return (pELEMENTTYPE)pDecStr->pAppSpec;
	}
	return NULL;
}


/*========================================================================
	Co_GetInvalidateURI
======================================================================== */

BYTE *Co_GetInvalidateURI (pELEMENTTYPE pElement, void* vsHeaderDef)
{
  pHEADERDEF pHeaderDef = NULL;
  pHEADERELEMENT pHeaderElement = NULL;
  BYTE*  pbContentLocation = NULL;
  BYTE* pbUrl;
  BYTE* pbTemp;
  UINT8 i;

  if (pElement!=NULL)
  {
  /* Check type */
    if (pElement->iType==Co_Type_invalidateobject)
    {
      if (((CO_INVOBJELEMENT *)(pElement))->pbURI!=NULL)
      {
        pbUrl = B_CopyByteString (((CO_INVOBJELEMENT *)(pElement))->pbURI,-1);
        if ( (vsHeaderDef!=NULL) && !(ME_Push_Check_if_InvalidateURI_Abs(pbUrl)))
        {
          pHeaderDef = (pHEADERDEF)vsHeaderDef;
          pHeaderElement = WSP_GetHeaderWK(1,Field_Content_Location,pHeaderDef->pHeadList);
          if(pHeaderElement != NULL)
            pbContentLocation = WSP_GetContentLocation( pHeaderElement );
          if (pbContentLocation != NULL){
#ifdef FEATURE_WAP
              i = strlen((char *)pbContentLocation);
#else
              i = strlen(pbContentLocation);
#endif //#ifdef FEATURE_WAP
#ifdef FEATURE_WAP
              pbTemp = NEWARRAY(BYTE, (i + strlen((char *)pbUrl)));
              strcpy((char *)pbTemp, (char *)pbContentLocation);
              strcpy((char *)(pbTemp + i),(char *) pbUrl);
#else
              pbTemp = NEWARRAY(BYTE, (i + strlen(pbUrl)));
              strcpy(pbTemp, pbContentLocation);
              strcpy(pbTemp + i, pbUrl);
#endif //#ifdef FEATURE_WAP

              DEALLOC(&pbUrl);
              return pbTemp;
          }
		}
        return pbUrl;
      }
    }
    else if (pElement->iType==Co_Type_invalidateservice)
    {
      if (((CO_INVSERVELEMENT *)(pElement))->pbURI!=NULL)
      {
         /* Get copy of href */
         return B_CopyByteString (((CO_INVSERVELEMENT *)(pElement))->pbURI,-1);
      }
    }
  }
  return NULL;
}


/*========================================================================
	Co_GetInvalidateType
======================================================================== */
UINT8 Co_GetInvalidateType (pELEMENTTYPE pElement)
{
	if (pElement!=NULL)
	{
		/* Check type */
		if (pElement->iType==Co_Type_invalidateobject)
		{
			return 1;
		}
		else if (pElement->iType==Co_Type_invalidateservice)
		{
			return 2;	
		}
	}

	/* Error */
	return 0;
}


/*========================================================================
	Co_GetNextInvElem
======================================================================== */
pELEMENTTYPE Co_GetNextInvElem (pELEMENTTYPE pCo, pELEMENTTYPE pElement)
{
    pELEMENTTYPE    pElem;

    if (pElement!=NULL)
    {
        /* Use pElement as starting point */
        pElem = pElement->pNextElement;
    }
    else
    {
        /* Use pCo as starting point */
        pElem = Co_GetContent(pCo);
    }

    return pElem;
}
