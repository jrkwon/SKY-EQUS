/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_snd.c
**
** Description: 
**     3100 based sound interface
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
#include "ska_vcmm.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define UI_HORN_ALERT()

#define IS54_LAST 0x0b
#define TONE_LAST 2

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

extern boolean ui_is_in_conversation(void); // in ska_call.c
extern BOOL SKY_IsInCall(void);


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
SOUND_TONE_E    g_KeyDownDTMF = SND_FIRST_TONE;
SOUND_TONE_E    g_KeyDownTone = SND_FIRST_TONE;
BOOL g_fKeyTone = FALSE;
BOOL g_fVolumeAdjust = FALSE;
WORD g_wMusicVolume;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL SOUND_DEVICE_E    l_OutDevice;

// Current Play info.
LOCAL BOOL          fPlayBell;
LOCAL BELL_TYPE_T   CurrentBellType;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
LOCAL void SKY_API sPlayMultiTone(BOOL fOverTheAir, 
                                  DTMF_TONE_T *pToneList, 
                                  BOOL fDTMFFromBS);

LOCAL void SKY_API sChooseVolume(SOUND_TYPE_E SoundType, 
                                 SOUND_VOLUME_T *pVolume);
LOCAL SOUND_CLASS_E SKY_API sChooseClass(SOUND_TYPE_E SoundType);

LOCAL void SKY_API sPlayMusic(SOUND_TYPE_E SoundType, BYTE *pszFileName,
                              BYTE *pbMusicBuf, MUSIC_ID_E MusicID,
                              WORD wRepeatCount,
                              WORD wVolume,
                              BOOL fSampleSound);

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
LOCAL void SKY_API sPlayMultiTone(BOOL fOverTheAir, 
                                  SOUND_COMPACT_TONE_T *pToneList, 
                                  BOOL fDTMFFromBS);

void SKY_API sPlayMusicBuffer(WORD *pwMusicBuf, WORD wVolume, 
                              BOOL fVolumeSet);

LOCAL void SKY_API sCallbackFunction1(void *pClientData, SOUND_STATUS_E Status);
LOCAL void SKY_API sPlayMusic(BYTE *pszFileName,
                              SOUND_ID_E MusicID,
                              UINT8 wRepeatCount,
                              WORD wVolume);
#endif

LOCAL SOUND_DEVICE_E SKY_API sChooseDevice(SOUND_TYPE_E SoundType);

//LOCAL void EQS_API sPlaySigTone(BYTE Signal);
//LOCAL void EQS_API sPlaySigISDN(BYTE Signal);
LOCAL void SKY_API sPlayToneSignal(TONE_SIGNAL_E Signal);
LOCAL void SKY_API sPlaySoundOrVibrator(SOUND_TYPE_E SoundType,
                                        SOUND_PREDEF_E Sound,
                                        SYS_WAKE_EVENT_E WakeEvent);
LOCAL void SKY_API sPlayISDNAlert(ISDN_ALERT ISDNAlert);
LOCAL void SKY_API sPlayIS54BAlert(PITCH_LEVEL_E Pitch, 
                                   TONE_SIGNAL_E Signal);

LOCAL WORD SKY_API sIntToLevel(UINT8 nLevel);

LOCAL void EQS_API sPlaySigCall(void);
LOCAL void EQS_API sPlaySigDataCall(void);
LOCAL void EQS_API sPlayAutoAnswerAlert(void);

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

/*===========================================================================
FUNCTION MOTOR_ON

DESCRIPTION
  Start/Stop Motor
  
DEPENDENCIES
  Send command to hs task
===========================================================================*/
void SKY_API SKY_MotorStart(UINT16 nDuration)
{
    if(SKY_IS_HANDS_FREE()) // 핸즈프리일때는 진동하면 안됨
        return;

    if(nDuration > 0)
    {
        // Start Motor off cb timer
        SKY_SetMotorOnTimer(nDuration);
    }

    SKY_MotorOn();
}

void SKY_API SKY_MotorStop(void)
{
    SKY_MotorOff();
    SKY_SetMotorOnTimer(0);
}

void SKY_API SKY_MotorTime(UINT16 nDuration)
{
    if(SKY_IS_HANDS_FREE()) // 핸즈프리일때는 진동하면 안됨
        return;

    if(nDuration > 0)
    {
        // Start Motor off cb timer
        SKY_SetMotorOffTimer(nDuration);
    }

    SKY_MotorOn();
}
///////////////////////////////////////////////////////////
//  단순히 진동을 On/Off 시킬 용도로만 사용
////////////////////////////////////////////////////////////
void SKY_API SKY_MotorOn(void)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_MOTOR_ON;
        pCommand->Header.pTask = NULL;
        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't get hs buffer");
    }
}

void SKY_API SKY_MotorOff(void)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_MOTOR_OFF;
        pCommand->Header.pTask = NULL;
        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't get hs buffer");
    }
}

/*===========================================================================
FUNCTION CHOOSE_PATH

DESCRIPTION
  Choose the correct output path
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
LOCAL SOUND_DEVICE_E SKY_API sChooseDevice(SOUND_TYPE_E SoundType)
{
    SOUND_DEVICE_E  SoundDevice;

    switch(SoundType)
    {
    case UI_BEEP_SND:
    case UI_ALERT_SND:
    case UI_EARBEEP_SND:
    case UI_MSG_SND:
    case UI_CALL_SND:
    case UI_CALLFAIL_SND:
    case UI_CALL_EAR_SND:
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
        SoundDevice = SND_DEVICE_CURRENT;  /* use default path */
        break;
    case UI_PWRUP_SND:
        SoundDevice = SND_DEVICE_EARPIECE;
        break;
    default:
        SKY_ERROR1("Bad sound type in choose_path type = %d", SoundType);
        SoundDevice = SND_DEVICE_EARPIECE;
        break;

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
		if( SKY_IS_HANDS_FREE() && SKY_IS_EAR_JACK_OFF() 
          && ( ui_is_in_conversation() || SKY_IS_ORIGINATE() )
            )
			SoundDevice = SND_DEVICE_HFK;  
		else
			SoundDevice = SND_DEVICE_CURRENT;  
        break;

    case UI_MUSIC_SND:
        //착신벨이면 loud speaker와 ear-phone을 이용한다. 에티켓만 아니면.
		if( SKY_IS_ALERT() && !SKY_IS_ETIQUETTE() )
			SoundDevice = SND_DEVICE_LOUDSPK;  
		else
			SoundDevice = SND_DEVICE_CURRENT;  
		break;

	case UI_ALARM_SND:
	case UI_SMS_MSG_SND:
        //착신벨이면 loud speaker와 ear-phone을 이용한다. 에티켓만 아니면.
		if(!SKY_IS_ETIQUETTE())
			SoundDevice = SND_DEVICE_LOUDSPK;  
		else
			SoundDevice = SND_DEVICE_CURRENT;  
		break;

    case UI_PWRUP_SND:
        SoundDevice = SND_DEVICE_HANDSET;
        break;
    default:
        SKY_ERROR1("Bad sound type in choose_path type = %d", SoundType);
        SoundDevice = SND_DEVICE_HFK;
        break ;
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE
    }
    return SoundDevice;
}

/* <EJECT> */
/*===========================================================================
FUNCTION CHOOSE_VOL

DESCRIPTION
  Choose the correct volume
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

LOCAL void SKY_API sChooseVolume(SOUND_TYPE_E SoundType, SOUND_VOLUME_T *pVolume)
{
    SOUND_VOLUME_T  KeyToneVolume;
    SOUND_VOLUME_T  AlertVolume;
    SOUND_VOLUME_T  SpeakerVolume;

    /* 
    ** SKY use ear jack, earpeice and speaker volume level as a same value 
    */
    
    /* initial volume structure */
    KeyToneVolume.phone = KeyToneVolume.ext 
                        = KeyToneVolume.priv 
                        = sIntToLevel(SKY_GET_KEY_TONE_VOLUME());

    AlertVolume.phone = AlertVolume.ext 
                      = AlertVolume.priv 
                      = sIntToLevel(SKY_GET_BELL_VOLUME());

    SpeakerVolume.phone = SpeakerVolume.ext 
                        = SpeakerVolume.priv 
                        = sIntToLevel(SKY_GET_SPEAKER_VOLUME());
    
    switch(SoundType)
    {
    case UI_PWRUP_SND:            /* power up beep */
    case UI_BEEP_SND:             /* key beep */
        /* Key beep volume will be used for both when in a call, and when in */
        /* idle.                                                             */
        SKY_TRACE(TL_HIGH, "Using key beep level");
        *pVolume = KeyToneVolume;
        break;

    case UI_ALERT_SND:
        /* Ringer volume will be used for both when in a call, and when in   */
        /* idle.                                                             */
        SKY_TRACE(TL_HIGH, "Using key beep level");
        *pVolume = KeyToneVolume;
        break ;

    case UI_MSG_SND:
        SKY_TRACE(TL_HIGH, "Using key beep level");
        *pVolume = KeyToneVolume;
        break;

    case UI_CALL_SND:
    case UI_MUSIC_ID_SND:
        /* When not in a call we will use the ringer volume and generate the */
        /* sounds at the ringer. When we are in a call we will use the ear   */
        /* volume and generate the sound at the earpiece.                    */

        /* in call */
        if(SKY_IsInCall())
        {
            SKY_TRACE(TL_HIGH, "Using ringer level");
            *pVolume = AlertVolume;
        }
        /* not in call */
        else
        {
            SKY_TRACE(TL_HIGH, "Using ear level");
            *pVolume = SpeakerVolume;
        }
        break;

    case UI_CALLFAIL_SND:
        SKY_TRACE(TL_HIGH, "Using ear level");
        *pVolume = SpeakerVolume;
        break;

    case UI_CALL_EAR_SND:
        /* We will always use the ear volume for these sounds. */
        SKY_TRACE(TL_HIGH, "Using ear level");
        *pVolume = SpeakerVolume;
        break;

    case UI_EARBEEP_SND:     /* beep in ear */
        /* Volume used will be ear volume whether we are in a call or not   */
        SKY_TRACE(TL_HIGH, "Using ear level");
        *pVolume = SpeakerVolume;
        break;

    default:
        SKY_ERROR1("Bad sound type %d", SoundType);
        /*
        ** using a low level in the original code 
        ** but, using just ear level (=speaker) in SKY
        */
        SKY_TRACE(TL_HIGH, "Using a lower level");
        *pVolume = SpeakerVolume;
    }
}
#endif // #if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

/*===========================================================================
FUNCTION CHOOSE_CLASS

DESCRIPTION
  Choose the correct sound class

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

LOCAL SOUND_CLASS_E SKY_API sChooseClass(SOUND_TYPE_E SoundType)
{
    SOUND_CLASS_E SoundClass;

#ifdef FEATURE_DS
    if(IsDataCall())
    {
        /* If we are in a data call, we must use the ringer as the generator */
        SKY_TRACE(TL_HIGH, "Data call : Using SND_CLASS_BEEP");
        SoundClass = SND_CLASS_BEEP ;
        return SoundClass;
    }
#endif

    switch(SoundType) 
    {
    case UI_BEEP_SND:             /* key beep */
    case UI_PWRUP_SND:            /* power up beep */
        /* We will use the ringer for sound generator whether we are in a   */
        /* call or not. Also we will use the volume level that is specified */
        /* in the sound command.                                            */
        SKY_TRACE(TL_HIGH, "Using SND_CLASS_BEEP ");
        SoundClass = SND_CLASS_BEEP;
        break;

    case UI_ALERT_SND:
        /* We will use the ringer for sound generator whether we are in a   */
        /* call or not. Also we will use the volume level that is specified */
        /* in the sound command.                                            */
        SKY_TRACE(TL_HIGH, "Using SND_CLASS_BEEP ");
        SoundClass = SND_CLASS_BEEP;
        break;

    case UI_MSG_SND:
    case UI_EARBEEP_SND:
    case UI_CALL_SND:
//        if((!SKY_IsInCall()) || (SKY_IsInSMSCall())) /* SMS6 */
        if(!SKY_IsInCall()) /* SMS6 */
        {
            /* We will use the ringer for sound generator when we are not in a  */
            /* call. Also we will use the volume level that is specified in the */
            /* sound command.                                                   */
            SoundClass = SND_CLASS_BEEP ;
            SKY_TRACE(TL_HIGH, "Using SND_CLASS_BEEP");
        }
        else
        {
            /* We will use the voice generator ( earpiece or speaker ) for the  */
            /* specified path. Also we will use the volume specified in the     */
            /* sound command.                                                   */
            SoundClass = SND_CLASS_VOICE ;
            SKY_TRACE(TL_HIGH, "Using SND_CLASS_VOICE");
        }
        break;

    case UI_CALLFAIL_SND:
        /* We will use the voice generator ( earpiece or speaker ) for the  */
        /* specified path. Also we will use the volume specified in the     */
        /* sound command.                                                   */
        SoundClass = SND_CLASS_VOICE;
        break;

    case UI_CALL_EAR_SND:
        /* If we are not in a data call :                                   */
        /* We will use the voice generator ( earpiece or speaker ) for the  */
        /* specified path. Also we will ignore the volume level that is     */
        /* specified in the sound command, and use ear volume instead.      */
        SoundClass = SND_CLASS_VOICE ;
        SKY_TRACE(TL_HIGH, "Using SND_CLASS_VOICE");
        break;

      /* If neither of the following features are defined, this effectively */
      /* becomes case UI_DTMF_SND: default:, which is the desired result    */

    default:
        SKY_ERROR1("Bad sound type %d: Using SND_CLASS_BEEP", SoundType);
        SoundClass = SND_CLASS_BEEP;
        break;

    }
    return SoundClass;
}

#endif // #if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

/*===========================================================================
FUNCTION CHOOSE_METHOD

DESCRIPTION
  Choose the correct sound method

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   

LOCAL SOUND_METHOD_E sChooseMethod(SOUND_TYPE_E SoundType)
{
    SOUND_METHOD_E SoundMethod; /* return value */

#ifdef FEATURE_DS
    if( IsDataCall() && SoundType != UI_MUSIC_SND )
    {
        /* If we are in a data call, we must use the ringer as the generator */
#ifdef NEW_SOUND_DEBUG
        SKY_TRACE(TL_HIGH, "Data call : Using SND_METHOD_MESSAGE ");
#endif
        SoundMethod = SND_METHOD_MESSAGE ;
        return (SoundMethod);
    }
#endif

    switch( SoundType ) 
    {
    case UI_BEEP_SND:             /* key beep */
#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#else
        /* We will use the keybeep method for the specified device.       */
        SoundMethod = SND_METHOD_KEY_BEEP ;
#endif  /* FEATURE_AUDIO_CONTROL */
        break ;

    case UI_PWRUP_SND:            /* power up beep */
        /* We will use the message method whether we are in a call or not.  */
        SoundMethod = SND_METHOD_MESSAGE ;
        break;

    case UI_ALERT_SND:
        /* We will use the ring method whether we are in a call or not.     */
#ifdef BUZZER_MELODY
	    /// Edited by happykid 2000.1.11
        SoundMethod = SND_METHOD_RING ;
#else
	    SoundMethod = SND_METHOD_KEY_BEEP;
#endif
        break;

    case UI_MSG_SND:
    case UI_EARBEEP_SND:
    case UI_CALL_SND:
        if (( !ui_is_in_conversation() ) || (SKY_CurrentCallIsSMS())) /* SMS6 */
        {
            /* We will use the ring method when we are not in a call          */
#ifdef BUZZER_MELODY
	        /// Edited by happykid 2000.1.11
            if (SoundType == UI_MSG_SND) 
            {
                SoundMethod = SND_METHOD_RING;
            }
            else 
#endif
		    {
                SoundMethod = SND_METHOD_KEY_BEEP ;
            }
        }
        else
        {
            /* We will use the voice method ( earpiece or speaker ) for the   */
            /* specified device.                                              */
            SoundMethod = SND_METHOD_VOICE ;
        }
        break;

    case UI_CALLFAIL_SND:
        /* We will use the voice method ( earpiece or speaker ) for the   */
        /* specified device.                                              */
        SoundMethod = SND_METHOD_VOICE ;
        break;

    case UI_CALL_EAR_SND:
        /* If we are not in a data call :                                   */
        /* We will use the voice method ( earpiece or speaker ) for the     */
        /* specified device.                                                */
        SoundMethod = SND_METHOD_VOICE ;
        break;

#if defined(FEATURE_UI_DTMF_THRU_EARPIECE) || defined(FEATURE_AUDIO_CONTROL)
#error code not present
#endif

        /* If neither of the following features are defined, this effectively */
        /* becomes case UI_DTMF_SND: default:, which is the desired result    */

#ifdef FEATURE_UI_DTMF_THRU_EARPIECE
#error code not present
#endif /* #ifdef FEATURE_UI_DTMF_THRU_EARPIECE */
#ifdef FEATURE_AUDIO_CONTROL 
#error code not present
#endif /* #ifdef FEATURE_AUDIO_CONTROL */

	case UI_MUSIC_SND:
		SoundMethod = SND_METHOD_MUSIC;
		break;

	case UI_ALARM_SND:
		SoundMethod = SND_METHOD_ALARM;
		break;

    case UI_SMS_MSG_SND:
		SoundMethod = SND_METHOD_SMS;
		break;

    default:
        SKY_ERROR1( "Bad sound type %d: Using SND_METHOD_KEY_BEEP", SoundType);
        SoundMethod = SND_METHOD_KEY_BEEP;
        break;

    }
#ifdef NEW_SOUND_DEBUG
    SKY_TRACE2(TK_HIGH, "Using method: %d type: %d ",SoundMethod, SoundType) ;
#endif
    return SoundMethod;
}
#endif//#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   

void SKY_API sSetVolume(SOUND_METHOD_E SoundMethod, INT8 nVolume)
{
    snd_set_volume(SND_DEVICE_HANDSET, SoundMethod, 
                   sIntToLevel(nVolume), NULL, NULL);
}

#endif//#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   

/*===========================================================================
FUNCTION UISND_BEEP_VOL

DESCRIPTION
  Set the beep volume.

DEPENDENCIES
  None.
===========================================================================*/
/* 키톤 볼륨 설정 - 하드웨어 설정이 아니라 볼륨 값만 변경 */
void SKY_API SKY_SetBeepVolume(INT8 nVolume)
{
    SKY_SET_KEY_TONE_VOLUME(nVolume);

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    sSetVolume(SND_METHOD_KEY_BEEP, nVolume);
#endif//#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
}

/*===========================================================================
FUNCTION UISND_ALERT_VOL

DESCRIPTION
  Set the SMS alert volume.
  SMS Volume
DEPENDENCIES
  None.
===========================================================================*/
void SKY_API SKY_SetRingerAlertVolume(INT8 nVolume)
{
    SKY_SET_BELL_VOLUME(nVolume);

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    sSetVolume(SND_METHOD_MUSIC, nVolume);
#endif//#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
}

void SKY_API SKY_SetSMSAlertVolume(INT8 nVolume)
{
    SKY_SET_SMS_ALERT_BELL_VOLUME(nVolume);

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    // XXX : what's difference???
    sSetVolume(SND_METHOD_MESSAGE, nVolume);
    sSetVolume(SND_METHOD_SMS, nVolume);        
#endif//#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
}

void SKY_API SKY_SetAlarmAlertVolume(INT8 nVolume)
{
    SKY_SET_ALARM_BELL_VOLUME(nVolume);

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    sSetVolume(SND_METHOD_ALARM, nVolume);
#endif//#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
}

/*===========================================================================
FUNCTION UISND_VOICE_VOL

DESCRIPTION
  Set the voice volume. Then also tell sound task.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
void SKY_API SKY_MusicPowerOff(void)
{
    SOUND_COMMAND_U *pCommand;

    if( ( pCommand = (SOUND_COMMAND_U*)snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd = SND_MUSIC_HW_POWER_DOWN;
        snd_cmd( pCommand );
    }
}
#endif

void SKY_API SKY_SetVoiceVolume(INT8 nVolume)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SOUND_COMMAND_U *pCommand;
    
    SKY_SET_SPEAKER_VOLUME(nVolume);
    
    //uisnd_voice_vol(nVolume);

    /* Tell sound about the volume change  */
    if( ( pCommand = (SOUND_COMMAND_U*)snd_get_packet()) != NULL ) 
    {
        pCommand->vol.volume.phone = sIntToLevel(SKY_GET_SPEAKER_VOLUME());/* ear piece */
        pCommand->vol.volume.ext   = sIntToLevel(SKY_GET_SPEAKER_VOLUME());/* speaker phone */
        pCommand->vol.volume.priv  = sIntToLevel(SKY_GET_SPEAKER_VOLUME());/* ear jack */

        pCommand->hdr.cmd = SND_VOLUME;
        snd_cmd( pCommand );

        SKY_TRACE2(TL_HIGH, "Voice Volume: %d, Path: %d", nVolume, l_OutDevice);
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    SKY_SET_SPEAKER_VOLUME(nVolume);
    sSetVolume(SND_METHOD_VOICE, nVolume);
#endif//#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
}

void SKY_API SKY_SetMicLevel(MIC_VOLUME_E Volume)
{
    /* need to change nLevel --> MIC_LEVEL_E */
    BYTE aMicLevel[] = { 0x5f, 0x3f, 0x1f };

    SKY_SET_MIC_VOLUME(Volume);

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SKY_TRACE2(TL_HIGH, "Mic gain: %d, Path: %d", aMicLevel[Volume], l_OutDevice);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    /* update our settings */
    if(SKY_IS_EAR_JACK_ON())
    {
		snd_set_mic_gain(SND_DEVICE_HEADSET, aMicLevel[Volume], NULL, NULL);
    }
	else if(SKY_IS_HANDS_FREE())
    {
        snd_set_mic_gain(SND_DEVICE_HFK, aMicLevel[Volume], NULL, NULL);
    }
	else if(SKY_IS_LOUD_SPEAKER())
    {
        snd_set_mic_gain(SND_DEVICE_LOUDSPK, aMicLevel[Volume], NULL, NULL);
    }
    else
    {
        snd_set_mic_gain(SND_DEVICE_HANDSET, aMicLevel[Volume], NULL, NULL);
    }
#endif//#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
}

/*===========================================================================
FUNCTION UISND_SET_PATH

DESCRIPTION
  Set the normal input and output path.
===========================================================================*/
void SKY_API SKY_SetSoundPath(BOOL fMuteIn, BOOL fMuteOut, SOUND_DEVICE_E Device)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SOUND_COMMAND_U *pCommand;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd           = SND_SET_PATH;
        pCommand->path.device       = Device;

        pCommand->path.manner       = (SKY_IS_ETIQUETTE() || IsWhisperMode());
        pCommand->path.out_mute_s = (snd_mute_control_type)(fMuteOut \
                                    ? SND_MUTE_MUTED : SND_MUTE_UNMUTED);
        pCommand->path.in_mute_s = (snd_mute_control_type)(fMuteIn \
                                    ? SND_MUTE_MUTED : SND_MUTE_UNMUTED);
        snd_cmd( pCommand );
    }
    l_OutDevice = Device;
    
    SKY_TRACE1(TL_HIGH, "Input path changed to %d", Device);
#endif//#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
}


/*===========================================================================
FUNCTION UISND_SET_DEVICE

DESCRIPTION
  Set the normal input and output path.
  DMSS5000에 있는 uisnd_set_path()로는 우리(?)가 원하는 path설정 기능을
  충족시키기에는 너무 허술해서 새로 만들었다.

  * A7/A10/B0/B1이 공통으로 사용할 수 있게 되었다.
===========================================================================*/
void SKY_API SKY_SetSoundDevice(SOUND_MUTE_CONTROL_E SpeakerMute, 
                                SOUND_MUTE_CONTROL_E MicMute,
                                BOOL                 fChangeMute)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SOUND_DEVICE_E SoundDevice;
    STATIC BOOL s_fOutMute = UISND_NOVOICE;
    STATIC BOOL s_fInMute  = UISND_NOVOICE;

    if(fChangeMute)
    {
        s_fOutMute = (SpeakerMute == SND_MUTE_UNMUTED) 
                      ?  UISND_VOICE : UISND_NOVOICE;
        s_fInMute =  (MicMute == SND_MUTE_UNMUTED) 
                      ?  UISND_VOICE : UISND_NOVOICE;
    }
    /* ------------------
    ** Priority of Device
    **  1. ear-phone
    **  2. hands-free
    **  3. handset(ear-piece)
    */
    /* ear jack */
    if(SKY_IS_EAR_JACK_ON())
        SoundDevice = SND_DEVICE_EARPHONE;
    /* hands free kit */
    else if(SKY_IS_HANDS_FREE())
        SoundDevice = SND_DEVICE_HFK_CORD;
    /* ear piece */
    else
        SoundDevice = SND_DEVICE_EARPIECE;

    SKY_SetSoundPath(s_fInMute, s_fOutMute, SoundDevice);

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    STATIC SOUND_MUTE_CONTROL_E s_OutMute = SND_MUTE_MUTED;
    STATIC SOUND_MUTE_CONTROL_E s_InMute  = SND_MUTE_MUTED;

    SOUND_DEVICE_E SoundDevice;

    if(fChangeMute)
    {
        s_OutMute = SpeakerMute;

        if (SKY_IS_MUTE()) 
		    s_InMute = SND_MUTE_MUTED;
	    else
		    s_InMute = MicMute;
    }

    /* ------------------
    ** Priority of Device
    **  1. ear-phone
    **  2. hands-free
    **  3. handset(ear-piece)
    */
    /* ear jack */
    if(SKY_IS_EAR_JACK_ON())
    {
        SoundDevice = SND_DEVICE_HEADSET;
    }
    /* hands free kit */
    else if(SKY_IS_HANDS_FREE())
    {
        if(ui_is_in_conversation() || SKY_IS_ORIGINATE() || gnVocoderMode != VR_NONE)
            SoundDevice = SND_DEVICE_HFK;
        else
            SoundDevice = SND_DEVICE_LOUDSPK;
    }
    else if(SKY_IS_LOUD_SPEAKER())
    {
        SoundDevice = SND_DEVICE_LOUDSPK;
    }
    else 
    {
        SoundDevice = SND_DEVICE_HANDSET;
    }

    snd_set_device(SoundDevice, s_OutMute, s_InMute, 
                    (BOOL)(SKY_IS_ETIQUETTE() || IsWhisperMode()), NULL, NULL);

#endif//#if SKY_SOUND_INTERFACE == SKY_XXXX_SOUND_INTERFACE   
}

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
LOCAL void SKY_API sCallbackFunction1(void *pClientData, 
                                      SOUND_STATUS_E Status)
{
    /*  NOT USED
    if(Status == SND_REPEAT)
        rex_set_sigs(&ui_tcb, UI_RING_SIG);
    */
}
#endif//#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   


/*===========================================================================
FUNCTION uisnd_VOICE

DESCRIPTION
  Play a voice prompt repeatly. In currently, continuously playing only available.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
#ifdef FEATURE_VOICEPLAY  // jwpark 00.11.06
void SKY_API SKY_PlayVoice(BYTE *pVoiceBuffer,
				           WORD nVoiceLength,
				           WORD nCount)
{
    SOUND_COMMAND_U *pCommand;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd          = SND_VOICE_START;
        pCommand->voice.device     = SND_DEVICE_CURRENT;
        pCommand->voice.sclass     = SND_CLASS_VOICE;
        pCommand->voice.volume_lvl = sIntToLevel(SKY_GET_GAME_VOLUME());
        pCommand->voice.voice_buf  = pVoiceBuffer;
        pCommand->voice.voice_len  = nVoiceLength;
        pCommand->voice.repeat_cnt = nCount;
#ifdef FEATURE_WAVELET
		pCommand->voice.Isfromwaveletbuffer = FALSE;
#endif // FEATURE_WAVELET
        snd_cmd(pCommand);
    }
}
#endif
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
void SKY_API SKY_PlayVoice(BYTE *pVoiceBuffer, 
                           UINT16 nVoiceLength, 
                           UINT8 nCount)
{
    snd_voice_start( SND_DEVICE_LOUDSPK , SND_METHOD_VOICE ,
                        pVoiceBuffer, nVoiceLength, nCount, 
                        sIntToLevel(SKY_GET_GAME_VOLUME()),   // ui.beep_lvl
                        NULL, NULL);
}
#endif  // if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   

/*===========================================================================
FUNCTION uisnd_VOICE_stop

DESCRIPTION
  Stop Playing a voice prompt

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
#ifdef FEATURE_VOICEPLAY  // jwpark 00.11.06
void SKY_API SKY_StopVoice(void)
{
    SOUND_COMMAND_U *pCommand;

    if((pCommand = snd_get_packet()) != NULL)
    {
        pCommand->hdr.cmd = SND_VOICE_STOP;
        snd_cmd(pCommand);
    }
}
#endif
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
void SKY_API SKY_StopVoice(void)
{
    snd_voice_stop( NULL, NULL);
}
#endif  // #if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   


#ifdef FEATURE_WAVELET
boolean MM_StartAudio(void)
{
    SOUND_COMMAND_U *pCommand;

	extern BOOL fPause;
	extern BOOL fDownload;

	extern BYTE* pAdoData;	// yoursun 01/06/13
	extern BYTE* ptAdoData;
	
	extern DWORD Audio_length;
	extern DWORD tAudio_length;

	if (fDownload == TRUE)
	{
		if (fPause == FALSE) // yoursun 01/06/21  PAUSE Problem
		{
				Audio_length = tAudio_length;
				pAdoData = ptAdoData;
		}
	}

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd          = SND_VOICE_START;
        pCommand->voice.device     = SND_DEVICE_CURRENT;
        pCommand->voice.sclass     = SND_CLASS_VOICE;
        pCommand->voice.volume_lvl = sIntToLevel(SKY_GET_SPEAKER_VOLUME());
        pCommand->voice.voice_buf  = NULL;
        pCommand->voice.voice_len  = 0;
        pCommand->voice.repeat_cnt = 1;
		pCommand->voice.Isfromwaveletbuffer = TRUE;
        snd_cmd(pCommand);
		return TRUE;
    }
	else
		return FALSE;
}

boolean MM_EndAudio(void)
{
    SOUND_COMMAND_U *pCommand;

	extern BOOL fPause;
	extern BOOL fDownload;
	extern BYTE * pAdoData;	// yoursun 01/06/13
	
	// yoursun 01/06/21  PAUSE Problem
	if (fDownload == TRUE)
	{
		if (fPause == FALSE) 
		{
			pAdoData =  NULL; // yoursun 01/06/13
		}
	}

    if((pCommand = snd_get_packet()) != NULL)
    {
        pCommand->hdr.cmd = SND_VOICE_STOP;
        snd_cmd(pCommand);
		return TRUE;
    }
	else
		return FALSE;
}
#endif // FEATURE_WAVELET
/*===========================================================================
FUNCTION uisnd_SND

DESCRIPTION
  Play a sound

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

void SKY_API SKY_PlaySound(SOUND_TYPE_E SoundType, SOUND_PREDEF_E Sound)
{
    SOUND_COMMAND_U *pCommand;

    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())
        return;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd = SND_SOUND_START;
        pCommand->snd.dtmf_path = SND_DTMF_APATH_LOCAL;
        if( SoundType == UI_ALERT_SND ) 
        {
            pCommand->hdr.task_ptr = rex_self();
            pCommand->hdr.sigs = 0;
            pCommand->snd.rpt_sigs = UI_RING_SIG;
        } 
        else 
        {
            pCommand->hdr.task_ptr = NULL;
            pCommand->snd.rpt_sigs = 0;
        }
        pCommand->snd.device  = sChooseDevice( SoundType );
        pCommand->snd.sclass = sChooseClass( SoundType );
        sChooseVolume( SoundType, &pCommand->snd.vol );

        pCommand->snd.sound = Sound;

        snd_cmd( pCommand );
    }
}

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

void SKY_API SKY_PlaySound(SOUND_TYPE_E SoundType, SOUND_ID_E Sound)
{
    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())  // !ui.powerup || ui.charger_on
        return;

    if ( (SKY_GET_BELL_VOLUME() == 0 
          && (SoundType == UI_ALERT_SND || SoundType == UI_MUSIC_SND ))
	    || (SKY_GET_KEY_TONE_VOLUME() == 0 && SoundType == UI_BEEP_SND ))
    {
	    return;
    }

    if(SKY_IS_HANDS_FREE() && SKY_IS_EAR_JACK_OFF() && Sound == SND_REORDER)
    {
        snd_sound_id_start(SND_DEVICE_HFK, sChooseMethod(SoundType), 
                           Sound, SND_PRIO_LOW, SND_APATH_LOCAL,
                           (snd_cb_func_ptr_type)sCallbackFunction1, NULL);
    }
	else if ( SoundType == UI_ALERT_SND)
    {
        snd_sound_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                            Sound, SND_PRIO_LOW, SND_APATH_LOCAL,
                            (snd_cb_func_ptr_type)sCallbackFunction1, NULL);
    } 
    else if( SoundType == UI_MUSIC_SND)
    {
        if( SKY_IS_ALERT() 
            && SKY_IS_EAR_JACK_ON() 
            && ( SKY_GET_BELL_VIBRATION() == BV_VIBRATION || SKY_IS_ETIQUETTE() ) )
        {
            snd_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                Sound, 0, SND_MELODY_NORMAL, 
                                NULL, NULL);
        }
        else if( SKY_IS_ALERT() )
        {
	        /// 착신벨일때와 아닐때의 차이를 SND_PRIO_HIGH로 둔다.
            snd_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                Sound, 0, SND_MELODY_ALERT, 
                                NULL, NULL);
        }
        else
        {
            snd_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                Sound, 0, SND_MELODY_NORMAL, 
                                NULL, NULL);
        }
    }
    else if( SoundType == UI_ALARM_SND )
        {
        if( SKY_IS_EAR_JACK_ON() 
            && ( SKY_GET_BELL_VIBRATION() == BV_VIBRATION || 
                ( SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_IMMEDIATE 
                  && SKY_IS_AUTO_ANSWER()))
            || SKY_IS_ETIQUETTE())
        {
            snd_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                Sound, 0, SND_MELODY_NORMAL, 
                                NULL, NULL);
        }
        else
        {
            snd_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                Sound, 0, SND_MELODY_ALERT, 
                                NULL, NULL);
        }
    }
    else if( SoundType == UI_SMS_MSG_SND)
    {
        if( SKY_IS_EAR_JACK_ON() 
            && ( SKY_GET_BELL_VIBRATION() == BV_VIBRATION ||
                ( SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_IMMEDIATE
                  && SKY_IS_AUTO_ANSWER()))
            || SKY_IS_ETIQUETTE())
        {
            snd_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                Sound, 1, SND_MELODY_NORMAL, 
                                NULL, NULL);
        }
        else
        {
            snd_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                Sound, 1, SND_MELODY_ALERT, 
                                NULL, NULL);
        }
    }
    else {
        snd_sound_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                            Sound, SND_PRIO_LOW, SND_APATH_LOCAL,
                            NULL, NULL);
    }

}

#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE
        

/*===========================================================================
FUNCTION uisnd_sample_snd

DESCRIPTION
  Play a sample sound

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
/* 
** it's same with SKY_PlaySound but passing nVolume value
*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

void SKY_API SKY_PlaySoundEx(SOUND_TYPE_E SoundType, 
                             SOUND_PREDEF_E Sound, INT8 nVolume,
                             UINT16 nRepeatCount)

{
    SOUND_COMMAND_U *pCommand;

    if(SKY_IS_SHUT_DOWN()) /* powering down, so no sounds */
        return;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd = SND_SOUND_START;
        pCommand->snd.dtmf_path  = SND_DTMF_APATH_LOCAL;
        if( SoundType == UI_ALERT_SND ) 
        {
            pCommand->hdr.task_ptr = rex_self();
            pCommand->hdr.sigs = 0;
            pCommand->snd.rpt_sigs = UI_RING_SIG;
        }
        else 
        {
            pCommand->hdr.task_ptr = NULL;
            pCommand->snd.rpt_sigs = 0;
        }
        pCommand->snd.device      = sChooseDevice( SoundType );
        pCommand->snd.sclass     = sChooseClass( SoundType );

        pCommand->snd.vol.phone  = (byte) sIntToLevel(nVolume);
        pCommand->snd.vol.ext    = (byte) sIntToLevel(nVolume);
        pCommand->snd.vol.priv   = (byte) sIntToLevel(nVolume);
        pCommand->snd.repeat_cnt = nRepeatCount;

        pCommand->snd.sound      = Sound;

        snd_cmd( pCommand );
    }

}

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

void SKY_API SKY_PlaySoundEx(SOUND_TYPE_E SoundType, 
                             SOUND_ID_E Sound, INT8 nVolume, 
                             UINT16 nRepeatCount)
{
    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())  // !ui.powerup || ui.charger_on
        return;

    if( nVolume == 0)
    {
        SKY_StopSound();
        return;
    }

    if( SKY_IS_HANDS_FREE() 
         && SKY_IS_EAR_JACK_OFF()
         && Sound == SND_REORDER )
    {
        snd_sample_sound_id_start(SND_DEVICE_HFK, sChooseMethod(SoundType), 
                                  Sound, sIntToLevel(nVolume), nRepeatCount, 
                                  (snd_cb_func_ptr_type)sCallbackFunction1, NULL);
    }
    else if( SoundType == UI_ALERT_SND ) {
		snd_sample_sound_id_start(sChooseDevice(SoundType), sChooseMethod(SoundType), 
                                  Sound, sIntToLevel(nVolume), nRepeatCount, 
							      (snd_cb_func_ptr_type)sCallbackFunction1, NULL);
    } 
    else if( SoundType == UI_MUSIC_SND 
             || SoundType == UI_ALARM_SND 
             || SoundType == UI_SMS_MSG_SND)
    {
        snd_sample_music_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
					               Sound, sIntToLevel(nVolume), nRepeatCount,
					               NULL, NULL);
    }
    else {
        snd_sample_sound_id_start( sChooseDevice(SoundType), sChooseMethod(SoundType),
                                   Sound, sIntToLevel(nVolume), nRepeatCount,
                                   NULL, NULL);
    }
}

#endif//#elif SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE

/*===========================================================================
FUNCTION uisnd_SND_STOP

DESCRIPTION
  Stop any sound
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up.
===========================================================================*/
void SKY_API SKY_StopSound(void)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_COMMAND_U *pCommand;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd = SND_SOUND_STOP;
        snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

    snd_sound_stop(SND_PRIO_LOW, NULL, NULL);
    snd_music_stop(NULL, NULL);
    snd_voice_stop(NULL, NULL);
#endif//#if SKY_SOUND_INTERFACE == SKY_XXXX_SOUND_INTERFACE
}

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
LOCAL void SKY_API sCallbackFunction2(void *pClientData, SOUND_STATUS_E Status)
{
    if(Status == SND_PLAY_DONE)
        rex_set_sigs(&ui_tcb, UI_MULTI_STOP_SIG);
}
#endif//#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   


/*===========================================================================
FUNCTION uisnd_MULTI

DESCRIPTION
  Play a multitone

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

/* 
fOverTheAir : transmit over air?
pToneList : list of tones 
fFromBS : Multi tones received from BS
*/

LOCAL void SKY_API sPlayMultiTone(BOOL fOverTheAir, 
                                  DTMF_TONE_T *pToneList, 
                                  BOOL fDTMFFromBS)
{
    SOUND_COMMAND_U *pCommand;

    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())
        return;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        /* DTMF tones received from base station */
        if (fDTMFFromBS) 
        {
            pCommand->multi.device    = sChooseDevice( UI_CALL_SND );
            pCommand->multi.sclass    = sChooseClass( UI_CALL_SND );
        }
        else 
        {
            pCommand->multi.device    = sChooseDevice( UI_BEEP_SND );
            pCommand->multi.sclass    = sChooseClass( UI_BEEP_SND );
        }
        pCommand->multi.dtmf_path 
            = (DTMF_APATH_E)(fOverTheAir 
                             ? SND_DTMF_APATH_BOTH 
                               : SND_DTMF_APATH_LOCAL);

        /* Use the type's volume */
        sChooseVolume( UI_BEEP_SND, &pCommand->multi.vol );

        pCommand->multi.multi_ptr = pToneList; /* sound pointer */
        pCommand->hdr.cmd         = SND_MULTI_START;
        pCommand->hdr.task_ptr    = rex_self();
        pCommand->hdr.sigs        = UI_MULTI_STOP_SIG;
        pCommand->multi.rpt_sigs  = 0;
        (void) rex_clr_sigs( rex_self(), UI_MULTI_STOP_SIG );
        snd_cmd( pCommand );
    }
}
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

#define MAX_SOUND_BUF_SIZE  66

LOCAL void SKY_API sPlayMultiTone(BOOL fOverTheAir, 
                                  SOUND_COMPACT_TONE_T *pToneList, 
                                  BOOL fDTMFFromBS)
{
    STATIC SOUND_COMPACT_SOUND_T MultiSound;
    STATIC SOUND_COMPACT_TONE_T  SoundBuf[MAX_SOUND_BUF_SIZE];
    INT8 i;
    DTMF_APATH_E    DTMFPath = SND_APATH_LOCAL;
    SOUND_DEVICE_E Device;
    SOUND_METHOD_E Method;

    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())
        return;

    for (i = 0; i < MAX_SOUND_BUF_SIZE; i++) 
    {
        SoundBuf[i] = pToneList[i];
    }
    MultiSound.type       = SND_COMPACT_SOUND;
    MultiSound.tone_array = SoundBuf;

    if(fOverTheAir)
    {
	    if ( SKY_IS_ETIQUETTE() 
             && SKY_IS_EAR_JACK_OFF()
             && !SKY_IS_HANDS_FREE() ) 
		    DTMFPath = SND_APATH_TX;
	    else
		    DTMFPath = SND_APATH_BOTH;
    }
    else
    {
        if ( SKY_IS_ETIQUETTE()
             && SKY_IS_EAR_JACK_OFF()
             && !SKY_IS_HANDS_FREE() )
	        DTMFPath = SND_APATH_MUTE;
    }

    Device = sChooseDevice((SOUND_TYPE_E)(fDTMFFromBS ? UI_CALL_SND : UI_BEEP_SND));
    Method = sChooseMethod((SOUND_TYPE_E)(fDTMFFromBS ? UI_CALL_SND : UI_BEEP_SND));

    (void) rex_clr_sigs( rex_self(), UI_MULTI_STOP_SIG );

    snd_sound_start(Device, Method, (SOUND_T *) &MultiSound, SND_PRIO_HIGH, DTMFPath, 
                  (snd_cb_func_ptr_type)sCallbackFunction2, NULL);
}

#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE


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
/*
  BOOL              fOverTheAir      // transmit over the air ?
  SOUND_TYPE_E      SoundType        // type of sound for vol setting
  WORD              wNumTones        // # of tones
  DTMF_TONE_T       *pToneList       // buffer containing tones
  SYS_WAKE_EVENT_E  SysWakeEvent     // module event, used only if MODULE defined
  BOOL              fDTMFFromBS      // received tones from base station 
*/

/*---------------------------------------------------------------------------
 Type definition for all events that could cause the SYS_WAKE pin to be 
 asserted.  Items cannot be added to this enum without taking others out,
 and MOD_NONE should always be -1 (or some other value less than 0).
---------------------------------------------------------------------------*/

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

void SKY_API SKY_PlayMultiTone(
                            BOOL fOverTheAir, SOUND_TYPE_E SoundType,
                            WORD wNumTones, DTMF_TONE_T *pToneList,
                            SYS_WAKE_EVENT_E SysWakeEvent, BOOL fDTMFFromBS)

{
    UNUSED(wNumTones);
    UNUSED(SoundType);
    UNUSED(SysWakeEvent);

    sPlayMultiTone(fOverTheAir, pToneList, fDTMFFromBS);
}

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

void SKY_API SKY_PlayMultiTone(
                            BOOL fOverTheAir, SOUND_TYPE_E SoundType,
                            WORD wNumTones, SOUND_COMPACT_TONE_T *pToneList,
                            SYS_WAKE_EVENT_E SysWakeEvent, BOOL fDTMFFromBS)
{
    UNUSED(wNumTones);
    UNUSED(SoundType);
    UNUSED(SysWakeEvent);

    sPlayMultiTone(fOverTheAir, pToneList, fDTMFFromBS);
}

#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE

/*===========================================================================
FUNCTION uisnd_MULTI_STOP

DESCRIPTION
  Stop any multi tone
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void SKY_API SKY_StopMultiTone(void)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_COMMAND_U *pCommand;

    if( ( pCommand = snd_get_packet()) != NULL ) {
      pCommand->hdr.cmd = SND_MULTI_STOP;
      snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
    snd_sound_stop(SND_PRIO_HIGH, NULL, NULL);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE
}

/* powering down, so no sounds */
/* key tone volume is 0 */
/* manners mode or whisfer mode */
#define sIS_NO_BEEP_CONDITION()    (SKY_IS_SHUT_DOWN() \
                                   || ((SKY_IS_ETIQUETTE() || IsWhisperMode()) \
                                        && SKY_IS_EAR_JACK_OFF()))

/*===========================================================================
FUNCTION uisnd_TONE

DESCRIPTION
  Play a tone
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void SKY_API SKY_PlayTone(SOUND_TYPE_E SoundType,   /* type of sound */
                          SOUND_TONE_E SoundTone,   /* which tone */
                          WORD wDurationMS)         /* duration in ms */
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_COMMAND_U *pCommand;

    if(sIS_NO_BEEP_CONDITION())
        return;

    if(SKY_GET_KEY_TONE_VOLUME() == 0)  
        return;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd = SND_TONE_START;

        pCommand->tone.device = sChooseDevice( SoundType );
        pCommand->tone.sclass = sChooseClass( SoundType );
        sChooseVolume( SoundType, &pCommand->tone.vol );

        pCommand->tone.dtmf_path = SND_DTMF_APATH_LOCAL;
        pCommand->tone.tone.tone = SoundTone; /* repetitious! */
        pCommand->tone.tone.duration_ms = wDurationMS;

        SKY_TRACE2(TL_HIGH, "Path: %d  Time: %d", 
        //                                pCommand->tone.apath, 
                                        pCommand->tone.dtmf_path,
                                        pCommand->tone.tone.duration_ms);

        pCommand->hdr.task_ptr = NULL;
        snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
    SOUND_DEVICE_E  Device;
    SOUND_METHOD_E  Method;

// XXX: not implemented yet!!!
//#ifdef SW_RESET_ENABLE
//  	if(mc_bSwReset) return;  // For SW_Reset
//#endif // SW_RESET_ENABLE

    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())
        return;

    if ( (SKY_IS_ETIQUETTE() || IsWhisperMode())
          && SKY_IS_EAR_JACK_OFF() )
        return;

    Device = sChooseDevice(SoundType);
    Method = sChooseMethod(SoundType);

    snd_tone_start(Device, Method, SoundTone, wDurationMS, SND_APATH_LOCAL, NULL, NULL);
#endif//#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
}


/*===========================================================================
FUNCTION uisnd_sample_tone

DESCRIPTION
  Play a sample tone while adjusting volume
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
/* 
** SKY_PlayTone과 같으나, 볼륨을 외부에서 전달한다. 따라서 볼륨 조정할 때
** 톤을 울려주기 위해 사용한다.
** 단순히 톤을 울리기 위해서는 SKY_PlayTone을 사용한다.
*/
void SKY_API SKY_PlayToneEx(SOUND_TYPE_E SoundType,
                            SOUND_TONE_E SoundTone,
                            WORD         wDurationMS,
                            INT8         nVolume)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_COMMAND_U *pCommand;

    if(sIS_NO_BEEP_CONDITION())
        nVolume = 0;        // youandi가 보코더때문에 바꿔야한다고 함.

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd = SND_TONE_START;

        pCommand->tone.device = sChooseDevice( SoundType );
        pCommand->tone.sclass = sChooseClass( SoundType );

        pCommand->tone.vol.phone = (byte) sIntToLevel(nVolume);
        pCommand->tone.vol.ext = (byte) sIntToLevel(nVolume);
        pCommand->tone.vol.priv = (byte) sIntToLevel(nVolume);

        pCommand->tone.dtmf_path = SND_DTMF_APATH_LOCAL;
        pCommand->tone.tone.tone = SoundTone; /* repetitious! */
        pCommand->tone.tone.duration_ms = wDurationMS;

        SKY_TRACE2(TL_HIGH, "Path: %d  Time: %d", 
        //                                pCommand->tone.apath, 
                                        pCommand->tone.dtmf_path,
                                        pCommand->tone.tone.duration_ms);

        pCommand->hdr.task_ptr = NULL;
        snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

    SOUND_DEVICE_E  Device;
    SOUND_METHOD_E  Method;

    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())
        return;

    if ( ((SKY_IS_ETIQUETTE() || IsWhisperMode())
            && SKY_IS_EAR_JACK_OFF())
          || nVolume == 0 )
        return;

    Device = sChooseDevice(SoundType);
    Method = sChooseMethod(SoundType);

    snd_sample_tone_start(Device, Method, SoundTone, wDurationMS, 
                          sIntToLevel(nVolume), NULL, NULL);

#endif//#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
}


/*===========================================================================
FUNCTION uisnd_DTMF

DESCRIPTION
  Play a dtmf - a special tone which may be transmitted, and is always
  played out the speaker
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void SKY_API SKY_PlayDTMF(SOUND_TONE_E  SoundTone, WORD wDurationMS)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    static db_items_value_type dbi; /* DB items */
    SOUND_COMMAND_U *pCommand;

    if(sIS_NO_BEEP_CONDITION())
        return;

    if(SKY_GET_KEY_TONE_VOLUME() == 0)  
        return;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd     = SND_TONE_START;

        pCommand->tone.device  = sChooseDevice( UI_BEEP_SND );
        pCommand->tone.sclass = sChooseClass( UI_BEEP_SND );
        
        sChooseVolume( UI_BEEP_SND, &pCommand->tone.vol );

        /* determine the path of the dtmfs */
        if(ui_is_in_conversation()) 
        {    
            /* silent scratchpad */
            //if( ui.pad )  --> FALSE
            //{
            //    /* mute it */
            //    pCommand->tone.dtmf_path = SND_DTMF_APATH_MUTE;
            //} 
            //else 
            {
                db_get( DB_DIGITAL, &dbi ); /* is digital? */
                pCommand->tone.dtmf_path 
                    = (snd_dtmf_apath_type)(dbi.digital ? SND_DTMF_APATH_LOCAL \
                                    : SND_DTMF_APATH_BOTH);
            }
        }
        /* not in call */
        else 
        {
            pCommand->tone.dtmf_path = SND_DTMF_APATH_LOCAL;
        }
        pCommand->tone.tone.tone = SoundTone; /* repetitious! */
        pCommand->tone.tone.duration_ms = wDurationMS;
        pCommand->hdr.task_ptr          = NULL;
        snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

    static db_items_value_type dbi; /* DB items */
    SOUND_DEVICE_E Device;
    SOUND_METHOD_E Method;
    DTMF_APATH_E   DTMFPath;

    /* powering down, so no sounds */
    if(SKY_IS_SHUT_DOWN())
        return;

    if ( (SKY_IS_ETIQUETTE() || IsWhisperMode())
          && SKY_IS_EAR_JACK_OFF())
        return;

    Device  = sChooseDevice( UI_BEEP_SND );
    Method  = sChooseMethod( UI_BEEP_SND );

    if( ui_is_in_conversation() ) 
    {         /* determine the path of the dtmfs */
        //if( ui.pad ) 
        //{                /* silent scratchpad */
        //    dtmf_path = SND_APATH_MUTE; /* mute it */
        //} 
        //else 
        {
            db_get( DB_DIGITAL, &dbi ); /* is digital? */
            DTMFPath = ((DTMF_APATH_E)(dbi.digital ? SND_APATH_LOCAL : SND_APATH_BOTH));
        }
    } 
    else 
    {                        /* not in call */
        DTMFPath = SND_APATH_LOCAL;
    }

    snd_tone_start( Device, Method, SoundTone, wDurationMS, DTMFPath, NULL, NULL);

#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE
}


/*===========================================================================
FUNCTION uisnd_TONE_STOP

DESCRIPTION
  Stop any tone
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void SKY_API SKY_StopTone(void)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_COMMAND_U *pCommand;

    /* Send the command to the sound task */
    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd = SND_TONE_STOP;
        snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

    snd_tone_stop(NULL, NULL);

#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE
}


/*===========================================================================
FUNCTION uisnd_SND_INIT

DESCRIPTION
  Initialize ui -> snd buffers
  
DEPENDENCIES
  Called once, by UI.C
===========================================================================*/
void SKY_API SKY_InitSound(void)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    l_OutDevice = SND_DEVICE_EARPIECE; /* through phone */
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
//    l_OutDevice = SND_DEVICE_HANDSET;
#endif//#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
}

/*===========================================================================
FUNCTION uisnd_STEREO_MUTE

DESCRIPTION
  Stereo mute/unmute.
  
DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void SKY_API SKY_SetStereoMute(BOOL fMute)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_COMMAND_U *pCommand;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        pCommand->hdr.cmd         = SND_MUTE_CAR_STEREO;
        pCommand->stereo.if_muted = fMute;
        snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

    snd_stereo_mute_ctl(fMute, NULL, NULL);

#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE
}

#ifdef FEATURE_DS
/*===========================================================================
FUNCTION uisnd_ADATA_CONFIG

DESCRIPTION
  Configure sound to handle an AMPS Data call via the PCMCIA card.
  This involves turning VOXing off, filtering off, unity gain off.

DEPENDENCIES
  uisnd_SND_INIT has been called to set things up
===========================================================================*/
void SKY_API SKY_ConfigAMPSDataSound(BOOL fAMPSDataCall)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_COMMAND_U *pCommand;

    if( ( pCommand = snd_get_packet()) != NULL ) 
    {
        /* Configure for an AMPS data call */
        if ( fAMPSDataCall ) 
        {
            pCommand->filters.filters = SND_FILTERS_DATA;
        }
        /* Configure for a normal call */
        else 
        {
            pCommand->filters.filters = SND_FILTERS_VOICE;
        }
        pCommand->hdr.cmd         = SND_FILTERS;
        snd_cmd( pCommand );
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE

    snd_filters(fAMPSDataCall ? SND_FILTERS_DATA : SND_FILTERS_VOICE, NULL, NULL);

#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE
}

#endif /* ifdef FEATURE_DS */

/*===========================================================================
FUNCTION UISND_KEY_SOUND

DESCRIPTION
  Do the beep associated with a key being pressed.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void SKY_API SKY_PlayKeySound(SKY_EVENT_E Key, WORD wDurationMS)
{
    /* ------------------------------------------------
    ** !! W A R N I N G !!
    ** -------------------
    ** snd.h 
    ** use SOUND_TONE_E as a index of aKeyTable SND_0 = 0, .... 
    ** Assumes key events and SND IDs are in order 
    */
    SOUND_TONE_E aToneTable[] = {
        SND_0, SND_1, SND_2, SND_3, SND_4, SND_5, 
        SND_6, SND_7, SND_8, SND_9, SND_STAR, SND_POUND
    };
    //static byte digits[NV_MAX_DIAL_DIGITS];

    if(!SKY_IS_KEY_TONE())
    {
        /* just ignore */
        return;
    }
    /* Choose the right beep for the right key */
    switch( Key ) 
    {
    /* 
    ** All these can be transmitted 
    ** over the air, or not, so they take special processing
    */
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
    case SE_KEY_POUND :
    case SE_KEY_STAR :
        {
            STATIC BYTE bDigit;
            SOUND_TONE_E SoundTone;

            SoundTone = aToneTable[Key-SE_KEY_0];
            
            if(Key == SE_KEY_POUND)
                bDigit = '#';
            else if(Key == SE_KEY_STAR)
                bDigit = '*';
            else
                bDigit = (BYTE)(Key - SE_KEY_0 + '0');

            /* Transmit it over the air */
            if(SKY_IS_TRANSMIT_DTMF())
            {
                /* Send burst DTMF or contin DTMF message if needed */
//                if(SKY_IS_DIGITAL() && ui_is_in_conversation())
                if(ui_is_in_conversation())
                {
                    if(SKY_BurstDTMF(CAI_DTMF_ON_350, 
                                     CAI_DTMF_OFF_200, 1, &bDigit))
                    {
                        /* make the beep */
                        SKY_PlayDTMF(SoundTone, wDurationMS);
                        /* save the beep for key handler */
                        SET_KEY_DOWN_DTMF(SoundTone);
                    }
                }
                /* just play it for analog */
                else
                {
                    SKY_PlayDTMF(SoundTone, wDurationMS);
                    SET_KEY_DOWN_DTMF(SoundTone);
                }
            }
            /* don't transmit it, just play it */
            else
            {
                SKY_PlayTone(UI_BEEP_SND, SoundTone, wDurationMS);
                SET_KEY_DOWN_DTMF(SoundTone);
            }
        }
        break;
    case SE_KEY_END : 
        /* Anywhere in a call, END key press must end the call */
        //if(ui_is_in_conversation())
        //{
        //    /* back to idle */
        //    SKY_DO_EVENT(SE_SYS_TO_IDLE);
        //}
        /* fall throught */

    /* Control keys which are never transmitted */
    case SE_KEY_POWER :
    case SE_KEY_SEND :
        /* don't beep on powerup */
        if(Key != SE_KEY_END || !SKY_IS_SHUT_DOWN())
        {
            SKY_PlayTone(UI_BEEP_SND, SND_CTRL, wDurationMS);
            SET_KEY_DOWN_TONE(SND_CTRL);
        }
        break;

    /* 
    ** Up and down arrows - don't make any 
    ** sound if we're changing speaker or
    */
    case SE_KEY_UP :
    case SE_KEY_DOWN :
        /* When volume key is pressed while phone is idling that will start    */
        /* volume adjustment, so we do not want any sound. If we are adjusting */
        /* the volume through the menus, then we do not want any sounds either */
        /* Lastly if we are in a call, no sound should be played by these keys */
        /* at all. No sounds while playing ringer samples too                  */

        /* 통화 중에 볼륨 조정할 때는 키톤이 나지 않는다 */
        if(!(ui_is_in_conversation() && SKY_IS_VOLUME_ADJUST()))
        {
            SKY_PlayTone(UI_BEEP_SND, SND_2ND, wDurationMS);
            SET_KEY_DOWN_DTMF(SND_2ND);
        }
        break;

    default:            /* Anything else - never transmitted */
        /* reset clear all beep flag for generating clear all beep */
        if(Key == SE_KEY_CLEAR)
            SET_CLEAR_ALL_BEEP_FLAG(FALSE);

        SKY_PlayTone(UI_BEEP_SND, SND_2ND, wDurationMS);
        SET_KEY_DOWN_TONE(SND_2ND);
    }   /* switch key */
}

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
/*
SoundType   : type of sound
Sound       : which sound
WakeEvent   : module event, used only if MODULE defined
*/

void SKY_API SKY_PlaySoundInHandset(SOUND_TYPE_E SoundType, 
                                    SOUND_PREDEF_E Sound, 
                                    SYS_WAKE_EVENT_E WakeEvent)
{
    UNUSED(WakeEvent);
    SKY_PlaySound(SoundType, Sound);
}

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
void SKY_API SKY_PlayCallFailSound(SOUND_PREDEF_E Sound)
{
    switch (Sound) 
    {
    case SND_REORDER:
        SKY_PlaySoundInHandset(UI_CALLFAIL_SND, SND_REORDER, MOD_REORDER);
        break;

    case SND_ABRV_REORDER:
        SKY_PlaySoundInHandset(UI_CALLFAIL_SND, SND_ABRV_REORDER, MOD_REORDER);
        break;

    case SND_INTERCEPT:
        SKY_PlaySoundInHandset(UI_CALLFAIL_SND, SND_INTERCEPT, MOD_INTERCEPT);
        break;

    case SND_ABRV_INTERCEPT:
        SKY_PlaySoundInHandset(UI_CALLFAIL_SND, SND_ABRV_INTERCEPT, MOD_INTERCEPT);
        break;

    case SND_FADE_TONE:
        SKY_PlaySoundInHandset(UI_CALLFAIL_SND, SND_FADE_TONE, MOD_FADE);
        break;

    default:
        SKY_ASSERT(FALSE);
    }
}

/*===========================================================================
FUNCTION play_tone_signal

DESCRIPTION
  play tone signal 

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void SKY_API sPlayToneSignal(TONE_SIGNAL_E Signal)
{
    switch (Signal) 
    {
    case CAI_TONE_DIAL:
        SKY_PlaySound(UI_CALL_EAR_SND, SND_DIAL_TONE);
        break;
    case CAI_TONE_RING:
        SKY_PlaySound(UI_CALL_EAR_SND, SND_RING_BACK);
        break;
    case CAI_TONE_BUSY:
        SKY_PlaySound(UI_CALL_EAR_SND, SND_BUSY_ALERT);
        break;
    case CAI_TONE_ANSWER:
        SKY_PlaySound(UI_CALL_EAR_SND, SND_ANSWER);
        break;

    case CAI_TONE_INTERCEPT:
        SKY_PlayCallFailSound(SND_INTERCEPT);
        break;
    case CAI_TONE_ABBR_INTERCEPT:
        SKY_PlayCallFailSound(SND_ABRV_INTERCEPT);
        break;
    case CAI_TONE_REORDER:
        SKY_PlayCallFailSound(SND_REORDER);
        break;
    case CAI_TONE_ABBR_REORDER:
        SKY_PlayCallFailSound(SND_ABRV_REORDER);
        break;

    case CAI_TONE_CONFIRM:
        SKY_PlaySoundInHandset(UI_CALL_SND, SND_CONFIRMATION, MOD_SOUND);
        break;
    case CAI_TONE_CALLWAIT:
        SKY_PlaySoundInHandset(UI_CALL_SND, SND_CALL_WAITING, MOD_SOUND);
        break;

    case CAI_TONE_OFF:
        //call fail sound is multitone if FEATURE_UI_ALT_CALL_FAIL defined
        //so we need to stop both multitone and sound
        SKY_StopMultiTone();
        SKY_StopSound();
        break;
    default:
        SKY_StopSound();
        break;
    }
}


/* <EJECT> */
/*===========================================================================
FUNCTION play_snd_or_vibrator

DESCRIPTION
  playing vibrator or play sound in handset/module

DEPENDENCIES
  depends on ui.vibrator or ui.onhook

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/

/*
SoundType : type of sound
Sound     : which sound
WakeEvent : module event, used only if MODULE defined
*/
LOCAL void SKY_API sPlaySoundOrVibrator(SOUND_TYPE_E SoundType,
                                         SOUND_PREDEF_E Sound,
                                         SYS_WAKE_EVENT_E WakeEvent)
{
/*    boolean vibrator_is_on;

    vibrator_is_on = FALSE;

    if (vibrator_is_on && !ui.onhook) 
    {

    } 
    else */
    if( SKY_IS_HANDS_FREE() )
    {
        SKY_PlaySound(SoundType, Sound);
    }

}

/* <EJECT> */
/*===========================================================================
FUNCTION play_isdn_alert

DESCRIPTION
  play IDSN alert 

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
/*
IDSNAlert : indicate which signals to play
*/
LOCAL void SKY_API sPlayISDNAlert(ISDN_ALERT ISDNAlert)
{
    switch (ISDNAlert) 
    {
    case CAI_ISDN_NORMAL:
        sPlaySoundOrVibrator(UI_ALERT_SND, SND_NORMAL_ALERT, MOD_ISDN_ALERT);
        UI_HORN_ALERT();
        break;

    case CAI_ISDN_INTERGROUP:
        sPlaySoundOrVibrator(UI_ALERT_SND,SND_INTR_GROUP_ALERT,MOD_ISDN_ALERT);
        UI_HORN_ALERT();
        break;

    case CAI_ISDN_SPECIAL:
        sPlaySoundOrVibrator(UI_ALERT_SND, SND_SPCL_ALERT, MOD_ISDN_ALERT);
        UI_HORN_ALERT();
        break;

    case CAI_ISDN_PING:
        // note!! in old UI, this is different from all other cases
        // design by purpose or  a bug ???
        SKY_PlaySoundInHandset(UI_ALERT_SND, SND_PING_RING, MOD_ISDN_ALERT);
        break;

    case CAI_ISDN_OFF:
      SKY_StopSound();
      break;

    default:
      // follow old UI, treat it as CAI_ISDN_NORMAL
      sPlaySoundOrVibrator(UI_ALERT_SND, SND_NORMAL_ALERT, MOD_ISDN_ALERT);
      UI_HORN_ALERT();
      break;
    }
}

/*===========================================================================
FUNCTION play_is54b_alert

DESCRIPTION
  play IS54B alert 

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
/*
nPitch : medium, high or low
nSignal : there are 12 types of IS54 signals
*/
LOCAL void SKY_API sPlayIS54BAlert(PITCH_LEVEL_E Pitch, 
                                    TONE_SIGNAL_E Signal)
{
#define IS54_SIGNAL_NUM        12 // there are 12 types of IS54 signals
#define PITCH_LEVEL_NUM        3  // medium, high and low

    /* sounds for IS54 signaling */
    STATIC SOUND_PREDEF_E aSoundIS54[IS54_SIGNAL_NUM][PITCH_LEVEL_NUM] = {
        { (SOUND_PREDEF_E)0, (SOUND_PREDEF_E)0, (SOUND_PREDEF_E)0 },
        { SND_IS54B_LONG_M_ALERT, SND_IS54B_LONG_H_ALERT, 
          SND_IS54B_LONG_L_ALERT },
        { SND_IS54B_SS_M_ALERT, SND_IS54B_SS_H_ALERT,   
          SND_IS54B_SS_L_ALERT },
        { SND_IS54B_SSL_M_ALERT, SND_IS54B_SSL_H_ALERT,  
          SND_IS54B_SSL_L_ALERT },
        { SND_IS54B_SS2_M_ALERT, SND_IS54B_SS2_H_ALERT,
          SND_IS54B_SS2_L_ALERT },
        { SND_IS54B_SLS_M_ALERT, SND_IS54B_SLS_H_ALERT,  
          SND_IS54B_SLS_L_ALERT },
        { SND_IS54B_SSSS_M_ALERT, SND_IS54B_SSSS_H_ALERT, 
          SND_IS54B_SSSS_L_ALERT },
        { SND_IS54B_PBX_LONG_M_ALERT, SND_IS54B_PBX_LONG_H_ALERT, 
          SND_IS54B_PBX_LONG_L_ALERT },
        { SND_IS54B_PBX_SS_M_ALERT, SND_IS54B_PBX_SS_H_ALERT, 
          SND_IS54B_PBX_SS_L_ALERT },
        { SND_IS54B_PBX_SSL_M_ALERT, SND_IS54B_PBX_SSL_H_ALERT, 
          SND_IS54B_PBX_SSL_L_ALERT },
        { SND_IS54B_PBX_SLS_M_ALERT, SND_IS54B_PBX_SLS_H_ALERT, 
          SND_IS54B_PBX_SLS_L_ALERT },
        { SND_IS54B_PBX_SSSS_M_ALERT, SND_IS54B_PBX_SSSS_H_ALERT, 
          SND_IS54B_PBX_SSSS_L_ALERT }
    };

    CONST INT IS54_DEFAULT_SIGNAL   = 1;
    /* --
    BOOL fIsVibratorisOn;
    BOOL fMultipleRingerEnalbe;
    BOOL fRemoteRingerEnalbe;

    fIsVibratorisOn = FALSE;
    fMultipleRingerEnalbe = FALSE;
    fRemoteRingerEnalbe = FALSE;
    -- */
    if (Signal >= IS54_SIGNAL_NUM) 
    {
        Signal = IS54_DEFAULT_SIGNAL;   // set to default for invalid signals
    }
    if (Pitch >= PITCH_LEVEL_NUM) 
    {
        Pitch = (PITCH_LEVEL_E)CAI_PITCH_MED;          // default pitch is medium
    }

    if (Signal > 0) 
    {
        // verify array access won't be out of bound
        ASSERT(Signal < ARR_SIZE(aSoundIS54) );
        ASSERT(Pitch < ARR_SIZE(aSoundIS54[0]) );

        SKY_PlaySoundInHandset(UI_ALERT_SND, aSoundIS54[Signal][Pitch], 
                               MOD_IS54B_ALERT);
        /* ---
        if (fIsVibratorisOn && !ui.onhook) {
          
        } 
        else 
        {
            if (aSoundIS54[nSignal][nPitch] == SND_IS54B_LONG_M_ALERT && 
               fMultipleRingerEnalbe && !fRemoteRingerEnalbe) 
            {

                db_items_value_type dbi;  // buffer to store db item
                db_get(DB_ROAM, &dbi);
                if ( (dbi.roam) && (ui.roam_ringer) ) 
                {
                    uisnd_snd(UI_ALERT_SND, SND_ROAM_RING);
                } 
                else 
                {
                    uisnd_snd(UI_ALERT_SND, SND_ALERT);
                }

            } 
            else 
            {
                uisnd_play_snd(UI_ALERT_SND, aSoundIS54[Signal][Pitch], MOD_IS54B_ALERT);
            }
        }
        -- */
        
    } 
    else 
    {
        SKY_StopSound();
    }
    UI_HORN_ALERT();
}

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
/*
SignalType : tone signal/ISDN alerting/IS-54B alert
Signal : indicate which signals to play
Pitch : medium, high or low
*/

void SKY_API SKY_PlaySignal(SIGNAL_TYPE_E SignalType, 
                            TONE_SIGNAL_E Signal, 
                            PITCH_LEVEL_E Pitch)
{
    switch (SignalType) 
    {
    case CAI_SIG_TONE:
        sPlayToneSignal(Signal);
        break;
    case CAI_SIG_ISDN:
        sPlayISDNAlert(Signal);
        break;
    case CAI_SIG_IS54B:
        sPlayIS54BAlert(Pitch, Signal);
        break;
    default:
        SKY_TRACE1(TL_HIGH, "Unknown SIGNAL type: %d", SignalType);
        break;
    }
}

/*===========================================================================
FUNCTION uisnd_music

DESCRIPTION
  Play a music (call, alarm, sms)
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   

LOCAL void SKY_API sPlayMusic(SOUND_TYPE_E SoundType, BYTE *pszFileName,
                           BYTE *pbMusicBuf, MUSIC_ID_E MusicID,
                           WORD wRepeatCount,
                           WORD wVolume,
                           BOOL fSampleSound)
{
    SOUND_COMMAND_U *pCommand;
    SOUND_DEVICE_E  SoundDevice;
    SOUND_CLASS_E   SoundClass;
    
    /*
    /* SND CLASS에 따라 EarJack이 연결됐을때 나는 소리가 다름
    /*----------------------------------------------------------------
    /* SND_CLASS_MUSIC - 소리가 Earjack과 스피커 양쪽으로 남
    /* SND_CLASS_SAMPLE_MUSIC - 소리가 이어잭으로만 나옴. 이어잭을 뽑으면 스피커로 나옴
    /* SND_CLASS_BGM - 소리가 Earjack으로만 나옴.  Earjack을 뽑으면 소리가 안남
    */

    if(SKY_IS_HANDS_FREE())
    {
        SoundDevice = SND_DEVICE_CURRENT;
        SoundClass = SND_CLASS_MUSIC;
    }
    else if(gnVocoderMode != VR_NONE ||  SKY_IS_ETIQUETTE())
    {
        SoundDevice = SND_DEVICE_EARPHONE;
        SoundClass = SND_CLASS_BGM;
    }
    else if(SKY_IS_ALERT() )
    {
        if(SKY_IS_AUTO_ANSWER() && IsSpace2RecordAutoAnswer()) 
        {
            switch(SKY_GET_AUTO_ANSWER_BELL_TIME()){
            case AAB_VIB_ONCE:
                SoundDevice = SND_DEVICE_EARPHONE;
                SoundClass = SND_CLASS_BGM;
                break;
            case AAB_BELL_5SECOND:
            case AAB_BELL_10SECOND:
                SoundDevice = SND_DEVICE_CURRENT;
                SoundClass = SND_CLASS_MUSIC;
                break;
            default:
                SoundDevice = SND_DEVICE_CURRENT;
                SoundClass = SND_CLASS_MUSIC;
                break;
            }
        }
        else if(SKY_GET_BELL_VIBRATION() == BV_VIBRATION)
        {
            SoundDevice = SND_DEVICE_EARPHONE;
            SoundClass = SND_CLASS_BGM;
        }
        else
        {
            SoundDevice = SND_DEVICE_CURRENT;
            SoundClass = SND_CLASS_MUSIC;
        }
    }
    else
    {
        SoundDevice = SND_DEVICE_CURRENT;
        SoundClass = SND_CLASS_MUSIC;
    }

    if(fSampleSound)
        SoundClass = SND_CLASS_SAMPLE_MUSIC;

    if((pCommand = snd_get_packet()) != NULL)
    {
        pCommand->hdr.cmd          = SND_MUSIC_START;
        pCommand->hdr.task_ptr     = NULL;
        pCommand->music.device     = SoundDevice;
        pCommand->music.sclass     = SoundClass;
        pCommand->music.file_name  = (SoundType == UI_MUSIC_FILE_SND) \
                                    ? (char *)pszFileName : NULL;
        pCommand->music.music_buf  = ((SoundType == UI_MUSIC_BUFF_SND) \
                                    ? (uint8 *)pbMusicBuf : NULL);
        pCommand->music.music_id   = MusicID;
        pCommand->music.volume     = sIntToLevel(wVolume);
        pCommand->music.repeat_cnt = wRepeatCount;
        snd_cmd(pCommand);
    }
} // uisnd_music()

void SKY_API SKY_ControlMusicVolume(WORD wVolume)
{
    SOUND_COMMAND_U *pCommand;
    if((pCommand = snd_get_packet()) != NULL)
    {
        pCommand->hdr.cmd          = SND_SPEAKER_VOLUME;
        pCommand->music.volume     = sIntToLevel(wVolume);
        snd_cmd(pCommand);
    }
}
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
LOCAL void SKY_API sPlayMusic(BYTE *pszFileName,
                              SOUND_ID_E MusicID,
                              UINT8 nRepeatCount,
                              WORD wVolume)
{
    snd_melody_type kind;

    if(pszFileName == NULL)
    {
        if(wVolume == 0xFFFF)
            SKY_PlaySound( UI_MUSIC_SND, MusicID); 
        else
            SKY_PlaySoundEx(UI_MUSIC_SND, MusicID, wVolume, nRepeatCount);
    }
    else
    {
        if(wVolume != 0xFFFF)
        {
	        snd_sample_music_start( sChooseDevice(UI_MUSIC_SND), 
			                        sChooseMethod(UI_MUSIC_SND),
			                        0,				/* file type                            */
			                        (char *)pszFileName,
			                        sIntToLevel(wVolume),
			                        nRepeatCount,   /* Nbr of times to repeat before stop   */
			                        NULL,			/* Call back function                   */ 
			                        NULL		);	/* Pointer to Client data               */
        }
        else
        {
            if( SKY_IS_ALERT() 
                && SKY_IS_EAR_JACK_ON() 
                && ( SKY_GET_BELL_VIBRATION() == BV_VIBRATION || SKY_IS_ETIQUETTE() ) )
            {
                kind = SND_MELODY_NORMAL;
            }
            else if( SKY_IS_ALERT() )
            {
	            /// 착신벨일때와 아닐때의 차이를 SND_PRIO_HIGH로 둔다.
                kind = SND_MELODY_ALERT;
            }
            else
            {
                kind = SND_MELODY_NORMAL;
            }
        	snd_music_start(sChooseDevice(UI_MUSIC_SND), 
			                sChooseMethod(UI_MUSIC_SND),
			                0,				/* file type                            */
			                (char *)pszFileName,
			                0, 	            /* Nbr of times to repeat before stop   */
			                kind,
			                NULL,			/* Call back function                   */ 
			                NULL   );			/* Pointer to Client data               */
        }
    }
}
#endif//#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
/*===========================================================================
FUNCTION 

DESCRIPTION
  Play buffer sound (call snd_music_buf_start in snd.c)
===========================================================================*/
void SKY_API sPlayMusicBuffer(WORD *pwMusicBuf, WORD wVolume, 
                              BOOL fVolumeSet)
{

    if(fVolumeSet)
    {
        snd_sample_music_buf_start(SND_DEVICE_CURRENT,
                                   SND_METHOD_MUSIC,
                                   pwMusicBuf,
                                   wVolume,
                                   1,
                                   NULL,
                                   NULL);
    }
    else
    {
        snd_music_buf_start(SND_DEVICE_CURRENT,
                            SND_METHOD_MUSIC,
                            pwMusicBuf,
                            SND_PRIO_LOW,
                            1,
                            NULL,
                            NULL);
    }
}

#endif  // #if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
/* 
SND_CLASS_MUSIC
SND_CLASS_SAMPLE_MUSIC
SND_CLASS_BGM 
*/

/*===========================================================================
FUNCTION 
    SKY_PlayMusic

DESCRIPTION
    Play Music in ROM
===========================================================================*/

void SKY_API SKY_PlayMusic(MUSIC_ID_E MusicID)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
    SOUND_VOLUME_T wVolume;

    sChooseVolume( UI_MUSIC_ID_SND, &wVolume );

    sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, MusicID, 
               0, (WORD)wVolume.phone, FALSE);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    sPlayMusic(NULL, MusicID, 0, 0xFFFF);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE   
}

void SKY_API SKY_PlayMusicEx(MUSIC_ID_E MusicID, WORD wVolume, 
                             WORD nRepeatCount)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, MusicID, 
               nRepeatCount, (WORD)wVolume, TRUE);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    sPlayMusic(NULL, MusicID, nRepeatCount, wVolume);
#endif
}

void SKY_API SKY_PlayMusicTime(MUSIC_ID_E MusicID, UINT16 nDuration)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SOUND_VOLUME_T wVolume;

    sChooseVolume( UI_MUSIC_ID_SND, &wVolume );

    sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, MusicID, 
               0, (WORD)wVolume.phone, FALSE);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    sPlayMusic(NULL, MusicID, 0, 0xFFFF);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE   

    SKY_SetSoundStopTimer(nDuration);
}
/*===========================================================================
FUNCTION 
    SKY_PlayMusicFile

DESCRIPTION
    Play Sound file
===========================================================================*/

void SKY_API SKY_PlayMusicFile(BYTE *pszFileName)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SOUND_VOLUME_T wVolume;

    sChooseVolume( UI_MUSIC_ID_SND, &wVolume );

    sPlayMusic(UI_MUSIC_FILE_SND, pszFileName, NULL, 0, 
               0, (WORD)wVolume.phone, FALSE);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    sPlayMusic(pszFileName, NULL, 0, 0xFFFF);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE   
}

void SKY_API SKY_PlayMusicFileEx(BYTE *pszFileName, WORD wVolume)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    sPlayMusic(UI_MUSIC_FILE_SND, pszFileName, NULL, 0, 
               0, (WORD)wVolume, TRUE);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE 
    sPlayMusic(pszFileName, NULL, 0, wVolume);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE   
}

void SKY_API SKY_PlayMusicFileTime(BYTE *pszFileName, UINT16 nDuration)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SOUND_VOLUME_T wVolume;

    sChooseVolume( UI_MUSIC_ID_SND, &wVolume );

    sPlayMusic(UI_MUSIC_FILE_SND, pszFileName, NULL, 0, 
               0, (WORD)wVolume.phone, FALSE);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
    sPlayMusic(pszFileName, NULL, 0, 0xFFFF);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE   

    SKY_SetSoundStopTimer(nDuration);
}
/*===========================================================================
FUNCTION 
    SKY_PlayMusicBuffer

DESCRIPTION
    Play buffered sound
===========================================================================*/

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE  
void SKY_API SKY_PlayMusicBuffer(BYTE *pbMusicBuf) 
{
    SOUND_VOLUME_T wVolume;

    sChooseVolume( UI_MUSIC_ID_SND, &wVolume );
    
    sPlayMusic(UI_MUSIC_BUFF_SND, NULL, pbMusicBuf, 0, 
               0, (WORD)wVolume.phone, FALSE);
}
void SKY_API SKY_PlayMusicBufferEx(BYTE *pbMusicBuf, WORD wVolume, WORD nRepeatCount)
{
    sPlayMusic(UI_MUSIC_BUFF_SND, NULL, pbMusicBuf, 0, 
               nRepeatCount, wVolume, TRUE);
}
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
void SKY_API SKY_PlayMusicBuffer(WORD *pwMusicBuf) 
{
    sPlayMusicBuffer(pwMusicBuf, 0, FALSE);
}
void SKY_API SKY_PlayMusicBufferEx(WORD *pwMusicBuf, WORD wVolume)
{ 
    sPlayMusicBuffer(pwMusicBuf, wVolume, TRUE);
}
#endif



/*===========================================================================
FUNCTION uisnd_music_stop

DESCRIPTION
    stop the current music
===========================================================================*/
void SKY_API SKY_StopMusic(void)
{
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE   
    SOUND_COMMAND_U *pCommand;

    if((pCommand = snd_get_packet()) != NULL)
    {
        pCommand->hdr.cmd          = SND_MUSIC_STOP;
        snd_cmd(pCommand);
    }
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
    snd_sound_stop(SND_PRIO_LOW, NULL, NULL);
    snd_music_stop(NULL, NULL);
    snd_voice_stop(NULL, NULL);
#endif//#if SKY_SOUND_INTERFACE == SKY_xxxx_SOUND_INTERFACE   

    fPlayBell = FALSE;
} // uisnd_music_stop()

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
void SKY_API SKY_PlayAlarm(MUSIC_ID_E MusicID)
{
    SKY_PlaySound(UI_ALARM_SND, MusicID);
}
#endif  // SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   

/*===========================================================================
FUNCTION 
    sIntToLevel

DESCRIPTION
    Convert volume to sound level
===========================================================================*/

LOCAL WORD SKY_API sIntToLevel(UINT8 nLevel)
{
    switch(nLevel)
    {
    case 1:
        return UISND_1ST_VOL;
    case 2:
        return UISND_2ND_VOL;
    case 3:
        return UISND_3RD_VOL;
    case 4:
        return UISND_4TH_VOL;
    case 5:
        return UISND_5TH_VOL;
    }

    return (word)0;
}

////////////////////////////////////////////////////////////////////////////
/*===========================================================================
FUNCTION 
    SKY_DoAlertSignal

DESCRIPTION
    Play call alerting signal
===========================================================================*/

void SKY_API SKY_DoAlertSignal(void)
{
    BYTE    fSignal, fAlertPitch;
    fSignal = 1;
    fAlertPitch = GetCallSignalAlertPitch();

    switch( GetCallSignalType() ) {
    // GetCallSignalType()에서 CAI_SIG_TONE을 return한다..왜??--;
      case CAI_SIG_TONE:        
      case CAI_SIG_ISDN:         
      case CAI_SIG_IS54B:        

        if( fSignal > IS54_LAST ) {
          fSignal = 1; 
        }
        if( fAlertPitch > TONE_LAST ) {
          fAlertPitch = CAI_PITCH_MED;
        }
        if( fSignal > 0 ) {

			if(IsDataCall())
            {
                sPlaySigDataCall();
            }

            else if (SKY_IS_AUTO_ANSWER() && IsSpace2RecordAutoAnswer())  
            {
                sPlayAutoAnswerAlert();
            }

            // 자동 착신
            else if((SKY_IS_HANDS_FREE_AUTO_RECEIPT() && SKY_IS_HANDS_FREE()) ||
                    (SKY_IS_EAR_PHONE_AUTO_RECEIPT() && SKY_IS_EAR_JACK_ON()))
            {
                SKY_SetBellTimer(10000, FALSE, SE_ANSWER);
                sPlaySigCall();
            }
            else
            {
                sPlaySigCall();
            }

        }
        break;

/*
      case CAI_SIG_TONE:        
          sPlaySigTone(fSignal);
          break;

      case CAI_SIG_ISDN:         
          sPlaySigISDN(fSignal);
          break;
*/
      default:
          SKY_StopSound();
          SKY_MotorStop();
          // Stop motoroff timer
    }

}

void SKY_API SKY_EndAlertSignal(void)
{
    SKY_StopMusic();
    SKY_MotorStop();

    // Stop Timer
    SKY_SetBellTimer(0, FALSE, SE_ANSWER);
    SKY_SetSoundStopTimer(0);
}

void SKY_API SKY_PlayBellAfterVib(void)
{
    UINT16 nBellTime = 0;

    if(SKY_GET_BELL_TIME() != BT_CONTINUATION)
        nBellTime = SKY_GET_BELL_TIME() * 5000;
    else 
        nBellTime = 0;

    SKY_PlayAlertSound(nBellTime);
}
/*===========================================================================
FUNCTION 
    SKY_PlayAlertSignal

DESCRIPTION
    Play call alerting signal
===========================================================================*/
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE 
#define MUS_MYBELL_SOUND 0xffff
#endif

void SKY_API SKY_PlayAlertSound(UINT16 nDuration )
{
    CNI_T*                  pCallerID;
    WORD                    MelodyType = 0; 

#ifdef FEATURE_CHARACTER_BELL
    EDITORINFO_T aaa;
    char        sPhoneNumber[MAX_TITLE_LENGTH+1], phone[MAX_PHONENUMBER_LENGTH];
    char        szFileName[MAX_TITLE_LENGTH];
    char        *loc;
    UINT8       index;
    BOOL        fCharacbellUsed = FALSE;
    BOOL        fCBGroupBellUsed = FALSE;
#endif // FEATURE_CHARACTER_BELL
    
    pCallerID   =  SKY_GetCallerID();

#ifdef FEATURE_CHARACTER_BELL
    aaa = pCallerID->Num;
    memset(phone, 0x00, MAX_PHONENUMBER_LENGTH);
    memset(sPhoneNumber, 0x00, MAX_TITLE_LENGTH+1);
    memset(szFileName, 0x00, MAX_TITLE_LENGTH);

    strcpy(sPhoneNumber, (char *)aaa.aBuf);

    if ((SKY_GET_CB_ADDRESS() != 0) && (strlen(sPhoneNumber) != 0)) {
        sPhoneNumber[strlen(sPhoneNumber)] = ',';
        for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
            if (SKY_GET_CB_EMPTY(index)) {
                strcpy(phone, SKY_GET_CB_PHONE(index));
                loc = strstr(phone, sPhoneNumber);
                if (loc != NULL) { 
                    MelodyType = SKY_GET_CB_MTYPE(index);
                    strcpy(szFileName, SKY_GET_CB_MTITLE(index));
                    fCharacbellUsed = TRUE;
                    break;
                }
            }
        }
    }

    if (SKY_IS_CB_GROUPBELL(pCallerID->GroupBell.wBellID)) {
        // 착신번호로는 연동 안되었으나 그룹단위의 캐릭벨인 경우
        char temp [MAX_TITLE_LENGTH];
        STRCPY((BYTE*)temp, pCallerID->GroupBell.szFileName);
        for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
            if (strcmp(temp, SKY_GET_CB_TITLE(index)) == 0) {
                fCBGroupBellUsed = TRUE;
                break;
            }
        }
    }
#endif // FEATURE_CHARACTER_BELL

    // Events : SE_CALL_STOP_BELL
    //          SE_AUTO_ANSWER
    if( SKY_IS_ETIQUETTE() || (gnVocoderMode != VR_NONE && !SKY_IS_HANDS_FREE()))
	{
        if(!SKY_IS_HANDS_FREE())
            SKY_MotorStart(1500);

		if(!SKY_IS_EAR_JACK_ON())
            return;
    }

#ifdef FEATURE_CHARACTER_BELL
    if (fCharacbellUsed) { // 캐릭벨에 연동된 착신번호인 경우
        if(SKY_IS_MY_BELL(MelodyType))   // Play file
        {
        
            SKY_StopMusic();
            CurrentBellType.wBellID = MelodyType;
            STRCPY(CurrentBellType.szFileName, (BYTE *)szFileName);

        
            if(nDuration > 0)
                SKY_PlayMusicFileTime((BYTE *)szFileName, nDuration);
            else
                SKY_PlayMusicFile((BYTE *)szFileName);

        }
        else
        {
            SKY_StopMusic();
            CurrentBellType.wBellID = MelodyType;
            if(nDuration > 0)
                SKY_PlayMusicTime(MelodyType, nDuration);
            else
                SKY_PlayMusic(MelodyType);
        }
    } // 캐릭벨에 연동된 번호일 때
    else if (!fCharacbellUsed && fCBGroupBellUsed) {
        MelodyType = SKY_GET_CB_MTYPE(index);
        strcpy(szFileName, SKY_GET_CB_MTITLE(index));

        if(SKY_IS_MY_BELL(MelodyType))   // Play file
        {
            SKY_StopMusic();
            CurrentBellType.wBellID = MelodyType;
            STRCPY(CurrentBellType.szFileName, (BYTE *)szFileName);

        
            if(nDuration > 0)
                SKY_PlayMusicFileTime((BYTE *)szFileName, nDuration);
            else
                SKY_PlayMusicFile((BYTE *)szFileName);

        }
        else
        {
            SKY_StopMusic();
            CurrentBellType.wBellID = MelodyType;
            if(nDuration > 0)
                SKY_PlayMusicTime(MelodyType, nDuration);
            else
                SKY_PlayMusic(MelodyType);
        }
    }
    else if(pCallerID->CalledParty.fChild)   // Two number Bell
#else
    if(pCallerID->CalledParty.fChild)   // Two number Bell
#endif // FEATURE_CHARACTER_BELL
    {
        if(SKY_IS_MY_BELL(SKY_GET_NUMBER_PLUS_BELL_ID()))    // Play file
        {
            if(fPlayBell)
            {
                if(!STRCMP(CurrentBellType.szFileName, SKY_GET_NUMBER_PLUS_BELL_FILENAME()))
                    return;
            }
            
            CurrentBellType.wBellID = SKY_GET_NUMBER_PLUS_BELL_ID();
            STRCPY(CurrentBellType.szFileName, SKY_GET_NUMBER_PLUS_BELL_FILENAME());

            SKY_StopMusic();
            if(nDuration > 0)
                SKY_PlayMusicFileTime(SKY_GET_NUMBER_PLUS_BELL_FILENAME(), nDuration);
            else
                SKY_PlayMusicFile(SKY_GET_NUMBER_PLUS_BELL_FILENAME());
        }
        else
        {
            if(fPlayBell)
            {
                if(CurrentBellType.wBellID == SKY_GET_NUMBER_PLUS_BELL_ID())
                    // same sound
                    return;
            }

            CurrentBellType.wBellID = SKY_GET_NUMBER_PLUS_BELL_ID();

            SKY_StopMusic();
            if(nDuration > 0)
                SKY_PlayMusicTime(SKY_GET_NUMBER_PLUS_BELL_ID(), nDuration);
            else
                SKY_PlayMusic(SKY_GET_NUMBER_PLUS_BELL_ID());
        }
    }
    else if(pCallerID->GroupBell.wBellID)   // 그룹벨
    {
        if(SKY_IS_MY_BELL(pCallerID->GroupBell.wBellID))    // Play file
        {
            if(fPlayBell)
            {
                if(!STRCMP(CurrentBellType.szFileName, pCallerID->GroupBell.szFileName))
                    return;
            }
            
            CurrentBellType.wBellID = pCallerID->GroupBell.wBellID;
            STRCPY(CurrentBellType.szFileName, pCallerID->GroupBell.szFileName);

            SKY_StopMusic();
            if(nDuration > 0)
                SKY_PlayMusicFileTime(pCallerID->GroupBell.szFileName, nDuration);
            else
                SKY_PlayMusicFile(pCallerID->GroupBell.szFileName);
        }
        else
        {
            if(fPlayBell)
            {
                if(CurrentBellType.wBellID == pCallerID->GroupBell.wBellID)
                    // same sound
                    return;
            }

            CurrentBellType.wBellID = pCallerID->GroupBell.wBellID;

            SKY_StopMusic();
            if(nDuration > 0)
                SKY_PlayMusicTime(pCallerID->GroupBell.wBellID, nDuration);
            else
                SKY_PlayMusic(pCallerID->GroupBell.wBellID);
        }
    }
    else if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))   // Play file
    {
        if(fPlayBell)
        {
            if(!STRCMP(CurrentBellType.szFileName, SKY_GET_BELL_FILE_NAME()))
                return;
        }
        SKY_StopMusic();
        CurrentBellType.wBellID = SKY_GET_BELL_ID();
        STRCPY(CurrentBellType.szFileName, SKY_GET_BELL_FILE_NAME());
    
        if(nDuration > 0)
            SKY_PlayMusicFileTime(SKY_GET_BELL_FILE_NAME(), nDuration);
        else
            SKY_PlayMusicFile(SKY_GET_BELL_FILE_NAME());
    }
    else
    {
        if(fPlayBell)
        {
            if(CurrentBellType.wBellID == SKY_GET_BELL_ID())
                // same sound
                return;
        }
        SKY_StopMusic();

        CurrentBellType.wBellID = SKY_GET_BELL_ID();
        if(nDuration > 0)
            SKY_PlayMusicTime(SKY_GET_BELL_ID(), nDuration);
        else
            SKY_PlayMusic(SKY_GET_BELL_ID());
    }

    fPlayBell = TRUE;
}

LOCAL void EQS_API sPlaySigCall(void)
{
    UINT16 nBellTime = 0;

	if(SKY_IS_HANDS_FREE())
	{
        if(SKY_GET_BELL_TIME() != BT_CONTINUATION)
            nBellTime = SKY_GET_BELL_TIME() * 5000;

/*
		if(SKY_GET_BELL_VIBRATION() == BV_BELL_AFTER_VIBRATION)
        {
			nBellTime = 6000;
        }
*/
        SKY_PlayAlertSound(nBellTime);
	}
    else 
    {
	  switch(SKY_GET_BELL_VIBRATION()) {
		case BV_BELL : 
            if(SKY_GET_BELL_TIME() != BT_CONTINUATION)
                nBellTime = SKY_GET_BELL_TIME() * 5000;
            else
                nBellTime = 0;

            SKY_PlayAlertSound(nBellTime);
			break;

		case BV_VIBRATION : 
            SKY_MotorStart(1500);

            if(SKY_IS_EAR_JACK_ON())
                SKY_PlayAlertSound(0);

			break;

		case BV_BELL_AFTER_VIBRATION : 
            SKY_MotorStart(1500);

            // 자동 착신
            if((SKY_IS_HANDS_FREE_AUTO_RECEIPT() && SKY_IS_HANDS_FREE()) ||
                    (SKY_IS_EAR_PHONE_AUTO_RECEIPT() && SKY_IS_EAR_JACK_ON()))
                    break;

            SKY_SetBellTimer(6000, FALSE, SE_CALL_PLAY_BELL);
			break;

		case BV_BOTH_BELL_VIBRATION: 
            if(SKY_GET_BELL_TIME() != BT_CONTINUATION)
                nBellTime = SKY_GET_BELL_TIME() * 5000;
            else 
                nBellTime = 0;

            SKY_MotorStart(1500);
            SKY_PlayAlertSound(nBellTime);
			break;

        default:
            break;
      }
    }
}

void EQS_API sPlayAutoAnswerAlert(void)
{
    UINT16 nBellTime = 0;

    if(SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_VIB_ONCE ||
       SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_BELL_5SECOND)
       nBellTime = 5000;
    else if(SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_BELL_10SECOND)
        nBellTime = 10000;
       
	switch (SKY_GET_AUTO_ANSWER_BELL_TIME())
	{
	case AAB_IMMEDIATE: 
        SKY_AddEvent(SE_AUTO_ANSWER);
		break;

	case AAB_VIB_ONCE: // Vibration once
        SKY_SetBellTimer(nBellTime, FALSE, SE_AUTO_ANSWER);

        if(SKY_IS_HANDS_FREE())
        {
            SKY_PlayAlertSound(nBellTime);
        }
        else
        {
		    SKY_MotorTime(1500);

		    if(SKY_IS_EAR_JACK_ON())
                SKY_PlayAlertSound(nBellTime);
        }
		break;

	case AAB_BELL_5SECOND: // Bell once
	case AAB_BELL_10SECOND: // Bell twice

        SKY_SetBellTimer(nBellTime, FALSE, SE_AUTO_ANSWER);
        SKY_PlayAlertSound(nBellTime);
        break;

    default:
        break;
    }
}

/*
void EQS_API sPlaySigTone(BYTE Signal)
{
    switch( Signal ) {
    case CAI_TONE_DIAL:
        SKY_PlaySound( UI_CALL_EAR_SND, SND_DIAL_TONE );
        break;

    case CAI_TONE_RING:
        SKY_PlaySound( UI_CALL_EAR_SND, SND_RING_BACK );
        break;

    case CAI_TONE_INTERCEPT:
        SKY_PlaySound( UI_CALL_SND, SND_INTERCEPT );
        break;

    case CAI_TONE_ABBR_INTERCEPT:
        SKY_PlaySound( UI_CALL_SND, SND_ABRV_INTERCEPT );
        break;

    case CAI_TONE_REORDER:
        SKY_PlaySound( UI_CALL_SND, SND_REORDER );
        break;

    case CAI_TONE_ABBR_REORDER:
        SKY_PlaySound( UI_CALL_SND, SND_ABRV_REORDER );
        break;

    case CAI_TONE_BUSY:
        SKY_PlaySound( UI_CALL_EAR_SND, SND_BUSY_ALERT );
        break;

    case CAI_TONE_CONFIRM:
        SKY_PlaySound( UI_CALL_SND, SND_CONFIRMATION );
        break;

    case CAI_TONE_ANSWER:
        SKY_PlaySound( UI_CALL_EAR_SND, SND_ANSWER );
        break;

    case CAI_TONE_CALLWAIT:
        SKY_PlaySound( UI_CALL_SND, SND_CALL_WAITING );
        break;

    case CAI_TONE_OFF:
#if (MODEL_ID == MODEL_B0)
		if(!snd_is_connection_tone_played())
#endif
			SKY_StopSound();
        break;

    default:
        SKY_StopSound();
        break;
    }
}
*/

/*
void EQS_API sPlaySigISDN(BYTE Signal)
{
    switch( Signal ) {
    case CAI_ISDN_NORMAL:
        SKY_PlaySound( UI_ALERT_SND, SND_NORMAL_ALERT );
        break;

    case CAI_ISDN_INTERGROUP:
        SKY_PlaySound( UI_ALERT_SND, SND_INTR_GROUP_ALERT );
        break;

    case CAI_ISDN_SPECIAL:
        SKY_PlaySound( UI_ALERT_SND, SND_SPCL_ALERT );
        break;

    case CAI_ISDN_PING:
        SKY_PlaySound( UI_ALERT_SND, SND_PING_RING );
        break;

    case CAI_ISDN_OFF:
        SKY_StopSound();
        break;

    default:
        SKY_PlaySound( UI_ALERT_SND, SND_NORMAL_ALERT );
        break;
    }
}
*/

void EQS_API sPlaySigDataCall(void)
{
    switch(SKY_GET_BELL_VIBRATION()) {
    case BV_BELL : 
        if (SKY_IS_ETIQUETTE() || gnVocoderMode == VR_RING)
        {
            SKY_MotorStart(1500);
            if(SKY_IS_EAR_JACK_ON())
                SKY_PlayAlertSound(0);
        }
        else
        {
#ifdef FEATURE_GVM
        if(!SKY_IS_FROM_GVM())
#endif
            SKY_PlayAlertSound(0);
        }
        break;

    case BV_VIBRATION : 
	    SKY_MotorStart(1500);
	    break;

    case BV_BELL_AFTER_VIBRATION : 
	    SKY_MotorStart(1500);
#ifdef FEATURE_GVM
        if(!SKY_IS_FROM_GVM())
#endif
            SKY_SetBellTimer(6000, FALSE, SE_CALL_PLAY_BELL);
	    break;

    case BV_BOTH_BELL_VIBRATION : 
	    SKY_MotorStart(1500);

        if (gnVocoderMode == VR_NONE)
        {
#ifdef FEATURE_GVM
          if(!SKY_IS_FROM_GVM())
#endif
                SKY_PlayAlertSound(0);
        }

        break;

    default :
	    SKY_MotorStop();
	    break;
    }
}

/*===========================================================================
FUNCTION 
    SKY_DoSMSAlert()

DESCRIPTION
    Play call alerting signal
===========================================================================*/

void EQS_API SKY_DoSMSAlert(void)
{
//    extern BOOL    fIsGame;

    if(ui_is_in_conversation() || SKY_IS_ALERT() || SKY_IS_ORIGINATE() 
        || SKY_IS_IN_GAME() || SKY_IS_IN_ALARM()
#ifdef FEATURE_WAP
        || SKY_IS_IN_WAP()
#endif //#ifdef FEATURE_WAP
#ifdef FEATURE_GVM
        || SKY_IS_IN_GVM() )
#else
        )
#endif
    {
        if(!SKY_IS_HANDS_FREE())
            SKY_MotorTime(1500);
        return;
    }

    if(gnVocoderMode != VR_NONE || SKY_GET_BELL_VIBRATION() != BV_BELL ||
       SKY_IS_ETIQUETTE() || 
       (SKY_IS_AUTO_ANSWER() && IsSpace2RecordAutoAnswer() && SKY_GET_AUTO_ANSWER_BELL_TIME() <= AAB_VIB_ONCE))
    {
        if(!SKY_IS_HANDS_FREE() && SKY_GET_SMS_ALERT_TYPE() != SAT_NO)
            SKY_MotorTime(1500);

        if(SKY_IS_AUTO_ANSWER() && IsSpace2RecordAutoAnswer() && SKY_GET_AUTO_ANSWER_BELL_TIME() <= AAB_VIB_ONCE)
            return;
    }

    //if(SKY_GET_SMS_ALERT_TYPE() != SAT_NO && gnVocoderMode == VR_NONE && SKY_GET_BELL_VIBRATION() != BV_VIBRATION)
    if(SKY_GET_SMS_ALERT_TYPE() != SAT_NO && gnVocoderMode == VR_NONE && 
        (SKY_GET_BELL_VIBRATION() != BV_VIBRATION || SKY_IS_HANDS_FREE()))
    {
		sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, SKY_GET_SMS_BELL_ID(), 
           1, SKY_GET_SMS_BELL_VOLUME(), FALSE);
    }

    /* rosa 2001-10-31 --> 이어마이크 꼿은 상태에서 SMS 동작 안 해 수정 */
    if(SKY_IS_EAR_JACK_ON() && 
       (SKY_GET_SMS_ALERT_TYPE() != SAT_NO && gnVocoderMode == VR_NONE && 
       (SKY_GET_BELL_VIBRATION() == BV_VIBRATION || SKY_IS_HANDS_FREE())))
        //SKY_GET_SMS_ALERT_TYPE() != SAT_NO && gnVocoderMode == VR_NONE && SKY_GET_BELL_VIBRATION() == BV_VIBRATION)
    {
		sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, SKY_GET_SMS_BELL_ID(), 
           1, SKY_GET_SMS_BELL_VOLUME(), TRUE);
    }
    /* <-- */
}

/*===========================================================================
FUNCTION 
    SKY_DoShutter()

DESCRIPTION
    Play call alerting signal
===========================================================================*/

void EQS_API SKY_DoShutter(void)
{
    sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, MUS_SHUTTER, 
                    1, SKY_GET_SMS_BELL_VOLUME(), FALSE);
}

/*===========================================================================
FUNCTION 
    SKY_PlayAlarmBell()

DESCRIPTION
    Play call alerting signal
===========================================================================*/

void EQS_API SKY_PlayAlarmBell(void)
{
    if(ui_is_in_conversation() || SKY_IS_ALERT() || SKY_IS_ORIGINATE()
#ifdef FEATURE_WAP
        || SKY_IS_IN_WAP()
#endif //#ifdef FEATURE_WAP
#ifdef FEATURE_GVM
        || SKY_IS_IN_GVM() )
#else
        )
#endif
    {
        if(!SKY_IS_HANDS_FREE())
            SKY_MotorTime(1500);
        return;
    }

    if(gnVocoderMode != VR_NONE || SKY_IS_ETIQUETTE())
    {
        if(!SKY_IS_HANDS_FREE())
            SKY_MotorStart(1500);

        if(!SKY_IS_EAR_JACK_ON())
            return;
    }

    if(SKY_GET_BELL_VIBRATION() != BV_BELL && !SKY_IS_HANDS_FREE()) 
        SKY_MotorStart(1500);


	//if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))   // Play file
	if( SKY_IS_MY_BELL(SKY_GET_ALARM_BELL_ID()) )
	{
		sPlayMusic(UI_MUSIC_FILE_SND, (BYTE *)SKY_GET_ALARM_BELL_FILE_NAME(), NULL, 0, 
				   0, SKY_GET_ALARM_BELL_VOLUME(), FALSE);
	}
	else
	{
		sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, SKY_GET_ALARM_BELL_ID(), 
		   0, SKY_GET_ALARM_BELL_VOLUME(), FALSE);
	}
}

/*===========================================================================
FUNCTION 
    SKY_PlaySchedulerBell()

DESCRIPTION
    Play call alerting signal
===========================================================================*/

void EQS_API SKY_PlaySchedulerBell(void)
{
    if(ui_is_in_conversation() || SKY_IS_ALERT() || SKY_IS_ORIGINATE()
#ifdef FEATURE_WAP
        || SKY_IS_IN_WAP()
#endif //#ifdef FEATURE_WAP
#ifdef FEATURE_GVM
        || SKY_IS_IN_GVM() )
#else
        )
#endif
    {
        if(!SKY_IS_HANDS_FREE())
            SKY_MotorTime(1500);
        return;
    }

    if(gnVocoderMode != VR_NONE || SKY_IS_ETIQUETTE())
    {
        if(!SKY_IS_HANDS_FREE())
            SKY_MotorStart(1500);

        if(!SKY_IS_EAR_JACK_ON())
            return;
    }

    if(SKY_GET_BELL_VIBRATION() != BV_BELL && !SKY_IS_HANDS_FREE()) 
        SKY_MotorStart(1500);

	if(SKY_IS_MY_BELL(SKY_GET_SCHL_ALERT_BELL_ID()))   // Play file
	{
		sPlayMusic(UI_MUSIC_FILE_SND, (BYTE *)SKY_GET_SCHL_ALERT_BELL_FILE_NAME(), NULL, 0, 
				   0, SKY_GET_ALARM_BELL_VOLUME(), FALSE);
	}
	else
	{
		sPlayMusic(UI_MUSIC_ID_SND, NULL, NULL, SKY_GET_SCHL_ALERT_BELL_ID(), 
		   0, SKY_GET_ALARM_BELL_VOLUME(), FALSE);
	}
}