//==============================================================================
//
//	SINJISOFT DOWNLOAD GAME LIBRARY
//
//	FILE NAME
//		MIDD.cpp
//
//	DESCRIPTION
//		Mobile Interface Device Driver
//
//	REFERENCES
//		SINJISOFT DOWNLOAD Game Porting Guide
//
//	Copyright (C) 2000 by SINJISOFT. All Rights Reserved.
//
//	HISTORY
//		2000. 5.20  Alpha Release
//		2000. 6.10  Beta Release
//		2000. 7. 3  Beta2 Release
//		2000. 7.24  Beta3 Release
//		2000. 8. 7  Version 1.0 Release
//		2000. 8.28  Version 1.01 Release
//		2000. 9.26  Version 1.02 Release
//		2001. 2.23  Version 1.03 Release
//
//==============================================================================

#include "Lib\SWAP.h"
#include "uisockApi.h"
#include "ska.h"
#include "clnt.h"
#include <stdio.h>

#define LOAD_BLOCK_SIZE		1024
#define SAVE_BLOCK_SIZE     1024

static db_items_value_type dbi;   /* DB items */

extern void srand( unsigned int seed );
extern int  rand( void );

UINT8	asciiBuffer[256];
static int l_nIdxOfBuffer;

//==============================================================================
//
//	Memory management
//
//==============================================================================

#if	(SWAP_MEMORY_TYPE == SWAP_FIXED_MEMORY)

static BYTE* FixedMemory;
static BYTE	GVMLcdBuffer[(SKY_SCREEN_BUFFER_WIDTH/8)*SKY_COLOR_SPACE * SKY_SCREEN_BUFFER_HEIGHT];

//==============================================================
// swWorkSpace에 static 메모리의 주소를 할당한다.
// 그리고 svMIN에 전화번호를 저장한다.
// 전화번호는 011을 포함한 번호로 다음 예와 같다.
// "0119871234" 또는 "01198761234"
//
void midd_InitMemory()
{
	// TOTAL AVAILABLE SIZE 200K
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];
	BYTE *GameAddress;
    nv_dir_number_type  nv_dir_num; /* current nam name */

	GameAddress = (byte*)Image$$APP_SHARE_RAM$$Base;

    FixedMemory     = GameAddress + (128*1024);
	swWorkSpace     = FixedMemory;
	swLcdBuffer     = (byte *)GVMLcdBuffer;
	swBackLcdBuffer = GameAddress + (128 * 1024) + (16 * 1024);
	svGameAddr = GameAddress;

	// 전화번호 지정
	nv_dir_num.nam = SKY_GET_CURRENT_NAM();
    (void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );
    if (nv_dir_num.dir_number[0] == '0')
    {
        STRNCPY((BYTE *)svMIN, (BYTE *)nv_dir_num.dir_number, 10);
        svMIN[10] = '\0';
    }
    else
    {
        STRNCPY((BYTE *)&(svMIN[1]), (BYTE *)nv_dir_num.dir_number, 10);
        svMIN[0] = '0';
        svMIN[11] = '\0';
    }

}

#else  // (SWAP_MEMORY_TYPE == SWAP_SHARED_MEMORY)
#error "Miss Memory Type!!!"
//
//extern	BYTE	SharedMemory[];
//
//==============================================================
// swWorkSpace에 공유 메모리의 주소를 할당한다.
//
//void midd_InitMemory()
//{
//	swLcdBuffer     = SharedMemory;
//	swBackLcdBuffer = SharedMemory + S_LCD_BUFFER;
//	swWorkSpace     = SharedMemory + S_LCD_BUFFER*2;
//
//	GET_NV_STR("svMIN", svMIN);
//}
//
#endif // SWAP_FIXED_MEMORY

//==============================================================
//	Deallocate SWAP SRAM buffers
//
void midd_CloseMemory()
{
	// Restore shared memory
}

//==============================================================
// WML Script의 파라메터를 버퍼에 저장한다.
//
void midd_StoreRunInfo(const char *uid, const int cpid, const int gid,
					   const char *dlsIP, const int dlsPort,
					   const char *svrIP, const int svrPort, const char *retURL)
{
    // set run game info
	strcpy(svRunUserID, uid);
	svRunGameCPID = (WORD)cpid;
	svRunGameID = (WORD)gid;
	strcpy(svDlsIP, dlsIP);
	svDlsPort = (WORD)dlsPort;
	strcpy(svRunSvrIP, svrIP);
	svRunSvrPort = (WORD)svrPort;
	strcpy(svRetURL, retURL);
}

//==============================================================================
//
//	Script download & SSB control
//
//==============================================================================

//==============================================================
// svCurSSB로 선택된 SSB의 스크립트를 지우고
// NV의 스크립트 정보를 초기화 한다.
//
void midd_DeleteScript()
{
	if(SKY_GET_GVM_GAME_CPID(svCurSSB) == svGameCPID ||
		SKY_GET_GVM_GAME_ID(svCurSSB) == svGameID)
	{
        ClearGVMVars();

        SKY_SET_GVM_USER_ID(svCurSSB,(BYTE*)"");
        SKY_SET_GVM_GAME_CPID(svCurSSB,0);
        SKY_SET_GVM_GAME_ID(svCurSSB,0);
        SKY_SET_GVM_DLOAD_SVR_IP(svCurSSB,(BYTE*)"");
        SKY_SET_GVM_DLOAD_SVR_PORT(svCurSSB,0);
        SKY_SET_GVM_RUN_SVR_IP(svCurSSB,(BYTE*)"");
        SKY_SET_GVM_RUN_SVR_PORT(svCurSSB,0);
        SKY_SET_GVM_RET_URL(svCurSSB,(BYTE*)"");

	    SKY_SET_GVM_GAME_NAME(svCurSSB,(BYTE*)"");
	    SKY_SET_GVM_GAME_VER(svCurSSB,0);
	    SKY_SET_GVM_GAME_TYPE(svCurSSB,0);
	    SKY_SET_GVM_GAME_SIZE(svCurSSB,0);
	}
}

//==============================================================
// cpid, gid와 동일한 게임 정보를 읽어와
// 스크립트를 실행할 준비를 한다.
// svGameAddr에 실행할 스크립트의 주소를 할당힌다.
//
// EFS에 스크립트를 저장한 경우,
// RAM에 복사하여 스크립트 실행 준비를 한다.
// 원하는 스크립트가 없는 경우 0을 return한다.
//
int midd_LoadGameScript(const WORD cpid, const WORD gid)
{
	extern  BYTE	Image$$APP_SHARE_RAM$$Base[];
	static  BYTE    *GameAddress;
    static  DWORD   FileSize;
	BYTE            *ptr;
    BYTE            szFileName[MAX_GVM_GAME_NAME_LENGTH + 1];
	INT			    i;
	static	HFILE	hReadFile;

	// 여러개의 저장 스크립트중에 cpid, gid를 비교하여
	// 실행할 스크립트의 저장 번호를 svCurSSB에 지정한다.
	svCurSSB = 0xFF;
    ClearGVMVars();
    for (i = 0; i < MAX_GVM_SLOT; i++)
    {
		if(SKY_GET_GVM_GAME_CPID(i) == cpid &&
			SKY_GET_GVM_GAME_ID(i) == gid)
        {
			svCurSSB = i;

            /* Set GVM Vars */
            STRNCPY((BYTE *)svUserID, (BYTE *)SKY_GET_GVM_USER_ID(svCurSSB), MAX_GVM_USER_ID_LENGTH); //8
            STRNCPY((BYTE *)svGameName, (BYTE *)SKY_GET_GVM_GAME_NAME(svCurSSB), MAX_GVM_GAME_NAME_LENGTH); //16
            svGameVer = SKY_GET_GVM_GAME_VER(svCurSSB);
	        svGameCPID = SKY_GET_GVM_GAME_CPID(svCurSSB);
	        svGameID = SKY_GET_GVM_GAME_ID(svCurSSB);
	        svGameType = SKY_GET_GVM_GAME_TYPE(svCurSSB);
	        svGameSize = SKY_GET_GVM_GAME_SIZE(svCurSSB);
	        STRNCPY((BYTE *)svSvrIP, (BYTE *)SKY_GET_GVM_RUN_SVR_IP(svCurSSB), MAX_GVM_RUN_SVR_LENGTH); //32
	        svSvrPort = SKY_GET_GVM_RUN_SVR_PORT(svCurSSB);

			break;
        }
    }

  	if (svCurSSB == 0xFF)
        return 0;

	GameAddress = (byte*)Image$$APP_SHARE_RAM$$Base;
    GetGVMFileName(szFileName, cpid, gid);
	FileSize = svGameSize;
	ptr = GameAddress;
    if(SKY_OpenDataFile(SDF_GVM, (BYTE*)szFileName, &hReadFile, SFT_READ) == SDR_OK)
    {
        // set to svGameAddr
	    svGameAddr = GameAddress;

		while(FileSize > 0)
		{
			if(FileSize >= LOAD_BLOCK_SIZE)
			{
				SKY_ReadDataBlock(hReadFile, ptr, LOAD_BLOCK_SIZE);
				ptr += LOAD_BLOCK_SIZE;
				FileSize -= LOAD_BLOCK_SIZE;
			}
			else
			{
				SKY_ReadDataBlock(hReadFile, ptr, FileSize);
				FileSize = 0;
				break;
			}
		}        
        SKY_CloseDataFile(hReadFile);
        return 1; /* success */
    } 
    else
    {
        return 0; /* error */
    }
    
}

#ifdef SWAP_LCD_COLOR
//==============================================================
//	Get stored script image type
//
//	ssbIndex	stored SSB index
//
//	return		image type (3rd byte)
//
int midd_GetScriptType(int ssbIndex)
{
    BYTE    szFileName[MAX_GVM_GAME_NAME_LENGTH + 1];
    DWORD   dwCount;
    BYTE    abBuf[4+1];
    STATIC  HFILE   hReadFile;

    GetGVMFileName(szFileName, SKY_GET_GVM_GAME_CPID(ssbIndex), SKY_GET_GVM_GAME_ID(ssbIndex));
    memset(abBuf, NULL, 5);
    if(SKY_OpenDataFile(SDF_GVM, (BYTE*)szFileName, &hReadFile, SFT_READ) == SDR_OK)
    {
        dwCount = SKY_ReadDataBlock(hReadFile, abBuf, 4); /* read 4 byte */
        SKY_CloseDataFile(hReadFile);
        return (int)abBuf[2];        
    }
    else /* error */
    {
        return 0;

    }



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
    //CHAR    szFileName[16 + 1];
    DWORD   dwCount;
    //STATIC  HFILE   hReadFile;

    //GetGVMFileName(szFileName, SKY_GET_GVM_GAME_CPID(ssbIndex), SKY_GET_GVM_GAME_ID(ssbIndex));
    //if(SKY_OpenDataFile(SDF_GVM, (BYTE*)szFileName, &hReadFile, SFT_READ) == SDR_OK)
    //{
        dwCount = SKY_SeekReadFile(g_hGVMDecompFile, FS_SO_SET, start, buf, bsize);
        //dwCount = SKY_SeekReadFile(hReadFile, FS_SO_SET, start, buf, bsize);
        //SKY_CloseDataFile(hReadFile);
        return dwCount;
    //}
    //else /* error */
    //{
        //return 0;
    //}
}
#endif // SWAP_LCD_COLOR


//==============================================================
// svCurSSB로 선택된 SSB에 스크립트를 저장할 준비를 한다.
//
// Return = 0: Out of memory
//			1: Ok
//
int midd_WriteScriptInit(const char *gameName, const DWORD size)
{
    DWORD dwFreeSpace;

    g_dwLackSpace = 0;
    g_unLackSpaceKB = 0;
    l_nIdxOfBuffer = 0;

    /* Usable space? */
    dwFreeSpace = SKY_GetFreeSpaceInDataFile();
    if(size > dwFreeSpace) /* 남아 있는 공간보다 스크립트 크기가 클 경우 */
    {        
        g_dwLackSpace = size - dwFreeSpace;
        g_unLackSpaceKB = (UINT8)((size - dwFreeSpace)/1024);
        return 0;
    }
    else if(size > 128*1024)
    {
        return 0;
    }
    else
    {
        return 1;
    }	
}

//==============================================================
// 스크립트 1블럭을 SSB에 저장한다.
// block: 0부터 1000byte 단위의 블럭 번호
// data: block 데이터
// size: 데이터의 길이
//
void midd_WriteScriptBlock(const int block, const BYTE* data, const int size)
{
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];
	BYTE *GameAddress;

	GameAddress = (byte*)Image$$APP_SHARE_RAM$$Base;
    memcpy(&GameAddress[l_nIdxOfBuffer], data, size);
	l_nIdxOfBuffer += size;
    return;
}

//==============================================================
// 스크립트 저장을 끝내고
// 스크립트 정보를 NV에 저장한다.
//
void midd_WriteScriptInfo(const char *uid, const char *gname, const WORD gver,
						  const WORD cpid, const WORD gid, const BYTE gtype,
						  const DWORD size, const char *svrIP, const WORD svrPort)
{

    // set the script info
	STRCPY((BYTE *)svUserID, (BYTE *)uid);
	strcpy(svGameName, gname);
	svGameVer = gver;
	svGameCPID = cpid;
	svGameID = gid;
	svGameType = gtype;
	svGameSize = size;
	strcpy(svSvrIP, svrIP);
	svSvrPort = svrPort;

    /*
	static  HFILE   hFile;

    BYTE                    *GameAddress;
	BYTE                    szFileName[MAX_GVM_GAME_NAME_LENGTH + 1];
	SKY_DATA_FILE_RESULT_E  FileHandleResult;
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];
	
	GameAddress = (byte*)Image$$APP_SHARE_RAM$$Base;
    // create file
 	GetGVMFileName(szFileName, cpid, gid);
	FileHandleResult = SKY_CreateDataFile(SDF_GVM, (BYTE*)szFileName, &hFile);
    if(FileHandleResult == SDR_OK)
    {
		SKY_WriteDataBlock(hFile, GameAddress, size);
		// close the file
		SKY_CloseDataFile(hFile);
	}
	else
	{
		MSG_ERROR("CAN NOT CREATE DATA FILE",0,0,0);
	}
    
    // set the nv item
    SKY_SET_GVM_USER_ID(svCurSSB, svUserID);
	SKY_SET_GVM_GAME_NAME(svCurSSB, svGameName);
	SKY_SET_GVM_GAME_VER(svCurSSB, svGameVer);
    SKY_SET_GVM_GAME_CPID(svCurSSB, svGameCPID);
    SKY_SET_GVM_GAME_ID(svCurSSB, svGameID);
	SKY_SET_GVM_GAME_TYPE(svCurSSB, svGameType);
	SKY_SET_GVM_GAME_SIZE(svCurSSB, svGameSize);
    SKY_SET_GVM_RUN_SVR_IP(svCurSSB, svSvrIP);
    SKY_SET_GVM_RUN_SVR_PORT(svCurSSB, svSvrPort);
    SKY_SET_GVM_RET_URL(svCurSSB, "");

    // set the flag
    SKY_SET_GVM_SSB(svCurSSB, TRUE);

    //AddFileList(svCurSSB, svGameName, svGameCPID, svGameID);
    */
}

void midd_WriteScriptInfo2(const char *uid, const char *gname, const WORD gver,
						  const WORD cpid, const WORD gid, const BYTE gtype,
						  const DWORD size, const char *svrIP, const WORD svrPort)
{
	static  HFILE   hFile;

    BYTE                    *GameAddress;
	BYTE                    szFileName[MAX_GVM_GAME_NAME_LENGTH + 1];
	SKY_DATA_FILE_RESULT_E  FileHandleResult;
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];
	
	GameAddress = (byte*)Image$$APP_SHARE_RAM$$Base;
    /* create file */
 	GetGVMFileName(szFileName, cpid, gid);
	FileHandleResult = SKY_CreateDataFile(SDF_GVM, (BYTE*)szFileName, &hFile);
    if(FileHandleResult == SDR_OK)
    {
		SKY_WriteDataBlock(hFile, GameAddress, size);
		// close the file
		SKY_CloseDataFile(hFile);
	}
	else
	{
		MSG_ERROR("CAN NOT CREATE DATA FILE",0,0,0);
	}

    // set the nv item
    SKY_SET_GVM_USER_ID(svCurSSB, uid);
	SKY_SET_GVM_GAME_NAME(svCurSSB, gname);
	SKY_SET_GVM_GAME_VER(svCurSSB, gver);
    SKY_SET_GVM_GAME_CPID(svCurSSB, cpid);
    SKY_SET_GVM_GAME_ID(svCurSSB, gid);
	SKY_SET_GVM_GAME_TYPE(svCurSSB, gtype);
	SKY_SET_GVM_GAME_SIZE(svCurSSB, size);
    SKY_SET_GVM_RUN_SVR_IP(svCurSSB, svrIP);
    SKY_SET_GVM_RUN_SVR_PORT(svCurSSB, svrPort);
    SKY_SET_GVM_RET_URL(svCurSSB, "");

    // set the flag
    SKY_SET_GVM_SSB(svCurSSB, TRUE);
}

//==============================================================================
//
//	LCD/Audio Control
//
//==============================================================================

//==============================================================
// LCD에 화면을 출력한다.
//
void midd_DrawLcd(BYTE *buf)
{
	STATIC BYTE *pLCD;

	pLCD = GetScreenBuffer();
	memcpy(&pLCD[14 * 120], (BYTE*)buf, 120 * 106);
	EQS_AppletDrawDirect();
}

//==============================================================
// 부저나 MA2을 연주한다.
//
void midd_PlaySound(const BYTE *data, const int len)
{
    if (SKY_GET_KEY_TONE_VOLUME() == 0)
		return;

	// MA2 play routine
	if (*data == 0x02)
	{
        SKY_StopMusic();
#if SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
		//SKY_PlayMusicBufferEx((BYTE*)(data + 2), SKY_GET_KEY_TONE_VOLUME(), 0);
        SKY_PlayMusicBufferEx((BYTE*)(data + 2), SKY_GET_KEY_TONE_VOLUME(), 1);
#elif SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
		SKY_PlayMusicBufferEx((WORD*)(data + 2), SKY_GET_KEY_TONE_VOLUME());
#endif
	}
	else
	{
		return;
	}
}

//==============================================================
// 부저나 MA1을 연주를 중지한다.
//
void midd_StopSound()
{
	SKY_StopMusic();
}

//==============================================================
// EVRC 보코더 데이터를 재생한다.
//
void midd_PlayVocoder(const BYTE* data, const int len)
{
    // 에티켓 모드인 경우 소리를 내지 않는다.
    if (SKY_IS_ETIQUETTE()) 
		return;

	SKY_PlayVoice((BYTE *)data, (UINT16)len, (UINT8)1);
}

//==============================================================
// EVRC 보코더 재생을 중지한다.
//
void midd_StopVocoder()
{
	// Stop EVRC vocoder data
	SKY_StopVoice();
}

//==============================================================
// 진동기를 울린다.
//
void midd_StartVib(const int t)
{
	SKY_MotorTime(t*1000);
}

//==============================================================
// 진동기를 멈춘다.
//
void midd_StopVib()
{
	SKY_MotorStop();
}


//==============================================================
// Keytone을 설정한다.
//
void midd_SetKeyTone(const int sw)
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
// Backlight를 켜거나 끈다.
// 0 : Off, 1: On, 2: default
void midd_SetBacklight(const int sw)
{
    switch (sw)
    {
    case 1: /* ON */
        //SKY_SET_NO_OFF_BACKLIGHT(TRUE);			//시작할때 반드시 넣기
		SKY_BacklightOn(AB_ALWAYS);
        break;
    case 0: /* OFF */
        //SKY_SET_NO_OFF_BACKLIGHT(FALSE);			//종료할때 반드시 넣기
        //SKY_BacklightOn(AB_NO);
        SKY_BacklightOff();
        break;

    case -1:
    default:
		SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());		//종료할때 반드시 호출 OR 사용자가 설정한 값으로 돌아가려고 할때 호출
        break;
    }
}

//==============================================================
// SWAP의 사용자 NV를 0으로 초기화 한다.
//

void midd_ClearUserNV()
{
	BYTE buf[S_NV_DATA_SIZE+1];

	memset(buf, 0, (S_NV_DATA_SIZE+1));
	SKY_SET_GVM_USER(svCurSSB, buf, (S_NV_DATA_SIZE+1));
}

//==============================================================
// SWAP의 사용자 NV를 data에 읽어온다.
//
void midd_GetUserNV(BYTE *data, int size)
{
	memcpy(data, (void *)SKY_GET_GVM_USER(svCurSSB), size);
}

//==============================================================
// SWAP의 사용자 NV에 data에 써넣는다.
//
void midd_PutUserNV(const BYTE *data, int size)
{
	BYTE	buf[S_NV_DATA_SIZE+1];
	int		i;

    /*
	for (i=0; i<size; i++)
    {
		buf[i] = *data++;
    }
    */
	memset(buf, 0, (S_NV_DATA_SIZE+1));
    memcpy(buf, data, size);
	SKY_SET_GVM_USER(svCurSSB, buf, size);
}

//==============================================================================
//
//	Timer Control
//
//==============================================================================

//==============================================================
// 지정된 시간으로 타이머를 설정한다.
//
void midd_StartTimer(const int id, const int t, const int repeat)
{
	// Start game timer
	switch(id) {
	case 0:
		//EQS_StartTimer(EQS_TIMER_1, (DWORD)t, (BOOL)repeat);
		SKY_SetGVMTimer1((LONG)t, (BOOL)repeat);
		break;
	case 1:
		//EQS_StartTimer(EQS_TIMER_2, (DWORD)t, (BOOL)repeat);
		SKY_SetGVMTimer2((LONG)t, (BOOL)repeat);
		break;
	case 2:
		//EQS_StartTimer(EQS_TIMER_3, (DWORD)t, (BOOL)repeat);
		SKY_SetGVMTimer3((LONG)t, (BOOL)repeat);
		break;
	default:
		break;
	}
}

//==============================================================
// 설정된 타이머를 정지 시킨다.
//
void midd_StopTimer(int id)
{
	switch(id) {
	case 0:
		//EQS_StopTimer(EQS_TIMER_1);
		SKY_SetGVMTimer1((LONG)0, (BOOL)FALSE);
		break;
	case 1:
		//EQS_StopTimer(EQS_TIMER_2);
		SKY_SetGVMTimer2((LONG)0, (BOOL)FALSE);
		break;
	case 2:
		//EQS_StopTimer(EQS_TIMER_3);
		SKY_SetGVMTimer3((LONG)0, (BOOL)FALSE);
		break;
	default:
		break;
	}
}

//==============================================================================
//
//	SMS API
//
//==============================================================================

//==============================================================
// SMS를 발신한다.
//
int midd_SendSMS(const WORD TI, const char *dial, const char *data)
{
	// Send SMS data
    if (g_GVMSmsMoType != GSMT_IDLE) 
	{
		return 0;
	}

    memset(&g_GVMSmsBuffer, NULL, sizeof(SMSMO_SEND_T));
	g_GVMSmsBuffer.nMsgSize = strlen(data);
	strncpy((char *)g_GVMSmsBuffer.pszMsg, data, MAX_SMS_TEXT);
	g_GVMSmsBuffer.nRcvNumberSize = strlen(svMIN);
	strncpy((char *)g_GVMSmsBuffer.pszRcvNumber, svMIN, MAX_SMS_DIAL);
	g_GVMSmsBuffer.nSendNumberSize = strlen(svMIN);
	strncpy((char *)g_GVMSmsBuffer.pszSendNumber, svMIN, MAX_SMS_DIAL);

	g_GVMSmsMoType = GSMT_SENDING;
	g_GVMSmsMoTI = TI;

	SendSMSMsg(&g_GVMSmsBuffer, g_GVMSmsMoTI);

	SKY_SetGeneralTimer(10*1000, FALSE);
	g_GVMFromTimeOut = GFT_SMS;

	return 1;
	/*
    NetGameSmsBuf.nTextSize = strlen(data);
    strncpy(NetGameSmsBuf.szText, data, MAX_SMS_TEXT);
    NetGameSmsBuf.nDialSize = strlen(svMIN);
    strncpy((char *)NetGameSmsBuf.szDial, svMIN, MAX_SMS_DIAL);

    NetGameSmsMoStatus = NETGAME_SMS_SENDING;
    NetGameSmsMoTI = TI;

    strcpy(NetGameSmsDial, dial);
 
    uih_SmsSend(&NetGameSmsBuf, NetGameSmsDial, NetGameSmsMoTI);

	NetGameTimeOutReason = NETGAME_SMS;
    kj_TimerStop(KJ_TM_F_CUR);
    kj_TimerStart(KJ_TM_F_CUR, KJ_T_10S);
	*/
}

//==============================================================================
//
//	Network API
//
//==============================================================================

//==============================================================
// 서버의 IP, port를 재설정한다.
//
void midd_Reconnect(CONST CHAR *ip, CONST WORD port)
{
	extern VOID	ConnectServer(CHAR* IP, UINT16 port);

	ConnectServer((CHAR*)ip, (UINT16)port);
}

//==============================================================
// TCP/IP 서버로 데이터를 송신한다.
//
void midd_SendData(CONST BYTE *data, CONST BYTE len)
{
	MSG_ERROR("SENDDATA",0,0,0);
	SvrLinkSend((BYTE*)data, (UINT16)len);
}

//==============================================================
// 상대 전화기로 데이터를 송신한다.
//
void midd_PtpSendData(CONST BYTE *data, CONST BYTE len)
{
	PtpLinkSend((BYTE*)data, (UINT16)len);
}

//char    svNetGameRetURL[S_LEN_URL + 1];
//==============================================================
// WAP 브라우저로 돌아와 url을 연다.
//
void midd_BackToBrowser(const char *url)
{	
	if(url[0] != NULL)
	{
        SKY_SET_TO_WAP(TRUE);
        //SKY_SET_SOCK_CONNECT(TRUE);

        /* set the WAP url */
        memset(g_szGoToURL, NULL, MAX_GOTOURL_LEN);
        strcpy((char*)g_szGoToURL, (char*)url);
        g_szGoToURL[strlen(url)] = NULL;            

		/* Sound */
		SKY_StopMusic();

		/* Vibrator */
		SKY_MotorStop();
	
		/* Backlight */
        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

		/* Key Tone */
		SKY_SET_KEY_TONE(TRUE);

		g_hMssgGoToWAP = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_GVM_MESSG_RECONNECT_NTOP),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE,
                                        MBT_SUCCESS);
		EQC_SET_STYLE(g_hMssgGoToWAP, CS_MESSAGE_BOX_DEFAULT | 
                                      CS_MESSAGE_BOX_IGNORE_KEYS);
		//SKY_SetGeneralTimer(MESSAGE_DEFAULT_TIME*1000, FALSE);
		//g_GVMFromTimeOut = GFT_BACKTOBROWSER;
	}
	else
	{
        SKY_SET_TO_WAP(FALSE);
		SKY_ERROR_TONE();
	}
    return;
}

//==============================================================
// svRunUserID, svRunGameCPID, svRunGameID,
// svDlsIP, svDlsPort, svRunSvrIP, svRunSvrPort 를 참조하여
// 새로운 컨텐츠를 다운로드 하여 실행한다.
//
void midd_Download()
{  
	/* Sound */
	SKY_StopMusic();

	/* Vibrator */
	SKY_MotorStop();

	/* Backlight */
    SKY_SET_NO_OFF_BACKLIGHT(TRUE);
    SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

	/* Key Tone */
	SKY_SET_KEY_TONE(TRUE);

    /* WAP에서 스크립트를 다운로드하기 위해       */
    /* 환경변수 설정하는 부분과 동일하게 처리했음 */
	/* for Download */
    SKY_SET_TO_GVM(TRUE); /* go to SKA_FromWAP */

#ifdef FEATURE_SKT_GVM_ONLINE
    if (P_REV_IS_6_OR_GREATER)
        SKY_SET_FROM_WAP(NeedConnection(svRunGameCPID,svRunGameID));
    else
#endif//#ifdef FEATURE_SKT_GVM_ONLINE
        SKY_SET_FROM_WAP(FALSE);

    EQS_END_DEST(SKA_MainGVM);
}

//==============================================================================
//
//	RS232C API
//
//==============================================================================

#define	TO_HEX(n)	(((n) >= 10) ? 'A' + (n) - 10 : '0' + (n))
#define	TO_NUM(n)	(((n) >= 'A') ? 10 + (n) - 'A' : (n) - '0')

//==============================================================
// RS-232C로 명령을 송신한다.
//
void midd_RsCom(const BYTE comCode, const int comDataSize, const BYTE *comData)
{

	char	buf[100*2 + 1];
	int		i;

	for (i = 0; i < comDataSize; i++) {
		buf[i*2] = TO_HEX(*comData >> 4);
		buf[i*2 + 1] = TO_HEX(*comData & 0x0F);
		comData++;
	}
	buf[i*2] = '\0';
	sprintf((char*)asciiBuffer, "*SWAP*COM:%d,%s", comCode, buf);
	at_send_extended_text((char *)asciiBuffer);

}

#define MIDD_SEND_DATA_SIZE     100
//============================================================== 
// RS-232C로 데이터를 송신한다.
//
void midd_RsDownload(const int size, const BYTE *data)
{

	char	buf[MIDD_SEND_DATA_SIZE*2 + 1];
	int		i;
	dword   nSendPtr;
	BYTE    *pSendPtr;

    nSendPtr = 0;
    pSendPtr = (BYTE *)data;

    while (nSendPtr + MIDD_SEND_DATA_SIZE < size)
    {
	    for (i = 0; i < MIDD_SEND_DATA_SIZE; i++) {
		    buf[i*2] = TO_HEX(*pSendPtr >> 4);
		    buf[i*2 + 1] = TO_HEX(*pSendPtr & 0x0F);
		    pSendPtr++;
	    }
	    buf[i*2] = '\0';
	    sprintf((char*)asciiBuffer, "*SWAP*DLOAD:%d,%d,%s",
		    (int)size, (int)nSendPtr, buf);
	    at_send_extended_text((char *)asciiBuffer);

        nSendPtr += MIDD_SEND_DATA_SIZE;
        pSendPtr = (BYTE *)data + nSendPtr;
    }

	for (i = 0; i < size - nSendPtr; i++) {
		buf[i*2] = TO_HEX(*pSendPtr >> 4);
		buf[i*2 + 1] = TO_HEX(*pSendPtr & 0x0F);
		pSendPtr++;
	}
	buf[i*2] = '\0';
	sprintf((char*)asciiBuffer, "*SWAP*DLOAD:%d,%d,%s",
		(int)size, (int)nSendPtr, buf);
	at_send_extended_text((char *)asciiBuffer);

}

//==============================================================================
//
//	Common API
//
//==============================================================================

//==============================================================
// Get location info
//
void midd_GetLocInfo(WORD* w)
{
	db_get(DB_SID, &dbi);
	*w      = dbi.sid;	// SID
	db_get(DB_NID, &dbi);
	*(w+ 1) = dbi.nid;	// NID
	db_get(DB_REG_ZONE, &dbi);
	*(w+ 2) = dbi.reg_zone;	// REG_ZONE
	db_get(DB_BASE_ID, &dbi);
	*(w+ 3) = dbi.base_id;	// BASE_ID
	db_get(DB_PILOT_PN_OFF, &dbi);
	*(w+ 4) = dbi.pilot_pn_off;	// PILOT_PN
}

//==============================================================
// Pseudo random seed를 설정한다.
//
void midd_RandomSeed(const int seed)
{
	srand(seed);
}

//==============================================================
// Pseudo random 값을 구한다.
//
int	midd_Random(const int begin, const int end)
{
	// Get pseudo random value
	return begin + (rand()%(end - begin));
}

//==============================================================
// Random ratio 값을 구한다.
//
int midd_RandomRatio(const int ratio)
{
	return ((rand() % 100 < ratio) ? 1 : 0);
}

//==============================================================
// 날짜와 시간을 구한다.
//
void midd_GetDate(INT2 *year, INT2 *month, INT2 *day, INT2 *wday)
{
    DWORD current_time;
    clk_julian_type	julian;

    current_time = SKY_GetLocalTime();
    clk_secs_to_julian( current_time, &julian);

	*year =	julian.year;
    *month = julian.month;
    *day = julian.day;
	*wday = julian.day_of_week;
	if (*wday < 6) *wday += 1;
    else if (*wday >= 6) *wday = 0;
}

//==============================================================
// 날짜와 시간을 구한다.
//
void midd_GetTime(INT2 *hour, INT2 *min, INT2 *sec, INT2 *msec)
{
    DWORD current_time;
    clk_julian_type	julian;

    current_time = SKY_GetLocalTime();
    clk_secs_to_julian( current_time, &julian);

    *hour = julian.hour;
    *min = julian.minute;
	*sec = julian.second;
    *msec = 0;
}

//==============================================================
// 채팅 예약 문자열을 NV에서 읽어와서
// svRsvdChat 배열에 저장한다.
// NV의 초기값은 아래 예시와 같다.
//
void midd_GetChatText()
{
	strcpy(svRsvdChat[0], SKY_GET_GVM_CHATTEXT(0));
	strcpy(svRsvdChat[1], SKY_GET_GVM_CHATTEXT(1));
	strcpy(svRsvdChat[2], SKY_GET_GVM_CHATTEXT(2));
	strcpy(svRsvdChat[3], SKY_GET_GVM_CHATTEXT(3));
	strcpy(svRsvdChat[4], SKY_GET_GVM_CHATTEXT(4));
	strcpy(svRsvdChat[5], SKY_GET_GVM_CHATTEXT(5));
	strcpy(svRsvdChat[6], SKY_GET_GVM_CHATTEXT(6));
	strcpy(svRsvdChat[7], SKY_GET_GVM_CHATTEXT(7));
	strcpy(svRsvdChat[8], SKY_GET_GVM_CHATTEXT(8));
}

//==============================================================
// 문자 입력 모드로 전환한다.
// 문자 입력 모드에서는 *buf에
// 32자 이내로 문자를 입력한다.
//
/* 한글 입력만을 위한 구조체
ROM EDIT_MODE_SELECT_T  hangulmode = {
    1,
    0,
    TEM_KOREAN,
	FALSE
};
*/
void midd_TextEdit(const char *title, const char *buf)
{
    g_hTextEdit = EQC_TextEdit(1,
                               65,
                               19,
                               2,
                               TEM_KOREAN, 
                               MAX_SIZE_INPUT_CHAT_TEXT, 
                               (BYTE *)buf, 
                               0);
    EQC_SET_STYLE(g_hTextEdit, EQC_GET_STYLE(g_hTextEdit) | CS_TEXT_EDIT_VSCROLL); 
    //EQC_SetTextEditModeSequence(g_hTextEdit, EDIT_MODE_SELECT_T hangulmode);
    EQS_SetIOCActive(g_hTextEdit);
}

//==============================================================
// data로 넘어오는 MA1데이터를 MusicBell로 저장한다.
//
typedef enum {
	StandardMIDI  =  1,
	MP3           =  2,
	MP4           =  3,
	WMA           =  4,
	Mybell        =  5,
    YAMAH_ma1     =  20, 
	YAMAH_mmf     =  21, 
	ROHM          =  22,
    Qualcomm_CMX  =  40
} MusicbellAudioType ;

enum tagWAP_MUSIC_BELL_INFO_E {
	MAX_MUSICBELL_TITLE = 20,
	MAX_MUSICBELL_SIZE  = 4096,//     -- 16 poly
                              //2048 --  4 poly
    MUSICBELL_PLAY_MODE = 0,
    MUSICBELL_DOWNLOAD_MODE = 1
};

void midd_RegMusicBell(const char *name, const BYTE *data, const int size)
{
	char			musicTitle[22];
	dword			mbSize;
	boolean         IsValid = FALSE;	//Title을 가진 음악화일이 현재존재하는지 여부

	//extern MusicbellAudioType;
	//extern tagWAP_MUSIC_BELL_INFO_E    WAP_MUSIC_BELL_INFO_E;
	extern byte *mb_MusicBellPreBuffer;	//ma1형식의 musicbell을 저장하는 RAM의 buffer
    extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);
	extern byte get_length( byte *chk_str, byte buff_length);
	extern DWORD sCompareLengthofMusicData(int audioType, int Length, int mode);
	extern BOOL WAP_IsValidFile(SKY_DATA_FILE_E SkyDataFile, CONST BYTE* pszFileName);

	MSG_ERROR("Musicbell_download",0,0,0);
	//memset(Result, NULL, MAX_SIZE_MUSIC_RESULT);
	//audio = atoi(AudioType);
	
	//Invalid parameter check!!
	if (name == NULL)// || audio != YAMAH_ma1)
	{
			//strcpy(Result, "1003");		// Invalid_parameter
			MSG_ERROR("name is Invaild",0,0,0);
			return;
	}
	// if music name is over 14 then truncation...
	memset(musicTitle, NULL, 22);
	WAP_HanCopy(musicTitle, (char *)name, get_length((byte *)name, SKY_FILE_NAME_MAX_LENGTH-1));


	// 데이타가 Null이거나 데이타의 길이가 0인 경우, 저장 실패
	if(data == NULL || size <= 2)
    {
			//strcpy(Result, "1006");		// Undefined_Error
			MSG_ERROR("Data is Null",0,0,0);
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
			MSG_ERROR("Wip Malloc Fail",0,0,0);
			return;
		}
		memset(mb_MusicBellPreBuffer, 0x00, size+5);
		memcpy(mb_MusicBellPreBuffer, "MMF ", MM_NUM_FILE_TYPE);   //(B0에만 해당)phone에 저장하기 위한 header(4 bytes)
		memcpy(mb_MusicBellPreBuffer+MM_NUM_FILE_TYPE, data, size);

		//file의 갯수가 full인지 check!!
        if(SKY_GetFileNumberInDataFile(SDF_MELODY) >= SKY_MAX_NUM_FILES)        
        {
            //strcpy(Result, "1004");		//file의 갯수 full
			MSG_ERROR("Filenum_Full",0,0,0);
		}
		else
		{	
            mbSize = sCompareLengthofMusicData((int)YAMAH_mmf, size, MUSICBELL_DOWNLOAD_MODE);            
            if(mbSize == 0) return;

            // 저장할 공간이 있는지 확인
            if(mbSize > SKY_GetFreeSpaceInDataFile())
            {
				//strcpy(Result, "1005");		//정할 공간이 없는 경우
				MSG_ERROR("Buffer_Full",0,0,0);
			}
			else
			{
				//name이 현재 존재하는지 check!! -- 존재하면 delete후, 저장 
				IsValid = WAP_IsValidFile(SDF_MELODY, (BYTE *)musicTitle);
				if(IsValid)		//존재하는 경우
					SKY_DeleteDataFile(SDF_MELODY,(BYTE *)musicTitle);

				// 저장시작
                if(SKY_WriteDataFile(SDF_MELODY, (BYTE*)musicTitle, mbSize, mb_MusicBellPreBuffer) == SDR_OK)
                {
					//strcpy(Result, "0");		//success
				}
				else
				{
					MSG_ERROR("Save Fail",0,0,0);
				}
			}	//end of 저장할 공간 있는 경우
		}	//end of file num not full  
	}	//end of if(data != NULL)
	if(mb_MusicBellPreBuffer)
	{
		SKY_Free((void*)mb_MusicBellPreBuffer);
		mb_MusicBellPreBuffer =NULL;
	}

}

//==============================================================
// 현재 LCD버퍼의 이미지를 배경 그림으로 저장한다.
//
void midd_RegScreen(const char *name, const BYTE *buf)
{

	PICTURE_TYPE_T      GVMImage;
	PICTURE_IN_CASE_T   CaseImage;
    BOOL				fDeleted = FALSE;
	BYTE				abImageTitle[SKY_FILE_NAME_MAX_LENGTH+1];

	/* check data */
	if(buf == NULL)
	{
		MSG_ERROR("Image Data Null",0,0,0);
		return;
	}

	/* check disk space */
	if(!CheckDiskSpaceForSetGVMImage((char *)buf))
	{
		MSG_ERROR("CheckDiskSpaceForSetGVMImage((char *)buf)) ERROR",0,0,0);
		return;
	}

	/* set image title */
	if (name == NULL)
		STRCPY(abImageTitle, (BYTE *)"이름없음");
		//memcpy(abImageTitle, (char *)"이름없음", STRLEN((BYTE*)"이름없음"));

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
	if(!SaveGVM_BMPToFS((char *)buf, 0, (char *)abImageTitle))
	{
		MSG_ERROR("Screen_download",0,0,0);
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
    STRCPY(GVMImage.szFileName, abImageTitle);
	SKY_SET_IDLE_PICTURE(GVMImage);
	SKY_SET_IDLE_SCREEN(IS_PICTURE);


	return;
}

