/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_wav.c
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
** 2001-01-16   khekim     Created.
** 2001-05-10   yoursun	   Changed
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"
#include "msg.h"
#include "ska_wav.h"
#include "wav_multisock.h"
#include "sky_sock.h"
#include "ColorTable.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/************************************************************************************************************/
//////////////////////////////// Packet Size ///////////////////////////////////////////////// yoursun 01/04/19
#define MTU_SIZE    1500	//	Maximum Packet Size on Wavelet 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
//////////////////////////////// Timer 개수 //////////////////////////////////////////////////////////////////
#define TIMER_NUM				4  // 총 사용 Timer 개수 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// TCM Timer ID //////////////////////////////////////////////// yoursun 01/04/04 
#define TCM2_TIMER_ID1  92 // EQS_Timer1에 대응(1 = EQS_Timer1 = TCM2_TIMER_ID1 -TIMER_ID_CHANGE)
#define TCM2_TIMER_ID2	93 // EQS_Timer2에 대응(2 = EQS_Timer2 = TCM2_TIMER_ID2 -TIMER_ID_CHANGE)
#define TCM2_TIMER_ID3	94 // EQS_Timer3에 대응(3 = EQS_Timer3 = TCM2_TIMER_ID3 -TIMER_ID_CHANGE)
#define TCM2_TIMER_ID4  95 // General_Timer에 대응 <= Alive Message를 보내는데 사용된다.
						   // (0 = General_Timer = TCM2_TIMER_ID3 -TIMER_ID_CHANGE - WAV_GENERAL_TIMER)
						   // (4 = WAV_GENERAL_TIMER = TCM2_TIMER_ID3 -TIMER_ID_CHANGE)					
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// UI Timer ID //////////////////////////////////////////////// yoursun 01/04/19
#define TIMER_ID_CHANGE		91 // UI의 Timer와 TCM의 Timer간의 전환을 위해 필요
#define WAV_GENERAL_TIMER	4  // 95번 Timer를 General Timer에 대응하기 위해 필요
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************************/


/************************************************************************************************************/
//////////////////////////////// Font Size ////////////////////////////////////////////////////yoursun 01/04/07
#if SKY_MODEL_ID == SKY_MODEL_B1 // B1 Model에 대한 폰트 크기
#define HANGUL_FONT_X 	12 // 한글폰트의 너비(픽셀단위)
#define HANGUL_FONT_Y 	13 // 한글폰트의 길이(픽셀단위) 
#define YOUNGMUN_FONT_X	6  // 영문폰트의 너비(픽셀단위)
#define YOUNGMUN_FONT_Y	13 // 영문폰드의 길이(픽셀단위)

#elif (SKY_MODEL_ID == SKY_MODEL_B2) // B2 Model에 대한 폰트 크기
#define HANGUL_FONT_X 	14 // 한글폰트의 너비(픽셀단위)
#define HANGUL_FONT_Y 	16 // 한글폰트의 길이(픽셀단위)
#define YOUNGMUN_FONT_X	 7 // 영문폰트의 너비(픽셀단위)
#define YOUNGMUN_FONT_Y	16 // 영문폰드의 길이(픽셀단위)
#endif 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Display Size of Video ///////////////////////////////////////yoursun 01/04/10
#define DISPLAY_X		SKY_SCREEN_WIDTH	// LCD With
#define DISPLAY_Y		SKY_SCREEN_HEIGHT   // LCD Height
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Buffer Message ///////////////////////////////////////////// yoursun 01/03/20
#define WAVELET_BUFFER_MSG_XPOS 10	// 다운로드나 버퍼링 정도를 표시하는 가로 위치
#define WAVELET_BUFFER_MSG_YPOS 80  // 다운로드나 버퍼링 정도를 표시하는 세로 위치
#define BUFFER_MSG_LENGTH		22  // 다운로드나 버퍼링 정도를 표시하는 메시지 길이
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
//////////////////////////////// TCM ERROR ///////////////////////////////////////////////////////////////////
// Error Code로 MM_GetLastError()를 통해 에러 값을 알아온다.
/* TMI Definition*/
#define TCM_ERROR_BASE					0xF0000

#define TCM_ERROR_INVALID_PARAM			( TCM_ERROR_BASE + 1 )
#define TCM_ERROR_INIT_DECODER_ERROR	( TCM_ERROR_BASE + 2 )
#define TCM_ERROR_NOT_INITIALIZED		( TCM_ERROR_BASE + 3 )
#define TCM_ERROR_CMD_NOT_SUPPORTED		( TCM_ERROR_BASE + 4 )
#define TCM_ERROR_DATA_SEND_ERROR		( TCM_ERROR_BASE + 5 )
#define TCM_ERROR_INVALID_SEQUENCE		( TCM_ERROR_BASE + 6 )
#define TCM_ERROR_DATA_RECV_ERROR		( TCM_ERROR_BASE + 7 )
#define TCM_ERROR_INVALID_SERVER_DATA	( TCM_ERROR_BASE + 8 )
#define TCM_ERROR_FILE_WRITE_ERROR		( TCM_ERROR_BASE + 9 )
#define TCM_ERROR_FILE_READ_ERROR		( TCM_ERROR_BASE + 10 )
#define TCM_ERROR_FILE_OPEN_ERROR		( TCM_ERROR_BASE + 11 )
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
//structure CONTENTS_INFO에 할당할 값들//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Available Audio Decoder 종류		<= DWORD	AudioDecoder	
/* TMI Definition*/
#define TCM_AD_EVRC						0x0001
#define TCM_AD_QCELP					0x0002
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color Depth 종류					<= DWORD	ColorDepth		
/* TMI Definition*/
#define	TCM_CD_256						0x0000
#define	TCM_CD_4						0x0001
#define	TCM_CD_NOT_DEFINED				0x0002
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Network Type 종류				<= WORD	NetType 
/* TMI Definition*/
#define TCM_NT_2G						0x0100
#define TCM_NT_2_5G						0x0200
#define TCM_NT_3G						0x0300

#define TCM_NT_95AB						0x0001
#define TCM_NT_95C						0x0002
#define TCM_NT_NOT_DEFINED				0x0003
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_MIN_SIZE					16 // MIN Number의 최대 길이
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
//////////////////////////////// Service Type ////////////////////////////////////////////////////////////////
#define SVC_IDLE					0		// Idle Menu Play
#define SVC_LIVE_STREAM				1		// Live Streaming
#define SVC_ONDEMAND_STREAM			2		// On Demand Streaming	
#define SVC_DOWNLOAD_PLAY			3		// Download & Play
#define SVC_DOWNLOAD_STORE			4		// Download & Store
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/



/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/************************************************************************************************************/
///////////////////////////////// Timer Callback Fn //////////////////////////////////////////////////////////
/* TMI Definition*/
typedef void (*TIMER_FUNC)(BYTE nTimerID, void *user );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Timer Item ////////////////////////////////////////////////////////////////////
/* TMI Definition*/
typedef struct {
   BYTE nTimerID;						// Timer ID ( TCM Timer ID 값 중 하나를 가짐)
   TIMER_FUNC func;						// Timer Callback Function
   void* user;							// Extra Info( Not Yet Use)
} TIMER_ITEM;							// Timer에 대한 정보
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
///////////////////////////////// UI Command ID //////////////////////////////////////////////////////////////
//MM_ProcessTCM2UICommand를 통해 UI가 Controller에게 보내 주어야 하는 Command들//
/* TMI Definition*/
typedef enum {
	TCM_UC_STREAMING	= 0x01,			// 스트리밍 서비스
	TCM_UC_STOP			= 0x02,			// 서비스 종료
	TCM_UC_DOWNLOAD		= 0x03,			// 다운로드 서비스
	TCM_UC_PLAY			= 0x04,			// 재생 서비스
	TCM_UC_PAUSE		= 0x05,			// 재생 일시 정시 
	TCM_UC_RESUME		= 0x06,			// 재생 재개
	TCM_UC_TCP_FAIL		= 0x21			// TCP Fail
} TCM_UI_CMD ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Decoder Status //////////////////////////////////////////////////////////////
//MM_TCM2ServerStatus를 통해 Controller가 UI에게 알려주는 상태들//
/* TMI Definition*/
typedef enum {
	TCM_DS_DOWNLOAD_ENDED		= 0x11, // 다운로드 완료
	TCM_DS_STREAMING_ENDED		= 0x12, // 스트리밍 완료
	TCM_DS_PLAY_ENDED			= 0x13, // 재생 완료
	TCM_DS_SETUP_STARTED		= 0x14, // Setup 시작
	TCM_DS_DOWNLOAD_STARTED		= 0x15, // 다운로드 시작
	TCM_DS_STREAMING_STARTED	= 0x16, // 스트리밍 시작
	TCM_DS_DECODER_ERROR		= 0x21, // 디코더 에러 발생
	TCM_DS_CONTROLLER_ERROR		= 0x22, // 컨트롤러 에러 발생
	TCM_DS_SERVER_ERROR			= 0x23, // 서버 에러 발생
	TCM_DS_SETUP_ERROR			= 0x24, // Setup 에러 발생
	TCM_DS_CONTENTS_NOT_FOUND	= 0x25, // 컨텐츠가 없음
	TCM_DS_CONTENTS_SIZE		= 0x26  // 컨텐츠의 사이즈를 알려줌
} TCM_DECODER_STATUS;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
////////////////////////////// Content Type //////////////////////////////////////////////////////////////////
/* TMI Definition*/
typedef enum {
	TCM_FT_TEXT		= 0x00,				// 자막 
	TCM_FT_VIDEO	= 0x01,				// 비디오	
	TCM_FT_AUDIO	= 0x02,				// 오디오 
	TCM_FT_SKIN		= 0x03				// 스킨
} TCM_FILE_TYPE ; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Buffer Type ///////////////////////////////////////////////////////////////////
/* TMI Definition*/
typedef enum {
	TCM_MT_FRAME_BUFFER = 0x01,			// 프레임 버퍼 : 디코딩된 프레임의 데이더를 위한 버퍼
	TCM_MT_PACKET_BUFFER = 0x02			// 패킷 버퍼 : 스트리밍 시 UDP로 전송되어 오는 데이터를 위한 버퍼
										//             다운로드 시에는 플레이 될 모든 컨텐츠를 올리는 부분으로 사용  
} TCM_MEM_TYPE ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
////////////////////////////// 256 Color이상의 Display Map ///////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	BYTE* m_pMap;					// 한 Frame을 8 By 8 Block으로 나눈 Map으로 수정해야 할 Block만 TRUE로 되어 있다.
	WORD m_Width;					// 한 Frame의 width (pixel단위)
	WORD m_Height;					// 한 Frame의 height (pixel 단위)
} COLORMAP, *PCOLORMAP;				// Color Frame에 대한 정보
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// 256 Color이상의 Display Data //////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	BYTE m_Type;
	BYTE* m_pFirst;					// 8 bit Y Data
	BYTE* m_pSecond;				// 8 bit U Data
	BYTE* m_pThird;					// 8 bit V Data
} COLORDATA, *PCOLORDATA;			// 실제적인 Color Data로 24bit YUV 형태이다.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// Skin Display Frame ///////////////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	unsigned char x;					// 스킨 Frame의 시작 x좌표 
	unsigned char y;					// 스킨 Frame의 시작 y좌표 
	unsigned char dx;					// 스킨 Frame의 width
	unsigned char dy;					// 스킨 Frame의 height
	unsigned char *pRGB;				// 24 bit RGB Pixel 정보 (RGBRGBRGB...형태로 8bit씩 값을 가짐)		
} SKIN_FRAME, *PSKIN_FRAME;				// 한번 Display할 스킨 Frame의 정보
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// Skin Display Information /////////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	unsigned char	m_Width, m_Height;	// 전체 스킨의 width와 height
	unsigned char	m_Depth, m_padding;	// 스킨의 Depth
	SKIN_FRAME		*pFrame;			// 한번 Display할 스킨 Frame의 정보
} TCM2_SKIN, *PTCM2_SKIN;				// 스킨 정보
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
////////////////////////////// Content Information ///////////////////////////////////////////////////////////
/* TMI Definition*/
typedef struct CONTENTS
{
	DWORD	CPID;			// Contents Provider ID
	DWORD	ContentsID;		// Contents ID
	DWORD	AudioDecoder;	// Available Audio Decoder
	DWORD	ColorDepth;		// Color Depth
	WORD	SkinType;		// SkinType
	WORD	UDP_Port1;		// Listening Port1
	WORD	UDP_Port2;		// Listening Port2
	WORD	NetType;			// Network Type
	CHAR	MIN[MAX_MIN_SIZE];	// min info
} CONTENTS_INFO; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Wavelet Header ////////////////////////////////////////////////////////////////
typedef struct {
BOOL fConnectDN;			// 이어받기 여부에 대한 Flag(다운로드가 완료된 경우에만 FALSE값을 가진다.)
int cpid;					// Contents Provider ID
int ctsid;					// Contents ID
char cntName[21];			// Contents Name
int cntProtect;				// Contents 재생 가능 회수
int cntExpDate;				// Contents 재생 가능 일수 
DWORD dnDate;				// 다운로드 된 날짜(재생 가능 일수의 체크를 위해 필요)
} wavelet_header;			// Wavelet Header File에 저장되어야 하는 Header Information
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
/************************************************************************************************************/
///////////////////////////////////////// Wavelet Message Number ////////////////////////////////////////////
enum tagWAVELET_MESSAGE_E{
    WAV_MES_1 = 1,         
    WAV_MES_2,            
    WAV_MES_3,            
    WAV_MES_4,          
    WAV_MES_5,
    WAV_MES_6,
	WAV_MES_7
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////// Wavelet Applet End Status //////////////////////////////////////////
enum WAVELET_STATUS{
	INIT_STATUS = 0x00,			// 아래의 상태들이 아닌 기본 상태
	PHONE_CLOSE = 0x01,			// Phone Close Event가 들어온 상태
	PPPOPEN_STATUS = 0x02,		// PPP Open을 하는 상태(2G에만 해당)
	SOCKCONNECT_STATUS = 0x03,	// Socket Open 및 Call Connect 상태
	CALL_DROP = 0x04			// Call Drop이 된 상태
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tcm library functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern BOOL	MM_InitTCM2Controller( CONTENTS_INFO* data );
extern BOOL	MM_CloseTCM2Controller( void );

extern BOOL	MM_ProcessTCM2UICommand( TCM_UI_CMD CommandID, DWORD data1, DWORD data2 );
extern BOOL	MM_ProcessTCM2ServerCommand( void );

extern WORD	MM_GetLastError( void );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
///////////////////////// Wap Script로부터 받은 Parameter ///////////////////////////////////////////////////////////////
extern int g_wcpid;				// Content Provider ID
extern int g_wctsid;			// Content ID
extern char g_wsndIP[101];		// IP
extern int g_wsndPort;			// Port
extern int g_wMO;				// MO
extern int g_wSO;				// SO
extern int g_wsvcType;			// 서비스 종류
extern int g_wskinType;			// 스킨 종류
extern char g_wcntName[21];		// Content 이름
extern int g_wcntProtect;		// 재생 가능 회수
extern int g_wcntExpDate;		// 재생 가능 날짜
extern char g_wretURL[101];		// 리턴 URL 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
/************************************************************************************************************/
//////////////////////////////// 다운로드 Type을 알려주는 변수 ///////////////////////////////////////////////
BYTE gDownInfo = NEW_CONTENTS;	// 할당값은 ska_wav.h에 정의 되어 있는 다음 값 중에 하나를 가진다.
								///////////////////////
								///	NEW_CONTENTS	///		
								///	OLD_CONTENTS	///
								///	CONNECT_CONTENTS///
								///////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// 컨텐츠 길이/////////////////////////////////////////////////////////////////
DWORD gContents_Length;			// 다운로드 받기 시작할 때는 다운로드 받을 컨텐츠 길이
								// 다운로드 완료 후에는 다운로드 받은 컨텐츠 길이
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fDownload = TRUE;			// Download 서비스인지 Streaming 서비스인지를 구별하는 flag로 값은 다음과 같다.
								////////////////////////
								/// TRUE  : Download ///
								/// FALSE : Streaming///
								////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fPause = FALSE;			// Pause 후 재생인지 아닌지 재생인지 구별 flag로 값은 다음과 같다.
								////////////////////////////
								/// TRUE  : Pause 후 재생///
								/// FALSE : 다른 경우    ///
								////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// 컨텐츠의 현재길이 //////////////////////////////////////////////////////////
DWORD Video_length = 0;	// 비디오 
DWORD Text_length = 0;	// 자막
DWORD Skin_length = 0;	// 스킨
DWORD Audio_length = 0;	// 오디오 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// 컨텐츠의 현재 포인터 ///////////////////////////////////////////////////////
BYTE * pVdoData = NULL; // 비디오
BYTE * pTxtData = NULL; // 자막 
BYTE * pSknData = NULL; // 스킨 
BYTE * pAdoData = NULL;	// 오디오
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// 컨텐츠의 총길이 ////////////////////////////////////////////////////////////
DWORD tVideo_length = 0; // 비디오 
DWORD tText_length = 0;	 // 자막 
DWORD tSkin_length = 0;	 // 스킨
DWORD tAudio_length = 0; // 오디오 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// 컨텐츠의 가장 앞을 가리키는 포인터 /////////////////////////////////////////
BYTE * ptVdoData = NULL; // 비디오
BYTE * ptTxtData = NULL; // 자막 
BYTE * ptSknData = NULL; // 스킨 
BYTE * ptAdoData = NULL; // 오디오 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BYTE g_bStride; // 1.MM_DisplayImage의 임시 구현을 위해 함수 내부에서 사용되는 SKY_UpdatePartialScreen함수가 
				//	MM_DisplayImage의 parameter인 stride정보를 포함하지 않으므로 SKY_UpdatePartialScreen함수 
				//	내부에서 이 값을 Access하도록 하기 위해 만들 Global 변수
				// 2.MM_DisplayImage는 4Gray에서만 사용하므로 256 Color이상의 단말에서는 
				//	이 함수의 내부를 비워 두어도 됨
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
rex_timer_type  wavelet_alive_timer;	// Rex Timer로 현재 사용되지는 않음
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/



/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
/************************************************************************************************************/
////////////////////////////// Wavelet Header 정보를 가지는 변수 /////////////////////////////////////////////
wavelet_header gWaveletHeader;			// "*.inf" 파일에 저장되는 정보
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// 컨텐츠 정보를 가지는 변수 /////////////////////////////////////////////////////
CONTENTS_INFO cData;					// Controller의 초기화(MM_InitTCM2Controller(&cData))시에 필요한 정보
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Applet에서 Timer를 처리하기 위한 변수 /////////////////////////////////////////
TIMER_ITEM ti[TIMER_NUM];				
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Wavelet 컨텐츠 저장을 위한 File Name //////////////////////////////////////////
BYTE  szFileNameHeader[25];				// 헤더 File Name	: "*.inf"
BYTE  szFileNameVideo[25];				// 비디오 File Name : "*.vdo"
BYTE  szFileNameAudio[25];				// 오디오 File Name : "*.ado"
BYTE  szFileNameText[25];				// 자막 File Name	: "*.txt"
BYTE  szFileNameSkin[25];				// 스킨 File Name	: "*.skn"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////// Wavelet 컨텐츠 저장을 위한  File Handle //////////////////////////////////////
HFILE hFile_Header = NULL;				// 헤더 File Handle		
HFILE hFile_Video = NULL;				// 비디오 File Handle	
HFILE hFile_Audio = NULL;				// 오디오 File Handle	
HFILE hFile_Text = NULL;				// 자막 File Handle		
HFILE hFile_Skin = NULL;				// 스킨 File Handle		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// 재생중 Volume 조절을 위한 Global Vatriable ///////////////////////////////////
UINT8 nWavVolume;						// 현재 Speaker Volume을 통해 조절하고 있음
										// 문제점 : Volume 조절시 통화음 크기가 바뀜
										// Wavelet Volume만을 조절하는 NV 변수 필요
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fInitPlay = TRUE;					// Play가 초기거나  End된 후 play 이면 TRUE 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fLongKey = FALSE;					// Long Key Command Line Display : TRUE(반복재생) , FALSE : 재생 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fDelete = FALSE;					// 컨텐츠 저장 공간이 부족한 경우 예전의 지울 컨텐트를 선택 여부 Flag
										// TRUE(선택) , FALSE : 비선택	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BYTE WaveletStatus = INIT_STATUS;		// Wavelet Applet의 상태를 가지는 변수
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Sock_Timer = FALSE;				// PPP close 또는 Open이 Would Block되었을 경우를 대비한 Timer Setting 변수 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Idle Menu 리스트를 위한 변수들 //////////////////////////////////////////////
UINT g_unNumOfWavelet;					// Wavelet 컨텐츠 리스트의 개수 

//--------------------------- 컨텐츠 이름 리스트를 가지는 변수들 -------------------------------------------//
BYTE* g_ppScrollList[SKY_MAX_NUM_FILES] = {NULL,};		  // Idle Menu상에서 Wavelet 보관함을 선택한 경우, 
														  // 컨텐츠 리스트 이름만을 보여주는 데 사용
//----------------------------------------------------------------------------------------------------------//
BYTE* g_ppScrollList_Length[SKY_MAX_NUM_FILES] = {NULL,}; // 다운로드 & 저장 서비스에서 컨텐츠 저장공간이 
														  // 모자라는 경우, 사용자가 기존 컨텐츠의 삭제를 원할때
														  // 기존에 저장된 Wavelet 컨텐츠 리스트를 보여주는데 사용
                                                          // 리스트를 보여줄 때 컨텐츠 이름과 함께 크기도 보여줌
//----------------------------------------------------------------------------------------------------------//

HCONTROL g_hListHnd;					// Wavelet 리스트 컨트롤
UINT g_unSelect;						// 리스트 중 선택된 컨텐츠의 순번 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
HCONTROL g_hWavControl;					// Wavelet 메시지 출력을 위한 Control
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

/*===========================================================================
Function:		sReleaseController

Description:	Controller와 File을 닫고, File Handle을 초기화 한다.

Return value:	None

Side effects:	
===========================================================================*/
void sReleaseController(void)
{
	MM_CloseTCM2Controller();	// TMI API
	
	// File Close & File Handle Initialize
	if (hFile_Header)		// 헤더
	{
		SKY_CloseDataFile(hFile_Header);
		hFile_Header = NULL;
	}

	if (hFile_Video)		// 비디오
	{
		SKY_CloseDataFile(hFile_Video);
		hFile_Video = NULL;
	}
	
	if (hFile_Audio)		// 오디오
	{
		SKY_CloseDataFile(hFile_Audio);
		hFile_Audio = NULL;
	}
	
	if (hFile_Text)			// 자막
	{
		SKY_CloseDataFile(hFile_Text);
		hFile_Text = NULL;
	}
	
	if (hFile_Skin)			// 스킨
	{
		SKY_CloseDataFile(hFile_Skin);
		hFile_Skin = NULL;
	}
}

/*===========================================================================
Function:		sReleaseWavelet

Description:	Wavelet 서비스가 끝났음을 저장하는 Setting

Return value:	None

Side effects:	
===========================================================================*/
void sReleaseWavelet(void)
{
	SKY_SET_IN_WAVELET(FALSE); // Wavelet이 실행되는지 여부를 세팅
}

/*===========================================================================
Function:		sWaveletReleaseSocket

Description:	할당된 Socket을 Release

Return value:	None

Side effects:	
===========================================================================*/
void sWaveletReleaseSocket(void)
{
	int result;

	// Socket Release
	result = MM_ReleaseSocket(); // (wav_multisock.c에 정의)

	if (result != WAV_SOCK_SUCCESS)	// socket이 release되지 않았을 경우(would block)
	{
		MSG_ERROR("Wavelet Release Would Block!",0,0,0);

		// Socket Release를 한번 더 불러 줌
		result = MM_ReleaseSocket();
		if (result != WAV_SOCK_SUCCESS) // 두번째 socket release 실패
		{
			MSG_ERROR("Wavelet Release Socket Error!",0,0,0);
		}
		else // socket release 성공
		{
			MSG_ERROR("Wavelet Release Socket Success!",0,0,0);
		}
	}
	else // socket release 성공
	{
		MSG_ERROR("Wavelet Release Socket Success!",0,0,0);
	}
}

/*===========================================================================
Function:		sWaveletPPPClose

Description:	PPP 연결을 끊음

Return value:	None

Side effects:	
===========================================================================*/
void sWaveletPPPClose(void)
{
	int result;
	
	// PPP Close
	result = MM_PPPClose(); // (wav_multisock.c에 정의)

	if (result == WAV_SOCK_SUCCESS)	// PPP Close 성공
	{
		MSG_ERROR("Wavelet PPP Close Success",0,0,0);
		SKY_SET_SOCK_MODE(FALSE);
		EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA);	
	}
	else if (result == WAV_SOCK_WOULDBLOCK)	// PPP CLose Would Block 
	{
		MSG_ERROR("Wavelet PPP Close would block",0,0,0);
		// PPP Close가 되도록 60초짜리 Timer를 주고 기다림
		//////////////////////////////////////////////////////////////////////////////////////////
		// 원래 EQS_TIMER_1을 CORE에서 쓰도록 할당해 주는데, PPP Close할 때	Would Block된		//
		// 경우에 60초를 주어 PPP Close가 되도록 기다리는데 이를 사용하기도 한다.				//
		// Core에서 사용될 때와 PPP Close를 위해 사용될 때 Timer가 해야하는 작업이 다르므로		//
		// Sock_Timer라는 Flag를 두어 TRUE일때는 PPP Close에 대한 동작을 하고,					//
		// FALSE일 때는 Core에서 필요로 하는 동작을 하도록 하였다.								//
		//////////////////////////////////////////////////////////////////////////////////////////
		Sock_Timer = TRUE;	// PPP Close Timer Flag	Setting			
		EQS_StartTimer(EQS_TIMER_1, (DWORD)60*1000, FALSE); // PPP CLose 위한 60초짜리 Timer Setting	
	}
	else // PPP CLose Error
	{
		MSG_ERROR("Wavelet PPP Close ERROR",0,0,0);
	}
}

/*============================================================================= 
Function:		MM_StartTimer

Description:	Start Timer 
  
Input:			nTimerID	: Timer ID
				nMilliSec	: Timer Interval
				bRepeat		: Repeat or not
				pfn			: Timer callback Function pointer
    
Output:			주어진 ID의 Timer를 시작한다.
      
Return value:	none
        
Side effects:	none
===============================================================================*/
void MM_StartTimer( BYTE nTimerID, WORD nMilliSec, BOOL bRepeat, TIMER_FUNC pfn, void* user ) // yoursun 01/04/19
{
	BYTE t_nTimerID = nTimerID - TIMER_ID_CHANGE;


	if (t_nTimerID == WAV_GENERAL_TIMER) // Core Timer 중 TCM2_TIMER_ID4(95) <= General Timer
	{
				MSG_ERROR("MM_StartTimer:ID %d, Intv %d, Rep: %d",nTimerID, nMilliSec, bRepeat);
		SKY_SetGeneralTimer((DWORD)(nMilliSec-250), bRepeat);				// General Timer Start
//		SKY_SetGeneralTimer((DWORD)4000, bRepeat);	
//		RestartWaveletAliveTimer(500);
		(ti[t_nTimerID-WAV_GENERAL_TIMER]).nTimerID = nTimerID;				// Timer ID 할당
		(ti[t_nTimerID-WAV_GENERAL_TIMER]).func = pfn;						// Callback Function 할당
		(ti[t_nTimerID-WAV_GENERAL_TIMER]).user = user;						// 현재 사용되지 않으나 할당
	}
	else // 나머지 세 개의 Core Timer(TCM2_TIMER_ID1(92), TCM2_TIMER_ID2(93), TCM2_TIMER_ID3(94)) <= EQS_Timer들임
	{
				MSG_ERROR("MM_StartTimer:ID %d, Intv %d, Rep: %d",nTimerID, nMilliSec, bRepeat);
		EQS_StartTimer((EQS_TIMER_E)t_nTimerID, (DWORD)nMilliSec, bRepeat);	// 해당 EQUS Timer Start
		(ti[t_nTimerID]).nTimerID = nTimerID;								// Timer ID 할당
	    (ti[t_nTimerID]).func = pfn;										// Callback Function 할당
		(ti[t_nTimerID]).user = user;										// 현재 사용되지 않으나 할당
	}
}

/*============================================================================= 
Function:		MM_StopTimer

Description:	Stop Timer
    
Input:			nTimerID : Timer ID 
    
Output:			주어진 ID의 Timer를 멈춘다.
            
Return value:	none	
              
Side effects:	none
===============================================================================*/
void MM_StopTimer(BYTE nTimerID)
{
	BYTE t_nTimerID = nTimerID - TIMER_ID_CHANGE;


	if (t_nTimerID == WAV_GENERAL_TIMER) // Core Timer 중 TCM2_TIMER_ID4(95) <= General Timer
	{
		MSG_ERROR("MM_StopTimer : ID %d", nTimerID, 0, 0);
		SKY_SetGeneralTimer(FALSE, FALSE);				// General Timer Stop
//		CancelWaveletAliveTimer();
	}
	else // 나머지 세 개의 Core Timer(TCM2_TIMER_ID1(92), TCM2_TIMER_ID2(93), TCM2_TIMER_ID3(94)) <= EQS_Timer들임
	{
		MSG_ERROR("MM_StopTimer : ID %d", nTimerID, 0, 0);
		EQS_StopTimer((EQS_TIMER_E)t_nTimerID);			// 해당 EQUS Tiemr Stop
	}
}

/*============================================================================= 
Function:		SKA_WaveletStreaming

Description: 스트리밍 서비스 시에 Wap Applet으로부터 불리는 스트리밍 Applet
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
void SKY_API SKA_WaveletStreaming(SKY_EVENT_T *pEvent)
{
	int result;
	nv_dir_number_type  nv_num; /* current nam name */
	extern int SetMOinSO33(word);
			   
    switch(pEvent->EventID)
    {
        /* APP */
		case SE_APP_START : /* The very first event */
			/* PtP IrDA를 닫아주기 위함 */
			if(!fIrDAPortClosed)
			{
				ChangeIrDAMode ( 0 );
				// Phone to PC IrDA가 off 되었음을 표시
				fIrDAPortClosed = TRUE;
				g_fIrDAPortForPC = FALSE;
			}

			//Back Light On //yoursun 01/06/19//
			SKY_SET_NO_OFF_BACKLIGHT(TRUE);
			SKY_BacklightOn(AB_ALWAYS);

//			EQS_GetDC(TRUE);
			EQS_ClearScreen();

			EQS_SetWindow(SKY_DEF_WIN_X,
						SKY_DEF_WIN_Y, 
						SKY_DEF_WIN_DX, 
						SKY_DEF_WIN_DY, 
						CL_NONE_NONE_CANCEL,
						NULL, // show annunciators
						WA_SHOW_HEADER|WA_SHOW_CML);	//	no command line  (WA_SHOW_CML)
			
			SKY_SET_IN_WAVELET(TRUE);	// Wavelet이 진행됨을 Setting

			EQC_StaticText(0, 0, (EQS_TOKEN)TKN_CALL_DS_WAVE_ORIG);

			SetKindOfService(WAVELET);	// 여러 부가 서비스 중 Wavelet Service임을 Setting

			fDownload = FALSE; // streaming임을 Setting

			// MO=0x921
			if( P_REV_IS_6_OR_GREATER) // 2.5G인 경우 ( Wap으로부터 항상 이 경우에만 접속되어야 한다.)
			{	
				if (SKY_IS_WAP_TO_WAVELET())		// Wap으로부터 온 것인지 확인
				{
					SKY_SET_WAP_TO_WAVELET(FALSE);	
					SKY_SET_SOCK_MODE(TRUE);		// PPP 연결 상태임을 Setting
					
					// 2.5G의 경우 PPP가 연결되어 오므로 SE_NET_CONNECTED 이벤트를 직접 보내줌 
					EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);	
				}
			}
			else // 2G 인 경우 (Wap으로부터 Streaming Applet으로 넘어 오면 안된다.)
			{
				// Error Message 출력				
				SKY_SET_WAP_TO_WAVELET(FALSE);
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				break;
			}
			break;

		case SE_NET_CONNECTED:	// PPP 연결이 완료되었을 때 받는 이벤트
			Sock_Timer = FALSE;	// PPP Open이 되었으므로 Timer가 Core용이 되도록 FALSE값 할당

			MM_InitSocket();	// Socket Initailize (wav_multisock.c에 정의)
			
			result = MM_OpenSocket();	// Socket Open (wav_multisock.c에 정의)
			if (result != WAV_SOCK_SUCCESS) 
			{
				MSG_ERROR("Socket Open ERROR",0,0,0);
				sWaveletReleaseSocket();	// Socket이 할당되어 있으면 Release 하고 초기화

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				break;
			}
			MSG_ERROR("Socket Open Success",0,0,0);

			result 	= MM_Connect(g_wsndIP, (WORD)g_wsndPort);	// TCP Connect (wav_multisock.c에 정의)	
			if (result != WAV_SOCK_SUCCESS) // TCP Connect가 실패한 경우
			{
				MSG_ERROR("CONNECT ERROR",0,0,0);
				sWaveletReleaseSocket(); 
				
				// Error Message 출력	
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				break;
			}
			// TCP Connect가 성공한 경우 
			MSG_ERROR("CONNECT Success",0,0,0);

			if (SKY_IS_IN_WAVELET())
			{
				MSG_HIGH("CONNECT_SUCCESS",0,0,0);
				//cData값 초기화
				cData.CPID = g_wcpid;			// Contents Provider ID
				cData.ContentsID = g_wctsid;		// Contents ID 70 or 72
				cData.AudioDecoder = TCM_AD_EVRC;	// Available Audio Decoder
				cData.ColorDepth = TCM_CD_256;
				cData.SkinType = g_wskinType;		// SkinType
				cData.UDP_Port1 = UDP_PORT1; // yoursun 01/04/10
				cData.UDP_Port2 = UDP_PORT2; // yoursun 01/04/10
					//yoursun 01/06/08
				// Network type 세팅
				cData.NetType = TCM_NT_2_5G;//TCM_NT_3G
//				cData.NetType = cData.NetType << 8;
				cData.NetType = cData.NetType ;
				cData.NetType = cData.NetType | TCM_NT_95C;
				
				// Min값 할당
				nv_num.nam = SKY_GET_CURRENT_NAM();
				(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_num );
				if (nv_num.dir_number[0] == '0') // 10자리 번호의 경우
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11자리 번호의 경우 (11.....이렇게 나옴)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}
				
				
				// Controller Initialize 성공
				if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
				{
					// Streaming 시작을 Core에 명령
					if (!MM_ProcessTCM2UICommand(TCM_UC_STREAMING, 0, 0 ))	// Streaming 시작이 실패한 경우
					{
								MSG_HIGH("Controller Error",0,0,0);			
						sReleaseController();				
						sWaveletReleaseSocket();

						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
					}
				}
				else // Controller Initialize 실패
				{
							MSG_HIGH("Controller Error",0,0,0);			
					sReleaseController();				
					sWaveletReleaseSocket();

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				}
			}
			else	// Wavelet 서비스가 아닌경우 
			{
				sReleaseController();				
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_NET_DISCONNECTED: // PPP 연결이 끊겼다는 이벤트
			Sock_Timer = FALSE; // PPP close 용 Timer로 사용이 끝났음을 세팅
			EQS_StopTimer(EQS_TIMER_1);	// PPP close Would Block시 사용하는 타이머 멈춤

			SKY_SET_SOCK_MODE(FALSE); // PPP Close되었음을 세팅
			MSG_HIGH("Net Disconnected Success!",0,0,0);

			// PPP가 끊긴 상태에 따라서 다음 처리가 다르므로 각 상태에 맞게 처리방법 기술
			switch (WaveletStatus)
			{
				case PHONE_CLOSE: // Phone close 이벤트로 PPP close한 경우
					MSG_HIGH("phone close!",0,0,0);
					SKY_SET_FROM_WAP(FALSE);
					SKY_SET_WAVELET_TO_WAP(FALSE);

					EQS_END_TO(EQS_POP_TO_ROOT);
				break;
				case INIT_STATUS: // Wavelet을 끝내는 상황에서 PPP close한 경우
					MSG_HIGH("init state",0,0,0);
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				case TCM_DS_CONTENTS_NOT_FOUND:	// 네트워크 에러 발생으로 인한 PPP close한 경우
					MSG_HIGH("contents not found",0,0,0);
				case TCM_DS_CONTROLLER_ERROR:
					MSG_HIGH("controller error",0,0,0);
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				case CALL_DROP: // Call Drop으로 인해 PPP close한 경우
					MSG_HIGH("Call Drop",0,0,0);
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				case TCM_DS_SETUP_ERROR: // 코어 에러 발생으로 인한 PPP close한 경우
					MSG_HIGH("setup error",0,0,0);
				case TCM_DS_SERVER_ERROR:
					MSG_HIGH("server_error",0,0,0);									
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				default:
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
			}
			break;

		case SE_CALL_ENDED: // Call Drop 처리를 위해 Call Drop이 일어난 call End의 경우만 처리
			if ((SKY_GetCallEndStatus() == CM_CALL_END_NO_SRV)||(SKY_GetCallEndStatus() == CM_CALL_END_FADE))
			 {
				// call drop 상태임을 세팅
				WaveletStatus = CALL_DROP;	

				MM_CloseTCM2Controller();
				// socket close & ppp close
				sWaveletReleaseSocket();
				if (SKY_IS_SOCK_MODE())
				{
					sWaveletPPPClose();
				}
			 }
			break;

		case SE_APP_END:   /* The very last event */ 
			SKY_SET_NO_OFF_BACKLIGHT(FALSE);
			SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

//			EQS_ReleaseDC();
			break;

		case SE_CALL_CONNECT:
		case SE_CALL_ORIGINATED:
			break;

		case SE_WAV_STREAM:
			break;

		case SE_SOFTKEY_MENU:
			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_GOTO_NTOP), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_KEY_CLEAR ://UI_CLRALL_F:
		case SE_SOFTKEY_END:
		case SE_SOFTKEY_CANCEL:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			MM_CloseTCM2Controller();
			sWaveletReleaseSocket();

			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_GOTO_NTOP), 2/*MESSAGE_DEFAULT_TIME*/, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_KEY_0 ://HS_0_K:
		case SE_KEY_1 ://HS_1_K:
		case SE_KEY_2 ://HS_2_K:
		case SE_KEY_3 ://HS_3_K:
		case SE_KEY_4 ://HS_4_K:
		case SE_KEY_5 ://HS_5_K:
		case SE_KEY_6 ://HS_6_K:
		case SE_KEY_7 ://HS_7_K:
		case SE_KEY_8 ://HS_8_K:
		case SE_KEY_9 ://HS_9_K:
		case SE_KEY_STAR ://HS_STAR_K:
		case SE_KEY_POUND ://HS_POUND_K:
		case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
		case SE_SOFTKEY_LEFT ://HS_PF1_K:
		case SE_SOFTKEY_RIGHT ://HS_PF2_K:
		case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
        // Ignore all these events
			break;

	    case SE_KEY_PHONE_CLOSE: // Phone close가 일어난 경우
			// Phone Close 상태임을 세팅
			WaveletStatus = PHONE_CLOSE; 
			
			// 하던 작업을 멈춤
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			MM_CloseTCM2Controller();			

			// socket close + PPP close
			sWaveletReleaseSocket();
			if (SKY_IS_SOCK_MODE())
			{
				sWaveletPPPClose();
			}
			break;

		case SE_KEY_END:
		case SE_SYS_TO_IDLE : /* very important */
		case SE_SYS_OFFLINE:
			WaveletStatus = INIT_STATUS;

			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			MM_CloseTCM2Controller();			
			sWaveletReleaseSocket();

			if (SKY_IS_SOCK_MODE())
			{ 
				sWaveletPPPClose();
			}
			break;

		case SE_MUXNEGO_SUCCESS: // Mux Option Nego가 성공
			MSG_HIGH("SE_MUXNEGO_SUCCESS",0,0,0);
			if (SKY_IS_IN_WAVELET())
			{
				sReleaseWavelet();
			}
			EQS_START_SIBLING(SKA_MainWAP, FALSE); // Wap으로 돌아감
			break;

		case SE_IO_MESGBOX_END:
           if (pEvent->dwParam == WAV_MES_1)
			{
				SKY_SET_FROM_WAP(FALSE); 			
				SKY_SET_WAVELET_TO_WAP(FALSE);
				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
			    
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else if( pEvent->dwParam == WAV_MES_2) // controller Init error
			{
				SKY_SET_FROM_WAP(FALSE); 				
				SKY_SET_WAVELET_TO_WAP(TRUE);
				// MO=0x01
				if ( P_REV_IS_6_OR_GREATER ) SetMOinSO33(0);   
				else 
				{
					if (SKY_IS_IN_WAVELET())
					{
						sReleaseWavelet();
					}
					EQS_START_SIBLING(SKA_MainWAP, FALSE);
				}
			}
			else if( pEvent->dwParam == WAV_MES_3)
			{
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_GOTO_NTOP), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_4)
			{
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_KEY_UP: // Sound Up!
			nWavVolume = SKY_GET_SPEAKER_VOLUME();
				
			if (nWavVolume != SPEAKER_VOLUME_LEVEL) 
			{
				SKY_SET_SPEAKER_VOLUME(nWavVolume+1);
				SKY_ControlMusicVolume(nWavVolume+1);
			}
			MSG_HIGH("Volume Up : %d ", nWavVolume,0,0);
			break;  

		case SE_KEY_DOWN:  // Sound Down!
			nWavVolume = SKY_GET_SPEAKER_VOLUME();

			if (nWavVolume != 0) 
			{
				SKY_SET_SPEAKER_VOLUME(nWavVolume-1);
				SKY_ControlMusicVolume(nWavVolume-1);
			}
			MSG_HIGH("Volume Down : %d ", nWavVolume,0,0);
			break;  

		case SE_TM_TIMER:
			MSG_ERROR("General_Timer",0,0,0);
			(ti[EQS_GENERAL_TIMER]).func(EQS_GENERAL_TIMER+TIMER_ID_CHANGE+WAV_GENERAL_TIMER,(ti[EQS_GENERAL_TIMER]).user);
			break;

		case SE_TM_TIMER1:
			if (Sock_Timer == FALSE)
			{
				(ti[EQS_TIMER_1]).func(EQS_TIMER_1+TIMER_ID_CHANGE,(ti[EQS_TIMER_1]).user);		
			}
			else //	Sock_Timer = TRUE;	<= PPP Close Would Block에 사용
			{
				Sock_Timer = FALSE;

				if (SKY_IS_SOCK_MODE())
				{
					sWaveletPPPClose();
				}
			}
			break;

		case SE_TM_TIMER2:
			(ti[EQS_TIMER_2]).func(EQS_TIMER_2+TIMER_ID_CHANGE,(ti[EQS_TIMER_2]).user);
			break;

		case SE_TM_TIMER3:
			(ti[EQS_TIMER_3]).func(EQS_TIMER_3+TIMER_ID_CHANGE,(ti[EQS_TIMER_3]).user);
			break;

		case SE_PH_RSSI: // 화면 리프레쉬를 통한 빈 화면 출력을 막음
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		sWaveletSoftKey

Description:	Wavelet 컨텐츠의 재생 중 재생 키가 눌렸을 경우의 동작하는 local 함수 
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
LOCAL void 	sWaveletSoftKey()
{
	MSG_ERROR("Play Button ",0,0,0);
	WaveletStatus = INIT_STATUS;

	if (fInitPlay == TRUE)	// 초기 재생 또는 재생 종료 후 재생 키를 누른 경우 
	{						// (Controller의 Init이 필요한 경우)
		fInitPlay = FALSE;
		fLongKey = FALSE;		
		if (MM_InitTCM2Controller(&cData) == TRUE)		// yoursun 01/04/07
		{
			if (g_wsvcType != SVC_DOWNLOAD_STORE)		// Donwload & play
				EQS_SetWindowCML(CL_PLAY_STOP_END);
			else										// Download & store
				EQS_SetWindowCML(CL_PLAY_STOP_SAVE);

			// Core에 재생을 시작하라고 하라고 알림
			if (MM_ProcessTCM2UICommand(TCM_UC_PLAY, 1, 0)) // 성공적으로 재생이 시작된 경우
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// 재생 시작이 실패한 경우 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);			
				// Error 메시지 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
		}
		else // Controller Init이 실패한 경우
		{
			MSG_HIGH("Controller Error",0,0,0);	
			if (g_wsvcType == SVC_IDLE)	// Wavelet 보관함에 있는 컨텐츠의 경우
			{
				// 감소된 플레이 가능 회수를 Write;
				if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
				{
					SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
				}
				if (hFile_Header)
				{
					SKY_CloseDataFile(hFile_Header);
					hFile_Header = NULL;
				}
			}
			// Error 메시지 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
			EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
		}
	}
	else	// 일시정지 후 재생 키를 누른 경우 또는 재생 도중 재생 키를 누른 경우
	{ 
		if (fPause == TRUE) // 일시 정지 후 재생 재개
		{
			if (fLongKey)
			{
				if (g_wsvcType != SVC_DOWNLOAD_STORE)		// Download & play
					EQS_SetWindowCML(CL_REPLY_STOP_END);
				else										// Download & store
					EQS_SetWindowCML(CL_REPLY_STOP_SAVE);
			}
			else
			{
				if (g_wsvcType != SVC_DOWNLOAD_STORE)		// Download & play
					EQS_SetWindowCML(CL_PLAY_STOP_END);
				else										// Download & store
					EQS_SetWindowCML(CL_PLAY_STOP_SAVE);
			}

			// Core에 Resume하라고 알림
			if (MM_ProcessTCM2UICommand(TCM_UC_RESUME, 0, 0)) // 성공적으로 Resume된 경우
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// Resume이 실패한 경우 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);			
				// Error 메시지 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}

			fPause = FALSE;
		}
		else // 반복 재생 또는 재생 도중 재생 키를 누른 경우
		{
			if (fLongKey)	// 반복 재생 도중 재생 키를 누른 경우 
			{				// 반복 재생을 끝내고 새로 재생을 시작한다.
				fLongKey = FALSE;
				if (g_wsvcType != SVC_DOWNLOAD_STORE)		// Download & play
					EQS_SetWindowCML(CL_PLAY_STOP_END);
				else										// Download & store
					EQS_SetWindowCML(CL_PLAY_STOP_SAVE);

				// Core에게 반복 재생을 종료하라고 알림
				if (MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0))	// 성공적으로 반복 재생을 종료한 경우
				{
					MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
				}
				else											// 반복 재생의 종료가 실패한 경우 
				{
					MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
					MSG_HIGH("Controller Error",0,0,0);			
					// Error 메시지 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}

				//Controller를 종료함
				MM_CloseTCM2Controller();

				// Controller를 초기화함
				if (MM_InitTCM2Controller(&cData) == TRUE)	// Controller 초기화가 성공 yoursun 01/04/07
				{
					// 비디오, 오디오, 텍스트의 길이와 포인터를 초기 값으로 새로 할당함
					Video_length = tVideo_length;
					Text_length = tText_length;
					Audio_length = tAudio_length;
					pVdoData = ptVdoData;
					pTxtData = ptTxtData;
					pAdoData = ptAdoData;

					if (g_wsvcType != SVC_IDLE)	// Download 후 재생일 경우 
					{							// 스킨에 대해서도 길이와 포인터를 초기값으로 할당함
						Skin_length = tSkin_length;
						pSknData = ptSknData;
					}

					// Core에 재생을 시작하라고 하라고 알림
					if (MM_ProcessTCM2UICommand(TCM_UC_PLAY, 1, 0)) // 성공적으로 재생이 시작된 경우
					{
						MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
					}
					else											// 재생 시작이 실패한 경우 
					{
						MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
						MSG_HIGH("Controller Error",0,0,0);			
						// Error 메시지 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
				}
				else									// Controller 초기화가 실패
				{
					MSG_HIGH("Controller Error",0,0,0);			
					if (g_wsvcType == SVC_IDLE) // Wavelet 보관함에 있는 컨텐츠의 경우
					{
						// 감소된 플레이 가능 회수를 Write;
						if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
						{
							SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
						}
						if (hFile_Header)
						{
							SKY_CloseDataFile(hFile_Header);
							hFile_Header = NULL;
						}
					}
					// Error 메시지 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
				}
			}
		}
	}
}

/*============================================================================= 
Function:		sWaveletF1Long

Description:	Wavelet 컨텐츠의 재생 중 반복 재생 키가 눌렸을 경우의 동작하는 local 함수
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
LOCAL void 	sWaveleF1Long()
{
	MSG_ERROR("Re-Play Button ",0,0,0);
	WaveletStatus = INIT_STATUS;

	if (fInitPlay == TRUE)	// 초기 반복 재생 또는 재생 종료 후 반복 재생 키를 누른 경우
	{						// (Controller의 Init이 필요한 경우)
		fLongKey = TRUE;	
		fInitPlay = FALSE;	
		if (MM_InitTCM2Controller(&cData) == TRUE)		// yoursun 01/04/07
		{
				MSG_ERROR("Play Repeat Initialization SUCCESS! ",0,0,0);
			if (g_wsvcType != SVC_DOWNLOAD_STORE)	// Download & play
				EQS_SetWindowCML(CL_REPLY_STOP_END);
			else									// Donwload & Store
				EQS_SetWindowCML(CL_REPLY_STOP_SAVE);
			
			// Core에 반복 재생을 시작하라고 알림
			if (MM_ProcessTCM2UICommand(TCM_UC_PLAY, 0, 0)) // 성공적으로 반복재생 시작된 경우
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// 반복재생 시작이 실패한 경우 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);		
				// Error 메시지 출력				
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
		}
		else // Controller Init이 실패한 경우
		{
			if (g_wsvcType == SVC_IDLE)
			{
				// 감소된 플레이 가능 회수를 Write
				if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
				{
					SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
				}
				if (hFile_Header)
				{
					SKY_CloseDataFile(hFile_Header);
					hFile_Header = NULL;
				}
			}
			// Error 메시지 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
			EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
		}
	}
	else // 일시정지 후 반복 재생 키를 누른 경우 또는 재생 도중 반복 재생 키를 누른 경우 
	{
		if (fPause == TRUE) // 일시 정지 후 반복 재생 재개
		{
			if (g_wsvcType != SVC_DOWNLOAD_STORE)	// Download & play
				EQS_SetWindowCML(CL_REPLY_STOP_END);
			else									// Donwload & Store
				EQS_SetWindowCML(CL_REPLY_STOP_SAVE);

			fLongKey = TRUE;

			// Core에 Resume하라고 알림
			if (MM_ProcessTCM2UICommand(TCM_UC_RESUME, 0, 0)) // 성공적으로 Resume된 경우
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// Resume이 실패한 경우 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);			
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}

			fPause = FALSE;
		}
		else  // 반복 재생 또는 재생 도중 반복 재생 키를 누른 경우
		{
			fLongKey = TRUE;						// 반복 재생 임을 Setting

			if (g_wsvcType != SVC_DOWNLOAD_STORE)	// Download & play
				EQS_SetWindowCML(CL_REPLY_STOP_END);
			else									// Donwload & Store
				EQS_SetWindowCML(CL_REPLY_STOP_SAVE);
		} 
	}
}

/*============================================================================= 
Function:		sSKA_IdleWaveletPlay

Description:	Wavelet 보관함 밑의 컨텐츠의 재생 Applet
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
void SKY_API sSKA_IdleWaveletPlay(SKY_EVENT_T *pEvent)
{
	nv_dir_number_type  nv_num; /* current nam name */
	DWORD ResultDate = 0;
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];	// yoursun 01/10/29

    switch(pEvent->EventID)
    {
        /* APP */
		case SE_APP_START : /* The very first event */
			//Back Light On //yoursun 01/06/19//
			SKY_SET_NO_OFF_BACKLIGHT(TRUE);
			SKY_BacklightOn(AB_ALWAYS);
			////////////////////////////////////

			EQS_SetWindow(SKY_DEF_WIN_X,
						SKY_DEF_WIN_Y, 
						SKY_DEF_WIN_DX, 
						SKY_DEF_WIN_DY, 
						CL_PLAY_STOP_END, // yoursun 01/06 
						NULL, // show annunciators
						WA_SHOW_HEADER|WA_SHOW_CML);	// yoursun 01/06 	

			SKY_SET_IN_WAVELET(TRUE);	

			if (fPause && SKY_IS_TO_WAVELET()) // ska_call.c에서 온 경우(Call처리 하고 다시 온 경우)
				// 기본적을 annunciator와 Command Line을 그린 후에 Wavelet Skin을 그리기 위해 중간에 Event를 둠
				EQS_SEND_EVENT( SE_USER_1 , EQS_NA, EQS_NA);
			else  // 다른 애플릿에서 오지 않은 경우 (call)
			{					
				WaveletStatus = INIT_STATUS;					
				fPause = FALSE; 
				fLongKey = FALSE;
				fInitPlay = TRUE;

				strcpy(g_wcntName,(BYTE *)(g_ppScrollList[g_unSelect]));

				SKY_SET_TO_WAVELET(FALSE);
				SKY_SET_FROM_WAVELET(FALSE);

				cData.CPID = 0;			// Contents Provider ID
				cData.ContentsID = 0;		// Contents ID
				cData.AudioDecoder = TCM_AD_EVRC;	// Available Audio Decoder
				cData.ColorDepth = TCM_CD_256;
				cData.SkinType = g_wskinType;		// SkinType
				cData.UDP_Port1 = 0; 
				cData.UDP_Port2 = 0; 
				cData.NetType = 0;

				nv_num.nam = SKY_GET_CURRENT_NAM();
				(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_num );
				if (nv_num.dir_number[0] == '0') // 10자리 번호의 경우
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11자리 번호의 경우 (11.....이렇게 나옴)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}

						MSG_HIGH("MIN 할당",0,0,0);
					
				strcpy(szFileNameHeader, g_wcntName);
				strcat(szFileNameHeader,".inf");
				
				// header Open & Header info Read
				if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_READ) == SDR_OK)
				{
					SKY_ReadDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
					if (hFile_Header)
					{
						SKY_CloseDataFile(hFile_Header);
						hFile_Header = NULL;
					}
				}
				else
				{
					if (hFile_Header)
					{
						SKY_CloseDataFile(hFile_Header);
						hFile_Header = NULL;			
					}

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				}
					
								MSG_HIGH("Count %d",gWaveletHeader.cntProtect,0,0);	
				// 재생 가능 회수체크 
				if (gWaveletHeader.cntProtect == 0x00)	// 재생 가능 회수가 "0" => 재생 불가
				{
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_PROTECTED), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
					return;
				}
				else if (gWaveletHeader.cntProtect != 0xff)	// 재생 가능 회수가 0xff이면 재생 제한이 없으므로 그냥 둠
				{
					gWaveletHeader.cntProtect--;	// 재생 가능 회수가 0이나 0xff가 아니면 재생 가능 회수를 줄여줌
				}
				
				// 재생 가능 날짜를 확인 하기 위해 재생가능 날짜를 초단위로 환산함
				ResultDate = 86400*gWaveletHeader.cntExpDate;	

				// 재생 가능 날짜의 제한이 없는 0xff이거나 재생 가능 날짜가 안 지난 경우 
				if ((gWaveletHeader.cntExpDate == 0xfF) || 
				((SKY_GetLocalTime()-gWaveletHeader.dnDate) <= ResultDate))
				{
					// 컨텐츠 파일 이름 세팅
					strcpy(szFileNameVideo,g_wcntName);
					strcpy(szFileNameAudio,g_wcntName);
					strcpy(szFileNameText,g_wcntName);
						
					strcat(szFileNameVideo,".vdo");
					strcat(szFileNameAudio,".ado");
					strcat(szFileNameText,".cap");

					// 컨텐츠 파일을 열기
					if ((SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameVideo, &hFile_Video, SFT_READ) == SDR_OK)
					&&(SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameAudio, &hFile_Audio, SFT_READ) == SDR_OK)
					&&(SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameText, &hFile_Text, SFT_READ) == SDR_OK))
					{
						// 컨텐츠 길이 및 컨텐츠 데이터 포인터 초기화
						tAudio_length = Audio_length = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, (BYTE*)szFileNameAudio);						
						tVideo_length = Video_length = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, (BYTE*)szFileNameVideo);
						tText_length = Text_length = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, (BYTE*)szFileNameText);
						ptVdoData = pVdoData = (BYTE *)(Image$$APP_SHARE_RAM$$Base+100000);		// yoursun 01/06/13
						ptTxtData = pTxtData = pVdoData + Video_length;					
						ptAdoData = pAdoData = pTxtData + Text_length;	
						// 비디오 컨텐츠를 Share RAM에 올림
						if (Video_length != (DWORD)SKY_ReadDataBlock(hFile_Video, pVdoData, Video_length))
						{
							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
						}
						if (hFile_Video) 
						{
							SKY_CloseDataFile(hFile_Video);
							hFile_Video = NULL;			
						}
						
						// 자막 컨텐츠를 Share RAM에 올림
						if (Text_length != (DWORD)SKY_ReadDataBlock(hFile_Text, pTxtData, Text_length))
						{
							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						if (hFile_Text) 
						{
							SKY_CloseDataFile(hFile_Text);
							hFile_Text = NULL;			
						}

						// 오디오 컨텐츠를 Share RAM에 올림	
						if (Audio_length != (DWORD)SKY_ReadDataBlock(hFile_Audio, pAdoData, Audio_length))
						{
							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						if (hFile_Audio)
						{
							SKY_CloseDataFile(hFile_Audio);
							hFile_Audio = NULL;			
						}

						// Controller Initialize 성공
						if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
						{
							MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! ",0,0,0);
							// 재생을 시작하게 하도록 이벤트를 불러줌
							EQS_SEND_EVENT(SE_SOFTKEY_MENU, EQS_NA, EQS_NA);
						}
						else // Controller Initialize 실패
						{
													MSG_HIGH("Controller Error",0,0,0);			
							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
						}
					}
					else // 컨텐츠 파일 열기가 실패한 경우 
					{
												MSG_HIGH("File Open Fail",0,0,0);			
						if (hFile_Video) 
						{
							SKY_CloseDataFile(hFile_Video);
							hFile_Video = NULL;			
						}
						if (hFile_Audio)
						{
							SKY_CloseDataFile(hFile_Audio);
							hFile_Audio = NULL;			
						}
						if (hFile_Text) 
						{
							SKY_CloseDataFile(hFile_Text);
							hFile_Text = NULL;			
						}

						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
					}
				}
				else	// 재생 가능 날짜가 지난 경우 
				{
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_USEDDATE_EXPIRE), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				}
			}
			break;

		case SE_USER_1:
			SKY_SET_TO_WAVELET(FALSE);
			MSG_HIGH("IdleWavelet Pause TRUE",0,0,0);
			MM_ProcessTCM2UICommand(TCM_UC_RESUME, 0, 0);
			fPause = FALSE;
			break;

		case SE_SOFTKEY_MENU:
			// 재생 시작을 Core에 명령
			MM_ProcessTCM2UICommand(TCM_UC_PLAY, 1, 0);
			fInitPlay = FALSE;
			break;

		case SE_APP_END :   /* The very last event */ 
			SKY_SET_NO_OFF_BACKLIGHT(FALSE);
			SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
			break;

		case SE_KEY_CLEAR :
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();
			
			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_SOFTKEY_REPLY : // 반복재생 버튼 
		case SE_SOFTKEY_PLAY : // 재생 버튼 
		    sWaveletSoftKey();
			break;

		case SE_KEY_F1_LONG : // 반복 재생 버튼
			sWaveleF1Long();
			break;

		case SE_SOFTKEY_STOP : // 일시 정지 버튼
			if (fLongKey) 		EQS_SetWindowCML(CL_REPLY_NONE_END);
			else EQS_SetWindowCML(CL_PLAY_NONE_END);
			fPause = TRUE;
			MM_ProcessTCM2UICommand(TCM_UC_PAUSE, 0, 0);
			MSG_ERROR("Stop Button ",0,0,0);
		    break;

		case SE_SOFTKEY_END : // 종료  키 
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			// Error Message 출력
		    g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_KEY_PHONE_CLOSE:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			// 감소된 플레이 가능 회수를 Write;
			if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
			{
				SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
			}
			if (hFile_Header)
			{
				SKY_CloseDataFile(hFile_Header);
				hFile_Header = NULL;
			}
			
			if (SKY_IS_IN_WAVELET())
			{
				sReleaseWavelet();
			}
			
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_END:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
		    break;

		case SE_SYS_TO_IDLE : // very important 
		case SE_SYS_OFFLINE:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

	    case SE_CALL_INCOMING:/*UI_CALL_INCOMING_E:*/		   
			if (WaveletStatus != TCM_DS_PLAY_ENDED)		fPause = TRUE;
			SKY_SET_FROM_WAVELET(TRUE);
			MM_ProcessTCM2UICommand(TCM_UC_PAUSE, 0, 0);
			/* goto call */
			EQS_PASS_CURRENT_EVENT();
			break;

		case SE_IO_MESGBOX_END:
			if( pEvent->dwParam == WAV_MES_1)
			{
				// 감소된 플레이 가능 회수를 Write;
				if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
				{
					SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
				}
				if (hFile_Header)
				{
					SKY_CloseDataFile(hFile_Header);
					hFile_Header = NULL;
				}

				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
				EQS_END();
			}
			else if (pEvent->dwParam == WAV_MES_2)
			{
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_3) 		
			{
				// 감소된 플레이 가능 회수를 Write;
				if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
				{
					SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
				}
				if (hFile_Header)
				{
					SKY_CloseDataFile(hFile_Header);
					hFile_Header = NULL;
				}

				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			break;

	    case SE_KEY_0 ://HS_0_K:
		case SE_KEY_1 ://HS_1_K:
	    case SE_KEY_2 ://HS_2_K:
		case SE_KEY_3 ://HS_3_K:
	    case SE_KEY_4 ://HS_4_K:
	    case SE_KEY_5 ://HS_5_K:
	    case SE_KEY_6 ://HS_6_K:
	    case SE_KEY_7 ://HS_7_K:
	    case SE_KEY_8 ://HS_8_K:
	    case SE_KEY_9 ://HS_9_K:
	    case SE_KEY_STAR ://HS_STAR_K:
	    case SE_KEY_POUND ://HS_POUND_K:
	    case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
		case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
        // Ignore all these events
			break;

		case SE_KEY_UP: // Sound Up!
			nWavVolume = SKY_GET_SPEAKER_VOLUME();
				
			if (nWavVolume != SPEAKER_VOLUME_LEVEL) 
			{
				SKY_SET_SPEAKER_VOLUME(nWavVolume+1);
				SKY_ControlMusicVolume(nWavVolume+1);
			}
			break;  

		case SE_KEY_DOWN:  // Sound Down!
			nWavVolume = SKY_GET_SPEAKER_VOLUME();

			if (nWavVolume != 0) 
			{
				SKY_SET_SPEAKER_VOLUME(nWavVolume-1);
				SKY_ControlMusicVolume(nWavVolume-1);
			}
			break;  

		case SE_TM_TIMER:
			(ti[EQS_GENERAL_TIMER]).func(EQS_GENERAL_TIMER+TIMER_ID_CHANGE+WAV_GENERAL_TIMER,(ti[EQS_GENERAL_TIMER]).user);
			break;

		case SE_TM_TIMER1:
			(ti[EQS_TIMER_1]).func(EQS_TIMER_1+TIMER_ID_CHANGE,(ti[EQS_TIMER_1]).user);		
			break;

		case SE_TM_TIMER2:
			(ti[EQS_TIMER_2]).func(EQS_TIMER_2+TIMER_ID_CHANGE,(ti[EQS_TIMER_2]).user);
			break;

		case SE_TM_TIMER3:
			(ti[EQS_TIMER_3]).func(EQS_TIMER_3+TIMER_ID_CHANGE,(ti[EQS_TIMER_3]).user);
			break;

		case SE_PH_RSSI: // 화면 리프레쉬를 통한 빈 화면 출력을 막음
			break;

	    default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		SKA_IdleWavelet

Description:	Wavelet 보관함 밑의 컨텐츠의 리스트를 보여주고 삭제와 재생을 
				선택하게 해주는 Applet
    
Input:
    
Output:		

  Test Site : "http://211.192.81.212/wap/t.wml"
      
Return value:
        
Side effects:
===============================================================================*/
void SKY_API SKA_IdleWavelet(SKY_EVENT_T *pEvent)
{
	UINT unIdx;
	BYTE  delFileName[40];
	int i;
	extern void MM_WaveletListDisplay(BOOL SizeOrNot);

    switch(pEvent->EventID)
    {
        /* APP */
		case SE_APP_START : /* The very first event */
			/* PtP IrDA를 닫아주기 위함 */
			if(!fIrDAPortClosed)
			{
				ChangeIrDAMode ( 0 );
				// Phone to PC IrDA가 off 되었음을 표시
				fIrDAPortClosed = TRUE;
				g_fIrDAPortForPC = FALSE;
			}
			if (SKY_IS_TO_WAVELET()) // ska_call.c에서 온 경우(Call처리 하고 다시 온 경우)
			{
				MSG_HIGH("IdleWavelet",0,0,0);
				EQS_START_CHILD(sSKA_IdleWaveletPlay);
			}

			EQS_GetDC(TRUE);
		
			EQS_SetWindow(SKY_DEF_WIN_X,
						SKY_DEF_WIN_Y, 
						SKY_DEF_WIN_DX, 
						SKY_DEF_WIN_DY, 
						CL_DELETE_CONFIRM_NONE, // yoursun 01/06 
						(BYTE*)SKY_GET_TOKEN(TKN_WAVELET_FILE_TITLE), // show annunciators
						WA_SHOW_CML|WA_SHOW_HEADER);	// yoursun 01/06 	

			MM_WaveletListDisplay(FALSE); // Wavelet 보관함의 List를 보여줌
			g_wsvcType = SVC_IDLE;
			break;

		case SE_APP_END :   /* The very last event */ 
			EQS_ReleaseDC();
			for (i = 0; i <= g_unNumOfWavelet; i++)	// scroll list의 메모리를 프리
			{
				SKY_Free(g_ppScrollList[i]);
			}
			MSG_HIGH("Release DC",0,0,0);
			break;

		case SE_APP_RESUME:
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

	    case SE_KEY_PHONE_CLOSE:
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_END:
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_IO_SCROLL_LIST_SELECT:	// 스크롤 리스트가 선택된 경우 
			g_unSelect = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
			// 선택된 Wavelet컨텐츠를 재생하는 child Applet을 부름
			EQS_START_CHILD(sSKA_IdleWaveletPlay);
			break;

		case SE_IO_SCROLL_LIST_CANCEL:
			EQS_END();
			break;

		case SE_SOFTKEY_DELETE:
			MSG_HIGH (" Delet Key! ",0,0,0);
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
			break;

		case SE_IO_MESGBOX_END:
			if( pEvent->dwParam == WAV_MES_1)
			{
				EQS_END();
			}
			else if (pEvent->dwParam == WAV_MES_2)
			{
				if (g_unNumOfWavelet) 
				{
					EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);
				}
				else 
				{
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NTOP_FILE_EMPTY), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);				
				}
			}
			break;

		case SE_IO_MESGBOX_YES:	// 사용자가 컨텐츠 삭제를 선택한 경우 
			MSG_HIGH (" Delet Yes! ",0,0,0);

			unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
			strcpy(delFileName,g_ppScrollList[unIdx]);

			// 지울 컨텐츠 이름을 세팅
			strcpy(szFileNameHeader, delFileName);
			strcpy(szFileNameVideo, delFileName);
			strcpy(szFileNameAudio, delFileName);
			strcpy(szFileNameText, delFileName);
			
			strcat(szFileNameHeader, ".inf");
			strcat(szFileNameVideo, ".vdo");
			strcat(szFileNameAudio, ".ado");
			strcat(szFileNameText, ".cap");

			// 컨텐츠 파일을 지움
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameHeader);
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameVideo);
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameAudio);
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameText);

			MSG_HIGH (" Delet Complete! ",0,0,0);
			
			// 컨텐츠 파일 삭제 후 스크롤 리스트도 수정
            EQC_DeleteScrollData(EQS_GetNthIOC(0), unIdx);
			RemoveListItem(g_hListHnd, unIdx);

			for( i=unIdx; i<SKY_MAX_NUM_FILES-1; i++)
			{
				g_ppScrollList[i] = g_ppScrollList[i+1];
			}
			SKY_Free(g_ppScrollList[i]);

			g_unNumOfWavelet = g_unNumOfWavelet - 1;

			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE_DONE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_IO_MESGBOX_NO:
			MSG_HIGH (" Delet No! ",0,0,0);
			break;

		case SE_TM_TIMER:
			(ti[EQS_GENERAL_TIMER]).func(EQS_GENERAL_TIMER+TIMER_ID_CHANGE+WAV_GENERAL_TIMER,(ti[EQS_GENERAL_TIMER]).user);
			break;

		case SE_TM_TIMER1:
			(ti[EQS_TIMER_1]).func(EQS_TIMER_1+TIMER_ID_CHANGE,(ti[EQS_TIMER_1]).user);		
			break;

		case SE_TM_TIMER2:
			(ti[EQS_TIMER_2]).func(EQS_TIMER_2+TIMER_ID_CHANGE,(ti[EQS_TIMER_2]).user);
			break;

		case SE_TM_TIMER3:
			(ti[EQS_TIMER_3]).func(EQS_TIMER_3+TIMER_ID_CHANGE,(ti[EQS_TIMER_3]).user);
			break;

		case SE_PH_RSSI: // 화면 리프레쉬를 통한 빈 화면 출력을 막음
			break;

		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		sSKA_DeleteOldContents

Description: 
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
void SKY_API sSKA_DeleteOldContents(SKY_EVENT_T *pEvent)
{
	extern void MM_WaveletListDisplay(BOOL SizeOrNot);	
	BYTE  delFileName[40];
	BYTE  delHeaderName[40];
	BYTE  delVideoName[40];
	BYTE  delAudioName[40];
	BYTE  delTextName[40];
	int i;

    switch(pEvent->EventID)
    {
        /* APP */
		case SE_APP_START : /* The very first event */
			EQS_ClearScreen();
		
			EQS_SetWindow(SKY_DEF_WIN_X,
						SKY_DEF_WIN_Y, 
						SKY_DEF_WIN_DX, 
						SKY_DEF_WIN_DY, 
						CL_NONE_CONFIRM_CANCEL, // yoursun 01/06 
						(BYTE*)SKY_GET_TOKEN(TKN_WAVELET_FILE_TITLE), // show annunciators
						WA_SHOW_CML|WA_SHOW_HEADER);	// yoursun 01/06 	
			
			MM_WaveletListDisplay(TRUE);	//
			break;

		case SE_APP_END :   /* The very last event */ 
			for (i = 0; i <= g_unNumOfWavelet; i++)
			{
				SKY_Free(g_ppScrollList[i]);
				SKY_Free(g_ppScrollList_Length[i]);
			}

			break;

	    case SE_KEY_0 ://HS_0_K:
		case SE_KEY_1 ://HS_1_K:
	    case SE_KEY_2 ://HS_2_K:
		case SE_KEY_3 ://HS_3_K:
	    case SE_KEY_4 ://HS_4_K:
	    case SE_KEY_5 ://HS_5_K:
	    case SE_KEY_6 ://HS_6_K:
	    case SE_KEY_7 ://HS_7_K:
	    case SE_KEY_8 ://HS_8_K:
	    case SE_KEY_9 ://HS_9_K:
	    case SE_KEY_STAR ://HS_STAR_K:
	    case SE_KEY_POUND ://HS_POUND_K:
	    case SE_KEY_UP ://HS_UP_K:
	    case SE_KEY_DOWN ://HS_DOWN_K:
	    case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
		case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
        // Ignore all these events
			break;

	    case SE_KEY_PHONE_CLOSE:
			sReleaseController();
			SKY_DeleteDataFiles(SDF_TEMPWAV);

			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_END:
		case SE_SYS_TO_IDLE : // very important 
		case SE_SYS_OFFLINE:
			sReleaseController();
			SKY_DeleteDataFiles(SDF_TEMPWAV);

			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_CALL_INCOMING:/*UI_CALL_INCOMING_E:*/		   
			{
				sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);
				/* goto call */
				EQS_PASS_CURRENT_EVENT();
			}
			break;

		case SE_IO_SCROLL_LIST_SELECT:
			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_SOFTKEY_CANCEL:
		case SE_IO_SCROLL_LIST_CANCEL:
			sReleaseController();
			SKY_DeleteDataFiles(SDF_TEMPWAV);

		    // Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_IO_MESGBOX_END:
			if( pEvent->dwParam == WAV_MES_1)
			{
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			if( pEvent->dwParam == WAV_MES_4)
			{
				fDelete = TRUE;
				EQS_END();
			}
			break;

		case SE_IO_MESGBOX_YES:
			if( pEvent->dwParam == WAV_MES_3)
			{
				MSG_HIGH (" Delet Yes! ",0,0,0);
				
				g_unSelect = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
				strcpy(delFileName,g_ppScrollList[g_unSelect]);

				strcpy(delHeaderName, delFileName);
				strcpy(delVideoName, delFileName);
				strcpy(delAudioName, delFileName);
				strcpy(delTextName, delFileName);
					
				strcat(delHeaderName, ".inf");
				strcat(delVideoName, ".vdo");
				strcat(delAudioName, ".ado");
				strcat(delTextName, ".cap");

					MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);
				SKY_DeleteDataFile(SDF_WAVELET, delHeaderName);
				SKY_DeleteDataFile(SDF_WAVELET, delVideoName);
				SKY_DeleteDataFile(SDF_WAVELET, delAudioName);
				SKY_DeleteDataFile(SDF_WAVELET, delTextName);

				MSG_HIGH (" Delet Complete! ",0,0,0);
				
				EQC_DeleteScrollData(EQS_GetNthIOC(0), g_unSelect);
				RemoveListItem(g_hListHnd, g_unSelect);

				for( i=g_unSelect; i<SKY_MAX_NUM_FILES-1; i++)
				{
					g_ppScrollList_Length[i] = g_ppScrollList_Length[i+1];
					g_ppScrollList[i] = g_ppScrollList[i+1];
				}
				SKY_Free(g_ppScrollList[i]);
				SKY_Free(g_ppScrollList_Length[i]);

				g_unNumOfWavelet = g_unNumOfWavelet - 1;

				if (SKY_GetFreeSpaceInDataFile() >= gContents_Length) 
				{
					MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE_DONE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
				else // 컨텐트 삭제시에도 저장 공간이 모자라는 경우 
				{
					MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);
					
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_OLD_CONTENTS_REDELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_5);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
			}
			else if( pEvent->dwParam == WAV_MES_2)
			{
				SKY_SET_FROM_WAP(FALSE);      			
				SKY_SET_WAVELET_TO_WAP(TRUE);

				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}				

				EQS_START_SIBLING(SKA_MainWAP, FALSE);
			}
			else if( pEvent->dwParam == WAV_MES_5)
			{
				EQS_SetIOCActive(EQS_GetNthIOC(0));
			}
 			break;

		case SE_IO_MESGBOX_NO:
			if( pEvent->dwParam == WAV_MES_3)
			{
				sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_2)
			{
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);				
			}
			else if( pEvent->dwParam == WAV_MES_5)
			{
				sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_PH_RSSI: // 화면 리프레쉬를 통한 빈 화면 출력을 막음
			break;

		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

//#define RENAME // rename을 사용하면 fs_dir.c의 4052에서 죽음
LOCAL void 	sWaveletContentsSave()	// 실제로 컨텐츠를 Wavelet 보관 디렉토리에 저장하는 함수
{
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];
	BYTE * pFileCnt;
	DWORD length = 0;
#ifdef RENAME
    BYTE szOldname[SKY_PATH_NAME_MAX_LENGTH];
    BYTE szNewname[SKY_PATH_NAME_MAX_LENGTH];
#endif
	
	pFileCnt = (BYTE*)(Image$$APP_SHARE_RAM$$Base+100000);
	gWaveletHeader.dnDate = SKY_GetLocalTime();

	MSG_HIGH(" Wap ExpData %d, %d",g_wcntExpDate,gWaveletHeader.cntExpDate,0);

#ifdef RENAME
	if (SKY_CreateDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header) == SDR_OK)
	{
		SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
	}
	else 
	{
		if (hFile_Header)
		{
			SKY_CloseDataFile(hFile_Header);
			hFile_Header = NULL;
		}

		// Error Message 출력
		g_hWavControl = EQC_MessageBox((BYTE*) (SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR)), 2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}
		MSG_HIGH("1",0,0,0);
	sprintf((char *)szOldname, "%s%s%s",SKY_TEMPWAV_DIR, SKY_DIR_CHAR, szFileNameVideo);
	sprintf((char *)szNewname, "%s%s%s",SKY_WAVELET_DIR, SKY_DIR_CHAR, szFileNameVideo);
	SKY_RenameFile((CONST BYTE*)szOldname,(CONST BYTE*)szNewname);
		MSG_HIGH("2",0,0,0);	
	sprintf((char *)szOldname, "%s%s%s",SKY_TEMPWAV_DIR, SKY_DIR_CHAR, szFileNameAudio);
	sprintf((char *)szNewname, "%s%s%s",SKY_WAVELET_DIR, SKY_DIR_CHAR, szFileNameAudio);
	SKY_RenameFile((CONST BYTE*)szOldname,(CONST BYTE*)szNewname);
		MSG_HIGH("3",0,0,0);	
	sprintf((char *)szOldname, "%s%s%s",SKY_TEMPWAV_DIR, SKY_DIR_CHAR, szFileNameText);
	sprintf((char *)szNewname, "%s%s%s",SKY_WAVELET_DIR, SKY_DIR_CHAR, szFileNameText);
	SKY_RenameFile((CONST BYTE*)szOldname,(CONST BYTE*)szNewname);
		MSG_HIGH("4",0,0,0);
	sReleaseController();
		MSG_HIGH("5",0,0,0);
	SKY_DeleteDataFile(SDF_TEMPWAV,szFileNameHeader);
		MSG_HIGH("6",0,0,0);
#else 
	// Wavelet 헤터 정보를 저장
	if (SKY_CreateDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header) == SDR_OK)
	{
		SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
	}
	else // 헤더 파일 생성이 실패한 경우
	{
		if (hFile_Header)
		{
			SKY_CloseDataFile(hFile_Header);
			hFile_Header = NULL;
		}

		// Error Message 출력
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}

	// 비디오 파일 생성 및 저장
	length = tVideo_length;
	if (hFile_Video)
	{
		SKY_CloseDataFile(hFile_Video);
		hFile_Video = NULL;
	}
	
	if (SKY_CreateDataFile(SDF_WAVELET, (BYTE*)szFileNameVideo, &hFile_Video) == SDR_OK)
	{
		SKY_WriteDataBlock(hFile_Video, ptVdoData, length);
	}
	else 
	{
		if (hFile_Video)
		{
			SKY_CloseDataFile(hFile_Video);		
			hFile_Video = NULL;
		}

		// Error Message 출력
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}
	
	// 오디오 파일 생성 및 저장
	length = tAudio_length;
	if (hFile_Audio)
	{
		SKY_CloseDataFile(hFile_Audio);
		hFile_Audio = NULL;
	}

	if (SKY_CreateDataFile(SDF_WAVELET, (BYTE*)szFileNameAudio, &hFile_Audio) == SDR_OK)
	{
		SKY_WriteDataBlock(hFile_Audio, ptAdoData, length);
	}
	else 
	{
		if (hFile_Audio) 
		{
			SKY_CloseDataFile(hFile_Audio);			
			hFile_Audio = NULL;
		}

		// Error Message 출력
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}

	// 자막 파일 생성 및 저장
	length = tText_length;
	if (hFile_Text)
	{
		SKY_CloseDataFile(hFile_Text);
		hFile_Text = NULL;
	}

	if (SKY_CreateDataFile(SDF_WAVELET, (BYTE*)szFileNameText, &hFile_Text) == SDR_OK)
	{
		SKY_WriteDataBlock(hFile_Text, ptTxtData, length);
	}
	else 
	{
		if (hFile_Text) 
		{
			SKY_CloseDataFile(hFile_Text);
			hFile_Text = NULL;
		}

		// Error Message 출력
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}

	sReleaseController();
	SKY_DeleteDataFiles(SDF_TEMPWAV);
#endif
	fDelete = FALSE;

	// Error Message 출력
	g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVE_DONE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
	EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
	EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
}

/*============================================================================= 
Function:		sSKA_MainOnWavelet

Description: 
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
void SKY_API sSKA_MainOnWavelet(SKY_EVENT_T *pEvent)
{
	nv_dir_number_type  nv_num; /* current nam name */
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];	// yoursun 01/10/29

    switch(pEvent->EventID)
    {
        /* APP */
		case SE_APP_START : /* The very first event */
			//Back Light On //yoursun 01/06/19//
			SKY_SET_NO_OFF_BACKLIGHT(TRUE);
			SKY_BacklightOn(AB_ALWAYS);

			if (g_wsvcType == SVC_DOWNLOAD_STORE)	// Download & Store
			{
				EQS_SetWindow(SKY_DEF_WIN_X,
							SKY_DEF_WIN_Y, 
							SKY_DEF_WIN_DX, 
							SKY_DEF_WIN_DY, 
							CL_PLAY_STOP_SAVE, // yoursun 01/06 
							NULL, // show annunciators
							WA_SHOW_HEADER|WA_SHOW_CML);	// yoursun 01/06 	
			}
			else  // g_wsvcType = SVC_DOWNLOAD_PLAY // Download & play
			{
				EQS_SetWindow(SKY_DEF_WIN_X,
							SKY_DEF_WIN_Y, 
							SKY_DEF_WIN_DX, 
							SKY_DEF_WIN_DY, 
							CL_PLAY_STOP_END, // yoursun 01/06 
							NULL, // show annunciators
							WA_SHOW_HEADER|WA_SHOW_CML);	// yoursun 01/06 	
			}

			SKY_SET_IN_WAVELET(TRUE);					
			
			if (fPause && SKY_IS_TO_WAVELET()) // ska_call.c에서 온 경우(Call처리 하고 다시 온 경우)
				// 기본적을 annunciator와 Command Line을 그린 후에 Wavelet Skin을 그리기 위해 중간에 Event를 둠
				EQS_SEND_EVENT( SE_USER_1 , EQS_NA, EQS_NA); 
			else  // 다른 애플릿(call)에서 오지 않은 경우 	
			{					
				WaveletStatus = INIT_STATUS;
				// Flag Setting
				fPause = FALSE; 
				fLongKey = FALSE;
				fInitPlay = TRUE;	
				
				SKY_SET_TO_WAVELET(FALSE);
				SKY_SET_FROM_WAVELET(FALSE);
				
				// cData값을 세팅
				cData.CPID = 0;			// Contents Provider ID
				cData.ContentsID = 0;		// Contents ID
				cData.AudioDecoder = TCM_AD_EVRC;	// Available Audio Decoder
				cData.ColorDepth = TCM_CD_256;
				cData.SkinType = g_wskinType;		// SkinType
				cData.UDP_Port1 = 0; 
				cData.UDP_Port2 = 0; 
				cData.NetType = 0;
				// Min값 세팅
				nv_num.nam = SKY_GET_CURRENT_NAM();
				(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_num );
				if (nv_num.dir_number[0] == '0') // 10자리 번호의 경우
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11자리 번호의 경우 (11.....이렇게 나옴)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}
				
				// 컨텐츠 파일들의 이름 세팅
				strcpy(szFileNameHeader, g_wcntName);						
				strcpy(szFileNameVideo, g_wcntName);
				strcpy(szFileNameAudio, g_wcntName);
				strcpy(szFileNameText, g_wcntName);
				strcpy(szFileNameSkin, g_wcntName); 
					
				strcat(szFileNameHeader, ".inf");
				strcat(szFileNameVideo, ".vdo");
				strcat(szFileNameAudio, ".ado");
				strcat(szFileNameText, ".cap");
				strcat(szFileNameSkin, ".skn");

				// 컨텐츠 파일을 열기
				if ((SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameVideo, &hFile_Video, SFT_READ) == SDR_OK)
				&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameAudio, &hFile_Audio, SFT_READ) == SDR_OK)
				&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameText, &hFile_Text, SFT_READ) == SDR_OK)
				&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameSkin, &hFile_Skin, SFT_READ) == SDR_OK))
				{
					// 컨텐츠 길이 및 컨텐츠 데이터 포인터 초기화 
					tVideo_length = Video_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameVideo);
					tAudio_length = Audio_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameAudio);
					tText_length = Text_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameText);
					tSkin_length = Skin_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameSkin);

					ptVdoData = pVdoData = (BYTE *)(Image$$APP_SHARE_RAM$$Base+100000);		// yoursun 01/06/13
					ptSknData = pSknData = pVdoData + Video_length;
					ptTxtData = pTxtData = pSknData + Skin_length;
					ptAdoData = pAdoData = pTxtData + Text_length;
					
					// 비디오 컨텐츠를 Share RAM에 올림 
					if (Video_length != (DWORD)SKY_ReadDataBlock(hFile_Video, pVdoData, Video_length))
					{
						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
					}
					if (hFile_Video) 
					{
						SKY_CloseDataFile(hFile_Video);
						hFile_Video = NULL;
					}

					// 자막 컨텐츠를 Share RAM에 올림 
					if (Text_length != (DWORD)SKY_ReadDataBlock(hFile_Text, pTxtData, Text_length))
					{
						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
					if (hFile_Text) 
					{
						SKY_CloseDataFile(hFile_Text);						
						hFile_Text = NULL;
					}
					// 스킨 컨텐츠를 Share RAM에 올림 
					if (Skin_length != (DWORD)SKY_ReadDataBlock(hFile_Skin, pSknData, Skin_length))
					{
						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
					if (hFile_Skin) 
					{
						SKY_CloseDataFile(hFile_Skin);						
						hFile_Skin = NULL;
					}

					// 오디오 컨텐츠를 Share RAM에 올림 
					if (Audio_length != (DWORD)SKY_ReadDataBlock(hFile_Audio, pAdoData, Audio_length))
					{
						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
					if (hFile_Audio) 
					{
						SKY_CloseDataFile(hFile_Audio);
						hFile_Audio = NULL;
					}

					// Controller Initialize 성공
					if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
					{
						MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! ",0,0,0);
						// 재생을 시작하게 하도록 이벤트를 불러줌
						EQS_SEND_EVENT(SE_SOFTKEY_MENU, EQS_NA, EQS_NA);
					}
					else // Controller Initialize 실패
					{
									MSG_HIGH("Controller Error",0,0,0);			

						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
				}
				else //컨텐츠 파일 열기가 실패한 경우
				{
										MSG_HIGH("File Open Fail",0,0,0);			
					// 파일 닫고 핸들러 초기화
					if (hFile_Video) 
					{
						SKY_CloseDataFile(hFile_Video);
						hFile_Video = NULL;
					}
					if (hFile_Audio) 
					{
						SKY_CloseDataFile(hFile_Audio);
						hFile_Audio = NULL;
					}
					if (hFile_Text) 
					{
						SKY_CloseDataFile(hFile_Text);						
						hFile_Text = NULL;
					}
					if (hFile_Skin) 
					{
						SKY_CloseDataFile(hFile_Skin);						
						hFile_Skin = NULL;
					}

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
			}
			break;

		case SE_SOFTKEY_MENU:
			// 재생 시작을 Core에 명령
			MM_ProcessTCM2UICommand(TCM_UC_PLAY, 1, 0);
			fInitPlay = FALSE;
			break;

		case SE_USER_1:
			SKY_SET_TO_WAVELET(FALSE);

			MSG_HIGH("IdleWavelet Pause TRUE",0,0,0);
			MM_ProcessTCM2UICommand(TCM_UC_RESUME, 0, 0);
			fPause = FALSE;
			break;

		case SE_APP_END :   /* The very last event */ 
			// Backlignt 원상 복귀
			SKY_SET_NO_OFF_BACKLIGHT(FALSE);
			SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
			break;

	    case SE_KEY_0 ://HS_0_K:
		case SE_KEY_1 ://HS_1_K:
	    case SE_KEY_2 ://HS_2_K:
		case SE_KEY_3 ://HS_3_K:
	    case SE_KEY_4 ://HS_4_K:
	    case SE_KEY_5 ://HS_5_K:
	    case SE_KEY_6 ://HS_6_K:
	    case SE_KEY_7 ://HS_7_K:
	    case SE_KEY_8 ://HS_8_K:
	    case SE_KEY_9 ://HS_9_K:
	    case SE_KEY_STAR ://HS_STAR_K:
	    case SE_KEY_POUND ://HS_POUND_K:
	    case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
		case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
        // Ignore all these events
			break;

		case SE_KEY_CLEAR :// HS_CLR_K:
			if (g_wsvcType == SVC_DOWNLOAD_PLAY) // Download & Play
			{
 
				MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
				sReleaseController();
				
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
		    else // SVC_DOWNLOAD_STORE // Download & Store
			{
				MM_ProcessTCM2UICommand(TCM_UC_PAUSE, 0, 0);
				MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
				MM_CloseTCM2Controller();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVE_OR_NOT), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_SOFTKEY_REPLY : // 반복재생 버튼 
		case SE_SOFTKEY_PLAY : // 재생 버튼 
			sWaveletSoftKey();
		    break;

		case SE_KEY_F1_LONG : // 반복 재생 버튼
			sWaveleF1Long();
			break;

		case SE_SOFTKEY_STOP : // 일시 정지 버튼
			if (g_wsvcType == SVC_DOWNLOAD_PLAY) // Download & Play
			{
				if (fLongKey) 		EQS_SetWindowCML(CL_REPLY_NONE_END);
				else EQS_SetWindowCML(CL_PLAY_NONE_END); 
			}
			else
			{
				if (fLongKey) 		EQS_SetWindowCML(CL_REPLY_NONE_SAVE);
				else EQS_SetWindowCML(CL_PLAY_NONE_SAVE);
			}

			fPause = TRUE;
			MM_ProcessTCM2UICommand(TCM_UC_PAUSE, 0, 0);
			MSG_ERROR("Stop Button ",0,0,0);
		    break;

		case SE_SOFTKEY_SAVE : // 저장  키 
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			MM_CloseTCM2Controller();

				MSG_ERROR("Save Button ",0,0,0);

			// Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVE_OR_NOT), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
		    break;

		case SE_SOFTKEY_END : // 종료  키 
				MSG_ERROR("End Button ",0,0,0);
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			SKY_DeleteDataFiles(SDF_TEMPWAV);

		    // Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
		    break;

		case SE_KEY_PHONE_CLOSE:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();
			SKY_DeleteDataFiles(SDF_TEMPWAV);
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_END:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);

			if (g_wsvcType == SVC_DOWNLOAD_STORE)
			{
				MM_CloseTCM2Controller();				

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVE_OR_NOT), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else 
			{
				sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT), 2, MBT_NONE, MBT_HELP);
		        EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
			    EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_SYS_TO_IDLE : /* very important */
		case SE_SYS_OFFLINE:
			sReleaseController();
			SKY_DeleteDataFiles(SDF_TEMPWAV);

		    // Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;


	    case SE_CALL_INCOMING:	//전화 온 경우
			{
				// Wavelet 재생을 멀추로 Call 처리 부분으로 감
				if (WaveletStatus != TCM_DS_PLAY_ENDED)		fPause = TRUE;
				SKY_SET_FROM_WAVELET(TRUE);
				MM_ProcessTCM2UICommand(TCM_UC_PAUSE, 0, 0);
				/* goto call */
				EQS_PASS_CURRENT_EVENT();
			}
			break;

		case SE_APP_RESUME:
			if (fDelete == TRUE)
			{
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVING),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_6);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_IO_MESGBOX_END:
            if (pEvent->dwParam == WAV_MES_1)
			{
				SKY_SET_FROM_WAP(FALSE);
				SKY_SET_WAVELET_TO_WAP(FALSE);

				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else if (pEvent->dwParam == WAV_MES_3)
			{
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
                EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			} 
			else if (pEvent->dwParam == WAV_MES_6)
			{
				sWaveletContentsSave();	// 저장을 실행
			}
			break;

		case SE_IO_MESGBOX_YES:
			if( pEvent->dwParam == WAV_MES_4)	// 사용자가 컨텐츠 저장을 선택한 경우
			{
			    MSG_HIGH (" Store Yes! ",0,0,0);

				if (SKY_IsDataFile(SDF_WAVELET,(BYTE*)szFileNameHeader))	// 새로 받기의 경우 
				{ //저장 전에 같은 이름의 컨텐츠를 지움
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader);
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameVideo);
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameAudio);
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameText);

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVING),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_6);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
				else // 새로받기가 아닌 경우
				{  
					// 저장할 Space가 없을 경우
					if ((SKY_GetFreeSpaceInDataFile() < gContents_Length)||((SKY_GetFileNumberInDataFile(SDF_WAVELET)+4) >= SKY_MAX_NUM_FILES )) 
					{
						MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);
						if((UINT)SKY_GetNumFile(SKY_WAVELET_DIR) == 0)
						{
							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE_OTHER_CONTENTS), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
						}
						else
						{
    						// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_OLD_CONTENTS_DELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_5);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
					}
					else // 저장할 Space가 있을 경우
					{
						MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);						
						
						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVING),  2, MBT_NONE, MBT_HELP);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_6);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
				}
			}
            else if (pEvent->dwParam == WAV_MES_2)
            {
				SKY_SET_FROM_WAP(FALSE);
				SKY_SET_WAVELET_TO_WAP(TRUE);

				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
				EQS_START_SIBLING(SKA_MainWAP, FALSE);
            }
			else if (pEvent->dwParam == WAV_MES_5)
			{
				EQS_START_CHILD(sSKA_DeleteOldContents);
			}
			break;

		case SE_IO_MESGBOX_NO:
	        if( pEvent->dwParam == WAV_MES_4)
			{
			    MSG_HIGH (" Store No! ",0,0,0);
				
				sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
                EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
            else if (pEvent->dwParam == WAV_MES_2)
            {
				SKY_SET_FROM_WAP(FALSE);
				SKY_SET_WAVELET_TO_WAP(FALSE);

				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}

				EQS_END_TO(EQS_POP_TO_ROOT);
            }
			else if (pEvent->dwParam == WAV_MES_5)
			{
			    MSG_HIGH (" Old File : Not Delete ! ",0,0,0);

			    sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
                EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_CALL_ENDED:
			MSG_HIGH("SE_CALL_ENDED Break",0,0,0);
			break;

		case SE_KEY_UP: // Sound Up!
			nWavVolume = SKY_GET_SPEAKER_VOLUME();
				
			if (nWavVolume != SPEAKER_VOLUME_LEVEL) 
			{
				SKY_SET_SPEAKER_VOLUME(nWavVolume+1);
				SKY_ControlMusicVolume(nWavVolume+1);
			}
			MSG_HIGH("Volume Up : %d ", nWavVolume,0,0);
			break;  

		case SE_KEY_DOWN:  // Sound Down!
			nWavVolume = SKY_GET_SPEAKER_VOLUME();

			if (nWavVolume != 0) 
			{
				SKY_SET_SPEAKER_VOLUME(nWavVolume-1);
				SKY_ControlMusicVolume(nWavVolume-1);
			}
			MSG_HIGH("Volume Down : %d ", nWavVolume,0,0);
			break;  

		case SE_TM_TIMER:
			(ti[EQS_GENERAL_TIMER]).func(EQS_GENERAL_TIMER+TIMER_ID_CHANGE+WAV_GENERAL_TIMER,(ti[EQS_GENERAL_TIMER]).user);
			break;

		case SE_TM_TIMER1:
			(ti[EQS_TIMER_1]).func(EQS_TIMER_1+TIMER_ID_CHANGE,(ti[EQS_TIMER_1]).user);		
			break;

		case SE_TM_TIMER2:
			(ti[EQS_TIMER_2]).func(EQS_TIMER_2+TIMER_ID_CHANGE,(ti[EQS_TIMER_2]).user);
			break;

		case SE_TM_TIMER3:
			(ti[EQS_TIMER_3]).func(EQS_TIMER_3+TIMER_ID_CHANGE,(ti[EQS_TIMER_3]).user);
			break;

		case SE_PH_RSSI: // 화면 리프레쉬를 통한 빈 화면 출력을 막음
			break;

	    default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		SKA_MainWavelet

Description: 
    
Input:
    
Output:

Return value:
        
Side effects:
===============================================================================*/
void SKY_API SKA_MainWavelet(SKY_EVENT_T *pEvent)
{
	int result;
	nv_dir_number_type  nv_num; /* current nam name */
	extern int SetMOinSO33(word);
	int index;

    switch(pEvent->EventID)
    {
        /* APP */
		case SE_APP_START : /* The very first event */
			/* PtP IrDA를 닫아주기 위함 */
			if(!fIrDAPortClosed)
			{
				ChangeIrDAMode ( 0 );
				// Phone to PC IrDA가 off 되었음을 표시
				fIrDAPortClosed = TRUE;
				g_fIrDAPortForPC = FALSE;
			}
			//Back Light On //yoursun 01/06/19//
			SKY_SET_NO_OFF_BACKLIGHT(TRUE);
			SKY_BacklightOn(AB_ALWAYS);

			EQS_ClearScreen();

			EQS_SetWindow(SKY_DEF_WIN_X,
						SKY_DEF_WIN_Y, 
						SKY_DEF_WIN_DX, 
						SKY_DEF_WIN_DY, 
						CL_NONE_NONE_CANCEL,
						NULL, // show annunciators
						WA_SHOW_HEADER|WA_SHOW_CML);	//	no command line  (WA_SHOW_CML)
					
			SKY_SET_IN_WAVELET(TRUE);
			
			SKY_ReinitMemory();		// Wap과 UI쪽에 할당된 Stack이 다른데, Wap에서 올 경우 Wap Stack을 사용하게 되므로
									// 할당할 영역이 부족하게 되어 폰이 죽는 현상이 발생하므로 UI Stack을 초기화 한다.

			EQC_StaticText(0, 0, (EQS_TOKEN)TKN_CALL_DS_WAVE_ORIG);

			SetKindOfService(WAVELET);	// Socket관련 Wavelet 서비스임을 세팅

			fDownload = TRUE;			//  다운로드 서비스임을 세팅

			// MO=0x921
			if( P_REV_IS_6_OR_GREATER) // 2.5G (PPP가 연결된 상태에서 Wap으로부터 넘어옴)
			{	
				if (SKY_IS_WAP_TO_WAVELET())	
				{	// 현재 Wap에서 불리는 경우만 있으므로 Wap으로부터 불리었는지 확인.
					SKY_SET_WAP_TO_WAVELET(FALSE);
					SKY_SET_SOCK_MODE(TRUE);	// PPP Open 상태임을 세팅
					// Controller Init에 필요한 cData의 network type을 세팅
					cData.NetType = TCM_NT_2_5G;
//					cData.NetType = cData.NetType << 8;
					cData.NetType = cData.NetType ;
					cData.NetType = cData.NetType | TCM_NT_95C;
					MSG_ERROR("PPP 2.5G",0,0,0);
					// Net Connect 메시지를 보냄
					EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);	
				}
				else 
				{	// Wap으로 부터 불리지 않은 경우(Error 처리)
					MSG_ERROR("Not Wap to Wavelet",0,0,0);
					
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				}
			}
			else // 2G (PPP가 끊긴 상태에서 Wap으로부터 넘어오므로 PPP 연결부터 해주어야 함)
			{
				MSG_ERROR("2G 진입",0,0,0);

				if (SKY_IS_WAP_TO_WAVELET())
				{	// 현재 Wap에서 불리는 경우만 있으므로 Wap으로부터 불리었는지 확인.
					SKY_SET_WAP_TO_WAVELET(FALSE);
					SKY_SET_SOCK_MODE(FALSE);
					
					// PPP Open 하고 있는 상태임을 세팅
					WaveletStatus = PPPOPEN_STATUS;
					
					// PPP Open 
					result = MM_PPPOpen(); // (wav_multisock.c에 정의)
					if(result == WAV_SOCK_SUCCESS)	// PPP Opne Success
					{
						MSG_ERROR("PPP Open Success",0,0,0);
						SKY_SET_SOCK_MODE(TRUE); // PPP Open 상태임을 세팅
						// Controller Init에 필요한 cData의 network type을 세팅	
						cData.NetType = TCM_NT_2G;
						cData.NetType = cData.NetType << 8;
						cData.NetType = cData.NetType | TCM_NT_95AB;
						// Net Connect 메시지를 보냄
						EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);	
					}
					else if (result == WAV_SOCK_WOULDBLOCK)	// Would Block된 경우 Timer Setting
					{
						MSG_ERROR("PPP Open would block",0,0,0);
						Sock_Timer = TRUE;
						// 20초간 기다리기 위한 TImer (EQS_TIMER_2) 세팅
						EQS_StartTimer(EQS_TIMER_2,(DWORD)20*1000, FALSE);	
					}
					else // PPP Open Error
					{
						MSG_ERROR("PPP Open ERROR",0,0,0);
						
						// Error Message 출력
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						break;
					}
				}
				else 
				{	// Wap으로 부터 불리지 않은 경우(Error 처리)
					MSG_ERROR("Not Wap to Wavelet",0,0,0);
					
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				}
			}
			break;
		
		// "***.*K 데이터서비스 연결"과 같은 곳으로 빠지지 않도록 하기 위해 break
		case SE_CALL_CONNECT:		
		case SE_CALL_ORIGINATED:
			break;

		case SE_NET_CONNECTED:
			SKY_SET_SOCK_MODE(TRUE); // PPP Open 상태임을 세팅
			Sock_Timer = FALSE;	// PPP Open이 성공하였으므로 Timer flag Reset
			EQS_StopTimer(EQS_TIMER_2);	// Timer Stop

			// Socket을 열고  Tcp Connect 상태임을 세팅
			WaveletStatus = SOCKCONNECT_STATUS;	

			// Socket 초기화
			MM_InitSocket();
			// Socket Open
			result = MM_OpenSocket();

			if (result != WAV_SOCK_SUCCESS) // Socket Open이 실패한 경우 
			{
				MSG_ERROR("Socket Open ERROR",0,0,0);
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;
			}
			
			// Socket Open이 성공한 경우
			MSG_ERROR("Socket Open Success",0,0,0);

			// Tcp Connect
			result 	= MM_Connect(g_wsndIP, (WORD)g_wsndPort);	
			
			if (result != WAV_SOCK_SUCCESS) // Tcp Connect가 실패한 경우 
			{
				MSG_ERROR("CONNECT ERROR",0,0,0);
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;
			}

			// Tcp Connect가 성공한 경우 
			MSG_ERROR("CONNECT Success",0,0,0);
			if (SKY_IS_IN_WAVELET())	// Wavelet임이 세팅이 된 경우 
			{
					MSG_HIGH("CONNECT_SUCCESS",0,0,0);
				// cData값을 세팅
				cData.CPID = g_wcpid;			// Contents Provider ID
				cData.ContentsID = g_wctsid;		// Contents ID
				cData.AudioDecoder = TCM_AD_EVRC;	// Available Audio Decoder
				cData.ColorDepth = TCM_CD_256;
				cData.SkinType = g_wskinType;		// SkinType
				cData.UDP_Port1 = 0; // yoursun 01/04/10
				cData.UDP_Port2 = 0; // yoursun 01/04/10

				//yoursun 01/06/08
				// MIN값 세팅
				nv_num.nam = SKY_GET_CURRENT_NAM();
				(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_num );
				if (nv_num.dir_number[0] == '0') // 10자리 번호의 경우
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11자리 번호의 경우 (11.....이렇게 나옴)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}
//////////////////////////////// Wavelet Contents Header Setting /////////////////////////////////////
				for (index = 0; index < 21; index++)	// 이름에 '/'를 포함한 컨텐츠의 경우 
				{										// 저장시 그 부분을 ' '으로 바꿔 저장
					if (g_wcntName[index] == '/') g_wcntName[index] = ' ';
				}
				
				// 헤더 파일 이름 세팅
				strcpy(szFileNameHeader, g_wcntName);
				strcat(szFileNameHeader, ".inf");
				
				// 헤더에 저장해야 하는 값들을 넣어줌
				gWaveletHeader.cpid = g_wcpid;
				gWaveletHeader.ctsid = g_wctsid;
				strcpy(gWaveletHeader.cntName,g_wcntName);
				gWaveletHeader.cntProtect = g_wcntProtect;
				gWaveletHeader.cntExpDate = g_wcntExpDate;

				gWaveletHeader.dnDate = SKY_GetLocalTime(); 
//////////////////////////////////////////////////////////////////////////////////////////////////////

				// 컨텐츠 파일들의 이름 세팅
				strcpy(szFileNameVideo, g_wcntName);
				strcpy(szFileNameAudio, g_wcntName);
				strcpy(szFileNameText, g_wcntName);
				strcpy(szFileNameSkin, g_wcntName);
				
				strcat(szFileNameVideo, ".vdo");
				strcat(szFileNameAudio, ".ado");
				strcat(szFileNameText, ".cap");
				strcat(szFileNameSkin, ".skn");

				// 이어 받기와 새로 받기,새 컨텐츠 받기를 구분
				switch (gDownInfo)
				{
					case OLD_CONTENTS :	// 새로 받기
					case NEW_CONTENTS :	// 새 컨텐츠 받기
						// 임시 공간에 저장되어 있는 것이 있으면 삭제
						if ((UINT)SKY_GetNumFile(SKY_TEMPWAV_DIR) != 0)	
						{
								SKY_DeleteDataFiles(SDF_TEMPWAV);
						}

						// 헤더 파일을 만들고 헤더 정보 헤더 파일에 저장
						if (SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameHeader, &hFile_Header) == SDR_OK)
						{
							gWaveletHeader.fConnectDN = TRUE;	// 다운로드가 시작되었으나 완료되지 않음을 알리는 flag
							SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
							if (hFile_Header) 
							{
								SKY_CloseDataFile(hFile_Header);
								hFile_Header = NULL;
							}
						}
						else // 헤더 정보 저장 실패시
						{
							MSG_HIGH(" Header Create Error!",0,0,0);						
							if (hFile_Header) 
							{
								SKY_CloseDataFile(hFile_Header);
								hFile_Header = NULL;
							}
							
							sWaveletReleaseSocket();

							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}

						// 컨텐츠 파일을 만들기
						if ((SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameVideo, &hFile_Video) == SDR_OK)
						&&(SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameAudio, &hFile_Audio) == SDR_OK)
						&&(SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameText, &hFile_Text) == SDR_OK)
						&&(SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameSkin, &hFile_Skin) == SDR_OK))
						{
							// Controller Initialize 성공
							if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
							{	
								gContents_Length = 0;	// 컨텐츠 길이 변수 0으로 초기화 
								// Download 시작을 Core에 명령
								if (!MM_ProcessTCM2UICommand(TCM_UC_DOWNLOAD, gContents_Length, (DWORD)180000 )) 
								{ // 다운로드 시작이 실패한 경우 
									sReleaseController();
									sWaveletReleaseSocket();

									// Error Message 출력
									g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
									EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
									EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
								}
							}
							else // Controller Initialize 실패
							{
								sReleaseController();				
								sWaveletReleaseSocket();

								// Error Message 출력
								g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
								EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
								EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
							}
						}
						else //컨텐츠 파일 만들기가 실패한 경우 
						{
							sReleaseController();				
							sWaveletReleaseSocket();

							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						break;

					case CONNECT_CONTENTS:	// 이어받기
						// 다운로드 받다 말은 컨텐츠의 헤더를 열음 
						if (SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
						{
							if (gWaveletHeader.fConnectDN == FALSE) // Download 완료된 컨텐츠가 이어받기로 선택된 경우
							{
								extern void MM_OnBuffering(BYTE nPercent); //yoursun 01/03/20
								WaveletStatus = TCM_DS_DOWNLOAD_ENDED;
								//EQC_StaticText(0, 0, (EQS_TOKEN)TKN_BLANK);
								//EQC_StaticText(50, 0, (EQS_TOKEN)TKN_BLANK);
								MSG_ERROR("TCM_DS_DOWNLOAD_ENDED",0, 0, 0);
								
								if (hFile_Header) 
								{
									SKY_CloseDataFile(hFile_Header);
									hFile_Header = NULL;
								}

								MM_CloseTCM2Controller();
								sWaveletReleaseSocket();
								MM_OnBuffering(100);
								
								if (SKY_IS_SOCK_MODE())
								{
									sWaveletPPPClose();
								}
								break;
							}
							else	// Download가 완료되지 않은 컨텐트가 이어받기로 선택된 경우
							{
								gWaveletHeader.fConnectDN = TRUE;
								SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
								if (hFile_Header) 
								{
									SKY_CloseDataFile(hFile_Header);
									hFile_Header = NULL;
								}
							}
						}
						else // 헤더 파일을 열다 에러가 발생한 경우
						{
							MSG_HIGH(" Header Open Error!",0,0,0);						
							if (hFile_Header) 
							{
								SKY_CloseDataFile(hFile_Header);
								hFile_Header = NULL;
							}

							sWaveletReleaseSocket();

							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						// 다운로드 시작 명령시 현재까지 다운받은 크기를 알려줘야 하므로 각 파일의 크기를 더한다.
						gContents_Length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, szFileNameVideo);
						gContents_Length += (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, szFileNameAudio);
						gContents_Length += (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, szFileNameText);
						// 각 컨텐츠 파일을 연다.
						if ((SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameVideo, &hFile_Video, SFT_WRITE) == SDR_OK)
						&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameAudio, &hFile_Audio, SFT_WRITE) == SDR_OK)
						&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameText, &hFile_Text, SFT_WRITE) == SDR_OK)
						&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameSkin, &hFile_Skin, SFT_WRITE) == SDR_OK))
						{
							// 연 각 컨텐츠 파일의 포인터를 맨 뒤로 옮긴다.
							SKY_SeekFile(hFile_Video, FS_SO_END, (DWORD)0);    
							SKY_SeekFile(hFile_Audio, FS_SO_END, (DWORD)0);    
							SKY_SeekFile(hFile_Text, FS_SO_END, (DWORD)0);    
							SKY_SeekFile(hFile_Skin, FS_SO_END, (DWORD)0);    
							
							// Controller Initialize 성공
							if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
							{
								if (!MM_ProcessTCM2UICommand(TCM_UC_DOWNLOAD, gContents_Length, (DWORD)(180000-gContents_Length)))
								{
									sReleaseController();
									sWaveletReleaseSocket();

									// Error Message 출력
									g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
									EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
									EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);			
								}
							}
							else // Controller Initialize 실패
							{
								sReleaseController();
								sWaveletReleaseSocket();

								// Error Message 출력
								g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
								EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
								EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
							}
						}
						else //컨텐츠 파일 열기가 실패한 경우
						{
							sReleaseController();
							sWaveletReleaseSocket();

							// Error Message 출력
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						break;
				}
			}
			else // Wavelet임이 세팅이 안된 경우 
			{
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_NET_DISCONNECTED:	// PPP 연결이 끊겼다는 이벤트
			Sock_Timer = FALSE;		// PPP close 용 Timer로 사용이 끝났음을 세팅
			EQS_StopTimer(EQS_TIMER_1);	// PPP close Would Block시 사용하는 타이머 멈춤

			SKY_SET_SOCK_MODE(FALSE);	// PPP Close되었음을 세팅
			MSG_HIGH("Net Disconnected Success!",0,0,0);

			// PPP가 끊긴 상태에 따라서 다음 처리가 다르므로 각 상태에 맞게 처리방법 기술
			switch (WaveletStatus)
			{
				case TCM_DS_DOWNLOAD_ENDED:		// 다운로드 완료 후 PPP close한 경우 
					// sSKA_MainOnWavelet Applet을 child로 불러 임시 공간에 다운받은 컨텐츠를 플레이하도록 이벤트를 줌
					EQS_SEND_EVENT(SE_WAV_PLAY, EQS_NA, EQS_NA);
					break;

				case PHONE_CLOSE:				// Phone close 이벤트로 PPP close한 경우
					MSG_HIGH("phone close!",0,0,0);
					SKY_SET_FROM_WAP(FALSE);
					SKY_SET_WAVELET_TO_WAP(FALSE);

					EQS_END_TO(EQS_POP_TO_ROOT);
					break;

				case CALL_DROP:					// Call Drop으로 인해 PPP close한 경우
					MSG_HIGH("Call Drop",0,0,0);

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				case INIT_STATUS:				// Wavelet을 끝내는 상황에서 PPP close한 경우
					MSG_HIGH("init state",0,0,0);

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				case TCM_DS_CONTENTS_NOT_FOUND:	// 네트워크 에러 발생으로 인한 PPP close한 경우
					MSG_HIGH("contents not found",0,0,0);
				case TCM_DS_CONTROLLER_ERROR:
					MSG_HIGH("controller error",0,0,0);

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				case TCM_DS_SETUP_ERROR:		// 코어 에러 발생으로 인한 PPP close한 경우
					MSG_HIGH("setup error",0,0,0);
				case TCM_DS_SERVER_ERROR:
					MSG_HIGH("server_error",0,0,0);									

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				default:
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
			}
			break;

		case SE_CALL_ENDED:	// Call Drop 처리를 위해 Call Drop이 일어난 call End의 경우만 처리
			MSG_HIGH("CALL END",0,0,0);
			if ((SKY_GetCallEndStatus() == CM_CALL_END_NO_SRV)||(SKY_GetCallEndStatus() == CM_CALL_END_FADE))
			 {
				// call drop 상태임을 세팅
				WaveletStatus = CALL_DROP;	
				
				// socket close & ppp close
				sReleaseController();
				sWaveletReleaseSocket();
				if (SKY_IS_SOCK_MODE())
				{
					sWaveletPPPClose();
				}
			 }
			break;

		case SE_APP_END :   /* The very last event */ 
			// Backlignt 원상 복귀
			SKY_SET_NO_OFF_BACKLIGHT(FALSE);
			SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
			break;

		case SE_WAV_PLAY:
			// 임시 공간에 다운받은 컨텐츠를 플레이하는 Applet을 Child로 부름
			EQS_START_CHILD(sSKA_MainOnWavelet);
			break;

		case SE_KEY_CLEAR :// HS_CLR_K:
		case SE_SOFTKEY_CANCEL:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			sReleaseController();
			sWaveletReleaseSocket();

		    // Error Message 출력
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_KEY_0 ://HS_0_K:
		case SE_KEY_1 ://HS_1_K:
		case SE_KEY_2 ://HS_2_K:
		case SE_KEY_3 ://HS_3_K:
		case SE_KEY_4 ://HS_4_K:
		case SE_KEY_5 ://HS_5_K:
		case SE_KEY_6 ://HS_6_K:
		case SE_KEY_7 ://HS_7_K:
		case SE_KEY_8 ://HS_8_K:
		case SE_KEY_9 ://HS_9_K:
		case SE_KEY_STAR ://HS_STAR_K:
		case SE_KEY_POUND ://HS_POUND_K:
		case SE_KEY_UP ://HS_UP_K:
		case SE_KEY_DOWN ://HS_DOWN_K:
		case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
		case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
        // Ignore all these events
			break;

	    case SE_KEY_PHONE_CLOSE:	// Phone close가 일어난 경우
			// Phone Close 상태임을 세팅
			WaveletStatus = PHONE_CLOSE;
			
			// 하던 작업을 멈춤
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			sReleaseController();

			// socket close + PPP close
			sWaveletReleaseSocket();
			if (SKY_IS_SOCK_MODE())
			{
				sWaveletPPPClose();
			}
			break;

		case SE_KEY_END:			// 사용자가 End 키를 누른 경우 
			// 하던 작업을 멈춤
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			sReleaseController();
			
			// socket close 
			sWaveletReleaseSocket();
			
			if (WaveletStatus == TCM_DS_DOWNLOAD_STARTED) // 다운로드 도중이면 
			{
			    // Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
		        EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
			    EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else										// 다운로드 도중이 아니면 
			{
				WaveletStatus = INIT_STATUS;
				if (SKY_IS_SOCK_MODE())
				{
					sWaveletPPPClose();
				}
			}
			break;

		case SE_SYS_TO_IDLE : /* very important */
		case SE_SYS_OFFLINE:
			WaveletStatus = INIT_STATUS;
			
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			sReleaseController();
			sWaveletReleaseSocket();

			if (SKY_IS_SOCK_MODE())
			{
				sWaveletPPPClose();
			}
			break;

		case SE_MUXNEGO_SUCCESS:		// Mux Option Nego가 성공
			MSG_HIGH("SE_MUXNEGO_SUCCESS",0,0,0);
			if (WaveletStatus != PPPOPEN_STATUS)	// 2G에서 PPP Open 할 때의 Mux Nego가 아닐 때
			{
				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
				EQS_START_SIBLING(SKA_MainWAP, FALSE);	// Wap으로 돌아감
			}
			break;

		case SE_IO_MESGBOX_END:
            if (pEvent->dwParam == WAV_MES_1)	// Wavelet을 종료하는 메시지
			{
				SKY_SET_FROM_WAP(FALSE);
				SKY_SET_WAVELET_TO_WAP(FALSE);

				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else if( pEvent->dwParam == WAV_MES_3)	
			{
			    // Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
                EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_4)
			{
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}

			break;

		case SE_IO_MESGBOX_YES:
			if ( pEvent->dwParam == WAV_MES_2)
			{
				SKY_SET_FROM_WAP(FALSE);      			
				SKY_SET_WAVELET_TO_WAP(TRUE);
				// MO=0x01			
				if ( P_REV_IS_6_OR_GREATER )	SetMOinSO33(0);   
				else 
				{
					if (SKY_IS_IN_WAVELET())
					{
						sReleaseWavelet();
					}
					EQS_START_SIBLING(SKA_MainWAP, FALSE);
				}
            }
			break;

		case SE_IO_MESGBOX_NO:
	        if( pEvent->dwParam == WAV_MES_2)
			{
				WaveletStatus = INIT_STATUS;
				if (SKY_IS_SOCK_MODE())
				{
					sWaveletPPPClose();
				}
				else 
				{
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
			}
			break;

		case SE_TM_TIMER:
			MSG_ERROR("General_Timer",0,0,0);
			(ti[EQS_GENERAL_TIMER]).func(EQS_GENERAL_TIMER+TIMER_ID_CHANGE+WAV_GENERAL_TIMER,(ti[EQS_GENERAL_TIMER]).user);
			break;

		case SE_TM_TIMER1:
			if (Sock_Timer == FALSE)
			{
				(ti[EQS_TIMER_1]).func(EQS_TIMER_1+TIMER_ID_CHANGE,(ti[EQS_TIMER_1]).user);		
			}
			else //	Sock_Timer = TRUE;	<= PPP Close Would Block에 사용
			{
				Sock_Timer = FALSE;
				if (SKY_IS_SOCK_MODE())
				{
					sWaveletPPPClose();
				}
			}
			break;

		case SE_TM_TIMER2:
			if (Sock_Timer == FALSE)
			{
				(ti[EQS_TIMER_2]).func(EQS_TIMER_2+TIMER_ID_CHANGE,(ti[EQS_TIMER_2]).user);		
			}
			else //	Sock_Timer = TRUE <= PPP Opne Would Block에 사용
			{
				Sock_Timer = FALSE;
				result = MM_PPPOpen();
				if(result == WAV_SOCK_SUCCESS)
				{
					MSG_ERROR("PPP Open Success",0,0,0);
					SKY_SET_SOCK_MODE(TRUE);

					cData.NetType = TCM_NT_2G;
					cData.NetType = cData.NetType << 8;
					cData.NetType = cData.NetType | TCM_NT_95AB;

					EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);	
				}
				else if (result == WAV_SOCK_WOULDBLOCK) {
					MSG_ERROR("PPP Open would block",0,0,0);
					Sock_Timer = TRUE;
					EQS_StartTimer(EQS_TIMER_2,(DWORD)20*1000, FALSE);	
				}
				else // WAV_SOCK_ERROR
				{
					MSG_ERROR("PPP Open ERROR",0,0,0);
					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
			}
			break;

		case SE_TM_TIMER3:
			(ti[EQS_TIMER_3]).func(EQS_TIMER_3+TIMER_ID_CHANGE,(ti[EQS_TIMER_3]).user);
			break;

		case SE_PH_RSSI: // 화면 리프레쉬를 통한 빈 화면 출력을 막음
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		MM_DisplayImage

Description:	이미지 출력(Partial Update)
  
Input:			pData : 디스플레이 될 Image
				stride: 다음 줄 출력을 위해 건너 뛰어야 하는 길이
				sx,sy : 출력을 시작할 좌표 
				width : Image의 너비
				height: Image의 높이
    
Output:			주어진 text를 (x,y)부터 width*height만큼 출력한다.
      
Return value:
        
Side effects:
===============================================================================*/
BOOL MM_DisplayImage(BYTE *pData, BYTE stride, BYTE sx, BYTE sy, BYTE width, BYTE height)
{
	// Set Image Data Q signal of HS task
	dog_report( DOG_UI_RPT);

	g_bStride = stride;
	
	MSG_ERROR("Displayimage x:%d, y:%d",sx, sy, 0);
	SKY_UpdatePartialScreen(pData, sx, sy+26/*+7+19*/, sx+width, sy+height+26/*+7+19*/);
	return TRUE;
}

/*============================================================================= 
Function:		MM_DisplayImageEx

Description:	이미지 출력(Partial Update) : 256 color 이상
  
Input:			
    
Output:			
      
Return value:
        
Side effects:
===============================================================================*/
//#define DISPAY_FOR_LOOP // For loop을 이용하여 Display할 경우 정의 
						  // 정의되어 있지 않으면 for loop을 푼 것을 말함 
BOOL MM_DisplayImageEx(PCOLORDATA pCD, PCOLORMAP pCMap)
{
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];	
	extern int SKY_UpdatePartialScreenWaveletEx(BYTE *, UINT8, UINT8, UINT8, UINT8);
	BYTE nWidth, nHeight;

	WORD *tColor = (WORD*)(Image$$APP_SHARE_RAM$$Base+300000);
	BYTE t_width = pCMap->m_Width >> 3;

	BYTE* pY = pCD->m_pFirst;
	BYTE* pU = pCD->m_pSecond;
	BYTE* pV = pCD->m_pThird;
	BYTE* pYL = pY+112;
	
	register unsigned short nOffset = 0, nIndex = 0;
	WORD *pDest  = (WORD*)(Image$$APP_SHARE_RAM$$Base+300000);
	WORD *pDestL = (WORD*)(Image$$APP_SHARE_RAM$$Base+300000+224);

	MSG_ERROR("DisplayImageEx1", 0, 0, 0);

	for (nHeight = 0; nHeight < pCMap->m_Height ; nHeight+=2)  
	{
#ifdef DISPLAY_FOR_LOOP
		for (nWidth = 0; nWidth < t_width ; nWidth++) 
		{
#endif // DISPLAY_FOR_LOOP
//1
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
#ifndef DISPLAY_FOR_LOOP
//2
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

//3
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

//4
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//5
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//6
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//7
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//8
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//9
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//10
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//11
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//12
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//13
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
//14
				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];

				nIndex = ( (unsigned short)*pU++ << 4 ) | *pV++;
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDest++ = RGBTable[ nIndex | ( (unsigned short)*pY++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
				*pDestL++ = RGBTable[ nIndex | ( (unsigned short)*pYL++ << 8 ) ];
#else // DISPLAY_FOR_LOOP
		}		
#endif // DISPLAY_FOR_LOOP
		pY+=112;
		pYL+=112;
		pDest+=112;
		pDestL+=112;

		if ( nHeight & 0x01 )
		{
			pU -= pCMap->m_Width >> 1;
			pV -= pCMap->m_Width >> 1;
		}
	}
				MSG_ERROR("DisplayImageEx2", 0, 0, 0);
//	SKY_RefreshDisplayRectByRegion(tColor, 8, 25, (t_width<<3)+7, pCMap->m_Height+24/*24+7*/); // Display Region
//	SKY_UpdatePartialDisplayRectByRegion(tColor, 8, 25, (t_width<<3)+7, pCMap->m_Height+24/*24+7*/);
	SKY_UpdatePartialScreenWaveletEx(tColor, 8, 25, 119, 120/*(t_width<<3)+7, pCMap->m_Height+24/*24+7*/);
				MSG_ERROR("DisplayImageEx3", 0, 0, 0);
	return TRUE;
}

/*============================================================================= 
Function:		MM_DisplaySkin

Description:	스킨 출력(Partial Update)
  
Input:			
    
Output:			
      
Return value:
        
Side effects:
===============================================================================*/
void	MM_DisplaySkin( PTCM2_SKIN pSkin )
{
	extern BYTE	Image$$APP_SHARE_RAM$$Base[];	
	BYTE nWidth, nHeight;
	BYTE t_width = pSkin->pFrame->dx, t_height = pSkin->pFrame->dy;
	WORD *wColor = (WORD*)(Image$$APP_SHARE_RAM$$Base+322000);
	BYTE * t_RGB = pSkin->pFrame->pRGB;

			MSG_ERROR("DisplaySkin %d %d %d", t_width, t_height, pSkin->pFrame->x);

	for (nHeight = 0; nHeight < t_height; nHeight++) // Change 24 Bit RGB into 16 Bit RGB
	{
		for (nWidth = 0; nWidth < t_width ; nWidth++)
		{
			wColor[nWidth + nHeight*t_width] = RGB565(*t_RGB, *(t_RGB+1), *(t_RGB+2));
			t_RGB += 3;
		}
	}
	// Display Region
//	SKY_RefreshDisplayRectByRegion((BYTE *)wColor, pSkin->pFrame->x, pSkin->pFrame->y ,pSkin->pFrame->x + t_width -1 , pSkin->pFrame->y + t_height -1/* -1+22*/);
	SKY_UpdatePartialDisplayRectByRegion((BYTE *)wColor, pSkin->pFrame->x, pSkin->pFrame->y ,pSkin->pFrame->x + t_width -1 , pSkin->pFrame->y + t_height -1/* -1+22*/);
}

/*============================================================================= 
Function:			MM_DisplayText

Description:		Text 출력
  
Input:				data : 디스플레이 될 text
					x,y : 좌표 

Output:				주어진 text를 (x,y)에 출력한다.
      
Return value:
        
Side effects:
===============================================================================*/
void MM_DisplayText(BYTE *data, BYTE x, BYTE y)
{
	extern int SKY_UpdatePartialScreenWaveletText(BYTE *, UINT8, UINT8, UINT8, UINT8);
	BYTE *pLCD;

			MSG_ERROR("Text x:%d,y:%d, %s", x, y, data);

	EQS_TextOutStr(x, y, data);
	pLCD = GetScreenBuffer();
	pLCD += 32264;
//	SKY_RefreshDisplayRect(pLCD, x, y, x+TextWidth(data)-1, y+HANGUL_FONT_Y);
	SKY_UpdatePartialScreenWaveletText(pLCD, x, y, 123, y+HANGUL_FONT_Y);
				MSG_ERROR("DisplayText x:%d, y:%d",x, y, 0);
}

/*============================================================================= 
Function:		MM_GetCharSize

Description:	폰트 크기 전달
  
Input:			bkor: 한글인지 영어인지 여부 ( 1 => 한글, 0 => 영어)
				x	: 각 폰트의 너비
				y	: 각 폰트의 높이

Output:			폰트의 너비와 높이 값이 x, y에 전달되어 진다.

Return value:

Side effects:
===============================================================================*/
BOOL MM_GetCharSize(BOOL bkor, BYTE *x, BYTE *y) //yoursun 01/04/07
{
			MSG_ERROR("MM_GetCharSize",0, 0, 0);
	if (bkor)
	{
		*x = HANGUL_FONT_X;
		*y = HANGUL_FONT_Y;
	}
	else 
	{
		*x = YOUNGMUN_FONT_X;
		*y = YOUNGMUN_FONT_Y;
	}
	return TRUE;
}

/*============================================================================= 
Function:		MM_GetDisplayRect

Description:	가용 LCD 크기 전달

Input:			x,   y : 가용 LCD의 시작점 위치
				ex, ey : 가용 LCD의 가로, 세로의 길이

Output:			x, y, ex, ey에 값이 전달되어 진다.

Return value:

Side effects:
===============================================================================*/
void MM_GetDisplayRect(BYTE *x, BYTE *y, BYTE* ex, BYTE* ey)
{
			MSG_ERROR("MM_GetDisplayRect",0, 0, 0);
	*x = 4;
	*y = 21;
	*ex = DISPLAY_X - 8;
	*ey = DISPLAY_Y - 17;
	return;
}

/*============================================================================= 
Function:	MM_OnBuffering

Description: 버퍼링 % 출력

Input:		nPercent : 버퍼링 률
    
Output:		입력된 버퍼링 %를 화면에 출력한다.
			다운로드와 버퍼링의 표시를 분리하여 출력한다.
      
Return value:
                
Side effects:
===============================================================================*/
void MM_OnBuffering(BYTE nPercent) //yoursun 01/03/20
{
	BYTE			szString[BUFFER_MSG_LENGTH];
	BYTE *pLCD; //MM_Text yoursun
	extern int sprintf( char *buffer, const char *format,...);   

	EQS_SetTextBackColor(EQS_GetBackColor());
	EQS_ClearRegion(0, WAVELET_BUFFER_MSG_YPOS, SKY_SCREEN_WIDTH-1, WAVELET_BUFFER_MSG_YPOS+HANGUL_FONT_Y );

	if (fDownload == TRUE)  // Download Service 일 경우 
	{
		if (nPercent == 100) sprintf((CHAR*)szString, SKY_GET_TOKEN(TKN_WAVELET_DOWNLOAD_DONE));
		else sprintf((CHAR*)szString, SKY_GET_TOKEN(TKN_WAVELET_DOWNLOAD_PERCENT), nPercent);
	}
	else  // Streaming Service 일 경우
	{
		if (nPercent == 100) 
		{
			sprintf((CHAR*)szString, SKY_GET_TOKEN(TKN_WAVELET_BUFFERING_DONE));
			EQS_SetWindowCML(CL_NONE_NONE_END);
		}
		else sprintf((CHAR*)szString, SKY_GET_TOKEN(TKN_WAVELET_BUFFERING_PERCENT), nPercent);
	}
	
	EQS_TextOutStr(WAVELET_BUFFER_MSG_XPOS, WAVELET_BUFFER_MSG_YPOS, szString);
	pLCD = GetScreenBuffer();
	
	SKY_RefreshDisplayRect(pLCD, WAVELET_BUFFER_MSG_XPOS, WAVELET_BUFFER_MSG_YPOS, \
						WAVELET_BUFFER_MSG_XPOS+TextWidth(szString)-1, WAVELET_BUFFER_MSG_YPOS+TextHeight(szString)-1);
}

/*============================================================================= 
Function:		MM_GetFreeMemory

Description:	메모리 할당
  
Input:			size : 할당되어야 하는 메모리 크기
					   0이면 최대할당 가능 메모리를 할당하라는 의미    
Output:			요구된 크기의 메모리를 할당한다.
      
Return value:	할당된 메모리의 포인터
        
Side effects:
===============================================================================*/
void * MM_GetFreeMemory(TCM_MEM_TYPE type, DWORD* size )
{
    extern BYTE	Image$$APP_SHARE_RAM$$Base[]; //  Shared RAM의 시작 Address

	// size의 값이 0인 경우는 각 Buffer에 대해 최대 할당 가는한 메모리를 할당함 
	// 현재 최대할당 메모리인 100k가 할당되었음을 *size에 assign함
	if(*size == (DWORD)0)    		*size = (DWORD)100000;

	// 메모리는 Shared RAM중 사용가능한 부분의 시작 포인터를 준다.
	if (type == TCM_MT_FRAME_BUFFER)		// Frame Buffer 할당 요청이 들어온 경우
	{
		MSG_ERROR("TCM_MT_FRAME_BUFFER : size %d",*size,0,0);
		return (BYTE *)Image$$APP_SHARE_RAM$$Base;
	}
	else if (type == TCM_MT_PACKET_BUFFER)	// Packet Buffer 할당 요청이 들어온 경우
	{
		MSG_ERROR("TCM_MT_PACKET_BUFFER : size %d",*size,0,0);
		return (BYTE*)(Image$$APP_SHARE_RAM$$Base+100000);
	}

	// Frame Buffer나 Packet Buffer가 아닌 다른 메모리 타입의 할당 요청이 들어온 경우 
	MSG_ERROR("BUFFER Type Error",0,0,0);
	return (BYTE *)0;
}

/*============================================================================= 
Function:		MM_TCM2ServerStatus

Description:	서버의 상태 리포트 
  
Input:			status	: 서버로부터 온 상태값
				data	: 상태에 따라 필요한 data 값

Output:			서버의 상태를 받는다.

Return value:

Side effects:
===============================================================================*/
void	MM_TCM2ServerStatus( TCM_DECODER_STATUS status, BYTE* data )
{
	switch (status)
		 {
			case TCM_DS_DOWNLOAD_ENDED:	//Download Ended
				WaveletStatus = TCM_DS_DOWNLOAD_ENDED;
				MM_OnBuffering(100);
						MSG_ERROR("TCM_DS_DOWNLOAD_ENDED",0, 0, 0);
				
				gWaveletHeader.fConnectDN = FALSE; // 다운로드 완료를 알림
				if (SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameHeader, &hFile_Header ,SFT_WRITE) == SDR_OK)
				{
					SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
					if (hFile_Header) 
					{
						SKY_CloseDataFile(hFile_Header);
						hFile_Header = NULL;
					}
					sReleaseController();
					sWaveletReleaseSocket();
					if (SKY_IS_SOCK_MODE())
					{
						sWaveletPPPClose();
					}
				}
				else 
				{
					MSG_HIGH(" Header Open Error!",0,0,0);						
					if (hFile_Header) 
					{
						SKY_CloseDataFile(hFile_Header);
						hFile_Header = NULL;
					}

					sReleaseController();
					sWaveletReleaseSocket();

					// Error Message 출력
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl, WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
				break;
			
			case TCM_DS_STREAMING_ENDED://Streaming Ended
				WaveletStatus = TCM_DS_STREAMING_ENDED;
						MSG_ERROR("TCM_DS_STREAMING_ENDED",0, 0, 0);

				MM_CloseTCM2Controller();
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_GOTO_NTOP), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;

			case TCM_DS_PLAY_ENDED://Play Ended
				fInitPlay = TRUE;
				WaveletStatus = TCM_DS_PLAY_ENDED;
						MSG_ERROR("TCM_DS_PLAY_ENDED",0, 0, 0);
				MM_CloseTCM2Controller();

				if (fLongKey == TRUE) EQS_SEND_EVENT(SE_KEY_F1_LONG, EQS_NA, EQS_NA);
				else fLongKey = FALSE;
				break;

			case TCM_DS_SETUP_STARTED://Setup Started
				WaveletStatus = TCM_DS_SETUP_STARTED;
				MSG_ERROR("TCM_DS_SETUP_STARTED", 0, 0, 0);
				break;

			case TCM_DS_DOWNLOAD_STARTED://Download Started
				WaveletStatus = TCM_DS_DOWNLOAD_STARTED;
				EQC_StaticText(0, 0, (EQS_TOKEN)TKN_BLANK);
				EQC_StaticText(50, 0, (EQS_TOKEN)TKN_BLANK);
				MSG_ERROR("TCM_DS_DOWNLOAD_STARTED",0, 0, 0);
				break;

			case TCM_DS_STREAMING_STARTED://Streaming Started
				WaveletStatus = TCM_DS_STREAMING_STARTED;
				EQC_StaticText(0, 0, (EQS_TOKEN)TKN_BLANK);
				EQC_StaticText(50, 0, (EQS_TOKEN)TKN_BLANK);
				MSG_ERROR("TCM_DS_STRAMING_STARTED",0, 0, 0);

				send_ui_cmd(UI_WAV_STREAM_CMD_F);
				break;
    
			case TCM_DS_DECODER_ERROR://Internal decoder error
				WaveletStatus = TCM_DS_DECODER_ERROR;
				MSG_ERROR("TCM_DS_DECODER_ERROR %d",(unsigned int)data, 0, 0);
				MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
				sReleaseController();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;

   
			case TCM_DS_CONTROLLER_ERROR://Internal controller error
				WaveletStatus = TCM_DS_CONTROLLER_ERROR;
				MSG_ERROR("TCM_DS_CONTROLLER_ERROR",0, 0, 0);
				
				MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
				sReleaseController();
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;
    
			case TCM_DS_SERVER_ERROR://Internal server error
				WaveletStatus = TCM_DS_SERVER_ERROR;
				MSG_ERROR("TCM_DS_SERVER_ERROR",0, 0, 0);
				
				MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
				sReleaseController();
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;
    
			case TCM_DS_SETUP_ERROR://Setup error
				WaveletStatus = TCM_DS_SETUP_ERROR;			
				MSG_ERROR("TCM_DS_SETUP_ERROR",0, 0, 0);

				sReleaseController();
				sWaveletReleaseSocket();
	
				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;
    
			case TCM_DS_CONTENTS_NOT_FOUND://Contents not found
				WaveletStatus = TCM_DS_CONTENTS_NOT_FOUND;
				MSG_ERROR("TCM_DS_CONTENTS_NOT_FOUND",0, 0, 0);
				sReleaseController();
				sWaveletReleaseSocket();

				// Error Message 출력
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;

			case TCM_DS_CONTENTS_SIZE:
				MSG_ERROR("TCM_DS_CONTNTS_SIZE",0, 0, 0);
				gContents_Length = *((DWORD *)data);
				MSG_ERROR("Contents Size is %d",gContents_Length, 0, 0);
				break;
    
	        default:
			    break;
     }
}

/*============================================================================= 
Function:		MM_ReadData

Description:	데이터 읽기

Input:			type : 읽을 데이터가 비디오, 오디오, 스킨, 텍스트인지 구분
				data : 파일로부터 읽혀진 데이터
				size : 읽을 데이터의 크기

Output:			주어진 타입의 데이터를 파일로부터 읽는다.

Return value:	실제로 읽은 데이터의 길이

Side effects:
===============================================================================*/
WORD	MM_ReadData( TCM_FILE_TYPE type, BYTE* data, WORD size )
{
	WORD	ResultSize = 0;
	MSG_ERROR("Read type %d, In_Size %d",type,size,0);

	if (type == TCM_FT_VIDEO)
	{
		if (Video_length != 0 )
		{
			if ((long)(Video_length-=size) >= 0)
			{
				memcpy(data, pVdoData, (DWORD)size);
				pVdoData += size;
				ResultSize = size;
			}
			else 
			{
				Video_length+=size;
				memcpy(data, pVdoData, (DWORD)Video_length);
				pVdoData += Video_length;
				ResultSize = Video_length;
				Video_length = 0;
			}
		}
		else ResultSize = 0;
		MSG_ERROR("MM_ReadData : 비디오 읽기, size %d",ResultSize, 0, 0);
	}
	else if (type == TCM_FT_AUDIO)
	{
		if (Audio_length != 0 )
		{
			if ((long)(Audio_length-=size) >= 0)
			{
				memcpy(data, pAdoData, (DWORD)size);
				pAdoData += size;
				ResultSize = size;
			}
			else 
			{
				Audio_length+=size;
				memcpy(data, pAdoData, (DWORD)Audio_length);
				pAdoData += Audio_length;
				ResultSize = Audio_length;
				Audio_length = 0;
			}
		}
		else ResultSize = 0;
	
			MSG_ERROR("MM_ReadData : 오디오 읽기, size %d",ResultSize, 0, 0);
	}
	else if (type == TCM_FT_TEXT)
	{
		if (Text_length != 0 )
		{
			if ((long)(Text_length-=size) >= 0)
			{
				memcpy(data, pTxtData, (DWORD)size);
				pTxtData += size;
				ResultSize = size;
			}
			else 
			{
				Text_length+=size;
				memcpy(data, pTxtData, (DWORD)Text_length);
				pTxtData += Text_length;
				ResultSize = Text_length;
				Text_length = 0;
			}
		}
		else ResultSize = 0;
			MSG_ERROR("MM_ReadData: 자막 읽기, size %d",ResultSize, 0, 0);
	}
	else if (type == TCM_FT_SKIN)
	{
		if (Skin_length != 0 )
		{
			if ((long)(Skin_length-=size) >= 0)
			{
				memcpy(data, pSknData, (DWORD)size);
				pSknData += size;
				ResultSize = size;
			}
			else 
			{
				Skin_length+=size;
				memcpy(data, pSknData, (DWORD)Skin_length);
				pSknData += Skin_length;
				ResultSize = Skin_length;
				Skin_length = 0;
			}
		}
		else ResultSize = 0;
			MSG_ERROR("MM_ReadData: 스킨 읽기, size %d",ResultSize,0,0);
	}
	if (ResultSize == 0) ResultSize = 0xffff;
	return (WORD)ResultSize;
}

/*============================================================================= 
Function:		MM_WriteData

Description:	데이터 쓰기

Input:			type : 쓸 데이터가 비디오, 오디오, 스킨, 텍스트인지 구분
				data : 파일에 쓸 데이터
				size : 쓸 데이터의 크기

Output:			입력 데이터를 주어진 타입의 파일에 쓴다.

Return value:	실제로 쓴 데이터의 길이

Side effects:
===============================================================================*/
DWORD	MM_WriteData( TCM_FILE_TYPE type, BYTE* data, DWORD size )
{// 다운로드 서비스에서만 사용되는 API
	static	BYTE	pMallocData[2000];	// 각 파일에 데이터를 쓰기 위해 필요한 변수 
	DWORD	ResultSize = 0;				// 실제로 쓴 데이터의 크기를 위한 변수

	memset((void*)pMallocData,0x00,2000);	// 메모리 초기화
	
	if (type == TCM_FT_VIDEO)	
	{				// 비디오 타입의 데이터를 비디오용 파일에 쓴다.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Video, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData:비디오 in %d, out %d", size, ResultSize, 0);
	}
	if (type ==	TCM_FT_AUDIO)
	{				// 오디오 타입의 데이터를 오디오용 파일에 쓴다.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Audio, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData:오디오 in %d, out %d", size, ResultSize, 0);
	}
	if (type == TCM_FT_TEXT)
	{				// 자막 타입의 데이터를 자막용 파일에 쓴다.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Text, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData:자막 in %d, out %d", size, ResultSize, 0);
	}
	if (type == TCM_FT_SKIN)
	{				// 스킨 타입의 데이터를 스킨용 파일에 쓴다.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Skin, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData: 스킨 in %d, out %d", size, ResultSize, 0);
	}
	if (ResultSize == 0) ResultSize = 0xffffffff;
	return ResultSize;
}

/*============================================================================= 
Function:		MM_Seek

Description:	파일 Seek
				offset이 O이 아닌 경우에는 사용되지 않으므로 0인 경우만 구현
				(즉 파일의 처음으로 이동할 때만 Seek가 사용됨)

Input:			type : seek할 파일이 비디오, 오디오, 스킨, 텍스트인지 구분
				offset : 파일 처음을 기준으로 seek할 위치

Output:			주어진 타입의 파일에 대한 주어진 offset만큼 seek한다.

Return value:	실제로 seek한 위치

Side effects:
===============================================================================*/
DWORD	MM_Seek(TCM_FILE_TYPE type, DWORD offset)
{
	if (g_wsvcType == SVC_IDLE)	// Wavelet 보관함에 있는 컨텐츠 재생시
	{
		switch (type)
		{
			case TCM_FT_VIDEO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Video_length = tVideo_length;	// 비디오 컨텐츠에 대한 Acces 길이 초기화
					pVdoData = ptVdoData;			// 비디오 컨텐츠에 대한 포인터 초기화
							MSG_ERROR("MM_Seek: 비디오 파일 length %d", Video_length, 0, 0);
				}
			break;

			case TCM_FT_AUDIO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Audio_length = tAudio_length;	// 오디오 컨텐츠에 대한 Acces 길이 초기화
					pAdoData = ptAdoData;			// 오디오 컨텐츠에 대한 포인터 초기화
							MSG_ERROR("MM_Seek: 오디오 파일 length %d",Audio_length, 0, 0);
				}
			break;

			case TCM_FT_TEXT:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Text_length = tText_length;	// 스킨 컨텐츠에 대한 Acces 길이 초기화
					pTxtData = ptTxtData;		// 스킨 컨텐츠에 대한 포인터 초기화
							MSG_ERROR("MM_Seek: Text 파일 length %d",Text_length, 0, 0);
				}
			break;
			}
	}
	else						// 다운로드 후 임시 저장 공간에 있는 컨텐츠 재생시
	{
		switch (type)
		{

			case TCM_FT_VIDEO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Video_length = tVideo_length;	// 비디오 컨텐츠에 대한 Acces 길이 초기화
					pVdoData = ptVdoData;			// 비디오 컨텐츠에 대한 포인터 초기화
							MSG_ERROR("MM_Seek: Video오 파일 length %d",Video_length, 0, 0);
				}
			break;

			case TCM_FT_AUDIO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Audio_length = tAudio_length;	// 오디오 컨텐츠에 대한 Acces 길이 초기화
					pAdoData = ptAdoData;			// 오디오 컨텐츠에 대한 포인터 초기화

							MSG_ERROR("MM_Seek: 오디오 파일 length %d",Audio_length, 0, 0);
				}
			break;

			case TCM_FT_TEXT:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Text_length = tText_length;		// 자막 컨텐츠에 대한 Acces 길이 초기화
					pTxtData = ptTxtData;			// 자막 컨텐츠에 대한 포인터 초기화
							MSG_ERROR("MM_Seek: Text 파일 length %d",Text_length, 0, 0);
				}
			break;

			case TCM_FT_SKIN:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Skin_length = tSkin_length;		// 스킨 컨텐츠에 대한 Acces 길이 초기화
					pSknData = ptSknData;			// 스킨 컨텐츠에 대한 포인터 초기화
							MSG_ERROR("MM_Seek: Skin 파일 length %d",Skin_length, 0, 0);
				}
			break;
			}
	}
	return offset;
}

/*============================================================================= 
Function:		MM_GetFileLength

Description:	파일 길이 
  
Input:			type : 길이를 알려줄 파일이 비디오, 오디오, 스킨, 텍스트인지 구분

Output:			주어진 타입의 파일의 길이를 알려준다.

Return value:	파일의 길이

Side effects:
===============================================================================*/
DWORD	MM_GetFileLength( TCM_FILE_TYPE type )//yoursun 01/04/10
{
	DWORD length = 0;
	MSG_ERROR("MM_GetFileLength",0,0,0);

	if (g_wsvcType == SVC_IDLE)		// Wavelet 보관함에 있는 컨텐츠 재생시
	{
		switch (type)
		{
			case TCM_FT_VIDEO:		// 비디오 
				length = tVideo_length;
				break;

			case TCM_FT_AUDIO:		// 오디오
				length = tAudio_length;				
				break;

			case TCM_FT_TEXT:		// 자막
				length = tText_length;
				break;
		}
	}
	else							// 다운로드 후 임시 저장 공간에 있는 컨텐츠 재생시
	{
		switch (type)
		{
			case TCM_FT_VIDEO:		// 비디오
				length = tVideo_length;
				break;

			case TCM_FT_AUDIO:		// 오디오
				length = tAudio_length;				
				break;

			case TCM_FT_TEXT:		// 자막
				length = tText_length;
				break;

			case TCM_FT_SKIN:		//스킨
				length = tSkin_length;
				break;
		}
	}
	return (DWORD)length;
}

/*============================================================================= 
Function:		MM_WaveletListDisplay

Description:	Idle 상태에서 다운받은 list를 보여줌
  
Input:			SizeOrNot - 파일 길이가 필요한 리스트인지 아닌지를 알려준다.

Output:			주어진 타입의 파일의 길이를 알려준다.

Return value:	

Side effects:
===============================================================================*/
void MM_WaveletListDisplay(BOOL SizeOrNot)
{	
	INT8 sWindowSize;
	BYTE* ppWaveletFileList[SKY_MAX_NUM_FILES];	// Wavelet File List를 읽어와서 임시로 저장하는 list 변수 
	int  tSize;									// 컨텐츠의 크기를 계산해서 임시로 가지는 변수
	UINT8 nLines = 0, index = 0;
	BYTE  tFileNameHeader[25];					// 컨텐츠 크기를 계산할 때 필요한 임시 파일 이름 변
	BYTE  tFileNameVideo[25];
	BYTE  tFileNameAudio[25];
	BYTE  tFileNameText[25];
	char Size[7];								// 컨텐츠 크기를 Kbyte 단위로 환산하여 character형으로 만든 변수
	
	g_unNumOfWavelet = 0;

    if((UINT)SKY_GetNumFile(SKY_WAVELET_DIR) == 0)	// Wavelet 보관함에 컨텐츠가 하나도 없는 경우 
    {
		MSG_ERROR("No List!",0,0,0);
		
		// Wavelet 보관함이 비었음을 알려주는 메시지 출력하고 리턴
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NTOP_FILE_EMPTY), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
		return;
    }
    else											// Wavelet 보관함에 컨텐츠가 있는 경우
    {
		// ppWaveletFileList의 메모리 할당
		for (index = 0; index < SKY_MAX_NUM_FILES; index++)	
		{
			ppWaveletFileList[index] = (BYTE*)SKY_Calloc(SKY_PATH_NAME_MAX_LENGTH,sizeof(BYTE));
			EQS_ASSERT(ppWaveletFileList[index] != NULL);
		}
		
		// ppWaveletFileList에 Wavelet 보관함에 있는 파일 리스트 이름과 그 개수를 가져옴
		nLines = SKY_GetFileList(SKY_WAVELET_DIR, ppWaveletFileList);

		if (SizeOrNot == TRUE)	// Content Name(Content Size) 형태의 Display가 필요한 경우
		{						// 다운로드 후 저장 공간이 모자라서 Wavelet 보관함의 컨텐츠를 삭제해야 할 경우 

			// ppWaveletFileList는 파일 리스트로 각 컨텐츠에 대해 4개의 파일이 존재하므로 4개의 동일한 이름을 가지는
			// 리스트가 된다. 각 커텐츠에 대해 한개의 리스트 요소만 가지도록 g_ppScrollList와 g_ppScrollList_Length에 
			// ppWaveletFileList를 4개마다 하나씩 건너 뛰면서 컨텐츠 이름을 넣어준다.
			// 그리고 파일 개수(nLines)가 아닌 컨텐츠 개수인 g_unNumOfWavelet도 For Loop을 돌면서 계산한다.
			for (index = 0; index < nLines; g_unNumOfWavelet++, index+=4 )
			{
				// g_ppScrollList와 g_ppScrollList_Length에 메모리를 할당
				g_ppScrollList[g_unNumOfWavelet] = (BYTE *)SKY_Calloc(LCD_COLUME_SIZE-1, sizeof(BYTE));
				g_ppScrollList_Length[g_unNumOfWavelet] = (BYTE *)SKY_Calloc(25+3, sizeof(BYTE));
				
				// 할당된 메모리에 컨텐츠 이름을 복사
				strcpy(g_ppScrollList[g_unNumOfWavelet], ppWaveletFileList[index]);
				strcpy(g_ppScrollList_Length[g_unNumOfWavelet], ppWaveletFileList[index]);

				// 컨텐츠 크기를 계산하기 위해 주어진 컨텐츠에 대해 파일 이름을 만들어줌
				strcpy(tFileNameHeader, g_ppScrollList[g_unNumOfWavelet]);
				strcpy(tFileNameVideo, g_ppScrollList[g_unNumOfWavelet]);
				strcpy(tFileNameAudio, g_ppScrollList[g_unNumOfWavelet]);
				strcpy(tFileNameText, g_ppScrollList[g_unNumOfWavelet]);
					
				strcat(tFileNameHeader, ".inf");
				strcat(tFileNameVideo, ".vdo");
				strcat(tFileNameAudio, ".ado");
				strcat(tFileNameText, ".cap");

				// 컨텐츠 크기 계산
				tSize = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameHeader)	
										+ (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameVideo)
										+ (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameAudio)
										+ (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameText);

				// 계산된 컨텐츠 사이즈(tSize)를 Kbyte단위로 환산하고 charater형으로 바꾸어 줌(Size)
				sprintf(Size,"%d",tSize>>10); 

				// 이름만 가진 리스트에 크기를 보여주는 부분을 붙여줌 : (ex) 엘라스틴(137Kbyte)
				strcat(g_ppScrollList_Length[g_unNumOfWavelet],"(");
				strcat(g_ppScrollList_Length[g_unNumOfWavelet],Size);
				strcat(g_ppScrollList_Length[g_unNumOfWavelet],"Kbyte)");
			}
			// Scroll List 구성
			sWindowSize = ( g_unNumOfWavelet > 6 ) ? 6 : g_unNumOfWavelet;
			g_hListHnd = EQC_ScrollList(5, 5, 18, sWindowSize,   g_ppScrollList_Length   , g_unNumOfWavelet, g_unNumOfWavelet, 0); 
		}
		else	// Content Name만 list에 Display할 경우 
		{		// Wavelet 보관함의 list를 Idle 메뉴상에서 보여줘야 할 경우
			for (index = 0; index < nLines; g_unNumOfWavelet++, index+=4 )
			{
				// g_ppScrollList에 메모리를 할당
				g_ppScrollList[g_unNumOfWavelet] = (BYTE *)SKY_Calloc(LCD_COLUME_SIZE-1, sizeof(BYTE));
				// 할당된 메모리에 컨텐츠 이름을 복사
				strcpy(g_ppScrollList[g_unNumOfWavelet], ppWaveletFileList[index]);			
			}
			// Scroll List 구성
			sWindowSize = ( g_unNumOfWavelet > 6 ) ? 6 : g_unNumOfWavelet;
			g_hListHnd = EQC_ScrollList(5, 5, 18, sWindowSize,   g_ppScrollList   , g_unNumOfWavelet, g_unNumOfWavelet, 0); 
		}
		
		// 스크롤 리스트의 한줄이 화면에 다 안 보이므로 TRUE로 설정하여 두면 
		// 한줄을 앞쪽과 뒤쪽으로 나누어 왔다 갔다 하면서 한줄을 다보이게 해준다.
		EQC_SetScrollFlowMode(g_hListHnd, TRUE);

		// 스크롤 리스트의 스타일을 지정
        EQC_SET_STYLE( g_hListHnd, EQC_GET_STYLE(g_hListHnd) |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_MAXIMIZE );
		// 스크롤 리스트를 액티브 
		EQS_SetIOCActive( g_hListHnd );
		
		// ppWaveletFileList의 메모리 반납
		for (index = 0; index < SKY_MAX_NUM_FILES; index++)
		{
			SKY_Free(ppWaveletFileList[index]);
		}
    }
}
/*============================================================================= 
Function:		MM_ReadContentInfo

Description:	Download 받을 Wavelet 컨텐츠의 종류를 CONNECT_CONTENTS, OLD_CONTENTS, NEW_CONTENTS
				중의 한 값으로 Return 해준는 함수로 Wap에서 call함
  
Input:			cpid	- Contents Provider ID
				ctsid	- Contenst ID
				cntName	- Contents Name

Output:			

Return value:	선택된 컨텐츠가 어떤 다운로드 타입인지 알려줌

Side effects:
===============================================================================*/
// 
BYTE MM_ReadContentInfo(int cpid, int ctsid, char *cntName)
{
    UINT index;

	// 같은 컨텐츠라면 우선 이름이 같을 것이므로 우선 컨텐츠 이름을 할당
	strcpy(szFileNameHeader, cntName);
    strcat(szFileNameHeader, ".inf");

	MSG_HIGH(" Read Content Info",0,0,0);
    
	if ((index = (UINT)SKY_GetNumFile(SKY_TEMPWAV_DIR)) != 0)	
	{		// Wavelet Temporary directory에 다운받다 말은 컨텐츠 있는 경우 이어받기용 컨텐츠 인지를 확인
		// Wap에서 다운로드를 위해 선택한 컨텐츠와 같은 이름을 가지는 지를 확인
		// 그 이름의 header가 열리면 우선 이름이 같은 컨텐츠임
        if (SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameHeader, &hFile_Header,SFT_READ) == SDR_OK)
        {
            // 현재 Wap의 List에서 선택한 컨텐츠와 임시 저장 공간에 다운받다 말은 컨텐츠가 같은 것인지
			// 좀더 확인하기 위해 임시저장공간에 있는  같은 이름의 컨텐츠 헤더 파일을 읽음
			SKY_ReadDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
			
			// Read 후 Header File Close
			if (hFile_Header)
			{
				SKY_CloseDataFile(hFile_Header);
				hFile_Header = NULL;
			}

			// 헤더에서 읽은 정보중 Centents Provider ID와 Content ID를 비교하여 같으면 
			// 같은 컨텐츠로 판단하여 이어받기의 대상이 되는 컨텐츠 임을 CONNECT_CONTENTS로 리턴하여 알려줌
			if ((gWaveletHeader.cpid == g_wcpid)&&(gWaveletHeader.ctsid == g_wctsid)) 
            {
				return CONNECT_CONTENTS;
            }
        }
	}
    else	// Wavelet Temporary directory에 다운받다 말은 컨텐츠가 없는 경우 새로받기용 컨텐츠 인지를 확인
	{
		// Wap에서 다운로드를 위해 선택한 컨텐츠와 같은 이름을 가지는 지를 확인
		// 그 이름의 header가 열리면 우선 이름이 같은 컨텐츠임
		if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_READ) == SDR_OK)
        {
            // 현재 Wap의 List에서 선택한 컨텐츠와 Wavelet 저장 공간에 이미 다운 받은 컨텐츠가 같은 것인지
			// 확인하기 위해 Wavelet 저장 공간에 있는 같은 이름의 컨텐츠 헤더 파일을 읽음
            SKY_ReadDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));

			// Read 후 Header File Close
			if (hFile_Header)
			{
				SKY_CloseDataFile(hFile_Header);
				hFile_Header = NULL;
			}

			// 헤더에서 읽은 정보중 Centents Provider ID와 Content ID를 비교하여 같으면 
			// 같은 컨텐츠로 판단하여 새로받기의 대상이 되는 임을 OLD_CONTENTS로 리턴하여 알려줌
			if ((gWaveletHeader.cpid == g_wcpid)&&(gWaveletHeader.ctsid == g_wctsid)) 
			{
				return OLD_CONTENTS; 
			}
	    }
	}

	// Open Error등으로 미처 닫히지 못했는지 확인하여 Header File Close
	if (hFile_Header)	
	{
		SKY_CloseDataFile(hFile_Header);		
		hFile_Header = NULL;
	}

	// 위의 경우가 아니라면 처음 다운받은 컨텐츠임을 NEW_CONTENTS로 리턴하여 알려줌
	return NEW_CONTENTS;
}

void MM_TimerDo(void) // 현재 쓰이지 않으나 timer가 더 필료할 때를 위해 만들어둔 Rex Timer의 동작
{
	MSG_HIGH(" Rex Timer! ",0,0,0);
	(ti[EQS_GENERAL_TIMER]).func(EQS_GENERAL_TIMER+TIMER_ID_CHANGE+WAV_GENERAL_TIMER,(ti[EQS_GENERAL_TIMER]).user);
}