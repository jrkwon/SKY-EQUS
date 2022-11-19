/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00.07.28   jwpark     Vocoder Rx Gain	      (-15dB ~ 10dB)
**						 CODEC Rx/Tx Gain         (-12dB ~ -9dB / -6dB ~ 8dB)
**                       Melody IC FM/HP/SPK Gain (-30dB ~  0dB + MUTE)
** 00-07-21   woodstok   Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#include <stdio.h>
#include "eqa.h"
#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "snd.h"
#include "vocmux.h"
#include "sndi.h"
#include "AudioTest.h"
#include "MusicDB.h"
#include "uixsnd.h"
#include "sndihw.h"

#include "uiuint.h"
#include "uistate.h"
#include "uixscrn.h"
#include "uixcm.h"
#include "uiutxt.h"
#include "uixsnd.h"
#include "uiumenu.h"
#include "uiudata.h"
#include "uiscall.h"
#include "uixuasms.h"

//redstar
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
#include "eqs_vari.h"    
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


/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
CONST CHAR* CONST VolMenu[4] = {
	SF_BLACK_NUM1"통화음 크기",
	SF_BLACK_NUM2"키톤 크기",
	SF_BLACK_NUM3"뮤직벨 크기",
	SF_BLACK_NUM4"자동재다이얼"
};



CONST CHAR* CONST OnOffMenu[2] = {
	SF_BLACK_NUM1"ON",
	SF_BLACK_NUM2"OFF"
};


/*===========================================================================
              
				   This section is inserted for Volume Test

                              by hjcho 00.08.06


===========================================================================*/ 

#define  TKN_VOICE_TEST        "통화음크기조절"
#define  TKN_VOICE_TEST_END    "통화음크기조절 끝"
#define  TKN_KEYTONE_TEST      "키톤크기조절"
#define  TKN_KEYTONE_TEST_END  "키톤크기조절 끝"
#define  TKN_MUSIC_TEST        "벨크기조절"
#define  TKN_MUSIC_TEST_END    "벨크기조절 끝"
#define  TKN_VERSION_TEST      "Ver. 0.05"
#define  TKN_VERSION_TITLE     "S/W Version"

VOID s_SetVol(BYTE szMode, UINT8 szIndex)
{
  static nv_item_type nvi;
  UINT8 VOL;
  BYTE bHeadsetVoiceLvl, bVoiceLvl, bHFKVoiceLvl, bMusicLvl;
	
  //VOL = s_GetVol_Lvl(szIndex);
  VOL =szIndex;

  if(szMode == BEEP_VOL)
   {
	 nvi.beep_level = VOL;
	 nvi.beep_spkr_lvl = VOL;
	 nvi.beep_lvl_shadow = VOL;

	 uisnd_beep_vol(VOL, NV_BEEP_LVL_I);
     uisnd_beep_vol(VOL, NV_BEEP_SPKR_LVL_I);
     uisnd_beep_vol(VOL, NV_BEEP_LVL_SHADOW_I);

	 uisnd_sample_tone(UI_BEEP_SND, SND_2ND, 300, VOL);
   }
   else if(szMode == VOICE_VOL)
   {     
	   
//	 nvi.speaker_level = ui.ear_spkr_lvl =VOL;
     nvi.speaker_level = bHFKVoiceLvl = VOL;
     EQS_PutUi( ES_SI_EAR_SPKR_I, (void*)&bHFKVoiceLvl, FALSE );
//	 nvi.ear_level    = ui.ear_lvl = VOL;
     nvi.ear_level    = bVoiceLvl = VOL;
     EQS_PutUi( ES_SI_EAR_LVL_I, (void*)&bVoiceLvl, FALSE ); 
//	 nvi.ear_lvl_shadow = ui.ear_headset_lvl = VOL;
     nvi.ear_lvl_shadow = bHeadsetVoiceLvl = VOL;
     EQS_PutUi( ES_SI_EAR_HEADSET_I, (void*)&bHeadsetVoiceLvl, FALSE ); 

	 uisnd_voice_vol(VOL, NV_SPEAKER_LVL_I );
     uisnd_voice_vol(VOL, NV_EAR_LVL_I );
     uisnd_voice_vol(VOL, NV_EAR_LVL_SHADOW_I );

	 uisnd_sample_tone(UI_BEEP_SND, SND_2ND, 300, VOL);
   }
   else if(szMode == MUSIC_VOL)
   {     
//	 nvi.music_lvl.vol_lvl = ui.music_lvl = VOL;
	 nvi.music_lvl.vol_lvl = bMusicLvl = VOL;
     EQS_PutUi( ES_SI_MUSIC_LVL_I, (void*)&bMusicLvl, FALSE );
#ifdef FEATURE_YMU757B_MELODY	 
//	 uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_TIMEGOES, ui.music_lvl);
     uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_TIMEGOES, bMusicLvl);
#elif defined FEATURE_YMU759_MELODY         //model B1

#endif
   }
}	 


VOID s_UpdateVol_NV(BYTE szMode, UINT8 szIndex)
{
    UINT8 VOL;
    BYTE  bHeadsetBeepLvl, bBeepLvl, bHFKBeepLvl, bHeadsetVoiceLvl, bVoiceLvl, bHFKVoiceLvl, bMusicLvl;
	static nv_item_type nvi;

	//VOL = s_GetVol_Lvl(szIndex);
	VOL = szIndex;

   if(szMode == BEEP_VOL)
   {
//     nvi.beep_level = ui.beep_lvl = ui.lval = VOL;
     //nvi.beep_level = gSndSet.BeepLvl = VOL;
	 //ui_put_nv(NV_BEEP_LVL_I, &nvi);
     bBeepLvl = VOL;
     EQS_PutUi( ES_SI_BEEP_LVL_I, (void*)&bBeepLvl, TRUE );
	 
//	 nvi.beep_spkr_lvl = ui.beep_spkr_lvl = ui.lval = VOL;
     //nvi.beep_spkr_lvl = gSndSet.HFKBeepLvl = VOL;
	 //ui_put_nv(NV_BEEP_SPKR_LVL_I, &nvi);
     bHFKBeepLvl = VOL;
     EQS_PutUi( ES_SI_BEEP_SPKR_I, (void*)&bHFKBeepLvl, TRUE );

//	 nvi.beep_lvl_shadow = ui.lval = ui.beep_headset_lvl = VOL;
     //nvi.beep_lvl_shadow = gSndSet.HeadsetBeepLvl = VOL;
	 //ui_put_nv(NV_BEEP_LVL_SHADOW_I, &nvi);
     bHeadsetBeepLvl = VOL;
     EQS_PutUi( ES_SI_BEEP_HEADSET_I, (void*)&bHeadsetBeepLvl, TRUE );
   }
   else if(szMode == VOICE_VOL)
   {
//	 nvi.speaker_level = ui.ear_spkr_lvl =VOL;
     //nvi.speaker_level = gSndSet.HFKVoiceLvl =VOL;
	 //ui_put_nv(NV_SPEAKER_LVL_I, &nvi);
     bHFKVoiceLvl = VOL;
     EQS_PutUi( ES_SI_EAR_SPKR_I, (void*)&bHFKVoiceLvl, TRUE );

//	 nvi.ear_level = ui.ear_lvl = VOL;
     //nvi.ear_level = gSndSet.VoiceLvl = VOL;
	 //ui_put_nv(NV_EAR_LVL_I, &nvi);
     bVoiceLvl = VOL;
     EQS_PutUi( ES_SI_EAR_LVL_I, (void*)&bVoiceLvl, TRUE ); 

//	 nvi.ear_lvl_shadow = ui.ear_headset_lvl = VOL;
     //nvi.ear_lvl_shadow = gSndSet.HeadsetVoiceLvl = VOL;
	 //ui_put_nv(NV_EAR_LVL_SHADOW_I, &nvi);
     bHeadsetVoiceLvl = VOL;
     EQS_PutUi( ES_SI_EAR_HEADSET_I, (void*)&bHeadsetVoiceLvl, TRUE ); 
   }
   else if(szMode == MUSIC_VOL)
   {
//	  nvi.music_lvl.vol_lvl = ui.music_lvl = szIndex;
      //nvi.music_lvl.vol_lvl = gSndSet.MusicLvl = szIndex;
	  //ui_put_nv(NV_MUSIC_LVL_I, &nvi);     
      bMusicLvl = szIndex;
      EQS_PutUi( ES_SI_MUSIC_LVL_I, (void*)&bMusicLvl, TRUE );
   }
}	 


VOID EQA_VoiceVol(EQS_EVENT *pInEvent)
{
	UINT8 bCurrentLvl;
	CHAR  VolBuff[20];
	CHAR  MessgBuff[20];
    BYTE  bVoiceLvl;
	
	//bCurrentLvl = s_DecideIndex(ui.ear_lvl);
    EQS_GetUi( ES_SI_EAR_LVL_I, (void*)&bVoiceLvl, FALSE ); 
	bCurrentLvl = bVoiceLvl;
	
	switch(pInEvent->EventID)
    {
    case EQE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
	   EQS_TextOutStr( 0, 0, (BYTE *)TKN_VOICE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_APP_END : /* The very last event */
	   EQS_ReleaseDC();
       break;

    case EQE_APP_DRAW :
	   EQS_TextOutStr( 0,  0, (BYTE *)TKN_VOICE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_UP :
       if(bCurrentLvl < 5) bCurrentLvl++;
	   else bCurrentLvl = 5;
	   s_SetVol(VOICE_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_DOWN :
	   if(bCurrentLvl > 0) bCurrentLvl--;
	   else bCurrentLvl = 0;
       s_SetVol(VOICE_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();
       break;

	case EQE_KEY_CENTER :
	   s_UpdateVol_NV(VOICE_VOL, bCurrentLvl);
	   sprintf((char *)MessgBuff,TKN_VOICE_TEST_END);
	   EQS_TextOutStr(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
	   break;
    
	case EQE_KEY_RIGHT :
	case EQE_KEY_CLEAR :
	case EQE_KEY_END :
		EQS_End();    	
	
	case EQE_TM_GENERAL : /* UI_TIMER_F: */
       EQS_TextOutStr(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_End();
      break;
    
    default:
      EQS_DefEventProc(pInEvent);
      break;
	}
}


VOID EQA_KeyToneVol(EQS_EVENT *pInEvent)
{
    UINT8 bCurrentLvl;
	CHAR  VolBuff[5];
	CHAR  MessgBuff[20];
    BYTE  bBeepLvl ;

	//bCurrentLvl = ui.beep_lvl;
    EQS_GetUi( ES_SI_BEEP_LVL_I, (void*)&bBeepLvl, FALSE );
    bCurrentLvl = bBeepLvl;
    
	switch(pInEvent->EventID)
    {
    case EQE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
	   EQS_TextOutStr( 0, 0, (BYTE *)TKN_KEYTONE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_APP_END : /* The very last event */
	   EQS_ReleaseDC();
       break;

    case EQE_APP_DRAW :
	   EQS_TextOutStr( 0,  0, (BYTE *)TKN_KEYTONE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_UP :
	   if(bCurrentLvl < 5) bCurrentLvl++;
	   else bCurrentLvl = 5;
	   s_SetVol(BEEP_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_DOWN :
	   if(bCurrentLvl > 0) bCurrentLvl--;
	   else bCurrentLvl = 0;
       s_SetVol(BEEP_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();
       break;

	case EQE_KEY_CENTER :
       s_UpdateVol_NV(BEEP_VOL, bCurrentLvl);
	   sprintf((char *)MessgBuff,TKN_KEYTONE_TEST_END);
	   EQS_TextOutStr(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
	   break;
    
	case EQE_KEY_RIGHT :
	case EQE_KEY_CLEAR :
	case EQE_KEY_END :
		EQS_End();    	
	
	case EQE_TM_GENERAL : /* UI_TIMER_F: */
       EQS_TextOutStr(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_End();
      break;
    
    default:
      EQS_DefEventProc(pInEvent);
      break;
	}
}

VOID EQA_MusicVol(EQS_EVENT *pInEvent)
{
    UINT8 bCurrentLvl;
	CHAR  VolBuff[5];
	CHAR  MessgBuff[20];
    BYTE  bMusicLvl;

//	bCurrentLvl = ui.music_lvl;
    EQS_GetUi( ES_SI_MUSIC_LVL_I, (void*)&bMusicLvl, FALSE );
    bCurrentLvl = bMusicLvl;
	
	switch(pInEvent->EventID)
    {
    case EQE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
	   EQS_TextOutStr( 0, 0, (BYTE *)TKN_MUSIC_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   

#ifdef FEATURE_YMU757B_MELODY	 
//	 uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_TIMEGOES, ui.music_lvl);
	 uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_TIMEGOES, bMusicLvl);
#elif defined FEATURE_YMU759_MELODY         //model B1

#endif
	   break;

    case EQE_APP_END : /* The very last event */
#ifdef FEATURE_YMU757B_MELODY           
    	uisnd_music_stop();
#elif defined FEATURE_YMU759_MELODY		// model b1

#endif
	   EQS_ReleaseDC();
       break;

    case EQE_APP_DRAW :
	   EQS_TextOutStr( 0,  0, (BYTE *)TKN_MUSIC_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_UP :
	   if(bCurrentLvl < 5) bCurrentLvl++;
	   else bCurrentLvl = 5;
	   s_SetVol(MUSIC_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_DOWN :
	   if(bCurrentLvl > 0) bCurrentLvl--;
	   else bCurrentLvl = 0;
       s_SetVol(MUSIC_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOutStr( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();
       break;

	case EQE_KEY_CENTER :
       s_UpdateVol_NV(MUSIC_VOL, bCurrentLvl);
	   sprintf((char *)MessgBuff,TKN_MUSIC_TEST_END);
	   EQS_TextOutStr(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
	   break;
    
	case EQE_KEY_RIGHT :
	case EQE_KEY_CLEAR :
	case EQE_KEY_END :
		EQS_End();    	
	
	case EQE_TM_GENERAL : /* UI_TIMER_F: */
       EQS_TextOutStr(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_End();
      break;
    
    default:
      EQS_DefEventProc(pInEvent);
      break;
	}
}
/*===========================================================================
              
			                End by hjcho 00.08.06

===========================================================================*/ 






/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

VOID EQA_AutoRedialTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	static nv_item_type nvi;
	UINT8           iKey = 0;
    BOOL fAutoRetry;
    BYTE bRetryTime;

	switch(pInEvent->EventID)
	{
    case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList(OnOffMenu, 2, 2, 0, 0, 16, 2, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;
    
	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

    case EQE_KEY_1:
	case EQE_KEY_2:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

    case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd))
		{
		case 0: // On 
//         nvi.auto_redial.enable = ui.isretry = TRUE;
//		   nvi.auto_redial.rings = ui.retry = 0x00;  // 0초후에 재다이얼 수정
           nvi.auto_redial.enable = fAutoRetry = TRUE;
           nvi.auto_redial.rings =  bRetryTime = 0x00;  // 0초후에 재다이얼 수정

           EQS_PutUi( ES_CM_AUTO_RETRY_I, (void*)&fAutoRetry, TRUE);
           EQS_PutUi( ES_CM_RETRY_I, (void*)&bRetryTime, TRUE);
		   //(void) ui_put_nv( NV_AUTO_REDIAL_I, &nvi );
		   EQS_TextOutStr(0, 30, (BYTE *)"자동재다이얼 ON");
		   EQS_UpdateDisplay();
		   EQS_StartTimer(EQS_TIMER_1, 1000, FALSE);
           break;

        case 1: // Off
//		   nvi.auto_redial.enable = ui.isretry = FALSE;
//         nvi.auto_redial.rings = ui.retry = 0;
		   nvi.auto_redial.enable = fAutoRetry = FALSE;
           nvi.auto_redial.rings = bRetryTime = 0;
           EQS_PutUi( ES_CM_AUTO_RETRY_I, (void*)&fAutoRetry, TRUE);
           EQS_PutUi( ES_CM_RETRY_I, (void*)&bRetryTime, TRUE);
           //(void) ui_put_nv( NV_AUTO_REDIAL_I, &nvi );
		   EQS_TextOutStr(0, 30, (BYTE *)"자동재다이얼 OFF");
		   EQS_UpdateDisplay();
		   EQS_StartTimer(EQS_TIMER_1, 1000, FALSE);
		   break;
       
		default: break;
		}
		break;
    
	case EQE_TM_GENERAL ://UI_TIMER_F:
        EQS_End();
		break;
	
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}


    





VOID EQA_VolumeUpDTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	UINT8			iKey = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList(VolMenu, 4, 4, 0, 0, 16, 4, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
	case EQE_KEY_3:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd))
		{
		case 0: EQS_StartChild(EQA_VoiceVol);          break;
        case 1: EQS_StartChild(EQA_KeyToneVol);        break;
        case 2: EQS_StartChild(EQA_MusicVol);          break;
		case 3: EQS_StartChild(EQA_AutoRedialTop);     break;
		default: break;
		}
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

VOID EQA_VersionTop(EQS_EVENT *pInEvent)
{	
    switch (pInEvent->EventID)
	{ 
	case EQE_APP_START:
       EQS_GetDC(TRUE);
	   SetTextDrawMode(TDM_REVERSE);
	   EQS_TextOutStr( 0, 0,   (BYTE *)TKN_VERSION_TITLE);
	   ClearTextDrawMode(TDM_REVERSE);
	   EQS_TextOutStr( 20, 60, (BYTE *)TKN_VERSION_TEST);
	   EQS_UpdateDisplay();
	   EQS_StartTimer(EQS_TIMER_1, 3000, FALSE);
	    break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;
	
	case EQE_TM_GENERAL : /* UI_TIMER_F: */
       EQS_TextOutStr( 20, 60, (BYTE *)TKN_VERSION_TEST);
	   EQS_UpdateDisplay();
	   EQS_End();
	    break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
