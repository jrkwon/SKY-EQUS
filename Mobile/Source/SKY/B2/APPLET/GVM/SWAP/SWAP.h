//==============================================================================
//
//	SINJISOFT SWAP MODULE LIBRARY HEADER
//
//	FILE NAME
//		SWAP.cpp
//
//	DESCRIPTION
//		SWAP Virtual Machine Library
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

#ifndef	_SWAP_H_
#define	_SWAP_H_

//==============================================================================
//
//	Global type definition
//
//==============================================================================

#ifndef BYTE_TYPE_DEFINED
typedef unsigned char   BYTE;           // Unsigned  8 bit value type
#define BYTE_TYPE_DEFINED
#endif
#ifndef WORD_TYPE_DEFINED
typedef unsigned short  WORD;           // Unsigned 16 bit value type
#define WORD_TYPE_DEFINED
#endif
#ifndef DWORD_TYPE_DEFINED
typedef unsigned long   DWORD;          // Unsigned 32 bit value type
#define DWORD_TYPE_DEFINED
#endif

typedef unsigned char	UINT1;			// Unsigned  8 bit value type
typedef unsigned short	UINT2;			// Unsigned 16 bit value type
typedef unsigned long	UINT4;			// Unsigned 32 bit value type

typedef signed char		INT1;			// Signed  8 bit value type
typedef signed short	INT2;			// Signed 16 bit value type
typedef signed long		INT4;			// Signed 32 bit value type

#ifndef T_ARM
#define	__packed
#endif // T_ARM

//==============================================================================
//
//	SWAP Vender, Feature Definition
//
//==============================================================================

	#define	SWAP_ASCII				0x01
	#define	SWAP_KSC5601			0x02
	#define	SWAP_SHIFTJIS			0x03
	#define	SWAP_HEBREW				0x04

	#define SWAP_CARRIER_SKT		0x01
	#define SWAP_CARRIER_ORANGE		0x02
	#define SWAP_CARRIER_TUKA		0x04
	#define SWAP_CARRIER_DDIP		0x08

#define	SWAP_CARRIER				SWAP_CARRIER_SKT

#if (SWAP_CARRIER == SWAP_CARRIER_SKT)

	#define	SWAP_LANG				SWAP_KSC5601
	//#define	SWAP_FEATURE_AUTH
	#define	SWAP_FEATURE_NETWORK
	#ifdef SWAP_FEATURE_NETWORK
	#define	SWAP_FEATURE_DOWNLOAD
	#define	SWAP_FEATURE_PTP
	#endif // SWAP_FEATURE_NETWORK
	#define	SWAP_FEATURE_SERIAL
	#define SWAP_FEATURE_IS95C
	#define SKT_SKVM

#elif (SWAP_CARRIER == SWAP_CARRIER_ORANGE)

	#define	SWAP_LANG				SWAP_HEBREW
	//#define	SWAP_FEATURE_AUTH
	#define	SWAP_FEATURE_NETWORK
	#ifdef SWAP_FEATURE_NETWORK
	#define	SWAP_FEATURE_DOWNLOAD
	//#define	SWAP_FEATURE_PTP
	#endif // SWAP_FEATURE_NETWORK
	//#define	SWAP_FEATURE_SERIAL

#elif (SWAP_CARRIER == SWAP_CARRIER_TUKA)

	#define	SWAP_LANG				SWAP_SHIFTJIS
	//#define	SWAP_FEATURE_AUTH
	#define	SWAP_FEATURE_NETWORK
	#ifdef SWAP_FEATURE_NETWORK
	#define	SWAP_FEATURE_DOWNLOAD
	//#define	SWAP_FEATURE_PTP
	#endif // SWAP_FEATURE_NETWORK
	//#define	SWAP_FEATURE_SERIAL

#elif (SWAP_CARRIER == SWAP_CARRIER_DDIP)

	#define	SWAP_LANG				SWAP_SHIFTJIS
	//#define	SWAP_FEATURE_AUTH
	//#define	SWAP_FEATURE_NETWORK
	#ifdef SWAP_FEATURE_NETWORK
	//#define	SWAP_FEATURE_DOWNLOAD
	//#define	SWAP_FEATURE_PTP
	#endif // SWAP_FEATURE_NETWORK
	//#define	SWAP_FEATURE_SERIAL

#endif // SWAP_CARRIER


//==============================================================================
//
//	Handset HW Profile
//
//==============================================================================

//==============================================================
//	LCD type
//
	#define	SWAP_LCD_BW_HORIZ_MF		0x01
	// #define	SWAP_LCD_BW_HORIZ_LF		0x02
	#define	SWAP_LCD_BW_VERT_MF			0x03
	#define	SWAP_LCD_BW_VERT_LF			0x04
	#define SWAP_LCD_BW_END				0x0F

	#define	SWAP_LCD_GRAY_VERT_MF		0x11
	// #define	SWAP_LCD_GRAY_VERT_LF		0x12
	#define	SWAP_LCD_GRAY_HORIZ_MF		0x13
	#define	SWAP_LCD_GRAY_HORIZ_LF		0x14
	#define	SWAP_LCD_GRAY_MP_HORIZ_MF	0x15
	// #define	SWAP_LCD_GRAY_MP_HORIZ_LF	0x16
	#define	SWAP_LCD_GRAY_MP_VERT_MF	0x17
	#define	SWAP_LCD_GRAY_MP_VERT_LF	0x18
	#define	SWAP_LCD_GRAY_VM_HORIZ_MF	0x19
	#define	SWAP_LCD_GRAY_VM_HORIZ_LF	0x1A
	#define	SWAP_LCD_GRAY_HW_HORIZ_LF	0x1B
	#define SWAP_LCD_GRAY_END			0x1F

	#define SWAP_LCD_COLOR16_HORIZ_MF	0x21
	#define SWAP_LCD_COLOR16_HORIZ_LF	0x22
	#define SWAP_LCD_COLOR256_HORIZ		0x23
	#define SWAP_LCD_COLOR_END			0x2F

	#define SWAP_LCD_COLOR4K_HORIZ		0x30
	#define SWAP_LCD_WORDCOLOR_START	0x31
	#define SWAP_LCD_COLOR4K_HORIZ_WORD	0x32
	#define SWAP_LCD_COLOR64K_HORIZ		0x33
	#define SWAP_LCD_TCCOLOR_END		0x3F

#define	SWAP_LCD_TYPE			SWAP_LCD_COLOR256_HORIZ //SWAP_LCD_COLOR256_HORIZ //SWAP_LCD_COLOR64K_HORIZ

#if	(SWAP_LCD_TYPE < SWAP_LCD_BW_END)
	#define	SWAP_COLOR_TYPE			0x01
	#define	SWAP_LCD_BW
#elif (SWAP_LCD_TYPE < SWAP_LCD_GRAY_END)
	#define	SWAP_COLOR_TYPE			0x02
	#define	SWAP_LCD_GRAY
#elif (SWAP_LCD_TYPE < SWAP_LCD_COLOR_END)
	#define	SWAP_COLOR_TYPE			0x04
	#define SWAP_LCD_COLOR
#elif (SWAP_LCD_TYPE < SWAP_LCD_TCCOLOR_END)
	#define	SWAP_COLOR_TYPE			0x08
	#define SWAP_LCD_COLOR
#endif

// Vender speciallized flag
//#define	LG_CR16

//==============================================================
//	LCD resolution & class
//
#define	SWAP_LCD_WIDTH			128
#define	SWAP_LCD_HEIGHT			144

	#define SWAP_LCD_CLASS_0		0x01	//  96x 64 ~ 120x 80 미만
	#define SWAP_LCD_CLASS_1		0x02	// 120x 80 ~ 128x128 미만
	#define SWAP_LCD_CLASS_2		0x04	// 128x128 ~ 176x176 미만
	#define SWAP_LCD_CLASS_3		0x08	// 176x176 ~ 256x256 미만

#if (SWAP_LCD_WIDTH < 120 || SWAP_LCD_HEIGHT < 80)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_0
#elif (SWAP_LCD_WIDTH < 128 || SWAP_LCD_HEIGHT < 128)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_1
#elif (SWAP_LCD_WIDTH < 176 || SWAP_LCD_HEIGHT < 176)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_2
#else // if (SWAP_LCD_WIDTH < 256 || SWAP_LCD_HEIGHT < 256)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_3
#endif

//==============================================================
//	Audio type
//
	#define	SWAP_AUDIO_NONE			0x00
	#define	SWAP_AUDIO_BUZZER		0x01
	#define	SWAP_AUDIO_MA1			0x02
	#define	SWAP_AUDIO_MA2			0x04
	#define	SWAP_AUDIO_CMX			0x08
	#define	SWAP_AUDIO_MIDI			0x10

#define	SWAP_AUDIO_TYPE			SWAP_AUDIO_MA2

//==============================================================
//	Advanced Audio type
//
	#define	SWAP_AAD_NONE			0x00
	#define	SWAP_AAD_G_723_1		0x01
	#define	SWAP_AAD_AAC			0x02
	#define	SWAP_AAD_MP3			0x04

#define	SWAP_AAD_TYPE			SWAP_AAD_NONE

//==============================================================
//	Vocoder type
//
#ifdef SWAP_FEATURE_IS95C
#define	SWAP_CARRIER_TYPE		SWAP_CARRIER
#else
#define	SWAP_CARRIER_TYPE		(0x80 | SWAP_CARRIER)
#endif

//==============================================================
//	HW Config number
//
								// SWAP_LCD_CLASS(8bit) + SWAP_COLOR_TYPE(8bit) + SWAP_AUDIO_TYPE(8bit) + SWAP_CARRIER_TYPE(8bit)
#define	SWAP_HW_CONFIG			((SWAP_LCD_CLASS << 24) | (SWAP_COLOR_TYPE << 16) | (SWAP_AUDIO_TYPE << 8) | SWAP_CARRIER_TYPE)

//==============================================================
//	Model name
//

#define	SWAP_HANDSET_NAME		"SKTT IM-3300"

//==============================================================
//	Number of SSB
//
#define	SWAP_SSB_SIZE			16

//==============================================================
// Download Protocol
//

#ifdef SWAP_FEATURE_DOWNLOAD

#define SWAP_DOWNLOAD_SNDERR
#define SWAP_DOWNLOAD_RCVERR
#define SWAP_DOWNLOAD_CFRRETRY

#define	SWAP_DOWNLOAD_SPEC2

	#define SWAP_DOWNLOAD_SLIDING	0x01
	#define SWAP_DOWNLOAD_MAGIC		0x02

#define	SWAP_DOWNLOAD_TYPE			SWAP_DOWNLOAD_SLIDING //SWAP_DOWNLOAD_MAGIC

#if (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_MAGIC)
	#define SWAP_DOWNLOAD_PKTRETRY
	#define	SWAP_MAGIC_UNIT			4			// data service receiving buffer
#endif

#endif


//==============================================================================
//
//	Debug message setting
//
//==============================================================================

// #define SWAP_SAMPLE
//#define SWAP_DEBUG

#ifdef SWAP_DEBUG

#ifdef _WIN32
#define SWAP_TRACE(str, p1, p2, p3)		 { TRACE3(str, p1, p2, p3); TRACE("\n"); }
//#define SWAP_TRACE(str, p1, p2, p3)		 { CString t; t.Format(str, p1, p2, p3); ((CWGEmulDlg*)AfxGetMainWnd())->m_pwndTrace->Print(t); }
#else
#include "msg.h"
#define SWAP_TRACE(str, p1, p2, p3)		 MSG_MED(str, p1, p2, p3)
#define error(str, p1, p2, p3)			MSG_HIGH(str, p1, p2, p3)
#endif

#else

#define SWAP_TRACE(str, p1, p2, p3)
#define error(str, p1, p2, p3)

#endif // SWAP_DEBUG


//==============================================================================
//
//	SWAP Virtual Machine Definition
//
//==============================================================================

//==============================================================
//	Major Memory Size
//
#ifndef SWAP_LCD_COLOR	// BW, 4Gray LCD

#ifdef SKT_SKVM
#define SWAP_MAX_WORKSPACE		0x8000			// 32K
#else
#define	SWAP_MAX_WORKSPACE		0x2000			// 8K
#endif

#define	SWAP_MAX_SCRIPT_SIZE	0xC000			// 48K

#else // Color LCD

#ifdef SKT_SKVM
#define SWAP_MAX_WORKSPACE		0x10000			// 64K
#else
#define	SWAP_MAX_WORKSPACE		0x4000			// 16K
#endif

#define	SWAP_MAX_SCRIPT_SIZE	0x20000			// 128K

#endif	// SWAP_LCD_COLOR

#define	SWAP_USERNV_SIZE		64				// User NV data size
#define	SWAP_BLOCK_SIZE			1000			// Download block data size

#define	SWAP_MAX_SNDBUF			256
#define	SWAP_MAX_RCVBUF			1200

#define	SWAP_DECOMP_UNIT		2000			// Decomp display unit
#define	SWAP_LOAD_TIMEOUT		20				// Decomp loop delay

#define SWAP_LOAD_BLOCK			16 //8				// Load block count
#define SWAP_DECOMP_BLOCK		4				// decomp block count

#define	SWAP_EXCEPTION_TIMEOUT	5000			// Exception timeout

//==============================================================
//	LCD buffer size
//
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 7)/8)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF || SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_HEIGHT + 7)/8)*SWAP_LCD_WIDTH)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VERT_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_VERT_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_HEIGHT + 7)/8)*2*SWAP_LCD_WIDTH)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 3)/4)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 7)/8)*2*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 7)/8)*SWAP_LCD_HEIGHT*2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*((SWAP_LCD_HEIGHT + 7)/8)*2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 3)/4)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 1)/2)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR4K_HORIZ)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT*3/2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR4K_HORIZ_WORD)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT*2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR64K_HORIZ)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT*2)
#endif // SWAP_LCD_TYPE

//==============================================================
//	String length definition
//
#define	SWAP_LEN_MIN			11				// svMIN length
#define	SWAP_LEN_USERID			8				// svUserID length
#define	SWAP_LEN_GNAME			16				// svGameName length
#define	SWAP_LEN_IP				32				// IP string length
#define	SWAP_LEN_URL			256				// URL string length
#define	SWAP_LEN_TEXT			32				// Input chat text len
#define	SWAP_LEN_CHAT_TEXT		((SWAP_LCD_WIDTH + 5)/6)
#define	SWAP_LEN_CHAT			(SWAP_LCD_HEIGHT/13)
#define	SWAP_LEN_DIAL			11				// PTP dial length
#define	SWAP_RSVD_CHAT_SIZE		24				// Chatting display size

//==============================================================================
//
//	SWAP Script Info Structure
//
//==============================================================================

typedef	struct _SSBInfo {
	BYTE	ValidCount;							// Valid number of running count, 0=invalid, 0xFF=unlimited count
	BYTE	ValidTerm[3];						// Valid date

	char	ScriptDir[SWAP_LEN_GNAME + 1];		// Directory name
	char	ScriptName[SWAP_LEN_GNAME + 1];		// Script name
	char	UserID[SWAP_LEN_USERID + 1];		// User ID
	WORD	ScriptVer;							// Script version
	BYTE	ImageType;							// Script image type
	WORD	ScriptCPID;							// CP ID
	WORD	ScriptID;							// Script ID
	BYTE	ScriptType;							// Script type
	DWORD	ScriptSize;							// Script size
	char	SvrIP[SWAP_LEN_IP + 1];				// Server IP
	WORD	SvrPort;							// Server port
} SSBInfo;

//==============================================================================
//
//	SWAP Key code
//
//==============================================================================

#define	SWAP_KEY_1				0x01
#define	SWAP_KEY_2				0x02
#define	SWAP_KEY_3				0x03
#define	SWAP_KEY_4				0x04
#define	SWAP_KEY_5				0x05
#define	SWAP_KEY_6				0x06
#define	SWAP_KEY_7				0x07
#define	SWAP_KEY_8				0x08
#define	SWAP_KEY_9				0x09
#define	SWAP_KEY_0				0x0A
#define	SWAP_KEY_STAR			0x0B
#define	SWAP_KEY_SHARP			0x0C

#define	SWAP_KEY_1_L			0x81
#define	SWAP_KEY_2_L			0x82
#define	SWAP_KEY_3_L			0x83
#define	SWAP_KEY_4_L			0x84
#define	SWAP_KEY_5_L			0x85
#define	SWAP_KEY_6_L			0x86
#define	SWAP_KEY_7_L			0x87
#define	SWAP_KEY_8_L			0x88
#define	SWAP_KEY_9_L			0x89
#define	SWAP_KEY_0_L			0x8A

#define	SWAP_KEY_CLR			0x0D
#define	SWAP_KEY_SND			0x0E
#define	SWAP_KEY_END			0x0F

#define	SWAP_KEY_LEFT			0x10
#define	SWAP_KEY_RIGHT			0x11
#define	SWAP_KEY_UP				0x12
#define	SWAP_KEY_DOWN			0x13
#define	SWAP_KEY_OK				0x14

//==============================================================================
//
//	SWAP Buzzer definition
//
//==============================================================================

typedef	enum {
	SWAP_NOTE_G3,	// 0
	SWAP_NOTE_G3P, 
    SWAP_NOTE_A3,  
    SWAP_NOTE_A3P, 
    SWAP_NOTE_B3,  
    SWAP_NOTE_C4,  
    SWAP_NOTE_C4P, 
    SWAP_NOTE_D4,  
    SWAP_NOTE_D4P, 
    SWAP_NOTE_E4,  
    SWAP_NOTE_F4,	// 10
    SWAP_NOTE_F4P, 
    SWAP_NOTE_G4,  
    SWAP_NOTE_G4P, 
    SWAP_NOTE_A4,  
    SWAP_NOTE_A4P, 
    SWAP_NOTE_B4,  
    SWAP_NOTE_C5,  
    SWAP_NOTE_C5P, 
    SWAP_NOTE_D5,  
    SWAP_NOTE_D5P,	// 20
    SWAP_NOTE_E5,  
    SWAP_NOTE_F5,  
    SWAP_NOTE_F5P, 
    SWAP_NOTE_G5,  
    SWAP_NOTE_G5P, 
    SWAP_NOTE_A5,  
    SWAP_NOTE_A5P, 
    SWAP_NOTE_B5,  
    SWAP_NOTE_C6,  
    SWAP_NOTE_C6P,	// 30
    SWAP_NOTE_D6,  
    SWAP_NOTE_D6P, 
    SWAP_NOTE_E6,  
    SWAP_NOTE_F6,  
    SWAP_NOTE_F6P, 
    SWAP_NOTE_G6,
	SWAP_NOTE_R,
	SWAP_NOTE_END	// 38: End of tone table
} S_NoteDef;

typedef	__packed struct _S_NoteType	{
	BYTE	Note;						// Tone height, SWAP_NOTEDef type
	BYTE	Milisec;					// Tone duration, 3 milisecond unit
} S_NoteType;

//==============================================================================
//
//	SWAP System Variables
//
//==============================================================================

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
extern	WORD	*swLcdBuffer;			// Foreground LCD buffer
extern	WORD	*swBackLcdBuffer;		// Background LCD buffer
#else
extern	BYTE	*swLcdBuffer;			// Foreground LCD buffer
extern	BYTE	*swBackLcdBuffer;		// Background LCD buffer
#endif
extern	BYTE	*swScriptBuffer;		// Script download/run buffer
extern	BYTE	*swWorkSpace;			// SWAP work space
extern	char	svRsvdChat[9][SWAP_RSVD_CHAT_SIZE + 1];	// Reserved chatting text

//==============================================================================
//
//	MIDD(Mobile Interface Device Driver)
//
//==============================================================================

//==============================================================
//	SRAM Memory management
//
extern	void	midd_InitMemory(void);
extern	void	midd_CloseMemory(void);

//==============================================================
//	Script download & SSB control
//
extern	int		midd_GetScriptInfo(int ssbIndex, SSBInfo* ssbInfo);
extern	void	midd_PutScriptInfo(int ssbIndex, SSBInfo* ssbInfo);
extern	int		midd_CheckFreeSSB(DWORD newSize, int *ssbIndex);
extern	void	midd_WriteScriptInfo(int ssbIndex, SSBInfo* ssbInfo);
extern	int		midd_LoadScriptBlock(int ssbIndex, DWORD start, int bsize, BYTE *buf);
extern	void	midd_DeleteScript(int ssbIndex);
extern	void	midd_GetUserNV(int ssbIndex, BYTE *data, int size);
extern	void	midd_PutUserNV(int ssbIndex, BYTE *data, int size);
#ifdef SWAP_FEATURE_PTP
extern	void	midd_GetPtpDial(char *dial);
extern	void	midd_PutPtpDial(char *dial);
#endif // SWAP_FEATURE_PTP

//==============================================================
//	LCD/Audio Control
//
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
extern	void	midd_DrawLcd(WORD *buf);
#else
extern	void	midd_DrawLcd(BYTE *buf);
#endif
extern	void	midd_PlaySound(BYTE *data, int size);
extern	void	midd_StopSound(void);
extern	void	midd_PlayVocoder(BYTE *data, int size);
extern	void	midd_StopVocoder(void);
extern	void	midd_StartVib(int t);
extern	void	midd_StopVib(void);
extern	void	midd_SetKeyTone(int sw);
extern	void	midd_SetBacklight(int sw);

//==============================================================
//	Timer Control
//
extern	void	midd_StartTimer(int id, int t, int repeat);
extern	void	midd_StopTimer(int id);

//==============================================================
//	Network API
//
#ifdef SWAP_FEATURE_NETWORK
extern	void	midd_Connect(char *ip, WORD port, int reasons);
extern	void	midd_Disconnect(int ret);
extern	int		midd_SendData(BYTE *data, BYTE len);
#ifdef SWAP_FEATURE_PTP
extern	void	midd_PtpCall(char *dial);
extern	void	midd_PtpWait(void);
extern	void	midd_PtpRelease(int ret);
extern	int		midd_PtpSendData(BYTE *data, BYTE len);
#endif // SWAP_FEATURE_PTP
#endif // SWAP_FEATURE_NETWORK
extern	void	midd_GetChatText(void);

//==============================================================
//	RS232C API
//
#ifdef SWAP_FEATURE_SERIAL
extern	void	midd_RsCom(BYTE code, int size, BYTE *data);
extern	void	midd_RsDownload(int size, BYTE *data);
#endif // SWAP_FEATURE_SERIAL

//==============================================================
//	Common API
//
extern	int		midd_SendSMS(WORD ti, char *dial, char *data);
extern	void	midd_BackToBrowser(char *url);
extern	void	midd_GetMIN(char *minStr);
extern	void	midd_VoiceCall(char *dial, int ret);
extern	void	midd_GetLocInfo(WORD *w);
extern	int		midd_GetDate(INT2 *year, INT2 *month, INT2 *day, INT2 *wday);
extern	int		midd_GetTime(INT2 *hour, INT2 *min, INT2 *sec, INT2 *msec);
extern	void	midd_RegMusicBell(char *name, BYTE *data, int size);
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
extern	void	midd_RegScreen(char *name, WORD *buf);
#else
extern	void	midd_RegScreen(char *name, BYTE *buf);
#endif

//==============================================================
//	Vender API
//
#ifdef SWAP_FEATURE_IS95C
extern	void	midd_SetSoMo(WORD so, WORD mo);
#endif // SWAP_FEATURE_IS95C

#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)
extern	void	midd_RegAdvAudio(char *name, BYTE *data, int size);
extern	void	midd_PlayAdvAudio(BYTE *data, int size);
extern	void	midd_PauseAdvAudio(void);
extern	void	midd_RestartAdvAudio(void);
extern	void	midd_StopAdvAudio(void);
#endif // SWAP_AAD_TYPE

extern	int		midd_ReadHandsetData(INT2 code, INT2 major, INT2 minor, BYTE *buf);
extern	int		midd_WriteHandsetData(INT2 code, INT2 major, INT2 minor, BYTE *buf);

//==============================================================================
//
//	SWAP Menu
//
//==============================================================================

//==============================================================
//	Event code of scl_DoSwapEvent
//
enum {
	SWAP_EVENT_MENU,
	SWAP_EVENT_DOWNLOAD_START,
	SWAP_EVENT_SVM_START,
	SWAP_EVENT_SVM_PTP_START,
	SWAP_EVENT_TEXTEDIT,
	SWAP_EVENT_CHATEDIT,
	SWAP_EVENT_CONTINUE,
	SWAP_EVENT_SMS_RESULT,
	SWAP_EVENT_CONNECT_RESULT,
	SWAP_EVENT_DISCON_OK,
	SWAP_EVENT_PTPDIAL,
	SWAP_EVENT_PTPWAIT,
	SWAP_EVENT_PTP_RESULT,
	SWAP_EVENT_PTPDISCON_OK
};

//==============================================================
//	SWAP menu state
//
enum {
	SWAP_MENU_ENTRANCE,
	SWAP_MENU_RECEIVE,
	SWAP_MENU_TIMEOUT,
	SWAP_MENU_KEYIN,
	SWAP_MENU_CALL,
	SWAP_MENU_CALLDROP,
	SWAP_MENU_EXIT
};

//==============================================================
//	SWAP menu return code
//
enum	{
	SWAP_RC_NONE,
	SWAP_RC_EXIT,
	SWAP_RC_EXEC_EXIT,
	SWAP_RC_TEXTEDIT,
	SWAP_RC_CHATEDIT,
	SWAP_RC_OUT_OF_MEM,
	SWAP_RC_CONNECT,
	SWAP_RC_CONNECT_FAIL,
	SWAP_RC_DISCONNECT,
	SWAP_RC_DISCONNECT_FAIL,
	SWAP_RC_INCOMINGCALL,
	SWAP_RC_VOICECALL,
	SWAP_RC_VOICECALL_FAIL,
	SWAP_RC_PTPCALL,
	SWAP_RC_PTPCALLMMI,
	SWAP_RC_PTPWAIT,
	SWAP_RC_PTPCALL_FAIL,
	SWAP_RC_PTPRELEASE,
	SWAP_RC_PTPRELEASE_FAIL,
	SWAP_RC_DOWNLOAD,
	SWAP_RC_DOWNLOAD_CANCEL,
	SWAP_RC_BROWSER,
	SWAP_RC_BACK_TO_BROWSER,
	SWAP_RC_EXCEPTION,
	SWAP_RC_ERROR
};

//==============================================================
//	Socket disconnect result reason
//
#define SWAP_RR_DISCONNECT				0
#define SWAP_RR_DISCONNECT_DLSSBERR		1
#define SWAP_RR_DISCONNECT_DLMEMERR		2
#define SWAP_RR_DISCONNECT_DLSCRERR		3
#define SWAP_RR_DISCONNECT_DLSIZEERR	4
#define SWAP_RR_DISCONNECT_DLSUMERR		5
#define SWAP_RR_DISCONNECT_DLAUTHERR	6
#define SWAP_RR_DISCONNECT_DLPKTERR		7
#define SWAP_RR_DISCONNECT_DLCOMP		8
#define SWAP_RR_DISCONNECT_DLRETRYFAIL	9
#define SWAP_RR_DISCONNECT_DLCALLDROP	10
#define SWAP_RR_DISCONNECT_DLEXIT		11
#define SWAP_RR_DISCONNECT_BROWSER		12
#define SWAP_RR_DISCONNECT_CALLDROP		13
#define SWAP_RR_DISCONNECT_SVREXIT		14
#define SWAP_RR_DISCONNECT_EXIT			15

#ifdef SWAP_DOWNLOAD_RCVERR
#define SWAP_RR_DISCONNECT_NAK			16
#endif

//==============================================================
//	PTP release result reason
//
#ifdef SWAP_FEATURE_PTP
#define SWAP_RR_PTPDISCONNECT			0
#define SWAP_RR_PTPDISCONNECT_WAIT		1
#define SWAP_RR_PTPDISCONNECT_RUNWAIT	2
#define SWAP_RR_PTPDISCONNECT_BROWSER	3
#define SWAP_RR_PTPDISCONNECT_CALLDROP	4
#define SWAP_RR_PTPDISCONNECT_PTPEXIT	5
#define SWAP_RR_PTPDISCONNECT_EXIT		6
#endif

//==============================================================
//	SWAP event, return code structure
//
enum	{
	SWAP_ERR_DECOMPFAIL,
	SWAP_ERR_LOADFAIL,
	SWAP_ERR_DOWNFAIL,
	SWAP_ERR_DOWNFAILSIZE,
	SWAP_ERR_DOWNFAILSUM,
	SWAP_ERR_AUTHFAIL,
	SWAP_ERR_NETSTARTFAIL,
	SWAP_ERR_WRONGPTP
};

//==============================================================
//	SWAP event, return code structure
//
typedef struct {
	BYTE	event;
	BYTE	*buf;
	int		data;
} S_MenuParam;

typedef struct {
	char	*ip;
	WORD	port;
	BYTE	reasons;
} S_SvrParam;

typedef struct {
	DWORD	size;
	DWORD	lack;
	int		index;
} S_SsbParam;

typedef struct {
	char	*title;
	char	*input;
	BYTE	maxlen;
	BYTE	strtype;
} S_EditParam;

typedef struct {
	char	*dial;
	BYTE	ret;
} S_CallParam;

typedef	struct {
	int		ecode;
	DWORD	addr;
	DWORD	pc;
#ifdef SWAP_DOWNLOAD_RCVERR
	char	*str;
#endif
} S_Exception;

typedef	struct {
	int		code;
	union {
		int			data;
		char		*str;
		S_MenuParam	menu;
	} param;
} S_Event;

typedef	struct {
	int		code;
	union {
		int			data;
		char		*str;
		S_SsbParam	ssb;
		S_EditParam	edit;
		S_SvrParam	svr;
		S_CallParam	call;
		S_Exception	exception;
	} param;
} S_Result;

//==============================================================
//	External SCL
//
extern	void		scl_RsEvent(int code, int size, BYTE *data);
extern	void		scl_RsUpload(int size, BYTE *data);

#ifdef SWAP_FEATURE_DOWNLOAD
extern	void		scl_StoreRunInfo(char *uid, WORD cpid, WORD gid,
									 char *dlsIP, WORD dlsPort,
									 char *svrIP, WORD svrPort, char *retURL);
#endif // SWAP_FEATURE_DOWNLOAD

extern	int			scl_CheckValidate(int ssbIndex);
extern	S_Result	*scl_DoSwapEvent(S_Event *event);
extern	int			scl_MakeScriptInfo(SSBInfo* ssbInfo);
extern	DWORD		scl_ModifyScriptInfo(int ssbIndex, int set);

//==============================================================================
//
//	Resource definition
//
//==============================================================================

#ifdef	SWAP_LCD_COLOR

#define	SWAP_CR_MARGIN			3			// black
#define	SWAP_CR_MAIN			0			// white
#define	SWAP_CR_MAIN_SHADOW		1			// light gray
#define	SWAP_CR_TITLE			46			// cyan
#define	SWAP_CR_TITLE_SHADOW	18			// blue
#define	SWAP_CR_DOWN			109			// Red
#define	SWAP_CR_DOWN_SHADOW		47			// dark red
#define	SWAP_CR_LOAD			43			// Red
#define	SWAP_CR_LOAD_SHADOW		23			// dark red
#define	SWAP_CR_PROG_BLANK		1			// light gray

#define	SWAP_CR_CHAT_BACK		3			// black
#define	SWAP_CR_CHAT_COM		137			// yellow
#define	SWAP_CR_CHAT_COM_BACK	16			// dark blue
#define	SWAP_CR_CHAT_TEXT		0			// white

#else	// Gray LCD

#define	SWAP_CR_MARGIN			0			// white
#define	SWAP_CR_MAIN			0			// white
#define	SWAP_CR_MAIN_SHADOW		0			// white
#define	SWAP_CR_TITLE			0			// white
#define	SWAP_CR_TITLE_SHADOW	0			// black
#define	SWAP_CR_DOWN			3			// black
#define	SWAP_CR_DOWN_SHADOW		2			// dark gray
#define	SWAP_CR_LOAD			2			// dark gray
#define	SWAP_CR_LOAD_SHADOW		3			// black
#define	SWAP_CR_PROG_BLANK		1			// light gray

#define	SWAP_CR_CHAT_BACK		0			// white
#define	SWAP_CR_CHAT_COM		0			// white
#define	SWAP_CR_CHAT_COM_BACK	3			// black
#define	SWAP_CR_CHAT_TEXT		3			// black

#endif // SWAP_LCD_COLOR

#if (SWAP_LANG == SWAP_ASCII || SWAP_LANG == SWAP_HEBREW)

#define	SWAP_TEXT_CHAT_TITLE	"Input Chat"
#define	SWAP_TEXT_CHAT_COM1		"1-9:Reserved"
#define	SWAP_TEXT_CHAT_COM2		"0:Input"
#define	SWAP_TEXT_SWAP10_DIR	"SWAP1.0 Contents"

#define	SWAP_TEXT_LD_TITLE		"Loading..."

#define	SWAP_TEXT_DL_NONAME		"?"
#define	SWAP_TEXT_DL_READY		"Connecting..."
#define	SWAP_TEXT_DL_TITLE		"Downloading..."
#define	SWAP_TEXT_DL_CANCEL		"Download Cancel!"
#define	SWAP_TEXT_DL_FAIL		"Download Fail!"
#define	SWAP_TEXT_DL_TIMEOUT	"Download Timeout!"

#define	SWAP_TEXT_DL_OUTOFSSB	"Out of Script Slot!"
#define	SWAP_TEXT_DL_OUTOFMEM	"Out of Memory!"
#define	SWAP_TEXT_DL_GUIDE1		"To Ensure Download,"
#define	SWAP_TEXT_DL_GUIDE2		"Remove Stored Script"

#define	SWAP_TEXT_PTPDIAL		"1:1 Dial Number"
#define	SWAP_TEXT_PTPCALL		"1:1 Calling..."
#define	SWAP_TEXT_PTPWAIT		"1:1 Call Waiting..."

#elif (SWAP_LANG == SWAP_KSC5601)

#define	SWAP_TEXT_CHAT_TITLE	"채팅 입력"
#define	SWAP_TEXT_CHAT_COM1		"1-9:예약어"
#define	SWAP_TEXT_CHAT_COM2		"0:입력"
#define	SWAP_TEXT_SWAP10_DIR	"GVM1X 컨텐츠"

#define	SWAP_TEXT_LD_TITLE		"실행준비 중..."

#define	SWAP_TEXT_DL_NONAME		"?"
#define	SWAP_TEXT_DL_READY		"서버 연결중..."
#define	SWAP_TEXT_DL_TITLE		"다운로드 중..."
#define	SWAP_TEXT_DL_CANCEL		"다운로드 중지!"
#define	SWAP_TEXT_DL_FAIL		"다운로드 실패!"
#define	SWAP_TEXT_DL_TIMEOUT	"다운로드 타임아웃!"

#define	SWAP_TEXT_DL_OUTOFSSB	"저장 목록 부족!"
#define	SWAP_TEXT_DL_OUTOFMEM	"저장 메모리 부족!"
#define	SWAP_TEXT_DL_GUIDE1		"기존컨텐츠 삭제 후"
#define	SWAP_TEXT_DL_GUIDE2		"저장할 수 있습니다."

#define	SWAP_TEXT_PTPDIAL		"1:1 발신번호?"
#define	SWAP_TEXT_PTPCALL		"1:1 발신 중..."
#define	SWAP_TEXT_PTPWAIT		"1:1 착신대기 중..."

#elif (SWAP_LANG == SWAP_SHIFTJIS)

#define	SWAP_TEXT_CHAT_TITLE	"Input Chat"
#define	SWAP_TEXT_CHAT_COM1		"1-9:Reserved"
#define	SWAP_TEXT_CHAT_COM2		"0:Input"
#define	SWAP_TEXT_DL_NONAME		"?"
#define	SWAP_TEXT_SWAP10_DIR	"SWAP1.0 Contents"

#define	SWAP_TEXT_LD_TITLE		"Loading..."

#define	SWAP_TEXT_DL_READY		"Connecting..."
#define	SWAP_TEXT_DL_TITLE		"Downloading..."
#define	SWAP_TEXT_DL_CANCEL		"Download Cancel!"
#define	SWAP_TEXT_DL_FAIL		"Download Fail!"
#define	SWAP_TEXT_DL_TIMEOUT	"Download Timeout!"

#define	SWAP_TEXT_DL_OUTOFSSB	"Out of Script Slot!"
#define	SWAP_TEXT_DL_OUTOFMEM	"Out of Memory!"
#define	SWAP_TEXT_DL_GUIDE1		"To Ensure Download,"
#define	SWAP_TEXT_DL_GUIDE2		"Remove Stored Script"

#define	SWAP_TEXT_PTPDIAL		"1:1 Dial Number"
#define	SWAP_TEXT_PTPCALL		"1:1 Calling..."
#define	SWAP_TEXT_PTPWAIT		"1:1 Call Waiting..."

#endif // SWAP_LANG

#endif // _SWAP_H_
