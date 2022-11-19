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
#ifndef WML_UAFN_H
#define WML_UAFN_H

#include "waedef.h"
#include "wae_cmmn.h"

typedef struct
{
  INT16  iIANACharID;
  UINT8  iPIVersion;
  BYTE    iContentTypeAsInt;
  BYTE*   pContentTypeAsText;
  BOOL    fCacheControl;
} CONTENTINFOSTRUCT,*pCONTENTINFOSTRUCT ;

#define WTAI_URI_NO_FUNCTION 0
#define WTAI_URI_FUNCTION    1
#define WTAI_URI_END_CONTEXT 2

/*---------------------------------------------
Create a new user agent.
Input: The view associated with the UA
Output: Pointer to the User Agent (ELEMENT),
        NULL if no memory
----------------------------------------------*/
void* SDL_CreateUA(int iViewID);

/*---------------------------------------------
A complete removal of all data associated with
this particular User Agent. To be used when a
complete shutdown is performed.

----------------------------------------------*/
void SDL_CleanUpUserAgent(void** ppUserAgent);

/*---------------------------------------------
clears the current UA context
----------------------------------------------*/
void SDL_ClearContext(void* pUserAgent);

/*---------------------------------------------
Removes the current deck and all its structures,
the variables and context are kept though.
Input: Pointer to the User Agent
Output: -
----------------------------------------------*/
void SDL_CleanUpDeck(void *pUser);

/*---------------------------------------------
Checks the deck for HEAD elements which may contain
access restrictions
Output: TRUE = (accessfailure), FALSE (no accessfailure)
----------------------------------------------*/
int SDL_CheckAccess(void* pUserAgent, void* pResponseURL, int iFromCard);

/*-----------------------------------------------------------
Retrieves the URL to be used, resolves it if necessary and
normalizes it. A history item is also allocated.
-------------------------------------------------------------*/
void SDL_GetElementURL( void* pUser, void* pURLElement, void** pURL, void** pHistoryItem);

/* initiate parsing */
BOOL SDL_ParseData(void *pUserAgent, void* pData, int iBufLen, void* pTheHeaderData);

int SDL_ParseNext(void *pUserAgent);

int SDL_EndParse(void *pUserAgent);

void SDL_AbortParsing(void *pUserAgent);

/* used by postcontent */
void AddPostData( URLREQSTRUCT* pRequestStruct, int iSendMode, BYTE* pPostData, UINT32 iPostDataLen, BYTE* pContentType, BYTE* pContentDispositionFileName, UINT32 iTotalSize);

void* AddMorePostData( void* vsPostData, int PostDataLen, int iSendMode );

/*
void* SDL_CreateRequestStruct( void* pWAEInObject, void* pInUserAgent, void* pActiveElement, void* pbURL, void* pAccept);
*/
void* SDL_CreateRequestStruct( void* pbURL, void* pAccept);

void* SDL_CreateExtendedRequestStruct( void* pWAEInObject, void* pInUserAgent, void* pActiveElement, void* pbURL, void* pAccept, void* pInHistoryElement);

/*--------------------------------------------------------------------
Returns IANA code if any and the content-type as a reference string 
-----------------------------------------------------------------------*/
int SDL_GetResponseContentType(void* pUser, void* pTheHeaderData, void** ppchInContentTypeText);

void* SDL_GetDetailedContentType(void* pTheHeaderData);
/*--------------------------------------------------------------------
Converts a content-type string to byte
-----------------------------------------------------------------------*/
int SDL_ContentTypeTextToInt(void* pbText);

/*--------------------------------------------------------------------
Check if the contentype corresponds to the acceptcontenttype, 1 (TRUE) if OK
-----------------------------------------------------------------------*/
int SDL_CheckResponseContentType(const void* pContentTypeText, const void* pCompareContentType);

int SDL_GetCacheControl(void* pInHeaderList);

/*---------------------------------------------
SDL_UpdateMMIVariables: Runs updates on all
external variables input variables
Returns the number of signals
----------------------------------------------*/
int SDL_SendSignalsInputVariables(void *pUser, void *pList);

BOOL SDL_GetResponseInformation( int iScheme,  void* pTheHeaderData, void** ppResult, void** ppContentType);

void SDL_DeleteResponseHeaderInfo( int iScheme, void** pHeaderHandle, void** pResponseInfo);

void DeleteWSPHeader(void** ppTheHeaderData);

/*---------------------------------------------
Checks if the card contains any NEWCONTEXT, in
that case deletes both History and Variabletable
Return NewContext 1 (TRUE), 0 (FALSE)
----------------------------------------------*/
int SDL_HandleNewContext(void *pUser,void *pTheCard);

void *SDL_GetCard (void *pUser, void* pCardName);

void *SDL_GetNextElement (void *pElementObj);

void* SDL_GetElementContent (void *TheElement);

int SDL_GetElementType (void *pElm);

WCHAR* SDL_GetOptionText (void *pUser, void* pElement);

void* SDL_GetText(void *pUser,void* pTextElement);

int SDL_GetTimerValue(void* pUser, void* pTimerElement);

void SDL_SetUserInputLocked(void* pUser, BOOL fWait);

void SDL_SetMMIUserAgentLocked(void* pUser, BOOL fMMILock);

void SDL_ToggleLoadIcon(void* pUser, BOOL fLoad, BYTE* pRequestURL);

int SDL_MMILocked(void* pUser);

UINT16 WSP2HTTPStatusConvert(UINT8 iWSPNbr);

void* SDL_Normalise(void* pTheUA, void* pURL);

void* SDL_URLGetFragment(void* pURL);

int SDL_URLOnSameDeck(void *pUserAgent,void* pInURL);

void* SDL_URLGetScheme(void* pURL);

int SDL_GetScheme(void* pURL);

int SDL_CheckIfPost(void* pTaskElement);

int SDL_CheckIfReload(void* pTaskElement);

void SDL_SetAuthParam(void* pInUser,void* pbURL,void* pbRealm, void *pbUserName, void *pbPassword);

void* SDL_GetAuthValues(void *pHeader, int iType);

int SDL_CheckAuth( void* pUser , void *pUrl, void* pHeader);

void SDL_SendAuthRequest( void* pUser, int iDialogId ,void* pRealm, int iType);

void SDL_UpdateBaseURL(void* pUserAgent, void* pResponseURL ,void* pHeaderData);

int SDL_UpdateInputVariables(void *pUserAgent, void* pInputElementList, int iInputID, void* pchTheInputString);

void SDL_SetSelectDefault ( void *pTheElement , void *pUser);

void SDL_UpdateAllSelect (void* pCard, void* pUser);

int SDL_GetContentRequestUrlID(void* pCRStruct);

int SDL_GetContentRequestTID( void* pContentRequestList,int iUrlID );

void SDL_StoreContentRequest( void* pContentRequestList,int iUrlID,int iTID, int iSendMode);

int SDL_GetContentSendMode( void* pContentRequestList,int iTID );

int SDL_CallWTAIURI( int objectId, void* pURL);

void SDL_About(void* pTheUA, void* pbReqURL);

void SDL_UpdateLanguage(UINT8 iViewID, void* pElement, void* pPrevELement,void* pLanguageStack );

/*-----------------------test functions------------------------------------------*/

#ifdef WAE_DEBUG

  void WMLUA_PrintWS(WCHAR *pchText);
  void PrintChar(char *pStr);
  void PrintInt(int iInt);

#endif

#endif
