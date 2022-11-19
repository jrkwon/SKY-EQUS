/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_hcbt.h
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
** 2000-11-08 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_HCBT_H_
#define _SKY_HCBT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define IS_CLEAR_ALL_BEEP_FLAG()        g_fClearAllBeep
#define SET_CLEAR_ALL_BEEP_FLAG(s)      g_fClearAllBeep = s

#define CLOCK_PACEMAKER_REPEAT_TIME     4000 // 4000 msec. // girin.2000.12.12

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
#define MAX_TIMER           EQS_TIMER_END

#define SKY_KEY_TIMER_SIG       0x0001 /* 0000 0000 0000 0001 - key down timer  */
/* rosa 01-04-08 --> @why, what */
#define SKY_GVM_TIMER1_SIG      0x0002 /* 0000 0000 0000 0010 - GVM Timer 1     */
#define SKY_GVM_TIMER2_SIG      0x0004 /* 0000 0000 0000 0100 - GVM Timer 2     */
#define SKY_GVM_TIMER3_SIG      0x0008 /* 0000 0000 0000 1000 - GVM Timer 3     */
/* <-- */
#define SKY_GEN_TIMER_SIG       0x0010 /* 0000 0000 0001 0000 - genernal timer  */
#define SKY_EQS_TIMER1_SIG      0x0020 /* 0000 0000 0010 0000 - equs timer 1    */
#define SKY_EQS_TIMER2_SIG      0x0040 /* 0000 0000 0100 0000 - equs timer 2    */
#define SKY_EQS_TIMER3_SIG      0x0080 /* 0000 0000 1000 0000 - equs timer 3    */
#define SKY_BKLT_TIMER_SIG      0x0100 /* 0000 0001 0000 0000 - backlight timer */
#define SKY_PACEMAKER_TIMER_SIG 0x0200 // girin. 2000.12.12
#define SKY_BELL_TIMER_SIG      0x0400 // redstar 2000.12.27
#define SKY_MOTER_TIMER_SIG     0x0800 // redstar 2000.12.27



// girin. 2000.12.29
// No need to make proprietary one-minute timer for smart battery.
//#ifdef FEATURE_SMART_BATTERY
//#define SKY_SMB_TIMER_SIG		0x1000 // happykid 2000.12.27
//#endif
#define SKY_SOUND_STOP_TIMER_SIG 0x1000 // redstar 2000.12.29
#define SKY_MOTER_OFF_TIMER_SIG  0x2000 // redstar 2000.12.29
#define SKY_LOCATION_TIMER_SIG   0x4000 // redstar 2001.03.16

#ifdef FEATURE_USE_IRDA
#define SKY_IRDA_STATUS_CHECK_TIMER  0x8000
#endif
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
/* prevent from re-entrance into clear all beep */
extern BOOL g_fClearAllBeep; /* only used in hcbt, snd */

extern BOOL SKY_API (*g_pfnSKY_SetTimer)(EQS_TIMER_E TimerID, 
                                         LONG lIntervalMS, BOOL fRepeat);

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

extern void SKY_RegKeyCB(LONG lFirstMS, LONG lPeriodicMS, BOOL fRepeat);
//void SKY_RegKeyCB( int4 ms_first,  int4 ms_periodic,  BOOL fRepeat );
extern void SKY_DeregKeyCB( void );
extern void SKY_API SKY_BacklightOn(AUTO_BACKLIGHT_E AutoBacklight);
extern void SKY_API SKY_BacklightOff(void);

extern void SKY_API SKY_SubBacklightOn(AUTO_BACKLIGHT_E AutoBacklight);
extern void SKY_API SKY_SubBacklightOff(void);
extern BOOL SKY_API SKY_IsSubBacklightOnTime(void);

extern void SKY_API SKY_SetupBacklight(void);
extern void SKY_API SKY_InitCallbackTimer(void);
extern void SKY_API SKY_HandleTimers(void);


// redstar. 2000.12.21
extern BOOL SKY_API SKY_SetGeneralTimer(LONG lIntervalMS, BOOL fRepeat);
// girin. 2000.12.12
extern DWORD SKY_GetPacemakerCount(void);
// girin. end.

// redstar 2000.12.27
extern void SKY_API SKY_SetBellTimer(LONG lInervalMS, BOOL fRepeat, SKY_EVENT_E Event);
extern void SKY_API SKY_SetMotorOnTimer(LONG lInervalMS);
extern void SKY_API SKY_SetMotorOffTimer(LONG lInervalMS);
extern void SKY_API SKY_SetSoundStopTimer(LONG lIntervalMS);
#ifdef FEATURE_SKTT_EQUS_LOCATION
extern void SKY_API SKY_SetLocationTimer(LONG lIntervalMS, BOOL fRepeat);
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
//extern void SKY_API SKY_StartGlobalTimer((LONG lInervalMS, BOOL fRepeat);
#ifdef FEATURE_USE_IRDA
extern void SKY_API SKY_IrDAStatusCheck_set(LONG lIntervalMS);
#endif
/* rosa 01-04-08 --> @why, what */
void SKY_API SKY_SetGVMTimer1(LONG lIntervalMS, BOOL fRepeat);
void SKY_API SKY_SetGVMTimer2(LONG lIntervalMS, BOOL fRepeat);
void SKY_API SKY_SetGVMTimer3(LONG lIntervalMS, BOOL fRepeat);
/* <-- */

// this function must be only used in eqs_aapi.c  
void SKY_API SKY_SetPaceMakerTimer(LONG lIntervalMS, BOOL fRepeat);

// girin. 2000.12.29
// Smart Battery.
extern void SKY_API smb_check_every_oneminute(BOOL isExtPwrOn);
// tortol 2001.7.3
// SKVM
#ifdef FEATURE_SKVM 
extern void SKY_API SKY_BacklightPeriodOn(int duration);
extern void SKY_API SKY_SetFromSKVMPaceMakerTimer(void);
extern void SKY_API SKY_DisablePacemakerCallback(void);
#endif


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_HCBT_H_ */



