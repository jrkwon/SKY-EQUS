#include "comdef.h"
#include	"audTest.h"
#ifdef YAMAHA_MELODY
#include	"yamaha.h"
#endif
#include	"..\qualcomm\voc.h"
#include	"..\qualcomm\vocdata.h"
#include	"..\qualcomm\sndihw.h"	// woodstok 00.4.27
#include	"..\..\mmi\sk\uicommon.h"
#include	"..\..\mmi\sk\ui\ui.h"
#include	"..\..\mmi\sk\ui\uistate.h"
#include	"..\..\mmi\sk\vocoder\voccommon.h"

//#define AUD_DEBUG

#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
#define AUDIO_MENU_NUM	4
#else
#define AUDIO_MENU_NUM	3
#endif

 
MENU_STATE_T 	VOC_loopback(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	VOC_PCMloopback(MENU_STATE_T state, UB keyin);

MENU_STATE_T 	All_control_volume(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_is_apply_value(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_volume(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_CodecRxGain(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_RxVolume(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_CodecTxGain(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_TxVolume(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_SideTone(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_MicAmp1Gain(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_MicAmp2Pass(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_DTMFTxGain(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_DTMFRxGain(MENU_STATE_T state, UB keyin);
#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
MENU_STATE_T 	ui_fn_Cntr_SPKVol(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_FMVol(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Cntr_HPVol(MENU_STATE_T state, UB keyin);
#endif

MENU_STATE_T 	ui_fn_Cntr_path(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Select_Mic(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_Select_Amp(MENU_STATE_T state, UB keyin);

MENU_STATE_T 	ui_fn_Cntr_filter(MENU_STATE_T state, UB keyin);
MENU_STATE_T	ui_fn_Make_TxPCMFilt(MENU_STATE_T state, UB keyin);
MENU_STATE_T	ui_fn_Make_TxPCMFilt(MENU_STATE_T state, UB keyin);
MENU_STATE_T	ui_fn_TxHPFilt(MENU_STATE_T state, UB keyin);
MENU_STATE_T	ui_fn_RxHPFilt(MENU_STATE_T state, UB keyin);
MENU_STATE_T	ui_fn_TxSlopeFilt(MENU_STATE_T state, UB keyin);

MENU_STATE_T 	ui_fn_Cntr_ECNS(MENU_STATE_T state, UB keyin);
MENU_STATE_T	ui_fn_controlNS(MENU_STATE_T state, UB keyin);
MENU_STATE_T	ui_fn_controlEC(MENU_STATE_T state, UB keyin);

MENU_STATE_T 	ui_fn_LoadDefaultSetup(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_SaveCurrentSetup(MENU_STATE_T state, UB keyin);
MENU_STATE_T 	ui_fn_LoadCurrentSetup(MENU_STATE_T state, UB keyin);

#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
MENU_STATE_T 	New_Melody_play(MENU_STATE_T state, UB keyin);
#endif
#ifdef AUD_DEBUG
MENU_STATE_T	audio_fn_execute(MENU_STATE_T state, UB keyin);
#endif

#ifdef AUD_DEBUG
MENU_STATE_T (* const ui_fntab_AudioTest[AUDIO_MENU_NUM+1])(MENU_STATE_T state, UB keyin)={
#else
MENU_STATE_T (* const ui_fntab_AudioTest[AUDIO_MENU_NUM])(MENU_STATE_T state, UB keyin)={
#endif
	VOC_loopback
	,VOC_PCMloopback
	,All_control_volume
#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
	,New_Melody_play
#endif
#ifdef AUD_DEBUG
	,audio_fn_execute
#endif
};

#ifdef AUD_DEBUG
const char ui_txt_AudioTest[AUDIO_MENU_NUM+1][NUM_LCD_COLUMNS]={
#else
const char ui_txt_AudioTest[AUDIO_MENU_NUM][NUM_LCD_COLUMNS]={
#endif
	"\204\301ALOOPBACK시험",
	"\204\302PLOOPBACK시험",
	"\204\303ValueTest"
#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
	,"\204\304새멜로디 청취"
#endif
#ifdef AUD_DEBUG
	,"\204\305audio debug   "
#endif
};

boolean Test_sound_on = FALSE;
audio_param_type TestVol;
qdsp_cmd_codec_config_type buffer_codec_config;

MENU_STATE_T	audio_test_menu(MENU_STATE_T state, UB keyin)
{

	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = " 오디오 테스트 ";

	MenuText = (char*)ui_txt_AudioTest;
	nLines = sizeof(ui_txt_AudioTest) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		ui.pad = TRUE;
		ui.menuvol =TRUE;
//		Test_sound_on = TRUE;
		ui.beep_updown = FALSE;
		ui.multi_key = FALSE;

		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
#ifdef AUD_DEBUG
		case DIGIT5_K:
#endif
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_AudioTest[keyin - DIGIT1_K];
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case CLR_K:
		case MEMORY_K:
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;

			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_AudioTest[Index];
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

void Get_All_volume(audio_param_type *Vol)
{
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
#ifdef SAVE_AUDIO_TEST_DATA
//	ui_get_nv( NV_AUDIO_PARAM_I ,( nv_item_type * )Vol);

#else
/*	Vol->CodecRxGain = get_voc_data_codec_rx_vol();
	Vol->CodecTxGain = get_voc_data_codec_tx_vol();
	Vol->RxVolume = get_voc_data_rx_vol();
	Vol->CodecStGain =  VOC_DBM_MIN;//get_voc_data_codec_st_vol();
	Vol->TxVolume = get_voc_data_tx_vol();
#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
	Vol->SPK_vol  = 0x1C;
	Vol->FM_vol  = 0x18;
	Vol->HP_vol  = 0x10;
#endif*/
	Vol->MicSel=0;
	Vol->AmpSel=0;
	Vol->MicAmp1Gain=0;
	Vol->MicAmp2Bpass=0;
	Vol->TxHPFdis=0;
	Vol->RxHPFdis=0;
	Vol->TxSlopeFiltDis=0;
	Vol->CodecTxGain=0;
	Vol->CodecStGain=0;
	Vol->CodecRxGain=0;
	Vol->TxVolume=0;
	Vol->RxVolume=0;
	Vol->DtmfTxGain=0;
	Vol->DtmfRxGain=0;
	Vol->NSSwitch=0;
	Vol->ECSwitch=0;
	Vol->ECmode=0;
#ifdef YAMAHA_MELODY
	Vol->SPKvol=0;
	Vol->FMvol=0;
	Vol->HPvol=0;
#endif
#endif
}

void Get_cur_audio_value(audio_param_type *TestVol)
{

}

void codec_reconfig(void)
{
  voc_codec_type             pcm_path;
  qdsp_cmd_codec_config_type codec_config;

  /* Issue the codec config command */
  (void) voc_data_get_codec(&pcm_path, &codec_config);
  qdsp_cmd_codec_config(&codec_config, FALSE);
}

const char ui_txt_loopback[2][NUM_LCD_COLUMNS]={
	"\205\241LoopbackOn ",
	"\205\242LoopbackOff "
};

MENU_STATE_T 	VOC_loopback(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_AudioTest[0];

	MenuText = (char*)ui_txt_loopback;
	nLines = sizeof(ui_txt_loopback) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
//		Get_All_volume(&TestVol);
		uisnd_set_device( SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE );
//		voc_acquire(VOC_ACQ_TEST, NULL) ;
		voc_config(VOC_ACQ_SND, VOC_CAP_IS127);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:
//			uisnd_set_device( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:
			ui.multi_key = TRUE;
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			uisnd_set_device( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
		case FUNCTION_K:
			switch(Index)
			{
				case 0:

					voc_audio_loop_cmd (TRUE) ;
				{
					//loopback 시 echo cancel부분을 off해서 소리가 끊기던 것을 막음 in 2000.5.12
					  qdsp_cmd_ec_config_type  ec_set;  

					ec_set.ec_reset_flag = (QDSP_ECHO_RES_EC_DIS_V |
                              QDSP_ECHO_RES_ECBNE_DIS_V);

					  ec_set.ec_switch = QDSP_ECHO_CANCEL_DIS_V;
					qdsp_cmd_ec_ctrl(&ec_set);
				}

					break;
				case 1:
				default:
					voc_audio_loop_cmd (FALSE) ; 
					uisnd_set_device( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );					
					break;
			}
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;
		uisnd_set_device( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );
//		voc_release(VOC_ACQ_TEST) ;
		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}

	return(NewSate);
//	return MENU_EXIT_S;
}


MENU_STATE_T 	VOC_PCMloopback(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_AudioTest[1];

	MenuText = (char*)ui_txt_loopback;
	nLines = sizeof(ui_txt_loopback) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
//		Get_All_volume(&TestVol);
//		uisnd_set_device( SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE );
		voc_acquire(VOC_ACQ_SND, NULL) ;
//		voc_config(VOC_ACQ_SND, VOC_CAP_IS127);
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:
//			uisnd_set_device( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:
			ui.multi_key = TRUE;
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			voc_release(VOC_ACQ_SND) ;
//			uisnd_set_device( SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE );
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
//		case DIGIT3_K:
//		case DIGIT4_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
		case FUNCTION_K:
			switch(Index)
			{
				case 0:
//					voc_acquire(VOC_ACQ_SND, NULL) ;
					sndhw_set_volume();
					voc_pcm_loop_cmd(TRUE);
						break;
				case 1:
					voc_pcm_loop_cmd(FALSE);
					voc_release(VOC_ACQ_SND) ;
					break;
/*				case 2:
					voc_acquire(VOC_ACQ_SND, NULL) ;
					voc_pcm_loop_cmd(TRUE);
						break;
				case 3:
					voc_pcm_loop_cmd(FALSE);
					voc_release(VOC_ACQ_SND) ;*/
				default:
					break;
			}
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;
		voc_release(VOC_ACQ_SND) ;
//		voc_release(VOC_ACQ_TEST) ;
		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}

	return(NewSate);
}

//#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
//MENU_STATE_T (* const ui_fntab_ContVol[8])(MENU_STATE_T state, UB keyin)={
//#else
MENU_STATE_T (* const ui_fntab_Control[8])(MENU_STATE_T state, UB keyin)={
//#endif
	ui_fn_is_apply_value,
	ui_fn_Cntr_volume,
	ui_fn_Cntr_path,
	ui_fn_Cntr_filter,
	ui_fn_Cntr_ECNS,
	ui_fn_LoadCurrentSetup,
	ui_fn_SaveCurrentSetup,
	ui_fn_LoadDefaultSetup
	
};

//#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
//const char ui_txt_Contvol[8][NUM_LCD_COLUMNS]={
//#else
const char ui_txt_Control[8][NUM_LCD_COLUMNS]={
//#endif
		"\204\301Is apply?",
		"\204\302Ctrl Gain",
		"\204\303Ctrl Path",
		"\204\304Ctrl Filter",
		"\204\305Ctrl EC&NS",
		"\204\306Load Current",
		"\204\307Save Current",
		"\204\310Load from NV"
};


MENU_STATE_T 	All_control_volume(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_AudioTest[2];

	MenuText = (char*)ui_txt_Control;
	nLines = sizeof(ui_txt_Control) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */

//		Get_All_volume(&TestVol);
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case DIGIT5_K:
		case DIGIT6_K:
		case DIGIT7_K:
		case DIGIT8_K:

			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_Control[keyin - DIGIT1_K];
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_Control[Index];
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}
/****************************************************************************

	ui_fn_is_apply_value() :테스트 값을 쓸건지 말건지

****************************************************************************/
const char ui_txt_Isapp[2][NUM_LCD_COLUMNS]={
	"\205\241 Yes  ",
	"\205\242 Remove "
};

MENU_STATE_T 	ui_fn_is_apply_value(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Control[0];

	MenuText = (char*)ui_txt_Isapp;
	nLines = sizeof(ui_txt_Isapp) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		if(Test_sound_on)
			Index = 0;
		else
			Index = 1;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			
			kj_ClearScreen();
			switch(Index)
			{
				case 0:
					Test_sound_on = TRUE;
					kj_SetTextYCenterAlign(1, "테스트오디오");
					kj_SetTextYCenterAlign(2, "값을");
					kj_TextOutYCenterAlign(3, "적용합니다");

				break;
				case 1:
					Test_sound_on = FALSE;
					kj_SetTextYCenterAlign(1, "테스트오디오");
					kj_SetTextYCenterAlign(2, "값을");
					kj_TextOutYCenterAlign(3, "해제합니다");

				break;
			}
			sndhw_set_volume( );
			codec_reconfig();
			voc_state_init_aud_process();
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
			//적용함 : 3번째 argu.를 FALSE로 두면 현재상태를 update하는 효과가 있음
			//uisnd_set_device(SND_MUTE_UNMUTED,SND_MUTE_UNMUTED, TRUE);
			// 2sec Timer
			ui_TimeOutReason = UI_SELECT_COMP_DISP;
			kj_TimerStart(KJ_TM_F_DISP,KJ_T_2S);
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case TIMEOUT_S:
		if(ui_TimeOutReason == UI_SELECT_COMP_DISP){
			kj_ClearScreen();
			ui_CurMenu=(MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if (ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

/****************************************************************************

	ui_fn_Cntr_volume() :GAIN관련 값들을 변경함

****************************************************************************/


#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
MENU_STATE_T (* const ui_fntab_ContVol[12])(MENU_STATE_T state, UB keyin)={
#else
MENU_STATE_T (* const ui_fntab_ContVol[9])(MENU_STATE_T state, UB keyin)={
#endif
	ui_fn_Cntr_CodecRxGain,
	ui_fn_Cntr_CodecTxGain,
	ui_fn_Cntr_SideTone,
	ui_fn_Cntr_TxVolume,
	ui_fn_Cntr_RxVolume,
	ui_fn_Cntr_MicAmp1Gain,
	ui_fn_Cntr_MicAmp2Pass,
	ui_fn_Cntr_DTMFTxGain,
	ui_fn_Cntr_DTMFRxGain
#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
	,ui_fn_Cntr_SPKVol,
	ui_fn_Cntr_FMVol,
	ui_fn_Cntr_HPVol
#endif
};

#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
const char ui_txt_Contvol[12][NUM_LCD_COLUMNS]={
#else
const char ui_txt_Contvol[9][NUM_LCD_COLUMNS]={
#endif
		"\204\301Codec Rx Gain",
		"\204\302Codec Tx Gain",
		"\204\303SideToneGain",
		"\204\304Tx Volume",
		"\204\305Rx volume",
		"\204\306MicAmpGain",
		"\204\307MicAmpPass",
		"\204\310DTMF TxGain",
		"\204\311DTMF RxGain"
#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
		,"\204\312Spk vol",
		"\204\313FM vol",
		"\204\314HP vol"
#endif
};


MENU_STATE_T 	ui_fn_Cntr_volume(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Control[1];

	MenuText = (char*)ui_txt_Contvol;
	nLines = sizeof(ui_txt_Contvol) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */

		Get_All_volume(&TestVol);
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case DIGIT5_K:
		case DIGIT6_K:
		case DIGIT7_K:
		case DIGIT8_K:
		case DIGIT9_K:
		case DIGIT0_K:

			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContVol[keyin - DIGIT1_K];
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContVol[Index];
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

const char ui_txt_dbm1[67][NUM_LCD_COLUMNS]={
	"\205\241 -50dB ",
	"\205\242 -49dB ",
	"\205\243 -48dB ",
	"\205\244 -47dB ",
	"\205\245 -46dB ",
	"\205\246 -45dB ",
	"\205\247 -44dB ",
	"\205\250 -43dB ",
	"\205\251 -42dB ",
	"\205\252 -41dB ",
	"\205\253 -40dB ",
	"\205\254 -39dB ", 
	"\205\255 -38dB ", 
	"\205\256 -37dB ", 
	"\205\257 -36dB ", 
	"\205\260 -35dB ",
	"\205\261 -34dB ",
	"\205\262 -33dB ",
	"\205\263 -32dB ",
	"\205\300 -31dB ",
	"\205\301 -30dB ",
	"\205\302 -29dB ",
	"\205\303 -28dB ",
	"\205\304 -27dB ",
	"\205\305 -26dB ",
	"\205\306 -25dB ",
	"\205\307 -24dB ",
	"\205\310 -23dB ",
	"\205\311 -22dB ",
	"\205\312 -21dB ",
	"\205\313 -20dB ",
	"\205\314 -19dB ",
	"\205\315 -18dB ",
	"\205\316 -17dB ",
	"\205\317 -16dB ",
	"\205\320 -15dB ",
	"\205\321 -14dB ",
	"\205\322 -13dB ",
	"\205\323 -12dB ",
	"\205\324 -11dB ",
	"\205\325 -10dB ",
	"\205\326  -9dB ",
	"\205\327  -8dB ",
	"\205\330  -7dB ",
	"\205\331  -6dB ",
	"\205\332  -5dB ",
	"\205\333  -4dB ",
	"\205\334  -3dB ",
	"\205\335  -2dB ",
	"\205\336  -1dB ",
	"\205\337   0dB ",
	"\205\340   1dB ",
	"\205\341   2dB ",
	"\205\342   3dB ",
	"\205\343   4dB ",
	"\205\344   5dB ",
	"\205\345   6dB ",
	"\205\346   7dB ",
	"\205\347   8dB ",
	"\205\350   9dB ",
	"\205\351  10dB ",
	"\205\352  11dB ",
	"\205\353  12dB ",
	"\205\354  13dB ",
	"\205\355  14dB ",
	"\205\356  15dB ",
	"\205\356  16dB "
};


MENU_STATE_T 	ui_fn_Cntr_CodecRxGain(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[0];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = (char)((TestVol.CodecRxGain - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.CodecRxGain = ((sint15)Index * VOC_DBM_STEP + VOC_DBM_MIN) ; 
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

MENU_STATE_T 	ui_fn_Cntr_CodecTxGain(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[1];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = (char)((TestVol.CodecTxGain - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.CodecTxGain = (sint15)Index* VOC_DBM_STEP + VOC_DBM_MIN; 
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

MENU_STATE_T 	ui_fn_Cntr_RxVolume(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[4];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = (char)((TestVol.RxVolume - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.RxVolume = (sint15)Index* VOC_DBM_STEP  + VOC_DBM_MIN; 
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

MENU_STATE_T 	ui_fn_Cntr_TxVolume(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[3];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = (char)((TestVol.TxVolume - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.TxVolume = (sint15)Index* VOC_DBM_STEP  + VOC_DBM_MIN; 
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

MENU_STATE_T 	ui_fn_Cntr_SideTone(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[2];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = (char)((TestVol.CodecStGain - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.CodecStGain = (sint15)Index* VOC_DBM_STEP  + VOC_DBM_MIN;
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

const char ui_txt_dbm2[4][NUM_LCD_COLUMNS]={
	"\205\241 -2dB ",
	"\205\242 +6dB ",
	"\205\243 +8dB ",
	"\205\244 +16dB "
};

MENU_STATE_T 	ui_fn_Cntr_MicAmp1Gain(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[5];

	MenuText = (char*)ui_txt_dbm2;
	nLines = sizeof(ui_txt_dbm2) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
//		if(Test_sound_on)
		Index = TestVol.MicAmp1Gain;
//		else
//			Index = 1;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			TestVol.MicAmp1Gain = (byte)Index;

			buffer_codec_config.codec_func_ctrl = (buffer_codec_config.codec_func_ctrl & 0xffe7) | (TestVol.MicAmp1Gain << 3);

			codec_reconfig();
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

const char ui_txt_MicAmp2Pass[2][NUM_LCD_COLUMNS]={
	"\205\241 Bypass",
	"\205\242 Connect"
};

MENU_STATE_T 	ui_fn_Cntr_MicAmp2Pass(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[6];

	MenuText = (char*)ui_txt_MicAmp2Pass;
	nLines = sizeof(ui_txt_MicAmp2Pass) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = TestVol.MicAmp2Bpass;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			if(Index == 0)
			{
				TestVol.MicAmp2Bpass = TRUE;
				buffer_codec_config.codec_func_ctrl = (buffer_codec_config.codec_func_ctrl | 0x0020);
			}
			else if(Index == 1)
			{
				TestVol.MicAmp2Bpass = FALSE;
				buffer_codec_config.codec_func_ctrl = (buffer_codec_config.codec_func_ctrl & 0xffdf);
			}
			codec_reconfig();
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

MENU_STATE_T 	ui_fn_Cntr_DTMFTxGain(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[7];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		ui_TimeOutReason = UI_SELECT_COMP_DISP;
		kj_TimerStart(KJ_TM_F_DISP,KJ_T_2S);
		kj_ClearScreen();
		kj_SetTextYCenterAlign(1, "Under");
		kj_TextOutYCenterAlign(2, "Construct");

		break;

		//Index = (char)((voc_data_convert_dbm(TestVol.CodecStGain) - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case TIMEOUT_S:
		if(ui_TimeOutReason == UI_SELECT_COMP_DISP){
			kj_ClearScreen();
			ui_CurMenu=(MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if (ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
		}
		break;

	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
//			TestVol.CodecStGain = voc_data_compute_dbm(0x4000,Index* VOC_DBM_STEP  + VOC_DBM_MIN); 
//			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

MENU_STATE_T 	ui_fn_Cntr_DTMFRxGain(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[8];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS - 16;//maximum value is 0dB

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = (char)((TestVol.DtmfRxGain - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.DtmfRxGain = (sint15)Index* VOC_DBM_STEP  + VOC_DBM_MIN; 
//			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}


#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만
const char ui_txt_yamahaVol[32][NUM_LCD_COLUMNS]={
	"\205\241 MUTE ",
	"\205\242 -30dB ",
	"\205\243 -29dB ",
	"\205\244 -28dB ",
	"\205\245 -27dB ",
	"\205\246 -26dB ",
	"\205\247 -25dB",
	"\205\250 -24dB",
	"\205\251 -23dB",
	"\205\252 -22dB",
	"\205\253 -21dB",
	"\205\254 -20dB", 
	"\205\255 -19dB", 
	"\205\256 -18dB", 
	"\205\257 -17dB", 
	"\205\260 -16dB",
	"\205\261 -15dB",
	"\205\262 -14dB",
	"\205\263 -13dB",
	"\205\300 -12dB",
	"\205\301 -11dB",
	"\205\302 -10dB",
	"\205\303  -9dB",
	"\205\304  -8dB",
	"\205\305  -7dB",
	"\205\306  -6dB  ",
	"\205\307  -5dB ",
	"\205\310  -4dB ",
	"\205\311  -3dB ",
	"\205\312  -2dB ",
	"\205\313  -1dB  ",
	"\205\314   0dB  "
};

MENU_STATE_T 	ui_fn_Cntr_SPKVol(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[9];

	MenuText = (char*)ui_txt_yamahaVol;
	nLines = sizeof(ui_txt_yamahaVol) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = TestVol.SPKvol;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;

			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.SPKvol = Index; 
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}


MENU_STATE_T 	ui_fn_Cntr_FMVol(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[10];

	MenuText = (char*)ui_txt_yamahaVol;
	nLines = sizeof(ui_txt_yamahaVol) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = TestVol.FMvol;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;

			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.FMvol = Index; 
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}


MENU_STATE_T 	ui_fn_Cntr_HPVol(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Contvol[11];

	MenuText = (char*)ui_txt_yamahaVol;
	nLines = sizeof(ui_txt_yamahaVol) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = TestVol.HPvol;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;

			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			TestVol.HPvol = Index; 
			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

#endif // YAMAHA_MELODY

/****************************************************************************

	ui_fn_Cntr_path() :패스에 관련된 값을 변경

****************************************************************************/

MENU_STATE_T (* const ui_fntab_ContPath[2])(MENU_STATE_T state, UB keyin)={
	ui_fn_Select_Mic,
	ui_fn_Select_Amp
};

const char ui_txt_ContPath[2][NUM_LCD_COLUMNS]={
		"\204\301Mic Select",
		"\204\302Amp Select"
};

MENU_STATE_T 	ui_fn_Cntr_path(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Control[2];

	MenuText = (char*)ui_txt_ContPath;
	nLines = sizeof(ui_txt_ContPath) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		ui.pad = TRUE;
		ui.menuvol =TRUE;
//		Test_sound_on = TRUE;
		ui.beep_updown = FALSE;
		ui.multi_key = FALSE;

//		Get_All_volume(&TestVol);
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContPath[keyin - DIGIT1_K];
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContPath[Index];
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

const char ui_txt_Select_Mic[4][NUM_LCD_COLUMNS]={
	"\205\241 No select",
	"\205\242 Mic1 Input",
	"\205\243 Mic2 Input",
	"\205\244 Aux Input"
};

MENU_STATE_T 	ui_fn_Select_Mic(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContPath[0];

	MenuText = (char*)ui_txt_Select_Mic;
	nLines = sizeof(ui_txt_Select_Mic) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
//		if(Test_sound_on)
		Index = TestVol.MicSel;
//		else
//			Index = 1;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			TestVol.MicSel = (byte)Index;
			buffer_codec_config.codec_func_ctrl = (buffer_codec_config.codec_func_ctrl & 0xfff9) | (TestVol.MicSel << 1);
			codec_reconfig();
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

const char ui_txt_Select_Amp[4][NUM_LCD_COLUMNS]={
	"\205\241 No select",
	"\205\242 Ear Amp1",
	"\205\243 Ear Amp2",
	"\205\244 Aux Amp"
};

MENU_STATE_T 	ui_fn_Select_Amp(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContPath[1];

	MenuText = (char*)ui_txt_Select_Amp;
	nLines = sizeof(ui_txt_Select_Amp) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
//		if(Test_sound_on)
		Index = TestVol.AmpSel;
//		else
//			Index = 1;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			TestVol.AmpSel = (byte)Index;
			buffer_codec_config.codec_func_ctrl = (buffer_codec_config.codec_func_ctrl & 0xff3f) | (TestVol.AmpSel << 6);
			codec_reconfig();
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

/****************************************************************************

	ui_fn_Cntr_filter() :PCM filter값을 지정함

****************************************************************************/

MENU_STATE_T (* const ui_fntab_ContFilter[5])(MENU_STATE_T state, UB keyin)={
	ui_fn_Make_TxPCMFilt,
	ui_fn_Make_TxPCMFilt,
//	ui_fn_Make_RxPCMFilt
	ui_fn_TxHPFilt,
	ui_fn_RxHPFilt,
	ui_fn_TxSlopeFilt
};

const char ui_txt_ContFilter[5][NUM_LCD_COLUMNS]={
		"\204\301TxPCMFilt",
		"\204\302RxPCMFilt",
		"\204\303TxHPF",
		"\204\304RxHPF",
		"\204\305TxSlopeFilt"
};

MENU_STATE_T 	ui_fn_Cntr_filter(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Control[3];

	MenuText = (char*)ui_txt_ContFilter;
	nLines = sizeof(ui_txt_ContFilter) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		ui.pad = TRUE;
		ui.menuvol =TRUE;
//		Test_sound_on = TRUE;
		ui.beep_updown = FALSE;
		ui.multi_key = FALSE;

//		Get_All_volume(&TestVol);
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContFilter[keyin - DIGIT1_K];
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContFilter[Index];
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

MENU_STATE_T 	ui_fn_Make_TxPCMFilt(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContFilter[0];

	MenuText = (char*)ui_txt_dbm1;
	nLines = sizeof(ui_txt_dbm1) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		ui_TimeOutReason = UI_SELECT_COMP_DISP;
		kj_TimerStart(KJ_TM_F_DISP,KJ_T_2S);
		kj_ClearScreen();
		kj_SetTextYCenterAlign(1, "Under");
		kj_TextOutYCenterAlign(2, "Construct");
		break;

		//Index = (char)((voc_data_convert_dbm(TestVol.CodecStGain) - VOC_DBM_MIN )/ VOC_DBM_STEP);

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case TIMEOUT_S:
		if(ui_TimeOutReason == UI_SELECT_COMP_DISP){
			kj_ClearScreen();
			ui_CurMenu=(MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if (ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
		}
		break;

	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
//			TestVol.CodecStGain = voc_data_compute_dbm(0x4000,Index* VOC_DBM_STEP  + VOC_DBM_MIN); 
//			sndhw_set_volume( );
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

const char ui_txt_ApplyFilter[2][NUM_LCD_COLUMNS]={
	"\205\241 Disable",
	"\205\242 Enable"
};

MENU_STATE_T 	ui_fn_TxHPFilt(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContFilter[2];

	MenuText = (char*)ui_txt_ApplyFilter;
	nLines = sizeof(ui_txt_ApplyFilter) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		if(TestVol.TxHPFdis)
			Index = 0;
		else
			Index = 1;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			if(Index == 0)
			{
				TestVol.TxHPFdis = TRUE;
				buffer_codec_config.pcm_format_ctrl = (buffer_codec_config.pcm_format_ctrl | 0x0400);
			}
			else if(Index == 1)
			{
				TestVol.TxHPFdis = FALSE;
				buffer_codec_config.pcm_format_ctrl = (buffer_codec_config.pcm_format_ctrl & 0xfbff);
			}
			codec_reconfig();
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}


MENU_STATE_T 	ui_fn_RxHPFilt(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContFilter[3];

	MenuText = (char*)ui_txt_ApplyFilter;
	nLines = sizeof(ui_txt_ApplyFilter) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		if(TestVol.RxHPFdis)
			Index = 0;
		else
			Index = 1;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			if(Index == 0)
			{
				TestVol.RxHPFdis = TRUE;
				buffer_codec_config.pcm_format_ctrl = (buffer_codec_config.pcm_format_ctrl | 0x0800);
			}
			else if(Index == 1)
			{
				TestVol.RxHPFdis = FALSE;
				buffer_codec_config.pcm_format_ctrl = (buffer_codec_config.pcm_format_ctrl & 0xf7ff);
			}
			codec_reconfig();
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

MENU_STATE_T 	ui_fn_TxSlopeFilt(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContFilter[4];

	MenuText = (char*)ui_txt_ApplyFilter;
	nLines = sizeof(ui_txt_ApplyFilter) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		if(TestVol.TxSlopeFiltDis)
			Index = 0;
		else
			Index = 1;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			if(Index == 0)
			{
				TestVol.TxSlopeFiltDis = TRUE;
				buffer_codec_config.pcm_format_ctrl = (buffer_codec_config.pcm_format_ctrl | 0x0200);
			}
			else if(Index == 1)
			{
				TestVol.TxSlopeFiltDis = FALSE;
				buffer_codec_config.pcm_format_ctrl = (buffer_codec_config.pcm_format_ctrl & 0xfdff);
			}
			codec_reconfig();
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}


/****************************************************************************

	ui_fn_Cntr_ECNS() :EC와 NS

****************************************************************************/

MENU_STATE_T (* const ui_fntab_ContNSEC[2])(MENU_STATE_T state, UB keyin)={

	ui_fn_controlNS,
	ui_fn_controlEC
};

const char ui_txt_ContNSEC[2][NUM_LCD_COLUMNS]={
		"\204\301NoiseSupp",
		"\204\302EchoCancel"
};

MENU_STATE_T 	ui_fn_Cntr_ECNS(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_Control[4];

	MenuText = (char*)ui_txt_ContNSEC;
	nLines = sizeof(ui_txt_ContNSEC) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:

//		Get_All_volume(&TestVol);
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContNSEC[keyin - DIGIT1_K];
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			ui_PushMenu(ui_CurMenu);
			ui_CurMenu = ui_fntab_ContNSEC[Index];
			(*ui_CurMenu)(ENTRANCE_S, 0);
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

MENU_STATE_T 	ui_fn_controlNS(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContNSEC[0];

	MenuText = (char*)ui_txt_ApplyFilter;
	nLines = sizeof(ui_txt_ApplyFilter) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = TestVol.NSSwitch;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			if(Index == 0)
			{
				TestVol.NSSwitch = FALSE;
			}
			else if(Index == 1)
			{
				TestVol.NSSwitch = TRUE;
			}
			voc_state_init_aud_process();
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

const char ui_txt_ECmode[5][NUM_LCD_COLUMNS]={
	"\205\241 OFF",
	"\205\242 ESEC",
	"\205\243 headset",
	"\205\244 AEC ",
	"\205\244 SPEAKER "
};

MENU_STATE_T 	ui_fn_controlEC(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_ContNSEC[1];

	MenuText = (char*)ui_txt_ECmode;
	nLines = sizeof(ui_txt_ECmode) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
//		if(!TestVol.ECSwitch)
//			Index = 0;
//		else
//			TestVol.ECmode
//		else
		Index = TestVol.ECmode;

	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case DIGIT5_K:
		case FUNCTION_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
			TestVol.ECmode = (byte)Index;
			voc_state_init_aud_process();
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;		// woodstok 00.4.27 
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);

}

/****************************************************************************

	ui_fn_LoadCurrentSetup() :현재값을 Load
****************************************************************************/


MENU_STATE_T 	ui_fn_LoadCurrentSetup(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
		kj_ClearScreen();
		kj_SetTextYCenterAlign(0, "현재의테스트");
		kj_SetTextYCenterAlign(1, "오디오값을적용");
		kj_SetTextYCenterAlign(2, "하시겠습니까");
		kj_TextOutY(4, "       \x88\xbf\205\276\205\277\205\274\205\275\6");

		// 2sec Timer
		ui_menutimer_set( ( int4 )60000 ); 
		break;

	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			kj_ClearScreen();
			kj_SetTextYCenterAlign(1, "현재값을");
			kj_SetTextYCenterAlign(2, "테스트오디오에");
			kj_TextOutYCenterAlign(3, "적용하였습니다");
			
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
			Get_cur_audio_value(&TestVol);
//			ui_put_nv( NV_AUDIO_PARAM_I ,( nv_item_type * )&TestVol);
			ui_TimeOutReason = UI_SELECT_COMP_DISP;
			kj_TimerStart(KJ_TM_F_DISP,KJ_T_2S);
			break;
		}
		break;
	case TIMEOUT_S:
		if(ui_TimeOutReason == UI_SELECT_COMP_DISP){
			kj_ClearScreen();
			ui_CurMenu=(MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if (ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}


/****************************************************************************

	ui_fn_LoadDefaultSetup() :기본값으로 초기화함

****************************************************************************/


MENU_STATE_T 	ui_fn_LoadDefaultSetup(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
		kj_ClearScreen();
		kj_SetTextYCenterAlign(0, "메모리에서 ");
		kj_SetTextYCenterAlign(1, "오디오값을");
		kj_SetTextYCenterAlign(2, "다시읽겠습니까");
		kj_TextOutYCenterAlign(4, "        \x88\xbf\205\276\205\277\205\274\205\275\6");

		// 2sec Timer
		ui_menutimer_set( ( int4 )60000 ); 
		break;

	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
			kj_ClearScreen();
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
			kj_SetTextYCenterAlign(1, "테스트오디오");
			kj_SetTextYCenterAlign(2, "값을 load");
			kj_TextOutYCenterAlign(3, "하였습니다");

//			ui_get_nv( NV_AUDIO_PARAM_I ,( nv_item_type * )&TestVol);
			ui_TimeOutReason = UI_SELECT_COMP_DISP;
			kj_TimerStart(KJ_TM_F_DISP,KJ_T_2S);
			break;
		}
		break;
	case TIMEOUT_S:
		if(ui_TimeOutReason == UI_SELECT_COMP_DISP){
			kj_ClearScreen();
			ui_CurMenu=(MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if (ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}


/****************************************************************************

	ui_fn_SaveCurrentSetup() :현재값을 저장

****************************************************************************/


MENU_STATE_T 	ui_fn_SaveCurrentSetup(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index;
	MENU_STATE_T		NewSate;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
		kj_ClearScreen();
		kj_SetTextYCenterAlign(0, "현재의테스트");
		kj_SetTextYCenterAlign(1, "오디오값을저장");
		kj_SetTextYCenterAlign(2, "하시겠습니까");
		kj_TextOutY(4, "       \x88\xbf\205\276\205\277\205\274\205\275\6");

		// 2sec Timer
		ui_menutimer_set( ( int4 )60000 ); 
		break;

	case KEYIN_S:
		switch (keyin){
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;
			NewSate = MENU_EXIT_S;
			break;
		case FUNCTION_K:
/*????????????????????????????????????????????????????????????????????????????
			테스트 값을 모두 적용할 수 있는 함수를 하나 만들자
????????????????????????????????????????????????????????????????????????????*/
			kj_ClearScreen();
			kj_SetTextYCenterAlign(1, "테스트오디오");
			kj_SetTextYCenterAlign(2, "값을 저장");
			kj_TextOutYCenterAlign(3, "하였습니다");

//			ui_put_nv( NV_AUDIO_PARAM_I ,( nv_item_type * )&TestVol);
			ui_TimeOutReason = UI_SELECT_COMP_DISP;
			kj_TimerStart(KJ_TM_F_DISP,KJ_T_2S);
			break;
        } 
		break;
	case TIMEOUT_S:
		if(ui_TimeOutReason == UI_SELECT_COMP_DISP){
			kj_ClearScreen();
			ui_CurMenu=(MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if (ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

#ifdef YAMAHA_MELODY // Melody chip쓰는 모델만

const char ui_txt_NewMelodyTest[83][NUM_LCD_COLUMNS]={
	"\205\241페르귄트-아침",
	"\205\242봄의 노래",
	"\205\243공주는잠못이루",
	"\205\244바이올린로망스",
	"\205\245비창소나타",
	"\205\246사계중가을",
	"\205\247개선행진곡",
	"\205\250비창교향곡",
	"\205\251성조기여영원하",
	"\205\252자유의종",
	"\205\253워싱턴 포스트",
	"\205\254무반주첼로조곡",
	"\205\255이고르공",
	"\205\256위풍당당행진곡",
	"\205\257가브리엘",
	"\205\260파리의 미국인",
	"\205\261자이언트",
	"\205\262바람과함께",
	"\205\263군밤타령",
	"\205\300트럼펫 협주곡",
	"\205\301사브레댄스",
	"\205\302카발레리아",
	"\205\303결혼행진곡",
	"\205\304멘델스존바올린",
	"\205\305크사르다스",
	"\205\306야상곡",
	"\205\307캉캉",
	"\205\310피아노",
	"\205\311아랑페즈",
	"\205\312로망스",
	"\205\313백조",
	"\205\314짐노페디",
	"\205\315핀란디아",
	"\205\316슬픈왈츠",
	"\205\317바르샤바",
	"\205\320잘살아보세",
	"\205\321금혼식",
	"\205\322축하합니다",
	"\205\323귀여운꼬마",
	"\205\324나란히",
	"\205\325우산",
	"\205\326황색리본",
	"\205\327숲의 요들",
	"\205\330몽금포 타령",
	"\205\331밀양 아리랑",
	"\205\332풍년가",
	"\205\333딱다구리",
	"\205\334고요한 밤",
	"\205\335징글벨",
	"\205\336기쁘다구주오셨",	// woostok 00.4.27 
	"\205\337축하하오기쁜성",	// Array Size에 맞춰서 자름
	"\205\340바하미뉴엣",
	"\205\341복케리니미뉴엣",
	"\205\342요술피리변주",
	"\205\343캐논",
	"\205\344즉흥 환상곡",
	"\205\345숭어",
	"\205\346유모레스크",
	"\205\347꿈길에서",
	"\205\350꽃의 왈츠",
	"\205\351flee as a bird",
	"\205\352투우사의 노래",
	"\205\353사계중 봄",
	"\205\354터키 행진곡",
	"\205\355백조의 호수",
	"\205\356윌리엄텔 서곡",
	"\205\357장난감 교향곡",
	"\205\360푸니쿨리푸니쿨",	// woodstok 00.4.27
	"\205\361하이포파이포",
	"\205\362쇼팽의 왈츠",
	"\205\363아라베스크",
	"\205\364도레미송",
	"\205\365엘리제를위하여",	// woodstok 00.4.27
	"\205\366병아리의 춤",
	"\205\367비창소나타",
	"\205\370라데츠키행진곡",
	"\205\371도나우강 왈츠1",
	"\205\372도나우강 왈츠2",
	"\205\373봄의 소리",
	"\205\374남국의 장미",
	"\205\375알함브라궁전",
	"\205\376프랑스조곡2번",
	"\205\377바하 미뉴엣"

};


MENU_STATE_T	New_Melody_play(MENU_STATE_T state, UB keyin)
{

	static char	Cursor, Index;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_AudioTest[3] ;

	MenuText = (char*)ui_txt_NewMelodyTest;
	nLines = sizeof(ui_txt_NewMelodyTest) / NUM_LCD_COLUMNS;

	NewSate=state;

	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		Test_sound_on = TRUE;

		NewSate=KEYIN_S;
		Cursor = 0;
		Index = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case DIGIT5_K:
		case DIGIT6_K:
		case DIGIT7_K:
		case DIGIT8_K:
		case DIGIT9_K:
		case DIGIT0_K:
			Index = keyin - DIGIT1_K;
			Cursor = 0;
		case FUNCTION_K:
			switch(Index)
			{
				case 0:
					uisnd_snd( UI_MUSIC_SND, MUS_PEERGYNT);
					break;
				case 1:
					uisnd_snd( UI_MUSIC_SND, MUS_SONGSPRING);
					break;
				case 2:
					uisnd_snd( UI_MUSIC_SND, MUS_TURANDOT);
					break;
				case 3:
					uisnd_snd( UI_MUSIC_SND, MUS_VIOLINROMANCE);
					break;
				case 4:
					uisnd_snd( UI_MUSIC_SND, MUS_PATHETIQUE);
					break;
				case 5:
					uisnd_snd( UI_MUSIC_SND, MUS_FALL);
  					break;
				case 6:
					uisnd_snd( UI_MUSIC_SND, MUS_AIDA);
  					break;
				case 7:
					uisnd_snd( UI_MUSIC_SND, MUS_PATHETIQUE_SYM);
  					break;
				case 8:

					uisnd_snd( UI_MUSIC_SND, MUS_SOUSA1);
  					break;
				case 9:

					uisnd_snd( UI_MUSIC_SND, MUS_SOUSA2);
  					break;
				case 10:

					uisnd_snd( UI_MUSIC_SND, MUS_SOUSA3);
  					break;
				case 11:

					uisnd_snd( UI_MUSIC_SND, MUS_1007);
  					break;
				case 12:

					uisnd_snd( UI_MUSIC_SND, MUS_IGOR);
  					break;
				case 13:

					uisnd_snd( UI_MUSIC_SND, MUS_MARCH);
  					break;
				case 14:

					uisnd_snd( UI_MUSIC_SND, MUS_STRANGER);
  					break;
				case 15:

					uisnd_snd( UI_MUSIC_SND, MUS_AMEPARIS);
  					break;
				case 16:

					uisnd_snd( UI_MUSIC_SND, MUS_GIANT);
  					break;
				case 17:

					uisnd_snd( UI_MUSIC_SND, MUS_GONEWIND);
  					break;
				case 18:

					uisnd_snd( UI_MUSIC_SND, MUS_GUMBAM);
  					break;
				case 19:

					uisnd_snd( UI_MUSIC_SND, MUS_TRUMPET);
  					break;
				case 20:

					uisnd_snd( UI_MUSIC_SND, MUS_SABREDANCE);
  					break;
				case 21:

					uisnd_snd( UI_MUSIC_SND, MUS_MASCAGNI);
  					break;
				case 22:

					uisnd_snd( UI_MUSIC_SND, MUS_WEDD);
  					break;
				case 23:

					uisnd_snd( UI_MUSIC_SND, MUS_VIOLINC);
  					break;
				case 24:

					uisnd_snd( UI_MUSIC_SND, MUS_CZARDAS);
  					break;
				case 25:

					uisnd_snd( UI_MUSIC_SND, MUS_NOCTURN);
  					break;
				case 26:

					uisnd_snd( UI_MUSIC_SND, MUS_CANCAN);
  					break;
				case 27:

					uisnd_snd( UI_MUSIC_SND, MUS_PIANOC);
  					break;
				case 28:
					uisnd_snd( UI_MUSIC_SND, MUS_ARANPEZ);
  					break;
				case 29:
					uisnd_snd( UI_MUSIC_SND, MUS_ROMANCE);
  					break;
				case 30:
				  uisnd_snd( UI_MUSIC_SND, MUS_CYNGNE);
  					break;
				case 31:
					uisnd_snd( UI_MUSIC_SND, MUS_GYMNOPEDIES);
  					break;
				case 32:
					uisnd_snd( UI_MUSIC_SND, MUS_SIBELIUS1);
  					break;
				case 33:
					uisnd_snd( UI_MUSIC_SND, MUS_SIBELIUS2);
					break;
				case 34:
					uisnd_snd( UI_MUSIC_SND, MUS_WARSAW);
					break;
				case 35:
					uisnd_snd( UI_MUSIC_SND, MUS_NEWTOWN);
					break;
				case 36:

					uisnd_snd( UI_MUSIC_SND, MUS_GOLDEN);
					break;
				case 37:

					uisnd_snd( UI_MUSIC_SND, MUS_CONGRATULATION);
					break;
				case 38:

					uisnd_snd( UI_MUSIC_SND, MUS_CUTEBOY);
					break;
				case 39:

					uisnd_snd( UI_MUSIC_SND, MUS_NARANHI);
					break;
				case 40:

					uisnd_snd( UI_MUSIC_SND, MUS_UMBRELLA);
					break;
				case 41:

					uisnd_snd( UI_MUSIC_SND, MUS_YELLOWRIBBON);
					break;
				case 42:

					uisnd_snd( UI_MUSIC_SND, MUS_YODDLE);
					break;
				case 43:

					uisnd_snd( UI_MUSIC_SND, MUS_MONGGUMPO);
					break;
				case 44:
					uisnd_snd( UI_MUSIC_SND, MUS_MILYANG);
					break;
				case 45:
					uisnd_snd( UI_MUSIC_SND, MUS_HARVEST);
					break;
				case 46:
					uisnd_snd( UI_MUSIC_SND, MUS_GUDDY);
					break;
				case 47:
					uisnd_snd( UI_MUSIC_SND, MUS_SILENTNIGHT);
					break;
				case 48:
					uisnd_snd( UI_MUSIC_SND, MUS_JINGLEBELL);
					break;
				case 49:
					uisnd_snd( UI_MUSIC_SND, MUS_JOY);
					break;
				case 50:
					uisnd_snd( UI_MUSIC_SND, MUS_MERRYXMAS);
					break;
				case 51:
					uisnd_snd( UI_MUSIC_SND, MUS_MENUET);
					break;
				case 52:
					uisnd_snd( UI_MUSIC_SND, MUS_JOYLOVE);
					break;
				case 53:
					uisnd_snd( UI_MUSIC_SND, MUS_MAGICFLUTE);
					break;
				case 54:
					uisnd_snd( UI_MUSIC_SND, MUS_CANON);
					break;
				case 55:
					uisnd_snd( UI_MUSIC_SND, MUS_FANTASY);
					break;
				case 56:
					uisnd_snd( UI_MUSIC_SND, MUS_TROUT);
					break;
				case 57:
					uisnd_snd( UI_MUSIC_SND, MUS_HUMORESQUE);
					break;
				case 58:
					uisnd_snd( UI_MUSIC_SND, MUS_DREAM);
					break;
				case 59:
					uisnd_snd( UI_MUSIC_SND, MUS_FLOWER);
					break;
				case 60:
					uisnd_snd( UI_MUSIC_SND, MUS_BIRD);
					break;
				case 61:
					uisnd_snd( UI_MUSIC_SND, MUS_TOREDOR);
					break;
				case 62:
					uisnd_snd( UI_MUSIC_SND, MUS_SPRING2);
					break;
				case 63:
					uisnd_snd( UI_MUSIC_SND, MUS_TURKEY);
					break;
				case 64:
					uisnd_snd( UI_MUSIC_SND, MUS_SWANLAKE);
					break;
				case 65:
					uisnd_snd( UI_MUSIC_SND, MUS_WILLIAMTELL);
					break;
				case 66:
					uisnd_snd( UI_MUSIC_SND, MUS_TOY);
					break;
				case 67:
					uisnd_snd( UI_MUSIC_SND, MUS_FUNIKULA);
					break;
				case 68:
					uisnd_snd( UI_MUSIC_SND, MUS_HYPOPIPO);
					break;
				case 69:
					uisnd_snd( UI_MUSIC_SND, MUS_WALTZ);
					break;
				case 70:
					uisnd_snd( UI_MUSIC_SND, MUS_ARABESQUE);
					break;
				case 71:
					uisnd_snd( UI_MUSIC_SND, MUS_DOREMI);
					break;
				case 72:
					uisnd_snd( UI_MUSIC_SND, MUS_FORELISE);
					break;
				case 73:
					uisnd_snd( UI_MUSIC_SND, MUS_CHICKY);
					break;
				case 74:
					uisnd_snd( UI_MUSIC_SND, MUS_PATHETIQUE_SONATA);
					break;
				case 75:
					uisnd_snd( UI_MUSIC_SND, MUS_LADETZKY);
					break;
				case 76:
					uisnd_snd( UI_MUSIC_SND, MUS_DONAU1);
					break;
				case 77:
					uisnd_snd( UI_MUSIC_SND, MUS_DONAU2);
					break;
				case 78:
					uisnd_snd( UI_MUSIC_SND, MUS_SOUNDSPRING);
					break;
				case 79:
					uisnd_snd( UI_MUSIC_SND, MUS_ROSE);
					break;
				case 80:
					uisnd_snd( UI_MUSIC_SND, MUS_ALHAMBRA);
					break;
				case 81:
					uisnd_snd( UI_MUSIC_SND, MUS_FRANCE2);
					break;
				case 82:
					uisnd_snd( UI_MUSIC_SND, MUS_BACHMENUET);
					break;

			}
			break;
		case CLR_K:
		case MEMORY_K:
			uisnd_music_stop();

			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:

			uisnd_music_stop();
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;

			NewSate = MENU_EXIT_S;
			break;
		case UP_K:
			if(Index == 0)	Index = nLines - 1;
			else			Index--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;
		case DOWN_K:
			if(Index >= nLines - 1) Index = 0;
			else					Index++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index);
			break;		
		}
		break;
	case INTERRUPT_S:
		uisnd_music_stop();
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}

#endif
#ifdef AUD_DEBUG
const char audio_fn_execute_txt[11][NUM_LCD_COLUMNS]={
	"\205\241VacquireCD",
	"\205\242VreleaseCD",
	"\205\243SndVocAcq",
	"\205\244VoicePlay 1",
	"\205\245VoicePlay 2",
	"\205\246VoicePlay 3",
	"\205\247VoicePlay 4",
	"\205\250VoicePlay 5",
	"\205\251VoicePlay 6",
	"\205\252VoicePlay 7",
	"\205\253VoiceStop"
};

extern snd_pstate_type       snd_crnt_path_out;
extern snd_pstate_type snd_voice_path_out;
extern void sndhw_which_codec
(
  snd_pstate_type *path
    /* Which Audio Path to Setup */
);

extern void sndhw_voc_acquire( void );

MENU_STATE_T	audio_fn_execute(MENU_STATE_T state, UB keyin)
{
	static char	Cursor, Index_s;
	MENU_STATE_T		NewSate;
	char	*MenuText, nLines;
	const char	*TitleText = ui_txt_AudioTest[AUDIO_MENU_NUM] ;

	MenuText = (char*)audio_fn_execute_txt;
	nLines = sizeof(audio_fn_execute_txt) / NUM_LCD_COLUMNS;

	NewSate=state;
	switch (state){
	case ENTRANCE_S:
		/* Initialize and display menu text */
		NewSate=KEYIN_S;
		Cursor = 0;
		Index_s = 0;
	case POPUP_S:
		ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index_s);
		break; 
	case KEYIN_S:
		switch (keyin){
		case DIGIT1_K:
		case DIGIT2_K:
		case DIGIT3_K:
		case DIGIT4_K:
		case DIGIT5_K:
		case DIGIT6_K:
		case DIGIT7_K:
		case DIGIT8_K:
		case DIGIT9_K:
		case DIGIT0_K:
			Index_s = keyin - DIGIT1_K;
			Cursor = 0;
		case FUNCTION_K:
			switch(Index_s)
			{
				case 0:
					voc_acquire(VOC_ACQ_CDMA, NULL);
//					voc_state_enable_clocks();
//						sndhw_which_codec(&snd_crnt_path_out);
//					Play_rec_init(VOC_CAP_EVRC);
//					uisnd_set_device(SND_MUTE_UNMUTED,SND_MUTE_MUTED, TRUE);
//					snd_set_device(SND_DEVICE_HFK,SND_MUTE_UNMUTED
//						,SND_MUTE_UNMUTED,FALSE, NULL, NULL);
					break;
				case 1:
					voc_release(VOC_ACQ_CDMA);
//						sndhw_which_codec(&snd_voice_path_out);
//					PLAY_voice_start(Ment1, 19274);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_UNMUTED
//						,SND_MUTE_UNMUTED,FALSE, NULL, NULL);
					break;
				case 2:
							sndhw_voc_acquire();      /* Acquire the Vocoder (if not already) */
//					snd_set_device(SND_DEVICE_HFK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
//							voc_vol_cmd(VOC_DBM_MIN);
//							voc_codec_gain(0,0,VOC_DBM_MIN,VOC_DBM_MIN);
//						PLAY_voice_start(Ment2, 14346);
////					voc_audio_loop_cmd(TRUE);
//					voc_acquire(VOC_ACQ_SND, NULL);
					break;
				case 3:
					uisnd_voice(Ment1, 5619,1);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
//										PLAY_voice_start(Ment3, 22287);
//					voc_release(VOC_ACQ_SND);	// woodstok 00.4.27 Param수 맞춤
////					voc_release(VOC_ACQ_SND, NULL);
//					   sndhw_set_volume();

//					voc_config(VOC_ACQ_SND, VOC_CAP_IS127);
					break;
				case 4:
					uisnd_voice(Ment2, 4215,1);
#ifdef YAMAHA_MELODY
					//MUSIC_power_on(MUS_SPK, TRUE,TRUE);
#endif
//					voc_config(VOC_ACQ_SND, VOC_CAP_EVRC ) ;
//							sndhw_voc_release();      /* Acquire the Vocoder (if not already) */
//					voc_tx_mute_cmd(FALSE);
//					snd_set_device(SND_DEVICE_HFK,SND_MUTE_UNMUTED
//						,SND_MUTE_UNMUTED,FALSE, NULL, NULL);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
//
////										PLAY_voice_start(Ment4, 9802);
 // 				    voc_acquire(VOC_ACQ_SND,sndhw_voc_change);
////					voc_audio_loop_cmd(FALSE);
//					voc_config(VOC_ACQ_CDMA, VOC_CAP_IS127);
					break;
				case 5:
					uisnd_voice(Ment3, 4428,1);
//						uisnd_set_device(SND_MUTE_UNMUTED,SND_MUTE_UNMUTED, TRUE);
//					voc_rx_mute_cmd(FALSE);
//					snd_set_device(SND_DEVICE_HFK,SND_MUTE_UNMUTED
//						,SND_MUTE_UNMUTED,FALSE, NULL, NULL);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_UNMUTED
//						,SND_MUTE_UNMUTED,FALSE, NULL, NULL);
//
////										PLAY_voice_start(Ment5, 17571);
//					voc_dtmf_cmd (880,880, -500, 300);
////					  uisnd_snd(UI_CALL_EAR_SND, SND_DIAL_TONE);
//					uisnd_play_file("시험",0);
//					voc_audio_loop_cmd(TRUE);
					break;
				case 6:
					uisnd_voice(Ment4, 7647,1);
//						uisnd_set_device(SND_MUTE_MUTED,SND_MUTE_MUTED, TRUE);
//					snd_set_device(SND_DEVICE_HFK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);

//					PLAY_voice_start(Ment6, 15441);
//					 UISND_ERR_TONE();
//					uisnd_snd_stop();
//					uisnd_music_stop();
//					Play_rec_init(VOC_CAP_EVRC);
					break;
				case 7:
					uisnd_voice(Ment5, 3575,1);
//					voc_release(VOC_ACQ_SND);
///					sndhw_which_codec(&snd_tone_path_out);
//					voc_pcm_loop_cmd(TRUE);
//					snd_set_device(SND_DEVICE_HANDSET,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
//							voc_vol_cmd(VOC_DBM_MIN);
//							voc_codec_gain(0,0,VOC_DBM_MIN,VOC_DBM_MIN);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
//
////					snd_rx_mute_ctl( SND_MUTE_SNDV, TRUE );
//					uisnd_voice(Ment1, Ment_length[0], 0 );
//					Play_rec_init(VOC_CAP_EVRC);
//					PLAY_voice_start(Ment7, 17382);
					break;
				case 8:
//					uisnd_voice(Ment, 3346,1);
					uisnd_voice(Ment6, 19804,1);
//						voc_pcm_loop_cmd(FALSE);
//					snd_set_device(SND_DEVICE_HANDSET,SND_MUTE_UNMUTED
//						,SND_MUTE_UNMUTED,FALSE, NULL, NULL);
//							voc_vol_cmd(VOC_DBM_MIN);
//							voc_codec_gain(0,0,VOC_DBM_MIN,VOC_DBM_MIN);
//					snd_set_device(SND_DEVICE_LOUDSPK,SND_MUTE_MUTED
//						,SND_MUTE_MUTED,FALSE, NULL, NULL);
////					snd_rx_mute_ctl( SND_MUTE_SNDV, FALSE);
////					uisnd_voice_stop();
//					Play_rec_init(VOC_CAP_EVRC);
//					uisnd_voice(AnsMent1_EVRC, 7475, 1);
//					PLAY_voice_start(Ment8, 20813);

					break;
				case 9:
//					voc_audio_loop_cmd(TRUE);
//					Play_rec_init(VOC_CAP_EVRC);
					uisnd_voice(Ment7, 3384,1);
//					 Release_play_rec();
//#ifdef YAMAHA_MELODY
//					snd_music_buf_start(    SND_DEVICE_LOUDSPK   ,SND_METHOD_MUSIC,
//					(word *)anpan , 0, SND_MELODY_NORMAL, NULL, NULL);	// woodstok 00.4.27 cast
//#endif

//					PLAY_voice_start(Ment1, Ment_length[0]);
					break;
				case 10:
//					voc_audio_loop_cmd(FALSE);
//						Play_rec_init(VOC_CAP_EVRC);
//						PLAY_voice_start(Ment8, 20813);
					uisnd_voice_stop();
//#ifdef YAMAHA_MELODY
//					uisnd_sample_snd(    UI_MUSIC_SND   , MUS_ANPAN,
//					0x9F, 0);
//#endif
//					Play_rec_init(VOC_CAP_EVRC);
//					PLAY_voice_start(Ment9, Ment_length[0]);
					break;

			}
			break;
		case CLR_K:
		case MEMORY_K:
			ui_CurMenu = (MENU_STATE_T (*)(MENU_STATE_T state, UB keyin))ui_PopMenu();
			if(ui_CurMenu) (*ui_CurMenu)(POPUP_S, 0);
			break;
		case END_K:
			ui.pad = FALSE;
			ui.menuvol =FALSE;
//			Test_sound_on = FALSE;
			ui.beep_updown = TRUE;
			ui.multi_key = TRUE;

			NewSate = MENU_EXIT_S;
			break;
		case UP_K:
			if(Index_s == 0)	Index_s = nLines - 1;
			else			Index_s--;
			if(Cursor > 0)	Cursor--;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index_s);
			break;
		case DOWN_K:
			if(Index_s>= nLines - 1) Index_s= 0;
			else					Index_s++;
			if(Cursor < NUM_LCD_LINES - 3) Cursor++;
			ui_ScrollDisplay2(MenuText, nLines, NUM_LCD_COLUMNS, (char*)TitleText, (char*)ui_CommandLine[UI_CL_SELECT_0], Cursor, Index_s);
			break;		
		}
		break;
	case INTERRUPT_S:
		ui.pad = FALSE;
		ui.menuvol =FALSE;
//		Test_sound_on = FALSE;
		ui.beep_updown = TRUE;
		ui.multi_key = TRUE;

		NewSate = MENU_EXIT_S;
		break;

	default:
		break;
	}
	return(NewSate);
}
#endif