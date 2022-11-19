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

	WAP Push Implementation Project

==========================================================================

	File: push.c

	Description:

	Author: Martin Ranstorp, Kent Olsson AU-System AB

	Revision history:
  	Date    Rev  Sign  Comment
  	990920  PA1  MRP   First version.
	991206  PA2  MRP   Updated. Working in WAE test environment.
	000118	PA3  MRP   Updated after validation inspection.
	000124	PA4  MRP   Updated after code inspection.
	000208	PA5	 JPR   Function ME_Push_DateTime2Int moved to PUSHElm.c
	000214  PA6  MRP   Copyright added.
	000316	PA7	 MRP   Push API changed.
	000918  PA8  MRP   Updated configuration handling (GWC 4.x).
	001114	PA9	 KEOL  Uppdated after change request regarding handling of GetSI and GetSL (reply with initiators URI as well) 	

------------------------------------------------------------------------*/

/* Private include statements */

#include "push.h"
#include "url.h"
#include "wbxmldef.h"
#include "wbxmldec.h"
#include "wbxmlelm.h"
#include "headdef.h"
#include "wae_cmmn.h"
#include "header.h"
#include "wipdb.h"
#include "dbfunct.h"
#include "aapipush.h"


#define EMPTY     0x0
#define MAX_TID   250


/*========================================================================
	INTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	EXTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	Functions called from the process Push_ME
=========================================================================*/

/*========================================================================
	ME_Push_GetConnection
==========================================================================
Purpose: Check and get first correct address and if ok request connection
Params:  pvAddressList
Return:  1 if ok, else 0
=========================================================================*/
UINT8 ME_Push_GetConnection( void* pvAddressList )
{
ADDRESS*	pAddressList=NULL;
ADDRESS*	pTemp=NULL;
ADDRESS*	pTemp2=NULL;
BYTE*		pbAddress=NULL;
INT16		itmp2;
INT32		itmp1;
UINT8		iLen=0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_GetConnection");
#endif

	if (pvAddressList!=NULL)
	{
		pAddressList=(ADDRESS*)pvAddressList;
		pTemp=pAddressList;
		pbAddress=pTemp->pbAddress;
		if (pbAddress!=NULL)
		{
			itmp1=pTemp->iPortNumber;
			itmp2=pTemp->iBearerType;
			iLen=pTemp->uiAddressLength;
			if (itmp1==-1)
			{
				itmp1=MODE_ANY;
			}
			if (pTemp->iBearerType==-1)
			{
				itmp2=BEARER_ANY;
			}
#ifdef FEATURE_WAP
			PUSHa_requestConnection(1,(UINT16)itmp1,(UINT8)itmp2,(const CHAR *)pbAddress,iLen);
#else
            PUSHa_requestConnection(1,(UINT16)itmp1,(UINT8)itmp2,pbAddress,iLen);
#endif //FEATURE_WAP

			while (pTemp!=NULL)
			{		
				pTemp2 = pTemp;
				pTemp=pTemp->pNext;
				/*DEALLOC(&(pTemp2->pbAddress));*/
				DEALLOC(&pTemp2);
			}		
			return 1;
		}
	}
	return 0;

} /* end of ME_Push_GetConnection */


/*========================================================================
	ME_Push_GetFreeIdNumber
==========================================================================
Purpose: Get free IdNumber to use
Params:  pvList (any list)
Return:  UINT16 IdNumber, 0 means failure
=========================================================================*/
UINT16 ME_Push_GetFreeIdNumber(void* pvList)
{
void*				pvListElement = NULL;
UINT16				iId = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_Push_GetFreeIdNumber");
#endif

    for (iId=1; iId < (UINT16)-1; iId++)
	{
		pvListElement = SDL_SearchListElement( pvList, iId, NULL );
		if( pvListElement == NULL )
		{
			return iId;
		}
	}

	return 0;
} /* end of ME_Push_GetFreeIdNumber */


/*========================================================================
	ME_Free_PushContentStruct
==========================================================================
Purpose: Free the PushContent struct (except the Url's)
Params:  vsPushContentStruct
Return:  TRUE
=========================================================================*/
void ME_Free_PushContentStruct(void* pvPushContentStruct)
{

	PUSHCONTENTSTRUCT* pPushContentStruct=NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End Free PushContentStruct");
#endif
	
	if (pvPushContentStruct != NULL)
	{
		pPushContentStruct = (PUSHCONTENTSTRUCT*)pvPushContentStruct;
		if (pPushContentStruct != NULL)
		{	
			if ( pPushContentStruct->iType == SI )
			{
				DEALLOC(&((pPushContentStruct)->content.pSIContentStruct->pwchSIid));
				DEALLOC(&((pPushContentStruct)->content.pSIContentStruct->pwchMessage));
				DEALLOC(&((pPushContentStruct)->content.pSIContentStruct->pbUrl) );
				DEALLOC(&((pPushContentStruct)->content.pSIContentStruct->pbInitUri) );
				DEALLOC(&((pPushContentStruct)->content.pSIContentStruct));
			}
			else
			{
				DEALLOC(&((pPushContentStruct)->content.pSLContentStruct->pbUrl) );
				DEALLOC(&((pPushContentStruct)->content.pSLContentStruct->pbInitUri) );
				DEALLOC(&((pPushContentStruct)->content.pSLContentStruct));
			}
			DEALLOC(&pPushContentStruct);
		}
	}

} /* end of ME_Free_PushContentStruct */


/*========================================================================
	Push_List functions, each element stores information (of PUSHLISTSTRUCT
	type) about a push stored in the Push Buffer. 
=========================================================================*/

/*========================================================================
	ME_Push_List_Add
==========================================================================
Purpose: Add a new element to the Push List
Params:  pvPushList, pvPushListStruct
Return:  1 if ok, else 0
=========================================================================*/
UINT8 ME_Push_List_Add( void* pvPushList,  void* pvPushListStruct )
{
UINT16 iId=0;
PUSHLISTSTRUCT* pListStruct=NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_List_Add");
#endif

	pListStruct = (PUSHLISTSTRUCT*)pvPushListStruct;
	iId=ME_Push_GetFreeIdNumber(pvPushList);
	if( (pvPushList != NULL) && (iId != 0) )
	{
		if( pvPushListStruct != NULL )
		{
			pListStruct->iIdNumber = iId;
			return SDL_AddElementLast( pvPushList,iId, (void*)pListStruct );
		}
	}
	return 0;

} /* end of ME_Push_List_Add */

/*========================================================================
	ME_Push_List_Delete
==========================================================================
Purpose: Deletes an element from the Push List
Params:  pvPushList, pvPushListStruct
Return:  
=========================================================================*/
void ME_Push_List_Delete( void* pvPushList, void* pvPushListStruct )
{
PUSHLISTSTRUCT*		pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_List_Delete");
#endif
	
	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement(pvPushList,NULL);
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent(pvListElement);
			if( pvElementContent != NULL )
			{
				pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
				if ( pPushListStruct == pvPushListStruct )
				{
					SDL_DeleteListElement(pvPushList,pvListElement);
                    OSConnectorFree(((PUSHLISTSTRUCT*)pPushListStruct)->pwchPushId);
                    OSConnectorFree(pPushListStruct);
					break;
				}
			}	
			pvListElement = SDL_GetNextListElement(pvPushList,pvListElement);
		}
	}

} /* end of ME_Push_List_Delete */


/*========================================================================
	ME_Push_List_Sort
==========================================================================
Purpose: Sort the elements in the Push List
         SLs are sorted first in list and then SIs. Oldest first (not
		 changing incoming order (apart from SLs that is put first in list)).
Params:  pvPushList 
Return:  Sorted PushList
=========================================================================*/
void* ME_Push_List_Sort(void* pvPushList)
{
PUSHLISTSTRUCT*		pPushListStruct = NULL;
PUSHLISTSTRUCT*		pPushListStructTemp = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;
void*				pvOldListElementTemp = NULL;
void*				pvListElementTemp = NULL;
void*				pvElementContentTemp = NULL;
void*				pPushListTemp = NULL;
UINT8				iRes;
void*				pvTemp;
BOOL				bFirst = TRUE;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_List_Sort");
#endif

if ( SDL_GetCardinal(pvPushList)<=1 )
   return pvPushList;

/* Create Temporary PushList */
pPushListTemp = SDL_NewList( 0 );

	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvPushList, NULL );
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
			{
				pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
				pvListElementTemp = SDL_GetNextListElement( pPushListTemp, NULL );
				pvOldListElementTemp = NULL;
				while (pvListElementTemp != NULL) /*  Sort into new list */
				{
					pvElementContentTemp = SDL_GetListElementContent( pvListElementTemp );
					if( pvElementContentTemp != NULL )
					{
						pPushListStructTemp = (PUSHLISTSTRUCT*)pvElementContentTemp;
							/* Check where to put it in temp list */
							if ( pPushListStruct->iPushType > pPushListStructTemp->iPushType )
							{	/* SI:s after SL */
								iRes = SDL_AddElementAfter(pPushListTemp, pPushListStruct->iIdNumber , (void*)pPushListStruct, pvOldListElementTemp );
								break;
							}
							else   /* Same or less Push Type */
                            {
								if ( pPushListStruct->iPushType == SI)
                                {
									if ( pPushListStructTemp->iPushType == SI )
									{										
										if ( pPushListStruct->iCreated < pPushListStructTemp->iCreated)
										{
											iRes = SDL_AddElementAfter(pPushListTemp, pPushListStruct->iIdNumber, (void*)pPushListStruct, pvOldListElementTemp );
											break;
										}
                                        
                                    }
                                }
                            }
					}
					/* Get next temp list element */
					pvOldListElementTemp = pvListElementTemp;
					pvListElementTemp = SDL_GetNextListElement( pPushListTemp, pvListElementTemp );
					if ( pvListElementTemp == NULL )  
					{	/* Put it last in temp list */
						iRes = SDL_AddElementLast(pPushListTemp, pPushListStruct->iIdNumber, (void*)pPushListStruct);
					}
				} /* end inner loop	*/
				if (bFirst) 
				{
					iRes = SDL_AddElementFirst(pPushListTemp, pPushListStruct->iIdNumber, (void*)pPushListStruct);
					bFirst = FALSE;
				}
			}
			pvTemp = SDL_GetNextListElement( pvPushList, pvListElement );
			pvListElement = pvTemp;
		} /* end outer loop	*/
	}
	SDL_DeleteList(&pvPushList);
	return pPushListTemp;

} /* end of ME_Push_List_Sort */

/*========================================================================
	ME_CompareWString
==========================================================================
Purpose: Compares two WCHAR strings
Params:  pwch1, pwch2
Return:  Boolean
=========================================================================*/
BOOL ME_CompareWString(WCHAR* pwch1, WCHAR* pwch2)
{
	UINT32 istrlength;

	if ( (pwch1!=NULL) && (pwch2!=NULL) )
	{
		istrlength = STRINGLENGTH(pwch1);
		if (istrlength == (STRINGLENGTH(pwch2))) {
			if (COMPARESTRINGN(pwch1, pwch2, istrlength) == 0)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;

} /* end of ME_CompareWString */

/*========================================================================
	ME_Push_Get_First_New( void* pvPushList )
==========================================================================
Purpose: Gets first new Push in Push list
Params:  pvPushList
Return:  pPushListStruct (NULL if not any new in list)
=========================================================================*/
void* ME_Push_Get_First_New( void* pvPushList )
{

PUSHLISTSTRUCT*		pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_Get_First_New");
#endif

	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvPushList, NULL );
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
			{
				pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
				if ( pPushListStruct->iStatus < PUSH_ACTIVE )
					return (void*)pPushListStruct;					
			}
			pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
		}	
	}
	return NULL;

} /* end of ME_Push_Get_First_New */

/*========================================================================
	ME_Push_SI_Expired
==========================================================================
Purpose: Check if SI is expired or not. If pvPushListStruct is NULL, then
		 the entire list is scanned to see if any SI in list is expired.
Params:  pvPushListStruct
Return:  Boolean 
=========================================================================*/
BOOL ME_Push_SI_Expired(void* pvPushList, void* pvPushListStruct)
{
UINT32 iTime=0;
PUSHLISTSTRUCT* pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_SI_Expired");
#endif

	pPushListStruct = (PUSHLISTSTRUCT*)pvPushListStruct;
	if( pPushListStruct != NULL )
	{
		iTime = CLNTa_currentTime ();     
		if ((pPushListStruct->iExpires < iTime) && 
			(pPushListStruct->iExpires != 0))    
				return TRUE;
	}
	else 
	{
		if (pvPushList!=NULL)
		{
			pvListElement = SDL_GetNextListElement( pvPushList, NULL );
			while ( pvListElement != NULL )
			{
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL )
				{
					pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
					if ((pPushListStruct->iExpires < iTime) && 
						(pPushListStruct->iExpires != 0))    
						return TRUE;				
				}
				pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
			}		
		}
	} 
 
	return FALSE;
} /* end of ME_Push_SI_Expired */

/*========================================================================
	ME_Push_Find_Id
==========================================================================
Purpose: Find push with PushId in Push List 
Params:  pvPushId, pvPushList
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_Find_Id(WCHAR* pwchId, void* pvPushList)
{

PUSHLISTSTRUCT*		pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_Find_Id");
#endif

	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvPushList, NULL );
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
			{
				pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
				if ( ME_CompareWString(pPushListStruct->pwchPushId, pwchId) )
					return (void*)pPushListStruct;					
			}
			pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
		}	
	}
	return NULL;


} /* end of ME_Push_Find_Id */

/*========================================================================
	ME_Push_Find_OldId
==========================================================================
Purpose: Find old push with PushId in Push List 
Params:  pvPushId, pvPushList, iLLPushId
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_Find_OldId(WCHAR* pwchId, void* pvPushList, UINT32 iLLPushId)
{

PUSHLISTSTRUCT*		pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_Find_OldId");
#endif

	if (pwchId == NULL)
		return NULL;
	else
	{
		if( pvPushList != NULL )
		{
			pvListElement = SDL_GetNextListElement( pvPushList, NULL );
			while ( pvListElement != NULL )
			{
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL )
				{
					pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
					if ( ME_CompareWString(pPushListStruct->pwchPushId, pwchId) )
					{
						if (pPushListStruct->iLLPushId != iLLPushId)
							return (void*)pPushListStruct;					
					}
				}
				pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
			}	
		}
		return NULL;
	}

} /* end of ME_Push_Find_OldId */

/*========================================================================
	ME_Push_FindWithIdNumber
==========================================================================
Purpose: Find push with IdNumber in Push List 
Params:  pvPushList, iIdNumber
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_FindWithIdNumber(void* pvPushList, UINT16 iId)
{

PUSHLISTSTRUCT*		pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_Push_FindWithIdNumber");
#endif

	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvPushList, NULL );
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
			{
				pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
				if ( pPushListStruct->iIdNumber == iId )
				{
						return (void*)pPushListStruct;					
				}
			}
			pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
		}
	}
	return NULL;
} /* end of ME_Push_FindWithIdNumber */


/*========================================================================
	ME_Push_Get_Last( void* pvPushList )
==========================================================================
Purpose: Retrieve last Push in Push list
Params:  pvPushList
Return:  pPushListStruct
=========================================================================*/
void* ME_Push_Get_Last( void* pvPushList )
{

PUSHLISTSTRUCT*		pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvListElementLast = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_Get_Last");
#endif

	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvPushList, NULL );
		while ( pvListElement != NULL )
		{
			pvListElementLast = pvListElement;
			pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
		}

		pvElementContent = SDL_GetListElementContent( pvListElementLast );
		if( pvElementContent != NULL )
		{
			pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
			return (void*)pPushListStruct;					
		}	
	}
	return NULL;

} /* end of ME_Push_Get_Last */


/*========================================================================
	ME_Push_BuildContent
==========================================================================
Purpose: Call parsing functions and build content for an SI or an SL.
Params:  void* pPushBody, UINT iBodyDataLen, iType
Return:  pPushContentStruct
=========================================================================*/
void* ME_Push_BuildContent(void* pBodyData, UINT32 iBodyDataLen, void* vsHeaderDef, 
						   UINT8 iPortType, UINT8 iViewID)
{

	PUSHCONTENTSTRUCT* pPushContent = NULL;
	SLCONTENTSTRUCT* pSLContent = NULL;
	SICONTENTSTRUCT* pSIContent = NULL;
	BYTE* pSLTemp = NULL;
	BYTE* pSITemp = NULL;
	pELEMENTTYPE pElem,pSi,pSl = NULL;
	BYTE* pbBody = NULL;
	BYTE* pbEnd = NULL;
	pHEADERDEF pHeaderDef = NULL;
	UINT8 iType;
	BOOL fError = FALSE;
	pHEADERELEMENT pElement = NULL;
	WSPPARAMETERS* pParameters = NULL;
	pDECSTR pStruct = NULL;
	INT16 iCharset = 0;
	UINT8 iLevel = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("ME_Push_BuildContent");
#endif

	pHeaderDef=(pHEADERDEF)vsHeaderDef;
	pbBody=(BYTE*)pBodyData;
	pbEnd=pbBody+iBodyDataLen;

	pElement=WSP_GetHeaderWK(1,Field_Content_Type,pHeaderDef->pHeadList);
	iType=(UINT8)WSP_GetContentTypeByte(pElement,&fError,&pParameters);
	if (fError)
	{
		return NULL;
	}
	if (pParameters != NULL) 
	{
		iCharset = pParameters->iCharset;
		iLevel = pParameters->iLevel;
		WSP_DeleteParameters(&pParameters);
	}
	switch(iType)
	{
	case 46:
		iType = SI;
		break;
	case 48:
		iType = SL;
		break;
	default:
		return NULL;
	}

	pPushContent = NEWSTRUCT(PUSHCONTENTSTRUCT);
	if (pPushContent==NULL)
	{
		return NULL;
	}
	
	switch (iType)
	{
	case SI:
			/* Parse the SI */
			pStruct=Si_BuildStructure (pbBody,pbEnd,iCharset,iLevel,iViewID);
			if (pStruct != NULL)
			{
				pSi=Si_GetSi (pStruct);
				pElem=Si_GetIndication(pSi);
				if (pElem != NULL)
				{
					pSIContent = NEWSTRUCT(SICONTENTSTRUCT);
					if (pSIContent==NULL)
					{
						return NULL;
					}
					pPushContent->iType = SI;
					pPushContent->iId = 0;
					pPushContent->iPortType = iPortType;
					pSIContent->iStatus = PUSH_NEW;
					pSIContent->pbUrl=Si_GetHref(pElem);
					pSIContent->pwchMessage=Si_GetIndicationText(pStruct,pElem);
					pSIContent->pwchSIid=Si_GetSiId(pElem);
					pSIContent->iAction=Si_GetAction(pElem);
					pSITemp = (BYTE*)ME_Push_GetXInitiatorURI(vsHeaderDef);
					if (pSITemp != NULL)
						pSIContent->pbInitUri = B_CopyByteString (pSITemp, -1);
					else
						pSIContent->pbInitUri = NULL;
					if (pSIContent->pwchSIid == NULL)
					{
						if (pSIContent->iAction == SI_DELETE)
						{
							return NULL;
						}
						if (pSIContent->pbUrl != NULL)
						{
							pSIContent->pwchSIid = wip_byte2wchar(pSIContent->pbUrl);
						}
					}
					pSIContent->iCreated=Si_GetCreated(pElem);
					pSIContent->iExpires=Si_GetExpires(pElem);
					pPushContent->content.pSIContentStruct=pSIContent;
					Si_DeleteStructure (&pStruct);
					return (void*)pPushContent;
				}
			}
			break;
	case SL:
			/* Parse the SL */
			pStruct=Sl_BuildStructure(pbBody,pbEnd,iCharset,iLevel,iViewID);
			if (pStruct != NULL)
			{
				pSLContent = NEWSTRUCT(SLCONTENTSTRUCT);
				if (pSLContent==NULL)
				{
					return NULL;
				}
				pSl=Sl_GetSl(pStruct);
				pSLContent->pbUrl=Sl_GetHref(pSl);
				if (pSLContent->pbUrl != NULL)
				{
					pPushContent->iType = SL;
					pPushContent->iId = 0;
					pPushContent->iPortType = iPortType;
					pSLContent->iStatus = PUSH_NEW;
					pSLContent->iAction=Sl_GetAction(pSl);
					pSLTemp = (BYTE*)ME_Push_GetXInitiatorURI(vsHeaderDef);
					if (pSLTemp != NULL)
						pSLContent->pbInitUri = B_CopyByteString (pSLTemp, -1);
					else
						pSLContent->pbInitUri = NULL;
					pPushContent->content.pSLContentStruct = pSLContent;
					Sl_DeleteStructure (&pStruct);
					return (void*)pPushContent;
				}
			}
			break;
	}
	DEALLOC(&pPushContent);
	return NULL;
} /* end of ME_Push_BuildContent */


/*========================================================================
	ME_Push_GetNextPPListElem
==========================================================================
Purpose: Gets next postponed (PP) Push in Push list. If pvPushListStruct
		 equals NULL, the first PP push in list is retrieved. If NULL is
		 returned, no more PP pushes exist in list.
Params:  pvPushList, pvPushListStruct
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_GetNextPPListElem( void* pvPushList, void* pvPushListStruct, UINT8 iType )
{
PUSHLISTSTRUCT*		pPushListStruct=NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_GetNextPPListElem");
#endif

	pvPushListStruct = (PUSHLISTSTRUCT*)pvPushListStruct;
	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvPushList, NULL );
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
			{
				pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
				if ( pvPushListStruct == NULL )  /* NULL case */
				{
					while ( pvListElement != NULL )
					{
						pvElementContent = SDL_GetListElementContent( pvListElement );
						if( pvElementContent != NULL )
						{
							pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
							if (pPushListStruct->iPushType == iType)
								if ( (pPushListStruct->iStatus == PUSH_POSTPONED) || (pPushListStruct->iStatus == PUSH_POSTPONED_READ))
									return (void*)pPushListStruct;					
						}
						pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
					}							
				}
				if ( pPushListStruct == pvPushListStruct)  /* found the start element */
				{
					pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
					while ( pvListElement != NULL )
					{
						pvElementContent = SDL_GetListElementContent( pvListElement );
						if( pvElementContent != NULL )
						{
							pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
							if (pPushListStruct->iPushType == iType)
								if ( (pPushListStruct->iStatus == PUSH_POSTPONED) || (pPushListStruct->iStatus == PUSH_POSTPONED_READ))
									return (void*)pPushListStruct;					
						}
						pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
					}							

				}
			}	
			if ( pvListElement!=NULL)
			{
				pvListElement = SDL_GetNextListElement( pvPushList, pvListElement );
			}
		}	
	}
	return NULL;

} /* end of ME_Push_GetNextPPListElem */

/*========================================================================
	ME_Push_Create_Elem
==========================================================================
Purpose: Creates a pPushListStruct element from a pPushContentStruct.
Params:  pvPushContentStruct
Return:  pPushListStruct 
=========================================================================*/
void* ME_Push_Create_Elem( void* pvPushContentStruct )
{
PUSHLISTSTRUCT*		pPushListStruct=NULL;
PUSHCONTENTSTRUCT*  pPushContentStruct=NULL;
UINT16				iLen = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_GetNextPPListElem");
#endif
	
	if( pvPushContentStruct != NULL )
	{
		pPushListStruct=NEWSTRUCT(PUSHLISTSTRUCT);
		if (pPushListStruct==NULL)
		{
			return NULL;
		}
		pPushContentStruct = (PUSHCONTENTSTRUCT*)pvPushContentStruct;
		pPushListStruct->iPushType = pPushContentStruct->iType;
		pPushListStruct->iLLPushId = pPushContentStruct->iId;
		if (pPushContentStruct->iType == SI)
		{
			pPushListStruct->iAction = pPushContentStruct->content.pSIContentStruct->iAction;
			pPushListStruct->iCreated = pPushContentStruct->content.pSIContentStruct->iCreated;
			pPushListStruct->iExpires = pPushContentStruct->content.pSIContentStruct->iExpires;
			pPushListStruct->iStatus = pPushContentStruct->content.pSIContentStruct->iStatus;
			if (pPushContentStruct->content.pSIContentStruct->pwchSIid != NULL)
			{
				iLen=STRINGLENGTH(pPushContentStruct->content.pSIContentStruct->pwchSIid);
				pPushListStruct->pwchPushId = NEWARRAY(WCHAR,iLen+1);
				if (pPushListStruct->pwchPushId==NULL)
				{
					return NULL;
				}
				COPYSTRINGN(pPushListStruct->pwchPushId,pPushContentStruct->content.pSIContentStruct->pwchSIid,iLen);
				/* Set termination char */
				pPushListStruct->pwchPushId[iLen]=0;
				}
			else
				pPushListStruct->pwchPushId = NULL;
		}
		else /* type is Service Loading */
		{
			pPushListStruct->iAction = pPushContentStruct->content.pSLContentStruct->iAction;
			pPushListStruct->iStatus = pPushContentStruct->content.pSLContentStruct->iStatus;
			pPushListStruct->pwchPushId = wip_byte2wchar(pPushContentStruct->content.pSLContentStruct->pbUrl);
			pPushListStruct->iExpires = 0;
			pPushListStruct->iCreated = 0;
		}
		return (void*)pPushListStruct;
	}
	return NULL;

} /* end of ME_Push_Create_Elem */

/*========================================================================
	ME_Push_Remove_List
==========================================================================
Purpose: Removes the entire Push List and deallocates struct data
Params:  pvPushList
Return:  -
=========================================================================*/
void ME_Push_Remove_List( void* pvPushList )
{

PUSHLISTSTRUCT*		pPushListStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_Remove_List");
#endif
	
	if( pvPushList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvPushList, NULL );
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
			{
				pPushListStruct = (PUSHLISTSTRUCT*)pvElementContent;
				SDL_DeleteListElement (pvPushList, pvListElement); 
				DEALLOC (&(pPushListStruct->pwchPushId)); 
				DEALLOC (&pPushListStruct);  
			}
			pvListElement = SDL_GetNextListElement(pvPushList,NULL );
		}
		SDL_DeleteList(&pvPushList); 
	}

} /* end of ME_Push_Remove_List */

/*========================================================================
	ME_Push_ParseHeader
==========================================================================
Purpose: Parse the Push Header. Returns a pHEADERDEF
Params:  pvPushHeader, iHeaderLength
Return:  pHEADERDEF
=========================================================================*/
void* ME_Push_ParseHeader( void* vsPushHeader, UINT32 iLen )
{
	pHEADERDEF pHeaderDef = NULL;
	BYTE* pbPushHeader = (BYTE*)vsPushHeader; 
	BOOL fError = FALSE;

	/* Preparse header */
	pHeaderDef=WSP_PreParseHeaders(pbPushHeader,iLen,&fError);
	
	if ((fError)||(pHeaderDef==NULL))
	{
		/* Error in pre-parse - remove all data and return NULL */
		WSP_EmptyHeaderList(pHeaderDef);
		pHeaderDef=NULL;
		return NULL;
	}
	return (void*)pHeaderDef;
} /* end ME_Push_ParseHeader */

/*========================================================================
	ME_Push_GetPushFlag
==========================================================================
Purpose: Retrieves the Push Flag from the Push Header.
Params:  vsHeaderDef
Return:  UINT8
=========================================================================*/
UINT8 ME_Push_GetPushFlag( void* vsHeaderDef )
{
	pHEADERDEF pHeaderDef = NULL;
	pHEADERELEMENT pElement = NULL;

	if (vsHeaderDef!=NULL)
	{
		pHeaderDef = (pHEADERDEF)vsHeaderDef;
		pElement=WSP_GetHeaderWK(1,Field_Push_Flag,pHeaderDef->pHeadList);
		return WSP_GetPushFlag(pElement);
	}
	return 0;

} /* end ME_Push_GetPushFlag */

/*========================================================================
	ME_Push_GetXInitiatorURI
==========================================================================
Purpose: Retrieves the X-Initiator-URI from the Push Header.
Params:  vsHeaderDef
Return:  Url (BYTE*)
=========================================================================*/
void* ME_Push_GetXInitiatorURI( void* vsHeaderDef )
{
	pHEADERDEF pHeaderDef = NULL;
	pHEADERELEMENT pElement = NULL;
	BYTE* pbUrl = NULL;

	if (vsHeaderDef!=NULL)
	{
		pHeaderDef = (pHEADERDEF)vsHeaderDef;
		pElement = WSP_GetHeaderWK(1,Field_X_Wap_Initiator_URI,pHeaderDef->pHeadList);
		pbUrl = WSP_GetXInitiatorURI(pElement);
		return (void*)pbUrl;
	}
	return NULL;

} /* end ME_Push_GetXInitiatorURI */

/*========================================================================
	ME_Push_GetXContentURI
==========================================================================
Purpose: Retrieves the X-Content-URI from the Push Header.
Params:  vsHeaderDef
Return:  Url (BYTE*)
=========================================================================*/
void* ME_Push_GetXContentURI( void* vsHeaderDef )
{
	pHEADERDEF pHeaderDef = NULL;
	pHEADERELEMENT pElement = NULL;
	BYTE* pbUrl = NULL;

	if (vsHeaderDef!=NULL)
	{
		pHeaderDef = (pHEADERDEF)vsHeaderDef;
		pElement = WSP_GetHeaderWK(1,Field_X_Wap_Content_URI,pHeaderDef->pHeadList);
		pbUrl = WSP_GetXContentURI(pElement);
		return (void*)pbUrl;
	}
	return NULL;

} /* end ME_Push_GetXContentURI */

/*========================================================================
	ME_Push_GetXAppId
==========================================================================
Purpose: Retrieves the X-Application-Id from the Push Header.
Params:  vsHeaderDef
Return:  TRUE if X-WAP-Applicatin-Id was provided in the header , FALSE otherwise
=========================================================================*/
BOOL ME_Push_GetXAppId( void* vsHeaderDef, long int* iPortType)
{
/*
	urn:x-wap-application:*         0x00       ;any app
    urn:x-wap-application:push.sia  0x01       ;WAP Push SIA
    urn:x-wap-application:wml.ua    0x02       ;WML User Agent
	urn:x-wap-application:wta.ua    0x03       ;WTA User Agent
    */

	pHEADERDEF pHeaderDef = NULL;
	pHEADERELEMENT pElement = NULL;
	int iAppId;
	BYTE* pbAppId = NULL;

	pHeaderDef = (pHEADERDEF)vsHeaderDef;
	pElement = WSP_GetHeaderWK(1,Field_X_Wap_Application_Id,pHeaderDef->pHeadList);
	/* Check if X-WAP-Application-Id was provided in the message*/
	if (pElement != NULL)     
	{
		/* WSP_GetPushHeaderAppId was succesfull*/
		if (WSP_GetPushHeaderAppId (pElement, &pbAppId, &iAppId))
		{
			/* Don't support if X-WAP-Application-Id is set to an AbsolutUri, discard Push message*/
			if (pbAppId != NULL)
				return FALSE;
			else
			{
				/* check if it is a valid id */
				if (iAppId<4)
				{
					if (iAppId == 2)
						*iPortType = 1;
					else if (iAppId == 3)
						*iPortType = 2;
					else if (iAppId == 1)
						*iPortType = 1;      /*PortType does not matter since it will never be used when application push.sia*/
					else
						*iPortType = 0;		
					return TRUE;
				}
				else
				{
					*iPortType = 1;
					return TRUE; /* If an App Id that we don't support use WML UA */
				}
			}
			}
		/* WSP_GetPushHeaderAppId was not succesfull, discard Push message  */
			else
				return FALSE;
		}
	/* If X-WAP-Application-Id was not provided, don't change iPortType*/
	else
		return TRUE;
}

/*========================================================================
	ME_Push_GetContentType
==========================================================================
Purpose: Retrieves the content type from the Push Header.
Params:  vsHeaderDef
Return:  UINT8 (1=SI, 2=SL, 3=CO, 4=SIA)
=========================================================================*/
UINT8 ME_Push_GetContentType( void* vsHeaderDef )
{
	pHEADERDEF pHeaderDef = NULL;
	UINT8 iType;
	BOOL fError = FALSE;
	pHEADERELEMENT pElement = NULL;
	WSPPARAMETERS* pParameters = NULL;

	if (vsHeaderDef!=NULL)
	{
		pHeaderDef=(pHEADERDEF)vsHeaderDef;

		pElement=WSP_GetHeaderWK(1,Field_Content_Type,pHeaderDef->pHeadList);
		iType=(UINT8)WSP_GetContentTypeByte(pElement,&fError,&pParameters);
		if (fError)
		{
			return 0;
		}
		switch(iType)
		{
		case 46:
			return SI;
		case 48:
			return SL;
		case 50:
			return CO;
		case 52:
			return SIA;
		}
		return 0;
	}
	if (pParameters!=NULL)
	{
		WSP_DeleteParameters(&pParameters);
	}
	return 0;

} /* end ME_Push_GetContentType */

/*========================================================================
	ME_Push_SecurityOk
==========================================================================
Purpose: Check if security levels match.
Params:  Push Flag, Seurity Level
Return:  TRUE if Ok, FALSE otherwise
=========================================================================*/
BOOL ME_Push_SecurityOk(UINT8 iPushFlag)
{
	UINT8   uiErrorCode = 0;
	UINT8   iSecLevel = 0;
	DB_ref  myDB_Root;

#ifdef FEATURE_WAP
	  myDB_Root = getRef(DB_root, (const BYTE*)"globl", &uiErrorCode);
#else
      myDB_Root = getRef(DB_root, "globl", &uiErrorCode);
#endif //FEATURE_WAP

	  if( uiErrorCode == DB_err_success ){
#ifdef FEATURE_WAP
		iSecLevel = (UINT8)getInt( myDB_Root, (const BYTE*)"pshSL", FALSE, &uiErrorCode );
#else
        iSecLevel = (UINT8)getInt( myDB_Root, "pshSL", FALSE, &uiErrorCode );
#endif //FEATURE_WAP

		if( uiErrorCode == DB_err_success ){
			if (iSecLevel == 0)
				return TRUE;
			if (iSecLevel == 1)
			{
				if (3 & iPushFlag)
					return TRUE;
			}
			if (iSecLevel == 2)
				return FALSE;
		}
	  }
	return TRUE;

} /* end ME_Push_SecurityOk */


/*========================================================================
	ME_Push_BuildCO
==========================================================================
Purpose: Call parsing functions and build content for an CO.
Params:  void* pPushBody, UINT32 iBodyDataLen, iType
Return:  pELEMENTTYPE (CO parser structure type)
=========================================================================*/
void* ME_Push_BuildCO(void* pBodyData, UINT32 iBodyDataLen, void* vsHeaderDef,
					  UINT8 iViewID)
{
	BYTE* pbBody = NULL;
	BYTE* pbEnd = NULL;
	pHEADERDEF pHeaderDef = NULL;
	UINT8 iType;
	BOOL fError = FALSE;
	pHEADERELEMENT pElement = NULL;
	WSPPARAMETERS* pParameters = NULL;
	pDECSTR pStruct = NULL;
	INT16 iCharset = 0;
	UINT8 iLevel = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("ME_Push_BuildCO");
#endif

	pHeaderDef=(pHEADERDEF)vsHeaderDef;

	pElement=WSP_GetHeaderWK(1,Field_Content_Type,pHeaderDef->pHeadList);
	iType=(UINT8)WSP_GetContentTypeByte(pElement,&fError,&pParameters);
	if (fError)
	{
		return NULL;
	}
	if (pParameters != NULL)
	{
		iCharset = pParameters->iCharset;
		iLevel = pParameters->iLevel;
		WSP_DeleteParameters(&pParameters);
	}

	pbBody=(BYTE*)pBodyData;
	pbEnd=pbBody+iBodyDataLen;

	/* Parse the CO */
	pStruct = Co_BuildStructure(pbBody,pbEnd,iCharset,iLevel,iViewID);
	return (void*)pStruct;
} /* end of ME_Push_BuildCO */

/*========================================================================
	ME_Push_GetAddress
==========================================================================
Purpose: Get addresslist from SIA content
Params:  void*
Return:  UINT8, 1=ok, 0=not ok
=========================================================================*/
void* ME_Push_GetAddress( void* pBodyHandle, UINT32 iBodyLen
						 /*void* pvAddressList*/  )
{
	BYTE* pbTemp = (BYTE*)pBodyHandle;
	BYTE* pbEnd = NULL;
	UINT8 iAppIdListLen=0;
	UINT8 iContactListLen=0;
	UINT32 uiPort = 0;
	UINT8 iAddressLength=0;
	ADDRESS* pAddressList = NULL;

	pbEnd = pbTemp + iBodyLen;
	iAppIdListLen = (UINT8)pbTemp[1];
	iContactListLen = (UINT8)pbTemp[2+iAppIdListLen];
	if (iContactListLen>0)
	{
		pbTemp = pbTemp+3+iAppIdListLen; 
		if (pbTemp < pbEnd)
		{
			/*pAddressList = (ADDRESS*)pvAddressList;*/
			iAddressLength=iContactListLen; 
			if (ParseRedirectAddress( (void*)pbTemp,iAddressLength,
				(void*)&pAddressList ))  
				if( pAddressList != NULL ){
					return (void*)pAddressList;
				}
		}
	}
	
	return NULL;	/* failure */
} /* end of ME_Push_GetAddress */

/*========================================================================
	ME_Push_GetAppId
==========================================================================
Purpose: Get App Id from SIA content
Params:  void*
Return:  UINT8
=========================================================================*/
UINT8 ME_Push_GetAppId( void* pBodyHandle, UINT32 iBodyLen )
{
	/*
	urn:x-wap-application:*         0x00       ;any app
    urn:x-wap-application:push.sia  0x01       ;WAP Push SIA
    urn:x-wap-application:wml.ua    0x02       ;WML User Agent
	urn:x-wap-application:wta.ua    0x03       ;WTA User Agent
    */
	
	BYTE* pbTemp = (BYTE*)pBodyHandle;
	UINT8 iAppIdListLen=0;
	UINT8 iAppId;

    iBodyLen = iBodyLen;
	iAppIdListLen = (UINT8)pbTemp[1];
	if (iAppIdListLen>0)
	{
		iAppId = (UINT8)pbTemp[2];
		/* check if it is a valid id */
        if (iAppId<4)
		{
			return iAppId;
		}
		else
		{
			return 2; /* default - WML App */
		}
	}
	else
	{
		return 2; /* default - WML App */
	}
} /* end of ME_Push_GetAppId */

/*========================================================================
	ME_Push_COCompareAuth
==========================================================================
Purpose: Compare Authority parts of two urls.
Params:  vsUrl1, vsUrl2
Return:  TRUE if equal, FALSE if not
=========================================================================*/
BOOL ME_Push_COCompareAuth( void* vsUrl1, void* vsUrl2 )
{
	BYTE* pbUrl1 = NULL;
	BYTE* pbUrl2 = NULL;
	
	pbUrl1=(BYTE*)vsUrl1;
	pbUrl2=(BYTE*)vsUrl2;

	return b_EqualURL(pbUrl1,pbUrl2,AUTHORITY_COMP);
}

/*========================================================================
	ME_Push_Remove_SignalList
==========================================================================
Purpose: Removes the entire Signal List and deallocates struct data
Params:  pvPushList
Return:  -
=========================================================================*/
void ME_Push_Remove_SignalList( void* pvSignalList )
{

SIGNALSTRUCT*		pSignalStruct = NULL;
void*				pvListElement = NULL;
void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Push_Remove_SignalList");
#endif
	
	if( pvSignalList != NULL )
	{
		pvListElement = SDL_GetNextListElement( pvSignalList, NULL );
		while ( pvListElement != NULL )
		{
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
			{
				pSignalStruct = (SIGNALSTRUCT*)pvElementContent;
				SDL_DeleteListElement (pvSignalList, pvListElement); 
				DEALLOC (&(pSignalStruct->pPushBody)); 
				DEALLOC (&(pSignalStruct->pPushHeader)); 
				DEALLOC (&pSignalStruct);  
			}
			pvListElement = SDL_GetNextListElement(pvSignalList,NULL );
		}
		SDL_DeleteList(&pvSignalList); 
	}

} /* end of ME_Push_Remove_SignalList */

