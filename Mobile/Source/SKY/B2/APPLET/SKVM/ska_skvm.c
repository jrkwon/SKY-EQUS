
/*
** ===========================================================================
**
** Project: 
**     neo-penta 
**
** File:
**     ska_xvm.c
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
** 2001-06-11 aromi      Modified
** 2001-04-17 tortol     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h" 
#include "sky.h"
#include "mdarm_def.h"

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
UCHAR g_pszmsdURL[MSDURL_MAX] = {0};
UCHAR g_pszretURL[MSDURL_MAX] = {0};
UCHAR g_psretUrlFromJava[MSDURL_MAX] = {0};	
char  retUrlFromJava[132];	// must be more than 128 bytes XVM core에서 사용함

UCHAR g_pszSKVMInputData[DATA_MAX] = {0};
INT   g_scvOption = 3;
INT	  g_iSKVMInputDataLen;
INT*  g_pSkvmState	= &xvm_state;
BOOL  g_bsmsdowmload = FALSE;
BOOL  g_bfirstrefresh = TRUE;
BOOL  g_bPlaySkvmInBG = FALSE;
BOOL  g_bEntryDormant = FALSE; 
BOOL  g_bDownLoadMode = FALSE; 
SKVM_COLOR_E g_Eskvmcolor ;
LOCAL BYTE *l_ppszInit[SKVM_COLOR_MAX];
LOCAL ROM SKY_TOKEN SKVMColor[2] = {
    TKN_SKVM_256,   // 256 칼라 사용
    TKN_SKVM_64K    // 65536 칼라 사용 
};

LOCAL BOOL   l_fAutoAnswerRelease = FALSE;
LOCAL UINT8  l_nCount = 0;
extern ALARM_DISP_TYPE_T     AlarmTypeInSKVM;


#ifdef SKVM_IN_UI_TASK
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

extern void xvm_reg_annun(void);
extern void xvm_dereg_annun(void);
#define sSKVM_INPUT_RADIO_XWIDTH  14
enum tag_DATA_SERVICE_E {
    DS_SKVMINPUT_RADIO,
    DS_SKVMINPUT_MAX
};
VOID SKY_API SKA_SKVMInputEnv(SKY_EVENT_T *pEvent)
{
    UINT8 i;
	BYTE *p_BTitle;
    INT8 nSel=0;
    HCONTROL hRadioButton;
    NV_ITEM_U nv_data;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC HCONTROL s_hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      300, 
	                  CL_NONE_CONFIRM_NONE, 
                      SKY_GET_TOKEN(TKN_SKVM_INPUT_ENV), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        l_ppszInit[0] = (BYTE*)SKY_GET_TOKEN(SKVMColor[0]);
        l_ppszInit[1] = (BYTE*)SKY_GET_TOKEN(SKVMColor[1]);

		nSel = SKY_GET_SKVMCOLORENV() ;

        hRadioButton = EQC_RadioButton(EQS_NA, EQS_NA, sSKVM_INPUT_RADIO_XWIDTH, 2, (BYTE **)l_ppszInit, nSel);
		EQC_SET_STYLE(hRadioButton, CS_RADIO_BUTTON_MAXIMIZE);
		if(nSel)
			EQC_SetRadioButton(hRadioButton, 1);
		else
			EQC_SetRadioButton(hRadioButton, 0);

		EQS_SetIOCActive(hRadioButton);

		break;

    case SE_APP_END :   /* The very last event */ 
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
            EQS_End();
        break;
       
    case SE_KEY_CLEAR:
        EQS_END();
        break;

  case SE_IO_RADIO_BUTTON_SELECT:
	    hRadioButton = EQS_GetNthIOC(DS_SKVMINPUT_RADIO);
		nSel = EQC_GetRadioButton(hRadioButton);
		SKY_SET_SKVMCOLORENV(nSel);
		if(!nSel)
			s_hControl = EQC_MessageBox((BYTE *) SKY_GET_TOKEN(TKN_COLOR_ENV_MSG_256), 
						 MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
		else
			s_hControl = EQC_MessageBox((BYTE *) SKY_GET_TOKEN(TKN_COLOR_ENV_MSG_64K),
			             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
		break;


    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
       
	case SE_IO_MESGBOX_END:
		EQS_End();
		break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSKA_SKVMExecuting(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    /* ------------------------------------------------------------------------*/
    /* APP */
    case SE_APP_START : /* The very first event */
		break;
	case SE_APP_END :   /* The very last event */ 
		break;
    case SE_APP_SUSPEND: /* child applet is started */
        break;
    case SE_APP_RESUME: /* child applet is ended */
        break;
	case SE_SKVM_KEYEVENT:
		break;
	case SE_SKVM_INPUT_DATA: /* 현재 실행 SKVM에게 data을 입력해줌 */
		break;
    case SE_SKVM_STOPPED:
		break;
    case SE_SKVM_STOPPED_TO_GVM:
        break;
    case SE_SKVM_SMS_SEND:
        break;
    case SE_SKVM_VOICECALL_CONNECT:
        break;
	case SE_CALL_ORIGINATED:
		break;
    case SE_CALL_CONNECT:
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
**     SKA_MainSKVM()
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
void SKY_API SKA_MainSKVM(SKY_EVENT_T *pEvent)
{
   static char *temperrmsg = NULL;
   STATIC HCONTROL hMsgControlend,hControl;
   STATIC BOOL	ToWap = FALSE;
   STATIC BOOL	ToIdle = FALSE;
   STATIC BOOL	ToGvm = FALSE;
   STATIC BOOL	ToCall = FALSE;
   STATIC BOOL  ClientCallEnd = FALSE;
   int result = DSS_ERROR;

   switch(pEvent->EventID)
    {
    case SE_APP_START :  /*The very first event */
        EQS_GetDC(TRUE);
        SKY_DeregKeyCB();        
		temperrmsg = NULL;
		ToWap = FALSE;
		ToIdle = FALSE;
		ToGvm = FALSE;
		ToCall = FALSE;
	    ClientCallEnd = FALSE;
		SKVM_SET_FIRST_REFRESH(TRUE);
		SKY_SET_IN_SKVM(TRUE);
		SKVM_SET_PLAY_BG(FALSE);
		SKVM_SET_DORMANT(FALSE);

		if (!SKY_IsFileOrDirExist((const BYTE*)"xce"))
			SKY_MakeDir((const BYTE*)"xce");
		if (!SKY_IsFileOrDirExist((const BYTE*)"xce/rs"))
			SKY_MakeDir((const BYTE*)"xce/rs");

        // for shared RAM 
        if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            //????
            //gfbIrDAPortForPC = FALSE;         
		}

		if (GET_SKVM_STATE == XVMSTATE_FROZEN)
		{
			SKVMResume();
		}
		else
		{
			SKVM_SET_SMS_DOWNLOAD(FALSE);
			SKVM_SET_DOWNLOAD_MODE(FALSE);
			XSIG_SKT_CLR(DS_PPP_XSIG|UPDATE_ANNU_XSIG|REFRESH_LCD_XSIG);
			switch(pEvent->dwParam)
			{
			case INIT_SKVM_MSDURL:
			    // WAP에서 온 경우 다시 Memory Pool Base를 재설정해야 한다.
				// 왜냐하면 WAP에서 Memory Pool로 Shared Memory를 사용했기 때문.. 
//				if (SKY_IS_TO_SKVM())
//			        SKY_ReinitMemory();
				// DOWNLOAD MODE SET => TRUE
				SKVM_SET_DOWNLOAD_MODE(TRUE);
				SKVMStartBymsdURL((char*)g_pszmsdURL,(char *)g_pszretURL,g_scvOption);
				break;
			case INIT_SKVM_INPUTDATA:
				SKVMInputData(g_pszSKVMInputData,g_iSKVMInputDataLen);
				break;
			default:
				SKVMStart();
				break;
			}
		}

#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
		SKY_DisablePacemakerCallback();
		xvm_reg_annun();
#endif
	    break;
    case SE_APP_END :   /* The very last event */ 
		if ((GET_SKVM_STATE != XVMSTATE_FROZEN) && (GET_SKVM_STATE != XVMSTATE_DEAD))
		{
			SKVMStop();
		}
#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
		xvm_dereg_annun();
		SKY_SetFromSKVMPaceMakerTimer();
#endif
		if(!SKY_GET_SKVMCOLORENV())
		{
			if (!xvm_errnum)
				SKY_256DisplayEnd();
		}
		SKY_SetThenCompareInUseState(SKY_GetInUseState());
        EQS_SetDirectDraw(FALSE);
		SKY_SET_IN_SKVM(FALSE);
		EQS_ReleaseDC();
		break;
	case SE_SKVM_STARTED:
		break;
	case SE_SKVM_INPUT_DATA: /* 현재 실행 SKVM에게 data을 입력해줌 */
		if (GET_SKVM_STATE) // XVM이 실행중인  경우
            SKVMInputData(g_pszSKVMInputData,g_iSKVMInputDataLen);
		else EQS_DEF_EVENT_PROC(pEvent);
		break;
   	case SE_CALL_INCOMING:
        if (!SKY_GET_DATACALL()) //only voice call일 경우만!!
		{	
			SKVMPause();
        }
        EQS_DEF_EVENT_PROC(pEvent);
		break;
	// 2001.10.24 For Dormant
#ifdef SUPPORT_DORMANT_IN_SKVM
	case SE_CALL_ENDED:
		// 프로토콜 revision이 6이상이면서 다운로드 모드가 아닐 때  
		// call end status를 봐서 
		if (P_REV_IS_6_OR_GREATER && !SKVM_IS_DOWNLOAD_MODE())
		{  
            switch (SKY_GetCallEndStatus())
			{
				case CM_CALL_END_REL_NORMAL:
				case CM_CALL_END_CLIENT_END:
					// (자바 어플리케이션)client의도로 호를 끊었을때는 
					// SE_CALL_ENDED event가 들어 올땐 SKVM_SET_DORMAT(TRUE) 세팅 안한다.
					break;
				default:
					// (자바 어플리케이션)client의도와 상관 없이 호가 끊겼을 때는  
					// SE_CALL_ENDED event가 들어 올땐 SKVM_SET_DORMAT(TRUE) 세팅한다. 
					SKVM_SET_DORMANT(TRUE);
					break;
			}
		}
		else 
		{  
			MSG_ERROR("Proctocol revision 6이하임",0,0,0);
			switch (SKY_GetCallEndStatus())
			{
			case CM_CALL_END_REL_NORMAL:
			case CM_CALL_END_CLIENT_END:
				ClientCallEnd = TRUE;
				break;
			default:
				break;
			}
			result = PPPCLOSE_VM();
			if (result == DSS_SUCCESS)
			{
   				CLOSE_NETLIB();
				if (!ClientCallEnd)
				{
					// (자바 어플리케이션)client의도와 상관 없이 호가 끊겼을 때는  
					// SE_CALL_ENDED event가 들어오면 XVM을 종료한다. 
					if (SKVM_IS_SMS_DOWNLOAD())
					{
						SKVM_SET_SMS_DOWNLOAD(FALSE);
					}
					if ((GET_SKVM_STATE != XVMSTATE_FROZEN) && (GET_SKVM_STATE != XVMSTATE_DEAD))
					{
						SKVMStop();
					}
					else EQS_END_TO(EQS_POP_TO_ROOT);
					break;
				}
			}
			else ToIdle = TRUE;
		}
		break;
#endif //#ifdef SUPPORT_DORMANT_IN_SKVM
	case SE_APP_SUSPEND: /* child applet is started */
		SKVMPause();
        EQS_SetDirectDraw(FALSE);
		if(!SKY_GET_SKVMCOLORENV())
			SKY_256DisplayEnd();
#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
		xvm_dereg_annun();
		SKY_SetFromSKVMPaceMakerTimer();
#endif
		EQS_ReleaseDC();
        break;
    case SE_APP_RESUME: /* child applet is ended */
        EQS_GetDC(TRUE);
        SKY_DeregKeyCB();        
		SKVM_SET_FIRST_REFRESH(TRUE);
        EQS_SetDirectDraw(TRUE);
		SKVMResume();

#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM 
		SKY_DisablePacemakerCallback();
		xvm_reg_annun();
#endif  
		break;
    case SE_SKVM_STOPPED_BY_ERROR:
		// ERROR발생하고 종료되었을때 메세지 박스
		if (xvm_errnum){
			if(!SKY_GET_SKVMCOLORENV())
				SKY_256DisplayEnd();
			temperrmsg = (char*)(xvm_errmsgs + (xvm_errnum << 4));
			if (temperrmsg)
				hMsgControlend = EQC_MessageBox((BYTE*)temperrmsg, 2, MBT_NONE, MBT_FAIL);
		}
		break;
    case SE_SKVM_STOPPED:
		// 호를 끊지 않고 IDLE로 빠지는 경우를 대비함.
		result = PPPCLOSE_VM();
		if (result == DSS_SUCCESS)
		{
   			CLOSE_NETLIB();
			if (SKVM_IS_SMS_DOWNLOAD())
			{
				SKVM_SET_SMS_DOWNLOAD(FALSE);
			}
			if ((GET_SKVM_STATE != XVMSTATE_FROZEN) && (GET_SKVM_STATE != XVMSTATE_DEAD))
			{
				SKVMStop();
			}
			else EQS_END_TO(EQS_POP_TO_ROOT);
		}
		else ToIdle = TRUE;
		break;
    case SE_SKVM_STOPPED_TO_GVM:
		// 호를 끊지 않고 IDLE로 빠지는 경우를 대비함.
		result = PPPCLOSE_VM();
		if (result == DSS_SUCCESS)
		{
   			CLOSE_NETLIB();
	        SKVM_gvm_start(gvm_app_idx);
			gvm_app_idx = -1;
		}
		else ToGvm = TRUE;
        break;
    case SE_SKVM_SMS_SEND:
        xvm_state = XVMSTATE_SUSPEND;
        SKVMPause();
        EQS_SetDirectDraw(FALSE);
        EQS_START_CHILD_EVENT(SKA_SMSSendMsg, SE_APP_START, EQS_NA,nTIValue);
        break;
    case SE_CALL_COMMAND_ERROR: //origination시 cm_call_cmd_error가 날 수있다.
        break;
	case SE_SKVM_VOICECALL_CONNECT:
        xvm_state = XVMSTATE_SUSPEND;
        SKVMPause();
		result = PPPCLOSE_VM();
		if (result == DSS_SUCCESS)
		{
   			CLOSE_NETLIB();
			EQS_START_SIBLING(SKA_StartOrigination, FALSE);	
		}
		else ToCall = TRUE;
        break;
    case SE_SKVM_STOPPED_TO_WAP:
		// XVM core에서 WAP으로 되돌아 갈때 ppp를 끊지 않고 종료됨.
		// 따라서 network에 따라 추후 처리가 필요함.
		if ( P_REV_IS_6_OR_GREATER )
		{
		    // 2.5G망에선 wap으로 돌아갈때 ppp를 유지한 상태에서 SO만 바꿔 줌 
			SetMOinSO33(0);   
	        SKVM_ntop_start();
		}
		else 
		{
			// 2G망에선 wap으로 돌아갈때 ppp를 close시켜햐 함
			result = PPPCLOSE_VM();
			if (result == DSS_SUCCESS)
			{
       			CLOSE_NETLIB();
				SKVM_ntop_start();
			}
			else ToWap = TRUE;
		}
		break;
	case SE_NET_DISCONNECTED: 
		// ToIdle은 제일 마지막에 check 한다.
		// 왜냐면 ToIdle 은 SE_CALL_ENDED에서 setting됨으로
		// ToWap이나 ToGvm 이나 ToCall등이 TRUE로 setting되어 있을 수가 있다.
		if (ToWap)
		{	ToWap = FALSE;
   			CLOSE_NETLIB();
	        SKVM_ntop_start();
		}
		else if (ToGvm)
		{
	        ToGvm = FALSE;
	   		CLOSE_NETLIB();
			SKVM_gvm_start(gvm_app_idx);
			gvm_app_idx = -1;
		}
		else if (ToCall)
		{
			ToCall = FALSE;
			CLOSE_NETLIB();
			EQS_START_SIBLING(SKA_StartOrigination, FALSE);	
		}
		else if (ToIdle)
		{
			ToIdle = FALSE;
			if (!ClientCallEnd){
	   			CLOSE_NETLIB();
				if (SKVM_IS_SMS_DOWNLOAD())
				{
					SKVM_SET_SMS_DOWNLOAD(FALSE);
				}
				if ((GET_SKVM_STATE != XVMSTATE_FROZEN) && (GET_SKVM_STATE != XVMSTATE_DEAD))
				{
					SKVMStop();
				}
				else EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else ClientCallEnd = FALSE;
		}
		break;
	case SE_IO_MESGBOX_END:
		// ERROR발생하고 종료되었을때 메세지 박스
		if(pEvent->wParam == hMsgControlend)
		{
			result = PPPCLOSE_VM();
			if (result == DSS_SUCCESS)
			{
   				CLOSE_NETLIB();
				if (SKVM_IS_SMS_DOWNLOAD())
				{
					SKVM_SET_SMS_DOWNLOAD(FALSE);
				}
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else 
				ToIdle = TRUE;
		}
		break;
	case SE_SKVM_PLAY_ALARM:
		if (!SKY_GetActiveAlarmFlag())
			EQS_START_CHILD_EVENT( sSKA_PlayAlarm, SE_APP_START, AlarmTypeInSKVM.Type, AlarmTypeInSKVM.nAddr );
		break;
    case SE_PH_SERVICE_STATE:	
	// Annunciator 부분을 직접 Update하기 위해서 .....
        if (SKY_GET_PHONE_INFO_SRV_STATE() == CM_SRV_STATE_NO_SRV)
        {
            // Do no Service Alarming ..
            //   was moved to SKY_SetThenCompareRSSI()
            // Qualcomm original:: ui.systime_valid = ui.digital = FALSE; // backward compatibility
        }
        else
        {
            if (SKY_GET_PHONE_INFO_SRV_STATE() == CM_SRV_STATE_DIGITAL)
            {
                // entering DIGITAL service
                // Qualcomm original:: ui.systime_valid = ui.digital = TRUE;
                SKY_SET_VALID_SYSTEM_TIME(TRUE);
                SKY_SET_DIGITAL(TRUE);
            } 
            else 
            {
                // entering ANALOG service
                // Qualcomm original:: ui.systime_valid = ui.digital = FALSE;
                SKY_SET_VALID_SYSTEM_TIME(FALSE);
                SKY_SET_DIGITAL(FALSE);
            }
        }
	case SE_CALL_ORIGINATED:
    case SE_CALL_CONNECT:
	case SE_PH_INUSE_STATE:
	case SE_SYS_PACEMAKER:
	case SE_PH_RSSI:
		break;
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainSKVMBG(SKY_EVENT_T *pEvent)
{
   static char *temperrmsg = NULL;
   STATIC HCONTROL hMsgControlend;
   STATIC HCONTROL hCtrlNoSrvc;
   STATIC BOOL     fDefence,  fPopupMessageBox;
   STATIC SKY_EVENT_E BackupEvent = -1;
   HCONTROL hControl;
   
   switch(pEvent->EventID)
    {
    case SE_APP_START :  /*The very first event */
		temperrmsg = NULL;
		fPopupMessageBox = FALSE;
		fDefence = FALSE;
		SKVM_SET_FIRST_REFRESH(TRUE);
		SKY_SET_IN_SKVM(TRUE);
        // for shared RAM 
        if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            //????
            //gfbIrDAPortForPC = FALSE;         
		}

		if (GET_SKVM_STATE == XVMSTATE_FROZEN)
		{
			SKVMResume();
		}
		else
		{
			SKVM_SET_SMS_DOWNLOAD(FALSE);
			SKVM_SET_PLAY_BG(TRUE);
			SKVMStartBG();
		}

#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
		SKY_DisablePacemakerCallback();
		xvm_reg_annun();
#endif
	    break;
    case SE_APP_END :   /* The very last event */ 
		if ((GET_SKVM_STATE != XVMSTATE_FROZEN) && (GET_SKVM_STATE != XVMSTATE_DEAD))
		{
			SKVMStop();
		}
		if(!SKY_GET_SKVMCOLORENV())
		{
			if (!xvm_errnum)
				SKY_256DisplayEnd();
		}

#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
		xvm_dereg_annun();
		SKY_SetFromSKVMPaceMakerTimer();
#endif
        
		SKY_SetThenCompareInUseState(SKY_GetInUseState());
        EQS_SetDirectDraw(FALSE);
		SKY_SET_IN_SKVM(FALSE);
		break;
	case SE_SKVM_STARTED:
		break;
	case SE_SKVM_INPUT_DATA: /* 현재 실행 SKVM에게 data을 입력해줌 */
		if (GET_SKVM_STATE) // XVM이 실행중인  경우
            SKVMInputData(g_pszSKVMInputData,g_iSKVMInputDataLen);
		else EQS_DEF_EVENT_PROC(pEvent);
		break;

   	case SE_CALL_INCOMING:
        if(!SKY_GET_DATACALL()) //only voice call일 경우만!!
		{	
			SKVMPause();
		}
        EQS_DEF_EVENT_PROC(pEvent);
		break;
	case SE_APP_SUSPEND: /* child applet is started */
		SKVMPause();
        EQS_SetDirectDraw(FALSE);
		if(!SKY_GET_SKVMCOLORENV())
			SKY_256DisplayEnd();
#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
		xvm_dereg_annun();
		SKY_SetFromSKVMPaceMakerTimer();
#endif
        break;
    case SE_APP_RESUME: /* child applet is ended */
		SKVM_SET_FIRST_REFRESH(TRUE);
        EQS_SetDirectDraw(TRUE);
		SKVMResume();

#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM 
		SKY_DisablePacemakerCallback();
		xvm_reg_annun();
#endif  
		break;
    case SE_SKVM_STOPPED_BY_ERROR:
		if (xvm_errnum){
			if(!SKY_GET_SKVMCOLORENV())
				SKY_256DisplayEnd();
			temperrmsg = (char*)(xvm_errmsgs + (xvm_errnum << 4));
			if (temperrmsg)
				hMsgControlend = EQC_MessageBox((BYTE*)temperrmsg, 2, MBT_NONE, MBT_FAIL);
		}
		break;
    case SE_SKVM_STOPPED:
		BackupEvent = -1;
		switch (BackupEvent)
		{
	    case SE_KEY_F2:
	    case SE_SOFTKEY_WAP:
			l_fAutoAnswerRelease = FALSE;
	        EQS_START_SIBLING( SKA_MainWAP ,FALSE);
			break;
		case SE_KEY_F2_LONG:		
			l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
			EQS_START_SIBLING( SKA_ChooseMenu, FALSE); 
#endif  
			break;
		default: 		
			if (SKVM_IS_SMS_DOWNLOAD())
			{
				SKVM_SET_SMS_DOWNLOAD(FALSE);
			}
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;
		}
		break;
    case SE_SKVM_STOPPED_TO_GVM:
        SKVM_gvm_start(gvm_app_idx);
		gvm_app_idx = -1;
        break;
    case SE_SKVM_SMS_SEND:
        xvm_state = XVMSTATE_SUSPEND;
        SKVMPause();
        EQS_SetDirectDraw(FALSE);
        EQS_START_CHILD_EVENT(SKA_SMSSendMsg, SE_APP_START, EQS_NA,nTIValue);
        break;
    case SE_SKVM_VOICECALL_CONNECT:
        xvm_state = XVMSTATE_SUSPEND;
        SKVMPause();
        EQS_SetDirectDraw(FALSE);
        //EQS_START_CHILD(SKA_StartOrigination);
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
    case SE_SKVM_STOPPED_TO_WAP:
        SKVM_ntop_start();
		break;
	case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hMsgControlend)
		{
			if (SKVM_IS_SMS_DOWNLOAD())
			{
				SKVM_SET_SMS_DOWNLOAD(FALSE);
			}
			EQS_END_TO(EQS_POP_TO_ROOT);
		}
		else 
		{
			fPopupMessageBox = FALSE;
		    fDefence = FALSE;
			if (pEvent->wParam == hCtrlNoSrvc)
				EQS_END();
		}
		break;
    case SE_PH_SERVICE_STATE:
	// Annunciator 부분을 직접 Update하기 위해서 .....
        if (SKY_GET_PHONE_INFO_SRV_STATE() == CM_SRV_STATE_NO_SRV)
        {
            // Do no Service Alarming ..
            //   was moved to SKY_SetThenCompareRSSI()
            // Qualcomm original:: ui.systime_valid = ui.digital = FALSE; // backward compatibility
        }
        else
        {
            if (SKY_GET_PHONE_INFO_SRV_STATE() == CM_SRV_STATE_DIGITAL)
            {
                // entering DIGITAL service
                // Qualcomm original:: ui.systime_valid = ui.digital = TRUE;
                SKY_SET_VALID_SYSTEM_TIME(TRUE);
                SKY_SET_DIGITAL(TRUE);
            } 
            else 
            {
                // entering ANALOG service
                // Qualcomm original:: ui.systime_valid = ui.digital = FALSE;
                SKY_SET_VALID_SYSTEM_TIME(FALSE);
                SKY_SET_DIGITAL(FALSE);
            }
        }
	case SE_CALL_ORIGINATED:
    case SE_CALL_CONNECT:
	case SE_PH_INUSE_STATE:
	case SE_SYS_PACEMAKER:
	case SE_PH_RSSI:
		break;
    case SE_TM_TIMER1:
        break;
    case SE_KEY_PHONE_OPEN:
        break;
    case SE_DTR_DISPLAY_CHANGE_F:   // 착신모드 화면 변경
        break;
    case SE_KEY_EAR_LONG_CLICK:
	case SE_KEY_SEND:
		if( TransferLastDialInfo2CurrentDialInfo() && 
            (SKY_IS_CALL_STACK_LOCK() == FALSE))
		{
            EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        }
		break;

    case SE_KEY_POUND:
    case SE_KEY_STAR:
    case SE_KEY_0:
    case SE_KEY_1:
    case SE_KEY_2:
    case SE_KEY_3:
    case SE_KEY_4:
    case SE_KEY_5:
    case SE_KEY_6:
    case SE_KEY_7:
    case SE_KEY_8:
    case SE_KEY_9:
        l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD_CURRENT_EVENT(SKA_Dialing);
        break;       
    case SE_KEY_POUND_LONG:
		SKY_SET_JAVAASBG(FALSE);
        EQS_End();
        break;
    
    case SE_KEY_F2:
    case SE_SOFTKEY_WAP:
		BackupEvent = pEvent->EventID;
		SKVMStopBG();
        break;
	case SE_KEY_F2_LONG:
		BackupEvent = pEvent->EventID;
		SKVMStopBG();
		break;
    case SE_KEY_F1:
    case SE_SOFTKEY_SMS:
       l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
		EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_KEY, EQS_NA);
#else // for test
        EQS_START_CHILD_CURRENT_EVENT(SKA_SMSMenu);
#endif
        break;
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_INFORM:
#endif // #ifdef FEATURE_SKTT_STI_LOCATION
    case SE_SMS_MESSAGE:
        l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
		EQS_START_CHILD_CURRENT_EVENT(SKA_MainSMS);
#endif
        break;
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_TIMEOUT:
		l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
		CheckCurrentTime();
#endif //
		break;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
    case SE_KEY_CENTER:
    case SE_SOFTKEY_MENU:
       l_fAutoAnswerRelease = FALSE;
        //EQS_START_CHILD(SKA_MainMenuTop);  //redstar
#ifdef FEATURE_CALL_STACK_SEPERATED
        EQS_START_CHILD(SKA_MainMenuCamera);
#else
        EQS_START_CHILD(SKA_MainMenuCode);          //white  
#endif //FEATURE_CALL_STACK_SEPERATED
        break;

    case SE_KEY_UP:
        l_fAutoAnswerRelease = FALSE;
        //EQS_START_CHILD(SKA_MainCallList); 
#ifdef FEATURE_CALL_STACK_SEPERATED
        SetReceivedCallStack(FALSE);
#endif//FEATURE_CALL_STACK_SEPERATED        
        EQS_START_CHILD(SKA_MainStackCode);
		break;
        
    case SE_KEY_DOWN:
        l_fAutoAnswerRelease = FALSE;
#ifdef FEATURE_CALL_STACK_SEPERATED
        SetReceivedCallStack(TRUE);
        EQS_START_CHILD(SKA_MainStackCode);
#else
        EQS_START_CHILD(SKA_MainMenuCamera);
#endif//FEATURE_CALL_STACK_SEPERATED        
        break;

	case SE_KEY_RIGHT:
        l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD(SKA_MainPbookCode); //redstar
        break;

    case SE_KEY_LEFT:
        l_fAutoAnswerRelease = FALSE;
#ifdef FEATURE_CALL_STACK_SEPERATED
        EQS_START_CHILD(SKA_MainMenuCode);          //white
#else
        EQS_START_CHILD(SKA_MainSchedule);
#endif // FEATURE_CALL_STACK_SEPERATED
        break;
//#endif// youandi 00.01.10 lcd contrast test
    case SE_DM_IN_PROCESSING:
        EQS_START_CHILD(SKA_IN_DataManager);
        break;

    case SE_DM_UPDATE_IDLE://white 2001.06.12 for DataManager Schedule
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_DM_MO_CMD_F:        // 2001.02.09 Send Mo in DataManager.
		l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD_EVENT(SKA_SMSSendMsg, SE_APP_START, EQS_NA, TI_DEFAULT);
        break;

    case SE_KEY_RECORD_LONG:
        l_fAutoAnswerRelease = FALSE;
        //white  
        if(SKY_IS_RECORD_PLAY_LOCK())
            EQS_START_CHILD_CURRENT_EVENT(SKA_MainRecCode );
        else
            EQS_START_CHILD_CURRENT_EVENT(SKA_MainVMemo);
        //white 
        break;

    case SE_KEY_RECORD:
    case SE_KEY_PLAY:
        if(SKY_IS_PHONE_OPEN())
        {
            l_fAutoAnswerRelease = FALSE;
           /* 팝업메뉴나 메시지박스가 뜬 상태에서 녹음키를 누르는 경우 */
           /* 녹음/재생화면으로 넘어가지 않는 경우 방지                */
           if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
           {
               /* Sub LCD에 글자가 나올 수 있도록 */
               if(SKY_IS_AUTO_ANSWER() == FALSE)
               {
                 l_nCount = 0;
               }

               SKY_SubLCDEndText(LN_UPPER);
               SKY_SubLCDEndText(LN_LOWER);               
               EQS_DeleteControl(hControl);
           }

            //white 
            if(SKY_IS_RECORD_PLAY_LOCK())// && SKY_IS_PHONE_OPEN())
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainRecCode );
            else
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainVMemo);
            //white 
        }
        break;

    case SE_KEY_RELEASE_AUTO_ANSWER:

        if(SKY_IS_AUTO_ANSWER() && SKY_IS_PHONE_OPEN())
        {
            UpdateAutoAnsDispInDebugScreen(SKY_GET_AUTO_ANSWER_GREETING());  //자동응답설정상태를 바꾸는 부분    
        }
        break;

    case SE_KEY_AUTO_ANSWER_LONG:
        if(fDefence == FALSE)
        {
          if(SKY_IS_AUTO_ANSWER() == FALSE)
          {
            if(SKY_IS_ETIQUETTE())
			{
			  if(!SKY_IS_HANDS_FREE()) SKY_MotorTime(800);

              SKY_SET_AUTO_ANSWER(TRUE);
              l_fAutoAnswerRelease = FALSE;
              EQS_END();
			}
			else
			{		
  	          SKY_SET_AUTO_ANSWER(TRUE);
              l_fAutoAnswerRelease = FALSE;                
#ifndef WIN32
              if(SKY_GET_AUTO_ANSWER_BELL_TIME() != AAB_VIB_ONCE)
              {                
                SKY_PlaySound(UI_MSG_SND, SND_AUDIO_APP_START);
              }
              else
#endif //WIN32
              {
                if(!SKY_IS_ETIQUETTE() && !SKY_IS_HANDS_FREE()) SKY_MotorTime(800);
              }

              EQS_END();
			}		
          }
          else 
          {
            if(SKY_IS_ETIQUETTE() == FALSE)
			{
			  //SKY_PlaySound(UI_ALERT_SND, SND_ANSWER);
              //SKY_PlaySoundEx( UI_MSG_SND, SND_PIP_TONE, 5, 1);
#ifndef WIN32
              SKY_PlaySound(UI_MSG_SND, SND_AUDIO_APP_END);
#endif
			}

    		SKY_SET_AUTO_ANSWER(FALSE);
            l_fAutoAnswerRelease = TRUE;
			EQS_END();			
          }
        }
        break;

    case SE_SYS_TO_IDLE:
    case SE_KEY_CLEAR:
        // Should be ignored.
        break;

    case SE_KEY_END:
#ifdef FEATURE_UI_DORMANT
        if(SKY_IS_DORMANT())
        {
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
            SetUIPktRelease();
        }
#endif
        break;
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

#else
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
LOCAL void SKY_API sSKA_SKVMExecuting(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    /* ------------------------------------------------------------------------
    /* APP */
    case SE_APP_START : /* The very first event */
		break;
	case SE_APP_END :   /* The very last event */ 
		break;
    case SE_APP_SUSPEND: /* child applet is started */
		send_vascmd(SKVM_SUSPEND);
        break;
    case SE_APP_RESUME: /* child applet is ended */
		send_vascmd(SKVM_RESUME);
        break;
	case SE_SKVM_KEYEVENT:
		send_vascmd(SKVM_KEYEVENT);
		break;
	case SE_SKVM_INPUT_DATA: /* 현재 실행 SKVM에게 data을 입력해줌 */
		break;
    case SE_SKVM_STOPPED:
		if (GET_SKVM_STATE){
			send_vascmd(SKVM_STOP);
		}
		EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
		break;
    case SE_SKVM_STOPPED_TO_GVM:
        break;
    case SE_SKVM_SMS_SEND:
        break;
    case SE_SKVM_VOICECALL_CONNECT:
        break;
	case SE_CALL_ORIGINATED:
		break;
    case SE_CALL_CONNECT:
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
**     SKA_MainSKVM()
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

void SKY_API SKA_MainSKVM(SKY_EVENT_T *pEvent)
{
   switch(pEvent->EventID)
    {
    case SE_APP_START :  /*The very first event */
	    if (pEvent->dwParam == INIT_SKVM_MSDURL)
		    send_vascmd(SKVM_START_BYMSDURL);
	    else send_vascmd(SKVM_START);
		//{
		//	extern md_wopenc(const char *pathname);
		//	md_wopenc("ABC/1/1.jar");
		//	md_wopenc("ABC/1/2/1.jar");
 		//}
	    break;
    case SE_APP_END :   /* The very last event */ 
		break;
	case SE_APP_SUSPEND: /* child applet is started */
        break;
    case SE_APP_RESUME: /* child applet is ended */
        break;
	case SE_SKVM_STARTED:
     	EQS_START_SIBLING(sSKA_SKVMExecuting,TRUE);
		break;
	case SE_SKVM_INPUT_DATA: /* 현재 실행 SKVM에게 data을 입력해줌 */
		if (GET_SKVM_STATE) // XVM이 실행중인  경우
            send_vascmd(SKVM_INPUT_DATA);
		else EQS_DEF_EVENT_PROC(pEvent);
		break;
	case SE_CALL_ORIGINATED:
		break;
    case SE_CALL_CONNECT:
        break;
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif
