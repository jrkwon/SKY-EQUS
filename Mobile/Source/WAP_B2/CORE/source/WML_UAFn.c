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

#include "WAEDef.h"

#include "WMLDef.h"
#include "WMLToken.h"
#include "WBXMLDef.h"
#include "WBXMLElm.h"

#include "WMLIF.h"
#include "URL.h"
#include "HeadDef.h"
#include "HdrUtil.h"
#include "WAE_Cmmn.h"
#include "aapimmi.h"
#include "aapiwta.h"
#include "wml_uafn.h"
#include "trnscode.h"
#include "aapiclnt.h"
#include "ui.h"
#include "HCodeCnvt.h"
#include "wip_mem.h"
#include "cmmnrsrc.h"
#include "clnt.h"
#include "waptask.h"

#ifdef BROWSER_UPDATE
	#define REDIRECT
#endif

#define SKTT
#ifndef WAP_USE_SECURE
	extern BOOL Wtlshello ;
#endif//#ifdef WAP_USE_SECURE

#ifdef SKTT
extern BYTE *b_EscapeURLString (const BYTE* pbString);
#endif

typedef struct tPOSTSTRINGPART
{
  void* pData;
  struct tPOSTSTRINGPART *pNextField;
} POSTSTRINGPART, *pPOSTSTRINGPART;

typedef struct
{
   UINT8 iUrlID;
} CONTENTREQUESTSTRUCT,*pCONTENTREQUESTSTRUCT;


BYTE* CreatePostString(UA* pUserAgent, ELEMENTTYPE* pGoElement, INT16* iCharset);


void SDL_StoreContentRequest( void* pContentRequestList,int iUrlID,int iTID)
{
   CONTENTREQUESTSTRUCT* pCRStruct;
/*  Removed by GBU,000314,  not used
   int iDummy;
*/
   pCRStruct = NEWSTRUCT(CONTENTREQUESTSTRUCT);
   if (pCRStruct)
   {
      pCRStruct->iUrlID=(UINT8)iUrlID;
/*  Removed by GBU,000314,  iDummy not used
      iDummy=SDL_AddElementLast(pContentRequestList,iTID, pCRStruct);
*/
      SDL_AddElementLast(pContentRequestList, iTID, pCRStruct);
   }
}

int SDL_GetContentRequestUrlID(void* pCRStruct)
{
  return ((CONTENTREQUESTSTRUCT*)pCRStruct)->iUrlID;
}

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
	 pUser->fUserLoadMode = FALSE;
     pUser->iViewID = (INT8)ViewID;
     pUser->pbURLBase = NULL;
	 pUser->pUAParam = New_UAPARAM();
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
    DeleteHostList(*ppUser);
    Delete_UAPARAM(& (*ppUser)->pUAParam);
   /* DeleteDECK(pUser,pUser->pDeck); */         /*TODO : har jens nåt för detta? */
    WML_DeleteUserAgent (ppUser);
}


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
      if ((pUser->pDecStr)->fAbort)
          return fRes;

      if ((pUser->pDeck)->iType==Type_WML )
      {
        pDeckPart = XML_GetContent(pUser->pDeck,DTD_WML); 
        while (pDeckPart != NULL)
        {
           /* Search for HEAD element */
           if (pDeckPart->iType==Type_HEAD)
           {
              pHeadPart = XML_GetContent(pDeckPart,DTD_WML);
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
				 pHeadPart = XML_GetNextElement(pHeadPart);
			  }
		   }
		   pDeckPart = XML_GetNextElement(pDeckPart);
		}
	  }
   }
   return fRes;
}



/*---------------------------------------------------------------------------------------------------------------------------

						                         Structure and elementhandling

----------------------------------------------------------------------------------------------------------------------------*/

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
   return WML_StartParser(pUser,(BYTE*)pData,(UINT32)iBufLen,iPIVersion,iContentTypeByte);
}

/*---------------------------------------------
parse next card
returns FALSE=more, TRUE=finished ok 
-----------------------------------------------*/
int SDL_ParseNext(void *pUserAgent)
{
  if (WML_ParseNextElement((UA*)pUserAgent))
     return FALSE;
  return TRUE;
}

/*---------------------------------------------
finalise parse returns FALSE=not ok, TRUE=finished ok 
-----------------------------------------------*/
int SDL_EndParse(void *pUserAgent)
{
  UA* pUser=NULL;
  BOOL fRetVal;
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
      return (pTheElement->iType);
    }
    return 0;
}


/*----------------------------------------------
some functions directly mapped from SDL, check
interface.h functions for further info.
------------------------------------------------*/
void* SDL_GetElementContent (void *TheElement)
{
  return (void*)XML_GetContent((ELEMENTTYPE*)TheElement,DTD_WML);
}

void *SDL_GetNextElement (void *pElementObj)
{
     return (void*)XML_GetNextElement ((ELEMENTTYPE *)pElementObj);
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
     if (((CARDELEMENT*)(((ELEMENTTYPE*)pTheCard)->pElm))->fNewContext)
     {
       SDL_ClearContext(pUser);
	   return 1;
     }
   }
   return 0;
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
    INT16 iCharset;
    UINT8 iPIVersion;

    pHeaderList = (HEADERDEF*)pTheHeaderData;

    if (pHeaderList !=NULL)
    {
       pHeaderElement = WSP_GetHeaderWK ( 1, Field_Content_Type, pHeaderList->pHeadList );
       if (pHeaderElement !=NULL)
       {
          pbContentTypeText= WSP_GetContentTypeString (pHeaderElement, &fError, &iCharset, & iPIVersion);
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
			  pCStruct->iContentTypeAsInt = WSP_GetContentTypeByte (pHeaderElement, &fError, &pCStruct->iIANACharID, & pCStruct->iPIVersion);

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

			pCStruct->pContentTypeAsText = (BYTE *)*ppContentType;
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

/*  Modified by GBU,0001014 */
    if (strstr((const char *)pCompareContentType, "*/*"))
      return TRUE;

    /* make downcase work copies */
    pbRecievedContentType = NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pContentTypeText)+1);
    if (pbRecievedContentType)
        DowncaseString (pbRecievedContentType, (BYTE*)pContentTypeText);

    pbAcceptContentType = NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pCompareContentType)+1);
    if (pbAcceptContentType)
        DowncaseString (pbAcceptContentType, (BYTE*)pCompareContentType);

    iRetVal =  CompareContentType(pbAcceptContentType, pbRecievedContentType);

    DEALLOC(&pbAcceptContentType);
    DEALLOC(&pbRecievedContentType);

    return iRetVal;
}


/*-------------------------------------------------------------------------------

  URL handling

---------------------------------------------------------------------------------*/

/*-----------------------------------------------------------
Retrieves the URL to be used, resolves it if necessary and
normalizes it. pURLElement can be different types of elements 
GoElement, ImageElement, or Text.
-------------------------------------------------------------*/
void* SDL_GetElementURL( void* pUser, void* pURLElement)
{
#ifdef REDIRECT
	extern CHAR RedirectUrl[1500];
	extern CHAR prevRedirect[1500];
	extern BOOL setPrev;
	extern BOOL Portdetect;
#endif //REDIRECT
   
   WCHAR* pwchTemp=NULL;
//   WCHAR* pwquery =NULL;

   BYTE* pbResult=NULL;
   BYTE* pbQuery=NULL;
   BYTE* pbTemp=NULL;
   ELEMENTTYPE* pElm;
   BOOL fDummy=FALSE;
   INT16 iCharset=0;

#ifdef SKTT
//for Hangul...jjkyg 1/24
   BYTE* pbTmpQuery = NULL;
   BYTE* pbTempHan  = NULL;
   int   iTemplength =0;
   BYTE* pCheck  = NULL;
   BYTE* pCheck2 = NULL;
#endif//SKTT

//#ifdef DEBUG_WSP
   CLNTa_log(1,0,"GetElementURL Start!!");
//#endif //#ifdef DEBUG_WSP

   if (pURLElement!=NULL)
   {
     pElm = (ELEMENTTYPE*)((ELEMENTTYPE*)pURLElement)->pElm;
     switch (((ELEMENTTYPE*)pURLElement)->iType)
	 {
		 case Type_IMG:
		   pwchTemp = WML_GetTextString((UA*)pUser, ((IMGELEMENT*)(pElm))->pSrc );
		   pbTemp = wip_wchar2byte( pwchTemp, & fDummy );
		   break;
		 case Type_GO:
		   pwchTemp = WML_GetTextString((UA*)pUser, ((GOELEMENT*)(pElm))->pHref );

#ifdef SKTT
		   iTemplength = KSCStrLenOfUni((WCHAR*)pwchTemp);   
		   if (iTemplength)
		   {
		        pbTemp = NEWARRAY(BYTE,iTemplength+1);
		   }
           
		   if (pbTemp && Uni2KSCString((WCHAR*)pwchTemp, (unsigned char *)pbTemp))
			{
			   pbTemp[iTemplength] = '\0'; /* ensure 0 termination */
			   DEALLOC(&pbTempHan);
			   pbTempHan =pbTemp;
			   pbTemp =NULL;
			}
            pbTemp = b_EscapeURLString((BYTE*)pbTempHan);	
			//CLNTa_log(1,0,"PBTEMPHAN>>>[%s]",pbTemp);

		    iTemplength = 0;
#else
		   pbTemp = wip_wchar2byte( pwchTemp, & fDummy );
#endif//#ifdef SKTT

		   if (((GOELEMENT*)(pElm))->iMethod==M_Get)
           { 
#ifdef SKTT
			   pbTmpQuery = CreatePostString((UA*)pUser, (ELEMENTTYPE*)pURLElement , & iCharset);
			   pbQuery = b_EscapeURLString(pbTmpQuery);
#else
			   pbQuery = CreatePostString((UA*)pUser, (ELEMENTTYPE*)pURLElement , & iCharset);
#endif//#ifdef SKTT

			   if (pbQuery)
			   {
				  pbResult = b_AppendToQuery (pbTemp, pbQuery);
				  DEALLOC(&pbTemp);
				  pbTemp = pbResult;
				  pbResult = NULL;
			   }
			   DEALLOC(&pbQuery);
#ifdef SKTT
			   DEALLOC(&pbTmpQuery);
#endif
		   }
		   break;
		 case Type_A:
		   pwchTemp = WML_GetTextString((UA*)pUser, ((AELEMENT*)(pElm))->pHref );
		   pbTemp = wip_wchar2byte( pwchTemp, & fDummy );
		   break;
		 case Type_Text:
		   pwchTemp = WML_GetString((UA*)pUser, (ELEMENTTYPE*)pURLElement);  /*since this is a elementtype */
		   pbTemp = wip_wchar2byte( pwchTemp, & fDummy );
#ifdef SKTT //URL¿¡ '"'¹®ÀÚ°¡ ÀÖÀ» ¶§ Àß¸øµÈURLÀÔ´Ï´Ù ¸Þ½ÃÁö³ª¿À´Â ¹®Á¦¼öÁ¤ºÎºÐ  
		   pCheck = NULL;
		   if(
			   (pCheck = memchr((BYTE*)pbTemp,0x22,strlen((char*)pbTemp))) != NULL 
			 )
		   {
			   if( (pCheck - pbTemp) == 0 )
			   {
				   if(
					   (pCheck2 = memchr((BYTE*)pbTemp+1,0x22,strlen((char*)pbTemp)-1)) != NULL
					 )
				   {
					   int iLen = 0;
					   iLen = pCheck2 - pCheck -1;
					   pbTemp = NULL;pbTemp = NEWARRAY(BYTE,iLen+1); 
					   memset(pbTemp,0x00,iLen +1);
					   memcpy((BYTE*)pbTemp,pCheck+1,iLen);
					   pbTemp[iLen] = 0x00;
					   DEALLOC(&pCheck);
				   }
			   }
		   }			
#endif//#ifdef SKT
		   break;
     }
   }
   DEALLOC(&pwchTemp);

   //CLNTa_log(1,0,"Base URL [%s]",((UA*)pUser)->pbURLBase);
   //CLNTa_log(1,0,"REL  URL [%s]",pbTemp);
   //CLNTa_log(1,0,"ABS  URL [%s]",pbResult);

   b_Resolve (((UA*)pUser)->pbURLBase, pbTemp, & pbResult);
   
   DEALLOC(&pbTemp);
#ifdef SKTT
   DEALLOC(&pbTempHan);
#endif

   pbTemp = b_CompleteURLHeuristically(pbResult);  /* to append trailing / */
   DEALLOC(&pbResult);
   pbResult = pbTemp;
   #ifdef WAE_DEBUG
     PrintChar("Get URL (from element)");
     PrintChar("Base:");
     PrintChar(((UA*)pUser)->pbURLBase);
     PrintChar("Go:");
     PrintChar(pbResult); 
   #endif

   CLNTa_log(1,0,"GetElementURL Return[%s]",pbResult);
   
#ifdef REDIRECT
	memset(prevRedirect,0x00,1500);
	if(
		Portdetect && (strstr((const char *)pbResult,(const char *)":9091")!=NULL)
	   )//º¸¾È session½ÃÀÛ 
	{
		strncpy(prevRedirect,RedirectUrl,1500);setPrev = TRUE;
	}	

	memset(RedirectUrl,0x00,1500);
	if(strlen((const char *)pbResult) < 1500 ) strncpy(RedirectUrl,(const char *)pbResult,strlen((const char *)pbResult));
#endif//#ifdef REDIRECT
   return pbResult;
}


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
           pFrag = (BYTE*)GenCopyString(2,((CARDELEMENT*)(pFirstCard->pElm))->pchId);  /*cast to avoid warnings, this is actually WCHAR */
           SDL_wchar2byte((void**)& pFrag);   /* transform to byte */
		}
  	    if (pFrag)
		{
           iBaseLength = B_STRINGLENGTH((const char *)pNewBase);
           pUser->pbURLBase = NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pFrag)+iBaseLength+2);
		   if (pUser->pbURLBase)
           {
             B_COPYSTRING((char *)pUser->pbURLBase,(const char *)pNewBase);
             B_COPYSTRING((char *)&(pUser->pbURLBase[iBaseLength]),"#");
             B_COPYSTRING((char *)&(pUser->pbURLBase[iBaseLength+1]),(const char *)pFrag);
             DEALLOC(&pNewBase);
           }
		}
		else
		{
			pUser->pbURLBase = pNewBase;
		}

	 }
	 else  /* there is a frag */
	 {
	    pUser->pbURLBase = pNewBase;
	 }

     DEALLOC(&pFrag);
  }

  #ifdef WAE_DEBUG
    PrintChar("Set new base URL:");
    PrintChar(pUser->pbURLBase);
  #endif
}

/*-------------------------------------------------------------
Constructs a postfield string to be used by get or post 
--------------------------------------------------------------*/

BYTE* CreatePostString(UA* pUserAgent, ELEMENTTYPE* pGoElement, INT16* iCharset)
{
    BYTE* pNameData=NULL;
    BYTE* pValueData=NULL;
    BYTE* pResStr1=NULL;
    BYTE* pResStr2=NULL;
    BYTE* pResult=NULL;
    UINT32 iNameLen = 0;
    UINT32 iValueLen = 0;
    UINT32 iTotalLength =0;
    ELEMENTTYPE* pPostField = NULL;
    POSTSTRINGPART* pListStart=NULL;
    POSTSTRINGPART* pNewStringPart=NULL;
    POSTSTRINGPART* pPrevStringPart=NULL;
    BYTE* pbAccChar=NULL;
    INT16 iCharEnc=IANA_CHARSET_INVALID;
    BOOL fDummy=FALSE;
	BOOL fContainsAcceptCharset=FALSE;
    pPostField = XML_GetContent( pGoElement, DTD_WML);

    if (pPostField)
    {
	   /* determine what char encoding to use */
#ifdef DEBUG_POST
	   //CLNTa_log(1,0,"HAve postfield???");
#endif //#ifdef DEBUG_POST
	   if ( pGoElement && ( (GOELEMENT*)(pGoElement->pElm) )->pchAccChar )
	     fContainsAcceptCharset=TRUE;

#ifndef SKTT
	    iCharEnc = pUserAgent->iTextEncodFormat;
#else
	    iCharEnc = IANA_CHARSET_KSC5601;
#endif

	  /* if wml content contains ACCEPT-CHARSET */
	   if ( fContainsAcceptCharset )
       {
         pbAccChar = NEWARRAY(BYTE, STRINGLENGTH( ( (GOELEMENT*)(pGoElement->pElm) )->pchAccChar ) +1 );
		 if (pbAccChar)
		 {
            pResult = wip_wchar2byte( ( (GOELEMENT*)(pGoElement->pElm) )->pchAccChar, & fDummy);
			DowncaseString (pbAccChar, pResult);
			iCharEnc = IANA_CHARSET_LATIN1;

/*  Modified by GBU,0001014 */
/*
            if (B_STRSTRING(pbAccChar, (BYTE*)"ucs-2"))
*/
            if (strstr((const char *)pbAccChar, "ucs-2"))
				iCharEnc= IANA_CHARSET_UCS2;

/*  Modified by GBU,0001014 */
/*
			if (B_STRSTRING(pbAccChar, (BYTE*)"us-ascii"))
*/
            if (strstr((const char *)pbAccChar, "us-ascii"))
				iCharEnc= IANA_CHARSET_USASCII;

/*  Modified by GBU,0001014 */
/*
			if (B_STRSTRING(pbAccChar, (BYTE*)"utf-8"))
*/
            if (strstr((const char *)pbAccChar, "utf-8"))
				iCharEnc= IANA_CHARSET_UTF8;

#ifdef SKTT
			if (strstr((const char *)pbAccChar, "ks_c_5601-1987"))
				iCharEnc= IANA_CHARSET_KSC5601;			
#endif

		 }
		 DEALLOC(&pResult);
		 DEALLOC(&pbAccChar);
	   }
    }
    
	pResult=NULL;

	while (pPostField)
	{
	  if (pPostField->iType == Type_POSTFIELD)
      {

        pNewStringPart=NEWSTRUCT(POSTSTRINGPART);
	    if (pNewStringPart)
        {

           if (!pListStart)
             pListStart=pNewStringPart;

           pNewStringPart->pData=NULL;

		   /* evaluate name and variable */
		   iNameLen =0;
		   iValueLen =0;
		   pResult = NULL;

		   pNameData = (BYTE*)WML_GetTextString(pUserAgent, ((POSTFIELDELEMENT*)(pPostField->pElm))->pName); 	
		   pValueData = (BYTE*)WML_GetTextString(pUserAgent, ((POSTFIELDELEMENT*)(pPostField->pElm))->pValue);

		   switch (iCharEnc)
           {
#ifdef SKTT
			case IANA_CHARSET_KSC5601:  /* transcode to KSC5601 */
			{
			  if (pNameData)
			  {	  
				/* get the encoded length */   
				iNameLen = KSCStrLenOfUni((WCHAR*)pNameData);
				if (iNameLen)
				  pResStr1 = NEWARRAY(BYTE,iNameLen+1);

				/* encode the unicode string */
				if (pResStr1 && Uni2KSCString((WCHAR*)pNameData, (unsigned char *)pResStr1) )
				{
				   pResStr1[iNameLen] = 0; /* ensure 0 termination */
				   DEALLOC(&pNameData);	   /* remove the old string */
				   pNameData=pResStr1;
 	   			   pResStr1=NULL;
				}
				DEALLOC(&pResStr1);
				pResStr2=NULL; 
 			  	iNameLen=0;
			}

			if (pValueData)
			{
				/* get the encoded length */
				iValueLen = KSCStrLenOfUni((WCHAR*)pValueData);
				if (iValueLen)
				  pResStr1 = NEWARRAY(BYTE,iValueLen+1);

				/* encode the unicode string */
				if (pResStr1 && Uni2KSCString((WCHAR*)pValueData, (unsigned char *)pResStr1) )
				{ 
				   pResStr1[iValueLen] = 0;    /* ensure 0 termination */
				   DEALLOC(&pValueData);	   /* remove the old string */
				   pValueData=pResStr1;
 	   			   pResStr1=NULL;
				}
				DEALLOC(&pResStr1);
				pResStr2=NULL;
 				iValueLen=0;
			 }
			 break;
		}
#endif//sktt
		   case IANA_CHARSET_UTF8:  /* transcode to UTF-8 */
			   {
				  if (pNameData)
				  {	  
					/* get the encoded length */   
					iNameLen = Unicode2UTF8_calcLen((WCHAR*) pNameData, STRINGLENGTH((WCHAR*)pNameData) );
					if (iNameLen)
					  pResStr1 = NEWARRAY(BYTE,iNameLen+1);

					/* encode the unicode string */
					if (pResStr1 && Unicode2UTF8_convert( (WCHAR*)pNameData, STRINGLENGTH((WCHAR*)pNameData), pResStr1, iNameLen, & pResStr2 ) )
					{
					   pResStr1[iNameLen] = 0; /* ensure 0 termination */
					   DEALLOC(&pNameData);	   /* remove the old string */
					   pNameData=pResStr1;
 	   				   pResStr1=NULL;
					}
					DEALLOC(&pResStr1);
					pResStr2=NULL; 
 					iNameLen=0;
				  }

				  if (pValueData)
				  {
					/* get the encoded length */
					iValueLen = Unicode2UTF8_calcLen((WCHAR*) pValueData, STRINGLENGTH((WCHAR*)pValueData) );

					if (iValueLen)
					  pResStr1 = NEWARRAY(BYTE,iValueLen+1);

					/* encode the unicode string */
					if (pResStr1 && Unicode2UTF8_convert((WCHAR*) pValueData, STRINGLENGTH((WCHAR*)pValueData), pResStr1, iValueLen, & pResStr2 ))
					{
					   pResStr1[iValueLen] = 0;    /* ensure 0 termination */
					   DEALLOC(&pValueData);	   /* remove the old string */
					   pValueData=pResStr1;
 	   				   pResStr1=NULL;
					}
					DEALLOC(&pResStr1);
					pResStr2=NULL;
 					iValueLen=0;
				  }

			   break;
			   }
		   case IANA_CHARSET_USASCII :
		   case IANA_CHARSET_LATIN1 : 
			   {
				  SDL_wchar2byte((void**)&pNameData);
				  SDL_wchar2byte((void**)&pValueData);
				  break; 
			   }
           case IANA_CHARSET_UCS2 : 
			   break;
		   default:  /* best effort ISO-LATIN... */
			   {
				  SDL_wchar2byte((void**)&pNameData);
				  SDL_wchar2byte((void**)&pValueData);
			      iCharEnc = IANA_CHARSET_LATIN1;
				  break; 
			   }
		   }

/*========================just test by jjkyg 2000/02/26=====================================*/
/*  º¸¾È ¹®¼­¸¦ °ËÅäÇÑ °á°ú realuri =./test.cgiÀÓ  ÀÌ·²°æ¿ì "/"µµ escapeÃ³¸®°¡ µÇ¹Ç·Î ¼­¹ö°¡ 
    ÀÎ½ÄÇÏÁö ¸øÇÏ´Â °æ¿ì°¡ »ý±æ °ÍÀ¸·Î º½.. 
	<½ÃÇè¹æ¹ý>
	1. ÀÏ´Ü ¿µ¹®À¸·Î Ã³¸®ÇÏµµ·Ï ÇÑ´Ù.
	2. "/"¸¦ escapeÃ³¸®ÇÏÁö ¾Êµµ·Ï ÇÑ´Ù.
	pResStr1 = b_EscapeString(pNameData);
	pResStr2 = b_EscapeString(pValueData);
*/	
/* escape result */
#ifdef FOR_MS
#ifndef WAP_USE_SECURE
	if(Wtlshello)
	{
		pResStr1 = b_EscapeURLString(pNameData);
		pResStr2 = b_EscapeURLString(pValueData);
	}
	else
#endif//#ifndef WAP_USE_SECURE
	{
		pResStr1 = b_EscapeString(pNameData);
		pResStr2 = b_EscapeString(pValueData);
	}
#else
	pResStr1 = b_EscapeString(pNameData);
	pResStr2 = b_EscapeString(pValueData);
#endif//#ifdef FOR_MS
/* ========================just test by jjkyg 2000/02/26===================================*/

		   if (iCharEnc==IANA_CHARSET_UCS2)
		   {
			 DEALLOC(&pResStr1);
			 DEALLOC(&pResStr2);
		     pResStr1 = b_UnicodeEscape((WCHAR*)pNameData);
		     pResStr2 = b_UnicodeEscape((WCHAR*)pValueData);
		   }

           /* to support stupid webservers with no ;charset implementation use cfg_wae_ua_methodPostCharsetOverride*/
	       if ( cfg_wae_ua_methodPostCharsetOverride && !fContainsAcceptCharset )
               iCharEnc=IANA_CHARSET_INVALID;

		   if (pResStr1)
			  iNameLen = B_STRINGLENGTH((const char *)pResStr1);
		   if (pResStr2)
			  iValueLen = B_STRINGLENGTH((const char *)pResStr2);

		   DEALLOC(&pNameData);
		   DEALLOC(&pValueData);

		   /* form the string ignore if variable name is empty*/
		   if (pResStr1)
           {
		      pResult = NEWARRAY(BYTE,iNameLen+iValueLen+2);
			  if (pResult)
              {
				  B_COPYSTRING((char *)pResult,(const char *)pResStr1);
				  B_COPYSTRING((char *)&pResult[iNameLen],"=");
				  if (pResStr2)
				     B_COPYSTRING((char *)&pResult[iNameLen+1],(const char *)pResStr2);
			  }
           }

   		   DEALLOC(&pResStr1);
		   DEALLOC(&pResStr2);

		   /* put result in list */ 
           pNewStringPart->pData=pResult;
		   if (pPrevStringPart)
              pPrevStringPart->pNextField=pNewStringPart;
		   pNewStringPart->pNextField=NULL;
		   pPrevStringPart = pNewStringPart;
		   
		   iTotalLength = iTotalLength+iNameLen+iValueLen+2; /*eg testname=testvalue */
		}
	  }
      pPostField = XML_GetNextElement( pPostField);
	}

	/* assemble the list */
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
  	        B_COPYSTRING((char *)&pResult[iTotalLength],(const char *)pNewStringPart->pData);
		    iTotalLength = iTotalLength + B_STRINGLENGTH((const char *)pNewStringPart->pData);
		    DEALLOC(&pNewStringPart->pData);
		  }
		  pPrevStringPart=pNewStringPart;
		  pNewStringPart=pNewStringPart->pNextField;
		  DEALLOC(&pPrevStringPart);
		  if (pNewStringPart)
          {
		     B_COPYSTRING((char *)&pResult[iTotalLength],"&");
		     iTotalLength++;
		  }
		}
	}

    *iCharset =  iCharEnc;

	return pResult;
}

/*-----------------------------------------------------------
Constructs a new requeststruct to be sent to wsp.
if pActiveElement is set i.e. a Go Element or image element is to be handled 
Used by user agents and the MMI
-------------------------------------------------------------*/
extern BOOL optionselect;
#ifdef REDIRECT
	extern redirect_config redirect;
	extern BOOL   isPost;
	extern UINT32 s_postlen;
	extern BYTE   s_postdata[1500];
#endif
void*  SDL_CreateRequestStruct( void* pWAEInObject, void* pInUserAgent, void* pActiveElement, void* pbURL, void* pAccept)
{
   GOELEMENT* pGoElement=NULL;
   BYTE* pPostData=NULL;
   BYTE* pbTempCredentials;
   URLREQSTRUCT* pURLStruct=NULL;   
/*  Removed by GBU,000314,  not used
   WAEMAINOBJECT* pWAEObject;
*/
   UINT8 iCredLength =0; 
   int iMethod;
   UA* pUser=NULL;

//jjkyg
#ifndef WAPCORE_FOR_WARNING
   ui_cmd_type   *ui_cmd_ptr;                        /* to hold the UI cmd. */
#endif //WAPCORE_FOR_WARNING
/*  Removed by GBU,000314,  not used
   pWAEObject = (WAEMAINOBJECT*)pWAEInObject;
*/

   /*   Added by GBU,000315,    to remove warnings for unused variable */
   pWAEInObject = pWAEInObject;

   pUser = (UA*)pInUserAgent;

   iMethod=(int)M_Get;   /*default method */


   if(optionselect)
   {
	  optionselect = FALSE;
#ifdef BLOCK // 01.02.02 ytchoi NO Use command
	  if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
	  {	
			//ERR_FATAL( "No UI cmd buffers",0,0,0);
		    CLNTa_log(1,0,"No UI cmd buffers");
	  }
	  else		
	  {
		    ui_cmd_ptr->hdr.cmd        = UI_OPTION_SELECT_F;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
	  }
#endif //BLOCK
   }

   pURLStruct = New_URLREQSTRUCT();

   if (pURLStruct)
   {
      if (pActiveElement)
      {
        switch ( ((ELEMENTTYPE*)pActiveElement)->iType )
		{	
		case Type_IMG:    		   /* if it´s an imageelement that is to be loaded */
		   pURLStruct->pbURL = (BYTE*)GenCopyString(1, pbURL);
		   /* the image content-types that are accepted by this request */
		   pURLStruct->pbAccept = (BYTE*)GenCopyString(1, ACCEPT_IMAGE);
		   break;

		case Type_GO:  /* if it´s a go element that has initiated the request */

			  pGoElement = (GOELEMENT*)((ELEMENTTYPE*)pActiveElement)->pElm;
			  if (pGoElement->fSendRef)
			  {
				 pUser = (UA*)pUser;
				 /* referring URL */
				 pURLStruct->pbReferer = b_MinRelativePath ((BYTE*)pbURL, pUser->pbURLBase, TRUE);
			  }

			  pURLStruct->pbAccept = (BYTE*)GenCopyString(1, ACCEPT_DEFAULT);

			  pURLStruct->pbURL = (BYTE*)GenCopyString(1, pbURL);

			  iMethod = (int)(pGoElement->iMethod);
			
			  if (iMethod==M_Post)		/* if method is get the body is already processed */
			  {	
			      pPostData = CreatePostString(pUser,(ELEMENTTYPE*)pActiveElement, & pURLStruct->iContentCharSet);
				  if (pPostData)
				  {
					 pURLStruct->pbBody = pPostData;
				     pURLStruct->iBodyLength  = B_STRINGLENGTH((const char *)pPostData);

				#ifdef REDIRECT
					 isPost = TRUE;
					 if(pURLStruct->iBodyLength >0)	s_postlen = pURLStruct->iBodyLength;
					 if(s_postlen > 0)
					 {					
						memset(s_postdata,0x00,1500);
						if(s_postlen < 1500)
							memcpy(s_postdata,pURLStruct->pbBody,s_postlen);
					 }
				#endif//#ifdef REDIRECT
				  }				  
              }
			  break;

		case Type_Text:
		case Type_A:
		case Type_PREV:
			  pURLStruct->pbURL = (BYTE*)GenCopyString(1, pbURL);
			  pURLStruct->pbAccept= (BYTE*)GenCopyString(1, ACCEPT_DEFAULT);
			  break;
		}
	  }
      else        /* ordinary LOAD URL */
      {
		 /* the URL that represents this request */

		 pURLStruct->pbURL = (BYTE*)GenCopyString(1, pbURL);

		 /* the content-types that are accepted by this request */
		 pURLStruct->pbAccept= (BYTE*)GenCopyString(1,pAccept);


#ifndef WAP_USE_SECURE
		 if(Wtlshello) iMethod = M_Post;	
#endif//#ifndef WAP_USE_SECURE

#ifdef REDIRECT
		/*==================================================================
			Ç×»ó redirect.bcorrect = TRUEÀÏ °æ¿ì¿¡¸¸ POST method°¡´É 
		 ===================================================================*/
	#ifndef WAP_USE_SECURE
		 else if(isPost && redirect.bCorrect) iMethod = M_Post;
	#else
		if(isPost && redirect.bCorrect) iMethod = M_Post;
	#endif//#ifndef WAP_USE_SECURE
#endif//#ifdef REDIRECT
      }
	  /*======================================================================
							JUST check Request URI whether
							    Security or not
	   ======================================================================*/
	    //9091port°¡ ¾øÀ» °æ¿ì 
#ifdef FOR_MS		
	   if(
			(strstr((const char *)pURLStruct->pbURL,"9091"))==NULL 
		  )
		{
			extern void close_wtls_not_air(void);
			
			close_wtls_not_air();//key¸¦ Áö¿î´Ù.
			//CLNTa_log(1,0,"clear ssls");
		}
#endif//#ifdef FOR_MS		

	  pURLStruct->iMethod = (UINT8)iMethod;

      #ifdef WAE_DEBUG
 	  PrintChar("New Request");
	  PrintChar((CHAR*)pURLStruct->pbURL);
	  PrintChar((CHAR*)pURLStruct->pbAccept);
	  #endif

		/* copy proxyparameters */
	 /* pURLStruct->iProxyCredentialsLength = CopyCredentialParameters(pWAEObject->pbProxyUserName, pWAEObject->pbProxyPassWord, & (pURLStruct->pbProxyCredentials) );
	*/	/* check basic auth */
	  pURLStruct->pbCredentials=0;
      iCredLength=0;

	  if (pUser)
	     CheckAuth((UA*)pUser, pURLStruct->pbURL, NULL, & pbTempCredentials, & iCredLength);   

	  if (iCredLength>0)
	  {
	    pURLStruct->pbCredentials = NEWARRAY(BYTE,iCredLength);
	    if (pURLStruct->pbCredentials!=NULL)
		{
		  B_COPYSTRINGN(pURLStruct->pbCredentials,pbTempCredentials,iCredLength);
		  pURLStruct->iCredentialsLength = iCredLength;
		}
	  }
	  else
	   pURLStruct->pbCredentials=NULL;
   }

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

/*-----------------------------------------------------------------------------------------
                                  History handling
-------------------------------------------------------------------------------------------*/

void* SDL_GetPreviousCardURL(void* pUser )
{
  void* pReturn=NULL;
  if (((UA*)pUser)->iElmInHistory > 1)
  {
     pReturn = WML_HistoryRead((UA*)pUser,2);
  }

  return pReturn;
}

void SDL_PopHistory( void* pUser )
{
   BYTE* pwchTemp;
   pwchTemp = WML_HistoryListPop ((UA*)pUser);
   DEALLOC(&pwchTemp);
}

void SDL_PutInHistory(void* pUser, void* pInUrl, void* pCurrentCard)
{
  WCHAR* pwchTitle;
  if ((pUser!=NULL) && (pCurrentCard!=NULL))
  {
    if ( pInUrl !=NULL)
    {
       pwchTitle = WML_GetCardTitle ( (UA*)pUser, (ELEMENTTYPE*)pCurrentCard);

       WML_HistoryListPush ( (UA*)pUser, (BYTE*)pInUrl, pwchTitle);
       
       #ifdef WAE_DEBUG
         PrintChar("Puthistory:");
         WMLUA_PrintWS(pwchTitle);
         PrintChar(pInUrl);
       #endif

         DEALLOC(&pwchTitle);
    }
  }
}


int SDL_UpdateInputVariables(void *pUserAgent, void* pInputElementList, int iInputID, void* pwchTheInputString)
{
   UA   *pUser;
   ELEMENTTYPE  *pTheElement=NULL;

   pUser = (UA*)pUserAgent;
   pTheElement = (ELEMENTTYPE  *)SDL_SearchListElement(pInputElementList, iInputID, NULL);

   if (pTheElement)
   {
      #ifdef WAE_DEBUG
        PrintChar("Update Input variable");
        WMLUA_PrintWS(pwchTheInputString);
      #endif
      pTheElement = (ELEMENTTYPE  *)SDL_GetListElementContent(pTheElement);
      WML_SetInputKeyValue (pUser, pTheElement, (WCHAR*)pwchTheInputString);
      return TRUE;
   }
   else
      return FALSE;
}

void SDL_SetSelectDefault ( void *pTheElement , void *pUser )
{
/*  Modified by GBU,000314,  fDummy not used
   BOOL fDummy;
   fDummy = WML_SetSelectDefault ( (ELEMENTTYPE*)pTheElement , (UA*)pUser );
*/
    WML_SetSelectDefault ( (ELEMENTTYPE*)pTheElement , (UA*)pUser );
}

void SDL_UpdateAllSelect (void* pCard, void* pUser)
{
/*  Modified by GBU,000314,  fDummy not used
   BOOL fDummy;
   fDummy = WML_UpdateAllSelect ( (ELEMENTTYPE*)pCard , (UA*)pUser );
*/
    WML_UpdateAllSelect ( (ELEMENTTYPE*)pCard , (UA*)pUser );
}


/*-------------------------------------------------------------------------------------------
                              Authenticate handling
---------------------------------------------------------------------------------------------*/

/*------------------------------------------
returns any wspheaders with realm 
--------------------------------------------*/

void* SDL_GetAuthValues(void *pHeader, int iType)
{
    HEADERELEMENT* pHeaderElement=NULL;
    BYTE* pRealm=NULL;
    BYTE* pResult=NULL;

    if (iType==AUTH_PROXY)
    {
       #ifdef WAE_DEBUG
        PrintChar("proxy:");
       #endif 
       pHeaderElement = WSP_GetHeaderWK ( 1, Field_Proxy_Authenticate, ((HEADERDEF*)pHeader)->pHeadList );
       pRealm= WSP_GetProxyAuthenticateRealm(pHeaderElement);
    }
    else if (iType==AUTH_SERVER)
    {
       #ifdef WAE_DEBUG
        PrintChar("server:");
       #endif
       pHeaderElement = WSP_GetHeaderWK ( 1, Field_WWW_Authenticate, ((HEADERDEF*)pHeader)->pHeadList );
       pRealm= WSP_GetWWWAuthenticateRealm (pHeaderElement);
    }
    pResult = (BYTE*)GenCopyString(1, pRealm);

      #ifdef WAE_DEBUG
        if (pHeader!=NULL)
        {
            PrintChar("Realm:");
            PrintChar(pResult);
        }
      #endif
    return pResult;
}


void SDL_SetAuthParam(void* pInUser, void* pbURL, void* pbRealm, void *pbUserName, void *pbPassword)
{
  BYTE * pbCredentials =NULL;
  int iCredLength=0;
  iCredLength = CopyCredentialParameters(pbUserName, pbPassword, (void**)& pbCredentials);
  #ifdef WAE_DEBUG
     PrintChar("Set parameters:");
     PrintChar(pbRealm);
     PrintChar(pbUserName);
     PrintChar(pbPassword);
  #endif
  if (iCredLength)
    StoreAuthParams ((UA*)pInUser, (BYTE*)pbURL, (BYTE*)pbRealm, pbCredentials, (UINT8)iCredLength);
  DEALLOC( & pbCredentials);
}


void SDL_SendAuthRequest( void* pUser, int iDialogId ,void* pRealm, int iType)
{
   MMIa_passwordDialog ( ((UA*)pUser)->iViewID, (UINT8)iDialogId, (CHAR*)pRealm, (INT8)iType);
}


int SDL_CheckAuth( void* pUser , void *pUrl, void* pbRealm)
{
  BYTE* pbDummyCredentials=NULL;
  UINT8 iCredLength=0;
  BOOL fResult; 
  fResult = CheckAuth ((UA*)pUser, (BYTE*)pUrl, (BYTE*)pbRealm,& pbDummyCredentials, & iCredLength);
  return fResult;
}

/* Converts a WSP status field to the corresponding HTTP field value */
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



void SDL_ToggleLoadIcon(void* pUser, BOOL fLoad, BYTE* pRequestURL)
{
   if ( ((UA*)pUser)->fUserLoadMode != fLoad)	/* change is needed */
   {
      ((UA*)pUser)->fUserLoadMode = fLoad;
	  if (fLoad)
	    SDL_OutputStatus(pUser, LoadingData, pRequestURL );
	  else
	    SDL_OutputStatus(pUser, LoadingDataDone, pRequestURL );
   }

}

int SDL_MMILocked(void* pUser)
{
  return ((UA*)pUser)->fLockMMI;
}

void* SDL_Normalise(void* pTheUA, void* pURL)
{
   BYTE *pbResult = NULL;
   if (pURL)
   {
      if (pTheUA)
          b_Resolve (((UA*)pTheUA)->pbURLBase, (BYTE*)pURL, & pbResult);
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
         if ( ((GOELEMENT*)( ((ELEMENTTYPE*)pTaskElement)->pElm ))->iMethod == M_Post )
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
/*  Modified by GBU,0001014 */
/*
       if (B_STRSTRING( (BYTE*)pReqURL, (BYTE*)"wapengine"))
*/
       if (strstr((const char *)pReqURL, "wapengine"))
	   {
/*  Modified by GBU,000103 added cast*/
/*
         WCHAR **AboutStrings2Wchar(CHAR *);
         WCHAR **ppVersionStr = AboutStrings2Wchar(pReqURL);
*/
         WCHAR **AboutStrings2Wchar(const CHAR *);
         WCHAR **ppVersionStr = AboutStrings2Wchar((const CHAR *)pReqURL);
         WCHAR **iVersionStr = ppVersionStr;

	     MMIa_newCard(iViewID, NULL, 0, 0, NULL, TRUE, NULL);
     	 MMIa_newParagraph (iViewID,ALIGN_LEFT,TRUE);
		 MMIa_newText (iViewID, 0, pInfoString,FALSE,NULL, 0);
		 MMIa_newBreak (iViewID);
	     while (*iVersionStr != NULL)
		 {
            pwchText = *iVersionStr;
            MMIa_newText (iViewID, 0, pwchText, FALSE,NULL, 0);
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

void SDL_PublicWTAI(void* pURL)
{
  BYTE* pWTAIFunction=NULL;
  BYTE* pWTAISubFunction=NULL;
  BYTE* pWTAINumber=NULL;
  BYTE* pTempWTAINumber=NULL;

  if (pURL && b_GetHost ((BYTE*)pURL, & pWTAIFunction) && b_GetPath ((BYTE*)pURL, & pWTAISubFunction) && b_GetParameters ((BYTE*)pURL, & pWTAINumber) )
  {
	  if ( pWTAIFunction && pWTAISubFunction && pWTAINumber )
	  {
/*  Modified by GBU,000114 */
/*
		 if(B_COMPARESTRING((BYTE*)"wp",pWTAIFunction)==0)
*/
         if (strcmp("wp",(const char *)pWTAIFunction)==0)
		 {
/*  Modified by GBU,000114 */
/*
		   if(B_COMPARESTRING((BYTE*)"mc",pWTAISubFunction)==0)
*/
           if (strcmp("/mc",(const char *)pWTAISubFunction)==0)
		   {
              pTempWTAINumber = b_UnescapeString (pWTAINumber);
			  WTAIa_publicMakeCall ((CHAR*)pTempWTAINumber);
		   }
/*  Modified by GBU,000114 */
/*
		   else if (B_COMPARESTRING((BYTE*)"sd",pWTAISubFunction)==0)
*/
           else if (strcmp("/sd",(const char *)pWTAISubFunction)==0)
		   {
			  pTempWTAINumber = b_UnescapeString (pWTAINumber);
			  WTAIa_publicSendDTMF( (CHAR*)pTempWTAINumber); 
		   }

/*  Modified by GBU,000114 */
/*
		   else if (B_COMPARESTRING((BYTE*)"ap",pWTAISubFunction)==0)*/  /*TODO: what should this be specs are inconsistant */
           else if (strcmp("/ap",(const char *)pWTAISubFunction)==0)  /*TODO: what should this be specs are inconsistant */
		   {
		    /* pTempWTAINumber = b_UnescapeString (pWTAINumber);
			  WTAIa_publicSendDTMF( (CHAR*)pTempWTAINumber); TODO adapt for PhoneBook*/
		   }
		 }
	  }
	  DEALLOC(& pWTAIFunction);
	  DEALLOC(& pWTAISubFunction);
	  DEALLOC(& pWTAINumber);  
	  DEALLOC(& pTempWTAINumber); 
  }
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
