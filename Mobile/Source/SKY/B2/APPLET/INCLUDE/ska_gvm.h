/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_gvm.h
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
** 2001-01-02 wshan      Created.
**
** ============================================================================
*/

#ifndef _SKA_GVM_H_
#define _SKA_GVM_H_

#ifdef __cplusplus
extern "C" {
#endif
	
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "..\GVM\ATSWAP\AtSwap.h"
#include "ska_mo.h"
    
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define GVM_LEN_CHATTEXT            9
#define MAX_SIZE_INPUT_CHAT_TEXT    33
#define GVM_ERROR                   -1
//#define MAX_SIZE_FILE_NAME          16
#define Cal_mbSize(dword, byte1, byte2, byte3, byte4)  \
			(dword = (((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4)) + 8 ))

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagGVM_CONNECT_SERVER_TYPE_E{
	GCST_NONE,
	GCST_DOWNLOAD, /* for Download */
	GCST_GAME,     /* for Game Server */
    GCST_MAX
};
typedef enum tagGVM_CONNECT_SERVER_TYPE_E GVM_CONNECT_SERVER_TYPE_E;

enum tagGVM_SMS_MO_TYPE_E{
	GSMT_IDLE,
	GSMT_SENDING,
	//GSMT_SUCCESS,
    GSMT_MAX
};
typedef enum tagGVM_SMS_MO_TYPE_E GVM_SMS_MO_TYPE_E;


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

enum tagWAP_MUSIC_BELL_INFO_E{
	MAX_MUSICBELL_TITLE = 20,
	MAX_MUSICBELL_SIZE  = 4096,//     -- 16 poly
                              //2048 --  4 poly
    MUSICBELL_PLAY_MODE = 0,
    MUSICBELL_DOWNLOAD_MODE = 1
};

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern GVM_CONNECT_SERVER_TYPE_E    g_TypeConnectSvr;
extern GVM_SMS_MO_TYPE_E            g_GVMSmsMoType;	
extern int                          g_GVMSmsMoTI;
extern SMSMO_SEND_T             	g_GVMSmsBuffer;
extern BOOL                         g_fGVMPaused;	
extern UINT8		                g_unTrySMS;                /* SMS retrial È½¼ö */

// undersea added
extern UINT8                        g_nCurSSB;
extern BOOL                         g_fDownloadRequestFromSMS;
extern BOOL                         g_fDeleteScript;
extern BOOL                         g_fIn256ColorDisplay;
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern VOID SKY_API SKA_MainGVM(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_RunGVM(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_Put256Bitmap(INT16 x, INT16 y, CONST BITMAP* pBitmap, BYTE* buffer);
extern VOID SKY_API sSKA_TextEdit(SKY_EVENT_T *pEvent);

extern VOID GetGVMFileName(BYTE *pabFileName, WORD wCPID, WORD wGID);
extern VOID GoToGVM( char* uid, int cpid, int gid, char* dlsIP, int dlsPort, char* svrIP, int svrPort, char* retURL, char* Result );
extern BOOL SaveGVM_BMPToFS(void *imgData, char *imgName);
extern BOOL CheckDiskSpaceForSetGVMImage(void);
//extern BOOL GetEmptySSB(VOID);
extern DWORD GetLengthofMusicData(int audioType, int Length, int mode);
extern void GVM_SocketStateChanged(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_GVM_H_ */
