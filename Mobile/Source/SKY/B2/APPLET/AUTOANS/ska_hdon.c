/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_hdon.c
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
** 2001-01-11   hjcho      Created.
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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/



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
#define HOLDON_IMAGE_XPOS       0
#define HOLDON_IMAGE_YPOS       0

void sInitHoldOnDisplay()
{
  HCONTROL hControl;

//  hControl = EQC_Animation(HOLDON_IMAGE_XPOS,            
//                           HOLDON_IMAGE_YPOS, 
//						   &g_AniHoldOnCall,
//                           //&g_AniHoldon120x106x256c,
//                           NULL);           
//  EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
//  EQC_StartAnimation(hControl, TRUE);  

	hControl = EQC_Image(HOLDON_IMAGE_XPOS,
		                 HOLDON_IMAGE_YPOS,
			             &g_BmpHoldOnCall);


#ifdef FEATURE_SKY_SUBLCD
    SKY_SubLCDEndText(LN_UPPER);
    SKY_SubLCDEndText(LN_LOWER);
    SKY_DrawSubAnnunciators();
    SKY_SubLCDTextOutStr(LN_LOWER, SKY_GET_TOKEN(TKN_SLCD_IN_HOLDON)); 
#endif //FEATURE_SKY_SUBLCD

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
extern BOOL IsEarpieceEnable(void);
void SKY_API SKA_MainHoldOn(SKY_EVENT_T* pEvent)
{    
   STATIC BOOL		      fPlayMent;
   STATIC UINT8           nCurrentVol;
	
	
	switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      EQS_GetDC(TRUE);
      EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE, 
                    NULL, 
                    WA_SHOW_HEADER);
      sInitHoldOnDisplay();
      SKY_SET_MUTE(TRUE);
      SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_MUTED, TRUE);
	  gnVocoderMode = VR_AUTO;
	  fPlayMent = FALSE;
	  nCurrentVol = SKY_GET_SPEAKER_VOLUME(); 
      SKY_SET_TRANSMIT_DTMF(FALSE);
	  EQS_StartTimer(EQS_TIMER_2, 300, FALSE);
	  SKY_AcceptKey(FALSE);
        break;

    case SE_APP_END :     /* The very last event */
	   SKY_AcceptKey(TRUE);
	   SKY_SetVoiceVolume(nCurrentVol);
	   SKY_SET_TRANSMIT_DTMF(TRUE);
	   SKY_SET_MUTE(FALSE);
       SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
       gnVocoderMode = VR_NONE;

#ifdef FEATURE_SKY_SUBLCD
	   SKY_SubLCDEndText(LN_UPPER);
	   SKY_SubLCDEndText(LN_LOWER);
#endif// FEATURE_SKY_SUBLCD
	   EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
       SKY_SetVoiceVolume(nCurrentVol);
       SKY_AcceptKey(TRUE);
       SKY_SET_TRANSMIT_DTMF(TRUE);
	   
	   SKY_SET_MUTE(FALSE);
       SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
       gnVocoderMode = VR_NONE;
        break;    

    case SE_APP_RESUME :  /* end child */
       SKY_AcceptKey(TRUE);
	   SKY_SetVoiceVolume(nCurrentVol);
       SKY_SET_TRANSMIT_DTMF(TRUE);
        break;
    
    case SE_APP_DRAW :    /* draw */
        break;

    case SE_TM_TIMER1:
       EQS_SEND_EVENT(SE_VM_PLAY_CONTINUE, EQS_NA, EQS_NA);
        break;

    case SE_TM_TIMER2:
       SKY_AcceptKey(TRUE);
	   EQS_SEND_EVENT(SE_VM_PLAY_CONTINUE, EQS_NA, EQS_NA);
		break;

    case SE_VM_PLAY_CONTINUE:  /* This event comes from SKA_InProg !! */
       SKY_SetVoiceVolume(1);
       PLAY_HO_voice_start_traffic();
	   fPlayMent = TRUE;
        break;

    case SE_AANS_PLAY_END:
    case SE_VM_PLAY_END:       /* This event is occured when a ment is ended */
       PLAY_HO_voice_stop_traffic();
	   fPlayMent =FALSE;
       EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
       break;        
  
    case SE_KEY_PHONE_OPEN:
    case SE_KEY_EAR_CLICK:
    case SE_KEY_EAR_DOUBLE_CLICK:
    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_AUTO_ANSWER:
    case SE_KEY_RECORD:
    case SE_KEY_RECORD_LONG:
    case SE_KEY_PLAY:
    case SE_KEY_PLAY_LONG:
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
    case SE_KEY_STAR :
    case SE_KEY_POUND :
    case SE_KEY_UP :
    case SE_KEY_DOWN :
    case SE_KEY_LEFT :   /* PF3 : */
    case SE_KEY_RIGHT :  /* PF2 : */
    case SE_KEY_CLEAR :
    case SE_KEY_CLEAR_LONG : /* UI_CLRALL_F: */
    case SE_KEY_END :
	case SE_KEY_F1:
	case SE_KEY_F2:
    case SE_KEY_SEND:
    case SE_KEY_CENTER:
      if(fPlayMent == TRUE)
	  {
	    PLAY_HO_voice_stop_traffic();
	    fPlayMent = FALSE;
	  }
      SKY_SET_MUTE(FALSE);
      SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
      gnVocoderMode = VR_NONE;
      EQS_END();
        break;

    case SE_SYS_TO_IDLE: /* ignore this event */
        break;

    case SE_KEY_PHONE_CLOSE:
      if(SKY_IS_EAR_JACK_OFF())
      {
        if(fPlayMent == TRUE)
	    {
	        PLAY_HO_voice_stop_traffic();
	        fPlayMent = FALSE;
	    }
        SKY_SET_MUTE(FALSE);
        SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
        gnVocoderMode = VR_NONE;
        SKY_EndCall();
      }
	    break;
    
	case SE_SYS_OFFLINE:
    case SE_CALL_ENDED:
		SKY_AcceptKey(FALSE);
      if(fPlayMent == TRUE)
	  {
	    PLAY_HO_voice_stop_traffic();
	    fPlayMent = FALSE;
	  }
      SKY_SET_MUTE(FALSE);
      SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
      gnVocoderMode = VR_NONE;
      SKY_SET_TRANSMIT_DTMF(TRUE);
      EQS_END_CURRENT_EVENT();
        break;

    case SE_CALL_COMMAND_ERROR:
		break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
