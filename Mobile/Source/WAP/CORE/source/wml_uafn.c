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



#ifndef WML_UAFN_C
#define WML_UAFN_C

#include "wml_uafn.h"

#include "wmldef.h"
#include "wmltoken.h"
#include "wbxmldef.h"
#include "wmlif.h"
#include "wmlelm.h"

#include "url.h"
#include "headdef.h"
#include "hdrutil.h"

#include "aapimmi.h"
#include "aapiwta.h"
#include "trnscode.h"
#include "history.h"
#include "wipprdct.h"

#ifdef USE_KSC5601
	#include "HCodeCnvt.h"
#endif


/* -------------------------------- internal defines ---------------------------------------- */

void CreatePostString(UA* pUserAgent, GOELEMENT* pGoElement, INT16 iCharEnc, BYTE** ppString, UINT16* piDataLen );
void CreateNavigationData( UA* pUserAgent, GOELEMENT* pGoElement, HISTORYITEM** ppHistoryItem, BYTE** ppTargetURL );
void CreatePostBody(UA* pUserAgent, GOELEMENT* pGoElement, INT16 iCharEnc, BYTE** pbData, UINT16* piDataLen);

WCHAR* getLang(void* pElement, UINT16 iType);

#ifdef USE_KSC5601
BYTE* Unicode2byteSpecialKSCConvert(WCHAR* pURL);
#endif

typedef struct tPOSTSTRINGPART
{
  void* pData;
  struct tPOSTSTRINGPART *pNextField;
} POSTSTRINGPART, *pPOSTSTRINGPART;

typedef struct
{
   UINT8 iUrlID;
   UINT8 iSendMode;
} CONTENTREQUESTSTRUCT,*pCONTENTREQUESTSTRUCT;





/*------------------------------Content UA functionality --------------------------------------*/

void SDL_StoreContentRequest( void* pContentRequestList,int iUrlID,int iTID, int iSendMode)
{
   CONTENTREQUESTSTRUCT* pCRStruct;
/* aspn 001101
   int iDummy;
*/
   pCRStruct = NEWSTRUCT(CONTENTREQUESTSTRUCT);
   if (pCRStruct)
   {
      pCRStruct->iUrlID=(UINT8)iUrlID;
	  pCRStruct->iSendMode=(UINT8)iSendMode;
/* aspn 001101
      iDummy=SDL_AddElementLast(pContentRequestList,iTID, pCRStruct);
*/
      SDL_AddElementLast(pContentRequestList,iTID, pCRStruct);
   }
}

int SDL_GetContentRequestUrlID(void* pCRStruct)
{
  return ((CONTENTREQUESTSTRUCT*)pCRStruct)->iUrlID;
}

int SDL_GetContentRequestTID( void* pContentRequestList,int iUrlID )
{
	void*					pvListElement = NULL;
	void*					pvElementContent = NULL;
	CONTENTREQUESTSTRUCT*	pCRStruct = NULL;

	if( pContentRequestList != NULL ){
		pvListElement = SDL_GetNextListElement( pContentRequestList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pCRStruct = (CONTENTREQUESTSTRUCT*)pvElementContent;
				if( pCRStruct != NULL && pCRStruct->iUrlID == iUrlID ){
					return SDL_GetListElementID( pvListElement );
				}
			}
			pvListElement = SDL_GetNextListElement( pContentRequestList, pvListElement );
		}
	}
	return 0;
}

int SDL_GetContentSendMode( void* pContentRequestList,int iTID )
{
	void*					pvListElement = NULL;
	CONTENTREQUESTSTRUCT*	pCRStruct = NULL;

	if( pContentRequestList != NULL ){
		pvListElement = SDL_SearchListElement( pContentRequestList, iTID, NULL);
		if( pvListElement != NULL ){
			pvListElement = SDL_GetListElementContent( pvListElement );
			if( pvListElement != NULL ){
				pCRStruct = (CONTENTREQUESTSTRUCT*)pvListElement;
				return pCRStruct->iSendMode;
			}
		}
	}
	return 0;
}


/*------------------------------General WML/WTA UA funct.---------------------------------------*/

/*---------------------------------------------
Create a new user agent.
Input: The view associated with the UA
Output: Pointer to the User Agent (ELEMENT),
        NULL if no memory
----------------------------------------------*/
void* SDL_CreateUA(int ViewID)
{
   UA* pUser=NULL;
   pUser = WML_CreateUserAgent();
   if (pUser !=NULL)
   {
     pUser->iViewID = (INT8)ViewID;
     pUser->pbURLBase = NULL;
   }
   return (void*)pUser;
}

/*---------------------------------------------
Removes the current deck and all its structures,
the variables and context are kept though.
Input: Pointer to the User Agent
Output: -
----------------------------------------------*/
void SDL_CleanUpDeck(void*pUser)
{
   WML_EmptyDECK((UA*)pUser);
}

/*---------------------------------------------
clears the current UA context
Input: Pointer to the User Agent
Output: -
----------------------------------------------*/
void SDL_ClearContext(void* pUserAgent)
{
   UA* pUser;
   pUser = (UA*)pUserAgent;
   WML_EmptyHistoryList(pUser);
   WML_EmptyVarTable(pUser);
 /*  SDL_Free( & (pUser->pbURLBase) );  */
}

/*---------------------------------------------
A complete removal of all data associated with
this particular User Agent. To be used when a
complete shutdown is performed.
----------------------------------------------*/
void SDL_CleanUpUserAgent(void** ppUserAgent)
{
    UA** ppUser;
    ppUser = (UA**)ppUserAgent;
    SDL_ClearContext(*ppUser);
    SDL_CleanUpDeck(*ppUser);
    WML_DeleteUserAgent (ppUser);
}




/*----------------------------------- Parser functionality -----------------------------------------*/

/*------------------------------------------------------------
 initiate parsing returns a pointer to the allocated buffer
-------------------------------------------------------------*/
BOOL SDL_ParseData(void *pUserAgent, void* pData, int iBufLen, void* pResponseInfo)
{
   UA* pUser=NULL;
   UINT8 iPIVersion=1; /* 1=unknown */
   INT16 iCharset=IANA_CHARSET_INVALID;
   BYTE iContentTypeByte=0;
   pUser = (UA*)pUserAgent;   

   /* use the old raw data to parse */
   if (pData==NULL)
   {
      #ifdef TASKFAILURE_REPARSE_RAWDATA
		  
	      iPIVersion = pUser->iOldPIVersion;
		  iContentTypeByte = pUser->iOldContentTypeByte;
		  iCharset = pUser->iOldCharset;
		  iBufLen = pUser->iOldRawWMLDeckDataLen;
		  pData = pUser->pOldRawWMLDeckData;

		  /* to enable the reparse to be performed several times */
		  pUser->iTempPIVersion = pUser->iOldPIVersion;
		  pUser->iTempContentTypeByte = pUser->iOldContentTypeByte;
		  pUser->iTempCharset = pUser->iOldCharset;
		  pUser->iTempRawWMLDeckDataLen = pUser->iOldRawWMLDeckDataLen;
		  DEALLOC(&pUser->pTempRawWMLDeckData);
		  pUser->pTempRawWMLDeckData = pUser->pOldRawWMLDeckData;
		  
		  if (pData==NULL)
			  return 0;
      #else
		  return 0;
      #endif
   }
   else
   {

        iCharset = ((CONTENTINFOSTRUCT*)pResponseInfo)->iIANACharID;
        iPIVersion = ((CONTENTINFOSTRUCT*)pResponseInfo)->iPIVersion;
        iContentTypeByte = ((CONTENTINFOSTRUCT*)pResponseInfo)->iContentTypeAsInt;
        
        #ifdef TASKFAILURE_REPARSE_RAWDATA
	   
			pUser->iTempPIVersion = iPIVersion;
			pUser->iTempContentTypeByte =iContentTypeByte;
			pUser->iTempCharset =iCharset;

			DEALLOC( & pUser->pTempRawWMLDeckData);
			pUser->pTempRawWMLDeckData = (BYTE*)pData;
			pUser->iTempRawWMLDeckDataLen = iBufLen;

        #endif
   }

   #ifdef WAE_DEBUG
     PrintChar("------Parsing--------------");
     PrintChar("Size:");
     PrintInt(iBufLen);
     PrintChar("character encoding:");
   #endif
   pUser->iTextEncodFormat = iCharset;  
   return WML_StartParser(pUser,(BYTE*)pData,(UINT32)iBufLen,iContentTypeByte,iPIVersion);
}

/*---------------------------------------------
parse next card
returns FALSE=more, TRUE=finished ok 
-----------------------------------------------*/
int SDL_ParseNext(void *pUserAgent)
{
  return (!WML_ParseNextElement((UA*)pUserAgent));
}

/*---------------------------------------------
finalise parse returns FALSE=not ok, TRUE=finished ok 
-----------------------------------------------*/
int SDL_EndParse(void *pUserAgent)
{
  UA* pUser=NULL;
  BOOL fRetVal;
  UINT32 iErrors=0;

  pUser = (UA*)pUserAgent;   

  fRetVal = WML_EndParser((UA*)pUserAgent);

  if (fRetVal)
  {
    /* all was ok then accept raw data as old fallbackdata */
    #ifdef TASKFAILURE_REPARSE_RAWDATA
        pUser->iOldPIVersion = pUser->iTempPIVersion;
        pUser->iTempPIVersion =0;
        pUser->iOldContentTypeByte = pUser->iTempContentTypeByte;
        pUser->iTempContentTypeByte =0;
        pUser->iOldCharset = pUser->iTempCharset;
        pUser->iTempCharset=0;

        if (pUser->pOldRawWMLDeckData != pUser->pTempRawWMLDeckData)
        { 
          DEALLOC( & pUser->pOldRawWMLDeckData);
        }
        pUser->pOldRawWMLDeckData = pUser->pTempRawWMLDeckData;
        pUser->pTempRawWMLDeckData=NULL;

        pUser->iOldRawWMLDeckDataLen = pUser->iTempRawWMLDeckDataLen;
        pUser->iTempRawWMLDeckDataLen=0;
    #endif
  }
	
  iErrors=pUser->pDecStr->iDecodeResult;

  /* Report warnings */
  if ( iErrors & WBXML_Warning_UnknownToken )
  {
	  CLNTa_error (pUser->iViewID, ERR_WAE_WML_UNKNOWN_TOKEN, ERRTYPE_INFORMATION);
  }

  if ( iErrors & WBXML_Warning_Version )
  {
	  CLNTa_error (pUser->iViewID, ERR_WAE_WBXML_CONTENT_VERSION_WARNING, ERRTYPE_INFORMATION);
  }

  if ( iErrors & WBXML_Warning_UnknownCharset )
  {
	  CLNTa_error (pUser->iViewID, ERR_WAE_WML_CONTENT_CHARSET_NOT_SUPPORTED, ERRTYPE_INFORMATION);
  }

  /* Report errors */
  if ( iErrors & WBXML_Error_DataStream )
  {
	  CLNTa_error (pUser->iViewID, ERR_WAE_WML_INSTREAM_FAULT, ERRTYPE_INFORMATION);
  }

  if ( iErrors & WBXML_Error_CharsetEncoding )
  {
	  CLNTa_error (pUser->iViewID, ERR_WAE_WML_CONTENT_CHARSET_ERROR, ERRTYPE_INFORMATION);
  }

  if ( iErrors & WBXML_Error_PublicID )
  {
	  CLNTa_error (pUser->iViewID, ERR_WAE_WBXML_CONTENT_PUBLIC_ID_ERROR, ERRTYPE_INFORMATION);
  }

  /* WBXML_Error_OutOfMemory, WBXML_Error_StringTable, WBXML_Error_MbuintEncoding,
	 WBXML_Error_Encoding, or WBXML_Error_Unknown */
  if ( iErrors & 0xAD0000 )
  {
	  CLNTa_error (pUser->iViewID, ERR_WAE_WML_WML_ERROR, ERRTYPE_INFORMATION);
  }
  
  return fRetVal;
}



void SDL_AbortParsing(void *pUserAgent)
{
    #ifdef TASKFAILURE_REPARSE_RAWDATA
	  UA* pUser=NULL;
      pUser = (UA*)pUserAgent; 
	  pUser->iTempPIVersion =0;
	  pUser->iTempContentTypeByte =0;
      pUser->iTempCharset=0;
      if (pUser->pOldRawWMLDeckData != pUser->pTempRawWMLDeckData)
      {
	    DEALLOC( & pUser->pTempRawWMLDeckData);
	  }
	  else
      {
        pUser->pTempRawWMLDeckData=NULL;
      }
      pUser->iTempRawWMLDeckDataLen=0;
    #endif
    WML_AbortParser ((UA*)pUserAgent);
}





/*----------------------------------------- WML Element funct.---------------------------------------*/

/*---------------------------------------------
Checks the deck for HEAD elements which may contain
access restrictions
Input: User Agent
Output: TRUE = accessfailure, FALSE = no accessfailure
----------------------------------------------*/
int SDL_CheckAccess(void* pUserAgent, void* pResponseURL, int iFromCard)
{
   ELEMENTTYPE* pDeckPart=NULL;
   ELEMENTTYPE* pHeadPart=NULL;
   BYTE* pbDefaultDomain=NULL;
   BYTE* pbBase=NULL;
   UA* pUser=NULL;
   BOOL fRes=FALSE;

   pUser = (UA*)pUserAgent;

   if (pUser->pDeck !=NULL && pUser->pDecStr !=NULL)
   {

      if ((pUser->pDeck)->iType==Type_WML )
      {
        pDeckPart = WML_GetContent(pUser->pDeck); 
        while (pDeckPart != NULL)
        {
           /* Search for HEAD element */
           if (pDeckPart->iType==Type_HEAD)
           {
             
			  pHeadPart = WML_GetContent(pDeckPart);

              /* search for access elements */
              while (pHeadPart!=NULL)
              {
                 if (pHeadPart->iType==Type_ACCESS)
				 {
					b_GetHost ((BYTE*)pResponseURL, & pbDefaultDomain);
					pbBase = pUser->pbURLBase;
					
					if (iFromCard==0)		/* if it is a direct navigation, the base should be empty */
						pbBase = NULL;
				    
					if (!WML_CheckAccessElement(pHeadPart, pbBase, pbDefaultDomain))
					{
                      fRes = TRUE;
					}
					DEALLOC(&pbDefaultDomain);
				 }
				 pHeadPart = WML_GetNextElement(pHeadPart);  
			  }
		   }
		   pDeckPart = WML_GetNextElement(pDeckPart);
		}
	  }
   }
   return fRes;
}


/*---------------------------------------------
SDL_GetTimerValue: Used by SDL to get the appropriate
timer value for a certaion card.
Input:
Output:
----------------------------------------------*/
int SDL_GetTimerValue(void* pUser, void* pTimerElement)
{
   return ((int)(WML_GetTimer ((UA*)pUser, (ELEMENTTYPE*)pTimerElement)));
}


/*---------------------------------------------
SDL_GetElementType: used from SDL to determine the
type of element.
Input: TheElement
Output: type - according to definitions.h
----------------------------------------------*/

int SDL_GetElementType (void *pElm)
{
	

    ELEMENTTYPE *pTheElement;
    pTheElement = (ELEMENTTYPE*)pElm;

    if (pTheElement!=NULL)
    {
      #ifdef WAE_DEBUG
        PrintChar("***ELEMENTTYPE***");
        PrintInt(pTheElement->iType);
      #endif
	  if ( isTypeWMLText(pTheElement->iType) )
		return Type_GenericText;
	  else
        return (pTheElement->iType);
    }
    return 0;
}


WCHAR* SDL_GetOptionText (void *pUser, void* pElement)
{
  WCHAR* pwchText=NULL;

  if (pElement != NULL)
  {
    pELEMENTTYPE pTempElm = pElement;
    UINT32 iLength=0;

    /* Calculate length */
    while (pTempElm != NULL)
    {
      /* Check if text */
      if (isTypeWMLText(pTempElm->iType))
      {
        /* Add length */
        iLength += WML_GetStringLength (pUser, pTempElm);
      }

      /* Get next element */
      pTempElm = WML_GetNextElement (pTempElm);
    }

    /* Allocate memory */
    pwchText = NEWARRAY (WCHAR,iLength+1);

    if (pwchText != NULL)
    {
      WCHAR* pwchTempText=NULL;
      pTempElm = pElement;

      /* Set termination character */
      *pwchText=0;

      while (pTempElm != NULL)
      {
        /* Check if text */
        if (isTypeWMLText(pTempElm->iType))
        {
          /* Get string */
          pwchTempText = WML_GetString (pUser, pTempElm);

          /* Concat string */
          if (pwchTempText != NULL)
          {
            CONCATSTRING (pwchText,pwchTempText);
            DEALLOC (&pwchTempText);
          }
        }

        /* Get next element */
        pTempElm = WML_GetNextElement (pTempElm);
      }
    }
  }

  return pwchText;
}


/*----------------------------------------------
some functions directly mapped from SDL, check
interface.h functions for further info.
------------------------------------------------*/
void* SDL_GetElementContent (void *TheElement) 
{
  return (void*)WML_GetContent((ELEMENTTYPE*)TheElement);
}

void *SDL_GetNextElement (void *pElementObj)
{
     return (void*)WML_GetNextElement ((ELEMENTTYPE *)pElementObj);
}

void* SDL_GetText(void *pUser,void* pTextElement)
{
   return (void*)WML_GetString((UA*)pUser, (ELEMENTTYPE*)pTextElement);
}

/*---------------------------------------------------
Returns a pointer to the card with pCardName. 
If pCardName is NULL then the first card is returned,
----------------------------------------------------*/
void *SDL_GetCard (void *pUser, void* pCardName)
{
   ELEMENTTYPE* pCard=NULL;
   WCHAR* pTheCardName=NULL;
   pTheCardName = wip_byte2wchar((BYTE*)pCardName);
   pCard = WML_GetCard ((UA*)pUser, pTheCardName);
   DEALLOC(& pTheCardName);
   return (void*)pCard; 
}


/*---------------------------------------------
Checks if the card contains any NEWCONTEXT, in
that case deletes both History and Variabletable
Return NewContext 1 (TRUE), 0 (FALSE)
----------------------------------------------*/

int SDL_HandleNewContext(void *pUser,void *pTheCard)
{
   if (pTheCard !=NULL)
   {

     if ( ( (CARDELEMENT*)(pTheCard) )->fNewContext)
     {
       SDL_ClearContext(pUser);
	   return 1;
     }
   }
   return 0;
}




/*---------------------------- Functionality to handle (WSP) responses ----------------------------------*/

/*----------------------------------------------
Checks the WSP header for additional information about
the contentbase (checks Content Location and 
Content Base). (see RFC 2068 14.11, 14.15)
Returns a copy of the new base URL (if any)
------------------------------------------------*/
void* GetResponseBaseURL(void* pHeaderData, void* pRequestURL )
{
  HEADERDEF *pHeaderList = NULL;
  HEADERELEMENT *pHeaderElement = NULL;
  BYTE* pbContentBase=NULL;
  BYTE* pbContentLocation=NULL;
  BYTE* pbResult=NULL;

  pHeaderList = (HEADERDEF*)pHeaderData;

  if (pHeaderList !=NULL && pRequestURL!=NULL)
  {
    pHeaderElement = WSP_GetHeaderWK( 1, Field_Content_Base, pHeaderList->pHeadList );
    pbContentBase = WSP_GetContentBase(pHeaderElement);

	/* if there is a contentbase use that */
	if (pbContentBase!=NULL)
    {
       /* if not absolute try to resolve */
       b_Resolve ((BYTE*)pRequestURL, pbContentBase, & pbResult);

       pHeaderElement = WSP_GetHeaderWK( 1, Field_Content_Location, pHeaderList->pHeadList );

	   /* then use content location to resolve */
       pbContentLocation = WSP_GetContentLocation (pHeaderElement);
	   if (pbContentLocation!=NULL)
	   {
         pbContentBase = pbResult;
         b_Resolve ((BYTE*)pbContentBase, pbContentLocation, & pbResult);
		 DEALLOC(&pbContentBase);
       }
	}
	else /* otherwise try any content location field (absolute or relative) */
    {
       pHeaderElement = WSP_GetHeaderWK( 1, Field_Content_Location, pHeaderList->pHeadList );
       pbContentLocation = WSP_GetContentLocation (pHeaderElement);
	   if (pbContentLocation!=NULL)
         b_Resolve ((BYTE*)pRequestURL, pbContentLocation, & pbResult);
	}
  }

 #ifdef WAE_DEBUG
    PrintChar("Response base URL:");
    PrintChar(pbResult);
  #endif

  return (void*)pbResult;
}

/*--------------------------------------------------------------------------
 Updates the baseurl of the current card (with either the received response 
 url or the sent request url) and also copies the fragment 
----------------------------------------------------------------------------*/
void SDL_UpdateBaseURL(void* pUserAgent, void* pRequestURL ,void* pHeaderData)
{
  BYTE* pNewBase=NULL;
  BYTE* pFrag=NULL;    
  UA* pUser=NULL;
  int iBaseLength =0; 
  ELEMENTTYPE* pFirstCard=NULL;

  pUser = (UA*)pUserAgent;

  DEALLOC(&(pUser->pbURLBase));  /* remove any old base */
	
  if (pHeaderData!=NULL)  /* this was the result of a network request */
  {
     pNewBase = (BYTE*)GetResponseBaseURL(pHeaderData, pRequestURL );  /* retrieve the new url */
  }

  if (pNewBase==NULL) 
  {
     #ifdef WAE_DEBUG
       PrintChar("Using request URL:");
       PrintChar(pRequestURL);
     #endif

	 pNewBase = (BYTE*)GenCopyString(1, pRequestURL);   /* no info in header use pRequestURL instead */
  }

  if (pNewBase)
  {
	 b_GetFragment ((BYTE*)pNewBase, & pFrag);
  
	 /* no frag present e.g it was the first card */
     if (pFrag==NULL)
	 {
        pFirstCard= WML_GetCard (pUser, NULL); 
        if (pFirstCard!=NULL)
		{
			/* TODOjens ->pElm) */
           pFrag = (BYTE*)GenCopyString(2,((CARDELEMENT*)(pFirstCard))->pchId);  /*cast to avoid warnings, this is actually WCHAR */
           SDL_wchar2byte((void*)& pFrag);   /* transform to byte */
		}
  	    if (pFrag)
		{
           iBaseLength = B_STRINGLENGTH((const char *)pNewBase);
           pUser->pbURLBase = NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pFrag)+iBaseLength+2);
		   if (pUser->pbURLBase)
           {
             B_COPYSTRING((CHAR*)pUser->pbURLBase,(CHAR*)pNewBase);
             B_COPYSTRING((CHAR*)(&(pUser->pbURLBase[iBaseLength])),(CHAR*)"#");
             B_COPYSTRING((CHAR*)(&(pUser->pbURLBase[iBaseLength+1])),(CHAR*)pFrag);
             DEALLOC(&pNewBase);
           }
		}
		else
		{
			pUser->pbURLBase = pNewBase;
		}

	 }
	 else  /* there is a frag, check if valid, if not change to first card */
	 {
	   ELEMENTTYPE* pCard=NULL;
	   WCHAR* pTheCardName=NULL;
   
	   BYTE* pNewURL=NULL;
	   BYTE* pbFragAncor=NULL;

	   BOOL overflow=FALSE;

	   pTheCardName = wip_byte2wchar(pFrag);
	   pCard = WML_GetCard (pUser, pTheCardName);

	   pUser->pbURLBase = pNewBase;

	   if (pCard && ( ((CARDELEMENT*)(pCard))->pchId ))
	   {
		   if ( COMPARESTRING(pTheCardName, ( ((CARDELEMENT*)(pCard))->pchId )  ) !=0 )
		   {
			   BYTE* pCardFrag=wip_wchar2byte(( ((CARDELEMENT*)(pCard))->pchId ), &overflow);

			   if (pCardFrag!=NULL && !overflow)
			   {
				   pbFragAncor=NEWARRAY(BYTE, B_STRINGLENGTH((const char*)pCardFrag)+2);
				   *pbFragAncor='#';
				   B_COPYSTRING( (char*)(pbFragAncor+1), (const char*)pCardFrag );

				   if (b_Resolve(pNewBase, pbFragAncor, &pNewURL))
				   {
					   DEALLOC(&pNewBase);
	    			   pUser->pbURLBase=pNewURL;
				   }
				   DEALLOC(&pbFragAncor);

			   }
			   DEALLOC(&pCardFrag);

		   }
	   }
	   DEALLOC(& pTheCardName);

	 }

     DEALLOC(&pFrag);
  }

  #ifdef WAE_DEBUG
    PrintChar("Set new base URL:");
    PrintChar(pUser->pbURLBase);
  #endif
}

/*--------------------------------------------------------------------
Returns IANA code if any and the content-type as a reference string 
-----------------------------------------------------------------------*/

void* SDL_GetDetailedContentType(void* pTheHeaderData)
{
    HEADERDEF *pHeaderList = NULL;
    HEADERELEMENT *pHeaderElement = NULL;
    BYTE* pbContentTypeText=NULL;
    BOOL fError=TRUE;
	WSPPARAMETERS* pWSPParam = NULL;

    pHeaderList = (HEADERDEF*)pTheHeaderData;

    if (pHeaderList !=NULL)
    {
       pHeaderElement = WSP_GetHeaderWK ( 1, Field_Content_Type, pHeaderList->pHeadList );
       if (pHeaderElement !=NULL)
       {
          pbContentTypeText= WSP_GetContentTypeString (pHeaderElement, &fError, &pWSPParam);
		  if (pWSPParam)
	         WSP_DeleteParameters (&pWSPParam);
	   }
    }
	return pbContentTypeText;
}


CONTENTINFOSTRUCT* SDL_CreateResponseInfo(void)
{
    CONTENTINFOSTRUCT* pCStruct=NULL;
	pCStruct = NEWSTRUCT(CONTENTINFOSTRUCT);
	
	if (pCStruct)
    {
		pCStruct->pContentTypeAsText = NULL;
		pCStruct->iIANACharID = IANA_CHARSET_INVALID;
		pCStruct->iPIVersion =1;  /* 1=unknown */
		pCStruct->iContentTypeAsInt =0;	
		pCStruct->fCacheControl =0;	
	}
	return pCStruct;
}

void SDL_DeleteResponseInfo(void** ppTheInfo)
{
	if (ppTheInfo && *ppTheInfo)
    {
	  DEALLOC( & ((CONTENTINFOSTRUCT*)(*ppTheInfo))->pContentTypeAsText);
      DEALLOC(ppTheInfo);
	}
}

BOOL SDL_GetResponseInformation( int iScheme,  void* pTheHeaderData, void** ppResult, void** ppContentType)
{
    HEADERDEF *pHeaderList = NULL;
    HEADERELEMENT *pHeaderElement = NULL;
    BOOL fError=TRUE;
    WSPPARAMETERS* pWSPParam = NULL;
    CONTENTINFOSTRUCT* pCStruct=NULL;

    pHeaderList = (HEADERDEF*)pTheHeaderData;

    pCStruct = SDL_CreateResponseInfo();

	if (!pCStruct)
		return FALSE;
	
    SDL_DeleteResponseInfo(ppResult);

	*ppResult = pCStruct;

    switch (iScheme)
	{ 
		case Scheme_http:
		case Scheme_https:
		case Scheme_wapdevice:

	        if (!pHeaderList)     /* do not proceed use default instead */
		       return TRUE;			

			/* get the first contenttypefield, there should only be one*/
			pHeaderElement = WSP_GetHeaderWK ( 1, Field_Content_Type, pHeaderList->pHeadList );
			if (pHeaderElement !=NULL)
			{
			  /* get contenttypevalue, represented as a byte */
			  pCStruct->iContentTypeAsInt = WSP_GetContentTypeByte (pHeaderElement, &fError, & pWSPParam); 
              
			  if (pWSPParam)
			  {
			     pCStruct->iIANACharID = pWSPParam->iCharset;
				 pCStruct->iPIVersion = pWSPParam->iLevel;
	             WSP_DeleteParameters (&pWSPParam);
			  }

			  /* do the conversion to a stringrepr. */
			  if (fError!=TRUE)
			  {
				 WSP_ConvertContentStringByte(&pCStruct->iContentTypeAsInt, & pCStruct->pContentTypeAsText);
			  }
			}

		  
		/*	  if (#SDL(ContentTypeAsText)== NULL && #SDL(ResponseData).Status == 0x20 )
				SDL_OutputError( (UA*)#SDL(UA), ERR_WAE_UA_WSP_RESPONSE_INVALID, ERRTYPE_INFORMATION ); */ 
			  
			pCStruct->fCacheControl = SDL_GetCacheControl(pHeaderList);
 			break;

		case Scheme_file:
		case Scheme_function:
	        if (!*ppContentType)     /* do not proceed use default instead */
		       return TRUE;	

			pCStruct->pContentTypeAsText = *ppContentType;
			*ppContentType = NULL;
			pCStruct->iIANACharID = cfg_wae_ua_fileCharEncoding;
			pCStruct->fCacheControl = 0;
			pCStruct->iContentTypeAsInt = 0;
			WSP_ConvertContentStringByte(&pCStruct->iContentTypeAsInt, & pCStruct->pContentTypeAsText);
			break;
	}

	*ppResult = pCStruct;

	return TRUE;

}

  /*--------------------------------------------------------------
Cleanup function to be used when the response header is
ready to be removed
----------------------------------------------------------------*/
void DeleteWSPHeader(void** ppTheHeaderData)
{
  HEADERDEF** ppHeaderList;
  ppHeaderList = (HEADERDEF**)ppTheHeaderData;
  if (*ppTheHeaderData != NULL)
  {
    DeleteHEADERDEF(*ppHeaderList);
    *ppHeaderList=NULL;
  }
}


void SDL_DeleteResponseHeaderInfo( int iScheme, void** ppHeaderHandle, void** ppResponseInfo)
{
  if (iScheme == Scheme_https || iScheme == Scheme_http || iScheme == Scheme_wapdevice)
    DeleteWSPHeader( ppHeaderHandle );


  SDL_DeleteResponseInfo(ppResponseInfo);
}



/*--------------------------------------------------------------------
Converts a content-type string to byte
-----------------------------------------------------------------------*/
int SDL_ContentTypeTextToInt(void* pbText)
{
    BYTE iValue;
    BYTE* pbContentType;
    pbContentType = (BYTE*)pbText;

    if (pbText)
    {
      if (WSP_ConvertContentStringByte(&iValue, & pbContentType))
          return iValue;
    }
    return 0;
}

int SDL_GetCacheControl(void* pInHeaderList)
{
   HEADERDEF *pHeaderList = NULL;
   HEADERELEMENT *pHeaderElement = NULL;
   BOOL fError=TRUE;
   UINT32 uiDummy=0;
   pHeaderList = (HEADERDEF*)pInHeaderList;
   if( pHeaderList != NULL )
   {
 	  if( pHeaderList->pHeadList != NULL )
	  {
		pHeaderElement = WSP_GetHeaderWK ( 1, Field_Cache_Control, pHeaderList->pHeadList );
		if( pHeaderElement != NULL )
		{
			return WSP_GetCacheControlByte( pHeaderElement, &uiDummy, &fError );
		}
	  }
   }
   return 0;
}

BOOL CompareContentType (BYTE* pbAccept, BYTE* pbReceived)
{
    BYTE* p = NULL;
    BYTE* pRec = NULL;
    int idx = 0;

    p = pbAccept;

	while (*p) {
	  while (wae_iswhitespace (*p))
		  p++;

	  idx = 0;
	  pRec = pbReceived;
      
	  while (*p && (*p == *pRec)) {		/*prefix match */
      p++;
      pRec++;
      idx++;	
    }

	  if (idx) {
		  if (*p == '*')
        return TRUE;		/* check wildcard */

      if (((*p == '\0') || (*p == ',') || wae_iswhitespace (*p)) &&
          (*pRec == '\0'))
        return TRUE;  /* check absolute match */
	  }
	  while (*p && (*p++ != ','))
      ;	    /* to next , */
	}
    
	return FALSE;	
}

/*--------------------------------------------------------------------
Check if the contentype corresponds to the acceptcontenttype, 1 (TRUE) if OK
-----------------------------------------------------------------------*/
int SDL_CheckResponseContentType(const void* pResponseInfo, const void* pCompareContentType)
{
    BYTE    *pbRecievedContentType=NULL;
    BYTE    *pbAcceptContentType=NULL;
    BOOL    iRetVal=FALSE;
    BYTE    *pContentTypeText=NULL;

    if (pResponseInfo==NULL)
	  return FALSE;
    
	pContentTypeText = ((const CONTENTINFOSTRUCT*)pResponseInfo)->pContentTypeAsText;

    if (pContentTypeText==NULL || pCompareContentType==NULL)
      return FALSE;

    if (strstr( pCompareContentType, "*/*" )) /*gbu 000204*/
      return TRUE;

    /* make downcase work copies */
    pbRecievedContentType = NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pContentTypeText)+1);
    if (pbRecievedContentType)
        DowncaseString (pbRecievedContentType, (BYTE*)pContentTypeText);

    pbAcceptContentType = NEWARRAY(BYTE,B_STRINGLENGTH(pCompareContentType)+1);
    if (pbAcceptContentType)
        DowncaseString (pbAcceptContentType, (BYTE*)pCompareContentType);

    iRetVal =  CompareContentType(pbAcceptContentType, pbRecievedContentType);

    DEALLOC(&pbAcceptContentType);
    DEALLOC(&pbRecievedContentType);

    return iRetVal;
}


#ifdef USE_KSC5601

#ifdef FEATURE_WAP //eqs func crash!!
WCHAR* STRCHRWAP(WCHAR* str, WCHAR c)
#else
WCHAR* STRCHR(WCHAR* str, WCHAR c)
#endif //FEATURE_WAP
{
	WCHAR	*p = str;

	for ( ; *p ; p++ )
		{
		if ( *p == c )
			return p;
		}
	return NULL;
}

#define wae_KCSKoreanFix(c)    (_wae_ctype[(c)] & (_C | _SP | _ARES | _UW | _DL))

BYTE*
b_EscapeStringSpecial (const BYTE* pbString)
{
  const BYTE *p;
  BYTE       *q, *s;
  UINT16     l = 0;
  UINT16     r = 0;

  if (pbString == NULL)
    return NULL;

  for (p = pbString; *p; p++) {
    if ( ((wae_KCSKoreanFix (*p)) && !((*p == 0x25)) ) || (*p > 0x7f))

      r++;
    else
      l++;
  }

  if ((s = NEWARRAY (BYTE, l + 3 * r + 1)) == NULL)
    return NULL;

  for (p = pbString, q = s; *p; p++) {
    if ( ((wae_KCSKoreanFix (*p)) && !((*p == 0x25)) ) || (*p > 0x7f)) {
/*    if ( *p > 0x7f ) {  modified by young 2000.6.16 */
      *q++ = '%';
      ByteToHex (*p, q);
      q += 2;
    }
    else
      *q++ = *p;
  }
  *q = '\0';

  return s;
}


BYTE* Unicode2byteSpecialKSCConvert(WCHAR* pURL)
{
	WCHAR* pURLString = pURL;
	WCHAR* pFragStart = NULL;
	WCHAR* pQueryStart = NULL;
	WCHAR* pQuerySubString = NULL;
	BYTE* pNewURL=NULL;
	BYTE* pByteQueryStart=NULL;
	BYTE* pURLStringAsByte=NULL;
	BYTE* pFragAsByte=NULL;
	UINT8 iFragLen=0;
	INT16 iQueryLen=0;
	INT16 iBaseLen = 0;
	INT16 iByteQueryLen=0;
	INT16 iTotalNewLength = 0;
	BOOL fDummy;
    INT16 iBuffLength;

    BYTE* pResultBuffer;

	if (!pURLString)
		return NULL;

	/* find start of query and fragment */
#ifdef FEATURE_WAP //eqs func crash!!
    pQueryStart = STRCHRWAP(pURLString, (WCHAR)'?');
    pFragStart = STRCHRWAP(pURLString, (WCHAR)'#');
#else
    pQueryStart = STRCHR(pURLString, (WCHAR)'?');
    pFragStart = STRCHR(pURLString, (WCHAR)'#');
#endif //#ifdef FEATURE_WAP //eqs func crash!!


	if (!pQueryStart)
	{
	   return wip_wchar2byte( pURLString, & fDummy );
	}
	pQueryStart++;
	if (pFragStart)	
	   iFragLen = (UINT8)STRINGLENGTH(pFragStart);

	iQueryLen = STRINGLENGTH(pQueryStart) - iFragLen;

	if (iQueryLen <=0)
	{
	   return wip_wchar2byte( pURLString, & fDummy );
	}

	/* copy query */
    
	pQuerySubString = NEWARRAY(WCHAR,iQueryLen+1);
    COPYSTRINGN( pQuerySubString, pQueryStart, iQueryLen);
	pQuerySubString[iQueryLen]=0;

	/* ... and transcode */

    iBuffLength = KSCStrLenOfUni((WCHAR*) pQuerySubString );
	if (iBuffLength)
	  pResultBuffer = NEWARRAY(BYTE,iBuffLength+1);

    iBuffLength = Uni2KSCString(pQuerySubString,pResultBuffer);
	DEALLOC(&pQuerySubString);

	/* escape result */
    pByteQueryStart = b_EscapeStringSpecial(pResultBuffer);
    
	DEALLOC(&pQuerySubString);
	if (pByteQueryStart)
		iByteQueryLen=B_STRINGLENGTH(pByteQueryStart);

	/* copy fragment */
	if (pFragStart)
	pFragAsByte = wip_wchar2byte( pFragStart, & fDummy );
	
	/* concatenate old url with new query and old fragment */
	iBaseLen = (pQueryStart-pURLString);
	iTotalNewLength = iBaseLen + iByteQueryLen + iFragLen;

	pNewURL = NEWARRAY(BYTE,iTotalNewLength+1);

	pURLStringAsByte = wip_wchar2byte( pURLString, & fDummy );

    B_COPYSTRINGN(pNewURL, pURLStringAsByte, iBaseLen);
	DEALLOC(&pURLStringAsByte);

	if (iByteQueryLen)
		B_COPYSTRINGN(& pNewURL[iBaseLen], pByteQueryStart, iByteQueryLen );
	if (iFragLen)
        B_COPYSTRING(& pNewURL[iBaseLen+iByteQueryLen], pFragAsByte);

    pNewURL[iTotalNewLength] = 0;

	DEALLOC(& pByteQueryStart );	
 	DEALLOC(& pResultBuffer );

	return pNewURL;
	
}

#endif


/*---------------------------- Functionality to handle new requests etc. ----------------------------------*/

/*-----------------------------------------------------------
Retrieves the URL to be used /and creates a history element if necessary) resolves it if necessary and
normalizes it. pURLElement can be different types of elements GoElement, ImageElement, or Text.
-------------------------------------------------------------*/
void SDL_GetElementURL( void* pUser, void* pURLElement, void** pURL, void** ppHistoryItem)
{
   WCHAR* pwchTemp=NULL;
   BYTE* pbResult=NULL;
/* gbu 000211   not used
   BYTE* pbQuery=NULL;
*/
   BYTE* pbTemp=NULL;
   ELEMENTTYPE* pElm;
   BOOL fDummy=FALSE;

   DEALLOC( pURL );

   if (pURLElement!=NULL)
   {
     pElm = ((ELEMENTTYPE*)pURLElement);
     switch (SDL_GetElementType(pURLElement))
	 {
		 case Type_IMG:
		   pwchTemp = SDL_GetText(pUser, ((IMGELEMENT*)(pElm))->pSrc );
		   pbTemp = wip_wchar2byte( pwchTemp, & fDummy );
		   break;
		 case Type_GO:
			 /* evalute the postfields etc. */
		   CreateNavigationData( (UA*)pUser, (GOELEMENT*)(pElm), (HISTORYITEM**)ppHistoryItem, & pbResult );
		   break;
		 case Type_A:
		   pwchTemp = SDL_GetText(pUser, ((AELEMENT*)(pElm))->pHref );
		   pbTemp = wip_wchar2byte( pwchTemp, & fDummy );
		   break;
		 case Type_GenericText:
		   pwchTemp = SDL_GetText(pUser, (ELEMENTTYPE*)pURLElement);  /*since this is a elementtype */
		   pbTemp = wip_wchar2byte( pwchTemp, & fDummy );
		   break;
     }
   }
   if (pwchTemp)
   {
     DEALLOC(&pwchTemp);
     /* resolve if necessary */
     b_Resolve (((UA*)pUser)->pbURLBase, pbTemp, & pbResult);
     DEALLOC(&pbTemp);
   }
   
   pbTemp = b_CompleteURLHeuristically(pbResult);  /* fix to append trailing slashes */
   DEALLOC(&pbResult);
   pbResult = pbTemp;
   *pURL = pbResult;

   #ifdef WAE_DEBUG
     PrintChar("Get URL (from element)");
     PrintChar("Base:");
     PrintChar(((UA*)pUser)->pbURLBase);
     PrintChar("Go:");
     PrintChar(pbResult); 
   #endif
}

/*-----------------------------------------------------------------
-------------------------------------------------------------------*/
void CreateNavigationData( UA* pUserAgent, GOELEMENT* pGoElement, HISTORYITEM** ppHistoryItem, BYTE** ppTargetURL )
{
/* aspn 001101
    ELEMENTTYPE* pPostField = NULL;
*/
    BYTE* pbAccChar=NULL;
    INT16 iCharEnc=IANA_CHARSET_INVALID;
/* aspn 001101
    BOOL fContainsAcceptCharset=FALSE;
*/
    BOOL fDummy=FALSE;
    WCHAR* pwchTemp=NULL;
	BYTE* pbTemp=NULL;
	BYTE* pbTemp2=NULL;
	BYTE* pbSendData=NULL;
	UINT16 iSendDataLen=0;
/* aspn 001101
    pPostField = SDL_GetElementContent( pGoElement);
*/
	SDL_GetElementContent( pGoElement);

	if (!pGoElement)
	{
	   *ppHistoryItem=NULL;
       *ppTargetURL=NULL;  
	   return;
	}
    
	/* determine what char encoding to use */

    if ( pGoElement->pchAccChar )   /* if wml content contains ACCEPT-CHARSET */
	{
/* aspn 001101
	  fContainsAcceptCharset=TRUE;'
*/
      pbAccChar = NEWARRAY(BYTE, STRINGLENGTH( pGoElement->pchAccChar ) +1 );
	  if (pbAccChar)
	  {
         pbTemp = wip_wchar2byte( pGoElement->pchAccChar, & fDummy);
	 	 DowncaseString (pbAccChar, pbTemp);
		 DEALLOC(&pbTemp);
		 iCharEnc = IANA_CHARSET_LATIN1;

/* GBU 000204
         if (B_STRSTRING((CHAR*)pbAccChar, (CHAR*)"ucs-2"))
*/
         if (strstr((const char *)pbAccChar, "ucs-2"))
			iCharEnc= IANA_CHARSET_UCS2;

/* GBU 000204
		 if (B_STRSTRING((CHAR*)pbAccChar, (CHAR*)"us-ascii"))
*/
         if (strstr((const char *)pbAccChar, "us-ascii"))
			iCharEnc= IANA_CHARSET_USASCII;
        
/* GBU 000204
		 if (B_STRSTRING((CHAR*)pbAccChar, (CHAR*)"utf-8"))
*/
         if (strstr((const char *)pbAccChar, "utf-8"))
			iCharEnc= IANA_CHARSET_UTF8;
	    
	    DEALLOC(&pbAccChar);
	  }
    }
	/* else use document charset */
	else            
	  iCharEnc = pUserAgent->iTextEncodFormat;


	iSendDataLen = 0;
	/* create a target url  */
    pwchTemp = SDL_GetText(pUserAgent, pGoElement->pHref );

#ifdef USE_KSC5601
	pbTemp2 = Unicode2byteSpecialKSCConvert(pwchTemp);
#else
	pbTemp2 = wip_wchar2byte( pwchTemp, & fDummy );
#endif

    DEALLOC(&pwchTemp);

	pbTemp = b_EscapeBlanks( pbTemp2 );		/*  Maybe not nedded in KSC5601 mode */
	DEALLOC(&pbTemp2);
	pbTemp2 = pbTemp;

	b_Resolve (pUserAgent->pbURLBase, pbTemp2, & pbTemp);
    DEALLOC(&pbTemp2);
	
	/* --- create post/query data, ignore postfields if it´s the same deck  */
	if (! b_EqualURL ( ((UA*)pUserAgent)->pbURLBase, (BYTE*)pbTemp, ALL_COMP ^ FRAG_COMP) )  
	{
		if (pGoElement->iEnctype  == GO_urlencoded)
		{
		   CreatePostString( pUserAgent, pGoElement, iCharEnc, & pbSendData, & iSendDataLen);
		}
		else if (pGoElement->iEnctype  == GO_formdata)
		{
		   CreatePostBody( pUserAgent,  pGoElement, iCharEnc, & pbSendData, & iSendDataLen); 
		}
	}

	 *ppTargetURL = pbTemp;

    /* add any query data */

    if ( pGoElement->iMethod == M_Get && pGoElement->iEnctype == GO_urlencoded)
	{
      if (pbSendData)
	  {
	    *ppTargetURL = b_AppendToQuery (pbTemp, pbSendData);
	    DEALLOC(&pbTemp);
	  }
	  DEALLOC(&pbSendData);
	  iSendDataLen = 0;
	}

    /* to support stupid webservers with no ;charset implementation use cfg_wae_ua_methodPostCharsetOverride*/
    if ( cfg_wae_ua_methodPostCharsetOverride && !(pGoElement->pchAccChar) )
      iCharEnc=IANA_CHARSET_INVALID;

	/* create a history item from all data */
    if (ppHistoryItem) {
      Delete_HistoryItem ((void**)ppHistoryItem);
      *ppHistoryItem = CreateAndInitNewHistoryItem (NULL, NULL, pbSendData,
                                                    iSendDataLen,
                                                    (UINT8)pGoElement->iMethod,
                                                    pGoElement->iEnctype,
                                                    iCharEnc);
    }
	else
	{
		DEALLOC(&pbSendData);
	}
}

 /*----------------- transcode according to characterencoding -------------- */

void TransCodeData(INT16 iCharEnc, WCHAR* pwchName, WCHAR* pwchValue, BYTE** ppNameResult, BYTE** ppValueResult )
{
   BYTE* pResStr1 = NULL;
   BYTE* pResStr2 = NULL;
   BYTE* pDummy = NULL;
   BOOL fDummy;

   switch (iCharEnc)
   {
   case IANA_CHARSET_UTF8:  /* transcode to UTF-8 */
	   {
		  UINT32 iNameLen =0;
		  UINT32 iValueLen =0;

		  if (pwchName)
		  {	  
			/* get the encoded length */   
			iNameLen = Unicode2UTF8_calcLen((WCHAR*) pwchName, STRINGLENGTH((WCHAR*)pwchName) );
			if (iNameLen)
				pResStr1 = NEWARRAY(BYTE,iNameLen+1);

			/* encode the unicode string */
		 	if (pResStr1 && Unicode2UTF8_convert( pwchName, STRINGLENGTH( pwchName ), pResStr1, iNameLen, & pDummy ) )
			{
				pResStr1[iNameLen] = 0; /* ensure 0 termination */
			}
			pDummy=NULL; 
 
		  }

		  if (pwchValue)
		  {
			/* get the encoded length */
			iValueLen = Unicode2UTF8_calcLen(  pwchValue, STRINGLENGTH( pwchValue ) );

			if (iValueLen)
			  pResStr2 = NEWARRAY(BYTE,iValueLen+1);

			/* encode the unicode string */
			if (pResStr2 && Unicode2UTF8_convert( pwchValue, STRINGLENGTH(pwchValue), pResStr2, iValueLen, & pDummy ))
			{
			   pResStr2[iValueLen] = 0;    /* ensure 0 termination */
			}
			pDummy=NULL;
		  }

	   break;
	   }
   case IANA_CHARSET_USASCII :
   case IANA_CHARSET_LATIN1 : 
 	  pResStr1 = wip_wchar2byte(pwchName, & fDummy);
	  pResStr2 = wip_wchar2byte(pwchValue, & fDummy);
	  break; 
   case IANA_CHARSET_UCS2 :
      pResStr1 = GenCopyString(2, pwchName);
      pResStr2 = GenCopyString(2, pwchValue);
	  break;
   default:  /* best effort ISO-LATIN... */
	   {
		  pResStr1 = wip_wchar2byte(pwchName, & fDummy);
		  pResStr2 = wip_wchar2byte(pwchValue, & fDummy);
		  iCharEnc = IANA_CHARSET_LATIN1;
		  break; 
	   }
   }

   *ppNameResult = pResStr1;
   *ppValueResult = pResStr2;

}		   

/*-----------------------------------------------------------------------------------------
 create a postbody according to multipart/form-data 
-------------------------------------------------------------------------------------------*/
void CreatePostBody(UA* pUserAgent, GOELEMENT* pGoElement, INT16 iCharEnc, BYTE** pbData, UINT16* piDataLen)
{
	BYTE* pPostBody = NULL;
    UINT32 iPostBodyLen = 0;
	MULTIPARTSTR* pMultiPartHandle=NULL;
	ELEMENTTYPE* pPostField = NULL;
	WCHAR* pwchName=NULL;
	WCHAR* pwchValue=NULL;
	BYTE* pbNameData = NULL;
	BYTE* pbValueData = NULL;
	UINT32 iDataLen=0;

    pPostField = SDL_GetElementContent( pGoElement );

	if (!pPostField)
	{
	   *pbData = NULL;
	   *piDataLen =0; 
	   return;
	}

	/* Changed 2000-02-09, JPR */

	while ( (pPostField) )
	{

		 if ((pPostField->iType==Type_POSTFIELD) )
		 {
#ifdef USE_KSC5601
			WCHAR   *pWCHAR=NULL;
			INT16 iBuffLength=0;
			BYTE* pResultBuffer=NULL;
#endif

			   /* evaluate name and variable */
			   pwchName =  SDL_GetText(pUserAgent, ((POSTFIELDELEMENT*)(pPostField))->pName); 	

#ifdef USE_KSC5601
			   pWCHAR = SDL_GetText(pUserAgent, ((POSTFIELDELEMENT*)(pPostField))->pValue);

			   iBuffLength = KSCStrLenOfUni(pWCHAR);
			   if (iBuffLength)
			   {
				  pResultBuffer = NEWARRAY(BYTE,iBuffLength+1);
				  iBuffLength = Uni2KSCString(pWCHAR, pResultBuffer);
			   }
			   OSConnectorFree(pWCHAR);
			   pwchValue = NULL;
#else
			   pwchValue = SDL_GetText(pUserAgent, ((POSTFIELDELEMENT*)(pPostField))->pValue);
#endif

			   /* transcode according to characterencoding  */
			   TransCodeData(iCharEnc, pwchName, pwchValue, & pbNameData, & pbValueData );
#ifdef USE_KSC5601
				pbValueData = pResultBuffer;
				pResultBuffer = NULL;
#endif
				DEALLOC( & pwchName);
				DEALLOC( & pwchValue); 


			   if (pbValueData!=NULL)
			   {
				   /* Get length of value data including termination character */
				   iDataLen=B_STRINGLENGTH((const char *)pbValueData)+1;

				   WAE_AddTextFormDataPart (&pMultiPartHandle, pbNameData, &pbValueData, iDataLen, iCharEnc );

				   /* Delete the memory for pNameData. pValueData is taken over by 
					  the function WAE_AddTextFormDataPart. */
				   DEALLOC(&pbNameData);

			   }
		 } 
	   pPostField = WML_GetNextElement( pPostField);
	}

	if (!(WSP_BuildMultipart (& pMultiPartHandle , &pPostBody, &iPostBodyLen, 0))) 
	{
		/* Multipart creation failed. Delete Multipart list */
		WSP_DeleteMultipartListWithContent (& pMultiPartHandle );
	}


	*pbData=pPostBody;
/*GBU 000210
     *piDataLen =iPostBodyLen;
*/
    *piDataLen = (UINT16)iPostBodyLen;
}


/*----------------------------------------------------------------------------------------- 
Create a application/x-url-encoded string to be attached to either the URL or the post body 
-------------------------------------------------------------------------------------------*/
void CreatePostString(UA* pUserAgent,  GOELEMENT* pGoElement, INT16 iCharEnc, BYTE** ppString, UINT16* piDataLen ) 
{

    void    *pNameData;
    void    *pValueData;
    WCHAR   *pWValue, *pWName;
    BYTE* pResStr1=NULL;
    BYTE* pResStr2=NULL;
    BYTE* pResult=NULL;
    UINT32 iTotalLength =0;
	UINT32 iNameLen =0;
	UINT32 iValueLen =0;
    ELEMENTTYPE* pPostField = NULL;
    POSTSTRINGPART* pListStart=NULL;
    POSTSTRINGPART* pNewStringPart=NULL;
    POSTSTRINGPART* pPrevStringPart=NULL;

    pPostField = SDL_GetElementContent( pGoElement );

	if (!pPostField)
	{
	   *ppString = NULL;
	   *piDataLen =0; 
	   return;
	}

	while (pPostField)
	{
	  if (pPostField->iType == Type_POSTFIELD)
      {
        pNewStringPart=NEWSTRUCT(POSTSTRINGPART);
	    if (pNewStringPart)
        {
#ifdef USE_KSC5601
			WCHAR   *pWCHAR=NULL;
			INT16 iBuffLength=0;
			BYTE* pResultBuffer=NULL;
#endif
           if (!pListStart)
             pListStart=pNewStringPart;

           pNewStringPart->pData=NULL;
		   
		   /* evaluate name and variable */
           pWName = SDL_GetText(pUserAgent, ((POSTFIELDELEMENT*)(pPostField))->pName); 	

#ifdef USE_KSC5601
           pWCHAR = SDL_GetText(pUserAgent, ((POSTFIELDELEMENT*)(pPostField))->pValue);

           iBuffLength = KSCStrLenOfUni(pWCHAR);
		   if (iBuffLength)
		   {
	          pResultBuffer = NEWARRAY(BYTE,iBuffLength+1);
		      iBuffLength = Uni2KSCString(pWCHAR, pResultBuffer);
		   }
           OSConnectorFree(pWCHAR);
		   pWValue = NULL;
#else
           pWValue = SDL_GetText(pUserAgent, ((POSTFIELDELEMENT*)(pPostField))->pValue);
#endif

           /* transcode according to characterencoding  */
           TransCodeData(iCharEnc, pWName, pWValue, (BYTE**)&pNameData, (BYTE**)&pValueData);
#ifdef USE_KSC5601
			pValueData = (void*)pResultBuffer;
			pResultBuffer = NULL;
#endif
		   DEALLOC(&pWName);
		   DEALLOC(&pWValue);
		   

           /* ------------------ escape result ---------------------- */

		   if (iCharEnc==IANA_CHARSET_UCS2)
		   {
             pResStr1 = b_UnicodeEscape((WCHAR*)pNameData);
             pResStr2 = b_UnicodeEscape((WCHAR*)pValueData);		   
		   }
		   else
		   {
               pResStr1 = b_EscapeString((const BYTE *)pNameData);
               pResStr2 = b_EscapeString((const BYTE *)pValueData);
           }

		   DEALLOC(&pNameData);
		   DEALLOC(&pValueData);

		   if (pResStr1)
			  iNameLen = B_STRINGLENGTH((const char *)pResStr1);
		   if (pResStr2)
			  iValueLen = B_STRINGLENGTH((const char *)pResStr2);

		   /* concatenate the strings ignore if variable name is empty*/
		   
		   if (pResStr1)
           {
		      pResult = NEWARRAY(BYTE,iNameLen+iValueLen+2);
			  if (pResult)
              {
				  B_COPYSTRING((CHAR*)pResult,(CHAR*)pResStr1);
                  B_COPYSTRING((CHAR*)(&pResult[iNameLen]),"=");
				  if (pResStr2)
				     B_COPYSTRING((CHAR*)(&pResult[iNameLen+1]),(CHAR*)pResStr2);
			  }
           }

   		   DEALLOC(&pResStr1);
		   DEALLOC(&pResStr2);
		   
		   /* put result in linked list */ 
           
		   pNewStringPart->pData=pResult;
		   if (pPrevStringPart)
              pPrevStringPart->pNextField=pNewStringPart;
		   pNewStringPart->pNextField=NULL;
		   pPrevStringPart = pNewStringPart;
		   
		   iTotalLength = iTotalLength+iNameLen+iValueLen+2; /*eg testname=testvalue */
		}
	  }
      pPostField = WML_GetNextElement( pPostField);
	}

/* ------------------ assemble the list ----------------------- */

	pResult=NULL;
    pNewStringPart=pListStart;
    if (iTotalLength)
	{
	  pResult = NEWARRAY(BYTE,iTotalLength);
      iTotalLength=0;
	  if (pResult)
        while (pNewStringPart)
		{
          if (pNewStringPart->pData)
		  {
  	        B_COPYSTRING((CHAR*)(&pResult[iTotalLength]),(CHAR*)pNewStringPart->pData);
		    iTotalLength = iTotalLength + B_STRINGLENGTH(pNewStringPart->pData);
		    DEALLOC(&pNewStringPart->pData);
		  }
		  pPrevStringPart=pNewStringPart;
		  pNewStringPart=pNewStringPart->pNextField;
		  DEALLOC(&pPrevStringPart);
		  if (pNewStringPart)
          {
		     B_COPYSTRING((CHAR*)(&pResult[iTotalLength]),(CHAR*)"&");
		     iTotalLength++;
		  }
		}
	}

	*ppString = pResult;
	*piDataLen = (UINT16)iTotalLength;
}

/* used by postcontent */
void AddPostData(URLREQSTRUCT* pRequestStruct, int iSendMode, BYTE* pPostData, UINT32 iPostDataLen, BYTE* pContentType, BYTE* pContentDispositionFileName, UINT32 iTotalSize) 
{
	MULTIPARTSTR* pMultiPartHandle=NULL;
	BYTE* pTemp=NULL;

    pRequestStruct->iContentCharSet = IANA_CHARSET_INVALID;
	pRequestStruct->iMethod = M_Post;
	
	if (iSendMode == SENDMODE_MULTIPART_FORMDATA)
	{
		WAE_AddGenericFormDataPart (& pMultiPartHandle, & pPostData, (UINT16)iPostDataLen, pContentType, pContentDispositionFileName);
		if (!(WSP_BuildMultipart (& pMultiPartHandle, & pRequestStruct->pbBody, & pRequestStruct->iBodyLength, iTotalSize)))
		{
			/* Multipart creation failed. */

			pRequestStruct->pbBody = NULL;
			pRequestStruct->iBodyLength = 0;
		}
		pRequestStruct->iContentType=0x24;
		WSP_DeleteMultipartListWithContent (&pMultiPartHandle);
		DEALLOC( &pContentType );
		DEALLOC( &pPostData);
	}
	else if (iSendMode == SENDMODE_BINARY)
	{
		BYTE	bValue = NULL;

		pRequestStruct->iBodyLength = iPostDataLen;
		pRequestStruct->pbBody = pPostData;

		if( pContentType != NULL )
			if( WSP_ConvertContentStringByte( &bValue, &pContentType ) ){
				pRequestStruct->iContentType = bValue;
				DEALLOC(&pContentType);
			}
			else{
				pRequestStruct->iContentType = CONTENTTYPE_INVALID;
				pRequestStruct->pbContentType = pContentType;
			}
		else
			pRequestStruct->iContentType=0;
	}
	else	/* iSendMode == SENDMODE_URL_ENCODED */
	{
	   pTemp=EscapeALLString(pPostData, (UINT16)iPostDataLen);
       if (pTemp)
	   {
		   pRequestStruct->iBodyLength = B_STRINGLENGTH((const char*)pTemp)+1;
           pRequestStruct->pbBody = pTemp;
	   }
       pRequestStruct->iContentType=0x12;
	   DEALLOC( &pContentType );
	   DEALLOC( &pPostData);
	}

	DEALLOC( &pContentDispositionFileName );
}

void* AddMorePostData( void* vsPostData, int PostDataLen, int iSendMode )
{
   URLREQSTRUCT*	pURLStruct=NULL;
   BYTE*			pTemp = NULL;

   pURLStruct = New_URLREQSTRUCT();

   if (pURLStruct)
   {
		pURLStruct->iMethod = M_Post;

		if (iSendMode == SENDMODE_MULTIPART_FORMDATA || iSendMode == SENDMODE_BINARY)
		{
			pURLStruct->pbBody = (BYTE*)vsPostData;
			pURLStruct->iBodyLength = PostDataLen;
		}
		else	/* iSendMode == SENDMODE_URL_ENCODED */
		{
			pTemp=EscapeALLString( (BYTE*)vsPostData, (UINT16)PostDataLen);
			if (pTemp)
			{
				pURLStruct->iBodyLength = B_STRINGLENGTH((const char*)pTemp)+1;
				pURLStruct->pbBody = pTemp;
			}
			DEALLOC( &vsPostData);
		}
   }

  return pURLStruct;
}


/*-----------------------------------------------------------
Constructs a new requeststruct to be sent to wsp.
This function is to be used by common User Agents and the MMI agent, i.e when no specfic task is related to the request
-------------------------------------------------------------*/
/*
void* SDL_CreateRequestStruct( void* pWAEInObject, void* pDeprecated1, void* pDeprecated2, void* pbURL, void* pAccept)
*/
void* SDL_CreateRequestStruct( void* pbURL, void* pAccept)

{
   URLREQSTRUCT* pURLStruct=NULL;   

   pURLStruct = New_URLREQSTRUCT();

   if (pURLStruct)
   {
	  /* the URL that represents this request */
      pURLStruct->pbURL = (BYTE*)GenCopyString(1, pbURL);
       /* the content-types that are accepted by this request */
      pURLStruct->pbAccept= (BYTE*)GenCopyString(1,pAccept);

	  pURLStruct->iMethod = M_Get;   /*default method */

      #ifdef WAE_DEBUG
 	    PrintChar("New Request");
	    PrintChar((CHAR*)pURLStruct->pbURL);
	    PrintChar((CHAR*)pURLStruct->pbAccept);
	  #endif
   }

  return pURLStruct;
}

/*-----------------------------------------------------------
Constructs a new EXTENDED requeststruct to be sent to wsp.
This function is to be used by WML/WTA User Agents
-------------------------------------------------------------*/
void* SDL_CreateExtendedRequestStruct( void* pWAEInObject, void* pInUserAgent, void* pActiveElement, void* pbURL, void* pAccept, void* pInHistoryElement)
{
    GOELEMENT* pGoElement=NULL;
	UA* pUser=NULL;
    URLREQSTRUCT* pURLStruct=NULL;  
	HISTORYITEM* pHistoryElement;
    pURLStruct = New_URLREQSTRUCT();

    pWAEInObject = pWAEInObject;

    pHistoryElement = (HISTORYITEM*)pInHistoryElement;

	if (!pURLStruct)
		return NULL;

    pURLStruct->iMethod = M_Get;   /*default method */

	if (pActiveElement)
	{
	   if ( ((ELEMENTTYPE*)pActiveElement)->iType == Type_GO )
	   {
	 	  pGoElement = (GOELEMENT*)(pActiveElement);  
		  if (pGoElement->fSendRef)
		  {
		     pUser = (UA*)pInUserAgent;
		     /* referring URL */
		     pURLStruct->pbReferer = b_MinRelativePath ((BYTE*)pbURL, pUser->pbURLBase, TRUE);
		  }
	   }
	}

	if (pHistoryElement)
	{
        if (pHistoryElement->iPostDataLength)
		{
		    pURLStruct->pbBody = NEWARRAY(BYTE,pHistoryElement->iPostDataLength);
			B_COPYSTRINGN(pURLStruct->pbBody,pHistoryElement->pbPostData,pHistoryElement->iPostDataLength);  
	        pURLStruct->iBodyLength = pHistoryElement->iPostDataLength;
		}
		pURLStruct->iMethod = pHistoryElement->iMethod;
		pURLStruct->iContentType = pHistoryElement->iContentType;
		pURLStruct->iContentCharSet = pHistoryElement->iContentCharSet;
	}

    /* the URL that represents this request */
    pURLStruct->pbURL = (BYTE*)GenCopyString(1, pbURL);
    /* the content-types that are accepted by this request */
    pURLStruct->pbAccept= (BYTE*)GenCopyString(1,pAccept);

    #ifdef WAE_DEBUG
 	  PrintChar("New Request");
	  PrintChar((CHAR*)pURLStruct->pbURL);
	  PrintChar((CHAR*)pURLStruct->pbAccept);
    #endif

	return pURLStruct;
}



/*---------------------------------------------
Runs updates on all
external variables input variables
Returns the number of signals
----------------------------------------------*/
int SDL_SendSignalsInputVariables(void *pUser, void *pList)
{
   void* pListElm=NULL;

   /*broadcast a getInputString */
   pListElm = SDL_GetNextListElement(pList,NULL);

   while (pListElm!=NULL)
   {
     MMIa_getInputString( (UINT8)((UA*)pUser)->iViewID, (UINT8)SDL_GetListElementID(pListElm) );
     pListElm = SDL_GetNextListElement(pList,pListElm);
   }
   return SDL_GetCardinal(pList);
}

WCHAR* getLang(void* pElement, UINT16 iType)
{
	WCHAR* pLang=NULL;

	if (!pElement)
		return NULL;

	switch (iType)
	{
		case Type_WML:
			pLang = ((WMLELEMENT*)pElement)->pwchXmlLang;
			break;
		case Type_CARD:
			pLang = ((CARDELEMENT*)pElement)->pwchXmlLang;
			break;
		case Type_DO:
			pLang = ((DOELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_SELECT:
			pLang = ((SELECTELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_OPTGROUP:
			pLang = ((OPTGROUPELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_OPTION:
			pLang = ((OPTIONELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_INPUT:
			pLang = ((INPUTELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_FIELDSET:
			pLang = ((FIELDSETELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_IMG:
			pLang = ((IMGELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_A:
			pLang = ((AELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_ANCHOR:
			pLang = ((ANCHORELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_TABLE:
			pLang = ((TABLEELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_TD:
			pLang = ((TDELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_BR:
			pLang = ((BRELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_P:
			pLang = ((PELEMENT*)pElement)->pwchXmlLang; 
			break;
		case Type_B:
		case Type_BIG:
		case Type_EM:
		case Type_I:
		case Type_SMALL:
		case Type_STRONG:
		case Type_U:
			pLang = ((EMPHELEMENT*)pElement)->pwchXmlLang; 
			break;
		default:
			break;
   }
	return pLang;
}

void SDL_UpdateLanguage(UINT8 iViewID, void* pElement, void* pPrevElement,void* pLanguageStack )
{
	WCHAR* pLang=NULL;
	BYTE* pbLanguage = NULL;
    BOOL fDummy;
	void* pTemp;
	UINT16 iType=0;

	   /* if the previous element was a lang remove from stack */
	if (pPrevElement && pLanguageStack && pPrevElement == SDL_CheckLIFOStack( pLanguageStack ) )
	{
        pTemp = SDL_POPLIFOStack( pLanguageStack );
		pTemp = SDL_CheckLIFOStack( pLanguageStack );
		iType = ((ELEMENTTYPE*)pTemp)->iType;
		pLang = getLang(pTemp, iType);
		if (pLang)
          pbLanguage =  wip_wchar2byte(pLang ,& fDummy );
        MMIa_setLanguage (iViewID, (const char *)pbLanguage);
	}

	if (!pElement)
		return;

	iType = ((ELEMENTTYPE*)pElement)->iType;

	if (isTypeWMLText(iType))
		return;

	pLang = getLang(pElement, iType);

       /* if the current element has a lang put on stack */
	if (pLang)
    {
      DEALLOC(& pbLanguage);
      pbLanguage =  wip_wchar2byte(pLang ,& fDummy );
      MMIa_setLanguage (iViewID, (const char *)pbLanguage);
	  DEALLOC(& pbLanguage);
      SDL_PUSHLIFOStack( pLanguageStack, pElement );
    }
	else if (pbLanguage)
	{
	  DEALLOC(& pbLanguage); 
      MMIa_setLanguage (iViewID, NULL);
	}
}


int SDL_UpdateInputVariables(void *pUserAgent, void* pInputElementList, int iInputID, void* pwchTheInputString)
{
   UA   *pUser;
   ELEMENTTYPE  *pTheElement=NULL;

   pUser = (UA*)pUserAgent;
   pTheElement = SDL_SearchListElement(pInputElementList, iInputID, NULL);

   if (pTheElement)
   {
      #ifdef WAE_DEBUG
        PrintChar("Update Input variable");
        WMLUA_PrintWS(pwchTheInputString);
      #endif
      pTheElement = SDL_GetListElementContent(pTheElement);
      WML_SetInputKeyValue (pUser, pTheElement, (WCHAR*)pwchTheInputString);
      return TRUE;
   }
   else
      return FALSE;
}

void SDL_SetSelectDefault ( void *pTheElement , void *pUser )
{
   BOOL fDummy;
   fDummy = WML_SetSelectDefault ( (ELEMENTTYPE*)pTheElement , (UA*)pUser );
}

void SDL_UpdateAllSelect (void* pCard, void* pUser)
{
   BOOL fDummy;
   fDummy = WML_UpdateAllSelect ( (ELEMENTTYPE*)pCard , (UA*)pUser );
}

UINT16 WSP2HTTPStatusConvert(UINT8 iWSPNbr)
{
	UINT16 iResult=0;
	if (iWSPNbr < 0x60)
      iResult = (UINT16)(( (( iWSPNbr >> 4) & 0x0f) * 100) + (iWSPNbr & 0x0f));
    else
      iResult = (UINT16)(500 + (iWSPNbr & 0x0f));	  

   return iResult;
}

/*---------------------------------------------------------------------------------------*/

/* changes the state for the MMIUserAgent, also affects the User Input */
void SDL_SetMMIUserAgentLocked(void* pUser, BOOL fMMILock)
{
  ((UA*)pUser)->fLockMMI = fMMILock;
  if (fMMILock)
    SDL_SetUserInputLocked(pUser, TRUE);
}

/* Changes the UserInputState. (only if the value in fact has changed) 
NOTE: when the MMIUserAgent is locked that value overrides the inputvalue */

void SDL_SetUserInputLocked(void* pUser, BOOL fWait)
{
   if ( ((UA*)pUser)->fUserInputMode != fWait)	/* change is needed */
   {
     if ( fWait == TRUE || ((UA*)pUser)->fLockMMI == FALSE )
     {
       ((UA*)pUser)->fUserInputMode = fWait;
       MMIa_wait( (UINT8)((UA*)pUser)->iViewID, fWait);
     }
   }
}


int SDL_MMILocked(void* pUser)
{
  return ((UA*)pUser)->fLockMMI;
}

void* SDL_Normalise(void* pTheUA, void* pURL)
{
   BYTE *pbResult = NULL;
   BYTE *pbTemp = NULL;
   if (pURL)
   {
	  if (pTheUA){
		  pbTemp = b_EscapeBlanks( pURL );
		  b_Resolve (((UA*)pTheUA)->pbURLBase, pbTemp, & pbResult);
		  DEALLOC(&pbTemp);
	  }
      else
      {
         WML_TrimTextBYTE ((BYTE*)pURL, TRIMTEXT_BOTH);
         pbResult = b_CompleteURLHeuristically((BYTE*)pURL);
      }
   }
   return pbResult;
}


int SDL_GetScheme(void* pURL)
{
   BYTE* pbScheme=NULL;
   int iScheme = Scheme_empty;

   if (pURL!=NULL)
   {
     if (b_GetScheme ((BYTE*)pURL, & pbScheme))
	 {
		if (pbScheme)
          iScheme = (int)Scheme_FromString (pbScheme);
     }
     DEALLOC(& pbScheme);
   }
   return iScheme;
}

int SDL_CheckIfPost(void* pTaskElement)
{
   if (pTaskElement!=NULL)
   {
     if (((ELEMENTTYPE*)pTaskElement)->iType==Type_GO)
     {
          /* if it´s a go element that has initiated the request */
         if (  ((GOELEMENT*)(pTaskElement))->iMethod == M_Post )  /* TODOjens pElm */
             return TRUE;
     }
   }
   return FALSE;
}

int SDL_URLOnSameDeck(void *pUserAgent,void* pInURL)
{
   BOOL res =FALSE;
   #ifdef WAE_DEBUG
     PrintChar("Compare :");
     PrintChar("Current URL:");
     PrintChar(((UA*)pUserAgent)->pbURLBase);
     PrintChar("New URL:");
     PrintChar(pInURL);
   #endif
   if (((UA*)pUserAgent)->pbURLBase && pInURL)
   {
     res = b_EqualURL ( ((UA*)pUserAgent)->pbURLBase, (BYTE*)pInURL, ALL_COMP ^ FRAG_COMP); /* compare without frag */
   }
  return res;
}

void SDL_About(void* pTheUA, void* pReqURL)
{
    const WCHAR pInfoString[]={'-','A','U','S',' ','W','A','P',' ','E','n','g','i','n','e','-',0};
    UINT8 iViewID;
    WCHAR *pwchText=NULL; 
    if (pReqURL && pTheUA)
    {
       iViewID = (UINT8)(((UA*)pTheUA)->iViewID);
/*
       if (B_STRSTRING( (BYTE*)pReqURL, (BYTE*)"wapengine"))
*/
       if (strstr( pReqURL, "wapengine"))
	   {
         WCHAR **AboutStrings2Wchar(const CHAR *);
         WCHAR **ppVersionStr = AboutStrings2Wchar((const CHAR *)pReqURL);
         WCHAR **iVersionStr = ppVersionStr;

	     MMIa_newCard(iViewID, NULL, 0, 0, NULL, TRUE, NULL);
     	 MMIa_newParagraph (iViewID,ALIGN_LEFT,TRUE,FALSE);
		 MMIa_newText (iViewID, 0, pInfoString,FALSE,NULL,0, 0);
		 MMIa_newBreak (iViewID);
	     while (*iVersionStr != NULL)
		 {
            pwchText = *iVersionStr;
            MMIa_newText (iViewID, 0, pwchText, FALSE,NULL,0, 0);
			MMIa_newBreak (iViewID );
			DEALLOC( &pwchText);
 	        iVersionStr++;
		 }
         DEALLOC( &ppVersionStr);
		 MMIa_closeParagraph (iViewID);
         MMIa_showCard(iViewID);
	   }
	}
}

void* SDL_URLGetFragment(void* pTheURL)
{
    BYTE    *pbResultString=NULL;
    if (pTheURL)
        b_GetFragment((BYTE*)pTheURL, & pbResultString);
    return (void*)pbResultString;
}


/*------------------------------------------------------------------------------------------------
                                     Future WTAI extensions
-------------------------------------------------------------------------------------------------*/

BOOL SDL_PublicWTAI( int objectId, void* pURL)
{
  BYTE* pWTAIFunction=NULL;
  BYTE* pWTAISubFunction=NULL;
  BYTE* pWTAINumber=NULL;
  BYTE* pTempWTAINumber=NULL;
  WCHAR* pwchName = NULL;
  BYTE* pbTemp=NULL;
  BOOL fRet=FALSE;

  if (pURL && b_GetHost ((BYTE*)pURL, & pWTAIFunction) && b_GetPath ((BYTE*)pURL, & pWTAISubFunction) && b_GetParameters ((BYTE*)pURL, & pWTAINumber) )
  {
	  if ( pWTAIFunction && pWTAISubFunction && pWTAINumber )
	  {
/* GBU 000204
		 if(B_COMPARESTRING((BYTE*)"wp",pWTAIFunction)==0)
*/
          if (strcmp("wp",(const char *)pWTAIFunction)==0)
		 {
/* GBU 000204
		   if(B_COMPARESTRING((BYTE*)"mc",pWTAISubFunction)==0)
*/
		   fRet = TRUE;
           if (strcmp("/mc",(const char *)pWTAISubFunction)==0)
		   {
              pTempWTAINumber = b_UnescapeString (pWTAINumber);
			  WTAIa_publicMakeCall ( (UINT8)objectId, (CHAR*)pTempWTAINumber);
		   }
/* GBU 000204
		   else if (B_COMPARESTRING((BYTE*)"sd",pWTAISubFunction)==0)
*/
           else if (strcmp("/sd",(const char *)pWTAISubFunction)==0)
		   {
			  pTempWTAINumber = b_UnescapeString (pWTAINumber);
			  WTAIa_publicSendDTMF( (UINT8)objectId, (CHAR*)pTempWTAINumber); 
		   }
           else if (strcmp("/ap",(const char *)pWTAISubFunction)==0)   
		   {
			      /* find next ; */
			    pbTemp = pWTAINumber;
				while (*pbTemp && *pbTemp != ';' && *pbTemp != '!')  
				{
					pbTemp++;
				}

				if (*pbTemp && *pbTemp == ';')	/* there must be a ; delimiter */
				{
					*pbTemp = 0;
				    pTempWTAINumber = b_UnescapeString (pWTAINumber);
					pbTemp++;
					pwchName = wip_byte2wchar(pbTemp);
			        WTAIa_publicAddPBEntry ((UINT8)objectId, (CHAR*)pTempWTAINumber, pwchName);
					DEALLOC(& pwchName);
				}
		   }
		 }
	  }
	  DEALLOC(& pWTAIFunction);
	  DEALLOC(& pWTAISubFunction);
	  DEALLOC(& pWTAINumber);  
	  DEALLOC(& pTempWTAINumber); 
  }
  return fRet;
}

/*-----------------------test functions------------------------------------------*/

#ifdef WAE_DEBUG

void PrintChar(char *pStr)
{
  xTraceAlg(pStr);
}

void PrintInt(int iInt)
{
  char string[25];
  itoa(iInt, string, 10);
  PrintChar(string);
}

void WMLUA_PrintWS(WCHAR *pchText)
{
   WCHAR* tmp;
   tmp = (WCHAR*)GenCopyString(2, pchText);
   if (tmp!=NULL)
   {
     SDL_wchar2byte(&tmp);
     PrintChar((CHAR*)tmp);
     DEALLOC(&tmp);
   }
   else
     PrintChar("NULL");
}

#endif

/*-----------------------test functions------------------------------------------*/

#endif
