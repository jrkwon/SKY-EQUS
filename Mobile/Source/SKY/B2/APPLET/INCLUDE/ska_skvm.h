/*
** ============================================================================
**
** Project: 
**     neo-penta
**
** File:
**     ska_skvm.h
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

#ifndef _SKA_SKVM_H_
#define _SKA_SKVM_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FEATURE_SKVM	
/* 
**----------------------------------------------------------------------------
**  Includes
*----------------------------------------------------------------------------
*/
    
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
enum tagSKVMCOLOR{
	SKVM_COLOR256,
	SKVM_COLOR64K,
    SKVM_COLOR_MAX
};
typedef enum tagSKVMCOLOR SKVM_COLOR_E;
extern SKVM_COLOR_E g_Eskvmcolor;

#define	MSDURL_MAX				130	
#define INIT_SKVM_MSDURL		1  // MSD URL을 넘겨 XVM시작
#define INIT_SKVM_INPUTDATA		2  // XVM이 실행중이 아닌 경우 SMS통해 Application Data를 수신받은 경우 XVM시작 
#define INIT_SKVM_BACKGROUND	3  // JAVA 컨텐츠를 배경화면으로 실행하는 경우 
#define DATA_MAX				80
#define	RET_NOT_PROCESS			-1
#define	RET_SUSPEND_0			0
#define	RET_SUSPEND_1			1
#define NULL_ID					-1
#define GET_SKVM_STATE		(*g_pSkvmState)
#define SKVM_IS_SMS_DOWNLOAD()	 g_bsmsdowmload
#define SKVM_SET_SMS_DOWNLOAD(f) g_bsmsdowmload = f
#define SKVM_IS_FIRST_REFRESH()	 g_bfirstrefresh
#define SKVM_SET_FIRST_REFRESH(f)g_bfirstrefresh = f
#define SKVM_GET_COLOR_ENV()	 g_Eskvmcolor
#define SKVM_SET_COLOR_ENV(f)	 g_Eskvmcolor   = f 
#define SKVM_IS_MUSIC_PLAY()	 music_play_in_skvm
#define SKVM_SET_MUSIC_PLAY(f)	 music_play_in_skvm   = f 
#define SKVM_IS_PLAY_BG()		 g_bPlaySkvmInBG
#define SKVM_SET_PLAY_BG(f)		 g_bPlaySkvmInBG  = f 
#define SKVM_IS_DORMANT()		 g_bEntryDormant
#define SKVM_SET_DORMANT(f)		 g_bEntryDormant = f 
#define SKVM_IS_DOWNLOAD_MODE()  g_bDownLoadMode
#define SKVM_SET_DOWNLOAD_MODE(f)g_bDownLoadMode = f
#define SKVM_GET_SOCKETID()		 g_iSocketID 
#define SKVM_SET_SOCKETID(f)	 g_iSocketID = f
	
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern int gvm_app_idx;
extern UINT16 nTIValue;
extern BOOL	music_play_in_skvm;


/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern UCHAR g_pszmsdURL[MSDURL_MAX];
extern UCHAR g_pszretURL[MSDURL_MAX];
extern char  retUrlFromJava[132];	// must be more than 128 bytes 
									// XVM core에서 Wap으로 되돌아갈 url을 setting함 

extern INT   g_scvOption;
extern UCHAR g_pszSKVMInputData[DATA_MAX];
extern INT	 g_iSKVMInputDataLen;
extern INT*  g_pSkvmState;
extern BOOL  g_bsmsdowmload;
extern BOOL  g_bfirstrefresh;
extern BOOL	 g_bPlaySkvmInBG;
extern BOOL  g_bStopByUser;
extern BOOL  g_bEntryDormant; 
extern BOOL  g_bDownLoadMode; 
extern int	 g_iSocketID;
extern short	  xvm_errnum;
extern const char xvm_errmsgs[];

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern VOID SKY_API SKA_MainSKVM(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_MainSKVMBG(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_SKVMInputEnv(SKY_EVENT_T *pEvent);
extern void SKVMStart(void);
extern void SKVMStartBymsdURL(char *msdURL,char *retURL,int svcOption);
extern void SKVMStartBG(void);
extern void SKVMStop(void);
extern void SKVMStopBG(void);
extern void SKVMPause(void);
extern void SKVMResume(void);
extern void SKVMInputData(unsigned char * msg,int msgLen);
extern void SKVMSetKeyEvent(void);
extern int  SKVMMain(void);
extern void SKVM_gvm_start(int gvm_id);
extern void SKVM_ntop_start(void);
extern void SKVM_SocketStateChanged(void);
extern void SKVM_NetworkStateChanged(sint15 errno);
extern void xsig_sound_play_end(int ret);
extern void xvm_pppclose();

#ifndef	__XVMSTATE__
#define	__XVMSTATE__

#ifndef SLEEP_PRI
#define SLEEP_PRI	50
#endif
#ifndef FS_PRI
#define  FS_PRI     78   /* REX priority for the File System Task     */
#endif

//#define XUI_PRI			(SLEEP_PRI + 1)
#define XUI_PRI			(FS_PRI + 1)
#define XVM_TIMERS_SIG	0x8000    /*reuse TASK_START_SIG */


extern int	xvm_state;

#define	XVMSTATE_ERROR			-1
#define	XVMSTATE_DEAD			0x0
#define	XVMSTATE_START			0x1
#define	XVMSTATE_RUNNING		0x2
#define	XVMSTATE_SUSPEND		0x3
#define	XVMSTATE_STOP			0x4
#define	XVMSTATE_STOPPED		0x5
#define	XVMSTATE_FROZEN			0x10

/* (xvm_state != XVMSTATE_DEAD && xvm_state != XVMSTATE_FROZEN) */
#define	xvmProcessing	(xvm_state & 0x0f)
#define	xvmActive	(xvm_state == XVMSTATE_RUNNING || xvm_state == XVMSTATE_SUSPEND || xvm_state == XVMSTATE_FROZEN)

extern short xvm_errnum;

#define XVMERR_NORMAL			0	/* normal exit */
#define XVMERR_ABORT			1	/* xvm abort */

#define XVMERR_CLASSFORMAT		2	/* class format */
#define XVMERR_CLASSSIG			3	/* malformed sig */
#define XVMERR_NOTCONST			4	/* not constant */
#define XVMERR_BYTECODE			5	/* bytecode err */

#define XVMERR_OUTOFMEMORY		6	/* out of memory */
#define XVMERR_MEMCONSIST		7	/* memory consist */
#define XVMERR_HEAPALLOC		8	/* heap alloc err */
#define XVMERR_MEMMARK			9	/* memory mark */
#define XVMERR_GCMARK			10	/* gc mark err */

#define XVMERR_NOSUCHMETHOD		11	/* no such method */
#define XVMERR_NOSUCHFIELD		12	/* no such field */
#define XVMERR_MEMBERACCESS		13	/* member access */

#define XVMERR_THROWEXCEPTION		14	/* throw except */
#define XVMERR_UNCATCHED		15	/* uncatched err */
#define XVMERR_THREADRESUME		16	/* thread resume */
#define XVMERR_THREADSWITCH		17	/* thread switch */
#define XVMERR_MONITOR			18	/* monitor err */

#define XVMERR_JLOGBUF			19	/* jlog buf err */
#define XVMERR_ASSERT			20	/* assert err */
#define XVMERR_FILEMAP			21	/* filemap err */

#endif	/* !__XVMSTATE__ */

#endif
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SKVM_H_ */
