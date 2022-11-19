/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_mbox.c
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
** 2001-02-08 woodstok     Created.
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
#ifdef WIN32
#define EMAL_PARSING_TEST
#endif
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
#define EMAIL_SEND          0
#define EMAIL_RECEIVE		1
#define EMAIL_RECEIVE_NEW   2

#define EMAIL_WAIT_TIME     20000 // 20Sec
#define EMAIL_WAIT_ENDTIME     5000 // 5Sec

enum tag_EMAIL_INBOX_RESULT_E {
    EIR_RECVOK,
    EIR_RECVERR,
    EIR_NOMAIL,
    EIR_USERERR,
    EIR_PASSERR
};

typedef enum tag_EMAIL_INBOX_RESULT_E EMAIL_INBOX_RESULT_E;
/* rosa 01-08-21 --> @why, what */
#define EMAIL_ANI_X     26//28
#define EMAIL_ANI_Y     29//17

#define EMAIL_STATUS_X  15//34//15
#define EMAIL_STATUS_Y  102//(EMAIL_ANI_Y + 37 + 14)

#define EMAIL_FRAME_X   4
#define EMAIL_FRAME_Y   94

#define EMAIL_TEXT_WIDTH    16//18
/* <-- */
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
enum tag_EMAIL_EDIT_MAIL_E {
    EEM_RECV_TITLE,
    EEM_RECV,
    EEM_TITLE_TITLE,
    EEM_TITLE,
    EEM_TEXT_TITLE,
    EEM_TEXT,
    EEM_MAX
};

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
//BOOL    g_fEmailInProg = FALSE;

EMAIL_SEND_T  g_SendMail;
EMAIL_RECV_T  g_RecvMail;

EMAIL_SEND_RESULT_E     g_SendOK;
EMAIL_INBOX_RESULT_E   g_ReceiveOK;
UINT8                  g_MailListNum[MAX_SAVED_MAIL];
EMAIL_LIST_T           g_MailList[MAX_SAVED_MAIL];
UINT8				   g_MailNum;

BYTE    g_RecvBuffer[MAX_RECV_MAIL_SIZE];
INT16   g_RecvLen;

BYTE    *l_MailScroll[MAX_SAVED_MAIL];
DWORD   l_startTime;

BOOL  g_fIdle = FALSE;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
static struct {
  byte                  ui_try_cnt;  // # of try, for UI originated call only
  boolean               secret;      // the # is secret? outgoing call only
  cm_num_s_type         num;         // origination/incom #, may include pause
  cm_alpha_s_type       alpha;       // alpha tag, phonebook match or caller ID
  dword                 connect_time;// time the call is connected
  cm_call_info_s_type   cm_info;     // information from call manager
  cm_inband_info_s_type inband_info; // inband info
} curr_call;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
// Send
LOCAL void SKY_API sAddEMailEditControls(UINT8 nMailNum, UINT8 nReplyNum);
LOCAL BOOL SKY_API sMakeEmailMessage(void);
LOCAL void SKY_API sSKA_EMailSMTPInProg(SKY_EVENT_T *pEvent);

// Receive
LOCAL UINT8 SKY_API sAddMailOutBoxControls(void);
LOCAL void SKY_API sAddRecvEMailControls(void);
LOCAL void SKY_API sSKA_EMailPOP3InProg(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_ViewRecvMail(SKY_EVENT_T *pEvent);

// Common
LOCAL  void sSKA_EMailOriginated(SKY_EVENT_T *pInEvent);

// Mail Box
LOCAL void SKY_API sReadOutBoxList(void);
LOCAL UINT8 SKY_API sSaveCurrentSendMail(UINT8 nMailNum);
LOCAL void SKY_API sDeleteSendMail(UINT8 nIndex);
LOCAL BOOL SKY_API sMakeMailFromSendFile(UINT8 nIndex);
LOCAL BOOL SKY_API sMakeMailFromRecvFile(UINT8 nIndex);

LOCAL void SKY_API sDeleteRecvMail(UINT8 nIndex);
LOCAL void SKY_API sReadInBoxList(void);

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
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
enum tag_EMAIL_INBOX_POPUP_E {
    EIP_RECV_NEW,
    EIP_RECV,
	EIP_DELE,
	EIP_DELE_ALL,
	EIP_REPLY
};


void SKY_API SKA_MainMailInBox(SKY_EVENT_T *pEvent)
{
    static UINT8    nWhere;
    static BOOL     fGetMail, fAccount, fDelete, fDeleteAll;
#ifdef EMAL_PARSING_TEST
    static HFILE   hFile;   // Temp
	UINT16	nSize;
#endif

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_MENU_CONFIRM_REPLY,    
                      SKY_GET_TOKEN(TKN_EMAIL_IN_BOX), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        sReadInBoxList();

        fGetMail = FALSE;
        fAccount = FALSE;
        fDelete = fDeleteAll = FALSE;
        g_fIdle = FALSE;
        SKY_SET_EMAIL_INPROG(TRUE);//g_fEmailInProg = TRUE;

        if(sAddMailOutBoxControls() == 0)
        {
            if(!SKY_IS_EMAIL_ACCOUNT())
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_NO_ACCOUNT), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // User ID Check
            if(!STRLEN(SKY_GET_EMAIL_USERID()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_NAME_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // User Password Check
            if(!STRLEN(SKY_GET_EMAIL_PASSWORD()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_PASSWD_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // POP3 server check
            if(!STRLEN(SKY_GET_EMAIL_POP3()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_POP3_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }

            fGetMail = TRUE;

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_INBOX_NOMAIL), 
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else 
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    case SE_APP_END :   /* The very last event */ 
        SKY_SET_EMAIL_INPROG(FALSE);//g_fEmailInProg = FALSE;
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(nWhere == EIP_RECV || nWhere == EIP_RECV_NEW)
        {
            sReadInBoxList();

            EQS_DeleteControl(EQS_GetNthIOC(0));
            sAddMailOutBoxControls();
            EQS_SetIOCActive(EQS_GetNthIOC(0));

            if(g_ReceiveOK == EIR_RECVOK)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_SUCCESS), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            else if(g_ReceiveOK == EIR_NOMAIL)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NO_MAIL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            else if(g_ReceiveOK == EIR_USERERR)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_POP3_USER), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            else if(g_ReceiveOK == EIR_PASSERR)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_POP3_PASS), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            else
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_FAIL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
        }
		else 
		{
            UINT8 nIndex;

            nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
            sReadInBoxList();

            EQS_DeleteControl(EQS_GetNthIOC(0));
            if(sAddMailOutBoxControls() == 0)
            {
                /* rosa 2001-10-31 --> @why, what */
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL),
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                //EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_INBOX_NOMAIL), 
                    //MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                /* <-- */
            }
            else
            {
                if(nIndex > EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1)
                    nIndex = EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1;

                EQC_SetScrollLogicalIndex(EQS_GetNthIOC(0), nIndex);
                EQS_SetIOCActive(EQS_GetNthIOC(0));
            }
		}

        g_ReceiveOK = EIR_RECVERR;
        break;

    case SE_IO_MESGBOX_CLEAR:
        if(fGetMail)
        {
            EQS_End();
            break;
        }
        // go through..
    case SE_IO_MESGBOX_END:
        if(g_fIdle)
        {
            g_fIdle = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;
        }
        else if(fAccount)
        {
            fAccount = FALSE;
            EQS_START_SIBLING( SKA_MainEMailAccount, FALSE );
        }
        else if(fDeleteAll)
        {
            fDeleteAll = FALSE;
			SKY_DeleteInBoxMail();

			EQS_DeleteControl(EQS_GetNthIOC(0));
			sAddMailOutBoxControls();

			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_MAILBOX_DELETE_DONE), 
			  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(fDelete)
        {
            UINT8 nIndex;

            fDelete = FALSE;
			sDeleteRecvMail((UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0)));

            nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
			EQS_DeleteControl(EQS_GetNthIOC(0));
			sAddMailOutBoxControls();

            if(nIndex > EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1)
                nIndex = EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1;

            EQC_SetScrollLogicalIndex(EQS_GetNthIOC(0), nIndex);
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_DELETE), 
					MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else
            EQS_SetIOCActive(EQS_GetNthIOC(0));

        /* rosa 2001-10-23 --> @why, what */
        if(EQC_GetScrollDataLine(EQS_GetNthIOC(0)) == 0) //받은 편지함에 편지가 전혀 없을 경우
            EQS_SetWindowCML(CL_MENU_NONE_NONE);
        /* <-- */

        break;

    case SE_IO_SCROLL_LIST_SELECT:  // 확인
		if(!g_MailNum)
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
		}
		else
		{
			nWhere = EIP_REPLY;
			sMakeMailFromRecvFile((UINT8)pEvent->dwParam);
            EQS_START_CHILD_EVENT(sSKA_ViewRecvMail, SE_APP_START, (WORD)pEvent->dwParam, 0);  
		}
        break;

    case SE_IO_SCROLL_LIST_CANCEL:  
        if(pEvent->dwParam != EQC_DELETED)
        {
            EQS_End();
        }
        break;

    case SE_SOFTKEY_MENU: 
        {
            BOOL paActive[4] = {TRUE, TRUE, FALSE, FALSE};

            SKY_TOKEN paMenu[4] =
            {
                TKN_EMAIL_RECV_NEW_MAIL,
                TKN_EMAIL_RECV_MAIL,
                TKN_EMAIL_RECV_DELETE_MAIL,
				TKN_EMAIL_RECV_DELETE_ALLMAIL
            };
            if(g_MailNum)
                EQC_PopUpMenu((EQS_TOKEN_EX *)paMenu, 4, 0);
            else
                EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paMenu, 4, 0, paActive);
        }
        break;
      
    case SE_IO_POPUP_SELECT:
        nWhere = (UINT8)pEvent->dwParam;

        if(nWhere == EIP_RECV || nWhere == EIP_RECV_NEW)
        {
            // 계정 check
            if(!SKY_IS_EMAIL_ACCOUNT())
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_NO_ACCOUNT), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // User ID Check
            if(!STRLEN(SKY_GET_EMAIL_USERID()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_NAME_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // User Password Check
            if(!STRLEN(SKY_GET_EMAIL_PASSWORD()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_PASSWD_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // POP3 server check
            if(!STRLEN(SKY_GET_EMAIL_POP3()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_POP3_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
        }

        switch(nWhere){
        case EIP_RECV:     // 메일받기
        case EIP_RECV_NEW:
            // Temp.... Mail Parsing

#if defined(WIN32) && defined (EMAL_PARSING_TEST)
            nSize = SKY_GetDataFileSize(SDF_MAIL, (BYTE*)"Test");
            SKY_OpenDataFile(SDF_MAIL, (BYTE*)"Test", &hFile, SFT_READ);
            SKY_ReadDataBlock(hFile, g_RecvBuffer, nSize);
            SKY_CloseDataFile(hFile);
            SKY_ParseEmail(nSize);

            EQS_START_CHILD(sSKA_ViewRecvMail);
			break;
#endif

			if(SKY_IS_NOSERVICE())
			{
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NO_SERVICE), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				break;
			}
            else if(SKY_IS_MAINTREQ())
		    {    
		       EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
		                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);    
               break;
		    }
            g_ReceiveOK = EIR_RECVERR;
            SKY_ConnectPOP3Server();
            SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
            break;  
        case EIP_DELE:     // 삭제
			if(!g_MailNum)
			{
				EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL), 
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
			}
			else
			{
				EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_MAIL_DELETE), 
								MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
			}
            break;
		case EIP_DELE_ALL:
			if(!g_MailNum)
			{
				EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL), 
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
			}
			else
			{
				EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_MAILBOX_DELETE), 
				  MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
			}
			break;
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(fGetMail)
        {
            EQS_End();
            break;
        }
        else
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SetIOCActive(EQS_GetNthIOC(0));

#if defined(WIN32) && defined (EMAL_PARSING_TEST)
            nSize = SKY_GetDataFileSize(SDF_MAIL, (BYTE*)"Test");
            SKY_OpenDataFile(SDF_MAIL, (BYTE*)"Test", &hFile, SFT_READ);
            SKY_ReadDataBlock(hFile, g_RecvBuffer, nSize);
            SKY_CloseDataFile(hFile);
            SKY_ParseEmail(nSize);

            EQS_START_CHILD(sSKA_ViewRecvMail);
			break;
#endif

        if(fGetMail)
        {
            fGetMail = FALSE;

            // 계정 check
            if(!SKY_IS_EMAIL_ACCOUNT())
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_NO_ACCOUNT), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // User ID Check
            if(!STRLEN(SKY_GET_EMAIL_USERID()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_NAME_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // User Password Check
            if(!STRLEN(SKY_GET_EMAIL_PASSWORD()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_PASSWD_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }
            // POP3 server check
            if(!STRLEN(SKY_GET_EMAIL_POP3()))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_POP3_ERR), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fAccount = TRUE;
                break;
            }

			if(SKY_IS_NOSERVICE())
			{
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NO_SERVICE), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				break;
			}
            else if(SKY_IS_MAINTREQ())
		    {    
		       EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
		                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);    
               break;
		    }
            nWhere = EIP_RECV;
            g_ReceiveOK = EIR_RECVERR;
            SKY_ConnectPOP3Server();
            SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
        }
		else if(nWhere == EIP_DELE_ALL)
		{
            fDeleteAll = TRUE;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_DELETING), 
                            0, MBT_NONE, MBT_HELP);
		}
		else if(nWhere == EIP_DELE)
		{
            fDelete = TRUE;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_DELETING), 
                            0, MBT_NONE, MBT_HELP);
		}
		break;

    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        if(SKY_IS_SOCK_MODE())
        {
            MSG_HIGH("InBox:SE_CALL_ENDED",0,0,0);
            SKY_DisconnectServer();
        }
    case SE_SOCK_RELEASED:
        if(SKY_IS_SOCK_MODE())
        {
            SKY_AcceptKey(TRUE);   // Originated 를 받으면 TRUE로 됨
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_FAIL), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
        }
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        MSG_HIGH("InBox:SE_CALL_ORIGINATED",0,0,0);
        if(nWhere == EIP_RECV)
            EQS_START_CHILD_EVENT(sSKA_EMailOriginated, SE_APP_START, EMAIL_RECEIVE, 0);  
        else if(nWhere == EIP_RECV_NEW)
            EQS_START_CHILD_EVENT(sSKA_EMailOriginated, SE_APP_START, EMAIL_RECEIVE_NEW, 0);  
        else
            break;

        SKY_SET_ORIGINATE(TRUE);    // ui.originate = TRUE; 

		curr_call.ui_try_cnt = 0;
	    curr_call.secret     = FALSE;
	    curr_call.num.len    = 0;
	    curr_call.alpha.len  = 0;
	    curr_call.connect_time = 0;  
        break;

    case SE_SOFTKEY_REPLY:  // 회신
		if(!g_MailNum)
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL), 
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
		}
		else
		{
			nWhere = EIP_REPLY;

			sMakeMailFromRecvFile((UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0)));
			EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, 
								  NULL, g_MailListNum[(UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0))]);  
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
**     sSKA_ViewRecvMail()
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
LOCAL void SKY_API sSKA_ViewRecvMail(SKY_EVENT_T *pEvent)
{
    static UINT8    nIndex;
    static BOOL fEndMessage, fDelete;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_DELETE_NONE_REPLY,    
                      SKY_GET_TOKEN(TKN_EMAIL_IN_BOX), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        nIndex = pEvent->wParam;
        fEndMessage = FALSE;
        fDelete = FALSE;

        sAddRecvEMailControls();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_SOFTKEY_DELETE:
		EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_MAIL_DELETE), 
						MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_END:
        if(fEndMessage)
        {
            EQS_End();
        }
        else if(fDelete)
        {
            fDelete = FALSE;
		    sDeleteRecvMail(nIndex);

		    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_DELETE), 
				    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_YES:
        fDelete = TRUE;
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_DELETING), 
                        0, MBT_NONE, MBT_HELP);
        break;

    case SE_SOFTKEY_REPLY:
		sMakeMailFromRecvFile(nIndex);
		EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, 
							  NULL, g_MailListNum[nIndex]);  
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL void SKY_API sAddRecvEMailControls(void)
{
    HCONTROL    hControl;
    BYTE    pszTmp[20];

    // 받은시간
    sprintf((char*)pszTmp, "%s", g_RecvMail.szDate);

    EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), 
        pszTmp);

    // 보낸사람
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_SEND_SENDER);

    hControl = EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 
                1, TEM_ENGLISH_LOWER, 
                 MAIL_RECEIVER-1, g_RecvMail.szSender, 0);

    EQC_FitTextEditLine(hControl);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SET_STYLE(hControl,
                 CS_TEXT_EDIT_DEFAULT|CS_TEXT_EDIT_READONLY);    

    // 제목
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_SEND_TITLE);

    hControl = EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH
                , 1, TEM_KOREAN, 
                 MAIL_TITLE-1, g_RecvMail.szTitle, 0);
    EQC_FitTextEditLine(hControl);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SET_STYLE(hControl,
                 CS_TEXT_EDIT_DEFAULT|CS_TEXT_EDIT_READONLY);    
    
    // 내용
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_SEND_TEXT);

    hControl = EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 
                3, TEM_KOREAN, 
                 MAIL_TEXT-1, g_RecvMail.szText, 0);
    EQC_FitTextEditLine(hControl);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SET_STYLE(hControl,
                 CS_TEXT_EDIT_DEFAULT|CS_TEXT_EDIT_READONLY);       

    EQS_AutoFormatting(7, EHA_LEFT, EVA_CENTER);
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
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
void SKY_API SKA_MainMailOutBox(SKY_EVENT_T *pEvent)
{
    static BOOL fEndMessage, fSelectMail, fDelete;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_DELETE_MODIFY_SEND, 
                      SKY_GET_TOKEN(TKN_EMAIL_SENT_BOX), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        sReadOutBoxList();
        if( g_MailNum == 0)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL), 
              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            fEndMessage = TRUE;
            break;
        }

        fEndMessage = FALSE;
        fSelectMail = FALSE;
        fDelete = FALSE;
        g_fIdle = FALSE;
        SKY_SET_EMAIL_INPROG(TRUE);//g_fEmailInProg = TRUE;
        sAddMailOutBoxControls();
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    case SE_APP_END :   /* The very last event */ 
        SKY_SET_EMAIL_INPROG(FALSE);//g_fEmailInProg = FALSE;
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(!fSelectMail)
        {
            if(g_SendOK == ESR_RCPTERR)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SMTP_RCPTERR), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            else if(g_SendOK == ESR_SENDOK)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_SUCCESS), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            else
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_FAIL_NOTSAVE), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

            fEndMessage = TRUE;
        }
        else
        {
            UINT8 nIndex;

            if(g_MailNum == 0)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fEndMessage = TRUE;
                break;
            }
            nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
            EQS_DeleteControl(EQS_GetNthIOC(0));
            sAddMailOutBoxControls();

            if(nIndex > EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1)
                nIndex = EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1;

            EQC_SetScrollLogicalIndex(EQS_GetNthIOC(0), nIndex);
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        }

        fSelectMail = FALSE;
        break;

    case SE_IO_MESGBOX_END:
        if(g_fIdle)
        {
            g_fIdle = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;
        }
        if(fEndMessage)
        {
            EQS_End();
        }
        else if(fDelete)
        {
            UINT8 nIndex;
            fDelete = FALSE;

		    sDeleteSendMail((UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0)));
            if(g_MailNum == 0)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_NOMAIL), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                fEndMessage = TRUE;
                break;
            }

            nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
            EQS_DeleteControl(EQS_GetNthIOC(0));
            sAddMailOutBoxControls();

            if(nIndex > EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1)
                nIndex = EQC_GetScrollMaxDataLine(EQS_GetNthIOC(0)) -1;

            EQC_SetScrollLogicalIndex(EQS_GetNthIOC(0), nIndex);

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_DELETE), 
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        else
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_SOFTKEY_DELETE:     // 삭제
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_MAIL_DELETE), 
                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;

    case SE_IO_MESGBOX_YES:
        fDelete = TRUE;
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_DELETING), 
                        0, MBT_NONE, MBT_HELP);
        break;

    case SE_IO_MESGBOX_NO:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_SOFTKEY_SEND:    // 전송
        if(sMakeMailFromSendFile((UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0))))
        {
			if(SKY_IS_NOSERVICE())
			{
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NO_SERVICE), 
                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				break;
			}
            else if(SKY_IS_MAINTREQ())
		    {    
		       EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
		                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);    
               break;
		    }

            SKY_ConnectSMTPServer();
            SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
            g_SendOK = ESR_FAIL;
        }
        break;

    case SE_IO_SCROLL_LIST_SELECT:  // 확인
        fSelectMail = TRUE;

        sMakeMailFromSendFile((UINT8)pEvent->dwParam);
        EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, 
                              g_MailListNum[pEvent->dwParam], NULL);  
        break;
        
    case SE_IO_SCROLL_LIST_CANCEL:  
        if(pEvent->dwParam != EQC_DELETED)
        {
            EQS_End();
        }
        break;

    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        if(SKY_IS_SOCK_MODE())
        {
            SKY_DisconnectServer();
        }
    case SE_SOCK_RELEASED:
        if(SKY_IS_SOCK_MODE())
        {
            SKY_AcceptKey(TRUE);   // Originated 를 받으면 TRUE로 됨
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_FAIL_NOTSAVE), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
        }
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        SKY_SET_ORIGINATE(TRUE);    // ui.originate = TRUE; 

		curr_call.ui_try_cnt = 0;
	    curr_call.secret     = FALSE;
	    curr_call.num.len    = 0;
	    curr_call.alpha.len  = 0;
	    curr_call.connect_time = 0;  

        EQS_START_CHILD_EVENT(sSKA_EMailOriginated, SE_APP_START, EMAIL_SEND, 0);  
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL UINT8 SKY_API sAddMailOutBoxControls(void)
{
    UINT8   i, nCount;

    for(i = 0, nCount = 0; i < MAX_SAVED_MAIL; i++)
    {
        if(g_MailListNum[i] > 0 && g_MailListNum[i] <= MAX_INBOX_MAIL) //(g_MailListNum[i] > 0)
        {
            l_MailScroll[nCount] = g_MailList[g_MailListNum[i]-1].szTitle;
            nCount++;
        }
    }

    EQC_SET_STYLE(EQC_ScrollList(EQS_NA, EQS_NA, 16, 1, l_MailScroll, 
                   nCount, nCount, 0),
                   CS_SCROLL_LIST_DEFAULT|CS_SCROLL_LIST_ENUMBERATED|
                   /* rosa 2001-10-23 --> @why, what */
                   //CS_SCROLL_LIST_MAXIMIZE);
                   CS_SCROLL_LIST_MAXIMIZE|CS_SCROLL_LIST_NO_CML);                   
                   /* <-- */
                   

    EQS_FitCanvasSize();

    return nCount;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
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


void SKY_API SKA_MainMailSend(SKY_EVENT_T *pEvent)
{
    static BOOL fEndMessage, fSearchBook, fAccount, fSave;
    static UINT8    nMailNum, nReplyNum;
    pb_email_info_type email_info;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_MENU_SELECT_SEND, 
                      SKY_GET_TOKEN(TKN_EMAIL_EDIT), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        
        // 계정 check
        if(!SKY_IS_EMAIL_ACCOUNT())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_NO_ACCOUNT), 
              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            fAccount = TRUE;
            g_fIdle = FALSE;
            break;
        }
        // User ID Check
        if(!STRLEN(SKY_GET_EMAIL_USERID()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_NAME_ERR), 
              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            fAccount = TRUE;
            g_fIdle = FALSE;
            break;
        }
        // SMTP server check
        if(!STRLEN(SKY_GET_EMAIL_SMTP()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_SMTP_ERR), 
              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            fAccount = TRUE;
            g_fIdle = FALSE;
            break;
        }

        nMailNum = 0;
        nReplyNum = 0;

        if(pEvent->wParam > 0)
            nMailNum = (UINT8)pEvent->wParam;
        else if(pEvent->dwParam > 0)
            nReplyNum = (UINT8)pEvent->dwParam;

        sAddEMailEditControls(nMailNum, nReplyNum);        

        if(EQS_GET_PARENT() == SKA_ViewBookContent && pEvent->dwParam != NULL)
        {
            email_info.address = pEvent->dwParam;
            ReadEmail(&email_info);

			//redstars-email
            EQC_SetTextEditData(EQS_GetNthIOC(EEM_RECV), email_info.szMailID);
        }

        fEndMessage = FALSE;
        fSearchBook = FALSE;
        fAccount = FALSE;
        g_fIdle = FALSE;
        fSave = FALSE;

        SKY_SET_EMAIL_INPROG(TRUE);//g_fEmailInProg = TRUE;
        break;
        
    case SE_APP_END :   /* The very last event */ 
        SKY_SET_EMAIL_INPROG(FALSE);//g_fEmailInProg = FALSE;
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_KEY_END:        
        g_fIdle = TRUE;
        // go through...
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_EDITCANCEL), 
					    MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            if(g_fIdle)
                EQS_END_TO(EQS_POP_TO_ROOT);
            else
                EQS_End();
        }
        break;

    case SE_APP_RESUME: /* child applet is ended */
        if(EQS_GET_CHILD() == SKA_MainPbookCode)
        {
            if(pEvent->wParam == BOOKCODE_IN_EMAIL)
            {
                fSearchBook = TRUE;
                EQS_START_CHILD(SKA_ViewEmailList);
                break;
            }
        }else
        {
            if(fSearchBook)
            {
                if(pEvent->dwParam != NULL)
                {
                    email_info.address = pEvent->dwParam;
                    ReadEmail(&email_info);

				    //redstars-email
                    EQC_SetTextEditData(EQS_GetNthIOC(EEM_RECV), email_info.szMailID);
                }

                fSearchBook = FALSE;
            }
            else
            {
                if(g_SendOK == ESR_SENDOK)
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_SUCCESS), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

                else if(g_SendOK == ESR_RCPTERR)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SMTP_RCPTERR), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

				    if(!nMailNum)
				    {
					    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_FAIL), 
									    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

					    sReadOutBoxList();
					    nMailNum = sSaveCurrentSendMail(0);

					    if(nMailNum == 0)
						    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_FULL), 
										    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
				    }
				    else
				    {
					    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_FAIL_NOTSAVE), 
									    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
				    }
                }
                else 
                {
				    if(!nMailNum)
				    {
					    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_FAIL), 
									    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);

					    sReadOutBoxList();
					    nMailNum = sSaveCurrentSendMail(0);

					    if(nMailNum == 0)
						    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_FULL), 
										    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
				    }
				    else
				    {
					    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_FAIL_NOTSAVE), 
									    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
				    }
                }

                fEndMessage = TRUE;
            }
        }
        break;

    case SE_IO_MESGBOX_YES:
        if(g_fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else
            EQS_End();
        break;

    case SE_IO_MESGBOX_END:
        if(g_fIdle)
        {
            g_fIdle = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;
        }
        if(fAccount)
        {
            fAccount = FALSE;
            EQS_START_SIBLING( SKA_MainEMailAccount, FALSE );
        }
        else if(fSave)
        {
            fSave = FALSE;

            sReadOutBoxList();

            nMailNum = sSaveCurrentSendMail(nMailNum);
            if(nMailNum == 0)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_FULL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            else
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_SAVE), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(fEndMessage)
        {
            fEndMessage = FALSE;
            EQS_End();
        }
        break;
        
    case SE_SOFTKEY_MENU:
        {
            BOOL paActive[2] = {TRUE, FALSE};

            SKY_TOKEN paMenu[2] =
            {
                TKN_EMAIL_SEND_SAVE_MAIL,
                TKN_EMAIL_SEND_PHONEBOOK
            };

            if(EQS_GetFocusedIOC() == EQS_GetNthIOC(EEM_RECV))
                EQC_PopUpMenu((EQS_TOKEN_EX *)paMenu, 2, 0);
            else
                EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paMenu, 2, 0, paActive);
        }
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam){
        case 0: // 저장
            if(!sMakeEmailMessage())
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_RECEIVER_ERR), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;
            }

            fSave = TRUE;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_SAVING), 
                            0, MBT_NONE, MBT_HELP);

/*
            nMailNum = sSaveCurrentSendMail(nMailNum);
            if(nMailNum == 0)
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_FULL), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            else
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_OUTBOX_SAVE), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
*/
            break;
        case 1: // 전화부
            if(!SKY_IS_PHONE_BOOK_LOCK())
            {
                fSearchBook = TRUE;
                EQS_START_CHILD(SKA_ViewEmailList);
            }else
            {
                EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, \
                    BOOKCODE_IN_EMAIL, 1);            
            }
            break;
        }
        break;

    case SE_SOFTKEY_SEND:   // 메일 전송
		if(SKY_IS_NOSERVICE())
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NO_SERVICE), 
              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			break;
		}
        else if(SKY_IS_MAINTREQ())
		{    
		   EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
		              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);    
           break;
		}

        if(sMakeEmailMessage())
        {
            SKY_ConnectSMTPServer();
            SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
            g_SendOK = ESR_FAIL;
        }
        else
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_RECEIVER_ERR), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        break;

    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        if(SKY_IS_SOCK_MODE())
        {
            SKY_DisconnectServer();
        }
    case SE_SOCK_RELEASED:
        if(SKY_IS_SOCK_MODE())
        {
            SKY_AcceptKey(TRUE);   // Originated 를 받으면 TRUE로 됨
		    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SEND_FAIL_NOTSAVE), 
						    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
        }
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        SKY_SET_ORIGINATE(TRUE);    // ui.originate = TRUE; 

		curr_call.ui_try_cnt = 0;
	    curr_call.secret     = FALSE;
	    curr_call.num.len    = 0;
	    curr_call.alpha.len  = 0;
	    curr_call.connect_time = 0;  

        EQS_START_CHILD_EVENT(sSKA_EMailOriginated, SE_APP_START, EMAIL_SEND, 0);  
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL  void sSKA_EMailOriginated(SKY_EVENT_T *pInEvent)
{
    static UINT8    nMode;
    static BOOL     fConnect;
    HCONTROL    hControl;

    switch(pInEvent->EventID)
    {
        case SE_APP_START : /* The very first event */
            EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        NULL, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

            nMode = (UINT8)pInEvent->wParam;

            //EQS_StartTimer(EQS_TIMER_1, 5, FALSE);
/*
            hControl = EQC_Animation(30, 9, &g_AniData59x36x256C, NULL);
            EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
            EQC_StartAnimation(hControl, TRUE);
*/
            /* rosa 2001-09-26 --> @why, what */
            //hControl = EQC_Image(30, 9, (BITMAP*)&g_BmpConnectDataService);            
            hControl = EQC_Image(EMAIL_ANI_X, EMAIL_ANI_Y, (BITMAP*)&g_BmpConnectDataService);
            //EQC_Image(EMAIL_FRAME_X, EMAIL_FRAME_Y, (BITMAP*)&g_BmpEframe115x15x256c);            

            EQC_StaticText(GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_CALL_DS_ORIGINATE))), 
                           EMAIL_STATUS_Y, 
                           (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
            /* <-- */

            fConnect = FALSE;
            SKY_SET_NO_OFF_BACKLIGHT(TRUE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            break;

        case SE_APP_END : /* The very last event */
            SKY_AcceptKey(TRUE);   // connect 시점에서 False를 해놨을므로 꼭 True로 돌려줘야 함.
            if(!fConnect)
            {
                SKY_SET_NO_OFF_BACKLIGHT(FALSE);
                SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            }
            break;
        
        case SE_APP_DRAW :    		
            break;

        case SE_KEY_0 ://HS_0_K:
        case SE_KEY_1 ://HS_1_K:
        case SE_KEY_2 ://HS_2_K:
        case SE_KEY_3 ://HS_3_K:
        case SE_KEY_4 ://HS_4_K:
        case SE_KEY_5 ://HS_5_K:
        case SE_KEY_6 ://HS_6_K:
        case SE_KEY_7 ://HS_7_K:
        case SE_KEY_8 ://HS_8_K:
        case SE_KEY_9 ://HS_9_K:
        case SE_KEY_STAR ://HS_STAR_K:
        case SE_KEY_POUND ://HS_POUND_K:
        case SE_KEY_UP ://HS_UP_K:
        case SE_KEY_DOWN ://HS_DOWN_K:
        // ignore all these events
            break;

        case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
            break;

        case SE_SOFTKEY_LEFT ://HS_PF1_K:
        case SE_SOFTKEY_RIGHT ://HS_PF2_K:
        case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
            // ignore all these events
            break;

        case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
        case SE_KEY_END ://HS_END_K:
        case SE_IO_TO_IDLE:		// From MessageBox
            g_fIdle = TRUE;
        case SE_KEY_CLEAR :// HS_CLR_K:
            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            break;

        case SE_KEY_SEND ://HS_SEND_K:
        case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
        case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
        case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
        case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
        case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        case SE_CALL_FLASHED :   //UI_CALL_FLASHED_E:
        case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
            break;

        case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
            if (GET_EVENT_PARAM()->call_cmd_err.cmd == CM_CALL_CMD_END )
            {          
                SKY_ERROR_TONE();
            }
            break;

        case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
            MSG_HIGH("Originated:SE_CALL_CONNECT",0,0,0);
            l_startTime = clk_uptime();
            fConnect = TRUE;

            // 통화연결음
            if(SKY_IS_CONNECT_WARNING_SIGNAL())
            {
                if(!SKY_IS_ETIQUETTE())
                {
                #if (MODEL_ID == MODEL_B0)
                    SKY_PlaySoundEx(UI_MUSIC_SND, MUS_TELEPHONE1, 1, 1);
                /* rosa 2001-10-31 --> @why, what */
                #elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
                /* <-- */
                    SKY_PlayMusicEx( MUS_DENWA_1, 1, 1);
                #endif
                }
            }

            switch(nMode){
            case EMAIL_SEND:
                EQS_START_SIBLING( sSKA_EMailSMTPInProg, FALSE );
                break;
            case EMAIL_RECEIVE:
                EQS_START_SIBLING_EVENT(sSKA_EMailPOP3InProg, SE_APP_START, EMAIL_RECEIVE, 0, FALSE);  
                break;
            case EMAIL_RECEIVE_NEW:
                EQS_START_SIBLING_EVENT(sSKA_EMailPOP3InProg, SE_APP_START, EMAIL_RECEIVE_NEW, 0, FALSE);  
                break;
            }
            break;

         //case SE_TM_TIMER1:
         case SE_CALL_ENDED ://UI_CALL_ENDED_E:
 /* rosa 2001-10-31 --> @why, what */
#if (MODEL_ID == MODEL_B2) || (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
 /* <-- */
            curr_call.cm_info = GET_EVENT_PARAM()->call_event.call_info;                
#else  /* (MODEL_ID == MODEL_B2) || (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
//          curr_call.cm_info = ui.evt_param.call_event.call_info;                
#endif /* (MODEL_ID == MODEL_B2) || (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            break;

        case SE_TCPCONNECT_FAIL:
        case SE_SOCK_RELEASED:
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
			EQS_End();
            break;

        default:
            EQS_DEF_EVENT_PROC(pInEvent);
            break;
        }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_EMailPOP3InProg()
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


LOCAL void SKY_API sSKA_EMailPOP3InProg(SKY_EVENT_T *pEvent)
{
    static BOOL fFail;
    static BOOL fNew, fTooMany, fDisconnect;
    static BOOL fConnected;
    static HCONTROL    hControl;
    static HCONTROL    hAniControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    NULL, 
                    NULL, // show annunciators
                    WA_SHOW_HEADER);

        //EQC_Image(EMAIL_ANI_BACK_X, EMAIL_ANI_BACK_Y, (BITMAP*)&g_BmpEbase41x64x256c);
        //EQC_Image(EMAIL_FRAME_X, EMAIL_FRAME_Y, (BITMAP*)&g_BmpEframe115x15x256c);

        /* rosa 01-08-17 --> @why, what */
        //hAniControl = EQC_Animation(EMAIL_ANI_X, EMAIL_ANI_Y, 
        //            &g_AniMailRece65x37x256c, NULL);

        hAniControl = EQC_Animation(EMAIL_ANI_X, EMAIL_ANI_Y, 
                    &g_AniReceiveEMail, NULL);
        /* <-- */
        EQC_SET_STYLE(hAniControl, CS_ANIMATION_REPEAT);
        EQC_StartAnimation(hAniControl, TRUE);

        /* rosa 2001-09-26 --> @why, what */
        //hControl = EQC_StaticText(EMAIL_STATUS_X, EMAIL_STATUS_Y, TKN_EMAIL_CONNECTING);
        hControl = EQC_StaticText(GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_CONNECTING))), 
                                  EMAIL_STATUS_Y, 
                                  TKN_EMAIL_CONNECTING);
        EQC_SET_STYLE(hControl, CS_STATIC_TEXT_DEFAULT|CS_STATIC_TEXT_CENTER);
        /* <-- */
        EQS_StartTimer(EQS_TIMER_1, EMAIL_WAIT_TIME, FALSE);

        fFail = FALSE;
        fConnected = FALSE;
        fTooMany = FALSE;
        fDisconnect = TRUE;
        if(pEvent->wParam == EMAIL_RECEIVE_NEW)
            fNew = TRUE;
        else
            fNew = FALSE;
        break;
        
    case SE_APP_END :   /* The very last event */ 
        UpdateOriginatedDataCallInfo(clk_uptime() - l_startTime);   // 데이터발신 통화시간

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_END:
    case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
    case SE_SYS_OFFLINE:
    case SE_IO_TO_IDLE:		// From MessageBox
        g_fIdle = TRUE;
    case SE_KEY_CLEAR:
        fFail = TRUE;

        if(fConnected)
        {
            SKY_SendRequest(EPS_QUIT, FALSE, fNew, hControl);
            EQS_StartTimer(EQS_TIMER_3, EMAIL_WAIT_ENDTIME, 0);
            fConnected = FALSE;
        }
        else
        {
            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        }
        break;

    case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
    case SE_CALL_INCOMING:/*UI_CALL_INCOMING_E:*/		   
        break;

	case SE_IO_MESGBOX_YES:
        if(fTooMany)
            fTooMany = FALSE;
        else
    		SKY_DeleteInBoxMail();

		SKY_SendRequest(EPS_NONE, FALSE, fNew, hControl);
		break;

	case SE_IO_MESGBOX_NO:
	case SE_IO_MESGBOX_CANCEL:
	case SE_IO_MESGBOX_CLEAR:
        if(g_MailNum == MAX_SAVED_MAIL)
        {
            SKY_SendRequest(EPS_QUIT, FALSE, fNew, hControl);
            fFail = TRUE;

            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        }
        else
        {
            SKY_SendRequest(EPS_NONE, FALSE, fNew, hControl);
        }
		break;

    case SE_RECV_DATA:
        //EQS_StartTimer(EQS_TIMER_1, EMAIL_WAIT_TIME, FALSE);

        switch(SKY_ReceivePOP3Reply(hControl, fNew)){
        case ERR_NEXT:
            SKY_SendRequest(EPS_NONE, FALSE, fNew, hControl);
            break;

        case ERR_CONTINUE: 
            break;

        case ERR_RECVOK:
            if(!fFail)
                g_ReceiveOK = EIR_RECVOK;

            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            break;

		case ERR_FULL:
            if(g_MailNum)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_MAILBOX_FULL), 
                                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            }
            else
            {
                fTooMany = TRUE;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_MAIL_TOOMANY), 
                                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            }
			break;

        case ERR_NOMAIL:
        case ERR_NONEWMAIL:
            g_ReceiveOK = EIR_NOMAIL;
            fFail = TRUE;
            break;

        case ERR_FAIL:
            fFail = TRUE;
            break;

        case ERR_USER:
            g_ReceiveOK = EIR_USERERR;
            fFail = TRUE;
            break;

        case ERR_PASS:
            g_ReceiveOK = EIR_PASSERR;
            fFail = TRUE;
            break;
        }

        if(fFail)
        {
            SKY_SendRequest(EPS_QUIT, FALSE, fNew, hControl);

            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        }
        break;

    case SE_TCPCONNECT_SUCCESS:
        MSG_HIGH("InProg:SE_TCPCONNECT_SUCCESS",0,0,0);
        SKY_SET_SOCK_CONNECT(TRUE);
        fConnected = TRUE;
        EQS_StartTimer(EQS_TIMER_1, EMAIL_WAIT_TIME, 0);
        SKY_SendRequest(EPS_READY, FALSE, fNew, hControl);
        break;

    case SE_CALL_ENDED:
    case SE_TM_TIMER3:      // Quit timeout
        MSG_HIGH("InProg:SE_CALL_ENDED",0,0,0);
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        SKY_DisconnectServer();
        break;

    case SE_TCPCONNECT_FAIL:
    case SE_SOCK_RELEASED:
        SKY_SET_SOCK_CONNECT(FALSE);
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        fDisconnect = FALSE;
        // go through..
        EQS_End();
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
LOCAL void SKY_API sSKA_EMailSMTPInProg(SKY_EVENT_T *pEvent)
{
    static HCONTROL    hControl;
    static HCONTROL    hAniControl;
    static BOOL        fDisconnect = TRUE;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE, 
                    NULL, // show annunciators
                    WA_SHOW_HEADER);

        //EQC_Image(EMAIL_ANI_BACK_X, EMAIL_ANI_BACK_Y, (BITMAP*)&g_BmpEbase41x64x256c);
        //EQC_Image(EMAIL_FRAME_X, EMAIL_FRAME_Y, (BITMAP*)&g_BmpEframe115x15x256c);

        /* rosa 01-08-17 --> @why, what */
        //hAniControl = EQC_Animation(EMAIL_ANI_X, EMAIL_ANI_Y, 
        //            &g_AniMailSend65x37x256c, NULL);
        hAniControl = EQC_Animation(EMAIL_ANI_X, EMAIL_ANI_Y, 
                    &g_AniSendEMail, NULL);
        /* <-- */

        EQC_SET_STYLE(hAniControl, CS_ANIMATION_REPEAT);
        EQC_StartAnimation(hAniControl, TRUE);

        /* rosa 2001-09-26 --> @why, what */
        //hControl = EQC_StaticText(EMAIL_STATUS_X, EMAIL_STATUS_Y, TKN_EMAIL_CONNECTING);
        hControl = EQC_StaticText(GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_CONNECTING))), 
                                  EMAIL_STATUS_Y, 
                                  TKN_EMAIL_CONNECTING);
        EQC_SET_STYLE(hControl, CS_STATIC_TEXT_DEFAULT|CS_STATIC_TEXT_CENTER);
        /* <-- */

        EQS_StartTimer(EQS_TIMER_1, EMAIL_WAIT_TIME, FALSE);
        fDisconnect = TRUE;

#ifdef EMAIL_DEBUG
        EQC_StaticText(0, 20, TKN_BLANK);  // State
        EQC_StaticText(0, 40, TKN_BLANK);  // Result
        EQS_AutoFormatting(3, EHA_LEFT, EVA_CENTER);
#endif
        break;
        
    case SE_APP_END :   /* The very last event */ 
        UpdateOriginatedDataCallInfo(clk_uptime() - l_startTime);   // 데이터발신 통화시간

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_END:
    case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
    case SE_SYS_OFFLINE:
    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:
    case SE_IO_TO_IDLE:		// From MessageBox
        SKY_DisconnectServer();
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        break;

    case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
    case SE_CALL_INCOMING:/*UI_CALL_INCOMING_E:*/		   
        break;

    case SE_TM_TIMER1:
        SKY_DisconnectServer();
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        break;

    case SE_RECV_DATA:
        EQS_StartTimer(EQS_TIMER_1, EMAIL_WAIT_TIME, FALSE);

        switch(SKY_ReceiveSMTPReply(hControl)){
        case ESR_NEXT:
            SKY_SendEMail(ESS_NONE, FALSE, hControl);
            break;

        case ESR_CONTINUE: 
            SKY_SendEMail(ESS_NONE, TRUE, hControl);
            break;

        case ESR_SENDOK:
            g_SendOK = ESR_SENDOK;
            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            break;

        case ESR_FAIL:
            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            break;

        case ESR_RCPTERR:
            g_SendOK = ESR_RCPTERR;
            SKY_DisconnectServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            break;
        }
        break;

    case SE_TCPCONNECT_SUCCESS:
        SKY_SET_SOCK_CONNECT(TRUE);
        EQS_StartTimer(EQS_TIMER_1, EMAIL_WAIT_TIME, FALSE);

        /* rosa 2001-09-26 --> @why, what */
        EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SENDING));
	    //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SENDING))));
        /* <-- */
        SKY_SendEMail(ESS_HELO, FALSE, hControl);
        break;

    case SE_CALL_ENDED:
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        SKY_DisconnectServer();
        break;

    case SE_TCPCONNECT_FAIL:
    case SE_SOCK_RELEASED:
        SKY_SET_SOCK_CONNECT(FALSE);
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        fDisconnect = FALSE;
        EQS_End();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL BOOL SKY_API sMakeEmailMessage(void)
{
    UINT16  nLen;

    STRCPY(g_SendMail.szSender, (BYTE*)SKY_GET_EMAIL_ADDRESS());
           
    nLen = EQC_GetTextEditData(EQS_GetNthIOC(EEM_RECV), 
                        g_SendMail.szReceiver);
    if(!nLen)
        return FALSE;

    EQC_GetTextEditData(EQS_GetNthIOC(EEM_TITLE), 
                        g_SendMail.szTitle);

    EQC_GetTextEditData(EQS_GetNthIOC(EEM_TEXT), 
                        g_SendMail.szText);

    if(STRLEN(g_SendMail.szTitle) == 0)
    {
        STRCPY(g_SendMail.szTitle, (BYTE*)"제목없음");
/*
        if(IsMiddleOfMultiByte(g_SendMail.szText, MAIL_TITLE))
            STRNCPY(g_SendMail.szTitle, g_SendMail.szText, MAIL_TITLE);
        else
            STRNCPY(g_SendMail.szTitle, g_SendMail.szText, MAIL_TITLE-1);
*/
    }

    return TRUE;
}



LOCAL void SKY_API sAddEMailEditControls(UINT8 nMailNum, UINT8 nReplyNum)
{
    // 받는 사람
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_SEND_RECEIVER);

    if(nMailNum)
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_RECEIVER-1, g_SendMail.szReceiver, 0);    
    else if(nReplyNum)
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_RECEIVER-1, g_RecvMail.szSender, 0);    
    else
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_RECEIVER-1, (BYTE*)"", 0);    

    // 제목
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_SEND_TITLE);

    if(nMailNum)
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_KOREAN, 
                     MAIL_TITLE-1, g_SendMail.szTitle, 0);    
    else if(nReplyNum)
    {
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_KOREAN, 
                     MAIL_TITLE-1, (BYTE*)"", 0);    
    }
    else
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_KOREAN, 
                     MAIL_TITLE-1, (BYTE*)"", 0);    
    
    // 내용
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_SEND_TEXT);

    if(nMailNum)
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 3, TEM_KOREAN, 
                     MAIL_TEXT-1, g_SendMail.szText, 0);    
    else

        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 3, TEM_KOREAN, 
                     MAIL_TEXT-1, (BYTE*)"", 0);    
/*
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 3, TEM_KOREAN, 
                     44, (BYTE*)"가\r\n나\r\n다\r\n라\r\n마\r\n바\r\n사\r\n아\r\n자\r\n차\r\n카", 0);    
*/
    EQS_AutoFormatting(EEM_MAX, EHA_LEFT, EVA_CENTER);
}

LOCAL void SKY_API sReadOutBoxList(void)
{
    HFILE   hFile;
    UINT8   i;

    g_MailNum = 0;

    if(!SKY_IsDataFile(SDF_MAIL,(BYTE*) OUTBOX_LIST_FILE))
    {
        // Create file
        if(SKY_CreateDataFile(SDF_MAIL, (BYTE*)OUTBOX_LIST_FILE, &hFile) == SDR_OK)
        {
            memset(g_MailList, NULL, sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL);
            memset(g_MailListNum, NULL, sizeof(UINT8)*MAX_SAVED_MAIL);

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
                    (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
                    (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

            SKY_CloseDataFile(hFile);
        }
    }
    else
    {
        if(SKY_OpenDataFile(SDF_MAIL,(BYTE*) OUTBOX_LIST_FILE, &hFile, SFT_READ) == SDR_OK)
        {
            SKY_ReadDataBlock(hFile, (void *)g_MailListNum, 
                    (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

            SKY_ReadDataBlock(hFile, (void *)g_MailList, 
                    (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

            for(i = 0; i < MAX_SAVED_MAIL; i++)
            {
                if(g_MailList[i].nMailNumber != 0)
                    g_MailNum ++;
            }

            SKY_CloseDataFile(hFile);



        }
    }
}


LOCAL UINT8 SKY_API sSaveCurrentSendMail(UINT8 nMailNum)
{
    HFILE   hFile;
    INT     i, nSaveMailNum;
    BYTE    szBuffer[10];

    if(nMailNum == 0)
    {
        for(i = 0; i < MAX_SAVED_MAIL; i++)
        {
            if(g_MailList[i].nMailNumber == 0)
                break;
        }

        if(i == MAX_SAVED_MAIL)
            return 0;

        nSaveMailNum = i+1;

        sprintf((char*)szBuffer, "Sent%2d", nSaveMailNum);
        if(SKY_OpenDataFile(SDF_MAIL, szBuffer, &hFile, SFT_WRITE) != SDR_OK)
        {
            if(SKY_CreateDataFile(SDF_MAIL, szBuffer, &hFile) != SDR_OK)
                return 0;
        }
    }
    else
    {
        nSaveMailNum = nMailNum;
        sprintf((char*)szBuffer, "Sent%2d", nSaveMailNum);
        if(SKY_OpenDataFile(SDF_MAIL, szBuffer, &hFile, SFT_WRITE) != SDR_OK)
        {
            if(SKY_CreateDataFile(SDF_MAIL, szBuffer, &hFile) != SDR_OK)
                return 0;
        }
    }

    g_SendMail.nMailNumber = nSaveMailNum;

    SKY_WriteDataBlock(hFile, (CONST void *)&g_SendMail, 
                (DWORD)sizeof(EMAIL_SEND_T));

    SKY_CloseDataFile(hFile);

    // Change List
    g_MailList[nSaveMailNum-1].nMailNumber = nSaveMailNum;
    STRCPY(g_MailList[nSaveMailNum-1].szReceiver, g_SendMail.szReceiver);
    STRCPY(g_MailList[nSaveMailNum-1].szSender, g_SendMail.szSender);
    STRCPY(g_MailList[nSaveMailNum-1].szTitle, g_SendMail.szTitle);
    g_MailList[nSaveMailNum-1].wTime = SKY_GetLocalTime();

    if(nMailNum == 0)    // Find Index
    {
        for(i = 0; i < MAX_SAVED_MAIL; i++)
        {
            if(g_MailListNum[i] <= 0 || g_MailListNum[i] > MAX_INBOX_MAIL)//(g_MailListNum[i] == 0)
            {
                g_MailListNum[i] = nSaveMailNum;
                break;
            }
        }
    }

    SKY_OpenDataFile(SDF_MAIL,(BYTE*) OUTBOX_LIST_FILE, &hFile, SFT_WRITE);

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
            (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
            (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

    SKY_CloseDataFile(hFile);

    return nSaveMailNum;
}


LOCAL void SKY_API sDeleteSendMail(UINT8 nIndex)
{
    UINT8   nMailNum, i;
    BYTE    szBuffer[10];
    HFILE   hFile;

    g_MailNum = 0;

    nMailNum = g_MailListNum[nIndex];
    sprintf((char*)szBuffer, "Sent%2d", nMailNum);

    if(SKY_IsDataFile(SDF_MAIL, szBuffer))
        SKY_DeleteDataFile(SDF_MAIL, szBuffer);

    // Change List
    if(SKY_OpenDataFile(SDF_MAIL, (BYTE*)OUTBOX_LIST_FILE, &hFile, SFT_WRITE) != SDR_OK)
        SKY_CreateDataFile(SDF_MAIL, (BYTE*)OUTBOX_LIST_FILE, &hFile);;

    for(i = nIndex; i < MAX_SAVED_MAIL-1; i++)
        g_MailListNum[i] = g_MailListNum[i+1];
    g_MailListNum[MAX_SAVED_MAIL-1] = 0;

    g_MailList[nMailNum-1].nMailNumber = 0;

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
            (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
            (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

    SKY_CloseDataFile(hFile);

    for(i = 0; i < MAX_SAVED_MAIL; i++)
    {
        if(g_MailList[i].nMailNumber != 0)
            g_MailNum ++;
    }
}

LOCAL BOOL SKY_API sMakeMailFromSendFile(UINT8 nIndex)
{
    UINT8   nMailNum;
    BYTE    szBuffer[10];
    HFILE   hFile;

    nMailNum = g_MailListNum[nIndex];
    sprintf((char*)szBuffer, "Sent%2d", nMailNum);

    if(!SKY_IsDataFile(SDF_MAIL, szBuffer))
        return FALSE;

    SKY_OpenDataFile(SDF_MAIL, szBuffer, &hFile, SFT_READ);

    SKY_ReadDataBlock(hFile, (void *)&g_SendMail, 
                (DWORD)sizeof(EMAIL_SEND_T));

    SKY_CloseDataFile(hFile);

    return TRUE;
}

LOCAL BOOL SKY_API sMakeMailFromRecvFile(UINT8 nIndex)
{
    UINT8   nMailNum;
    BYTE    szBuffer[10];
    HFILE   hFile;

    nMailNum = g_MailListNum[nIndex];
    sprintf((char*)szBuffer, "Recv%2d", nMailNum);

    if(!SKY_IsDataFile(SDF_MAIL, szBuffer))
        return FALSE;

    SKY_OpenDataFile(SDF_MAIL, szBuffer, &hFile, SFT_READ);

    SKY_ReadDataBlock(hFile, (void *)&g_RecvMail, 
                (DWORD)sizeof(EMAIL_RECV_T));

    SKY_CloseDataFile(hFile);

    return TRUE;
}

LOCAL void SKY_API sDeleteRecvMail(UINT8 nIndex)
{
    UINT8   nMailNum, i;
    BYTE    szBuffer[10];
    HFILE   hFile;

    g_MailNum = 0;

    nMailNum = g_MailListNum[nIndex];
    sprintf((char*)szBuffer, "Recv%2d", nMailNum);

    if(SKY_IsDataFile(SDF_MAIL, szBuffer))
        SKY_DeleteDataFile(SDF_MAIL, szBuffer);

    // Change List
    if(SKY_OpenDataFile(SDF_MAIL, (BYTE*)INBOX_LIST_FILE, &hFile, SFT_WRITE) != SDR_OK)
        SKY_CreateDataFile(SDF_MAIL, (BYTE*)INBOX_LIST_FILE, &hFile);

    for(i = nIndex; i < MAX_SAVED_MAIL-1; i++)
        g_MailListNum[i] = g_MailListNum[i+1];
    g_MailListNum[MAX_SAVED_MAIL-1] = 0;

    g_MailList[nMailNum-1].nMailNumber = 0;

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
            (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
            (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

    SKY_CloseDataFile(hFile);

    for(i = 0; i < MAX_SAVED_MAIL; i++)
    {
        if(g_MailList[i].nMailNumber != 0)
            g_MailNum ++;
    }
}

void SKY_API SKY_DeleteInBoxMail(void)
{
    UINT8   i;
    BYTE    szBuffer[10];
    HFILE   hFile;

    for(i = 1; i <= MAX_SAVED_MAIL; i++)
    {
        sprintf((char*)szBuffer, "Recv%2d", i);

        if(SKY_IsDataFile(SDF_MAIL, szBuffer))
            SKY_DeleteDataFile(SDF_MAIL, szBuffer);
    }

    memset(g_MailList, NULL, sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL);
    memset(g_MailListNum, NULL, sizeof(UINT8)*MAX_SAVED_MAIL);

	g_MailNum = 0;

    if(!SKY_IsDataFile(SDF_MAIL,(BYTE*) INBOX_LIST_FILE))
        SKY_CreateDataFile(SDF_MAIL, (BYTE*)INBOX_LIST_FILE, &hFile);
    else
        SKY_OpenDataFile(SDF_MAIL,(BYTE*) INBOX_LIST_FILE, &hFile, SFT_WRITE);

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
            (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
            (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

    SKY_CloseDataFile(hFile);
}

LOCAL void SKY_API sReadInBoxList(void)
{
    HFILE   hFile;
    UINT8   i;

    g_MailNum = 0;

    if(!SKY_IsDataFile(SDF_MAIL,(BYTE*) INBOX_LIST_FILE))
    {
        // Create file
        if(SKY_CreateDataFile(SDF_MAIL, (BYTE*)INBOX_LIST_FILE, &hFile) == SDR_OK)
        {
            memset(g_MailList, NULL, sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL);
            memset(g_MailListNum, NULL, sizeof(UINT8)*MAX_SAVED_MAIL);

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
                    (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
                    (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));




            SKY_CloseDataFile(hFile);
        }
    }
    else
    {
        SKY_OpenDataFile(SDF_MAIL,(BYTE*) INBOX_LIST_FILE, &hFile, SFT_READ);

        SKY_ReadDataBlock(hFile, (void *)g_MailListNum, 
                (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

        SKY_ReadDataBlock(hFile, (void *)g_MailList, 
                (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

        for(i = 0; i < MAX_SAVED_MAIL; i++)
        {
            if(g_MailListNum[i] > 0 && g_MailListNum[i] <= MAX_INBOX_MAIL)
                g_MailNum ++;
        }

        SKY_CloseDataFile(hFile);
    }
}

void SKA_InitMail(void)
{
    HFILE   hFile;

    if(!SKY_IsDataFile(SDF_MAIL,(BYTE*) OUTBOX_LIST_FILE))
    {
        // Create file
        if(SKY_CreateDataFile(SDF_MAIL, (BYTE*)OUTBOX_LIST_FILE, &hFile) == SDR_OK)
        {
            memset(g_MailList, NULL, sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL);
            memset(g_MailListNum, NULL, sizeof(UINT8)*MAX_SAVED_MAIL);

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
                    (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
                    (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

            SKY_CloseDataFile(hFile);
        }
    }

    if(!SKY_IsDataFile(SDF_MAIL,(BYTE*) INBOX_LIST_FILE))
    {
        // Create file
        if(SKY_CreateDataFile(SDF_MAIL, (BYTE*)INBOX_LIST_FILE, &hFile) == SDR_OK)
        {
            memset(g_MailList, NULL, sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL);
            memset(g_MailListNum, NULL, sizeof(UINT8)*MAX_SAVED_MAIL);

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
                    (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

            SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
                    (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

            SKY_CloseDataFile(hFile);
        }
    }
}

