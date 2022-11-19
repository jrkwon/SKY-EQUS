#ifndef UIH_H
#define UIH_H
/*===========================================================================

              U S E R   I N T E R F A C E   H A N D L E R


DESCRIPTION

  This include file contains the prototypes of the functions that are shared
  among UI Handler files. It also, exports the global variables that are
  shared.
  
  Copyright (c) 1999 through 2000 by QUALCOMM INCORPORATED  All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uih.h_v   1.1   29 Mar 2000 18:10:34   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/02/00   KAR     UI timers don't disable sleep
10/26/99   CS      Added new function prototypes for FEATURE_BSPUI_API.
07/22/99   VT      Initial version; Carved it out of ui.c

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


//redstar  2000.10.08
#if  (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) 
#include "ui.h"
#define EQS_CODE_SIZE   4
typedef struct tagEFS_PHONESTATE_T     EFS_PHONESTATE_T;
typedef struct tagPHONESTATE_T         PHONESTATE_T;


struct  tagEFS_PHONESTATE_T
{
   BOOL  fLock;                   // ui.lock
   BYTE  bPassWord[EQS_CODE_SIZE]; // ui.pass
   BYTE  bRestrict;                    // ui.restrict
   BOOL  fOneTouch;                    // ui.onetouch
};

struct tagPHONESTATE_T{
	BOOL   fDebug;                 //ui.isdebug        
	BOOL   fSleepOK;               //ui.ok_to_sleep
	BOOL   fPowerOn;               //ui.pwr
	BOOL   fOneTouched;            //ui.wasonetouch
	BOOL   fMute;                  //ui.mute
	BOOL   fOneMinBattery;         //ui.one_min_bat
	BOOL   fBatteryWarn;           //ui.batwarn

#ifdef FEATURE_DS_DEBUG_UI	
	BOOL   fDataDebug;           //ui.isdatadebug
#endif /* FEATURE_DS_DEBUG_UI */	

	BOOL   fOffline;             //ui.offline
	BOOL   fPowerUp;               //ui.powerup
	BOOL   fSystemValid;           //ui.system_valid
	BOOL   fGetKeys;               //ui.getkeys
	BOOL   fOpenPhone;            //ui.earpieceup
	BOOL   fSilentPad;             //ui.pad
	dword  dwIgnitionTime;         //ui.igntime
    BOOL   fIgnition;              //ui.ign    
    BOOL   fSysTimeValid;          //ui.systime_valid
    BOOL   fIdling;
    ui_cmd_type EventParam;           //ui.evt_param
    EFS_PHONESTATE_T     PhoneConfig;
/*
	BOOL   fIdle;                  //ui.is_idling
	BOOL   fAlphaEntry;          //ui.alpha_entry
	BOOL   fFromEdit2RCL;          //ui.editrcl
	BOOL   fIdling;              //ui.idling
	BOOL   fDispAlloc;             //ui.disploc
	BOOL   fListAlloc;             //ui.listloc
*/
};


extern PHONESTATE_T       gPhoneState;

extern BOOL  GetPSLock( void );
extern void  SetPSLock( BOOL fLock );
extern BYTE* GetPSPassWord( void );
extern void  SetPSPassWord( BYTE* pbString);
extern BYTE GetPSRestrict( void );
extern void SetPSRestrict( BYTE bValue );
extern BOOL GetPSOneTouch( void );
extern void SetPSOneTouch( BOOL fValue );
extern PHONESTATE_T* GetPSData(void);
extern void SetPSData( PHONESTATE_T* pStateValue );



#define GET_PS_DEBUG                 (  gPhoneState.fDebug                  )
#define SET_PS_DEBUG( fValue )       (  gPhoneState.fDebug = fValue         )  

#define GET_PS_SLEEPOK               (  gPhoneState.fSleepOk                )
#define SET_PS_SLEEPOK( fValue )     (  gPhoneState.fSleepOk = fValue       )
#define GET_PS_POWERON               (  gPhoneState.fPowerOn                )
#define SET_PS_POWERON( fValue )     (  gPhoneState.fPowerOn = fVaue        )
#define GET_PS_ONETOUCHED            (  gPhoneState.fOneTouched             )
#define SET_PS_ONETOUCHED(fValue)    (  gPhoneState.fOneTouched = fValue    )
#define GET_PS_MUTE                  (  gPhoneState.fMute                   )
#define SET_PS_MUTE( fValue )        (  gPhoneState.fMute = fValue          )
#define GET_PS_ONEMINBATTERY         (  gPhoneState.fOneMinBattery          )
#define SET_PS_ONEMINBATTERY(fValue) (  gPhoneState.fOneMinBattery = fValue )
#define GET_PS_BATTERYWARN           (  gPhoneState.fBatteryWarn            )
#define SET_PS_BATTERYWARN(fValue)   (  gPhoneState.fBatteryWarn = fValue   )
#define GET_PS_DATADEBUG             (  gPhoneState.fDataDebug   = fValue   )
#define SET_PS_DATADEBUG( fValue )   (  gPhoneState.fDataDebug = fValue     )
#define GET_PS_OFFLINE               (  gPhoneState.fOffline                )
#define SET_PS_OFFLINE( fValue )     (  gPhoneState.fOffline = fValue       )
#define GET_PS_POWERUP               (  gPhoneState.fPowerUp                )
#define SET_PS_POWERUP( fValue )     (  gPhoneState.fPowerUp = fValue       )
#define GET_PS_SYSTEMVALID           (  gPhoneState.fSystemValid            )
#define SET_PS_SYSTEMVALID(fValue)   (  gPhoneState.fSystemValid = fValue   )
#define GET_PS_GETKEYS                  (  gPhoneState.fGetKeys             )
#define SET_PS_GETKEYS( fValue )     (  gPhoneState.fGetKeys = fValue       )
#define GET_PS_PAD                   (  gPhoneState.fSilentPad              )
#define SET_PS_PAD( fValue )         (  gPhoneState.fSilentPad = fValue     )
#define GET_PS_IGNITIONTIME          (  gPhoneState.dwIgnitionTime          )
#define SET_PS_IGNITIONTIME(dwValue) (  gPhoneState.dwIgnitionTime = dwValue)
#define GET_PS_IGNITION              (  gPhoneState.fIgnition               )
#define SET_PS_IGNITION( fValue )    (  gPhoneState.fIgnition = fValue      )
#define GET_PS_OPENPHONE             (  gPhoneState.fOpenPhone              )
#define SET_PS_OPENPHONE( fValue )   (  gPhoneState.fOpenPhone = fValue     )
#define GET_PS_SYSTIMEVALID          (  gPhoneState.fSysTimeValid           )
#define SET_PS_SYSTIMEVALID(fValue)  (  gPhoneState.fSysTimeValid = fValue  )
#define GET_PS_IDLING                (  gPhoneState.fIdling                 )
#define SET_PS_IDLING( fValue )      (  gPhoneState.fIdling       = fValue  )
#define GET_PS_EVENTPARAM            (  gPhoneState.EventParam              )
#define SET_PS_EVENTPARAM(EventParam)(  gPhoneState.EventParam = EventParam )


#define GET_PS_LOCK                  ( GetPSLock()              )
#define SET_PS_LOCK( fValue )        ( SetPSLock(fValue)        )
#define GET_PS_PASSWORD              ( GetPSPassWord()          )
#define SET_PS_PASSWORD(pbString)    ( SetPSPassWord(pbString)  )
#define GET_PS_RESTRICT              ( GetPSRestrict()          )
#define SET_PS_RESTRICT(bValue)      ( SetPSRestrict(bValue)    )
#define GET_PS_ONETOUCH              ( GetPSOneTouch()          )
#define SET_PS_ONETOUCH(fValue)      ( SetPSOneTouch(fValue)    )
#define GET_PS_DATA                  ( GetPSData()              )
#define SET_PS_DATA(pStateValue)     ( SetPSData(pStateValue)   )

/*
**----------------------------------------------------------------------------
**	       NEW NV and VARIABLES  METHODS
**----------------------------------------------------------------------------
*/
#define EQS_MAX_NAMS  4
#define EQS_MAX_MINS  2


typedef struct tagNAMPROG_T         NAMPROG_T;

struct tagNAMPROG_T{

	BOOL     fAllowMode;                            //ui.allow_mode_sel
	WORD     wForceSo;                              //ui.force_so
	BOOL     fCDMA[EQS_MAX_NAMS];                    //ui.hascdma
	BOOL     fAnalog[EQS_MAX_NAMS];                  //ui.hasfm

	DWORD    dwMIN1[EQS_MAX_NAMS][EQS_MAX_MINS];       //ui.min1
	WORD     wMIN2[EQS_MAX_NAMS][EQS_MAX_MINS];       //ui.min2
	BYTE     bCurrentNAM;                           //ui.nam
	BOOL     ChannelA_CDMA[EQS_MAX_NAMS];           //ui.hasacdma
	BOOL     ChannelB_CDMA[EQS_MAX_NAMS];           //ui.hasbcdma
};

BYTE   GetNPCurrentNAM( void );
void   SetNPCurrentNAM( BYTE bValue );
DWORD* GetNPMIN1(UINT8 nIndex );
void   SetNPMIN1(UINT8 index, DWORD* pdwValue );
WORD*  GetNPMIN2(UINT8 nIndex );
void   SetNPMIN2(UINT8 index, WORD*  pwValue );
NAMPROG_T* GetNPData(void);
void   SetNPData( NAMPROG_T* pNPProg );

extern NAMPROG_T       gNAMProg;


#define GET_NP_ALLOWMODE              (  gNAMProg.fAllowMode                )
#define SET_NP_ALLOWMODE(fValue)      (  gNAMProg.fAllowMode = fValue       )
#define GET_NP_FORCESO                (  gNAMProg.wForceSo                  )
#define SET_NP_FORCESO(wValue)        (  gNAMProg.wForceSo   = wValue       )

#define GET_NP_CDMA( index )          (  gNAMProg.fCDMA[index]              )
#define SET_NP_CDMA( index, fValue)   (  gNAMProg.fCDMA[index] = fValue     )
#define GET_NP_ANALOG( index )        (  gNAMProg.fAnalog[index]            )
#define SET_NP_ANALOG(index, fValue)  (  gNAMProg.fAnalog[index] = fValue   )

#define GET_NP_CURRENTNAM             (  GetNPCurrentNAM()                  )
#define SET_NP_CURRENTNAM( bValue )   (  SetNPCurrentNAM(bValue)            )
#define GET_NP_MIN1( index )          (  GetNPMIM1(index)                   )
#define SET_NP_MIN1( index, pdwValue) (  SetNPMIN1(index, pdwValue)         )
#define GET_NP_MIN2(index )           (  GetNPMIM2(index)                   )
#define SET_NP_MIN2(index, pwValue)   (  SetNPMIN2(index, pdwValue)         )

#define GET_NP_DATA                   (  GetNPData()                        )
#define SET_NP_DATA(pNPProg)          (  SetNPData(pNPProg)                 )




/**---------------------------------------------------------------------------
**                  External Device (BackLight, Headset, Hand Free Kit ...)
**---------------------------------------------------------------------------*/
#ifndef WIN32
#include "eqs.h"
#endif
typedef struct tagEXTDEVICE_T     EXTDEVICE_T;

struct tagEXTDEVICE_T{
	BOOL     fHandyphone;      //ui.handyphone      
	BOOL     fCharging;      //ui.charging
	BOOL     fExtDev;       //ui.extdev
	BOOL     fExtPower;        //ui.extpwr
	BOOL     fHeadset;         //ui.headset
	BOOL     fOnHook;          //ui.onhook
	BOOL     fSpkr;         //ui.spkr
    BYTE     bFirstBacklight;  //ui.backlight1
	BYTE     bSecondBacklight;     //ui.backlight2

	BYTE     bBacklight;              //ui.backval   -- nv item
};

extern EXTDEVICE_T     gExtDevice;
extern BYTE GetEDBlight(void);
extern void SetEDBlight(BYTE bValue);
extern EXTDEVICE_T*  GetEDData(void);
extern void  SetEDData( EXTDEVICE_T* pExtDevice );

#define GET_ED_HANDYPHONE               (  gExtDevice.fHandyphone           )
#define SET_ED_HANDYPHONE(fValue)       (  gExtDevice.fHandyphone = fValue  )
#define GET_ED_CHARGING                 (  gExtDevice.fCharging             )
#define SET_ED_CHARGING(fValue)         (  gExtDevice.fCharging   = fValue  )
#define GET_ED_EXTDEV                   (  gExtDevice.fExtDev               )
#define SET_ED_EXTDEV(fValue)           (  gExtDevice.fExtDev     = fValue  )
#define GET_ED_EXTPOWER                 (  gExtDevice.fExtPower             )
#define SET_ED_EXTPOWER(fValue)         (  gExtDevice.fExtPower   = fValue  )
#define GET_ED_HEADSET                  (  gExtDevice.fHeadset              )
#define SET_ED_HEADSET(fValue)          (  gExtDevice.fHeadset    = fValue  )
#define GET_ED_ONHOOK                   (  gExtDevice.fOnHook               )
#define SET_ED_ONHOOK(fValue)           (  gExtDevice.fOnHook     = fValue  )
#define GET_ED_SPEAKER                  (  gExtDevice.fSpkr                 )
#define SET_ED_SPEAKER(fValue)          (  gExtDevice.fSpkr       = fValue  )
#define GET_ED_BLIGHT1                  (  gExtDevice.bFirstBacklight       )
#define SET_ED_BLIGHT1(bValue)          (  gExtDevice.bFirstBacklight=bValue)
#define GET_ED_BLIGHT2                  (  gExtDevice.bSecondBacklight      )
#define SET_ED_BLIGHT2(bValue)          (  gExtDevice.bSecondBacklight=bValue)
#define GET_ED_BLIGHT                   (  GetEDBlight()                    )
#define SET_ED_BLIGHT(bValue)           (  SetEDBlight(bValue)              )
#define GET_ED_DATA                     (  GetEDData()                      )
#define SET_ED_DATA(pExtDevice)         (  SetEDData(pExtDevice)            )

#include "eqs_file.h"
#endif



#ifdef T_SLEEP
#include "sleep.h"

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
#include "uistate.h"
//redstar
#include "eqs_vari.h"
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */


#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
#define UI_ENABLE_SLEEP()                                \
{                                                        \
    BOOL  bTemp;                                         \
    EQS_GetUi( ES_PS_SLEEP_I,(void*)&bTemp, TRUE);        \
    if(!bTemp) {                                         \
	(void)rex_set_sigs( &sleep_tcb, SLEEP_UI_OKTS_SIG ); \
    MSG_HIGH( "UI Sleep enable", 0, 0, 0 );              \
    bTemp = TRUE;                                        \
    EQS_PutUi( ES_PS_SLEEP_I,(void*)&bTemp, TRUE);        \
  }                                                      \
}
#else
#define UI_ENABLE_SLEEP()                                \
{                                                        \
    if(!ui.ok_to_sleep) {                                \
	(void)rex_set_sigs( &sleep_tcb, SLEEP_UI_OKTS_SIG ); \
    MSG_HIGH( "UI Sleep enable", 0, 0, 0 );              \
    ui.ok_to_sleep = TRUE;                               \
  }                                                      \
}
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */


#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
#define UI_DISABLE_SLEEP()                               \
{                                                        \
    BOOL  bTemp;                                         \
    EQS_GetUi( ES_PS_SLEEP_I,(void*)&bTemp, TRUE);        \
    if(bTemp) {                                          \
    (void)rex_clr_sigs( &sleep_tcb, SLEEP_UI_OKTS_SIG ); \
    MSG_HIGH( "UI Sleep disable", 0, 0, 0 );             \
    bTemp = FALSE;                                       \
    EQS_PutUi( ES_PS_SLEEP_I,(void*)&bTemp, TRUE);        \
  }                                                      \
}
#else
#define UI_DISABLE_SLEEP()                               \
{                                                        \
    if(ui.ok_to_sleep) {                                 \
    (void)rex_clr_sigs( &sleep_tcb, SLEEP_UI_OKTS_SIG ); \
    MSG_HIGH( "UI Sleep disable", 0, 0, 0 );             \
    ui.ok_to_sleep = FALSE;                              \
  }                                                      \
}
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */





#else
#define UI_ENABLE_SLEEP() MSG_LOW( "UI Sleep enable", 0, 0, 0 );
#define UI_DISABLE_SLEEP() MSG_LOW( "UI Sleep disable", 0, 0, 0 );
#endif

/* Function Prototype */

void ui_signal( rex_sigs_type sigs, q_type *ui_cmd_q_ptr );

#ifdef FEATURE_BSPUI_API
#error code not present
#endif /* FEATURE_BSPUI_API */

#ifdef FEATURE_PM1000
#error code not present
#endif /* FEATURE_PM1000 */

void handle_keys( void );

void ui_cbt_init( void );
void handle_timers( void );
void ui_keycb_dereg( void );
void ui_keycb_reg( int4 ms_first, int4 ms_periodic, boolean repeat );

void ui_kick_dog( void );

/* Variables shared among handler routines */

extern word key_down_key;
extern int  pwr_off_cnt;    /* count down for power off */


#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
/*===========================================================================

                                EQUS UI Variables
	   
                          (External Device, Back Light)

============================================================================*/
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
//typedef struct tagEXTERNALDEVICE             eqsExtDevice;
//typedef struct tagLIGHT                      eqsLight;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
#if 0
/**---------------------------------------------------------------------------
**                  External Device (Headset, Hand Free Kit ...)
**---------------------------------------------------------------------------*/
struct tagEXTERNALDEVICE{
	boolean     Handyphone;      //ui.handyphone      
	boolean     InCharging;      //ui.charging
	boolean     ExtDev;       //ui.extdev
	boolean     ExtPower;        //ui.extpwr
	boolean     Headset;         //ui.headset
	boolean     OnHook;          //ui.onhook
	boolean     HFKSpkr;         //ui.spkr
};

/**---------------------------------------------------------------------------
**                             LCD Light Setting
**---------------------------------------------------------------------------*/
struct tagLIGHT{
	byte     NormalVal;  //ui.backlight1
	byte     HFKVal;     //ui.backlight2
	byte     Val;        //ui.backval
};


/*
**--------------------------------------------------------------------------------
**  Variable Declarations
**--------------------------------------------------------------------------------
*/
extern eqsExtDevice  gExtDev;   // defined in uihkey.c
extern eqsLight      gLight;    // defined in uihkey.c
#endif

#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)



#endif /* #ifndef UIINT_H */

