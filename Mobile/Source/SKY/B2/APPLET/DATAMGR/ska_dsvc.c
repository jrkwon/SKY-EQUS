/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_dsvc.c
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
** 4334-02-08 white     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"
#include "uisockapi.h"
#include <stdio.h>
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define PORT_NUM		9999
#define PC_ADDRESS_LEN	32+1
#define sFAST_PACEMAKER_SECOND      1000    /* 1 sec */
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagDIALING_MESSAGE_E
{
	SERVICE_PROGRAM_MESSAGE_E = 1,
	PHONE_BOOK_SEARCH_MESSAGE_E,
	NUMBER_SEARCH_FAIL_MESSAGE_E
};
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

SKY_TOKEN paIPHotMenu[4] =       /* Hot Menu!! */
{
  TKN_CALL_HOT_MENU1,          /* 자기번호발신 */
  TKN_CALL_HOT_MENU2,          /* 송화음차단   */
  TKN_CALL_HOT_MENU3,          /* 호전환서비스 */
  TKN_CALL_HOT_MENU4           /* 호보류서비스 */
};

BOOL g_fRelease;
BOOL g_fSleepOff;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
/*============================================================================= 
 Function:    
     SKA_AirDmgrSend

 Description: 
     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
enum tag_AIRDM_SEND_E {
    AS_STATICTEXT = 0,
    AS_NUMEDIT,
    AS_MAX
};

void SKY_API SKA_AirDmgrSend(SKY_EVENT_T *pEvent)
{
    static UINT8     Mode;
    STATIC BYTE      aPhoneNumber[NE_MAX_SIZE];
    HCONTROL         hDialEditBox, hControl;
    BOOL             ListHandle;
    pb_number_info_type NumberInfo;
    db_items_value_type dbi;   /* DB items */
    
    switch(pEvent->EventID)
    {
    /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        Mode = 0;
        if(SKY_GET_AIRDATAMANAGER_ORG())
        {
            SKY_SET_AIRDATAMANAGER_ORG(FALSE);
#ifndef WIN32            
            PtpLinkRelease();
#endif
        }
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_CONFIRM_FIND,
            NULL,//SKY_GET_TOKEN(TKN_AIR_DATAMGR_SEND), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(0, 0, TKN_CONNECT_MODEM_NUM);

        hDialEditBox = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                      DIAL_DIGIT_POS_Y, 
                                      DIAL_DIGIT_LENGTH_PER_LINE, 
                                      DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_RIGHT2LEFT | \
            CS_NUMBER_EDIT_NO_CML | CS_NUMBER_EDIT_TP_PAUSE|CS_NUMBER_EDIT_ALONE);
        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font
        EQS_SetIOCActive(hDialEditBox);
        break;

    case SE_IO_NEDIT_CANCEL:
    case SE_IO_NEDIT_NODIGIT:
        if(SKY_GET_AIRDATAMANAGER_ORG())
        {
            SKY_SET_AIRDATAMANAGER_ORG(FALSE);
#ifndef WIN32            
            PtpLinkRelease();
#endif
        }
        EQS_End();
        break;

    case SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(AS_NUMEDIT), aPhoneNumber);
        db_get( DB_NO_SVC, &dbi );
        if(dbi.no_svc)
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_SERVICE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }else
        {
            if(aPhoneNumber[0] != 0)
            {
                EQS_SetIOCActive((HCONTROL)pEvent->wParam);
                SKY_SET_AIRDATAMANAGER_ORG(TRUE);
                EQC_GetNumberEditValue(EQS_GetNthIOC(AS_NUMEDIT), aPhoneNumber);
                SKY_AcceptKey(FALSE);
    #ifdef  FEATURE_SKT_PIWFPTPAPI
                PtpLinkConnect(DS_ASYNC_SRVC, (char *)aPhoneNumber);
    #else   //	#ifdef	FEATURE_SKT_PIWFPTPAPI
                PtpLinkConnect((char *)aPhoneNumber);
    #endif  //	#ifdef	FEATURE_SKT_PIWFPTPAPI
            }else
            {
                EQS_SetIOCActive((HCONTROL)pEvent->wParam);
            }
        }
        break;

    case SE_SOFTKEY_FIND:
        if(pEvent->wParam == BOOKCODE_IN_DATASRV)
        {
            hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ON_SEARCHING),\
                       0,\
                       MBT_NONE, MBT_HELP);
		    EQC_SET_USERDATA(hControl, PHONE_BOOK_SEARCH_MESSAGE_E );
            break;
        }

        if(SKY_IS_PHONE_BOOK_LOCK())
        {
            EQS_START_CHILD_EVENT(SKA_MainPbookCode , SE_APP_START , BOOKCODE_IN_DATASRV , EQS_NA);
        }
        else
        {
            hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ON_SEARCHING),\
                       0,\
                       MBT_NONE, MBT_HELP);
		    EQC_SET_USERDATA(hControl, PHONE_BOOK_SEARCH_MESSAGE_E );
        }
        break; 

    case SE_IO_MESGBOX_END:	 
       if( pEvent->dwParam == PHONE_BOOK_SEARCH_MESSAGE_E )
	   { // 전화번호 검색
		    
			EQC_GetNumberEditValue(EQS_GetNthIOC(AS_NUMEDIT), aPhoneNumber);
			if( aPhoneNumber[0] != 0 )
			{
				ListHandle = FindNumberInBook(aPhoneNumber);
				if(!ListHandle)
				{
				   hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NO_SEARCH_NUMBER_MSG),\
					   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);            
				}
			}
			else
			{
				EQS_START_CHILD_EVENT( SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_SEARCH_BOOK_E);
			}

	   }else
       {
            EQS_SetIOCActive(EQS_GetNthIOC(AS_NUMEDIT));
       }
       break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
        SKY_AcceptKey(TRUE);
         if(SKY_GET_AIRDATAMANAGER_ORG())
        {
            SKY_SET_AIRDATAMANAGER_ORG(FALSE);
#ifndef WIN32            
            PtpLinkRelease();
#endif
        }else
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;

    case SE_CALL_ENDED:
      if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
      {
          EQS_DeleteControl(hControl);
      }
      EndCall();
      /* 상대방이 끊었을 경우에는 통화 종료음을 내 준다 */
      /* Voice Call인 경우만                            */
      EQS_END_TO(EQS_POP_TO_ROOT);
      break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        EQS_SetIOCActive(EQS_GetNthIOC(AS_NUMEDIT));
        if(EQS_GET_CHILD()==SKA_SelectNumber|| EQS_GET_CHILD()==SKA_SpeedSearch ||
            EQS_GET_CHILD()==SKA_ViewSearchNumber||EQS_GET_CHILD()==SKA_ViewBookList)
        {
            NumberInfo.address = pEvent->dwParam;
            if(ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) == TRUE)
            {
                STRCPY(aPhoneNumber, NumberInfo.digits);
            }
            EQC_SetNumberEditValue(EQS_GetNthIOC(AS_NUMEDIT), (BYTE*)aPhoneNumber);
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

#define AIR_DATAMANAGER_DISP_Y          20
/*============================================================================= 
 Function:    
     SKA_AirDmgrGet

 Description: 
     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
#define sLINE_HEIGHT    5

void SKY_API SKA_AirDmgrGet(SKY_EVENT_T *pEvent)
{
    UINT8       nY;
    switch(pEvent->EventID)
    {
    /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE,
            NULL,//SKY_GET_TOKEN(TKN_AIR_DATAMGR_GET), 
            WA_SHOW_HEADER);

        nY = AIR_DATAMANAGER_DISP_Y;
        EQC_StaticText((SKY_SCREEN_WIDTH-STRLEN(SKY_GET_TOKEN(TKN_WAIT_AIR_DATAMGR1))*CharWidth(' '))/2, nY, TKN_WAIT_AIR_DATAMGR1);
        nY += CharHeight(' ') + sLINE_HEIGHT;
        EQC_StaticText((SKY_SCREEN_WIDTH-STRLEN(SKY_GET_TOKEN(TKN_WAIT_AIR_DATAMGR2))*CharWidth(' '))/2, nY, TKN_WAIT_AIR_DATAMGR2);
        nY += CharHeight(' ') + sLINE_HEIGHT;
        EQC_StaticText((SKY_SCREEN_WIDTH-STRLEN(SKY_GET_TOKEN(TKN_WAIT_AIR_DATAMGR3))*CharWidth(' '))/2, nY, TKN_WAIT_AIR_DATAMGR3);

        SKY_SET_AIRDATAMANAGER_RCV(TRUE);
#ifdef	FEATURE_SKT_PIWFPTPAPI
        EnterPtpLinkMode(DS_ASYNC_SRVC);
#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
        EnterPtpLinkMode();
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
        break;

    case SE_KEY_CLEAR:
        if(SKY_GET_AIRDATAMANAGER_RCV())
        {
            SKY_SET_AIRDATAMANAGER_RCV(FALSE);
#ifndef WIN32
            PtpLinkRelease();
#endif
        }
        EQS_End();
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
        if(SKY_GET_AIRDATAMANAGER_RCV())
        {
            SKY_SET_AIRDATAMANAGER_RCV(FALSE);
#ifndef WIN32
            PtpLinkRelease();
#endif
        }
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*============================================================================= 
 Function:    
     SKA_IPDmgr

 Description: 
     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
enum tag_IP_DATAMANAGER_E {
    ID_STATICTEXT = 0,
    ID_NUMEDIT,
    ID_MAX
};

void SKY_API SKA_IPDmgr(SKY_EVENT_T *pEvent)
{
    STATIC BYTE sza_pcAddress[PC_ADDRESS_LEN+1];
    BYTE sza_TempPcAddress[PC_ADDRESS_LEN+1];
    HCONTROL        sMsgHandle = 0;
    db_items_value_type dbi;   /* DB items */
    HCONTROL    hControl;
    BYTE    bIdx = 0, i = 0;
    
    switch(pEvent->EventID)
    {
    /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_CANCEL_CONFIRM_DOT,
            NULL,//SKY_GET_TOKEN(TKN_IP_AIR_DATAMGR), 
            WA_SHOW_HEADER|WA_SHOW_CML);
        
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_SOCK_CONNECT(FALSE);

        EQC_StaticText(4, 3, TKN_CONNECT_IP_ADDR);
        EQC_NumberEdit(4, 23, 17, 2, PC_ADDRESS_LEN, sza_pcAddress);
        EQC_SET_STYLE(EQS_GetNthIOC(ID_NUMEDIT), CS_NUMBER_EDIT_BORDER|\
            CS_NUMBER_EDIT_CURSOR|CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_NO_CML|CS_NUMBER_EDIT_ALONE);
        EQS_SetIOCActive(EQS_GetNthIOC(ID_NUMEDIT));
        break;

    case SE_APP_END :   /* The very last event */ 
        g_fRelease = FALSE;
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam != sMsgHandle)
        {
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
        }
        break;

    case SE_KEY_SEND:
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(ID_NUMEDIT));
        db_get( DB_NO_SVC, &dbi );
        if(dbi.no_svc)
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_SERVICE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }else
        {
            EQC_GetNumberEditValue(EQS_GetNthIOC(ID_NUMEDIT), sza_pcAddress);
            if(sza_pcAddress[0] == 0)
            {
                SKY_SET_IP_AIRDATAMANAGER(FALSE);
                sMsgHandle
                    = EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_INPUT_IP_ADDR), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            }else
            {
                SKY_SET_IP_AIRDATAMANAGER(TRUE);
                SKY_SET_SOCK_MODE(TRUE);
            
	            if (SKY_IS_SOCK_CONNECT())
		            SvrLinkReconnect(sza_pcAddress, PORT_NUM);
	            else
		            SvrLinkConnect(sza_pcAddress, PORT_NUM);
                SKY_AcceptKey(FALSE);
            }
        }
        break;

    case SE_SOFTKEY_DOT:
        EQS_SetIOCActive(hControl);
        hControl = EQS_GetNthIOC(ID_NUMEDIT);
        bIdx = EQC_GetNumberEditPos(hControl);
        EQC_GetNumberEditValue(hControl, sza_pcAddress);
        if(!(STRLEN(sza_pcAddress) > PC_ADDRESS_LEN))
        {
            STRCPY(sza_TempPcAddress, sza_pcAddress);
            sza_TempPcAddress[PC_ADDRESS_LEN] = 0;
            sza_pcAddress[bIdx] = '.';
            sza_pcAddress[bIdx+1] = 0;
        
            for(i = bIdx; sza_TempPcAddress[i] != 0 ; i++)
                sza_pcAddress[i+1] = sza_TempPcAddress[i] ;

            sza_pcAddress[i+1] = 0;
            //STRCAT(sza_pcAddress, sza_DestPcAddress );
        //STRCAT(sza_pcAddress, ".");
            EQC_SetNumberEditValue(hControl,sza_pcAddress);
        }
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
        SKY_AcceptKey(TRUE);
        SKY_SET_IP_AIRDATAMANAGER(FALSE);
        sza_pcAddress[0] = 0;
        if(SKY_IS_SOCK_MODE())          //white 2001.03.15
        {
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            g_fRelease = TRUE;
#ifndef WIN32            
            SvrLinkRelease();
#endif
        }else
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
    case SE_IO_NEDIT_NODIGIT:
        sza_pcAddress[0] = 0;
        SKY_SET_IP_AIRDATAMANAGER(FALSE);
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_SOCK_CONNECT(FALSE);
        EQS_END();
        //EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_RECV_DATA:
	    if (SKY_GET_IP_AIRDATAMANAGER())
	    {
            BYTE data[1000];
		    //BYTE data[DSM_DS_SMALL_ITEM_SIZ];
		    WORD length=0;

		    while((length = SvrLinkRcv(data)) > 0)
		    {
			    AirDataManagerRX((BYTE *)data, (word)length);
		    }
	    } 
	    break;
        
    case SE_SOFTKEY_SELECT:
        break;

    case SE_SOCK_RELEASED:      //IP DM
        g_fRelease = FALSE;
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_SOCK_CONNECT(FALSE);
        SKY_SET_IP_AIRDATAMANAGER(FALSE);
        EQS_END();
        break;

    case SE_CALL_ENDED :
        if(SKY_IS_SOCK_MODE() && g_fRelease)        //애플릿에서 끊을때
        {
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            g_fRelease = FALSE;
            EQS_END();
        }
        else if(!SKY_IS_SOCK_MODE())            //상대가 끊을때
        {
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            g_fRelease = TRUE;
#ifndef WIN32
            SvrLinkRelease();
#endif
        }
        //EMAIL_RECV_QUIT
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API DataManagerEnd(SKY_EVENT_T *pEvent)
{
    if(SKY_GET_AIRDATAMANAGER_ORG())
    {
        SKY_SET_AIRDATAMANAGER_ORG(FALSE);
#ifndef WIN32            
        PtpLinkRelease();
#endif
    }
    if(SKY_GET_AIRDATAMANAGER_RCV())
    {
        SKY_SET_AIRDATAMANAGER_RCV(FALSE);
#ifndef WIN32
        PtpLinkRelease();
#endif
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
VOID AirDataManagerTX(BYTE* input_ptr, INT length)
{
    INT    i;

    for (i = length - 1; i >= 0; i--)
		input_ptr[i + 2] = input_ptr[i];

	length += 2;
	input_ptr[0] = '\r';
	input_ptr[1] = '\n';
	input_ptr[length++] = '\r';
	input_ptr[length++] = '\n';
	input_ptr[length]   = '\0';
    MSG_HIGH("AIR BUFFER %s",input_ptr,0,0);
#ifdef	FEATURE_IP_AIRDATA		
	if (SKY_GET_IP_AIRDATAMANAGER())
	{
		SvrLinkSend(input_ptr, (uint16)length);
		return;
	}
	else
#endif	// FEATURE_IP_AIRDATA	
	{
#ifndef WIN32
		PtpLinkSend(input_ptr, (uint16)length);
#endif
		return;
	}
}

VOID AirDataManagerRX(byte* input_ptr, word length)
{
    STATIC BYTE	    s_bState = 0;
    STATIC BYTE     s_bBuffer[256];
    STATIC WORD     s_wPos = 0;
    WORD            wPos = 0;


	while (length > 0) 
	{
		switch (s_bState) 
		{
		case 0:	// Wait A
			if (input_ptr[wPos] == 'A')
				s_bState = 1;
			break;

		case 1:	// Wait T
			if (input_ptr[wPos] == 'T')
				s_bState = 2;
			else
				s_bState = 0;
			break;

		case 2:	// Wait *
			if (input_ptr[wPos] == '*')
				s_bState = 3;
			else
				s_bState = 0;
			break;

		case 3:	// Wait S
			if (input_ptr[wPos] == 'S')
				s_bState = 4;
			else
				s_bState = 0;
			break;

		case 4:	// Wait K
			if (input_ptr[wPos] == 'K')
				s_bState = 5;
			else
				s_bState = 0;
			break;

		case 5:	// Wait T
			if (input_ptr[wPos] == 'T')
				s_bState = 6;
			else
				s_bState = 0;
			break;

		case 6:	// Wait T
			if (input_ptr[wPos] == 'T')
				s_bState = 7;
			else
				s_bState = 0;
			break;

		case 7:	// 0|1*...
			while (length > 0 && input_ptr[wPos] != '\r') {
				s_bBuffer[s_wPos++] = input_ptr[wPos++];
				length--;
			}

			if (length > 0 && input_ptr[wPos] == '\r') {
				s_bBuffer[s_wPos++] = '\0';

				DmReceiveMessage((BYTE*)s_bBuffer);
				s_bState = 0;
				s_wPos = 0;
			}

			wPos--;
			length++;
			break;
		}

		wPos++;
		length--;
	}
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_InProgIPAirDM()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define SKY_ONE_SEC             1000
#define SKY_END_TIME_MSG_BOX    5

void SKY_API SKA_InProgIPAirDM(SKY_EVENT_T *pEvent)
{
    HCONTROL hControl;
    CNI_T*  pCallerID;
    
    switch(pEvent->EventID)
    {     
    /* NC_APP */
    case SE_APP_START :
		EQS_GetDC(TRUE);
        g_fSleepOff = TRUE;
        InProgModeEntry();              
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_MENU_VOLUME_PBOOK, 
                      NULL,                  /* show annunciator */
                      WA_SHOW_HEADER|WA_SHOW_CML);   
        
        g_fRelease = FALSE;
        SKY_DrawSubAnnunciators();

        InitInProgDisplayInDataService();
        EQS_StartTimer(EQS_TIMER_1, SKY_ONE_SEC, TRUE);
        SKY_AcceptKey(TRUE); 
        MSG_HIGH("SKA_DSVC: INPROG", 0, 0, 0);
        break;

	case SE_APP_RESUME:
        g_fRelease = FALSE;
        EQS_StartTimer(EQS_TIMER_1, SKY_ONE_SEC, TRUE);
        SKY_AcceptKey(TRUE);
        if(EQS_GET_CHILD() == SKA_BookInCalling)
        {
            SKY_SET_TRANSMIT_DTMF(TRUE);
            /* restore pacemaker repeat time */
            SKY_SetPaceMakerTimer(CLOCK_PACEMAKER_REPEAT_TIME, TRUE);
        }
        else if(EQS_GET_CHILD() == SKA_MainClid)
        {
          pCallerID = SKY_GetCallerID();
          if(IsData2DisplayInCallWaitingCall())
          {
            EQS_START_CHILD(SKA_CallWaitingCall);
          }
        }
		else if(EQS_GET_CHILD() == SKA_CallForward)
		{

		}
		else if(EQS_GET_CHILD() == SKA_CallPending)
		{
		}
	
    case SE_TM_TIMER1 :
		EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        DisplayCallTime(EQS_GetNthIOC(DS_TIME));
        break;
      
    case SE_SOFTKEY_MENU:
        /* 자동잠금이 아닌 경우 */
        SKY_SET_TRANSMIT_DTMF(FALSE);    /*전화부 검색 중 DTMF 발신 방지 */
        if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
        {
            break;      //white 자동잠금 중에 접근 안됨/
        }

        if(SKY_IS_MENU_LOCK())
        {
            if(pEvent->wParam != MENUCODE_IN_CALL)
            {
                EQS_START_CHILD_EVENT(SKA_MainMenuCode, SE_APP_START, MENUCODE_IN_CALL, EQS_NA);
                break;
            }
        }

        if(!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK()))
        {
          EQC_PopUpMenu((EQS_TOKEN_EX*)paIPHotMenu, 4, 0);
        }
        break;

    case SE_SOFTKEY_PBOOK:

        /* 자동잠금> 전화부잠금 */
        if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
        {
            break;  //white 자동잠금 중에 접근 안됨/
        }

        if((!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())) && 
           (SKY_IS_PHONE_BOOK_LOCK() == FALSE))
        {
            SKY_SET_TRANSMIT_DTMF(FALSE);    /*전화부 검색 중 DTMF 발신 방지 */
            EQS_START_CHILD(SKA_BookInCalling);
            /* 
            ** jrkwon 2001.06.28
            ** 통화 중 녹음 시 외부 LCD에 통화시간이 늦게 표시되는 문제 해결 
            */
            /* make faster for display call time */
            SKY_SetPaceMakerTimer(sFAST_PACEMAKER_SECOND, TRUE);
        }
        else if((!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())) && SKY_IS_PHONE_BOOK_LOCK())
        {
          SKY_SET_TRANSMIT_DTMF(FALSE);    /*전화부 검색 중 DTMF 발신 방지 */
          EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, BOOKCODE_IN_CALL, EQS_NA);
        }
        break;

    case SE_IO_POPUP_CANCEL:
        SKY_SET_TRANSMIT_DTMF(TRUE);    
        break;
    
    case SE_IO_POPUP_SELECT :      
        SKY_SET_TRANSMIT_DTMF(TRUE);    /*전화부 검색 중 DTMF 발신 방지 */
        switch(pEvent->dwParam)
        {
         case BURSTMYNUMBER:
            EQS_START_CHILD(SKA_BurstDTMFWithMyNumber);
            break;

         case TOGGLEMUTE:
	        EQS_START_CHILD_EVENT(SKA_ToggleMute, SE_IO_POPUP_SELECT, EQS_NA, EQS_NA);
            break;

         case CALLFORWARD:
            EQS_START_CHILD(SKA_CallForward);
            break;

         case CALLWAITING:
            EQS_START_CHILD(SKA_CallPending);
            break;
        }       
       break;  


    case SE_KEY_UP:
    case SE_KEY_DOWN:
         EQS_START_CHILD(SKA_AdjstConversationVol);
        break;


    case SE_APP_END :   /* The very last event */ 
        g_fRelease = FALSE;
        SKY_SET_IP_AIRDATAMANAGER(FALSE);
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_SOCK_CONNECT(FALSE);
        EndCall();
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
		EQS_ReleaseDC();
        break;

    case SE_TCPCONNECT_SUCCESS:
#ifdef	FEATURE_IP_AIRDATA	
        if (SKY_GET_IP_AIRDATAMANAGER())
        {
            SKY_SET_SOCK_CONNECT(TRUE);
        }
#endif	// #ifdef	FEATURE_IP_AIRDATA
        break;

	case SE_TCPCONNECT_FAIL:
        //DataManagerEnd(pEvent);
        EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_RETRY_FAIL), MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
        if(SKY_IS_SOCK_MODE())
        {
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            EQS_END();
        }
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_IO_TO_IDLE :
    case SE_KEY_END :
        if(SKY_IS_SOCK_MODE())
        {
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            g_fRelease = TRUE;
#ifndef WIN32
            SvrLinkRelease();
#endif
        }
        break;

    case SE_SOCK_RELEASED:      //IP DM
        g_fRelease = FALSE;
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_SOCK_CONNECT(FALSE);
        SKY_SET_IP_AIRDATAMANAGER(FALSE);
        /* IP Air Data Manager 종료 후  종료 화면을 보여주기 위해 */
        EQS_END();
        break;

    case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
      // not supposed to be here, CM will end the call first
        ASSERT(FALSE);
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        if(SKY_GET_IP_AIRDATAMANAGER())
            SKY_AcceptKey(TRUE); 
    case SE_CALL_CONNECT :   //UI_CALL_CONNECT_E:
        ASSERT(FALSE);
        break;

	case SE_KEY_CLEAR:
        break;


    case SE_CALL_SIGNAL:    
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* SE_SOCK_RELEASED 나오지 않는 현상 방지                            */      
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }

        if(g_fRelease)        //애플릿에서 끊을때
        {
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            g_fRelease = FALSE;
            EQS_END();
        }
        else                                        //상대가 끊을때
        {
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            g_fRelease = TRUE;
#ifndef WIN32
            SvrLinkRelease();
#endif
        }
        hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_EMAIL_RECV_QUIT), SKY_END_TIME_MSG_BOX, MBT_NONE,MBT_HELP);
        EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |CS_MESSAGE_BOX_IGNORE_KEYS|CS_MESSAGE_BOX_DEFAULT);
        break;

    case SE_KEY_1:
        break;
        
    case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
        break;

    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
        break;

    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}