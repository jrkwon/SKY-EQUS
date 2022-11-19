/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_clid.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-11   hjcho      Created.
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "comdef.h"

#include "target.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "db.h"
#ifdef FEATURE_IS95B_EXT_DISP 
#include "cm.h"
#endif

#include "ska.h"

#ifdef FEATURE_IS95B_EXT_DISP 
/* Whether the CLI stored represents the whole caller information from
** the same OTA message, (including number and name)
*/
#ifdef WIN32
boolean fCallerIDComplete;
#else
extern boolean fCallerIDComplete;
#endif //WIN32
#endif //FEATURE_IS95B_EXT_DISP 

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL CNI_T l_CallerID;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void SKY_API sEnter(void);
LOCAL void SKY_API sClearCLISubfieldInSecretGroup(void);
/* <EJECT> */
/*===========================================================================
FUNCTION CNI_IS_VALID

DESCRIPTION
  Verify that the incoming Caller ID (CNI) contains only valid characters,
  i.e., digits, '*', '#' or '-'.


DEPENDENCIES
  ui.cni must contain the Caller ID.

RETURNS
  Boolean denoting valid CNI or not.
===========================================================================*/





/*****************************************************************************/
/*                                                                           */
/*                              EQUS SECTION                                 */
/*                                                                           */
/*                        2000.11.1          HJCHO                           */
/*                                                                           */
/*****************************************************************************/

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetupCalledPartyNumber
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetupCalledPartyNumber(UINT8 nLength, BYTE bpszNumberBuffer[])
{
	CNI_T*   pCallerID;
	pCallerID = SKY_GetCallerID();
    
	if(nLength > 0)
	{
	   pCallerID->CalledParty.nPos    = nLength;
	   pCallerID->CalledParty.fSecret = FALSE;
       STRCPY(pCallerID->CalledParty.aBuf, bpszNumberBuffer);
	}
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetupCNAP(CHAR*, BYTE)
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
int SetupCNAP(BYTE* pszNameBuf, BYTE nNameLen)
{
  CNI_T*   pCallerID;
  BYTE     abTmpBuffer[CM_MAX_ALPHA_TAG_CHARS]; /* temp buffer to store formatted CNAP */
  BYTE     bTmpLength, iLoop;

  pCallerID   =  SKY_GetCallerID();

  /* The UI supports only name with maximum length UI_WIDE
  ** truncate the name and append with ellipsis character if it is longer
  */
  if (nNameLen > CM_MAX_ALPHA_TAG_CHARS)
  {
    bTmpLength = CM_MAX_ALPHA_TAG_CHARS;
  }
  else
  {
    bTmpLength = nNameLen;
  }

  /* Convert non-printable control characters into spaces */
  for (iLoop=0; (iLoop<bTmpLength) && pszNameBuf[iLoop]; iLoop++)
  {
    if ((pszNameBuf[iLoop] > 0)
         && (pszNameBuf[iLoop] < 0x20))
    {
      abTmpBuffer[iLoop] = ' ';
    }
    else
    {
      abTmpBuffer[iLoop] = pszNameBuf[iLoop];
    }
  }

  if(iLoop > 0)
  {
    memcpy(pCallerID->aName, abTmpBuffer, iLoop);
    pCallerID->aName[iLoop] = '\0';
    pCallerID->bNameLen     = iLoop;
  }


  return iLoop;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sTransferPBookInfo2CallerID()
**
** Description: 
**     Transfer Phonebook information which is matched by incoming dial number
**     to CallerID structure.
**     CallerID structure will be showed on the LCD.
**
** Input:
**     None
**
** Output:
**     None
**     
**
** Return value:
**
**     PB_READ_OK              : Phonebook read ok
**     PB_SECRET_OK            : input number is secret
**     PB_READ_NO_BOOK_INFO    : no phonebook information to read
**     PB_READ_NO_GROUP_INDEX  : no phonebook index to read
**     PB_READ_NO_GROUP_INFO   : no group information to read
**     
**
** Side effects:
**     The contents of global variable, CallerID.
**
** ---------------------------------------------------------------------------
*/
READPBOOK_E SKY_API sTransferPBookInfo2CallerID()
{
  CNI_T*                   pCallerID;
  pb_book_info_type        sPBookInfo;
  pb_group_info_type       sGroupInfo;
  INT16                    nBookAddress;
   
  

  if(SKY_GET_AUTO_LOCK_MODE() == FALSE)
  {
      pCallerID   =  SKY_GetCallerID();

  
      nBookAddress = SearchNumber(pCallerID->Num.aBuf, BSM_NUMBER_FULL_MATCH);
      if( nBookAddress > 0 )
      {
          STRCPY(pCallerID->aNameInPhoneBook, g_szPersonName[nBookAddress-1] );
      }
      else
      {
          return PB_READ_NO_BOOK_INFO;
      }
      sPBookInfo.address = nBookAddress;
  
      if(ui_get_pb(PB_BOOK_INFO_I, &sPBookInfo) == TRUE)
      {
        pCallerID->bNameLenInPhoneBook  =  STRLEN(pCallerID->aNameInPhoneBook);
        pCallerID->dwPicture    =  sPBookInfo.dwImageIndex;
        sGroupInfo.address      =  sPBookInfo.group_address;
    
        if(ui_get_pb(PB_GROUP_INFO_I, &sGroupInfo) > 0)
        {
            STRCPY(pCallerID->aGroupNAME, sGroupInfo.group_name);
        }
        else
        {
            return PB_READ_NO_GROUP_INDEX;
        }    
    
        pCallerID->GroupBell.wBellID  =  sGroupInfo.Bell.wBellID;
        STRCPY(pCallerID->GroupBell.szFileName, sGroupInfo.Bell.szFileName);
    #ifdef FEATURE_COLOR_ID_LCD
        pCallerID->GroupBacklight = sGroupInfo.alert_color;
    #endif
        if(sGroupInfo.status & GROUP_STATUS_SECRET) /* open group member                */
        {
           sClearCLISubfieldInSecretGroup();
	       pCallerID->fSecret  =  TRUE;
	    }
        else                                        /* secret group member              */
	    {
	       pCallerID->fSecret  =  FALSE;
	    }
    
        return PB_READ_OK;
      }
      else
      {
          return PB_READ_NO_BOOK_INFO;
      }
  }
  else
  {
     return PB_READ_NO_BOOK_INFO;
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     ClearCLI (ui_clear_cli)
**
** Description: 
**     Clear the information of CLI buffer
**
** Input:
**     None
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     Clear the CLI buffer
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API ClearCLI(VOID)
{
  CNI_T*      pCallerID;

  pCallerID = SKY_GetCallerID();

  /* Clear the name */
  memset(pCallerID->aName, '\0', SKY_CALL_MAX_LTRS);
  memset(pCallerID->aGroupNAME, '\0', SKY_CALL_MAX_LTRS);       
  memset(pCallerID->aNameInPhoneBook, '\0', SKY_CALL_MAX_LTRS); 
  pCallerID->bNameLenInPhoneBook      = 0;
  pCallerID->bNameLen                 = 0;
  pCallerID->bGNameLen                = 0;
  pCallerID->Num.fSecret              = FALSE;
  pCallerID->bPI                      = CAI_ALLOW_PI;
  memset(pCallerID->Num.aBuf, '\0', EQS_MAX_DIAL_DIGITS);
  pCallerID->Num.nPos                 = 0;  
  pCallerID->GroupBell.wBellID        = 0;      //Initialize Group Bell index
  pCallerID->GroupBell.szFileName[0]  = '\0';
  
  memset(pCallerID->CalledParty.aBuf, '\0', EQS_MAX_DIAL_DIGITS);
  pCallerID->CalledParty.nPos         = 0;
  pCallerID->CalledParty.fSecret      = FALSE; 
  pCallerID->CalledParty.fChild       = FALSE;
#ifdef FEATURE_COLOR_ID_LCD 
  pCallerID->GroupBacklight           = (ID_LCD_COLOR_E)NONE_GROUP_ALERT_LCD;
#endif
  pCallerID->dwPicture                = 0; /* 사진이 없을 경우 */
} /* end ClearCLI */



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     ClearCLI (ui_clear_cli)
**
** Description: 
**     Clear the information of CLI buffer
**
** Input:
**     None
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     Clear the CLI buffer
**
** ---------------------------------------------------------------------------
*/
void SKY_API sClearCLISubfieldInSecretGroup(void)
{
  CNI_T*      pCallerID;

  pCallerID = SKY_GetCallerID();

  memset(pCallerID->aGroupNAME, '\0', SKY_CALL_MAX_LTRS);       
  memset(pCallerID->aNameInPhoneBook, '\0', SKY_CALL_MAX_LTRS); 
  pCallerID->bNameLenInPhoneBook      = 0;
  pCallerID->bGNameLen                = 0;
  pCallerID->GroupBell.wBellID        = 0;      //Initialize Group Bell index
  pCallerID->GroupBell.szFileName[0]  = '\0';
  #ifdef FEATURE_COLOR_ID_LCD 
  pCallerID->GroupBacklight           = (ID_LCD_COLOR_E)NONE_GROUP_ALERT_LCD;
#endif
  pCallerID->dwPicture                = 0; /* 사진이 없을 경우 */
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define MAX_CLI_PHONENUMBER       15

BOOL sMakeCompleteNumber(BYTE aCLIBuf[])
{
  BYTE    aTmpCLIBuf[MAX_CLI_PHONENUMBER];
  UINT8   iLoop;

    
  if((STRNCMP(aCLIBuf, (BYTE*)"11", 2) == 0) ||
	 (STRNCMP(aCLIBuf, (BYTE*)"19", 2) == 0) ||
	 (STRNCMP(aCLIBuf, (BYTE*)"16", 2) == 0) ||
	 (STRNCMP(aCLIBuf, (BYTE*)"17", 2) == 0))  /* 1zxxxxyyyy로 저장되어 있는 경우 */
  {
    aTmpCLIBuf[0] = '0';
    for(iLoop = 0; aCLIBuf[iLoop] != '\0'; iLoop++)
    {
      aTmpCLIBuf[iLoop + 1] = aCLIBuf[iLoop];
    }
    aTmpCLIBuf[iLoop+1] ='\0';
    STRCPY(aCLIBuf, aTmpCLIBuf);

	return TRUE;
  }
  else
  {
	return FALSE;
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetupCLICDNIP (ui_setup_cli)
**
** Description: 
**     Setup Buffer to Display CLI information
**
** Input:
**     pszNumBuf -  CLI String
**     nLenBuf   -  CLI String length
**     bPI       -  CLI presentation indicator
**
** Output:
**     
**
** Return value:
**     
**
** Side effects:
**     Put CLI information to CallerID buffer
**
** ---------------------------------------------------------------------------
*/
void SKY_API SetupCLICDNIP(CHAR* pszCLIBuf, UINT8 nLenBuf, BYTE bPI, SKY_EVENT_E EVENT)
{
    BOOL         fTwoNumber;
    UINT8        nLoop, jLoop;
    BYTE         aTmpCLIBuf[UI_MAX_DIALED_DIGITS+1];
    BYTE         aTmpMyNumber[UI_MAX_DIALED_DIGITS+1];
    CNI_T*       pCallerID;

    fTwoNumber = FALSE;
    pCallerID = SKY_GetCallerID();

    if(pCallerID->bPI != CAI_RES_PI)
    {
      pCallerID->bPI = bPI;
    }


    /* The UI supports only number with maximum length UI_MAX_DIALED_DIGITS 
    ** truncate the number if it is longer
    */
    if (nLenBuf > UI_MAX_DIALED_DIGITS)
    {
      nLenBuf = UI_MAX_DIALED_DIGITS;
    }
      
    /* Discard control characters into spaces */
    for (nLoop=0, jLoop=0; nLoop<nLenBuf && pszCLIBuf[nLoop]; nLoop++)
    {
      if ((pszCLIBuf[nLoop] <= 0x20) || (pszCLIBuf[nLoop] == '-'))
      {
          continue;
      }
      else
      {
          aTmpCLIBuf[jLoop] = pszCLIBuf[nLoop];
          jLoop++;
      }
    }
    /* Null Terminate 시킨다. */
    aTmpCLIBuf[jLoop] = '\0';      
    
	/* 4자리 국번의 경우 11-xxxx-yyyy의 날아올 때가 있다	     */
	/* sMakeCompleteNumber에서 앞에 0을 붙여주고 문자열의 길이를 */
	/* 나타내는 jLoop를 하나 증가 시킨다.                        */
	if(sMakeCompleteNumber(aTmpCLIBuf))
	{
		jLoop++;
	}

    if(SKY_IS_NUMBER_PLUS())    /* TWO NUMBER SERVICE가 TRUE로 설정되어 있는 경우*/
    {        
        if(EVENT == SE_CALL_CALLED_PARTY)
        {        
            /* Number Plus로 설정된 넘버 또는 현재의 자기번호와 동일한 번호가    */
            /* pCallerID->Num.aBuf에 있을 경우에는 pCallerID->CalledParty.aBuf로 */
            /* 넘긴다.                                                           */            
            STRCPY(aTmpMyNumber, GetMyNumber());       
            if(STRCMP(aTmpMyNumber, aTmpCLIBuf) == 0) 
            {
                /* 모번호인 경우 */
                memcpy(pCallerID->CalledParty.aBuf, aTmpCLIBuf, jLoop);
                pCallerID->CalledParty.aBuf[jLoop] = '\0';
                pCallerID->CalledParty.nPos        = jLoop;
                pCallerID->CalledParty.fSecret     = FALSE;
                pCallerID->CalledParty.fChild      = FALSE;
                fTwoNumber = TRUE;
            } 
            else if(((SKY_GET_SERVICE_PROVIDER() == SP_SKT)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_SKT(), aTmpCLIBuf) == 0)) ||
                    ((SKY_GET_SERVICE_PROVIDER() == SP_STI)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_STI(), aTmpCLIBuf) == 0)))

            {
                /* 자번호인 경우 */
                memcpy(pCallerID->CalledParty.aBuf, aTmpCLIBuf, jLoop);
                pCallerID->CalledParty.aBuf[jLoop] = '\0';
                pCallerID->CalledParty.nPos        = jLoop;
                pCallerID->CalledParty.fSecret     = FALSE;
                pCallerID->CalledParty.fChild      = TRUE;
                fTwoNumber = TRUE;
            }
            else
            {
                /* Call Forwarding 날아오는 Number */
                memcpy(pCallerID->CalledParty.aBuf, aTmpCLIBuf, jLoop);
                pCallerID->CalledParty.aBuf[jLoop] = '\0';
                pCallerID->CalledParty.nPos        = jLoop;
                pCallerID->CalledParty.fSecret     = FALSE;
                pCallerID->CalledParty.fChild      = FALSE;
                fTwoNumber = TRUE;
            }
        }
        else if(EVENT == SE_CALL_CALLER_ID)
        {
            /* Number Plus로 설정된 넘버 또는 현재의 자기번호와 동일한 번호가    */
            /* pCallerID->Num.aBuf에 있을 경우에는 pCallerID->CalledParty.aBuf로 */
            /* 넘긴다.                                                           */            
            STRCPY(aTmpMyNumber, GetMyNumber());       
            if(STRCMP(aTmpMyNumber, aTmpCLIBuf) == 0) 
            {
                /* 모번호인 경우 */
                memcpy(pCallerID->CalledParty.aBuf, aTmpCLIBuf, jLoop);
                pCallerID->CalledParty.aBuf[jLoop] = '\0';
                pCallerID->CalledParty.nPos        = jLoop;
                pCallerID->CalledParty.fSecret     = FALSE;
                pCallerID->CalledParty.fChild      = FALSE;
                fTwoNumber = TRUE;
            } 
            else if(((SKY_GET_SERVICE_PROVIDER() == SP_SKT)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_SKT(), aTmpCLIBuf) == 0)) ||
                    ((SKY_GET_SERVICE_PROVIDER() == SP_STI)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_STI(), aTmpCLIBuf) == 0)))
            {
                /* 자번호인 경우 */
                memcpy(pCallerID->CalledParty.aBuf, aTmpCLIBuf, jLoop);
                pCallerID->CalledParty.aBuf[jLoop] = '\0';
                pCallerID->CalledParty.nPos        = jLoop;
                pCallerID->CalledParty.fSecret     = FALSE;
                pCallerID->CalledParty.fChild      = TRUE;
                fTwoNumber = TRUE;
            }          
			
			
		  /* CNIP의 경우   */
		  if ((bPI == CAI_ALLOW_PI) && (fTwoNumber == FALSE))       //presentation allowed!!
		  {  
			memcpy(pCallerID->Num.aBuf, aTmpCLIBuf, jLoop);
			pCallerID->Num.aBuf[jLoop] = '\0';
			pCallerID->Num.nPos = jLoop;
			pCallerID->Num.fSecret = FALSE;

			if (jLoop) /* We know a incoming call number */
			{
			   sTransferPBookInfo2CallerID();
			}
		  }
		  else if((bPI == CAI_RES_PI) && (fTwoNumber == FALSE))
		  {
			  STRCPY(pCallerID->Num.aBuf, SKY_GET_TOKEN(TKN_CALL_CNIR));
			  pCallerID->Num.nPos = STRLEN(SKY_GET_TOKEN(TKN_CALL_CNIR));
			  pCallerID->Num.fSecret = FALSE;
		  }
        }
	}
    else              /* TWO NUMBER SERVICE가 FALSE인 경우 */
    {        
        if(EVENT == SE_CALL_CALLED_PARTY)
        {
            /* 사용자가 Two Number Service설정에서 "화면에 표시안함"으로 */
            /* 설정해 놓아도 시스템에서는 CDNIP Number가 날아온다.       */
            STRCPY(aTmpMyNumber, GetMyNumber());       
            if(STRCMP(aTmpMyNumber, aTmpCLIBuf) == 0) 
            {
                fTwoNumber = TRUE;
				pCallerID->CalledParty.fChild      = FALSE;

            } 
            else if(((SKY_GET_SERVICE_PROVIDER() == SP_SKT)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_SKT(), aTmpCLIBuf) == 0)) ||
                    ((SKY_GET_SERVICE_PROVIDER() == SP_STI)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_STI(), aTmpCLIBuf) == 0)))
            {
                fTwoNumber = TRUE;
				pCallerID->CalledParty.fChild      = TRUE;
            } 
            else
            {                
                fTwoNumber = TRUE;
                memcpy(pCallerID->CalledParty.aBuf, aTmpCLIBuf, jLoop);
                pCallerID->CalledParty.aBuf[jLoop] = '\0';
                pCallerID->CalledParty.nPos        = jLoop;
                pCallerID->CalledParty.fSecret     = FALSE;
                pCallerID->CalledParty.fChild      = FALSE;
            }
        }
		else if(EVENT == SE_CALL_CALLER_ID)
        {
            /* 사용자가 Two Number Service설정에서 "화면에 표시안함"으로 */
            /* 설정해 놓아도 시스템에서는 CDNIP Number가 날아온다.       */
            STRCPY(aTmpMyNumber, GetMyNumber());       
            if(STRCMP(aTmpMyNumber, aTmpCLIBuf) == 0) 
            {
                fTwoNumber = TRUE;
				pCallerID->CalledParty.fChild      = FALSE;

            } 
            else if(((SKY_GET_SERVICE_PROVIDER() == SP_SKT)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_SKT(), aTmpCLIBuf) == 0)) ||
                    ((SKY_GET_SERVICE_PROVIDER() == SP_STI)&&(STRCMP(SKY_GET_NUMBER_PLUS_NUMBER_STI(), aTmpCLIBuf) == 0)))
            {
                fTwoNumber = TRUE;
				pCallerID->CalledParty.fChild      = TRUE;
            }


            if((bPI == CAI_ALLOW_PI) && (fTwoNumber == FALSE))  
            {
                memcpy(pCallerID->Num.aBuf, aTmpCLIBuf, jLoop);
                pCallerID->Num.aBuf[jLoop] = '\0';
                pCallerID->Num.nPos = jLoop;
                pCallerID->Num.fSecret = FALSE;

                if (jLoop) /* We know a incoming call number */
                {
                    sTransferPBookInfo2CallerID();
                }
            }
            else if((bPI == CAI_RES_PI) && (fTwoNumber == FALSE))  
            {
                STRCPY(pCallerID->Num.aBuf, SKY_GET_TOKEN(TKN_CALL_CNIR));
		        pCallerID->Num.nPos = STRLEN(SKY_GET_TOKEN(TKN_CALL_CNIR));
		        pCallerID->Num.fSecret = FALSE;
            }

       }
	}
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API sEnter()
{
    SKY_AcceptKey(FALSE);

    SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
#ifdef FEATURE_SKY_SUBLCD
    // comment out by jrkwon 2001.05.03
    //SKY_SubBacklightOn(AB_10SECOND); 
#endif

#ifdef FEATURE_IS95B_EXT_DISP   
    if (IsCallerIDComplete()) 
    {
        SetCallerIDComplete(FALSE);
    }
#endif
}




/*============================================================================= 
 Function:    
     SKA_MainClid

 Description: 
     
       
 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
void SKY_API SKA_MainClid(SKY_EVENT_T *pInEvent)
{
    switch(pInEvent->EventID)
	{
    case SE_APP_START :
        sEnter();
        EQS_END();
		break;	

	case SE_APP_END : 
		SKY_AcceptKey(TRUE);

	    break;

case SE_KEY_END:
	case SE_CALL_ENDED:
	    EQS_END_CURRENT_EVENT();
		break;

    default :
		CallDefaultEventProc(pInEvent, ALERTED);
		break;
	}
}


CNI_T* SKY_API SKY_GetCallerID(void)
{
    return &l_CallerID;
}

