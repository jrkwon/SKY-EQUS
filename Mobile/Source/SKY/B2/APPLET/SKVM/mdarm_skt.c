/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
		MDARM_SKT.C
GENERAL DESCRIPTION
  This Software unit contains all SK Telecom service functions for XVM engine.
  This includes all REX Interfacing functions and XVM engine Interfacing functions.
  
EXTERNALIZED FUNCTIONS
	
INITIALIZATION AND SEQUENCING REQUIREMENTS 

   Copyright (c) 2001 by SK Teletech Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/******************************************************************/
/*	   include Header Files				                          */
/******************************************************************/

#include "graphics.h"
#include "extern.h" 
#include <string.h>
#include "sky.h"
#include "mdarm_def.h"
#include "snd.h"
#include "YMU759.h"
#ifdef FEATURE_GVM
#include "..\GVM\SWAP\gvm_config.h"
#endif

/******************************************************************/
/*	   define Constans and Macro		                          */
/******************************************************************/

/******************************************************************/
/*	   grobal variables					                          */
/******************************************************************/
// SKVM에서 보내는 SMS종류를 구분하고자 TI값을 두었음.
UINT16  nTIValue;
/******************************************************************/
/*	   static variables					                          */
/******************************************************************/

/******************************************************************/
/*	   extern variables					                          */
/******************************************************************/

/******************************************************************/
/*	   extern functions					                          */
/******************************************************************/
extern void xvm_signal_sound_play_end(int ret);
// PHONEBOOK TEST를 위해 문자열을 찍느라고 잠시 EXTERN했었음..
// 이건 나중에 지우자...
extern void    CLNTa_log (unsigned char viewId, signed short logNo, const char *format, ...);
/******************************************************************/
/*     function Declare											  */
/******************************************************************/

/******************************************************************/
/*     function Define											  */
/******************************************************************/

/*******************************************************************
 *
 *	VIBRATION
 *
 *******************************************************************/

int md_vibration_is_supported(void)
{
	return 1;
}
void md_vibration_on(int level, int time)
{
	// 현재는 1단계의 진동만을 제공한다. 
	// 추후에 다단계 진동 제공시.. 변경 요망..  
	//SKY_MotorTime(time*1000);
    SKY_MotorTime( (UINT16)time );
}

void md_vibration_off(void)
{
	SKY_MotorStop();
}

int md_vibration_numoflevels(void)
{
	return 1;
}


/*******************************************************************
 *
 *	BACKLIGHT   
 *
 *******************************************************************/

int  md_backlight_get_numofcolors(void)
{
	return 1;
}

long md_backlight_get_color(int idx)
{
    return 0x00;
}

void md_backlight_on( int duration, int idx)
{
    if (duration == 0)
		SKY_BacklightOn(AB_ALWAYS);
	else SKY_BacklightPeriodOn(duration);
}

void md_backlight_off(void)
{
	SKY_BacklightOff();
}


/*******************************************************************
 *
 *	CALL  => 완료
 *
 *************************************************************/

// 1: support, 0: unsupport 
int md_call_is_supported(void)
{
    return 1;
}

int md_call_connect( char *number )
{
    UINT8  nLen;

    nLen = strlen(number);
    SET_NUMEDIT_BUFFER((BYTE*)number, nLen);
	SET_NUMEDIT_POS(nLen); 
#ifdef SKVM_IN_UI_TASK
    SKY_DoEvent(SE_SKVM_VOICECALL_CONNECT);
#else
    SKY_InitKey( &ui_tcb, UI_KEY_SIG );
    SKVMSendEvent2UI(UI_SKVM_VOICECALL_CONNECT);
#endif      
    // return 값에 대해서 명시되어 있지 않음
    return 0;
}
     

/*******************************************************************
 *
 *	SMS
 *
 *******************************************************************/

int md_sms_is_supported(void)
{
    // 1: support, 0: unsupport 
    MSG_ERROR("md_sms_is_supported called & returned",0,0,0);
    return 1;
}

/*
 * Tele-service ID 64100 SMS JAVA DOWNLOAD NOTIFICATION
 * Tele-service ID 64101 SMS JAVA APPLICATION DATA
 */
/*************************************************************
 * NAME
 *	md_sms_send_shortmessage
 *
 * DESCRIPTION
 *	send data(SHORT MESSAGE) to receiverNun with senderNum.
 *	if senderNum is NULL, senderNum is my default phone number.
 *
 * RETURN VALUE
 *	On success, zero is returned. On error, -1  is  returned.
 */
int md_sms_send_shortmessage( char *receiverNum,
	unsigned char *msg, int msgLen, char *senderNum )
{
    MSG_ERROR("md_sms_send_shortmessage called",0,0,0);

    memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
    
    memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
    STRCPY(g_UIMOBuffer.pszRcvNumber, (BYTE*)receiverNum);
    g_UIMOBuffer.nRcvNumberSize = 0;
    g_UIMOBuffer.nRcvNumberSize = STRLEN(g_UIMOBuffer.pszRcvNumber);
    g_UIMOBuffer.pszRcvNumber[g_UIMOBuffer.nRcvNumberSize] = NULL;

    // my party number 
    memset(g_UIMOBuffer.pszSendNumber, NULL, MAX_SMS_DIAL + 1);
    if (STRLEN((unsigned char *)senderNum) == 0)
		STRCPY(g_UIMOBuffer.pszSendNumber, GetMyNumber());                
	else
		STRCPY(g_UIMOBuffer.pszSendNumber, (BYTE*)senderNum);
    
    g_UIMOBuffer.nSendNumberSize = 0;
	g_UIMOBuffer.nSendNumberSize= STRLEN(g_UIMOBuffer.pszSendNumber); 
    g_UIMOBuffer.pszSendNumber[g_UIMOBuffer.nSendNumberSize] = NULL; 

    // msg 
    memset(g_UIMOBuffer.pszMsg, NULL, MAX_SMS_DIAL + 1);
    g_UIMOBuffer.nMsgSize = 0;
    g_UIMOBuffer.nMsgSize = msgLen;
	STRCPY(g_UIMOBuffer.pszMsg, (BYTE*)msg);
	g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;

    // priority 
    g_UIMOBuffer.nPriority = UASMS_PRIORITY_NORMAL; 

    nTIValue = TI_DEFAULT;
#ifdef SKVM_IN_UI_TASK
    SKY_DoEvent(SE_SKVM_SMS_SEND);
#else
    SKY_InitKey( &ui_tcb, UI_KEY_SIG );
    SKVMSendEvent2UI(UI_SKVM_SMS_SEND);
#endif  
    MSG_ERROR("md_sms_send_shortmessage returned",0,0,0);
	return 0;
}

/*************************************************************
 * NAME
 *	md_sms_send_downloadnotification
 *
 * DESCRIPTION
 *	send JAVA DOWNLOAD NOTIFICATION
 *	msgLen == 80
 *
 * RETURN VALUE
 *	On success, zero is returned. On error, -1  is  returned.
 */
int md_sms_send_downloadnotification(char *receiverNum,
	unsigned char *msg, int msgLen)
{
    MSG_ERROR("md_sms_send_downloadnotification called",0,0,0);

	memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
    
    memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
    STRCPY(g_UIMOBuffer.pszRcvNumber, (BYTE*)receiverNum);
    g_UIMOBuffer.nRcvNumberSize = 0;
    g_UIMOBuffer.nRcvNumberSize = STRLEN(g_UIMOBuffer.pszRcvNumber);
    g_UIMOBuffer.pszRcvNumber[g_UIMOBuffer.nRcvNumberSize] = NULL;

    // my party number 
    memset(g_UIMOBuffer.pszSendNumber, NULL, MAX_SMS_DIAL + 1);
    STRCPY(g_UIMOBuffer.pszSendNumber, GetMyNumber());                
	    
    g_UIMOBuffer.nSendNumberSize = 0;
	g_UIMOBuffer.nSendNumberSize= STRLEN(g_UIMOBuffer.pszSendNumber); 
    g_UIMOBuffer.pszSendNumber[g_UIMOBuffer.nSendNumberSize] = NULL; 

    // msg 
    memset(g_UIMOBuffer.pszMsg, NULL, MAX_SMS_DIAL + 1);
    g_UIMOBuffer.nMsgSize = 0;
    g_UIMOBuffer.nMsgSize = msgLen;
	STRCPY(g_UIMOBuffer.pszMsg, (BYTE*)msg);
	g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;

    // priority 
    g_UIMOBuffer.nPriority = UASMS_PRIORITY_NORMAL; 

    nTIValue = TI_LOAD;
#ifdef SKVM_IN_UI_TASK
    SKY_DoEvent(SE_SKVM_SMS_SEND);
#else
    SKY_InitKey( &ui_tcb, UI_KEY_SIG );
    SKVMSendEvent2UI(UI_SKVM_SMS_SEND);
#endif  
    MSG_ERROR("md_sms_send_downloadnotification returned",0,0,0);
	return 0;
}

/*************************************************************
 * NAME
 *	md_sms_send_applicationdata
 *
 * DESCRIPTION
 *	send JAVA APPLICATION DATA
 *	msgLen == 80
 *
 * RETURN VALUE
 *	On success, zero is returned. On error, -1  is  returned.
 */
int md_sms_send_applicationdata(char *receiverNum,
	unsigned char *msg, int msgLen)
{
    MSG_ERROR("md_sms_send_applicationdata called",0,0,0);

	memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
    
    memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
    STRCPY(g_UIMOBuffer.pszRcvNumber, (BYTE*)receiverNum);
    g_UIMOBuffer.nRcvNumberSize = 0;
    g_UIMOBuffer.nRcvNumberSize = STRLEN(g_UIMOBuffer.pszRcvNumber);
    g_UIMOBuffer.pszRcvNumber[g_UIMOBuffer.nRcvNumberSize] = NULL;

    // my party number 
    memset(g_UIMOBuffer.pszSendNumber, NULL, MAX_SMS_DIAL + 1);
    STRCPY(g_UIMOBuffer.pszSendNumber, GetMyNumber());                
	    
    g_UIMOBuffer.nSendNumberSize = 0;
	g_UIMOBuffer.nSendNumberSize= STRLEN(g_UIMOBuffer.pszSendNumber); 
    g_UIMOBuffer.pszSendNumber[g_UIMOBuffer.nSendNumberSize] = NULL; 

    // msg 
    memset(g_UIMOBuffer.pszMsg, NULL, MAX_SMS_DIAL + 1);
    g_UIMOBuffer.nMsgSize = 0;
    g_UIMOBuffer.nMsgSize = msgLen;
	STRCPY(g_UIMOBuffer.pszMsg, (BYTE*)msg);
	g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;

    // priority 
    g_UIMOBuffer.nPriority = UASMS_PRIORITY_NORMAL; 

    nTIValue = TI_DATA;
#ifdef SKVM_IN_UI_TASK
    SKY_DoEvent(SE_SKVM_SMS_SEND);
#else
    SKY_InitKey( &ui_tcb, UI_KEY_SIG );
    SKVMSendEvent2UI(UI_SKVM_SMS_SEND);
#endif
	return 0;
}

/*************************************************************
 * NAME
 *	md_set_menu_location
 *
 * DESCRIPTION
 *	set menu location (ex, /메뉴/게임, XGAME)
 *
 * RETURN VALUE
 *	On success, zero is returned. On error, -1  is  returned.
 */
//extern int md_set_menu_location(char *location, char *appName)
extern int md_set_menu_location(char *location, char *cmd)
{
    // FIXME: 주어진 location에 메뉴를 추가하고, 
    // FIXME: 선택시 xvm_setapp(appName)을 부르도록 한다. 
    // NOTE: 일단은 구현하지 말고 이대로 놔두십시오. 
    MSG_ERROR("md_set_menu_location called",0,0,0);
    MSG_ERROR("md_set_menu_location returned",0,0,0);
    return -1;
}

extern int md_remove_menu_location(char *location)
{
    // FIXME: 주어진 location에 메뉴를 제거한다. 
    // NOTE: 일단은 구현하지 말고 이대로 놔두십시오. 
    MSG_ERROR("md_remove_menu_location called",0,0,0);
    MSG_ERROR("md_remove_menu_location returned",0,0,0);
    return -1;
}

/*******************************************************************
 *
 *	SOUND
 *
 *******************************************************************/
/* SOUND와 관련된 정리.....   2001.08.09 bgjung

  B2는 공식적으로 buzzer는 지원하지 않는 것이라고 합니다.
   또한 VM에서처럼 ma2와 mmf가 이렇게 다른 타입으로 정의되어 있다면
     우리는 mmf를 지원한다고 하는 것이 맞는 말이라고 합니다.
   => 그리하여 지원하는 format을 mmf라고만 return 하도록 했음.
  
  XCE에서 기본적으로 EVRC와 Buzzer를 지원해야 한다고 하는군요.
  그리하여 사운드는 최종적으로 mmf와 EVRC, buzzer를 지원합니다.
   (또한 BEEP음도 play해야 한느군요)

  sound 출력음 크기로는 스피커 볼륨 크기가 아닌 키톤 볼륨 크기로 정함.
   //(SKY_GET_SPEAKER_VOLUME 대신에 SKY_GET_KEY_TONE_VOLUME으로....)
   (SKY_GET_SPEAKER_VOLUME 대신에 SKY_GET_GAME_VOLUME으로....)
   => 에티켓키를 누르면 키톤은 묵음이 되어서 게임소리를 안나게 할 수 있으니까..
*/

 /*
 * NAME
 *	md_sound_formates
 *
 * DESCRIPTION
 *	unsigned long md_sound_formats(void)
 * 	
 *  return the all sound formats using ORing on this terminal . 
 *
 * RETURN VALUE
 *	all defined usable sound formats on this terminal
 */
unsigned long md_sound_formats(void){
    // FIXME: 사용가능 소리종류를 ORING한다. 
    return SOUND_MMF | SOUND_BUZZER | SOUND_EVRC;
}

/*************************************************************
 * NAME
 *	md_sound_format_name
 *
 * DESCRIPTION
 *	int md_sound_format_name(unsigned long fid, char *name, int maxlen)
 * 	
 *  return the given sound's name of format. 
 *
 * RETURN VALUE
 *	in ther char *name : the sound format name
 *  return 0: on success;
 *  return -1 : on failure;
 all defined usable sound formats on this terminal
 */
int md_sound_format_name(unsigned long fid, char *name, int maxlen){
	switch(fid){
		case SOUND_BUZZER :
			strncpy(name, "buzzer",  maxlen);
			break;
		case SOUND_MA1:
			strncpy(name, "ma1",  maxlen);
			break;
		case SOUND_MA2:
			strncpy(name, "ma2",  maxlen);
			break;
		case SOUND_MMF:
			strncpy(name, "mmf",  maxlen);
			break;
		case SOUND_CMX:
			strncpy(name, "cmx",  maxlen);
			break;
		case SOUND_MIDI :
			strncpy(name, "midi",  maxlen);
			break;
		case SOUND_EVRC :
			strncpy(name, "evrc",  maxlen);
			break;
        case SOUND_WAVE :
            strncpy(name, "wave", maxlen);
            break;
		default : 
			return -1;
	}
	name[maxlen-1] = '\0';
	return 0;
}

int md_sound_is_valid_data(unsigned long type, unsigned char *data, int length)
{
    // FIXME: type에 valid한가를 검사한다. 
    // 적합하면 0, 그렇지 않으면 -1을 넘긴다. 
    static	UINT32 SmafSize1;
    static	void *pSmaf;
    static	int Status;		

    if (type == SOUND_MMF) {
        Smaf_Initialize();
	    MA2Manager_Initialize();
        pSmaf = 0;
        Status = 0;
        SmafSize1 = 100;
        pSmaf = Smaf_Load(pSmaf, data, SmafSize1, 0, &Status);
        if (pSmaf == 0)	{
            Smaf_Unload(pSmaf);
	        return -1;
        }
        else {
            Smaf_Unload(pSmaf);
            return 0;
        }
    }
    // 여기서 EVRC와 BUZZER의 경우는 mmf 형식으로 변환한 뒤에 
    // 데이터를 검사한다.
    else if (type == SOUND_BUZZER) {
        extern void ConvGVMBellData2SMAF(UINT8*, BYTE);
        extern BYTE mb_MyBellPreBuffer[MYMELODY_DATA_MAX];

        ConvGVMBellData2SMAF((BYTE *)data, length);

        Smaf_Initialize();
	    MA2Manager_Initialize();
        pSmaf = 0;
        Status = 0;
        SmafSize1 = 100;
        pSmaf = Smaf_Load(pSmaf, mb_MyBellPreBuffer, SmafSize1, 0, &Status);
        if (pSmaf == 0) {
            Smaf_Unload(pSmaf);
	        return -1;
        }
        else {
            Smaf_Unload(pSmaf);
            return 0;
        }
    }
    else if (type == SOUND_EVRC) {
        return 0;
    }
    else
        return -1;
}

/*************************************************************
 * NAME
 *	md_sound_play
 *
 * DESCRIPTION
 *	int md_sound_play(unsigned long type, unsigned char * data, const int length, int repeat)
 * 	
 *  play the sound the buffer data for a length and repeat is repeat count
 * 
 * RETURN VALUE
 *  return 0: on success;
 *  return -1 : on failure;
 */
int md_sound_play(unsigned long type, unsigned char *data, int length, int repeat )
{
    /*if (SKY_GET_KEY_TONE_VOLUME() == 0) {
		return 0;
    }*/
    if ((SKY_GET_GAME_VOLUME() == 0) || SKY_IS_ETIQUETTE()) {
        return 0;
    }

    // play routine
    if (type == SOUND_BUZZER) {
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE        
        extern void ConvGVMBellData2SMAF(UINT8*, BYTE);
        extern BYTE mb_MyBellPreBuffer[MYMELODY_DATA_MAX];

        SKY_StopMusic();
        ConvGVMBellData2SMAF((BYTE *)data, length); 
        //SKY_PlayMusicBufferEx((BYTE*)mb_MyBellPreBuffer, SKY_GET_KEY_TONE_VOLUME(), repeat);
        SKY_PlayMusicBufferEx((BYTE*)mb_MyBellPreBuffer, SKY_GET_GAME_VOLUME(), repeat+1);
        SKVM_SET_MUSIC_PLAY(TRUE);
#endif
        return 0;
    }
    else if (type == SOUND_EVRC) {
        SKY_StopVoice();
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
        SKY_PlayVoice((BYTE *)data, (WORD)length, (WORD)repeat+1);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
        SKY_PlayVoice((BYTE *)data, (UINT16)length, (UINT8)repeat+1);
#endif
        return 0;
    }
    else if (type == SOUND_MMF) {
	    SKY_StopMusic();
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE 
		//SKY_PlayMusicBufferEx((BYTE*)data, SKY_GET_KEY_TONE_VOLUME(), repeat);
        SKY_PlayMusicBufferEx((BYTE*)data, SKY_GET_GAME_VOLUME(), repeat+1);

#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
        //SKY_PlayMusicBufferEx((WORD*)data, SKY_GET_KEY_TONE_VOLUME());
        SKY_PlayMusicBufferEx((WORD*)data, SKY_GET_GAME_VOLUME());
#endif
		SKVM_SET_MUSIC_PLAY(TRUE);
		return 0;
	}
    else{
		return -1 ;
    }
}

/*************************************************************
* NAME
*	md_sound_stop
*
* DESCRIPTION
*	int md_sound_stop(void)
* 	
*  stop the playing sounds and then send the stop signal for XVM.
* 
* RETURN VALUE
*  return 0: on success;
*  return -1 : on failure;
*/
int md_sound_stop(void)
{
    // FIXME: type에 따라 연주를 종류한다 
    // FIXME: 종료시 xvm_signal_sound_play_end(retstate)가 불려야 한다 
    // FIXME: 주로 sound play의 callback에 넣어 준다 
    void xvm_signal_sound_play_end(int ret);
    // 음원 타입을 알 수 없어서 둘 다 종료시킴.
	SKY_StopMusic(); // mmf/buzzer
    SKY_StopVoice(); // EVRC
	xvm_signal_sound_play_end(0);
    return 0;
}

/*************************************************************
* NAME
*	md_sound_pause
*
* DESCRIPTION
*	int md_sound_pause(void)
* 	
*  pause the playing sounds and then send the stop signal for XVM.
* 
* RETURN VALUE
*  return 0: on success;
*  return -1 : on failure;
*/
int md_sound_pause(void)
{
    // 구현에서 제외됨.
    MSG_ERROR("md_sound_pause called & returned",0,0,0);
    return -1;
}

/*************************************************************
* NAME
*	md_sound_resume
*
* DESCRIPTION
*	int md_sound_resume(void)
* 	
*  resume the pausing sounds from the paused position.
* 
* RETURN VALUE
*  return 0: on success;
*  return -1 : on failure;
*/
int md_sound_resume(void)
{
    // 구현에서 제외됨.
    MSG_ERROR("md_sound_resume called & returned",0,0,0);
    return -1;
}

/*************************************************************
* NAME
*	md_sound_alert
 *************************************************************/
int md_sound_alert(int alerttype)
{ 
	switch(alerttype){
		case ALERT_ALARM: // alert_alarm
			SKY_PlayMusicEx(MUS_ERROR_9, 3, 1);
			break;
		case ALERT_CONFIRMATION: // alert_confirmation
			SKY_PlayMusicEx(MUS_DDING, 3, 1);
			break;
		case ALERT_ERROR: // alert_error
			SKY_PlayMusicEx(MUS_ERROR_6, 3, 1);
			break;
		case ALERT_INFO: // alert_info
			SKY_PlayMusicEx(MUS_ALLERT2, 3, 1);
			break;
		case ALERT_WARNING: // alert_warning
			SKY_PlayMusicEx(MUS_ETIQUETTE, 3, 1);
			break;
		default : 
            return -1;
	}
	return 0;
}

 void md_beep(int freq, int duration)
{
    // FIXME: freq(hertz)에 해당하는 비프음을 duration(msec)
    // 	      동안 내도록 한다.          
    // 여기서 걸리는 문제는....
    // 우리는 플레이할때 Hz를 넘겨주는 것이 아니라 해당 Hz를 매핑해놓은 ENUM을 쓴다.
    // 고로 인터페이스 필요.. (오정택선배에게 말했씀...)
    //SKY_PlayDTMF (요기서 걸림, (WORD)duration);     

	//youandi 01.08.24 오정택 씀.
	//아래feature를 살리면 소리가 납니다. 
	//제 휴가가 8월30일까지이니 31일에 feature를 살리겠습니다.

	#ifdef SND_PARA_SUPPORT
		static snd_type snds;
		snds.tone=SND_FREQ;
		snds.duration_ms=(word)duration;
		snds.freq.freq_hi=(word)(0xff00&&freq);
		snds.freq.freq_lo=(word)(0x00ff&&freq);
		SKY_PlayMultiTone( FALSE, UI_BEEP_SND, 0, &snds, MOD_KEY_BEEP, FALSE);
	#endif

}

int md_get_max_volume(int type)
{
    // FIXME: 단말기에서 지원되는 최대 볼륨의 인덱스를 리턴 
    if ((type == SOUND_MMF) || (type == SOUND_BUZZER) || (type == SOUND_EVRC)) 
        //return 5;
        return 4; // 0~4
    else 
        return -1;
}

int md_get_volume(int type)
{
    // FIXME: 현재 볼륨의 인덱스를 리턴 
    if ((type == SOUND_MMF) || (type == SOUND_BUZZER) || (type == SOUND_EVRC)) 
        //return SKY_GET_KEY_TONE_VOLUME();
        return SKY_GET_GAME_VOLUME();
    else 
        return -1;
}

void md_set_volume(int type, int vol)
{
    // FIXME: 단말기의 볼륨을 주어진 인덱스로 세팅한다 
    //        vol은 0 ~ md_get_max_volume() 사이의 값  
    if ((type == SOUND_MMF) || (type == SOUND_BUZZER) || (type == SOUND_EVRC)) {
        if (vol > md_get_max_volume(type)) {
            /*SKY_SetBeepVolume (md_get_max_volume(type));
        else SKY_SetBeepVolume (vol); // 이게 키톤*/
            SKY_SET_GAME_VOLUME (md_get_max_volume(type));
        } 
        else { 
            SKY_SET_GAME_VOLUME (vol); // 게임볼륨으로 하기로 함.
        }
    }
}

/*******************************************************************
 *
 *	PhoneBook
 *
 *******************************************************************/
int md_pb_get_max_record_id(void)
{
    // FIXME: 전화번호부의 최대 레코드 ID를 리턴 
	// 
    return MAX_BOOK_NUMBER-1; // 1200
}

int md_pb_is_used(int id)
{
    // FIXME: 해당 레코드 ID가 사용중인가를 리턴 
    // address는 1부터 시작된다.. 그래서 +1이 필요..
    if(IS_VALID_BOOK(id+1)) { // 사용중인 경우
		return 1;
    }
    else { // 비어있는 경우
         return 0;
    }
}

int md_pb_get_field(int id, int field, char *buf, int maxLen)
{
    // FIXME: 주어진 레코드 ID와 field에 해당하는 값을 리턴 
    pb_book_info_type       BookInfo;
    pb_number_info_type     NumberInfo;
    pb_group_info_type      GroupInfo;
    pb_email_info_type      EMailInfo;
    static INT8             nNumber;
    
    // address는 1부터 시작된다.. 그래서 +1이 필요..
	BookInfo.address = id+1;
    nNumber = 0;
	if (ui_get_pb(PB_BOOK_INFO_I, &BookInfo) <= 0)
		return -1;

    if (field == PBOOK_NAME){ // 이름  
        strncpy(buf, (char *) BookInfo.name, maxLen);
		buf[maxLen] = '\0';
        CLNTa_log(1, 0, "넘겨주는 이름값 [%d] [%d] [%s]",id, field, buf);
		return 0;
	}
    else if (field == PBOOK_GROUP) { // 그룹 
        GroupInfo.address = BookInfo.group_address;
        if (ui_get_pb(PB_GROUP_INFO_I, &GroupInfo) <= 0)
            return -1;
        else {
            strncpy(buf, (char *)g_szGroupName[GroupInfo.address-1], maxLen);
            buf[maxLen] = '\0';
            CLNTa_log(1, 0, "넘겨주는 그룹값 [%d] [%d] [%s]",id, field, buf);
	        return 0;
        }
    }   
    else if ((field >= PBOOK_HANDPHONE) && (field <= PBOOK_OFFICE)) { // 번호
        NumberInfo.address = BookInfo.number_address;
        while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) == TRUE && nNumber < PERSONAL_NUMBER_LIMIT) {
            // 여기 IF조건 부분이 바뀌어야 한다.
            if (NumberInfo.number_type == (field-2)) { // 여기가 2차이가 난다.
                if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 ) {
                    strncpy(buf,(char *)NumberInfo.digits, maxLen);
                    buf[maxLen] = '\0';
                    CLNTa_log(1, 0, "넘겨주는 번호값 [%d] [%d] [%s]",id, field, buf);
			        return 0;
                }
                else { // 해당 데이터가 없는 경우
                    strncpy(buf, NULL, maxLen);
                    buf[maxLen] = '\0';
			        return 0;
                }
            }
            nNumber++; 
            NumberInfo.address = NumberInfo.next_index;
        }
        return -1;
	}
    else if (field == PBOOK_EMAIL) { // 이메일
        EMailInfo.address = BookInfo.email_address;
        while( ui_get_pb( PB_EMAIL_INFO_I, &EMailInfo) == TRUE && nNumber < PERSONAL_EMAIL_LIMIT) {
            if( EMailInfo.book_address == BookInfo.address && STRLEN(EMailInfo.szMailID) > 0 ) {
                strncpy(buf,(char *)EMailInfo.szMailID, maxLen);
                buf[maxLen] = '\0';
                CLNTa_log(1, 0, "넘겨주는 메일값 [%d] [%d] [%s]",id, field, buf);
                return 0;
            }
            nNumber++; 
            EMailInfo.address = EMailInfo.next_index;
        }	
        // 해당 데이터가 없는 경우.
        strncpy(buf, NULL, maxLen);
        buf[maxLen] = '\0';
		return 0;
    } // E-MAIL
    else if (field == PBOOK_MEMO) { // 메모
        if( STRLEN( BookInfo.szText ) > 0 ) {
            strncpy(buf,(char *)BookInfo.szText, maxLen);
            buf[maxLen] = '\0';
            CLNTa_log(1, 0, "넘겨주는 메모 [%d] [%d] [%s]",id, field, buf);
            return 0;
        }
        else { // 해당 데이터가 없는 경우
            strncpy(buf, NULL, maxLen);
            buf[maxLen] = '\0';
            return 0;
        }
    } // MEMO
    else 
        return -1;
}

int md_pb_get_max_group_id(void)
{
    // FIXME: 그룹의 최대 ID를 리턴 
    return MAX_GROUP_NUMBER-1; // 21
}

int md_pb_get_group_name(int gid, char *buf, int maxLen)
{
    // FIXME: 주어진 gid의 이름을 리턴 
    pb_group_info_type      GroupInfo;

    // gid가 0부터 넘어오게 되니까 이렇게 넘겨줘야지..
    // 만약 1부터 넘어온다면 다시 1을 빼주도록 한다.
    GroupInfo.address = gid + 1;
    if (ui_get_pb(PB_GROUP_INFO_I, &GroupInfo) > 0) {
        strncpy(buf, (char *)g_szGroupName[GroupInfo.address-1], maxLen);
        buf[maxLen] = '\0';
        CLNTa_log(1, 0, "넘겨주는 그룹값 [%d] [%s]",gid, buf);
        return 0;
    }
    else
        return -1;
}

/*******************************************************************
 *
 *	GVM support
 *
 *******************************************************************/

void md_gvm_load_content_name(void)
{
    // FIXME: NVRAM에 저장된 GVM 컨텐츠 이름을 메모리로 로딩 
    // 로딩과정은 필요없음. NULL함수로 둔다.
}

int md_gvm_get_max_contents(void)
{
    // FIXME: 저장가능한 GVM 컨텐츠의 최대 인덱스를 리턴 
    return SWAP_SSB_SIZE -1;
}

char *md_gvm_get_content_name(int idx)
{
    // FIXME: 인덱스에 해당하는 컨텐츠 이름을 리턴 
    if (SKY_GET_GVM_SSB(idx) != 0) {
         return (char*)SKY_GET_GVM_GAME_NAME(idx);
    }
    return NULL;
}

unsigned char md_gvm_delete_content(int idx) {
    // FIXME: 인덱스에 해당하는 컨텐츠를 삭제 
    // FIXME: 삭제 여부 리턴; 1 or 0   => 대체 어느게 1이야?
    if (SKY_GET_GVM_SSB(idx) != 0) {
		g_fDeleteScript = TRUE;
		midd_DeleteScript(idx);
		return 1;
	}
    return 0;
}

void md_gvm_load_chat_strings(void)
{
    // FIXME: NVRAM에 저장된 채팅예약어들을 메모리로 로딩 
    // 로딩과정은 필요없음. NULL함수로 둔다.
}

int md_gvm_get_max_chat_strings(void)
{
    // FIXME: 단말기에 저장된 채팅예약어의 최대 인덱스 리턴 
    return (GVM_LEN_CHATTEXT - 1); // 9개니까 8
}

char *md_gvm_get_chat_string(int index)
{
    // FIXME: 인덱스에 해당하는 채팅예약어를 리턴 
	return (char*)SKY_GET_GVM_CHATTEXT(index);
}

void md_gvm_set_chat_string(int index, char *str)
{
    // FIXME: 해당 인덱스 위치에 채팅예약어 저장 
    SKY_SET_GVM_CHATTEXT(index, (BYTE *) str);
}


/*******************************************************************
 *
 *	SKT WAP/GVM Launch 
 *
 *******************************************************************/

void SKVM_gvm_start(int gvm_id)
{
    // XVM은 이 함수를 호출한 후 작업을 종료한다. 
    // 즉, UI Task가 Idle 상태로 가게 되고, UI는 Idle 상태에서
    // 주어진 gvm_id에 대한 GVM 프로그램을 호출하여야 한다. 
    // it may be implemented in uismenu.c 
    if (SKY_GET_GVM_SSB(gvm_id) != 0) {
        g_nCurSSB = gvm_id;

	    SKY_SET_FROM_SKVM(TRUE);
	    SKY_SET_TO_GVM(TRUE);
	    EQS_START_SIBLING(SKA_MainGVM, FALSE);
	}
}

// Java에서 WAP으로 넘어갈 때 WAP에 URL을 전달하는 변수이다. 
void SKVM_ntop_start(void)
{
    MSG_ERROR("ntop_start_after_java called - 미구현",0,0,0);

    // XVM은 이 함수를 호출한 후 작업을 종료한다. 
    // 즉, UI Task가 Idle 상태로 가게 되고, UI는 Idle 상태에서
    // retUrlFromJava에 등록된 URL로 WAP을 기동한다.
    // it may be implemented in uismenu.c 

	// MO=0x01
	memset((void*)g_pszretURL,0x00,MSDURL_MAX);
	memcpy((void*)g_pszretURL,(void*)retUrlFromJava,MSDURL_MAX);
	// WAP Return URL이 NULL인 경우는 default로 wap 초기 접속 url로.....
	if (!strcmp((char*)g_pszretURL,""))
		strcpy((char*)g_pszretURL,gWAPinit_url);
	SKY_SET_FROM_SKVM(TRUE);
	EQS_START_SIBLING(SKA_MainWAP, FALSE);
}