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
#ifndef _Push_H
#define _Push_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: push.h

	Description:

	Author: Martin Ranstorp, Kent Olsson AU-System Radio AB

	Revision history:
  	Date    Rev  Sign  Comment
  	990920  PA1  MRP   First version.
	991206  PA2  MRP   Updated. Working in WAE test environment.
	000118	PA3  MRP   Updated after validation inspection.
	000124	PA4  MRP   Updated after code inspection.
	000208	PA5	 JPR   Function ME_Push_DateTime2Int moved to PUSHElm.h
	000214  PA6  MRP   Copyright addded.
	000316	PA7	 MRP   Push API changed.
	000918  PA8  MRP   Updated configuration handling (4.x).
	001114	PA9	 KEOL  Uppdated after change request regarding handling of GetSI and GetSL (reply with initiators URI as well) 	
	001220  PA10 HEAD  Added ME_Push_Get_First=Get first element in the list,
	                   ME_Push_Get_Next=get next element in list, and
					   ME_Push_Get_PushListStruct=get content of the list element.

=========================================================================*/

#include "cmmnrsrc.h"
#include "wae_cmmn.h"
#include "wmlelm.h"
#include "p_contif.h"
#include "pushdef.h"

/************************************/
/* Service Indication Content type  */
/************************************/
typedef struct
{
	UINT8	iStatus;
	UINT32	iCreated;
	UINT32	iExpires;
	UINT8	iAction;
	WCHAR*	pwchSIid;
	WCHAR*	pwchMessage;
	BYTE*	pbUrl;
	BYTE*   pbInitUri;								

} SICONTENTSTRUCT, *pSICONTENTSTRUCT;

/************************************/
/* Service Loading Content type     */
/************************************/
typedef struct
{
	UINT8	iStatus;
	UINT8	iAction;
	BYTE*	pbUrl;
	BYTE*   pbInitUri;
	UINT32	iReceived;

} SLCONTENTSTRUCT, *pSLCONTENTSTRUCT;

typedef struct
{
	UINT8	iType;
	UINT8   iPortType;
	UINT32	iId;
    UINT16  iIdNumber;
	UINT8	iNewChannelId;

	union {
		SICONTENTSTRUCT*	pSIContentStruct;
		SLCONTENTSTRUCT*	pSLContentStruct;
	} content;
} PUSHCONTENTSTRUCT, *pPUSHCONTENTSTRUCT;

typedef struct
{
	UINT8	iPushType;						
	UINT8	iStatus;
	UINT32  iCreated;
	UINT32  iExpires;
	UINT8	iAction;
	UINT32	iLLPushId;
	WCHAR*	pwchPushId;
	UINT8	iChannel;
	UINT16	iIdNumber;
	UINT8	iPortType;

} PUSHLISTSTRUCT, *pPUSHLISTSTRUCT;

typedef struct
{
	UINT8	iChannel;
	UINT16	iClientPushId;
	UINT8	iPortType;
	BOOL	bConfirm;
	VOID*	pPushBody;
	UINT32	iBodyLen;
	VOID*	pPushHeader;
	UINT32	iHeaderLen;

} SIGNALSTRUCT, *pSIGNALSTRUCT;

typedef struct
{
	UINT8	iSIAId;
	UINT8	iChannel;

} SIASTRUCT, *pSIASTRUCT;

/***********************/
/* Push Type values    */
/***********************/
#define SI 1
#define SL 2
#define CO 3
#define SIA 4

/***********************/
/* Push Status values  */
/***********************/
#define PUSH_NEW 1
#define PUSH_STORED 2
#define PUSH_ACTIVE 3
#define PUSH_POSTPONED 4
#define PUSH_POSTPONED_READ 5


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
	FUNCTION PROTOTYPES
=========================================================================*/

/*========================================================================
	ME_Push_GetConnection
==========================================================================
Purpose: Check and get first correct address and if ok request connection
Params:  pvAddressList
Return:  1 if ok, else 0
=========================================================================*/
UINT8 ME_Push_GetConnection( void* pvAddressList );

/*========================================================================
	ME_Push_GetFreeIdNumber
==========================================================================
Purpose: Get free IdNumber to use
Params:  pvList (any list)
Return:  UINT16 IdNumber, 0 means failure
=========================================================================*/
UINT16 ME_Push_GetFreeIdNumber(void* pvList);

/*========================================================================
	ME_Free_PushContentStruct
==========================================================================
Purpose: Free the PushContent struct
Params:  vsPushContent
Return:  TRUE
=========================================================================*/
void ME_Free_PushContentStruct(void* pvPushContentStruct);

/*========================================================================
	ME_Push_List_Add
==========================================================================
Purpose: Add a new element to the Push List
Params:  pvPushList, pvPushListStruct
Return:  1 if ok, else 0
=========================================================================*/
UINT8 ME_Push_List_Add( void* pvPushList,  void* pvPushListStruct , void* pvPushContentStruct);

/*========================================================================
	ME_Push_Change_Place_In_List
==========================================================================
Purpose: Change place on OldElement and NewElement in the list
Params:  pvPushList, pvPushListStructNew, pvPushListStructOld
Return:  
=========================================================================*/
void ME_Push_Change_ElementId_In_List(void* pvPushList,  void* pvPushListStructNew, void* pvPushListStructOld);

/*========================================================================
	ME_Push_List_Delete
==========================================================================
Purpose: Deletes an element from the Push List
Params:  pvPushList, pvPushListStruct
Return:  
=========================================================================*/
void ME_Push_List_Delete( void* pvPushList,  void* pvPushListStruct );

/*========================================================================
	ME_Push_List_Sort
==========================================================================
Purpose: Sort the elements in the Push List
Params:  pvPushList 
Return:  
=========================================================================*/
void* ME_Push_List_Sort(void* pvPushList);

/*========================================================================
	ME_CompareWString
==========================================================================
Purpose: Compares two WCHAR strings
Params:  pwch1, pwch2
Return:  Boolean
=========================================================================*/
BOOL ME_CompareWString(WCHAR* pwch1, WCHAR* pwch2);

/*========================================================================
	ME_Push_Get_First_New
==========================================================================
Purpose: Gets first new Push in Push list
Params:  pvPushList
Return:  pPushListStruct
=========================================================================*/
void* ME_Push_Get_First_New(void* pvPushList);

/*========================================================================
	ME_Push_SI_Expired
==========================================================================
Purpose: Check if SI is expired or not. If pvPushListStruct is NULL, then
		 the entire list is scanned to see if any SI in list is expired.
Params:  pvPushListStruct
Return:  Boolean 
=========================================================================*/
BOOL ME_Push_SI_Expired(void* pvPushList, void* pvPushListStruct);

/*========================================================================
	ME_Push_Find_Id
==========================================================================
Purpose: Find push with PushId in Push List 
Params:  pvPushId, pvPushList
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_Find_Id(WCHAR* pwchId, void* pvPushList);

/*========================================================================
	ME_Push_Find_OldId
==========================================================================
Purpose: Find old push with PushId in Push List 
Params:  pvPushId, pvPushList, iLLPushId
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_Find_OldId(WCHAR* pwchId, void* pvPushList, UINT32 iLLPushId);

/*========================================================================
	ME_Push_FindWithIdNumber
==========================================================================
Purpose: Find push with IdNumber in Push List 
Params:  pvPushList, iIdNumber
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_FindWithIdNumber(void* pvPushList, UINT16 iId);

/*========================================================================
	ME_Push_Get_Oldest(void* pvPushList, INT8 iType)
==========================================================================
Purpose: Retrieve the oldest SI or SL message.
Params:  pvPushList, The List
		 iType, What should be retrieved? SI, SL 
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_Get_Oldest(void* pvPushList, INT8 iType) ;

/*========================================================================
	ME_Push_Get_Last( void* pvPushList )
==========================================================================
Purpose: Gets last Push in Push list
Params:  pvPushList
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_Get_Last( void* pvPushList );

/*========================================================================
	ME_Push_Get_First( void* pvPushList )
==========================================================================
Purpose: Retrieve first Push in Push list
Params:  pvPushList
Return:  pvListElement
=========================================================================*/
void* ME_Push_Get_First( void* pvPushList );

/*========================================================================
	ME_Push_Get_Next( void* pvPushList, void* pvListElement )
==========================================================================
Purpose: Retrieve next Push in Push list
Params:  pvPushList, pvListElement
Return:  pvNextListElement
=========================================================================*/
void* ME_Push_Get_Next( void* pvPushList, void* pvListElement );

/*========================================================================
	ME_Push_Get_PushListStruct( void* pvListElement )
==========================================================================
Purpose: Retrieve last Push in Push list
Params:  pvListElement
Return:  pPushListStruct
=========================================================================*/
void* ME_Push_Get_PushListStruct( void* pvListElement );

/*========================================================================
	ME_Push_BuildContent
==========================================================================
Purpose: Call parsing functions and build content
Params:  See below
Return:  pPushContentStruct
=========================================================================*/
void* ME_Push_BuildContent(void* pBodyData, UINT32 iBodyDataLen, void* vsHeaderDef, 
						   UINT8 iPortType, UINT8 iViewID, UINT8 iNewChannelId);

/*========================================================================
	ME_Push_GetNextPPListElem
==========================================================================
Purpose: Gets next postponed (PP) Push in Push list. If pvPushListStruct
		 equals NULL, the first PP push in list is retrieved. If NULL is
		 returned, no more PP pushes exist in list.
Params:  pvPushList, pvPushListStruct
Return:  pPushListStruct or NULL
=========================================================================*/
void* ME_Push_GetNextPPListElem( void* pvPushList, void* pvPushListStruct, UINT8 iType );

/*========================================================================
	ME_Push_Create_Elem
==========================================================================
Purpose: Creates a pPushListStruct element from a pPushContentStruct.
Params:  pvPushContentStruct
Return:  pPushListStruct 
=========================================================================*/
void* ME_Push_Create_Elem( void* pvPushContentStruct );

/*========================================================================
	ME_Push_Remove_List
==========================================================================
Purpose: Removes the entire Push List and deallocates struct data
Params:  pvPushList
Return:  -
=========================================================================*/
void ME_Push_Remove_List( void* pvPushList );

/*========================================================================
	ME_Push_ParseHeader
==========================================================================
Purpose: Parse the Push Header. Returns a pHEADERDEF
Params:  vsPushHeader, iHeaderLength
Return:  pHEADERDEF
=========================================================================*/
void* ME_Push_ParseHeader( void* vsPushHeader, UINT32 iLen );

/*========================================================================
	ME_Push_GetPushFlag
==========================================================================
Purpose: Retrieves the Push Flag from the Push Header.
Params:  vsHeaderDef
Return:  UINT8
=========================================================================*/
UINT8 ME_Push_GetPushFlag( void* vsHeaderDef );

/*========================================================================
	ME_Push_GetXInitiatorURI
==========================================================================
Purpose: Retrieves the X-Initiator-URI from the Push Header.
Params:  vsHeaderDef
Return:  Url (BYTE*)
=========================================================================*/
void* ME_Push_GetXInitiatorURI( void* vsHeaderDef );

/*========================================================================
	ME_Push_GetXContentURI
==========================================================================
Purpose: Retrieves the X-Content-URI from the Push Header.
Params:  vsHeaderDef
Return:  Url (BYTE*)
=========================================================================*/
void* ME_Push_GetXContentURI( void* vsHeaderDef );

/*========================================================================
	ME_Push_GetXAppId
==========================================================================
Purpose: Retrieves the X-Application-Id from the Push Header.
Params:  vsHeaderDef
Return:  TRUE if X-WAP-Applicatin-Id was provided in the header , FALSE otherwise
=========================================================================*/
BOOL ME_Push_GetXAppId( void* vsHeaderDef, long int* iPortType);

/*========================================================================
	ME_Push_GetContentType
==========================================================================
Purpose: Retrieves the content type from the Push Header.
Params:  vsHeaderDef
Return:  UINT8
=========================================================================*/
UINT8 ME_Push_GetContentType( void* vsHeaderDef );

/*========================================================================
	ME_Push_SecurityOk
==========================================================================
Purpose: Check if security levels match.
Params:  Push Flag, Seurity Level
Return:  TRUE if Ok, FALSE otherwise
=========================================================================*/
BOOL ME_Push_SecurityOk(UINT8 iPushFlag);

/*========================================================================
	ME_Push_BuildCO
==========================================================================
Purpose: Call parsing functions and build content for an CO.
Params:  void* pPushBody, UINT32 iBodyDataLen, iType
Return:  pELEMENTTYPE (CO parser structure type)
=========================================================================*/
void* ME_Push_BuildCO(void* pBodyData, UINT32 iBodyDataLen, void* vsHeaderDef,
					  UINT8 iViewID);

/*========================================================================
	ME_Push_GetAddress
==========================================================================
Purpose: Get contact point from SIA content
Params:  void*
Return:  BYTE* (address type)
=========================================================================*/
void* ME_Push_GetAddress( void* pBodyHandle, UINT32 iBodyLen );

/*========================================================================
	ME_Push_GetAppId
==========================================================================
Purpose: Get App Id from SIA content
Params:  void*
Return:  UINT8
=========================================================================*/
UINT8 ME_Push_GetAppId( void* pBodyHandle, UINT32 iBodyLen );

/*========================================================================
	ME_Push_Check_if_InvalidateURI_Abs
==========================================================================
Purpose: Check if the invalidate URI is absolute
Params:  InvURI
Return:  TRUE if InvUri is an absolut URI
=========================================================================*/
BOOL ME_Push_Check_if_InvalidateURI_Abs(const BYTE *InvURI);

/*========================================================================
	ME_Push_COCompareAuth
==========================================================================
Purpose: Compare Authority parts of two urls.
Params:  vsUrl1, vsUrl2, iPushFlag
Return:  TRUE if equal and initiator is authenticated flag is set || or content is trusted flag is set, else FALSE
=========================================================================*/
BOOL ME_Push_COCompareAuth( void* vsUrl1, void* vsUrl2, UINT8 iPushFlag );

/*========================================================================
	ME_Push_Remove_SignalList
==========================================================================
Purpose: Removes the entire Signal List and deallocates struct data
Params:  pvPushList
Return:  -
=========================================================================*/
void ME_Push_Remove_SignalList( void* pvSignalList );

#endif
