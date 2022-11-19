/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_hcbt.c
**
** Description: 
**     The core functions that provide EQS functionality.
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-11-08   jrkwon     re-created
** 00-10-12   redstar    created
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
#include "clk.h"
#include "msg.h"

#include "sky.h"

#ifdef FEATURE_WAP
#include "wapcommon.h"
#endif //#ifdef FEATURE_WAP
// tasik 00.09.04
#ifdef FEATURE_SMART_BATTERY
#include "bq2018.h"
#include "smb.h"
#include "smbdev.h"
#include "mccdma.h"

#define SMB_MAX_RAM_CHECK_COUNT		 30		// RAM 점검은 30분에 한번 한다.
//#define SMB_MAX_IDENTITY_CHECK_COUNT  1
#define SMB_BATT_NOT_EXIST_VAL_1	0xFF	// 배터리가 장착되지 않았을 때의 값
#define SMB_BATT_NOT_EXIST_VAL_2	0x00	// 배터리 RAM이 고장났을 때의 값
#define SMB_BATT_NOT_EXIST_VAL	0xFF	// 배터리가 장착되지 않았을 때의 값


extern void srand(unsigned int);
extern int rand(void);

//	happykid 00.12.28
//	이함수는 테스트상의 커브추적을 위한 함수로써 나중에 필요할 경우에만 다시 복원하기로 한다.
//extern void smb_batt_curve_estimate(void);
static byte smb_ram_check_count = SMB_MAX_RAM_CHECK_COUNT-1;
//static SmbRAMStatusType isValidRam;
static byte smb_random_time_count = 0;
static boolean SmbInitDoneAtBattChange = FALSE; // 배터리 교환때 초기화를 행했는가?
static boolean SmbEnterExtPwrRoutine = FALSE;   // external power routine을 거치고 나왔는가?

int SmbIdentityCheck;
//extern boolean smbIsBrokenDown;//01.03.30. youandi 링크에러 발생 smb.c
// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
// tasik 00.11.30
/*
	스마트 배터리 루틴이 1분에 한번씩 호출되기 위한 타이머 설정
*/
//clk_cb_type smb_cb;  

#endif
// tasik end

#ifdef FEATURE_SKVM
#include "ska_skvm.h"
#endif


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

typedef struct tagTIMER_T       TIMER_T;
typedef word                    TIMER_SIGNAL;

/*
**---------------------------------------------------------------------------
**  General Declarations
**---------------------------------------------------------------------------
*/
struct tagTIMER_T 
{
    TIMER_SIGNAL        Signal;
    CLOCK_CALLBACK      *pCallback;
    void SKY_API (*pfnCallback)(LONG lIntervalMS);
};
    

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
/* prevent from re-entrance into clear all beep */
BOOL g_fClearAllBeep = FALSE;


LOCAL BOOL SKY_API sSetTimer(EQS_TIMER_E TimerID, LONG lIntervalMS, BOOL fRepeat);
BOOL SKY_API (*g_pfnSKY_SetTimer)(EQS_TIMER_E TimerID, LONG lIntervalMS, 
                                  BOOL fRepeat) = sSetTimer;

#ifdef FEATURE_USE_IRDA
extern short			IrDA_AutoOffTimeOutCounter;
extern BOOL    g_fIrDAPortForPC;
#endif

#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.09
extern UINT8 g_nKeyFlag;
#endif  //FEATURE_SKT_EIF 

extern void smb_batt_curve_estimate(void);
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL TIMER_SIGNAL   l_TimerFlags;       /* timer flags */

LOCAL CLOCK_CALLBACK l_KeyDownCallback;  /* key down callback */
LOCAL CLOCK_CALLBACK l_TimerCallback;    /* general purpose ui callback */
LOCAL CLOCK_CALLBACK l_Timer1Callback;   /* equs general purpose ui callback */
LOCAL CLOCK_CALLBACK l_Timer2Callback;   /* equs general purpose ui callback */
LOCAL CLOCK_CALLBACK l_Timer3Callback;   /* equs general purpose ui callback */
LOCAL CLOCK_CALLBACK l_BacklightCallback;/* callback for backlighting */

//redstar 2000.12.27
LOCAL CLOCK_CALLBACK l_BellCallback;     /* for bell timer */ 
LOCAL SKY_EVENT_E    l_BellEvent;

LOCAL CLOCK_CALLBACK l_MoterCallback;     /* for moter on   */
LOCAL CLOCK_CALLBACK l_MoterOffCallback;  /* for moter off  */
LOCAL CLOCK_CALLBACK l_SoundStopCallback; /* for sound stop */
LOCAL CLOCK_CALLBACK l_LoactionCallback;  /* for location detection */
/* rosa 01-04-08 --> @why, what */
LOCAL CLOCK_CALLBACK l_GVMTimer1Callback; /* for GVM */
LOCAL CLOCK_CALLBACK l_GVMTimer2Callback; /* for GVM */
LOCAL CLOCK_CALLBACK l_GVMTimer3Callback; /* for GVM */
/* <-- */

#ifdef FEATURE_USE_IRDA
LOCAL CLOCK_CALLBACK l_IrDAStatusCheckCallback; /* for IrDA */
#endif
// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
//// happykid 00.12.28
//#ifdef FEATURE_SMART_BATTERY
//LOCAL CLOCK_CALLBACK l_SmbCallback;    /* for smart battery  */
//#endif
//// happykid end


// girin. 2000.12.12
// Clock call-back structure used to register the pacemaker routine
// to be called at regular intervals (CLOCK_PACEMAKER_REPEAT_TIME).
LOCAL CLOCK_CALLBACK l_PacemakerCallback;
LOCAL DWORD l_dwPacemakerCount = 0;

//LOCAL CLOCK_CALLBACK l_GlobalTimerCallback; // girin.

LOCAL BOOL    l_fIsBacklightActive; /* if callback is active */
LOCAL TIMER_T l_TimerSignal[MAX_TIMER];



/*
**---------------------------------------------------------------------------
**  Local Function Declaration
**---------------------------------------------------------------------------
*/

LOCAL void SKY_API sSetSignal(TIMER_SIGNAL* pBitMask, TIMER_SIGNAL OrMask);
LOCAL void SKY_API sClearSignal(TIMER_SIGNAL* pBitMask, TIMER_SIGNAL AndMask);

LOCAL void SKY_API sHandleKeyDownCallback(LONG lInterval);
LOCAL void SKY_API sHandleGeneralTimerCallback(LONG lInterval);
LOCAL void SKY_API sHandleGeneralTimer1Callback(LONG lInterval);
LOCAL void SKY_API sHandleGeneralTimer2Callback(LONG lInterval);
LOCAL void SKY_API sHandleGeneralTimer3Callback(LONG lInterval);

LOCAL void SKY_API sBacklightCallback(LONG lIntervalMS);
LOCAL BOOL SKY_API sIsBacklightOnTime(void);
LOCAL BOOL SKY_API sSetTimer(EQS_TIMER_E TimerID, LONG lIntervalMS, BOOL fRepeat);

LOCAL void SKY_API sHandleLocationCallback(LONG lInterval); //redstar 2001.03.16
/* rosa 01-04-08 --> @why, what */
LOCAL void SKY_API sHandleGVMTimer1Callback(LONG lInterval);
LOCAL void SKY_API sHandleGVMTimer2Callback(LONG lInterval);
LOCAL void SKY_API sHandleGVMTimer3Callback(LONG lInterval);
/* <-- */


/*
**---------------------------------------------------------------------------
**  Local Function Definitions
**---------------------------------------------------------------------------
*/

/*===========================================================================
FUNCTION sSEtSignal

DESCRIPTION
Sets a given signal to a bitmask.

===========================================================================*/
LOCAL void SKY_API sSetSignal(TIMER_SIGNAL* pBitMask, TIMER_SIGNAL OrMask)
{
    INTLOCK();
    *pBitMask |= OrMask;
    INTFREE();
}


/*===========================================================================
FUNCTION sClearSignal

DESCRIPTION
Clears a given signal to a bitmask.

===========================================================================*/
LOCAL void SKY_API sClearSignal(TIMER_SIGNAL* pBitMask, TIMER_SIGNAL AndMask)
{
    INTLOCK();
    *pBitMask &= ~AndMask;
    INTFREE();
}


/*===========================================================================
FUNCTION sHandleKeyDownCallback

DESCRIPTION
  Handle key down callback

DEPENDENCIES
  Should only be called from clock callback

SIDE EFFECTS
  sets UI_TIMERS_SIG
===========================================================================*/
LOCAL void SKY_API sHandleKeyDownCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal(&l_TimerFlags, SKY_KEY_TIMER_SIG);
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}

/*===========================================================================
FUNCTION sHandleGeneralTimerCallback

DESCRIPTION
  Handle general timer callback

DEPENDENCIES
  Should only be called from clock callback

SIDE EFFECTS
  sets UI_TIMERS_SIG
===========================================================================*/

LOCAL void SKY_API sHandleGeneralTimerCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal (&l_TimerFlags, SKY_GEN_TIMER_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}


LOCAL void SKY_API sHandleGeneralTimer1Callback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal (&l_TimerFlags, SKY_EQS_TIMER1_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}

LOCAL void SKY_API sHandleGeneralTimer2Callback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal (&l_TimerFlags, SKY_EQS_TIMER2_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}


LOCAL void SKY_API sHandleGeneralTimer3Callback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal (&l_TimerFlags, SKY_EQS_TIMER3_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}

LOCAL void SKY_API sPacemakerCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    l_dwPacemakerCount++;
	sSetSignal (&l_TimerFlags, SKY_PACEMAKER_TIMER_SIG); 		
	(void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}


//LOCAL void SKY_API sGlobalTimerCallback(LONG lInterval)
//{
//    UNUSED(lInterval); /* not used but I hate a warning */
//
//    sSetSignal (&l_TimerFlags, SKY_GLOBAL_TIMER_SIG); 
//    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
//}

// redstar   2000.12.27
LOCAL void SKY_API sBellCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */
 
    sSetSignal (&l_TimerFlags, SKY_BELL_TIMER_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}

LOCAL void SKY_API sMoterCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */
 
    sSetSignal (&l_TimerFlags, SKY_MOTER_TIMER_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */

}
LOCAL void SKY_API sMoterOffCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */
 
    sSetSignal (&l_TimerFlags, SKY_MOTER_OFF_TIMER_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */

}
LOCAL void SKY_API sSoundStopCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */
 
    sSetSignal (&l_TimerFlags, SKY_SOUND_STOP_TIMER_SIG); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */

}

LOCAL void SKY_API sHandleLocationCallback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal(&l_TimerFlags, SKY_LOCATION_TIMER_SIG);
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}


#ifdef FEATURE_USE_IRDA
LOCAL void SKY_API sIrDAStatusCheck_cb (LONG  lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */
 
    sSetSignal (&l_TimerFlags, SKY_IRDA_STATUS_CHECK_TIMER); 
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}
#endif

/* rosa 01-04-08 --> @why, what */
LOCAL void SKY_API sHandleGVMTimer1Callback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal(&l_TimerFlags, SKY_GVM_TIMER1_SIG);
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}

LOCAL void SKY_API sHandleGVMTimer2Callback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal(&l_TimerFlags, SKY_GVM_TIMER2_SIG);
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}

LOCAL void SKY_API sHandleGVMTimer3Callback(LONG lInterval)
{
    UNUSED(lInterval); /* not used but I hate a warning */

    sSetSignal(&l_TimerFlags, SKY_GVM_TIMER3_SIG);
    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
}
/* <-- */

// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
//// happykid 00.12.28
//#ifdef FEATURE_SMART_BATTERY
//
//LOCAL void SKY_API sSmartBatteryCallback(LONG lInterval)
//{
//    UNUSED(lInterval); /* not used but I hate a warning */
// 
//    sSetSignal (&l_TimerFlags, SKY_SMB_TIMER_SIG); 
//    (void) SKY_Set_Signal( &ui_tcb, UI_TIMERS_SIG ); /* kick me! */
//
//}
//#endif //FEATURE_SMART_BATTERY
/*===========================================================================
FUNCTION sBacklightCallback

DESCRIPTION
  The callback for backlighing.  Just turn it off.

DEPENDENCIES
  Should only be set as a callback

RETURN VALUE
  Nothing

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void SKY_API sBacklightCallback(LONG lIntervalMS)
{
    if(SKY_IS_HANDS_FREE())
        return;

    UNUSED(lIntervalMS);

    SKY_BacklightSensor(OFF);
#ifdef FEATURE_SKY_SUBLCD
#ifdef FEATURE_COLOR_ID_LCD
    SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
    SKY_SubBacklight(OFF);
#endif
#endif//#ifdef FEATURE_SKY_SUBLCD
    l_fIsBacklightActive = FALSE;
}

LOCAL BOOL SKY_API sIsBacklightOnTime(void)
{
    static clk_julian_type tm;
    DWORD dwCurrentTime;
    BOOL fReturn;

    dwCurrentTime = SKY_GetLocalTime();
    
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian( dwCurrentTime, &tm );

        dwCurrentTime = tm.hour * 60 * 60;
        dwCurrentTime += tm.minute * 60;

        if(SKY_GET_AUTO_BACKLIGHT_END_TIME() > SKY_GET_AUTO_BACKLIGHT_START_TIME())
        {
            fReturn = (dwCurrentTime >= SKY_GET_AUTO_BACKLIGHT_START_TIME())
                      && (dwCurrentTime <= SKY_GET_AUTO_BACKLIGHT_END_TIME());
        }
        else /* GET_AUTO_BACKLIGHT_TIME().dwEndTime 
                <= GET_AUTO_BACKLIGHT_TIME().dwStartTime */
        {
            fReturn = (dwCurrentTime >= SKY_GET_AUTO_BACKLIGHT_START_TIME())
                      || (dwCurrentTime <= SKY_GET_AUTO_BACKLIGHT_END_TIME());
        }
    }        
    else
        fReturn = FALSE;

    return fReturn;
}

#ifdef FEATURE_SKY_SUBLCD
BOOL SKY_API SKY_IsSubBacklightOnTime(void)
{
    static clk_julian_type tm;
    DWORD dwCurrentTime;
    BOOL fReturn;

    dwCurrentTime = SKY_GetLocalTime();
    
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian( dwCurrentTime, &tm );

        dwCurrentTime = tm.hour * 60 * 60;
        dwCurrentTime += tm.minute * 60;

        if(SKY_GET_AUTO_SUBBACKLIGHT_END_TIME() > SKY_GET_AUTO_SUBBACKLIGHT_START_TIME())
        {
            fReturn = (dwCurrentTime >= SKY_GET_AUTO_SUBBACKLIGHT_START_TIME())
                      && (dwCurrentTime <= SKY_GET_AUTO_SUBBACKLIGHT_END_TIME());
        }
        else /* GET_AUTO_BACKLIGHT_TIME().dwEndTime 
                <= GET_AUTO_BACKLIGHT_TIME().dwStartTime */
        {
            fReturn = (dwCurrentTime >= SKY_GET_AUTO_SUBBACKLIGHT_START_TIME())
                      || (dwCurrentTime <= SKY_GET_AUTO_SUBBACKLIGHT_END_TIME());
        }
    }        
    else
        fReturn = FALSE;

    return fReturn;
}
#endif

DWORD SKY_GetPacemakerCount(void)
{
    return l_dwPacemakerCount;
}


/*
**---------------------------------------------------------------------------
**  Global Function Definitions
**---------------------------------------------------------------------------
*/
// this function must be only used in eqs_aapi.c  
void SKY_API SKY_SetPaceMakerTimer(LONG lIntervalMS, BOOL fRepeat)
{
#if 0







        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_PacemakerCallback);
            /* clear bell timer */
            sClearSignal (&l_TimerFlags, SKY_PACEMAKER_TIMER_SIG);
        } 
        else 
        {
			SKY_Set_Clock(
							SKY_PACEMAKER_TIMER_SIG,
							&l_PacemakerCallback,
							sPacemakerCallback,
							lIntervalMS, (fRepeat) ? lIntervalMS : 0, fRepeat );
        }
#endif
}

#ifdef FEATURE_SKVM
void SKY_API SKY_SetFromSKVMPaceMakerTimer(void)
{
    clk_reg(&l_PacemakerCallback, sPacemakerCallback, 1000, 1000, TRUE);
}

void SKY_API SKY_DisablePacemakerCallback()
{
	clk_dereg(&l_PacemakerCallback);
}
#endif 



/*===========================================================================
FUNCTION SKY_SetTimer

DESCRIPTION
  Set up for general purpose callback

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
LOCAL BOOL SKY_API sSetTimer(EQS_TIMER_E TimerID, LONG lIntervalMS, BOOL fRepeat)
{
    
    if( TimerID < MAX_TIMER )
    {
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(l_TimerSignal[TimerID].pCallback);
            /* clear general timer */
            sClearSignal (&l_TimerFlags, (word) l_TimerSignal[TimerID].Signal);
//            dwSleepFlag &= ~l_TimerSignal[TimerID].Signal;  /* ok to sleep */            
        } 
        else 
        {
            SKY_Set_Clock( l_TimerSignal[TimerID].Signal,
                           l_TimerSignal[TimerID].pCallback, 
                           l_TimerSignal[TimerID].pfnCallback,
                           lIntervalMS, (fRepeat) ? lIntervalMS : 0, fRepeat );

//            dwSleepFlag |= l_TimerSignal[TimerID].Signal;   /* not ok to sleep */
/*
            clk_reg(l_TimerSignal[TimerID].pCallback, 
                    l_TimerSignal[TimerID].pfnCallback,
                    lIntervalMS, (fRepeat) ? lIntervalMS : 0, TRUE );
*/
        }
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}


BOOL SKY_API SKY_SetGeneralTimer(LONG lIntervalMS, BOOL fRepeat)
{
    return sSetTimer(EQS_GENERAL_TIMER, lIntervalMS, fRepeat);
}


void SKY_API SKY_SetBellTimer(LONG lIntervalMS, BOOL fRepeat, SKY_EVENT_E Event)
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_BellCallback);
            /* clear bell timer */
            sClearSignal (&l_TimerFlags, SKY_BELL_TIMER_SIG);
        } 
        else 
        {
            SKY_Set_Clock( SKY_BELL_TIMER_SIG,
                           &l_BellCallback, 
                           sBellCallback,
                           lIntervalMS, (fRepeat) ? lIntervalMS : 0, fRepeat );

            l_BellEvent = Event;

/*
            clk_reg(l_BellCallback, 
                    sBellCallback,
                    lIntervalMS, (fRepeat) ? lIntervalMS : 0, TRUE );
*/
        }
}
void SKY_API SKY_SetMotorOnTimer(LONG lIntervalMS)     //redstar  00.12.29  for moter on-off
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_MoterCallback);
            /* clear moter off timer */
            sClearSignal (&l_TimerFlags, SKY_MOTER_TIMER_SIG);
        } 
        else 
        {
            SKY_SetMotorOffTimer(1500);

            SKY_Set_Clock( SKY_MOTER_TIMER_SIG,
                           &l_MoterCallback, 
                           sMoterCallback,
                           lIntervalMS, 3000, TRUE );

        }

}

void SKY_API SKY_SetMotorOffTimer(LONG lIntervalMS)   //redstar  00.12.29  for moter off   
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_MoterOffCallback);
            /* clear bell timer */
            sClearSignal (&l_TimerFlags, SKY_MOTER_OFF_TIMER_SIG);
        } 
        else 
        {
            SKY_Set_Clock( SKY_MOTER_OFF_TIMER_SIG,
                           &l_MoterOffCallback, 
                           sMoterOffCallback,
                           lIntervalMS, 0, FALSE );         
        }

}

void SKY_API SKY_SetSoundStopTimer(LONG lIntervalMS)     //redstar  00.12.29  for Sound Stop
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_SoundStopCallback);
            /* clear sound stop timer */
            sClearSignal (&l_TimerFlags, SKY_SOUND_STOP_TIMER_SIG);
        } 
        else 
        {
            SKY_Set_Clock( SKY_SOUND_STOP_TIMER_SIG,
                           &l_SoundStopCallback, 
                           sSoundStopCallback,
                           lIntervalMS, 0, FALSE );         
        }

}

void SKY_API SKY_SetLocationTimer(LONG lIntervalMS, BOOL fRepeat)
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_LoactionCallback);
            /* clear bell timer */
            sClearSignal (&l_TimerFlags, SKY_LOCATION_TIMER_SIG);
        } 
        else 
        {
            SKY_Set_Clock( SKY_LOCATION_TIMER_SIG,
                           &l_LoactionCallback,
                           sHandleLocationCallback,
                           lIntervalMS, (fRepeat) ? lIntervalMS : 0, fRepeat );
        }    
}

#ifdef FEATURE_USE_IRDA
void SKY_API SKY_IrDAStatusCheck_set (LONG lIntervalMS)
{
        if( lIntervalMS == 0 ) 
        {      
            clk_dereg( &l_IrDAStatusCheckCallback );
            sClearSignal (&l_TimerFlags, SKY_IRDA_STATUS_CHECK_TIMER);
        } 
        else 
        {
            SKY_Set_Clock( SKY_IRDA_STATUS_CHECK_TIMER,
                           &l_IrDAStatusCheckCallback, 
                           sIrDAStatusCheck_cb,
                           lIntervalMS, 3000L, TRUE );         
        }
}
#endif // FEATURE_USE_IRDA

/* rosa 01-04-08 --> @why, what */
void SKY_API SKY_SetGVMTimer1(LONG lIntervalMS, BOOL fRepeat)
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_GVMTimer1Callback);
            /* clear bell timer */
            sClearSignal (&l_TimerFlags, SKY_GVM_TIMER1_SIG);
        } 
        else 
        {
            SKY_Set_Clock( SKY_GVM_TIMER1_SIG,
                           &l_GVMTimer1Callback,
                           sHandleGVMTimer1Callback,
                           lIntervalMS, (fRepeat) ? lIntervalMS : 0, fRepeat );
        }    
}

void SKY_API SKY_SetGVMTimer2(LONG lIntervalMS, BOOL fRepeat)
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_GVMTimer2Callback);
            /* clear bell timer */
            sClearSignal (&l_TimerFlags, SKY_GVM_TIMER2_SIG);
        } 
        else 
        {
            SKY_Set_Clock( SKY_GVM_TIMER2_SIG,
                           &l_GVMTimer2Callback,
                           sHandleGVMTimer2Callback,
                           lIntervalMS, (fRepeat) ? lIntervalMS : 0, fRepeat );
        }    
}

void SKY_API SKY_SetGVMTimer3(LONG lIntervalMS, BOOL fRepeat)
{
        /* if lInterval is 0, clear general timer */
        if(lIntervalMS == 0 ) 
        {
            clk_dereg(&l_GVMTimer3Callback);
            /* clear bell timer */
            sClearSignal (&l_TimerFlags, SKY_GVM_TIMER3_SIG);
        } 
        else 
        {
            SKY_Set_Clock( SKY_GVM_TIMER3_SIG,
                           &l_GVMTimer3Callback,
                           sHandleGVMTimer3Callback,
                           lIntervalMS, (fRepeat) ? lIntervalMS : 0, fRepeat );
        }    
}
/* <-- */

// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
//// happykid 00.12.28
//#ifdef FEATURE_SMART_BATTERY
//
//void SKY_API SKY_SetSmartBatteryTimer(BOOL OnOff)
//{
//        /* if lInterval is 0, clear general timer */
//        if(OnOff) 
//        {
//            SKY_Set_Clock( SKY_SMB_TIMER_SIG,
//                           &l_SmbCallback, 
//                           sSmartBatteryCallback,
//                           60000L, 60000L, TRUE );         
//        } 
//        else 
//        {
//            clk_dereg(&l_SmbCallback);
//            /* clear bell timer */
//            sClearSignal (&l_TimerFlags, SKY_SMB_TIMER_SIG);
//        }
//
//}
//#endif //FEATURE_SMART_BATTERY

/*===========================================================================
FUNCTION SKY_BacklightOn

DESCRIPTION
  Handle backlight off timing

DEPENDENCIES
  ui.backval should be set correctly

RETURN VALUE
  Nothing

SIDE EFFECTS
  Sets up a clock callback

===========================================================================*/

void SKY_API SKY_BacklightOn(AUTO_BACKLIGHT_E AutoBacklight)
{
    if(SKY_IS_HANDS_FREE())
    {
        if(SKY_IS_PHONE_OPEN())
        {
            SKY_Backlight(ON);
        
            if( l_fIsBacklightActive ) 
            {       /* if already on, deregister */
                clk_dereg( &l_BacklightCallback );
                l_fIsBacklightActive = FALSE;
            }
        }
        return;
    }
    
    /* if power is being down, turn backlight off */
    if(SKY_IS_SHUT_DOWN())
    {
        SKY_BacklightOff();
    }

    /* 조명을 끄지 않음이 셋팅되어 있으면 항상 켜놓는다.*/
    if(SKY_GET_NO_OFF_BACKLIGHT())
    {
        if(SKY_IS_PHONE_OPEN())
        {
            SKY_Backlight(ON);
        
            if( l_fIsBacklightActive ) 
            {       /* if already on, deregister */
                clk_dereg( &l_BacklightCallback );
                l_fIsBacklightActive = FALSE;
            }
        }
        return;
    }
    /* 항상 꺼져 있음 */
    if(AutoBacklight == AB_NO) 
    {
        SKY_BacklightOff();
        return;
    }

    /* if power is being up or the external power is attached */
    if(!SKY_IS_SHUT_DOWN() && SKY_IS_EXTERNAL_POWER())      //white
    {
        if(SKY_IS_PHONE_OPEN())
                ;//            SKY_Backlight( TRUE ); /* only backlight if power on */
#ifdef FEATURE_SKY_SUBLCD
//        SKY_SubBacklight(TRUE);
#endif /* #ifdef FEATURE_SKY_SUBLCD */
    }

    /* 시간 지정 자동 조명*/
    if(AutoBacklight == AB_TIME)
    {
        if(SKY_IS_PHONE_OPEN())
        {
            if(sIsBacklightOnTime())
            {
                SKY_Backlight( ON );
    #ifdef FEATURE_SKY_SUBLCD
    //            SKY_SubBacklight(ON);
    #endif /* #ifdef FEATURE_SKY_SUBLCD */
            clk_reg(&l_BacklightCallback, sBacklightCallback,
                    1000*(int4)(AUTO_BACKLIGHT_TIME), (int4)0, FALSE );
            l_fIsBacklightActive = TRUE;
                                 //white 시간 지정에서도 꺼져야 한다.     
            }else
                SKY_BacklightOff();
        }
        else
        {
            SKY_Backlight( OFF );
#ifdef FEATURE_SKY_SUBLCD
//            SKY_SubBacklight(OFF);
#endif /* #ifdef FEATURE_SKY_SUBLCD */
        }
    }
    /*자동 조명 - 10초 후 꺼짐 */
    else if(AutoBacklight == AB_10SECOND) 
    {
        if(SKY_IS_PHONE_OPEN())
        {
            SKY_Backlight(ON);
#ifdef FEATURE_SKY_SUBLCD
    //        SKY_SubBacklight(ON);
#endif /* #ifdef FEATURE_SKY_SUBLCD */       
            clk_reg(&l_BacklightCallback, sBacklightCallback, 
                    1000*(int4)(AUTO_BACKLIGHT_TIME), (int4)0, FALSE );
            l_fIsBacklightActive = TRUE;
        }
    }
    /* 항상 켜져 있음 */
    else if(AutoBacklight == AB_ALWAYS)
    {
        if(SKY_IS_PHONE_OPEN())
        {
            SKY_Backlight(ON);
        
            if( l_fIsBacklightActive ) 
            {       /* if already on, deregister */
                clk_dereg( &l_BacklightCallback );
                l_fIsBacklightActive = FALSE;
            }
        }
    }
}

#ifdef FEATURE_SKVM        /* tortol for time 
/*===========================================================================
FUNCTION SKY_BacklightPeriodOn

DESCRIPTION
  Handle backlight off during given period 

DEPENDENCIES
  ui.backval should be set correctly

RETURN VALUE
  Nothing

SIDE EFFECTS
  Sets up a clock callback

===========================================================================*/

void SKY_API SKY_BacklightPeriodOn(int duration)
{
    {
		SKY_Backlight(ON);
#ifdef FEATURE_SKY_SUBLCD
//        SKY_SubBacklight(ON);
#endif /* #ifdef FEATURE_SKY_SUBLCD */       
		clk_reg(&l_BacklightCallback, sBacklightCallback, 
			(int4)(duration), (int4)0, FALSE );
		l_fIsBacklightActive = TRUE;
    }
}
#endif

void SKY_API SKY_BacklightOff(void)
{
    /* if phone is placed in hands free, do not turn backlight off */
/*
    if(SKY_IS_HANDS_FREE())
        return;
*/

    /* turn backlight off */
    SKY_Backlight( OFF );
#ifdef FEATURE_SKY_SUBLCD
//    SKY_SubBacklight(OFF);
#endif /* #ifdef FEATURE_SKY_SUBLCD */

    /* if already on, deregister */
    if( l_fIsBacklightActive ) 
    {
        clk_dereg( &l_BacklightCallback );
        l_fIsBacklightActive = FALSE;
    }
}

#ifdef FEATURE_SKY_SUBLCD
void SKY_API SKY_SubBacklightOn(AUTO_BACKLIGHT_E AutoBacklight)     //white
{
    /* if power is being down, turn backlight off */
    if(SKY_IS_SHUT_DOWN())
    {
        SKY_SubBacklightOff();
    }

    if(!SKY_IS_SHUT_DOWN() && SKY_IS_HANDS_FREE())
    {
        if(!SKY_IS_PHONE_OPEN())
        {
#ifdef FEATURE_COLOR_ID_LCD
            SKY_SubBacklight(TRUE, SKY_GET_IDLCD_COLOR_IDLE());
#else
            SKY_SubBacklight(TRUE);
#endif
        }else
        {
#ifdef FEATURE_COLOR_ID_LCD
            SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
            SKY_SubBacklight(OFF);
#endif
        }
        return;
    }

    /* 항상 꺼져 있음 */
    if(AutoBacklight == AB_NO) 
    {
#ifdef FEATURE_COLOR_ID_LCD
        if(!SKY_GET_NO_OFF_BACKLIGHT())
        {
            SKY_SubBacklight(OFF, NONE_LCD_COLOR);
        }
#else
        if(!SKY_GET_NO_OFF_BACKLIGHT())
        {
            SKY_SubBacklight(OFF);
        }
#endif
        return;
    }

    /* if power is being up or the external power is attached */
/*    
    if(!SKY_IS_SHUT_DOWN() && SKY_IS_EXTERNAL_POWER())              //white
    {
        if(!SKY_IS_PHONE_OPEN())
        {
#ifdef FEATURE_COLOR_ID_LCD
            SKY_SubBacklight(TRUE, SKY_GET_IDLCD_COLOR_IDLE());
#else
            SKY_SubBacklight(TRUE);
#endif
        }else
        {
#ifdef FEATURE_COLOR_ID_LCD
            SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
            SKY_SubBacklight(OFF);
#endif
        }
        clk_reg(&l_BacklightCallback, sBacklightCallback, 
                1000*(int4)(AUTO_BACKLIGHT_TIME), (int4)0, FALSE );
        l_fIsBacklightActive = TRUE;
    }
*/
    /* 시간 지정 항상 켜짐 */
    if(AutoBacklight == AB_TIME)
    {
        if(SKY_IsSubBacklightOnTime())
        {
            if(!SKY_IS_PHONE_OPEN())
            {
#ifdef FEATURE_COLOR_ID_LCD
                SKY_SubBacklight(TRUE, SKY_GET_IDLCD_COLOR_IDLE());
#else
                SKY_SubBacklight(TRUE);
#endif
            }else
            {
#ifdef FEATURE_COLOR_ID_LCD
                SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
                SKY_SubBacklight(OFF);
#endif
            }

            if( l_fIsBacklightActive ) 
            {       
                clk_dereg( &l_BacklightCallback );
                l_fIsBacklightActive = FALSE;
            }
        }
        else                //if(SKY_IsSubBacklightOnTime())
        {
            if(!SKY_IS_PHONE_OPEN())
            {
#ifdef FEATURE_COLOR_ID_LCD
                if(!SKY_GET_NO_OFF_BACKLIGHT())
                {
                    SKY_SubBacklight(OFF, NONE_LCD_COLOR);
                }
#else
                if(!SKY_GET_NO_OFF_BACKLIGHT())
                {
                    SKY_SubBacklight(OFF);
                }
#endif
            }else
            {
#ifdef FEATURE_COLOR_ID_LCD
                SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
                SKY_SubBacklight(OFF);
#endif
            }
            clk_reg(&l_BacklightCallback, sBacklightCallback, 
                    1000*(int4)(AUTO_BACKLIGHT_TIME), (int4)0, FALSE );
            l_fIsBacklightActive = TRUE;
        }            
    }
    /* 자동 조명 : 폴더 닫혀 있을때 10초간 켜짐 */
    else if(AutoBacklight == AB_10SECOND) 
    {
            if(!SKY_IS_PHONE_OPEN())
            {
#ifdef FEATURE_COLOR_ID_LCD
                SKY_SubBacklight(TRUE, SKY_GET_IDLCD_COLOR_IDLE());
#else
                SKY_SubBacklight(TRUE);
#endif
            }else
            {
#ifdef FEATURE_COLOR_ID_LCD
                SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
                SKY_SubBacklight(OFF);
#endif
            }        
        clk_reg(&l_BacklightCallback, sBacklightCallback, 
                1000*(int4)(AUTO_BACKLIGHT_TIME), (int4)0, FALSE );
        l_fIsBacklightActive = TRUE;
    }
    /* 항상 켜져 있음 */
    else if(AutoBacklight == AB_ALWAYS)
    {
            if(!SKY_IS_PHONE_OPEN())
            {
#ifdef FEATURE_COLOR_ID_LCD
                SKY_SubBacklight(TRUE, SKY_GET_IDLCD_COLOR_IDLE());
#else
                SKY_SubBacklight(TRUE);
#endif
            }else
            {
#ifdef FEATURE_COLOR_ID_LCD
                SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
                SKY_SubBacklight(OFF);
#endif
            }

        if( l_fIsBacklightActive ) 
        {       
            clk_dereg( &l_BacklightCallback );
            l_fIsBacklightActive = FALSE;
        }

    }
}

void SKY_API SKY_SubBacklightOff(void)                          //white
{
    /* if phone is placed in hands free, do not turn backlight off */
/*
    if(SKY_IS_HANDS_FREE())
        return;
*/

    /* turn backlight off */
#ifdef FEATURE_COLOR_ID_LCD
    SKY_SubBacklight(OFF, NONE_LCD_COLOR);
#else
    SKY_SubBacklight(OFF);
#endif

    /* if already on, deregister */
    if( l_fIsBacklightActive ) 
    {
        clk_dereg( &l_BacklightCallback );
        l_fIsBacklightActive = FALSE;
    }
}
#endif /* #ifdef FEATURE_SKY_SUBLCD */

/*===========================================================================
FUNCTION Sky_SetupBacklight                  //ui_backlight_which

DESCRIPTION
  Choose either ui.backlight1 or ui.backlight2 to be our backlight value
  depending on whether we are in the portable car kit.  Called when
  backlight setting changed (e.g. menus, or entered hands free mode).

DEPENDENCIES
  ui.backlight1, ui.backlight2, ui.onhook

RETURN VALUE
  Nothing

SIDE EFFECTS
  Changes current backlighting, and ui.backval

===========================================================================*/
void SKY_API SKY_SetupBacklight(void)
{
    /* if extenal power is attached */
    if(SKY_IS_EXTERNAL_POWER())
    {
        SKY_BacklightOn(AB_ALWAYS);
        //SKY_SET_AUTO_BACKLIGHT(AB_ALWAYS);
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubBacklightOn(AB_ALWAYS);
        //SKY_SET_AUTO_SUBBACKLIGHT(AB_ALWAYS);
#endif
        return;
    }

    if((SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS)              //Main LCD
       || (SKY_GET_AUTO_BACKLIGHT() == AB_10SECOND))
    {
        /* the flip or folder is open, so turn backlight on */
        if(SKY_IS_PHONE_OPEN()) 
        {
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        }
        /* the flip or folder is close, so turn backlight off */
        else 
        {
            SKY_BacklightOff();
        }
    }
    else
    {
        /* backlight always off */
        if(SKY_GET_AUTO_BACKLIGHT() == AB_NO )
        {
            if(!SKY_IS_HANDS_FREE() && !SKY_GET_NO_OFF_BACKLIGHT())
                SKY_BacklightOff();
            
            if(SKY_GET_NO_OFF_BACKLIGHT())
                SKY_BacklightOn(AB_ALWAYS);
        }
        /* backlight always on */
/*                          //white 여기에 있을 필요 없음.
        else if(SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS)
        {
            SKY_BacklightOn(AB_ALWAYS);
        }
*/
        /* timed backlight */
        else 
        {
            SKY_BacklightOn(AB_TIME);
        }    
    }
#ifdef FEATURE_SKY_SUBLCD                               //white 2001.01.04
    if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_NO)            //사용안함
    {   
        if(!SKY_IS_HANDS_FREE())
        {
            SKY_SubBacklightOff();                                 
            
            if(SKY_GET_NO_OFF_BACKLIGHT() && !SKY_IS_PHONE_OPEN())
            {
                SKY_SubBacklightOn(AB_ALWAYS);
            }
        }
    }
    else if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_10SECOND)   // 자동조명
    {
        if(!SKY_IS_PHONE_OPEN())
            SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
        else
            SKY_SubBacklightOff();
        
    }
    else                                   //시간 지정 켜짐
    {
        SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
        /* 시간 지정 켜짐의 경우에 backlight를 끄는 경우가  */
        /* 발생하므로 flag를 검사해서 다시 켜줄 필요가 있을 */
        /* 경우에는 다시 켜준다                             */
        if(SKY_GET_NO_OFF_BACKLIGHT() && !SKY_IS_PHONE_OPEN())
        {
            SKY_SubBacklightOn(AB_ALWAYS);
        }
    }
#endif
}


/*===========================================================================
FUNCTION UI_KEYCB_REG

DESCRIPTION
  It starts key timer to callback at regular interval of time.
RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
//void SKY_RegKeyCB(  int4 ms_first,  int4 ms_periodic,  BOOL fRepeat )
void SKY_RegKeyCB(LONG lFirstMS, LONG lPeriodicMS, BOOL fRepeat )
{
   
    clk_reg(&l_KeyDownCallback, sHandleKeyDownCallback,
             lFirstMS, lPeriodicMS, fRepeat);

} /* end of ui_keycb_reg */

/*===========================================================================
FUNCTION SKY_DeregKeyCB

DESCRIPTION
  It de-registers key callback clock.
RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
void SKY_DeregKeyCB( void )
{
    clk_dereg( &l_KeyDownCallback ); /* no need to repeat */

} /* end of ui_keycb_dereg */

/*===========================================================================
FUNCTION SKY_InitTimer

DESCRIPTION
  Initialize the user interface callback timers.

RETURN VALUE
  None

DEPENDENCIES
  Called only once from SKY_Initialize

===========================================================================*/

void SKY_API SKY_InitCallbackTimer(void)
{
    l_fIsBacklightActive = FALSE;
    clk_def( &l_BacklightCallback );        /* backlight callback */
    
    clk_def( &l_KeyDownCallback );          /* key down callback */
    clk_def( &l_TimerCallback );            /* general purpose timer */
    clk_def( &l_Timer1Callback );           /* equs general purpose timer1 */
    clk_def( &l_Timer2Callback );           /* equs general purpose timer2 */
    clk_def( &l_Timer3Callback );           /* equs general purpose timer3 */
    
    // girin. 2000.12.12
    clk_def( &l_PacemakerCallback );

    //clk_def( &l_GlobalTimerCallback ); // girin.

    // redstar 2000.12.27
    clk_def( &l_BellCallback);
    clk_def( &l_MoterCallback);
    clk_def( &l_MoterOffCallback);
    clk_def( &l_SoundStopCallback);
	clk_def( &l_LoactionCallback);
	/* rosa 01-04-08 --> @why, what */	
	clk_def( &l_GVMTimer1Callback);
	clk_def( &l_GVMTimer2Callback);
	clk_def( &l_GVMTimer3Callback);
	/* <-- */

#ifdef FEATURE_USE_IRDA
    clk_def( &l_IrDAStatusCheckCallback);
#endif
#ifdef FEATURE_WAP
    clk_def(&clk_timer);
#endif //#ifdef FEATURE_WAP

// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
//// happykid 00.12.28
//// 1분마다 한번씩 스마트 배터리를 체크하기 위해서 
//#ifdef FEATURE_SMART_BATTERY
//	clk_def( &l_SmbCallback);
//#endif
//// happykid end

#ifdef WIN32
    SKY_Set_Clock(
                    SKY_PACEMAKER_TIMER_SIG,
                    &l_PacemakerCallback,
                    sPacemakerCallback,
                    4000,
                    4000,
                    TRUE);
#else
    //clk_reg(&l_PacemakerCallback, sPacemakerCallback, CLOCK_PACEMAKER_REPEAT_TIME, CLOCK_PACEMAKER_REPEAT_TIME, TRUE); // Register
    clk_reg(&l_PacemakerCallback, sPacemakerCallback, 1000, 1000, TRUE); // Register
#endif

// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
//// happykid 00.12.28
////power off 시에 꺼야하나?
//#ifdef FEATURE_SMART_BATTERY
//	SKY_SetSmartBatteryTimer(TRUE);
//#endif
    l_TimerSignal[0].Signal = SKY_GEN_TIMER_SIG;
    l_TimerSignal[0].pCallback = &l_TimerCallback;
    l_TimerSignal[0].pfnCallback = sHandleGeneralTimerCallback;
    
    l_TimerSignal[1].Signal = SKY_EQS_TIMER1_SIG;
    l_TimerSignal[1].pCallback = &l_Timer1Callback;
    l_TimerSignal[1].pfnCallback = sHandleGeneralTimer1Callback;

    l_TimerSignal[2].Signal = SKY_EQS_TIMER2_SIG;
    l_TimerSignal[2].pCallback = &l_Timer2Callback;
    l_TimerSignal[2].pfnCallback = sHandleGeneralTimer2Callback;

    l_TimerSignal[3].Signal = SKY_EQS_TIMER3_SIG;
    l_TimerSignal[3].pCallback = &l_Timer3Callback;
    l_TimerSignal[3].pfnCallback = sHandleGeneralTimer3Callback;

} 

// happykid 00.12.28
/*===========================================================================
FUNCTION smb_check_every_oneminute

DESCRIPTION
  1분마다 혹은 hand free kit가 인식되었을 때(external power ON), 
  가스게이지의 RAM 점검과 배터리의 유무 및 상이성을 판별하고 
  필요하면 초기화를 행한다.

===========================================================================*/

#ifdef FEATURE_SMART_BATTERY
void SKY_API smb_check_every_oneminute(BOOL isExtPwrOn)
{	
	extern SmbRAMStatusType    smbIsOperation;
	extern byte    smbErrorCode;
	static boolean smbFirstOneMinute = TRUE;
    //static byte    smb_random_time_count = 0;
    //static byte    smb_ram_check_count   = SMB_MAX_RAM_CHECK_COUNT-1;
    //static boolean SmbInitDoneAtBattChange = FALSE; //배터리 교환때 초기화를 행했는가?
    //static boolean SmbEnterExtPwrRoutine   = FALSE; // external power routine을 거치고 나왔는가?

	smb_ram_check_count = (smb_ram_check_count + 1) % SMB_MAX_RAM_CHECK_COUNT;


    // tasik 00.12.18
	/*
		// tasik 00.12.22
		매 일분마다 identity를 체크하는 것은 생각해보면 external power
		가 들어왔을 때만 고려하면 된다. 즉, external power가 들어오지 않은 
		상황에선 배터리를 구별할 필요가 없기 때문이다. 왜냐하면 
		external power가 들어오지 않은 상황에서 배터리가 바뀔 수 있는 것은
		배터리를 빼서 다시 전화기를 켜는 방법뿐이다. 이런 경우는 다시 
		초기화를 하므로 구별할 필요가 없다.

		// tasik 00.12.26
		external power가 들어 왔을 때는 따로 RAM Validity를 점검하지않는다.
		대신에 identity를 하면서 동시에 RAM을 점검한다.
		external power가 들어 왔을 때는 RAM 점검을 하되 30분에 한번씩 한다.
		
		// tasik 00.12.28
		external power와 관련된 문제는 경우의 수가, 생각하기로, 16가지가 된다.
		그중에서 대처하지 못했던 3경우를 보강했다. 그 세가지는
		  
		   % external power를 뽑을 순간
		   1. idle/battery off->on, but not initialized
		   2. traffic/battery off->on, but not initialized.
		   3. traffic/batttery1 -> battery2, but not initialized.

	    위의 문제점은 external power를 뽑고 전화기 단독으로 쓰일 때 생길 수 
		있는 문제점들이다. 
   
		  // tasik 00.12.18
		매 일분마다 한번씩 부르는 것은 기본적으로 hand free kit때문이다.
		hand free kit에는 power가 들어오는 선이 있으므로 배터리가 없는 
		상황에서도 동작이 가능하다. 따라서 이경우 배터리를 빼버릴 수도 있으므로
		배터리가 존재하는 지를 계속 점검해봐야 하기 때문이다.

		아래의 루틴은 RAM 점검을 매 일분마다 하지 않고 SMB_MAX_RAM_CHECK_COUNT(30분)
		마다 한번씩만 하도록 하였다. 이유는 RAM이 고장날 확률은 매우 낮은데
		비해서 그것을 체크하는 횟수는 너무 많다는 것이다. 이럴 경우는 bq2018에서
		읽기 동작을 너무 자주하게 되는 일이 많아진다. 이것은 피하고 싶은 동작이다.
		읽기/쓰기 동작은 intlock을 걸기 때문에 전화기의 기본 동작에 영향을 줄 수 있다.

		그러나 문제가 되는 부분은 hand free kit를 장착한 후에 배터리를 탈착한 경우
		혹은 다른 배터리를 장착한 경우이다. 이 경우에 대해선 SmartBatteryNotExist
		의 값이 들어오면 배터리를 탈착한 경우이고 장착을 하게 되면 기존의 논리대로
		SmbIdentityCheck를 통해서 검사한다.

		smb_init()에서 처음 한번을 체크한다.
	*/
	  if(!isExtPwrOn == TRUE)
	  {// external power가 들어오지 않았을 때, 보통의 상황

		  //초기화과정에서 external power가 들어온 상황에서 에러가 
		  // 났으면 그것이 배터리가 없어서 그런것인지 
		  // 배터리가 고장이 나서 그런지를 구별할 수 없다.
		  // 그런 상황에서 부팅후 1분이내에 배터리를 꼽고 
		  // 바로 external power를 제거하면 초기화 루틴을 
		  // 결국 한번도 실행하지 않고 배터리 고장이란 메시지가 
		  // 계속 나올 수 있다. 이를 해소하기 위해서 아래를 넣었다.
		   if((smbIsOperation == BQ2018_RAM_DESTROYED_S) && 
			  (smbFirstOneMinute == TRUE))
			       smb_init();
		   
		   smbFirstOneMinute = FALSE;

	       if((smb_ram_check_count == 0)				    && // RAM 점검한지 30분이 지났다.
			  (MAIN_STATE(cdma.curr_state) != CDMA_SYS_ACC) && // traffic/ access channel 상태가 아니다.
	          (MAIN_STATE(cdma.curr_state) != CDMA_TC))
		   {
			  smbIsOperation = smb_check_RAM_validity(); 
		   }

		   // tasik 00.12.28
		  if((SmbEnterExtPwrRoutine == TRUE)			   && // External power routine을 거치고 왔다.
			 (MAIN_STATE(cdma.curr_state) != CDMA_SYS_ACC) && // traffic/ access channel 상태가 아니다.
			 (MAIN_STATE(cdma.curr_state) != CDMA_TC))
		  {
			  byte smb_id_chk;

			  SmbEnterExtPwrRoutine = FALSE;

			  if(SmbInitDoneAtBattChange == FALSE) 
			  {// 배터리 교환시 초기화를 하지 않았다.
				  SmbInitDoneAtBattChange = TRUE;
				  smb_init();
			  }
			  else
			  {// 초기화를 했거나, 하지 않아도 되었거나, 할 수 없던 상황이었다.

				  smb_id_chk = SmbRead1Byte(BQ2018_RANDOM_NUMBER_B); 
				  if(smb_id_chk != SmbIdentityCheck)
				  {  // external power를 빼면서 배터리를 교환했다.
				     smb_init();
				  }
			  }
		  }
		  
		  if(smbIsOperation == BQ2018_RAM_OK_S)
		  {
			  SmbEvent(SMB_TIMER_S);
			  smb_batt_curve_estimate();
		  }
	  }
	  else
	  {// external power가 들어 왔을 때
		  byte smb_id_chk;
		
		  SmbEnterExtPwrRoutine = TRUE; //외부전원 루틴을 실행했다.
		  smbFirstOneMinute = FALSE; //처음 일분이 지났다.

		  smb_random_time_count = (smb_random_time_count + 1) % 128; 

		  // tasik 00.12.22
		  /*
	  		아래에 스마트 배터리에 읽기/쓰기를 원래는 2바이트를 1바이트로 바꾸었는데 
			그러면 random number가 2^16의 경우의 수에서 2^8경우의 수로 줄게 되어서
			실제로 random하지 않을 수 있다. 이런 단점을 보완하기 위해서 seed를 random
			하게 주게 되면 2^8의 경우의 수가 보다  random하게 될 수 있기 때문이다.

			1바이트로 바꾼 이유는 {SmbRead2Byte(BQ2018_RANDOM_NUMBER_W)
			-> SmbRead1Byte(BQ2018_RANDOM_NUMBER_B)} 1분마다 불려질 때 읽기/쓰기
			에서 많은 시간을 INTLOCK을 걸게 되면 다른 기능들에 영향을 주기 때문이다. 
		  */

		  if(smb_random_time_count == 0)
		  {
			  srand((unsigned) SKY_GetLocalTime());
		  }

		  //tasik 00.12.29
		  /*
			traffic 상태에선 배터리 검사를 하지 않는다.
			통화중에는 배터리가 고장이 났거나 배터리를 교환한 사실은
			알 수 없다. 만약 bq2018과 통신을 시도했을 때는
			심하면 call drop이 일어나고 약하게는 음질이 나빠질 수도 있다.
		  */
		  if((MAIN_STATE(cdma.curr_state) != CDMA_SYS_ACC) &&
	         (MAIN_STATE(cdma.curr_state) != CDMA_TC))
			 smb_id_chk = SmbRead1Byte(BQ2018_RANDOM_NUMBER_B);
		  else
			  smb_id_chk = SmbIdentityCheck;
				
		  if(smb_id_chk ==  SmbIdentityCheck)
		  {// 같은 배터리 혹은 이전에 인식되었던 배터리
		     SmbInitDoneAtBattChange = TRUE; // 배터리 교환시 초기화를 안했을 때 대비
			
			 if((MAIN_STATE(cdma.curr_state) != CDMA_SYS_ACC) &&
	            (MAIN_STATE(cdma.curr_state) != CDMA_TC))
			 { // access channel에 접속상태 혹은 통화상태가 아닐때
     	        smbIsOperation = BQ2018_RAM_OK_S; //UI에게 알림
			  
		        // -1은 혹시나 0xFF가 나올 때를 대비해서이다.
		        SmbIdentityCheck = ((byte) rand()) - 1; 
	            if(SmbIdentityCheck == -1 || SmbIdentityCheck == 0)
				{//  -1은 2'complement에서 0xFF와 같다.
	               SmbIdentityCheck = 1; 
				}
		        SmbWrite1Byte(BQ2018_RANDOM_NUMBER_B, (byte) SmbIdentityCheck);
			 }

			 SmbEvent(SMB_TIMER_S);
		     smb_batt_curve_estimate();
		  }
		  else
		  {
			  if(smb_id_chk == SMB_BATT_NOT_EXIST_VAL)
			  {// 배터리가 존재하지 않을 때, 배터리가 고장났을 때
				  smbIsOperation = BQ2018_RAM_DESTROYED_S;  // UI에게 알림
				  smbErrorCode = 5; //외부전원이 들어왔을 때 배터리가 고장나거나 없음.

				  SmbInitDoneAtBattChange = FALSE; //초기화를 안했다.

				  // 같은 배터리를 뺐다가 다시 꽂았을 때도 마찬가지로 초기화
				  // 를 하기 위해서 SmbIdentityCheck를 미리 바꿔놓는다.
                  SmbIdentityCheck = ((byte) rand()) - 1; 
	              if(SmbIdentityCheck == -1 || SmbIdentityCheck == 0)
				  {//  -1은 2'complement에서 0xFF와 같다.
	                 SmbIdentityCheck = 1; 
				  }
			  }
			  else 
			  {// 다른 배터리 혹은 새로히 인식된 배터리
				  if((MAIN_STATE(cdma.curr_state) != CDMA_SYS_ACC) &&
	                 (MAIN_STATE(cdma.curr_state) != CDMA_TC))
				  { // access channel에 접속상태 혹은 통화상태가 아닐때
					  // tasik 00.12.28
					  /*
					    아래의 함수는 traffic 상태에서 실행되면 call drop이 일어 날 수 있다.

						단점: 통화중에 배터리를 갈아 끼울 때 혹은 배터리를 장착할 때는 
						통화가 끝나기 전에는 배터리 정보를 나타낼 수 없다.
					  */
					 smb_init();
					 SmbInitDoneAtBattChange = TRUE; //배터리 교환시 초기화를 행하지 않았을 때를 대비
				  }
			  }
		  }// if(smb_id_chk ==  SmbIdentityCheck)

	  }// if (!isExtPwrOn == TRUE)
	
	return;
}
#endif
/*===========================================================================
FUNCTION SKY_HandleTimers
DESCRIPTION
  Handle the UI timers.

DEPENDENCIES
  REX is running.  NV and DIAG and HS are running.

RETURN VALUE
  None

SIDE EFFECTS
  modifies timeflags, ui.offhook_offline(ISS2), uisleep(ISS2),
  ui.alarms_init_done(ISS2), ui.alarm_ringing(ISS2), ui.on_hook(ISS2 & CSS),
  ui.n, ui.wasonetouch (NGP).
===========================================================================*/
void SKY_API SKY_HandleTimers(void)
{
    /* sky local timer */
    
    if( l_TimerFlags & SKY_GEN_TIMER_SIG)
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_GEN_TIMER_SIG);  /* clear flag */
        SKY_DoEvent(SE_TM_TIMER);                     /* tell state machine */
    }        
    if( l_TimerFlags & SKY_EQS_TIMER1_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_EQS_TIMER1_SIG);  /* clear flag */
        SKY_DoEvent(SE_TM_TIMER1);              /* tell state machine */
    }
    if( l_TimerFlags & SKY_EQS_TIMER2_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_EQS_TIMER2_SIG);  /* clear flag */
        SKY_DoEvent(SE_TM_TIMER2);              /* tell state machine */
    }
    if( l_TimerFlags & SKY_EQS_TIMER3_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_EQS_TIMER3_SIG);  /* clear flag */
        SKY_DoEvent(SE_TM_TIMER3);              /* tell state machine */
    }
    
    /* key down timer */
    if( l_TimerFlags & SKY_KEY_TIMER_SIG ) 
    {
#ifdef FEATURE_SKT_EIF   //redstar  2001.02.13
		ds_cmd_type	ds_cmd_item;				// item to hold cmd to dsmgr	   
		extern	rex_sigs_type dsi_requested_sig_mask;
		ds_cmd_item.cmd_name = DS_EIF_KEY_SCAN;
		dsi_requested_sig_mask |= DS_CMD_Q_SIG;	
#endif //FEATURE_SKT_EIF


        sClearSignal(&l_TimerFlags, (WORD)SKY_KEY_TIMER_SIG);    /* clear flag */
        
        switch(GET_KEY_DOWN_KEY()) 
        {
        /* Digit held down - triggers speed dialing.  Don't repeat, */
        /* however, so change key_down_key                          */
        case SE_KEY_0 : 
        case SE_KEY_1 : 
        case SE_KEY_2 : 
        case SE_KEY_3 : 
        case SE_KEY_4 : 
        case SE_KEY_5 : 
        case SE_KEY_6 : 
        case SE_KEY_7 : 
        case SE_KEY_8 : 
        case SE_KEY_9 : 
            SKY_DoEvent((SKY_EVENT_E)(SE_KEY_0_LONG + GET_KEY_DOWN_KEY()-SE_KEY_0));
            SKY_DoEvent(SE_KEY_DIGIT_LONG);
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_0_LONG+GET_KEY_DOWN_KEY()-SE_KEY_0;
		        ds_cmd(&ds_cmd_item);            
            }
#endif  //FEATURE_SKT_EIF             

            SET_KEY_DOWN_KEY(SE_KEY_DIGIT_LONG);
            clk_dereg( &l_KeyDownCallback );
//            dwSleepFlag &= ~SKY_KEY_TIMER_SIG;    /* ok to sleep */
            break;

        case SE_KEY_F1:   //redstar  2001.10.04
			SET_KEY_DOWN_KEY(SE_KEY_F1_LONG);
            SKY_DoEvent(SE_KEY_F1_LONG);
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_F1_LONG;
		        ds_cmd(&ds_cmd_item);                  
            }
#endif  //FEATURE_SKT_EIF 
			break;
        
        case SE_KEY_F2:   //redstar  2001.03.22  for key ntop - gvm
			SET_KEY_DOWN_KEY(SE_KEY_F2_LONG);
            SKY_DoEvent(SE_KEY_F2_LONG);
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_F2_LONG;
		        ds_cmd(&ds_cmd_item);                  
            }
#endif  //FEATURE_SKT_EIF 
			break;
			
        case SE_KEY_POUND:
            SKY_DoEvent(SE_KEY_POUND_LONG);  
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_POUND_LONG;
		        ds_cmd(&ds_cmd_item);                                 
            }
#endif  //FEATURE_SKT_EIF 
            break;
        case SE_KEY_STAR:

            //if(!SKY_IS_PLAYING_GVM()) // /* undersea 2001-08-30 --> GVM2.05에서는 에티켓 처리합니다 */
            //{
                SKY_DoEvent(SE_KEY_STAR_LONG);  
                SKY_DoEvent(SE_KEY_MANNERS);
                SET_KEY_DOWN_KEY(SE_KEY_STAR_LONG);
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
                if( g_nKeyFlag == 2 )
                {
                    ds_cmd_item.parms.srv_opt = SE_KEY_STAR_LONG;
		            ds_cmd(&ds_cmd_item);                  
                }
#endif  //FEATURE_SKT_EIF 
            //}
            break;
        /* end key long --> power down */
        case SE_KEY_END :      /* SKTT D1 Power key must be held down */
        //csse SE_KEY_POWER :
            if( --GET_POWER_OFF_COUNT() == 0 && SKY_IS_PHONE_OPEN())          
            {               
                SKY_KeyUp();
                SKY_TRACE(TL_HIGH, "Got Power Down from End key");
                SKY_DoEvent(SE_SYS_TO_IDLE);   /* back to idle */
                SKY_DoEvent(SE_KEY_POWER_OFF);
            }
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_END_LONG;
		        ds_cmd(&ds_cmd_item);                 
            }
#endif  //FEATURE_SKT_EIF 
            break;

        /* down key repeat */
        case SE_KEY_DOWN :
            //SKY_DoEvent(SE_KEY_DOWN);
			SKY_AddEvent(SE_KEY_DOWN);
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_DOWN;
		        ds_cmd(&ds_cmd_item);               
            }
#endif  //FEATURE_SKT_EIF 
            break;

        /* up key repeat */
        case SE_KEY_UP :
            //SKY_DoEvent(SE_KEY_UP);
			SKY_AddEvent(SE_KEY_UP);

#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_UP;
		        ds_cmd(&ds_cmd_item);               
            }
#endif  //FEATURE_SKT_EIF 
            break;

        // woodstok 01.4.15 - generate left,right long key for moving home and end cursor in editor mode
        case SE_KEY_LEFT:
            SKY_DoEvent(SE_KEY_LEFT_LONG);  
            SET_KEY_DOWN_KEY(SE_KEY_LEFT_LONG);
            break;

        case SE_KEY_RIGHT:
            SKY_DoEvent(SE_KEY_RIGHT_LONG);  
            SET_KEY_DOWN_KEY(SE_KEY_RIGHT_LONG);
            break;
        // end woodstok.

        /* clear long */
        case SE_KEY_CLEAR :
            if(!IS_CLEAR_ALL_BEEP_FLAG())
            {
                SKY_CLEAR_ALL_TONE();
                SET_CLEAR_ALL_BEEP_FLAG(TRUE);
            }
            SKY_DoEvent(SE_KEY_CLEAR_LONG);
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_CLEAR_LONG;
		        ds_cmd(&ds_cmd_item);               
            }
#endif  //FEATURE_SKT_EIF 
            break;

        case SE_KEY_PLAY :
            SKY_DoEvent(SE_KEY_PLAY_LONG);
            clk_dereg( &l_KeyDownCallback );
            UI_ENABLE_SLEEP(); /* okay to sleep */
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_PLAY_LONG;
		        ds_cmd(&ds_cmd_item);                  
            }
#endif  //FEATURE_SKT_EIF 
            break;
        
        case SE_KEY_RECORD :
            SKY_DoEvent(SE_KEY_RECORD_LONG);
            clk_dereg( &l_KeyDownCallback );
            UI_ENABLE_SLEEP(); /* okay to sleep */
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_RECORD_LONG;
		        ds_cmd(&ds_cmd_item);               
            }
#endif  //FEATURE_SKT_EIF 
            break;

        case SE_KEY_AUTO_ANSWER :
			SET_KEY_DOWN_KEY(SE_KEY_AUTO_ANSWER_LONG);
            SKY_DoEvent(SE_KEY_AUTO_ANSWER_LONG);            
            clk_dereg( &l_KeyDownCallback );
            UI_ENABLE_SLEEP(); /* okay to sleep */
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {
                ds_cmd_item.parms.srv_opt = SE_KEY_AUTO_ANSWER_LONG;
		        ds_cmd(&ds_cmd_item);               
            }
#endif  //FEATURE_SKT_EIF 
            break;

        default:                /* other keys don't repeat */
            break;
        }
        
    }

    // girin. 2000.12.12
    if( l_TimerFlags & SKY_PACEMAKER_TIMER_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_PACEMAKER_TIMER_SIG);  /* clear flag */
		SKY_DoEvent(SE_SYS_PACEMAKER);                     /* tell state machine */
    }

    // girin.
    //if( l_TimerFlags & SKY_GLOBAL_TIMER_SIG) 
    //{
    //    sClearSignal(&l_TimerFlags, (WORD)~SKY_GLOBAL_TIMER_SIG);  /* clear flag */
    //    SKY_DoEvent(SE_SYS_GLOBAL_TIMER);                  /* tell state machine */
    //}

    //redstar 2000.12.27
    if( l_TimerFlags & SKY_BELL_TIMER_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_BELL_TIMER_SIG);  /* clear flag */
        SKY_DoEvent(l_BellEvent);                     /* tell state machine */
    }  

    if( l_TimerFlags & SKY_MOTER_TIMER_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_MOTER_TIMER_SIG);  /* clear flag */
        SKY_MotorOn();      //HS_MOTER_ON();        /* call moter off function */
        SKY_SetMotorOffTimer(1500);
    }  

    if( l_TimerFlags & SKY_MOTER_OFF_TIMER_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_MOTER_OFF_TIMER_SIG);  /* clear flag */     
        SKY_MotorOff();      //HS_MOTER_OFF();        /* call moter off function */        
    }  

    if( l_TimerFlags & SKY_SOUND_STOP_TIMER_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_SOUND_STOP_TIMER_SIG);  /* clear flag */
        SKY_StopMusic();     
    }  

    if( l_TimerFlags & SKY_LOCATION_TIMER_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_LOCATION_TIMER_SIG);  /* clear flag */
		SKY_DoEvent(SE_LOCATION_TIMEOUT);		            
    }  

#ifdef FEATURE_USE_IRDA
    if( l_TimerFlags & SKY_IRDA_STATUS_CHECK_TIMER) 
    {
		// 수신 모드로 들어간후 전화를 못받는 문제를 해결하기 위해
		if(IrDA_Off)
		{
			IrDA_Off = 0;
			// Close IrDA port
			SKY_CloseIrDAPort();
		}
		else
		{
			if(SKY_CheckLAPConnection())
			{
				IrDA_AutoOffTimeOutCounter = 200;
                //SKY_DoEvent(SE_IRDA_CONNECTED_F);
				//IrDA_DrawIrDAStatusLine(2);
			}
			else
			{
				IrDA_AutoOffTimeOutCounter--;
                //SKY_DoEvent(SE_IRDA_DISCONNECTED_F);
				//IrDA_DrawIrDAStatusLine(1);
			}
			if(IrDA_AutoOffTimeOutCounter <=0 )
			{
				// Close IrDA port
				ChangeIrDAMode ( FALSE );
                                g_fIrDAPortForPC = FALSE;
			}
		}

        sClearSignal(&l_TimerFlags, (WORD)SKY_IRDA_STATUS_CHECK_TIMER);  /* clear flag */
        // XXX_wooodstok : N.A.
    }  
#endif
	/* rosa 01-04-08 --> @why, what */
    if( l_TimerFlags & SKY_GVM_TIMER1_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_GVM_TIMER1_SIG);  /* clear flag */
		SKY_DoEvent(SE_GVM_TIMER1);		            
    }
    if( l_TimerFlags & SKY_GVM_TIMER2_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_GVM_TIMER2_SIG);  /* clear flag */
		SKY_DoEvent(SE_GVM_TIMER2);		            
    }
    if( l_TimerFlags & SKY_GVM_TIMER3_SIG) 
    {
        sClearSignal(&l_TimerFlags, (WORD)SKY_GVM_TIMER3_SIG);  /* clear flag */
		SKY_DoEvent(SE_GVM_TIMER3);		            
    }
	/* <-- */

// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
// happykid 00.12.28
//#ifdef FEATURE_SMART_BATTERY
//    if( l_TimerFlags & SKY_SMB_TIMER_SIG) 
//    {
//        sClearSignal(&l_TimerFlags, (WORD)~SKY_MOTER_TIMER_SIG);  /* clear flag */
//		smb_check_every_oneminute(ui.extpwr);
//		ui_do_event(SMB_ONEMINUTE_F);
//    }  
//#endif
}
