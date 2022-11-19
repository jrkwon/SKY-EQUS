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
//#define FEATURE_SMS_RETRY      // SMS������ ���н� �ٽ� �������ΰ��� ���� ����.
#ifdef FEATURE_SMS_RETRY
#define NUM_OF_SEND_RETRIAL	    	3
#endif

#define GVM_ERROR                   -1
#define MAX_VIEW_SCRIPT_NAME_LEN    14
#define	NET_MAX_SIZE	            668
#define SMS_TIMEOUT                 10*1000
#define MAX_ALLOC_CTRLNUM           9
#define MAX_GVM_VOLUME				5		/* ���� �ִ� ���� */

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

// ä�� ����� ���� ȭ�鿡�� ���� ���ڵ�
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

GVM_CONNECT_SERVER_TYPE_E       g_TypeConnectSvr = GCST_NONE;	// Server����Ÿ��
UINT8                           g_nCurSSB = 0;					// ���� �������� SSB index
BOOL                            g_fGVMPaused = FALSE;			// Call incoming������ ��� ���� ���� ǥ�� flag
UINT8		                    g_unTrySMS;						// SMS retrial Ƚ�� 
BOOL                            g_fDownloadRequestFromSMS = FALSE;	// SMS�� �ٿ�ε�� TRUe
BOOL                            g_fDeleteScript = FALSE;		// ���� ���� ���� ǥ��
BOOL                            g_fIn256ColorDisplay = FALSE;	// 256 color mode�� display�Ұ��ΰ��� ���� Flag

// GVM �ڵ� �������� �����ϴ� ����
DWORD                           g_dwFreeSpace;					// EFS�� ���� ���� ǥ��	
GVM_RUNNING_TYPE                g_nRunningType = GRT_NONE;		// ���� �����ϴ� mode (���� ������������ �����)
GVM_DISCONNECT_TYPE_E           g_eDisconnectType = DT_NONE;	// ��������Ÿ��
S_Result                        *recv_result = NULL; 
BOOL							g_fFromWAP = FALSE;				// WAP���� �Դ��� ǥ���Ұ� // SKY_IS_FROM_WAP ������
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
/* START - �ٿ�ε带 �ϱ� ���� �Ķ���͵� */
STATIC      BYTE                l_szUserID[SWAP_LEN_USERID+1];  // ������ ���ӿ��� ����ϴ� ����� �̸�
STATIC      int                 l_nScriptCPID;                  // CP�� id, ������           0x0000 ~ 0xfffe
STATIC      int                 l_nScriptID;                    // ������ ������ id          0x0000 ~ 0xffff
STATIC      BYTE                l_szDlsIP[SWAP_LEN_IP+1];       // �ٿ�ε弭���� IP address "game.sinjisoft.com"
STATIC      int                 l_nDlsPort;                     // �ٿ�ε弭���� port       0 ~ 65535
STATIC      BYTE                l_szSvrIP[SWAP_LEN_IP+1];       // ���� IP address           
STATIC      int                 l_nSvrPort;                     // ���� port
STATIC      BYTE                l_szRetURL[SWAP_LEN_URL+1];     // ���������� go back�� ���� URL
/* END - �ٿ�ε带 �ϱ� ���� �Ķ���͵� */

CHAR	                        l_acRcvBuf[NET_MAX_SIZE+1];		// Data Receive Buffer

// ��������� �޴�����
STATIC      DWORD               l_dwScriptSize;
STATIC      DWORD               l_dwLackSize;
STATIC      int                 l_nSSBIndex;
STATIC      HCONTROL            l_hFileList, l_hFileSize, l_hUsedSize, l_hFreeSize, l_hLackSize;
STATIC      HCONTROL            l_hFileSizeTitle, l_hUsedSizeTitle, l_hFreeSizeTitle, l_hLackSizeTitle;
STATIC      BYTE*               l_pabGVMName[SWAP_SSB_SIZE];
STATIC      INT8                l_anGVMFileIdx[SWAP_SSB_SIZE];
STATIC      BYTE                l_ppGVMFileName[SWAP_SSB_SIZE][SKY_PATH_NAME_MAX_LENGTH];
STATIC      GVM_MESSG_TYPE_E    l_MessageReason;

// ������ MMI�� ���������� ����ϴ� Flag
STATIC      BOOL                l_fEditPaused = FALSE; 
STATIC      BOOL                l_fChatEditPaused = FALSE;

// �����Է±⿡ �ʿ��� ������
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
LOCAL VOID SKY_API  sSKA_GVMFilesInfo(SKY_EVENT_T *pEvent);  // ������������ ���� ���ø�

LOCAL VOID          sStartGVM(VOID);
LOCAL VOID          sEndGVM(VOID);
LOCAL void			sSetSSBIndex(const WORD cpid, const WORD gid);
LOCAL VOID			sCheckValidScript(const WORD cpid, const WORD gid);
LOCAL VOID          sCheckScriptInfo(void);	// SKA_MainGVM���� ��������... 
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
    << ������ by undersea >>

	                            +-- SKA_ViewTextMessage(indirect) 
	                            |      or SKA_MessageNotif(direct) (SMS���� �ٿ�ε�)
                                |
     SKA_MainGVM       <--------+-- SKA_MainIdleScreen (AT Command�� ���� ��ũ��Ʈ �����) 
         |                      |
         |                      +-- xce_gvm_start (SK_VM���� �Ҹ���)
         |                      |
	     |                      +-- WAP����...
	  {child}
	     | 
	SKA_RunGVM
	     |
	     +---------------------+------------------------+
	     |			       	   |                        |
	  {child}	            {child}                  {child}
	     |		               |                        |
    sSKA_TextEdit     sSKA_EditGVMChatText      sSKA_GVMFilesInfo
    (�ؽ�Ʈ ����)     (ä���ؽ�Ʈ ����)       (���������-���丮)
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
			if (SKY_IS_PHONE_CLOSE())	// ���� ���� ���·� ������..
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
							if (P_REV_IS_6_OR_GREATER)	// 95C�̸� �׳� �Ѿ.
							{								
								EQS_START_CHILD(SKA_RunGVM);
							}
							else 
							{
                                

								SKY_SetGeneralTimer(WAIT4CALLENDED, FALSE);	// ȣ�� ����ǵ��� ��� ���
							}
                            break;
                        }
						else 
						{
							EQS_START_CHILD(SKA_RunGVM);
						}
                    }
					else	// At command�� �Ǵ� ����
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
				error("�̻��� ��Ȳ�̴�~ ����?", 0, 0, 0);
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
                // Ȥ�� ������ �� ������ ���¸� �����ִ� ��ƾ�� �־�� ���� ������...
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
    ���� ���ø�
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
				// ���� valid���� ���� �Ŷ� �ٿ�ε带 �޾ƾ� �Ѵٸ� midd_DeleteScript�� �ҷ���� �Ѵ�.
                sCheckValidScript(l_nScriptCPID, l_nScriptID);
                error("���� �ٿ�ε带 �޴´�", 0, 0, 0);
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

                if (g_fGVMPaused) // pause�Ǿ��ٰ� �ٽ� continue�ɶ�...
                {
                    g_fGVMPaused = FALSE;
					        
                    if (l_fEditPaused)     // editing�߿� �����̸�..
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
                else   // �������� ��ũ��Ʈ ����
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
                // ä�� ����� �����Ϸ� �̺�Ʈ �Ϸ� �˸�
                event.code = SWAP_EVENT_CHATEDIT;
                result = scl_DoSwapEvent(&event);
            }
            else if (EQS_GET_CHILD() == sSKA_TextEdit) 
            {                
                if (pEvent->dwParam == SE_KEY_END)     // �׳� ������.
                {
                    SKY_Free((void*)l_szEditedText);
                    l_szEditedText = NULL;

                    if (l_fConnected)    // ������ ���� ...
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
					// ���ڿ� �Է� �̺�Ʈ �Ϸ� �˸�
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
				// ���� ���� �Էµ� EndŰó�� -> n-top����
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
                case UASMS_ACCESS_TOO_LARGE_S:      /* Traffic Channel ���� */
                    uasms_DTC_connect();            /* message is too large to be sent over DTC */
                    break;
                case UASMS_WAITING_FOR_TL_ACK_S:    /* SMS ���� ���� : layer 2 ack */
                    break;                          /* intermediate status for msg requesting TL ack */
                case UASMS_OK_S:                    /* SMS ���� ���� : layer 3 ack */
			                                        /* 32768 */			        

					EQS_StopTimer(EQS_TIMER_2);
					
					/* traffic channel ���´� */
					if (IsCallTypeSMS() && g_nRunningType != GRT_RUN_PTP) //if(ui_curr_call_is_sms())
					{
						error("sms call discon!", 0, 0, 0);
						uasms_DTC_disconnect();
					}

					error("SMS������ ����~~~~", 0, 0, 0);
        	        g_GVMSmsMoType = GSMT_IDLE; // ��������.
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
						/* traffic channel ���´� */
						if (IsCallTypeSMS() && g_nRunningType != GRT_RUN_PTP) //if(ui_curr_call_is_sms())
						{
							error("sms call end!", 0, 0, 0);
							uasms_DTC_disconnect();
						}

#ifdef FEATURE_SMS_RETRY
                        if(g_unTrySMS < NUM_OF_SEND_RETRIAL) /* �ٽýõ� */
                        {
							g_unTrySMS++; 
							g_GVMSmsMoType = GSMT_SENDING;

							error("sms send retry~~~(%dth try)", (g_unTrySMS+1), 0, 0);
							EQS_StartTimer(EQS_TIMER_2, SMS_TIMEOUT, FALSE);

					        SendSMSMsg(&g_GVMSmsBuffer, g_GVMSmsMoTI);
                        }
                        else/* ���۽��� */
#endif
                        {
					        g_unTrySMS = 0; /* process for out */
					        g_GVMSmsMoType = GSMT_IDLE;	
                            hasResult = TRUE;
                            if (IsCallTypeSMS()) l_fSMSEnded = TRUE;

                            error("[SMS]SMS������ ����!!!!", 0, 0, 0);
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

		// ������ �������� ����!!!
        case SE_CALL_ENDED:     
            error("[CALL_ENDED, runtype:%d] <- RUN!!", g_nRunningType, 0, 0);
			error("discon type:%d", g_eDisconnectType, 0, 0);

            if(l_fSMSEnded || (IsCallTypeSMS() && g_nRunningType != GRT_RUN_PTP) ) // SMS�߽��� Call End
            {
                hasResult = FALSE;
                l_fSMSEnded = FALSE;
                break;
            }
            
#ifdef SWAP_FEATURE_PTP
            if (g_nRunningType == GRT_RUN_PTPCALL || g_nRunningType == GRT_RUN_PTPWAIT)  // ptp connect fail
            {                
                error("[call_ended]ptp connect fail!!", 0, 0, 0);
                g_nRunningType = GRT_NONE; // PTP ���� ����/   ??????????????????????
				
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
					error("ptp�� call drop�̰���?:%d", g_eDisconnectType, 0, 0);
					/* 						
						SE_CALL_ENDED�̺�Ʈ���� g_eDisconnecType�� DT_NONE�̸�
						PTP���߿� Call drop���� �����. midd_Disconnect�� �Ҹ� ���Ŀ�
						g_eDisconnectType�� DT_CALLDROP���� �����Ͽ� ������
						SE_PTP_RELEASED�̺�Ʈ�� �������� �Ѵ�.					
					*/

                    event.code = SWAP_EVENT_MENU;
                    event.param.menu.event = SWAP_MENU_CALLDROP;
                    result = scl_DoSwapEvent(&event);
				}
				else 
                //else if (g_eDisconnectType != DT_NONE && g_eDisconnectType > DT_NONE) 
                {
                    error("��ư.. PTP ������ ����", 0, 0, 0);

					l_fConnected = FALSE;
					g_eDisconnectType = DT_NONE;
					g_nRunningType = GRT_NONE;                    

                    event.code = SWAP_EVENT_PTPDISCON_OK;
                    result = scl_DoSwapEvent(&event);
                }
            }
            else if (g_eDisconnectType == DT_NONE)  // call drop���� �˰Եȴ�.            
#else
            if (g_eDisconnectType == DT_NONE)  // call drop���� �˰Եȴ�.
#endif  // PTP
            {
                error("call drop!!!", 0, 0, 0);

				// 95C & Dormant  -> �ϴ� ���� -> ����.
				if (l_fPPPOpened && l_fConnected)	// ���� ���¿��� call_ended���� call drop
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
        case SE_PTP_CONNECT_SUCCESS:           // Sock�� ����
            error("[SE_PTP_CONNECT_SUCCESS]ptp ���� ����", 0, 0, 0);
            
            SKY_SET_TO_WAP(FALSE);

            l_fConnected = TRUE;
            
            /* ���� �� ��쿡�� CM���� UI�� SE_PH_IN_USE��� �̺�Ʈ��    */
            /* �������� �ʱ� ������ Annunciator�� ���� �׷� �־�� �Ѵ�. */
            /* 2001.09.20 hjcho                                          */
            if (SKY_SetThenCompareInUseState(IUS_CONNECT))
            {
				/* SKY_IS_IN_USE()���� ����� �ϴ� ���� ���ŵ��� �ʾұ� */
				/* ������ ������ ���Ž��� �ش�.   2001.10.05 hjcho      */
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
            error("[SE_PTP_CONNECT_FAIL]ptp ���� ����", 0, 0, 0);

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
                error("socket open & connect ����~~~~~~~", 0, 0, 0);
                hasResult = FALSE;
                
                SKY_SET_TO_WAP(FALSE);

                l_fConnected = FALSE;        
				l_fConnectFailed = TRUE;

				//SKY_SetGeneralTimer(PPP_TIMEOUT, FALSE);
                midd_Disconnect(SWAP_RR_DISCONNECT);                
            }
            else 
			{
                error("socket open & connect ����~~~~~~~", 0, 0, 0);

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
                WAP->GVM & IS-95C & ppp_opened���� ��ũ��Ʈ�� �־ ����� �� ���� 
                ����� SWAP�� �Ѱ����� �ʴ´�.
            */
            if (g_eDisconnectType == DT_NORESULT) 
            {
                g_eDisconnectType = DT_NONE;                
                break;
            }
            
            /*
                BackToBrowser�ϰ�� IS-95C�̸� ���ϸ� �ݰ� ����
            */
            if (g_eDisconnectType == DT_TOWAP && P_REV_IS_6_OR_GREATER) 
            {                
                l_fConnected = FALSE;
                l_fPPPOpened = FALSE;   // ��¥�� �����ٰ� ��
                g_eDisconnectType = DT_NONE;
                
                error("[SE_NET_DISCONNECTED]���ϸ� ���� -> to WAP!!", 0, 0,0);
                
                event.code = SWAP_EVENT_DISCON_OK;
                result = scl_DoSwapEvent(&event);                
                break;
            }
			
			if (g_eDisconnectType == DT_NONE || l_fConnectFailed)	// midd_Disconnect�Լ��� �Ҹ��� ���� ����/midd_Connect2���� return -1�� ���
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

		case SE_KEY_MANNERS:    // ��Ƽ�ϸ��� ��ȯ/����       
        
            hasResult = FALSE;
			error("�ų� Ű ����~~~~~~~~~~~~~~~", 0, 0, 0);

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
            ���� ������ ���� Ű set -> �ӽ÷� 
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

        case SE_TM_TIMER2:  // SMS ����
            hasResult = FALSE;

			UI_DISABLE_SLEEP();
            EQS_StopTimer(EQS_TIMER_2);
			
			/* traffic channel ���´� */
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
			        g_unTrySMS++; /* �ٽýõ� */
					EQS_StartTimer(EQS_TIMER_2, SMS_TIMEOUT, FALSE);
			        SendSMSMsg(&g_GVMSmsBuffer, g_GVMSmsMoTI);
		        }
		        else/* ���۽��� */
#endif
		        {
                    g_unTrySMS = 0; /* process for out */
                    g_GVMSmsMoType = GSMT_IDLE;
					if (IsCallTypeSMS()) l_fSMSEnded = TRUE;
                    hasResult = TRUE;

                    event.code = SWAP_EVENT_SMS_RESULT;
                    event.param.data = 0; // fail
                    result = scl_DoSwapEvent(&event);
                    error("[TIMER]SMS������ ����!!!!", 0, 0, 0);                    
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
                if (SKY_IS_PHONE_CLOSE())   // ���� �����鼭 ���� ���  <- ���� ��Ȳ
                {
                    EQS_END_TO(EQS_POP_TO_ROOT);
                } 
                else 
                {
                    error("���� WAP���� ��~~~~~~~~~", 0, 0, 0);
                    SKY_SET_TO_GVM(FALSE);
                    EQS_END(); // SKA_MainGVM���� WAP���� ��
                }
            }
            else if (pEvent->dwParam == SE_KEY_END)    // ������
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
				     
                if (SKY_IS_PHONE_CLOSE())   // ���� �����鼭 ���� ���
                {
                    EQS_END_TO(EQS_POP_TO_ROOT);
                }                
                else if (pEvent->EventID == SE_CALL_INCOMING) // ����ġ ���� ��Ȳ�� ���� �� ����
                {
                    EQS_END_TO_EVENT(EQS_POP_TO_ROOT, SE_CALL_INCOMING, EQS_NA, EQS_NA);                
                }
                else // ���� �޼��� ���
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
                �ٿ�ε� �� �޸� ������ ���...
            */
            case SWAP_RC_OUT_OF_MEM:
                error("<<OUT_OF_MEM>>!!!", 0, 0, 0);

                l_dwScriptSize = result->param.ssb.size; // script size
                l_dwLackSize = result->param.ssb.lack; // ������ Flash-SSb 
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
                TCP/IP ����..
            */
            case SWAP_RC_CONNECT:
                error("<<RC_CONNECT!!!>> - GRT:%d", g_nRunningType, 0, 0);
                if (result->param.svr.reasons == 0)
                {
                    g_TypeConnectSvr = GCST_DOWNLOAD;
					g_nRunningType = GRT_DOWNLOAD_START;	// �����ؼ� ����!!!					
                }
                else    // ���� ���� ���� 
                {                    
                    g_TypeConnectSvr = GCST_GAME;					
					if (g_nRunningType == GRT_NONE)
					{
						error("���� RunningType�� NONE��", 0, 0, 0);
						g_nRunningType = GRT_RUN_START;	// �����ؼ� ����!!!
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

				// WAP -> GVM & IS-95C ���� pppopen���̸�...
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
                CALL����
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
                if (SKY_IS_PHONE_CLOSE())   // ���� �����鼭 ���� ��� <- ���� ��Ȳ
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
	if (fDereg)	// key event dereg���״����� �ٽ� reg��.
	{
#define KEY_FIRST_REPEAT 1200  // �ø�.  init:600
#define KEY_NEXT_REPEAT 300

		SKY_RegKeyCB( (int4)KEY_FIRST_REPEAT, (int4)KEY_NEXT_REPEAT, TRUE );
		fDereg = FALSE;
	}
}


/*
    ���� ���ø����� ����. - Control ����
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
        
    /* ���ڶ� �뷮 -> �ǽð� �߰� */
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
        sprintf((char*)pszTmpBuffer, "����");
    }
	else 
	{
		sprintf((char*)pszTmpBuffer, "����");
	}
    EQC_SetStaticText(l_hLackSize, pszTmpBuffer);

    /* ���� �뷮 */ 
    l_hFileSizeTitle = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_SIZE_FILE);
    l_hFileSize = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    sprintf((char*)pszTmpBuffer, 
            (char*)SKY_GET_TOKEN(TKN_GVM_FIXED_FILE_SIZE_FORMAT), 
            GET_KB_SIZE(SKY_GetDataFileSize(SDF_GVM, l_ppGVMFileName[0])));
    EQC_SetStaticText(l_hFileSize, pszTmpBuffer);

    /* ����� �뷮 */
    l_hUsedSizeTitle = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_SIZE_ALLOC);
	l_hUsedSize = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    sprintf((char*)pszTmpBuffer,
            (char*)SKY_GET_TOKEN(TKN_GVM_NO_FIXED_FILE_SIZE_FORMAT),
            GET_KB_SIZE(nUsedSize));
    EQC_SetStaticText(l_hUsedSize, pszTmpBuffer);
    
    /* ���� �뷮 */
    l_hFreeSizeTitle = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_SIZE_FREE);
    l_hFreeSize = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
	sprintf((char*)pszTmpBuffer,
            (char*)SKY_GET_TOKEN(TKN_GVM_NO_FIXED_FILE_SIZE_FORMAT), 
            GET_KB_SIZE(g_dwFreeSpace)); // ���� ������ �ƴϴ��� ������ �þ.
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
            if(!nCtrl)	// ù��° ��!
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
    OutOfMemory ���� �߻��� ���� ����� ���� Applet
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

    case SE_IO_SCROLL_LIST_CHG: /* ���� �뷮 */
        sprintf((char*)pszTmpBuffer, 
                (char*)SKY_GET_TOKEN(TKN_GVM_FIXED_FILE_SIZE_FORMAT), 
                GET_KB_SIZE(SKY_GetDataFileSize(SDF_GVM, l_ppGVMFileName[pEvent->dwParam])));
        EQC_SetStaticText(l_hFileSize, pszTmpBuffer);
		break;

    case SE_SOFTKEY_DELETE: /* ������ ���� ? */
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

    case SE_IO_SCROLL_LIST_SELECT: /* okŰ */ 
    case SE_SOFTKEY_COMPLETE: /* �Ϸ� */
        EQS_END();
        break;

    case SE_IO_MESGBOX_YES: /* ���� OK */
        if (l_MessageReason == GDMT_EXIT_CONFIRM)   /* ���� Ȯ������ �������*/
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
            midd_DeleteScript(l_nSSBIndex); // ���� ��û�� ���..
        }
        else 
        {
            midd_DeleteScript(l_anGVMFileIdx[unIdx]); // ���� ��û�� ���..
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

    case SE_IO_MESGBOX_NO: /* ���� NO */
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

            case GDMT_NO_FILE: /* �������� ���� ��� */
                EQS_END_PARAM(EQS_NA, SE_SYS_TO_IDLE); //EQS_END();
		        break;


            case GDMT_DELETED_ALL: /* �������� �� �������� ��� */
                EQS_SEND_EVENT(SE_SOFTKEY_COMPLETE, EQS_NA, EQS_NA);
                break;

            case GDMT_DELETED:
				
				nLackSize = (signed long)(g_dwFreeSpace - l_dwScriptSize);
				if ( nLackSize > 0 && l_nSSBIndex != -1)	// ���� ������ Ȯ���Ǿ��ٸ�?
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
    case SE_IO_MESGBOX_CLEAR: /* MessageBox���� CLEARŰ ������ ��� */
    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
        {     
            EQS_END_PARAM(EQS_NA, SE_KEY_END);
        }
        break;

    case SE_KEY_END:
    case SE_IO_TO_IDLE: /* MessageBox���� ENDŰ ������ ��� */
        EQS_END_PARAM(EQS_NA, SE_KEY_END);
		break;

    case SE_KEY_PHONE_CLOSE:
        EQS_END_TO(EQS_POP_TO_ROOT);
		break;

    case SE_SYS_TO_IDLE:
		break;

	case SE_CALL_INCOMING:
		// ���⼭ data call�� ���� ���ɼ� ����. ���� PTP��� ȣ�� �����־�� ������.
		if (SKY_IsVoiceCall()) EQS_END_CURRENT_EVENT();
		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*
    ���� �Է� Applet
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
            case GEST_DEFAULT:          // �Ϲ� ���ڿ� �Է�
                EditMode = TEM_KOREAN;
                break;
            case GEST_NUMBER:           // ���� ���ڿ� �Է�                
            case GEST_PASSWORD_NUMBER:  // �н������ ���� ���ڿ� �Է�
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

    case SE_IO_TEDIT_DONE:  // �����Է� �Ϸ�
        EQC_GetTextEditData(EQS_GetNthIOC(0), l_szEditedText);
        EQS_END();
        break;

    case SE_IO_NEDIT_CANCEL:
    case SE_IO_TEDIT_CANCEL:
    case SE_KEY_CLEAR:
        if(pEvent->dwParam != EQC_DELETED) 
        {
            STRNCPY(l_szEditedText, l_szEditInitialString, l_nEditMaxLen);    // ��ҽ� �ʱⰪ setting
            EQS_END();
        }        
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
        error("[KEY_END] <- SKA_TextEdit", 0, 0, 0);        
        EQS_END_PARAM(EQS_NA, SE_KEY_END);
		break;

    case SE_RECV_DATA: // ptp, server�߿� chatting��...
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

    case SE_CALL_INCOMING:  // �ѱ��Է�state���� ����->�ӽ÷� �����ϰ� ��ȭ���Ĵٽ� �簳
        error("[CALL_INCOMING] <- TextEdit!!", 0, 0, 0);
        
		// ���⼭ data call�� ���� ���ɼ� ����. ���� PTP��� ȣ�� �����־�� ������.
		if (SKY_IsVoiceCall())         
		{
			/* ���� ������ ���� -> l_szEditedText */
			if (l_nEditStrType == GEST_NUMBER || l_nEditStrType == GEST_PASSWORD_NUMBER)    // ���� ������
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
    Chatting Text ���� Applet
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

	case SE_KEY_PHONE_CLOSE:	//	���ٲ� ������ �����ϳ� ����?
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

    case SE_CALL_INCOMING:  // ������ �Ǹ� ��� �����ϰ� ������
        error("[CALL_INCOMING] <- EditChatText!!", 0, 0, 0);
        
		// ���⼭ data call�� ���� ���ɼ� ����. ���� PTP��� ȣ�� �����־�� ������.
		if (SKY_IsVoiceCall()) 
		{
			/* ���� ������ ���� -> l_szEditedText */ // -> ������ ����?
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
    Error �޼��� ����ϴ� Applet -> �� �� �պ���
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
    B2: 16��Ʈ Į�� ���� LCD�� 256Į��� Bitmap�� �׸��� ���� �Լ�
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
	char* uid,       // ������ ���ӿ��� ����ϴ� ����� �̸�
	int   cpid,      // CP�� id, ������           0x0000 ~ 0xfffe
	int   gid,       // ������ ������ id          0x0000 ~ 0xffff
	char* dlsIP,     // �ٿ�ε弭���� IP address "game.sinjisoft.com"
	int   dlsPort,   // �ٿ�ε弭���� port       0 ~ 65535
	char* svrIP,     // ���� IP address           
	int   svrPort,   // ���� port
	char* retURL,    // ���������� go back�� ���� URL
	char* Result
*******************************************************************************/

VOID GoToGVM( char* uid, int cpid, int gid, char* dlsIP, int dlsPort, 
              char* svrIP, int svrPort, char* retURL, char* Result )
{
#ifndef FEATURE_WAP
    ui_cmd_type	    *ui_cmd_ptr;
#endif //#ifndef FEATURE_WAP

    error("GoToGVM �Ҹ���~ cpid:%d,gid:%d ----------", cpid, gid, 0);	

    if (g_fDownloadRequestFromSMS)  // SMS���� ���� result�� NULL�̴�.
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

    if (!g_fDownloadRequestFromSMS)  // SMS���� ���� result�� NULL�̴�.
    {
		g_fFromWAP = TRUE;	// SKY_SET_FROM_WAP������

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
	
	/* PtP IrDA�� �ݾ��ֱ� ���� */
    if(!fIrDAPortClosed)
	{
		ChangeIrDAMode ( 0 );
		// Phone to PC IrDA�� off �Ǿ����� ǥ��
		fIrDAPortClosed = TRUE;
        //gfbIrDAPortForPC = FALSE;
	}

	//	�̺κ��� ������ �Ϻν�ũ��Ʈ�� ����� �������� ����. -> sky_conf.c�� �ű�.
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
	
	SKY_WRITE_CONFIG(SC_GVM); // ���������� UserData�����ϱ� ����.
	error("config file�� ����Ϸ�~~~~~~~~~~~~~`", 0, 0, 0);

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

	sCheckScriptInfo();	// ����ִ°� ���� ������?

	/* traffic channel ���´� */
    if(ui_curr_call_is_sms())
    {
        uasms_DTC_disconnect();
    }
    return;
}

/*
    �̹��� ���� �Լ�
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
    �̹��� �������� ���� üũ�ϴ� �Լ�
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
	�̹� �ִ� ��ũ��Ʈ���� �ٿ�ε带 �ߴ��Ҷ� g_nCurSSB�� �������ֱ� ���� �Լ�
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
	�ٿ�ε� �� Ƚ��/�Ⱓ������ �ִ� ��ũ��Ʈ�̸鼭 �� ��ȿ�Ⱓ�� expire�Ǿ��� ���
	���� �ٿ�ε带 �ޱ� ���ؼ� midd_DeleteScript�Լ��� �ҷ��־� ����flag�� �������־
	midd_WriteScriptInfo�� �Ҷ� ������ ��ũ��Ʈ�� �������־�� �Ѵ�.
	(SWAP��⿡�� �ؾ� �� ���̶�� ������ �ǳ� �׷��� �����Ƿ� �ϴ� ���Լ��� ���� ������ ����)
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

			// valid count ����
			if (SKY_GET_GVM_VALID_COUNT(ssbIndex) == 0) 
			{
				valid = FALSE;
				break;
			}

			// valid term ����
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
            //�����δ� down�޾� ma1���� �ٲ� file�� size�̾����
            Size = *mb_MusicBellPreBuffer;
            break;
        case YAMAH_mmf:
            //�����δ� down�޾� ma1���� �ٲ� file�� size�̾����
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
    256 Į��� 16��Ʈ Į���� ��� ���� �ʿ��� �� setting
	��� �÷��϶� ������� ����.. ���߿� ��� display type�� �°� ���ľ� ��..
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
    g_fIn256ColorDisplay = FALSE;   // draw�� GVM���� ���� ����.
    EQS_SetDirectDraw(flag);
	if (flag) SKY_DrawAnnunciators();
#else
#endif  
}

/*
    256 color�϶� Annunciator�� �׷��ִ� �Լ�
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
    GVM ���� �ִ��� ������ üũ 
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
    GVM����� ����flag�� setting�Ǿ� �ִ� ��ũ��Ʈ�� ã�Ƽ� ������ �������ִ� �Լ�
*/
LOCAL VOID sCheckScriptInfo(void) 
{
    STATIC  UINT8   i=0;

    // �����.. ������ϸ� �Ǿ� �ִ� ���� Ǯ���ش�.
    for (i=0; i < SWAP_SSB_SIZE; i++)  // i��° ��ũ��Ʈ�� ���� Flag�� �����Ǿ����� Ȯ��        
    {
		if (scl_ModifyScriptInfo(i, 0)) 
        {
			// Clear script delete flag
			scl_ModifyScriptInfo(i, 1);
            
            // ���� ssb������ ���� ���ϵ� ���������� �ʴٸ� �׳� �����ؾ�????????????????????
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