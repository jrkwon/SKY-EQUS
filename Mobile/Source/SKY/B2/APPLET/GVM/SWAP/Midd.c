//==============================================================================
//
//	SINJISOFT SWAP MODULE LIBRARY
//
//	FILE NAME
//		MIDD.cpp
//
//	DESCRIPTION
//		Mobile Interface Device Driver
//
//	REFERENCES
//		SINJISOFT SWAP Porting Guide
//
//	Copyright (C) 2000, 2001 by SINJISOFT. All Rights Reserved.
//
//	HISTORY
//		2000. 5.20  Alpha Release
//		2000. 6.10  Beta Release
//		2000. 7. 3  Beta2 Release
//		2000. 7.24  Beta3 Release
//		2000. 8. 7  Version 1.0 Release
//		2000. 8.28  Version 1.01 Release
//		2000. 9.26  Version 1.02 Release
//		2000.10.27  Version 1.10 Release
//		2000.12.19	Version 1.5 Beta Start
//		2001. 1. 1	Version 1.5 Beta1 Release
//
//==============================================================================


//#include <stdio.h>
#include <string.h>
//#include <sys/timeb.h>
#include <time.h>


#include "SWAP.h"
#include "ska.h"
#include "gvm_config.h"
#include "sky_sock.h"   // added for new sock api

#ifdef SWAP_FEATURE_PTP
#include "uisockApi.h"
#endif

#include "psmisc.h"	// for dns query error. added 11/15/2001


        UINT8	asciiBuffer[256];
STATIC  db_items_value_type dbi;   /* DB items */ 
STATIC  BYTE    szSvrIP[SWAP_LEN_IP+1];
STATIC  WORD    nSvrPort;

// for new socket api
STATIC  sint15  gvm_sock_fd = DSS_ERROR;




//==============================================================================
//
//	Function Definition
//
//==============================================================================
#ifdef FEATURE_SKT_EIF
extern BOOL g_EIF_WAP_GVMINFO;
extern char resultBuf[4096];
extern void at_send_extended_text(char*);
#endif	//#ifdef FEATURE_SKT_EIF

//==============================================================================
//
//	SRAM Memory management
//
//==============================================================================

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  // 64K color
static WORD*    _lcdBuffer1;
static WORD*    _lcdBuffer2; 
#else                                           // 256 color
static BYTE*    _lcdBuffer1;
static BYTE*    _lcdBuffer2; 
#endif

static BYTE*	_scriptBuffer;
static BYTE*	_workspace;

//==============================================================
//	Allocate SWAP SRAM buffers
//
void midd_InitMemory()
{
    extern BYTE	Image$$APP_SHARE_RAM$$Base[];
    
    // SWAP script buffer
	_scriptBuffer = (BYTE*)Image$$APP_SHARE_RAM$$Base;
    swScriptBuffer = _scriptBuffer;

    // SWAP Work space
    _workspace = _scriptBuffer + SWAP_MAX_SCRIPT_SIZE;
    swWorkSpace = _workspace;

    // LCD Buffer    
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  
    _lcdBuffer1 = (WORD*)(_workspace + SWAP_MAX_WORKSPACE); // 64K color
#else 
    _lcdBuffer1 = _workspace + SWAP_MAX_WORKSPACE;          // 256 color
#endif
    swLcdBuffer = _lcdBuffer1;
	memset(swLcdBuffer, 0, S_LCD_BUFFER);

    // BackLcd Buffer
//#ifdef SWAP_FEATURE_BACKBUF

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)  
    //_lcdBuffer2 = (WORD*)(_lcdBuffer1 + (WORD)(S_LCD_BUFFER/2));    // 64k color
	_lcdBuffer2 = (WORD*)(_lcdBuffer1 + S_LCD_BUFFER);    // 64k color
    swBackLcdBuffer = _lcdBuffer2;
#else
    _lcdBuffer2 = _lcdBuffer1 + S_LCD_BUFFER;   // 256 color
    swBackLcdBuffer = _lcdBuffer2;
	memset(swBackLcdBuffer, 0, S_LCD_BUFFER);

//#endif  // lcd ����
    
#endif //SWAP_FEATURE_BACKBUF    
}

//==============================================================
//	Deallocate SWAP SRAM buffers
//
void midd_CloseMemory()
{
	SWAP_TRACE("midd_CloseMemory()", 0, 0, 0);
    
	// Restore shared memory
    return;
}

//==============================================================
//	Read script info from NV-ROM
//
//	ssbIndex	index of SSB info, 0..SWAP_SSB_SIZE - 1
//	ssbInfo		SSB info buffer pointer
//
//	return		1 = Success, 0 = Fail
//

int midd_GetScriptInfo(int ssbIndex, SSBInfo* ssbInfo)
{
    SSBInfo mySSBInfo;
    memset( ssbInfo, 0, sizeof(SSBInfo));
    
    if (SKY_GET_GVM_SSB(ssbIndex) == 0) 
    {
        // this Script is not Valid
        error("[midd_GetScriptInfo] %dth ssb don't exist---------------------", ssbIndex, 0, 0);
        return 0;
    }   
    
    mySSBInfo = SKY_GET_GVM_SSBINFO(ssbIndex); 
    *ssbInfo = mySSBInfo;

    return 1;
}

//==============================================================
//	Write script info into NV-ROM
//
//	ssbIndex	index of SSB info, 0..SWAP_SSB_SIZE - 1
//	ssbInfo		SSB info buffer pointer
//
//	return		1 = Success, 0 = Fail
//
void midd_PutScriptInfo(int ssbIndex, SSBInfo* ssbInfo)
{
    error("[midd_PutScriptInfo] ssbindex:%d -------------", ssbIndex, 0, 0);    
    SKY_SET_GVM_SSBINFO(ssbIndex, *ssbInfo);
    return;
}

//==============================================================
//	Check free space of SSB
//
//	newSize		required size of SSB memory
//	ssbIndex	returned free SSB index
//
//	return		0 = Enough free space
//				-1 = Out of SSB slot
//				else = Insufficient memory size
//
int midd_CheckFreeSSB(DWORD newSize, int *ssbIndex)
{
	int	size = 0, i=0;
    
    error("[midd_CheckFreeSSB]ssb:%d,new:%d,free:%d -------------", *ssbIndex, newSize, g_dwFreeSpace);
    
    // check size
    /* 
        1. ���� �ִ� �������� ��ũ��Ʈ ũ�Ⱑ Ŭ ��� 
            -> ���ڶ� �����ŭ return
    */
    if (newSize > g_dwFreeSpace) // usable space?
    {
       size = (int)(newSize - g_dwFreeSpace);  // ������ 1024�ؾ� �ϳ�?>???
       error("��ũ��Ʈ�� ũ��.. ����ڸ��� %d", size, 0, 0);
       return size;
    }
    
    // Find free SSB
	*ssbIndex = -1;
    for (i=0;i<SWAP_SSB_SIZE;i++) 
    {
        if (SKY_GET_GVM_SSB(i) == 0 || scl_ModifyScriptInfo(i, 0))  // checkvalidate�� �Ⱦ���???
        {
            error("scl_ModifyScriptInfo=%d(CHECK)", scl_ModifyScriptInfo(i, 0), 0, 0);
			*ssbIndex = i;			// Found
			break;
		}
    }

    if (*ssbIndex == -1) 
    {
        /*
            2. Flash-SSB�� ���� ������ ������
                -> ssbindex=-1;
                -> ������ ���� ������ ���ڶ�� ũ��, ������ ���� ������ 0
        */
        if (newSize > g_dwFreeSpace) 
            size = (int)(newSize - g_dwFreeSpace);
        else
            size = 0;
        error("���� ������ ����(size:%d)<-checkFreeSSB", size, 0, 0);
    }
    else 
    {
        /*
            3. ���� ���� ������
                -> ssbindex=slot
                -> size=0
        */
        size = 0;
        error("���� ����(%d)�� �ִ�<-CheckFreeSSB", *ssbIndex, 0, 0);
    }

	return size;						// Enough free space
}

//==============================================================
//	Store script data into SSB and store script info into NV-ROM
//
//	ssbIndex	index of SSB info
//	ssbInfo		Downloaded script info data pointer
//
void midd_WriteScriptInfo(int ssbIndex, SSBInfo* ssbInfo)
{
	STATIC  HFILE   hFile;
	STATIC  UINT8	i;
	        CHAR    szFileName[SWAP_LEN_GNAME + 1];
	        SKY_DATA_FILE_RESULT_E  FileHandleResult;
        
    error("[midd_WriteScriptInfo] ssbindex:%d -------------", ssbIndex, 0, 0);    	

    // Flash-SSB �� �����ϱ� ���� midd_DeleteScript�� ���� ���� ��û�� ��ũ��Ʈ����
    // scl_ModifyScriptINfo�Լ��� ���� Ȯ���� ��, ������û�� ��ũ��Ʈ�� ����
    // NV-ROM�� ����� ��ũ��Ʈ ������ Flash-SSB�� ����Ǿ��� ��ũ��Ʈ�� �����Ѵ�.
    for (i=0; i < SWAP_SSB_SIZE; i++) 
    { 	
        if (scl_ModifyScriptInfo(i, 0)) // i��° ��ũ��Ʈ�� ���� Flag�� �����Ǿ����� Ȯ��
        {  
			// Clear script delete flag
    		scl_ModifyScriptInfo(i, 1);
            
            // delete script & script info
            g_fDeleteScript = TRUE;
            midd_DeleteScript(i);            
    	}
	}

    /* create file */
    memset(szFileName, 0, SWAP_LEN_GNAME+1);
    GetGVMFileName((BYTE*)szFileName, ssbInfo->ScriptCPID, ssbInfo->ScriptID);
    FileHandleResult = SKY_CreateDataFile(SDF_GVM, (BYTE*)szFileName, &hFile);
    if(FileHandleResult == SDR_OK)
    {
		SKY_WriteDataBlock(hFile, swScriptBuffer, ssbInfo->ScriptSize);
		
        // close the file
		SKY_CloseDataFile(hFile);

        // set the flag        
        g_nCurSSB = ssbIndex;
        g_TypeConnectSvr = GCST_NONE;   // �ٿ�ε� ����..
        
        // Write script info into NV-ROM
        SKY_SET_GVM_SSB(ssbIndex, TRUE);
	    midd_PutScriptInfo(ssbIndex, ssbInfo);
        g_dwFreeSpace = g_dwFreeSpace - ssbInfo->ScriptSize;

        error("�ٿ�ε� ��-ssb(idx:%d)��:%d", ssbIndex, (int)(SKY_GET_GVM_SSB(ssbIndex)), 0);
	}
	else
	{
		error("CAN NOT CREATE DATA FILE",0,0,0);
	}
    return;
}

//==============================================================
//	Load stored script into script buffer by block unit
//
//	ssbIndex	stored SSB index
//	start		start address to be read
//	buf			block data buffer
//	bsize		read request size
//
//	return		read size
//
int midd_LoadScriptBlock(int ssbIndex, DWORD start, int bsize, BYTE *buf)
{
    DWORD   dwCount = 0;
    STATIC  HFILE   hReadFile;
    BYTE szFileName[SKY_FILE_NAME_MAX_LENGTH];
    
    GetGVMFileName(szFileName, SKY_GET_GVM_GAME_CPID(ssbIndex), SKY_GET_GVM_GAME_ID(ssbIndex));
    
    if(SKY_OpenDataFile(SDF_GVM, szFileName, &hReadFile, SFT_READ) == SDR_OK)
    {
        dwCount = SKY_SeekReadFile(hReadFile, FS_SO_SET, start, buf, bsize);
        SKY_CloseDataFile(hReadFile);        
    }
    else error("����� open���� ����", 0, 0, 0);
    return dwCount;
}

//==============================================================
//	Delete stored script and script info
//
//	ssbIndex	deleted SSB index
//
// ���� ��û ���!!!

void midd_DeleteScript(int ssbIndex)
{
	SSBInfo ssbInfo;
    BYTE szFileName[SKY_FILE_NAME_MAX_LENGTH+1];
	BYTE szUserData[SWAP_USERNV_SIZE+1];

    // << �߰��� ���� >>
    // ��ũ��Ʈ�� scl_ModifyScriptInfo �Լ��� ȣ���Ͽ� ������û�� ����Ѵ�.
    // ������ ��ũ��Ʈ ������ �����Ͽ� Flash-SSB�� �������� ������ �����Ѵ�.
    
    error("[midd_DeleteScript] ssbindex:%d -------------", ssbIndex, 0, 0);
    if (SKY_GET_GVM_SSB(ssbIndex) == 0) 
    {
        error("������ ������ ����! -> ���� �̸��� ���Ҽ� ����.", 0, 0, 0);
        return;
    }
    
    if (g_fDeleteScript) // ���� ���� <-  FROM SK-VM
    {          
        // Delete script
        GetGVMFileName(szFileName, SKY_GET_GVM_GAME_CPID(ssbIndex), SKY_GET_GVM_GAME_ID(ssbIndex) );
        SKY_DeleteDataFile(SDF_GVM, szFileName);
 
        // Delete script info
	    memset(&ssbInfo, 0, sizeof(ssbInfo));
		memset(szUserData, 0, SWAP_USERNV_SIZE+1);
	    midd_PutScriptInfo(ssbIndex, &ssbInfo);
        SKY_SET_GVM_SSB(ssbIndex, FALSE);
		SKY_SET_GVM_USER(ssbIndex, szUserData, SWAP_USERNV_SIZE);
        g_fDeleteScript = FALSE;

        error("%d��°�� ������ ������ <- WriteScript", ssbIndex, 0, 0);
    }
    else                // �׳� ������û�� ǥ��
    {      
        // Update script info
        scl_ModifyScriptInfo(ssbIndex, 1);
        
        // �������� ���� ����
        g_dwFreeSpace = g_dwFreeSpace + SKY_GET_GVM_GAME_SIZE(ssbIndex);  
        error("%d��°�� �ӽ÷� ����ó���� <- WriteScript", ssbIndex, 0, 0);
    }    
}



//==============================================================
//	Read user NV data from NV-ROM
//
//	data	user data pointer
//	size	read data size
//
void midd_GetUserNV(int ssbIndex, BYTE *data, int size)
{
    /*if (size > SWAP_USERNV_SIZE) 
    {
        error("[GetUserNV]sizeoverflow!-%d", size, 0, 0);
        size = SWAP_USERNV_SIZE;  
    }
    memcpy(data, (void *)SKY_GET_GVM_USER(ssbIndex), size);*/
	memcpy(data, (void *)SKY_GET_GVM_USER(ssbIndex), size > SWAP_USERNV_SIZE ? SWAP_USERNV_SIZE : size);
}

//==============================================================
//	Write user NV data into NV-ROM
//
//	data	user data pointer
//	size	read data size
//
void midd_PutUserNV(int ssbIndex, BYTE *data, int size)
{
    STATIC  BYTE	buf[SWAP_USERNV_SIZE+1];

	error("midd_PutUserNV�Ҹ�--------------", 0, 0, 0);
    memset(buf, 0, SWAP_USERNV_SIZE+1);    
    memcpy(buf, data, size > SWAP_USERNV_SIZE ? SWAP_USERNV_SIZE : size);
	SKY_SET_GVM_USER_TEMP(ssbIndex, buf, size > SWAP_USERNV_SIZE ? SWAP_USERNV_SIZE : size);
}

//==============================================================
//	Read PTP dial number from NV-ROM
//
//	dial	dial string
//
#ifdef SWAP_FEATURE_PTP
void midd_GetPtpDial(char *dial)
{
    STRNCPY((BYTE*)dial, (BYTE*)(SKY_GET_GVM_PTP_DIAL()), SWAP_LEN_DIAL);
}
#endif // SWAP_FEATURE_PTP

//==============================================================
//	Write PTP dial number  into NV-ROM
//
//	dial	dial string
//
#ifdef SWAP_FEATURE_PTP
void midd_PutPtpDial(char *dial)
{
    STATIC  BYTE	buf[SWAP_LEN_DIAL+1];
	    
    memset(buf, 0x00, SWAP_LEN_DIAL+1);
    if (strlen(dial) > SWAP_LEN_DIAL) 
        memcpy(buf, dial, SWAP_LEN_DIAL);
    else 
        memcpy(buf, dial, strlen(dial));
	SKY_SET_GVM_PTP_DIAL(buf);
}
#endif // SWAP_FEATURE_PTP


//==============================================================================
//
//	LCD/Audio Control
//
//==============================================================================

//==============================================================
//	Display LCD buffer
//
//	buf		LCD buffer pointer
//
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START) //SWAP_LCD_COLOR256_HORIZ)
void midd_DrawLcd(WORD *buf)        // -> partial �� ��ĥ ��!!!
{
	/*
		�����Է� ���� se_recv_data �̺�Ʈ �߻��� �̺�Ʈ ó���� �ϵ�
		������ ���⿡�� ����ؼ� �Է��� �޴� ��ó�� ���̰� �ϱ� ���ؼ� 
		midd_DrawLcd�Լ��� ������ ���ѽ�Ų��.
	*/
	if (EQS_GET_APPLET() != sSKA_TextEdit)	// <- ���� ����� �� ��
	{
		STATIC UINT16 *pLCD;
		
		pLCD = (UINT16 *)GetScreenBuffer()+SKY_SCREEN_WIDTH*SKY_ANNUNCIATORS_HEIGHT;
		memcpy(pLCD, buf, S_LCD_BUFFER);
		SKY_UpdatePartialScreenWithoutAnnu((BYTE*)buf);
		//SKY_UpdateScreen();
		//EQS_AppletDrawDirect();
		//SKY_UpdatePartialScreenLine((BYTE*)buf, SKY_ANNUNCIATORS_HEIGHT, SKY_SCREEN_HEIGHT-1);    			
	}
}
#else
void midd_DrawLcd(BYTE *buf)
{
	/*
		�����Է� ���� se_recv_data �̺�Ʈ �߻��� �̺�Ʈ ó���� �ϵ�
		������ ���⿡�� ����ؼ� �Է��� �޴� ��ó�� ���̰� �ϱ� ���ؼ� 
		midd_DrawLcd�Լ��� ������ ���ѽ�Ų��.
	*/
	//if (EQS_GET_APPLET() != sSKA_TextEdit) 
	if (g_fIn256ColorDisplay)
	{
		SKY_256UpdatePartialScreenWithoutAnnu(buf);
		//SKY_256UpdatePartialDisplay(buf, SKY_ANNUNCIATORS_HEIGHT, SKY_SCREEN_HEIGHT-1);        
	}
}
#endif // WORDCOLOR_START


//==============================================================
//	Play the handset sound device
//
//	data	sound data pointer
//	len		sound data length
//
void midd_PlaySound(BYTE *data, int len)
{
    
#if (SWAP_AUDIO_TYPE == SWAP_AUDIO_MA2)
	// MA2 play routine
	if (*data == 0x02)
	{
        //SKY_StopMusic(); // ���������� ���� �θ�
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
        SKY_PlayMusicBufferEx((BYTE*)(data + 2), SKY_GET_GAME_VOLUME(), 1);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
		SKY_PlayMusicBufferEx((WORD*)(data + 2), SKY_GET_GAME_VOLUME());
#endif
	}
    else if(*data == 0x00) /* ���� play routine */
    {

#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE        
        extern void ConvGVMBellData2SMAF(UINT8*, BYTE);
        extern BYTE mb_MyBellPreBuffer[MYMELODY_DATA_MAX];

        error("now buzzer play", 0, 0, 0);

        //SKY_StopMusic();  // ���������� ���� �θ�
        // Melody parsing        
        ConvGVMBellData2SMAF((BYTE *)(data+2), len-2); /* len�� ��� ������ ������ ��� �� 2bytes�� ���� �Ѱ��� */
        // Pre-play        
        SKY_PlayMusicBufferEx((BYTE*)mb_MyBellPreBuffer, SKY_GET_GAME_VOLUME(), 1);
#endif
    }
	else
#endif // #if (SWAP_AUDIO_TYPE == SWAP_AUDIO_MA2)
	{
		return;
	}
}

//==============================================================
//	Stop playing the handset sound
//
void midd_StopSound()
{
    error("midd_StopSound �Ҹ� -------------", 0, 0, 0);
    SKY_StopMusic();
}

//==============================================================
//	Play the handset vocoder
//
//	data	vocoder data pointer
//	len		vocoder data length
//
void midd_PlayVocoder(BYTE* data, int len)
{
    
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
#ifdef FEATURE_VOICEPLAY  // jwpark 00.11.06
    SKY_PlayVoice((BYTE *)data,(WORD)len, (WORD)1);
#endif
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE   
	SKY_PlayVoice((BYTE *)data, (UINT16)len, (UINT8)1);
#endif
}

//==============================================================
//	Stop the handset vocoder
//
void midd_StopVocoder()
{
    // Stop EVRC vocoder data
	SKY_StopVoice();
}

//==============================================================
//	Turn on the vibrator
//
//	t	vibration time (sec unit)
//
void midd_StartVib(int t)
{
    SKY_MotorTime( (UINT16)(t*1000) );    
}

//==============================================================
//	Turn off the vibrator
//
void midd_StopVib()
{
    SKY_MotorStop();
}

//==============================================================
//	Set keytone state
//
//	sw	0 = turn off, 1 = turn on
//
void midd_SetKeyTone(int sw)
{
    if (sw)
    {
		/* enable */
		if (SKY_GET_KEY_TONE_VOLUME() == 0)
		{
			SKY_SET_KEY_TONE(FALSE);
		}		
		else
		{
			SKY_SET_KEY_TONE(TRUE);
		}
    }
    else
    {
		/* disable */
		SKY_SET_KEY_TONE(FALSE);
    }
}

//==============================================================
//	Set backlight state
//
//	sw	0 = turn off, 1 = turn on
//
void midd_SetBacklight(int sw)
{
    SKY_SET_IGNORE_BACKLIGHT(TRUE);
    switch (sw)
    {
        case 1: /* ON */
            SKY_BacklightOn(AB_ALWAYS);
            break;
        case 0: /* OFF */
            SKY_BacklightOff();
            break;
        default:
		    SKY_SET_NO_OFF_BACKLIGHT(FALSE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());		//�����Ҷ� �ݵ�� ȣ�� OR ����ڰ� ������ ������ ���ư����� �Ҷ� ȣ��
            break;
    }    
}

//==============================================================================
//
//	Timer Control
//
//==============================================================================

//==============================================================
//	Start handset timer
//
//	id		timer id (0,1 or 2)
//	t		timer period (milisec uint)
//	repeat	timer repeat flag
//
void midd_StartTimer(int id, int t, int repeat)
{
    // Start game timer
	switch(id) {
	    case 0:
            SKY_SetGVMTimer1((LONG)t, (BOOL)repeat);		    
		    break;
	    case 1:
            SKY_SetGVMTimer2((LONG)t, (BOOL)repeat);
		    break;
	    case 2:
            SKY_SetGVMTimer3((LONG)t, (BOOL)repeat);
		    break;
	    default:
		    break;
	}
}

//==============================================================
//	Stop running timer
//
//	id		timer id (0,1 or 2)
//
void midd_StopTimer(int id)
{
    switch(id) {
	    case 0:
            SKY_SetGVMTimer1((LONG)0, (BOOL)FALSE);		    
            break;
	    case 1:
		    SKY_SetGVMTimer2((LONG)0, (BOOL)FALSE);
            break;
	    case 2:
            SKY_SetGVMTimer3((LONG)0, (BOOL)FALSE);
            break;
	    default:
		    break;
	}
}


//==============================================================================
//
//	Network API
//
//==============================================================================

#ifdef SWAP_FEATURE_NETWORK

//==============================================================
//	Connect TCP/IP socket connection
//
//	ip		server IP string
//	port	server port
//  reason  connect reason (0 : downloading)
//
//	After connection process,
//	the connection result was called mmiSwapMenu
//
void midd_Connect(char *ip, WORD port, int reason)
{
    error("[midd_Connect] (port:%d", port, 0, 0);

    if (reason == 0) 
    {
#ifdef FEATURE_SKT_EIF
		if(g_EIF_WAP_GVMINFO)
		{
			//GVM ���� ���� ����
    		CLNTa_log(1,0, "GVMINFO : S");
			at_send_extended_text("*SKT*GVMINFO:S");
		}
#endif	//#ifdef FEATURE_SKT_EIF

        error("connection FOR DOWNLOAD!!!!", 0, 0, 0);
#ifdef SWAP_FEATURE_IS95C
        midd_SetSoMo(33, 0x921);
#endif //#ifdef SWAP_FEATURE_IS95C
    }
    else 
    {
        error("connection FOR SERVER!!!!", 0, 0, 0);
    }

    // midd_Connect2�Լ����� ����ϱ� ���� ���� ������.
    memset((void*)szSvrIP, 0, SWAP_LEN_IP+1);
    STRNCPY(szSvrIP, (CONST BYTE*)ip, SWAP_LEN_IP);
    nSvrPort = port;
    
    // ���� ���� �����ε� �ٸ� ������ �����ؾ� �ϴ� ��� �߻��ϸ� socket ����
    if (gvm_sock_fd != DSS_ERROR) 
    {
        if (midd_closesocket() == DSS_ERROR) 
        {
            midd_closesocket();
        }
        /*gvm_sock_fd = DSS_ERROR;
        EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);
        return;*/
    }

    // init socket fd
    gvm_sock_fd = DSS_ERROR;
    
    // ���ܻ��� check 
    if (g_fFromWAP && P_REV_IS_6_OR_GREATER && NetworkGetState() == NET_PPPOPEN_S)    // from wap && 2.5G => ppp open�Ǿ� �ִ� ������
	{
        error("fromwap && pppopen�����̹Ƿ� return��", 0, 0, 0);
		g_fFromWAP = FALSE;
        EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);
        return; // �ϴ� return
    }
	g_fFromWAP = FALSE;

    //if (SKY_IS_FROM_WAP()) {    // WAP => GVM
    if (NetworkGetState() == NET_PPPOPEN_S) 
	{
        error("�ϴ� pppopen�����̹Ƿ� return��", 0, 0, 0);
        EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);
        return; // �ϴ� return
    }    
    
    error("���� pppopen�� �Ѵ�.-%d", (int)NetworkGetState(), 0,0);    
    
    // pppopen
	//midd_pppopen();
	if(PPPOPEN() == DSS_ERROR)
	{
        if(NetworkGetState() == NET_PPPOPENING_S) // ������ ��ٸ��� ��  
        {
			error("ppp_open wouldblock! ", 0,0,0);
            return;
		}
		else   // ���� ����!
        {
			error("ppp_open error occurred! ",0,0,0);	
            //EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA);
            return;
		}        
	}
}


//==============================================================
//	midd_Connect2 : make socket & connect <- made by undersea(sktt)
//
//  called after SE_NET_CONNECTED event
//
//	return		connect result (success:> DSS_ERROR, fail:DSS_ERROR:-1)
//
int midd_Connect2(void) {
    
    struct  sockaddr_in addr;
    struct  hostent *hostentry = NULL ;    

    /* socket initialize */
    SetKindOfService(GVM);
    memset((char *)&addr, 0, sizeof(struct sockaddr_in));	

    /* dns query */
	hostentry = gethostbyname((const char*)szSvrIP);
	if(hostentry == NULL)
	{
		error("gethostbyname error",0,0,0);
        return DSS_ERROR;
	}
	// for dns query error. added 11/15/2001
    addr.sin_addr.s_addr = get32( (unsigned char *)(hostentry->h_addr_list[0]) );
	//memcpy( &addr.sin_addr, hostentry->h_addr_list[0], sizeof(struct in_addr) );	// ���� ���

	error("REMOTE ADDRESS [0x%x%x]", (int)((addr.sin_addr.s_addr)>>16), 
									    (int)addr.sin_addr.s_addr, 0);
    /* make socket */
    if (gvm_sock_fd == DSS_ERROR) {
        
        /* Creation Socket */
        gvm_sock_fd = socket((byte)AF_INET, (byte)SOCK_STREAM, (byte)IPPROTO_TCP);

        if (gvm_sock_fd == DSS_ERROR) 
        {
		    error("fail to open socket [%x]", gvm_sock_fd, 0, 0);	
            return DSS_ERROR;
	    }
    }
    else 
    {
        error("SOCKET Already exists!", 0, 0, 0);        
    }
    error("make socket  <== midd_Connect2!!!!", 0, 0, 0);
	
    /* set others */
	addr.sin_family      = AF_INET;           //0
	addr.sin_port        = dss_htons(nSvrPort); //HOST_PORT);//9203 or alternative
	addr.sin_addr.s_addr = dss_htonl(addr.sin_addr.s_addr); 

	/* Connect */
	if(connect(gvm_sock_fd, (const struct sockaddr *)&addr, sizeof(struct sockaddr)) == DSS_ERROR)
	{
		error("TCP connection fail",0,0,0);        
		return DSS_ERROR;
	}
	error("TCP connection success",0,0,0);
    return DSS_SUCCESS;
}


//==============================================================
//	Disconnect TCP/IP socket connection
//
//	ret		result report flag
//
//	After disconnection process, 
//	the disconnection result was called mmiSwapMenu
//
//		mmiEvent.code = SWAP_EVENT_DISCON_OK;
//		mmiSwapMenu(EVENT_S, 0);
//
void midd_Disconnect(int ret)
{  
    error("[midd_Disconnect] ret:%d -------------", ret, 0, 0);
	/*if (g_eDisconnectType != DT_NONE && NetworkGetState() == NET_CLOSING_S)
	{
		error("�̹� �ݰ� �ִ� ��???? ret=%d", ret, 0, 0);
	}*/

    switch (ret) 
    {
	    case SWAP_RR_DISCONNECT:    // SWAP_RC_DISCONNECT��û�� ����..
            error("disconnect ��û~~~", 0, 0, 0);
            if(g_eDisconnectType == DT_NONE)
            {
                g_eDisconnectType = DT_DISCONNECT;
            }
            break;
	    case SWAP_RR_DISCONNECT_DLSSBERR:  // 1
            error("Flash-SSB�� �� slot�� ���� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;
	    case SWAP_RR_DISCONNECT_DLMEMERR:  // 2
            error("Flash-SSB�� ��������� ���� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
            break;
	    case SWAP_RR_DISCONNECT_DLSCRERR:   // 3
            error("�߸��� ��ũ��Ʈ�� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;  
	    case SWAP_RR_DISCONNECT_DLSIZEERR:  // 4
            error("������ �ʰ��ϴ� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;
	    case SWAP_RR_DISCONNECT_DLSUMERR:   // 5
            error("checksum ������ ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
            break;
	    case SWAP_RR_DISCONNECT_DLAUTHERR:  // 6
            error("���� ������ ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;
	    case SWAP_RR_DISCONNECT_DLPKTERR:   // 7
            error("��Ŷ �ν��� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;
	    case SWAP_RR_DISCONNECT_DLCOMP:  // 8
            error("�ٿ�ε� ���� �� Disconnect~", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
            g_nRunningType = GRT_DOWNLOAD_SUCCESS;      			
            break;
	    case SWAP_RR_DISCONNECT_DLRETRYFAIL:    // 9
            error("retry�� ������ ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;
	    case SWAP_RR_DISCONNECT_DLCALLDROP: // 10
            error("�ٿ�ε� ���� -> DL call drop", 0, 0, 0);
            g_eDisconnectType = DT_CALLDROP;
		    break;
	    case SWAP_RR_DISCONNECT_DLEXIT: // 11
            error("�ٿ�ε� ���� ����Ű���� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
            break;
	    case SWAP_RR_DISCONNECT_BROWSER:    // 12
            error("wap���� ���ͽ�û�� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_TOWAP;
		    break;
	    case SWAP_RR_DISCONNECT_CALLDROP:   //13
            error("������ũ��Ʈ���� call-drop�� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_CALLDROP;
		    break;
	    case SWAP_RR_DISCONNECT_SVREXIT:    // 14
            error("������ũ��Ʈ ���� �� ����Ű ���� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;
	    case SWAP_RR_DISCONNECT_EXIT:       // 15
            error("������ũ��Ʈ���� exception�߻��� ��� disconnect ��û", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
		    break;
	    default:
            error("�Ӿ�?-ret:%d", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
            break;        
	}


#ifdef FEATURE_SKT_EIF
	if(g_EIF_WAP_GVMINFO)
	{
		memset(resultBuf, 0x00, sizeof(resultBuf));
		if (ret == SWAP_RR_DISCONNECT_DLCOMP)	// �ٿ�ε� ����
		{
			BYTE szFileName[SWAP_LEN_GNAME+1];
			int nFileKSize = 0;	//Kbyte����
			int nFileSize = (int)SKY_GET_GVM_GAME_SIZE(g_nCurSSB); // size(bytes)

			memset(szFileName, 0, SWAP_LEN_GNAME+1);
			
			//���� ����Ǵ� ���� �̸�
			GetGVMFileName((BYTE*)szFileName, (unsigned short)SKY_GET_GVM_GAME_CPID(g_nCurSSB), (unsigned short)SKY_GET_GVM_GAME_ID(g_nCurSSB));
			// ���� ����
			//STRNCPY((BYTE *)szFileName, (BYTE *)SKY_GET_GVM_GAME_NAME(g_nCurSSB), SWAP_LEN_GNAME);

			nFileKSize = nFileSize/1000;
			if(nFileKSize == 0)	//1K���� ���� ũ��
			{
				nFileKSize = 1;			
			}
			else	//1K�̻�
			{
				if((nFileSize%1000) > 0)	nFileKSize ++;	
			}

			CLNTa_log(1,0, "GVMINFO[����]: name[%s], size[%d] ", szFileName, nFileKSize);

			sprintf(resultBuf, "*SKT*GVMINFO:1,%s,%d", szFileName, nFileKSize);
			at_send_extended_text(resultBuf);		

		}
		else //����
		{
			if (g_nRunningType == GRT_DOWNLOAD_START || g_nRunningType == GRT_DOWNLOADING)	// �ٿ�ε� �� ����
			{
    			CLNTa_log(1,0, "GVMINFO : ����");
				at_send_extended_text("*SKT*GVMINFO:0[Downloading failure]");
    		}
		}
	}
#endif
    
    SKY_SNI(HS_SNI_CALL_ALERT, FALSE);
    SKY_MotorStop();
    SKY_StopMusic();    

    // close socket
	midd_closesocket();
    
    if (g_eDisconnectType == DT_TOWAP && P_REV_IS_6_OR_GREATER)  // 95C & GVM->WAP
    {
        if (gvm_sock_fd != DSS_ERROR)   // ���� �������� �ѹ� �� Ȯ��
        {
            midd_closesocket();
        }
        EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA);
        return;
    }
    	
	// SWAP��⿡�� �ٷ� midd_Disconnect�Լ��� �θ��� �����Ƿ� �� ��쿡 ����Ͽ�
	// pppclose�ٷ� ���� �θ����� �Ѵ�.
	SKY_SetGeneralTimer(PPP_TIMEOUT, FALSE);
	
	//midd_pppclose();	
	// ppp close
	if (NetworkGetState() == NET_NULL_S || NetworkGetState() == NET_CLOSED_S) 
    {
        error("ppp:NULL or CLOSED!!!", 0, 0, 0);
		EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA);	// ��.. �־�� �ϴ°� ���°�.. -_-;
		return;
        //return DSS_SUCCESS;
    }
    else 
    {        
        if(PPPCLOSE() == DSS_ERROR)
	    {
		    error("PPP CLOSE fail",0,0,0);
			return;
            //return DSS_ERROR;
	    }
    }
    error("PPP close success!!!", 0, 0, 0);    
    return;

}

//==============================================================
//	midd_closesocket : close socket <- made by undersea(sktt)
//
//  call closesocket function
//
//	return		close result (success:1, fail:0)
//
int midd_closesocket(void)
{
	sint15 ret = DSS_ERROR;
	
    if (gvm_sock_fd != DSS_ERROR) 
    {
        // socket close
        ret = closesocket(gvm_sock_fd);
        if(ret == DSS_ERROR)
	    {
		    error("close socket fail -> retry!!!",0,0,0);
			ret = midd_closesocket();
			error("close retry result :%d", ret, 0, 0);
	    }
        else 
        {
            gvm_sock_fd = DSS_ERROR;
            error(">>>>>> socket close �Ϸ�",0,0,0);
        }
    }
    else ret = DSS_SUCCESS;

    return ret;
}


//==============================================================
//	Send data via TCP/IP socket connection
//
//	data	network data to be sent
//	len		data length
//
//	return		1 = Success, 0 = Fail
//

int midd_SendData(BYTE *data, BYTE len)
{
    sint15 ret = DSS_ERROR;

    /*-----------------------------------------------------------------------
       TCP packet is now ready - process it according to psi_state 
    -----------------------------------------------------------------------*/    
    if ((int)len > 0 && gvm_sock_fd != DSS_ERROR && data != NULL) 
    {
		if (strncmp((const char*)data, (const char*)"SGFQ", 4) == 0) 
		{
			error("Send RRQ now #############", 0, 0, 0);
			CLNTa_log(1, 0, "######### %s", data);
		}
		else if (strncmp((const char*)data, (const char*)"SGFR", 4) == 0)
			error("Send RELEASE now #############", 0, 0, 0);
		else if (strncmp((const char*)data, (const char*)"SGFN", 4) == 0)
			error("Send NAK now #############", 0, 0, 0);

 		ret = send(gvm_sock_fd, (const char*)data, len, 0);
	}

    if (ret == DSS_ERROR || ret == 0) // �ϴ� 0�� ������ ����
    {
        error("send data FAIL!!!(len:%d)", len, 0, 0);
        return 0;       
    }
    else 
    {        
		error("send data success!!!(len:%d)", len, 0, 0);
        return 1; // ����
    }
}

//==============================================================
//	GVM_SocketStateChanged  <- made by undersea(sktt)
//
//  to read data from socket
//  read event occurred -> ProcessSocketCallback -(GVM)-> GVM_SocketStateChanged
//
void GVM_SocketStateChanged(void)
{
#define nChar (536*2)

	int sock_event;
    sint15  ret = DSS_ERROR;
    BYTE	acRcvBuf[nChar+1]; // Data Receive Buffer
    S_Event event;
    S_Result *result; 
    
    //  Should be changed for Application to call CheckSocketState fn.
    if (gvm_sock_fd == DSS_ERROR) 
        return;	
		
	sock_event = CheckSocketState(gvm_sock_fd);
	if (sock_event == 0)
	{
		error("no event", 0, 0, 0);
		return;
	}
	else if (sock_event == 1) // receive data
	{
    	//EQS_SEND_EVENT(SE_RECV_DATA, EQS_NA, EQS_NA);

		memset(acRcvBuf, 0, nChar+1);
        ret = recv(gvm_sock_fd, (char*)acRcvBuf, nChar, 0); //0=unsuppot flag  
        if (ret == 0 || ret == DSS_ERROR) 
        {
			DisableReadEvent(gvm_sock_fd);
        }
		else
		{
			EnableReadEvent(gvm_sock_fd);
		//}			
			//CLNTa_log(1, 0, "0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", acRcvBuf[0],acRcvBuf[1],
			//	acRcvBuf[2],acRcvBuf[3],acRcvBuf[4],acRcvBuf[5],acRcvBuf[6],acRcvBuf[7],acRcvBuf[8],acRcvBuf[9]);

			/*if (strncmp((const char*)acRcvBuf, (const char*)"SGFD", 4) == 0)	
				error("Data Received  **********************", 0, 0, 0);*/

			event.code = SWAP_EVENT_MENU;
			event.param.menu.event = SWAP_MENU_RECEIVE;
			event.param.menu.buf = (unsigned char*)acRcvBuf;
			event.param.menu.data = ret;
			result = scl_DoSwapEvent(&event);
			if (result->code != SWAP_RC_NONE) 
			{
				recv_result = result;
				EQS_SEND_EVENT(SE_RECV_DATA, EQS_NA, result->code);				
			}
		}
	}
	else if (sock_event == 3)
	{
		error("sock closed?????????", 0, 0, 0);		
	}
    return;
}


/*
    RECV ����

*/
void midd_recvdata(char* buf, int len, int *recv_length) {

    sint15  ret = DSS_ERROR;

	//You should check bearer and destPort
    if (gvm_sock_fd != DSS_ERROR) 
    {        
        ret = recv(gvm_sock_fd, buf, len, 0); //0=unsuppot flag  

        if (ret == 0 || ret == DSS_ERROR) 
		{
			DisableReadEvent(gvm_sock_fd); 
            ret =  DSS_ERROR;
		}
        else 
            EnableReadEvent(gvm_sock_fd);   // ���������� �а� �ϱ� ����.    
    }
    else 
    {
        error("[midd_recvdata]SOCKET is NULL!!!", 0, 0, 0);        
    }

    *recv_length = ret;
}

//==============================================================
//	Connect PTP connection by call
//
//	dial	PTP dial number
//
#ifdef SWAP_FEATURE_PTP
void midd_PtpCall(char *dial)
{
	error("[midd_PtpCall] called=================", 0, 0, 0);
	PtpLinkConnect(DS_PIWF_SRVC, dial);	
}
#endif // SWAP_FEATURE_PTP

//==============================================================
//	Connect PTP connection by wait
//
//	After connection process,
//	the connection result was called mmiSwapMenu
//
//		mmiEvent.code = SWAP_EVENT_PTP_RESULT;
//		mmiEvent.param.data = 1;	// 1=Success, 0=Fail
//		mmiSwapMenu(EVENT_S, 0);
//
#ifdef SWAP_FEATURE_PTP
void midd_PtpWait()
{
	error("midd_PtpWait �Ҹ� -------------", 0, 0, 0);
	PtpLinkWait(DS_PIWF_SRVC);	
}
#endif // SWAP_FEATURE_PTP

//==============================================================
//	Release PTP connection
//
//	ret		result report flag
//
//	After release process, 
//	the release result was called mmiSwapMenu
//
//		mmiEvent.code = SWAP_EVENT_PTPDISCON_OK;
//		mmiSwapMenu(EVENT_S, 0);
//
#ifdef SWAP_FEATURE_PTP
void midd_PtpRelease(int ret)
{
    //ds_cmd_type ds_cmd_item;                    /* item to hold cmd to dsmgr */

    error("midd_PtpRelease �Ҹ� -------------", 0, 0, 0);
    
    switch(ret) 
    {
        case SWAP_RR_PTPDISCONNECT: // ��ũ��Ʈ���� ��û
            error("<<��ũ��Ʈ���� ��û - PTPDISCONNECT>>", 0, 0, 0);     
            g_eDisconnectType = DT_DISCONNECT;
            break;
        case SWAP_RR_PTPDISCONNECT_WAIT: // ���ſ��� ��ҿ�û
            error("<<���ſ��� ��ҿ�û - PTPDISCONNECT_WAIT>>", 0, 0, 0);
            g_eDisconnectType = DT_WAIT;
            break;
        case SWAP_RR_PTPDISCONNECT_RUNWAIT: // ���� �� ���ſ��� ��ҿ�û
            error("<<������ ���ſ��� ��ҿ�û - PTPDISCONNECT_RUNWAIT>>", 0, 0, 0);
            g_eDisconnectType = DT_WAIT;
            break;
        case SWAP_RR_PTPDISCONNECT_BROWSER: // ���� �� WAP������ ���Ϳ�û�߻�
            error("<<������ WAP���Ϳ�û�߻� - SWAP_RR_PTPDISCONNECT_BROWSER>>", 0, 0, 0);
            g_eDisconnectType = DT_TOWAP;
            break;
        case SWAP_RR_PTPDISCONNECT_CALLDROP: // PTP ������ call-drop�� �߻�
            error("<<PTP ������ call-drop - PTPDISCONNECT_CALLDROP>>", 0, 0, 0);
            g_eDisconnectType = DT_CALLDROP;
            break;
        case SWAP_RR_PTPDISCONNECT_PTPEXIT: // ������ ����Ű �������
            error("<<������ ����Ű : ptpdisconnect_PTPEXIT>>", 0, 0, 0);
            g_eDisconnectType = DT_DISCONNECT;
            break;
        case SWAP_RR_PTPDISCONNECT_EXIT: // ������ Exception�߻�            
            error("<<������ Exception�߻� : ptpdisconnect_EXIT>>", 0, 0, 0);            
            g_eDisconnectType = DT_DISCONNECT;
            break;
        default:
            g_eDisconnectType = DT_DISCONNECT;
            break;
    }
    
	error( "UI=>DS: DS_EXIT_AIRDATA_MODE_CMD", 0, 0, 0);
	PtpLinkRelease();
	
    SKY_SNI(HS_SNI_CALL_ALERT, FALSE);
    SKY_MotorStop();
    SKY_StopMusic();
    
    if (ret == SWAP_RR_PTPDISCONNECT_WAIT || ret == SWAP_RR_PTPDISCONNECT_CALLDROP) 
    {
        error("���������� end �̺�Ʈ ����", 0, 0, 0);
        EQS_SendEvent(SE_PTP_RELEASED, EQS_NA, EQS_NA);
    }
	 // ���Ŵ���� end key������ �� ���� �������� ����Ǵ� ���
	else if (g_nRunningType == GRT_RUN_PTPCALL || g_nRunningType == GRT_RUN_PTPWAIT || g_nRunningType == GRT_NONE)
	{
		error("���������� end �̺�Ʈ ����2", 0, 0, 0);
        EQS_SendEvent(SE_PTP_RELEASED, EQS_NA, EQS_NA);
	}
}
#endif // SWAP_FEATURE_PTP

//==============================================================
//	Send data via PTP connection
//
//	data	network data to be sent
//	len		data length
//
//	return		1 = Success, 0 = Fail
//
//	After packet sending process,
//	the sending result was called mmiSwapMenu
//
#ifdef SWAP_FEATURE_PTP
int midd_PtpSendData(BYTE *data, BYTE len)
{
	PtpLinkSendbyPIWF(data, (uint16)len);
    return 1;
}

//==============================================================
//	receive data from ptp Queue <- added by undersea (sktt)
//
//  data    data buffer
//  length  read size
//
void midd_PtpRecvData(byte* data, int* length)
{
	uint16 len;
	len = PtpLinkRcv(data);
	*length = len;	
    
	error("PtpLinkRcv>> [%d]",*length, 0, 0);
}

//==============================================================
//	Clean Ptp Receive Queue <- added by undersea (sktt)
//
/*void	midd_cleanPtpDataQueue
(
	void
)
{
	dsm_item_type	*item_ptr;

	while(( item_ptr = (dsm_item_type *)q_get(&ptpRecvDataQ)) != NULL)
	{
		dsm_free_buffer(item_ptr);
	}
}*/

#endif // SWAP_FEATURE_PTP

#endif // SWAP_FEATURE_NETWORK

//==============================================================
//	Get chatting reserved text from NV-ROM
//
void midd_GetChatText()
{
    /*for (i = 0 ; i < GVM_LEN_CHATTEXT ; i++) 
    {
        memset(svRsvdChat[i], 0, SWAP_RSVD_CHAT_SIZE+1);
        STRNCPY((BYTE*)svRsvdChat[i], (BYTE*)SKY_GET_GVM_CHATTEXT(i), SWAP_RSVD_CHAT_SIZE);
    }*/
	memset(svRsvdChat[0], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[0], (BYTE*)SKY_GET_GVM_CHATTEXT(0), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[1], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[1], (BYTE*)SKY_GET_GVM_CHATTEXT(1), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[2], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[2], (BYTE*)SKY_GET_GVM_CHATTEXT(2), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[3], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[3], (BYTE*)SKY_GET_GVM_CHATTEXT(3), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[4], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[4], (BYTE*)SKY_GET_GVM_CHATTEXT(4), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[5], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[5], (BYTE*)SKY_GET_GVM_CHATTEXT(5), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[6], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[6], (BYTE*)SKY_GET_GVM_CHATTEXT(6), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[7], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[7], (BYTE*)SKY_GET_GVM_CHATTEXT(7), SWAP_RSVD_CHAT_SIZE);
	memset(svRsvdChat[8], 0, SWAP_RSVD_CHAT_SIZE+1);
    STRNCPY((BYTE*)svRsvdChat[8], (BYTE*)SKY_GET_GVM_CHATTEXT(8), SWAP_RSVD_CHAT_SIZE);
}

//==============================================================================
//
//	RS232C API
//
//==============================================================================
#define	TO_HEX(n)	(((n) >= 10) ? 'A' + (n) - 10 : '0' + (n))
#define	TO_NUM(n)	(((n) >= 'A') ? 10 + (n) - 'A' : (n) - '0')
#define MIDD_SEND_DATA_SIZE     100

//==============================================================
//	TX RS-232C command and data
//
//	code	command byte
//	size	data size
//	data	data pointer
//
// �ܸ��� -(RS-232C)-> TE2
#ifdef SWAP_FEATURE_SERIAL
void midd_RsCom(BYTE code, int size, BYTE *data)
{
    STATIC  char	buf[MIDD_SEND_DATA_SIZE*2 + 1];
	STATIC  int		i;

	for (i = 0; i < size; i++) 
    {
		buf[i*2] = TO_HEX(*data >> 4);
		buf[i*2 + 1] = TO_HEX(*data & 0x0F);
		data++;
	}
	buf[i*2] = '\0';
	sprintf((char*)asciiBuffer, "*SWAP*COM:%d,%s", code, buf);
	at_send_extended_text((char *)asciiBuffer);
}
#endif // SWAP_FEATURE_SERIAL

//==============================================================
//	TX RS-232C data
//
//	size	data size
//	data	data pointer
//
#ifdef SWAP_FEATURE_SERIAL

void midd_RsDownload(int size, BYTE *data)
{   
    STATIC  char	buf[MIDD_SEND_DATA_SIZE*2 + 1];
	STATIC  int		i;
	STATIC  dword   nSendPtr;
	STATIC  BYTE    *pSendPtr;

    nSendPtr = 0;
    pSendPtr = (BYTE *)data;

    while (nSendPtr + MIDD_SEND_DATA_SIZE < size)
    {
	    for (i = 0; i < MIDD_SEND_DATA_SIZE; i++) 
        {
		    buf[i*2] = TO_HEX(*pSendPtr >> 4);
		    buf[i*2 + 1] = TO_HEX(*pSendPtr & 0x0F);
		    pSendPtr++;
	    }
	    buf[i*2] = '\0';
	    sprintf((char*)asciiBuffer, "*SWAP*DLOAD:%d,%d,%s", (int)size, (int)nSendPtr, buf);
	    at_send_extended_text((char *)asciiBuffer);

        nSendPtr += MIDD_SEND_DATA_SIZE;
        pSendPtr = (BYTE *)data + nSendPtr;
    }

	for (i = 0; i < size - nSendPtr; i++) 
    {
		buf[i*2] = TO_HEX(*pSendPtr >> 4);
		buf[i*2 + 1] = TO_HEX(*pSendPtr & 0x0F);
		pSendPtr++;
	}
	buf[i*2] = '\0';
	sprintf((char*)asciiBuffer, "*SWAP*DLOAD:%d,%d,%s", (int)size, (int)nSendPtr, buf);
	at_send_extended_text((char *)asciiBuffer);

}
#endif // SWAP_FEATURE_SERIAL

//==============================================================================
//
//	Common API
//
//==============================================================================

//==============================================================
//	Send SMS
//
//	ti		Teleservice ID
//	dial	Receiver dial string
//	data	Send text data (null terminated)
//
//	return	1 = send request success, 0 = fail
//			(not send result but request result)
//
//	After send process,
//	the result was called mmiSwapMenu
//
//		mmiEvent.code = SWAP_EVENT_SMS_RESULT;
//		mmiEvent.param.data = 1;	// 1=Success, 0=Fail
//		mmiSwapMenu(EVENT_S, 0);
//
int midd_SendSMS(WORD ti, char *dial, char *data)
{
    STATIC  BYTE svMIN[SWAP_LEN_DIAL+1];

    error("midd_SendSMS �Ҹ� -------------", 0, 0, 0);

    // Send SMS data
    if (g_GVMSmsMoType != GSMT_IDLE) 
	{
        error("����� IDLE�� �ƴϹǷ� �߽� �ȵ�", 0, 0, 0);
		return 0;
	}

    // get phone number
    memset(svMIN, 0, SWAP_LEN_DIAL+1);
    midd_GetMIN((char*)svMIN);

    if (svMIN[0] == NULL || data[0] == NULL) 
    {
        error("data null!!!->�߽� �ȵ�", 0, 0, 0);
        return 0;    
    }
    
    // data setting
    memset(&g_GVMSmsBuffer, NULL, sizeof(SMSMO_SEND_T));
    if (strlen(data) > MAX_SMS_TEXT) 
        g_GVMSmsBuffer.nMsgSize = MAX_SMS_TEXT;
	else 
        g_GVMSmsBuffer.nMsgSize = strlen(data);

	strncpy((char *)g_GVMSmsBuffer.pszMsg, data, MAX_SMS_TEXT);
	g_GVMSmsBuffer.nRcvNumberSize = strlen(dial);
	STRNCPY(g_GVMSmsBuffer.pszRcvNumber, (BYTE*)dial, SWAP_LEN_DIAL);
    
    g_GVMSmsBuffer.nSendNumberSize = STRLEN(svMIN);
    STRNCPY(g_GVMSmsBuffer.pszSendNumber, svMIN, SWAP_LEN_DIAL);

	g_GVMSmsMoType = GSMT_SENDING;
	g_GVMSmsMoTI = ti;

    // set timer & retry num
    error("[SMS] Now Send SMS~~", 0, 0, 0);
    g_unTrySMS = 0;
	// ���� ���� call type�� SMS��� ���� ���� SMS�� �������� �ϴ� ���̶� ��
	if (IsCallTypeSMS())	
	{
		uasms_DTC_disconnect();
	}    

    EQS_StopTimer(EQS_TIMER_2);
    EQS_StartTimer(EQS_TIMER_2, 10*1000, FALSE);

	SendSMSMsg(&g_GVMSmsBuffer, g_GVMSmsMoTI);    
	return 1;
}

//==============================================================
//	Return to browser
//
//	URL		browset connect URL
//
void midd_BackToBrowser(char *URL)
{
    HCONTROL	hMessageBox;
    
	if(URL[0] != NULL && STRCMP((BYTE*)URL, (BYTE*)"") != 0 )
	{
        SKY_SET_TO_WAP(TRUE);
        
        /* set the WAP url -> ���߿� �ٲܰ�(WAP 4.8) */ 
        memset(g_szGoToURL, NULL, MAX_GOTOURL_LEN);
        STRNCPY(g_szGoToURL, (CONST BYTE*)URL, MAX_GOTOURL_LEN-1);
        		
        hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_MESSG_RECONNECT_NTOP),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE,
                                        MBT_SUCCESS); //MBT_HELP); //CONFIRM);
        EQC_SET_STYLE(hMessageBox, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);

        EQC_SET_USERDATA(hMessageBox, SE_SOFTKEY_WAP);

	}
    else // url�� NULL�̰ų� ""�� ��� ������ ����������.
    {
        hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_MESSG_CONFIRM_EXIT),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE,
                                        MBT_SUCCESS); //CONFIRM);

        EQC_SET_USERDATA(hMessageBox, SE_KEY_END);
    }
	return;
}

//==============================================================
//	Get mobile idetification number
//
//	minStr	MIN buffer, null ternimated
//
void midd_GetMIN(char* minStr)
{
    nv_dir_number_type  nv_dir_num; /* current nam name */
    STATIC  int i;

    // ��ȭ��ȣ ����
	nv_dir_num.nam = SKY_GET_CURRENT_NAM();
    (void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );
    if (nv_dir_num.dir_number[0] == '0')
    {
        STRNCPY((BYTE *)minStr, (BYTE *)nv_dir_num.dir_number, 10);
        i=10;
    }
    else
    {
        STRNCPY((BYTE *)&(minStr[1]), (BYTE *)nv_dir_num.dir_number, 10);
        minStr[0] = '0';
        i=11;
    }    
    for (;i<STRLEN((unsigned char*)minStr);i++) minStr[i]='\0';
}

//==============================================================
//	Voice call
//
//	dial	receiver dial number
//	ret		1 = continue after voice call
//			0 = no action after voice call
// 
//	After voice call release, if the ret set to 1
//	the release result was called mmiSwapMenu
//
//		mmiEvent.code = SWAP_EVENT_CONTINUE;
//		mmiSwapMenu(EVENT_S, 0);
//
void midd_VoiceCall(char *dial, int ret)
{
    UINT8   nNumOfDial;
    
    nNumOfDial = strlen(dial);
    if(dial[0] != '\0')
    {             
      SET_NUMEDIT_BUFFER((BYTE*)dial, nNumOfDial);
      SET_NUMEDIT_POS(nNumOfDial);  

      if (ret) 
          g_fGVMPaused = TRUE;
      else 
          g_fGVMPaused = FALSE;

      SKY_SET_FROM_GVM(TRUE);
      sSetColorDisplay(FALSE);
	  //ClearScreen();
      EQS_START_CHILD(SKA_StartOrigination);
    }
}

//==============================================================
//	Get location info
//
//	loc		location info buffer
//
void midd_GetLocInfo(WORD* loc)
{
	db_get(DB_SID, &dbi);
	*loc      = dbi.sid;	// SID 15bit
	db_get(DB_NID, &dbi);
	*(loc+ 1) = dbi.nid;	// NID 16bit
	db_get(DB_REG_ZONE, &dbi);
	*(loc+ 2) = dbi.reg_zone;	// REG_ZONE 12bit
	db_get(DB_BASE_ID, &dbi);
	*(loc+ 3) = dbi.base_id;	// BASE_ID // BASWAP_EVENT_ID 16bit	???
	db_get(DB_PILOT_PN_OFF, &dbi);
	*(loc+ 4) = dbi.pilot_pn_off;	// PILOT_PN 9bit
}

//==============================================================
//	Get current date
//
//	year	call by reference year (2000..2099)
//	month	call by reference month (1..12)
//	day		call by reference day (1..31)
//	wday	call by reference weekday (0..6, 0=Sunday)
//
//	return	1 = valid, 0 = invalid
//
int midd_GetDate(INT2 *year, INT2 *month, INT2 *day, INT2 *wday)
{
	DWORD current_time;
    clk_julian_type	julian;

    current_time = SKY_GetLocalTime();
    if (current_time == SKY_INVALID_SYS_TIME) 
        return 0;

    clk_secs_to_julian( current_time, &julian);
	*year =	julian.year;
    *month = julian.month;
    *day = julian.day;
	*wday = julian.day_of_week;
	if (*wday < 6) *wday += 1;
    else if (*wday >= 6) *wday = 0;

    return 1;
}

#define _MILISECOND
//==============================================================
//	Get current time
//
//	hour	call by reference hour (0..23)
//	min		call by reference minute (0..59)
//	sec		call by reference second (0..59)
//	msec	call by reference milisec (0..999, if available)
//
//	return	1 = valid, 0 = invalid
//
int midd_GetTime(INT2 *hour, INT2 *min, INT2 *sec, INT2 *msec)
{
	// Get current date

	DWORD current_time;
    clk_julian_type	julian;

    current_time = SKY_GetLocalTime();
    if (current_time == SKY_INVALID_SYS_TIME) 
        return 0;

    clk_secs_to_julian( current_time, &julian);
    *hour = julian.hour;
    *min = julian.minute;
	*sec = julian.second;
    *msec = 0;

    return 1;
}

//==============================================================
//	Store audio as paging bell
//
//	name	audio name
//	data	audio data
//	size	audio size
//
void midd_RegMusicBell(char *name, BYTE *data, int size)
{
    char			musicTitle[22];
	dword			mbSize;
	boolean         IsValid = FALSE;	//Title�� ���� ����ȭ���� ���������ϴ��� ����

    extern byte *mb_MusicBellPreBuffer;	//ma1������ musicbell�� �����ϴ� RAM�� buffer -> ma2???
    extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);
	extern byte get_length( byte *chk_str, byte buff_length);
	extern BOOL WAP_IsValidFile(SKY_DATA_FILE_E SkyDataFile, CONST BYTE* pszFileName);

	//Invalid parameter check!!
	if (name == NULL)
	{
		error("[midd_RegMusicBell]-name is Invaild",0,0,0);
		return;
	}

	// if music name is over 14 then truncation...
	memset(musicTitle, NULL, 22);
	WAP_HanCopy(musicTitle, (char *)name, get_length((byte *)name, SKY_FILE_NAME_MAX_LENGTH-1));

	// ����Ÿ�� Null�̰ų� ����Ÿ�� ���̰� 0�� ���, ���� ����
	if(data == NULL || size <= 2)
    {
        error("[midd_RegMusicBell]-Data is Null",0,0,0);
        return;
	}
	else
	{
		if(mb_MusicBellPreBuffer)
        {
			SKY_Free((void*)mb_MusicBellPreBuffer);
			mb_MusicBellPreBuffer =NULL;
		}

        mb_MusicBellPreBuffer = (byte *)SKY_Malloc(size+5);
        if(mb_MusicBellPreBuffer == NULL) 
		{
			error("[midd_RegMusicBell]-Wip Malloc Fail",0,0,0);
			return;
		}
		memset(mb_MusicBellPreBuffer, 0x00, size+5);
		memcpy(mb_MusicBellPreBuffer, "MMF ", MM_NUM_FILE_TYPE);   //(B0���� �ش�)phone�� �����ϱ� ���� header(4 bytes)
		memcpy(mb_MusicBellPreBuffer+MM_NUM_FILE_TYPE, data, size);

		//file�� ������ full���� check!!
        if(SKY_GetFileNumberInDataFile(SDF_MELODY) >= SKY_MAX_NUM_FILES)        
        {
			error("[midd_RegMusicBell]-Filenum_Full",0,0,0);			
		}
		else
		{	
            mbSize = GetLengthofMusicData((int)YAMAH_mmf, size, MUSICBELL_DOWNLOAD_MODE);            
            if(mbSize == 0) 
			{
				return;
			}

			//name�� ���� �����ϴ��� check!! -- �����ϸ� delete��, ���� 
			IsValid = WAP_IsValidFile(SDF_MELODY, (BYTE *)musicTitle);
			if(IsValid)		//�����ϴ� ���
				SKY_DeleteDataFile(SDF_MELODY,(BYTE *)musicTitle);

			// �������
            if(SKY_WriteDataFile(SDF_MELODY, (BYTE*)musicTitle, mbSize, mb_MusicBellPreBuffer) != SDR_OK)
			{
				error("[midd_RegMusicBell]-Save Fail",0,0,0);
			}
            else 
            {
                // ���ź� �����ϱ�!~~~~~~~~~~~~~~~~~
                SKY_SET_BELL_FILE_NAME(musicTitle);
                SKY_SET_BELL_ID(SKY_MY_BELL_ID);
			}	//end of ������ ���� �ִ� ���
		}	//end of file num not full  

		if(mb_MusicBellPreBuffer)
		{
			SKY_Free((void*)mb_MusicBellPreBuffer);
			mb_MusicBellPreBuffer =NULL;
		}

	}	//end of if(data != NULL)
	
}

//==============================================================
//	Store LCD as background image
//
//	name	image name
//	data	image data
//
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
void midd_RegScreen(char *name, WORD *data)
#else
void midd_RegScreen(char *name, BYTE *data)
#endif  // (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
{

    PICTURE_TYPE_T      GVMImage;
	PICTURE_IN_CASE_T   CaseImage;
    BOOL				fDeleted = FALSE;
	BYTE				abImageTitle[SKY_FILE_NAME_MAX_LENGTH+1];

	/* check data */
	if(data == NULL)
	{
		error("[midd_RegScreen]Image Data Null",0,0,0);
		return;
	}

	/* check disk space */
	if(!CheckDiskSpaceForSetGVMImage())
	{
		error("[RegScreen]CheckDisk ERROR",0,0,0);
		return;
	}

	/* set image title */
	if (name == NULL)
		STRCPY(abImageTitle, (BYTE*)"�̸�����");
		
	if(strlen(name) > SKY_FILE_NAME_MAX_LENGTH)
	{
		if( IsMiddleOfMultiByte((BYTE *)name, SKY_FILE_NAME_MAX_LENGTH) )
		{
			memcpy(abImageTitle, (char *)name, SKY_FILE_NAME_MAX_LENGTH-1);
			abImageTitle[SKY_FILE_NAME_MAX_LENGTH-1] = '\0';
		}
		else
		{
			memcpy(abImageTitle, (char *)name, SKY_FILE_NAME_MAX_LENGTH);
			abImageTitle[SKY_FILE_NAME_MAX_LENGTH] = '\0';
		}		
	}
	else
	{
		memcpy(abImageTitle, (char *)name, strlen(name));
		abImageTitle[strlen(name)] = '\0';
	}

	/* check if the file exists */
	if(SKY_IsDataFile(SDF_IMAGE, (BYTE *)abImageTitle) == TRUE)
    {
		CaseImage.Idle = SKY_GET_IDLE_PICTURE(); 
		CaseImage.IncomingCall = SKY_GET_INCOMING_PICTURE();
		CaseImage.PowerOn = SKY_GET_POWER_ON_PICTURE(); 
		CaseImage.PowerOff = SKY_GET_POWER_OFF_PICTURE();
		CaseImage.WAPStart = SKY_GET_WAP_PICTURE();

        SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)abImageTitle);
		fDeleted = TRUE;
    }
	/* save the LCD law data */
	if(!SaveGVM_BMPToFS((void*)data, (char *)abImageTitle))
	{
		error("[midd_RegScreen]Screen_download",0,0,0);
		return;
	}		

	/* set image info again */
    if(fDeleted)
    {
		SKY_SET_INCOMING_PICTURE(CaseImage.IncomingCall);
		SKY_SET_POWER_ON_PICTURE(CaseImage.PowerOn);
		SKY_SET_POWER_OFF_PICTURE(CaseImage.PowerOff);
		SKY_SET_WAP_PICTURE(CaseImage.WAPStart);    
	}
    GVMImage.wPictureID = sMY_PICTURE;
    STRNCPY(GVMImage.szFileName, abImageTitle, SWAP_LEN_GNAME);
	SKY_SET_IDLE_PICTURE(GVMImage);
	SKY_SET_IDLE_SCREEN(IS_PICTURE);

	return;
}


//==============================================================================
//
//	Vendor Dependent API
//
//==============================================================================

//==============================================================
//	Vendor spec: Set SO(Service Option) & MO(Mux Option)
//
#ifdef SWAP_FEATURE_IS95C
void midd_SetSoMo(WORD so, WORD mo)
{
    error("[midd_SetSoMo] so:%d,mo:%d", so, mo, 0);    

    if (P_REV_IS_6_OR_GREATER)
    {        
        if (so == 33) 
        {
#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO
extern	void	SetMOinSO33(word	nMuxOption);            
            SetMOinSO33(mo);
        }
    }
#endif	//	#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO
}
#endif // SWAP_FEATURE_IS95C

//==============================================================
//	Vendor spec: Store advanced audio
//
//	name	audio name
//	data	audio data
//	size	audio size
//
#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)
void midd_RegAdvAudio(char *name, BYTE *data, int size)
{
    return;
}
#endif // SWAP_AAD_TYPE

//==============================================================
//	Vendor spec: Play advanced audio
//
//	data	audio data
//	size	audio size
//
#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)
void midd_PlayAdvAudio(BYTE *data, int size)
{
    return;
}
#endif // SWAP_AAD_TYPE

//==============================================================
//	Vendor spec: Pause advanced audio playing
//
#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)
void midd_PauseAdvAudio()
{
    return;
}
#endif // SWAP_AAD_TYPE

//==============================================================
//	Vendor spec: Restart advanced audio playing
//
#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)
void midd_RestartAdvAudio()
{
    return;
}
#endif // SWAP_AAD_TYPE

//==============================================================
//	Vendor spec: Stop advanced audio playing
//
#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)
void midd_StopAdvAudio()
{
    return;
}
#endif // SWAP_AAD_TYPE

//==============================================================================
//
//	Data exchange API
//
//==============================================================================

//==============================================================
//	Read handset data
//
//	return 1=Success, 0=Fail
//
#define HIGH(data)  
int midd_ReadHandsetData(INT2 code, INT2 major, INT2 minor, BYTE *buf)
{
    return 0;
    /*STATIC BOOL fReadSuccess = TRUE;   // ����� �ǰ� ���� ��ġ��
    error("[midd_ReadHandsetData]code:%d,major:%d,minor:%d", code, major, minor);

    switch(code) 
    {
        case 0: // SMS �б�
        {
            switch(major) 
            {
                case 0: // ���� �б�
                {
                    // total size : 8
                    // 0 ~ 1 : ���ŵ� SMS ���� (2bytes : high-low����) -> gReadTextMessage
                    // 2 ~ 3 : �ִ� ���� ����  -> MAX_RECEIVE_MESSAGE
                    // 4 ~ 5 : ����� SMS ���� 
                    // 6 ~ 7 : �ִ� ���� ���� -> MAX_RECEIVE_MESSAGE

                    WORD    data[4];
                    data[0] = 1;
                    data[1] = 20;
                    data[2] = 0;
                    data[3] = 20;

                    buf[0] = data[0] >> 8;
                    buf[1] = data[0];
                    buf[2] = data[1] >> 8;
                    buf[3] = data[1];
                    buf[4] = data[2] >> 8;
                    buf[5] = data[2];
                    buf[6] = data[3] >> 8;
                    buf[7] = data[3];
                    
                    break;
                }
                case 1: // ���� SMS
                {
                    // total size : 99
                    // 0 ~ 80 : SMS ������(null-terminate ���ڿ� ����)
                    // 81 ~ 86 : ������(��:2001.06.28.20.00.59) -> 14���� -> ??
                    // 87 ~ 98 : �߽Ź�ȣ (������ ������ 10�ڸ� �Ǵ� 11�ڸ��� null-terminate ���ڿ�)

                    //BYTE buffer[99];
                    //ui_sms_read_op_type sms_read;
                    //ui_sms_edit_info_type list_info;

                    memset(buf, 0, 99);
                    fReadSuccess = FALSE;
                    //gMakeMsgList();
                    //paMsgListBuf[minor]

                    
                    /*list_info.cmd = ;
                    list_info.dest_idx = ;
                    list_info.dest_list = ;
                    list_info.protect = ;
                    list_info.sms_msg = ;
                    list_info.src_idx = ;
                    list_info.src_list = ;*/
                    
                    /* Setup for text message read */
                    /* Check new then old list     */
                    //sms_read.cmd = UI_SMS_READ_ANY;  
                    
                    /* Determine if there is text messages to read * /
                    //ui_sms_read_text (&sms_read);


                    break;
                }
                case 2: // �߽� SMS
                {
                    // total size : 87
                    // 0 ~ 80 : SMS ������(null-terminate ���ڿ� ����)
                    // 81 ~ 86 : ������(��:2001.06.28.20.00.59) -> 14���� -> ??
                    memset(buf, 0, 87);
                    fReadSuccess = FALSE;
                    break;
                }
                default:
                    fReadSuccess = FALSE;
                    break;

            }
            break;
        }
        case 2: // ��ȭ�� �б�
        {
            switch(major) 
            {
                case 0: // ��ȭ�� ���� �б�
                {
                    //  total size : 4
                    // 0 ~ 1 : ����� ��ȭ�� ����(2byte->high-low����)
                    // 2 ~ 3 : �ִ� ��ȭ�� ����

                    // minor : - 

                    //WORD buffer[2];
                    WORD nSavedPhonebookSize = 2;
                    WORD nMaxPhonebookSize = 600;

                    buf[0] = nSavedPhonebookSize >> 8;
                    buf[1] = nSavedPhonebookSize;

                    buf[2] = nMaxPhonebookSize >> 8;
                    buf[3] = nMaxPhonebookSize;

                    //buffer[0] = (WORD)2;//nSavedPhonebookSize;
                    //buffer[1] = (WORD)600; //nMaxPhonebookSize;
                    
                    //buf = (BYTE*)buffer;
                    //error("buf:[%x][%x]", buf[0], buf[1], 0);
                    //error("buf2:[%x][%x]", buf[2], buf[3], 0);
                    break;
                }
                case 1: // ��ȭ�� �б�
                {
                    // total size : 199
                    // 0 ~ 16 : �̸� (null-terminate ���ڿ�)
                    // 17 ~ 33 : �׷� (null-terminate ���ڿ�)
                    // 34 ~ 66 : ����ȭ��ȣ (32�ڸ� �̸��� null-terminate ���ڿ�)
                    // 67 ~ 99 : �繫�� ��ȭ��ȣ 
                    // 100 ~ 132 : �޴��� ��ȣ
                    // 133 ~ 154 : FAX ��ȣ
                    // 155 ~ 198 : �̸��� �ּ� (32�ڸ� �̸��� null-terminate ���ڿ�)

                    // minor : ��ȭ�� Index <- Address!!!

                    //BYTE buffer[199];
                    BYTE phone[4][33];
                    pb_book_info_type       BookInfo;
                    pb_number_info_type     NumberInfo;
                    pb_group_info_type      GroupInfo;  
                    pb_email_info_type      EMailInfo;
                    UINT8       i = 0, idx=0;
                    

                    if (IS_VALID_BOOK(minor+1) == FALSE) {
                        fReadSuccess = FALSE;
                        break;
                    }

                    BookInfo.address = minor+1;
                    memset(buf, 0, 199);
                    for (i=0;i<4;i++) {
                        memset(phone[i], 0, 33);
                    }

                    // �̸�
                    if(ui_get_pb(PB_BOOK_INFO_I, &BookInfo) > 0) {                        
                        //STRNCPY(buffer, BookInfo.name, 16);		                
                        memcpy(buf, BookInfo.name, 16);
                    }
                    idx = idx+16;
                    //buffer = buffer + 16;   // ������ �̵�                        

                    // �׷�
                    GroupInfo.address = BookInfo.group_address;
                    if (ui_get_pb(PB_GROUP_INFO_I, &GroupInfo) > 0) {
                        //STRNCPY(buffer+idx, g_szGroupName[GroupInfo.address-1], 16);
                        memcpy(buf+idx, g_szGroupName[GroupInfo.address-1], 16);
                    }
                    idx = idx+16;
                    //buffer = buffer + 16;   // ������ �̵�           
                    
                    // ��ȭ��ȣ
                    NumberInfo.address = BookInfo.number_address;                    
                    while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) == TRUE) {
                        /* 1: CELL  2:HOME  3:WORK   4:PAGER  5:FAX * /    
                        if (NumberInfo.number_type == 2) { // Home
                            if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 ) {
                                STRNCPY(phone[0],NumberInfo.digits, 32);                                
                            }                            
                        }
                        else if (NumberInfo.number_type == 3) { // �繫��
                            if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 ) {
                                STRNCPY(phone[1],NumberInfo.digits, 32);
                            }                            
                        }
                        else if (NumberInfo.number_type == 1) { // �޴���
                            if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 ) {
                                STRNCPY(phone[2],NumberInfo.digits, 32);
                            }                            
                        }
                        else if (NumberInfo.number_type == 5) { // Fax
                            if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 ) {
                                STRNCPY(phone[3],NumberInfo.digits, 32);
                            }                            
                        }                        
                        NumberInfo.address = NumberInfo.next_index;
                    }
                    for (i=0;i<4;i++) {
                        //STRNCPY(buffer+idx , phone[i], 32);
                        memcpy(buf+idx, phone[i], 32);
                        //buffer = buffer + 32;
                        idx = idx+32;
                    }

                    // �̸��� �ּ�
                    EMailInfo.address = BookInfo.email_address;
                    if (ui_put_pb(PB_GROUP_INFO_I, &EMailInfo) > 0) {
                        //STRNCPY(buffer+idx, EMailInfo.szMailID, 32);
                        memcpy(buf+idx, EMailInfo.szMailID, 32);
                    }

                    //buf = buffer;
                    break;
                }

                default:
                    fReadSuccess = FALSE;
                    break;
            }
            break;
        }
        case 4: // �̹��� �б�
        {
            if (major == 0)     // �̹��� ���� �б�
            {
                // 0~1:����� �̹��� ����
                // 2~3:�ִ� ���� ����
                fReadSuccess = FALSE;
            }
            else if (major == 1) // �̹��� �������� �б�
            {
                // minor:�̹��� index
                // 0~1:�̹��� ũ��
                // 2~3:image format
                // 4~19:image name
                fReadSuccess = FALSE;
            }
            else fReadSuccess = FALSE;
            break;
        }
        case 5: // �̹��� ������ �б�
        {
            // major:image index
            // minor:block index
            // max:256
            fReadSuccess = FALSE;
            break;
        }
        default: // �̿�?
            error("�ٸ� �б�?-code:%d", code, 0, 0);
            fReadSuccess = FALSE;
            break;
    }    

	return fReadSuccess;*/
}

//==============================================================
//	Write handset data
//
//	return 1=Success, 0=Fail
//
int midd_WriteHandsetData(INT2 code, INT2 major, INT2 minor, BYTE *buf)
{
    return 0;
    /*
    STATIC  BOOL    fWriteSuccess = FALSE;
    error("midd_WriteHandsetData -------------", 0, 0, 0);
    error("code:%d, major:%d, minor:%d", code, major, minor);

    switch (code) {
        case 1:  // SMS ���� 
        {
            switch(major) 
            {
                case 0 : //���� SMS ����
                    {
                        // minor : ���� SMS index
                        // size : 0
                        // Ư�̻��� : ����

                        break;
                    }
                case 1: // �߽� SMS ����
                    {
                        break;
                    }
                case 2: // �߽� SMS ����
                    {
                        break;
                    }
            }
            break;
        }
        case 3: // ��ȭ�� ����
        {
            switch(major) {
                case 0: // ��ȭ�� ����
                    {
                        break;
                    }
                case 1: // ��ȭ�� ����
                    {
                        break;
                    }
            }
            break;
        }
        case 5: // �̹��� ���� ����
        {
            switch(major) {
                case 0 : // �̹��� ���� ����
                    {
                        break;
                    }
                case 1: // �̹��� ����
                    {
                        break;
                    }
            }
            
            break;
        }
        case 7: // �̹��� ������ ����
        {
            break;
        }
        case 8: // �̹��� ����(?)
        {
            break;
        }
    }

	return fWriteSuccess;*/
}
