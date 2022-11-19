//==============================================================================
//
//	MOBILE GAME EMULATOR STANDARD HEADER
//
//	FILE NAME
//		GExt.cpp
//
//	DESCRIPTION
//		All the declarations and definitions necessary for the Mobile Game
//		Emulator to interact with the Mobile and Game Emulator
//
//	REFERENCES
//		Mobile Game Emulator Porting Guide
//
//	Copyright (C) 1999 by SINJISOFT. All Rights Reserved.
//
//		1999.11. 0	Release Version 0.9
//		1999.12.21	Release Version 1.0
//		2000. 1. 7  Release Version 1.1
//		2000. 2.22  Release Version 1.2
//		2000.10. 8  Release Version 2.0
//
//==============================================================================
#include <stdlib.h>
#include "sky.h"

#include <time.h>
#include "GLib.h"

#define	SKY_GAME_FILE	SKY_SYS_DIR""SKY_DIR_CHAR"GAME.SKY"
#define SKY_GAME_DATA_SIZE	76
#define SKY_GAME_NUMBER	5

//==============================================================================
//
//	Game Definition List
//
//==============================================================================

const s_GameDefType	s_GameDef[] = {

//	"Game Title",			Game title
//	"Game Description",		Game description
//	GameMain,				Game main function
//	0,						Game mode (0=Stand-alone, 1=1:1, 2=Network)
//	120, 80,				LCD width, height
//	76,						NV-ROM size = (((11+2+2)*G_SCOREBOARD_SIZE) +1) Gext.h와 Glib.h의 숫자와 일치하여야 한다.

/*
#ifdef RINGTEST_COLOR
	"G00 RingColor",
	"Ring Test",
	RingColorMain,
	0,
//	128, 160, //SKTT5
   120, 133,	
	76,
#endif	//#ifdef RINGTEST_COLOR
*/	
#ifdef SKTT5

	"G80 Apache5",
	"Apache5",
	Apache5Main,
	0,
	128, 160,
	76,

	"G81 Raccoon3",
	"Raccoon3",
	Raccoon3Main,
	0,
	128, 160,
	76,

	"G82 MarsScouter3",
	"MarsScouter3",
	MarsScouter3Main,
	0,
	128, 160,
	76,

	"G83 Samtris3",
	"Samtris3",
	Samtris3Main,
	0,
	128, 160,
	76,

	"G84 Puzzle3",
	"Puzzle3",
	Puzzle3Main,
	0,
	128, 160,
	76,

#endif	//#ifdef SKTT5


	NULL, NULL, 0, 0, 0, 0
};

//==============================================================================
//
//	Game Extension Function
//
//==============================================================================
//jimylee 2001.06.08
//static	WORD	lcdBuffer[S_LCD_BUFFER];
//static	WORD	backBuffer[S_LCD_BUFFER];
//======================================
//	Initialize game buffer
//
void gext_InitMemory()
{
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];

	swLcdBuffer     = (BYTE*)Image$$APP_SHARE_RAM$$Base;
    swBackLcdBuffer = swLcdBuffer + (S_LCD_BUFFER);

    memset(swLcdBuffer, NULL, S_LCD_BUFFER);
    memset(swBackLcdBuffer, NULL, S_LCD_BUFFER);
}

//======================================
//	Close game buffer
//
void gext_CloseMemory()
{
}

//======================================
//	LCD update function
//
//jimylee 2001.06.08
void gext_DrawLcd(const char* pLcdBuf)
{
	//static WORD *pLCD;
    //BYTE *pbLCD;
	//BYTE i;
    //INT j;

	//pbLCD = (BYTE*)GetScreenBuffer();
    
    //memcpy(pbLCD, pLcdBuf, SKY_SCREEN_BUFFER_WIDTH*SKY_SCREEN_BUFFER_HEIGHT*(SKY_COLOR_SPACE/8));

		SKY_256UpdatePartialDisplay(pLcdBuf,0,160);//SKY_UpdateScreen();
}
//======================================
//	Sound play function
//
//		data : sound data
//		size : byte size
//
void gext_PlaySound(const BYTE *pData, const int nSize)
{
    if (SKY_GET_GAME_VOLUME() == 0)
		return;

	if (*pData == 0x02)
    {
        SKY_StopMusic();
//		pParentWnd->m_MA1.PlayMA2((unsigned short*)(pData + 2), (nSize - 2)/2, MA1_HIGH_LOW);
        SKY_PlayMusicBufferEx((BYTE*)(pData + 2), SKY_GET_GAME_VOLUME(), 1);
    }
}

//======================================
//	Sound play function
//
//		data : sound data
//		size : byte size
//
void gext_PlaySound_background(const BYTE *pData, const int nSize)
{
    INT8 nVolume;

    if (SKY_GET_GAME_VOLUME() == 0)
		return;

    if(SKY_GET_GAME_VOLUME() <= 1)
        nVolume = SKY_GET_GAME_VOLUME();
    else
        nVolume = SKY_GET_GAME_VOLUME() -1;

	if (*pData == 0x02)
    {
        SKY_StopMusic();
//		pParentWnd->m_MA1.PlayMA2((unsigned short*)(pData + 2), (nSize - 2)/2, MA1_HIGH_LOW);
        SKY_PlayMusicBufferEx((BYTE*)(pData + 2), nVolume, 0);
    }
}

//======================================
//	Stop playing sound background
//
void gext_StopSound()
{
    SKY_StopMusic();
}

//======================================
//	Start vibrator
//
void gext_StartVib(const int nSec)
{
    SKY_MotorTime(nSec*1000);
}

//======================================
//	Stop vibrator
//
void gext_StopVib()
{
    SKY_MotorStop();
}

//======================================
//	Start timer
//
//		nTid    : timer ID, 0, 1, 2
//		nMse    : timeout duration
//		nRepeat : timer repeat switch
//
void gext_StartTimer(const int nTid, const int nMse, const int nRepeat)
{
    switch(nTid) {
	case 0:
		EQS_StartTimer(EQS_TIMER_1, (DWORD)nMse, (BOOL)nRepeat);
		break;
	case 1:
		EQS_StartTimer(EQS_TIMER_2, (DWORD)nMse, (BOOL)nRepeat);
		break;
	case 2:
		EQS_StartTimer(EQS_TIMER_3, (DWORD)nMse, (BOOL)nRepeat);
		break;
	default:
		break;
	}
}

//======================================
//	Stop timer
//
//		nTid : timer ID, 0, 1, 2
//
void gext_StopTimer(const int nTid)
{
    switch(nTid) {
	case 0:
		EQS_StopTimer(EQS_TIMER_1);
		break;
	case 1:
		EQS_StopTimer(EQS_TIMER_2);
		break;
	case 2:
		EQS_StopTimer(EQS_TIMER_3);
		break;
	default:
		break;
	}
}

//======================================
//	Get game NV
//
//		nGid  : Game ID
//		pData : NV data pointer
//
void gext_GetNV(const int nGid, BYTE* const pData)
{
    HFILE	hFile;

    if(!SKY_IsFileOrDirExist((BYTE*)SKY_GAME_FILE))
	{
		memset(pData, 0x00, SKY_GAME_DATA_SIZE);
		return;
	}

	hFile = SKY_OpenFileEx((BYTE*)SKY_GAME_FILE, FS_AM_READWRITE, FS_CO_DISCARD);

    SKY_SeekFile(hFile, 
                 FS_SO_SET, 
                 SKY_GAME_DATA_SIZE * nGid);

	SKY_ReadDataBlock(hFile, pData, SKY_GAME_DATA_SIZE);

    SKY_CloseFile(hFile);
}

//======================================
//	Put game NV-ROM data
//
//		nGid  : Game ID
//		pData : NV data pointer
//
void gext_PutNV(const int nGid, const BYTE* pData)
{
    HFILE	hFile;
	BYTE	szTmp[SKY_GAME_DATA_SIZE];
	UINT8	i;

    if(!SKY_IsFileOrDirExist((BYTE*)SKY_GAME_FILE))
	{
		hFile = SKY_OpenFileEx((BYTE*)SKY_GAME_FILE, FS_AM_CREATE, FS_CO_DISCARD);
		memset(szTmp, 0x00, SKY_GAME_DATA_SIZE);

		for(i = 0; i < SKY_GAME_NUMBER; i++)
			SKY_WriteDataBlock(hFile, szTmp, SKY_GAME_DATA_SIZE);

		SKY_SeekFile(hFile,  FS_SO_SET, 0);
	}
	else
		hFile = SKY_OpenFileEx((BYTE*)SKY_GAME_FILE, FS_AM_READWRITE, FS_CO_DISCARD);

    SKY_SeekFile(hFile, 
                 FS_SO_SET, 
                 SKY_GAME_DATA_SIZE * nGid);

	SKY_WriteDataBlock(hFile, pData, SKY_GAME_DATA_SIZE);

    SKY_CloseFile(hFile);
}

//======================================
//	Get current time
//
void gext_GetDate(int* year, int* month, int* day, int* hour, int* min, int* sec)
{
    DWORD wTime;
    clk_julian_type clkTime;

    wTime = SKY_GetLocalTime();
    clk_secs_to_julian( wTime, &clkTime );

	*year = clkTime.year;
	*month = clkTime.month;
	*day = clkTime.day;
	*hour = clkTime.hour;
	*min = clkTime.minute;
	*sec = clkTime.second;
}

//======================================
//	Set pseudo random seed
//
//		nSeed : pseudo random seed
//
void gext_RandomSeed(const int nSeed)
{
    srand(nSeed);
}

//======================================
//	Get pseudo random value
//
//		nBegin : random min boundary
//		nEnd   : random max boundary
//
int	gext_Random(const int nBegin, const int nEnd)
{
    return nBegin + (rand()%(nEnd - nBegin));
}

//jimylee 2001.04.27
//======================================
//
//	Turn On Key Tone Sound
//
//
void gext_OnKeyTone()
{
    SKY_SET_KEY_TONE(TRUE);
}
//jimylee 2001.04.27
//======================================
//
//	Turn Off Key Tone Sound
//
//
void gext_OffKeyTone()
{   
    SKY_SET_KEY_TONE(FALSE);
}

