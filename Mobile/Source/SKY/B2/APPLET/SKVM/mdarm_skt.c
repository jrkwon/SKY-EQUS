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
// SKVM���� ������ SMS������ �����ϰ��� TI���� �ξ���.
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
// PHONEBOOK TEST�� ���� ���ڿ��� ������ ��� EXTERN�߾���..
// �̰� ���߿� ������...
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
	// ����� 1�ܰ��� �������� �����Ѵ�. 
	// ���Ŀ� �ٴܰ� ���� ������.. ���� ���..  
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
 *	CALL  => �Ϸ�
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
    // return ���� ���ؼ� ��õǾ� ���� ����
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
 *	set menu location (ex, /�޴�/����, XGAME)
 *
 * RETURN VALUE
 *	On success, zero is returned. On error, -1  is  returned.
 */
//extern int md_set_menu_location(char *location, char *appName)
extern int md_set_menu_location(char *location, char *cmd)
{
    // FIXME: �־��� location�� �޴��� �߰��ϰ�, 
    // FIXME: ���ý� xvm_setapp(appName)�� �θ����� �Ѵ�. 
    // NOTE: �ϴ��� �������� ���� �̴�� ���νʽÿ�. 
    MSG_ERROR("md_set_menu_location called",0,0,0);
    MSG_ERROR("md_set_menu_location returned",0,0,0);
    return -1;
}

extern int md_remove_menu_location(char *location)
{
    // FIXME: �־��� location�� �޴��� �����Ѵ�. 
    // NOTE: �ϴ��� �������� ���� �̴�� ���νʽÿ�. 
    MSG_ERROR("md_remove_menu_location called",0,0,0);
    MSG_ERROR("md_remove_menu_location returned",0,0,0);
    return -1;
}

/*******************************************************************
 *
 *	SOUND
 *
 *******************************************************************/
/* SOUND�� ���õ� ����.....   2001.08.09 bgjung

  B2�� ���������� buzzer�� �������� �ʴ� ���̶�� �մϴ�.
   ���� VM����ó�� ma2�� mmf�� �̷��� �ٸ� Ÿ������ ���ǵǾ� �ִٸ�
     �츮�� mmf�� �����Ѵٰ� �ϴ� ���� �´� ���̶�� �մϴ�.
   => �׸��Ͽ� �����ϴ� format�� mmf��� return �ϵ��� ����.
  
  XCE���� �⺻������ EVRC�� Buzzer�� �����ؾ� �Ѵٰ� �ϴ±���.
  �׸��Ͽ� ����� ���������� mmf�� EVRC, buzzer�� �����մϴ�.
   (���� BEEP���� play�ؾ� �Ѵ�����)

  sound ����� ũ��δ� ����Ŀ ���� ũ�Ⱑ �ƴ� Ű�� ���� ũ��� ����.
   //(SKY_GET_SPEAKER_VOLUME ��ſ� SKY_GET_KEY_TONE_VOLUME����....)
   (SKY_GET_SPEAKER_VOLUME ��ſ� SKY_GET_GAME_VOLUME����....)
   => ��Ƽ��Ű�� ������ Ű���� ������ �Ǿ ���ӼҸ��� �ȳ��� �� �� �����ϱ�..
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
    // FIXME: ��밡�� �Ҹ������� ORING�Ѵ�. 
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
    // FIXME: type�� valid�Ѱ��� �˻��Ѵ�. 
    // �����ϸ� 0, �׷��� ������ -1�� �ѱ��. 
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
    // ���⼭ EVRC�� BUZZER�� ���� mmf �������� ��ȯ�� �ڿ� 
    // �����͸� �˻��Ѵ�.
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
    // FIXME: type�� ���� ���ָ� �����Ѵ� 
    // FIXME: ����� xvm_signal_sound_play_end(retstate)�� �ҷ��� �Ѵ� 
    // FIXME: �ַ� sound play�� callback�� �־� �ش� 
    void xvm_signal_sound_play_end(int ret);
    // ���� Ÿ���� �� �� ��� �� �� �����Ŵ.
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
    // �������� ���ܵ�.
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
    // �������� ���ܵ�.
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
    // FIXME: freq(hertz)�� �ش��ϴ� �������� duration(msec)
    // 	      ���� ������ �Ѵ�.          
    // ���⼭ �ɸ��� ������....
    // �츮�� �÷����Ҷ� Hz�� �Ѱ��ִ� ���� �ƴ϶� �ش� Hz�� �����س��� ENUM�� ����.
    // ��� �������̽� �ʿ�.. (�����ü��迡�� ���߾�...)
    //SKY_PlayDTMF (��⼭ �ɸ�, (WORD)duration);     

	//youandi 01.08.24 ������ ��.
	//�Ʒ�feature�� �츮�� �Ҹ��� ���ϴ�. 
	//�� �ް��� 8��30�ϱ����̴� 31�Ͽ� feature�� �츮�ڽ��ϴ�.

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
    // FIXME: �ܸ��⿡�� �����Ǵ� �ִ� ������ �ε����� ���� 
    if ((type == SOUND_MMF) || (type == SOUND_BUZZER) || (type == SOUND_EVRC)) 
        //return 5;
        return 4; // 0~4
    else 
        return -1;
}

int md_get_volume(int type)
{
    // FIXME: ���� ������ �ε����� ���� 
    if ((type == SOUND_MMF) || (type == SOUND_BUZZER) || (type == SOUND_EVRC)) 
        //return SKY_GET_KEY_TONE_VOLUME();
        return SKY_GET_GAME_VOLUME();
    else 
        return -1;
}

void md_set_volume(int type, int vol)
{
    // FIXME: �ܸ����� ������ �־��� �ε����� �����Ѵ� 
    //        vol�� 0 ~ md_get_max_volume() ������ ��  
    if ((type == SOUND_MMF) || (type == SOUND_BUZZER) || (type == SOUND_EVRC)) {
        if (vol > md_get_max_volume(type)) {
            /*SKY_SetBeepVolume (md_get_max_volume(type));
        else SKY_SetBeepVolume (vol); // �̰� Ű��*/
            SKY_SET_GAME_VOLUME (md_get_max_volume(type));
        } 
        else { 
            SKY_SET_GAME_VOLUME (vol); // ���Ӻ������� �ϱ�� ��.
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
    // FIXME: ��ȭ��ȣ���� �ִ� ���ڵ� ID�� ���� 
	// 
    return MAX_BOOK_NUMBER-1; // 1200
}

int md_pb_is_used(int id)
{
    // FIXME: �ش� ���ڵ� ID�� ������ΰ��� ���� 
    // address�� 1���� ���۵ȴ�.. �׷��� +1�� �ʿ�..
    if(IS_VALID_BOOK(id+1)) { // ������� ���
		return 1;
    }
    else { // ����ִ� ���
         return 0;
    }
}

int md_pb_get_field(int id, int field, char *buf, int maxLen)
{
    // FIXME: �־��� ���ڵ� ID�� field�� �ش��ϴ� ���� ���� 
    pb_book_info_type       BookInfo;
    pb_number_info_type     NumberInfo;
    pb_group_info_type      GroupInfo;
    pb_email_info_type      EMailInfo;
    static INT8             nNumber;
    
    // address�� 1���� ���۵ȴ�.. �׷��� +1�� �ʿ�..
	BookInfo.address = id+1;
    nNumber = 0;
	if (ui_get_pb(PB_BOOK_INFO_I, &BookInfo) <= 0)
		return -1;

    if (field == PBOOK_NAME){ // �̸�  
        strncpy(buf, (char *) BookInfo.name, maxLen);
		buf[maxLen] = '\0';
        CLNTa_log(1, 0, "�Ѱ��ִ� �̸��� [%d] [%d] [%s]",id, field, buf);
		return 0;
	}
    else if (field == PBOOK_GROUP) { // �׷� 
        GroupInfo.address = BookInfo.group_address;
        if (ui_get_pb(PB_GROUP_INFO_I, &GroupInfo) <= 0)
            return -1;
        else {
            strncpy(buf, (char *)g_szGroupName[GroupInfo.address-1], maxLen);
            buf[maxLen] = '\0';
            CLNTa_log(1, 0, "�Ѱ��ִ� �׷찪 [%d] [%d] [%s]",id, field, buf);
	        return 0;
        }
    }   
    else if ((field >= PBOOK_HANDPHONE) && (field <= PBOOK_OFFICE)) { // ��ȣ
        NumberInfo.address = BookInfo.number_address;
        while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) == TRUE && nNumber < PERSONAL_NUMBER_LIMIT) {
            // ���� IF���� �κ��� �ٲ��� �Ѵ�.
            if (NumberInfo.number_type == (field-2)) { // ���Ⱑ 2���̰� ����.
                if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 ) {
                    strncpy(buf,(char *)NumberInfo.digits, maxLen);
                    buf[maxLen] = '\0';
                    CLNTa_log(1, 0, "�Ѱ��ִ� ��ȣ�� [%d] [%d] [%s]",id, field, buf);
			        return 0;
                }
                else { // �ش� �����Ͱ� ���� ���
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
    else if (field == PBOOK_EMAIL) { // �̸���
        EMailInfo.address = BookInfo.email_address;
        while( ui_get_pb( PB_EMAIL_INFO_I, &EMailInfo) == TRUE && nNumber < PERSONAL_EMAIL_LIMIT) {
            if( EMailInfo.book_address == BookInfo.address && STRLEN(EMailInfo.szMailID) > 0 ) {
                strncpy(buf,(char *)EMailInfo.szMailID, maxLen);
                buf[maxLen] = '\0';
                CLNTa_log(1, 0, "�Ѱ��ִ� ���ϰ� [%d] [%d] [%s]",id, field, buf);
                return 0;
            }
            nNumber++; 
            EMailInfo.address = EMailInfo.next_index;
        }	
        // �ش� �����Ͱ� ���� ���.
        strncpy(buf, NULL, maxLen);
        buf[maxLen] = '\0';
		return 0;
    } // E-MAIL
    else if (field == PBOOK_MEMO) { // �޸�
        if( STRLEN( BookInfo.szText ) > 0 ) {
            strncpy(buf,(char *)BookInfo.szText, maxLen);
            buf[maxLen] = '\0';
            CLNTa_log(1, 0, "�Ѱ��ִ� �޸� [%d] [%d] [%s]",id, field, buf);
            return 0;
        }
        else { // �ش� �����Ͱ� ���� ���
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
    // FIXME: �׷��� �ִ� ID�� ���� 
    return MAX_GROUP_NUMBER-1; // 21
}

int md_pb_get_group_name(int gid, char *buf, int maxLen)
{
    // FIXME: �־��� gid�� �̸��� ���� 
    pb_group_info_type      GroupInfo;

    // gid�� 0���� �Ѿ���� �Ǵϱ� �̷��� �Ѱ������..
    // ���� 1���� �Ѿ�´ٸ� �ٽ� 1�� ���ֵ��� �Ѵ�.
    GroupInfo.address = gid + 1;
    if (ui_get_pb(PB_GROUP_INFO_I, &GroupInfo) > 0) {
        strncpy(buf, (char *)g_szGroupName[GroupInfo.address-1], maxLen);
        buf[maxLen] = '\0';
        CLNTa_log(1, 0, "�Ѱ��ִ� �׷찪 [%d] [%s]",gid, buf);
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
    // FIXME: NVRAM�� ����� GVM ������ �̸��� �޸𸮷� �ε� 
    // �ε������� �ʿ����. NULL�Լ��� �д�.
}

int md_gvm_get_max_contents(void)
{
    // FIXME: ���尡���� GVM �������� �ִ� �ε����� ���� 
    return SWAP_SSB_SIZE -1;
}

char *md_gvm_get_content_name(int idx)
{
    // FIXME: �ε����� �ش��ϴ� ������ �̸��� ���� 
    if (SKY_GET_GVM_SSB(idx) != 0) {
         return (char*)SKY_GET_GVM_GAME_NAME(idx);
    }
    return NULL;
}

unsigned char md_gvm_delete_content(int idx) {
    // FIXME: �ε����� �ش��ϴ� �������� ���� 
    // FIXME: ���� ���� ����; 1 or 0   => ��ü ����� 1�̾�?
    if (SKY_GET_GVM_SSB(idx) != 0) {
		g_fDeleteScript = TRUE;
		midd_DeleteScript(idx);
		return 1;
	}
    return 0;
}

void md_gvm_load_chat_strings(void)
{
    // FIXME: NVRAM�� ����� ä�ÿ������� �޸𸮷� �ε� 
    // �ε������� �ʿ����. NULL�Լ��� �д�.
}

int md_gvm_get_max_chat_strings(void)
{
    // FIXME: �ܸ��⿡ ����� ä�ÿ������ �ִ� �ε��� ���� 
    return (GVM_LEN_CHATTEXT - 1); // 9���ϱ� 8
}

char *md_gvm_get_chat_string(int index)
{
    // FIXME: �ε����� �ش��ϴ� ä�ÿ��� ���� 
	return (char*)SKY_GET_GVM_CHATTEXT(index);
}

void md_gvm_set_chat_string(int index, char *str)
{
    // FIXME: �ش� �ε��� ��ġ�� ä�ÿ���� ���� 
    SKY_SET_GVM_CHATTEXT(index, (BYTE *) str);
}


/*******************************************************************
 *
 *	SKT WAP/GVM Launch 
 *
 *******************************************************************/

void SKVM_gvm_start(int gvm_id)
{
    // XVM�� �� �Լ��� ȣ���� �� �۾��� �����Ѵ�. 
    // ��, UI Task�� Idle ���·� ���� �ǰ�, UI�� Idle ���¿���
    // �־��� gvm_id�� ���� GVM ���α׷��� ȣ���Ͽ��� �Ѵ�. 
    // it may be implemented in uismenu.c 
    if (SKY_GET_GVM_SSB(gvm_id) != 0) {
        g_nCurSSB = gvm_id;

	    SKY_SET_FROM_SKVM(TRUE);
	    SKY_SET_TO_GVM(TRUE);
	    EQS_START_SIBLING(SKA_MainGVM, FALSE);
	}
}

// Java���� WAP���� �Ѿ �� WAP�� URL�� �����ϴ� �����̴�. 
void SKVM_ntop_start(void)
{
    MSG_ERROR("ntop_start_after_java called - �̱���",0,0,0);

    // XVM�� �� �Լ��� ȣ���� �� �۾��� �����Ѵ�. 
    // ��, UI Task�� Idle ���·� ���� �ǰ�, UI�� Idle ���¿���
    // retUrlFromJava�� ��ϵ� URL�� WAP�� �⵿�Ѵ�.
    // it may be implemented in uismenu.c 

	// MO=0x01
	memset((void*)g_pszretURL,0x00,MSDURL_MAX);
	memcpy((void*)g_pszretURL,(void*)retUrlFromJava,MSDURL_MAX);
	// WAP Return URL�� NULL�� ���� default�� wap �ʱ� ���� url��.....
	if (!strcmp((char*)g_pszretURL,""))
		strcpy((char*)g_pszretURL,gWAPinit_url);
	SKY_SET_FROM_SKVM(TRUE);
	EQS_START_SIBLING(SKA_MainWAP, FALSE);
}