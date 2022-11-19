/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_gvm.c
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
** 2000-12-22 wshan      Created.
** 2001-09-07 undersea   2.05
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
#include "SWAP\SWAP.h"
#include "..\..\..\EQUS\INCLUDE\eqs_grph.h"
#include "uisockApi.h"
#include "SWAP\gvm_config.h"
#include "sky_sock.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
//#define FEATURE_SMS_RETRY      // SMS보내기 실패시 다시 보낼것인가에 대한 것임.
#ifdef FEATURE_SMS_RETRY
#define NUM_OF_SEND_RETRIAL	    	3
#endif

#define GVM_ERROR                   -1
#define MAX_VIEW_SCRIPT_NAME_LEN    14
#define	NET_MAX_SIZE	            668
#define SMS_TIMEOUT                 10*1000
#define MAX_ALLOC_CTRLNUM           9
#define MAX_GVM_VOLUME				5		/* 게임 최대 볼륨 */

#define SMB_SCROLL_LIST_STYLE       CS_SCROLL_LIST_DEFAULT | \
                                    CS_SCROLL_LIST_ENUMBERATED

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

enum tagGVM_EDIT_STRTYPE_E {
    GEST_DEFAULT,
    GEST_NUMBER,
    GEST_PASSWORD_NUMBER,
    GEST_MAX
};
typedef enum tagGVM_EDIT_STRTYPE_E GVM_EDIT_STRTYPE_E;

// 채팅 예약어 편집 화면에서 앞의 숫자들
SKY_TOKEN l_atGVMChatTextNum[GVM_LEN_CHATTEXT] =    
{
    TKN_GVM_CHATTEXT_NUM1,
    TKN_GVM_CHATTEXT_NUM2,
    TKN_GVM_CHATTEXT_NUM3,
    TKN_GVM_CHATTEXT_NUM4,
    TKN_GVM_CHATTEXT_NUM5,
    TKN_GVM_CHATTEXT_NUM6,
    TKN_GVM_CHATTEXT_NUM7,
    TKN_GVM_CHATTEXT_NUM8,
    TKN_GVM_CHATTEXT_NUM9
};

enum tagGVM_MESSG_TYPE_E {
    GDMT_WAITING,
    GDMT_REQUEST,
    GDMT_NO_FILE,
    GDMT_DELETE_CONFIRM,
    GDMT_DELETED_ALL,
    GDMT_DELETED,
    GDMT_DELETING,
    GDMT_EXIT_CONFIRM
};
typedef enum tagGVM_MESSG_TYPE_E GVM_MESSG_TYPE_E;
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

// for SMS
int						        g_GVMSmsMoTI;
SMSMO_SEND_T			        g_GVMSmsBuffer;
GVM_SMS_MO_TYPE_E		        g_GVMSmsMoType;

GVM_CONNECT_SERVER_TYPE_E       g_TypeConnectSvr = GCST_NONE;	// Server연결타입
UINT8                           g_nCurSSB = 0;					// 현재 실행중인 SSB index
BOOL                            g_fGVMPaused = FALSE;			// Call incoming등으로 잠시 쉬는 상태 표시 flag
UINT8		                    g_unTrySMS;						// SMS retrial 횟수 
BOOL                            g_fDownloadRequestFromSMS = FALSE;	// SMS로 다운로드시 TRUe
BOOL                            g_fDeleteScript = FALSE;		// 실제 삭제 여부 표시
BOOL                            g_fIn256ColorDisplay = FALSE;	// 256 color mode로 display할것인가에 대한 Flag

// GVM 코드 내에서만 공유하는 변수
DWORD                           g_dwFreeSpace;					// EFS의 남은 영역 표시	
GVM_RUNNING_TYPE                g_nRunningType = GRT_NONE;		// 현재 실행하는 mode (보통 실행전까지만 사용함)
GVM_DISCONNECT_TYPE_E           g_eDisconnectType = DT_NONE;	// 연결해제타입
S_Result                        *recv_result = NULL; 
BOOL							g_fFromWAP = FALSE;				// WAP에서 왔는지 표시할것 // SKY_IS_FROM_WAP 사용안함
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
/* START - 다운로드를 하기 위한 파라메터들 */
STATIC      BYTE                l_szUserID[SWAP_LEN_USERID+1];  // 선택한 게임에서 사용하는 사용자 이름
STATIC      int                 l_nScriptCPID;                  // CP의 id, 범위는           0x0000 ~ 0xfffe
STATIC      int                 l_nScriptID;                    // 실행할 게임의 id          0x0000 ~ 0xffff
STATIC      BYTE                l_szDlsIP[SWAP_LEN_IP+1];       // 다운로드서버의 IP address "game.sinjisoft.com"
STATIC      int                 l_nDlsPort;                     // 다운로드서버의 port       0 ~ 65535
STATIC      BYTE                l_szSvrIP[SWAP_LEN_IP+1];       // 서버 IP address           
STATIC      int                 l_nSvrPort;                     // 서버 port
STATIC      BYTE                l_szRetURL[SWAP_LEN_URL+1];     // 게임종료후 go back을 위한 URL
/* END - 다운로드를 하기 위한 파라메터들 */

CHAR	                        l_acRcvBuf[NET_MAX_SIZE+1];		// Data Receive Buffer

// 공간만들기 메뉴위함
STATIC      DWORD               l_dwScriptSize;
STATIC      DWORD               l_dwLackSize;
STATIC      int                 l_nSSBIndex;
STATIC      HCONTROL            l_hFileList, l_hFileSize, l_hUsedSize, l_hFreeSize, l_hLackSize;
STATIC      HCONTROL            l_hFileSizeTitle, l_hUsedSizeTitle, l_hFreeSizeTitle, l_hLackSizeTitle;
STATIC      BYTE*               l_pabGVMName[SWAP_SSB_SIZE];
STATIC      INT8                l_anGVMFileIdx[SWAP_SSB_SIZE];
STATIC      BYTE                l_ppGVMFileName[SWAP_SSB_SIZE][SKY_PATH_NAME_MAX_LENGTH];
STATIC      GVM_MESSG_TYPE_E    l_MessageReason;

// 별도의 MMI로 빠져나갈때 사용하는 Flag
STATIC      BOOL                l_fEditPaused = FALSE; 
STATIC      BOOL                l_fChatEditPaused = FALSE;

// 문자입력기에 필요한 변수들
STATIC      BYTE*               l_szEditTitle;
STATIC      BYTE*               l_szEditInitialString;
STATIC      int                 l_nEditMaxLen = 0;
STATIC      UINT8               l_nEditStrType = 0;
STATIC      BYTE*               l_szEditedText = NULL;

STATIC      BOOL                l_fConnected = FALSE;
STATIC      BOOL                l_fPPPOpened = FALSE;
STATIC      BOOL                l_fSMSEnded = FALSE;

STATIC		BOOL				l_fConnectFailed = FALSE;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* game applet */
LOCAL VOID SKY_API  sSKA_ShowErrorMsg(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API  sSKA_EditGVMChatText(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API  sSKA_GVMFilesInfo(SKY_EVENT_T *pEvent);  // 저장공간만들기 관련 애플릿

LOCAL VOID          sStartGVM(VOID);
LOCAL VOID          sEndGVM(VOID);
LOCAL void			sSetSSBIndex(const WORD cpid, const WORD gid);
LOCAL VOID			sCheckValidScript(const WORD cpid, const WORD gid);
LOCAL VOID          sCheckScriptInfo(void);	// SKA_MainGVM에서 끝날때만... 
LOCAL INT           sGetSWAPKeyCode(int keycode);
// out of memory
LOCAL BOOL          sHaveGVMFiles(VOID);
LOCAL BOOL          sAddGVMControls(VOID);
// for 256 color
#if (SWAP_LCD_TYPE ==  SWAP_LCD_COLOR256_HORIZ)  // 256 color
LOCAL VOID          sSKA_GVMDrawAnnunciators(BOOL fAlways);
#endif


/*
** ***************************************************************************
** APPLETS
** ***************************************************************************
*/

/*
    << 구조도 by undersea >>

	                            +-- SKA_ViewTextMessage(indirect) 
	                            |      or SKA_MessageNotif(direct) (SMS에서 다운로드)
                                |
     SKA_MainGVM       <--------+-- SKA_MainIdleScreen (AT Command로 인한 스크립트 실행시) 
         |                      |
         |                      +-- xce_gvm_start (SK_VM에서 불릴때)
         |                      |
	     |                      +-- WAP에서...
	  {child}
	     | 
	SKA_RunGVM
	     |
	     +---------------------+------------------------+
	     |			       	   |                        |
	  {child}	            {child}                  {child}
	     |		               |                        |
    sSKA_TextEdit     sSKA_EditGVMChatText      sSKA_GVMFilesInfo
    (텍스트 편집)     (채팅텍스트 편집)       (공간만들기-디렉토리)
*/


/*
    Main Applet
*/
VOID SKY_API SKA_MainGVM(SKY_EVENT_T *pEvent) {
    
#define WAIT4CALLENDED    2000
    
    switch(pEvent->EventID) 
    {        
        case SE_APP_START:

			EQS_SetDirectDraw(TRUE);
			if (SKY_IS_PHONE_CLOSE())	// 만약 닫힌 상태로 들어오면..
			{
				g_fGVMPaused = FALSE;
				SKY_SET_TO_GVM(FALSE);
				SKY_SET_TO_WAP(FALSE);
				SKY_SET_FROM_SKVM(FALSE);
				EQS_END();
				break;
			}

            SKY_SET_IN_GVM(TRUE);
           
            /* Interrupt -> Pause -> Continue */
            if (g_fGVMPaused)
            {
                g_nRunningType = GRT_RUN_START;
                EQS_START_CHILD(SKA_RunGVM);
            }
            /* GVM <- WAP */
            else if(SKY_IS_TO_GVM()) 
            {                
                if (SKY_IS_FROM_SKVM()) // just RUN!
                {
					SKY_ReinitMemory();
                    g_nRunningType = GRT_RUN_START;
                    SKY_SET_FROM_SKVM(FALSE);
					EQS_START_CHILD(SKA_RunGVM); /* Download */
                }
                else
                {
                    if (g_nRunningType == GRT_NONE) // DOWNLOAD!!!
                    {
                        g_nRunningType = GRT_DOWNLOAD_START;
                        
						if (g_fFromWAP)
                        {
							SKY_ReinitMemory();
							if (P_REV_IS_6_OR_GREATER)	// 95C이면 그냥 넘어감.
							{								
								EQS_START_CHILD(SKA_RunGVM);
							}
							else 
							{
                                

								SKY_SetGeneralTimer(WAIT4CALLENDED, FALSE);	// 호가 종료되도록 잠시 대기
							}
                            break;
                        }
						else 
						{
							EQS_START_CHILD(SKA_RunGVM);
						}
                    }
					else	// At command로 되는 상태
					{
						EQS_START_CHILD(SKA_RunGVM);
					}
                }
            }
            else if(SKY_IS_TO_WAP())
            {
#ifdef FEATURE_WAP
                EQS_START_SIBLING(SKA_MainWAP, FALSE);
#endif//#ifdef FEATURE_WAP
            }
			else 
			{
				error("이상한 상황이다~ 머지?", 0, 0, 0);
				EQS_END();
			}
            break;

        case SE_TM_TIMER:
            UI_DISABLE_SLEEP();
            EQS_START_CHILD(SKA_RunGVM); /* Download */
            break;

        case SE_APP_RESUME:
            error("[SE_APP_RESUME] <- MainGVM", 0, 0, 0);
                        
            if(SKY_IS_TO_GVM())
            {
                g_nRunningType = GRT_DOWNLOAD_START;
		        EQS_START_CHILD(SKA_RunGVM);
            }
            else if(SKY_IS_TO_WAP())
            {
#ifdef FEATURE_WAP
                EQS_START_SIBLING(SKA_MainWAP, FALSE);
#endif//#ifdef FEATURE_WAP
            }
            else
            {               
                // 혹시 연결이 안 끊어진 상태면 끊어주는 루틴을 넣어야 하지 않을지...
                EQS_END();
            }
            break;

        case SE_APP_END:
            error("[END-SKA_MainGVM]", 0, 0, 0);
            SKY_SET_IN_GVM(FALSE);
            g_nRunningType = GRT_NONE;
			EQS_SetDirectDraw(FALSE);
            break;

        case SE_APP_DRAW:
            break;

        default:
            break;
    }
}


/*
    실행 애플릿
*/
VOID SKY_API SKA_RunGVM(SKY_EVENT_T *pEvent) 
{
	STATIC		BOOL                    hasResult = TRUE;
	STATIC		BOOL					fDereg = FALSE;
    STATIC      S_Event                 event;
    STATIC      S_Result                *result;       
    
    STATIC      HCONTROL                hMessageBox;
                uasms_status_e_type	    RetStatus;
                int                     nLenOfRcvBuf = 0;
    
    switch(pEvent->EventID) 
    {
        /*
            START Event
        */
        case SE_APP_START:
          
            /* Set Window */
            EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y,
                        SKY_DEF_WIN_DX,
                        SKY_DEF_WIN_DY,
                        NULL,
                        NULL,
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
						WA_SHOW_HEADER);
#else
                        WA_NONE);
#endif      

            sSetColorDisplay(TRUE);
            sStartGVM();						
            g_dwFreeSpace = SKY_GetFreeSpaceInDataFile();

            if (g_nRunningType == GRT_DOWNLOAD_START)   // Download
            {                
				// 만약 valid하지 않은 거라서 다운로드를 받아야 한다면 midd_DeleteScript를 불러줘야 한다.
                sCheckValidScript(l_nScriptCPID, l_nScriptID);
                error("새로 다운로드를 받는다", 0, 0, 0);
				scl_StoreRunInfo((char*)l_szUserID,
                            l_nScriptCPID,
                            l_nScriptID,
                            (char*)l_szDlsIP,
                            l_nDlsPort,
                            (char*)l_szSvrIP,
                            l_nSvrPort,
                            (char*)l_szRetURL);
                
                event.code = SWAP_EVENT_DOWNLOAD_START;
                result = scl_DoSwapEvent(&event);
            }
            else //if (g_nRunningType == GRT_RUN_START) 
            {   
                SKY_SET_PLAYING_GVM(TRUE);

                if (g_fGVMPaused) // pause되었다가 다시 continue될때...
                {
                    g_fGVMPaused = FALSE;
					        
                    if (l_fEditPaused)     // editing중에 착신이면..
                    {
                        hasResult = FALSE;
                        
                        sSetColorDisplay(FALSE);
						EQS_START_CHILD(sSKA_TextEdit);
                    }
                    else if (l_fChatEditPaused) 
                    {
                        hasResult = FALSE;
                        l_fChatEditPaused = FALSE;

                        sSetColorDisplay(FALSE);
						EQS_START_CHILD(sSKA_EditGVMChatText);
                    }
                    else 
                    {
                        event.code = SWAP_EVENT_CONTINUE;
                        result = scl_DoSwapEvent(&event);
                    }
                }
                else   // 정상적인 스크립트 실행
                {
					event.code = SWAP_EVENT_SVM_START;
                    event.param.data = g_nCurSSB;
                    result = scl_DoSwapEvent(&event);                    
                }
            }
            break;

        /*
            RESUME Event
        */
        case SE_APP_RESUME:
            error("[SE_APP_RESUME] <- RUN", 0, 0, 0);
            sSetColorDisplay(TRUE);			

            if (EQS_GET_CHILD() == sSKA_EditGVMChatText) 
            {                
                // 채팅 예약어 편집완료 이벤트 완료 알림
                event.code = SWAP_EVENT_CHATEDIT;
                result = scl_DoSwapEvent(&event);
            }
            else if (EQS_GET_CHILD() == sSKA_TextEdit) 
            {                
                if (pEvent->dwParam == SE_KEY_END)     // 그냥 끝낸다.
                {
                    SKY_Free((void*)l_szEditedText);
                    l_szEditedText = NULL;

                    if (l_fConnected)    // 연결중 상태 ...
                    {
						hasResult = FALSE;

						if (NetworkGetState() == NET_PPPOPEN_S)
                        //if (g_nRunningType == GRT_RUN_SERVER) 
						{
							//SKY_SetGeneralTimer(PPP_TIMEOUT, FALSE);
                            midd_Disconnect(SWAP_RR_DISCONNECT_SVREXIT);
						}
                        else 
                            midd_PtpRelease(SWAP_RR_PTPDISCONNECT_PTPEXIT);
                    }
                    else 
                    {
                        event.code = SWAP_EVENT_MENU;
                        event.param.menu.event = SWAP_MENU_EXIT;
                        result = scl_DoSwapEvent(&event);
                    }
                }
                else 
                {
					// 문자열 입력 이벤트 완료 알림
					event.code = SWAP_EVENT_TEXTEDIT;                    
					event.param.str = (char*)l_szEditedText;
					result = scl_DoSwapEvent(&event);
				
                    SKY_Free((void*)l_szEditedText);
                    l_szEditedText = NULL;
                }
            }
            else if (EQS_GET_CHILD() == sSKA_GVMFilesInfo) 
            {
				if (pEvent->dwParam == SE_SYS_TO_IDLE)	// No File
				{
					event.code = SWAP_EVENT_MENU;
					event.param.menu.event = SWAP_MENU_EXIT;
					result = scl_DoSwapEvent(&event);
				}
				// 삭제 도중 입력된 End키처리 -> n-topㅇ로
				else if (pEvent->dwParam == SE_KEY_END) 
				{
					hasResult = FALSE;                

					if (l_szRetURL != NULL) 
						midd_BackToBrowser((char*)l_szRetURL);
					else
						midd_BackToBrowser((char*)SKY_GET_WAP_INIT_URL());					
				}
				else
				{
                    event.code = SWAP_EVENT_DOWNLOAD_START;
                    result = scl_DoSwapEvent(&event);
                }
            }
            else hasResult = FALSE;
            break;

        /*
            END Event
        */
        case SE_APP_END:
            error("[SE_APP_END] <- RUN!!", 0, 0, 0);
            hasResult = FALSE;
            sEndGVM();           
            
            sSetColorDisplay(FALSE);
            break;

        /*
            DRAW Event
        */
        case SE_APP_DRAW:
            hasResult = FALSE;
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
            SKY_UpdateScreen();
#endif            
            break;

        case SE_PH_INUSE_STATE:    /* inuse state is changed */
			hasResult = FALSE;

            if (SKY_SetThenCompareInUseState(SKY_GetInUseState()))
            {
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)  // 256 color
                sSKA_GVMDrawAnnunciators(TRUE);
#elif (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
				SKY_DrawAnnunciators();
				SKY_UpdateScreen();
#else
#endif
            }
            break;

        /*
            GVM Timer Event
        */
        case SE_GVM_TIMER1:
			UI_DISABLE_SLEEP();
            event.code = SWAP_EVENT_MENU;
            event.param.menu.event = SWAP_MENU_TIMEOUT;
            event.param.menu.data = 0;
            result = scl_DoSwapEvent(&event);
            break;
        case SE_GVM_TIMER2:
			UI_DISABLE_SLEEP();
            event.code = SWAP_EVENT_MENU;
            event.param.menu.event = SWAP_MENU_TIMEOUT;
            event.param.menu.data = 1;
            result = scl_DoSwapEvent(&event);
            break;
        case SE_GVM_TIMER3:
			UI_DISABLE_SLEEP();
            event.code = SWAP_EVENT_MENU;
            event.param.menu.event = SWAP_MENU_TIMEOUT;
            event.param.menu.data = 2;
            result = scl_DoSwapEvent(&event);
            break;
            
        /*
            SMS
        */
        case SE_SMS_MO_STATUS_MESSAGE:           

            RetStatus = (uasms_status_e_type)(GET_EVENT_PARAM()->sms_mo_status.status);            
            hasResult = FALSE;            

            switch(RetStatus)
            {
                case UASMS_ACCESS_TOO_LARGE_S:      /* Traffic Channel 연결 */
                    uasms_DTC_connect();            /* message is too large to be sent over DTC */
                    break;
                case UASMS_WAITING_FOR_TL_ACK_S:    /* SMS 전송 성공 : layer 2 ack */
                    break;                          /* intermediate status for msg requesting TL ack */
                case UASMS_OK_S:                    /* SMS 전송 성공 : layer 3 ack */
			                                        /* 32768 */			        

					EQS_StopTimer(EQS_TIMER_2);
					
					/* traffic channel 끊는다 */
					if (IsCallTypeSMS() && g_nRunningType != GRT_RUN_PTP) //if(ui_curr_call_is_sms())
					{
						error("sms call discon!", 0, 0, 0);
						uasms_DTC_disconnect();
					}

					error("SMS보내기 성공~~~~", 0, 0, 0);
        	        g_GVMSmsMoType = GSMT_IDLE; // 성공했음.
                    g_unTrySMS = 0;
                    
                    hasResult = TRUE;
                    if (IsCallTypeSMS()) l_fSMSEnded = TRUE;

                    event.code = SWAP_EVENT_SMS_RESULT;
                    event.param.data = 1; //success
                    result = scl_DoSwapEvent(&event);
                    break;   
					
                default:
                    error("SMS MO FAIL:unexpected status", 0, 0, 0);
                    if(GET_EVENT_PARAM()->sms_mo_status.error_class == UASMS_ERROR_PERM)
                    {
                        EQS_StopTimer(EQS_TIMER_2);      
						/* traffic channel 끊는다 */
						if (IsCallTypeSMS() && g_nRunningType != GRT_RUN_PTP) //if(ui_curr_call_is_sms())
						{
							error("sms call end!", 0, 0, 0);
							uasms_DTC_disconnect();
						}

#ifdef FEATURE_SMS_RETRY
                        if(g_unTrySMS < NUM_OF_SEND_RETRIAL) /* 다시시도 */
                        {
							g_unTrySMS++; 
							g_GVMSmsMoType = GSMT_SENDING;

							error("sms send retry~~~(%dth try)", (g_unTrySMS+1), 0, 0);
							EQS_StartTimer(EQS_TIMER_2, SMS_TIMEOUT, FALSE);

					        SendSMSMsg(&g_GVMSmsBuffer, g_GVMSmsMoTI);
                        }
                        else/* 전송실패 */
#endif
                        {
					        g_unTrySMS = 0; /* process for out */
					        g_GVMSmsMoType = GSMT_IDLE;	
                            hasResult = TRUE;
                            if (IsCallTypeSMS()) l_fSMSEnded = TRUE;

                            error("[SMS]SMS보내기 실패!!!!", 0, 0, 0);
                            event.code = SWAP_EVENT_SMS_RESULT;
                            event.param.data = 0; //fail
                            result = scl_DoSwapEvent(&event);                            
                        }
                    }   
                    break;
            }
            break;

        /*
            CALL Event
        */
        case SE_CALL_ORIGINATED:
            error("[CALL_ORIGINATED] <- RUN!!", 0, 0, 0);

            hasResult = FALSE;
            
            if (!IsCallTypeSMS()) 
            {
                SKY_SET_ORIGINATE(TRUE);
            }
            break;

        case SE_CALL_CONNECT:
            error("[CALL_CONNECT] <- RUN!!", 0, 0, 0);
            hasResult = FALSE;

            SKY_SET_ORIGINATE(FALSE);
            
            if (IsCallTypeSMS())
                SendSMSMsg(&g_GVMSmsBuffer,g_GVMSmsMoTI);
            else 
			{
				SKY_SNI(HS_SNI_CALL_CONNECT, TRUE);
                EQS_PASS_CURRENT_EVENT();
			}
            break;

        case SE_CALL_INCOMING:
            error("[CALL_INCOMING] <- RUN!!", 0, 0, 0);

            if (SKY_IsVoiceCall()) 
			{
				error("Voice Call Incoming~~~~~~~~~~~~~~", 0, 0, 0);
				event.code = SWAP_EVENT_MENU;
				event.param.menu.event = SWAP_MENU_CALL;
				result = scl_DoSwapEvent(&event);
			}
            else 
			{
				error("Data Call Incoming~~~~~~~~~~~~~~", 0, 0, 0);
				hasResult = FALSE;				
			}
            break;

		// 연결이 끊어지는 시점!!!
        case SE_CALL_ENDED:     
            error("[CALL_ENDED, runtype:%d] <- RUN!!", g_nRunningType, 0, 0);
			error("discon type:%d", g_eDisconnectType, 0, 0);

            if(l_fSMSEnded || (IsCallTypeSMS() && g_nRunningType != GRT_RUN_PTP) ) // SMS발신후 Call End
            {
                hasResult = FALSE;
                l_fSMSEnded = FALSE;
                break;
            }
            
#ifdef SWAP_FEATURE_PTP
            if (g_nRunningType == GRT_RUN_PTPCALL || g_nRunningType == GRT_RUN_PTPWAIT)  // ptp connect fail
            {                
                error("[call_ended]ptp connect fail!!", 0, 0, 0);
                g_nRunningType = GRT_NONE; // PTP 연결 끊김/   ??????????????????????
				
                g_eDisconnectType = DT_NONE;                
                l_fConnected = FALSE;

				if (g_eDisconnectType != DT_NONE)
				{					
					event.code = SWAP_EVENT_PTPDISCON_OK;
				}
				else 
				{
					event.code = SWAP_EVENT_PTP_RESULT;
					event.param.data = 0;  // fail
				}
				result = scl_DoSwapEvent(&event);
            }
            else if (g_nRunningType == GRT_RUN_PTP) 
            {				
				if (g_eDisconnectType == DT_NONE) // || g_eDisconnectType < DT_NONE) 
				{
					error("ptp중 call drop이겠지?:%d", g_eDisconnectType, 0, 0);
					/* 						
						SE_CALL_ENDED이벤트에서 g_eDisconnecType이 DT_NONE이면
						PTP도중에 Call drop으로 여긴다. midd_Disconnect가 불린 이후에
						g_eDisconnectType을 DT_CALLDROP으로 변경하여 강제로
						SE_PTP_RELEASED이벤트를 보내도록 한다.					
					*/

                    event.code = SWAP_EVENT_MENU;
                    event.param.menu.event = SWAP_MENU_CALLDROP;
                    result = scl_DoSwapEvent(&event);
				}
				else 
                //else if (g_eDisconnectType != DT_NONE && g_eDisconnectType > DT_NONE) 
                {
                    error("암튼.. PTP 끝나는 것임", 0, 0, 0);

					l_fConnected = FALSE;
					g_eDisconnectType = DT_NONE;
					g_nRunningType = GRT_NONE;                    

                    event.code = SWAP_EVENT_PTPDISCON_OK;
                    result = scl_DoSwapEvent(&event);
                }
            }
            else if (g_eDisconnectType == DT_NONE)  // call drop됨을 알게된다.            
#else
            if (g_eDisconnectType == DT_NONE)  // call drop됨을 알게된다.
#endif  // PTP
            {
                error("call drop!!!", 0, 0, 0);

				// 95C & Dormant  -> 일단 보류 -> 안함.
				if (l_fPPPOpened && l_fConnected)	// 연결 상태에서 call_ended오면 call drop
				{
					event.code = SWAP_EVENT_MENU;
					event.param.menu.event = SWAP_MENU_CALLDROP;
					result = scl_DoSwapEvent(&event);
				}
				else
				{
					hasResult = FALSE;
					error("connect fail!", 0, 0, 0);					
				}
            }
            else hasResult = FALSE;
            break;
            
            
        case SE_CALL_COMMAND_ERROR:            
            error("[SE_CALL_COMMAND_ERROR] <- RUN!!", 0, 0, 0);
            hasResult = FALSE;
            break;

        /*
            PTP Event
        */
#ifdef SWAP_FEATURE_PTP
        case SE_PTP_CONNECT_SUCCESS:           // Sock과 별개
            error("[SE_PTP_CONNECT_SUCCESS]ptp 연결 성공", 0, 0, 0);
            
            SKY_SET_TO_WAP(FALSE);

            l_fConnected = TRUE;
            
            /* 착신 일 경우에는 CM에서 UI로 SE_PH_IN_USE라는 이벤트를    */
            /* 날려주지 않기 때문에 Annunciator를 직접 그려 주어야 한다. */
            /* 2001.09.20 hjcho                                          */
            if (SKY_SetThenCompareInUseState(IUS_CONNECT))
            {
				/* SKY_IS_IN_USE()에서 참고로 하는 값이 갱신되지 않았기 */
				/* 때문에 강제로 갱신시켜 준다.   2001.10.05 hjcho      */
				SKY_GET_PHONE_INFO().is_in_use = TRUE;

				// draw annunciator
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)  // 256 color
				sSKA_GVMDrawAnnunciators(TRUE);
#elif (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
				SKY_DrawAnnunciators();
				SKY_UpdateScreen();                
#else                
#endif                
            }
			
            g_nRunningType = GRT_RUN_PTP;
            
            event.code = SWAP_EVENT_PTP_RESULT;                
            event.param.data = 1;  // success
            result = scl_DoSwapEvent(&event);
            break;
    
        case SE_PTP_CONNECT_FAIL:               // ok
            error("[SE_PTP_CONNECT_FAIL]ptp 연결 실패", 0, 0, 0);

            g_nRunningType = GRT_NONE;
            g_eDisconnectType = DT_NONE;

            l_fConnected = FALSE;

            event.code = SWAP_EVENT_PTP_RESULT;                
            event.param.data = 0;  // fail
            result = scl_DoSwapEvent(&event);
            break;

        case SE_PTP_RELEASED:
            error("[SE_PTP_RELEASED]ptp released!!!", 0, 0, 0);

            g_nRunningType = GRT_NONE;
            g_eDisconnectType = DT_NONE;
            
            l_fConnected = FALSE;
            
            event.code = SWAP_EVENT_PTPDISCON_OK;
            result = scl_DoSwapEvent(&event);
            break;
#endif // #ifdef SWAP_FEATURE_PTP

        /*
            Socket Event
        */
        case SE_NET_CONNECTED:  // after ppp open 
            error("[SE_NET_CONNECTED] <- RUN!!", 0, 0, 0);
            
            SKY_SetGeneralTimer(0, FALSE);
			EQS_StopTimer(EQS_TIMER_1);
            l_fPPPOpened = TRUE;			
			        
            if (midd_Connect2() == DSS_ERROR) 
			{
                error("socket open & connect 실패~~~~~~~", 0, 0, 0);
                hasResult = FALSE;
                
                SKY_SET_TO_WAP(FALSE);

                l_fConnected = FALSE;        
				l_fConnectFailed = TRUE;

				//SKY_SetGeneralTimer(PPP_TIMEOUT, FALSE);
                midd_Disconnect(SWAP_RR_DISCONNECT);                
            }
            else 
			{
                error("socket open & connect 성공~~~~~~~", 0, 0, 0);

                //SKY_SetGeneralTimer(0, FALSE);
				EQS_StopTimer(EQS_TIMER_1);

                SKY_SET_TO_WAP(FALSE);
                
                l_fConnected = TRUE;
            
                if (g_nRunningType == GRT_DOWNLOAD_START) 
                    g_nRunningType = GRT_DOWNLOADING;            
                else 
                    g_nRunningType = GRT_RUN_START;

                event.code = SWAP_EVENT_CONNECT_RESULT;
                event.param.data = 1; // success
                result = scl_DoSwapEvent(&event);
            }
            break;

		case SE_NET_DISCONNECTED:   // after ppp close
            
            error("[NET_DISCONNECTED] ppp:%d, conn:%d", l_fPPPOpened, l_fConnected, 0);         
			error("discon type : %d", g_eDisconnectType, 0, 0);
            SKY_SetGeneralTimer(0, FALSE);
			EQS_StopTimer(EQS_TIMER_1);
            g_TypeConnectSvr = GCST_NONE;
			
            /*
                WAP->GVM & IS-95C & ppp_opened인테 스크립트가 있어서 끊어야 할 경우는 
                결과를 SWAP에 넘겨주지 않는다.
            */
            if (g_eDisconnectType == DT_NORESULT) 
            {
                g_eDisconnectType = DT_NONE;                
                break;
            }
            
            /*
                BackToBrowser일경우 IS-95C이면 소켓만 닫고 간다
            */
            if (g_eDisconnectType == DT_TOWAP && P_REV_IS_6_OR_GREATER) 
            {                
                l_fConnected = FALSE;
                l_fPPPOpened = FALSE;   // 가짜로 닫혔다고 함
                g_eDisconnectType = DT_NONE;
                
                error("[SE_NET_DISCONNECTED]소켓만 닫힘 -> to WAP!!", 0, 0,0);
                
                event.code = SWAP_EVENT_DISCON_OK;
                result = scl_DoSwapEvent(&event);                
                break;
            }
			
			if (g_eDisconnectType == DT_NONE || l_fConnectFailed)	// midd_Disconnect함수가 불리지 않은 상태/midd_Connect2에서 return -1인 경우
			{ //?????????????????????????????				

				l_fConnectFailed = FALSE;
				event.code = SWAP_EVENT_CONNECT_RESULT;
                event.param.data = 0; // FAIL			
			}
			else 
			{
				event.code = SWAP_EVENT_DISCON_OK;
			}
            result = scl_DoSwapEvent(&event);
            
            l_fConnected = FALSE;
            l_fPPPOpened = FALSE;            
            
            g_eDisconnectType = DT_NONE;
            
            if (g_nRunningType == GRT_DOWNLOAD_SUCCESS) 
            {
                g_nRunningType = GRT_RUN_START;				
            }
            break;
        
        case SE_RECV_DATA:  // Receive Data!!!            

            if (pEvent->dwParam != EQS_NA) {
				result = recv_result;
                break;
            }

            hasResult = FALSE;
            memset(l_acRcvBuf, 0, NET_MAX_SIZE+1);
#ifdef SWAP_FEATURE_PTP
            if (g_nRunningType == GRT_RUN_PTP) 
                midd_PtpRecvData((BYTE*)l_acRcvBuf, &nLenOfRcvBuf);
            else 
#endif // #ifdef SWAP_FEATURE_PTP
                midd_recvdata(l_acRcvBuf, NET_MAX_SIZE, &nLenOfRcvBuf);
        
            error("[se_recv_data]data=%d", nLenOfRcvBuf, 0, 0);
            if (nLenOfRcvBuf > 0) 
            {
                event.code = SWAP_EVENT_MENU;
                event.param.menu.event = SWAP_MENU_RECEIVE;
                event.param.menu.buf = (unsigned char*)l_acRcvBuf;                    
                event.param.menu.data = nLenOfRcvBuf;
                result = scl_DoSwapEvent(&event);

                if (result->code != SWAP_RC_NONE)
                {
                    hasResult = TRUE;
                    break;
                }
                nLenOfRcvBuf = 0;
            }
            else 
            {
                break;
            }
            break;

        /*
            KEY Event
        */
        case SE_KEY_1:
        case SE_KEY_2:
        case SE_KEY_3:
        case SE_KEY_4:
        case SE_KEY_5:
        case SE_KEY_6:
        case SE_KEY_7:
        case SE_KEY_8:
        case SE_KEY_9:
        case SE_KEY_0:
        case SE_KEY_STAR:
        case SE_KEY_POUND:
        case SE_KEY_CLEAR:
        case SE_KEY_SEND:
        case SE_KEY_1_LONG:
        case SE_KEY_2_LONG:
        case SE_KEY_3_LONG:
        case SE_KEY_4_LONG:
        case SE_KEY_5_LONG:
        case SE_KEY_6_LONG:
        case SE_KEY_7_LONG:
        case SE_KEY_8_LONG:
        case SE_KEY_9_LONG:
        case SE_KEY_0_LONG:
        case SE_KEY_LEFT:
        case SE_KEY_RIGHT:
        case SE_KEY_UP:
        case SE_KEY_DOWN:
        case SE_KEY_CENTER:
        case SE_KEY_END:
			
			SKY_DeregKeyCB();	// to detect long key 
			fDereg = TRUE;

            event.code = SWAP_EVENT_MENU;
            event.param.menu.event = SWAP_MENU_KEYIN;
            event.param.menu.data = sGetSWAPKeyCode((int)(pEvent->EventID));
            result = scl_DoSwapEvent(&event);
            break;

		case SE_KEY_MANNERS:    // 에티켓모드로 전환/해제       
        
            hasResult = FALSE;
			error("매너 키 눌림~~~~~~~~~~~~~~~", 0, 0, 0);

            if(SKY_IS_ETIQUETTE())
            {
#ifdef FEATURE_SKY_SUBLCD
                SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_CLEAR_ETIQUETTE);
                SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD

                SKY_SET_ETIQUETTE(FALSE);
            }
            else
            {
#ifdef FEATURE_SKY_SUBLCD
                SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_SET_ETIQUETTE);
                SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
                SKY_SET_ETIQUETTE(TRUE);
                SKY_MotorTime(800);
            }

            SKY_SetSoundDevice(SND_MUTE_MAX, SND_MUTE_MAX, FALSE);
            SKY_DisplayBellVibIcon();

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)  // 256 color
            sSKA_GVMDrawAnnunciators(TRUE);
#elif (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
			SKY_DrawAnnunciators();
			SKY_UpdateScreen();            
#else            
#endif
            /* jrkwon : re-display sub lcd as default */
            SKY_SubLCDDefault();
            break;

        /*
            볼륨 조절을 위한 키 set -> 임시로 
        */
        case SE_KEY_F2: // UP!
            {
                STATIC  UINT8   nVolume;
                nVolume = SKY_GET_GAME_VOLUME();
				
                if (nVolume != MAX_GVM_VOLUME) {
                    SKY_SET_GAME_VOLUME(nVolume+1);
                    SKY_ControlMusicVolume(nVolume+1);
                }

                hasResult = FALSE;
                break;  
            }            

        case SE_KEY_F1:  // DOWN!
            {
                STATIC  UINT8   nVolume;
                nVolume = SKY_GET_GAME_VOLUME();

                if (nVolume != 0) {
                    SKY_SET_GAME_VOLUME(nVolume-1);
                    SKY_ControlMusicVolume(nVolume-1);
                }

                hasResult = FALSE;
                break;  
            }

        /*
            Timer Event
        */
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)  // 256 color
        case SE_SYS_PACEMAKER:
            hasResult = FALSE;
			UI_DISABLE_SLEEP();

            sSKA_GVMDrawAnnunciators(FALSE);            
            EQS_DEF_EVENT_PROC(pEvent);
            break;
#endif

        case SE_TM_TIMER2:  // SMS 관련
            hasResult = FALSE;

			UI_DISABLE_SLEEP();
            EQS_StopTimer(EQS_TIMER_2);
			
			/* traffic channel 끊는다 */
            if(IsCallTypeSMS() && g_nRunningType != GRT_RUN_PTP)
            {
				error("sms call disconnect!", 0, 0, 0);
                uasms_DTC_disconnect();
            }

            if (g_GVMSmsMoType == GSMT_SENDING) {
                
                error("[SMS] TimeOut!!!", 0, 0, 0);

#ifdef FEATURE_SMS_RETRY
                if(g_unTrySMS < NUM_OF_SEND_RETRIAL)
		        {
			        g_unTrySMS++; /* 다시시도 */
					EQS_StartTimer(EQS_TIMER_2, SMS_TIMEOUT, FALSE);
			        SendSMSMsg(&g_GVMSmsBuffer, g_GVMSmsMoTI);
		        }
		        else/* 전송실패 */
#endif
		        {
                    g_unTrySMS = 0; /* process for out */
                    g_GVMSmsMoType = GSMT_IDLE;
					if (IsCallTypeSMS()) l_fSMSEnded = TRUE;
                    hasResult = TRUE;

                    event.code = SWAP_EVENT_SMS_RESULT;
                    event.param.data = 0; // fail
                    result = scl_DoSwapEvent(&event);
                    error("[TIMER]SMS보내기 실패!!!!", 0, 0, 0);                    
		        }
            }
            break;

		case SE_TM_TIMER1:	// connection time-out check
			UI_DISABLE_SLEEP();
			error("connection timer expired!!!!!", 0, 0, 0);
			EQS_StopTimer(EQS_TIMER_1);

			if (NetworkGetState() == NET_PPPOPEN_S)	// already opened, then just skip!
			{
				hasResult = FALSE;
			}
			// not connected & can not get call_ended or net_disconnectd event, so send event here
			else if (NetworkGetState()== NET_NULL_S || NetworkGetState() == NET_CLOSED_S)
			{
				g_eDisconnectType = DT_NORESULT;
				midd_Disconnect(SWAP_RR_DISCONNECT);  

				event.code = SWAP_EVENT_CONNECT_RESULT;
                event.param.data = 0; // FAIL
                result = scl_DoSwapEvent(&event);
			}
			// opening? or closing? -> just call disconnect function
			else 
			{
				hasResult = FALSE;
				l_fConnectFailed = TRUE;
				midd_Disconnect(SWAP_RR_DISCONNECT);  
			}
			break;

        case SE_TM_TIMER:       // Disconnection time-out check
            
			UI_DISABLE_SLEEP();
		    SKY_SetGeneralTimer(0, FALSE);			
			hasResult = FALSE;

            
            error("ppp close TimeOut!!! => Netstate=%d", NetworkGetState(), 0, 0);
            l_fPPPOpened = FALSE;

			// already disconnected, don't care about this case.
			if (NetworkGetState() == NET_NULL_S || NetworkGetState() == NET_CLOSED_S) 
			{
				// after 20 seconds,, ... so just send event -_-;;
				EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA);
			}
            else    // disconnect time-out
            {                    
				midd_Disconnect(SWAP_RR_DISCONNECT);                        
            }
            break;
        
        /*
            MessageBox Event
        */
        case SE_IO_MESGBOX_END:
            
            hasResult = FALSE;

            if (pEvent->dwParam == SE_SOFTKEY_WAP) 
			{
                // backtobrowser
                if (SKY_IS_PHONE_CLOSE())   // 폰을 닫으면서 끝낼 경우  <- 예외 상황
                {
                    EQS_END_TO(EQS_POP_TO_ROOT);
                } 
                else 
                {
                    error("이제 WAP으로 감~~~~~~~~~", 0, 0, 0);
                    SKY_SET_TO_GVM(FALSE);
                    EQS_END(); // SKA_MainGVM에서 WAP으로 감
                }
            }
            else if (pEvent->dwParam == SE_KEY_END)    // 끝내기
			{
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            
            EQS_DEF_EVENT_PROC(pEvent);
            break;

        /*
            Sys Event
        */
        case SE_KEY_PHONE_CLOSE:
            error("[SE_KEY_PHONE_CLOSE] <- RUN", 0, 0, 0);
            event.code = SWAP_EVENT_MENU;
            event.param.menu.event = SWAP_MENU_EXIT;
            result = scl_DoSwapEvent(&event);
            break;
		
        default :            
            hasResult = FALSE;
            EQS_DEF_EVENT_PROC(pEvent);
            break;
    }

    if (hasResult && result->code != SWAP_RC_NONE) 
    {
        switch(result->code) 
        {
            case SWAP_RC_ERROR:
                error("<<SWAP_RC_ERROR>>!!!", 0, 0, 0);
				
                EQS_START_SIBLING_EVENT(sSKA_ShowErrorMsg, SE_APP_START, 
                    EQS_NA, (DWORD)result->param.data, FALSE);
                break;

            case SWAP_RC_EXIT:
                error("<<SWAP_RC_EXIT>>!!!", 0, 0, 0);
                SKY_SET_TO_GVM(FALSE);
                SKY_SET_PLAYING_GVM(FALSE);  
				     
                if (SKY_IS_PHONE_CLOSE())   // 폰을 닫으면서 끝낼 경우
                {
                    EQS_END_TO(EQS_POP_TO_ROOT);
                }                
                else if (pEvent->EventID == SE_CALL_INCOMING) // 예기치 못한 상황에 착신 시 종료
                {
                    EQS_END_TO_EVENT(EQS_POP_TO_ROOT, SE_CALL_INCOMING, EQS_NA, EQS_NA);                
                }
                else // 종료 메세지 출력
                {
                    hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_MESSG_CONFIRM_EXIT), 
                                             2, 
                                             MBT_NONE, 
                                             MBT_SUCCESS);
					EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
                
                    EQC_SET_USERDATA(hMessageBox, SE_KEY_END);
                }
                break;

            /*
                다운로드 시 메모리 부족할 경우...
            */
            case SWAP_RC_OUT_OF_MEM:
                error("<<OUT_OF_MEM>>!!!", 0, 0, 0);

                l_dwScriptSize = result->param.ssb.size; // script size
                l_dwLackSize = result->param.ssb.lack; // 부족한 Flash-SSb 
                l_nSSBIndex = result->param.ssb.index; // Flash-SSB index
                
                sSetColorDisplay(FALSE);
				EQS_START_CHILD(sSKA_GVMFilesInfo);
                break;

            case SWAP_RC_TEXTEDIT:
                error("<<SWAP_RC_TEXTEDIT>>!!!", 0, 0, 0);

                l_szEditTitle = (BYTE*)result->param.edit.title;
                l_szEditInitialString = (BYTE*)result->param.edit.input;
                l_nEditMaxLen = (int)result->param.edit.maxlen;
                l_nEditStrType = (UINT8)result->param.edit.strtype;

                l_szEditedText = (BYTE*)SKY_Malloc(l_nEditMaxLen+1);
                memset(l_szEditedText, 0, l_nEditMaxLen+1);

                sSetColorDisplay(FALSE);
				EQS_START_CHILD(sSKA_TextEdit);
                break;

#ifdef SWAP_FEATURE_NETWORK
            /*
                TCP/IP 관련..
            */
            case SWAP_RC_CONNECT:
                error("<<RC_CONNECT!!!>> - GRT:%d", g_nRunningType, 0, 0);
                if (result->param.svr.reasons == 0)
                {
                    g_TypeConnectSvr = GCST_DOWNLOAD;
					g_nRunningType = GRT_DOWNLOAD_START;	// 주의해서 보기!!!					
                }
                else    // 게임 서버 연결 
                {                    
                    g_TypeConnectSvr = GCST_GAME;					
					if (g_nRunningType == GRT_NONE)
					{
						error("현재 RunningType이 NONE임", 0, 0, 0);
						g_nRunningType = GRT_RUN_START;	// 주의해서 보기!!!
					}
                }
				g_eDisconnectType = DT_NONE;
				
                //SKY_SetGeneralTimer(PPP_TIMEOUT, FALSE);
				EQS_StartTimer(EQS_TIMER_1, PPP_TIMEOUT, FALSE);
                midd_Connect(result->param.svr.ip, result->param.svr.port, result->param.svr.reasons);
                break;

            case SWAP_RC_DISCONNECT:
                error("<<SWAP_RC_DISCONNECT>>!!!", 0, 0, 0);
                
                //SKY_SetGeneralTimer(PPP_TIMEOUT, FALSE);
                midd_Disconnect(SWAP_RR_DISCONNECT);
                break;

            /*
                Edit Chatting Text
            */
            case SWAP_RC_CHATEDIT:
                error("<<SWAP_RC_CHATEDIT>>!!!", 0, 0, 0);

                sSetColorDisplay(FALSE);
                l_fChatEditPaused = FALSE;

				EQS_START_CHILD(sSKA_EditGVMChatText);
                break;
			
			case SWAP_RC_DOWNLOAD_CANCEL:
				error("<<SWAP_RC_DOWNLOAD_CANCEL>>", 0, 0, 0);				
				g_nRunningType = GRT_RUN_START;
                SKY_SET_PLAYING_GVM(TRUE);
				
				sSetSSBIndex(l_nScriptCPID, l_nScriptID);

				// WAP -> GVM & IS-95C 여서 pppopen중이면...
                if (NetworkGetState() == NET_PPPOPEN_S) 
                {                    
					g_eDisconnectType = DT_NORESULT;
					//SKY_SetGeneralTimer(PPP_TIMEOUT, FALSE);
                    midd_Disconnect(SWAP_RR_DISCONNECT);
					break;
                }
                break;

#endif // NETWORK

#ifdef SWAP_FEATURE_PTP
            case SWAP_RC_PTPCALL:
                error("<<SWAP_RC_PTPCALL>>!!!", 0, 0, 0);
                
                g_nRunningType = GRT_RUN_PTPCALL;
                g_eDisconnectType = DT_NONE;
                
                midd_PtpCall(result->param.call.dial);
                break;

            case SWAP_RC_PTPWAIT:
                error("<<SWAP_RC_PTPWAIT>>!!!", 0, 0, 0);
                
                g_nRunningType = GRT_RUN_PTPWAIT;
                g_eDisconnectType = DT_NONE;
                
                midd_PtpWait();
                break;

            case SWAP_RC_PTPRELEASE:
                error("<<SWAP_RC_PTPRELEASE>>!!!", 0, 0, 0);
                
                midd_PtpRelease(SWAP_RR_PTPDISCONNECT);
                break;
#endif // PTP

            /*
                CALL관련
            */
            case SWAP_RC_INCOMINGCALL:
                error("<<SWAP_RC_INCOMINGCALL>>!!!", 0, 0, 0);

                g_fGVMPaused = TRUE;
                SKY_SET_FROM_GVM(TRUE);
                
                /* goto call */
                EQS_PASS_CURRENT_EVENT();
                break;

            case SWAP_RC_VOICECALL:
                error("<<SWAP_RC_VOICE>>!!!", 0, 0, 0);
                
				midd_VoiceCall(result->param.call.dial, result->param.call.ret);
				break;

            case SWAP_RC_BACK_TO_BROWSER:
                error("<<BACK_TO_BROWSER>>!!!", 0, 0, 0);
                
                //sCheckScriptInfo();
                if (SKY_IS_PHONE_CLOSE())   // 폰을 닫으면서 끝낼 경우 <- 예외 상황
                {
                    EQS_END_TO(EQS_POP_TO_ROOT);
                } 
                else 
				{
                    midd_BackToBrowser(result->param.str);
                }
                break;
			
            default:
				error("WHAT RESULT???  -%d", result->code, 0, 0);
                break;

        } // end of switch        
    }
    hasResult = TRUE;
	if (fDereg)	// key event dereg시켰던것을 다시 reg함.
	{
#define KEY_FIRST_REPEAT 1200  // 늘림.  init:600
#define KEY_NEXT_REPEAT 300

		SKY_RegKeyCB( (int4)KEY_FIRST_REPEAT, (int4)KEY_NEXT_REPEAT, TRUE );
		fDereg = FALSE;
	}
}


/*
    밑의 애플릿에서 사용됨. - Control 나열
*/
LOCAL BOOL sAddGVMControls(VOID)
{
    UINT8   i, nCtrl, unIdx;
    BYTE	pszTmpBuffer[7];
    int     nUsedSize;
	signed long	nLackSize;

    unIdx = 0;
    nUsedSize = 0;
    
    for(i=0; i<SWAP_SSB_SIZE; i++)
    {
        l_anGVMFileIdx[i] = GVM_ERROR;
    }
	
    for(i=0; i<SWAP_SSB_SIZE; i++)
    {
        if (SKY_GET_GVM_SSB(i) && scl_ModifyScriptInfo(i, 0) == 0) //if(SKY_GET_GVM_GAME_CPID(i) != 0)
        {
            l_pabGVMName[unIdx] = (BYTE*)SKY_GET_GVM_GAME_NAME(i);
            GetGVMFileName(l_ppGVMFileName[unIdx], SKY_GET_GVM_GAME_CPID(i), SKY_GET_GVM_GAME_ID(i));
            l_anGVMFileIdx[unIdx] = i;
            unIdx++;
            nUsedSize += (int)SKY_GET_GVM_GAME_SIZE(i);
        }
    }
    
    /*
    ** ***************************************************************************
    ** CREATE
    ** ***************************************************************************
    */
    l_hFileList = EQC_ScrollList(EQS_NA,
                                 EQS_NA, 
                                 17, 
                                 3,
                                 l_pabGVMName, 
                                 unIdx, 
                                 unIdx,
                                 0);
    EQC_SET_STYLE(l_hFileList, CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_NO_CML);
        
    /* 모자란 용량 -> 실시간 중계 */
    l_hLackSizeTitle = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_SIZE_LACK);
    l_hLackSize = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
	nLackSize = (signed long)((signed long)l_dwScriptSize - (signed long)g_dwFreeSpace);
    if (nLackSize > (signed long)0) //l_dwLackSize > 0) 
    {
        sprintf((char*)pszTmpBuffer, 
                (char*)SKY_GET_TOKEN(TKN_GVM_FIXED_FILE_SIZE_FORMAT), 
                GET_KB_SIZE(nLackSize)); //l_dwLackSize));
    }	
    else if (l_nSSBIndex == -1) 
    {
        sprintf((char*)pszTmpBuffer, "공간");
    }
	else 
	{
		sprintf((char*)pszTmpBuffer, "없음");
	}
    EQC_SetStaticText(l_hLackSize, pszTmpBuffer);

    /* 파일 용량 */ 
    l_hFileSizeTitle = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_SIZE_FILE);
    l_hFileSize = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    sprintf((char*)pszTmpBuffer, 
            (char*)SKY_GET_TOKEN(TKN_GVM_FIXED_FILE_SIZE_FORMAT), 
            GET_KB_SIZE(SKY_GetDataFileSize(SDF_GVM, l_ppGVMFileName[0])));
    EQC_SetStaticText(l_hFileSize, pszTmpBuffer);

    /* 사용한 용량 */
    l_hUsedSizeTitle = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_SIZE_ALLOC);
	l_hUsedSize = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    sprintf((char*)pszTmpBuffer,
            (char*)SKY_GET_TOKEN(TKN_GVM_NO_FIXED_FILE_SIZE_FORMAT),
            GET_KB_SIZE(nUsedSize));
    EQC_SetStaticText(l_hUsedSize, pszTmpBuffer);
    
    /* 남은 용량 */
    l_hFreeSizeTitle = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_SIZE_FREE);
    l_hFreeSize = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
	sprintf((char*)pszTmpBuffer,
            (char*)SKY_GET_TOKEN(TKN_GVM_NO_FIXED_FILE_SIZE_FORMAT), 
            GET_KB_SIZE(g_dwFreeSpace)); // 실제 삭제는 아니더라도 공간은 늘어남.
    EQC_SetStaticText(l_hFreeSize, pszTmpBuffer);

    EQS_SetIOCActive(l_hFileList);

    /* Formatting... */
    EQS_StartFormatter();
    for(nCtrl = 0; nCtrl < MAX_ALLOC_CTRLNUM ; nCtrl++)
    {
        if(nCtrl > 0 && (nCtrl % 2) == 0)    // 2, 4, 6
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_RIGHT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        else
        {
            if(!nCtrl)	// 첫번째 것!
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
                EQS_NewLine();
            }
            else
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, TRUE);
            }
        }
    }
    EQS_EndFormatter();    
    return TRUE;
}

/*
    OutOfMemory 에러 발생시 공간 만들기 위한 Applet
*/
LOCAL VOID SKY_API sSKA_GVMFilesInfo(SKY_EVENT_T *pEvent)
{
    UINT8   unIdx;
    BYTE    pszTmpBuffer[7];
    BYTE    pszMessage[MESSAGE_BOX_MAX_MESG_LEN];
	HCONTROL	hMessageBox;
	INT4		nLackSize;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_DELETE_COMPLETE_NONE,
                      SKY_GET_TOKEN(TKN_GVM_DELETE_SCRIPT_MENU), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        if(!sHaveGVMFiles())
        {
            l_MessageReason = GDMT_NO_FILE;
            hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_DELETE_IMAGE_N_MELODY), 
										   5, 
										   MBT_NONE, 
										   MBT_HELP);
			EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else
        {
            l_MessageReason = GDMT_WAITING;
    	    hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WAITING_A_MOMENT), 
										   MESSAGE_DEFAULT_TIME, 
										   MBT_NONE, 
										   MBT_HELP);
			EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
        }

        break;

    case SE_APP_END:        
        EQS_ReleaseDC();
		break;        

    case SE_IO_SCROLL_LIST_CHG: /* 파일 용량 */
        sprintf((char*)pszTmpBuffer, 
                (char*)SKY_GET_TOKEN(TKN_GVM_FIXED_FILE_SIZE_FORMAT), 
                GET_KB_SIZE(SKY_GetDataFileSize(SDF_GVM, l_ppGVMFileName[pEvent->dwParam])));
        EQC_SetStaticText(l_hFileSize, pszTmpBuffer);
		break;

    case SE_SOFTKEY_DELETE: /* 컨텐츠 삭제 ? */
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(l_hFileList);
		sprintf((char*)pszMessage, 
                (char*)SKY_GET_TOKEN(TKN_GVM_FILE_NAME_TO_DELETE), 
                l_pabGVMName[unIdx]);
        STRNCAT(pszMessage, (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_Q_CONFIRM_DELETE),
                MESSAGE_BOX_MAX_MESG_LEN);
        l_MessageReason = GDMT_DELETE_CONFIRM;
        EQC_MessageBox(pszMessage, 
                       MESSAGE_DEFAULT_TIME, 
                       MBT_YESNO, 
                       MBT_CONFIRM);
        break;

    case SE_IO_SCROLL_LIST_SELECT: /* ok키 */ 
    case SE_SOFTKEY_COMPLETE: /* 완료 */
        EQS_END();
        break;

    case SE_IO_MESGBOX_YES: /* 삭제 OK */
        if (l_MessageReason == GDMT_EXIT_CONFIRM)   /* 공간 확보전에 끝낼경우*/
        {
            EQS_END();
            break;
        }
        
        if(l_MessageReason != GDMT_DELETE_CONFIRM) 
            break;
              
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(l_hFileList);        
        if (l_nSSBIndex == -1) 
        {
            l_nSSBIndex = l_anGVMFileIdx[unIdx];
            midd_DeleteScript(l_nSSBIndex); // 삭제 요청만 기록..
        }
        else 
        {
            midd_DeleteScript(l_anGVMFileIdx[unIdx]); // 삭제 요청만 기록..
        }
		
        EQC_DeleteScrollData(l_hFileList, unIdx);
        l_anGVMFileIdx[unIdx] = GVM_ERROR;

		l_MessageReason = GDMT_DELETING;
    	hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WAITING_A_MOMENT), 
									   MESSAGE_DEFAULT_TIME, 
									   MBT_NONE, 
									   MBT_HELP);
		EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
	                                    CS_MESSAGE_BOX_IGNORE_KEYS);
		break;

    case SE_IO_MESGBOX_NO: /* 삭제 NO */
		EQS_SetIOCActive(l_hFileList);
		break;

    case SE_IO_MESGBOX_END:
        switch(l_MessageReason)
		{
            case GDMT_WAITING:
                sAddGVMControls();
                l_MessageReason = GDMT_REQUEST;
    	        hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_MESSG_CHOOSE_DELETE_FILE), 
											   MESSAGE_DEFAULT_TIME, 
											   MBT_NONE, 
											   MBT_HELP);
				EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
                break;

            case GDMT_REQUEST:
                EQS_FitCanvasSize();
                break;

            case GDMT_NO_FILE: /* 컨텐츠가 없을 경우 */
                EQS_END_PARAM(EQS_NA, SE_SYS_TO_IDLE); //EQS_END();
		        break;


            case GDMT_DELETED_ALL: /* 컨텐츠를 다 삭제했을 경우 */
                EQS_SEND_EVENT(SE_SOFTKEY_COMPLETE, EQS_NA, EQS_NA);
                break;

            case GDMT_DELETED:
				
				nLackSize = (signed long)(g_dwFreeSpace - l_dwScriptSize);
				if ( nLackSize > 0 && l_nSSBIndex != -1)	// 만약 공간이 확보되었다면?
				{
					l_MessageReason = GDMT_DELETED_ALL;
                    hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_DOWNLOAD_RETRY), 
												   3, 
												   MBT_NONE, 
												   MBT_HELP);
					EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				}				

                if(!sHaveGVMFiles())
                {
                    l_MessageReason = GDMT_DELETED_ALL;
                    hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_NO_FILE), 
												   5, 
												   MBT_NONE, 
												   MBT_HELP);
					EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
                }
                else
                {
				    EQS_FitCanvasSize();
                    EQS_SetIOCActive(l_hFileList);
                }
                break;

            case GDMT_DELETING:
			    /* delete control */
			    EQS_DeleteControl(l_hFileList);
                EQS_DeleteControl(l_hLackSizeTitle);
                EQS_DeleteControl(l_hLackSize);
			    EQS_DeleteControl(l_hFileSizeTitle);
			    EQS_DeleteControl(l_hFileSize);
			    EQS_DeleteControl(l_hUsedSizeTitle);
			    EQS_DeleteControl(l_hUsedSize);
			    EQS_DeleteControl(l_hFreeSizeTitle);
			    EQS_DeleteControl(l_hFreeSize);

			    /* create control again */
			    sAddGVMControls();
                l_MessageReason = GDMT_DELETED;
                hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_MESSG_CONFIRM_DELETE), 
											   MESSAGE_DEFAULT_TIME, 
											   MBT_NONE,
											   MBT_SUCCESS);
				EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
                break;

            default:
                break;
        }
        break;

	case SE_KEY_CLEAR:
    case SE_IO_MESGBOX_CLEAR: /* MessageBox에서 CLEAR키 눌렀을 경우 */
    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
        {     
            EQS_END_PARAM(EQS_NA, SE_KEY_END);
        }
        break;

    case SE_KEY_END:
    case SE_IO_TO_IDLE: /* MessageBox에서 END키 눌렀을 경우 */
        EQS_END_PARAM(EQS_NA, SE_KEY_END);
		break;

    case SE_KEY_PHONE_CLOSE:
        EQS_END_TO(EQS_POP_TO_ROOT);
		break;

    case SE_SYS_TO_IDLE:
		break;

	case SE_CALL_INCOMING:
		// 여기서 data call이 들어올 가능성 없다. 만약 PTP라면 호를 끊어주어야 할지도.
		if (SKY_IsVoiceCall()) EQS_END_CURRENT_EVENT();
		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*
    문자 입력 Applet
*/
VOID SKY_API sSKA_TextEdit(SKY_EVENT_T *pEvent)
{    
    STATIC  HCONTROL                hTextEdit;
    STATIC  TEXT_EDIT_MODE_E        EditMode;
    STATIC  UINT8                   unLine;
    STATIC  UINT8                   unCharNumPerALine = 16;
			int                     nLenOfRcvBuf = 0;
			S_Event					event;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        error("[APP_START] <- SKA_TextEdit", 0, 0, 0);
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY,                      
                      NULL,
                      l_szEditTitle,
                      WA_SHOW_HEADER);
        
		/*
        ** ***************************************************************************
        ** CREATE
        ** ***************************************************************************
        */
        switch(l_nEditStrType) 
        {
            case GEST_DEFAULT:          // 일반 문자열 입력
                EditMode = TEM_KOREAN;
                break;
            case GEST_NUMBER:           // 숫자 문자열 입력                
            case GEST_PASSWORD_NUMBER:  // 패스워드용 숫자 문자열 입력
                EditMode = TEM_NUMBER;
                break;
            default:
                EditMode = TEM_KOREAN;
                break;
        }
        unLine = (l_nEditMaxLen/unCharNumPerALine)+1;
        
        if (EditMode == TEM_NUMBER) 
        {
            if (l_fEditPaused) 
            {
                hTextEdit = EQC_NumberEdit(EQS_NA, EQS_NA, unCharNumPerALine, unLine, l_nEditMaxLen,l_szEditedText);
            }
            else 
            {
                hTextEdit = EQC_NumberEdit(EQS_NA, EQS_NA, unCharNumPerALine, unLine, l_nEditMaxLen,l_szEditInitialString);
            }
            if (l_nEditStrType == GEST_PASSWORD_NUMBER) 
            {
                EQC_SET_STYLE(hTextEdit, CS_NUMBER_EDIT_DEFAULTS | CS_NUMBER_EDIT_PASSWD
                            | CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_DIGIT_ONLY | CS_NUMBER_EDIT_ALONE);
            }
            else 
            {
                EQC_SET_STYLE(hTextEdit, CS_NUMBER_EDIT_DEFAULTS
                            | CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_DIGIT_ONLY | CS_NUMBER_EDIT_ALONE);
                            
            }
            EQC_NotifyNumberEditMatchLen(hTextEdit, l_nEditMaxLen);
        }
        else 
        {
            if (l_fEditPaused) 
            {
                hTextEdit = EQC_TextEdit( EQS_NA,
                                      EQS_NA,
                                      unCharNumPerALine,
                                      unLine,
                                      EditMode,
                                      l_nEditMaxLen, 
                                      l_szEditedText,
                                      0 ); /* data */                
            }
            else 
            {
                hTextEdit = EQC_TextEdit( EQS_NA,
                                      EQS_NA,
                                      unCharNumPerALine,
                                      unLine,
                                      EditMode,
                                      l_nEditMaxLen, 
                                      l_szEditInitialString,
                                      0 ); /* data */
            }
			EQC_SET_STYLE(hTextEdit, CS_TEXT_EDIT_BORDER |
                                CS_TEXT_EDIT_ALONE);			
        }

        // Screen Formatting
        EQS_StartFormatter(); 
        EQS_NewLine();
        EQS_SetControlPosition(hTextEdit, ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
        EQS_NewLine();
        EQS_EndFormatter();

        EQS_SetIOCActive(EQS_GetNthIOC(0));
        EQS_FitCanvasSize();
        
        l_fEditPaused = FALSE;
        break;

    case SE_APP_END:
        error("[APP_END] <- SKA_TextEdit", 0, 0, 0);
        EQS_ReleaseDC();
        break;

    case SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(0), l_szEditedText);
        EQS_END();
        break;

    case SE_IO_TEDIT_DONE:  // 문자입력 완료
        EQC_GetTextEditData(EQS_GetNthIOC(0), l_szEditedText);
        EQS_END();
        break;

    case SE_IO_NEDIT_CANCEL:
    case SE_IO_TEDIT_CANCEL:
    case SE_KEY_CLEAR:
        if(pEvent->dwParam != EQC_DELETED) 
        {
            STRNCPY(l_szEditedText, l_szEditInitialString, l_nEditMaxLen);    // 취소시 초기값 setting
            EQS_END();
        }        
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
        error("[KEY_END] <- SKA_TextEdit", 0, 0, 0);        
        EQS_END_PARAM(EQS_NA, SE_KEY_END);
		break;

    case SE_RECV_DATA: // ptp, server중에 chatting중...
        //l_fDataReceived = TRUE;

        while(TRUE) 
        {
            memset(l_acRcvBuf, 0, NET_MAX_SIZE+1);
#ifdef SWAP_FEATURE_PTP
            if (g_nRunningType == GRT_RUN_PTP) 
                midd_PtpRecvData((BYTE*)l_acRcvBuf, &nLenOfRcvBuf);
            else 
#endif
                midd_recvdata(l_acRcvBuf, NET_MAX_SIZE, &nLenOfRcvBuf);
        
            error("[se_recv_data]data=%d", nLenOfRcvBuf, 0, 0);
            if (nLenOfRcvBuf > 0) 
            {
                event.code = SWAP_EVENT_MENU;
                event.param.menu.event = SWAP_MENU_RECEIVE;
                event.param.menu.buf = (unsigned char*)l_acRcvBuf;                    
                event.param.menu.data = nLenOfRcvBuf;
                scl_DoSwapEvent(&event);

                nLenOfRcvBuf = 0;
            }
            else 
            {
                break;
            }
        }

        break;        

    case SE_CALL_INCOMING:  // 한글입력state에서 착신->임시로 저장하고 통화이후다시 재개
        error("[CALL_INCOMING] <- TextEdit!!", 0, 0, 0);
        
		// 여기서 data call이 들어올 가능성 없다. 만약 PTP라면 호를 끊어주어야 할지도.
		if (SKY_IsVoiceCall())         
		{
			/* 기존 데이터 저장 -> l_szEditedText */
			if (l_nEditStrType == GEST_NUMBER || l_nEditStrType == GEST_PASSWORD_NUMBER)    // 숫자 데이터
			{
				EQC_GetNumberEditValue(EQS_GetNthIOC(0), l_szEditedText);
			}
			else 
			{
				EQC_GetTextEditData(EQS_GetNthIOC(0), l_szEditedText);
			}

			g_fGVMPaused = TRUE;
			l_fEditPaused = TRUE;
			SKY_SET_FROM_GVM(TRUE);

			/* goto call */
			EQS_PASS_CURRENT_EVENT();
		}
        break;

    case SE_CALL_ENDED:
        EQS_END_CURRENT_EVENT();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;

    }
}


/*
    Chatting Text 편집 Applet
*/
LOCAL VOID SKY_API sSKA_EditGVMChatText(SKY_EVENT_T *pEvent)
{
    STATIC  INT16       i;
    STATIC  UINT8       unCtrl, j;
    STATIC  BYTE        abEditText[SWAP_RSVD_CHAT_SIZE+1];
    STATIC  HCONTROL    hMessageBox;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_GetDC(TRUE);
		
		EQS_SetWindow(SKY_DEF_WIN_X,
					SKY_DEF_WIN_Y, 
					SKY_DEF_WIN_DX, 
					SKY_DEF_WIN_DY, 
					CL_NONE_SELECT_SAVE,  
					SKY_GET_TOKEN(TKN_GVM_CHATTING_EDIT),
					WA_SHOW_CML | WA_SHOW_HEADER);


        for(i=0; i<GVM_LEN_CHATTEXT;i++)
        {
            EQC_StaticText(EQS_NA, EQS_NA, (SKY_TOKEN)l_atGVMChatTextNum[i]);
            EQC_TextEdit(EQS_NA,
                         EQS_NA,
                         14,
                         1,
                         TEM_KOREAN, 
                         SWAP_RSVD_CHAT_SIZE,
                         (BYTE*)(SKY_GET_GVM_CHATTEXT(i)),
                         0);
        }

        /* Formatting */
        EQS_StartFormatter();        
        for(unCtrl = 0; unCtrl < (GVM_LEN_CHATTEXT*2) ; unCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(unCtrl), 
                                   ES_AUTO, 
                                   0, 
                                   EHA_LEFT, 
                                   EVA_CENTER, 
                                   FALSE);
            if(unCtrl%2 == 1)
                EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();
        break;

    case SE_APP_END:
		EQS_ReleaseDC();
        break;
        
    case SE_SOFTKEY_SAVE:
        if(EQS_IsControlDataChanged())
        {
            j=0;
            for(i = 1; i < GVM_LEN_CHATTEXT*2; i += 2)
            {
                memset(abEditText, NULL, SWAP_RSVD_CHAT_SIZE+1);
                EQC_GetTextEditData(EQS_GetNthIOC(i), (BYTE*)abEditText);
				
                if (STRCMP((BYTE*)SKY_GET_GVM_CHATTEXT(j), (BYTE*)abEditText) != 0)
                {
                    SKY_SET_GVM_CHATTEXT(j, abEditText);
                    STRNCPY( (BYTE*)svRsvdChat[j], abEditText, SWAP_RSVD_CHAT_SIZE);
                }
                j++;
            }
        }
		hMessageBox = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_SAVE), 
                                     MESSAGE_DEFAULT_TIME, 
                                     MBT_NONE, 
                                     MBT_SUCCESS);
		EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
        EQC_SET_USERDATA(hMessageBox, pEvent->dwParam);
        break;

    case SE_IO_TEDIT_DONE:
        break;  
    case SE_IO_TEDIT_CANCEL:
        break;

	case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
    		hMessageBox = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME,
                                         MBT_YESNO, 
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hMessageBox, SE_KEY_CLEAR);
        }
		else
			EQS_END();
        break;

	case SE_KEY_PHONE_CLOSE:	//	ㅂ바뀐 내용은 저장하나 마나?
		EQS_END();
		break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
    		hMessageBox = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME,
                                         MBT_YESNO, 
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hMessageBox, SE_KEY_END);
        }
		else
            EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        if (pEvent->dwParam != EQS_NA) 
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, pEvent->dwParam);
        else
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(pEvent->dwParam == SE_KEY_END || pEvent->dwParam == SE_KEY_CLEAR)
            EQS_END();
        break;

    case SE_IO_MESGBOX_END:
        EQS_END();
        break;

    case SE_CALL_INCOMING:  // 착신이 되면 모두 저장하고 나가기
        error("[CALL_INCOMING] <- EditChatText!!", 0, 0, 0);
        
		// 여기서 data call이 들어올 가능성 없다. 만약 PTP라면 호를 끊어주어야 할지도.
		if (SKY_IsVoiceCall()) 
		{
			/* 기존 데이터 저장 -> l_szEditedText */ // -> 무조건 저장?
			j=0;
			for(i = 1; i < GVM_LEN_CHATTEXT*2; i += 2, j++)
			{
				memset(abEditText, NULL, SWAP_RSVD_CHAT_SIZE+1);
				EQC_GetTextEditData(EQS_GetNthIOC(i), (BYTE*)abEditText);                
				SKY_SET_GVM_CHATTEXT(j, abEditText);
				STRNCPY( (BYTE*)svRsvdChat[j], abEditText, SWAP_RSVD_CHAT_SIZE);
			}
			g_fGVMPaused = TRUE;
			l_fChatEditPaused = TRUE;
			SKY_SET_FROM_GVM(TRUE);

			/* goto call */
			EQS_PASS_CURRENT_EVENT();
		}
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
	}
}

/*
    Error 메세지 출력하는 Applet -> 좀 더 손볼것
*/
LOCAL VOID SKY_API sSKA_ShowErrorMsg(SKY_EVENT_T *pEvent) {

            BYTE        *msg; //[MESSAGE_BOX_MAX_MESG_LEN];
    STATIC  BOOL        reload = FALSE;
    STATIC  DWORD       reload_msg;
	STATIC	HCONTROL	hMessageBox;

    switch(pEvent->EventID) 
    {
        case SE_APP_START:
            
            EQS_GetDC(TRUE);
            /* show the annunciator */
            EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y,
                        SKY_DEF_WIN_DX,
                        SKY_DEF_WIN_DY,
                        NULL,
                        NULL,
                        WA_SHOW_HEADER);

            switch(pEvent->dwParam) 
            {
                case SWAP_ERR_DECOMPFAIL:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_DECOMP_ERROR);                    
                    break;

                case SWAP_ERR_LOADFAIL:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_OUTOF_SCRIPT);                    
                    break;

                case SWAP_ERR_DOWNFAIL:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_DOWNLOAD_ERROR_SCRIPT);
                    break;

                case SWAP_ERR_DOWNFAILSIZE:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_SCRIPT_SIZE_ERROR);
                    break;

                case SWAP_ERR_DOWNFAILSUM:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_CHECKSUM_ERROR);
                    break;

                case SWAP_ERR_AUTHFAIL:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_AUTH_ERROR);
                    break;
                    
                case SWAP_ERR_NETSTARTFAIL:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_CONNECT_FAIL);
                    reload = TRUE;
                    reload_msg = pEvent->dwParam;
                    break;

                case SWAP_ERR_WRONGPTP:
                    msg = (BYTE*)SKY_GET_TOKEN(TKN_GVM_MESSG_WRONG_PTP);
                    break;

                default:
                    break;
            }
            hMessageBox = EQC_MessageBox((BYTE*)msg,
										   5, //MESSAGE_DEFAULT_TIME,
										   MBT_NONE,
										   MBT_HELP);
			EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);

            break;

        case SE_IO_MESGBOX_END:
            if (pEvent->dwParam == SE_KEY_END) 
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            else
            {                
                hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_MESSG_CONFIRM_EXIT), 
                             MESSAGE_DEFAULT_TIME, 
                             MBT_NONE, 
                             MBT_SUCCESS);
				EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
                EQC_SET_USERDATA(hMessageBox, SE_KEY_END);
            }
            break;

		case SE_KEY_PHONE_CLOSE:
        case SE_APP_END:			
            EQS_ReleaseDC();
            break;

        default:
            EQS_DEF_EVENT_PROC(pEvent);
            break;
    }
}


#define INDEX_ON_SCREEN(x, y, xWidth) ((x) + ((y)*(xWidth)))
#define CLR16TOCLR(COL16, COL8) do{\
		COL8 = ((COL16&0xE000)>>8)| ((COL16&0X0700)>>6)| ((COL16&0x0018)>>3) ;\
		}while(0)
#define PALETTE_SIZE_4GRAY              4
/*
    B2: 16비트 칼라 지원 LCD에 256칼라용 Bitmap을 그리기 위한 함수
*/
VOID SKY_API SKA_Put256Bitmap(INT16 x, INT16 y, CONST BITMAP* pBitmap, BYTE* buffer) 
{
    INT16 xWidth, yHeight=0;
    INT16 yOffset, xOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE bImageData;    
    int i,j;
	
	BYTE ForeColor, BackColor;
    BYTE* pbImageData;
	BYTE* pbScreenData;
    INT8    nBitPerPixel;
	
    ROM BYTE abBWMask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    ROM INT8 anBWShift[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
    ROM BYTE ab4GrayMask[] = { 0xC0, 0x30, 0x0C, 0x03 };
    ROM INT8 an4GrayShift[] = { 6, 4, 2, 0 };
    COLOR a4GrayColor[] = { WHITE, LIGHTGRAY, GRAY, BLACK };

    COLOR l_ForeColor = EQS_GetForeColor();
    COLOR l_BackColor = EQS_GetBackColor();
    CLR16TOCLR(l_ForeColor, ForeColor);
    CLR16TOCLR(l_BackColor, BackColor);
 	
    nBitPerPixel = pBitmap->Header.nBitsPerPixel;
	xWidth = pBitmap->Header.xWidth; 
    yHeight = pBitmap->Header.Size.yHeight;

    if (nBitPerPixel == BIT_PER_PIXEL_4G) 
    {
        xWidth <<= 1;
        if(pBitmap->Header.fPalette)
        {
            memcpy(a4GrayColor, pBitmap->pbPalette, sizeof(COLOR)*PALETTE_SIZE_4GRAY);
        }
    }
    
    if (xWidth <= 0 || yHeight <= 0) return;

    if (nBitPerPixel == BIT_PER_PIXEL_4G) { // 4 Gray
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
            if((y < 0) || ((y + yOffset) > SKY_SCREEN_HEIGHT-1))
                break;//continue;
            nBytePos = INDEX_ON_SCREEN(x, y+yOffset, SKY_SCREEN_WIDTH);
            /* get screen data at the position */
            pbScreenData = buffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>2)*(pBitmap->Header.yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+(j<<1)) < xWidth && j < 4; j++)
                    *(pbScreenData++) = a4GrayColor[(bImageData & ab4GrayMask[j]) >> an4GrayShift[j]];
            }
        }
    }
    else if(nBitPerPixel == BIT_PER_PIXEL_BW)   // BW
    {  
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
            if((y < 0) || ((y + yOffset) > SKY_SCREEN_HEIGHT-1 ))
                break;
            nBytePos = INDEX_ON_SCREEN(x, y+yOffset, SKY_SCREEN_WIDTH);
            /* get screen data at the position */
            pbScreenData = buffer + nBytePos;
            
            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yOffset);

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+j) < xWidth && j < 8; j++) 
                {
                    if ((bImageData & abBWMask[j]) >> anBWShift[j]) 
                        *(pbScreenData++) = ForeColor;
                    else 
                        *(pbScreenData++) = BackColor;
                        
                }
            }
        }
    }
    else if(nBitPerPixel == BIT_PER_PIXEL_64KC) // 64k color
    {  
	    for(yOffset = 0; yOffset < yHeight; yOffset++)
		{
            if((y < 0) || ((y + yOffset) > SKY_SCREEN_HEIGHT-1 ))
                break;//continue;
	        nBytePos =INDEX_ON_SCREEN(x, y+yOffset, SKY_SCREEN_WIDTH);

		    /* get screen data at the position */
	        pbScreenData = (BYTE*)buffer + nBytePos;
			
			pbImageData = (BYTE*)(pBitmap->pbData) + 2*(pBitmap->Header.Size.xWidth)*(pBitmap->Header.yStart+yOffset);
	        /* offset is the negative value, do not draw */

	        for(xOffset = 0; xOffset < xWidth; xOffset++)
		    {
			    //if(l_TransparentColor == *pbImageData) /* transparent color */
				if(pBitmap->Header.abReserved[0]!=1){
						UINT16 wColor;
			            wColor= (*(pbImageData++));//* 32;
						wColor|=(*(pbImageData++))<<8;
						CLR16TOCLR(wColor, bImageData);
						*(pbScreenData++)=bImageData;
				}
				else{
					if(pBitmap->Header.TransparentColor == *pbImageData)
					{
						pbScreenData++, pbImageData+=2;
					}
					else
					{
						UINT16 wColor = 0x0;
			            wColor=  (*(pbImageData++));//* 32;
						wColor|=(*(pbImageData++))<<8;
						CLR16TOCLR(wColor, bImageData);
						*(pbScreenData++) =  bImageData;
				    }
				}
			}
		}
	 }
//#endif
}
/*
** ***************************************************************************
** FUNCTIONS
** ***************************************************************************
*/
/*****************************************************************************
	char* uid,       // 선택한 게임에서 사용하는 사용자 이름
	int   cpid,      // CP의 id, 범위는           0x0000 ~ 0xfffe
	int   gid,       // 실행할 게임의 id          0x0000 ~ 0xffff
	char* dlsIP,     // 다운로드서버의 IP address "game.sinjisoft.com"
	int   dlsPort,   // 다운로드서버의 port       0 ~ 65535
	char* svrIP,     // 서버 IP address           
	int   svrPort,   // 서버 port
	char* retURL,    // 게임종료후 go back을 위한 URL
	char* Result
*******************************************************************************/

VOID GoToGVM( char* uid, int cpid, int gid, char* dlsIP, int dlsPort, 
              char* svrIP, int svrPort, char* retURL, char* Result )
{
#ifndef FEATURE_WAP
    ui_cmd_type	    *ui_cmd_ptr;
#endif //#ifndef FEATURE_WAP

    error("GoToGVM 불리당~ cpid:%d,gid:%d ----------", cpid, gid, 0);	

    if (g_fDownloadRequestFromSMS)  // SMS에서 오면 result는 NULL이다.
    {
        memset(Result, NULL, sizeof(Result) );
    
        if( uid == NULL || dlsIP == NULL || svrIP == NULL )
	    {
		    ERR("Invalid Parameter",0,0,0);
		    return;
	    }

	    if( cpid > 0xfffe || gid > 0xffff || dlsPort > 0xffff || svrPort > 0xffff )
	    {
		    ERR("Invalid Parameter",0,0,0);
		    return;
	    }
    }
    else 
    {
        memset(Result, NULL, sizeof(Result) );
    
        if( uid == NULL || dlsIP == NULL || svrIP == NULL || retURL == NULL  )
	    {
		    STRCPY((BYTE*)Result, (CONST BYTE*)"1003");
		    ERR("Invalid Parameter",0,0,0);
		    return;
	    }

	    if( cpid > 0xfffe || gid > 0xffff || dlsPort > 0xffff || svrPort > 0xffff )
	    {
		    STRCPY((BYTE*)Result, (CONST BYTE*)"1003");
		    ERR("Invalid Parameter",0,0,0);
		    return;
	    }
    }

    /* copy the data of script */    
	memset((VOID*)l_szUserID, 0x00, SWAP_LEN_USERID+1);
    STRNCPY(l_szUserID, (CONST BYTE*)uid, SWAP_LEN_USERID);
	
    l_nScriptCPID = cpid;
    l_nScriptID = gid;
    
	memset((VOID*)l_szDlsIP, 0x00, SWAP_LEN_IP+1);
    STRNCPY(l_szDlsIP, (CONST BYTE*)dlsIP, SWAP_LEN_IP);
	
	l_nDlsPort = dlsPort;
    
	memset((VOID*)l_szSvrIP, 0x00, SWAP_LEN_IP+1);
    STRNCPY(l_szSvrIP, (CONST BYTE*)svrIP, SWAP_LEN_IP);
	
	l_nSvrPort = svrPort;
    
	memset((VOID*)l_szRetURL, 0x00, SWAP_LEN_URL+1);
    if (g_fDownloadRequestFromSMS && retURL == NULL) 
        STRCPY(l_szRetURL, (CONST BYTE*)"");
    else 
        STRNCPY(l_szRetURL, (CONST BYTE*)retURL, SWAP_LEN_URL);
	        
	/* in order to go to netgame menu after exit */
    SKY_SET_TO_GVM(TRUE);

    if (!g_fDownloadRequestFromSMS)  // SMS에서 오면 result는 NULL이다.
    {
		g_fFromWAP = TRUE;	// SKY_SET_FROM_WAP사용안함

#ifndef FEATURE_WAP
	    if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
	    {
		    error( "No UI cmd buffers",0,0,0);
	    }
	    else		
	    {
		    ui_cmd_ptr->hdr.cmd        = UI_TONETGAME_F;
		    ui_cmd_ptr->hdr.task_ptr   = NULL;
		    ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		    ui_cmd( ui_cmd_ptr);
	    }	
#else //#ifndef FEATURE_WAP
        EQS_SEND_EVENT(SE_GOTO_GVM, EQS_NA, EQS_NA);
#endif //#ifndef FEATURE_WAP
    }
	
	return;
}



VOID GetGVMFileName(BYTE *pabFileName, WORD wCPID, WORD wGID)
{
    /* make the file name with CPID & GID */
    sprintf((char*)pabFileName, (char*)"GVM%04X%04X", wCPID, wGID);
    return;
}


/*
    Starting setting
*/
LOCAL VOID sStartGVM(VOID)
{
	g_GVMSmsMoType = GSMT_IDLE; // for SMS
    g_unTrySMS = 0;

    g_eDisconnectType = DT_NONE;	
	
	/* PtP IrDA를 닫아주기 위함 */
    if(!fIrDAPortClosed)
	{
		ChangeIrDAMode ( 0 );
		// Phone to PC IrDA가 off 되었음을 표시
		fIrDAPortClosed = TRUE;
        //gfbIrDAPortForPC = FALSE;
	}

	//	이부분이 빠지면 일부스크립트가 제대로 동작하지 않음. -> sky_conf.c로 옮김.
	/*srand(1);
	rand();*/

    return;
}

LOCAL VOID sEndGVM(VOID)
{
    /* Flag setting */
    g_eDisconnectType = DT_NONE;
    g_fDownloadRequestFromSMS = FALSE;
    g_TypeConnectSvr = GCST_NONE;
    g_nRunningType = GRT_NONE;
	
	SKY_WRITE_CONFIG(SC_GVM); // 마지막으로 UserData저장하기 위함.
	error("config file에 저장완료~~~~~~~~~~~~~`", 0, 0, 0);

    g_nCurSSB = 0xFF;
    g_unTrySMS = 0;    
	
	UI_ENABLE_SLEEP();	
    SKY_SET_PLAYING_GVM(FALSE);

	/* Sound */
	SKY_StopMusic();
	/* Vibrator */
	SKY_MotorStop();
	/* Backlight */    
	SKY_SET_NO_OFF_BACKLIGHT(FALSE);    
	SKY_SET_IGNORE_BACKLIGHT(FALSE);
	SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
	/* Key Tone */
	SKY_SET_KEY_TONE(TRUE);
    /* Timer */
    midd_StopTimer(0);
    midd_StopTimer(1);
    midd_StopTimer(2);
    SKY_SetGeneralTimer(0, FALSE);
	EQS_StopTimer(EQS_TIMER_1);
    EQS_StopTimer(EQS_TIMER_2);
	UI_ENABLE_SLEEP();

	sCheckScriptInfo();	// 여기넣는게 맞지 않을까?

	/* traffic channel 끊는다 */
    if(ui_curr_call_is_sms())
    {
        uasms_DTC_disconnect();
    }
    return;
}

/*
    이미지 저장 함수
*/
BOOL SaveGVM_BMPToFS(void *imgData, char *imgName)
{
    STATIC	HFILE	hFile;
	BITMAP			Image;      
    INT16           nWidth = SWAP_LCD_WIDTH;
    INT16           nHeight = SWAP_LCD_HEIGHT - SKY_COMMANDLINE_HEIGHT;
	DWORD           nSize=0; //nWidth*nHeight;
    	
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
    nSize = nWidth*nHeight*2;
#else
    nSize = nWidth*nHeight; // 256 color
#endif

    /* Bitmap Header */
    memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
    Image.Header.Size.xWidth = nWidth;
    Image.Header.Size.yHeight = nHeight;
    Image.Header.xWidth = nWidth;
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
    Image.Header.nBitsPerPixel = SKY_COLOR_SPACE;
#else
    Image.Header.nBitsPerPixel = 8; //SKY_COLOR_SPACE;
#endif
    Image.Header.fPalette = FALSE;  
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
    Image.Header.TransparentColor = MAX_COLOR_64K;
#else
    Image.Header.TransparentColor = MAX_COLOR_256;
#endif
    Image.Header.yStart = 0;    

    
    /* copy data */
    Image.pbData = (BYTE*)g_abBmpBuf;
    memset((VOID*)Image.pbData, 0x00, nSize);
    memcpy((VOID*)Image.pbData, imgData, nSize);
    
    /* save file */
    if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile) == SDR_OK)
    {
        SKY_WriteDataBlock(hFile, &Image.Header, sizeof(Image.Header));
        SKY_WriteDataBlock(hFile, Image.pbData, nSize);
        SKY_CloseDataFile(hFile);
		return TRUE;
	}   
	return FALSE;
}

/*
    이미지 저장위해 공간 체크하는 함수
*/
BOOL CheckDiskSpaceForSetGVMImage()
{

    /* check file number */
    if(SKY_GetFileNumberInDataFile(SDF_IMAGE) >= SKY_MAX_NUM_FILES)
    {
		ERR("Filenum_Full",0,0,0);
		return FALSE;
	}
	else
	{
		DWORD nFileSize = 0, nDiskSpace;

        /* check disk size */
        nDiskSpace = SKY_GetFreeSpaceInDataFile();

		/* check file size */
        nFileSize = sizeof(BITMAP_HEADER) + (SWAP_LCD_WIDTH*SKY_SCREEN_HEIGHT); 

        /* check space to save */
    	if(nDiskSpace < nFileSize )
			return FALSE;
		else
			return TRUE;
	}
    return FALSE;
}

/*
	이미 있는 스크립트여서 다운로드를 중단할때 g_nCurSSB를 설정해주기 위한 함수
*/
LOCAL void	sSetSSBIndex(const WORD cpid, const WORD gid)
{
	UINT8   i;
    g_nCurSSB = 0xFF;

    for (i=0; i<SWAP_SSB_SIZE; i++)
    {
		if (SKY_GET_GVM_GAME_CPID(i) == cpid && SKY_GET_GVM_GAME_ID(i) == gid)
		{
			g_nCurSSB = i;
			break;
		}
	}
}

/*
	다운로드 시 횟수/기간제한이 있는 스크립트이면서 그 유효기간이 expire되었을 경우
	새로 다운로드를 받기 위해서 midd_DeleteScript함수를 불러주어 삭제flag를 설정해주어서
	midd_WriteScriptInfo를 할때 이전의 스크립트를 삭제해주어야 한다.
	(SWAP모듈에서 해야 할 일이라고 생각이 되나 그렇지 않으므로 일단 이함수를 통해 그일을 수행)
*/
LOCAL VOID  sCheckValidScript(const WORD cpid, const WORD gid) //int ssbIndex) 
{
    UINT8   i, ssbIndex;   
	BOOL	valid = TRUE;	

    for (i=0; i<SWAP_SSB_SIZE; i++)
    {
		if (SKY_GET_GVM_GAME_CPID(i) == cpid && SKY_GET_GVM_GAME_ID(i) == gid)
        {            
            short year, month, day, wday; 
			short valid_year, valid_month, valid_day;

			ssbIndex = i;

			// valid count 조사
			if (SKY_GET_GVM_VALID_COUNT(ssbIndex) == 0) 
			{
				valid = FALSE;
				break;
			}

			// valid term 조사
			if (SKY_GET_GVM_VALID_TERM(i, 2) != 0xFF) 
			{
				if (midd_GetDate(&year, &month, &day, &wday)) 
				{
					year = year - 2000;
					valid_year  = SKY_GET_GVM_VALID_TERM(ssbIndex, 0);
					valid_month = SKY_GET_GVM_VALID_TERM(ssbIndex, 1);
					valid_day   = SKY_GET_GVM_VALID_TERM(ssbIndex, 2);

					if (valid_year < year) 
					{
						valid = FALSE;
						break;						
					}
					else if (valid_year == year) 
					{
						if (valid_month < month) 
						{
							valid = FALSE;
							break;							
						}
						else if (valid_month == month) 
						{
							if (valid_day < day) 
							{
								valid = FALSE;
								break;								
							}                
						}
					}
				}
			}
			break;
		}
    }

	if (valid == FALSE)
		midd_DeleteScript(ssbIndex);

    return;
}

/* 
** ---------------------------------------------------------------------------
** Compare the Length of Music Data  
** 
** return value : 0    - not equal
**                Size - Size of Music Data 
*/
DWORD GetLengthofMusicData(int audioType, int Length, int mode)
{
    dword Size = 0;
    extern byte *mb_MusicBellPreBuffer;

    switch(audioType)
    {
        case YAMAH_ma1:
            //실제로는 down받아 ma1으로 바꾼 file의 size이어야함
            Size = *mb_MusicBellPreBuffer;
            break;
        case YAMAH_mmf:
            //실제로는 down받아 ma1으로 바꾼 file의 size이어야함
            Cal_mbSize(Size, mb_MusicBellPreBuffer[4], mb_MusicBellPreBuffer[5],
                     mb_MusicBellPreBuffer[6], mb_MusicBellPreBuffer[7]); 
            break;
        default:
            break;
    }

    Size += (MM_NUM_FILE_TYPE*mode);    //4*0(play) / 1(download)

    if(Length+(MM_NUM_FILE_TYPE*mode) > (int)Size)
	{
        if((Length+(MM_NUM_FILE_TYPE*mode)-(int)Size) == 1)
            return Size;
        else
        {
		    if(mb_MusicBellPreBuffer)
		    {
			    error("mb_MusicBellPreBuffer =NULL",0,0,0);
			    SKY_Free((void*)mb_MusicBellPreBuffer);
			    mb_MusicBellPreBuffer =NULL;
		    }            
            return 0;
        }
	}
    else if(Length+(MM_NUM_FILE_TYPE*mode) == (int)Size)
    {
        return Size;
    }
    else
    {
        return 0;
    }    
}

/*
    256 칼라와 16비트 칼라의 경우 각각 필요한 값 setting
	모든 컬러일때 적용되지 않음.. 나중에 모든 display type에 맞게 고쳐야 함..
*/
VOID sSetColorDisplay(BOOL flag) {

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)  // 256 color
    g_fIn256ColorDisplay = flag;	
	
    EQS_SetDirectDraw(flag);	
    if (g_fIn256ColorDisplay) 
    {
		SKY_FillScreenImage(); //SKY_FillScreenOneColor(0xFF);
        SKY_256DisplayStart();		
        sSKA_GVMDrawAnnunciators(TRUE);
    }
    else 
    {
        SKY_256DisplayEnd(); 
	    SKY_DrawAnnunciators();
        SKY_UpdateScreen();
    }

#elif (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
    g_fIn256ColorDisplay = FALSE;   // draw를 GVM에서 하지 않음.
    EQS_SetDirectDraw(flag);
	if (flag) SKY_DrawAnnunciators();
#else
#endif  
}

/*
    256 color일때 Annunciator를 그려주는 함수
*/
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)  // 256 color
LOCAL VOID sSKA_GVMDrawAnnunciators(BOOL fAlways){ // ok!
    
    if (fAlways || SKY_CheckAnnunciators() )
    {
		SKY_Draw256Annunciators();
        SKY_256UpdatePartialDisplay((BYTE*)GetScreenBuffer(), 0, SKY_ANNUNCIATORS_HEIGHT-1);
	}
}
#endif 



/*
    GVM 파일 있는지 없는지 체크 
*/
LOCAL BOOL sHaveGVMFiles(VOID)
{
    UINT8 i;

    for(i=0; i<SWAP_SSB_SIZE; i++)
    {
        if (SKY_GET_GVM_SSB(i) && scl_ModifyScriptInfo(i, 0) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*
    GVM종료시 삭제flag가 setting되어 있는 스크립트를 찾아서 설정을 해제해주는 함수
*/
LOCAL VOID sCheckScriptInfo(void) 
{
    STATIC  UINT8   i=0;

    // 종료시.. 삭제기록만 되어 있는 것을 풀어준다.
    for (i=0; i < SWAP_SSB_SIZE; i++)  // i번째 스크립트에 삭제 Flag가 설정되었는지 확인        
    {
		if (scl_ModifyScriptInfo(i, 0)) 
        {
			// Clear script delete flag
			scl_ModifyScriptInfo(i, 1);
            
            // 만약 ssb정보도 없고 파일도 정상적이지 않다면 그냥 삭제해야????????????????????
		}
	}
}

/*
    EQUS/SKY Key code -> SWAP Key code
*/
LOCAL INT sGetSWAPKeyCode(int keycode) {
    switch(keycode) 
    {
        case SE_KEY_1:      return SWAP_KEY_1;
        case SE_KEY_2:      return SWAP_KEY_2;
        case SE_KEY_3:      return SWAP_KEY_3;
        case SE_KEY_4:      return SWAP_KEY_4;
        case SE_KEY_5:      return SWAP_KEY_5;
        case SE_KEY_6:      return SWAP_KEY_6;
        case SE_KEY_7:      return SWAP_KEY_7;
        case SE_KEY_8:      return SWAP_KEY_8;
        case SE_KEY_9:      return SWAP_KEY_9;
        case SE_KEY_0:      return SWAP_KEY_0;
        case SE_KEY_STAR:   return SWAP_KEY_STAR;
        case SE_KEY_POUND:  return SWAP_KEY_SHARP;
        case SE_KEY_CLEAR:  return SWAP_KEY_CLR;
        case SE_KEY_SEND:   return SWAP_KEY_SND;
        case SE_KEY_1_LONG: return SWAP_KEY_1_L;
        case SE_KEY_2_LONG: return SWAP_KEY_2_L;
        case SE_KEY_3_LONG: return SWAP_KEY_3_L;
        case SE_KEY_4_LONG: return SWAP_KEY_4_L;
        case SE_KEY_5_LONG: return SWAP_KEY_5_L;
        case SE_KEY_6_LONG: return SWAP_KEY_6_L;
        case SE_KEY_7_LONG: return SWAP_KEY_7_L;
        case SE_KEY_8_LONG: return SWAP_KEY_8_L;
        case SE_KEY_9_LONG: return SWAP_KEY_9_L;
        case SE_KEY_0_LONG: return SWAP_KEY_0_L;
        case SE_KEY_LEFT:   return SWAP_KEY_LEFT;
        case SE_KEY_RIGHT:  return SWAP_KEY_RIGHT;
        case SE_KEY_UP:     return SWAP_KEY_UP;
        case SE_KEY_DOWN:   return SWAP_KEY_DOWN;
        case SE_KEY_CENTER: return SWAP_KEY_OK;
        case SE_KEY_END:    return SWAP_KEY_END;
        default:            return 0;
    }
    return 0;
}