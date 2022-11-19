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

	WAP WTA Implementation Project

==========================================================================

	File: ChPars.c

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	990722  CEF		First version
		990815	JPR		Functionality added
		991105	JPR		Correction Ch_GetTitle and Ch_GetAbstract
		000204	JPR		Updated to support new WBXML-decoder
		000207	JPR		Corrections
		000210	JPR		Functions updated
		000223	JPR		Correction in Ch_IsResourceFresh
		000224	JPR		Function Ch_ValidateChannelContent added
		000630	JPR		Updated for WAP 1.2.1
		000814	JPR		Handling of PIs corrected
		001011	ASPN	Lower-case filenames

=========================================================================*/

/* Private include statements */

#include "chpars.h"
#include "wbxmldec.h"
#include "wtachtok.h"
#include "wtachelm.h"
#include "wtachdef.h"
#include "wtachdec.h"
#include "hdrutil.h"
#include "headdef.h"

/*========================================================================
	INTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	Ch_ValidateChannelContent
==========================================================================
	The function validates the decoded structure and checks that all
	REQUIRED attributes are specified, and all mandatory elements are
	included. If the validation is OK, TRUE is returned. Otherwise FALSE
	is returned.
	
	Input :	Structure of channel content (pELEMENTTYPE)
	Output:	BOOL
==========================================================================*/
BOOL Ch_ValidateChannelContent (pELEMENTTYPE pChannel)
{
	pELEMENTTYPE pContent=NULL;

	/* Check if channel exists */
	if (( pChannel != NULL ) && ( pChannel->iType == Ch_Type_channel ))
	{
		/* Maxspace ??? */

		/* ChannelId */
		if (((pCH_CHANNELELEMENT)(pChannel))->pwchChannelID == NULL )
		{
			return FALSE;
		}

		/* Get channel content. At least title MUST be defined - DTD */
		pContent=Ch_GetContent ( pChannel );

		/* Check if title exists */
		if (( pContent != NULL ) && ( pContent->iType == Ch_Type_title ))
		{
			pContent = pContent->pNextElement;

			/* Check if abstract exists */
			if (( pContent!= NULL ) && ( pContent->iType == Ch_Type_abstract ))
			{
				/* Step past abstract */
				pContent = pContent->pNextElement;
			}

			/* Check if all resources have their url defined */
			while ( pContent!=NULL )
			{
				if ( pContent->iType == Ch_Type_resource )
				{
					if ( ((pCH_RESOURCEELEMENT)(pContent))->pbHref == NULL )
					{
						/* No url */
						return FALSE;
					}
				}
				else
				{
					/* Not a resource */
					return FALSE;
				}

				/* Next */
				pContent = pContent->pNextElement;
			}

			/* Tests passed */
			return TRUE;
		}
	}

	return FALSE;
}


/*========================================================================
	EXTERNAL FUNCTIONS
=========================================================================*/


/*========================================================================
	Ch_BuildStructure
==========================================================================*/
pDECSTR Ch_BuildStructure (BYTE *pbData, BYTE *pbEnd, INT16 iCharset, 
						   UINT8 iContentLevel, UINT8 iViewID)
{
	pWBXMLDECODESTR pDecStr=NULL;
	pELEMENTTYPE pStructure=NULL;

	/* IS THIS PARAMETER NEEDED? (Error handling) */
	iViewID=iViewID;

	/* Create tWBXMLDECODESTR and init the attributes */
	pDecStr=WBXML_CreateWBXMLDecStr();

	if (pDecStr!=NULL)
	{
		/* Set the function pointers */
		pDecStr->DTD_CreateElement=Ch_CreateElement;
		pDecStr->DTD_DeleteElement=Ch_DeleteElement;
		pDecStr->DTD_AddContent=Ch_AddContent;
		pDecStr->DTD_GetContent=Ch_GetContent;
		pDecStr->DTD_AppSpecParseMod=Ch_AppSpecParseMod;
		pDecStr->DTD_StoreAttributeValue=Ch_StoreAttributeValue;
		pDecStr->DTD_GetAttributeValue=Ch_GetAttributeValue;
		pDecStr->DTD_LiteralAttributeToToken=Ch_LiteralAttributeToToken;
		pDecStr->DTD_LiteralTagToToken=Ch_LiteralTagToToken;
		pDecStr->DTD_CheckPublicID=Ch_CheckPublicID;

		/* Store data in the decode struct */
		pDecStr->bContentType='\x16';	/* text/vnd.wap.channelc */
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
			pStructure=WBXML_DecodeElement(pDecStr, NULL);

			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);
		}

		/* Check if any errors or warnings */
		if ( pDecStr->iDecodeResult == 0 )
		{
			/* Validate structure */
			if (Ch_ValidateChannelContent (pStructure))
			{
				/* Store the structure under the application specific pointer
				   in the decode struct. */
				pDecStr->pAppSpec=pStructure;

				/* Return the decode struct */
				return pDecStr;
			}
		}

		/* Error - delete */
		Ch_DeleteElement(pDecStr,&pStructure);

		/* Delete decode struct */
		WBXML_DeleteWBXMLDecStr(&pDecStr);
	}

	/* Error */
	return NULL;
}


/*========================================================================
	Ch_DeleteStructure
==========================================================================*/
void Ch_DeleteStructure (pDECSTR* ppDecStr)
{
	if (*ppDecStr!=NULL)
	{
		/* Get the top element */
		pELEMENTTYPE pStructure=(*ppDecStr)->pAppSpec;

		/* Delete structure */
		Ch_DeleteElement(*ppDecStr,&pStructure);
		(*ppDecStr)->pAppSpec=NULL;

		/* Delete decode struct */
		WBXML_DeleteWBXMLDecStr(ppDecStr);
	}
}


/*========================================================================
	Ch_GetAbstract
==========================================================================*/
WCHAR* Ch_GetAbstract (pDECSTR pDecStr)
{
	pELEMENTTYPE pContent=NULL;
	pELEMENTTYPE pText=NULL;

	if (pDecStr!=NULL)
	{
		pContent=pDecStr->pAppSpec;

		/* Check type */
		if ((pContent!=NULL)&&(pContent->iType==Ch_Type_channel))
		{
			pContent=Ch_GetContent (pContent);

			while (pContent!=NULL)
			{
				/* Check if abstract element */
				if (pContent->iType==Ch_Type_abstract)
				{
					/* Get text */
					pText=Ch_GetContent (pContent);

					/* Only #PCDATA allowed as content */
					if (pText!=NULL)
					{
						return XML_GetString (pText,pDecStr);
					}
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
	Ch_GetTitle
==========================================================================*/
WCHAR* Ch_GetTitle (pDECSTR pDecStr)
{
	pELEMENTTYPE pContent=NULL;
	pELEMENTTYPE pText=NULL;

	if (pDecStr!=NULL)
	{
		pContent=pDecStr->pAppSpec;

		/* Check type */
		if ((pContent!=NULL)&&(pContent->iType==Ch_Type_channel))
		{
			pContent=Ch_GetContent (pContent);

			while (pContent!=NULL)
			{
				/* Check if abstract element */
				if (pContent->iType==Ch_Type_title)
				{
					/* Get text */
					pText=Ch_GetContent (pContent);

					/* Only #PCDATA allowed as content */
					if (pText!=NULL)
					{
						return XML_GetString (pText,pDecStr);
					}
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
	Ch_Getuseraccessible
==========================================================================
Purpose:  The function returns the value of the useraccessible attribute.
======================================================================== */
BOOL Ch_Getuseraccessible (pDECSTR pDecStr)
{
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			return (((CH_CHANNELELEMENT*)(pChannel))->fUserAccessible);
		}
	}
	return FALSE;
}


/*========================================================================
	Ch_GetSuccessUrl
==========================================================================*/
BYTE* Ch_GetSuccessUrl (pDECSTR pDecStr)
{
	BYTE *pbCopy=NULL;
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			if (((CH_CHANNELELEMENT*)(pChannel))->pbSuccess!=NULL)
			{
				/* Get copy of success url */
				pbCopy=B_CopyByteString (
					((CH_CHANNELELEMENT*)(pChannel))->pbSuccess,-1);

				/* return copy */
				return pbCopy;
			}
		}
	}

	return NULL;
}


/*========================================================================
	Ch_GetFailureUrl
==========================================================================*/
BYTE* Ch_GetFailureUrl (pDECSTR pDecStr)
{
	BYTE *pbCopy=NULL;
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			if (((CH_CHANNELELEMENT*)(pChannel))->pbFailure!=NULL)
			{
				/* Get copy of failure url */
				pbCopy=B_CopyByteString (
					((CH_CHANNELELEMENT*)(pChannel))->pbFailure,-1);

				/* return copy */
				return pbCopy;
			}
		}
	}

	return NULL;
}


/*========================================================================
	Ch_GetBaseUrl
==========================================================================*/
BYTE* Ch_GetBaseUrl (pDECSTR pDecStr)
{
	BYTE *pbCopy=NULL;
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			if (((CH_CHANNELELEMENT*)(pChannel))->pbBase!=NULL)
			{
				/* Get copy of base url */
				pbCopy=B_CopyByteString (
					((CH_CHANNELELEMENT*)(pChannel))->pbBase,-1);

				/* return copy */
				return pbCopy;
			}
		}
	}

	return NULL;	
}


/*========================================================================
	Ch_GetEventID
==========================================================================*/
WCHAR* Ch_GetEventID (pDECSTR pDecStr)
{
	WCHAR *pwchCopy=NULL;
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			/* Get copy of EventID */
			pwchCopy=CreateStringCopy (
				((CH_CHANNELELEMENT*)(pChannel))->pwchEventID);

			/* return copy */
			return pwchCopy;
		}
	}

	return NULL;	
}

/*========================================================================
	Ch_Getchannelid
==========================================================================
Purpose:  The function returns a copy of the channelid in the channel. 
					NULL is returned if no channelid is present. The string MUST be 
					deleted after it is used.
======================================================================== */
WCHAR* Ch_Getchannelid (pDECSTR pDecStr)
{
	WCHAR *pwchCopy=NULL;
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			/* Get copy of EventID */
			pwchCopy=CreateStringCopy (
				((CH_CHANNELELEMENT*)(pChannel))->pwchChannelID);

			/* return copy */
			return pwchCopy;
		}
	}

	return NULL;	
}

/*========================================================================
	Ch_GetMaxSpace
==========================================================================*/
INT32 Ch_GetMaxSpace (pDECSTR pDecStr)
{
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			/* return MaxSpace */
			return (((CH_CHANNELELEMENT*)(pChannel))->iMaxSpace);
		}
	}

	return 0;
}


/*========================================================================
	Ch_CheckIfChExpired
==========================================================================
Purpose:  Check if the content has expired.
returns:  TRUE if content has expired or header corrupt. FALSE otherwise.
======================================================================== */
BOOL Ch_CheckIfChExpired (HEADERDEF* pContentHeader, UINT32 iTime)
{
	/* Field_Expires */
	pHEADERELEMENT pExpires=NULL;
	BOOL fError=FALSE;
	UINT32 iExpires=0;

	if (pContentHeader!=NULL)
	{
		/* Find Expires header */
		pExpires=WSP_GetHeaderWK (1, Field_Expires, pContentHeader->pHeadList);

		if (pExpires!=NULL)
		{
			/* Get expires value */
			iExpires=WSP_GetExpires (pExpires,&fError);

			if (fError)
			{
				/* Expires value error -> expired */
				return TRUE;
			}

			if (iTime>=iExpires)
			{
				/* Content expired */
				return TRUE;
			}
		}
	}

	return FALSE;
}


/*========================================================================
	Ch_GetNbrOfResources
==========================================================================*/
UINT32 Ch_GetNbrOfResources (pDECSTR pDecStr)
{
	UINT32 iCount=0;
	pELEMENTTYPE pContent=NULL;
	pELEMENTTYPE pChannel=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			/* Get channel content */
			pContent=Ch_GetContent (pChannel);

			/* Count all resources */
			while (pContent!=NULL)
			{
				if (pContent->iType==Ch_Type_resource)
				{
					iCount++;
				}

				/* Get next content */
				pContent=pContent->pNextElement;
			}
		}
	}

	return iCount;
}


/*========================================================================
	Ch_GetFirstResource
==========================================================================*/
pELEMENTTYPE Ch_GetFirstResource (pDECSTR pDecStr)
{
	pELEMENTTYPE pChannel=NULL;
	pELEMENTTYPE pContent=NULL;

	if (pDecStr!=NULL)
	{
		pChannel=pDecStr->pAppSpec;

		/* Check type */
		if ((pChannel!=NULL)&&(pChannel->iType==Ch_Type_channel))
		{
			/* Get channel content */
			pContent=Ch_GetContent (pChannel);

			/* Find first resource */
			while (pContent!=NULL)
			{
				if (pContent->iType==Ch_Type_resource)
				{
					return pContent;
				}

				/* Get next content */
				pContent=pContent->pNextElement;
			}
		}
	}

	return NULL;
}


/*========================================================================
	Ch_GetNextResource
==========================================================================*/
pELEMENTTYPE Ch_GetNextResource (pELEMENTTYPE pResource)
{
	if (pResource!=NULL)
	{
		/* Check type */
		if (pResource->iType==Ch_Type_resource)
		{
			/* Get next content */
			pResource=pResource->pNextElement;

			/* Find next resource */
			while (pResource!=NULL)
			{
				if (pResource->iType==Ch_Type_resource)
				{
					return pResource;
				}

				/* Get next content */
				pResource=pResource->pNextElement;
			}
		}
	}

	return NULL;
}


/*========================================================================
	Ch_GetResourcehref
==========================================================================*/
BYTE* Ch_GetResourcehref (pELEMENTTYPE pResource)
{
	BYTE *pbCopy=NULL;

	if (pResource!=NULL)
	{
		/* Check type */
		if (pResource->iType==Ch_Type_resource)
		{
			if (((CH_RESOURCEELEMENT*)(pResource))->pbHref!=NULL)
			{
				/* Get copy of resource url */
				pbCopy=B_CopyByteString (
					((CH_RESOURCEELEMENT*)(pResource))->pbHref,-1);

				/* return copy */
				return pbCopy;
			}
		}
	}

	return NULL;	
}


/*========================================================================
	Ch_GetResourceLastMod
==========================================================================
Purpose:  Returns zero if no lastMod is specified.

======================================================================== */
UINT32 Ch_GetResourceLastMod (pELEMENTTYPE pResource)
{
	if (pResource!=NULL)
	{
		/* Check type */
		if (pResource->iType==Ch_Type_resource)
		{
			/* Get last mod */
			return ((CH_RESOURCEELEMENT *)(pResource))->iLastMod;
		}
	}

	return 0;
}


/*========================================================================
	Ch_GetResourceMd5
==========================================================================*/
WCHAR* Ch_GetResourceMd5 (pELEMENTTYPE pResource)
{
	WCHAR *pwchCopy=NULL;

	if (pResource!=NULL)
	{
		/* Check type */
		if (pResource->iType==Ch_Type_resource)
		{
			/* Get copy of EventID */
			pwchCopy=CreateStringCopy (
				((CH_RESOURCEELEMENT *)(pResource))->pwchMd5);

			/* return copy */
			return pwchCopy;
		}
	}

	return NULL;
}


/*========================================================================
	Ch_GetResourceEtag
==========================================================================*/
BYTE* Ch_GetResourceEtag (pELEMENTTYPE pResource)
{
	BYTE *pbCopy=NULL;

	if (pResource!=NULL)
	{
		/* Check type */
		if (pResource->iType==Ch_Type_resource)
		{
			/* Get copy of Etag */
			if (((CH_RESOURCEELEMENT *)(pResource))->pbEtag!=NULL)
			{
				pbCopy=B_CopyByteString (((CH_RESOURCEELEMENT *)(pResource))->pbEtag,-1);
			}
		}
	}

	/* return copy */
	return pbCopy;
}


/*========================================================================
	Ch_IsResourceFresh
======================================================================== */
BOOL Ch_IsResourceFresh (pHEADERELEMENT pHeaderList, UINT32 iTime,
						 BOOL* pfCheckDone)
{
	pHEADERELEMENT pLastMod=NULL;
	BOOL fError=FALSE;
	UINT32 iLastMod=0;

	*pfCheckDone=FALSE;

	/* Find WSP-header */
	pLastMod=WSP_GetHeaderWK (1, Field_Last_Modified, pHeaderList);

	if (pLastMod!=NULL)
	{
		/* Get last mod value */
		iLastMod=WSP_GetLastModified (pLastMod,&fError);

		if (!fError)
		{
			*pfCheckDone=TRUE;

			/* If the stored resource (pHeaderList) is older than the
			   resource to be fetched (iTime), the resource must be 
			   updated. */
			if ( iTime <= iLastMod )
			{
				/* Resource is OK to keep */
				return TRUE;
			}	
		}
	}

	/* The resource must be fetched again */
	return FALSE;
}


/*========================================================================
	Ch_CheckResourceEtag
======================================================================== */
BOOL Ch_CheckResourceEtag (pHEADERELEMENT pHeaderList, BYTE *pbEtag, 
						   BOOL* pfCheckDone)
{
	pHEADERELEMENT pEtag=NULL;
	BYTE* pbHeadEtag=NULL;

	*pfCheckDone=FALSE;

	/* Find Etag header */
	pEtag=WSP_GetHeaderWK (1, Field_Etag, pHeaderList);

	if (pEtag!=NULL)
	{
		/* Get etag value */
		pbHeadEtag=WSP_GetEtag (pEtag);

		if ((pbHeadEtag!=NULL)&&(pbEtag!=NULL))
		{
			*pfCheckDone=TRUE;

			/* Compare */
			if (B_COMPARESTRING(pbEtag,pbHeadEtag)==0)
			{
				/* OK. return TRUE */
				DEALLOC(&pbEtag);
				return TRUE;
			}
		}
	}

	return FALSE;
}


/*========================================================================
	Ch_CheckResourceMd5
======================================================================== */
BOOL Ch_CheckResourceMd5 (pHEADERELEMENT pHeaderList, WCHAR* pchMd5,
						  BOOL* pfCheckDone)
{
	pHEADERELEMENT pMd5=NULL;
	BYTE* pbHeadMd5=NULL;
	BYTE* pbConvertedMd5=NULL;
	BOOL fError=FALSE;
	INT16 iNbrBytes=0;

	*pfCheckDone=FALSE;

	/* Find Md5 header */
	pMd5=WSP_GetHeaderWK (1, Field_Content_MD5, pHeaderList);

	if (pMd5!=NULL)
	{
		/* Get header md5-value */
		pbHeadMd5 = WSP_GetMd5 ( pMd5, &fError );

		if ( (pbHeadMd5!=NULL) && (pchMd5!=NULL) && (!fError) )
		{
			*pfCheckDone=TRUE;

			/* Convert from base-64 to bytes */
			pbConvertedMd5 = WSP_ConvertBase64ToByte (pchMd5,&iNbrBytes);
					
			/* Compare */
			if ( memcmp ( pbConvertedMd5 , pbHeadMd5, (16)*sizeof(BYTE) ) == 0 )
			{
				DEALLOC(&pbConvertedMd5);
				return TRUE;
			}

			DEALLOC(&pbConvertedMd5);
		}
	}

	return FALSE;
}


