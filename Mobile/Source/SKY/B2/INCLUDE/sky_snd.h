/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     sky_snd.h
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
** ============================================================================
*/

#ifndef _SKY_SND_H_
#define _SKY_SND_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "mod.h"

#include "snd.h"

#include "sky.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/*
** 음량 조절할 때 UP/DOWN arrow 키톤이 발생하지 않게 하기 위해
** 음량 조절하기 전에는 항상 다음 매크로를 불러주어야 한다.
*/
#define SKY_ENTER_VOLUME_ADJUST()       g_fVolumeAdjust = TRUE
#define SKY_EXIT_VOLUME_ADJUST()        g_fVolumeAdjust = FALSE
#define SKY_IS_VOLUME_ADJUST()          g_fVolumeAdjust

#define SKY_SET_KEY_TONE(f)             g_fKeyTone = f
#define SKY_IS_KEY_TONE()               g_fKeyTone

/* ---------------------------------------------------------------------------
** sky_hkey.c와 sky_snd.c 사이에서만사용 됨 
*/
#define GET_KEY_DOWN_DTMF()             g_KeyDownDTMF
#define GET_KEY_DOWN_TONE()             g_KeyDownTone

#define SET_KEY_DOWN_DTMF(SoundTone)    g_KeyDownDTMF = SoundTone
#define SET_KEY_DOWN_TONE(SoundTone)    g_KeyDownTone = SoundTone

/*
** 볼륨 정책 :
** 볼륨을 경우에 따라 다르게 낼 필요가 있을까?
** --> 볼륨 설정 함수를 따로 두고, 다른 함수들에서는 이 볼륨값을 이용하도록 하자.
** 
** UI 쪽에서는 직접 이 매크로를 부르지 않는다.
*/

//#define SKY_SET_MUSIC_VOLUME(nVol)      g_wMusicVolume = nVol
//#define SKY_GET_MUSIC_VOLUME()          g_wMusicVolume

/*
** 
** FEATURE_NEW_SOUND_API
**
** --------------------------------------------------------
** 3100                     5000
** --------------------------------------------------------
** TRUE                     FALSE
** uisnd_set_device         uisnd_set_path
**                          uisnd_set_path_dtmf
**
*/

/* Used to make a standard error beep from the UI state machine */

#define SKY_ERROR_TONE()        SKY_PlayTone( UI_BEEP_SND, SND_ERR, 300 )

/* make a standard clearall tone */

#define SKY_CLEAR_ALL_TONE()    SKY_PlayTone( UI_BEEP_SND, SND_CTRL, 200 );

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

#define UISND_1ST_VOL           1
#define UISND_2ND_VOL           2
#define UISND_3RD_VOL           3
#define UISND_4TH_VOL           4
#define UISND_5TH_VOL           5

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE  

#define UISND_1ST_VOL           0x1F
#define UISND_2ND_VOL           0x3F
#define UISND_3RD_VOL           0x5F
#define UISND_4TH_VOL           0x7F
#define UISND_5TH_VOL           0x9F

#endif

/* Average default volume if there is none defined */

/* whether outpath gets voice or not */
#define UISND_NOVOICE           TRUE
#define UISND_VOICE             FALSE

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/* cai.h */
/* ----------------
CAI_PITCH_MED  0x00  // Medium pitch (standard alert)
CAI_PITCH_HIGH 0x01  // High pitch 
CAI_PITCH_LOW  0x02  // Low pitch 
CAI_PITCH_RSV  0x03  // Reserved pitch 
*/
enum tagPITCH_LEVEL_E
{
    PL_MEDIUM = CAI_PITCH_MED, 
    PL_HIGH   = CAI_PITCH_HIGH, 
    PL_LOW    = CAI_PITCH_LOW
};
typedef enum tagPITCH_LEVEL_E PITCH_LEVEL_E;

/* snd.h */
typedef snd_tone_type           SOUND_TONE_E;
typedef snd_device_type         SOUND_DEVICE_E;

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
  UI_CALL_EAR_SND,             /* CAll feedback sound earpiece/earpiece   */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ear piece       */
                               /*  volume used          = constant        */
                               /* these will be generated at the earpiece */
                               /* even if the phone is not in use         */
#ifdef FEATURE_UI_ALT_CALL_FAIL
#error code not present
#endif /* FEATURE_UI_ALT_CALL_FAIL */
#ifdef FEATURE_YMU757B_MELODY           // jwpark 00.08.04
  UI_MUSIC_ID_SND,
  UI_MUSIC_BUFF_SND,
  UI_MUSIC_FILE_SND,
  
#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.08.04 end
#ifdef FEATURE_YMU759_MELODY
  UI_MUSIC_ID_SND,
  UI_MUSIC_BUFF_SND,
  UI_MUSIC_FILE_SND,
#endif // #ifdef FEATURE_YMU759_MELODY
  
#ifdef YAMAHA_MELODY
    /// Added by happykid 99.12.20
    UI_MUSIC_SND,              /* CAll feedback sound earpiece/earpiece   */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ear piece       */
                               /*  volume used          = constant        */
                               /* these will be generated at the earpiece */
                               /* even if the phone is not in use         */

   UI_ALARM_SND, // alram sound
   UI_SMS_MSG_SND, // sms 
#endif
  /* last value */
  UI_MUSIC_MAX
} uisnd_snd_type;

typedef uisnd_snd_type      SOUND_TYPE_E;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern SOUND_TONE_E    g_KeyDownDTMF;
extern SOUND_TONE_E    g_KeyDownTone;
extern BOOL            g_fKeyTone;
extern BOOL            g_fVolumeAdjust;
extern WORD            g_wMusicVolume;


/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
// 착신벨을 play하기 위한 함수
// 벨설정등을 고려하여 설계됨
extern void SKY_API SKY_DoAlertSignal(void);
extern void SKY_API SKY_EndAlertSignal(void);
extern void SKY_API SKY_PlayBellAfterVib(void);
// SMS 알람벨
extern void EQS_API SKY_DoSMSAlert(void);
//카메라 shutter소리
extern void EQS_API SKY_DoShutter(void);
// 알람/스케쥴러 벨
extern void EQS_API SKY_PlayAlarmBell(void);
extern void EQS_API SKY_PlaySchedulerBell(void);

// Motor START/STOP (nDuration : 진동주기)
extern void SKY_API SKY_MotorStart(UINT16 nDuration);
extern void SKY_API SKY_MotorStop(void);
extern void SKY_API SKY_MotorTime(UINT16 nDuration);

// Volume Level
extern void SKY_API SKY_SetBeepVolume(INT8 nVolume);
extern void SKY_API SKY_SetRingerAlertVolume(INT8 nVolume);
extern void SKY_API SKY_SetSMSAlertVolume(INT8 nVolume);
extern void SKY_API SKY_SetAlarmAlertVolume(INT8 nVolume);
extern void SKY_API SKY_SetVoiceVolume(INT8 nVolume);
extern void SKY_API SKY_SetMicLevel(MIC_VOLUME_E Volume);


extern void SKY_API SKY_SetSoundPath(BOOL fMuteIn, BOOL fMuteOut, 
                                     SOUND_DEVICE_E Device);
extern void SKY_API SKY_SetSoundDevice(SOUND_MUTE_CONTROL_E SpeakerMute, 
                                       SOUND_MUTE_CONTROL_E MicMute,
                                       BOOL                 fChangeMute);


#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
extern void SKY_API SKY_PlaySound(SOUND_TYPE_E SoundType, 
                                  SOUND_PREDEF_E Sound);
extern void SKY_API SKY_PlaySoundEx(SOUND_TYPE_E SoundType, 
                                        SOUND_PREDEF_E Sound, INT8 nVolume,
                                        UINT16 nRepeatCount);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
extern void SKY_API SKY_PlaySound(SOUND_TYPE_E SoundType, SOUND_ID_E Sound);
extern void SKY_API SKY_PlaySoundEx(SOUND_TYPE_E SoundType, 
                                    SOUND_ID_E Sound, INT8 nVolume, 
                                    UINT16 nRepeatCount);
#endif

extern void SKY_API SKY_StopSound(void);

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
extern void SKY_API SKY_PlayMultiTone(BOOL fOverTheAir, 
                                      SOUND_TYPE_E SoundType,
                                      WORD wNumTones, 
                                      DTMF_TONE_T *pToneList,
                                      SYS_WAKE_EVENT_E SysWakeEvent, 
                                      BOOL fDTMFFromBS);

extern void SKY_API SKY_PlayVoice(BYTE *pVoiceBuffer,
				           WORD nVoiceLength,
				           WORD nCount);
extern void SKY_API SKY_MusicPowerOff(void);

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
extern void SKY_API SKY_PlayMultiTone(BOOL fOverTheAir, 
                                      SOUND_TYPE_E SoundType,
                                      WORD wNumTones, 
                                      SOUND_COMPACT_TONE_T *pToneList,
                                      SYS_WAKE_EVENT_E SysWakeEvent, 
                                      BOOL fDTMFFromBS);

extern void SKY_API SKY_PlayVoice(BYTE *pVoiceBuffer, 
                           UINT16 nVoiceLength, 
                           UINT8 nCount);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE

extern void SKY_API SKY_PlayMusicEx(MUSIC_ID_E MusicID, WORD wVolume, WORD nRepeatCount);
extern void SKY_API SKY_StopVoice(void);

#ifdef FEATURE_WAVELET
extern boolean MM_StartAudio(void);
extern boolean MM_EndAudio(void);
#endif // FEATURE_WAVELET

extern void SKY_API SKY_StopMultiTone(void);
extern void SKY_API SKY_PlayTone(SOUND_TYPE_E SoundType,
                                 SOUND_TONE_E SoundTone,
                                 WORD wDurationMS);
extern void SKY_API SKY_PlayToneEx(SOUND_TYPE_E SoundType,
                            SOUND_TONE_E SoundTone,
                            WORD         wDurationMS,
                            INT8         nVolume);
extern void SKY_API SKY_PlaySampleTone(SOUND_TYPE_E SoundType,
                                       SOUND_TONE_E SoundTone,
                                       WORD         wDurationMS,
                                       INT8         nVolume);
extern void SKY_API SKY_PlayDTMF(SOUND_TONE_E  SoundTone, WORD wDurationMS);
extern void SKY_API SKY_StopTone(void);

extern void SKY_API SKY_InitSound(void);
extern void SKY_API SKY_SetStereoMute(BOOL fMute);
extern void SKY_API SKY_ConfigAMPSDataSound(BOOL fAMPSDataCall);
extern void SKY_API SKY_PlayKeySound(SKY_EVENT_E Key, WORD wDurationMS);
extern void SKY_API SKY_PlaySoundInHandset(SOUND_TYPE_E SoundType, 
                                           SOUND_PREDEF_E Sound, 
                                           SYS_WAKE_EVENT_E WakeEvent);
extern void SKY_API SKY_PlayCallFailSound(SOUND_PREDEF_E Sound);
extern void SKY_API SKY_PlaySignal(SIGNAL_TYPE_E SignalType, 
                                   TONE_SIGNAL_E Signal, 
                                   PITCH_LEVEL_E Pitch);
extern void SKY_API SKY_ControlMusicVolume(WORD wVolume);
extern void SKY_API SKY_PlayAlertSound(UINT16 nDuration );
extern void SKY_API SKY_PlayMusic(MUSIC_ID_E MisicID);
extern void SKY_API SKY_StopMusic(void);
extern void SKY_API SKY_PlayMusicTime(MUSIC_ID_E MusicID, UINT16 nDuration);
extern void SKY_API SKY_PlayMusicFile(BYTE *pszFileName);
extern void SKY_API SKY_PlayMusicFileEx(BYTE *pszFileName, WORD wVolume);
extern void SKY_API SKY_PlayMusicFileTime(BYTE *pszFileName, UINT16 nDuration);

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
extern void SKY_API SKY_PlayMusicBuffer(BYTE *pbMusicBuf);
extern void SKY_API SKY_PlayMusicBufferEx(BYTE *pbMusicBuf, WORD wVolume, WORD nRepeatCount);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
extern void SKY_API SKY_PlayMusicBuffer(WORD *pwMusicBuf);
extern void SKY_API SKY_PlayMusicBufferEx(WORD *pwMusicBuf, WORD wVolume);
#endif

// 단순히 Motor ON/OFF를 위한 함수임.
extern void SKY_API SKY_MotorOn(void);
extern void SKY_API SKY_MotorOff(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_SND_H_ */

