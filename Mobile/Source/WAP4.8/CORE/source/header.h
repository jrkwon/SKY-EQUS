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

#include "wipprdct.h"
#include "cmmnrsrc.h"
#include "headdef.h"
#include "wipdb.h"

#if defined XSCT_CMICRO
    #include "ml_typw.h"
#else
    #include "scttypes.h"
#endif

#include "glbldecl.h"

#ifdef CONFIG_PUSH
/*************************/
/* Push Multipart values */
/*************************/
	#define No_Multipart 0
	#define Multipart_Mixed 1
	#define Multipart_Related 2
	#define Multipart_Alternative 3
/*************************/
/* Connectivity value (used at the same place as the Multipart values) */
/*************************/
	#define Connectivity 4
#endif

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


/* --- Internall CacheDirectives --- */
#define CACHE_CONTENT			0
#define UPDATE_CACHE_CONTENT	1
#define NOT_CACHE_CONTENT		2
/* --- Internall CacheDirectives --- */

#define ENCODING_VERSION_1_2	0x12
#define ENCODING_VERSION_1_3	0x13
#define ENCODING_VERSION_1_4	0x14

#define TR_LIST_WAIT_FOR_CHANNEL	1
#define TR_LIST_WAIT_FOR_E2E		2


#define SKT_CLIENT_HELLO	1
#define SKT_SERVER_HELLO	2
#define SKT_KEY_EXCHANGE	3
#define SKT_FINISHED		4

#define SKT_PORT_NR				"9091"
#define SKT_CONTENTTYPE_WML		"application/vnd.wap.skt_e2e_wml"
#define SKT_CONTENTTYPE_WMLS	"application/vnd.wap.skt_e2e_wmls"

typedef struct
{
	BYTE*	pbAccept;
	BYTE*	pbProxyCredentials;
	int		iProxyCredentialsLength;
	BYTE*	pbLanguage;
	UINT8	iLanguage_Length;
} UASETTINGS, *pUASETTINGS;

typedef struct
{
	int		iStackMode;
	int		iActiveBearer;
	int		iClientPort;
	BYTE*	pbUDP_CLIENT_IP;
	UINT8	pbUDP_CLIENT_IP_LEN;
	BYTE*	pbUDP_GW_IP;
	UINT8	pbUDP_GW_IP_LEN;
	BYTE*	pbSMS_CENTER;
	UINT8	pbSMS_CENTER_LEN;
	BYTE*	pbSMS_GW;
	UINT8	pbSMS_GW_LEN;
	BYTE*	pbUSSD_CENTER;
	UINT8	pbUSSD_CENTER_LEN;
	int		iUSSD_SC_TYPE;
	BYTE*	pbUSSD_SC;
	UINT8	pbUSSD_SC_LEN;
    UINT16	iTimeOut;
} NETWORKSTRUCT, *pNETWORKSTRUCT;

NETWORKSTRUCT* New_NETWORKSTRUCT(void);
void Delete_NETWORKSTRUCT(NETWORKSTRUCT** ppTheObject);

UASETTINGS* New_UASETTINGS(void);
void Delete_UASETTINGS(UASETTINGS** ppTheObject);


/*========================================================================
	FUNCTION PROTOTYPES
=========================================================================*/

/*========================================================================
	urlIsValid
==========================================================================
Purpose: Check if a URL is valid and has at least a scheme, host, and 
		 path component.
Params:  pvReqStruct	pURLREQSTRUCT	casted to a VOID*
Return:  TRUE if valid, otherwise FALSE
=========================================================================*/
int urlIsValid( VOID* pvReqStruct );

/*========================================================================
	FreeReqStruct 
==========================================================================
Purpose: Release a URLREQSTRUCT and all of it's parts
Params:  pvReqStruct	pURLREQSTRUCT*	casted to a VOID**
Return:	 1
=========================================================================*/
int FreeReqStruct( VOID** pvReqStruct );

/*========================================================================
	FreeNetStruct 
==========================================================================
Purpose: Release a NETWORKSTRUCT and all of it's parts
Params:  vsNetStruct	pNETWORKSTRUCT*	casted to a VOID**
Return:	 1
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
Purpose: Create a WSP-Header, pvHeader, from the input parameters.
Params:  pvReqStruct				URLREQSTRUCT*	casted to VOID*
		 pvUaSettings				UASETTINGS*		casted to VOID*
		 pNetStruct					NETWORKSTRUCT*	casted to VOID*
		 pvWAEMaStruct				WAEMAINOBJECT*	casted to VOID*
		 iConnectionMode			int (0=CL, 1=CM)
		 iProfileEnabled			int (1=UAProf is enabled)
		 vsProxyCredentials			BYTE*			casted to VOID*
		 iProxyCredentialsLength	int
		 pvHeader					BYTE**			casted to VOID**
Return:  length of pvHeader
=========================================================================*/
int GetHeader( VOID* pvReqStruct, VOID* pvUaSettings, VOID* pNetStruct, VOID* pvWAEMaStruct,
			  int iConnectionMode, int iProfileEnabled, int iEncodingVersion, VOID* vsProxyCredentials,
			  int iProxyCredentialsLength, VOID** pvHeader );

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
Purpose: Add if required Post and Reload headers to an existing WSP-header. 
Params:  pvHeader			BYTE*			casted to VOID*
		 iHeadLen			int				length of pvHeader
		 iReload			int				1=Reload
		 ucMethod			unsigned char	Get or Post
		 pvReqStruct		URLREQSTRUCT*	casted to VOID*
		 pliHeaderLength	long int*		length of the hole WSP-header
Return:  pointer to the WSP-header
=========================================================================*/
void* CreateWSPHeader( VOID* pvHeader, int iHeadLen,
		int iReload, unsigned char ucMethod, int iEncodingVersion,
		VOID* pvReqStruct, long int* pliHeaderLength );

/*========================================================================
	ExamineCacheInfo 
==========================================================================
Purpose: Examine the content. If it is Multipart this function is invoked 
		 recursive, stored every multipart part in the cache, returns the 
		 first content part to the UserAgent.
Params:  pHeaderHandle			pHEADERDEF*
		 iUaType				int
		 iCacheInIdle			int
		 iCacheDirektive		int
		 pvReqStruct			URLREQSTRUCT*		casted to void*
		 pvHeader				BYTE**				casted to void**
		 pliHeadLen				long int*
		 pvBody					BYTE**				casted to void**
		 pliBodyLen				long int*
		 iFirstWrite			int*
		 iMultipart				int
Return:  1, if the content is a Multipart and there are some errors in the 
		 Multpart, 0 is returned.
=========================================================================*/
int ExamineCacheInfo( pHEADERDEF *pHeaderHandle, int iUaType, int iCacheInIdle,
                      int iCacheDirektive, void* pvReqStruct, void** pvHeader,
                      long int* pliHeadLen, void** pvBody, long int* pliBodyLen,
                      int* iFirstWrite, int iMultipart );

/*========================================================================
	GetURLfromLocation 
==========================================================================
Purpose: Get the Location value from the pvHeader and store the Location in pvUrl
Params:  pvHeader		BYTE*		casted to void*
		 iHeaderLength	int
		 pvUrl			BYTE**		casted to VOID**
		 liUrlLength	long int*
Return:  1 if success, otherwise 0
=========================================================================*/
int GetURLfromLocation( void* pvHeader, int iHeaderLength, VOID** pvUrl, long int* liUrlLength );

/*========================================================================
	GetHeaderHandle
==========================================================================
Purpose: Create a HeaderHandle from a Bytestream
Params:  ppHeaderHandle	pHEADERDEF*	casted to void**
		 pvHeader		BYTE*		casted to a VOID*
		 iHeadLen		int
		 iRedirect		int
		 pvUrl			BYTE*		casted to VOID*
Return:  1 if success, otherwise 0
=========================================================================*/
int GetHeaderHandle( void** ppHeaderHandle, VOID* pvHeader, int iHeadLen, int iRedirect,
							VOID* pvUrl );

/*========================================================================
	manageCookies
==========================================================================
Purpose: Retrieves cookies from the list of headers and requests the
         database to store these.
Params:  pHeaderHandle which is the list of headers,
         pbURL, i.e. the request url which cookies are checked against
         before storage
Return:  -
=========================================================================*/
void manageCookies( void* pHeaderHandle, BYTE* pbURL );

/*========================================================================
	createCookieData
==========================================================================
Purpose: Retrieves matching coookies previously searched for in the DB.
         The retrieved cookies are encoded according to WSP and put
         in *ppbString. The resulting *ppbString is to be used as the
         value part of a cookie header.
Params:  ppbString is an outparam. ppbString must not be NULL.
         *ppbString is a string allocated by this function but its
         delete responsibility is passed to the caller upon return.
         I.e the caller must delete *ppbString.
         If no matching cookies were found, *ppbString will be set to 
         NULL and 0 will be returned.
Return:  The length of the *ppbString.
=========================================================================*/
UINT32 createCookieData( BYTE** ppbString );

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
		 iServerSet		int				1 (Serverpart), 0 (Clientpart)
Return:  SecondAddressFormat
=========================================================================*/
unsigned char getAddrSecAddrFormat( VOID* pNetStruct, int iServerSet );

/*========================================================================
	getAddrAddress 
==========================================================================
Purpose: Take the Address from the NETWORKSTRUCT and stored it in DeviceAddress
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
		 pAddress		DeviceAddress*
Return:  
=========================================================================*/
void getAddrAddress( VOID* pNetStruct, int iServerSet, DeviceAddress* pAddress );

/*========================================================================
	getProxyAddress 
==========================================================================
Purpose: Take the Address from the NETWORKSTRUCT and stored it in pAddress
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 pAddress		BYTE**			
		 iAddressLength	UINT8*
Return:  
=========================================================================*/
void getProxyAddress( VOID* pNetStruct, BYTE** pAddress, UINT8* iAddressLength );

/*========================================================================
	getAddrSecondAddress 
==========================================================================
Purpose: Take the Address from the NETWORKSTRUCT and stored it in AuxiliaryAddress
Params:  pNetStruct			NETWORKSTRUCT*		casted to VOID*,
		 iServerSet			Int					1 (Serverpart), 0 (Clientpart)
		 pSecondAddress		AuxiliaryAddress*	
Return: 
=========================================================================*/
void getAddrSecondAddress( VOID* pNetStruct, int iServerSet, 
						  AuxiliaryAddress* pSecondAddress );

/*========================================================================
	getTimerValue 
==========================================================================
Purpose: Get the TimeOut part from the NETWORKSTRUCT
Params:  pNetStruct	NETWORKSTRUCT*	casted to VOID*
Return:  TimeOut value in 1/10 of a second.
=========================================================================*/
int getTimerValue( VOID* pNetStruct );

/*========================================================================
	getStackMode 
==========================================================================
Purpose: Get the StackMode part from the NETWORKSTRUCT
Params:  pNetStruct	NETWORKSTRUCT*	casted to VOID*
Return:  StackMode
=========================================================================*/
int getStackMode( VOID* pNetStruct );

int getClientPort( VOID* pNetStruct );

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
Purpose: Check in the pvReqStruct if the request expect an image in the response.
Params:  pvReqStruct	URLREQSTRUCT*	casted to void*
Return:  1 if an image, otherwise 0
=========================================================================*/
int checkIfImageRequest( void* pvReqStruct );


void*  getMyPtrFromMem( DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode, UINT16* uiLength );

int setNetworkParameters( VOID* pvWAEMaStruct, VOID* pvNetwork, VOID* pvUaSettings, int iObjectID, int iChannelRef, int iConnectionOriented );

/*========================================================================
	GetRealm
==========================================================================
Purpose: Get the Realm value from the pvHeader.
Params:  pvHeader		BYTE*		casted to void*
		 iHeaderLength	int
Return:  Pointer to a Realm
=========================================================================*/
void* GetRealm( void* pvHeader, int iHeadLen );

/*========================================================================
	getContentTypeByte
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
BYTE getContentTypeByte( void* pvHeaderHandle );

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
	getNrOfFreeSession
==========================================================================
Purpose: Get the iNrOfFreeSessionProc parameter from WAEMAINOBJECT.
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  iNrOfFreeSessionProc
=========================================================================*/
int getNrOfFreeSession( VOID* pvWAEMaStruct );


/*========================================================================
	InitNrOfFreeSession
==========================================================================
Purpose: Set the iNrOfFreeSessionProc parameter in WAEMainObject
Params:  pvWAEMainObject	pWAEMAINOBJECT	casted to a VOID*
		 iMaxSessionProc	Int				MaxNumberOfSessions
Return:  
=========================================================================*/
void InitNrOfFreeSession( VOID* pvWAEMainObject, int iMaxSessionProc );

/*========================================================================
	decNrOfFreeSession
==========================================================================
Purpose: Decrease the iNrOfFreeSessionProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void decNrOfFreeSession( VOID* pvWAEMaStruct );

/*========================================================================
	incNrOfFreeSession
==========================================================================
Purpose: Increase the iNrOfFreeSessionProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void incNrOfFreeSession( VOID* pvWAEMaStruct );


/*========================================================================
	Functions to handeling PID for SDL-Processes
=========================================================================*/

/*========================================================================
	pidList_AddPid
=========================================================================*/
int pidList_AddPid( void* pList, int iId, unsigned long int uiPid );

/*========================================================================
	pidList_AddPid_Resend
=========================================================================*/
/*int pidList_AddPid_Resend( void* pList, int iId, unsigned long int uiPid, int iResend );*/

/*========================================================================
	pidList_SetSessionID 
=========================================================================*/
/*int pidList_SetSessionID( void* pList, int iId );*/

/*========================================================================
	pidList_ClearSessionID 
=========================================================================*/
/*int pidList_ClearSessionID( void* pList, unsigned long int uiPid );*/

/*========================================================================
	pidList_GetPid 
=========================================================================*/
unsigned long int pidList_GetPid( void* pList, int iId, int iRemove );

/*========================================================================
	pidList_GetPid_Resend 
=========================================================================*/
/*unsigned long int pidList_GetPid_Resend( void* pList, int iId );*/

/*========================================================================
	pidList_RemovePid 
=========================================================================*/
/*int pidList_RemovePid( void* pList, unsigned long int uiPid );*/

/*========================================================================
	pidList_GetFirstPid 
=========================================================================*/
/*unsigned long int pidList_GetFirstPid( void* pList );*/

/*========================================================================
	pidList_GetPidFromElement 
=========================================================================*/
/*unsigned long int pidList_GetPidFromElement( void* pList, void** ppListElement );*/

/*========================================================================
	pidList_Delete 
=========================================================================*/
int pidList_Delete( void** pList );

/*========================================================================
	releaseExtraTids 
=========================================================================*/
/*void releaseExtraTids( void* pvWAEMaStruct, void* pList );*/

/*========================================================================
	TR_List_Add 
==========================================================================
Purpose: Insert (priority, highest first) a new element in the Transaction-queue
Params:  pvTransList	void*					pointer to the queue
		 iSID			Int
		 iTID			Int
		 pvReqStruct	void*					pointer to a URLREQSTRUCT
		 iOptions		Int
		 iPrio			Int						Priority
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_Add( void* pvTransList, int iObjectID, int iSID, int iTID, void* pvReqStruct, 
			int iOptions, int iPrio, unsigned long int uiPid, char chWaitingFor );

/*========================================================================
	TR_List_CheckForWaitingForConn 
==========================================================================
Purpose: 
Params:  
Return: 
=========================================================================*/
int TR_List_CheckForWaitingForConn( void* pvTransList, int iObjectID );

/*========================================================================
	TR_List_ClearWaitingForE2E 
==========================================================================
Purpose: 
Params:  
Return: 
=========================================================================*/
int TR_List_ClearWaitingForE2E( void* pvTransList, int iTID );

/*========================================================================
	TR_List_ClearWaitingForConn 
==========================================================================
Purpose: 
Params:  
Return: 
=========================================================================*/
int TR_List_ClearWaitingForConn( void* pvTransList, int iObjectID );

/*========================================================================
	TR_List_GetNextObjectTrans
==========================================================================
Purpose:
Params:
Return:
=========================================================================*/
int TR_List_GetNextObjectTrans( void* pvTransList, int iObjectID, long int* liTID, 
	void** pvReqStruct, unsigned long int* uiPid );

/*========================================================================
	TR_List_GetNext 
==========================================================================
Purpose: Get the next element from the Transaction-queue and delete this
			element from the queue.
Params:  pvTransList	void*					pointer to the queue
		 liSID			Long Int*
		 liTID			Long Int*
		 pvReqStruct	void**					pointer to a URLREQSTRUCT
		 liOptions		Long Int*
		 liPrio			Long Int*				Priority
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_GetNext( void* pvTransList, long int* liObjectID, long int* liSID, long int* liTID, 
	void** pvReqStruct, long int* liOptions,	long int* liPrio, unsigned long int* uiPid );

/*========================================================================
	TR_List_GetNextSIDTrans
==========================================================================
Purpose: Get the next element from the Transaction-queue with a specific iSID 
			and delete this	element from the queue.
Params:  pvTransList	void*					pointer to the queue
		 iSID			Int
		 liTID			Long Int*
		 pvReqStruct	void**					pointer to a URLREQSTRUCT
		 liOptions		Long Int*
		 liPrio			Long Int*				Priority
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_GetNextSIDTrans( void* pvTransList, int iObjectID, int iSID, long int* liTID, 
	void** pvReqStruct, long int* liOptions,	long int* liPrio, unsigned long int* uiPid );

int TR_List_CheckForSIDTrans( void* pvTransList, int iObjectID, int iSID );

	
/*========================================================================
	TR_List_DeleteTID
==========================================================================
Purpose: Delete an element from the Transaction-queue with a specific iTID.
Params:  pvTransList	void*					pointer to the queue
		 iTID			Int
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if the element was found, otherwise FALSE
=========================================================================*/
int TR_List_DeleteTID( void* pvTransList, int iTID, unsigned long int* uiPid,
					  long int* liObjectID, long int* liSID );

/*========================================================================
	TR_List_DeleteTransactions
==========================================================================
Purpose: Delete elements from the Transaction-queue with a specific iSID.
Params:  pvTransList	void*					pointer to the queue
		 iSID			Int
Return:  TRUE
=========================================================================*/
/*int TR_List_DeleteTransactions( void* pvTransList, int iObjectID, int iSID );*/

/*========================================================================
	TR_List_Delete 
==========================================================================
Purpose: Delete the hole Transaction-queue and each elements datastructure
Params:  pvTransList	void**		pointer to the queue
Return:  TRUE
=========================================================================*/
int TR_List_Delete( void** pvTransList );

/*========================================================================
	SE_List_Add
==========================================================================
Purpose: Add a SID to the sessionList, iMaxNrOfTrans defines how many 
			transactions a session can simultaneous handle.
=========================================================================*/
int SE_List_Add( void* pvSessionList, unsigned long int uiPid, int iObjectID, int iSID, int iMaxNrOfTrans );

/*int SE_List_SetID( void* pvSessionList, int iObjectID, int iSID );*/

unsigned long int SE_List_GetPid( void* pvSessionList, int iObjectID, int iSID );

unsigned long int SE_List_GetFirstPid( void* pvSessionList );

int SE_List_DeleteSession( void* pvSessionList, unsigned long int uiPid, long int* iSID );

/*========================================================================
	SE_List_CheckConnection
==========================================================================
Purpose: Check if a specific Session is connected to the server.
Return:  Return 1 if connected, otherwise 0
=========================================================================*/
int SE_List_CheckConnection( void* pvSessionList, int iObjectID, int iSID );

/*========================================================================
	SE_List_CheckNrOfFreeTrans
==========================================================================
Purpose: Check if a session can handle a new transaction, retuns 1 if OK, 
			otherwise 0.
=========================================================================*/
int SE_List_CheckNrOfFreeTrans( void* pvSessionList, int iObjectID, int iSID );

/*========================================================================
	SE_List_ChangeConnection
==========================================================================
Purpose: Change the connection state to a specific session.
=========================================================================*/
int SE_List_ChangeConnection( void* pvSessionList, int iObjectID, int iSID, int iConnected );

/*========================================================================
	SE_List_ChangeNrOfFreeTrans
==========================================================================
Purpose: Change the number of Free transactions to a specific session.
=========================================================================*/
int SE_List_ChangeNrOfFreeTrans( void* pvSessionList, int iObjectID, int iSID, int iIncDec );

/*========================================================================
	SE_List_SetNrOfFreeTrans
==========================================================================
Purpose: Set the maximum number of transactions a session can simultaneous
			handle.
=========================================================================*/
int SE_List_SetNrOfFreeTrans( void* pvSessionList, int iObjectID, int iSID, int iMaxNrOfTrans );

/*========================================================================
	SE_List_DeleteSession
==========================================================================
Purpose: Deletes a specific session struct from the list.
=========================================================================*/
/*int SE_List_DeleteSession( void* pvSessionList, int iSID );*/

/*========================================================================
	SE_List_Delete
==========================================================================
Purpose: Deletes the hole list.
=========================================================================*/
int SE_List_Delete( void** pvSessionList );

#ifdef CONFIG_PUSH

int SE_List_Find_An_Object_For_Push( void* pvSessionList );

int Check_If_Multipart_Push( void* vsPushHeader, UINT32 iLen, void** vsEntityList, UINT16 *iSEC, void** vsMAC );

void* Get_Multipart_Url(void* vsHeaderHandle);

void* PreParse_Multipart_Body(void* vsBody, int iBodyLen);

int Check_If_Entity_Is_PushMessage(void* vsEntityList, void* vsHeaderHandle, int iCacheInIdle, void** vsEntityHeader, UINT32 *iHeaderLength, void** vsEntityBody, UINT32 *iBodyLen);

void Write_To_Cache_If_Ok(pHEADERDEF pHeaderDef, void* vsHeaderHandle, int iCacheInIdle, pMULTIPARTSTR	pMultiPartList);

BYTE* Get_Url(pHEADERDEF pHeaderDef);

void* Get_Next_Entity(void* vsEntityList, void* vsHeaderHandle, long int *iTemp);

#endif

int ACT_TR_List_Add( void* pvList, int iTID, unsigned long int uiSM_Pid );

int ACT_TR_List_SetTR_Pid( void* pvList, int iTID, unsigned long int uiTR_Pid );

unsigned long int ACT_TR_List_GetPid( void* pvList, int iTID, int iSM_TR_pid );

int ACT_TR_List_DeleteTID( void* pvList, int iTID );

unsigned long int ACT_TR_List_GetPidFromElement( void* pvList, unsigned long int uiSM_Pid,
												void** ppListElement );

int ACT_TR_List_Reomve_Pid( void* pvList, unsigned long int uiTR_Pid );

int ACT_TR_List_Delete( void** pvList );

/*========================================================================
	ParseRedirectAddress
==========================================================================
Purpose: Parse the Redirect Address and stores the new Address in the 
			Network struct
Params:  pvRedirectAddress	BYTE*				casted to void*
		 iAddressLength		int
		 pNetStruct			NETWORKSTRUCT*		casted to void*
		 uiPort				unsigned long int*
Return:  1 if success, otherwise 0.
=========================================================================*/
int ParseRedirectAddress( /*void* pvRedirectAddress, int iAddressLength,
						 void* pNetStruct, unsigned long int* uiPort, int iChannelRef*/
						 void* pvRedirectAddress, int iAddressLength,
						  void** pvAddressList );




/*========================================================================
				FUNCTIONS ONLY USED IN CONNECTIONORIENTED
=========================================================================*/

#ifdef CONFIG_CO

/*========================================================================
	CreateConnectHeader
==========================================================================
Purpose: Create a ConnectHeader that you can send to the Gateway.
Params:  pvStruct			UASETTINGS*		casted to void*
		 pNetStruct			NETWORKSTRUCT*	casted to void*
		 pvWAEMaStruct		WAEMAINOBJECT*	casted to VOID*
		 pliHeaderLength	long int*
Return:  Pointer to the ConnectHeader
=========================================================================*/
void* CreateConnectHeader( VOID* pvStruct, VOID* pNetStruct, 
						  VOID* pvWAEMaStruct, long int* pliHeaderLength );

/*========================================================================
	CheckServerHeaders
==========================================================================
Purpose: Check if the gateway supports UAProf features.
Params:  pvServerHeaders	BYTE*	casted to void*
		 iLen				int
Return:  1 if the Gateway support UAProf, otherwise 0.
=========================================================================*/
int CheckServerHeaders( VOID* pvServerHeaders, int iLen, long int* pliEncodingVersion );

#endif



#endif
