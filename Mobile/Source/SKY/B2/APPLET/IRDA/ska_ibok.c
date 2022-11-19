/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_ibok.c
**
** Description: 
**     Phone to Phone IrDA (PhoneBook)
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
#define	NO_GROUP_NAME_INDEX		100
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

enum tag_SIB_MESSAGE_REASON_E {
    SMR_NONE,
    SMR_CHILD,
    SMR_END
};

typedef enum tag_SIB_MESSAGE_REASON_E SIB_MESSAGE_REASON_E;

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
extern HCONTROL    ghMsgLine1, ghMsgLine2, ghState;
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

LOCAL IREX_RESULT_E SKY_API sProcessOldPhoneBookMsg(BOOL fOverWrite);
LOCAL IREX_RESULT_E SKY_API sProcessEQUSPhoneBookMsg(BOOL fOverWrite);

LOCAL IREX_RESULT_E SKY_API sProcessOldSDialMsg(BOOL fOverWrite);
LOCAL IREX_RESULT_E SKY_API sProcessEQUSSDialMsg(BOOL fOverWrite);

LOCAL INT16 SKY_API sGetNextItemInAll(BOOL fInitial);
LOCAL INT16 SKY_API sGetNextItemInGroup(BOOL fInitial, INT8 nHandle);
LOCAL INT8 sGetNextSDialItem(BOOL fInitial, TelInfo *pTelInfo);

LOCAL void SKY_API sMakePhoneBookMsg(INT16 nAddress, IREX_MSG_TYPE_T *pTxMsg);
LOCAL void SKY_API sMakeC5PhoneBookMsg(INT16 nAddress, IREX_MSG_TYPE_T *pTxMsg);
LOCAL void SKY_API sMakeA7PhoneBookMsg(INT16 nAddress, IREX_MSG_TYPE_T *pTxMsg);
LOCAL void SKY_API sMakeEQUSPhoneBookMsg(INT16 nAddress, 
                                         IREX_PHONEBOOK_S_TYPE_T *pPhoneBook_s);
LOCAL void sMakeMyInfoMsg(IREX_MSG_TYPE_T *pTxMsg);

LOCAL IREX_RESULT_E SKY_API sSaveEQUSBook(IREX_PHONEBOOK_S_TYPE_T *pDialData);

LOCAL void sMakePhoneBookSDialMsg(INT8 nAddress, TelInfo* SDial, 
                                  IREX_MSG_TYPE_T *pTxMsg);
LOCAL void sMakeA7PhoneBookSDialMsg(INT8 nAddress, TelInfo* SDial, 
                                    IREX_MSG_TYPE_T *pTxMsg);
LOCAL void sMakeEQUSPhoneBookSDialMsg(INT8 nAddress, TelInfo* SDial, 
                                      IREX_MSG_TYPE_T *pTxMsg);

LOCAL void SKY_API sSKA_GroupPassword(SKY_EVENT_T *pEvent);
/*============================================================================= 
Function:    
    SKA_MainIrDABookAll

Description: 
    전체 전화부 전송
  
Input:
    @ArgumentsList

Output:
    @TheResultOfThisFunction

Return value:
    @ReturnValue

Side effects:
===============================================================================*/
void SKY_API SKA_MainIrDABookAll(SKY_EVENT_T *pEvent)
{
    static SIB_MESSAGE_REASON_E MessageReason;
    WORD  ResultCode;
    IREX_STATE_E   Ret;
    INT16   nAddress;
    static BOOL fEndBook;
    /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpSend;
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
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_BOOK), 
                      WA_SHOW_HEADER);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        MessageReason = SMR_NONE;

        if(sGetNextItemInAll(TRUE) == -1)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NOBOOK), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            MessageReason = SMR_END;
        }

		if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            g_fIrDAPortForPC = FALSE;
		}

        SKY_OpenIrDAPort(0);	// Set to secondary
        gSendFile = gRecvFile = NULL;     
        fEndBook = FALSE;

        /* rosa 01-08-17 --> @why, what */
        // Message Line
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_BOOK);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_IRDA_MSG_PBOOK_SEND_ALL);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_BLANK);

        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);

        bmpSend = g_BmpExchangeDataIrDA;
        bmpSend.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpSend.Header) * IB_SEND; //2

        EQC_Image( IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpSend);
        /* <-- */

        SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);

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
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        if(MessageReason == SMR_END)
            EQS_End();
        break;

    case SE_TM_TIMER1:
    case SE_TM_TIMER2:      // ping response
        SKY_IrDAServerStateMachine(TIMEOUT_E, 0, &ResultCode);
        break;
        
    case SE_IRDA_EX_RX_DATA_F:
		Ret = SKY_IrDAServerStateMachine(GET_MSG_E, 0, &ResultCode);
		
		if(Ret == IREX_PING_FINISHED)
		{
            nAddress = sGetNextItemInAll(TRUE);
			if( nAddress == -1) 
			{
				gIrex_TxMsg.MsgType = IREX_FINISHED_S;
				gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
				SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
			}
            else
            {
			    sMakePhoneBookMsg(nAddress, &gIrex_TxMsg);
			    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
			    Ret = IREX_ON_GOING;
            }
			break;
		}
		else if(Ret == IREX_SUCCESS)
		{
			switch(ResultCode)
			{
			case IREX_GROUP_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FULL_GROUP), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;

				Ret = IREX_ON_GOING;
				break;

			case IREX_ITEM_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_MEMORY), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;

				Ret = IREX_ON_GOING;
				break;
			
			case IREX_ITEM_SAME_NAME:
				SKY_IrDAServerStateMachine(WAIT_RESPONSE_E, 0, &ResultCode);
				Ret = IREX_ON_GOING;
				break;

			case IREX_CONTINUE_PHONEBOOK_RES:
			case IREX_OK_RES:
                nAddress = sGetNextItemInAll(FALSE);
				if(nAddress == -1) 
				{
                    if(!fEndBook)
                    {
					    gIrex_TxMsg.MsgType = IREX_FINISHED_S;
					    gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
					    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
                        if(gIrex_ModelIndex == IREX_MODEL_B1)
                        {
                            fEndBook = TRUE;
                            Ret = IREX_ON_GOING;
                        }
                        /* rosa 2001-09-19 --> @why, what */
                        else if(gIrex_ModelIndex == IREX_MODEL_B2)
                        {
                            fEndBook = TRUE;
                            Ret = IREX_ON_GOING;
                        }
                        /* <-- */
                    }
				}
                else
                {
					sMakePhoneBookMsg(nAddress, &gIrex_TxMsg);
					SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
					Ret = IREX_ON_GOING;
                }
				break;
			}
		}

		if(Ret == IREX_SUCCESS)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_END_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
		else if(Ret == IREX_FAILURE)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FAIL_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
		break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/*============================================================================= 
Function:    
    SKA_MainIrDABookGroup

Description: 
    그룹별 전화부 전송
  
Input:
    @ArgumentsList

Output:
    @TheResultOfThisFunction

Return value:
    @ReturnValue

Side effects:
===============================================================================*/
void SKY_API SKA_MainIrDABookGroup(SKY_EVENT_T *pEvent)
{
    static BOOL fSending;
//    static BYTE *ppGroupList[MAX_GROUP_NUMBER];  // 전화부그룹
    static INT16   nGroupIndex;
    static UINT8   nListHandle;
    static SIB_MESSAGE_REASON_E MessageReason;
    static BOOL fEndBook;

    WORD  ResultCode;
    IREX_STATE_E   Ret;
    pb_group_info_type  GroupInfo;
    INT16   nAddress;
    /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpSend;
    /* <-- */

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_BOOK), 
                      WA_SHOW_HEADER);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        MessageReason = SMR_NONE;

        if(!VALID_GROUP_NUMBER)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NOBOOK), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);

            MessageReason = SMR_END;
            break;
        }

        /* rosa 01-08-17 --> @why, what */
        // Message Line
        fEndBook = FALSE;
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_BOOK);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_IRDA_MSG_PBOOK_SEND_GROUP);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_IRDA_MSG_PBOOK_SENDING);

        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);        

        bmpSend = g_BmpExchangeDataIrDA;
        bmpSend.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpSend.Header) * IB_SEND; //2

        EQC_Image( IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpSend);
        /* <-- */    

        EQS_START_CHILD(SKA_ViewIRDAGroup);    // 그룹리스트
        fSending = FALSE;

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;

        /* rosa 2001-09-21 --> @why, what */
    //case SE_APP_DRAW :
        //EQS_PutBitmap(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        //break;
        /* <-- */
        
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
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(pEvent->dwParam > 0)
        {
		    if(!fIrDAPortClosed)
		    {
			    ChangeIrDAMode ( 0 );
			    // Phone to PC IrDA가 off 되었음을 표시
			    fIrDAPortClosed = TRUE;
                g_fIrDAPortForPC = FALSE;
		    }

            SKY_OpenIrDAPort(0);	// Set to secondary
            gSendFile = gRecvFile = NULL;

//            if(!fSecret)
            nGroupIndex = pEvent->dwParam;

            GroupInfo.address = nGroupIndex;
            ReadGroup(&GroupInfo);
/*
            if(!fSecret && IS_SECRET_GROUP(nGroupIndex))
            {
                // Require Password
                EQS_START_CHILD(sSKA_GroupPassword);  
                fSecret = TRUE;
                break;
            }

            fSecret = FALSE;
*/
            nListHandle = GetBookList(nGroupIndex);
             
            if(sGetNextItemInGroup(TRUE, nListHandle) < 0)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NOBOOK), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                MessageReason = SMR_CHILD;
                break;
            }

            EQC_SetStaticText(ghMsgLine2, GroupInfo.group_name);
            // Send Item
            SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);
            fSending = TRUE;
        }
        else
        {
/*
            if(fSecret)
            {
                fSecret = FALSE;
                EQS_START_CHILD(SKA_ViewIRDAGroup);
            }
            else
*/
                EQS_End();
        }
        break;

    case SE_SOFTKEY_CANCEL:
    case SE_KEY_CLEAR:
		if(!fIrDAPortClosed)
            SKY_CloseIrDAPort();

        EQS_START_CHILD(SKA_ViewIRDAGroup);    // 그룹리스트
        break;

    case SE_IO_MESGBOX_END:
        switch(MessageReason){
        case SMR_END:
            EQS_End();
            break;
        case SMR_CHILD:
            EQS_START_CHILD(SKA_ViewIRDAGroup);
            break;
        }
        break;

    case SE_TM_TIMER1:
    case SE_TM_TIMER2:      // ping response
        if(fSending)
            SKY_IrDAServerStateMachine(TIMEOUT_E, 0, &ResultCode);
        break;

    case SE_IRDA_EX_RX_DATA_F:
        if(!fSending)   break;

		Ret = SKY_IrDAServerStateMachine(GET_MSG_E, 0, &ResultCode);

		if(Ret == IREX_PING_FINISHED)
		{
            nAddress = sGetNextItemInGroup(TRUE, nListHandle);
			if( nAddress == -1) 
			{
				gIrex_TxMsg.MsgType = IREX_FINISHED_S;
				gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
				SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
			}
            else
            {
			    sMakePhoneBookMsg(nAddress, &gIrex_TxMsg);
			    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
			    Ret = IREX_ON_GOING;
            }
			break;
		}
		else if(Ret == IREX_SUCCESS)
		{
			switch(ResultCode)
			{
			case IREX_GROUP_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FULL_GROUP), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;

				Ret = IREX_ON_GOING;
				break;

			case IREX_ITEM_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_MEMORY), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;

				Ret = IREX_ON_GOING;
				break;
			
			case IREX_ITEM_SAME_NAME:
				SKY_IrDAServerStateMachine(WAIT_RESPONSE_E, 0, &ResultCode);
				Ret = IREX_ON_GOING;
				break;

			case IREX_CONTINUE_PHONEBOOK_RES:
			case IREX_OK_RES:
                nAddress = sGetNextItemInGroup(FALSE, nListHandle);
				if(nAddress == -1) 
				{
                    if(!fEndBook)
                    {
					    gIrex_TxMsg.MsgType = IREX_FINISHED_S;
					    gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
					    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
                        if(gIrex_ModelIndex == IREX_MODEL_B1)
                        {
                            fEndBook = TRUE;
                            Ret = IREX_ON_GOING;
                        }
                        /* rosa 2001-09-19 --> @why, what */
                        else if(gIrex_ModelIndex == IREX_MODEL_B2)
                        {
                            fEndBook = TRUE;
                            Ret = IREX_ON_GOING;
                        }
                        /* <-- */
                    }
				}
                else
                {
					sMakePhoneBookMsg(nAddress, &gIrex_TxMsg);
					SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
					Ret = IREX_ON_GOING;
                }
				break;
			}
		}
		if(Ret == IREX_SUCCESS)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_END_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
		else if(Ret == IREX_FAILURE)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FAIL_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Xxx()
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
LOCAL void SKY_API sSKA_GroupPassword(SKY_EVENT_T *pEvent)
{
    static HCONTROL sNumHandle;
    BYTE   szPasswd[5];
    static BOOL fStart = TRUE;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_SECRET_GROUP), 
            WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_INPUT_PASSWORD), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        fStart = TRUE;
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
  
    case SE_IO_MESGBOX_END:
        if(fStart)
        {
            EQC_StaticText(37, 25, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(45, 40, 4, 1, 4,(BYTE*)"");
    
            EQC_SET_STYLE(EQS_GetNthIOC(1), CS_NUMBER_EDIT_PASSWD | \
                    CS_NUMBER_EDIT_PLAIN_NUM |CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_NO_CML);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
            fStart = FALSE;
        }
        EQC_SetNumberEditValue(EQS_GetNthIOC(1), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(1));
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_END_EVENT(SE_APP_RESUME, 0, EQS_NA);
        break;
    
    case SE_IO_NEDIT_MATCH_LENGTH:                 //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, szPasswd);
        if(!STRCMP(szPasswd, SKY_GET_PASSWORD()))
           EQS_END_EVENT(SE_APP_RESUME, 1, EQS_NA);
        else
           EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), 
           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*============================================================================= 
Function:    
    SKA_MainIrDABookName

Description: 
    이름별 전화부 전송
  
Input:
    @ArgumentsList

Output:
    @TheResultOfThisFunction

Return value:
    @ReturnValue

Side effects:
===============================================================================*/
void SKY_API SKA_MainIrDABookName(SKY_EVENT_T *pEvent)
{
    static INT16   nAddress, nGroupIndex;
    static BOOL    fItemSelect, fSending;
    static SIB_MESSAGE_REASON_E MessageReason;
    WORD  ResultCode;
    IREX_STATE_E   Ret;
    pb_book_info_type BookInfo;
    static BOOL fEndBook;
     /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpSend;
    /* <-- */
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_BOOK), 
                      WA_SHOW_HEADER);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        MessageReason = SMR_NONE;

        if(!VALID_BOOK_NUMBER)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NOBOOK), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            MessageReason = SMR_END;
            break;
        }

        /* rosa 01-08-17 --> @why, what */
        // Message Line
        fEndBook = FALSE;
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_BOOK);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_IRDA_MSG_PBOOK_SEND_NAME);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_IRDA_MSG_PBOOK_SENDING);

        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);        

        bmpSend = g_BmpExchangeDataIrDA;
        bmpSend.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpSend.Header) * IB_SEND; //2

        EQC_Image( IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpSend);
        /* <-- */        

        fItemSelect = FALSE;
        fSending = FALSE;
        EQS_START_CHILD(SKA_ViewIRDAGroup);    // 그룹리스트
        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
        /* rosa 2001-09-21 --> @why, what */
    //case SE_APP_DRAW:
        //EQS_PutBitmap(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        //break;
        /* <-- */

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
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_SOFTKEY_CANCEL:
    case SE_KEY_CLEAR:
		if(!fIrDAPortClosed)
            SKY_CloseIrDAPort();

        EQS_START_CHILD_EVENT(SKA_ViewIRDAList, SE_APP_START, nGroupIndex, 0);  
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(fItemSelect)
        {
            if(pEvent->dwParam > 0)
            {
		        if(!fIrDAPortClosed)
		        {
			        ChangeIrDAMode ( 0 );
			        // Phone to PC IrDA가 off 되었음을 표시
			        fIrDAPortClosed = TRUE;
                    g_fIrDAPortForPC = FALSE;
		        }

                SKY_OpenIrDAPort(0);	// Set to secondary
                gSendFile = gRecvFile = NULL;
                fSending = TRUE;

                nAddress = pEvent->dwParam;
                BookInfo.address = nAddress;
                ReadBook(&BookInfo);

                EQC_SetStaticText(ghMsgLine2, BookInfo.name);
                // Send Item
                SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);
            }
            else
            {
                fItemSelect = FALSE;
                EQS_START_CHILD(SKA_ViewIRDAGroup);    // 그룹리스트
            }
        }
        else
        {
            if(pEvent->dwParam > 0)
            {
                //if(!fSecret)
                    nGroupIndex = pEvent->dwParam;
/*
                if(!fSecret && IS_SECRET_GROUP(nGroupIndex))
                {
                    // Require Password
                    EQS_START_CHILD(sSKA_GroupPassword);  
                    fSecret = TRUE;
                    break;
                }

                fSecret = FALSE;
*/
                fItemSelect = TRUE;
                EQS_START_CHILD_EVENT(SKA_ViewIRDAList, SE_APP_START, nGroupIndex, 0);  
            }
            else
            {
/*
                if(fSecret)
                {
                    fSecret = FALSE;
                    EQS_START_CHILD(SKA_ViewIRDAGroup);
                }
                else
*/
                    EQS_End();
            }
        }
        break;
        
    case SE_IRDA_EX_RX_DATA_F:
        if(!fSending)   break;

		Ret = SKY_IrDAServerStateMachine(GET_MSG_E, 0, &ResultCode);

		if(Ret == IREX_PING_FINISHED)
		{
			    sMakePhoneBookMsg(nAddress, &gIrex_TxMsg);
			    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
				Ret = IREX_ON_GOING;
        }
		else if(Ret == IREX_SUCCESS)
		{
			switch(ResultCode)
			{
			case IREX_GROUP_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FULL_GROUP), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;
				Ret = IREX_ON_GOING;
				break;

			case IREX_ITEM_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_MEMORY), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;
				Ret = IREX_ON_GOING;
				break;
			
			case IREX_ITEM_SAME_NAME:
				SKY_IrDAServerStateMachine(WAIT_RESPONSE_E, 0, &ResultCode);
				Ret = IREX_ON_GOING;
				break;

			case IREX_CONTINUE_PHONEBOOK_RES:
			case IREX_OK_RES:
                if(!fEndBook)
                {
					gIrex_TxMsg.MsgType = IREX_FINISHED_S;
					gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
					SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
                    if(gIrex_ModelIndex == IREX_MODEL_B1)
                    {
                        fEndBook = TRUE;
                        Ret = IREX_ON_GOING;
                    }
                    /* rosa 2001-09-19 --> @why, what */
                    else if(gIrex_ModelIndex == IREX_MODEL_B2)
                    {
                        fEndBook = TRUE;
                        Ret = IREX_ON_GOING;
                    }
                    /* <-- */
                }
            }
        }
		if(Ret == IREX_SUCCESS)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_END_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndBook = FALSE;
            MessageReason = SMR_CHILD;
		}
		else if(Ret == IREX_FAILURE)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FAIL_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
        break;

    case SE_IO_MESGBOX_END:
        switch(MessageReason){
        case SMR_END:
            EQS_End();
            break;
        case SMR_CHILD:
		    if(!fIrDAPortClosed)
                SKY_CloseIrDAPort();

            fItemSelect = TRUE;
            fSending = FALSE;
            EQS_START_CHILD_EVENT(SKA_ViewIRDAList, SE_APP_START, nGroupIndex, 0);    // 전체리스트
            break;
        }
        break;

    case SE_TM_TIMER1:
    case SE_TM_TIMER2:      // ping response
        SKY_IrDAServerStateMachine(TIMEOUT_E, 0, &ResultCode);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*============================================================================= 
Function:    
    SKA_MainIrDABookSDial

Description: 
    단축다이얼 전송
  
Input:
    @ArgumentsList

Output:
    @TheResultOfThisFunction

Return value:
    @ReturnValue

Side effects:
===============================================================================*/
void SKY_API SKA_MainIrDABookSDial(SKY_EVENT_T *pEvent)
{
    static SIB_MESSAGE_REASON_E MessageReason;
    WORD  ResultCode;
    IREX_STATE_E   Ret;
    INT8   nAddress;
    TelInfo    SDial;
    static BOOL fEndBook;
    /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpSend;
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
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_BOOK), 
                      WA_SHOW_HEADER);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        MessageReason = SMR_NONE;

        if(sGetNextSDialItem(TRUE, &SDial) == -1)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_SDIAL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            MessageReason = SMR_END;
        }

		if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            g_fIrDAPortForPC = FALSE;
		}
        fEndBook = FALSE;

        SKY_OpenIrDAPort(0);	// Set to secondary
        gSendFile = gRecvFile = NULL;

        /* rosa 01-08-17 --> @why, what */
        // Message Line
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_BOOK);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_IRDA_MSG_PBOOK_SEND_SDIAL);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_BLANK);

        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        
        bmpSend = g_BmpExchangeDataIrDA;
        bmpSend.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpSend.Header) * IB_SEND; //2

        EQC_Image( IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpSend);
        /* <-- */        

        SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);
        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;

        /* rosa 2001-09-21 --> @why, what */
    //case SE_APP_DRAW:
        //EQS_PutBitmap(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        //break;
        /* <-- */
        
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
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        if(MessageReason == SMR_END)
            EQS_End();
        break;

    case SE_TM_TIMER1:
    case SE_TM_TIMER2:      // ping response
        SKY_IrDAServerStateMachine(TIMEOUT_E, 0, &ResultCode);
        break;
        
    case SE_IRDA_EX_RX_DATA_F:
		Ret = SKY_IrDAServerStateMachine(GET_MSG_E, 0, &ResultCode);
		
		if(Ret == IREX_PING_FINISHED)
		{
            if(gIrex_ModelIndex < IREX_MODEL_B0)
            {
                // Not Support
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NOT_SUPPORT), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_END;
            }

            nAddress = sGetNextSDialItem(TRUE, &SDial);
			if( nAddress == -1) 
			{
				gIrex_TxMsg.MsgType = IREX_FINISHED_S;
				gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
				SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
			}
            else
            {
			    sMakePhoneBookSDialMsg(nAddress, &SDial, &gIrex_TxMsg);
			    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
			    Ret = IREX_ON_GOING;
            }
			break;
		}
		else if(Ret == IREX_SUCCESS)
		{
			switch(ResultCode)
			{
			case IREX_GROUP_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FULL_GROUP), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;

				Ret = IREX_ON_GOING;
				break;

			case IREX_ITEM_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_MEMORY), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;

				Ret = IREX_ON_GOING;
				break;
			
			case IREX_ITEM_SAME_NAME:
            case IREX_ITEM_NOT_EXIST:
				SKY_IrDAServerStateMachine(WAIT_RESPONSE_E, 0, &ResultCode);
				Ret = IREX_ON_GOING;
				break;

			case IREX_CONTINUE_PHONEBOOK_RES:
			case IREX_OK_RES:
                nAddress = sGetNextSDialItem(FALSE, &SDial);
				if(nAddress == -1) 
				{
                    if(!fEndBook)
                    {
					    gIrex_TxMsg.MsgType = IREX_FINISHED_S;
					    gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
					    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
                        if(gIrex_ModelIndex == IREX_MODEL_B1)
                        {
                            fEndBook = TRUE;
                            Ret = IREX_ON_GOING;
                        }
                        /* rosa 2001-09-19 --> @why, what */
                        else if(gIrex_ModelIndex == IREX_MODEL_B2)
                        {
                            fEndBook = TRUE;
                            Ret = IREX_ON_GOING;
                        }
                        /* <-- */
                    }
				}
                else
                {
					sMakePhoneBookSDialMsg(nAddress, &SDial, &gIrex_TxMsg);
					SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
					Ret = IREX_ON_GOING;
                }
				break;
			}
		}

		if(Ret == IREX_SUCCESS)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_END_SDIAL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
		else if(Ret == IREX_FAILURE)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FAIL_SDIAL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
		break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainIrDABookMyInfo()
**
** Description: 
**     자기정보 전송
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
void SKY_API SKA_MainIrDABookMyInfo(SKY_EVENT_T *pEvent)
{
    static SIB_MESSAGE_REASON_E MessageReason;
    WORD  ResultCode;
    IREX_STATE_E   Ret;
    static BOOL fEndBook;
    /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpSend;
    /* <-- */

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_BOOK), 
                      WA_SHOW_HEADER);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        MessageReason = SMR_NONE;

		if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            g_fIrDAPortForPC = FALSE;
		}

        fEndBook = FALSE;
        SKY_OpenIrDAPort(0);	// Set to secondary
        gSendFile = gRecvFile = NULL;        

        /* rosa 01-08-17 --> @why, what */
        // Message Line
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_BOOK);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_IRDA_MSG_PBOOK_SEND_MYINFO);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_BLANK);        

        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);        

        bmpSend = g_BmpExchangeDataIrDA;
        bmpSend.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpSend.Header) * IB_SEND; //2

        EQC_Image( IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpSend);
        /* <-- */        

        SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
        /* rosa 2001-09-21 --> @why, what */
    //case SE_APP_DRAW:
        //EQS_PutBitmap(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        //break;
        /* <-- */

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

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IRDA_EX_RX_DATA_F:
		Ret = SKY_IrDAServerStateMachine(GET_MSG_E, 0, &ResultCode);

		if(Ret == IREX_PING_FINISHED)
		{
			sMakeMyInfoMsg(&gIrex_TxMsg);
			SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
			Ret = IREX_ON_GOING;
        }
		else if(Ret == IREX_SUCCESS)
		{
			switch(ResultCode)
			{
			case IREX_GROUP_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FULL_GROUP), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;
				Ret = IREX_ON_GOING;
				break;

			case IREX_ITEM_CREATE_FAILURE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_MEMORY), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                MessageReason = SMR_NONE;
				Ret = IREX_ON_GOING;
				break;
			
			case IREX_ITEM_SAME_NAME:
				SKY_IrDAServerStateMachine(WAIT_RESPONSE_E, 0, &ResultCode);
				Ret = IREX_ON_GOING;
				break;

			case IREX_CONTINUE_PHONEBOOK_RES:
			case IREX_OK_RES:
                if(!fEndBook)
                {
					gIrex_TxMsg.MsgType = IREX_FINISHED_S;
					gIrex_TxMsg.Msg.Finished_s.ResultCode = 0;
					SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
                    if(gIrex_ModelIndex == IREX_MODEL_B1)
                    {
                        fEndBook = TRUE;
                        Ret = IREX_ON_GOING;
                    }
                    /* rosa 2001-09-19 --> @why, what */
                    else if(gIrex_ModelIndex == IREX_MODEL_B2)
                    {
                        fEndBook = TRUE;
                        Ret = IREX_ON_GOING;
                    }
                    /* <-- */
                }
            }
        }
		if(Ret == IREX_SUCCESS)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_END_MYINFO), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
		else if(Ret == IREX_FAILURE)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_FAIL_MYINFO), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

            fEndBook = FALSE;
            MessageReason = SMR_END;
		}
        break;

    case SE_IO_MESGBOX_END:
        switch(MessageReason){
        case SMR_END:
            EQS_End();
            break;
        }
        break;

    case SE_TM_TIMER1:
    case SE_TM_TIMER2:      // ping response
        SKY_IrDAServerStateMachine(TIMEOUT_E, 0, &ResultCode);
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*============================================================================= 
Function:    
    SKY_ProcessPhoneBookMsg

Description: 
    전화부 수신 메시지 처리
  
Input:
    @ArgumentsList

Output:
    @TheResultOfThisFunction

Return value:
    @ReturnValue

Side effects:
===============================================================================*/
IREX_RESULT_E SKY_API SKY_ProcessPhoneBookMsg(BOOL fOverWrite)
{

    if(gIrex_ModelIndex <= IREX_MODEL_B0)
        return sProcessOldPhoneBookMsg(fOverWrite);
    else
        return sProcessEQUSPhoneBookMsg(fOverWrite);

    dog_report(DOG_PS_RPT | DOG_DS_RPT);
}

IREX_RESULT_E SKY_API sProcessOldPhoneBookMsg(BOOL fOverWrite)
{
	UINT8			GroupIndex;
	a7_nv_dial_type    Dial;
    BYTE	num, digits[A7_NV_MAX_DIAL_DIGITS];
    INT16   nAddress, nItemIndex;

	switch(gIrex_ModelIndex)
	{
	case IREX_MODEL_C5:
		// Fill the email with spaces
		gIrex_RxMsg.Msg.c5_PhoneBook_s.Data.reserved1 = 0;
		memset(
			(void*)gIrex_RxMsg.Msg.c5_PhoneBook_s.Data.email, 
			' ',	
			NV_MAX_DIAL_DIGITS);

	case IREX_MODEL_C5_WAP:
	case IREX_MODEL_B5:
	case IREX_MODEL_B5_WAP:

		// copy items from address to anniv_content
		memcpy(
			(void*)&Dial, 
			(void*)&gIrex_RxMsg.Msg.c5_PhoneBook_s.Data, 
			sizeof(c5_nv_dial_type) - C5_NV_MAX_MEMO - C5_NV_MAX_LTRS - 16);

		// convert multi_digits
	    num = Dial.multi_num_digits[0];
	    memcpy(digits, (BYTE*)Dial.multi_digits[0], A7_NV_MAX_DIAL_DIGITS);

	    Dial.multi_num_digits[0] = Dial.multi_num_digits[2];
	    memcpy((BYTE*)Dial.multi_digits[0], 
                   (BYTE*)Dial.multi_digits[2], 
                   A7_NV_MAX_DIAL_DIGITS);

	    Dial.multi_num_digits[2] = Dial.multi_num_digits[1];
	    memcpy((BYTE*)Dial.multi_digits[2], 
                   (BYTE*)Dial.multi_digits[1], 
                   A7_NV_MAX_DIAL_DIGITS);

	    Dial.multi_num_digits[1] = num;
	    memcpy((BYTE*)Dial.multi_digits[1], digits, A7_NV_MAX_DIAL_DIGITS);

		// Copy memo
		memcpy(
			(void*)Dial.memo,
			(void*)gIrex_RxMsg.Msg.c5_PhoneBook_s.Data.memo,
			C5_NV_MAX_MEMO);
		memset(
			(void*)&Dial.memo[C5_NV_MAX_MEMO],
			' ',
			A7_NV_MAX_MEMO_SIZ - C5_NV_MAX_MEMO);
				
		// Copy letters
		memcpy(
			(void*)Dial.letters,
			(void*)gIrex_RxMsg.Msg.c5_PhoneBook_s.Data.letters,
			A7_NV_MAX_LTRS);

		// Copy sync_flag
		Dial.sync_flag = 'C';

		// Copy tstamp
		Dial.tstamp
			= gIrex_RxMsg.Msg.c5_PhoneBook_s.Data.tstamp;

        memcpy(g_GroupName, (void *)gIrex_RxMsg.Msg.c5_PhoneBook_s.GroupName,
                   MAX_GROUP_NAME);
        memcpy(g_Letters, (BYTE *)gIrex_RxMsg.Msg.c5_PhoneBook_s.Data.letters,
                   MAX_BOOK_NAME);
        g_Letters[MAX_BOOK_NAME] = '\0';
		break;

	case IREX_MODEL_A7:
    case IREX_MODEL_A8:
    case IREX_MODEL_B0:
	default:
        memcpy(g_GroupName, (void *)gIrex_RxMsg.Msg.a7_PhoneBook_s.GroupName,
                   MAX_GROUP_NAME);
        memcpy(g_Letters, (BYTE *)gIrex_RxMsg.Msg.a7_PhoneBook_s.Data.letters,
                   MAX_BOOK_NAME);
        g_Letters[MAX_BOOK_NAME] = '\0';

		Dial = gIrex_RxMsg.Msg.a7_PhoneBook_s.Data;
		Dial.sync_flag = 'C';
		break;
	}

    MakeValidString(g_GroupName, MAX_GROUP_NAME);
    MakeValidString(g_Letters, MAX_BOOK_NAME);

	GroupIndex = SearchGroup(g_GroupName);
	if(!GroupIndex)
    {
        if(!AddGroup(g_GroupName, FALSE))
		    return IREX_GROUP_CREATE_FAILURE;
    }

    if(!STRCMP(g_Letters, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NO_NAME)))
        nItemIndex = NULL;  // 이름없음
    else
        nItemIndex = FindBook(g_GroupName, g_Letters);

    if(!nItemIndex)  // Not found
    {
        PBOOK_ERROR_E pbResult;

        pbResult = SaveOldBook(g_GroupName, (NV_DIAL_TYPE*)&Dial, &nAddress);

        switch(pbResult){
        case PBE_NO_ERROR:
            break;
        default:
            return IREX_ITEM_CREATE_FAILURE;
            break;
        }
    }
    else            // found
    {
        if(gfPhoneBookSkipAll)      // 전부 '아니오'
            return IREX_OK_RES;

	    if(gfPhoneBookOverwriteAll || fOverWrite)   // 전부 '예' or '예'     
	    {
            PBOOK_ERROR_E pbResult;

            DeleteBookInfo(nItemIndex);
            pbResult = SaveOldBook(g_GroupName, (NV_DIAL_TYPE*)&Dial, &nAddress);

            switch(pbResult){
            case PBE_NO_ERROR:
                break;
            default:
                return IREX_ITEM_CREATE_FAILURE;
                break;
            }
        }
        else
        {
            return IREX_ITEM_SAME_NAME;
        }
    }

    EQC_SetStaticText(ghMsgLine1, g_Letters);
    /* rosa 2001-09-24 --> ghMsgLine2은 한번만 업데이트시킴 */
    //EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_PBOOK_ITEM));
    /* <-- */

	return IREX_OK_RES;
}


IREX_RESULT_E SKY_API sProcessEQUSPhoneBookMsg(BOOL fOverWrite)
{
    INT16   nItemIndex;
    IREX_RESULT_E result = IREX_OK_RES;

    STRCPY(g_Letters, (void *)gIrex_RxMsg.Msg.PhoneBook_s.szName);
    STRCPY(g_GroupName, (void *)gIrex_RxMsg.Msg.PhoneBook_s.szGroupName);

    if(!STRCMP(g_Letters, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NO_NAME)))
        nItemIndex = NULL;  // 이름없음
    else
        nItemIndex = FindBook((void *)gIrex_RxMsg.Msg.PhoneBook_s.szGroupName, 
                           (void *) gIrex_RxMsg.Msg.PhoneBook_s.szName);

    if(nItemIndex)
    {
        if(gfPhoneBookSkipAll)      // 전부 '아니오'
            return IREX_OK_RES;

	    if(gfPhoneBookOverwriteAll || fOverWrite)   // 전부 '예' or '예'     
	    {
            DeleteBookInfo(nItemIndex);
        }
        else
        {
            return IREX_ITEM_SAME_NAME;
        }
    }

    result = sSaveEQUSBook(&gIrex_RxMsg.Msg.PhoneBook_s);

    EQC_SetStaticText(ghMsgLine1, (void *)gIrex_RxMsg.Msg.PhoneBook_s.szName);
    /* rosa 2001-09-24 --> ghMsgLine2은 한번만 업데이트시킴 */
    //EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_PBOOK_ITEM));
    /* <-- */


    return result;
}



/*============================================================================= 
Function:    
    SKY_ProcessSDialMsg

Description: 
    전화부 단축다이얼 수신 메시지 처리
  
Input:
    @ArgumentsList

Output:
    @TheResultOfThisFunction

Return value:
    @ReturnValue

Side effects:
===============================================================================*/
IREX_RESULT_E SKY_API SKY_ProcessSDialMsg(BOOL fOverWrite)
{

    if(gIrex_ModelIndex <= IREX_MODEL_B0)
        return sProcessOldSDialMsg(fOverWrite);
    else
        return sProcessEQUSSDialMsg(fOverWrite);

    dog_report(DOG_PS_RPT | DOG_DS_RPT);
}


IREX_RESULT_E SKY_API sProcessOldSDialMsg(BOOL fOverWrite)
{
    BYTE szNumber[PB_MAX_DIGIT_LENGTH+1]; 
    BYTE szTmp[A7_NV_MAX_LTRS+5];
    BYTE szTmp1[A7_NV_MAX_LTRS];
    a7_nv_dial_type    Dial;
    TelInfo         DialInfo;
	UINT8			GroupIndex, whereto;
    INT16   nAddress, nItemIndex;
    pb_sdial_type    SDial;

    memcpy((void*)&Dial, 
               (void*)&gIrex_RxMsg.Msg.a7_PhoneBook_SDial_s.DialData,
               sizeof(a7_nv_dial_type));

    whereto = gIrex_RxMsg.Msg.a7_PhoneBook_SDial_s.whereto;

    memcpy(szNumber, 
               (void*)Dial.multi_digits[whereto],
               Dial.multi_num_digits[whereto]);

    szNumber[Dial.multi_num_digits[whereto]] = '\0';
    //MakeValidString(szNumber, PB_MAX_DIGIT_LENGTH);

    memcpy(g_Letters, (BYTE *)gIrex_RxMsg.Msg.a7_PhoneBook_SDial_s.DialData.letters,
               MAX_BOOK_NAME);
    MakeValidString(g_Letters, MAX_BOOK_NAME);

    if(!GetTelInfo(szNumber, &DialInfo))        // Not Found
    {
        if(!gfPhoneBookOverwriteAll && !fOverWrite)
            return IREX_ITEM_NOT_EXIST;

        memcpy(g_GroupName, (void*)gIrex_RxMsg.Msg.a7_PhoneBook_SDial_s.GroupName,
                   MAX_GROUP_NAME);
	    Dial.sync_flag = 'C';

        MakeValidString(g_GroupName, MAX_GROUP_NAME);

	    GroupIndex = SearchGroup(g_GroupName);
	    if(!GroupIndex)
        {
            if(!AddGroup(g_GroupName, FALSE))
		        return IREX_GROUP_CREATE_FAILURE;
        }

        nItemIndex = FindBook(g_GroupName, g_Letters);

        if(!nItemIndex)            // found
        {
            if(gfPhoneBookSkipAll)      // 전부 '아니오'
                return IREX_OK_RES;

	        if(gfPhoneBookOverwriteAll || fOverWrite)   // 전부 '예' or '예'     
	        {
                PBOOK_ERROR_E pbResult;

                DeleteBookInfo(nItemIndex);
                pbResult = SaveOldBook(g_GroupName, (NV_DIAL_TYPE*)&Dial, &nAddress);

                switch(pbResult){
                case PBE_NO_ERROR:
                    break;
                default:
                    return IREX_ITEM_CREATE_FAILURE;
                    break;
                }
            }
            else
            {
                return IREX_ITEM_NOT_EXIST;
            }
        }
        
        GetTelInfo(szNumber, &DialInfo);
    }

    SDial.address = gIrex_RxMsg.Msg.a7_PhoneBook_SDial_s.address;
    SDial.dial_address = DialInfo.nNumAddress;

    SetShortDialInfo(&SDial);

    STRCPY(szTmp1, g_Letters);
    MakeShortString( szTmp1, A7_NV_MAX_LTRS-5);
    sprintf((char*)szTmp, "[%2d]%s", SDial.address, szTmp1);
    EQC_SetStaticText(ghMsgLine1, szTmp);
    EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_PBOOK_ITEM));

	return IREX_OK_RES;
}

IREX_RESULT_E SKY_API sProcessEQUSSDialMsg(BOOL fOverWrite)
{
    BYTE szTmp[A7_NV_MAX_LTRS+5];
    BYTE szTmp1[A7_NV_MAX_LTRS];
    TelInfo         DialInfo;
	INT16   nItemIndex;
    pb_sdial_type    SDial;
    IREX_RESULT_E result = IREX_OK_RES;

    if(!GetTelInfo((void *)gIrex_RxMsg.Msg.PhoneBook_SDial_s.Number.szNumber, &DialInfo))        // Not Found
    {
        STRCPY(g_Letters, (void *)gIrex_RxMsg.Msg.PhoneBook_SDial_s.DialData.szName);
        STRCPY(g_GroupName,(void *)gIrex_RxMsg.Msg.PhoneBook_SDial_s.DialData.szGroupName);

        if(!gfPhoneBookOverwriteAll && !fOverWrite)
            return IREX_ITEM_NOT_EXIST;

        nItemIndex = FindBook((void *)gIrex_RxMsg.Msg.PhoneBook_SDial_s.DialData.szGroupName,
                            (void *)gIrex_RxMsg.Msg.PhoneBook_SDial_s.DialData.szName);

        if(!nItemIndex)
        {
            if(gfPhoneBookSkipAll)      // 전부 '아니오'
                return IREX_OK_RES;

	        if(gfPhoneBookOverwriteAll || fOverWrite)   // 전부 '예' or '예'     
	        {
                DeleteBookInfo(nItemIndex);
            }
            else
            {
                return IREX_ITEM_SAME_NAME;
            }
        }

        result = sSaveEQUSBook(&gIrex_RxMsg.Msg.PhoneBook_SDial_s.DialData);
        
        GetTelInfo((void *)gIrex_RxMsg.Msg.PhoneBook_SDial_s.Number.szNumber, &DialInfo);
    }

    SDial.address = gIrex_RxMsg.Msg.PhoneBook_SDial_s.nAddress;
    SDial.dial_address = DialInfo.nNumAddress;

    SetShortDialInfo(&SDial);

    STRCPY(szTmp1, (void *)gIrex_RxMsg.Msg.PhoneBook_SDial_s.DialData.szName);
    MakeShortString( szTmp1, A7_NV_MAX_LTRS-5);
    sprintf((char*)szTmp, "[%2d]%s", SDial.address, szTmp1);
    EQC_SetStaticText(ghMsgLine1, szTmp);
    EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_PBOOK_ITEM));

	return result;
}


IREX_RESULT_E SKY_API sSaveEQUSBook(IREX_PHONEBOOK_S_TYPE_T *pDialData)
{
    pb_book_info_type BookInfo;
    pb_anniv_info_type  AnnivInfo;
    pb_group_info_type  GroupInfo;
    PBOOK_ERROR_E pbResult;

    INT16   nGroupIndex;
    UINT8   i;

    init_pb_node(PB_BOOK_INFO_I, &BookInfo);

    // Group
    nGroupIndex = SearchGroup((void *)pDialData->szGroupName);
	if(!nGroupIndex)
    {
        nGroupIndex = AddGroup((void *)pDialData->szGroupName, FALSE);

        if(!nGroupIndex)
		    return IREX_GROUP_CREATE_FAILURE;
    }

    GroupInfo.address = nGroupIndex;
    ReadGroup(&GroupInfo);

    // Multi group
    if(STRLEN((void *)pDialData->szMultiGroup))
    {
        nGroupIndex = SearchGroup((void *)pDialData->szMultiGroup);
	    if(!nGroupIndex)
        {
            if(!AddGroup((void *)pDialData->szMultiGroup, FALSE))
		        return IREX_GROUP_CREATE_FAILURE;
        }
        BookInfo.multi_group = nGroupIndex;
    }

    // Name
    STRCPY(BookInfo.name, (void *)pDialData->szName);
    // memo
    STRCPY(BookInfo.szText,(void *)pDialData->szMemo);//pDialData->szEmail);
    
    if(!AddBook(&GroupInfo, &BookInfo))
    {
        return IREX_ITEM_CREATE_FAILURE;
    }

    // Number
    for(i = 0; i < IRDA_PERSONAL_NUMBER_LIMIT; i++)
    {
        if(STRLEN((void *)pDialData->Number[i].szNumber) == 0)
            continue;

        ReverseAddNumber(&BookInfo, 
                  (void *)pDialData->Number[i].szNumber,
                  pDialData->Number[i].bType);
    }

    // email
    for(i = 0; i < PERSONAL_EMAIL_LIMIT; i++)
    {
        if(STRLEN((void *)pDialData->aszEmail[i]) == 0)
            continue;

        AddEmail(&BookInfo,
                (void *)pDialData->aszEmail[i]);
    }

    // Anniv. Data
    for(i = 0; i < PERSONAL_ANNIV_LIMIT; i++)
    {
        if(!pDialData->AnnivData[i].bType)
            continue;

        init_pb_node(PB_ANNIV_INFO_I, &AnnivInfo);
        AnnivInfo.type = pDialData->AnnivData[i].bType;
        AnnivInfo.day = pDialData->AnnivData[i].nDay;
        AnnivInfo.month = pDialData->AnnivData[i].nMonth;
        AnnivInfo.year = pDialData->AnnivData[i].wYear;
        AnnivInfo.fLunar = pDialData->AnnivData[i].fLunar;
        /* rosa 2001-10-04 --> @why, what */
        STRCPY((BYTE *)AnnivInfo.content, (BYTE *)pDialData->AnnivData[i].szContent);
        /* <-- */

        AddAnnivInfo(&BookInfo, &AnnivInfo);
    }

    // Write
    if(WriteBook(&BookInfo))    // Success
        return IREX_OK_RES;
    else
        return IREX_ITEM_CREATE_FAILURE;
}

INT16 sGetNextItemInAll(BOOL fInitial)
{
	static INT16    nAddress;

	if(fInitial) nAddress = 1;

	while(nAddress <= MAX_BOOK_NUMBER)
	{
		if(IS_VALID_BOOK(nAddress) &&
            !IS_SECRET_GROUP(g_PersonGroup[nAddress-1]))
        {
            nAddress ++;
            return nAddress - 1;
        }

		nAddress++;
	}

	return -1;
}

INT16 sGetNextItemInGroup(BOOL fInitial, INT8 nHandle)
{
	static INT16    nSeq;
    INT16   nAddress;

	if(fInitial) nSeq = 0;

    if(nSeq < GetListSize(nHandle))
	{
        nAddress = GetListValue(nHandle,nSeq);
        nSeq ++;

        return nAddress;
    }
    else
	    return -1;
}

INT8 sGetNextSDialItem(BOOL fInitial, TelInfo *pTelInfo)
{
	static INT8    nAddress;

	if(fInitial) nAddress = 1;

	while(nAddress <= MAX_SDIAL_NUMBER)
	{
        if(GetShortDialInfo(nAddress, pTelInfo))
        {
            nAddress ++;
            return nAddress - 1;
        }

		nAddress++;
	}

	return -1;
}

void sMakePhoneBookSDialMsg(INT8 nAddress, TelInfo* SDial, 
                            IREX_MSG_TYPE_T *pTxMsg)
{
    pTxMsg->MsgType = IREX_PHONEBOOK_SDIAL_S;

    if(gIrex_ModelIndex == IREX_MODEL_B0)  // Not EQUS structure
    {
        sMakeA7PhoneBookSDialMsg(nAddress, SDial, pTxMsg);
    }
    else        // EQUS Structure
    {
        sMakeEQUSPhoneBookSDialMsg(nAddress, SDial, pTxMsg);
    }
}

void sMakeA7PhoneBookSDialMsg(INT8 nAddress, TelInfo* SDial, 
                              IREX_MSG_TYPE_T *pTxMsg)
{
    NV_DIAL_TYPE Dial;
    pb_group_info_type GroupInfo;
    pb_number_info_type NumberInfo;
    UINT8   index;

    pTxMsg->Msg.a7_PhoneBook_SDial_s.address = nAddress;
    MakeOldBook(SDial->nBookAddress, &Dial);
    GroupInfo.address = g_PersonGroup[SDial->nBookAddress-1];
    ReadGroup(&GroupInfo);

	if(Dial.group == NO_GROUP_NAME_INDEX)
	{
		STRCPY((BYTE*)pTxMsg->Msg.a7_PhoneBook_SDial_s.GroupName, 
               (BYTE*)"지정안됨");
	}
	else
	{
		STRNCPY((BYTE*)pTxMsg->Msg.a7_PhoneBook_SDial_s.GroupName,
			    (BYTE*)GroupInfo.group_name,
			    A7_UI_WIDE);
	}

    MakeOldString((BYTE*)pTxMsg->Msg.a7_PhoneBook_SDial_s.GroupName, A7_UI_WIDE);

    NumberInfo.address = SDial->nNumAddress;
    ReadNumber(&NumberInfo);

    if(NumberInfo.number_type < 5)
        index = NumberInfo.number_type - 1;
    else
        index = 3;

    Dial.multi_num_digits[index] = STRLEN(NumberInfo.digits);
    STRNCPY((BYTE*)Dial.multi_digits[index], NumberInfo.digits, A7_NV_MAX_DIAL_DIGITS);
    MakeOldString((BYTE*)Dial.multi_digits[index], A7_NV_MAX_DIAL_DIGITS);

    pTxMsg->Msg.a7_PhoneBook_SDial_s.whereto = index;

    memcpy((void*)&pTxMsg->Msg.a7_PhoneBook_SDial_s.DialData,
               (void*)&Dial,
               sizeof(NV_DIAL_TYPE));
}

void sMakeEQUSPhoneBookSDialMsg(INT8 nAddress, TelInfo* SDial, 
                                IREX_MSG_TYPE_T *pTxMsg)
{
    pb_number_info_type NumberInfo;

    pTxMsg->Msg.PhoneBook_SDial_s.nAddress = nAddress;

    NumberInfo.address = SDial->nNumAddress;
    ReadNumber(&NumberInfo);

    pTxMsg->Msg.PhoneBook_SDial_s.Number.bType = 
        NumberInfo.number_type;

    STRCPY((void *)pTxMsg->Msg.PhoneBook_SDial_s.Number.szNumber,
           NumberInfo.digits);

    sMakeEQUSPhoneBookMsg(SDial->nBookAddress, 
                          &pTxMsg->Msg.PhoneBook_SDial_s.DialData);
    
}


void sMakePhoneBookMsg(INT16 nAddress, IREX_MSG_TYPE_T *pTxMsg)
{
	pTxMsg->MsgType = IREX_PHONEBOOK_S;

	switch(gIrex_ModelIndex)
	{
	case IREX_MODEL_C5:
	case IREX_MODEL_C5_WAP:
	case IREX_MODEL_B5:
	case IREX_MODEL_B5_WAP:
        sMakeC5PhoneBookMsg(nAddress, pTxMsg);
		break;

	case IREX_MODEL_A7:
    case IREX_MODEL_A8:
    case IREX_MODEL_B0:
        sMakeA7PhoneBookMsg(nAddress, pTxMsg);
		break;

    case IREX_MODEL_B1:
    /* rosa 2001-09-19 --> @why, what */
    case IREX_MODEL_B2:
    /* <-- */
    default:
        sMakeEQUSPhoneBookMsg(nAddress, &pTxMsg->Msg.PhoneBook_s);
        break;
	}
}

void sMakeC5PhoneBookMsg(INT16 nAddress, IREX_MSG_TYPE_T *pTxMsg)
{
    NV_DIAL_TYPE Dial;
    BYTE    num, digits[NV_MAX_DIAL_DIGITS];
    pb_group_info_type GroupInfo;

    MakeOldBook(nAddress, &Dial);
    GroupInfo.address = g_PersonGroup[nAddress-1];
    ReadGroup(&GroupInfo);

	memcpy((void*)&pTxMsg->Msg.c5_PhoneBook_s.Data, 
		       (void*)&Dial, 
		       sizeof(c5_nv_dial_type) - C5_NV_MAX_MEMO - C5_NV_MAX_LTRS - 16);

	// convert multi_digits
	num = pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[2]; 
	memcpy(digits, 
               (void*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[2], 
               NV_MAX_DIAL_DIGITS);

	pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[2] = 
            pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[0];

	memcpy((void*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[2], 
               (void*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[0], 
               NV_MAX_DIAL_DIGITS);

	pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[0] = 
            pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[1];

	memcpy((void*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[0], 
               (void*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[1], 
               NV_MAX_DIAL_DIGITS);

	pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[1] = num;

	memcpy((void*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[1], 
               digits, 
               NV_MAX_DIAL_DIGITS);

	// Copy memo
	memcpy((void*)pTxMsg->Msg.c5_PhoneBook_s.Data.memo,
			   (void*)Dial.memo,
			   C5_NV_MAX_MEMO);

	// Copy letters
	memcpy((void*)pTxMsg->Msg.c5_PhoneBook_s.Data.letters,
			   (void*)Dial.letters,
			   C5_NV_MAX_LTRS);

	// Copy sync_flag
	pTxMsg->Msg.c5_PhoneBook_s.Data.sync_flag
		= Dial.sync_flag;

	// Copy tstamp
	pTxMsg->Msg.c5_PhoneBook_s.Data.tstamp = Dial.tstamp;

	if(Dial.group == NO_GROUP_NAME_INDEX)
	{
		STRCPY((void *)pTxMsg->Msg.c5_PhoneBook_s.GroupName, (BYTE*)"지정안됨");
	}
	else
	{
		STRNCPY((void *)pTxMsg->Msg.c5_PhoneBook_s.GroupName,
			    GroupInfo.group_name,
			    A7_UI_WIDE);
	}

    MakeOldString((void *)pTxMsg->Msg.c5_PhoneBook_s.GroupName, A7_UI_WIDE);
}

void sMakeA7PhoneBookMsg(INT16 nAddress, IREX_MSG_TYPE_T *pTxMsg)
{
    NV_DIAL_TYPE Dial;
    pb_group_info_type GroupInfo;

    MakeOldBook(nAddress, &Dial);
    GroupInfo.address = g_PersonGroup[nAddress-1];
    ReadGroup(&GroupInfo);

	memcpy((void*)&pTxMsg->Msg.a7_PhoneBook_s.Data, 
			   (void*)&Dial, 
			    sizeof(a7_nv_dial_type));

	if(Dial.group == NO_GROUP_NAME_INDEX)
	{
		STRCPY((void *)pTxMsg->Msg.a7_PhoneBook_s.GroupName, (BYTE*)"지정안됨");
	}
	else
	{
		STRNCPY((void *)pTxMsg->Msg.a7_PhoneBook_s.GroupName,
			    GroupInfo.group_name,
			    A7_UI_WIDE);
	}

    MakeOldString((void *)pTxMsg->Msg.a7_PhoneBook_s.GroupName, A7_UI_WIDE);
}


void sMakeEQUSPhoneBookMsg(INT16 nAddress, 
                           IREX_PHONEBOOK_S_TYPE_T *pPhoneBook_s)
{
    int i;
    pb_book_info_type BookInfo;
    pb_group_info_type GroupInfo;
    pb_number_info_type NumberInfo;
    pb_email_info_type   EMailInfo;
    pb_anniv_info_type  AnnivInfo;

    INT16   nItemAddress;

    BookInfo.address = nAddress;
    ReadBook(&BookInfo);

    // Group
    GroupInfo.address = BookInfo.group_address;
    ReadGroup(&GroupInfo);

    STRCPY((void *)pPhoneBook_s->szGroupName,
           GroupInfo.group_name);

    // Multi Group
    if(BookInfo.multi_group)
    {
        GroupInfo.address = BookInfo.multi_group;
        ReadGroup(&GroupInfo);
        STRCPY((void *)pPhoneBook_s->szMultiGroup,
               GroupInfo.group_name);
    }
    else
    {
        STRCPY((void *)pPhoneBook_s->szMultiGroup,
               (BYTE*)"");
    }

    // Name
    STRCPY((void *)pPhoneBook_s->szName,
           BookInfo.name);

    // Memo
    STRCPY((void *)pPhoneBook_s->szMemo,
           BookInfo.szText);

    // Phone Number
    nItemAddress = BookInfo.number_address;
    for(i = IRDA_PERSONAL_NUMBER_LIMIT-1; i >= 0; i--)
    {
        if(nItemAddress > 0)
        {
            NumberInfo.address = nItemAddress;
            if(ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) > 0)
            {
                pPhoneBook_s->Number[i].bType = NumberInfo.number_type;
                STRCPY((void *)pPhoneBook_s->Number[i].szNumber,
                       NumberInfo.digits);
                nItemAddress = NumberInfo.next_index;
            }
            else
            {
                STRCPY((void *)pPhoneBook_s->Number[i].szNumber, (BYTE*)"");
                nItemAddress = 0;
            }
        }
        else
        {
            STRCPY((void *)pPhoneBook_s->Number[i].szNumber, (BYTE*)"");
            nItemAddress = 0;
        }
    }

    // EMail
    nItemAddress = BookInfo.email_address;
    for(i = PERSONAL_EMAIL_LIMIT-1; i >= 0; i--)
    {
        if(nItemAddress > 0)
        {
            EMailInfo.address = nItemAddress;
            if(ui_get_pb(PB_EMAIL_INFO_I, &EMailInfo) > 0)
            {
                STRCPY((void *)pPhoneBook_s->aszEmail[i], EMailInfo.szMailID);
                nItemAddress = EMailInfo.next_index;
            }
            else
            {
                STRCPY((void *)pPhoneBook_s->aszEmail[i], (BYTE*)"");
                nItemAddress = 0;
            }
        }
        else
        {
            STRCPY((void *)pPhoneBook_s->aszEmail[i], (BYTE*)"");
            nItemAddress = 0;
        }
    }

    // Anniv Data
    nItemAddress = BookInfo.anniv_address;
    for(i = 0; i < PERSONAL_ANNIV_LIMIT; i++)
    {
        if(nItemAddress > 0)
        {
            AnnivInfo.address = nItemAddress;
            if(ui_get_pb(PB_ANNIV_INFO_I, &AnnivInfo) > 0)
            {
                pPhoneBook_s->AnnivData[i].bType = AnnivInfo.type;
                pPhoneBook_s->AnnivData[i].nDay = AnnivInfo.day;
                pPhoneBook_s->AnnivData[i].nMonth = AnnivInfo.month;
                pPhoneBook_s->AnnivData[i].fLunar = AnnivInfo.fLunar;
                pPhoneBook_s->AnnivData[i].wYear = AnnivInfo.year;

                STRCPY((void *)pPhoneBook_s->AnnivData[i].szContent,
                       AnnivInfo.content);

                nItemAddress = AnnivInfo.next_index;
            }
            else
            {
                pPhoneBook_s->AnnivData[i].bType = 0;
                nItemAddress = 0;
            }
        }
        else
        {
            pPhoneBook_s->AnnivData[i].bType = 0;
            nItemAddress = 0;
        }
    }
}


void sMakeEQUSMyInfoMsg(IREX_PHONEBOOK_S_TYPE_T *pPhoneBook_s)
{
    int i;
    pb_info_type        PersonalInfo;

    ui_get_pb(PB_INFO_I, &PersonalInfo);

    // Group
    STRCPY((void *)pPhoneBook_s->szGroupName, (BYTE*)"지정안됨");

    // Multi Group
    STRCPY((void *)pPhoneBook_s->szMultiGroup, (BYTE*)"");

    // Name
    /* rosa 2001-10-04 --> @why, what */
    //STRCPY((void *)pPhoneBook_s->szName, PersonalInfo.Name);
    if(STRLEN(PersonalInfo.Name))
        STRCPY((BYTE *)pPhoneBook_s->szName, (BYTE *)PersonalInfo.Name);
    else
        STRCPY((BYTE *)pPhoneBook_s->szName, (BYTE *)"이름없음");
    /* <-- */

    // Memo
    STRCPY((void *)pPhoneBook_s->szMemo, PersonalInfo.Address);

    // Phone Number
    // 집전화
    pPhoneBook_s->Number[IRDA_PERSONAL_NUMBER_LIMIT-2].bType = HOME;
    STRCPY((void *)pPhoneBook_s->Number[IRDA_PERSONAL_NUMBER_LIMIT-2].szNumber, PersonalInfo.HomeTel);

    // 이동전화
    pPhoneBook_s->Number[IRDA_PERSONAL_NUMBER_LIMIT-1].bType = CELL;
    STRCPY((void *)pPhoneBook_s->Number[IRDA_PERSONAL_NUMBER_LIMIT-1].szNumber, PersonalInfo.MobileTel);

    for(i = IRDA_PERSONAL_NUMBER_LIMIT -3; i >= 0; i--)
    {
        STRCPY((void *)pPhoneBook_s->Number[i].szNumber, (BYTE*)"");
    }

    // EMail
    STRCPY((void *)pPhoneBook_s->aszEmail[0], PersonalInfo.Email);
    STRCPY((void *)pPhoneBook_s->aszEmail[1], (BYTE*)"");

    // Anniv Data
    if(PersonalInfo.wYear)
    {
        pPhoneBook_s->AnnivData[0].bType = BIRTH_ANNIV;
        pPhoneBook_s->AnnivData[0].nDay = PersonalInfo.nDay;
        pPhoneBook_s->AnnivData[0].nMonth = PersonalInfo.nMonth;
        pPhoneBook_s->AnnivData[0].fLunar = PersonalInfo.fLunar;
        pPhoneBook_s->AnnivData[0].wYear = PersonalInfo.wYear;

        STRCPY((void *)pPhoneBook_s->AnnivData[0].szContent, (BYTE*)"");
    }
}

void sMakeC5MyInfoMsg(IREX_MSG_TYPE_T *pTxMsg)
{
    pb_info_type        PersonalInfo;

    ui_get_pb(PB_INFO_I, &PersonalInfo);

    // Group
    STRCPY((void *)pTxMsg->Msg.c5_PhoneBook_s.GroupName, (BYTE*)"지정안됨");
    MakeOldString((void *)pTxMsg->Msg.c5_PhoneBook_s.GroupName, A7_UI_WIDE);

    // Name	
    /* rosa 2001-10-04 --> @why, what */
    //STRCPY((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.letters, PersonalInfo.Name);
    if(STRLEN(PersonalInfo.Name))
        STRCPY((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.letters, PersonalInfo.Name);
    else
        STRCPY((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.letters, (BYTE*)"이름없음");
    /* <-- */
    MakeOldString((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.letters, C5_NV_MAX_LTRS);

    // Memo
	STRCPY((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.memo, PersonalInfo.Address);
    MakeOldString((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.memo, C5_NV_MAX_MEMO);

    // Phone Number
    // 이동전화
	pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[2] = 
            STRLEN(PersonalInfo.MobileTel);

	STRCPY((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[2], 
               PersonalInfo.MobileTel);
    MakeOldString((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[2], C5_NV_MAX_DIAL_DIGITS);
    
    // 집전화
	pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[0] = 
            STRLEN(PersonalInfo.HomeTel);

	STRCPY((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[0], 
               PersonalInfo.HomeTel);
    MakeOldString((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.multi_digits[0], C5_NV_MAX_DIAL_DIGITS);


    pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[1] = 0;
    pTxMsg->Msg.c5_PhoneBook_s.Data.multi_num_digits[3] = 0;

    // EMail
    STRCPY((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.email, PersonalInfo.Email);
    MakeOldString((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.email, C5_NV_MAX_DIAL_DIGITS);

    // Anniv Data
    if(PersonalInfo.wYear)
    {
        pTxMsg->Msg.c5_PhoneBook_s.Data.anniv_contidx = 1;
        pTxMsg->Msg.c5_PhoneBook_s.Data.anniv_month = PersonalInfo.nMonth;
        pTxMsg->Msg.c5_PhoneBook_s.Data.anniv_day = PersonalInfo.nDay;
        pTxMsg->Msg.c5_PhoneBook_s.Data.anniv_lunar = PersonalInfo.fLunar;

        memset((BYTE*)pTxMsg->Msg.c5_PhoneBook_s.Data.anniv_content, ' ', C5_NV_MAX_LTRS);
    }

    pTxMsg->Msg.c5_PhoneBook_s.Data.tstamp = 0;
}

void sMakeA7MyInfoMsg(IREX_MSG_TYPE_T *pTxMsg)
{
    NV_DIAL_TYPE Dial;
    pb_info_type        PersonalInfo;

    ui_get_pb(PB_INFO_I, &PersonalInfo);

    // Name    
    /* rosa 2001-10-04 --> @why, what */
    //STRCPY( (BYTE*)Dial.letters, PersonalInfo.Name);
    if(STRLEN(PersonalInfo.Name))
        STRCPY((BYTE*)Dial.letters, PersonalInfo.Name);
    else
        STRCPY((BYTE*)Dial.letters, (BYTE*)"이름없음");
    /* <-- */
    MakeOldString((BYTE*)Dial.letters,A7_NV_MAX_LTRS);

    // Memo
    STRCPY( (BYTE*)Dial.memo  , PersonalInfo.Address);
	MakeOldString((BYTE*)Dial.memo, A7_NV_MAX_MEMO_SIZ);

    // Phone Number
    // 이동전화
    Dial.multi_num_digits[0] =  STRLEN(PersonalInfo.MobileTel);
    STRCPY((BYTE*) Dial.multi_digits[0], PersonalInfo.MobileTel);
    MakeOldString((BYTE*)Dial.multi_digits[0], A7_NV_MAX_DIAL_DIGITS);

    // 집전화
    Dial.multi_num_digits[1] =  STRLEN(PersonalInfo.HomeTel);
    STRCPY((BYTE*) Dial.multi_digits[1], PersonalInfo.HomeTel);
    MakeOldString((BYTE*)Dial.multi_digits[1], A7_NV_MAX_DIAL_DIGITS);

    Dial.multi_num_digits[2] =  0;
    Dial.multi_num_digits[3] =  0;

    // EMail
    STRCPY((BYTE*)Dial.email, PersonalInfo.Email);
    MakeOldString((BYTE*)Dial.email, A7_NV_MAX_DIAL_DIGITS);       

    // Anniv Data
    if(PersonalInfo.wYear)
    {
        Dial.anniv_contidx = BIRTH_ANNIV;
        Dial.anniv_day     = PersonalInfo.nDay;
        Dial.anniv_month   = PersonalInfo.nMonth;
        Dial.anniv_lunar   = PersonalInfo.fLunar == TRUE ? 1 : 0 ;
        STRCPY( (BYTE*) Dial.anniv_content, (BYTE*)"");
        MakeOldString((BYTE*) Dial.anniv_content, A7_NV_MAX_LTRS);
    }

    Dial.tstamp    = 0;

	memcpy((void*)&pTxMsg->Msg.a7_PhoneBook_s.Data, 
			   (void*)&Dial, 
			    sizeof(a7_nv_dial_type));

    // Group
    STRCPY((void *)pTxMsg->Msg.a7_PhoneBook_s.GroupName, (BYTE*)"지정안됨");
    MakeOldString((void *)pTxMsg->Msg.a7_PhoneBook_s.GroupName, A7_UI_WIDE);
}

void sMakeMyInfoMsg(IREX_MSG_TYPE_T *pTxMsg)
{
	pTxMsg->MsgType = IREX_PHONEBOOK_S;

	switch(gIrex_ModelIndex)
	{
	case IREX_MODEL_C5:
	case IREX_MODEL_C5_WAP:
	case IREX_MODEL_B5:
	case IREX_MODEL_B5_WAP:
        sMakeC5MyInfoMsg(pTxMsg);
		break;

	case IREX_MODEL_A7:
    case IREX_MODEL_A8:
    case IREX_MODEL_B0:
        sMakeA7MyInfoMsg(pTxMsg);
		break;

    case IREX_MODEL_B1:
    /* rosa 2001-09-19 --> @why, what */
    case IREX_MODEL_B2:
    /* <-- */
    default:
        sMakeEQUSMyInfoMsg(&pTxMsg->Msg.PhoneBook_s);
        break;
	}
}    