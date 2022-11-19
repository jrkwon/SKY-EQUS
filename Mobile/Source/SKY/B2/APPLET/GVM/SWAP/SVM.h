//==============================================================================
//
//	SINJISOFT SWAP MODULE LIBRARY HEADER
//
//	FILE NAME
//		SVM.h
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
//		2001.10.17	Version 1.55.01 Release
//
//==============================================================================

#ifndef	_SVM_H_
#define	_SVM_H_

#include "SWAP.h"
#include "Mil.h"

//==============================================================================
//
//	SWAP Definition
//
//==============================================================================

//==============================================================
//	SWAP version
//
	#define	SWAP_VERSION_10			0x01
	#define	SWAP_VERSION_15			0x02

#define	SWAP_VERSION			SWAP_VERSION_15

//==============================================================================
//
//	SWAP Virtual Machine Parameters
//
//==============================================================================

#define	SWAP_STACK_SIZE			64			// Max calc stack size
#define	SWAP_PC_STACK_SIZE		16			// Max call stack size

#define	SWAP_MEDIA_ACCEPT_DIFF	16			// Media size threshold
#define	SWAP_DOWNLOAD_RETRY		3			// Download retry

#define	SWAP_COMPRESS_POS		0x64		// Compress position

//==============================================================================
//
//	SWAP Script Structure
//
//==============================================================================

//==============================================================
//	Script file header
//
typedef __packed struct _S_HeaderType {

	////////////////////////////////
	// Script info
	////////////////////////////////

	// 0x00:

	BYTE	ScriptVer;
	BYTE	LcdClass;
	BYTE	ImageType;
	BYTE	AudioType;
	BYTE	VocType;
	BYTE	ScriptType;

	// 0x06:

	WORD	ScriptCPID;						// Script manufacturer ID
	WORD	ScriptID;						// Script ID
	char	ScriptName[16];					// Script name

	// 0x1A:

	WORD	Auth;							// Not used

	////////////////////////////////
	// Entry table
	////////////////////////////////

	// 0x1C:

	WORD	StartEntry;
	WORD	EndEntry;
	WORD	TimeoutEntry;
	WORD	KeyEntry;
	WORD	RSEventEntry;
	WORD	RSReceiveEntry;
	WORD	ResultEntry;
	WORD	ReceiveEntry;

	////////////////////////////////
	// Data table
	////////////////////////////////

	// 0x2C:

	WORD	SymTable;
	WORD	SymPool;
	WORD	MediaTable;
	WORD	MediaPool;

	//======================================
	// Additional attribute field
	// on SWAP version 1.5
	//======================================

	////////////////////////////////
	// Script directory name
	////////////////////////////////

	// 0x34:

	char	DirName[16];					// Directory name
	char	MakerName[16];					// Maker name

	BYTE	ValidCount;						// Valid number of running count, 0xFF=unlimited count
	BYTE	ValidTerm;						// Valid number of day, 0xFF=unlimited term

	DWORD	AppAuth;						// Application auth field
	DWORD	UserAuth;						// User auth field
	BYTE	UserData;

	// 0x5F

	BYTE	Reserved[5];					// Reserved field

	// 0x64: Compress start point

} S_HeaderType;

//==============================================================
//	Symbol Table on the ROM (Script)
//
typedef __packed struct _S_SymElem {
	BYTE	var;							// 0=Const, 1=Variable
	BYTE	dim;							// Dimension
	BYTE	init;							// 0=No data, 1=Init data in pool
	BYTE	reserved;
} S_SymElem;

//==============================================================
//	Symbol Table on the RAM
//
typedef struct _S_RamSymElem {
	BYTE	var;							// 0=Const, 1=Variable
	BYTE	dim;							// Dimension
	INT2	*addr;							// Address
} S_RamSymElem;

//==============================================================
//	Media Table on the ROM (Script)
//
typedef __packed struct _SpMediaElem {
	BYTE	var;							// 0=Const, 1=Variable with init, 2=Variable without init
	BYTE	type;							// 0x00=String, 0x01=Image, 0x02=Sound, 0x03=Vocoder
	WORD	size;							// Media size
} S_MediaElem;

//==============================================================
//	Media Table on the RAM
//
typedef struct _SpRamMediaElem {
	BYTE	var;							// 0x00=Const, 0x01=Variable
	BYTE	type;							// 0x00=String, 0x01=Image, 0x02=Sound, 0x03=Vocoder
	WORD	size;							// Media size
	BYTE	*addr;							// Media address
} S_RamMediaElem;

//==============================================================================
//
//	SWAP System Variables
//
//==============================================================================

//==============================================================
//	Download reqest structure
//
typedef struct _ReqInfo {
	char	UserID[SWAP_LEN_USERID + 1];	// User ID
	WORD	ScriptCPID;						// CP ID
	WORD	ScriptID;						// Script ID
	char	DlsIP[SWAP_LEN_IP + 1];			// Download Server IP
	WORD	DlsPort;						// Download Server port
	char	SvrIP[SWAP_LEN_IP + 1];			// Server IP
	WORD	SvrPort;						// Server port
	char	RetURL[SWAP_LEN_URL + 1];		// Return URL
} ReqInfo;

//==============================================================
//	SWAP system variables
//
extern	char	svMIN[SWAP_LEN_MIN + 1];	// Handset number

extern	SSBInfo		svSSBInfo;				// Current script info
extern	int			svSSBIndex;				// Current script info index of NV-ROM

extern	BYTE		svMyUserID;				// PTP user ID
extern	BYTE		svRunScriptType;		// Current running mode
extern	DWORD		svScriptInfo[SWAP_SSB_SIZE];	// Delete flag of script

extern	BYTE		swDispMode;				// 0:Normal, 1:Chat mode

extern	int			swLcdWidth;				// LCD width
extern	int			swLcdHeight;			// LCD height

extern	ReqInfo		svReqInfo;				// Download requested SSB info
extern	S_Result	svResult;				// SWAP result info
extern	int			svResultReason;			// SWAP result reason
extern	char		svResultString[SWAP_LEN_URL + 1];	// SWAP result string (URL, MIN etc)

//==============================================================
//	Work space variables
//
extern	INT2			*swRcvBuf;			// Network receive buffer
extern	INT2			*swRsRcvBuf;		// RS-232C receive buffer
extern	S_RamMediaElem	*swRamMediaTable;	// RAM: media table

//==============================================================================
//
//	SCL(SWAP Control Library)
//
//==============================================================================

//==============================================================
//	Script running type
//
#define	SWAP_STYPE_ALONE			1		// Stand-alone
#define	SWAP_STYPE_PTP				2		// Phone-to-phone
#define	SWAP_STYPE_SERVER			3		// TCP/IP
#define	SWAP_STYPE_FREE				4		// Connect by stand-alone & free
#define	SWAP_STYPE_ALONE_PTP		5		// Stand-alone + Phone-to-phone
#define	SWAP_STYPE_ALONE_SERVER		6		// Stand-alone + TCP/IP
#define	SWAP_STYPE_PTP_SERVER		7		// Phone-to-phone + TCP/IP
#define	SWAP_STYPE_ALL				8		// Stand-alone + Phone-to-phone + TCP/IP

//==============================================================
//	Chat mode
//
#define	S_CM_GAME					0		// Chat mode
#define	S_CM_CHAT					1

//==============================================================
//	svm_Result parameters
//
#define	SWAP_RST_RECON_FAIL			0
#define	SWAP_RST_RECON_OK			1
#define	SWAP_RST_TEDIT_DONE			2
#define	SWAP_RST_MDOWN_OK			3
#define	SWAP_RST_MDOWN_FAIL			4
#define	SWAP_RST_INVALID_LCD		5
#define	SWAP_RST_RCV_CHAT			6
#define	SWAP_RST_SMS_OK				7
#define	SWAP_RST_SMS_FAIL			8
#define	SWAP_RST_DISCON_OK			9
#define	SWAP_RST_DISCON_FAIL		10
#define	SWAP_RST_TALK_FAIL			11
#define	SWAP_RST_PTPCON_OK			12
#define	SWAP_RST_PTPCON_FAIL		13
#define	SWAP_RST_PTPREL_OK			14
#define	SWAP_RST_PTPREL_FAIL		15
#define	SWAP_RST_ABN_DISCON			16
#define	SWAP_RST_ABN_PTPREL			17
#define	SWAP_RST_CONTINUE			18

//==============================================================
//	Download result reason
//
#define	SWAP_RR_DOWNLOAD			0
#define	SWAP_RR_CONNECT_START		1
#define	SWAP_RR_CONNECT_FROM_ALONE	2
#define	SWAP_RR_CONNECT_FROM_SVR	3
#define	SWAP_RR_PTPCALL				4
#define	SWAP_RR_PTPWAIT				5
#define	SWAP_RR_PTPCALL_FROM_ALONE	6
#define	SWAP_RR_PTPWAIT_FROM_ALONE	7
#define	SWAP_RR_PTP_CHATEDIT		8
#define	SWAP_RR_SVR_CHATEDIT		9

//==============================================================
//	Continue result reason
//
#define	SWAP_RR_RUNALONE			0
#define	SWAP_RR_SELECT				1
#define	SWAP_RR_SVRSELECT			2
#define	SWAP_RR_PTPSELECT			3
#define SWAP_RR_PTPREADY			4
#define	SWAP_RR_PTPDIAL				5
#define	SWAP_RR_PTPDIAL_FROM_ALONE	6
#define	SWAP_RR_SVRDOWNLOAD_RETRY	7

//==============================================================
//	Exception code
//
#define	SWAP_ECPT_STOP_OP			0
#define	SWAP_ECPT_DIV_ZERO			1
#define	SWAP_ECPT_MOD_ZERO			2
#define	SWAP_ECPT_SYM_OVER			3
#define	SWAP_ECPT_MDA_OVER			4
#define	SWAP_ECPT_STACK_OVER		5
#define	SWAP_ECPT_PCSTACK_OVER		6
#define	SWAP_ECPT_INVALID_OP		7
#define	SWAP_ECPT_INVALID_SYM_INDEX	8
#define	SWAP_ECPT_INVALID_MDA_INDEX	9
#define	SWAP_ECPT_INVALID_SYM_ADDR	10
#define	SWAP_ECPT_INVALID_MDA_ADDR	11
#define	SWAP_ECPT_OUT_OF_DIM		12

#ifdef SWAP_DOWNLOAD_RCVERR
#define SWAP_ECPT_RCV_NAK			99
#endif

extern	void	svm_ResetAllTimers(void);
extern	int		svm_SetMediaSize(int, int);
extern	void	svm_StartGame(void);
extern	void	svm_EndGame(void);
extern	void	svm_TimeOut(int id);
extern	void	svm_KeyPress(int Key);
extern	void	svm_Result(int result);
extern	void	svm_PauseGame(void);
extern	void	svm_ContinueGame(void);
extern	void	svm_ReceiveData(int);
extern	void	svm_SetText(char *str);
extern	void	svm_ReceiveChat(char*);

//==============================================================
//	Return code of mil_ReceiveData
//

#ifdef SWAP_FEATURE_NETWORK

#define	SWAP_RCV_NONE				1
#define	SWAP_RCV_DLCOMP				2
#define	SWAP_RCV_DLSSBERR			3
#define	SWAP_RCV_DLMEMERR			4
#define	SWAP_RCV_DLSCRERR			5
#define SWAP_RCV_DLSIZEERR			6
#define	SWAP_RCV_DLSUMERR			7
#define	SWAP_RCV_DLAUTHERR			8
#define	SWAP_RCV_DLPKTERR			9

#ifdef SWAP_DOWNLOAD_PKTRETRY
#define SWAP_RCV_DLPKTSKIP			10
#define SWAP_RCV_PROCFAIL			11

#endif

#ifdef SWAP_DOWNLOAD_RCVERR
#define	SWAP_RCV_DLNAKERR			12
#endif

//==============================================================
//	Send code of s_SendNak
//

#ifdef SWAP_DOWNLOAD_SNDERR

#define	SWAP_NAK_DLRETRY			0
#define	SWAP_NAK_DLSSBERR			1
#define	SWAP_NAK_DLMEMERR			2
#define	SWAP_NAK_DLSCRERR			3
#define SWAP_NAK_DLSIZEERR			4
#define	SWAP_NAK_DLSUMERR			5
#define	SWAP_NAK_DLAUTHERR			6
#define	SWAP_NAK_DLPKTERR			7
#define	SWAP_NAK_DLTIMEOUTERR		8
#define	SWAP_NAK_DLUSREXIT			9

#endif

#ifdef SWAP_FEATURE_DOWNLOAD

extern	int		mil_StartDownload(void);
extern	int		mil_RetryDownload(void);
extern	int		mil_SendNak(const WORD, const WORD);

#endif // SWAP_FEATURE_DOWNLOAD

extern	void	mil_InitNetwork(void);
extern	void	mil_SendChat(char*);
extern	int		mil_LoadMedia(const INT2, const INT2);
extern	int		mil_LoadMResult(const INT2);
extern	int		mil_SendData(const BYTE*, const WORD);
extern	int		mil_ReceiveData(BYTE* Data, WORD nLength);

#endif // SWAP_FEATURE_NETWORK

//==============================================================================
//
//	SWAP menu
//
//==============================================================================

extern	void	mil_DrawProg(DWORD down, DWORD size, int flag);

#endif // _SVM_H_
