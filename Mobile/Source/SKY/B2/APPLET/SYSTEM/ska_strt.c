 /*
** ===========================================================================
**
** Project: 
**     Athens
**
** File:
**     ska_strt.c
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
** 2000-11-28 redstar    Modify to SKY Sytem
** 2000-07-20 jrkwon     Created.
**
** ===========================================================================
*/

/*
** ***************************************************************************
  The state transition diagram
  ============================
  S: Sibling, C: Child

  SKA_MainOff 
       |             
       | 0)S      
       |          
       V          
  SKA_MainStartUp 
       |       
       |       
       | 1)S   
       V       
  sSKA_Entry --> sSKA_WaitForTimeOutOrKeyPress --> sSKA_GetReadyToGoToIdle -+
             2)S                               3)S                          |
  +-------------------------------------------------------------------------+
  |                        
  +--> SKA_MainIdle  
   4)S                

** ***************************************************************************
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "comdef.h"
#include "filesys.h"    /* read_superblock */
#include "voc.h"
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
// tasik 00.04.24
#define FEATURE_KOREA_BAND

#define THREE_SECS 3000           /* 3 seconds in millisecs */

// ??? must put these defaults in header and share with uinsserv.c
// ??? search for PCH_A_DEF
// modified by chos 00.03.03    default channel을 400으로
// remodifed by tasik 00.04.24  default channel to 779 for A, to 738 for B

/* Primary Paging Channel Defaults */
#define CDMAMIN 1
#define NID_DEFAULTS 65535

#define PCH_A_DEF 779           /* Primary Channel A Default */
#define PCH_B_DEF 779           /* Primary Channel B Default */

/* Secondary Paging Channel Defaults */
#define SCH_A_DEF 738           /* Secondary Channel A Default */
#define SCH_B_DEF 738           /* Secondary Channel B Default */


/* Primary Paging Channel Defaults */
#define PCH_A_DEFAULT 779           /* Primary Channel A Default */
#define PCH_B_DEFAULT 779           /* Primary Channel B Default */

/* Secondary Paging Channel Defaults */
#define SCH_A_DEFAULT 738           /* Secondary Channel A Default */
#define SCH_B_DEFAULT 738           /* Secondary Channel B Default */


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
** 2001.02.14 jrkwon
** below flags are defined in the SKY signal handler (sky_hsig.c)
** for the case when MC decides the phone should power down,
** because of wrong lock code being sent over the air
*/

/* use these flags to treat all IMSI components as one */
boolean initialize_imsi[NV_MAX_NAMS] ;
boolean imsi_all_zeros[NV_MAX_NAMS] ;

#ifdef FEATURE_IS95B_TRUE_IMSI
boolean initialize_imsi_t[NV_MAX_NAMS];
boolean imsi_t_all_zeros[NV_MAX_NAMS];
#endif


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
//static nv_banner_type  nv_banner; /* banner */
static nv_item_type    nvi;       /* NV items */

static nv_cmd_type ui_nv;   /* NV command structure */
//static nv_name_nam_type  nv_nam_name; /* current nam name */



#define HAS_FM(nam)            fHasfm[nam]
#define HAS_CDMA(nam)          fHascdma[nam]
#define HAS_ACDMA(nam)         fHasacdma[nam]
#define HAS_BCDMA(nam)         fHasbcdma[nam]


#define SET_HAS_FM(nam,     flag)            fHasfm[nam]      = flag
#define SET_HAS_CDMA(nam,   flag)            fHascdma[nam]    = flag
#define SET_HAS_ACDMA(nam,  flag)            fHasacdma[nam]   = flag
#define SET_HAS_BCDMA(nam,  flag)            fHasbcdma[nam]   = flag

//redstar 00.11.28
BOOL fHasfm[NV_MAX_NAMS];     /* whether there is analog in NAM */
BOOL fHascdma[NV_MAX_NAMS];   /* whether there is CDMA in NAM */
BOOL fHasacdma[NV_MAX_NAMS];  /* has channel A CDMA */
BOOL fHasbcdma[NV_MAX_NAMS];  /* has channel B CDMA */

#define GET_MIN1( a, b)                 dwMin1[a][b]
#define GET_MIN2( a, b)                 wMin2[a][b]
#define SET_MIN1( a, b, dwValue)        dwMin1[a][b] = dwValue
#define SET_MIN2( a, b, wValue)         wMin2[a][b]  = wValue

DWORD   dwMin1[NV_MAX_NAMS][NV_MAX_MINS]; /* hold min1s */
WORD    wMin2[NV_MAX_NAMS][NV_MAX_MINS]; /* hold min2s */

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_ShutDown(SKY_EVENT_T* pEvent); /* SHUT_S */
LOCAL VOID SKY_API sSKA_WaitForTimeOutOrKeyPress(SKY_EVENT_T* pEvent); /* ID_S */
LOCAL VOID SKY_API sSKA_GetReadyToGoToIdle(SKY_EVENT_T* pEvent); /* IDLE_S */
LOCAL VOID SKY_API sSKA_Entry(SKY_EVENT_T* pEvent); /* ENTER_S */

LOCAL void SKY_API sSetServiceProvider(void);

/*
**---------------------------------------------------------------------------
**
**
**
**
**
**
**  QualComm startup code
**
**
**
**
**
**
**---------------------------------------------------------------------------
*/

// modified by chos 00.03.03    A,B System별 channel check 안하게 수정
// remodified by tasik 00.04.24    Allow 'A, B band' to be check.

#ifdef FEATURE_SSPR_ENHANCEMENTS
/* <EJECT> */
/*===========================================================================

FUNCTION IS_CELLULAR_SYS_A_CHANNEL

DESCRIPTION
  Check if the CDMA channel is with Cellular Band System A.

DEPENDENCIES
  None

RETURN VALUE
  boolean: TRUE - if the channel is within Cellular Band System A
           FALSE - otherwise.

SIDE EFFECTS
  None

===========================================================================*/

static boolean is_cellular_sys_a_channel
(
  word cdma_channel       /* The channel to examine */
)
{
        /* Refer to IS-95A, Section 6.1, Table 6.1.1.1-1 */
#ifdef OMIT //kjh '00.4.28
        return (
#ifdef FEATURE_KOREA_BAND

        ((cdma_channel >=    1) && (cdma_channel <=  333))  ||  /* Block A  */
        ((cdma_channel >=  667) && (cdma_channel <=  716))  ||   /* Block A' */
        ((cdma_channel >=  991) && (cdma_channel <= 1023))       /* Block A" */

#else

        ((cdma_channel >=    1) && (cdma_channel <=  311))  ||  /* Block A  */
        ((cdma_channel >=  689) && (cdma_channel <=  694))  ||  /* Block A' */
        ((cdma_channel >= 1013) && (cdma_channel <= 1023))      /* Block A" */

#endif
                );
#else 
        return(TRUE);
#endif
} /* is_cellular_sys_a_channel */


/* <EJECT> */
/*===========================================================================

FUNCTION IS_CELLULAR_SYS_B_CHANNEL


DESCRIPTION
  Check if the CDMA channel is with Cellular Band System B

DEPENDENCIES
  None

RETURN VALUE
  boolean: TRUE - if the channel is within Cellular Band System B
           FALSE - otherwise.

SIDE EFFECTS
  None

===========================================================================*/
static boolean is_cellular_sys_b_channel
(
  word cdma_channel       /* The channel to examine */
)
{
    /* Refer to IS-95A, Section 6.1, Table 6.1.1.1-1 */
#ifdef OMIT //kjh '00.4.28
    return (

#ifdef FEATURE_KOREA_BAND

           ((cdma_channel >=  334) && (cdma_channel <=  666))  ||  /* Block B  */
       ((cdma_channel >=  717) && (cdma_channel <=  799))     /* Block B' */

#else

            ((cdma_channel >=  356) && (cdma_channel <=  644))  ||  /* Block B  */
        ((cdma_channel >=  739) && (cdma_channel <=  777))     /* Block B' */

#endif
               ); // return
#else
        return(TRUE);
#endif
} /* is_cellular_sys_b_channel */

#endif //FEATURE_SSPR_ENHANCEMENTS

// finish chos

/* <EJECT> */
/*===========================================================================
FUNCTION FINAL_INIT

DESCRIPTION
  User interface startup final state initialization.

DEPENDENCIES
  Preliminary initialization is complete.

SIDE EFFECTS
  Plays with the display
===========================================================================*/

/////////////////////////////////////////////////////////////////
// jwpark 01.05.10
static rex_timer_type ui_play_rec_timer;
static void ui_play_rec_cb(voc_change_status_type status)
{
	if(status == VOC_ACQ_RE_INIT_CDMA)
      rex_set_sigs(&ui_tcb, UI_PLAY_REC_SIG);
}
/////////////////////////////////////////////////////////////////

void SKY_API sInitStartup(void)
{
#ifndef WIN32
#ifdef T_MSM5000 
		//키톤이 깨지는 문제를 회피하기 위해서 넣었습니다.
	//문제가 생기면 jwpark이나 youandi 에게 연락주세요.  
	//키톤문제는 MSM5000에만 있으니 다른 MSM을 쓰시는 분들에겐 필요없는 코드이니 빼주세요
	//youandi 01.07.19
   /////////////////////////////////////////////////////////////////
   // jwpark 01.05.10
	rex_def_timer(&ui_play_rec_timer, &ui_tcb, UI_PLAY_REC_SIG);
	voc_acquire(VOC_ACQ_UI, ui_play_rec_cb);
	rex_set_timer(&ui_play_rec_timer, 500);
	for(;;)
	{
		extern rex_timer_type ui_rpt_timer;
		rex_sigs_type sigs;
		sigs = rex_wait(UI_RPT_TIMER_SIG | UI_PLAY_REC_SIG);

		if(sigs & UI_RPT_TIMER_SIG)
		{
			rex_set_timer(&ui_rpt_timer, DOG_UI_RPT_TIME);
			dog_report(DOG_UI_RPT);
		}
		if(sigs & UI_PLAY_REC_SIG)
		{
			rex_clr_timer(&ui_play_rec_timer);
			rex_clr_sigs(&ui_tcb, UI_PLAY_REC_SIG);
			break;
		}
	}
   /////////////////////////////////////////////////////////////////
	voc_config(VOC_ACQ_UI, VOC_CAP_EVRC);
	rex_set_timer(&ui_play_rec_timer, 100);
	for(;;)
	{
		extern rex_timer_type ui_rpt_timer;
		rex_sigs_type sigs;
		sigs = rex_wait(UI_RPT_TIMER_SIG | UI_PLAY_REC_SIG);
		if(sigs & UI_RPT_TIMER_SIG)
		{
			rex_set_timer(&ui_rpt_timer, DOG_UI_RPT_TIME);
			dog_report(DOG_UI_RPT);
		}
		if(sigs & UI_PLAY_REC_SIG)
		{
			rex_clr_timer(&ui_play_rec_timer);
			rex_clr_sigs(&ui_tcb, UI_PLAY_REC_SIG);
			break;
		}
	}
   /////////////////////////////////////////////////////////////////
	voc_release(VOC_ACQ_UI);
	rex_set_timer(&ui_play_rec_timer, 100);
	for(;;)
	{
		extern rex_timer_type ui_rpt_timer;
		rex_sigs_type sigs;
		sigs = rex_wait(UI_RPT_TIMER_SIG | UI_PLAY_REC_SIG);
		if(sigs & UI_RPT_TIMER_SIG)
		{
			rex_set_timer(&ui_rpt_timer, DOG_UI_RPT_TIME);
			dog_report(DOG_UI_RPT);
		}
		if(sigs & UI_PLAY_REC_SIG)
		{
			rex_clr_timer(&ui_play_rec_timer);
			rex_clr_sigs(&ui_tcb, UI_PLAY_REC_SIG);
			break;
		}
	}
   /////////////////////////////////////////////////////////////////
//youandi 01.07.19
#endif /* T_MSM5000 */
#endif /* ifndef WIN32 */


//#ifdef FEATURE_NEW_SOUND_API
    /* Set the paths */
    SKY_SetSoundDevice( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );
    SKY_SET_AUTO_LOCK_MODE(SKY_IS_AUTO_LOCK());
    SKY_SET_SMS_LOCK_MODE(SKY_IS_SMS_LOCK());        
//#else
    /* Set the paths */
//    SKY_SetSoundPath( UISND_NOVOICE, UISND_NOVOICE, SND_DEVICE_EARPIECE );
//#endif /* FEATURE_NEW_SOUND_API */

 
    // Init the volumes 
    SKY_SetBeepVolume(SKY_GET_KEY_TONE_VOLUME());        // Key-beep Volume 
    SKY_SetRingerAlertVolume(SKY_GET_BELL_VOLUME());
    SKY_SetSMSAlertVolume(SKY_GET_SMS_BELL_VOLUME());
    SKY_SetAlarmAlertVolume(SKY_GET_ALARM_BELL_VOLUME());
    SKY_SetVoiceVolume(SKY_GET_SPEAKER_VOLUME());
#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE  
    SKY_SetMicLevel(SKY_GET_MIC_VOLUME());             // set mic gain 
#endif

    SKY_SET_KEY_TONE(TRUE);

    // audio path 초기화
    SKY_SetSoundDevice( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );
    SKY_StopTone();

    SKY_SET_VALID_SYSTEM_TIME(FALSE);

#ifdef FEATURE_UASMS
    ui_sms_power_up ();

#ifdef FEATURE_BROADCAST_SMS
    //uiuasms_bcsms_init ();
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

    EQC_SetSKYHangulMode(SKY_GET_SEL_HANGUL()); // 한글입력방법
    //EQC_SetCurrentMesgBoxTheme(SKY_GET_COLOR_SCHEME());    

    SKY_DisplayBellVibIcon();
    SKY_SET_SHORTBURST_DTMF(TRUE);  /* DTMF type을 CDMA type으로 설정 */
    SKY_SET_DIGITAL(FALSE);
    InitMessageQueue();             /* Message Queue를 초기화-착신전환*/
    ClearCLI();                     /* CNAP, CNIP, CDNIP정보를 초기화 */
    UpdateLastDialInfo((BYTE *)SKY_GET_LASTDIAL_INFO_BUFF(), SKY_GET_LASTDIAL_INFO_POS());

    /* delete un-closed data file in the previous session */
    if(SKY_IS_OPEN_DATA_FILE())
    {
        SKY_DeleteFile(SKY_GET_OPEN_DATA_FILE_NAME());
        /* reset file name */
        SKY_CLEAR_OPEN_DATA_FILE_NAME();
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

//static char *badbatt_txt_ptr;          /* ptr to text field for UI_BADBATT_F */
static boolean good, goodnam;
static boolean id_is_timeout;          // Display ID timeout


void SKY_API SKY_PowerOff(POWER_OFF_MODE PowerOffMode)
{
    if(PowerOffMode == POM_DOWN)
    {
        ui_power_down();
    }
    else if(PowerOffMode == POM_RESET)
    {
        ui_power_reset();
    }
    SKY_SET_POWER_OFF_MODE(PowerOffMode);
}


LOCAL VOID SKY_API sSKA_ShutDown(SKY_EVENT_T* pEvent) /* SHUT_S */
{

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindowCML(CL_NONE);

#ifdef FEATURE_HOLD_PWRKEY_TO_PWRUP
#error code not present
#endif /* FEATURE_HOLD_PWRKEY_TO_PWRUP */

        //ui.powerup = FALSE;      
        SKY_SET_SHUT_DOWN(TRUE);

        SKY_Screen(OFF);  // 01.6.5  shutdown 절차를 수행하기 전에 LCD를 먼저꺼줌.
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubScreen(OFF); // jrkwon 2001.06.26 sub lcd도 꺼줌
        /* jrkwon 2001.06.28 --> 근데, 꺼주는 게 없네.. 휴.. 대신 공백을 출력해서 꺼진 거 처럼 보이게 함 */
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
        SKY_SubLCDTextOut(LN_UPPER, TKN_NULL);
        SKY_SubLCDTextOut(LN_LOWER, TKN_NULL);
#endif
        /* jrkwon 2001.06.26 --> backlight도 꺼줘야지... */
        SKY_Backlight(OFF);

        #if (TG!=T_MD)
        SKY_StopSound();
        SKY_StopMultiTone();
        SKY_StopTone();
        #endif /* TG!=T_MD */
        
        SKY_AcceptKey(FALSE);/* ignore keys */

        //if((ui.vibrator != 1)&&(!ui.manner)|| ui.ear_jack)
#ifdef FEATURE_DS_UP
#error code not present
#endif

        ui_nv.tcb_ptr = NULL;
        ui_nv.done_q_ptr = NULL;
        ui_nv.cmd = NV_CHKPNT_DIS_F; /* Disable NV checkpointing so that */
        nv_cmd ( &ui_nv );           /* powerdown won't be miserably slow */

 //       ui_nv_savestuff();       
        SKY_SaveNV();                 /* save stuff to NV */

    
        SKY_TRACE(TL_HIGH," SENDING POWERDOWN TO MC ");

        //ui_power_down();
        /* jrkwon : 2001-02-17 
        ** SKY_PowerDown이 불리지 않고, 
        ** 이 함수가 호출된 경우(전원키를 눌러서)에만 
        ** SKY_PowerDown()을 불러준다.
        */
        if(SKY_GET_POWER_OFF_MODE() == POM_NONE)
            SKY_PowerOff(POM_DOWN); 
        
        /* jrkwon 2000-08-09 --> No meaning next time state in EQUS */
#ifdef FEATURE_OTASP
                // jrkwon 2001-02-14 ui_pwrdown_complete = TRUE ; /* we can ack to TASK_STOP_SIG */
        g_fPowerDownComplete = TRUE;
#endif
        /* <-- */
        break;
    }
}

/*****************************************************************/
/* Wait for timeout or key press while the powerup display is up */
/*****************************************************************/
LOCAL VOID SKY_API sSKA_WaitForTimeOutOrKeyPress(SKY_EVENT_T* pEvent) /* ID_S */
{
    //cm_ph_info_s_type PhoneInfo;

#ifndef WIN32
    if (ui_ph_info_is_avail()) 
    {
        SKY_AcceptKey(TRUE);      
    } 
    else 
    {
        /* jrkwon 2001.06.26 
        ** 키입력을 무시하면, SE_KEY_EAR_JACK_ON 키가 
        ** 시작음이 스피커로 울린 후에야 들어오는 경우가 있다.
        */
        //SKY_AcceptKey(FALSE);  // disable key input 
    }
#else
    SKY_AcceptKey(TRUE);
#endif

    /* jrkwon 2001.06.19 
       move here from the end of this function.
       because, sSka_GetReadyToGoToIdle as a sibling is called more one time.
    */
    if(EQS_GetCategory((EQS_EVENT_ID) pEvent->EventID) == EQC_KEY)
    {
        EQS_StopTimer(EQS_TIMER_1);    
        /* jrkwon 2001.06.26 
        ** 키입력을 무시하면, SE_KEY_EAR_JACK_ON 키가 
        ** 시작음이 스피커로 울린 후에야 들어오는 경우가 있다.
        */
        //SKY_AcceptKey(FALSE);   

        EQS_START_SIBLING(sSKA_GetReadyToGoToIdle, TRUE);

        return; /* next event is SE_APP_END */
    }

    switch(pEvent->EventID)
    {

    case SE_APP_START:
        /* clear all keyborad event.. watch out the side effects...!!! */
        SKY_ClearAllEvent();

#ifdef SW_RESET_ENABLE
        if(!mc_bSwReset)
#endif
        {
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
        }
        EQS_StartTimer(EQS_TIMER_1, 3000, FALSE); // 3 sec.s
        break;

    case SE_APP_END:
        break;

    case SE_PH_INFO_AVAIL :  
        SKY_AcceptKey(TRUE);   // enable key input
        if (id_is_timeout) 
        {
            // go to IDLE_S if ID is already timeout
            EQS_START_SIBLING(sSKA_GetReadyToGoToIdle, TRUE);
        }
#if (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) )
            /*--- Reset to Standard if system is either A or B ---*/
        {
            cm_sys_pref_e_type sys_pref = ui.ph_info.sys_pref;          

            if (sys_pref == CM_SYS_PREF_A_ONLY || sys_pref == CM_SYS_PREF_B_ONLY) 
            {
                ui_set_ph_sys_pref(CM_SYS_PREF_STANDARD);
                SKY_TRACE(TL_MED, "Powerup system is invalid - A/B", 0, 0, 0);
            }
        }
            
        /*--- Set the proper Mode ---*/
        {
            cm_mode_pref_e_type mode_pref = ui.ph_info.mode_pref;          

            if (mode_pref == CM_MODE_PREF_AUTOMATIC
                || mode_pref == CM_MODE_PREF_DIGITAL_ONLY
                || mode_pref == CM_MODE_PREF_CELL_CDMA_ONLY
                || mode_pref == CM_MODE_PREF_PCS_CDMA_ONLY) 
            {
                SKY_TRACE(TL_MED, "Powerup mode is valid", 0, 0, 0);
            }
            else
            {
                ui_set_ph_mode_pref(CM_MODE_PREF_AUTOMATIC, 
                                    CM_MODE_TERM_PERMANENT, 0);
                SKY_TRACE(TL_MED, "Powerup mode is invalid; Forcing it to Auto", 0, 0, 0);
            }
        }
#endif /* TriMode */

        // girin. 2000.12.25
        EQS_PASS_CURRENT_EVENT(); // To update and show RSSI annunciator.

        // fall through.

    case SE_TM_TIMER1 :
        id_is_timeout = TRUE;
#ifndef WIN32
        if (ui_ph_info_is_avail()) //redstar 
#endif
        {
            EQS_START_SIBLING(sSKA_GetReadyToGoToIdle, TRUE);
        } 

        break;

    case SE_KEY_CLEAR:
    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_SYS_TO_IDLE : /* very important */
    case SE_SYS_OFFLINE:
    case SE_CALL_INCOMING:
    case SE_CALL_ORIGINATED:
    case SE_CALL_ENDED:
    case SE_CALL_COMMAND_ERROR:

        // The root applet isn't ready yet...
        break;

    default:
        SKY_DefEventProc(pEvent);
        break;
    } /* switch(pEvent->EventID) */


    if(EQS_GetCategory((EQS_EVENT_ID) pEvent->EventID) == EQC_KEY)
    {
        EQS_StopTimer(EQS_TIMER_1);    
        SKY_AcceptKey(FALSE);       
        EQS_START_SIBLING(sSKA_GetReadyToGoToIdle, TRUE);
    }
}

/*********************************************************/
/* Get rady to go to the idle substate                   */
/*********************************************************/

LOCAL BITMAP l_BmpBuf = {
    { EQUS_IMAGE_ID, { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT}, 
        SKY_SCREEN_WIDTH, BIT_PER_PIXEL_256C, FALSE, MAX_COLOR, 0 },
    g_abBmpBuf
};

/* 시작 화면이 정지 이미지인 경우, 화면에 표시할 시간 */
#define sMAX_DISP_IMAGE_SEC                 3000
#define sDELAY_SEC_TO_PLAY_START_MUSIC      1000

LOCAL VOID SKY_API sSKA_GetReadyToGoToIdle(SKY_EVENT_T* pEvent) /* IDLE_S */
{
    HFILE hFile;
    BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 
    DWORD   dwTimeInfo = 0;
    STATIC s_fStartIdle = FALSE;

    switch (pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(SKY_DEF_WIN_X,
        SKY_DEF_WIN_Y, 
        SKY_DEF_WIN_DX, 
        SKY_DEF_WIN_DY, 
        CL_NONE, 
        NULL, 
        WA_NONE);

        dog_report( DOG_UI_RPT );

        SKY_AcceptKey(TRUE);

#ifdef SW_RESET_ENABLE
        if(mc_bSwReset)
        {
            EQS_START_SIBLING(SKA_MainIdle, FALSE);
            break;
        }
#endif

        s_fStartIdle = FALSE;

        if(IS_MY_PICTURE(SKY_GET_POWER_ON_PICTURE().wPictureID))    // 나만의 그림
        {
            if(SKY_OpenDataFile(SDF_IMAGE, SKY_GET_POWER_ON_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
            {
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

                if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
                {
                    SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                    SKY_CloseDataFile(hFile);
                    EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);

                    EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
                }
                else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
                {
                    ANIMATION_HEADER_T AniHeader;
                    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                    SKY_CloseDataFile(hFile);
                    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_POWER_ON_PICTURE().szFileName);
                    EQC_StartAnimation(EQC_Animation(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), NULL, abBuf), TRUE);
                }
            }
            else
                EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
        }
#ifdef FEATURE_CHARACBELL
        if(IS_CHARAC_PICTURE(SKY_GET_POWER_ON_PICTURE().wPictureID))    // 나만의 그림
        {
            if(SKY_OpenDataFile(SDF_CHA_IMAGE, SKY_GET_POWER_ON_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
            {
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

                if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
                {
                    SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                    SKY_CloseDataFile(hFile);
                    EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);

                    EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
                }
                else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
                {
                    ANIMATION_HEADER_T AniHeader;
                    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                    SKY_CloseDataFile(hFile);
                    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_CHA_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_POWER_ON_PICTURE().szFileName);
                    EQC_StartAnimation(EQC_Animation(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), NULL, abBuf), TRUE);
                }
            }
            else
                EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
        }
#endif // FEATURE_CHARACBELL
        else if(IS_MY_PHOTO(SKY_GET_POWER_ON_PICTURE().wPictureID)) // 나만의 사진
        {
            if(SKY_OpenDataFile(SDF_PHOTO, SKY_GET_POWER_ON_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
            {
                SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
                SKY_CloseDataFile(hFile); 

                l_BmpBuf.Header.yStart = 0;
                EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);
            }
            EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
        }
        else if(!IS_PICTURE_NOT_USED(SKY_GET_POWER_ON_PICTURE().wPictureID))
        {
            /* 기본 제공 애니메이션은 무조건 0,0부터 시작 */
            EQC_StartAnimation(EQC_Animation(0, 0, g_pBuiltInAnimation[SKY_GET_POWER_ON_PICTURE().wPictureID], NULL), TRUE);
        }
        else    // 그림지정 안함
        {
            s_fStartIdle = TRUE;
            //EQS_START_SIBLING(SKA_MainIdle, FALSE);
        }
        /* delay play the start music */
        EQS_StartTimer(EQS_TIMER_2, sDELAY_SEC_TO_PLAY_START_MUSIC, FALSE); 
        break;

    case SE_TM_TIMER2 :
        // Play PowerOn Melody
        if(SKY_GET_BELL_VIBRATION() != BV_BELL || SKY_IS_ETIQUETTE() ||
            (SKY_IS_AUTO_ANSWER() && SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_VIB_ONCE))
            SKY_MotorTime(1000);
    
        //if(((nvi.vibrator == 0) || (nvi.vibrator == 2) || (nvi.vibrator == 3)) && !ui.manner || ui.ear_jack)
        if((SKY_GET_BELL_VIBRATION() != BV_VIBRATION) && !SKY_IS_ETIQUETTE() || SKY_IS_EAR_JACK_ON() )
#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
            SKY_PlayMusicEx(MUS_PWR_ON, 3, 1);
#else
            SKY_PlayMusicEx(MUS_SMS_6, 2, 1);
    //        SKY_PlayTone( UI_PWRUP_SND, SND_MSG_WAITING, 200 );
#endif
        if(s_fStartIdle)
        {
            EQS_START_SIBLING(SKA_MainIdle, FALSE);
            s_fStartIdle = FALSE;
        }
        break;

    case SE_APP_END :   /* The very last event */ 
        /* jrkwon 2001.06.19 */
        /* clear all key event before going MainIdle applet */
        SKY_ClearAllEvent();
        break;

    case SE_TM_TIMER1 :
    case SE_IO_ANIM_TURN_AROUND:
    case SE_KEY_END:
    case SE_KEY_CLEAR:
    case SE_KEY_CENTER:
    case SE_KEY_LEFT:
    case SE_KEY_RIGHT:
    case SE_KEY_UP:
    case SE_KEY_DOWN:
    case SE_KEY_F1:
    case SE_KEY_F2:
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

        //SKY_MotorStop();
        //SKY_StopMusic();
        EQS_START_SIBLING(SKA_MainIdle, FALSE);

        break;

    //case SE_KEY_CLEAR:
    case SE_KEY_PHONE_CLOSE:
    //case SE_KEY_END:
    case SE_SYS_TO_IDLE : /* very important */
    case SE_SYS_OFFLINE:
    case SE_CALL_INCOMING:
    case SE_CALL_ORIGINATED:
    case SE_CALL_ENDED:
    case SE_CALL_COMMAND_ERROR:

        // The root applet isn't ready yet...
        break;

    default:
        SKY_DefEventProc(pEvent);
        break;
    }
}

void SKY_API SKY_InitSystemFileAndDataDir(void)
{
    /* init system information file */
    SKY_InitConfig();
    /* 데이터 디렉토리 초기화 */
    SKY_InitDataDir();
}

void SKY_API SKY_InitApplicationFiles(void)
{
    PBookOpen();    
    SKY_InitScheduler();
#ifndef WIN32
    SKY_InitVMS();
    SKY_InitMybell();
#endif
#ifdef FEATURE_SKTT_PM
    SKY_InitPM();
#endif
    SKY_InitCallStack();    /* Initialize Call Stack        */
#ifdef FEATURE_CALL_STACK_SEPERATED
    SKY_InitCallStackReceived();
#endif //FEATURE_CALL_STACK_SEPERATED
    SKY_InitMOSend();
    SKY_InitMOSent();
    SKY_InitMOSave();
#ifdef FEATURE_WAP
    WAP_READ_INITDATA();
#endif //#ifdef FEATURE_WAP
    SKY_InitCameraList();

    SKA_InitMail();     // Email list file
}

/* HW.C */
extern boolean hw_force_offline( void );

LOCAL VOID SKY_API sSKA_Entry(SKY_EVENT_T* pEvent) /* ENTER_S */
{
    INT16 i, j;
//    UINT8 nLoop;
    BOOL  fSuccess;
#ifdef FEATURE_SKTT_EQUS_LOCATION
        LOCATION_INFO_T         location;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

    if( pEvent->EventID == SE_APP_END )
        return;
    
    SKY_InitSystemFileAndDataDir();

#ifdef FEATURE_SKTT_EQUS_LOCATION
        location = SKY_GET_LOCATION_INFO();     
        SKY_TRACE1(TL_HIGH, "onoffstate = %d", location.onoffstate);
        if(location.rcvinfo && location.onoffstate)
        {
                SKY_SetLocationTimer((LONG)(location.cycle*60*1000), TRUE);
                SKY_TRACE(TL_HIGH, "SKY_SetLocationTimer");
        }               
#endif //FEATURE_SKTT_EQUS_LOCATION

    /* set theme */
    EQS_SetTheme(SKY_GET_COLOR_SCHEME(), SKY_GET_FONT_TYPE(), SKY_GET_SCROLLBAR_SIZE());
    SKY_SetCommandLineColor(SKY_GET_COLOR_SCHEME());
    
    for (i=0; i<NV_MAX_NAMS; i++)
    {
        initialize_imsi[i] = FALSE ; /* assume no need */
        imsi_all_zeros[i] = TRUE ;   /* assume true    */
    }

#ifdef FEATURE_IS95B_TRUE_IMSI 
    for (i=0; i<NV_MAX_NAMS; i++)
    {
        initialize_imsi_t[i] = FALSE ; /* assume no need */
        imsi_t_all_zeros[i] = TRUE ;   /* assume true    */
    }
#endif // FEATURE_IS95B_TRUE_IMSI

    /************************************************/
    /* Do all the initialization of the state items */
    /************************************************/
    /* jrkwon 2001.06.26 
    ** 키입력을 무시하면, SE_KEY_EAR_JACK_ON 키가 
    ** 시작음이 스피커로 울린 후에야 들어오는 경우가 있다.
    */
    //SKY_AcceptKey(FALSE);     /* ignore keys */

    SKY_SET_OFFLINE(FALSE);   /* offline */     
    SKY_SET_MAINTREQ(FALSE);
    SKY_SET_ALERT(FALSE);       /* alert on */ 
    SKY_SET_DTR_DISP(VOICE_INCOMMING);

    SKY_SET_MUTE(FALSE);          /* mute */    
    SKY_SET_DIGITAL(FALSE);       /* not digital */

    SKY_SET_TRANSMIT_DTMF(FALSE);    /* don't transmit dtmfs */ //ui.txdtmf

#ifdef FEATURE_UI_QUICK_VM_DIAL
#error code not present
#endif


#ifdef FEATURE_TTY
#error code not present
#endif /* FEATURE_TTY */

#ifdef FEATURE_USB_TESTING
#error code not present
#endif /* FEATURE_USB_TESTING */
       
    SKY_SET_SHUT_DOWN(FALSE);  //ui.powerup = TRUE; 
    SKY_SET_FORCE_SERVICE_OPTION(CAI_SO_VOICE_WILDCARD);   /* No force service option yet */   // ui.force_so 
   
    /*******************************************************************/
    /* Default the 13K origination option based on hardware capability */
    /*******************************************************************/

#ifdef FEATURE_OTASP

    /* The below variabels are needed to detect and correctly complete */
    /* an MC initated power down.                                      */
    /* jrkwon 2001-02-14 */
    g_fPowerDownComplete = FALSE;
    g_fMCInitiatedPowerDown = FALSE;
    //ui_pwrdown_complete = FALSE ;
    //ui_mc_initiated_pwrdown = FALSE ;
    SKY_SET_OTASP_FAILED(TRUE);            //ui.otasp_failed = TRUE ;    
    
#endif
    
    if( SKY_IS_POWER_ON() == FALSE )   // if( !ui.pwr ) 
    {           /* power off */      
        EQS_START_SIBLING(sSKA_ShutDown, TRUE);
        return; 
    }           

    /***************************************/
    /* Initialize the screen and backlight */
    /***************************************/

    // girin. 2000.12.31
    // SKY_LcdOn() is already called in InitHS() 
    // to display SKY boot logo.
    // girin:: SKY_Screen(TRUE);//uiscrn_scrn( TRUE, 500 ); /* turn screen on */
    //

#ifdef FEATURE_SKY_SUBLCD
    SKY_SubScreen(TRUE);
#endif//#ifdef FEATURE_SKY_SUBLCD

#ifdef SW_RESET_ENABLE
    if(!mc_bSwReset)
#endif
        SKY_SetupBacklight();    //ui_backlight_which();

    

    /*******************************************/
    /* Verify whether we have good NAMs or not */
    /*******************************************/
    good = goodnam = TRUE;

    SKY_GetNV( NV_ESN_CHKSUM_I, &nvi );
    if( nvi.esn_chksum.chksum != NV_VALID_ESN_CHKSUM ) 
    {
        SKY_TRACE(TL_HIGH, "BAD ESN CHECKSUM");
        good=FALSE;
    }

        /* Get all the NAM/MIN information that we need */
    nvi.curr_nam = (BYTE)SKY_GET_CURRENT_NAM();
        (void) SKY_PutNV(NV_CURR_NAM_I, &nvi ); /* put current nam */

    /* Get all the NAM/MIN information that we need */
    //(void) SKY_GetNV( NV_CURR_NAM_I, &nvi ); /* get current nam */

    /* If the NAM is out of range, default to the first NAM */
    if ( nvi.curr_nam >= NV_MAX_NAMS ) 
    {
      nvi.curr_nam = 0;
      (void) SKY_PutNV ( NV_CURR_NAM_I, &nvi );
    }

    for( i=0; i<NV_MAX_NAMS; i++ ) 
    {
        SET_HAS_CDMA(i, TRUE);   /* Assume the NAM is good for CDMA */     

#ifdef FEATURE_ACP
        SET_HAS_FM(i, TRUE);      /* Assume NAM is good for AMPS */     
#else
        SET_HAS_FM(i, FALSE);     /* No FEATURE_ACP, No AMPS! */ 
 
#endif /* FEATURE_ACP */
#ifdef FEATURE_ACP
        /*********************************/
        /* Determine if we have valid FM */
        /*********************************/
        nvi.analog_home_sid.nam = (byte) i; /* has analog if home_sid != 0 */
        /* if inactive or has invalid range ( i.e > 32767 */
        if( NV_NOTACTIVE_S == SKY_GetNV( NV_ANALOG_HOME_SID_I, &nvi ) ||
            nvi.analog_home_sid.sid &0x8000 )
        {
           /* then initialize it */
           nvi.analog_home_sid.sid = 0 ;
           (void) SKY_PutNV( NV_ANALOG_HOME_SID_I, &nvi );
        }
#endif /* FEATURE_ACP */

#ifdef FEATURE_CDMA_800
        /***********************************/
        /* Determine if we have valid CDMA */
        /***********************************/
        nvi.pcdmach.nam = (byte) i;  /* has CDMA if any of the 8 channels set */

        {
            boolean need_nv_save = FALSE;
            if ( NV_NOTACTIVE_S == SKY_GetNV( NV_PCDMACH_I, &nvi ) )
            {
                nvi.pcdmach.nam = i;
                nvi.pcdmach.channel_a = PCH_A_DEFAULT;
                nvi.pcdmach.channel_b = PCH_B_DEFAULT;
                need_nv_save = TRUE;
            }
// modified by chos 00.03.03    A,B System별 channel check 안하게 수정
// remodified by tasik 00.04.24, Allow 'A, B BAND' check

#ifdef FEATURE_SSPR_ENHANCEMENTS
            else
            {
                if (!is_cellular_sys_a_channel(nvi.pcdmach.channel_a)) 
                {
                    /* Channel A number is invalid, default it */
                    nvi.pcdmach.channel_a = PCH_A_DEF;
                    need_nv_save = TRUE;
                }

                if (!is_cellular_sys_b_channel(nvi.pcdmach.channel_b)) 
                {
                  /* Channel B number is invalid, default it */
                    nvi.pcdmach.channel_b = PCH_B_DEF;
                    need_nv_save = TRUE;
                }
            }
#endif
            /* If any of the value changed, update NV */
            if (need_nv_save) 
            {
                (void) SKY_PutNV( NV_PCDMACH_I, &nvi );
            }
        }
        SET_HAS_ACDMA( i, (BOOL) nvi.pcdmach.channel_a);
        SET_HAS_BCDMA( i, (BOOL)  nvi.pcdmach.channel_b);
  
        
        nvi.scdmach.nam = (byte) i;
        {
            boolean need_nv_save = FALSE;
            if ( NV_NOTACTIVE_S == SKY_GetNV( NV_SCDMACH_I, &nvi ) )
            {
                nvi.scdmach.nam = i;
                nvi.scdmach.channel_a = SCH_A_DEFAULT;
                nvi.scdmach.channel_b = SCH_B_DEFAULT;
                need_nv_save = TRUE;
            }

#ifdef FEATURE_SSPR_ENHANCEMENTS
            else 
            {
                if (!is_cellular_sys_a_channel(nvi.scdmach.channel_a)) 
                {
                    /* Channel A number is invalid, default it */
                    nvi.scdmach.channel_a = SCH_A_DEF;
                    need_nv_save = TRUE;
                }

                if (!is_cellular_sys_b_channel(nvi.scdmach.channel_b)) 
                {
                    /* Channel B number is invalid, default it */
                    nvi.scdmach.channel_b = SCH_B_DEF;
                    need_nv_save = TRUE;
                }
            }
#endif

// finish chos, remodified 'chos modification' by tasik

            if (need_nv_save) 
            {
                (void) SKY_PutNV( NV_SCDMACH_I, &nvi );
            }
        }

        if (HAS_ACDMA(i) == TRUE) 
        {
            SET_HAS_ACDMA(i, (BOOL) nvi.scdmach.channel_a);          
        }
        if (HAS_BCDMA(i) == TRUE) 
        {
            SET_HAS_BCDMA(i, (BOOL) nvi.scdmach.channel_b);          
        }
        SET_HAS_CDMA( i, HAS_ACDMA(i) | HAS_BCDMA(i) );

#endif /* FEATURE_CDMA_800 */
          
#if defined(FEATURE_SSPR_ENHANCEMENTS)

        {
            byte indx;

            /**********************************/
            /* Check for a valid SID/NID List */
            /**********************************/
            nvi.home_sid_nid.nam = (byte) i;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == SKY_GetNV( NV_HOME_SID_NID_I, &nvi ) )
            {
                for( indx=0; indx<NV_MAX_HOME_SID_NID; indx++)
                {
                    /*  NID default */
                    nvi.home_sid_nid.pair[indx].sid = 0;
                    nvi.home_sid_nid.pair[indx].nid = NID_DEFAULTS ;
                    (void) SKY_PutNV( NV_HOME_SID_NID_I, &nvi );
                }
            }
            else
            {
                for( indx=0; indx<NV_MAX_HOME_SID_NID; indx++)
                {
                    /* Is range valid ? */
                    if ( nvi.home_sid_nid.pair[indx].sid & 0x8000 )
                    {
                        /*  NID default */
                        nvi.home_sid_nid.pair[indx].sid = 0;
                        nvi.home_sid_nid.pair[indx].nid = NID_DEFAULTS ;
                        (void) SKY_PutNV( NV_HOME_SID_NID_I, &nvi );
                    }
                }
            }

#if (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) )
            /* SID/NID Lockout not allowed for TriMode */
#else
            
            /******************************************/
            /* Check for a valid Lockout SID/NID List */
            /******************************************/

            nvi.sid_nid_lock.nam = (byte) i;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == SKY_GetNV( NV_SID_NID_LOCK_I, &nvi ) )
            {
                for( indx=0; indx<NV_MAX_SID_NID_LOCK; indx++)
                {
                    /*  NID default */
                    nvi.sid_nid_lock.pair[indx].sid = 0;
                    nvi.sid_nid_lock.pair[indx].nid = NID_DEFAULTS ;
                    (void) SKY_PutNV( NV_SID_NID_LOCK_I, &nvi );
                }
            }
            else
            {
                for( indx=0; indx<NV_MAX_SID_NID_LOCK; indx++)
                {
                    /* Is range valid ? */
                    if ( nvi.sid_nid_lock.pair[indx].sid & 0x8000 )
                    {
                        /*  NID default */
                        nvi.sid_nid_lock.pair[indx].sid = 0;
                        nvi.sid_nid_lock.pair[indx].nid = NID_DEFAULTS ;
                        (void) SKY_PutNV( NV_SID_NID_LOCK_I, &nvi );
                    }
                }
            }
#endif /* TriMode */

        }
#else
        {
            int sid, issid;

            /*************************/
            /* Check for a valid SID */
            /*************************/
            nvi.sid_nid.nam = (byte) i;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == SKY_GetNV( NV_SID_NID_I, &nvi ) )
            {
                for( sid=0; sid<NV_MAX_SID_NID; sid++)
                {
                    /*  NID default */
                    nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].sid = 0;
                    nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].nid = NID_DEFAULTS ;
                    (void) SKY_PutNV( NV_SID_NID_I, &nvi );
                }
            }
            else
            {
                for( sid=0; sid<NV_MAX_SID_NID; sid++)
                {
                    /* Is range valid ? */
                    if ( nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][i].sid & 0x8000 )
                    {
                        /*  NID default */
                        nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].sid = 0;
                        nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].nid = NID_DEFAULTS ;
                        (void) SKY_PutNV( NV_SID_NID_I, &nvi );
                    }
                }
            }
            issid = 0;
            for( sid=0; sid<NV_MAX_SID_NID; sid++ ) 
            {
                issid |= (int)nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].sid;
            }
        }

#if defined(FEATURE_CDMA_800)
        /******************************************/
        /* Check for a valid Lockout SID List */
        /******************************************/

        {
            byte indx;

            nvi.cdma_sid_lock.nam = (byte) i;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == SKY_GetNV( NV_CDMA_SID_LOCK_I, &nvi ) )
            {
                for( indx=0; indx<NV_MAX_SID_LOCK; indx++)
                {
                    /*  default */
                    nvi.cdma_sid_lock.sid[indx] = 0;
                    (void) SKY_PutNV( NV_CDMA_SID_LOCK_I, &nvi );
                }
            }
            else
            {
                for( indx=0; indx<NV_MAX_SID_NID; indx++)
                {
                    /* Is range valid ? */
                    if ( nvi.cdma_sid_lock.sid[indx] & 0x8000 )
                    {
                        /*  default */
                        nvi.cdma_sid_lock.sid[indx] = 0;
                        (void) SKY_PutNV( NV_CDMA_SID_LOCK_I, &nvi );
                    }
                }
            }
        }

#endif /* FEATURE_CDMA_800 */

#endif /* FEATURE_SSPR_ENHANCEMENTS */

#ifdef FEATURE_SSPR_800
        /******************************************/
        /* Check for a valid PRL Enabled          */
        /******************************************/
        nvi.prl_enabled.nam = (byte) i;

#if (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) )
        nvi.prl_enabled.enabled = TRUE;
        (void) SKY_PutNV( NV_PRL_ENABLED_I, &nvi );
#else
        /* Is it active and in valid range */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_PRL_ENABLED_I, &nvi ) )
        {
            nvi.prl_enabled.enabled = FALSE;
            (void) SKY_PutNV( NV_PRL_ENABLED_I, &nvi );
        }
#endif /* TriMode */
#endif /* FEATURE_SSPR_800 */

#if (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) )
            /*--- It is done in ID_S minor state ---*/
#else
#if defined( FEATURE_ACP ) && defined( FEATURE_CDMA_800 )

        if ( HAS_CDMA(i) == FALSE ) 
        {        
            /*
            ** There is NO valid CDMA. In this case we default the mode
            ** to AUTOMATIC.  Note in this, phone goes offline.
            */
            ui_set_ph_mode_pref(CM_MODE_PREF_AUTOMATIC, 
                                CM_MODE_TERM_PERMANENT, 0);
        } 
        else 
        {
            /*
            ** There is a valid CDMA. In this case we default the mode to
            ** DIGITAL_ONLY if there is NO valid FM.
            */
            if( HAS_FM(i) == FALSE ) 
            {          
                ui_set_ph_mode_pref(CM_MODE_PREF_DIGITAL_ONLY, 
                                  CM_MODE_TERM_PERMANENT, 0);
            }
        } /* ui.hascdma[i] */


#ifndef FEATURE_SSPR_ENHANCEMENTS
        nvi.cdma_pref_serv.nam = i ;
        /* is it active and in valid range ? */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_CDMA_PREF_SERV_I, &nvi ) ||
           nvi.cdma_pref_serv.sys > NV_SYS_HOME_PREF )
        {
            /* no, so default it */
            nvi.cdma_pref_serv.nam = i ;
            nvi.cdma_pref_serv.sys = NV_SYS_HOME_PREF;
            (void) SKY_PutNV( NV_CDMA_PREF_SERV_I, &nvi );
        }

        nvi.analog_pref_serv.nam = i ;
        /* is it active and in valid range ? */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_ANALOG_PREF_SERV_I, &nvi ) ||
           nvi.analog_pref_serv.sys > NV_SYS_HOME_PREF )
        {
            /* no, so default it */
            nvi.analog_pref_serv.nam = i ;
            nvi.analog_pref_serv.sys = NV_SYS_HOME_PREF;
            (void) SKY_PutNV( NV_ANALOG_PREF_SERV_I, &nvi );
        }
#endif /* FEATURE_SSPR_ENHANCEMENTS */

#endif /* defined( FEATURE_ACP ) && defined( FEATURE_CDMA_800 ) */

#endif /* (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) ) */
          
        /**********************************************/
        /* Make sure MCC and IMSI_11_12 are set       */
        /**********************************************/
        nvi.imsi_mcc.nam = i;
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_MCC_I, &nvi ) )
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            if ( nvi.imsi_mcc.imsi_mcc > IMSI_MCC_0 )
            {
                initialize_imsi[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_mcc.imsi_mcc != IMSI_MCC_0 )
            {
                imsi_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_11_12.nam = i;
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_11_12_I, &nvi ) )
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            if ( nvi.imsi_11_12.imsi_11_12 > IMSI_11_12_0 )
            {
                initialize_imsi[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_11_12.imsi_11_12 != IMSI_11_12_0 )
            {
                imsi_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_addr_num.nam = i;
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_ADDR_NUM_I, &nvi ) )
        {
            initialize_imsi[i] = TRUE ;
        }

#ifdef FEATURE_IS95B_TRUE_IMSI

        /*  Add five TRUE NV items checking  
        **               IMSI_T_S1_I, 
        **               IMSI_T_S2_I, 
        **               IMSI_T_MCC_I,
        **               IMSI_T_11_12_I,
        **               IMSI_T_ADDR_NUM_I
        **                
        */

        SKY_TRACE( TL_MED, "Reading NV_IMSI_T_S1_I");

        /* Loop through the number of NAMs-1 and read the IMSI_T_S1 for each NAM */
        /* (NOTE:  We are looping to queue NV reads via uinv_access() with
        ** a NULL as the callback, then we queue the last NV read for the
        ** last NAM with this same routine (uicall_strtup_checking ()) as
        ** the callback */

        /*************************/
        /* Read the IMSI_T_S1 from NV */
        /*************************/

        /* Get and validate NV_IMSI_T_S1_I code goes here */


        nvi.imsi_t_s1.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_T_S1_I, &nvi ) )
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else 
        {
            /* Since it is valid for only CDMA, only CDMA MIN is checked. FMMIN is ignored */
            if ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0xFF000000 ) != 0 ) ||
               ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x00FFC000 ) >> 14 ) > 999 ) ||
               ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x00003C00 ) >> 10 ) > 10 ) ||
               ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x00003C00 ) >> 10 ) == 0 ) ||
               ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x000003FF ) > 999 ) ) )
            {
                /* Invalid Range See IS-95A section 6.3.1 */
                initialize_imsi_t[i] = TRUE ;
            }
            if ( nvi.imsi_t_s1.min1[CDMAMIN] != IMSI_S1_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }


        nvi.imsi_t_s2.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_T_S2_I, &nvi ) )
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else 
        {
            /* Since it is valid for only CDMA, only CDMA MIN is checked. FMMIN is ignored */
            if ( nvi.imsi_t_s2.min2[CDMAMIN] > IMSI_S2_0 )
            {
                /* invalid range */
                initialize_imsi_t[i] = TRUE ;
            }
            if ( nvi.imsi_t_s2.min2[CDMAMIN] != IMSI_S2_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }

  
        nvi.imsi_t_11_12.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_T_11_12_I, &nvi ) )
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else 
        {
            if ( nvi.imsi_t_11_12.imsi_11_12 > IMSI_11_12_0 )
            {
                initialize_imsi_t[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_t_11_12.imsi_11_12 != IMSI_11_12_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }

    
        nvi.imsi_t_mcc.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_T_MCC_I, &nvi ) )
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else 
        {
            if ( nvi.imsi_t_mcc.imsi_mcc > IMSI_MCC_0 )
            {
                initialize_imsi_t[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_t_mcc.imsi_mcc != IMSI_MCC_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }

      
        nvi.imsi_t_addr_num.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_IMSI_T_ADDR_NUM_I, &nvi ) )
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else if (!initialize_imsi_t[i])
        {
            /* Check if the IMSI_T_ADDR_NUM is invalid */
            if ((nvi.imsi_t_addr_num.num > 7) &&
                (nvi.imsi_t_addr_num.num != 0xFF))
            {
                SKY_TRACE(TL_HIGH,"Invalid IMSI_T_ADDR_NUM for NAM ");
       
                /* Set the flag that we need to default this NAM's IMSI data */
                initialize_imsi_t[i] = TRUE;
            }
        }
        else ;

#endif /*** End of FEATURE_IS95B_TRUE_IMSI ***/


        /**********************************/
        /* Read out the MINs for each NAM */
        /**********************************/
        nvi.min1.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_MIN1_I, &nvi ) )
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            /* Loop through the number of MINs */
            for (j = 0; j < NV_MAX_MINS; j++)
            {
                if ( ( ( nvi.min1.min1[j] & 0xFF000000 ) != 0 ) ||
                   ( ( ( nvi.min1.min1[j] & 0x00FFC000 ) >> 14 ) > 999 ) ||
                   ( ( ( nvi.min1.min1[j] & 0x00003C00 ) >> 10 ) > 10 ) ||
                   ( ( ( nvi.min1.min1[j] & 0x00003C00 ) >> 10 ) == 0 ) ||
                   ( ( ( nvi.min1.min1[j] & 0x000003FF ) > 999 ) ) )
                {
                    /* Invalid Range See IS-95A section 6.3.1 */
                    initialize_imsi[i] = TRUE ;
                }
                if ( nvi.min1.min1[j] != IMSI_S1_0 )
                {
                    imsi_all_zeros[i] = FALSE ;
                }
            } /* end for (j = 0; j < NV_MAX_MINS; j++) */
        }

        for( j=0; j<NV_MAX_MINS; j++ ) 
        {
            SET_MIN1( i, j, nvi.min1.min1[j] );                 
        }
        
        nvi.min2.nam = (byte) i;
        if ( NV_NOTACTIVE_S == SKY_GetNV( NV_MIN2_I, &nvi ) )
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            /* Loop through each MIN */
            for (j = 0; j < NV_MAX_MINS; j++)
            {
                if ( nvi.min2.min2[j] > IMSI_S2_0 )
                {
                    /* invalid range */
                    initialize_imsi[i] = TRUE ;
                }
                if ( nvi.min2.min2[j] != IMSI_S2_0 )
                {
                    imsi_all_zeros[i] = FALSE ;
                }
            } /* end for (j = 0; j < NV_MAX_MINS; j++) */
        }
        for( j=0; j<NV_MAX_MINS; j++ ) 
        {
            SET_MIN2(i, j, nvi.min2.min2[j] );                 
        }
        
    } /* for ( i=0; i<NV_MAX_NAMS; i++ ) */

    /*****************************************************************/
    /* By this point all imsi components are read from NV. The flags */
    /* are set indicating if any one of the components were inactive */
    /* or if the whole IMSI was all zeros. In either case, we must   */
    /* initialize all components of IMSI to zero, except the last    */
    /* four digits of MIN1 will be taken from the ESN                */
    /*****************************************************************/
    for ( i=0; i<NV_MAX_NAMS; i++ )
    {
        if ( initialize_imsi[i] == TRUE || imsi_all_zeros[i] == TRUE )
        {
            gInitializeIMSI(i);
        }
    }

#ifdef FEATURE_IS95B_TRUE_IMSI

    /* HHD check all items */
    for ( i=0; i<NV_MAX_NAMS; i++ )
    {
        if ( initialize_imsi_t[i] == TRUE || imsi_t_all_zeros[i] == TRUE )
        {
            gInitializeIMSIT(i);
            //ui_initialize_imsi_t_components ( i ) ;
        }
    }
#endif /* END of FEATURE_IS95B_TRUE_IMSI */

#ifdef FEATURE_CDMA_800
    /***********************************/
    /* Check if CDMA is valid or not ? */
    /***********************************/

    if( HAS_CDMA( SKY_GET_CURRENT_NAM() ) == FALSE )  
    {    
        goodnam = FALSE;
        SKY_TRACE( TL_HIGH, "Bad NAM!!!");
    }
#endif /* FEATURE_CDMA_800 */

    /***************************************************/
    /* Hardware has determined that we must go offline */
    /***************************************************/
    if( hw_force_offline() ) 
    {  /* hardware force offline */
        SKY_TRACE( TL_HIGH, "Hardware force offline is shorted (on)");
        good = FALSE;
    }

    /******************************************************************/
    /* Tell MC whether UI thinks we can go online or not based on the */
    /* state of the NAMs.                                             */
    /******************************************************************/
#ifdef WIN32
    /* these values are always TRUE in the PC simulator program */
    good = goodnam = TRUE;
#endif

    if (good && goodnam) 
    {
        ui_set_ph_oprt_mode(CM_OPRT_MODE_ONLINE);
    } 
    else 
    {
        ui_set_ph_oprt_mode(CM_OPRT_MODE_OFFLINE);
        SKY_SET_OFFLINE(TRUE);          
        //(void) uiscrn_act( UI_SERVICE_F );
    } 


#ifdef FEATURE_GVM
        /* GVM관련 mode setting */
        SKY_SET_IN_GVM(FALSE);
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_SOCK_CONNECT(FALSE);
#endif //FEATURE_GVM

    /**************************/
    /* Finish up initializing */
    /**************************/
    sInitStartup();         //final_init ();
    SKY_InitApplicationFiles();

        
        // 자동응답 및 녹음 재생에 사용됨
    /********************************/
    /* Initialize Voice Info        */
    /********************************/
        //SKY_SET_AUTO_ANSWER(FALSE);
    fSuccess = read_superblock();
    if(fSuccess != TRUE)
    {
      SKY_ASSERT(fSuccess == TRUE);
    }


    /*****************************/
    /* Time the power up display */
    /*****************************/
    
    id_is_timeout = FALSE;
#ifdef SW_RESET_ENABLE
    if(!mc_bSwReset)
#endif
    {
        /* to get contrast value is after initializing config */
        SKY_GetNV( NV_LCD_I, (nv_item_type *)&ContrastLevel); // happykid in 01.05.08
        //SKY_SetDisplayContrast((HS_CONTRAST_E)SKY_GET_CONTRAST(),ContrastLevel);
		SKY_SetDisplayContrast((HS_CONTRAST_E)SKY_GET_CONTRAST(), ContrastLevel);
        //    SKY_SetDisplayContrast((HS_CONTRAST_E)SKY_GET_IDCONTRAST(), ContrastLevel);  //white 01.05.08
    }
    SKY_GetNV( NV_BACK_LIGHT_I, (nv_item_type *)&SubContrastLevel); // happykid in 01.05.09
    SKY_SetSubDisplayContrast( (HS_CONTRAST_E)SKY_GET_IDCONTRAST() ,SubContrastLevel);
    //new_state = ID_S;

    SKY_GetNV( NV_AUTO_MUTE_I, (nv_item_type *)&IsApplyHighMelodyGain); // happykid in 01.05.14

#ifdef SW_RESET_ENABLE
    if(mc_bSwReset)
        SKY_RestoreSWResetData();
#endif 
    //SKY_SetCameraCount();           //white 2001.05.17
    g_fSubBackLightOn = FALSE; 
    g_fSubBackLightOff = FALSE;
    EQS_START_SIBLING(sSKA_WaitForTimeOutOrKeyPress, TRUE);


    /* NV_SPARE_1_I 값 검사 */
    SKY_GetNV(NV_SPARE_1_I, &nvi);
    if(nvi.spare_1 == SKY_NOVIRGIN_SYSTEM_FLAG)
        SKY_SET_VIRGIN_SYSTEM(FALSE);
    else /* SKY_VIRGIN_SYSTEM_FLAG or NON_ACTIVE nv item */
        SKY_SET_VIRGIN_SYSTEM(TRUE); 

    /* select service provider */
    sSetServiceProvider();

    /* set built in animation */
    SKY_SetBuiltInAnimation();

#if MODEL_ID == MODEL_B2		// by asdf 01.10.16 ui에서 읽도록 함 -> sky_lcd.c에서 사용
	{
	extern byte bPartial_Off_Flag;	//0 : rest이용 , 1:read이용 

	if(bPartial_Off_Flag == 0xff)	// 처음 들어온 경우 
		{
			if(NV_DONE_S != SKY_GetNV (NV_PA_RANGE_STEP_CAL_I, (nv_item_type *)&bPartial_Off_Flag))
				bPartial_Off_Flag = 0;
		}
	}
#endif

}

#define sSP_ID_LENGTH   3

LOCAL void SKY_API sSetServiceProvider(void)
{
    BYTE* pMyNumber = GetMyNumber();

    /* SK TELECOM */
    if(STRNCMP(pMyNumber, (BYTE *)SKY_SP_ID_SK_TELECOM, sSP_ID_LENGTH) == 0)
    {
        SKY_SET_SERVICE_PROVIDER(SP_SKT);
    }
    /* 신세기 */
    else if(STRNCMP(pMyNumber, (BYTE *)SKY_SP_ID_SHINSEGI, sSP_ID_LENGTH) == 0)
    {
        SKY_SET_SERVICE_PROVIDER(SP_STI);
    }
    /* 아직 MIN 입력되지 않았거나 잘못된 MIN */
    else
    {
        SKY_SET_SERVICE_PROVIDER(SP_NONE);
    }
}

VOID SKY_API SKA_MainStartUp(SKY_EVENT_T* pEvent)
{
    switch( pEvent->EventID )
    {

    case SE_APP_START:
        EQS_START_SIBLING(sSKA_Entry, TRUE);
        break;
    case SE_APP_END:
        break;
    default:
        break;
    }
}

VOID SKY_API SKA_ShutDown(SKY_EVENT_T* pEvent)
{
    HFILE hFile;
    BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 
    DWORD  dwTimeInfo = 0; 

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
        SKY_DEF_WIN_Y, 
        SKY_DEF_WIN_DX, 
        SKY_DEF_WIN_DY, 
        CL_NONE, 
        NULL, 
        WA_NONE);

        PBookClose();     
        UI_DISABLE_SLEEP(); // To prevent power-down animation slow down.

        if((SKY_GET_BELL_VIBRATION() != BV_VIBRATION && !SKY_IS_ETIQUETTE())
            || SKY_IS_EAR_JACK_ON() )
#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
            SKY_PlayMusicEx(MUS_PWR_OFF, 2, 1);
#else
            SKY_PlayMusicEx(MUS_DREAMEND_10, 2, 1);
#endif

        if(IS_MY_PICTURE(SKY_GET_POWER_OFF_PICTURE().wPictureID))    // 나만의 그림
        {
            if(SKY_OpenDataFile(SDF_IMAGE, SKY_GET_POWER_OFF_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
            {
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));
                
                if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
                {
                    SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                    SKY_CloseDataFile(hFile);
                    EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);
                
                    EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
                }
                else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
                {
                    ANIMATION_HEADER_T AniHeader;
                    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];
                
                    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                    SKY_CloseDataFile(hFile);
                    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_POWER_OFF_PICTURE().szFileName);
                    EQC_StartAnimation(EQC_Animation(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), NULL, abBuf), TRUE);
                }
            }
            else
                EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
        }
#ifdef FEATURE_CHARACBELL
        else if(IS_CHARAC_PICTURE(SKY_GET_POWER_OFF_PICTURE().wPictureID))    // 나만의 그림
        {
            if(SKY_OpenDataFile(SDF_CHA_IMAGE, SKY_GET_POWER_OFF_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
            {
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));
                
                if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
                {
                    SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                    SKY_CloseDataFile(hFile);
                    EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);
                
                    EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
                }
                else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
                {
                    ANIMATION_HEADER_T AniHeader;
                    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];
                
                    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                    SKY_CloseDataFile(hFile);
                    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_CHA_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_POWER_OFF_PICTURE().szFileName);
                    EQC_StartAnimation(EQC_Animation(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), NULL, abBuf), TRUE);
                }
            }
            else
                EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
        }
#endif // FEATURE_CHARACBELL
        else if(IS_MY_PHOTO(SKY_GET_POWER_OFF_PICTURE().wPictureID)) // 나만의 사진
        {
            if(SKY_OpenDataFile(SDF_PHOTO, SKY_GET_POWER_OFF_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
            {
                SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
                SKY_CloseDataFile(hFile); 

                l_BmpBuf.Header.yStart = 0;
                EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);
            }
            EQS_StartTimer(EQS_TIMER_1, sMAX_DISP_IMAGE_SEC, FALSE); 
        }
        else if(!IS_PICTURE_NOT_USED(SKY_GET_POWER_OFF_PICTURE().wPictureID))
        {
            /* 기본 제공 애니메이션은 0,0부터 시작 */
            EQC_StartAnimation(EQC_Animation(0, 0, g_pBuiltInAnimation[SKY_GET_POWER_OFF_PICTURE().wPictureID], NULL), TRUE);

        }
        else    //그림지정 안함
        {
            EQS_START_SIBLING(sSKA_ShutDown, FALSE);
        }
        break;

    case SE_IO_ANIM_TURN_AROUND:
        /* only one animation control in this applet
        ** only if TURN_AROUND event from this applet */
        if(pEvent->wParam != EQS_GetNthIOC(0))
            break;
        /* fall through */
    case SE_TM_TIMER1 :
        EQS_START_SIBLING(sSKA_ShutDown, FALSE);

        break;

    case SE_APP_END :   /* The very last event */ 
        break;

    default:
        break;
    }

}

VOID SKY_API SKA_ShutDownBadBattery(SKY_EVENT_T* pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
        SKY_DEF_WIN_Y, 
        SKY_DEF_WIN_DX, 
        SKY_DEF_WIN_DY, 
        CL_NONE, 
        NULL, 
        WA_NONE);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_POWER_DOWN_ON_BAD_BATT), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        break;

    case SE_IO_MESGBOX_END:
        EQS_START_SIBLING(sSKA_ShutDown, FALSE);
        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    default:
        break;
    }

}
