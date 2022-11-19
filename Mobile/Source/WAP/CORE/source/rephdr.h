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

#ifndef _RepHdr_H
#define _RepHdr_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: RepHdr.H

	Description:

	Author: Charlotte Eköf, AU-System Radio AB

	Revision history:
  	Date    Rev  Sign  Comment
  	990723  PA1  CEF    First version
	991221		 JPR	Minor Corrections
	000103		 JPR	Minor Corrections
	001011		ASPN	Lower-case filenames

=========================================================================*/

#include "cmmnrsrc.h"
#include "hdrutil.h"		/* ? */

/* Private include statements */

typedef struct
{
	UINT32	iLastMod;
	BYTE*	pbEtag;
	
} HEADERPARAMSTRUCT, *pHEADERPARAMSTRUCT;

typedef struct
{
	UINT8	iStatus;
	UINT32	iExpiryDate;
	WCHAR*	pwchTitle;
	WCHAR*	pwchAbstract;
	WCHAR*	pwchEventId;
	WCHAR*	pwchchannelid;
	BOOL	useraccess;
	UINT32* piAllResInChList;
	UINT32	iResCounter;
} CHCONTENTSTRUCT, *pCHCONTENTSTRUCT;

typedef struct
{
	BYTE*	pbUrl;
	UINT8	iStatus;
	UINT8	iRefCounter;
	UINT8	iInstallCounter;
	HEADERDEF* pHeaderHandle;
	BYTE*	pbBodyData;
	UINT32	iBodyLength;
} RESCONTENTSTRUCT, *pRESCONTENTSTRUCT;

typedef struct
{

	UINT8	Type;
	UINT32	iId;

	union {
		CHCONTENTSTRUCT*	pChContentStruct;
		RESCONTENTSTRUCT*	pResContentStruct;
	} content;

} REPCONTENTSTRUCT, *pREPCONTENTSTRUCT;

#define CONTENT_ACTIVE 1
#define CONTENT_INSTALLING 2
#define CONTENT_STALE 3
#define CONTENT_RUNNING 4
#define CONTENT_WAITINSTALL 5
#define CONTENTTYPE_CHANNEL 1
#define CONTENTTYPE_RESOURCE 2
#define REPROOT_STATUS_OK 1
#define REPROOT_STATUS_PROCESSING 2
#define REPROOT_STATUS_CORRUPT 3
#define STOREFLAG_CHANNEL 0
#define STOREFLAG_RESOURCE 1
#define STOREFLAG_UPDATERESOURCE 2

#define REQUPDATETOREP 0
#define STOREDINREP 1
/* #define LOADEDTOREP 2, Removed 1999-12-21 JPR */
#define NOTSTOREDINREP 3
#define NOCHECKDONE 4

/***********************/
/* Debug stuff         */
/***********************/

#ifdef WAE_DEBUG
    #if defined XSCT_CMICRO || defined NOT_USING_SDL
      #define URL_DEBUG_PRINT(a)         printf(a)
    #else
      #define URL_DEBUG_PRINT(a)         xTraceAlg(a)
    #endif
#endif

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
void Free_RepContent(void* pvRepContentStruct);

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
									  boolStatus);

/*========================================================================
	ME_CompareString
==========================================================================
Purpose: Compares two strings.
Params:	 pwch1(IN) and pwch2(IN).
Return:	 TRUE if the two string are equal, FAlSE otherwise.
=========================================================================*/
BOOL ME_CompareString(WCHAR* pwch1, WCHAR* pwch2);

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
						UINT32 iLLRepHIdToFirstRes);

/*========================================================================
	ME_RepCh_List_CheckEventId
==========================================================================
Purpose: Check if an element with the same EventId is stored in the RepCh_
		 List.
Params:  pvRepChList(IN) and pvEventId(IN).
Return:  (void*)(REPCHSTRUCT*)
=========================================================================*/
void* ME_RepCh_List_CheckEventId( void* pvRepChList, void* pvEventId );

/*========================================================================
	ME_RepCh_List_Checkchannelid
==========================================================================
Purpose: Check if an element with the same channelid is stored in the RepCh_
		 List.
Params:  pvRepChList(IN) and pvchannelid(IN).
Return:  (void*)(REPCHSTRUCT*)
=========================================================================*/
void* ME_RepCh_List_Checkchannelid( void* pvRepChList, void* pvchannelid );

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
									UINT32 iLLRepHIdToFirstRes);

/*========================================================================
	ME_RepCh_List_GetIdForChannel
==========================================================================
Purpose: Get low level Id for next channel.
Params: pvRepChList (IN), pliRepHLLId(OUT) and pvPrevChannel(IN);
Return:
=========================================================================*/
void* ME_RepCh_List_GetIdForChannel( void* pvRepChList, 
									unsigned long int* pliRepHLLId,
									void* pvPrevChannel );

/*========================================================================
	ME_RepCh_List_RemoveCh
==========================================================================
Purpose: Remove an element (REPCHSTRUCT*) from the RepCh_List.
Params:  pvRepChList(IN) and pvRepChStruct(IN).
Return:  TRUE if ok, otherwise FALSE.
=========================================================================*/
BOOL ME_RepCh_List_RemoveCh( void* pvRepChList, void* pvRepChStruct );

/*========================================================================
	ME_RepCh_List_RemoveChWithRepHLLId
==========================================================================
Purpose: Remove an element (REPCHSTRUCT*) from the RepCh_List.
Params:  pvRepChList(IN) and iLLRepHId(IN).
Return:  BOOL
=========================================================================*/
BOOL ME_RepCh_List_RemoveChWithRepHLLId( void* pvRepChList, UINT32 iLLRepHId );

/*========================================================================
	ME_RepCh_List_RemoveList
==========================================================================
Purpose: Remove all elements in the RepCh_List and delete the REPCHSTRUCT 
		 that the elemets points to. When all the elements are removed the 
		 list header is removed if the indata boolRemoveHeader is set.
Params:  pvRepChList(IN) and boolRemoveHeader(IN).
Return:  void
=========================================================================*/
void ME_RepCh_List_RemoveList( void** ppvRepChList, BOOL boolRemoveHeader);

/*========================================================================
	ME_ServiceList_Init
==========================================================================
Purpose: Initiate the service list.
Params: iLength(IN).
Return: (void*)pServiceList
=========================================================================*/
void* ME_ServiceList_Init(UINT32 iLength);

/*========================================================================
	ME_ServiceList_Add
==========================================================================
Purpose: Add a new Service to the Service List.
Params: pvServiceList(IN) and pvRepContent(IN).
Return:
=========================================================================*/
void ME_ServiceList_Add(void** ppvServiceList, void* pvRepContent);

/*========================================================================
	ME_ServiceList_Delete
==========================================================================
Purpose: Deletes a Service List
Params:	pvServiceList(IN)
Return:
=========================================================================*/
void ME_ServiceList_Delete(void** ppvServiceList);

/*========================================================================
	ME_InstallPId_List_Add
==========================================================================
Purpose: Adds a new PId Id to the Installation PId list.
Params:  pvInstallPIdList(IN) and iPId(IN).
Return:  void
=========================================================================*/
void ME_InstallPId_List_Add( void* pvInstallPIdList, UINT32 iPId, 
							INT8 iInstallId);

/*========================================================================
	ME_InstallPId_List_GetFirstPId
==========================================================================
Purpose: Remove a PId from the InstallPId_List.
Params:  pvInstallPIdList(IN).
Return:  PId.
=========================================================================*/

UINT32 ME_InstallPId_List_GetFirstPId( void* pvInstallPIdList);

/*========================================================================
	ME_InstallPId_List_RemovePId
==========================================================================
Purpose: Remove a PId from the InstallPId_List.
Params:  pvInstallPIdList(IN) and iPId(IN).
Return:  TRUE if ok, otherwise FALSE.
=========================================================================*/
void ME_InstallPId_List_RemovePId( void* pvInstallPIdList, UINT32 iPId);

/*========================================================================
	ME_InstallPId_List_InstallProcActive
==========================================================================
Purpose: Checks if a specific InstallId is stored in the InstallPid_List.
Params:  pvInstallPIdList(IN), piPId(OUT) and iInstallId(IN).
Return:  TRUE if ok, otherwise FALSE.
=========================================================================*/
BOOL ME_InstallPId_List_InstallProcActive( void* pvInstallPIdList, 
										  unsigned long int* piPId, 
										  INT8 iInstallId);

/*========================================================================
	ME_CreateChannelType
==========================================================================
Purpose: Creates a CHANNELTYPE struct from in parameters.
Params:  pvHeaderHandle(IN), pvBodyData(IN) and iBodyDataLength(IN).
Return:  (void*)(CHANNELTYPE*)
=========================================================================*/
void* ME_CreateChannelType( void* pvHeaderHandle, void* pvBodyData, 
						   UINT32 iBodyDataLength);

/*========================================================================
	DeleteChannelType
==========================================================================
Purpose: Deletes a CHANNELTYPE struct.
Params:  ppvChannelType(IN).
Return:  void
=========================================================================*/
void DeleteChannelType( void** ppvChannelType);

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
void ME_BuffInstall_List_Add( void* pvBuffInstallList, void* pvChannelType);

/*========================================================================
	ME_BuffInstall_List_GetFirst
==========================================================================
Purpose: Get the data stored in the (INSTALLREQSTRUCT*) for the first 
		 element stored in the buffer Insall requests list.
Params:  pvBuffInstallList(IN), liiWTAIIFId(OUT) and ppvChannelType(OUT).
Return:  void
=========================================================================*/
void* ME_BuffInstall_List_GetFirst( void* pvBuffInstallList);

/*========================================================================
	ME_SDL_Ch_BuildStructure
==========================================================================
Purpose: This function calls the channel parsing function 
		 Ch_BuildStructure in the ChParse file.
Params:	 pvChannelType(IN) and iViewId(IN).
Return:  (void*)pDECSTR
=========================================================================*/
void* ME_SDL_Ch_BuildStructure( void* pvChannelType, UINT8 iViewId);

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
void* Install_piListInit(UINT32 iLength);

/*========================================================================
	Install_piListAdd
==========================================================================
Purpose: Add an UINT32 to an integer list.
Params:  iValue(IN) and pviList(IN)
Return:  void
=========================================================================*/
void Install_piListAdd(UINT32 iValue, void* pviList);

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
						   void* pvUrl, void* pvHeaderParams);

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
									void** ppvHeaderParams);

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
										UINT8 iStoredInRepStatus);

/*========================================================================
	Install_ChRes_List_MarkHeaderParamNULL
==========================================================================
Purpose: Set pointer to Header Parameters to NULL.
Params:	 pvInstallChResList(IN) and iIdInList(IN)
Return:  void
=========================================================================*/
void Install_ChRes_List_MarkHeaderParamNULL( void* pvInstallChResList, 
										UINT16 iIdInList);

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
								void** ppvHeaderParams);

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
										void** ppvResource);

/*========================================================================
	Install_ChRes_List_DeleteListContent
==========================================================================
Purpose: Delete the ChRes_List.
Params:	 pvInstallChResList(IN).
Return:  void
=========================================================================*/
void Install_ChRes_List_DeleteListContent( void* pvInstallChResList);

/*========================================================================
	Install_ChRes_List_Delete
==========================================================================
Purpose: Delete the ChRes_List.
Params:	 pvInstallChResList(IN).
Return:  void
=========================================================================*/
void Install_ChRes_List_Delete( void** ppvInstallChResList);

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
void* Install_SDL_UrlResolve( void** ppvUrl, void* pvBaseUrl );

/*========================================================================
	Install_SDL_IsResourceFresh
==========================================================================
Purpose: Check if a resource is fresh.
Params:	 pvRepContent(IN).
Return:  int (REQUPDATETOREP, STOREDINREP, NOTSTOREDINREP, or NOCHECKDONE)

		 Changed 1999-12-21, JPR
=========================================================================*/
int Install_SDL_IsResourceFresh( void* pvRepContent, void* pvResource);

/*========================================================================
	Install_SDL_CheckResourceEtag
==========================================================================
Purpose: Check if stored Etag is the same as received Etag.
Params:	 pvRepContent(IN) and pvResource(IN).
Return:  int (REQUPDATETOREP, STOREDINREP, NOTSTOREDINREP, or NOCHECKDONE)

		 Changed 1999-12-21, JPR
=========================================================================*/
int Install_SDL_CheckResourceEtag( void* pvRepContent, void* pvResource);

/*========================================================================
	Install_SDL_CheckResourceMd5
==========================================================================
Purpose: Check if stored Md5 is the same as received Md5.
Params:	 pvRepContent(IN) and pvResource(IN).
Return:  int (REQUPDATETOREP, STOREDINREP, NOTSTOREDINREP, or NOCHECKDONE)

		 Changed 1999-12-21, JPR
=========================================================================*/
int Install_SDL_CheckResourceMd5( void* pvRepContent, void* pvResource);
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
void* Install_GetHeaderParameters(void* pvRepContent );

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
								 UINT32 BodyDataLength, void* pvhref);

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
								 void* pvChannelType );

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
int LL_SizeOfStoredRes( void* pvRepVars, void* pvResStoredInRepIds );

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
					  void* pvRepContent, UINT32 iPId);

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
							 INT8* pliStoreFlag, 
							 unsigned long int* pliLLRepHId,
							 unsigned long int* pliPId);
#endif
