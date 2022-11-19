/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_irph.c
**
** Description: 
**     Phone to Phone IrDA
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-04 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define	IrDA_LAP_CHECK_TIMER_INTERVAL		100
#define	IrDA_SEARCH_TIMER_INTERVAL			3000

#define	LAP_CHECK_TIMER						1
#define SEARCH_TIMER						2

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
enum tag_PBOOK_OVERWRITE_E {
    POW_YES,
    POW_NO,
    POW_YESALL,
    POW_NOALL
};

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
extern short  MDM_SearchIrDADevice(void);

HCONTROL    ghMsgLine1, ghMsgLine2, ghState;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL   BOOL  fConnect;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL HCONTROL sUpdateIrDALinkIcon(byte bConnect, byte bDisp);
LOCAL void SKY_API sSKA_IrDAChangeFileName(SKY_EVENT_T *pEvent);

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


/*============================================================================= 
Function:    
SKA_MainXxxx

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
void SKY_API SKA_MainIrDAPhoneReceive(SKY_EVENT_T *pEvent)
{
    static HCONTROL hPopControl;
    static UINT8  nCount = 0, nTimerType = 0;
    static IREX_RESULT_E ClientResultCode;
    UINT8   nRet = 0;
    /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpReady;
    static BITMAP   bmpReceive;
    static BOOL     fReady;
    /* <-- */

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_RECEIVE), 
                      WA_SHOW_HEADER);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);
		if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            g_fIrDAPortForPC = FALSE;
		}

        SKY_OpenIrDAPort(1);	// Set to primary

        /* rosa 01-08-17 --> @why, what */
        
        // set the images of ready & receive
        bmpReceive = g_BmpExchangeDataIrDA;
        bmpReceive.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpReceive.Header) * IB_RECEIVE; //1

        bmpReady = g_BmpExchangeDataIrDA;
        bmpReady.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpReady.Header) * IB_READY; //0

        // set pannel image
        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);

        // set message lines
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_WAIT);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_IRDA_MSG_RCV_WAITING);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_BLANK);

        fReady = TRUE;
        /* <-- */

        gIrDACommType = IRT_READY;

        nTimerType = SEARCH_TIMER;
        EQS_StartTimer(EQS_TIMER_3, IrDA_SEARCH_TIMER_INTERVAL/3, FALSE);

        nCount = 0;
        gfPhoneBookOverwriteAll = FALSE;
        gfPhoneBookSkipAll = FALSE;

        fConnect = FALSE;
        gSendFile = gRecvFile = NULL;

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

        break;

    case SE_APP_END :   /* The very last event */
		if(!fIrDAPortClosed)
		{
			// 수신 모드로 들어간후 전화를 못받는 문제를 해결하기 위해
			if(SKY_IS_ALERT())
			{
				IrDA_Off = TRUE;
				SKY_IrDAStatusCheck_set (500);
			}
			else
				SKY_CloseIrDAPort();
		}  
        
        PhoneBookUpdate();
        SKY_IrDACloseFile();

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;        

    case SE_APP_DRAW:
        /* rosa 2001-09-20 --> @why, what */
        //EQS_PutBitmap(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        if(fConnect)
            //EQS_PutBitmap(IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&g_BmpIrph3_85x52x256c);
            EQS_PutBitmap(IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpReceive);
        else
            //EQS_PutBitmap(IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&g_BmpIrph1_85x52x256c);
            EQS_PutBitmap(IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpReady);
        break;
        /* <-- */

    case SE_TM_TIMER1:
    case SE_TM_TIMER2:
        SKY_IrDAClientStateMachine(TIMEOUT_E);
        break;

    case SE_TM_TIMER3:  
        if(EQS_GetModal() != INVALID_CONTROL_HANDLE)    // PopUp or Message Box
        {
            EQS_StartTimer(EQS_TIMER_3, IrDA_LAP_CHECK_TIMER_INTERVAL, FALSE);
            break;
        }

        if(gIrDACommType == IRT_READY)
        {
            if(!fReady)
            {
                EQC_SetStaticText(ghMsgLine1, 
                                  (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_WAITING));
                EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_BLANK));
                fReady = TRUE;
            }
        }
        else
        {
            fReady = FALSE;
        }

        switch(nTimerType){
        case LAP_CHECK_TIMER:
/*
            if(nCount >= IrDA_SEARCH_TIMER_INTERVAL/IrDA_LAP_CHECK_TIMER_INTERVAL
                && gIrDACommType == IRT_READY)
            {
                EQC_SetStaticText(ghMsgLine1, 
                                  (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_WAITING));
                EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_BLANK));

                nCount = 0;
            }

            nCount ++;
*/
			nRet = SKY_CheckLAPConnection();
			sUpdateIrDALinkIcon(nRet, 1);

			if(nRet)
			{
				nTimerType = LAP_CHECK_TIMER;
                EQS_StartTimer(EQS_TIMER_3, IrDA_LAP_CHECK_TIMER_INTERVAL, FALSE);
			}
			else
			{
				nTimerType = SEARCH_TIMER;
                EQS_StartTimer(EQS_TIMER_3, IrDA_SEARCH_TIMER_INTERVAL, FALSE);
			}
            break;

		case SEARCH_TIMER:
			nRet = SKY_CheckLAPConnection();
			sUpdateIrDALinkIcon(nRet, 1);

			if(nRet)
			{
				nTimerType = LAP_CHECK_TIMER;
                EQS_StartTimer(EQS_TIMER_3, IrDA_LAP_CHECK_TIMER_INTERVAL, FALSE);
				break;
			}

			nRet = MDM_SearchIrDADevice();
			if(nRet)
			{
				nTimerType = SEARCH_TIMER;
                EQS_StartTimer(EQS_TIMER_3, IrDA_SEARCH_TIMER_INTERVAL, FALSE);
			}
			else
			{
				nTimerType = LAP_CHECK_TIMER;
                EQS_StartTimer(EQS_TIMER_3, IrDA_LAP_CHECK_TIMER_INTERVAL, FALSE);
			}
			break;
        }
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam){
        case POW_YESALL:
            gfPhoneBookOverwriteAll = TRUE;
            gfPhoneBookSkipAll = FALSE;
            // go through..
        case POW_YES:
            if(gIrDACommType == IRT_PHONEBOOK)
            {
                ClientResultCode = SKY_ProcessPhoneBookMsg(TRUE);
		        gIrex_TxMsg.MsgType = IREX_PHONEBOOK_C;
            }
            else if(gIrDACommType == IRT_PHONEBOOK_SDIAL)
            {
                ClientResultCode = SKY_ProcessSDialMsg(TRUE);
		        gIrex_TxMsg.MsgType = IREX_PHONEBOOK_SDIAL_C;
            }
		    gIrex_TxMsg.Msg.PhoneBook_c.ResultCode = ClientResultCode;
            SKY_IrDAClientStateMachine(SEND_MSG_E);
            break;

        case POW_NOALL:
            gfPhoneBookOverwriteAll = FALSE;
            gfPhoneBookSkipAll = TRUE;
            // go through..
        case POW_NO:
            if(gIrDACommType == IRT_PHONEBOOK)
		        gIrex_TxMsg.MsgType = IREX_PHONEBOOK_C;
            else if(gIrDACommType == IRT_PHONEBOOK_SDIAL)
                gIrex_TxMsg.MsgType = IREX_PHONEBOOK_SDIAL_C;

		    gIrex_TxMsg.Msg.PhoneBook_c.ResultCode = IREX_CONTINUE_PHONEBOOK_RES;
		    SKY_IrDAClientStateMachine(SEND_MSG_E);
            break;
        }
        EQS_StartTimer(EQS_TIMER_3, IrDA_SEARCH_TIMER_INTERVAL/3, FALSE);
        break;

    case SE_IO_POPUP_CANCEL:
        if(gIrDACommType == IRT_PHONEBOOK)
		    gIrex_TxMsg.MsgType = IREX_PHONEBOOK_C;
        else if(gIrDACommType == IRT_PHONEBOOK_SDIAL)
            gIrex_TxMsg.MsgType = IREX_PHONEBOOK_SDIAL_C;

		gIrex_TxMsg.Msg.PhoneBook_c.ResultCode = IREX_CONTINUE_PHONEBOOK_RES;
		SKY_IrDAClientStateMachine(SEND_MSG_E);
        EQS_StartTimer(EQS_TIMER_3, IrDA_SEARCH_TIMER_INTERVAL/3, FALSE);
        break;

    case SE_IO_MESGBOX_END:
        EQC_SetStaticText(ghState, (BYTE *)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_STATE_WAIT));
        EQC_SetStaticText(ghMsgLine1, (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_WAITING));
        EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_BLANK));
        gIrDACommType = IRT_READY;
        EQS_StartTimer(EQS_TIMER_3, IrDA_SEARCH_TIMER_INTERVAL/3, FALSE);
        break;

    case SE_APP_RESUME: /* child applet is ended */
		nRet = SKY_CheckLAPConnection();
		sUpdateIrDALinkIcon(nRet, 1);

		if(pEvent->wParam == 0)
        {
			SKY_IrDASendESCtoServer();	// Next
            break;
        }
        else
        {
            ClientResultCode = SKY_ProcessFileTransferMsg();
        }
        // Go through..
    case SE_IRDA_EX_RX_DATA_F:
        if(pEvent->EventID == SE_IRDA_EX_RX_DATA_F)
        {
            SKY_SNI(HS_SNI_TRANS_IRDA, TRUE);
      		ClientResultCode = SKY_IrDAClientStateMachine(GET_MSG_E);
        }
        
		switch(ClientResultCode)
		{
        case IREX_FINISHED_RES:
            switch(gIrDACommType){
            case IRT_PHONEBOOK:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_END_PBOOK), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            case IRT_PHONEBOOK_SDIAL:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_END_SDIAL), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            case IRT_IMAGE:
            case IRT_MELODY:
            case IRT_PHOTO:
                // Screen was updated in ska_ftrs.c
                break;
            }
            break;

        case IREX_INVAILD_MELODY_RES:
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_NOT_SUPPORT_BELL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;

        case IREX_INVAILD_ANI_RES:
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_NOT_SUPPORT_ANI), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;

        case IREX_ERR_FILE_VER:
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_NOT_SUPPORT_FILE), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;

        case IREX_NO_MEM_AVAIL:
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_NOMEMORY), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;

        case IREX_ERR_FULE_FILE:
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_FULL_FILE), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;

        case IREX_GROUP_CREATE_FAILURE:
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_FULL_GROUP), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;
        
        case IREX_ITEM_CREATE_FAILURE:
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_NO_PBOOKMEMORY), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;

        case IREX_WAIT_FOR_USER_RES:
            EQS_StopTimer(EQS_TIMER_1);
            EQS_StopTimer(EQS_TIMER_2);
            EQS_START_CHILD(sSKA_IrDAChangeFileName);
            break;

        case IREX_ITEM_SAME_NAME:
        {
            SKY_TOKEN paMenu[4] =
            {
                TKN_IRDA_MSG_RCV_PBOOK_YES,
                TKN_IRDA_MSG_RCV_PBOOK_NO,
                TKN_IRDA_MSG_RCV_PBOOK_YESALL,
                TKN_IRDA_MSG_RCV_PBOOK_NOALL
            };

            EQS_StopTimer(EQS_TIMER_3);

            if(gIrDACommType == IRT_PHONEBOOK || gIrDACommType == IRT_PHONEBOOK_SDIAL)
                hPopControl = EQC_PopUpMenuExtend((EQS_TOKEN_EX *)paMenu, 4, 0,
                                        CS_POPUP_DEFAULT|CS_POPUP_STATUS,
                                        TKN_IRDA_MSG_RCV_PBOOK_EXIST, NULL,
                                        g_Letters);
            else
                hPopControl = EQC_PopUpMenuExtend((EQS_TOKEN_EX *)paMenu, 4, 0,
                                        CS_POPUP_DEFAULT|CS_POPUP_STATUS,
                                        TKN_IRDA_MSG_RCV_PBOOK_EXIST, NULL,
                                        g_ReceiveFileName);
            break;
        }

        case IREX_ITEM_NOT_EXIST:
        {
            SKY_TOKEN paMenu[4] =
            {
                TKN_IRDA_MSG_RCV_PBOOK_YES,
                TKN_IRDA_MSG_RCV_PBOOK_NO,
                TKN_IRDA_MSG_RCV_PBOOK_YESALL,
                TKN_IRDA_MSG_RCV_PBOOK_NOALL
            };

            EQS_StopTimer(EQS_TIMER_3);

            if(gIrDACommType == IRT_PHONEBOOK || gIrDACommType == IRT_PHONEBOOK_SDIAL)
                hPopControl = EQC_PopUpMenuExtend((EQS_TOKEN_EX *)paMenu, 4, 0,
                                        CS_POPUP_DEFAULT|CS_POPUP_STATUS,
                                        TKN_IRDA_MSG_RCV_PBOOK_NOTEXIST, NULL,
                                        g_Letters);
            else
                hPopControl = EQC_PopUpMenuExtend((EQS_TOKEN_EX *)paMenu, 4, 0,
                                        CS_POPUP_DEFAULT|CS_POPUP_STATUS,
                                        TKN_IRDA_MSG_RCV_PBOOK_NOTEXIST, NULL,
                                        g_ReceiveFileName);
            break;
        }

        default:
			break;
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/*============================================================================= 
 Function:    
     sSKA_Xxx

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
LOCAL void SKY_API sSKA_IrDAChangeFileName(SKY_EVENT_T *pEvent)
{
    static HCONTROL hControl;
    static BYTE szChangedName[SKY_FILE_EXT_NAME_MAX_LENGTH];
    static BOOL fChanged;
    BOOL    fExist;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_IRDA_RCV_CHANGE_NAME), 
                      WA_SHOW_HEADER);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_IRDA_RCV_CHANGE_NAME);
        hControl = EQC_TextEdit(EQS_NA, EQS_NA, SKY_FILE_NAME_MAX_LENGTH, 1, 
                     TEM_KOREAN, SKY_FILE_NAME_MAX_LENGTH,
                     g_ReceiveFileName, 0);

        EQS_AutoFormatting(2, EHA_CENTER, EVA_CENTER);

        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_CHANGE_NAME), 
                            1, MBT_NONE, MBT_HELP);

        fChanged = FALSE;
        break;

    case SE_APP_END :   /* The very last event */ 
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
        break;

    case SE_IO_TEDIT_CANCEL:
        EQS_END_EVENT( SE_APP_RESUME, (WORD)fChanged, 0);
        break;

    case SE_IO_TEDIT_DONE:
        if(pEvent->dwParam == 0)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_CHANGE_NAME_ERR), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;
        }

        EQC_GetTextEditData(hControl, szChangedName);

        if(!STRCMP(szChangedName, (void *)gIrex_RxMsg.Msg.FileTrans_s.Body.Inf.FileName))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_CHANGE_NAME), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;
        }

        fExist = FALSE;

        switch(gIrDACommType){
        case IRT_MELODY:
            if(SKY_IsDataFile(SDF_MELODY, szChangedName))
                fExist = TRUE;
            break;

        case IRT_IMAGE:
            if(SKY_IsDataFile(SDF_IMAGE, szChangedName))
                fExist = TRUE;
            break;
        }
        
        if(fExist)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_CHANGE_NAME), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;
        }

		STRCPY((void *)gIrex_RxMsg.Msg.FileTrans_s.Body.Inf.FileName,
			   szChangedName);

        fChanged = TRUE;
        EQS_END_EVENT( SE_APP_RESUME, (WORD)fChanged, 0);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

HCONTROL sUpdateIrDALinkIcon(byte bConnect, byte bDisp)
{
	static byte	PrevIcon = 0;

	if(!bDisp)
	{
		PrevIcon = 0;
	}
	else
	{
		if(bConnect)
		{
            fConnect = TRUE;

			if(PrevIcon != 2)
			{
				PrevIcon = 2;
                EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
			}
		}
		else
		{
            fConnect = FALSE;

			if(PrevIcon != 1)
			{
				PrevIcon = 1;
                EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
			}
		}
	}
}
