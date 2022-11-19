/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_evnt.c
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
** 2000-10-30 jrkwon     Created.
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

//extern boolean ui_is_in_conversation(void); // in ska_call.c
//extern IN_USE_STATE_E SKY_GetInUseState(void);
//extern BOOL SKY_SetThenCompareInUseState(IN_USE_STATE_E InUseState);

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
LOCAL void SKY_API sDispatcher(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent);

void SKY_API (*g_pfnEQS_Dispatcher)(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent)
    = sDispatcher;

BOOL g_fSubBackLightOn;
BOOL g_fSubBackLightOff;

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
LOCAL void SKY_API sDispatcher(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent);

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

/*
typedef void (*EQS_Applet)(EQS_EVENT* pInEvent); 
--> 
typedef void (EQS_APPLET)(void* pEvent);

use EQS_APPLET *pfnXXXX
*/

LOCAL void SKY_API sDispatcher(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent)
{
    SKY_EVENT_T SkyEvent;

    /* if current event is an EQS native event */
    if(SKY_IS_EQS_NATIVE_EVENT(pEvent->EventID))
    {
        /* put EQS event into SKY one */
        SkyEvent.EventID = (SKY_EVENT_E)pEvent->EventID;
        SkyEvent.wParam  = pEvent->wParam;
        SkyEvent.dwParam = pEvent->dwParam;
    }
    else
    {
        /* 
        ** it means current event is a SKY UI event. 
        ** the EQS engine bypasses the event to SKY UI.
        */
        SkyEvent.EventID = (SKY_EVENT_E)pEvent->dwExtEventID; /* SKY event bypassed by EQS */
        SkyEvent.wParam  = pEvent->wParam;
        SkyEvent.dwParam = pEvent->dwParam;
    }
    /* run current applet with the SKY event */
    pfnApplet(&SkyEvent);
}

#define sMAX_SEC_SYS_MSGBOX                  5
#define sIS_ANNUNCIATORS_WINDOW(pszStr)      ((EQS_GetWindowAttr() & WA_SHOW_HEADER) && ((pszStr) == NULL))


void SKY_API SKY_DefEventProc(SKY_EVENT_T* pInEvent)
{
#ifdef FEATURE_SKTT_EQUS_LOCATION
	LOCATION_INFO_T*		pLocation;
#endif // #ifdef FEATURE_SKTT_EQUS_LOCATION

    extern VOID SKY_API SKA_ShutDown(SKY_EVENT_T* pEvent);
    extern VOID SKY_API SKA_ShutDownBadBattery(SKY_EVENT_T* pEvent);
	extern VOID SKY_API SKA_Keypad_Test(SKY_EVENT_T* pEvent);
    CONST BYTE *pszStr = EQS_GetCurrentHeaderStr();
    
//#ifdef EXTENDED_EVENT_REGIME // NOTICE !!! 2000.12.10 girin EQE_BYPASS processing
    // Just do it ( or anything you can ... )
//#endif

    switch (pInEvent->EventID)
    {
    case SE_APP_RESUME:
#ifdef  FEATURE_SKT_EIF     // heroine 01/04/10
        if( EQS_GET_CHILD() == SKA_MainEif )
        {
            EQS_END();
        }
#endif  // FEATURE_SKT_EIF     // heroine 01/04/10
        break;
    case SE_IO_FREE_MEMORY:
		SKY_Free((VOID*)pInEvent->dwParam);
		break;
    case SE_IO_SCROLL_LIST_DELETE_LIST:
		DeleteList((UINT8) pInEvent->dwParam );
		break;
	case SE_KEYPAD_TEST:
		if(SKY_IS_KEYPAD_TEST())
			EQS_START_SIBLING(SKA_Keypad_Test, TRUE);
		break;

    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:
        EQS_END();
        break;

    case SE_KEY_PHONE_CLOSE:
        //if(SKY_IsInCall())
        //{
        //    /* Call의 sDefaultHandleEvent에서 처리한다. */
        //    break;
        //}
        //else
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;

    case SE_KEY_PHONE_OPEN:
        break;

    case SE_KEY_END:
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_KEY_POWER_OFF:      /* power off */                          
        EQS_START_SIBLING(SKA_ShutDown, FALSE);
        break;

    case SE_SYS_BAD_BATT:
        EQS_START_SIBLING(SKA_ShutDownBadBattery, FALSE);
        break;

    case SE_SYS_LOW_BATT:
        if(SKY_IS_LOBATT_WARNING_SIGNAL())
            EQC_BeepSystemMessageBox((BYTE *)SKY_GET_TOKEN(TKN_CHARGING_NEEDED), 
                                2, 
                                MBT_NONE, 
                                MBT_SYSTEM);
/*
        else
            EQC_SystemMessageBox((BYTE *)SKY_GET_TOKEN(TKN_CHARGING_NEEDED), 
                                2, 
                                MBT_NONE, 
                                MBT_SYSTEM);
*/
        break;

    case SE_SYS_UPDATE_DISPLAY:
        /* isn't there annunciators in current screen? */
        if(sIS_ANNUNCIATORS_WINDOW(pszStr))
        {
            SKY_DrawAnnunciators();
        }
		SKY_UpdateScreen();	

        break;

//    case SE_SYS_OFFLINE:
    case SE_CALL_ORIGINATED:
//    case SE_CALL_ENDED:       // woodstok 01.2.15
    case SE_CALL_INCOMING:
        if(SKY_GET_DATAMANAGER())
            SKY_SET_DATAMANAGER(FALSE);
    case SE_SYS_TO_IDLE : /* very important */
    case SE_DM_IN_PROCESSING:
    case SE_CALL_COMMAND_ERROR: // hjcho 01.04.20
        {
            HCONTROL hControl;

            if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
            {
              EQS_DeleteControl(hControl);
            }

			if(SKY_IsInCall() && (pInEvent->EventID == SE_SYS_TO_IDLE))
			{				
				break;
			}
			else
			{
				EQS_EndToCurrentEvent(EQS_POP_TO_ROOT);
			}
        }
        break;

    // woodstok 01.2.15
    case SE_SYS_OFFLINE: /*UI_OFFLISE_F:       go offline */
        SKY_SET_OFFLINE(TRUE);
        SKY_SaveNV();    // ui_nv_savestuff();

        SKY_TRACE( TL_HIGH, "UI has gone offline");
        break;

    case SE_SYS_PACEMAKER:
        /* 음성 통화인 경우에만 통화 시간을 표시한다. */
        if(IsInVoiceConversation())
        //if((SKY_IS_IN_GVM() == FALSE)
        //   && (SKY_IS_IN_WAP() == FALSE) 
        //   && (ui_curr_call_is_sms() == FALSE)
        //   && (SKY_IS_EMAIL_INPROG() == FALSE)
        //   && (SKY_IS_PM() == FALSE)
        //   && ui_is_in_conversation())
        {
            clk_julian_type julian;

            SKY_GetCallingTime(&julian);
#ifdef FEATURE_SKY_SUBLCD
            SKY_DisplayCallingTimeInSubLCD(&julian);
#endif 
        }
        else
        {
#ifdef FEATURE_SKY_SUBLCD
            DisplayCurrentDateTimeInSubLCD(FALSE);
#endif //#ifdef FEATURE_SKY_SUBLCD
        }

        if (SKY_SetThenCompareProtocolRevision(SKY_GET_PROTOCOL_REV())
            || (SKY_IS_NEW_MESSAGE()))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);

            /* isn't there annunciators in current screen? */
            if(sIS_ANNUNCIATORS_WINDOW(pszStr) == FALSE)
            {
#ifdef FEATURE_SKY_SUBLCD
#ifdef WAPPER
                if(SKY_IS_IN_WAP())
                    SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
                else
#endif
                    SKY_DrawSubAnnunciators();
#endif
            }
        }
        if(!g_fSubBackLightOff && (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME))
        {
            if(!SKY_IsSubBacklightOnTime())
            {
                SKY_SubBacklightOff();  
                g_fSubBackLightOff = TRUE; 
                g_fSubBackLightOn = FALSE; 
            }
        }
        if(!g_fSubBackLightOn && (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME))
        {
            if(SKY_IsSubBacklightOnTime())
            {
                SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());  
                g_fSubBackLightOn = TRUE; 
                g_fSubBackLightOff = FALSE; 
            }        
        }

#ifdef FEATURE_SKTT_EQUS_LOCATION
        if((EQS_GET_APPLET() == SKA_MainAllCode)
            ||(EQS_GET_APPLET() == SKA_Dialing)
            ||(EQS_GET_APPLET() == SKA_MissedCall)
            ||(EQS_GET_APPLET() == SKA_MainIdleScreen)
            ||(EQS_GET_APPLET() == SKA_InProg)
            ||(EQS_GET_APPLET() == SKA_IdleDebugScreen)
            ||(EQS_GET_APPLET() == SKA_MessageNotif)
            ||(EQS_GET_APPLET() == SKA_FlashWithInfo))
        {
            pLocation =(LOCATION_INFO_T *)&SKY_GET_LOCATION_INFO();
            if( pLocation->onoffstate && pLocation->rcvinfo && pLocation->reportwait)
            {
                EQS_SEND_EVENT(SE_LOCATION_TIMEOUT, EQS_NA, EQS_NA);
                pLocation->reportwait = 0;
                SKY_SET_LOCATION_INFO(*pLocation);
            }   
        }
#endif //FEATURE_SKTT_EQUS_LOCATION
        break;

    case SE_PH_SERVICE_STATE:
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

        // Qualcomm original:: fast_update_rssi_icon((ui_ph_event_type *)&(ui.evt_param.ph_event));
        if (SKY_SetThenCompareRSSI(SKY_GET_PHONE_INFO_RSSI()) ||
            SKY_SetThenCompareProtocolRevision(SKY_GET_PROTOCOL_REV()))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);

            if(sIS_ANNUNCIATORS_WINDOW(pszStr) == FALSE)
            {
#ifdef FEATURE_SKY_SUBLCD
#ifdef WAPPER
                if(SKY_IS_IN_WAP())
                    SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
                else
#endif
                    SKY_DrawSubAnnunciators();
#endif
            }
        }
        break;

    case SE_PH_RSSI:

        // Qualcomm original:: fast_update_rssi_icon( &(cmd_ptr->ph_event));
        if (SKY_SetThenCompareRSSI(SKY_GET_PHONE_INFO_RSSI()) ||
            SKY_SetThenCompareProtocolRevision(SKY_GET_PROTOCOL_REV()))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);

            if(sIS_ANNUNCIATORS_WINDOW(pszStr) == FALSE)
            {
#ifdef FEATURE_SKY_SUBLCD
#ifdef WAPPER
                if(SKY_IS_IN_WAP())
                    SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
                else
#endif
                    SKY_DrawSubAnnunciators();
#endif
            }
        }
        break;

    case SE_PH_INUSE_STATE:
        if (SKY_SetThenCompareInUseState(SKY_GetInUseState()))//SKY_IS_IN_USE()))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);

            if(sIS_ANNUNCIATORS_WINDOW(pszStr) == FALSE)
            {
#ifdef FEATURE_SKY_SUBLCD
#ifdef WAPPER
                if(SKY_IS_IN_WAP())
                    SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
                else
#endif
                    SKY_DrawSubAnnunciators();
#endif
            }
        }
        break;

#ifdef FEATURE_UASMS
    case SE_SMS_MYBELL:   
    case SE_SMS_MESSAGE:
//        SKY_StopMusic();
		if (!SKY_IS_IN_WAVELET()) // yoursun 01/10/08
			SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());  
        break;
#endif
/* reo 2001.03.19 {-- */
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_INFORM:
//        SKY_StopMusic();
        SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());  
        break;

    case SE_LOCATION_TIMEOUT:
        pLocation =(LOCATION_INFO_T *)&SKY_GET_LOCATION_INFO();
        if(!pLocation->reportwait) 
        {
            pLocation->reportwait = 0x01;
            SKY_SET_LOCATION_INFO(*pLocation);
        }
        break;
#endif //FEATURE_SKTT_EQUS_LOCATION
/* reo 2001.03.19 --} */
    // Handle Call Alerting Events.
    case SE_CALL_SIGNAL:
        // Call중이고 Originated인 경우에는 처리하지 않음.
        if(!ui_is_in_conversation() && !SKY_IS_ORIGINATE() && SKY_IS_ALERT())
        {
            SKY_SetBellTimer(500, FALSE, SE_CALL_SIGNAL_TIMEOUT);   // Alerting After 500msec.
//            SKY_DoAlertSignal();
//            SKY_SNI(HS_SNI_CALL_ALERT, TRUE);
        }
        break;

    case SE_CALL_SIGNAL_TIMEOUT:
        if(!ui_is_in_conversation() && !SKY_IS_ORIGINATE() && SKY_IS_ALERT())
        {
            SKY_DoAlertSignal();
        }
        break;

    case SE_CALL_PLAY_BELL: // 진동후 벨일때 벨을 연주하기 위한 Event
        // Call중일때는 처리하지 않음.
        SKY_MotorStop();

        if(!ui_is_in_conversation() && !SKY_IS_ORIGINATE() && SKY_IS_ALERT())
        {
            SKY_PlayBellAfterVib();
        }
        break;
    
    // End Call Alerting

    case SE_IO_BEEP_SOUND:      // from EQS Control
//        SKY_PlayToneEx( UI_BEEP_SND, SND_WARN, 150 , 3 );
        break;

    // Message box에 따른 소리를 내기 위해서
    case SE_IO_OK_MESGBOX_SOUND:
    case SE_IO_FAIL_MESGBOX_SOUND:
    case SE_IO_HELP_MESGBOX_SOUND:
    case SE_IO_USER_MESGBOX_SOUND:
        if(IsInVoiceConversation() || SKY_IS_ALERT() || SKY_IS_ORIGINATE() // Data Call일때는 소리가 나도록 
            || SKY_IS_IN_GAME() || SKY_IS_IN_ALARM()

#ifdef FEATURE_WAP
            || SKY_IS_IN_WAP()
#endif//#ifdef FEATURE_WAP
#ifdef FEATURE_GVM
            || SKY_IS_PLAYING_GVM()
#endif
            )
            break;

        if(!SKY_IS_MESSAGE_BEEP_SOUND())
            break;
        // go through..
    case SE_IO_SYSTEM_MESGBOX_SOUND:
        if(SKY_IS_ALERT() || SKY_IS_IN_GAME() || SKY_IS_IN_ALARM()
#ifdef FEATURE_GVM
            || SKY_IS_PLAYING_GVM()
#endif
            )
            break;

        if((gnVocoderMode != VR_NONE || SKY_IS_ETIQUETTE()) && (!SKY_IS_EAR_JACK_ON()))
            break;

        if(!SKY_IS_MESSAGE_BEEP_SOUND())
            break;

        switch(pInEvent->EventID){
        case SE_IO_OK_MESGBOX_SOUND:
        case SE_IO_HELP_MESGBOX_SOUND:
            SKY_PlayMusicEx(MUS_ERROR_6, 1, 1);
            break;
        case SE_IO_FAIL_MESGBOX_SOUND:
            SKY_PlayMusicEx(MUS_ERROR_3, 1, 1);
            break;
        case SE_IO_USER_MESGBOX_SOUND:
            SKY_PlayMusicEx(MUS_ERROR_9, 1, 1);
            break;
        case SE_IO_SYSTEM_MESGBOX_SOUND:
            //SKY_PlayMusicEx(MUS_ERROR_1, 1, 1);
            SKY_PlayMusicEx(MUS_ALLERT2, 1, 1);
            break;
        }
        break;

    // woodstok 01.4.16
    // 대기화면뿐아니라 모든 상태에서 에티켓 진입할 수 있도록 추가된 코드
    // default event handler에 넣어서 어떤 문제가 생길지 알 수 없음.
    case SE_KEY_MANNERS:        
        if(SKY_IS_ETIQUETTE())
        {
#ifdef FEATURE_SKY_SUBLCD
            SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_CLEAR_ETIQUETTE);
            SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD

            SKY_SET_ETIQUETTE(FALSE);
            if(!SKY_IS_ALERT())
                SKY_PlaySoundEx( UI_MSG_SND, SND_PIP_TONE, 5, 1);
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

        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        /* jrkwon : re-display sub lcd as default */
        SKY_SubLCDDefault();
        break;

	case SE_IO_TO_IDLE:		// From MessageBox
            //if(SKY_IsInCall())
            //{
            /* 자동응답/녹음/재생 중 메시지 박스가 떠 있는 상태에서 INCOMING CALL이      */
            /* 왔을 때, 종료키를 누르면 SE_IO_TO_IDLE이 발생하고 이로 인해 call이 끊기지 */
	    /* 않고 대기상태로 나가는 경우가 발생한다. 이를 방지하기 위해 call 중인 경우 */
	    /* SE_IO_TO_IDLE이 발생하면 무시한다.                                        */
            //			break;
            //		}
            //		else
            {
	       EQS_END_TO(EQS_POP_TO_ROOT);
            }
            break;

    case SE_TM_TIMER:
        SKY_SetGeneralTimer(0, FALSE); // Stop general timer.
        break;

    case SE_PH_MAINTREQ : /* 미 등록 단말기 */
        EQC_SystemMessageBox((BYTE* )SKY_GET_TOKEN(TKN_NOT_REGISTERED), 
                              sMAX_SEC_SYS_MSGBOX, MBT_NONE, MBT_SYSTEM);

        SKY_SET_MAINTREQ(TRUE);
/* jrkwon 2001.11.20 */
#ifdef FEATURE_MULTI_SID
        if(SKY_GET_PHONE_INFO_CURRENT_NAME() == CM_NAM_3)
            EQS_START_CHILD(SKA_ReceiveMultiSID);
#endif //FEATURE_MULTI_SID
        break;

    case SE_PH_UNLOCK : /* 미 등록 단말기해제 */
        SKY_SET_MAINTREQ(FALSE);
        break;
#ifdef  FEATURE_SKT_EIF     // heroine 01/04/10
    case SE_SET_LCD:

        EQS_START_CHILD(SKA_MainEif);
        break;
#endif  // FEATURE_SKT_EIF     // heroine 01/04/10

    case SE_RESET_LCD:
        break;

#ifdef  FEATURE_SKT_EIF     // heroine 01/04/10
    case SE_MO_REQ:
    case SE_EM_REQ:

        EQS_START_CHILD_CURRENT_EVENT( SKA_MainEif );
        break;
#endif  // FEATURE_SKT_EIF     // heroine 01/04/10

#ifdef  FEATURE_SKT_EIF     //redstar 01.04.11
	case SE_EIF_PLAY_BEEP:
		//SKY_PlayTone(UI_MSG_SND, SND_RING_A5,100);
		break;
	case SE_EIF_PLAY_BELL:
		if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))
             SKY_PlayMusicFileEx(SKY_GET_BELL_FILE_NAME(), 5);
        else
             SKY_PlayMusicEx(SKY_GET_BELL_ID(), 5, 0);
		//SKY_PlayMusic(SKY_GET_BELL_ID());
		break;
	case SE_EIF_STOP_BELL:
		SKY_StopMusic();
		break;
#endif  //FEATURE_SKT_EIF     //redstar 01.04.11


#ifdef FEATURE_UI_DORMANT
	case SE_DATA_DORM_F:
		EQS_END_TO(EQS_POP_TO_ROOT);
		break;

	case SE_DATA_IDLE_F:
		SKY_SET_DORMANT(FALSE);
        SKY_SetThenCompareInUseState(SKY_GetInUseState());
		EQS_END_TO(EQS_POP_TO_ROOT);
		break;
#endif
     case SE_CALL_DISPLAY:
        EQS_EndToCurrentEvent(EQS_POP_TO_ROOT);
		break;

     case SE_RECV_DATA:
#ifdef FEATURE_IP_AIRDATA
	    if (SKY_GET_IP_AIRDATAMANAGER())
	    {
            byte data[DSM_DS_SMALL_ITEM_SIZ];
		    uint16 length=0;

		    while((length = SvrLinkRcv(data)) > 0)
		    {
			    AirDataManagerRX(data, (word)length);
		    }
	    } 
	    else
#endif	// #ifdef FEATURE_IP_AIRDATA
#ifdef	FEATURE_SKT_AIRDATA	
	    if (SKY_GET_AIRDATAMANAGER()) 
	    {
		    byte data[DSM_DS_LARGE_ITEM_SIZ];
		    uint16 length=0;
#ifndef WIN32
		    while((length = PtpLinkRcv(data)) > 0)
		    {
			    AirDataManagerRX(data, (word)length);
		    }
#endif
	    }
#else
        break;
#endif        // #ifdef	FEATURE_SKT_AIRDATA
        break;
     
     case SE_CALL_STACK_CHANGED:
        if(SKY_GET_CALL_STACK_CHANGED() > 254)
        {
            SKY_SET_CALL_STACK_CHANGED(0);
        }
        SKY_SET_CALL_STACK_CHANGED((SKY_GET_CALL_STACK_CHANGED()+1));
        break;

     case SE_DATA_CALL:
        EQS_EndToCurrentEvent(EQS_POP_TO_ROOT);
        break;
#ifdef FEATURE_SKVM
	 case SE_SKVM_INPUT_DATA:	
		SKY_SET_PUSH_SMS_IN_SKVM(TRUE);
        SKY_MotorTime(800);
		break;
#endif
#ifdef FEATURE_MULTI_SID
    case SE_SYS_MULTI_SID:
        EQS_START_CHILD(SKA_ReceiveMultiSID);
        break;
#endif //FEATURE_MULTI_SID
     default:
        break;
    }
}
