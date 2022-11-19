#ifndef UINSND_H
#define UINSND_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


         U S E R   I N T E R F A C E   S O U N D   M A N A G E R

GENERAL DESCRIPTION
  This manages the nasty details of what volume and which path to use
  at which time.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

      Copyright (c) 1991 through 1998 by QUALCOMM, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/uixsnd.h_v   1.4   04 Aug 2000 10:44:14   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/12/00   CS      Added uisnd_tty_vol for FEATURE_TTY support.
05/31/00   HD      Added Multimedia and SDAC Volume settings.
11/10/99   CS      Added UI_CALLFAIL_SND type.
10/08/99   CS      Added FEATURE_AUDIO_AGC support.
08/12/99  st/jkl   Adapted for new Sound API
05/18/98   KAR     Changes for Odie volume levels.
                   Added function uisnd_sample_snd
04/24/98   KAR     Code cleanup. Different beep/ear levels for module defined
03/04/98   KAR     Code cleanup. Moved do_key_sound from uin as uisnd_key_sound.
                   Moved uisnd_ringer_on_off_txt to uinscrn.c as
                   uiscrn_ringer_on_off_txt
                   Support for seven level earpiece volume
02/14/98   KAR     Added more ringer type options
                   Added UI_DTACO_TONE_SND for DTACO dial tones
02/05/98   KAR     Added function prototype for uisnd_sample_tone and
                   uisnd_remote_sample_tone
01/07/98   KAR     Removed associated code with FEATURE_CAR_KIT_DTMF
12/23/97   ces     Add new FEATURE_UI_SEVEN_LEVEL_SND.
12.22.97   skk     Added SMS Volume menu.
11/24/97   KAR     Added function declaration uisnd_vibrate_to_ring_txfr()
                   CR 4353
10/03/97   jjn     Added audio control feature.  For now, only Module has this
                   feature.
07/09/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/17/97   SIU     Removed pragma statement with target name.
06/10/97   SIU     Added function uisnd_adata_config().
06/06/97   SIU     Added mapping from UI ringer type to SND enums
06/04/97   SIU     Added missing edit history of change 06/02/97.
06/02/97   SIU     Added function uisnd_set_escalating_ring_lvl().
03/21/97   RC      Added function uisnd_vibrate_as_multi();
02/28/97   jjn     Created new procedures that take the place of UI's
                   interface to the sound task, for the module.  This
                   was done because the module does not have an on-board
                   ringer
02/28/97   RC      Removed conditional FEATURE_UI_ESCALATING_RING
                   from UISND_ESC_VOL -- used in uinsstrt.c.
02/18/97   RC      Implemented escalating ringing -- added UISND_ESC_VOL.
02/11/97   BK      Changed volume setting fns 2nd param to nv_items_enum_type.
01/29/97   BK      Added FEATURE_VIBRATOR initial version.
01/09/97   BK      New UI to SOUND interface modifications.
10/23/96   CAH     Alternate call fail sound.
07/17/96   BK      Added FEATURE_UI_QUICK_VM_DIAL.
04/18/96   BK      New UI branching created from uisnd.h ver 1.19
03/05/96   BK      Replace T_PRMCO1900 with T_GP.
02/13/96   BK      T_PRMCO1900 introduction.
01/15/96   CAH     Only call stereo_mute for targets that support it.
10/30/95   RAS     Updates to have css build from \ref
10/24/95   CAH     New sound type for message alerting controlled by ring vol.
09/08/95   RAS     Added  iss2 to list w/ 4th_lvl
08/28/95   BK      Correct default for Beep volume
08/22/95   CAH     Stereo mute support.
06/13/95   CAH     Gemini blink SNI LED during alert.
05/01/95   CAH     Initial Gemini support.
08/16/92   RRD     Created from UISTATE.C

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "mod.h"

#include "snd.h"



/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#ifdef FEATURE_NEW_SOUND_API
#error code not present
#else   /* FEATURE_NEW_SOUND_API */

/* Used to make a standard error beep from the UI state machine */

#ifdef FEATURE_REMOTE_RINGER
#error code not present
#else
#define UISND_ERR_TONE() uisnd_tone( UI_BEEP_SND, SND_ERR, 300 )
#endif /* FEATURE_REMOTE_RINGER */

/* make a standard clearall tone */

#ifdef FEATURE_REMOTE_RINGER
#error code not present
#else
#define UISND_CLRALL() uisnd_tone( UI_BEEP_SND, SND_CTRL, 200 );
#endif /* FEATURE_REMOTE_RINGER */

#ifdef FEATURE_UI_QUICK_VM_DIAL
#error code not present
#endif

#if (MODEL_ID == MODEL_A10) // jwpark 00.09.04

#define UISND_EAR_LVLS  4
#define UISND_SPKR_LVLS 4
#define UISND_1ST_VOL   1
#define UISND_2ND_VOL   2
#define UISND_3RD_VOL   3
#define UISND_4TH_VOL   4
#define UISND_5TH_VOL   5
#define UISND_AVG_VOL   UISND_2ND_VOL

#else
 
#ifdef FEATURE_UI_SEVEN_LEVEL_SND
#error code not present
#else

#define UISND_EAR_LVLS 4
#define UISND_SPKR_LVLS 4

#define UISND_1ST_VOL 0x1F
#define UISND_2ND_VOL 0x6F
#define UISND_3RD_VOL 0xAF
#define UISND_4TH_VOL 0xFF

#endif /* FEATURE_UI_SEVEN_LEVEL_SND */

/* Average default volume if there is none defined */
#define UISND_AVG_VOL UISND_2ND_VOL

#endif // (MODEL_ID == MODEL_A10)

#ifdef FEATURE_REMOTE_RINGER
#error code not present
#else

#ifdef FEATURE_UI_ESCALATING_RING
#error code not present
#else

#define UISND_RING_LVLS 5
#define UISND_RING_SPKR_LVLS 5

#endif /* FEATURE_UI_ESCALATING_RING */
#define UISND_BEEP_LVLS 5
#define UISND_BEEP_SPKR_LVLS 5
#define UISND_MSG_LVLS 5
#define UISND_MSG_SPKR_LVLS 5

#endif /* FEATURE_REMOTE_RINGER */

/*
** Define UISND_ESC_VOL for targets that does NOT support escalating
** ringing as well -- in uinsstrt we need to overwrite UISND_ESC_VOL
** to UISND_AVE_VOL value for such targets.
*/
#define UISND_ESC_VOL 0x8F

/* whether outpath gets voice or not */
#define UISND_NOVOICE FALSE
#define UISND_VOICE TRUE

#define ALERT_SOUND_LENGTH 2000   /* Length (ms) of Alert sound (Ring) */

/* Which type of sound we want to make */
typedef enum {                    /* the volumes */
  UI_PWRUP_SND,                /* Sounds for power on, ext power off/on   */
                               /*  in use generator     = ringer          */
                               /*  not in use generator = ringer          */
                               /*  volume used          = key beep volume */
  UI_BEEP_SND,                 /* Beep snd for most keys                  */
                               /*  in use generator     = ringer          */
                               /*  not in use generator = ringer          */
                               /*  volume used          = key beep volume */
  UI_EARBEEP_SND,              /* Beep for low battery, minute beep etc.. */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ringer          */
                               /*  volume used          = ear volume      */
                               /*  in car kit volume    = ringer volume   */
  UI_ALERT_SND,                /* alert ring sound - includes signalling  */
                               /*  in use generator     = ringer          */
                               /*  not in use generator = ringer          */
                               /*  volume used          = ringer volume   */
  UI_CALL_SND,                 /* Call feedback sound ringer/earpiece     */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ringer          */
                               /*  volume used          = constant        */
  UI_CALLFAIL_SND,             /* Call feedback sound earpiece            */
                               /*  volume used          = constant        */

  UI_MSG_SND,                  /* Message notifications                   */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ringer          */
                               /*  volume used          = ringer volume   */
#ifdef FEATURE_UI_DTMF_THRU_EARPIECE
#error code not present
#endif
#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif
  UI_CALL_EAR_SND              /* CAll feedback sound earpiece/earpiece   */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ear piece       */
                               /*  volume used          = constant        */
                               /* these will be generated at the earpiece */
                               /* even if the phone is not in use         */
#ifdef FEATURE_UI_ALT_CALL_FAIL
#error code not present
#endif /* FEATURE_UI_ALT_CALL_FAIL */
#ifdef FEATURE_YMU757B_MELODY           // jwpark 00.08.04
 ,UI_MUSIC_ID_SND
 ,UI_MUSIC_BUFF_SND
 ,UI_MUSIC_FILE_SND
#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.08.04 end
} uisnd_snd_type;


#ifdef FEATURE_MULTIPLE_RINGER_TYPES
#error code not present
#endif /*  #ifdef FEATURE_MULTIPLE_RINGER_TYPES */


//redstar  2000.10.08
#if  (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) 


/*
**----------------------------------------------------------------------------
**	       SOUND STATE
**----------------------------------------------------------------------------
*/
typedef struct tagEFS_SNDSTATE_T  EFS_SNDSTATE_T;
typedef struct tagSNDSTATE_T      SNDSTATE_T;


struct  tagEFS_SNDSTATE_T
{
    BOOL     fAreaBeep;           //ui.areabeep
	BOOL     fFadeBeep;           //ui.fadebeep
	BOOL     fMinBeep;            //ui.minbeep		
    BOOL     fBurstDTMF;          //ui.burst	
	BOOL     fRoamRinger;         //ui.roam_ringer

};

struct tagSNDSTATE_T{

	BOOL     fShortBurstDTMF;         //ui.sburst
    BOOL     fEditVolume;             //ui.edvol
    BOOL     fMenuVolume;             //ui.menuvol
    BOOL     fClrAllBeep;             //ui.clrallbeep
    EFS_SNDSTATE_T SaveSNDState;

};

extern SNDSTATE_T       gSoundState;

//extern void EQS_GetSNDState(EFS_SNDSTATE_T* data);
//extern void EQS_SetSNDState(EFS_SNDSTATE_T* data);
extern BOOL GetSNDAreaBeep(void);
extern void SetSNDAreaBeep(BOOL fValue);
extern BOOL GetSNDFadeBeep(void);
extern void SetSNDFadeBeep(BOOL fValue);
extern BOOL GetSNDMinBeep(void);
extern void SetSNDMinBeep(BOOL fValue);
extern BOOL GetSNDBurst(void);
extern void SetSNDBurst(BOOL fValue);
extern BOOL GetSNDRoamRinger(void);
extern void SetSNDRoamRinger(BOOL fValue);
extern SNDSTATE_T* GetSNDData(void);
extern void SetSNDData(SNDSTATE_T* pSndState);



#define GET_SND_SHORTBURST              (  gSoundState.fShortBurstDTMF          )
#define SET_SND_SHORTBURST(fValue)      (  gSoundState.fShortBurstDTMF = fValue )
#define GET_SND_EDITVOLUME              (  gSoundState.fEditVolume              )
#define SET_SND_EDITVOLUME(fValue)      (  gSoundState.fEditVolume     = fValue )
#define GET_SND_MENUVOLUME              (  gSoundState.fMenuVolume              )
#define SET_SND_MENUVOLUME(fValue)      (  gSoundState.fMenuVolume     = fValue )
#define GET_SND_CLEARBEEP               (  gSoundState.fClrAllBeep              )
#define SET_SND_CLEARBEEP(fValue)       (  gSoundState.fClrAllBeep     = fValue )

#define GET_SND_AREABEEP                (  GetSNDAreaBeep()                     )
#define SET_SND_AREABEEP(fValue)        (  SetSNDAreaBeep(fValue)               )
#define GET_SND_FADEBEEP                (  GetSNDFadeBeep()                     )
#define SET_SND_FADEBEEP(fValue)        (  SetSNDFadeBeep(fValue)               )
#define GET_SND_MINBEEP                 (  GetSNDMinBeep()                      )
#define SET_SND_MINBEEP(fValue)         (  SetSNDMinBeep(fValue)                )
#define GET_SND_BURSTDTMF               (  GetSNDBurst()                        )
#define SET_SND_BURSTDTMF(fValue)       (  SetSNDBurst(fValue)                  )
#define GET_SND_ROAMRINGER              (  GetSNDRoamRinger()                   )
#define SET_SND_ROAMRINGER(fValue)      (  SetSNDRoamRinger(fValue)             )
#define GET_SND_DATA                    (  GetSNDData()                         )
#define SET_SND_DATA(pSndState)         (  SetSNDData(pSndState)                )



/*
**----------------------------------------------------------------------------
**	       SOUND SETTING
**----------------------------------------------------------------------------
*/
typedef struct tagSNDSETTING_T      SNDSETTING_T;
typedef struct tagEFS_SNDSETTING_T  EFS_SNDSETTING_T;

struct tagEFS_SNDSETTING_T
{
	BYTE bAlertLvl;            //ui.alerts_lvl
	BYTE bAlertLvlShadow;         //ui.alerts_lvl_shadow
	BYTE bHeadsetLvl;         //ui.beep_headset_lvl
	BYTE bBeepLvl;                //ui.beep_lvl
	BYTE bBeepSPKRLvl;             //ui.beep_spkr_lvl
	BYTE bEarHeadsetLvl;        //ui.ear_headset_lvl
	BYTE bEarLvl;               //ui.ear_lvl
	BYTE bEarSPKRLvl;            //ui.ear_spkr_lvl
	BYTE bMusicLvl;               //ui.music_lvl
	BYTE bRingerLvl;              //ui.ringer_level
	BYTE bRingerSPKRLvl;           //ui.ringer_spkr_lvl
};

struct tagSNDSETTING_T
{
    snd_tone_type       KeyDownDTMF;            //ui.key_down_dtmf
	snd_tone_type       KeyDownTone;            //ui.key_down_tone
	DWORD               dwStopDTMF;             //ui.needstop
    EFS_SNDSETTING_T    SaveSNDSetting;

};

extern SNDSETTING_T       gSNDSetting;

extern void EQS_GetSNDSetting(EFS_SNDSETTING_T* data);
extern void EQS_SetSNDSetting(EFS_SNDSETTING_T* data);


BYTE GetSTAlertLVL(void);
void SetSTAlertLVL(BYTE bValue);
BYTE GetSTAlertLVLShadow(void);
void SetSTAlertLVLShadow(BYTE bValue);
BYTE GetSTHeadsetLVL(void);
void SetSTHeadsetLVL(BYTE bValue);
BYTE GetSTBeepLVL(void);
void SetSTBeepLVL(BYTE bValue);
BYTE GetSTSPKRLVL(void);
void SetSTSPKRLVL(BYTE bValue);
BYTE GetSTEarHeadsetLVL(void);
void SetSTEarHeadsetLVL(BYTE bValue);
BYTE GetSTEarLVL(void);
void SetSTEarLVL(BYTE bValue);
BYTE GetSTEarSPKRLVL(void);
void SetSTEarSPKRLVL(BYTE bValue);
BYTE GetSTMusicLVL(void);
void SetSTMusicLVL(BYTE bValue);
BYTE GetSTRingerLVL(void);
void SetSTRingerLVL(BYTE bValue);
BYTE GetSTRingerSPKRLVL(void);
void SetSTRingerSPKRLVL(BYTE bValue);

SNDSETTING_T* GetSTData(void);
void SetSTData(SNDSETTING_T* pSNDSetting);



#define  GET_ST_KEYDOWNDTMF                 ( gSNDSetting.KeyDownDTMF               )
#define  SET_ST_KEYDOWNDTMF(KeyDownDTMF)    ( gSNDSetting.KeyDownDTMF = KeyDown     )
#define  GET_ST_KEYDOWNTONE                 ( gSNDSetting.KeyDownTone               )
#define  SET_ST_KEYDOWNTONE(KeyDownTone)    ( gSNDSetting.KeyDownTone = KeyDownTone )
#define  GET_ST_STOPDTMF                    ( gSNDSetting.dwStopDTMF                )
#define  SET_ST_STOPDTMF(dwValue)           ( gSNDSetting.dwStopDTMF  = dwValue     )


#define  GET_ST_ALERTLVL                    ( GetSTAlertLVL()                       )
#define  SET_ST_ALERTLVL(bValue)            ( SetSTAlertLVL(bValue)                 )
#define  GET_ST_ALERTLVL_SHADOW             ( GetSTAlertLVLShadow()                 )
#define  SET_ST_ALERTLVL_SHADOW(bValue)     ( SetSTAlertLVLShadow(bValue)           )
#define  GET_ST_HEADSET_BEEPLVL             ( GetSTHeadsetLVL()                     )
#define  SET_ST_HEADSET_BEEPLVL(bValue)     ( SetSTHeadsetLVL(bValue)               )
#define  GET_ST_BEEPLVL                     ( GetSTBeepLVL()                        )
#define  SET_ST_BEEPLVL(bValue)             ( SetSTBeepLVL(bValue)                  )
#define  GET_ST_SPKR_BEEPLVL                ( GetSTSPKRLVL()                        )
#define  SET_ST_SPKR_BEEPLVL(bValue)        ( SetSTSPKRLVL(bValue)                  )
#define  GET_ST_EAR_HEADSETLVL              ( GetSTEarHeadsetLVL()                  )
#define  SET_ST_EAR_HEADSETLVL(bValue)      ( SetSTEarHeadsetLVL(bValue)            )
#define  GET_ST_EARLVL                      ( GetSTEarLVL()                         )
#define  SET_ST_EARLVL(bValue)              ( SetSTEarLVL(bValue)                   )
#define  GET_ST_EAR_SPKRLVL                 ( GetSTEarSPKRLVL()                     )
#define  SET_ST_EAR_SPKRLVL(bValue)         ( SetSTEarSPKRLVL(bValue)               )
#define  GET_ST_MUSICLVL                    ( GetSTMusicLVL()                       )
#define  SET_ST_MUSICLVL(bValue)            ( SetSTMusicLVL(bValue)                 )
#define  GET_ST_RINGERLVL                   ( GetSTRingerLVL()                      )
#define  SET_ST_RINGERLVL(bValue)           ( SetSTRingerLVL(bValue)                )
#define  GET_ST_RINGER_SPKRLVL              ( GetSTRingerSPKRLVL()                  )
#define  SET_ST_RINGER_SPKRLVL(bValue)      ( SetSTRingerSPKRLVL(bValue)            )

#define  GET_ST_DATA                        ( GetSTData()                           )
#define  SET_ST_DATA(pSNDSetting)           ( SetSTData(pSNDSetting)                )

#ifndef WIN32
#include "eqs_file.h"
#endif  //WIN32
#endif


/* <EJECT> */
/*===========================================================================
FUNCTION UISND_VOICE_VOL

DESCRIPTION
  Set the voice volume.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
extern void uisnd_voice_vol(
  byte vol,                         /* the volume                  */
  nv_items_enum_type nv_item                      /* nv item indicating the path */
);
#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif

#ifdef FEATURE_REMOTE_RINGER
#error code not present
#endif

/*===========================================================================
FUNCTION UISND_BEEP_VOL

DESCRIPTION
  Set the beep volume.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
extern void uisnd_beep_vol(
  byte vol,                         /* the volume                  */
  nv_items_enum_type nv_item                      /* nv item indicating the path */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISND_alert_VOL

DESCRIPTION
  Set the SMS volume.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
extern void uisnd_alert_vol(
  byte vol,                         /* the volume                  */
  nv_items_enum_type nv_item                      /* nv item indicating the path */
);


#if (MODEL_ID == MODEL_A10)
/*===========================================================================
FUNCTION UISND_SMS_VOL

DESCRIPTION
  Set the sms melody volume level.
===========================================================================*/
extern void uisnd_sms_vol(byte vol, nv_items_enum_type nv_item);


/*===========================================================================
FUNCTION UISND_ALARM_VOL

DESCRIPTION
  Set the alarm melody volume level.
===========================================================================*/
extern void uisnd_alarm_vol(byte vol, nv_items_enum_type nv_item);
#endif


/* <EJECT> */
/*===========================================================================
FUNCTION UISND_RING_VOL

DESCRIPTION
  Set the ring volume

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
extern void uisnd_ring_vol(
  byte vol,                         /* the volume                  */
  nv_items_enum_type nv_item                      /* nv item indicating the path */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISND_SET_PATH

DESCRIPTION
  Set the normal input and output path.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_set_path(
  boolean  mute_in,        /* which input path       */
  boolean  mute_out,       /* which output path      */
  snd_device_type         device         /* if out path includes voice */
);
/* <EJECT> */
/*===========================================================================
FUNCTION UISND_SET_DEVICE

DESCRIPTION
  Set the normal input and output path.
  DMSS5000에 있는 uisnd_set_path()로는 우리(?)가 원하는 path설정 기능을
  충족시키기에는 너무 허술해서 새로 만들었다.

  * A7/A10/B0/B1이 공통으로 사용할 수 있게 되었다.
===========================================================================*/
void uisnd_set_device(snd_mute_control_type speaker_mute,
					  snd_mute_control_type microphone_mute,
					  boolean				change_mute);

/*===========================================================================
FUNCTION uisnd_PATH_DTMF

DESCRIPTION
  Set the normal DTMF path

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_path_dtmf(
  snd_dtmf_apath_type path        /* which path */
);


#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */


/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_SND

DESCRIPTION
  Play a sound

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_snd(
  uisnd_snd_type    type,         /* type of sound */
  snd_predef_type   snd           /* which sound */
);

/*===========================================================================
FUNCTION uisnd_sample_snd

DESCRIPTION
  Play a sample sound

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_sample_snd(
  uisnd_snd_type    type,         /* type of sound */
  snd_predef_type   snd,          /* which sound */
  byte              vol
);

/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_SND_STOP

DESCRIPTION
  Stop any sound

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_snd_stop( void );


/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_MULTI

DESCRIPTION
  Play a multitone

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_multi(
  boolean         dotx,           /* transmit over air? */
#ifdef FEATURE_UI_ALT_CALL_FAIL
#error code not present
#endif /* FEATURE_UI_ALT_CALL_FAIL */
  snd_type        *sound,         /* list of tones */
  boolean         received        /* Multi tones received from BS */
);

/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_MULTI_STOP

DESCRIPTION
  Stop any multi tone

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_multi_stop( void );


/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_TONE

DESCRIPTION
  Play a tone

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_tone(
  uisnd_snd_type  type,           /* type of sound */
  snd_tone_type   tone,           /* which tone */
  word            time            /* duration in ms */
);

/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_sample_tone

DESCRIPTION
  Play a sample tone while adjusting volumes.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_sample_tone(
  uisnd_snd_type  type,           /* type of sound */
  snd_tone_type   tone,           /* which tone */
  word            time,           /* duration in ms */
  byte            vol             /* volume to be played in */
);

#ifdef FEATURE_REMOTE_RINGER
#error code not present
#endif /* FEATURE_REMOTE_RINGER */

/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_DTMF

DESCRIPTION
  Play a dtmf - a special tone which may be transmitted, and is always
  played out the speaker

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_dtmf(
  snd_tone_type   tone,           /* which dtmf */
  word            time            /* duration in ms */
);

#ifdef FEATURE_REMOTE_RINGER
#error code not present
#endif


/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_TONE_STOP

DESCRIPTION
  Stop any tone

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_tone_stop( void );



#ifdef FEATURE_VOICEPLAY // jwpark 00.11.06
void uisnd_voice(byte *, word, word);
void uisnd_sample_voice(byte *, word, word, word);
void uisnd_voice_stop(void);
#endif



/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_SND_INIT

DESCRIPTION
  Initialize ui -> snd buffers

DEPENDENCIES
  Called once, by UI.C to init buffers
===========================================================================*/
void uisnd_snd_init( void );

/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_STEREO_MUTE

DESCRIPTION
  Stereo mute/unmute.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_stereo_mute( boolean smute );

#ifdef FEATURE_UI_ALT_CALL_FAIL
#error code not present
#endif /* FEATURE_UI_ALT_CALL_FAIL */


#ifdef FEATURE_DS
/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_ADATA_CONFIG

DESCRIPTION
  Configure sound to handle an AMPS Data call via the PCMCIA card.
  This involves turning VOXing off, filtering off, unity gain off.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void uisnd_adata_config ( boolean adata_call );
#endif /* ifdef FEATURE_DS */

#ifdef FEATURE_UI_ESCALATING_RING
#error code not present
#endif /* ifdef FEATURE_UI_ESCALATING_RING */

#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif /* ifdef FEATURE_AUDIO_CONTROL */

/* <EJECT> */
/*===========================================================================
FUNCTION UISND_KEY_SOUND

DESCRIPTION
  Do the beep associated with a key being pressed.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void uisnd_key_sound(word key,word time);

/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_play_snd

DESCRIPTION
  a wrapper for playing sound in handset/module

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void uisnd_play_snd(
  uisnd_snd_type          type,  // type of sound
  snd_predef_type         snd,   // which sound
  mod_sys_wake_event_type event  // module event, used only if MODULE defined
);



/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_multi_tone

DESCRIPTION
  a wrapper for playing multitones in handset/module

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void uisnd_multi_tone(
  boolean        over_the_air,     // transmit over the air ?
  uisnd_snd_type vol_type,         // type of sound for vol setting
  word           num_tones,        // # of tones
  snd_type*      snds,             // buffer containing tones
  mod_sys_wake_event_type event,   // module event, used only if MODULE defined
  boolean        received          // received tones from base station
);


/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_call_fail

DESCRIPTION
  play call fail sound

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void uisnd_call_fail(snd_predef_type snd);



/* <EJECT> */
/*===========================================================================
FUNCTION uisnd_play_signal

DESCRIPTION
  play tone signal or ISDN alert or IS-54B alert

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void uisnd_play_signal(
  byte signal_type,        // tone signal/ISDN alerting/IS-54B alert
  byte signal,             // indicate which signals to play
  byte pitch               // medium, high or low
);

#endif /* FEATURE_NEW_SOUND_API */


#ifdef FEATURE_YMU757B_MELODY	// jwpark 00.07.25
#include "MusicDb.h"

void uisnd_music(
  uisnd_snd_type type,
  char          *file_name,
  uint16        *music_buf,
  mus_id_type    music_id,
  uint16         volume_lvl
);

void uisnd_music_with_repeat(
  uisnd_snd_type type,
  char          *file_name,
  uint16        *music_buf,
  mus_id_type    music_id,
  uint16         volume_lvl,
  uint16         repeat_cnt
);

void uisnd_sample_music(
  uisnd_snd_type type,
  char          *file_name,
  uint16        *music_buf,
  mus_id_type    music_id,
  uint16         volume_lvl,
  uint16		 repeat_cnt
);

#ifdef FEATURE_BGM
void uisnd_BGM(
  uisnd_snd_type type,
  char          *file_name,
  uint16        *music_buf,
  mus_id_type    music_id,
  uint16         volume_lvl
);
#endif

boolean uisnd_play_file(
	char	*file_name,
	byte	volume,
	word	repeat_cnt
);

void uisnd_music_stop(void);

void uisnd_play_sound(nv_mel_option_type *pMelOpt);
void uisnd_play_alarm(nv_mel_option_type *pMelOpt);
void uisnd_play_sms(nv_mel_option_type *pMelOpt);

void uisnd_play_sample_sound(nv_mel_option_type *pMelOpt, byte volume, word RepeatCount );

#endif // FEATURE_YMU757B_MELODY	// jwpark 00.07.25 end



#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
/*===============================================================================

                                  EQUS UI Variables
	                                   (Sound)

=================================================================================*/
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
//typedef struct tagSOUNDSETTING      eqsSndSetting;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

#if 0
/**----------------------------------------------------------------------------
**	          Sound Setting (Volumen Setting, Key tone....) 
**---------------------------------------------------------------------------*/
struct tagSOUNDSETTING{
	byte            SMSAlertLvl;            //ui.alerts_lvl
	byte            SMSHFKAlertLvl;         //ui.alerts_lvl_shadow
	byte            HeadsetBeepLvl;         //ui.beep_headset_lvl
	byte            BeepLvl;                //ui.beep_lvl
	byte            HFKBeepLvl;             //ui.beep_spkr_lvl
	byte            HeadsetVoiceLvl;        //ui.ear_headset_lvl
	byte            VoiceLvl;               //ui.ear_lvl
	byte            HFKVoiceLvl;            //ui.ear_spkr_lvl
	snd_tone_type   KeyDownDTMF;            //ui.key_down_dtmf
	snd_tone_type   KeyDownTone;            //ui.key_down_tone
	byte            MusicLvl;               //ui.music_lvl
	dword           StopDTMF;               //ui.needstop
	byte            RingerLvl;              //ui.ringer_level
	byte            HFKRingerLvl;           //ui.ringer_spkr_lvl
};
#endif

/*
**--------------------------------------------------------------------------------
**  Variable Declarations
**--------------------------------------------------------------------------------
*/
//extern eqsSndSetting    gSndSet;             //defined in eqsqsnd.c

#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
#endif  /* #ifndef UINSND_H */

