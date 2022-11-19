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

	File: RepHdr.c

	Description:

	Author: Charlotte Eklöf, AU-System Radio AB

	Revision history:
  	Date    Rev  Sign  Comment
  	990722  PA1  CEF    First version
  	991202  PA2  CEF    Distributed for inspection
	991212	PA3	 CEF	Updated after inspection. 
	991212	PA4	 CEF	Updated with functions for garbage collection and 
						changes for the md5, etag and lastmod handling.

	991221	PA5	 JPR	Minor Corrections
	000103	PA6	 JPR	Minor Corrections
	000608	PA7	 CEF	Minor corrections done before new changes due to 
						WTA 1.2.1.
	000627	PA8	 CEF	Changes due to new attributes (useracceccability 
						and channelid) added in the channel DTD. 
	001011		ASPN	Lower-case filenames

=========================================================================*/

/* Private include statements */

#include "rephdr.h"
#include "chpars.h"
#include "repll.h"
#include "cmmnrsrc.h"
#include "waedef.h"
#include "wae_cmmn.h"
#include "hdrutil.h"
#include "url.h"
#include "wbxmldef.h"
#include "wbxmlelm.h"
#include "headdef.h"
#include "aapiwta.h"


typedef struct
{
	UINT32	iLLRepHId;
	UINT8	iStoredInRep;
	BYTE*	pbUrl;
	void*	pvResource;
	HEADERPARAMSTRUCT*	pHeaderParams;

} INSTALLRESSTRUCT, *pINSTALLRESSTRUCT;

typedef struct
{
	void* pvHeaderHandle;
	void* pvBodyData;
	UINT32 iBodyDataLength;

} CHANNELTYPE, *pCHANNELTYPE;

typedef struct
{
	UINT16	iWTAIFId;
	void*	pvChannelType;
	
} INSTALLREQSTRUCT, *pINSTALLREQSTRUCT;

typedef struct
{
	UINT32	iPId;

} INSTALLPID, *pINSTALLPID;

typedef struct
{
	UINT32	iLLRepHId;
	UINT32	iPId;
	INT8	iStoreflag;
	void*	pvRepContent;
	
} COMPACTLISTSTRUCT, *pCOMPACTLISTSTRUCT;

/*========================================================================
	INTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	EXTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	Functions called from several processes
=========================================================================*/

/*========================================================================
	Free_RepContent
==========================================================================
Purpose: Free the RepContent struct
Params:  pvRepContentStruct(IN)
Return:  void
=========================================================================*/
void Free_RepContent(void* pvRepContentStruct)
{
	pREPCONTENTSTRUCT pRepContentStruct;

	#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End Free_RepContent");
	#endif

	if (pvRepContentStruct != NULL){

		pRepContentStruct = ((REPCONTENTSTRUCT*)pvRepContentStruct);
		if (pRepContentStruct != NULL){	

			if ( ((pRepContentStruct)->Type) == CONTENTTYPE_CHANNEL ){

				DEALLOC(&((pRepContentStruct)->content.pChContentStruct->
					pwchTitle));
				DEALLOC(&((pRepContentStruct)->content.pChContentStruct->
					pwchAbstract));
				DEALLOC(&((pRepContentStruct)->content.pChContentStruct->
					pwchEventId));
				DEALLOC(&((pRepContentStruct)->content.pChContentStruct->
					pwchchannelid));
				DEALLOC(&((pRepContentStruct)->content.pChContentStruct->
					piAllResInChList)); 
				DEALLOC(&((pRepContentStruct)->content.pChContentStruct));

			}
			else{

				DEALLOC(&((pRepContentStruct)->content.pResContentStruct->
					pbUrl)); 
				DEALLOC(&((pRepContentStruct)->content.pResContentStruct->
					pbBodyData));
				DeleteHEADERDEF((pRepContentStruct)->content.pResContentStruct->
					pHeaderHandle);
				DEALLOC(&((pRepContentStruct)->content.pResContentStruct));

			}

			DEALLOC(&pvRepContentStruct);

		}
	}
} /* end of Free_RepContent */

/*========================================================================
	Functions called from the process RepH_ME
=========================================================================*/

/*========================================================================
	ME_Free_RepContentStructResource
==========================================================================
Purpose: Remove the Resource RepContentStruct. If the parameter boolStatus 
		 is set to FALSE the pHeaderHandle and pbBodyData will not be 
		 deleted. Used when a resource is fetched from the Repository 
		 memory. The pHeaderHandle and pbBodyData will be sent 
		 in the SDL struct type ResponseStruct.
Params:  pvRepContentStruct(IN) and boolStatus(IN).
Return:  void
=========================================================================*/
void Free_RepContentStructResource(void* pvRepContentStruct, BOOL 
									  boolStatus)
{
	pREPCONTENTSTRUCT pRepContentStruct;

	#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End ME_Free_RepContentStructResource");
	#endif

	if (pvRepContentStruct != NULL)
	{
		pRepContentStruct = ((REPCONTENTSTRUCT*)pvRepContentStruct);
		if (pRepContentStruct != NULL)
		{	

			if (boolStatus) {

				DEALLOC(&((pRepContentStruct->content.pResContentStruct)->
					pbBodyData));
				DeleteHEADERDEF((pRepContentStruct)->content.pResContentStruct->
					pHeaderHandle);
				DEALLOC(&((pRepContentStruct->content.pResContentStruct)->
					pbUrl));
			}
			DEALLOC(&((pRepContentStruct)->content.pResContentStruct));
			DEALLOC(&pvRepContentStruct);
		}
	}
} /* end of ME_Free_RepContentStructResource */

/*========================================================================
	ME_CompareString
==========================================================================
Purpose: Compares two strings.
Params:	 pwch1(IN) and pwch2(IN).
Return:	 TRUE if the two string are equal, FAlSE otherwise.
=========================================================================*/
BOOL ME_CompareString(WCHAR* pwch1, WCHAR* pwch2)
{
	UINT16 istrlength;

	if ( (pwch1 != NULL) && (pwch2 != NULL) ) {

		istrlength = STRINGLENGTH(pwch1);

		if (istrlength == (STRINGLENGTH(pwch2)))

			if ( COMPARESTRINGN(pwch1, pwch2, istrlength) == 0 )
				return TRUE;
	}
	return FALSE;
} /* end of ME_CompareString */

/*========================================================================
	RepCh_List functions, each element stores information ( EventId, 
	LLRepHId and the LLRepHId to the first resource )about a channel 
	stored in the repository. The list is used at either a 'Control of 
	global binding' request or at a request from WSPIF.
=========================================================================*/

/*========================================================================
	ME_RepCh_List_Add
==========================================================================
Purpose: Create a new element (REPCHSTRUCT*) and add it to the Repository 
		 Channels List.
Params:  pvRepChList(IN), pvEventId(IN), iLLRepHId(IN), 
		 iLLRepHIdToFirstRes(IN) and iStatus(IN).
Return:  TRUE if ok, otherwise FALSE.
=========================================================================*/
BOOL ME_RepCh_List_Add( void* pvRepChList, void* pvEventId, 
						void* pvchannelid, UINT32 iLLRepHId, 
						UINT32 iLLRepHIdToFirstRes)
{
	REPCHSTRUCT* pRepChStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start RepCh_List_Add");
#endif

	if( pvRepChList != NULL ){

		pRepChStruct = NEWSTRUCT(REPCHSTRUCT);

		if( pRepChStruct != NULL ){

			pRepChStruct->pchEventId = (WCHAR*)pvEventId;
			pRepChStruct->pchchannelid = (WCHAR*)pvchannelid;
			pRepChStruct->iLLRepHId = iLLRepHId;
			pRepChStruct->iLLRepHIdToFirstRes = iLLRepHIdToFirstRes;

			return SDL_AddElementLast( pvRepChList, 0, (void*)pRepChStruct );
		}
	}
	return FALSE;
} /* end of ME_RepCh_List_Add */

/*========================================================================
	ME_RepCh_List_CheckEventId
==========================================================================
Purpose: Check if an element with the same EventId is stored in the RepCh_
		 List.
Params:  pvRepChList(IN) and pvEventId(IN).
Return:  (void*)(REPCHSTRUCT*)
=========================================================================*/
void* ME_RepCh_List_CheckEventId( void* pvRepChList, void* pvEventId )
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	WCHAR*	pwcharhelpEventId = NULL;
	BOOL	boolfound = FALSE;
	REPCHSTRUCT* pRepChStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_RepCh_List_CheckEventId");
#endif

	if( pvRepChList != NULL ){

		pvListElement = SDL_GetNextListElement( pvRepChList, NULL );

		while ( (pvListElement != NULL) ) {
			
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pRepChStruct = (REPCHSTRUCT*)pvElementContent;
				pwcharhelpEventId = pRepChStruct->pchEventId;

				boolfound = ME_CompareString( pwcharhelpEventId, 
					((WCHAR*)(pvEventId)) );

				if (boolfound)
					return (void*)pRepChStruct;
			}
			pvListElement = SDL_GetNextListElement( pvRepChList, pvListElement );
		}
	}
	return NULL;
} /* end of ME_RepCh_List_CheckEventId */

/*========================================================================
	ME_RepCh_List_Checkchannelid
==========================================================================
Purpose: Check if an element with the same channelid is stored in the RepCh_
		 List.
Params:  pvRepChList(IN) and pvchannelid(IN).
Return:  (void*)(REPCHSTRUCT*)
=========================================================================*/
void* ME_RepCh_List_Checkchannelid( void* pvRepChList, void* pvchannelid )
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	WCHAR*	pwcharhelpchannelid = NULL;
	BOOL	boolfound = FALSE;
	REPCHSTRUCT* pRepChStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_RepCh_List_Checkchannelid");
#endif

	if( pvRepChList != NULL ){

		pvListElement = SDL_GetNextListElement( pvRepChList, NULL );

		while ( (pvListElement != NULL) ) {
			
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pRepChStruct = (REPCHSTRUCT*)pvElementContent;
				pwcharhelpchannelid = pRepChStruct->pchchannelid;

				boolfound = ME_CompareString( pwcharhelpchannelid, 
					((WCHAR*)(pvchannelid)) );

				if (boolfound)
					return (void*)pRepChStruct;
			}
			pvListElement = SDL_GetNextListElement( pvRepChList, pvListElement );
		}
	}
	return NULL;
} /* end of ME_RepCh_List_Checkchannelid */

/*========================================================================
	ME_RepCh_List_CheckchEventid
==========================================================================
Purpose: Check if an element with the same channelid or EventId is stored 
		 in the RepCh_List.
Params:  pvRepChList(IN) and pvchannelid(IN).
Return:  (void*)(REPCHSTRUCT*)
=========================================================================*/
void* ME_RepCh_List_CheckchEventid( void* pvRepChList, void* pvchannelid, 
									void* pvEventId, UINT32 iLLRepHId, 
									UINT32 iLLRepHIdToFirstRes)
{

	REPCHSTRUCT*	pRepChStructChIdSame = NULL;
	REPCHSTRUCT*	pRepChStructEventIdSame = NULL;
	UINT32*	piList = NULL;
	UINT32*	helppiList;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_RepCh_List_CheckchEventid");
#endif

	if( pvRepChList != NULL ){

		pRepChStructChIdSame = (REPCHSTRUCT*)
			(ME_RepCh_List_Checkchannelid(pvRepChList, pvchannelid));
		if ( pRepChStructChIdSame == NULL ){
			
			pRepChStructEventIdSame = (REPCHSTRUCT*)
				(ME_RepCh_List_CheckEventId(pvRepChList, pvEventId));
			if ( pRepChStructEventIdSame == NULL ){	
			
			/* New channel, the channel id is unique and the event id is unique */

				ME_RepCh_List_Add(pvRepChList, pvEventId, pvchannelid, 
					iLLRepHId, iLLRepHIdToFirstRes);
			}
			else{

				/* New channel, the channel id is unique but the event id 
				   is not unique. The channel for which the event id is 
				   not unique must be removed from the RepCh list and the 
				   iLLRepHId must be stored in a list so that the channel 
				   can be removed from the memory*/

				ME_RepCh_List_Add(pvRepChList, pvEventId, pvchannelid, 
					iLLRepHId, iLLRepHIdToFirstRes);

				piList = NEWARRAY(UINT32, 2);
				helppiList = piList;
				*helppiList = (pRepChStructEventIdSame->iLLRepHId);
				helppiList++;
				*helppiList = NULL;

				ME_RepCh_List_RemoveCh(pvRepChList, (void*)(pRepChStructEventIdSame));
			} /* if (pRepChStructEventIdSame == NULL) {*/
		}
		else{
			
			pRepChStructEventIdSame = (REPCHSTRUCT*)
				(ME_RepCh_List_CheckEventId(pvRepChList, pvEventId));
			if (pRepChStructEventIdSame == NULL){

				/* Channel is already stored. The event id is unique. 
				   Update the RepChList with new values for the event id, 
				   iLLRepHId and the iLLRepHIdToFirstRes. ILLRepHId must 
				   be stored in a list so that the channel can be removed 
				   from the memory*/	

				piList = NEWARRAY(UINT32, 2);
				helppiList = piList;
				*helppiList = pRepChStructChIdSame->iLLRepHId;
				helppiList++;
				*helppiList = NULL;
				
				
				DEALLOC(&(pRepChStructChIdSame->pchEventId));
				DEALLOC(&pvchannelid);
				pRepChStructChIdSame->pchEventId = pvEventId;
				pRepChStructChIdSame->iLLRepHId = iLLRepHId;
				pRepChStructChIdSame-> iLLRepHIdToFirstRes = iLLRepHIdToFirstRes;
			}
			else{
				/* Channel is already stored. The event id is not unique. */ 
				if (pRepChStructChIdSame == pRepChStructEventIdSame){

					/*Update the RepChList with new values for the iLLRepHId and 
					the iLLRepHIdToFirstRes. ILLRepHId must be stored in a list so 
					that the channel can be removed from the memory*/	

					piList = NEWARRAY(UINT32, 2);
					helppiList = piList;
					*helppiList = pRepChStructChIdSame->iLLRepHId;
					helppiList++;
					*helppiList = NULL;

					pRepChStructChIdSame->iLLRepHId = iLLRepHId;
					pRepChStructChIdSame-> iLLRepHIdToFirstRes = iLLRepHIdToFirstRes;
					DEALLOC(&pvchannelid);
					DEALLOC(&pvEventId);

				}
				else{
					/*An old channel is stored with the same channel id and an old 
					channel is stored with the same event id. The RepChList must be 
					updated with new event id for the channel id which is already 
					stored. The channel for which the event id is the same must be 
					deleted from the RepChList. Both iLLRepHId to the old channel 
					must be stored so that these can be removed from the memory*/	

					piList = NEWARRAY(UINT32, 3);
					helppiList = piList;
					*helppiList = pRepChStructChIdSame->iLLRepHId;
					helppiList++;
					*helppiList = pRepChStructEventIdSame->iLLRepHId;
					helppiList++;
					*helppiList = NULL;

					DEALLOC(&(pRepChStructChIdSame->pchEventId));
					pRepChStructChIdSame->pchEventId = pvEventId;
					pRepChStructChIdSame->iLLRepHId = iLLRepHId;
					pRepChStructChIdSame-> iLLRepHIdToFirstRes = iLLRepHIdToFirstRes;

					ME_RepCh_List_RemoveCh(pvRepChList, (void*)(pRepChStructEventIdSame));

				} /* if (pvRepChStructChIdSame == pvRepChStructEventIdSame)... */
			} /* (pRepChStructEventIdSame == NULL){*/
		} /* if (pRepChStructChIdSame == NULL) */
	} /* if( pvRepChList != NULL ){ */	
	return piList;
} /* end of ME_RepCh_List_CheckchEventid */

/*========================================================================
	ME_RepCh_List_GetIdForChannel
==========================================================================
Purpose: Get low level Id for next channel.
Params: pvRepChList (IN), pliRepHLLId(OUT) and pvPrevChannel(IN);
Return:
=========================================================================*/
void* ME_RepCh_List_GetIdForChannel( void* pvRepChList, 
									unsigned long int* pliRepHLLId,
									void* pvPrevChannel )
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	REPCHSTRUCT* pRepChStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_RepCh_List_GetIdForChannel");
#endif

	if( pvRepChList != NULL ){

		pvListElement = SDL_GetNextListElement( pvRepChList, pvPrevChannel );
		if (pvListElement != NULL) {

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pRepChStruct = (REPCHSTRUCT*)pvElementContent;
				*pliRepHLLId = pRepChStruct->iLLRepHId;
				return (void*)(pvListElement);
			}
		}
	}
	return NULL;
} /* end of ME_RepCh_List_GetIdForChannel */

/*========================================================================
	ME_RepCh_List_RemoveCh
==========================================================================
Purpose: Remove an element (REPCHSTRUCT*) from the RepCh_List.
Params:  pvRepChList(IN) and pvRepChStruct(IN).
Return:  TRUE if ok, otherwise FALSE.
=========================================================================*/
BOOL ME_RepCh_List_RemoveCh( void* pvRepChList, void* pvRepChStruct )
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	BOOL	boolfound = FALSE;
	REPCHSTRUCT* pRepChStruct = NULL;

	/* OBS Här måste också uppdateras, man skall göra dealloc på pchchannelid också */

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_RepCh_List_RemoveCh");
#endif

	if( pvRepChList != NULL ){

		pvListElement = SDL_GetNextListElement( pvRepChList, NULL );

		while ( (pvListElement != NULL) && (!boolfound) ){
			
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pRepChStruct = (REPCHSTRUCT*)pvElementContent;

				if ( pRepChStruct == ((REPCHSTRUCT*)pvRepChStruct) ){

					DEALLOC(&(pRepChStruct->pchEventId));
					DEALLOC(&(pRepChStruct->pchchannelid));
					DEALLOC(&pRepChStruct);
					SDL_DeleteListElement (pvRepChList, pvListElement);
					return TRUE;
				}

			}
			pvListElement = SDL_GetNextListElement( pvRepChList, 
				pvListElement );
		}
	}
	return FALSE;
} /* end of ME_RepCh_List_RemoveCh */

/*========================================================================
	ME_RepCh_List_RemoveChWithRepHLLId
==========================================================================
Purpose: Remove an element (REPCHSTRUCT*) from the RepCh_List.
Params:  pvRepChList(IN) and iLLRepHId(IN).
Return:  BOOL
=========================================================================*/
BOOL ME_RepCh_List_RemoveChWithRepHLLId( void* pvRepChList, UINT32 iLLRepHId )
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	BOOL	boolfound = FALSE;
	REPCHSTRUCT* pRepChStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_RepCh_List_RemoveChWithRepHLLId");
#endif

	if( pvRepChList != NULL ){

		pvListElement = SDL_GetNextListElement( pvRepChList, NULL );

		while ( (pvListElement != NULL) && (!boolfound) ){
			
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pRepChStruct = (REPCHSTRUCT*)pvElementContent;

				if (pRepChStruct->iLLRepHId == iLLRepHId) {

					DEALLOC(&(pRepChStruct->pchEventId));
					DEALLOC(&(pRepChStruct->pchchannelid));
					DEALLOC(&pRepChStruct);
					SDL_DeleteListElement (pvRepChList, pvListElement);
					return TRUE;
				}

			}
			pvListElement = SDL_GetNextListElement( pvRepChList, 
				pvListElement );
		}
	}
	return FALSE;
} /* end of ME_RepCh_List_RemoveChWithRepHLLId */

/*========================================================================
	ME_RepCh_List_RemoveList
==========================================================================
Purpose: Remove all elements in the RepCh_List and delete the REPCHSTRUCT 
		 that the elemets points to. When all the elements are removed the 
		 list header is removed if the indata boolRemoveHeader is set.
Params:  pvRepChList(IN) and boolRemoveHeader(IN).
Return:  void
=========================================================================*/
void ME_RepCh_List_RemoveList( void** ppvRepChList, BOOL boolRemoveHeader)
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	REPCHSTRUCT* pRepChStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_RepCh_List_RemoveCh");
#endif

	if( *ppvRepChList != NULL ){

		pvListElement = SDL_GetNextListElement( *ppvRepChList, NULL );
		while ( pvListElement != NULL ) {
			
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pRepChStruct = (REPCHSTRUCT*)pvElementContent;
				DEALLOC(&(pRepChStruct->pchEventId));
				DEALLOC(&(pRepChStruct->pchchannelid));
				DEALLOC(&pRepChStruct);
				SDL_DeleteListElement (*ppvRepChList, pvListElement);
			}
			pvListElement = SDL_GetNextListElement( *ppvRepChList, 
				NULL );
		}
		if (boolRemoveHeader)
			SDL_DeleteList (ppvRepChList);
	}
} /* end of ME_RepCh_List_RemoveList */

/*========================================================================
	ME_ServiceList_Init
==========================================================================
Purpose: Initiate the service list.
Params: iLength(IN).
Return: (void*)pServiceList
=========================================================================*/
void* ME_ServiceList_Init(UINT32 iLength)
{
	ServiceType** ppServiceList = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_ServiceList_Init");
#endif

	ppServiceList = NEWARRAY(ServiceType*, iLength+1);
	*ppServiceList = NULL;
	return (void*)(ppServiceList);
} /* end of ME_ServiceList_Init */

/*========================================================================
	ME_ServiceList_Add
==========================================================================
Purpose: Add a new Service to the Service List.
Params: pvServiceList(IN) and pvRepContent(IN).
Return:
=========================================================================*/
void ME_ServiceList_Add(void** ppvServiceList, void* pvRepContent)
{
	ServiceType* pServiceTypeStruct = NULL;
	CHCONTENTSTRUCT* pChContent = NULL;
	ServiceType** ppServiceList = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_ServiceList_Add");
#endif

	ppServiceList = (ServiceType**)ppvServiceList;
	if (ppServiceList != NULL) {

		pChContent = ((REPCONTENTSTRUCT*)pvRepContent)->content.pChContentStruct;

		/* Allocate a new Service Type struct */

		pServiceTypeStruct = NEWSTRUCT(ServiceType);
	
		pServiceTypeStruct->title = pChContent->pwchTitle;
		pServiceTypeStruct->abstract = pChContent->pwchAbstract;
		pServiceTypeStruct->channelid = pChContent->pwchchannelid;
		pServiceTypeStruct->useraccess = pChContent->useraccess;


		/* Insert the created Service Type struct into the Service List */

		while (*ppServiceList != NULL)
			ppServiceList++;

		*ppServiceList  = pServiceTypeStruct;


		/* Mark the end of the list */

		ppServiceList++;
		*ppServiceList = NULL;

		/* Data not used in the Service List must be deallocated */

		DEALLOC(&(pChContent->piAllResInChList) );
		DEALLOC(&(pChContent) );
		DEALLOC(&(pvRepContent));
	}
} /* end of ME_ServiceList_Add */

/*========================================================================
	ME_ServiceList_Delete
==========================================================================
Purpose: Deletes a Service List
Params:	pvServiceList(IN)
Return:
=========================================================================*/
void ME_ServiceList_Delete(void** ppvServiceList)
{
	ServiceType** ppServiceList; 
	ServiceType* pServiceTypeStruct; 

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_ServiceList_Delete");
#endif

	ppServiceList = (ServiceType**)ppvServiceList;

	if (ppServiceList != NULL) {

		while (*ppServiceList != NULL) {

			pServiceTypeStruct = *ppServiceList;

			DEALLOC( &(pServiceTypeStruct->title) );
			DEALLOC( &(pServiceTypeStruct->abstract) );
			DEALLOC( &(pServiceTypeStruct->channelid) );
			DEALLOC( &(pServiceTypeStruct) );

			ppServiceList++;
		}
		DEALLOC(&(ppvServiceList));
	}
} /* end of ME_ServiceList_Delete */

/*========================================================================
	ME_InstallPId_List_Add
==========================================================================
Purpose: Adds a new PId Id to the Installation PId list.
Params:  pvInstallPIdList(IN) and iPId(IN).
Return:  void
=========================================================================*/
void ME_InstallPId_List_Add( void* pvInstallPIdList, UINT32 iPId, 
							INT8 iInstallId)
{
	INSTALLPID* pInstallPId = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_InstallPId_List_Add");
#endif

	if( pvInstallPIdList != NULL ){

		pInstallPId = NEWSTRUCT(INSTALLPID);

		if( pInstallPId != NULL ){

			pInstallPId->iPId = iPId;

			SDL_AddElementLast( pvInstallPIdList, iInstallId, 
				(void*)pInstallPId );
		}
	}
} /* end of ME_InstallPId_List_Add */

/*========================================================================
	ME_InstallPId_List_GetFirstPId
==========================================================================
Purpose: Remove a PId from the InstallPId_List.
Params:  pvInstallPIdList(IN).
Return:  PId.
=========================================================================*/

UINT32 ME_InstallPId_List_GetFirstPId( void* pvInstallPIdList)
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	INSTALLPID* pInstallPId = NULL;
	UINT32 iPId = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_InstallPId_List_GetFirstPId");
#endif

	if( pvInstallPIdList != NULL ){

		pvListElement = SDL_GetNextListElement (pvInstallPIdList, NULL);
		if( pvListElement != NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallPId = (INSTALLPID*)pvElementContent;
				iPId = pInstallPId->iPId;
				SDL_DeleteListElement( pvInstallPIdList, pvListElement );
				DEALLOC(&pInstallPId);
				return iPId;
			}
		}
	}
	return 0;
} /* end of ME_InstallPId_List_GetFirstPId */

/*========================================================================
	ME_InstallPId_List_RemovePId
==========================================================================
Purpose: Remove a PId from the InstallPId_List.
Params:  pvInstallPIdList(IN) and iPId(IN).
Return:  TRUE if ok, otherwise FALSE.
=========================================================================*/
void ME_InstallPId_List_RemovePId( void* pvInstallPIdList, UINT32 iPId)
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	INSTALLPID* pInstallPId = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_InstallPId_List_RemovePId");
#endif

	if( pvInstallPIdList != NULL ){

		pvListElement = SDL_GetNextListElement( pvInstallPIdList, NULL );
		while ( (pvListElement != NULL) ){
			
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallPId = (INSTALLPID*)pvElementContent;
				if (iPId == pInstallPId->iPId){

					DEALLOC(&pInstallPId);
					SDL_DeleteListElement (pvInstallPIdList, pvListElement);
					return;
				}
			}
			pvListElement = SDL_GetNextListElement( pvInstallPIdList, 
				pvListElement );
		}
	}
} /* end of ME_InstallPId_List_RemovePId */

/*========================================================================
	ME_InstallPId_List_InstallProcActive
==========================================================================
Purpose: Checks if a specific InstallId is stored in the InstallPid_List.
Params:  pvInstallPIdList(IN), piPId(OUT) and iInstallId(IN).
Return:  TRUE if ok, otherwise FALSE.
=========================================================================*/
BOOL ME_InstallPId_List_InstallProcActive( void* pvInstallPIdList, 
										  unsigned long int* piPId, 
										  INT8 iInstallId)
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	INSTALLPID* pInstallPId = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_InstallPId_List_InstallProcActive");
#endif

	if( pvInstallPIdList != NULL ){

		pvListElement = SDL_SearchListElement (pvInstallPIdList, iInstallId, NULL);
		pvElementContent = SDL_GetListElementContent( pvListElement );
		if( pvElementContent != NULL ){

			pInstallPId = (INSTALLPID*)pvElementContent;
			*piPId = pInstallPId->iPId;
			return TRUE;
		}
	}
	return FALSE;
} /* end of ME_InstallPId_List_InstallProcActive */

/*========================================================================
	ME_CreateChannelType
==========================================================================
Purpose: Creates a CHANNELTYPE struct from in parameters.
Params:  pvHeaderHandle(IN), pvBodyData(IN) and iBodyDataLength(IN).
Return:  (void*)(CHANNELTYPE*)
=========================================================================*/
void* ME_CreateChannelType( void* pvHeaderHandle, void* pvBodyData, 
						   UINT32 iBodyDataLength)
{
CHANNELTYPE* pChannelType = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_CreateChannelType");
#endif
	pChannelType = NEWSTRUCT(CHANNELTYPE);

	pChannelType->pvHeaderHandle = pvHeaderHandle;
	pChannelType->pvBodyData = pvBodyData;
	pChannelType->iBodyDataLength = iBodyDataLength;

	return (void*)pChannelType;
} /* end of ME_CreateChannelType */

/*========================================================================
	DeleteChannelType
==========================================================================
Purpose: Deletes a CHANNELTYPE struct.
Params:  ppvChannelType(IN).
Return:  void
=========================================================================*/
void DeleteChannelType( void** ppvChannelType)
{
CHANNELTYPE* pChannelType = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start DeleteChannelType");
#endif

	pChannelType = (CHANNELTYPE*)(*ppvChannelType);

	DeleteHEADERDEF( (pHEADERDEF)(pChannelType->pvHeaderHandle) );
	DEALLOC(& (pChannelType->pvBodyData) );

	DEALLOC(ppvChannelType);

} /* end of DeleteChannelType */

/*========================================================================
	BuffInstall_List functions, this list is used when a channel 
	installation is requested and there are no more RepH_Install processes 
	to start. The request is queued in the BuffInstall_List until the 
	Channel installation can be started.
=========================================================================*/

/*========================================================================
	ME_BuffInstall_List_Add
==========================================================================
Purpose: Add a new element with a pointer to a (INSTALLREQSTRUCT*) to the 
		 buffer Install requests list.
Params:  pvBuffInstallList(IN) and pvChannelType(IN).
Return:  void.
=========================================================================*/
void ME_BuffInstall_List_Add( void* pvBuffInstallList, void* pvChannelType)
{
	INSTALLREQSTRUCT* pInstallReqStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start BuffInstall_List_Add");
#endif

	if( pvBuffInstallList != NULL ){
		pInstallReqStruct = NEWSTRUCT(INSTALLREQSTRUCT);

		if( pInstallReqStruct != NULL ){

			pInstallReqStruct->pvChannelType = pvChannelType;

			SDL_AddElementLast(pvBuffInstallList, 0, 
				(void*)pInstallReqStruct );
		}
	}
} /* end of ME_BuffInstall_List_Add */


/*========================================================================
	ME_BuffInstall_List_GetFirst
==========================================================================
Purpose: Get the data stored in the (INSTALLREQSTRUCT*) for the first 
		 element stored in the buffer Insall requests list.
Params:  pvBuffInstallList(IN), liiWTAIIFId(OUT) and ppvChannelType(OUT).
Return:  void
=========================================================================*/
void* ME_BuffInstall_List_GetFirst( void* pvBuffInstallList)
{
	INSTALLREQSTRUCT* pInstallReqStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	void*				pvChannelType = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start BuffInstall_List_GetFirst");
#endif

	if( pvBuffInstallList != NULL ){

		pvListElement = SDL_GetNextListElement( pvBuffInstallList, NULL );
		if( pvListElement != NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallReqStruct = (INSTALLREQSTRUCT*)pvElementContent;
				pvChannelType = pInstallReqStruct->pvChannelType;
				SDL_DeleteListElement( pvBuffInstallList, pvListElement );
				DEALLOC( &pInstallReqStruct );
				return pvChannelType;
			}
		}
	}
	return NULL;
} /* end of ME_BuffInstall_List_GetFirst */

/*========================================================================
	ME_SDL_Ch_BuildStructure
==========================================================================
Purpose: This function calls the channel parsing function 
		 Ch_BuildStructure in the ChParse file.
Params:	 pvChannelType(IN) and iViewId(IN).
Return:  (void*)pDECSTR
=========================================================================*/
void* ME_SDL_Ch_BuildStructure( void* pvChannelType, UINT8 iViewId)
{
	BYTE	bContentType;
	BYTE*	pbData;
	BYTE*	pbEnd;
	BOOL	boolErrflag = TRUE;
	INT16	iCharset = 0;
	UINT8	iContentLevel = 0;
	HEADERDEF* pHeaderDef = NULL;
	pWSPPARAMETERS pParameters = NULL;

	CHANNELTYPE* pChannelType;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ME_SDL_Ch_BuildStructure");
#endif

	pChannelType = (CHANNELTYPE*)pvChannelType;
	pbData = (BYTE*)pChannelType->pvBodyData;
	pbEnd = pbData + (pChannelType->iBodyDataLength);
	pHeaderDef = (HEADERDEF*)pChannelType->pvHeaderHandle;

	bContentType = WSP_GetContentTypeByte( pHeaderDef->pHeadList, &boolErrflag, 
		&pParameters);
	if (pParameters != NULL) {

		iCharset = pParameters->iCharset;
		iContentLevel = pParameters->iLevel;
		WSP_DeleteParameters(&pParameters);
	}

	return (void*)(Ch_BuildStructure(pbData, pbEnd, iCharset, 
		iContentLevel, iViewId));
} /* end of ME_SDL_Ch_BuildStructure */

/*========================================================================
	Functions called from the process RepH_Install
=========================================================================*/

/*========================================================================
	Install_piListInit
==========================================================================
Purpose: Initiate a 0 terminated integer list. 
Params:	 iLenth(IN)
Return:  (void*)(UINT32), pointer to the first integer in the list.
=========================================================================*/
void* Install_piListInit(UINT32 iLength)
{
	UINT32*	piList;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_piListInit");
#endif
	
	piList = NEWARRAY(UINT32, iLength+1);
	*piList = 0;	/* Mark end of the list */

	return (void*)piList;
} /* end of Install_piListInit */

/*========================================================================
	Install_piListAdd
==========================================================================
Purpose: Add an UINT32 to an integer list.
Params:  iValue(IN) and pviList(IN)
Return:  void
=========================================================================*/
void Install_piListAdd(UINT32 iValue, void* pviList)
{

		UINT32* piList;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_piListAdd");
#endif
	
	piList = (UINT32*)pviList;

	while (*piList != 0) 
		piList++;

	*piList = iValue;


#ifdef WAE_DEBUG
    {
        char    stTemp[10];
					URL_DEBUG_PRINT("Resource stored in list vsAllChResId:");
					_itoa( *piList, stTemp, 10 );
					URL_DEBUG_PRINT(stTemp);
    }
#endif

	piList++;
	*piList = 0;	/* Mark end of the list */
} /* end of Install_piListAdd */

/*========================================================================
	ChRes_List functions, this list is used during a channel installation. 
	At a channel installation all resources will be retrived and stored in 
	the ChRes_List, to be easy accessible at load of resources, 
	activation of resources or if the channel installation fails. 
=========================================================================*/

/*========================================================================
	Install_ChRes_List_Add
==========================================================================
Purpose: Add an element to the ChRes_List.
Params:	 pvInstallChResLisst(IN), iIdInList(IN), iLLRepHId(IN), 
		 pvResource(IN), iStoredInRep(IN) and pvUrl(IN).
Return:  BOOL, TRUE if ok, otherwise FALSE.
=========================================================================*/
BOOL Install_ChRes_List_Add( void* pvInstallChResList, UINT16 iIdInList, 
						   UINT32 iLLRepHId, void* pvResource, UINT8 iStoredInRep, 
						   void* pvUrl, void* pvHeaderParams)
{
	INSTALLRESSTRUCT* pInstallResStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_Add");
#endif

	if( pvInstallChResList != NULL ){
		pInstallResStruct = NEWSTRUCT(INSTALLRESSTRUCT);

		if( pInstallResStruct != NULL ){

			pInstallResStruct->iStoredInRep = iStoredInRep;
			pInstallResStruct->iLLRepHId = iLLRepHId;
			pInstallResStruct->pvResource = pvResource;
			pInstallResStruct->pbUrl = (BYTE*)pvUrl;
			pInstallResStruct->pHeaderParams = (HEADERPARAMSTRUCT*)pvHeaderParams;
			return SDL_AddElementLast( pvInstallChResList, iIdInList, 
				(void*)pInstallResStruct );
		}
	}
	return FALSE;
} /* end of Install_ChRes_List_Add */

/*========================================================================
	Install_ChRes_List_GetFirstRes
==========================================================================
Purpose: Get some data from the first element in the ChRes_List. 
Params:	 pvInstallChResList(IN), ppvUrl(OUT), ppvReource(OUT) and 
		 piLLRepHId(OUT).
Return:  int (REQUPDATETOREP, STOREDINREP, NOTSTOREDINREP, or NOCHECKDONE)

		 Changed 2000-01-03, JPR
=========================================================================*/
int Install_ChRes_List_GetFirstRes( void* pvInstallChResList, void** ppvUrl, 
									void** ppvResource, 
									unsigned long int* piLLRepHId, 
									void** ppvHeaderParams)
{
	INSTALLRESSTRUCT* pInstallResStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_GetFirstRes");
#endif

	if( pvInstallChResList != NULL ){

		pvListElement = SDL_GetNextListElement( pvInstallChResList, NULL );
		if( pvListElement != NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallResStruct = (INSTALLRESSTRUCT*)pvElementContent;
				*piLLRepHId = (UINT32)(pInstallResStruct->iLLRepHId);
				*ppvUrl = (void*)(pInstallResStruct->pbUrl);
				
				/* Changed JPR 1999-12-16 */
				*ppvHeaderParams = (void*)(pInstallResStruct->pHeaderParams);
				*ppvResource = (void*)(pInstallResStruct->pvResource);

				return pInstallResStruct->iStoredInRep;
			}
		}
	}
	return FALSE;
} /* end of Install_ChRes_List_GetFirstRes */

/*========================================================================
	Install_ChRes_List_UpdateResource
==========================================================================
Purpose: Update the iStoredInRep flag and the iLLRepHId in the 
		 INSTALLRESSTRUCT.
Params:	 pvInstallChResList(IN), iIdInList(IN), iLLRepHId(IN) and 
		 iStoredInRepStatus.
Return:  void
=========================================================================*/
void Install_ChRes_List_UpdateResource( void* pvInstallChResList, 
										UINT16 iIdInList, UINT32 iLLRepHId, 
										UINT8 iStoredInRepStatus)
{
	INSTALLRESSTRUCT* pInstallResStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_UpdateResource");
#endif

	if( pvInstallChResList != NULL ){

		pvListElement = SDL_SearchListElement( pvInstallChResList, iIdInList, NULL );
		if( pvListElement != NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallResStruct = (INSTALLRESSTRUCT*)pvElementContent;
				pInstallResStruct->iStoredInRep = iStoredInRepStatus;
				pInstallResStruct->iLLRepHId = iLLRepHId;
			}
		}
	}
} /* end of Install_ChRes_List_UpdateResource */

/*========================================================================
	Install_ChRes_List_MarkHeaderParamNULL
==========================================================================
Purpose: Set pointer to Header Parameters to NULL.
Params:	 pvInstallChResList(IN) and iIdInList(IN)
Return:  void
=========================================================================*/
void Install_ChRes_List_MarkHeaderParamNULL( void* pvInstallChResList, 
										UINT16 iIdInList)
{
	INSTALLRESSTRUCT* pInstallResStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_MarkHeaderParamNULL");
#endif

	if( pvInstallChResList != NULL ){

		pvListElement = SDL_SearchListElement( pvInstallChResList, iIdInList, NULL );
		if( pvListElement != NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallResStruct = (INSTALLRESSTRUCT*)pvElementContent;
				pInstallResStruct->pHeaderParams = NULL;
			}
		}
	}
} /* end of Install_ChRes_List_MarkHeaderParamNULL */


/*========================================================================
	Install_ChRes_List_GetRes
==========================================================================
Purpose: Get some data from the INSTALLRESSTRUCT from the element 
		 identified with iIdInList.
Params:	 pvInstallChResList(IN), iIdInList(IN), ppvUrl(OUT), 
		 ppvReource(OUT) and liiLLRepHId(OUT).
Return:  int, flag if resource is stored in the Repository. 
=========================================================================*/
int Install_ChRes_List_GetRes( void* pvInstallChResList, UINT16 iIdInList, 
								void** ppvUrl, void** ppvResource, 
								unsigned long int* liiLLRepHId, 
								void** ppvHeaderParams)
{

	INSTALLRESSTRUCT* pInstallResStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_GetRes");
#endif

	/* This is the value if no more resources are found. */
	*ppvResource=NULL;

	if( pvInstallChResList != NULL ){

		pvListElement = SDL_SearchListElement( pvInstallChResList, iIdInList, NULL );
		if( pvListElement != NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallResStruct = (INSTALLRESSTRUCT*)pvElementContent;
				*liiLLRepHId = (unsigned long int)(pInstallResStruct->iLLRepHId);
				*ppvUrl = (void*)(pInstallResStruct->pbUrl);
				*ppvResource = (pInstallResStruct->pvResource);

				/* Correction 1999-12-20, JPR */
				*ppvHeaderParams = (void*)(pInstallResStruct->pHeaderParams);

				return (pInstallResStruct->iStoredInRep);
			}
		}
	}
	return 0;
} /* end of Install_ChRes_List_GetRes */

/*========================================================================
	Install_ChRes_List_RemoveFirstRes
==========================================================================
Purpose: Get some data from the first element in the ChRes_List, then 
		 remove and delete the element from the ChRes_List. If no more 
		 resources in the list the value of *ppvResurce is set to NULL.
Params:	 pvInstallChResList(IN), liiLLRepHId(OUT) and ppvResource(OUT)
Return:  int, flag if the resource is stored in the repository.
=========================================================================*/
int Install_ChRes_List_RemoveFirstRes( void* pvInstallChResList,  
										unsigned long int* liiLLRepHId, 
										void** ppvResource)
{
	INSTALLRESSTRUCT* pInstallResStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	int					iStoredInRep;
	HEADERPARAMSTRUCT*	pHeaderParams = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_RemoveFirstRes");
#endif

	*ppvResource = NULL;

	if( pvInstallChResList != NULL ){

		pvListElement = SDL_GetNextListElement( pvInstallChResList, NULL );
		if( pvListElement != NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallResStruct = (INSTALLRESSTRUCT*)pvElementContent;
				SDL_DeleteListElement(pvInstallChResList, pvListElement);
				*liiLLRepHId = (long int)(pInstallResStruct->iLLRepHId)	;
				*ppvResource = pInstallResStruct->pvResource;
				iStoredInRep = pInstallResStruct->iStoredInRep;

				DEALLOC( &(pInstallResStruct->pbUrl) );

				if ((pInstallResStruct->pHeaderParams) != NULL) {

					pHeaderParams = pInstallResStruct->pHeaderParams;
					DEALLOC( &(pHeaderParams->pbEtag) );
					DEALLOC( &(pHeaderParams) );
				}
				DEALLOC( &pInstallResStruct );

				return iStoredInRep;
			}
		}
	}
	return 0;
} /* end of Install_ChRes_List_RemoveFirstRes */

/*========================================================================
	Install_ChRes_List_DeleteListContent
==========================================================================
Purpose: Delete the ChRes_List.
Params:	 pvInstallChResList(IN).
Return:  void
=========================================================================*/
void Install_ChRes_List_DeleteListContent( void* pvInstallChResList)
{

	INSTALLRESSTRUCT* pInstallResStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	HEADERPARAMSTRUCT*	pHeaderParams = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_DeleteListContent");
#endif

	if( pvInstallChResList != NULL ){

		pvListElement = SDL_GetNextListElement( pvInstallChResList, NULL );
		while (pvListElement!= NULL ){

			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pInstallResStruct = (INSTALLRESSTRUCT*)pvElementContent;
				DEALLOC( &(pInstallResStruct->pbUrl) );

				if ((pInstallResStruct->pHeaderParams) != NULL) {

					pHeaderParams = pInstallResStruct->pHeaderParams;
					DEALLOC( &(pHeaderParams->pbEtag) );
					DEALLOC( &(pHeaderParams) );
				}

				DEALLOC( &pInstallResStruct );
				SDL_DeleteListElement(pvInstallChResList, pvListElement);
			}

			pvListElement = SDL_GetNextListElement( pvInstallChResList, NULL );
		}
	}
} /* end of Install_ChRes_List_DeleteListContent */

/*========================================================================
	Install_ChRes_List_Delete
==========================================================================
Purpose: Delete the ChRes_List.
Params:	 pvInstallChResList(IN).
Return:  void
=========================================================================*/
void Install_ChRes_List_Delete( void** ppvInstallChResList)
{

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_ChRes_List_Delete");
#endif

	SDL_DeleteList(ppvInstallChResList);
} /* end of Install_ChRes_List_Delete */

/*========================================================================
	The Install_SDL_ functions are used when parsing the received channel 
	content. The functions calls parsing functions from the file ChPars.c.
=========================================================================*/

/*========================================================================
	Install_SDL_UrlResolve
==========================================================================
Purpose: Determine the absolute Url.
Params: pvUrl(IN) and pvBaseUrl(IN).
Return: void*(BYTE*), absolute Url.
=========================================================================*/
void* Install_SDL_UrlResolve( void** ppvUrl, void* pvBaseUrl )
{
	BYTE* pbabsUrl = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_SDL_UrlResolve");
#endif

	if ( (b_Resolve ((BYTE*)pvBaseUrl, (BYTE*)(*ppvUrl), &pbabsUrl)) == TRUE) 
	{
		DEALLOC( ppvUrl );
		return (void*)(pbabsUrl);
	}
	else
		return *ppvUrl;
} /* end of Install_SDL_UrlResolve */

/*========================================================================
	Install_SDL_IsResourceFresh
==========================================================================
Purpose: Check if a resource is fresh.
Params:	 pvRepContent(IN).
Return:  int (REQUPDATETOREP, STOREDINREP, NOTSTOREDINREP, or NOCHECKDONE)

		 Changed 1999-12-21, JPR
=========================================================================*/
int Install_SDL_IsResourceFresh( void* pvRepContent, void* pvResource)
{
	HEADERELEMENT* pHeaderElement;
	RESCONTENTSTRUCT* pResContent = NULL;
	HEADERDEF* pHeaderDef = NULL;
	UINT32 iLastMod = 0;
	BOOL boolCheckDone = FALSE;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_SDL_IsResourceFresh");
#endif

	pResContent = ((REPCONTENTSTRUCT*)pvRepContent)->content.pResContentStruct;
	pHeaderDef = pResContent->pHeaderHandle;
	pHeaderElement = pHeaderDef->pHeadList;

	iLastMod = Ch_GetResourceLastMod((pELEMENTTYPE)pvResource);

	if (iLastMod != 0) 
	{
		if ( Ch_IsResourceFresh(pHeaderElement, iLastMod, &boolCheckDone) )
		{
			return STOREDINREP;
		}
		else
		{
			if (boolCheckDone)
			{
				return REQUPDATETOREP;
			}
			else
			{
				return NOCHECKDONE;
			}
		}
	}
	return NOCHECKDONE;
} /* end of Install_SDL_IsResourceFresh */

/*========================================================================
	Install_SDL_CheckResourceEtag
==========================================================================
Purpose: Check if stored Etag is the same as received Etag.
Params:	 pvRepContent(IN) and pvResource(IN).
Return:  int (REQUPDATETOREP, STOREDINREP, NOTSTOREDINREP, or NOCHECKDONE)

		 Changed 1999-12-21, JPR
=========================================================================*/
int Install_SDL_CheckResourceEtag( void* pvRepContent, void* pvResource)
{
	HEADERELEMENT* pHeaderElement;
	RESCONTENTSTRUCT* pResContent = NULL;
	HEADERDEF* pHeaderDef = NULL;
	BOOL boolCheckDone = FALSE;
	BYTE* pbReceivedEtag;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_SDL_CheckResourceEtag");
#endif

	pResContent = ((REPCONTENTSTRUCT*)pvRepContent)->content.pResContentStruct;
	pHeaderDef = pResContent->pHeaderHandle;
	pHeaderElement = pHeaderDef->pHeadList;
	pbReceivedEtag = Ch_GetResourceEtag((pELEMENTTYPE)pvResource);

	if (pbReceivedEtag != NULL) {

		if ( Ch_CheckResourceEtag(pHeaderElement, pbReceivedEtag, &boolCheckDone) ) {
			DEALLOC(&pbReceivedEtag);
			return STOREDINREP;
		}
		else
			if (boolCheckDone){

				DEALLOC(&pbReceivedEtag);
				return REQUPDATETOREP;
			}
			else {
				DEALLOC(&pbReceivedEtag);
				return NOCHECKDONE;
			}
	}
	else 
		return NOCHECKDONE;
} /* end of Install_SDL_CheckResourceEtag */

/*========================================================================
	Install_SDL_CheckResourceMd5
==========================================================================
Purpose: Check if stored Md5 is the same as received Md5.
Params:	 pvRepContent(IN) and pvResource(IN).
Return:  int (REQUPDATETOREP, STOREDINREP, NOTSTOREDINREP, or NOCHECKDONE)

		 Changed 1999-12-21, JPR
=========================================================================*/
int Install_SDL_CheckResourceMd5( void* pvRepContent, void* pvResource)
{
	HEADERELEMENT* pHeaderElement;
	RESCONTENTSTRUCT* pResContent = NULL;
	HEADERDEF* pHeaderDef = NULL;
	BOOL boolCheckDone = FALSE;

	WCHAR* pwchReceivedMd5;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start Install_SDL_CheckResourceMd5");
#endif

	pResContent = ((REPCONTENTSTRUCT*)pvRepContent)->content.pResContentStruct;
	pHeaderDef = pResContent->pHeaderHandle;
	pHeaderElement = pHeaderDef->pHeadList;
	pwchReceivedMd5 = Ch_GetResourceMd5((pELEMENTTYPE)pvResource);

	if (pwchReceivedMd5 != NULL) {

		if ( Ch_CheckResourceMd5(pHeaderElement, pwchReceivedMd5, &boolCheckDone) ) {
			DEALLOC(&pwchReceivedMd5);
			return STOREDINREP;
		}
		else
			if (boolCheckDone){

				DEALLOC(&pwchReceivedMd5);
				return REQUPDATETOREP;
			}
			else {
				DEALLOC(&pwchReceivedMd5);
				return NOCHECKDONE;
			}
	}
	else 
		return NOCHECKDONE;
} /* end of Install_SDL_CheckResourceMd5 */

/* End of Install_SDL_ parsing functions*/

/*========================================================================
	Install_GetHeaderParameters
==========================================================================
Purpose: Creates a struct of the type REPCONTENTSTRUCT with the header 
		 parameters etag and lastmod. If the etag is not present the value 
		 is set to NULL and if the lastmod is not present the value is set 
		 to zero. 
Params:	 pvRepContent(IN).
Return:  (void*)(HEADERPARAMSTRUCT*)
=========================================================================*/
void* Install_GetHeaderParameters(void* pvRepContent )
{
	HEADERPARAMSTRUCT* pHeaderParams = NULL;

	RESCONTENTSTRUCT* pResContent = NULL;
	HEADERDEF* pHeaderDef = NULL;
	HEADERELEMENT* pHeaderElement;
	BOOL bError;
	UINT32 iLastMod;
	BYTE* pbEtag;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Install_GetHeaderParameters");
#endif

	pHeaderParams = NEWSTRUCT(HEADERPARAMSTRUCT);

	pResContent = ((REPCONTENTSTRUCT*)( pvRepContent ))->content.pResContentStruct;
	pHeaderDef = pResContent->pHeaderHandle;

	/* Set Last modified */
	pHeaderElement = WSP_GetHeaderWK (1, Field_Last_Modified,
                                      pHeaderDef->pHeadList);

    if (pHeaderElement != NULL){

		iLastMod = WSP_GetLastModified (pHeaderElement, &bError);
		if (!bError)
			pHeaderParams->iLastMod = iLastMod;
		else
			pHeaderParams->iLastMod = 0;
	}
	else
		pHeaderParams->iLastMod = 0;

	/* Set Etag */

	pHeaderElement = WSP_GetHeaderWK (1, Field_Etag,
                                      pHeaderDef->pHeadList);

    if (pHeaderElement != NULL){

		pbEtag = WSP_GetEtag (pHeaderElement);
		pHeaderParams->pbEtag = (BYTE*)(GenCopyString(1, (void*)pbEtag));
	}
	else
		pHeaderParams->pbEtag = NULL;

	return ((void*)pHeaderParams);
} /* end of Install_GetHeaderParameters */

/*========================================================================
	Install_CreateRepContentRes
==========================================================================
Purpose: Creates a struct of the type REPCONTENTSTRUCT with content = 
		 RESCONTENTSTRUCT* which can be stored in the repository.
Params:	 pvHeaderHandle(IN), pvBodyData(IN), BodyDataLength(IN) and 
		 pvhref(IN).
Return:  (void*)(REPCONTENTSTRUCT*)
=========================================================================*/
void* Install_CreateRepContentRes( void* pvHeaderHandle, void* pvBodyData, 
								 UINT32 BodyDataLength, void* pvhref)
{
	REPCONTENTSTRUCT* pRepContent = NULL;
	RESCONTENTSTRUCT* pResContent = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Install_CreateRepContentRes");
#endif

	pRepContent = NEWSTRUCT(REPCONTENTSTRUCT);																		
	pResContent = NEWSTRUCT(RESCONTENTSTRUCT);


	pRepContent->Type = CONTENTTYPE_RESOURCE;
	pRepContent->iId = 0;

	pResContent->pbUrl = (BYTE*)(GenCopyString(1, (BYTE*)pvhref));
	pResContent->iStatus = CONTENT_INSTALLING;
	pResContent->iRefCounter = 0;
	pResContent->iInstallCounter = 1;
	pResContent->pHeaderHandle = (HEADERDEF*)pvHeaderHandle;
	pResContent->pbBodyData = (BYTE*)pvBodyData;
	pResContent->iBodyLength = BodyDataLength;

	pRepContent->content.pResContentStruct = pResContent;

	return (void*)pRepContent;
} /* end of Install_CreateRepContentRes */

/*========================================================================
	Install_CreateRepContentCh
==========================================================================
Purpose: Creates a struct of the type REPCONTENTSTRUCT with content = 
		 CHCONTENTSTRUCT* which can be stored in the repository.
Params:	 pvElementStruct(IN), pvAllResInChList(IN) and pvChannelType(IN).
Return:	 (void*)(REPCONTENTSTRUCT*)
=========================================================================*/
void* Install_CreateRepContentCh( void* pvElementStruct, 
								 void* pvAllResInChList, 
								 void* pvChannelType )
{
	REPCONTENTSTRUCT* pRepContent = NULL;
	CHCONTENTSTRUCT* pChContent = NULL;
	pDECSTR pChannel = NULL;
	CHANNELTYPE* pChannelType;
	pHEADERELEMENT pExpireDateElem = NULL;
	pHEADERDEF pHeaderDef = NULL;
	BOOL bError;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Install_CreateRepContentCh");
#endif

	pChannelType = (CHANNELTYPE*)pvChannelType;

	pHeaderDef = (HEADERDEF*)pChannelType->pvHeaderHandle;

	pExpireDateElem = WSP_GetHeaderWK(1, Field_Expires, pHeaderDef->pHeadList);

	bError = FALSE;
	
	pChannel = (pDECSTR)pvElementStruct;

	pRepContent = NEWSTRUCT(REPCONTENTSTRUCT);																		
	pChContent = NEWSTRUCT(CHCONTENTSTRUCT);

	pRepContent->Type = CONTENTTYPE_CHANNEL;
	pRepContent->iId = 0;

	pChContent->iStatus = CONTENT_INSTALLING;

	if (pExpireDateElem != NULL) {
		pChContent->iExpiryDate = WSP_GetExpires (pExpireDateElem, &bError);
		if (bError)
			pChContent->iExpiryDate = 0;
	}
	else
		pChContent->iExpiryDate = 0;

	pChContent->pwchTitle = Ch_GetTitle(pChannel);
	pChContent->pwchAbstract = Ch_GetAbstract(pChannel);
	pChContent->pwchEventId = Ch_GetEventID(pChannel);
	pChContent->pwchchannelid = Ch_Getchannelid(pChannel);
	pChContent->useraccess = Ch_Getuseraccessible(pChannel);
	pChContent->piAllResInChList = (UINT32*)pvAllResInChList;
	pChContent->iResCounter = Ch_GetNbrOfResources(pChannel);

	pRepContent->content.pChContentStruct = pChContent;

	return (void*)pRepContent;
} /* end of Install_CreateRepContentCh */

/*========================================================================
	Functions called from the process RepH_Install
=========================================================================*/

/*========================================================================
	LL_SizeOfStoredRes
==========================================================================
Purpose: Check the size in the repository of stored resources.
Params:  pvRepVars(IN) and pvResStoredInRepIds(IN). 
Return:  int, the size of all stored resources in the repository.
=========================================================================*/
int LL_SizeOfStoredRes( void* pvRepVars, void* pvResStoredInRepIds )
{
		UINT32* piLLRepHIds;
		UINT32 iAllStoredRes = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("LL_SizeOfStoredRes");
#endif

	piLLRepHIds = (UINT32*)pvResStoredInRepIds;

	while (*piLLRepHIds != 0) {

		iAllStoredRes = iAllStoredRes + REP_GetSizeOfContent( 
			pvRepVars, *piLLRepHIds );
		piLLRepHIds++;
	}
	return iAllStoredRes;
} /* end of LL_SizeOfStoredRes */

/*========================================================================
	LL_CompactList_Add
==========================================================================
Purpose: Create a new element (COMPACTLISTSTRUCT*) and add it to the 
		Repository low level Compactisation list.
Params:  pvCompactList(IN), iStoreFlag(IN), iLLRepHId(IN) and 
		pvRepContent(IN)
Return:  BOOL
=========================================================================*/
BOOL LL_CompactList_Add( void* pvCompactList, INT8 iStoreFlag, UINT32 iLLRepHId, 
					  void* pvRepContent, UINT32 iPId)
{
	COMPACTLISTSTRUCT* pCompactListStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start LL_CompactList_Add");
#endif

	if( pvCompactList != NULL ){

		pCompactListStruct = NEWSTRUCT(COMPACTLISTSTRUCT);

		pCompactListStruct->iLLRepHId = iLLRepHId;
		pCompactListStruct->iPId = iPId;
		pCompactListStruct->iStoreflag = iStoreFlag;
		pCompactListStruct->pvRepContent = pvRepContent;
		
		return SDL_AddElementLast( pvCompactList, 0, (void*)pCompactListStruct );
	}
	return FALSE;
} /* end of LL_CompactList_Add */

/*========================================================================
	LL_CompacList_GetFirst
==========================================================================
Purpose: Create a new element (COMPACTLISTSTRUCT*) and add it to the 
		Repository low level Compactisation list.
Params:  pvCompactList(IN), iStoreFlag(IN), iLLRepHId(IN) and 
		pvRepContent(IN)
Return:  void* pvRepContent
=========================================================================*/
void* LL_CompacList_GetFirst( void* pvCompactList, 
							 INT8* piStoreFlag, 
							 unsigned long int* pliLLRepHId,
							 unsigned long int* pliPId)
{
	void*	pvListElement = NULL;
	void*	pvElementContent = NULL;
	void*	pvRepContent = NULL;
	COMPACTLISTSTRUCT* pCompactListStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start LL_CompacList_GetFirst");
#endif

	if( pvCompactList != NULL ){

		pvListElement = SDL_GetNextListElement( pvCompactList, NULL );
		if (pvListElement != NULL){
			
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){

				pCompactListStruct = (COMPACTLISTSTRUCT*)pvElementContent;
				*piStoreFlag = pCompactListStruct->iStoreflag;
				*pliLLRepHId = pCompactListStruct->iLLRepHId;
				*pliPId = pCompactListStruct->iPId;
				pvRepContent = pCompactListStruct->pvRepContent;

				DEALLOC(&pCompactListStruct);
				SDL_DeleteListElement (pvCompactList, pvListElement);

				return pvRepContent;
			}
		}
	}
	return NULL;
} /* end of LL_CompacList_GetFirst */
