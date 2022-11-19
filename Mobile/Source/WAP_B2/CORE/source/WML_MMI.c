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

#ifndef WML_MMI_C
#define WML_MMI_C

#include "WMLIF.h"
#include "wae_cmmn.h"
#include "WMLToken.h"
#include "aapimmi.h"
#include "aapiclnt.h"
#include "capiclnt.h"


void SDL_ShowCard(void *pUser)
{
 #ifdef WAE_DEBUG
    PrintChar("---SHOWCARD--\n");
 #endif
 if (pUser)
   MMIa_showCard(((UA*)pUser)->iViewID);
}

void SDL_CancelCard(void *pUser)
{
 #ifdef WAE_DEBUG
    PrintChar("---CANCELCARD--\n");
 #endif
 if (pUser)
   MMIa_cancelCard(((UA*)pUser)->iViewID);
}


/*-------------------------------------------------------------------
CreateCard is called by the SDL system when a new
card should be created.
----------------------------------------------------------------------*/
void SDL_CreateCard(void* pUserAgent, void *pCard, BOOL fRefresh, void* pbURL, BOOL fNewContext)
{
  UA *pUser=NULL;
  WCHAR** ppHistory=NULL;
  WCHAR* pwchTitle=NULL;
  BOOL fStyleIsList=FALSE;
  #ifdef WAE_DEBUG 
    WCHAR** ppTempHistory=NULL;
  #endif
  pUser = (UA*)pUserAgent;
  #ifdef WAE_DEBUG
    PrintChar("--------------------------NEW CARD--------------------------\n");
  #endif
  if (pCard!=NULL)
  {
    pwchTitle = WML_GetCardTitle (pUser, (ELEMENTTYPE*) pCard);

    #ifdef WAE_DEBUG
      PrintChar("Title:");
      WMLUA_PrintWS(pwchTitle);
    #endif

    ppHistory = WML_GetHistoryListTitles (pUser);

    fStyleIsList = ( ((CARDELEMENT*) ( ((ELEMENTTYPE*)pCard)->pElm ))->fOrdered);

    #ifdef WAE_DEBUG
      PrintChar("----------- HISTORY ---------\n");
      PrintChar("Cardtitles:");
      if (ppHistory!=NULL)
      {
        ppTempHistory = ppHistory;
        while (*ppTempHistory != NULL)
        {
          WMLUA_PrintWS(*ppTempHistory);
          ppTempHistory++;
        }
      }
      PrintChar("------------:");
    #endif

    MMIa_newCard((UINT8)pUser->iViewID, (const WCHAR *)pwchTitle, (BOOL)fStyleIsList, (BOOL)fRefresh, (const CHAR *)pbURL, (BOOL)fNewContext, ppHistory);

    DEALLOC(&pwchTitle);
  }
  else
  {
    MMIa_newCard(pUser->iViewID, NULL, 0, 0, NULL, TRUE, NULL);
  }

  DEALLOC(& ppHistory); 
}

/* clears the current card */
void SDL_ClearCard(void* pUserAgent, BOOL fNewContext )
{
	WCHAR** ppHistory=NULL;
	BYTE* pURL =NULL;
	if (pUserAgent)
    {
	  if (!fNewContext)
	  {	  
        ppHistory = WML_GetHistoryListTitles ((UA*)pUserAgent);
	    pURL = ((UA*)(pUserAgent))->pbURLBase;
      }
	  MMIa_newCard(((UA*)pUserAgent)->iViewID, NULL, TRUE, FALSE, (CHAR*)pURL, fNewContext, ppHistory);
      MMIa_showCard(((UA*)pUserAgent)->iViewID);
    }
	DEALLOC(& ppHistory); 
}


BOOL SDL_CreateText(void *pUserAgent,void* pTextElement, int iTextID, int iEmphMode, void* pLinkElement, int iWhiteSpace )
{
  UA *pUser;
  WCHAR* pwchText=NULL;
/*  Removed by GBU,000314,  not used
  TEXTELEMENT *pTheElement=NULL;
*/
  WCHAR* pLinkTitle=NULL;
  BOOL fIsLink=FALSE;
  BOOL fRes=FALSE;

  pUser = (UA*)pUserAgent;
  if (pTextElement !=NULL)
  {
/*  Removed by GBU,000314,  not used
    pTheElement=(TEXTELEMENT*)(((ELEMENTTYPE*)pTextElement)->pElm);
*/
    pwchText = WML_GetString(pUser, (ELEMENTTYPE*)pTextElement);

    #ifdef WAE_DEBUG
      PrintChar("--Text--\n");
      WMLUA_PrintWS(pwchText);
    #endif

    /* if a linkelement is present the title is retrieved */ 
    if (pLinkElement)
    {
       fIsLink = TRUE;
       if ( ((ELEMENTTYPE*)pLinkElement)->iType == Type_A)
       {
          pLinkTitle = WML_GetTextString(pUser, ((AELEMENT*)(((ELEMENTTYPE*)pLinkElement)->pElm))->pTitle );
       }
       else
       {
          pLinkTitle = WML_GetTextString(pUser, ((ANCHORELEMENT*)(((ELEMENTTYPE*)pLinkElement)->pElm))->pTitle );
       }
    }
    else
       fIsLink = FALSE;

/* whitespace criteria 
   If the element is a text and the flag is set, trim the text with the
   parameter set to left. Also, if the text ends with a white space, set
   the flag. Otherwise clear it.

   else

   If the element is a text and the flag is not set, trim the text with
   the parameter set to right. Also, if the text ends with a white 
   space, set the flag. Otherwise clear it.
*/

    if (pwchText)
    {
       if (iWhiteSpace)
         fRes = WML_TrimTextWCHAR( pwchText, TRIMTEXT_LEFT);
       else
         fRes = WML_TrimTextWCHAR( pwchText, TRIMTEXT_NONE);
    }

    MMIa_newText ((UINT8)(pUser->iViewID), (UINT8)iTextID, pwchText, fIsLink, pLinkTitle, (INT8)iEmphMode);
    DEALLOC(&pLinkTitle);
    DEALLOC(&pwchText);
  }
  return fRes;

   /* NOTE: TextID is always 0 if it´s not a link */
}


int SDL_UpdateEmph(void* pEmphElement, int pEmphMode, BOOL fModeOn)
{
   UINT8 iType=0;
   UINT8 iEmph =0;
   if (pEmphElement)
   {
	   iType = ((ELEMENTTYPE*)pEmphElement)->iType;
	   switch (iType)
	   {
		 case Type_SMALL:
			  iEmph = TXT_SMALL;
			  break;
		 case Type_BIG:
			  iEmph = TXT_BIG;
			  break;
		 case Type_B:
			  iEmph = TXT_BOLD;
			  break;
		 case Type_I:
			  iEmph = TXT_ITALIC;
			  break;
		 case Type_U:
			  iEmph = TXT_UNDERLINE;
			  break;
		 case Type_EM:
			  iEmph = TXT_EMPHASIS;
			  break;
		 case Type_STRONG:
			  iEmph = TXT_STRONG;
			  break;
		 default :
			  break;
	   }
	   if (fModeOn)
		 return ((UINT8)pEmphMode | iEmph);  /* set emphmode */
	   else
		 return ((UINT8)pEmphMode ^ iEmph);  /* clear emphmode */
   }
   return pEmphMode;
}

void SDL_CreateInput(void *pUser, void* pElement, int iInputId)
{
  WCHAR* pwchTitle=NULL;
  WCHAR* pwchText=NULL;
  INPUTELEMENT *pInputElm=NULL;
  BOOL fIsPassword=FALSE;
  if (pElement !=NULL)
  {
    if (((ELEMENTTYPE*)pElement)->iType==Type_INPUT)
    {
      pInputElm=(INPUTELEMENT *)(((ELEMENTTYPE*)pElement)->pElm);
      pwchTitle = WML_GetTextString((UA*)pUser, pInputElm->pTitle);
      pwchText = WML_GetInputKeyValue((UA*)pUser, (ELEMENTTYPE*)pElement);

      if (pInputElm->iType==DEF_TYPE_PASSWORD)
          fIsPassword = TRUE;

      MMIa_newInput (((UA*)pUser)->iViewID, (UINT8)iInputId, pwchTitle , pwchText, fIsPassword, pInputElm->fEmptyOK, pInputElm->pchFormat, (INT8)(pInputElm->iSize), (INT8)(pInputElm->iMaxLength), pInputElm->iTabIndex);
 
      #ifdef WAE_DEBUG
        PrintChar("--Input--\n");
        PrintChar("Title:");
        PrintChar("Text");
        WMLUA_PrintWS(pwchText);
      #endif

    DEALLOC(&pwchTitle);
    DEALLOC(&pwchText);
    }
  }
}

/*---------------------------------------------------------------------
Handle tables  SDL_CreateTable, SDL_CloseTable,SDL_CreateTableCell
----------------------------------------------------------------------*/
void SDL_CreateTable(void* pUserAgent, void* pElement)
{
  WCHAR* pwchTitle=NULL;
  BYTE* pbAlign=NULL;
  BOOL fDummy;
  TABLEELEMENT *pTableElm=NULL;

  if (pElement !=NULL)
  {
    if (((ELEMENTTYPE*)pElement)->iType==Type_TABLE)
    {
      pTableElm=(TABLEELEMENT*)( ((ELEMENTTYPE*)pElement)->pElm );
      pwchTitle = WML_GetTextString((UA*)pUserAgent, pTableElm->pTitle);

      pbAlign =  wip_wchar2byte( pTableElm->pchAlign, & fDummy);
      MMIa_newTable (((UA*)pUserAgent)->iViewID, pwchTitle, pTableElm->iColumns, (CHAR*)pbAlign);

      DEALLOC(&pbAlign);

      #ifdef WAE_DEBUG
        PrintChar("--Table--\n");
      #endif

      DEALLOC(&pwchTitle);
    }
  }
}

void SDL_CloseTable(void* pUserAgent)
{
  if (pUserAgent)
    MMIa_closeTable(((UA*)pUserAgent)->iViewID);
}

void SDL_CreateTableCell(void* pUserAgent )
{
  if (pUserAgent)
    MMIa_newTableData(((UA*)pUserAgent)->iViewID);
}


/*---------------------------------------------------------------------
   SDL_CreateSelect,SDL_CreateOptGrp,SDL_CloseOptGrp,SDL_CreateOption
----------------------------------------------------------------------*/
void SDL_CreateSelect(void *pUserAgent, void* pElement)
{
  WCHAR* pwchTitle=NULL;
  UA* pUser=NULL;
  SELECTELEMENT *pSelectElm=NULL;

  if (pElement)
  {
    pUser = (UA*)pUserAgent;
    pSelectElm=(SELECTELEMENT*)(((ELEMENTTYPE*)pElement)->pElm);
    pwchTitle = WML_GetTextString(pUser, pSelectElm->pTitle);

    MMIa_newSelect(pUser->iViewID, pwchTitle, pSelectElm->fMultiple, (INT8)(pSelectElm->iTabIndex) );

    #ifdef WAE_DEBUG
      PrintChar("--Select--\n");
    #endif

    DEALLOC(&pwchTitle);
  }
}

void SDL_CloseSelect(void *pUser)
{
 #ifdef WAE_DEBUG
    PrintChar("--CloseSelect--\n");
 #endif
 if (pUser)
   MMIa_closeSelect(((UA*)pUser)->iViewID);
}

void SDL_CreateOptGrp(void *pUser, void* pElement)
{
  WCHAR* pwchTitle=NULL;
  OPTGROUPELEMENT *pOptGrpElm=NULL;

  if (pElement !=NULL)
  {
     pOptGrpElm=(OPTGROUPELEMENT *)(((ELEMENTTYPE*)pElement)->pElm);
     pwchTitle = WML_GetTextString((UA*)pUser, pOptGrpElm->pTitle);

     #ifdef WAE_DEBUG
       PrintChar("--OptGrp--\n");
     #endif
     MMIa_newOptionGroup ( ((UA*)pUser)->iViewID, pwchTitle);
     DEALLOC(&pwchTitle);
  }
}

void SDL_CloseOptGrp(void *pUser)
{
 #ifdef WAE_DEBUG
    PrintChar("--CloseOptGrp--\n");
 #endif
 if (pUser)
     MMIa_closeOptionGroup (((UA*)pUser)->iViewID);
}

void SDL_CreateOption(void* pUserAgent, void* pElement, void* pwchLabelText, int iOptionID)
{
  OPTIONELEMENT *pOptionElm=NULL;
  WCHAR* pwchTitle=NULL;

  if (pElement)
  {
    pOptionElm=(OPTIONELEMENT *)(((ELEMENTTYPE*)pElement)->pElm);
    pwchTitle = WML_GetTextString((UA*)pUserAgent, pOptionElm->pTitle);
    if (pOptionElm !=NULL)
      MMIa_newOption (((UA*)pUserAgent)->iViewID, (UINT8)iOptionID, (WCHAR*)pwchLabelText, pwchTitle, pOptionElm->fSelected);
    DEALLOC(&pwchTitle);
    #ifdef WAE_DEBUG
      PrintChar("--Option--\n");
      WMLUA_PrintWS(pwchLabelText);
    #endif
  }
}


/*-------------------------------------------------------------------
updates and returns the option element 
----------------------------------------------------------------------*/
void* SDL_UpdateOption(void *pUserAgent, void* pOptionList, int OptionID)
{
/*  Removed by GBU,000314,  not used
  UA *pUser;
*/

  void* pListElement=NULL;
  void* pSelectListElement=NULL;

  ELEMENTTYPE* pTheReturnElement=NULL;
  OPTIONELEMENT* pOptElm=NULL;

  ELEMENTTYPE* pTheWMLElement=NULL;
  #ifdef WAE_DEBUG
       PrintChar("--Updateoption--\n");
  #endif

 /* int iTmp=0; */

    /*  Added by GBU,000315,    to remove warning for unused variable */
    pUserAgent = pUserAgent;

/*  Removed by GBU,000314,  not used
  pUser = (UA*)pUserAgent;
*/
  if (pOptionList!=NULL)
  {
    /*get first element */
    pListElement = SDL_GetNextListElement(pOptionList,NULL);

    while (pListElement!=NULL)
    {
       pTheWMLElement = (ELEMENTTYPE*)SDL_GetListElementContent(pListElement);
       /* found a select list element - always saving the latest*/
       if (pTheWMLElement->iType==Type_SELECT)
         pSelectListElement = pListElement;

       /* found the actual option element to toggle */
       else  if (SDL_GetListElementID(pListElement) == OptionID)
       {
         pTheReturnElement = pTheWMLElement;
         pOptElm = (OPTIONELEMENT*)(pTheReturnElement->pElm);
         break;
       }
       pListElement = SDL_GetNextListElement(pOptionList,pListElement);
    }

    /* scan the select */
    if (pSelectListElement!=NULL && pOptElm!=NULL)
    {
       pTheWMLElement = (ELEMENTTYPE*)SDL_GetListElementContent(pSelectListElement);

       /* if multiple select no problem, just toggle */
       if ( ((SELECTELEMENT*)(pTheWMLElement->pElm))->fMultiple==TRUE)
       {
           /* set the option */ 
           if (pOptElm->fSelected)
               pOptElm->fSelected=FALSE;
           else
               pOptElm->fSelected=TRUE;
       }

       /* otherwise clear all fields and toggle*/
       else
       {
          #ifdef WAE_DEBUG
             PrintChar("Single option select");
          #endif
         pListElement = SDL_GetNextListElement(pOptionList,pSelectListElement);
         while (pListElement!=NULL)
         {
            pTheWMLElement = (ELEMENTTYPE*)SDL_GetListElementContent(pListElement);

            if (pTheWMLElement->iType != Type_OPTION)
            {
              /* select is empty or no more optelelemts*/
              break;
            }
            ((OPTIONELEMENT*)(pTheWMLElement->pElm))->fSelected=FALSE;
            pListElement = SDL_GetNextListElement(pOptionList,pListElement);
         }
         pOptElm->fSelected=TRUE;
       }
    }
  }

  #ifdef WAE_DEBUG
    pListElement = SDL_GetNextListElement(pOptionList,pSelectListElement);
    while (pListElement!=NULL)
    {
        pTheWMLElement = (ELEMENTTYPE*)SDL_GetListElementContent(pListElement);

        if (pTheWMLElement->iType != Type_OPTION)
        {
            /* select is empty or no more optelelemts*/
            break;
        }

        if (((OPTIONELEMENT*)(pTheWMLElement->pElm))->fSelected)
            PrintChar("TRUE");
        else
            PrintChar("FALSE");
        pListElement = SDL_GetNextListElement(pOptionList,pListElement);
    }
    PrintChar("--Updateoption--\n");
  #endif

  return (void*)pTheReturnElement;
}


/*-------------------------------------------------------------------
Creates a new image element, depending on if dynamic updating is set
or not the UpdateImage is used when the actual image data arrives
----------------------------------------------------------------------*/
void SDL_CreateImage(void *pUserAgent, void *pElement, void* pData, int iDataLen, void* pbContentTypeAsString, int iImageID, void* pLinkElement)
{
  IMGELEMENT    *pImageElm=NULL;
  UA    *pUser;
  WCHAR *pwchAlternativeText = NULL;
  WCHAR *pwchLocalSrc = NULL;
  WCHAR *pLinkTitle=NULL;
  BOOL  fIsLink=FALSE;

  pUser = (UA*)pUserAgent;
  if (pElement !=NULL)
  {
     pImageElm=(IMGELEMENT *)(((ELEMENTTYPE*)pElement)->pElm);

     pImageElm->iMMIElementID = (UINT8)iImageID;

     pwchAlternativeText = WML_GetTextString((UA*) pUserAgent, pImageElm->pAlt );
     pwchLocalSrc = WML_GetTextString((UA*) pUserAgent, pImageElm->pLocalSrc );

     /* if a linkelement is present the title is retrieved */ 
     if (pLinkElement)
     {
        fIsLink = TRUE;
        if ( ((ELEMENTTYPE*)pLinkElement)->iType == Type_A)
        {
           pLinkTitle = WML_GetTextString(pUser, ((AELEMENT*)(((ELEMENTTYPE*)pLinkElement)->pElm))->pTitle );
        }
        else
        {
           pLinkTitle = WML_GetTextString(pUser, ((ANCHORELEMENT*)(((ELEMENTTYPE*)pLinkElement)->pElm))->pTitle );
        }
     }
     else
        fIsLink = FALSE;

     MMIa_newImage (pUser->iViewID, (UINT8)iImageID, (CHAR*)pData, (UINT16)iDataLen , (CHAR*)pbContentTypeAsString, pwchAlternativeText, pwchLocalSrc,fIsLink,pLinkTitle, pImageElm->iVSpace,pImageElm->iHSpace, pImageElm->iWidth, pImageElm->iHeight, pImageElm->iIsPercent, (INT8)(pImageElm->iIAlign));

     DEALLOC( & pwchAlternativeText);
     DEALLOC( & pwchLocalSrc);
     DEALLOC( & pLinkTitle);
  }
  else
  {
     MMIa_newImage (pUser->iViewID, (UINT8)iImageID, NULL, 0 , NULL, NULL,NULL, FALSE, NULL,0, 0, 0, 0, 0, 0);
  }
}

void SDL_UpdateImage(void *pUserAgent, void *pImageElement, void* pData, int iDataLen, void* pchContentTypeAsString)
{
  IMGELEMENT *pElm;
 #ifdef WAE_DEBUG
    PrintChar("--IMAGEUPDATE-\n");
 #endif

  if (pImageElement)
  {
    if ( ((ELEMENTTYPE*)pImageElement)->iType == Type_IMG )
    {
     pElm = (IMGELEMENT*)(((ELEMENTTYPE*)pImageElement)->pElm);
      MMIa_completeImage ( ((UA*)pUserAgent)->iViewID, pElm->iMMIElementID, (CHAR*)pData, (UINT16)iDataLen, (CHAR*)pchContentTypeAsString);
     
      #ifdef WAE_DEBUG
        PrintChar("Image updated\n");
      #endif
    }
  }
}


/*-------------------------------------------------------------------
Creates a softkey element with reference id iDoRef
----------------------------------------------------------------------*/
void SDL_CreateKey(void *pUser,void *pElement, int iDoRef)
{
  DOELEMENT *pDoElm;
  WCHAR *pwchLabel;
  if (pElement)
  {
    pDoElm=(DOELEMENT *)(((ELEMENTTYPE*)pElement)->pElm);

    pwchLabel = WML_GetTextString((UA*)pUser, pDoElm->pLabel);
    MMIa_newKey ( ((UA*)pUser)->iViewID, (UINT8)iDoRef, pDoElm->pchType, pwchLabel, pDoElm->fOptional );
    #ifdef WAE_DEBUG
      PrintChar("--SOFTKEY--\n");
    #endif
    DEALLOC(&pwchLabel);
  }
}

/* returns TRUE if there already exists a identical do element i.e same name */
int SDL_CheckDoDuplicates(void *pUser, void *pDoElement, void* pDoList)
{
    ELEMENTTYPE *pCompareDoElement=NULL;
    WCHAR   *pwchDoName1 = NULL;
    WCHAR   *pwchDoName2 = NULL;
    void    *pListDoElement=NULL;

    if (pDoElement == NULL || pDoList==NULL)
        return TRUE;

    /*get first element */
    pListDoElement = SDL_GetNextListElement(pDoList,NULL);

    while (pListDoElement!=NULL)
    {
       pCompareDoElement = (ELEMENTTYPE*)SDL_GetListElementContent(pListDoElement);  

       if (pCompareDoElement->iType==Type_DO)
       {
	     /* check is name is duplicate */
		  pwchDoName1 = ((DOELEMENT*)(pCompareDoElement->pElm))->pchName;
		  if (pwchDoName1==NULL)
			 pwchDoName1 = ((DOELEMENT*)(pCompareDoElement->pElm))->pchType;  

		  pwchDoName2 = ((DOELEMENT*)(((ELEMENTTYPE*)pDoElement)->pElm))->pchName;
		  if (pwchDoName2==NULL)
		     pwchDoName2 = ((DOELEMENT*)(((ELEMENTTYPE*)pDoElement)->pElm))->pchType;

		  if (pwchDoName1 && pwchDoName2)
          { 
		     if (!COMPARESTRING(pwchDoName1,pwchDoName2))
				return TRUE;
		  }
		  else
		  {
		    return TRUE;
          }
	   }

       pListDoElement = SDL_GetNextListElement(pDoList,pListDoElement);
    }
	pwchDoName2 = (WCHAR   *)pUser; /* at this point only a dummy assignment */
    return FALSE;
}


void SDL_CreateParagraph(void *pUser,void *pElement)
{
  PELEMENT *pPElm=NULL;

  if (pElement)
  {
      pPElm=(PELEMENT*)(((ELEMENTTYPE*)pElement)->pElm);
      MMIa_newParagraph (((UA*)pUser)->iViewID, (INT8)(pPElm->iAlign),!((BOOL)(pPElm->iMode)));
  }
  else
  {
      MMIa_newParagraph (((UA*)pUser)->iViewID,ALIGN_LEFT,TRUE);
  }

  #ifdef WAE_DEBUG
    PrintChar("--P--\n");
  #endif
}

void SDL_CloseParagraph(void *pUser)
{
  #ifdef WAE_DEBUG
    PrintChar("--/P--\n");
  #endif
  if (pUser)
    MMIa_closeParagraph (((UA*)pUser)->iViewID);
}

void SDL_CreateBR(void *pUser)
{
  if (pUser)
    MMIa_newBreak (((UA*)pUser)->iViewID);

  #ifdef WAE_DEBUG
    PrintChar("--BR--\n");
  #endif
}

/*-------------------------------------------------------------------
Create and close a new fieldset element
----------------------------------------------------------------------*/
void SDL_CreateField(void *pUser,void *pElement)
{
 FIELDSETELEMENT *pFieldElm;
 WCHAR* pwchText=NULL;
 if (pElement)
 {
    pFieldElm=(FIELDSETELEMENT *)(((ELEMENTTYPE*)pElement)->pElm);
    pwchText = WML_GetTextString((UA*)pUser, pFieldElm->pTitle);
    MMIa_newFieldSet(((UA*)pUser)->iViewID,pwchText);

    #ifdef WAE_DEBUG
      PrintChar("--FIELD--\n");
     #endif
    DEALLOC(&pwchText);
 }
}

void SDL_CloseField(void* pUser)
{
 if (pUser)
   MMIa_closeFieldSet(((UA*)pUser)->iViewID);

 #ifdef WAE_DEBUG
    PrintChar("--FIELDCLOSE--\n");
 #endif

}
/*----------------------------------------------------------------------*/


#endif
