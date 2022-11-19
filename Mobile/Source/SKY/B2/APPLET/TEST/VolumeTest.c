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
#include "eqs.h"
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
**  Global variables
**---------------------------------------------------------------------------
*/
AudioTestGainType	AudioTestGain;
boolean				IsAudioGainTest = FALSE;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
CHAR *VolumeMenu[8] = {
	SF_BLACK_NUM1"Current Gain",
	SF_BLACK_NUM2"Vocoder Rx",
	SF_BLACK_NUM3"DTMF",
	SF_BLACK_NUM4"CODEC Rx",
	SF_BLACK_NUM5"CODEC Tx",
	SF_BLACK_NUM6"Melody FM",
	SF_BLACK_NUM7"Melody HP",
	SF_BLACK_NUM8"Melody SPK",
};



CHAR AudioGainText0[13];
CHAR AudioGainText1[13];
CHAR AudioGainText2[13];
CHAR AudioGainText3[13];
CHAR AudioGainText4[13];
CHAR AudioGainText5[13];
CHAR AudioGainText6[13];
CHAR *AudioGainText[7] = {
	AudioGainText0,
	AudioGainText1,
	AudioGainText2,
	AudioGainText3,
	AudioGainText4,
	AudioGainText5,
	AudioGainText6
};

CHAR *VocoderRxText[26] = { // 26개 
	"-15dB",
	"-14dB",
	"-13dB",
	"-12dB",
	"-11dB",
	"-10dB",
	" -9dB",
	" -8dB",
	" -7dB",
	" -6dB",
	" -5dB",
	" -4dB",
	" -3dB",
	" -2dB",
	" -1dB",
	"  0dB",
	"  1dB",
	"  2dB",
	"  3dB",
	"  4dB",
	"  5dB",
	"  6dB",
	"  7dB",
	"  8dB",
	"  9dB",
	" 10dB"
};


CHAR *CODECRxText[8] = { // 8개
	SF_BLACK_NUM1"-12dB",
	SF_BLACK_NUM2" -9dB",
	SF_BLACK_NUM3" -6dB",
	SF_BLACK_NUM4" -3dB",
	SF_BLACK_NUM5"  0dB",
	SF_BLACK_NUM6"  3dB",
	SF_BLACK_NUM7"  6dB",
	SF_BLACK_NUM8"  9dB"
};

CHAR *CODECTxText[8] = { // 8개
	SF_BLACK_NUM1"-6dB",
	SF_BLACK_NUM2"-4dB",
	SF_BLACK_NUM3"-2dB",
	SF_BLACK_NUM4" 0dB",
	SF_BLACK_NUM5" 2dB",
	SF_BLACK_NUM6" 4dB",
	SF_BLACK_NUM7" 6dB",
	SF_BLACK_NUM8" 8dB"
};

CHAR *MelodyICText[32] = { // 32개 
	"MUTE ",
	"-30dB",
	"-29dB",
	"-28dB",
	"-27dB",
	"-26dB",
	"-25dB",
	"-24dB",
	"-23dB",
	"-22dB",
	"-21dB",
	"-20dB", 
	"-19dB", 
	"-18dB", 
	"-17dB", 
	"-16dB",
	"-15dB",
	"-14dB",
	"-13dB",
	"-12dB",
	"-11dB",
	"-10dB",
	" -9dB",
	" -8dB",
	" -7dB",
	" -6dB",
	" -5dB",
	" -4dB",
	" -3dB",
	" -2dB",
	" -1dB",
	"  0dB"
};

CHAR *VolLvlText[4] = { // 4개
	SF_BLACK_NUM1" 0",
	SF_BLACK_NUM2" 1",
	SF_BLACK_NUM3" 2",
	SF_BLACK_NUM4" 3",
};


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
/////////////////////////////////////////////////////////////////////////////
VOID EQA_CurrentGain(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		sprintf(AudioGainText[0], "Vo.Rx: %4x", AudioTestGain.VocoderRx);
		sprintf(AudioGainText[1], "DTMF : %4x", AudioTestGain.DTMF);
		sprintf(AudioGainText[2], "CO.Rx:%3ddB",
			(AudioTestGain.CODECRx        - 4) * 3);
		sprintf(AudioGainText[3], "CO.TX:%3ddB",
			((AudioTestGain.CODECTx >> 4) - 3) * 2);
		if(AudioTestGain.FM == 0)     sprintf(AudioGainText[4], "FM   : MUTE");
		else   sprintf(AudioGainText[4], "FM   :%3ddB", AudioTestGain.FM - 31);
		if(AudioTestGain.HP == 0)     sprintf(AudioGainText[5], "HP   : MUTE");
		else   sprintf(AudioGainText[5], "HP   :%3ddB", AudioTestGain.HP - 31);
		if(AudioTestGain.SPK == 0)    sprintf(AudioGainText[6], "SPK  : MUTE");
		else   sprintf(AudioGainText[6], "SPK  :%3ddB",AudioTestGain.SPK - 31);

		hHnd = EQC_ScrollList((CONST CHAR** CONST) AudioGainText, 7, 7, 0, 0, 13, 5, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////
VOID EQA_VocoderRxGain(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	UCHAR			szTmpString[20];
	BYTE			bCurrentLevel;
	static uint16 voc_dB_to_hex[26] = {	// -15dB ~ 10dB
		0x002d, 0x0033, 0x0039, 0x0040, 0x0048,
		0x0050, 0x005a, 0x0065, 0x0072, 0x0080,
		0x008f, 0x00a1, 0x00b5, 0x00cb, 0x00e4,
		0x0100, 0x011f, 0x0142, 0x0169, 0x0195,
		0x01c7, 0x01f3, 0x023d, 0x0283, 0x02d1,
		0x0329
	};

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList((CHAR** CONST)VocoderRxText, 26, 26, 0, 0, 13, 5, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_SOFTKEY_SELECT:
		bCurrentLevel = EQC_GetScrollIndex(hHnd);
		AudioTestGain.VocoderRx = voc_dB_to_hex[bCurrentLevel];
		sndhw_set_volume();

		sprintf((CHAR*)szTmpString, "%ddB", bCurrentLevel - 15);
		EQC_MesgBox((CHAR*)szTmpString, 3,FT_MED, MB_BEEP);
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////
VOID EQA_CODECRxGain(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	UCHAR			szTmpString[20];
	BYTE			bCurrentLevel;


	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList((CHAR** CONST)CODECRxText, 8, 8, 0, 0, 13, 5, AudioTestGain.CODECRx, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_SOFTKEY_SELECT:
		AudioTestGain.CODECRx = bCurrentLevel = EQC_GetScrollIndex(hHnd);
		sndhw_set_volume();

		sprintf((CHAR*)szTmpString, "%ddB", (bCurrentLevel - 4) * 3);
		EQC_MesgBox((CHAR*)szTmpString, 3,FT_MED, MB_BEEP);
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////
VOID EQA_CODECTxGain(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	UCHAR			szTmpString[20];
	BYTE			bCurrentLevel;


	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList((CHAR** CONST)CODECTxText, 8, 8, 0, 0, 13, 5, AudioTestGain.CODECTx, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_SOFTKEY_SELECT:
		bCurrentLevel = EQC_GetScrollIndex(hHnd);
		AudioTestGain.CODECTx = (bCurrentLevel << 4);
		sndhw_set_volume();

		sprintf((CHAR*)szTmpString, "%3ddB", (bCurrentLevel - 3) * 2);
		EQC_MesgBox((CHAR*)szTmpString, 3, FT_MED,MB_BEEP);
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////
VOID EQA_MelodyICFMGain(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	UCHAR			szTmpString[20];
	BYTE			bCurrentLevel;


	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList((CHAR** CONST)MelodyICText, 32, 32, 0, 0, 13, 5, AudioTestGain.FM, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
#ifdef FEATURE_YMU757B_MELODY // girin. 2000.8.3
		uisnd_music_stop();
#endif
        EQS_ReleaseDC();
        break;

	case EQE_SOFTKEY_SELECT:
		AudioTestGain.FM = bCurrentLevel = EQC_GetScrollIndex(hHnd);
		sndhw_set_volume();
#ifdef FEATURE_YMU757B_MELODY // girin. 2000.8.3
		uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_ONCEMORE, 5);
#endif

		if(bCurrentLevel == 0)
			sprintf((CHAR*)szTmpString, "MUTE");
		else
			sprintf((CHAR*)szTmpString, "%ddB", bCurrentLevel - 31);
		EQC_MesgBox((CHAR*)szTmpString, 3, FT_MED,MB_BEEP);
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////
VOID EQA_MelodyICHPGain(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	UCHAR			szTmpString[20];
	BYTE			bCurrentLevel;


	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList((CHAR** CONST)MelodyICText, 32, 32, 0, 0, 13, 5, AudioTestGain.HP, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
#ifdef FEATURE_YMU757B_MELODY // girin. 2000.8.3
		uisnd_music_stop();
#endif
        EQS_ReleaseDC();
        break;

	case EQE_SOFTKEY_SELECT:
		AudioTestGain.HP = bCurrentLevel = EQC_GetScrollIndex(hHnd);
		sndhw_set_volume();
#ifdef FEATURE_YMU757B_MELODY // girin. 2000.8.3
		uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_ONCEMORE, 5);
#endif

		if(bCurrentLevel == 0)
			sprintf((CHAR*)szTmpString, "MUTE");
		else
			sprintf((CHAR*)szTmpString, "%ddB", bCurrentLevel - 31);
		EQC_MesgBox((CHAR*)szTmpString, 3, FT_MED,MB_BEEP);
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////
VOID EQA_MelodyICSPKGain(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	UCHAR			szTmpString[20];

	BYTE			bCurrentLevel;


	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList((CHAR** CONST)MelodyICText, 32, 32,0, 0, 13, 5, AudioTestGain.SPK, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
#ifdef FEATURE_YMU757B_MELODY // girin. 2000.8.3
		uisnd_music_stop();
#endif
        EQS_ReleaseDC();
        break;

	case EQE_SOFTKEY_SELECT:
		AudioTestGain.SPK = bCurrentLevel = EQC_GetScrollIndex(hHnd);
//		sndhw_set_volume();
#ifdef FEATURE_YMU757B_MELODY 
		uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_ONCEMORE, 5);
#endif

		if(bCurrentLevel == 0)
			sprintf((CHAR*)szTmpString, "MUTE");
		else
			sprintf((CHAR*)szTmpString, "%ddB", bCurrentLevel - 31);
		EQC_MesgBox((CHAR*)szTmpString, 3, FT_MED,MB_BEEP);
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

#ifdef OMIT // This part is moved to VolumeUpDTest.c
/*===========================================================================
              
				   This section is inserted for Volume Test

                              by hjcho 00.08.06


===========================================================================*/ 
#define  BEEP_VOL  0
#define  VOICE_VOL 1
#define  MUSIC_VOL 2

#define  UISND_NONE_VOL 0x00

#define  TKN_VOICE_TEST   "통화음크기조절"
#define  TKN_KEYTONE_TEST "키톤크기조절"
#define  TKN_MUSIC_TEST   "벨크기조절"
/*
UINT8 s_GetVol_Lvl(UINT8 szIndex)
{
	UINT8 RealVol;
	switch (szIndex)
    {
	 case 0 :
		RealVol = UISND_NONE_VOL;
		break;
	 case 1 :
		RealVol = UISND_1ST_VOL;
		break;
	 case 2 :
		RealVol = UISND_2ND_VOL;
		break;
	 case 3 :
		RealVol = UISND_3RD_VOL;
		break;
	 case 4 :
		RealVol = UISND_4TH_VOL;
		break;
     default :
		RealVol = UISND_3RD_VOL;
	}
	return RealVol;
}
*/
/*
UINT8 s_DecideIndex(BYTE VolLvl)
{
	UINT8 Index;
	
	switch (VolLvl)
	{
    case UISND_NONE_VOL:
		Index = (UINT8) 0;
		break;
	case UISND_1ST_VOL:
	    Index = (UINT8) 1;
	    break;
    case UISND_2ND_VOL:
		Index = (UINT8) 2;
		break;
	case UISND_3RD_VOL:
		Index = (UINT8) 3;
		break;
	case UISND_4TH_VOL:
		Index = (UINT8) 4;
		break;
	default :
		Index = (UINT8) 3;
    }

	return Index;
}
*/

VOID s_SetVol(BYTE szMode, UINT8 szIndex)
{
  static nv_item_type nvi;
  UINT8 VOL;
  BYTE   bHeadsetVoiceLvl, bVoiceLvl, bHFKVoiceLvl, bMusicLvl;
	
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
	 
     EQS_GetUi( ES_SI_EAR_HEADSET_I, (void*)&bHeadsetVoiceLvl, FALSE );       
     EQS_GetUi( ES_SI_EAR_LVL_I, (void*)&bVoiceLvl, FALSE ); 
     EQS_GetUi( ES_SI_EAR_SPKR_I, (void*)&bHFKVoiceLvl, FALSE );

//	 nvi.speaker_level = ui.ear_spkr_lvl =VOL;
     nvi.speaker_level = bHFKVoiceLvl = VOL;
//	 nvi.ear_level    = ui.ear_lvl = VOL;
     nvi.ear_level = bVoiceLvl  = VOL;
//	 nvi.ear_lvl_shadow = ui.ear_headset_lvl = VOL;
     nvi.ear_lvl_shadow = bHeadsetVoiceLvl = VOL;
     


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
	 uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_TIMEGOES, bMusicLvl);
#elif defined FEATURE_YMU759_MELODY         //model B1

#endif
   }
}	 


VOID s_UpdateVol_NV(BYTE szMode, UINT8 szIndex)
{
    UINT8 VOL;
    BYTE  bHeadsetBeepLvl, bBeepLvl, bHFKBeepLvl, bHeadsetVoiceLvl, bVoiceLvl;
	static nv_item_type nvi;

	//VOL = s_GetVol_Lvl(szIndex);
	VOL = szIndex;

   if(szMode == BEEP_VOL)
   {
//   nvi.beep_level = ui.beep_lvl = ui.lval = VOL;
     //nvi.beep_level = gSndSet.BeepLvl = VOL;
	 //ui_put_nv(NV_BEEP_LVL_I, &nvi);
     bBeepLvl = VOL;
     EQS_PutUi( ES_SI_BEEP_LVL_I, (void*)&bBeepLvl, TRUE );
	 
//	 nvi.beep_spkr_lvl = ui.beep_spkr_lvl = ui.lval = VOL;
     //nvi.beep_spkr_lvl = gSndSet.HFKBeepLvl = VOL;
	 //ui_put_nv(NV_BEEP_SPKR_LVL_I, &nvi);
     bHFKBeepLvl = VOL;
     EQS_PutUi( ES_SI_BEEP_SPKR_I, (void*)&bHFKBeepLvl, TRUE );

//	 nvi.beep_lvl_shadow = ui.beep_headset_lvl = VOL;
	 //nvi.beep_lvl_shadow = gSndSet.HeadSetBeepLvl = VOL;
	 //ui_put_nv(NV_BEEP_LVL_SHADOW_I, &nvi);
     bHeadsetBeepLvl = VOL;
     EQS_PutUi( ES_SI_BEEP_HEADSET_I, (void*)&bHeadsetBeepLvl, TRUE );
   }
   else if(szMode == VOICE_VOL)
   {
//	 nvi.speaker_level = ui.ear_spkr_lvl =VOL;
     //nvi.speaker_level = gSndSet.HFKVoiceLvl = VOL;
	 //ui_put_nv(NV_SPEAKER_LVL_I, &nvi);
     bHFKVoiceLvl = VOL;
     EQS_PutUi( ES_SI_EAR_SPKR_I, (void*)&bHFKVoiceLvl, TRUE );

//	 nvi.ear_level    = ui.ear_lvl = VOL;
     //nvi.ear_level    = gSndSet.VoiceLvl = VOL;
	 //ui_put_nv(NV_EAR_LVL_I, &nvi);
     bVoiceLvl = VOL;
     EQSPuUi( ES_SI_EAR_LVL_I, (void*)&bVoiceLvl, TRUE ); 

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
	
	//bCurrentLvl = ui.ear_lvl;
    EQS_GetUi( ES_SI_EAR_LVL_I, (void*)&bVoiceLvl, FALSE ); 
    bCurrentLvl = bVoiceLvl;
	
	switch(pInEvent->EventID)
    {
    case EQE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
	   EQS_TextOut( 0, 0, (BYTE *)TKN_VOICE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_APP_END : /* The very last event */
	   EQS_ReleaseDC();
       break;

    case EQE_APP_DRAW :
	   EQS_TextOut( 0,  0, (BYTE *)TKN_VOICE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_UP :
       if(bCurrentLvl < 4) bCurrentLvl++;
	   else bCurrentLvl = 4;
	   s_SetVol(VOICE_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_DOWN :
	   if(bCurrentLvl > 0) bCurrentLvl--;
	   else bCurrentLvl = 0;
       s_SetVol(VOICE_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();
       break;

	case EQE_KEY_CENTER :
	   s_UpdateVol_NV(VOICE_VOL, bCurrentLvl);
	   sprintf((char *)MessgBuff,"통화음크기설정 끝");
	   EQS_TextOut(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
	   break;
    
	case EQE_KEY_RIGHT :
	case EQE_KEY_CLEAR :
	case EQE_KEY_END :
		EQS_End();    	
	
	case EQE_TM_GENERAL : /* UI_TIMER_F: */
       EQS_TextOut(0, 0, (BYTE *)MessgBuff);
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
    BYTE  bBeepLvl;

	//bCurrentLvl = ui.beep_lvl;
    EQS_GetUi( ES_SI_BEEP_LVL_I, (void*)&bBeepLvl, FALSE );
    bCurrentLvl = bBeepLvl;
	
	switch(pInEvent->EventID)
    {
    case EQE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
	   EQS_TextOut( 0, 0, (BYTE *)TKN_KEYTONE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_APP_END : /* The very last event */
	   EQS_ReleaseDC();
       break;

    case EQE_APP_DRAW :
	   EQS_TextOut( 0,  0, (BYTE *)TKN_KEYTONE_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_UP :
	   if(bCurrentLvl < 4) bCurrentLvl++;
	   else bCurrentLvl = 4;
	   s_SetVol(BEEP_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_DOWN :
	   if(bCurrentLvl > 0) bCurrentLvl--;
	   else bCurrentLvl = 0;
       s_SetVol(BEEP_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();
       break;

	case EQE_KEY_CENTER :
       s_UpdateVol_NV(BEEP_VOL, bCurrentLvl);
	   sprintf((char *)MessgBuff,"키톤크기설정 끝");
	   EQS_TextOut(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
	   break;
    
	case EQE_KEY_RIGHT :
	case EQE_KEY_CLEAR :
	case EQE_KEY_END :
		EQS_End();    	
	
	case EQE_TM_GENERAL : /* UI_TIMER_F: */
       EQS_TextOut(0, 0, (BYTE *)MessgBuff);
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
	   EQS_TextOut( 0, 0, (BYTE *)TKN_MUSIC_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   

#ifdef FEATURE_YMU757B_MELODY	 
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
       uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_TIMEGOES, bMusicLvl);       
#else
       uisnd_sample_music(UI_MUSIC_ID_SND, NULL, NULL, MUS_TIMEGOES, ui.music_lvl);       
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
#elif defined FEATURE_YMU759_MELODY         //model B1

#endif
	   break;

    case EQE_APP_END : /* The very last event */

	   EQS_ReleaseDC();
       break;

    case EQE_APP_DRAW :
	   EQS_TextOut( 0,  0, (BYTE *)TKN_MUSIC_TEST);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_UP :
	   if(bCurrentLvl < 5) bCurrentLvl++;
	   else bCurrentLvl = 5;
	   s_SetVol(MUSIC_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();	   
	   break;

    case EQE_KEY_DOWN :
	   if(bCurrentLvl > 0) bCurrentLvl--;
	   else bCurrentLvl = 0;
       s_SetVol(MUSIC_VOL, bCurrentLvl);
	   sprintf((char *)VolBuff,"%d",bCurrentLvl);
       EQS_TextOut( 0, 60, (BYTE*)VolBuff );
	   EQS_UpdateDisplay();
       break;

	case EQE_KEY_CENTER :
       s_UpdateVol_NV(MUSIC_VOL, bCurrentLvl);
	   sprintf((char *)MessgBuff,"뮤직벨크기설정 끝");
	   EQS_TextOut(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
#ifdef FEATURE_YMU757B_MELODY           
    	uisnd_music_stop();
#elif defined FEATURE_YMU759_MELODY		// model b1

#endif
	   EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
	   break;
    
	case EQE_KEY_RIGHT :
	case EQE_KEY_CLEAR :
	case EQE_KEY_END :
		EQS_End();    	
	
	case EQE_TM_GENERAL : /* UI_TIMER_F: */
       EQS_TextOut(0, 0, (BYTE *)MessgBuff);
	   EQS_UpdateDisplay();
	   EQS_End();
      break;
    
    default:
      EQS_DefEventProc(pInEvent);
      break;
	}
}
#endif // OMIT 
/*===========================================================================
              
			                End by hjcho 00.08.06

===========================================================================*/ 






/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

VOID EQA_VolumeTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	UCHAR			szTmpString[20];
	UINT8			iKey = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
		if(IsAudioGainTest == FALSE)
		{

			typedef struct {
				word           voc_gain;    /* Vocoder Gain for this setting        */
				word           dtmf_gain;   /* DTMF gain for this setting           */
				voc_coeff_type voc_filter;  /* Vocoder Filters for this setting     */
				byte           codec_gain;  /* Codec Gain for this setting          */
			} sndhw_ctl_type;
			extern const sndhw_ctl_type *sndhw_get_voc_setting(snd_pstate_type);
			extern snd_pstate_type	snd_crnt_path_out;
			sndhw_ctl_type			*hw_table;
			hw_table = (sndhw_ctl_type *)sndhw_get_voc_setting(snd_crnt_path_out);
			
			AudioTestGain.VocoderRx	= hw_table->voc_gain;
			AudioTestGain.DTMF		= hw_table->dtmf_gain;
			AudioTestGain.CODECRx	= 0x04;
			AudioTestGain.CODECTx	= 0x40;
			AudioTestGain.FM		= 0x1A;
			AudioTestGain.HP		= 0x1F;
			AudioTestGain.SPK		= 0x1F;
		}
		IsAudioGainTest = TRUE;		

        EQS_GetDC(TRUE);
		hHnd = EQC_ScrollList((CHAR** CONST)VolumeMenu, 8, 8, 0, 0, 16, 5, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
	case EQE_KEY_3:
	case EQE_KEY_4:
	case EQE_KEY_5:
	case EQE_KEY_6:
	case EQE_KEY_7:
	case EQE_KEY_8:
    case EQE_KEY_9:
	case EQE_KEY_STAR:
	case EQE_KEY_POUND:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd))
		{
		case 0: EQS_StartChild(EQA_CurrentGain);    break;
		case 1:	EQS_StartChild(EQA_VocoderRxGain);	break;
		case 2:
			sprintf((CHAR*)szTmpString, "없음");
			EQC_MesgBox((CHAR*)szTmpString, 3, FT_MED,MB_BEEP);
			break;
		case 3:	EQS_StartChild(EQA_CODECRxGain);	break;
		case 4:	EQS_StartChild(EQA_CODECTxGain);	break;
		case 5:	EQS_StartChild(EQA_MelodyICFMGain);	break;
		case 6:	EQS_StartChild(EQA_MelodyICHPGain);	break;
		case 7:	EQS_StartChild(EQA_MelodyICSPKGain);break;
		default:break;
		}
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
