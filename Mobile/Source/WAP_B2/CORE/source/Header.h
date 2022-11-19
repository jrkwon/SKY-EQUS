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
#ifndef _Header_H
#define _Header_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: Header.H

	Description:

	Author: Ingmar Persson, AU-System Radio AB

	Revision history:
  	Date    Rev  Sign  Comment
  	980917  PA1  IPN    First version
	990624       IPN    Latest version

=========================================================================*/

/* Private include statements */

#include "cmmnrsrc.h"
#include "wipprdct.h"

#if defined XSCT_CMICRO
    #include "ml_typ.h"
#else
    #include "scttypes.h"
#endif

#include "GlblDecl.h"




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

#define SKT_CLIENT_HELLO	1
#define SKT_SERVER_HELLO	2
#define SKT_KEY_EXCHANGE	3
#define SKT_FINISHED		4

#define SKT_PORT_NR				"9091"
#define SKT_CONTENTTYPE_WML		"application/vnd.wap.skt_e2e_wml"
#define SKT_CONTENTTYPE_WMLS	"application/vnd.wap.skt_e2e_wmls"

/*========================================================================
	FUNCTION PROTOTYPES
=========================================================================*/

/*========================================================================
	urlIsValid
==========================================================================
Purpose: Validate a Url and consist of scheme, host and a path. 
Params:  pvReqStruct	pURLREQSTRUCT	casted to a VOID*
Return:  TRUE if valid, otherwise FALSE
=========================================================================*/
int urlIsValid( VOID* pvReqStruct );

/*========================================================================
	FreeReqStruct 
==========================================================================
Purpose: Release a URLREQSTRUCT and all of it's parts
Params:  pvReqStruct	pURLREQSTRUCT*	casted to a VOID**
Return:	 TRUE
=========================================================================*/
int FreeReqStruct( VOID** pvReqStruct );

/*========================================================================
	FreeNetStruct 
==========================================================================
Purpose: Release a NETWORKSTRUCT and all of it's parts
Params:  vsNetStruct	pNETWORKSTRUCT*	casted to a VOID**
Return:	 TRUE
=========================================================================*/
int FreeNetStruct(VOID** vsNetStruct);

/*========================================================================
	FreeUaSettingsStruct 
==========================================================================
Purpose: Release a UASETTINGSSTRUCT and all of it's parts
Params:  vsStruct	pUASETTINGS*	casted to a VOID**
Return:	 TRUE
=========================================================================*/
int FreeUaSettingsStruct(VOID** vsStruct);

/*========================================================================
	GetMethod
==========================================================================
Purpose: Get the Method part from the URLREQSTRUCT
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
Return:  Method
=========================================================================*/
int GetMethod( VOID* pvReqStruct );

/*========================================================================
	GetUrl 
==========================================================================
Purpose: Get the Url part from the URLREQSTRUCT
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
		 pvUrl			BYTE**			casted to VOID**
Return:  length of pvUrl
=========================================================================*/
int GetUrl( VOID* pvReqStruct, VOID** pvUrl );

/*========================================================================
	GetHeader 
==========================================================================
Purpose: Create the Header from the URLREQSTRUCT and pUASETTINGS
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
		 pvUaSettings	pUASETTINGS		casted to a VOID*
		 pvHeader		BYTE**			casted to VOID**
Return:  length of pvHeader
=========================================================================*/
int GetHeader( VOID* pvReqStruct, VOID* pvUaSettings, int iConnectionMode, VOID** pvHeader );

/*========================================================================
	GetBody 
==========================================================================
Purpose: Get the Body from the URLREQSTRUCT
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
		 pvBody			BYTE**			casted to VOID**
Return:  length of pvBody
=========================================================================*/
int GetBody( VOID* pvReqStruct, VOID** pvBody);

/*========================================================================
	CreateWSPHeader
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* CreateWSPHeader( VOID* pvHeader, int iHeadLen,
		unsigned long int uiLastMod, int iReload, unsigned char ucMethod,
		VOID* pvReqStruct, long int* pliHeaderLength );





/*========================================================================
	GetCacheControl 
==========================================================================
Purpose: Get the CacheControl value from the Header (MBM-Buffer)
Params:  pbuHeader	buffhead*	MBM-Buffer
Return:  cachecontrol value.
=========================================================================*/
unsigned char GetCacheControl( void* pHeaderHandle );

/*========================================================================
	GetURLfromLocation 
==========================================================================
Purpose: Get the Location value from the Header (MBM-Buffer) and store the 
		 Location in pvUrl
Params:  pbuHeader		buffhead*	MBM-Buffer
		 pvUrl			BYTE**		casted to VOID**
		 liUrlLength	Long Int*	the length of pvUrl
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int GetURLfromLocation( void* pvHeader, int iHeaderLength, VOID** pvUrl, long int* liUrlLength );

/*========================================================================
	GetHeaderHandle
==========================================================================
Purpose: Create a HeaderHandle from a Bytestream
Params:  pvHeader	BYTE*	casted to a VOID*
		 iHeadLen	Int		length of pvHeader
Return:  pHEADERDEF casted to a VOID*.
=========================================================================*/
int GetHeaderHandle( void** ppHeaderHandle, VOID* pvHeader, int iHeadLen, int iRedirect,
							VOID* pvUrl );

/*========================================================================
	FreeVoidStar 
==========================================================================
Purpose: Deallocate a pointer
Params:  pvBuffer	VOID**
Return:  
=========================================================================*/
VOID FreeVoidStar( VOID** pvBuffer );

/*========================================================================
	getAddrBearer 
==========================================================================
Purpose: Get the Bearer part from the NETWORKSTRUCT
Params:  pNetStruct	NETWORKSTRUCT*	casted to VOID*
Return:  Bearer
=========================================================================*/
unsigned char getAddrBearer( VOID* pNetStruct );

/*========================================================================
	getAddrSecAddrFormat 
==========================================================================
Purpose: Get the SecondAddressFormat part from the NETWORKSTRUCT
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
Return:  SecondAddressFormat
=========================================================================*/
unsigned char getAddrSecAddrFormat( VOID* pNetStruct, int iServerSet );

/*========================================================================
	getAddrAddress 
==========================================================================
Purpose: Create a OctetString with the Address part from the NETWORKSTRUCT
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
		 liLength		Long Int*		length of the OctetString
Return:  The created OctetString
=========================================================================*/
void getAddrAddress( VOID* pNetStruct, int iServerSet, DeviceAddress* pAddress );

/*========================================================================
	getAddrSecondAddress 
==========================================================================
Purpose: Create a OctetString with the SecondAddress part from the NETWORKSTRUCT
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
		 liLength		Long Int*		length of the OctetString
Return:  The created OctetString
=========================================================================*/
void getAddrSecondAddress( VOID* pNetStruct, int iServerSet, 
						  AuxiliaryAddress* pSecondAddress );

/*========================================================================
	getTimerValue 
==========================================================================
Purpose: Get the TimeOut part from the WAEMAINOBJECT
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  TimeOut
=========================================================================*/
int getTimerValue( VOID* pNetStruct );

/*========================================================================
	getStackMode 
==========================================================================
Purpose: Get the StackMode part from the WAEMAINOBJECT
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  StackMode
=========================================================================*/
int getStackMode( VOID* pNetStruct );

/*========================================================================
	getOTAMode 
==========================================================================
Purpose: Get the OTAIsAllowed part from the WAEMAINOBJECT
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  OTAIsAllowed
=========================================================================*/
int getOTAMode( VOID* pvWAEMaStruct );

/*========================================================================
	GetFile 
==========================================================================
Purpose: Invokes the adapter function CLNTa_getFile with a URL from 
		 pvRequestStruct and also dealloc the pvRequestStruct
Params:  iTID				Int
		 pvRequestStruct	pURLREQSTRUCT*	casted to VOID**
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int GetFile( int iTID, VOID** pvReqStruct );

/*========================================================================
	checkIfImageRequest 
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int checkIfImageRequest( void* pvReqStruct );

/*========================================================================
	changeSessionParam
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int changeSessionParam( VOID* pvOldNetwork, VOID* pvNewNetwork, 
	VOID* pvOldUaSettings, VOID* pvNewUaSettings );

/*========================================================================
	getNrOfFreeReqstruct 
==========================================================================
Purpose: Get the iNrOfFreeRequestProc parameter from WAEMAINOBJECT.
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  iNrOfFreeRequestProc
=========================================================================*/
int getNrOfFreeReqstruct( VOID* pvWAEMaStruct );

/*========================================================================
	InitNrOfFreeTrProc
==========================================================================
Purpose: Set the iNrOfFreeRequestProc parameter in WAEMainObject
Params:  pvWAEMainObject	pWAEMAINOBJECT	casted to a VOID*
		 iMaxTrProc			Int				MaxNumberOfRequests
Return:  
=========================================================================*/
void InitNrOfFreeTrProc( VOID* pvWAEMainObject, int iMaxTrProc );

/*========================================================================
	decNrOfFreeReqstruct 
==========================================================================
Purpose: Decrease the iNrOfFreeRequestProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void decNrOfFreeReqstruct( VOID* pvWAEMaStruct );

/*========================================================================
	incNrOfFreeReqstruct 
==========================================================================
Purpose: Increase the iNrOfFreeRequestProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void incNrOfFreeReqstruct( VOID* pvWAEMaStruct );

/*========================================================================
	InitNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void InitNrOfFreeSession( VOID* pvWAEMainObject, int iMaxSessionProc );

/*========================================================================
	decNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void decNrOfFreeSession( VOID* pvWAEMaStruct );

/*========================================================================
	incNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void incNrOfFreeSession( VOID* pvWAEMaStruct );

/*========================================================================
	getNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int getNrOfFreeSession( VOID* pvWAEMaStruct );





/*========================================================================
	pidList_AddPid
=========================================================================*/
int pidList_AddPid( void* pList, int iId, unsigned long int uiPid );

/*========================================================================
	pidList_AddPid_Resend
=========================================================================*/
int pidList_AddPid_Resend( void* pList, int iId, unsigned long int uiPid, int iResend );

/*========================================================================
	pidList_SetSessionID 
=========================================================================*/
int pidList_SetSessionID( void* pList, int iId );

/*========================================================================
	pidList_ClearSessionID 
=========================================================================*/
int pidList_ClearSessionID( void* pList, unsigned long int uiPid );

/*========================================================================
	pidList_GetPid 
=========================================================================*/
unsigned long int pidList_GetPid( void* pList, int iId, int iRemove );

/*========================================================================
	pidList_GetPid_Resend 
=========================================================================*/
unsigned long int pidList_GetPid_Resend( void* pList, int iId );

/*========================================================================
	pidList_RemovePid 
=========================================================================*/
int pidList_RemovePid( void* pList, unsigned long int uiPid );

/*========================================================================
	pidList_GetFirstPid 
=========================================================================*/
unsigned long int pidList_GetFirstPid( void* pList );

/*========================================================================
	pidList_GetPidFromElement 
=========================================================================*/
unsigned long int pidList_GetPidFromElement( void* pList, void** ppListElement );

/*========================================================================
	pidList_Delete 
=========================================================================*/
int pidList_Delete( void** pList );

/*========================================================================
	releaseExtraTids 
=========================================================================*/
void releaseExtraTids( void* pvWAEMaStruct, void* pList );

/*========================================================================
	TR_List_Init 
==========================================================================
Purpose: Initialize the Transaction-queue
Params:  iSize	Int		the max number of element in queue, if NULL, no limit.
Return:  pointer to the queue
=========================================================================*/
void* TR_List_Init( int iSize );

/*========================================================================
	TR_List_Count 
==========================================================================
Purpose: Count how many element there is in the Transaction-queue
Params:  pvTRList	void*	pointer to the queue
Return:  number of elements
=========================================================================*/
int TR_List_Count( void* pvTRList );

/*========================================================================
	TR_List_Add 
==========================================================================
Purpose: Insert (priority, highest first) a new element in the Transaction-queue
Params:  pvTransList	void*	pointer to the queue
		 iTID			Int
		 pvReqStruct	void*	pointer to a URLREQSTRUCT
		 iReload		Int
		 iPrio			Int		Priority
		 pvNetStruct	void*	pointer to a NETWORKSTRUCT
		 iStackMode		Int
		 iPid			Int		index mapped to a SDL-Pid
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_Add( void* pvTransList, int iSID, int iTID, void* pvReqStruct, 
			int iReload, int iPrio, unsigned long int uiPid );

/*========================================================================
	TR_List_GetNext 
==========================================================================
Purpose: Get the next element from the Transaction-queue and delete this
			element from the queue.
Params:  pvTransList	void*		pointer to the queue
		 liTID			Long Int*
		 pvReqStruct	void**		pointer to a URLREQSTRUCT
		 liReload		Long Int*
		 liPrio			Long Int*	Priority
		 pvNetStruct	void**		pointer to a NETWORKSTRUCT
		 liStackMode	Long Int*
		 liPid			Long Int*	index mapped to a SDL-Pid
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_GetNext( void* pvTransList, long int* liSID, long int* liTID, 
	void** pvReqStruct, long int* liReload,	long int* liPrio, unsigned long int* uiPid );

/*========================================================================
	TR_List_GetNextSIDTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int TR_List_GetNextSIDTrans( void* pvTransList, int iSID, long int* liTID, 
	void** pvReqStruct, long int* liReload,	long int* liPrio, unsigned long int* uiPid );


/*========================================================================
	TR_List_DeleteTransactions
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int TR_List_DeleteTransactions( void* pvTransList, int iSID );

/*========================================================================
	TR_List_Delete 
==========================================================================
Purpose: Delete the hole Transaction-queue and each elements datastructure
Params:  pvTransList	void**		pointer to the queue
Return:  TRUE
=========================================================================*/
int TR_List_Delete( void** pvTransList );

/*========================================================================
	SE_List_Init
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* SE_List_Init( int iSize );

/*========================================================================
	SE_List_Add
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_Add( void* pvSessionList, int iSID, int iMaxNrOfTrans );

/*========================================================================
	SE_List_CheckConnection
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_CheckConnection( void* pvSessionList, int iSID );

/*========================================================================
	SE_List_CheckNrOfFreeTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_CheckNrOfFreeTrans( void* pvSessionList, int iSID );

/*========================================================================
	SE_List_ChangeConnection
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_ChangeConnection( void* pvSessionList, int iSID, int iConnected );

/*========================================================================
	SE_List_ChangeNrOfFreeTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_ChangeNrOfFreeTrans( void* pvSessionList, int iSID, int iIncDec );

/*========================================================================
	SE_List_SetNrOfFreeTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_SetNrOfFreeTrans( void* pvSessionList, int iSID, int iMaxNrOfTrans );

/*========================================================================
	SE_List_DeleteSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_DeleteSession( void* pvSessionList, int iSID );

/*========================================================================
	SE_List_Delete
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_Delete( void** pvSessionList );




/*========================================================================
				FUNCTIONS ONLY USED IN CONNECTIONORIENTED
=========================================================================*/

#ifdef CONFIG_CO

/*========================================================================
	CreateConnectHeader
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* CreateConnectHeader( VOID* pvStruct, long int* pliHeaderLength );

/*========================================================================
	CreateCapabilities
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* CreateCapabilities( int iMaxNrOfTrans, long int* pliCapabilLen );

/*========================================================================
	ParseCapabilities
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int ParseCapabilities( VOID* pvNegotiatedCapabilities, int iLen);


/*========================================================================
	ParseRedirectAddress
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int ParseRedirectAddress( void* pvRedirectAddress, int iAddressLength,
						 void* pNetStruct, unsigned long int* uiPort );

#endif



#endif
