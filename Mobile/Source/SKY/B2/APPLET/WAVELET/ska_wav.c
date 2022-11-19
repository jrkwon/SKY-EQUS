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
//////////////////////////////// Timer ���� //////////////////////////////////////////////////////////////////
#define TIMER_NUM				4  // �� ��� Timer ���� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// TCM Timer ID //////////////////////////////////////////////// yoursun 01/04/04 
#define TCM2_TIMER_ID1  92 // EQS_Timer1�� ����(1 = EQS_Timer1 = TCM2_TIMER_ID1 -TIMER_ID_CHANGE)
#define TCM2_TIMER_ID2	93 // EQS_Timer2�� ����(2 = EQS_Timer2 = TCM2_TIMER_ID2 -TIMER_ID_CHANGE)
#define TCM2_TIMER_ID3	94 // EQS_Timer3�� ����(3 = EQS_Timer3 = TCM2_TIMER_ID3 -TIMER_ID_CHANGE)
#define TCM2_TIMER_ID4  95 // General_Timer�� ���� <= Alive Message�� �����µ� ���ȴ�.
						   // (0 = General_Timer = TCM2_TIMER_ID3 -TIMER_ID_CHANGE - WAV_GENERAL_TIMER)
						   // (4 = WAV_GENERAL_TIMER = TCM2_TIMER_ID3 -TIMER_ID_CHANGE)					
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// UI Timer ID //////////////////////////////////////////////// yoursun 01/04/19
#define TIMER_ID_CHANGE		91 // UI�� Timer�� TCM�� Timer���� ��ȯ�� ���� �ʿ�
#define WAV_GENERAL_TIMER	4  // 95�� Timer�� General Timer�� �����ϱ� ���� �ʿ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************************/


/************************************************************************************************************/
//////////////////////////////// Font Size ////////////////////////////////////////////////////yoursun 01/04/07
#if SKY_MODEL_ID == SKY_MODEL_B1 // B1 Model�� ���� ��Ʈ ũ��
#define HANGUL_FONT_X 	12 // �ѱ���Ʈ�� �ʺ�(�ȼ�����)
#define HANGUL_FONT_Y 	13 // �ѱ���Ʈ�� ����(�ȼ�����) 
#define YOUNGMUN_FONT_X	6  // ������Ʈ�� �ʺ�(�ȼ�����)
#define YOUNGMUN_FONT_Y	13 // ���������� ����(�ȼ�����)

#elif (SKY_MODEL_ID == SKY_MODEL_B2) // B2 Model�� ���� ��Ʈ ũ��
#define HANGUL_FONT_X 	14 // �ѱ���Ʈ�� �ʺ�(�ȼ�����)
#define HANGUL_FONT_Y 	16 // �ѱ���Ʈ�� ����(�ȼ�����)
#define YOUNGMUN_FONT_X	 7 // ������Ʈ�� �ʺ�(�ȼ�����)
#define YOUNGMUN_FONT_Y	16 // ���������� ����(�ȼ�����)
#endif 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Display Size of Video ///////////////////////////////////////yoursun 01/04/10
#define DISPLAY_X		SKY_SCREEN_WIDTH	// LCD With
#define DISPLAY_Y		SKY_SCREEN_HEIGHT   // LCD Height
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Buffer Message ///////////////////////////////////////////// yoursun 01/03/20
#define WAVELET_BUFFER_MSG_XPOS 10	// �ٿ�ε峪 ���۸� ������ ǥ���ϴ� ���� ��ġ
#define WAVELET_BUFFER_MSG_YPOS 80  // �ٿ�ε峪 ���۸� ������ ǥ���ϴ� ���� ��ġ
#define BUFFER_MSG_LENGTH		22  // �ٿ�ε峪 ���۸� ������ ǥ���ϴ� �޽��� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
//////////////////////////////// TCM ERROR ///////////////////////////////////////////////////////////////////
// Error Code�� MM_GetLastError()�� ���� ���� ���� �˾ƿ´�.
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
//structure CONTENTS_INFO�� �Ҵ��� ����//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Available Audio Decoder ����		<= DWORD	AudioDecoder	
/* TMI Definition*/
#define TCM_AD_EVRC						0x0001
#define TCM_AD_QCELP					0x0002
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color Depth ����					<= DWORD	ColorDepth		
/* TMI Definition*/
#define	TCM_CD_256						0x0000
#define	TCM_CD_4						0x0001
#define	TCM_CD_NOT_DEFINED				0x0002
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Network Type ����				<= WORD	NetType 
/* TMI Definition*/
#define TCM_NT_2G						0x0100
#define TCM_NT_2_5G						0x0200
#define TCM_NT_3G						0x0300

#define TCM_NT_95AB						0x0001
#define TCM_NT_95C						0x0002
#define TCM_NT_NOT_DEFINED				0x0003
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_MIN_SIZE					16 // MIN Number�� �ִ� ����
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
   BYTE nTimerID;						// Timer ID ( TCM Timer ID �� �� �ϳ��� ����)
   TIMER_FUNC func;						// Timer Callback Function
   void* user;							// Extra Info( Not Yet Use)
} TIMER_ITEM;							// Timer�� ���� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
///////////////////////////////// UI Command ID //////////////////////////////////////////////////////////////
//MM_ProcessTCM2UICommand�� ���� UI�� Controller���� ���� �־�� �ϴ� Command��//
/* TMI Definition*/
typedef enum {
	TCM_UC_STREAMING	= 0x01,			// ��Ʈ���� ����
	TCM_UC_STOP			= 0x02,			// ���� ����
	TCM_UC_DOWNLOAD		= 0x03,			// �ٿ�ε� ����
	TCM_UC_PLAY			= 0x04,			// ��� ����
	TCM_UC_PAUSE		= 0x05,			// ��� �Ͻ� ���� 
	TCM_UC_RESUME		= 0x06,			// ��� �簳
	TCM_UC_TCP_FAIL		= 0x21			// TCP Fail
} TCM_UI_CMD ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Decoder Status //////////////////////////////////////////////////////////////
//MM_TCM2ServerStatus�� ���� Controller�� UI���� �˷��ִ� ���µ�//
/* TMI Definition*/
typedef enum {
	TCM_DS_DOWNLOAD_ENDED		= 0x11, // �ٿ�ε� �Ϸ�
	TCM_DS_STREAMING_ENDED		= 0x12, // ��Ʈ���� �Ϸ�
	TCM_DS_PLAY_ENDED			= 0x13, // ��� �Ϸ�
	TCM_DS_SETUP_STARTED		= 0x14, // Setup ����
	TCM_DS_DOWNLOAD_STARTED		= 0x15, // �ٿ�ε� ����
	TCM_DS_STREAMING_STARTED	= 0x16, // ��Ʈ���� ����
	TCM_DS_DECODER_ERROR		= 0x21, // ���ڴ� ���� �߻�
	TCM_DS_CONTROLLER_ERROR		= 0x22, // ��Ʈ�ѷ� ���� �߻�
	TCM_DS_SERVER_ERROR			= 0x23, // ���� ���� �߻�
	TCM_DS_SETUP_ERROR			= 0x24, // Setup ���� �߻�
	TCM_DS_CONTENTS_NOT_FOUND	= 0x25, // �������� ����
	TCM_DS_CONTENTS_SIZE		= 0x26  // �������� ����� �˷���
} TCM_DECODER_STATUS;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
////////////////////////////// Content Type //////////////////////////////////////////////////////////////////
/* TMI Definition*/
typedef enum {
	TCM_FT_TEXT		= 0x00,				// �ڸ� 
	TCM_FT_VIDEO	= 0x01,				// ����	
	TCM_FT_AUDIO	= 0x02,				// ����� 
	TCM_FT_SKIN		= 0x03				// ��Ų
} TCM_FILE_TYPE ; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Buffer Type ///////////////////////////////////////////////////////////////////
/* TMI Definition*/
typedef enum {
	TCM_MT_FRAME_BUFFER = 0x01,			// ������ ���� : ���ڵ��� �������� ���̴��� ���� ����
	TCM_MT_PACKET_BUFFER = 0x02			// ��Ŷ ���� : ��Ʈ���� �� UDP�� ���۵Ǿ� ���� �����͸� ���� ����
										//             �ٿ�ε� �ÿ��� �÷��� �� ��� �������� �ø��� �κ����� ���  
} TCM_MEM_TYPE ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/************************************************************************************************************/
////////////////////////////// 256 Color�̻��� Display Map ///////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	BYTE* m_pMap;					// �� Frame�� 8 By 8 Block���� ���� Map���� �����ؾ� �� Block�� TRUE�� �Ǿ� �ִ�.
	WORD m_Width;					// �� Frame�� width (pixel����)
	WORD m_Height;					// �� Frame�� height (pixel ����)
} COLORMAP, *PCOLORMAP;				// Color Frame�� ���� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// 256 Color�̻��� Display Data //////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	BYTE m_Type;
	BYTE* m_pFirst;					// 8 bit Y Data
	BYTE* m_pSecond;				// 8 bit U Data
	BYTE* m_pThird;					// 8 bit V Data
} COLORDATA, *PCOLORDATA;			// �������� Color Data�� 24bit YUV �����̴�.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// Skin Display Frame ///////////////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	unsigned char x;					// ��Ų Frame�� ���� x��ǥ 
	unsigned char y;					// ��Ų Frame�� ���� y��ǥ 
	unsigned char dx;					// ��Ų Frame�� width
	unsigned char dy;					// ��Ų Frame�� height
	unsigned char *pRGB;				// 24 bit RGB Pixel ���� (RGBRGBRGB...���·� 8bit�� ���� ����)		
} SKIN_FRAME, *PSKIN_FRAME;				// �ѹ� Display�� ��Ų Frame�� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// Skin Display Information /////////////////////////////////////////////////////
/* TMI Definition*/
typedef struct
{
	unsigned char	m_Width, m_Height;	// ��ü ��Ų�� width�� height
	unsigned char	m_Depth, m_padding;	// ��Ų�� Depth
	SKIN_FRAME		*pFrame;			// �ѹ� Display�� ��Ų Frame�� ����
} TCM2_SKIN, *PTCM2_SKIN;				// ��Ų ����
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
BOOL fConnectDN;			// �̾�ޱ� ���ο� ���� Flag(�ٿ�ε尡 �Ϸ�� ��쿡�� FALSE���� ������.)
int cpid;					// Contents Provider ID
int ctsid;					// Contents ID
char cntName[21];			// Contents Name
int cntProtect;				// Contents ��� ���� ȸ��
int cntExpDate;				// Contents ��� ���� �ϼ� 
DWORD dnDate;				// �ٿ�ε� �� ��¥(��� ���� �ϼ��� üũ�� ���� �ʿ�)
} wavelet_header;			// Wavelet Header File�� ����Ǿ�� �ϴ� Header Information
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
	INIT_STATUS = 0x00,			// �Ʒ��� ���µ��� �ƴ� �⺻ ����
	PHONE_CLOSE = 0x01,			// Phone Close Event�� ���� ����
	PPPOPEN_STATUS = 0x02,		// PPP Open�� �ϴ� ����(2G���� �ش�)
	SOCKCONNECT_STATUS = 0x03,	// Socket Open �� Call Connect ����
	CALL_DROP = 0x04			// Call Drop�� �� ����
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
///////////////////////// Wap Script�κ��� ���� Parameter ///////////////////////////////////////////////////////////////
extern int g_wcpid;				// Content Provider ID
extern int g_wctsid;			// Content ID
extern char g_wsndIP[101];		// IP
extern int g_wsndPort;			// Port
extern int g_wMO;				// MO
extern int g_wSO;				// SO
extern int g_wsvcType;			// ���� ����
extern int g_wskinType;			// ��Ų ����
extern char g_wcntName[21];		// Content �̸�
extern int g_wcntProtect;		// ��� ���� ȸ��
extern int g_wcntExpDate;		// ��� ���� ��¥
extern char g_wretURL[101];		// ���� URL 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
/************************************************************************************************************/
//////////////////////////////// �ٿ�ε� Type�� �˷��ִ� ���� ///////////////////////////////////////////////
BYTE gDownInfo = NEW_CONTENTS;	// �Ҵ簪�� ska_wav.h�� ���� �Ǿ� �ִ� ���� �� �߿� �ϳ��� ������.
								///////////////////////
								///	NEW_CONTENTS	///		
								///	OLD_CONTENTS	///
								///	CONNECT_CONTENTS///
								///////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// ������ ����/////////////////////////////////////////////////////////////////
DWORD gContents_Length;			// �ٿ�ε� �ޱ� ������ ���� �ٿ�ε� ���� ������ ����
								// �ٿ�ε� �Ϸ� �Ŀ��� �ٿ�ε� ���� ������ ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fDownload = TRUE;			// Download �������� Streaming ���������� �����ϴ� flag�� ���� ������ ����.
								////////////////////////
								/// TRUE  : Download ///
								/// FALSE : Streaming///
								////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fPause = FALSE;			// Pause �� ������� �ƴ��� ������� ���� flag�� ���� ������ ����.
								////////////////////////////
								/// TRUE  : Pause �� ���///
								/// FALSE : �ٸ� ���    ///
								////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// �������� ������� //////////////////////////////////////////////////////////
DWORD Video_length = 0;	// ���� 
DWORD Text_length = 0;	// �ڸ�
DWORD Skin_length = 0;	// ��Ų
DWORD Audio_length = 0;	// ����� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// �������� ���� ������ ///////////////////////////////////////////////////////
BYTE * pVdoData = NULL; // ����
BYTE * pTxtData = NULL; // �ڸ� 
BYTE * pSknData = NULL; // ��Ų 
BYTE * pAdoData = NULL;	// �����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// �������� �ѱ��� ////////////////////////////////////////////////////////////
DWORD tVideo_length = 0; // ���� 
DWORD tText_length = 0;	 // �ڸ� 
DWORD tSkin_length = 0;	 // ��Ų
DWORD tAudio_length = 0; // ����� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// �������� ���� ���� ����Ű�� ������ /////////////////////////////////////////
BYTE * ptVdoData = NULL; // ����
BYTE * ptTxtData = NULL; // �ڸ� 
BYTE * ptSknData = NULL; // ��Ų 
BYTE * ptAdoData = NULL; // ����� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BYTE g_bStride; // 1.MM_DisplayImage�� �ӽ� ������ ���� �Լ� ���ο��� ���Ǵ� SKY_UpdatePartialScreen�Լ��� 
				//	MM_DisplayImage�� parameter�� stride������ �������� �����Ƿ� SKY_UpdatePartialScreen�Լ� 
				//	���ο��� �� ���� Access�ϵ��� �ϱ� ���� ���� Global ����
				// 2.MM_DisplayImage�� 4Gray������ ����ϹǷ� 256 Color�̻��� �ܸ������� 
				//	�� �Լ��� ���θ� ��� �ξ ��
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
rex_timer_type  wavelet_alive_timer;	// Rex Timer�� ���� �������� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/



/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
/************************************************************************************************************/
////////////////////////////// Wavelet Header ������ ������ ���� /////////////////////////////////////////////
wavelet_header gWaveletHeader;			// "*.inf" ���Ͽ� ����Ǵ� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// ������ ������ ������ ���� /////////////////////////////////////////////////////
CONTENTS_INFO cData;					// Controller�� �ʱ�ȭ(MM_InitTCM2Controller(&cData))�ÿ� �ʿ��� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Applet���� Timer�� ó���ϱ� ���� ���� /////////////////////////////////////////
TIMER_ITEM ti[TIMER_NUM];				
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Wavelet ������ ������ ���� File Name //////////////////////////////////////////
BYTE  szFileNameHeader[25];				// ��� File Name	: "*.inf"
BYTE  szFileNameVideo[25];				// ���� File Name : "*.vdo"
BYTE  szFileNameAudio[25];				// ����� File Name : "*.ado"
BYTE  szFileNameText[25];				// �ڸ� File Name	: "*.txt"
BYTE  szFileNameSkin[25];				// ��Ų File Name	: "*.skn"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////// Wavelet ������ ������ ����  File Handle //////////////////////////////////////
HFILE hFile_Header = NULL;				// ��� File Handle		
HFILE hFile_Video = NULL;				// ���� File Handle	
HFILE hFile_Audio = NULL;				// ����� File Handle	
HFILE hFile_Text = NULL;				// �ڸ� File Handle		
HFILE hFile_Skin = NULL;				// ��Ų File Handle		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// ����� Volume ������ ���� Global Vatriable ///////////////////////////////////
UINT8 nWavVolume;						// ���� Speaker Volume�� ���� �����ϰ� ����
										// ������ : Volume ������ ��ȭ�� ũ�Ⱑ �ٲ�
										// Wavelet Volume���� �����ϴ� NV ���� �ʿ�
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fInitPlay = TRUE;					// Play�� �ʱ�ų�  End�� �� play �̸� TRUE 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fLongKey = FALSE;					// Long Key Command Line Display : TRUE(�ݺ����) , FALSE : ��� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL fDelete = FALSE;					// ������ ���� ������ ������ ��� ������ ���� ����Ʈ�� ���� ���� Flag
										// TRUE(����) , FALSE : ����	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BYTE WaveletStatus = INIT_STATUS;		// Wavelet Applet�� ���¸� ������ ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Sock_Timer = FALSE;				// PPP close �Ǵ� Open�� Would Block�Ǿ��� ��츦 ����� Timer Setting ���� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Idle Menu ����Ʈ�� ���� ������ //////////////////////////////////////////////
UINT g_unNumOfWavelet;					// Wavelet ������ ����Ʈ�� ���� 

//--------------------------- ������ �̸� ����Ʈ�� ������ ������ -------------------------------------------//
BYTE* g_ppScrollList[SKY_MAX_NUM_FILES] = {NULL,};		  // Idle Menu�󿡼� Wavelet �������� ������ ���, 
														  // ������ ����Ʈ �̸����� �����ִ� �� ���
//----------------------------------------------------------------------------------------------------------//
BYTE* g_ppScrollList_Length[SKY_MAX_NUM_FILES] = {NULL,}; // �ٿ�ε� & ���� ���񽺿��� ������ ��������� 
														  // ���ڶ�� ���, ����ڰ� ���� �������� ������ ���Ҷ�
														  // ������ ����� Wavelet ������ ����Ʈ�� �����ִµ� ���
                                                          // ����Ʈ�� ������ �� ������ �̸��� �Բ� ũ�⵵ ������
//----------------------------------------------------------------------------------------------------------//

HCONTROL g_hListHnd;					// Wavelet ����Ʈ ��Ʈ��
UINT g_unSelect;						// ����Ʈ �� ���õ� �������� ���� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
HCONTROL g_hWavControl;					// Wavelet �޽��� ����� ���� Control
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

Description:	Controller�� File�� �ݰ�, File Handle�� �ʱ�ȭ �Ѵ�.

Return value:	None

Side effects:	
===========================================================================*/
void sReleaseController(void)
{
	MM_CloseTCM2Controller();	// TMI API
	
	// File Close & File Handle Initialize
	if (hFile_Header)		// ���
	{
		SKY_CloseDataFile(hFile_Header);
		hFile_Header = NULL;
	}

	if (hFile_Video)		// ����
	{
		SKY_CloseDataFile(hFile_Video);
		hFile_Video = NULL;
	}
	
	if (hFile_Audio)		// �����
	{
		SKY_CloseDataFile(hFile_Audio);
		hFile_Audio = NULL;
	}
	
	if (hFile_Text)			// �ڸ�
	{
		SKY_CloseDataFile(hFile_Text);
		hFile_Text = NULL;
	}
	
	if (hFile_Skin)			// ��Ų
	{
		SKY_CloseDataFile(hFile_Skin);
		hFile_Skin = NULL;
	}
}

/*===========================================================================
Function:		sReleaseWavelet

Description:	Wavelet ���񽺰� �������� �����ϴ� Setting

Return value:	None

Side effects:	
===========================================================================*/
void sReleaseWavelet(void)
{
	SKY_SET_IN_WAVELET(FALSE); // Wavelet�� ����Ǵ��� ���θ� ����
}

/*===========================================================================
Function:		sWaveletReleaseSocket

Description:	�Ҵ�� Socket�� Release

Return value:	None

Side effects:	
===========================================================================*/
void sWaveletReleaseSocket(void)
{
	int result;

	// Socket Release
	result = MM_ReleaseSocket(); // (wav_multisock.c�� ����)

	if (result != WAV_SOCK_SUCCESS)	// socket�� release���� �ʾ��� ���(would block)
	{
		MSG_ERROR("Wavelet Release Would Block!",0,0,0);

		// Socket Release�� �ѹ� �� �ҷ� ��
		result = MM_ReleaseSocket();
		if (result != WAV_SOCK_SUCCESS) // �ι�° socket release ����
		{
			MSG_ERROR("Wavelet Release Socket Error!",0,0,0);
		}
		else // socket release ����
		{
			MSG_ERROR("Wavelet Release Socket Success!",0,0,0);
		}
	}
	else // socket release ����
	{
		MSG_ERROR("Wavelet Release Socket Success!",0,0,0);
	}
}

/*===========================================================================
Function:		sWaveletPPPClose

Description:	PPP ������ ����

Return value:	None

Side effects:	
===========================================================================*/
void sWaveletPPPClose(void)
{
	int result;
	
	// PPP Close
	result = MM_PPPClose(); // (wav_multisock.c�� ����)

	if (result == WAV_SOCK_SUCCESS)	// PPP Close ����
	{
		MSG_ERROR("Wavelet PPP Close Success",0,0,0);
		SKY_SET_SOCK_MODE(FALSE);
		EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA);	
	}
	else if (result == WAV_SOCK_WOULDBLOCK)	// PPP CLose Would Block 
	{
		MSG_ERROR("Wavelet PPP Close would block",0,0,0);
		// PPP Close�� �ǵ��� 60��¥�� Timer�� �ְ� ��ٸ�
		//////////////////////////////////////////////////////////////////////////////////////////
		// ���� EQS_TIMER_1�� CORE���� ������ �Ҵ��� �ִµ�, PPP Close�� ��	Would Block��		//
		// ��쿡 60�ʸ� �־� PPP Close�� �ǵ��� ��ٸ��µ� �̸� ����ϱ⵵ �Ѵ�.				//
		// Core���� ���� ���� PPP Close�� ���� ���� �� Timer�� �ؾ��ϴ� �۾��� �ٸ��Ƿ�		//
		// Sock_Timer��� Flag�� �ξ� TRUE�϶��� PPP Close�� ���� ������ �ϰ�,					//
		// FALSE�� ���� Core���� �ʿ�� �ϴ� ������ �ϵ��� �Ͽ���.								//
		//////////////////////////////////////////////////////////////////////////////////////////
		Sock_Timer = TRUE;	// PPP Close Timer Flag	Setting			
		EQS_StartTimer(EQS_TIMER_1, (DWORD)60*1000, FALSE); // PPP CLose ���� 60��¥�� Timer Setting	
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
    
Output:			�־��� ID�� Timer�� �����Ѵ�.
      
Return value:	none
        
Side effects:	none
===============================================================================*/
void MM_StartTimer( BYTE nTimerID, WORD nMilliSec, BOOL bRepeat, TIMER_FUNC pfn, void* user ) // yoursun 01/04/19
{
	BYTE t_nTimerID = nTimerID - TIMER_ID_CHANGE;


	if (t_nTimerID == WAV_GENERAL_TIMER) // Core Timer �� TCM2_TIMER_ID4(95) <= General Timer
	{
				MSG_ERROR("MM_StartTimer:ID %d, Intv %d, Rep: %d",nTimerID, nMilliSec, bRepeat);
		SKY_SetGeneralTimer((DWORD)(nMilliSec-250), bRepeat);				// General Timer Start
//		SKY_SetGeneralTimer((DWORD)4000, bRepeat);	
//		RestartWaveletAliveTimer(500);
		(ti[t_nTimerID-WAV_GENERAL_TIMER]).nTimerID = nTimerID;				// Timer ID �Ҵ�
		(ti[t_nTimerID-WAV_GENERAL_TIMER]).func = pfn;						// Callback Function �Ҵ�
		(ti[t_nTimerID-WAV_GENERAL_TIMER]).user = user;						// ���� ������ ������ �Ҵ�
	}
	else // ������ �� ���� Core Timer(TCM2_TIMER_ID1(92), TCM2_TIMER_ID2(93), TCM2_TIMER_ID3(94)) <= EQS_Timer����
	{
				MSG_ERROR("MM_StartTimer:ID %d, Intv %d, Rep: %d",nTimerID, nMilliSec, bRepeat);
		EQS_StartTimer((EQS_TIMER_E)t_nTimerID, (DWORD)nMilliSec, bRepeat);	// �ش� EQUS Timer Start
		(ti[t_nTimerID]).nTimerID = nTimerID;								// Timer ID �Ҵ�
	    (ti[t_nTimerID]).func = pfn;										// Callback Function �Ҵ�
		(ti[t_nTimerID]).user = user;										// ���� ������ ������ �Ҵ�
	}
}

/*============================================================================= 
Function:		MM_StopTimer

Description:	Stop Timer
    
Input:			nTimerID : Timer ID 
    
Output:			�־��� ID�� Timer�� �����.
            
Return value:	none	
              
Side effects:	none
===============================================================================*/
void MM_StopTimer(BYTE nTimerID)
{
	BYTE t_nTimerID = nTimerID - TIMER_ID_CHANGE;


	if (t_nTimerID == WAV_GENERAL_TIMER) // Core Timer �� TCM2_TIMER_ID4(95) <= General Timer
	{
		MSG_ERROR("MM_StopTimer : ID %d", nTimerID, 0, 0);
		SKY_SetGeneralTimer(FALSE, FALSE);				// General Timer Stop
//		CancelWaveletAliveTimer();
	}
	else // ������ �� ���� Core Timer(TCM2_TIMER_ID1(92), TCM2_TIMER_ID2(93), TCM2_TIMER_ID3(94)) <= EQS_Timer����
	{
		MSG_ERROR("MM_StopTimer : ID %d", nTimerID, 0, 0);
		EQS_StopTimer((EQS_TIMER_E)t_nTimerID);			// �ش� EQUS Tiemr Stop
	}
}

/*============================================================================= 
Function:		SKA_WaveletStreaming

Description: ��Ʈ���� ���� �ÿ� Wap Applet���κ��� �Ҹ��� ��Ʈ���� Applet
    
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
			/* PtP IrDA�� �ݾ��ֱ� ���� */
			if(!fIrDAPortClosed)
			{
				ChangeIrDAMode ( 0 );
				// Phone to PC IrDA�� off �Ǿ����� ǥ��
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
			
			SKY_SET_IN_WAVELET(TRUE);	// Wavelet�� ������� Setting

			EQC_StaticText(0, 0, (EQS_TOKEN)TKN_CALL_DS_WAVE_ORIG);

			SetKindOfService(WAVELET);	// ���� �ΰ� ���� �� Wavelet Service���� Setting

			fDownload = FALSE; // streaming���� Setting

			// MO=0x921
			if( P_REV_IS_6_OR_GREATER) // 2.5G�� ��� ( Wap���κ��� �׻� �� ��쿡�� ���ӵǾ�� �Ѵ�.)
			{	
				if (SKY_IS_WAP_TO_WAVELET())		// Wap���κ��� �� ������ Ȯ��
				{
					SKY_SET_WAP_TO_WAVELET(FALSE);	
					SKY_SET_SOCK_MODE(TRUE);		// PPP ���� �������� Setting
					
					// 2.5G�� ��� PPP�� ����Ǿ� ���Ƿ� SE_NET_CONNECTED �̺�Ʈ�� ���� ������ 
					EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);	
				}
			}
			else // 2G �� ��� (Wap���κ��� Streaming Applet���� �Ѿ� ���� �ȵȴ�.)
			{
				// Error Message ���				
				SKY_SET_WAP_TO_WAVELET(FALSE);
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				break;
			}
			break;

		case SE_NET_CONNECTED:	// PPP ������ �Ϸ�Ǿ��� �� �޴� �̺�Ʈ
			Sock_Timer = FALSE;	// PPP Open�� �Ǿ����Ƿ� Timer�� Core���� �ǵ��� FALSE�� �Ҵ�

			MM_InitSocket();	// Socket Initailize (wav_multisock.c�� ����)
			
			result = MM_OpenSocket();	// Socket Open (wav_multisock.c�� ����)
			if (result != WAV_SOCK_SUCCESS) 
			{
				MSG_ERROR("Socket Open ERROR",0,0,0);
				sWaveletReleaseSocket();	// Socket�� �Ҵ�Ǿ� ������ Release �ϰ� �ʱ�ȭ

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				break;
			}
			MSG_ERROR("Socket Open Success",0,0,0);

			result 	= MM_Connect(g_wsndIP, (WORD)g_wsndPort);	// TCP Connect (wav_multisock.c�� ����)	
			if (result != WAV_SOCK_SUCCESS) // TCP Connect�� ������ ���
			{
				MSG_ERROR("CONNECT ERROR",0,0,0);
				sWaveletReleaseSocket(); 
				
				// Error Message ���	
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				break;
			}
			// TCP Connect�� ������ ��� 
			MSG_ERROR("CONNECT Success",0,0,0);

			if (SKY_IS_IN_WAVELET())
			{
				MSG_HIGH("CONNECT_SUCCESS",0,0,0);
				//cData�� �ʱ�ȭ
				cData.CPID = g_wcpid;			// Contents Provider ID
				cData.ContentsID = g_wctsid;		// Contents ID 70 or 72
				cData.AudioDecoder = TCM_AD_EVRC;	// Available Audio Decoder
				cData.ColorDepth = TCM_CD_256;
				cData.SkinType = g_wskinType;		// SkinType
				cData.UDP_Port1 = UDP_PORT1; // yoursun 01/04/10
				cData.UDP_Port2 = UDP_PORT2; // yoursun 01/04/10
					//yoursun 01/06/08
				// Network type ����
				cData.NetType = TCM_NT_2_5G;//TCM_NT_3G
//				cData.NetType = cData.NetType << 8;
				cData.NetType = cData.NetType ;
				cData.NetType = cData.NetType | TCM_NT_95C;
				
				// Min�� �Ҵ�
				nv_num.nam = SKY_GET_CURRENT_NAM();
				(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_num );
				if (nv_num.dir_number[0] == '0') // 10�ڸ� ��ȣ�� ���
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11�ڸ� ��ȣ�� ��� (11.....�̷��� ����)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}
				
				
				// Controller Initialize ����
				if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
				{
					// Streaming ������ Core�� ���
					if (!MM_ProcessTCM2UICommand(TCM_UC_STREAMING, 0, 0 ))	// Streaming ������ ������ ���
					{
								MSG_HIGH("Controller Error",0,0,0);			
						sReleaseController();				
						sWaveletReleaseSocket();

						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
					}
				}
				else // Controller Initialize ����
				{
							MSG_HIGH("Controller Error",0,0,0);			
					sReleaseController();				
					sWaveletReleaseSocket();

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				}
			}
			else	// Wavelet ���񽺰� �ƴѰ�� 
			{
				sReleaseController();				
				sWaveletReleaseSocket();

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_NET_DISCONNECTED: // PPP ������ ����ٴ� �̺�Ʈ
			Sock_Timer = FALSE; // PPP close �� Timer�� ����� �������� ����
			EQS_StopTimer(EQS_TIMER_1);	// PPP close Would Block�� ����ϴ� Ÿ�̸� ����

			SKY_SET_SOCK_MODE(FALSE); // PPP Close�Ǿ����� ����
			MSG_HIGH("Net Disconnected Success!",0,0,0);

			// PPP�� ���� ���¿� ���� ���� ó���� �ٸ��Ƿ� �� ���¿� �°� ó����� ���
			switch (WaveletStatus)
			{
				case PHONE_CLOSE: // Phone close �̺�Ʈ�� PPP close�� ���
					MSG_HIGH("phone close!",0,0,0);
					SKY_SET_FROM_WAP(FALSE);
					SKY_SET_WAVELET_TO_WAP(FALSE);

					EQS_END_TO(EQS_POP_TO_ROOT);
				break;
				case INIT_STATUS: // Wavelet�� ������ ��Ȳ���� PPP close�� ���
					MSG_HIGH("init state",0,0,0);
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				case TCM_DS_CONTENTS_NOT_FOUND:	// ��Ʈ��ũ ���� �߻����� ���� PPP close�� ���
					MSG_HIGH("contents not found",0,0,0);
				case TCM_DS_CONTROLLER_ERROR:
					MSG_HIGH("controller error",0,0,0);
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				case CALL_DROP: // Call Drop���� ���� PPP close�� ���
					MSG_HIGH("Call Drop",0,0,0);
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				case TCM_DS_SETUP_ERROR: // �ھ� ���� �߻����� ���� PPP close�� ���
					MSG_HIGH("setup error",0,0,0);
				case TCM_DS_SERVER_ERROR:
					MSG_HIGH("server_error",0,0,0);									
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				default:
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
			}
			break;

		case SE_CALL_ENDED: // Call Drop ó���� ���� Call Drop�� �Ͼ call End�� ��츸 ó��
			if ((SKY_GetCallEndStatus() == CM_CALL_END_NO_SRV)||(SKY_GetCallEndStatus() == CM_CALL_END_FADE))
			 {
				// call drop �������� ����
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
			// Error Message ���
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

			// Error Message ���
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

	    case SE_KEY_PHONE_CLOSE: // Phone close�� �Ͼ ���
			// Phone Close �������� ����
			WaveletStatus = PHONE_CLOSE; 
			
			// �ϴ� �۾��� ����
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

		case SE_MUXNEGO_SUCCESS: // Mux Option Nego�� ����
			MSG_HIGH("SE_MUXNEGO_SUCCESS",0,0,0);
			if (SKY_IS_IN_WAVELET())
			{
				sReleaseWavelet();
			}
			EQS_START_SIBLING(SKA_MainWAP, FALSE); // Wap���� ���ư�
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
				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_GOTO_NTOP), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_4)
			{
				// Error Message ���
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
			else //	Sock_Timer = TRUE;	<= PPP Close Would Block�� ���
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

		case SE_PH_RSSI: // ȭ�� ���������� ���� �� ȭ�� ����� ����
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		sWaveletSoftKey

Description:	Wavelet �������� ��� �� ��� Ű�� ������ ����� �����ϴ� local �Լ� 
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
LOCAL void 	sWaveletSoftKey()
{
	MSG_ERROR("Play Button ",0,0,0);
	WaveletStatus = INIT_STATUS;

	if (fInitPlay == TRUE)	// �ʱ� ��� �Ǵ� ��� ���� �� ��� Ű�� ���� ��� 
	{						// (Controller�� Init�� �ʿ��� ���)
		fInitPlay = FALSE;
		fLongKey = FALSE;		
		if (MM_InitTCM2Controller(&cData) == TRUE)		// yoursun 01/04/07
		{
			if (g_wsvcType != SVC_DOWNLOAD_STORE)		// Donwload & play
				EQS_SetWindowCML(CL_PLAY_STOP_END);
			else										// Download & store
				EQS_SetWindowCML(CL_PLAY_STOP_SAVE);

			// Core�� ����� �����϶�� �϶�� �˸�
			if (MM_ProcessTCM2UICommand(TCM_UC_PLAY, 1, 0)) // ���������� ����� ���۵� ���
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// ��� ������ ������ ��� 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);			
				// Error �޽��� ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
		}
		else // Controller Init�� ������ ���
		{
			MSG_HIGH("Controller Error",0,0,0);	
			if (g_wsvcType == SVC_IDLE)	// Wavelet �����Կ� �ִ� �������� ���
			{
				// ���ҵ� �÷��� ���� ȸ���� Write;
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
			// Error �޽��� ���
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
			EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
		}
	}
	else	// �Ͻ����� �� ��� Ű�� ���� ��� �Ǵ� ��� ���� ��� Ű�� ���� ���
	{ 
		if (fPause == TRUE) // �Ͻ� ���� �� ��� �簳
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

			// Core�� Resume�϶�� �˸�
			if (MM_ProcessTCM2UICommand(TCM_UC_RESUME, 0, 0)) // ���������� Resume�� ���
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// Resume�� ������ ��� 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);			
				// Error �޽��� ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}

			fPause = FALSE;
		}
		else // �ݺ� ��� �Ǵ� ��� ���� ��� Ű�� ���� ���
		{
			if (fLongKey)	// �ݺ� ��� ���� ��� Ű�� ���� ��� 
			{				// �ݺ� ����� ������ ���� ����� �����Ѵ�.
				fLongKey = FALSE;
				if (g_wsvcType != SVC_DOWNLOAD_STORE)		// Download & play
					EQS_SetWindowCML(CL_PLAY_STOP_END);
				else										// Download & store
					EQS_SetWindowCML(CL_PLAY_STOP_SAVE);

				// Core���� �ݺ� ����� �����϶�� �˸�
				if (MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0))	// ���������� �ݺ� ����� ������ ���
				{
					MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
				}
				else											// �ݺ� ����� ���ᰡ ������ ��� 
				{
					MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
					MSG_HIGH("Controller Error",0,0,0);			
					// Error �޽��� ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}

				//Controller�� ������
				MM_CloseTCM2Controller();

				// Controller�� �ʱ�ȭ��
				if (MM_InitTCM2Controller(&cData) == TRUE)	// Controller �ʱ�ȭ�� ���� yoursun 01/04/07
				{
					// ����, �����, �ؽ�Ʈ�� ���̿� �����͸� �ʱ� ������ ���� �Ҵ���
					Video_length = tVideo_length;
					Text_length = tText_length;
					Audio_length = tAudio_length;
					pVdoData = ptVdoData;
					pTxtData = ptTxtData;
					pAdoData = ptAdoData;

					if (g_wsvcType != SVC_IDLE)	// Download �� ����� ��� 
					{							// ��Ų�� ���ؼ��� ���̿� �����͸� �ʱⰪ���� �Ҵ���
						Skin_length = tSkin_length;
						pSknData = ptSknData;
					}

					// Core�� ����� �����϶�� �϶�� �˸�
					if (MM_ProcessTCM2UICommand(TCM_UC_PLAY, 1, 0)) // ���������� ����� ���۵� ���
					{
						MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
					}
					else											// ��� ������ ������ ��� 
					{
						MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
						MSG_HIGH("Controller Error",0,0,0);			
						// Error �޽��� ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
				}
				else									// Controller �ʱ�ȭ�� ����
				{
					MSG_HIGH("Controller Error",0,0,0);			
					if (g_wsvcType == SVC_IDLE) // Wavelet �����Կ� �ִ� �������� ���
					{
						// ���ҵ� �÷��� ���� ȸ���� Write;
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
					// Error �޽��� ���
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

Description:	Wavelet �������� ��� �� �ݺ� ��� Ű�� ������ ����� �����ϴ� local �Լ�
    
Input:
    
Output:
      
Return value:
        
Side effects:
===============================================================================*/
LOCAL void 	sWaveleF1Long()
{
	MSG_ERROR("Re-Play Button ",0,0,0);
	WaveletStatus = INIT_STATUS;

	if (fInitPlay == TRUE)	// �ʱ� �ݺ� ��� �Ǵ� ��� ���� �� �ݺ� ��� Ű�� ���� ���
	{						// (Controller�� Init�� �ʿ��� ���)
		fLongKey = TRUE;	
		fInitPlay = FALSE;	
		if (MM_InitTCM2Controller(&cData) == TRUE)		// yoursun 01/04/07
		{
				MSG_ERROR("Play Repeat Initialization SUCCESS! ",0,0,0);
			if (g_wsvcType != SVC_DOWNLOAD_STORE)	// Download & play
				EQS_SetWindowCML(CL_REPLY_STOP_END);
			else									// Donwload & Store
				EQS_SetWindowCML(CL_REPLY_STOP_SAVE);
			
			// Core�� �ݺ� ����� �����϶�� �˸�
			if (MM_ProcessTCM2UICommand(TCM_UC_PLAY, 0, 0)) // ���������� �ݺ���� ���۵� ���
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// �ݺ���� ������ ������ ��� 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);		
				// Error �޽��� ���				
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
		}
		else // Controller Init�� ������ ���
		{
			if (g_wsvcType == SVC_IDLE)
			{
				// ���ҵ� �÷��� ���� ȸ���� Write
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
			// Error �޽��� ���
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
			EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
		}
	}
	else // �Ͻ����� �� �ݺ� ��� Ű�� ���� ��� �Ǵ� ��� ���� �ݺ� ��� Ű�� ���� ��� 
	{
		if (fPause == TRUE) // �Ͻ� ���� �� �ݺ� ��� �簳
		{
			if (g_wsvcType != SVC_DOWNLOAD_STORE)	// Download & play
				EQS_SetWindowCML(CL_REPLY_STOP_END);
			else									// Donwload & Store
				EQS_SetWindowCML(CL_REPLY_STOP_SAVE);

			fLongKey = TRUE;

			// Core�� Resume�϶�� �˸�
			if (MM_ProcessTCM2UICommand(TCM_UC_RESUME, 0, 0)) // ���������� Resume�� ���
			{
				MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! %d ",g_wsvcType,0,0);
			}
			else											// Resume�� ������ ��� 
			{
				MSG_ERROR("MM_ProcessTCM2UICommand Error %d ",MM_GetLastError(),0,0);
				MSG_HIGH("Controller Error",0,0,0);			
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}

			fPause = FALSE;
		}
		else  // �ݺ� ��� �Ǵ� ��� ���� �ݺ� ��� Ű�� ���� ���
		{
			fLongKey = TRUE;						// �ݺ� ��� ���� Setting

			if (g_wsvcType != SVC_DOWNLOAD_STORE)	// Download & play
				EQS_SetWindowCML(CL_REPLY_STOP_END);
			else									// Donwload & Store
				EQS_SetWindowCML(CL_REPLY_STOP_SAVE);
		} 
	}
}

/*============================================================================= 
Function:		sSKA_IdleWaveletPlay

Description:	Wavelet ������ ���� �������� ��� Applet
    
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

			if (fPause && SKY_IS_TO_WAVELET()) // ska_call.c���� �� ���(Calló�� �ϰ� �ٽ� �� ���)
				// �⺻���� annunciator�� Command Line�� �׸� �Ŀ� Wavelet Skin�� �׸��� ���� �߰��� Event�� ��
				EQS_SEND_EVENT( SE_USER_1 , EQS_NA, EQS_NA);
			else  // �ٸ� ���ø����� ���� ���� ��� (call)
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
				if (nv_num.dir_number[0] == '0') // 10�ڸ� ��ȣ�� ���
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11�ڸ� ��ȣ�� ��� (11.....�̷��� ����)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}

						MSG_HIGH("MIN �Ҵ�",0,0,0);
					
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

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);		
				}
					
								MSG_HIGH("Count %d",gWaveletHeader.cntProtect,0,0);	
				// ��� ���� ȸ��üũ 
				if (gWaveletHeader.cntProtect == 0x00)	// ��� ���� ȸ���� "0" => ��� �Ұ�
				{
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_PROTECTED), 2, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
					return;
				}
				else if (gWaveletHeader.cntProtect != 0xff)	// ��� ���� ȸ���� 0xff�̸� ��� ������ �����Ƿ� �׳� ��
				{
					gWaveletHeader.cntProtect--;	// ��� ���� ȸ���� 0�̳� 0xff�� �ƴϸ� ��� ���� ȸ���� �ٿ���
				}
				
				// ��� ���� ��¥�� Ȯ�� �ϱ� ���� ������� ��¥�� �ʴ����� ȯ����
				ResultDate = 86400*gWaveletHeader.cntExpDate;	

				// ��� ���� ��¥�� ������ ���� 0xff�̰ų� ��� ���� ��¥�� �� ���� ��� 
				if ((gWaveletHeader.cntExpDate == 0xfF) || 
				((SKY_GetLocalTime()-gWaveletHeader.dnDate) <= ResultDate))
				{
					// ������ ���� �̸� ����
					strcpy(szFileNameVideo,g_wcntName);
					strcpy(szFileNameAudio,g_wcntName);
					strcpy(szFileNameText,g_wcntName);
						
					strcat(szFileNameVideo,".vdo");
					strcat(szFileNameAudio,".ado");
					strcat(szFileNameText,".cap");

					// ������ ������ ����
					if ((SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameVideo, &hFile_Video, SFT_READ) == SDR_OK)
					&&(SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameAudio, &hFile_Audio, SFT_READ) == SDR_OK)
					&&(SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameText, &hFile_Text, SFT_READ) == SDR_OK))
					{
						// ������ ���� �� ������ ������ ������ �ʱ�ȭ
						tAudio_length = Audio_length = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, (BYTE*)szFileNameAudio);						
						tVideo_length = Video_length = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, (BYTE*)szFileNameVideo);
						tText_length = Text_length = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, (BYTE*)szFileNameText);
						ptVdoData = pVdoData = (BYTE *)(Image$$APP_SHARE_RAM$$Base+100000);		// yoursun 01/06/13
						ptTxtData = pTxtData = pVdoData + Video_length;					
						ptAdoData = pAdoData = pTxtData + Text_length;	
						// ���� �������� Share RAM�� �ø�
						if (Video_length != (DWORD)SKY_ReadDataBlock(hFile_Video, pVdoData, Video_length))
						{
							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
						}
						if (hFile_Video) 
						{
							SKY_CloseDataFile(hFile_Video);
							hFile_Video = NULL;			
						}
						
						// �ڸ� �������� Share RAM�� �ø�
						if (Text_length != (DWORD)SKY_ReadDataBlock(hFile_Text, pTxtData, Text_length))
						{
							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						if (hFile_Text) 
						{
							SKY_CloseDataFile(hFile_Text);
							hFile_Text = NULL;			
						}

						// ����� �������� Share RAM�� �ø�	
						if (Audio_length != (DWORD)SKY_ReadDataBlock(hFile_Audio, pAdoData, Audio_length))
						{
							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						if (hFile_Audio)
						{
							SKY_CloseDataFile(hFile_Audio);
							hFile_Audio = NULL;			
						}

						// Controller Initialize ����
						if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
						{
							MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! ",0,0,0);
							// ����� �����ϰ� �ϵ��� �̺�Ʈ�� �ҷ���
							EQS_SEND_EVENT(SE_SOFTKEY_MENU, EQS_NA, EQS_NA);
						}
						else // Controller Initialize ����
						{
													MSG_HIGH("Controller Error",0,0,0);			
							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
						}
					}
					else // ������ ���� ���Ⱑ ������ ��� 
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

						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
					}
				}
				else	// ��� ���� ��¥�� ���� ��� 
				{
					// Error Message ���
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
			// ��� ������ Core�� ���
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
			
			// Error Message ���
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_SOFTKEY_REPLY : // �ݺ���� ��ư 
		case SE_SOFTKEY_PLAY : // ��� ��ư 
		    sWaveletSoftKey();
			break;

		case SE_KEY_F1_LONG : // �ݺ� ��� ��ư
			sWaveleF1Long();
			break;

		case SE_SOFTKEY_STOP : // �Ͻ� ���� ��ư
			if (fLongKey) 		EQS_SetWindowCML(CL_REPLY_NONE_END);
			else EQS_SetWindowCML(CL_PLAY_NONE_END);
			fPause = TRUE;
			MM_ProcessTCM2UICommand(TCM_UC_PAUSE, 0, 0);
			MSG_ERROR("Stop Button ",0,0,0);
		    break;

		case SE_SOFTKEY_END : // ����  Ű 
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			// Error Message ���
		    g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_KEY_PHONE_CLOSE:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			// ���ҵ� �÷��� ���� ȸ���� Write;
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

			// Error Message ���
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
		    break;

		case SE_SYS_TO_IDLE : // very important 
		case SE_SYS_OFFLINE:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			// Error Message ���
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
				// ���ҵ� �÷��� ���� ȸ���� Write;
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
				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_3) 		
			{
				// ���ҵ� �÷��� ���� ȸ���� Write;
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

		case SE_PH_RSSI: // ȭ�� ���������� ���� �� ȭ�� ����� ����
			break;

	    default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		SKA_IdleWavelet

Description:	Wavelet ������ ���� �������� ����Ʈ�� �����ְ� ������ ����� 
				�����ϰ� ���ִ� Applet
    
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
			/* PtP IrDA�� �ݾ��ֱ� ���� */
			if(!fIrDAPortClosed)
			{
				ChangeIrDAMode ( 0 );
				// Phone to PC IrDA�� off �Ǿ����� ǥ��
				fIrDAPortClosed = TRUE;
				g_fIrDAPortForPC = FALSE;
			}
			if (SKY_IS_TO_WAVELET()) // ska_call.c���� �� ���(Calló�� �ϰ� �ٽ� �� ���)
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

			MM_WaveletListDisplay(FALSE); // Wavelet �������� List�� ������
			g_wsvcType = SVC_IDLE;
			break;

		case SE_APP_END :   /* The very last event */ 
			EQS_ReleaseDC();
			for (i = 0; i <= g_unNumOfWavelet; i++)	// scroll list�� �޸𸮸� ����
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

		case SE_IO_SCROLL_LIST_SELECT:	// ��ũ�� ����Ʈ�� ���õ� ��� 
			g_unSelect = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
			// ���õ� Wavelet�������� ����ϴ� child Applet�� �θ�
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
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NTOP_FILE_EMPTY), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);				
				}
			}
			break;

		case SE_IO_MESGBOX_YES:	// ����ڰ� ������ ������ ������ ��� 
			MSG_HIGH (" Delet Yes! ",0,0,0);

			unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
			strcpy(delFileName,g_ppScrollList[unIdx]);

			// ���� ������ �̸��� ����
			strcpy(szFileNameHeader, delFileName);
			strcpy(szFileNameVideo, delFileName);
			strcpy(szFileNameAudio, delFileName);
			strcpy(szFileNameText, delFileName);
			
			strcat(szFileNameHeader, ".inf");
			strcat(szFileNameVideo, ".vdo");
			strcat(szFileNameAudio, ".ado");
			strcat(szFileNameText, ".cap");

			// ������ ������ ����
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameHeader);
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameVideo);
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameAudio);
			SKY_DeleteDataFile(SDF_WAVELET, szFileNameText);

			MSG_HIGH (" Delet Complete! ",0,0,0);
			
			// ������ ���� ���� �� ��ũ�� ����Ʈ�� ����
            EQC_DeleteScrollData(EQS_GetNthIOC(0), unIdx);
			RemoveListItem(g_hListHnd, unIdx);

			for( i=unIdx; i<SKY_MAX_NUM_FILES-1; i++)
			{
				g_ppScrollList[i] = g_ppScrollList[i+1];
			}
			SKY_Free(g_ppScrollList[i]);

			g_unNumOfWavelet = g_unNumOfWavelet - 1;

			// Error Message ���
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

		case SE_PH_RSSI: // ȭ�� ���������� ���� �� ȭ�� ����� ����
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

			// Error Message ���
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
			// Error Message ���
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;

		case SE_SOFTKEY_CANCEL:
		case SE_IO_SCROLL_LIST_CANCEL:
			sReleaseController();
			SKY_DeleteDataFiles(SDF_TEMPWAV);

		    // Error Message ���
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

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE_DONE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
				else // ����Ʈ �����ÿ��� ���� ������ ���ڶ�� ��� 
				{
					MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);
					
					// Error Message ���
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

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_2)
			{
				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);				
			}
			else if( pEvent->dwParam == WAV_MES_5)
			{
				sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_PH_RSSI: // ȭ�� ���������� ���� �� ȭ�� ����� ����
			break;

		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

//#define RENAME // rename�� ����ϸ� fs_dir.c�� 4052���� ����
LOCAL void 	sWaveletContentsSave()	// ������ �������� Wavelet ���� ���丮�� �����ϴ� �Լ�
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

		// Error Message ���
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
	// Wavelet ���� ������ ����
	if (SKY_CreateDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header) == SDR_OK)
	{
		SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
	}
	else // ��� ���� ������ ������ ���
	{
		if (hFile_Header)
		{
			SKY_CloseDataFile(hFile_Header);
			hFile_Header = NULL;
		}

		// Error Message ���
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}

	// ���� ���� ���� �� ����
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

		// Error Message ���
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}
	
	// ����� ���� ���� �� ����
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

		// Error Message ���
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}

	// �ڸ� ���� ���� �� ����
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

		// Error Message ���
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
	}

	sReleaseController();
	SKY_DeleteDataFiles(SDF_TEMPWAV);
#endif
	fDelete = FALSE;

	// Error Message ���
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
			
			if (fPause && SKY_IS_TO_WAVELET()) // ska_call.c���� �� ���(Calló�� �ϰ� �ٽ� �� ���)
				// �⺻���� annunciator�� Command Line�� �׸� �Ŀ� Wavelet Skin�� �׸��� ���� �߰��� Event�� ��
				EQS_SEND_EVENT( SE_USER_1 , EQS_NA, EQS_NA); 
			else  // �ٸ� ���ø�(call)���� ���� ���� ��� 	
			{					
				WaveletStatus = INIT_STATUS;
				// Flag Setting
				fPause = FALSE; 
				fLongKey = FALSE;
				fInitPlay = TRUE;	
				
				SKY_SET_TO_WAVELET(FALSE);
				SKY_SET_FROM_WAVELET(FALSE);
				
				// cData���� ����
				cData.CPID = 0;			// Contents Provider ID
				cData.ContentsID = 0;		// Contents ID
				cData.AudioDecoder = TCM_AD_EVRC;	// Available Audio Decoder
				cData.ColorDepth = TCM_CD_256;
				cData.SkinType = g_wskinType;		// SkinType
				cData.UDP_Port1 = 0; 
				cData.UDP_Port2 = 0; 
				cData.NetType = 0;
				// Min�� ����
				nv_num.nam = SKY_GET_CURRENT_NAM();
				(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_num );
				if (nv_num.dir_number[0] == '0') // 10�ڸ� ��ȣ�� ���
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11�ڸ� ��ȣ�� ��� (11.....�̷��� ����)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}
				
				// ������ ���ϵ��� �̸� ����
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

				// ������ ������ ����
				if ((SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameVideo, &hFile_Video, SFT_READ) == SDR_OK)
				&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameAudio, &hFile_Audio, SFT_READ) == SDR_OK)
				&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameText, &hFile_Text, SFT_READ) == SDR_OK)
				&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameSkin, &hFile_Skin, SFT_READ) == SDR_OK))
				{
					// ������ ���� �� ������ ������ ������ �ʱ�ȭ 
					tVideo_length = Video_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameVideo);
					tAudio_length = Audio_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameAudio);
					tText_length = Text_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameText);
					tSkin_length = Skin_length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, (BYTE*)szFileNameSkin);

					ptVdoData = pVdoData = (BYTE *)(Image$$APP_SHARE_RAM$$Base+100000);		// yoursun 01/06/13
					ptSknData = pSknData = pVdoData + Video_length;
					ptTxtData = pTxtData = pSknData + Skin_length;
					ptAdoData = pAdoData = pTxtData + Text_length;
					
					// ���� �������� Share RAM�� �ø� 
					if (Video_length != (DWORD)SKY_ReadDataBlock(hFile_Video, pVdoData, Video_length))
					{
						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);							
					}
					if (hFile_Video) 
					{
						SKY_CloseDataFile(hFile_Video);
						hFile_Video = NULL;
					}

					// �ڸ� �������� Share RAM�� �ø� 
					if (Text_length != (DWORD)SKY_ReadDataBlock(hFile_Text, pTxtData, Text_length))
					{
						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
					if (hFile_Text) 
					{
						SKY_CloseDataFile(hFile_Text);						
						hFile_Text = NULL;
					}
					// ��Ų �������� Share RAM�� �ø� 
					if (Skin_length != (DWORD)SKY_ReadDataBlock(hFile_Skin, pSknData, Skin_length))
					{
						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
					if (hFile_Skin) 
					{
						SKY_CloseDataFile(hFile_Skin);						
						hFile_Skin = NULL;
					}

					// ����� �������� Share RAM�� �ø� 
					if (Audio_length != (DWORD)SKY_ReadDataBlock(hFile_Audio, pAdoData, Audio_length))
					{
						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR), 2, MBT_NONE, MBT_SUCCESS);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
					if (hFile_Audio) 
					{
						SKY_CloseDataFile(hFile_Audio);
						hFile_Audio = NULL;
					}

					// Controller Initialize ����
					if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
					{
						MSG_ERROR("MM_ProcessTCM2UICommand SUCCESS! ",0,0,0);
						// ����� �����ϰ� �ϵ��� �̺�Ʈ�� �ҷ���
						EQS_SEND_EVENT(SE_SOFTKEY_MENU, EQS_NA, EQS_NA);
					}
					else // Controller Initialize ����
					{
									MSG_HIGH("Controller Error",0,0,0);			

						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					}
				}
				else //������ ���� ���Ⱑ ������ ���
				{
										MSG_HIGH("File Open Fail",0,0,0);			
					// ���� �ݰ� �ڵ鷯 �ʱ�ȭ
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

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
			}
			break;

		case SE_SOFTKEY_MENU:
			// ��� ������ Core�� ���
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
			// Backlignt ���� ����
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

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
		    else // SVC_DOWNLOAD_STORE // Download & Store
			{
				MM_ProcessTCM2UICommand(TCM_UC_PAUSE, 0, 0);
				MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
				MM_CloseTCM2Controller();

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVE_OR_NOT), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_SOFTKEY_REPLY : // �ݺ���� ��ư 
		case SE_SOFTKEY_PLAY : // ��� ��ư 
			sWaveletSoftKey();
		    break;

		case SE_KEY_F1_LONG : // �ݺ� ��� ��ư
			sWaveleF1Long();
			break;

		case SE_SOFTKEY_STOP : // �Ͻ� ���� ��ư
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

		case SE_SOFTKEY_SAVE : // ����  Ű 
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			MM_CloseTCM2Controller();

				MSG_ERROR("Save Button ",0,0,0);

			// Error Message ���
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVE_OR_NOT), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
		    break;

		case SE_SOFTKEY_END : // ����  Ű 
				MSG_ERROR("End Button ",0,0,0);
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0);
			sReleaseController();

			SKY_DeleteDataFiles(SDF_TEMPWAV);

		    // Error Message ���
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

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVE_OR_NOT), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else 
			{
				sReleaseController();
				SKY_DeleteDataFiles(SDF_TEMPWAV);

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT), 2, MBT_NONE, MBT_HELP);
		        EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
			    EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_SYS_TO_IDLE : /* very important */
		case SE_SYS_OFFLINE:
			sReleaseController();
			SKY_DeleteDataFiles(SDF_TEMPWAV);

		    // Error Message ���
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;


	    case SE_CALL_INCOMING:	//��ȭ �� ���
			{
				// Wavelet ����� ���߷� Call ó�� �κ����� ��
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
				// Error Message ���
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
				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
                EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			} 
			else if (pEvent->dwParam == WAV_MES_6)
			{
				sWaveletContentsSave();	// ������ ����
			}
			break;

		case SE_IO_MESGBOX_YES:
			if( pEvent->dwParam == WAV_MES_4)	// ����ڰ� ������ ������ ������ ���
			{
			    MSG_HIGH (" Store Yes! ",0,0,0);

				if (SKY_IsDataFile(SDF_WAVELET,(BYTE*)szFileNameHeader))	// ���� �ޱ��� ��� 
				{ //���� ���� ���� �̸��� �������� ����
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader);
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameVideo);
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameAudio);
					SKY_DeleteDataFile(SDF_WAVELET, (BYTE*)szFileNameText);

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SAVING),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_6);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
				else // ���ιޱⰡ �ƴ� ���
				{  
					// ������ Space�� ���� ���
					if ((SKY_GetFreeSpaceInDataFile() < gContents_Length)||((SKY_GetFileNumberInDataFile(SDF_WAVELET)+4) >= SKY_MAX_NUM_FILES )) 
					{
						MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);
						if((UINT)SKY_GetNumFile(SKY_WAVELET_DIR) == 0)
						{
							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_DELETE_OTHER_CONTENTS), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
						}
						else
						{
    						// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_OLD_CONTENTS_DELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_5);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
					}
					else // ������ Space�� ���� ���
					{
						MSG_HIGH("Size %d",SKY_GetFreeSpaceInDataFile(),0,0);						
						
						// Error Message ���
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

				// Error Message ���
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

				// Error Message ���
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

		case SE_PH_RSSI: // ȭ�� ���������� ���� �� ȭ�� ����� ����
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
			/* PtP IrDA�� �ݾ��ֱ� ���� */
			if(!fIrDAPortClosed)
			{
				ChangeIrDAMode ( 0 );
				// Phone to PC IrDA�� off �Ǿ����� ǥ��
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
			
			SKY_ReinitMemory();		// Wap�� UI�ʿ� �Ҵ�� Stack�� �ٸ���, Wap���� �� ��� Wap Stack�� ����ϰ� �ǹǷ�
									// �Ҵ��� ������ �����ϰ� �Ǿ� ���� �״� ������ �߻��ϹǷ� UI Stack�� �ʱ�ȭ �Ѵ�.

			EQC_StaticText(0, 0, (EQS_TOKEN)TKN_CALL_DS_WAVE_ORIG);

			SetKindOfService(WAVELET);	// Socket���� Wavelet �������� ����

			fDownload = TRUE;			//  �ٿ�ε� �������� ����

			// MO=0x921
			if( P_REV_IS_6_OR_GREATER) // 2.5G (PPP�� ����� ���¿��� Wap���κ��� �Ѿ��)
			{	
				if (SKY_IS_WAP_TO_WAVELET())	
				{	// ���� Wap���� �Ҹ��� ��츸 �����Ƿ� Wap���κ��� �Ҹ������� Ȯ��.
					SKY_SET_WAP_TO_WAVELET(FALSE);
					SKY_SET_SOCK_MODE(TRUE);	// PPP Open �������� ����
					// Controller Init�� �ʿ��� cData�� network type�� ����
					cData.NetType = TCM_NT_2_5G;
//					cData.NetType = cData.NetType << 8;
					cData.NetType = cData.NetType ;
					cData.NetType = cData.NetType | TCM_NT_95C;
					MSG_ERROR("PPP 2.5G",0,0,0);
					// Net Connect �޽����� ����
					EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);	
				}
				else 
				{	// Wap���� ���� �Ҹ��� ���� ���(Error ó��)
					MSG_ERROR("Not Wap to Wavelet",0,0,0);
					
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				}
			}
			else // 2G (PPP�� ���� ���¿��� Wap���κ��� �Ѿ���Ƿ� PPP ������� ���־�� ��)
			{
				MSG_ERROR("2G ����",0,0,0);

				if (SKY_IS_WAP_TO_WAVELET())
				{	// ���� Wap���� �Ҹ��� ��츸 �����Ƿ� Wap���κ��� �Ҹ������� Ȯ��.
					SKY_SET_WAP_TO_WAVELET(FALSE);
					SKY_SET_SOCK_MODE(FALSE);
					
					// PPP Open �ϰ� �ִ� �������� ����
					WaveletStatus = PPPOPEN_STATUS;
					
					// PPP Open 
					result = MM_PPPOpen(); // (wav_multisock.c�� ����)
					if(result == WAV_SOCK_SUCCESS)	// PPP Opne Success
					{
						MSG_ERROR("PPP Open Success",0,0,0);
						SKY_SET_SOCK_MODE(TRUE); // PPP Open �������� ����
						// Controller Init�� �ʿ��� cData�� network type�� ����	
						cData.NetType = TCM_NT_2G;
						cData.NetType = cData.NetType << 8;
						cData.NetType = cData.NetType | TCM_NT_95AB;
						// Net Connect �޽����� ����
						EQS_SEND_EVENT(SE_NET_CONNECTED, EQS_NA, EQS_NA);	
					}
					else if (result == WAV_SOCK_WOULDBLOCK)	// Would Block�� ��� Timer Setting
					{
						MSG_ERROR("PPP Open would block",0,0,0);
						Sock_Timer = TRUE;
						// 20�ʰ� ��ٸ��� ���� TImer (EQS_TIMER_2) ����
						EQS_StartTimer(EQS_TIMER_2,(DWORD)20*1000, FALSE);	
					}
					else // PPP Open Error
					{
						MSG_ERROR("PPP Open ERROR",0,0,0);
						
						// Error Message ���
						g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
						EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
						EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						break;
					}
				}
				else 
				{	// Wap���� ���� �Ҹ��� ���� ���(Error ó��)
					MSG_ERROR("Not Wap to Wavelet",0,0,0);
					
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
				}
			}
			break;
		
		// "***.*K �����ͼ��� ����"�� ���� ������ ������ �ʵ��� �ϱ� ���� break
		case SE_CALL_CONNECT:		
		case SE_CALL_ORIGINATED:
			break;

		case SE_NET_CONNECTED:
			SKY_SET_SOCK_MODE(TRUE); // PPP Open �������� ����
			Sock_Timer = FALSE;	// PPP Open�� �����Ͽ����Ƿ� Timer flag Reset
			EQS_StopTimer(EQS_TIMER_2);	// Timer Stop

			// Socket�� ����  Tcp Connect �������� ����
			WaveletStatus = SOCKCONNECT_STATUS;	

			// Socket �ʱ�ȭ
			MM_InitSocket();
			// Socket Open
			result = MM_OpenSocket();

			if (result != WAV_SOCK_SUCCESS) // Socket Open�� ������ ��� 
			{
				MSG_ERROR("Socket Open ERROR",0,0,0);
				sWaveletReleaseSocket();

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;
			}
			
			// Socket Open�� ������ ���
			MSG_ERROR("Socket Open Success",0,0,0);

			// Tcp Connect
			result 	= MM_Connect(g_wsndIP, (WORD)g_wsndPort);	
			
			if (result != WAV_SOCK_SUCCESS) // Tcp Connect�� ������ ��� 
			{
				MSG_ERROR("CONNECT ERROR",0,0,0);
				sWaveletReleaseSocket();

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;
			}

			// Tcp Connect�� ������ ��� 
			MSG_ERROR("CONNECT Success",0,0,0);
			if (SKY_IS_IN_WAVELET())	// Wavelet���� ������ �� ��� 
			{
					MSG_HIGH("CONNECT_SUCCESS",0,0,0);
				// cData���� ����
				cData.CPID = g_wcpid;			// Contents Provider ID
				cData.ContentsID = g_wctsid;		// Contents ID
				cData.AudioDecoder = TCM_AD_EVRC;	// Available Audio Decoder
				cData.ColorDepth = TCM_CD_256;
				cData.SkinType = g_wskinType;		// SkinType
				cData.UDP_Port1 = 0; // yoursun 01/04/10
				cData.UDP_Port2 = 0; // yoursun 01/04/10

				//yoursun 01/06/08
				// MIN�� ����
				nv_num.nam = SKY_GET_CURRENT_NAM();
				(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_num );
				if (nv_num.dir_number[0] == '0') // 10�ڸ� ��ȣ�� ���
				{
					strncpy((BYTE *)cData.MIN, (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[10] = '\0';
				}
				else // 11�ڸ� ��ȣ�� ��� (11.....�̷��� ����)
				{
					cData.MIN[0] = '0';
					strncpy((BYTE *)&(cData.MIN[1]), (BYTE *)(nv_num.dir_number), 10);
					cData.MIN[11] = '\0';
				}
//////////////////////////////// Wavelet Contents Header Setting /////////////////////////////////////
				for (index = 0; index < 21; index++)	// �̸��� '/'�� ������ �������� ��� 
				{										// ����� �� �κ��� ' '���� �ٲ� ����
					if (g_wcntName[index] == '/') g_wcntName[index] = ' ';
				}
				
				// ��� ���� �̸� ����
				strcpy(szFileNameHeader, g_wcntName);
				strcat(szFileNameHeader, ".inf");
				
				// ����� �����ؾ� �ϴ� ������ �־���
				gWaveletHeader.cpid = g_wcpid;
				gWaveletHeader.ctsid = g_wctsid;
				strcpy(gWaveletHeader.cntName,g_wcntName);
				gWaveletHeader.cntProtect = g_wcntProtect;
				gWaveletHeader.cntExpDate = g_wcntExpDate;

				gWaveletHeader.dnDate = SKY_GetLocalTime(); 
//////////////////////////////////////////////////////////////////////////////////////////////////////

				// ������ ���ϵ��� �̸� ����
				strcpy(szFileNameVideo, g_wcntName);
				strcpy(szFileNameAudio, g_wcntName);
				strcpy(szFileNameText, g_wcntName);
				strcpy(szFileNameSkin, g_wcntName);
				
				strcat(szFileNameVideo, ".vdo");
				strcat(szFileNameAudio, ".ado");
				strcat(szFileNameText, ".cap");
				strcat(szFileNameSkin, ".skn");

				// �̾� �ޱ�� ���� �ޱ�,�� ������ �ޱ⸦ ����
				switch (gDownInfo)
				{
					case OLD_CONTENTS :	// ���� �ޱ�
					case NEW_CONTENTS :	// �� ������ �ޱ�
						// �ӽ� ������ ����Ǿ� �ִ� ���� ������ ����
						if ((UINT)SKY_GetNumFile(SKY_TEMPWAV_DIR) != 0)	
						{
								SKY_DeleteDataFiles(SDF_TEMPWAV);
						}

						// ��� ������ ����� ��� ���� ��� ���Ͽ� ����
						if (SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameHeader, &hFile_Header) == SDR_OK)
						{
							gWaveletHeader.fConnectDN = TRUE;	// �ٿ�ε尡 ���۵Ǿ����� �Ϸ���� ������ �˸��� flag
							SKY_WriteDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
							if (hFile_Header) 
							{
								SKY_CloseDataFile(hFile_Header);
								hFile_Header = NULL;
							}
						}
						else // ��� ���� ���� ���н�
						{
							MSG_HIGH(" Header Create Error!",0,0,0);						
							if (hFile_Header) 
							{
								SKY_CloseDataFile(hFile_Header);
								hFile_Header = NULL;
							}
							
							sWaveletReleaseSocket();

							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}

						// ������ ������ �����
						if ((SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameVideo, &hFile_Video) == SDR_OK)
						&&(SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameAudio, &hFile_Audio) == SDR_OK)
						&&(SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameText, &hFile_Text) == SDR_OK)
						&&(SKY_CreateDataFile(SDF_TEMPWAV, (BYTE*)szFileNameSkin, &hFile_Skin) == SDR_OK))
						{
							// Controller Initialize ����
							if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
							{	
								gContents_Length = 0;	// ������ ���� ���� 0���� �ʱ�ȭ 
								// Download ������ Core�� ���
								if (!MM_ProcessTCM2UICommand(TCM_UC_DOWNLOAD, gContents_Length, (DWORD)180000 )) 
								{ // �ٿ�ε� ������ ������ ��� 
									sReleaseController();
									sWaveletReleaseSocket();

									// Error Message ���
									g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
									EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
									EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
								}
							}
							else // Controller Initialize ����
							{
								sReleaseController();				
								sWaveletReleaseSocket();

								// Error Message ���
								g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
								EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
								EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
							}
						}
						else //������ ���� ����Ⱑ ������ ��� 
						{
							sReleaseController();				
							sWaveletReleaseSocket();

							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						break;

					case CONNECT_CONTENTS:	// �̾�ޱ�
						// �ٿ�ε� �޴� ���� �������� ����� ���� 
						if (SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameHeader, &hFile_Header, SFT_WRITE) == SDR_OK)
						{
							if (gWaveletHeader.fConnectDN == FALSE) // Download �Ϸ�� �������� �̾�ޱ�� ���õ� ���
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
							else	// Download�� �Ϸ���� ���� ����Ʈ�� �̾�ޱ�� ���õ� ���
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
						else // ��� ������ ���� ������ �߻��� ���
						{
							MSG_HIGH(" Header Open Error!",0,0,0);						
							if (hFile_Header) 
							{
								SKY_CloseDataFile(hFile_Header);
								hFile_Header = NULL;
							}

							sWaveletReleaseSocket();

							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						// �ٿ�ε� ���� ��ɽ� ������� �ٿ���� ũ�⸦ �˷���� �ϹǷ� �� ������ ũ�⸦ ���Ѵ�.
						gContents_Length = (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, szFileNameVideo);
						gContents_Length += (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, szFileNameAudio);
						gContents_Length += (DWORD)SKY_GetDataFileSize(SDF_TEMPWAV, szFileNameText);
						// �� ������ ������ ����.
						if ((SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameVideo, &hFile_Video, SFT_WRITE) == SDR_OK)
						&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameAudio, &hFile_Audio, SFT_WRITE) == SDR_OK)
						&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameText, &hFile_Text, SFT_WRITE) == SDR_OK)
						&&(SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameSkin, &hFile_Skin, SFT_WRITE) == SDR_OK))
						{
							// �� �� ������ ������ �����͸� �� �ڷ� �ű��.
							SKY_SeekFile(hFile_Video, FS_SO_END, (DWORD)0);    
							SKY_SeekFile(hFile_Audio, FS_SO_END, (DWORD)0);    
							SKY_SeekFile(hFile_Text, FS_SO_END, (DWORD)0);    
							SKY_SeekFile(hFile_Skin, FS_SO_END, (DWORD)0);    
							
							// Controller Initialize ����
							if (MM_InitTCM2Controller(&cData)==TRUE)		// yoursun 01/04/07
							{
								if (!MM_ProcessTCM2UICommand(TCM_UC_DOWNLOAD, gContents_Length, (DWORD)(180000-gContents_Length)))
								{
									sReleaseController();
									sWaveletReleaseSocket();

									// Error Message ���
									g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
									EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
									EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);			
								}
							}
							else // Controller Initialize ����
							{
								sReleaseController();
								sWaveletReleaseSocket();

								// Error Message ���
								g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
								EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
								EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
							}
						}
						else //������ ���� ���Ⱑ ������ ���
						{
							sReleaseController();
							sWaveletReleaseSocket();

							// Error Message ���
							g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
							EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
							EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
						}
						break;
				}
			}
			else // Wavelet���� ������ �ȵ� ��� 
			{
				sWaveletReleaseSocket();

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			break;

		case SE_NET_DISCONNECTED:	// PPP ������ ����ٴ� �̺�Ʈ
			Sock_Timer = FALSE;		// PPP close �� Timer�� ����� �������� ����
			EQS_StopTimer(EQS_TIMER_1);	// PPP close Would Block�� ����ϴ� Ÿ�̸� ����

			SKY_SET_SOCK_MODE(FALSE);	// PPP Close�Ǿ����� ����
			MSG_HIGH("Net Disconnected Success!",0,0,0);

			// PPP�� ���� ���¿� ���� ���� ó���� �ٸ��Ƿ� �� ���¿� �°� ó����� ���
			switch (WaveletStatus)
			{
				case TCM_DS_DOWNLOAD_ENDED:		// �ٿ�ε� �Ϸ� �� PPP close�� ��� 
					// sSKA_MainOnWavelet Applet�� child�� �ҷ� �ӽ� ������ �ٿ���� �������� �÷����ϵ��� �̺�Ʈ�� ��
					EQS_SEND_EVENT(SE_WAV_PLAY, EQS_NA, EQS_NA);
					break;

				case PHONE_CLOSE:				// Phone close �̺�Ʈ�� PPP close�� ���
					MSG_HIGH("phone close!",0,0,0);
					SKY_SET_FROM_WAP(FALSE);
					SKY_SET_WAVELET_TO_WAP(FALSE);

					EQS_END_TO(EQS_POP_TO_ROOT);
					break;

				case CALL_DROP:					// Call Drop���� ���� PPP close�� ���
					MSG_HIGH("Call Drop",0,0,0);

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_4);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				case INIT_STATUS:				// Wavelet�� ������ ��Ȳ���� PPP close�� ���
					MSG_HIGH("init state",0,0,0);

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				case TCM_DS_CONTENTS_NOT_FOUND:	// ��Ʈ��ũ ���� �߻����� ���� PPP close�� ���
					MSG_HIGH("contents not found",0,0,0);
				case TCM_DS_CONTROLLER_ERROR:
					MSG_HIGH("controller error",0,0,0);

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NETWORK_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				case TCM_DS_SETUP_ERROR:		// �ھ� ���� �߻����� ���� PPP close�� ���
					MSG_HIGH("setup error",0,0,0);
				case TCM_DS_SERVER_ERROR:
					MSG_HIGH("server_error",0,0,0);									

					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;

				default:
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CALL_DS_WAVE_EXIT),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
					break;
			}
			break;

		case SE_CALL_ENDED:	// Call Drop ó���� ���� Call Drop�� �Ͼ call End�� ��츸 ó��
			MSG_HIGH("CALL END",0,0,0);
			if ((SKY_GetCallEndStatus() == CM_CALL_END_NO_SRV)||(SKY_GetCallEndStatus() == CM_CALL_END_FADE))
			 {
				// call drop �������� ����
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
			// Backlignt ���� ����
			SKY_SET_NO_OFF_BACKLIGHT(FALSE);
			SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
			break;

		case SE_WAV_PLAY:
			// �ӽ� ������ �ٿ���� �������� �÷����ϴ� Applet�� Child�� �θ�
			EQS_START_CHILD(sSKA_MainOnWavelet);
			break;

		case SE_KEY_CLEAR :// HS_CLR_K:
		case SE_SOFTKEY_CANCEL:
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			sReleaseController();
			sWaveletReleaseSocket();

		    // Error Message ���
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

	    case SE_KEY_PHONE_CLOSE:	// Phone close�� �Ͼ ���
			// Phone Close �������� ����
			WaveletStatus = PHONE_CLOSE;
			
			// �ϴ� �۾��� ����
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			sReleaseController();

			// socket close + PPP close
			sWaveletReleaseSocket();
			if (SKY_IS_SOCK_MODE())
			{
				sWaveletPPPClose();
			}
			break;

		case SE_KEY_END:			// ����ڰ� End Ű�� ���� ��� 
			// �ϴ� �۾��� ����
			MM_ProcessTCM2UICommand(TCM_UC_STOP, 0, 0 );
			sReleaseController();
			
			// socket close 
			sWaveletReleaseSocket();
			
			if (WaveletStatus == TCM_DS_DOWNLOAD_STARTED) // �ٿ�ε� �����̸� 
			{
			    // Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
		        EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
			    EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else										// �ٿ�ε� ������ �ƴϸ� 
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

		case SE_MUXNEGO_SUCCESS:		// Mux Option Nego�� ����
			MSG_HIGH("SE_MUXNEGO_SUCCESS",0,0,0);
			if (WaveletStatus != PPPOPEN_STATUS)	// 2G���� PPP Open �� ���� Mux Nego�� �ƴ� ��
			{
				if (SKY_IS_IN_WAVELET())
				{
					sReleaseWavelet();
				}
				EQS_START_SIBLING(SKA_MainWAP, FALSE);	// Wap���� ���ư�
			}
			break;

		case SE_IO_MESGBOX_END:
            if (pEvent->dwParam == WAV_MES_1)	// Wavelet�� �����ϴ� �޽���
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
			    // Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_RETURN_NTOP), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                EQC_SET_USERDATA(g_hWavControl,WAV_MES_2);
                EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			}
			else if( pEvent->dwParam == WAV_MES_4)
			{
				// Error Message ���
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
					// Error Message ���
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
			else //	Sock_Timer = TRUE;	<= PPP Close Would Block�� ���
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
			else //	Sock_Timer = TRUE <= PPP Opne Would Block�� ���
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
					// Error Message ���
					g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR),  2, MBT_NONE, MBT_HELP);
					EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
					EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				}
			}
			break;

		case SE_TM_TIMER3:
			(ti[EQS_TIMER_3]).func(EQS_TIMER_3+TIMER_ID_CHANGE,(ti[EQS_TIMER_3]).user);
			break;

		case SE_PH_RSSI: // ȭ�� ���������� ���� �� ȭ�� ����� ����
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*============================================================================= 
Function:		MM_DisplayImage

Description:	�̹��� ���(Partial Update)
  
Input:			pData : ���÷��� �� Image
				stride: ���� �� ����� ���� �ǳ� �پ�� �ϴ� ����
				sx,sy : ����� ������ ��ǥ 
				width : Image�� �ʺ�
				height: Image�� ����
    
Output:			�־��� text�� (x,y)���� width*height��ŭ ����Ѵ�.
      
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

Description:	�̹��� ���(Partial Update) : 256 color �̻�
  
Input:			
    
Output:			
      
Return value:
        
Side effects:
===============================================================================*/
//#define DISPAY_FOR_LOOP // For loop�� �̿��Ͽ� Display�� ��� ���� 
						  // ���ǵǾ� ���� ������ for loop�� Ǭ ���� ���� 
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

Description:	��Ų ���(Partial Update)
  
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

Description:		Text ���
  
Input:				data : ���÷��� �� text
					x,y : ��ǥ 

Output:				�־��� text�� (x,y)�� ����Ѵ�.
      
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

Description:	��Ʈ ũ�� ����
  
Input:			bkor: �ѱ����� �������� ���� ( 1 => �ѱ�, 0 => ����)
				x	: �� ��Ʈ�� �ʺ�
				y	: �� ��Ʈ�� ����

Output:			��Ʈ�� �ʺ�� ���� ���� x, y�� ���޵Ǿ� ����.

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

Description:	���� LCD ũ�� ����

Input:			x,   y : ���� LCD�� ������ ��ġ
				ex, ey : ���� LCD�� ����, ������ ����

Output:			x, y, ex, ey�� ���� ���޵Ǿ� ����.

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

Description: ���۸� % ���

Input:		nPercent : ���۸� ��
    
Output:		�Էµ� ���۸� %�� ȭ�鿡 ����Ѵ�.
			�ٿ�ε�� ���۸��� ǥ�ø� �и��Ͽ� ����Ѵ�.
      
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

	if (fDownload == TRUE)  // Download Service �� ��� 
	{
		if (nPercent == 100) sprintf((CHAR*)szString, SKY_GET_TOKEN(TKN_WAVELET_DOWNLOAD_DONE));
		else sprintf((CHAR*)szString, SKY_GET_TOKEN(TKN_WAVELET_DOWNLOAD_PERCENT), nPercent);
	}
	else  // Streaming Service �� ���
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

Description:	�޸� �Ҵ�
  
Input:			size : �Ҵ�Ǿ�� �ϴ� �޸� ũ��
					   0�̸� �ִ��Ҵ� ���� �޸𸮸� �Ҵ��϶�� �ǹ�    
Output:			�䱸�� ũ���� �޸𸮸� �Ҵ��Ѵ�.
      
Return value:	�Ҵ�� �޸��� ������
        
Side effects:
===============================================================================*/
void * MM_GetFreeMemory(TCM_MEM_TYPE type, DWORD* size )
{
    extern BYTE	Image$$APP_SHARE_RAM$$Base[]; //  Shared RAM�� ���� Address

	// size�� ���� 0�� ���� �� Buffer�� ���� �ִ� �Ҵ� ������ �޸𸮸� �Ҵ��� 
	// ���� �ִ��Ҵ� �޸��� 100k�� �Ҵ�Ǿ����� *size�� assign��
	if(*size == (DWORD)0)    		*size = (DWORD)100000;

	// �޸𸮴� Shared RAM�� ��밡���� �κ��� ���� �����͸� �ش�.
	if (type == TCM_MT_FRAME_BUFFER)		// Frame Buffer �Ҵ� ��û�� ���� ���
	{
		MSG_ERROR("TCM_MT_FRAME_BUFFER : size %d",*size,0,0);
		return (BYTE *)Image$$APP_SHARE_RAM$$Base;
	}
	else if (type == TCM_MT_PACKET_BUFFER)	// Packet Buffer �Ҵ� ��û�� ���� ���
	{
		MSG_ERROR("TCM_MT_PACKET_BUFFER : size %d",*size,0,0);
		return (BYTE*)(Image$$APP_SHARE_RAM$$Base+100000);
	}

	// Frame Buffer�� Packet Buffer�� �ƴ� �ٸ� �޸� Ÿ���� �Ҵ� ��û�� ���� ��� 
	MSG_ERROR("BUFFER Type Error",0,0,0);
	return (BYTE *)0;
}

/*============================================================================= 
Function:		MM_TCM2ServerStatus

Description:	������ ���� ����Ʈ 
  
Input:			status	: �����κ��� �� ���°�
				data	: ���¿� ���� �ʿ��� data ��

Output:			������ ���¸� �޴´�.

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
				
				gWaveletHeader.fConnectDN = FALSE; // �ٿ�ε� �ϷḦ �˸�
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

					// Error Message ���
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

				// Error Message ���
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

				// Error Message ���
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

				// Error Message ���
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

				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
				break;
    
			case TCM_DS_SETUP_ERROR://Setup error
				WaveletStatus = TCM_DS_SETUP_ERROR;			
				MSG_ERROR("TCM_DS_SETUP_ERROR",0, 0, 0);

				sReleaseController();
				sWaveletReleaseSocket();
	
				// Error Message ���
				g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_SYSTEM_ERROR),  2, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
				EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;
    
			case TCM_DS_CONTENTS_NOT_FOUND://Contents not found
				WaveletStatus = TCM_DS_CONTENTS_NOT_FOUND;
				MSG_ERROR("TCM_DS_CONTENTS_NOT_FOUND",0, 0, 0);
				sReleaseController();
				sWaveletReleaseSocket();

				// Error Message ���
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

Description:	������ �б�

Input:			type : ���� �����Ͱ� ����, �����, ��Ų, �ؽ�Ʈ���� ����
				data : ���Ϸκ��� ������ ������
				size : ���� �������� ũ��

Output:			�־��� Ÿ���� �����͸� ���Ϸκ��� �д´�.

Return value:	������ ���� �������� ����

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
		MSG_ERROR("MM_ReadData : ���� �б�, size %d",ResultSize, 0, 0);
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
	
			MSG_ERROR("MM_ReadData : ����� �б�, size %d",ResultSize, 0, 0);
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
			MSG_ERROR("MM_ReadData: �ڸ� �б�, size %d",ResultSize, 0, 0);
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
			MSG_ERROR("MM_ReadData: ��Ų �б�, size %d",ResultSize,0,0);
	}
	if (ResultSize == 0) ResultSize = 0xffff;
	return (WORD)ResultSize;
}

/*============================================================================= 
Function:		MM_WriteData

Description:	������ ����

Input:			type : �� �����Ͱ� ����, �����, ��Ų, �ؽ�Ʈ���� ����
				data : ���Ͽ� �� ������
				size : �� �������� ũ��

Output:			�Է� �����͸� �־��� Ÿ���� ���Ͽ� ����.

Return value:	������ �� �������� ����

Side effects:
===============================================================================*/
DWORD	MM_WriteData( TCM_FILE_TYPE type, BYTE* data, DWORD size )
{// �ٿ�ε� ���񽺿����� ���Ǵ� API
	static	BYTE	pMallocData[2000];	// �� ���Ͽ� �����͸� ���� ���� �ʿ��� ���� 
	DWORD	ResultSize = 0;				// ������ �� �������� ũ�⸦ ���� ����

	memset((void*)pMallocData,0x00,2000);	// �޸� �ʱ�ȭ
	
	if (type == TCM_FT_VIDEO)	
	{				// ���� Ÿ���� �����͸� ������ ���Ͽ� ����.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Video, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData:���� in %d, out %d", size, ResultSize, 0);
	}
	if (type ==	TCM_FT_AUDIO)
	{				// ����� Ÿ���� �����͸� ������� ���Ͽ� ����.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Audio, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData:����� in %d, out %d", size, ResultSize, 0);
	}
	if (type == TCM_FT_TEXT)
	{				// �ڸ� Ÿ���� �����͸� �ڸ��� ���Ͽ� ����.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Text, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData:�ڸ� in %d, out %d", size, ResultSize, 0);
	}
	if (type == TCM_FT_SKIN)
	{				// ��Ų Ÿ���� �����͸� ��Ų�� ���Ͽ� ����.
		memcpy(pMallocData, data, size);
		ResultSize = (WORD)SKY_WriteDataBlock(hFile_Skin, pMallocData, (DWORD)size);
			MSG_ERROR("MM_WriteData: ��Ų in %d, out %d", size, ResultSize, 0);
	}
	if (ResultSize == 0) ResultSize = 0xffffffff;
	return ResultSize;
}

/*============================================================================= 
Function:		MM_Seek

Description:	���� Seek
				offset�� O�� �ƴ� ��쿡�� ������ �����Ƿ� 0�� ��츸 ����
				(�� ������ ó������ �̵��� ���� Seek�� ����)

Input:			type : seek�� ������ ����, �����, ��Ų, �ؽ�Ʈ���� ����
				offset : ���� ó���� �������� seek�� ��ġ

Output:			�־��� Ÿ���� ���Ͽ� ���� �־��� offset��ŭ seek�Ѵ�.

Return value:	������ seek�� ��ġ

Side effects:
===============================================================================*/
DWORD	MM_Seek(TCM_FILE_TYPE type, DWORD offset)
{
	if (g_wsvcType == SVC_IDLE)	// Wavelet �����Կ� �ִ� ������ �����
	{
		switch (type)
		{
			case TCM_FT_VIDEO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Video_length = tVideo_length;	// ���� �������� ���� Acces ���� �ʱ�ȭ
					pVdoData = ptVdoData;			// ���� �������� ���� ������ �ʱ�ȭ
							MSG_ERROR("MM_Seek: ���� ���� length %d", Video_length, 0, 0);
				}
			break;

			case TCM_FT_AUDIO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Audio_length = tAudio_length;	// ����� �������� ���� Acces ���� �ʱ�ȭ
					pAdoData = ptAdoData;			// ����� �������� ���� ������ �ʱ�ȭ
							MSG_ERROR("MM_Seek: ����� ���� length %d",Audio_length, 0, 0);
				}
			break;

			case TCM_FT_TEXT:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Text_length = tText_length;	// ��Ų �������� ���� Acces ���� �ʱ�ȭ
					pTxtData = ptTxtData;		// ��Ų �������� ���� ������ �ʱ�ȭ
							MSG_ERROR("MM_Seek: Text ���� length %d",Text_length, 0, 0);
				}
			break;
			}
	}
	else						// �ٿ�ε� �� �ӽ� ���� ������ �ִ� ������ �����
	{
		switch (type)
		{

			case TCM_FT_VIDEO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Video_length = tVideo_length;	// ���� �������� ���� Acces ���� �ʱ�ȭ
					pVdoData = ptVdoData;			// ���� �������� ���� ������ �ʱ�ȭ
							MSG_ERROR("MM_Seek: Video�� ���� length %d",Video_length, 0, 0);
				}
			break;

			case TCM_FT_AUDIO:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Audio_length = tAudio_length;	// ����� �������� ���� Acces ���� �ʱ�ȭ
					pAdoData = ptAdoData;			// ����� �������� ���� ������ �ʱ�ȭ

							MSG_ERROR("MM_Seek: ����� ���� length %d",Audio_length, 0, 0);
				}
			break;

			case TCM_FT_TEXT:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Text_length = tText_length;		// �ڸ� �������� ���� Acces ���� �ʱ�ȭ
					pTxtData = ptTxtData;			// �ڸ� �������� ���� ������ �ʱ�ȭ
							MSG_ERROR("MM_Seek: Text ���� length %d",Text_length, 0, 0);
				}
			break;

			case TCM_FT_SKIN:
				if (offset == 0) 	 //yoursun 01/06/21 
				{
					Skin_length = tSkin_length;		// ��Ų �������� ���� Acces ���� �ʱ�ȭ
					pSknData = ptSknData;			// ��Ų �������� ���� ������ �ʱ�ȭ
							MSG_ERROR("MM_Seek: Skin ���� length %d",Skin_length, 0, 0);
				}
			break;
			}
	}
	return offset;
}

/*============================================================================= 
Function:		MM_GetFileLength

Description:	���� ���� 
  
Input:			type : ���̸� �˷��� ������ ����, �����, ��Ų, �ؽ�Ʈ���� ����

Output:			�־��� Ÿ���� ������ ���̸� �˷��ش�.

Return value:	������ ����

Side effects:
===============================================================================*/
DWORD	MM_GetFileLength( TCM_FILE_TYPE type )//yoursun 01/04/10
{
	DWORD length = 0;
	MSG_ERROR("MM_GetFileLength",0,0,0);

	if (g_wsvcType == SVC_IDLE)		// Wavelet �����Կ� �ִ� ������ �����
	{
		switch (type)
		{
			case TCM_FT_VIDEO:		// ���� 
				length = tVideo_length;
				break;

			case TCM_FT_AUDIO:		// �����
				length = tAudio_length;				
				break;

			case TCM_FT_TEXT:		// �ڸ�
				length = tText_length;
				break;
		}
	}
	else							// �ٿ�ε� �� �ӽ� ���� ������ �ִ� ������ �����
	{
		switch (type)
		{
			case TCM_FT_VIDEO:		// ����
				length = tVideo_length;
				break;

			case TCM_FT_AUDIO:		// �����
				length = tAudio_length;				
				break;

			case TCM_FT_TEXT:		// �ڸ�
				length = tText_length;
				break;

			case TCM_FT_SKIN:		//��Ų
				length = tSkin_length;
				break;
		}
	}
	return (DWORD)length;
}

/*============================================================================= 
Function:		MM_WaveletListDisplay

Description:	Idle ���¿��� �ٿ���� list�� ������
  
Input:			SizeOrNot - ���� ���̰� �ʿ��� ����Ʈ���� �ƴ����� �˷��ش�.

Output:			�־��� Ÿ���� ������ ���̸� �˷��ش�.

Return value:	

Side effects:
===============================================================================*/
void MM_WaveletListDisplay(BOOL SizeOrNot)
{	
	INT8 sWindowSize;
	BYTE* ppWaveletFileList[SKY_MAX_NUM_FILES];	// Wavelet File List�� �о�ͼ� �ӽ÷� �����ϴ� list ���� 
	int  tSize;									// �������� ũ�⸦ ����ؼ� �ӽ÷� ������ ����
	UINT8 nLines = 0, index = 0;
	BYTE  tFileNameHeader[25];					// ������ ũ�⸦ ����� �� �ʿ��� �ӽ� ���� �̸� ��
	BYTE  tFileNameVideo[25];
	BYTE  tFileNameAudio[25];
	BYTE  tFileNameText[25];
	char Size[7];								// ������ ũ�⸦ Kbyte ������ ȯ���Ͽ� character������ ���� ����
	
	g_unNumOfWavelet = 0;

    if((UINT)SKY_GetNumFile(SKY_WAVELET_DIR) == 0)	// Wavelet �����Կ� �������� �ϳ��� ���� ��� 
    {
		MSG_ERROR("No List!",0,0,0);
		
		// Wavelet �������� ������� �˷��ִ� �޽��� ����ϰ� ����
		g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_NTOP_FILE_EMPTY), 2, MBT_NONE, MBT_HELP); //yoursun 2001.3.15 
		EQC_SET_USERDATA(g_hWavControl,WAV_MES_1);
		EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);	
		return;
    }
    else											// Wavelet �����Կ� �������� �ִ� ���
    {
		// ppWaveletFileList�� �޸� �Ҵ�
		for (index = 0; index < SKY_MAX_NUM_FILES; index++)	
		{
			ppWaveletFileList[index] = (BYTE*)SKY_Calloc(SKY_PATH_NAME_MAX_LENGTH,sizeof(BYTE));
			EQS_ASSERT(ppWaveletFileList[index] != NULL);
		}
		
		// ppWaveletFileList�� Wavelet �����Կ� �ִ� ���� ����Ʈ �̸��� �� ������ ������
		nLines = SKY_GetFileList(SKY_WAVELET_DIR, ppWaveletFileList);

		if (SizeOrNot == TRUE)	// Content Name(Content Size) ������ Display�� �ʿ��� ���
		{						// �ٿ�ε� �� ���� ������ ���ڶ� Wavelet �������� �������� �����ؾ� �� ��� 

			// ppWaveletFileList�� ���� ����Ʈ�� �� �������� ���� 4���� ������ �����ϹǷ� 4���� ������ �̸��� ������
			// ����Ʈ�� �ȴ�. �� Ŀ������ ���� �Ѱ��� ����Ʈ ��Ҹ� �������� g_ppScrollList�� g_ppScrollList_Length�� 
			// ppWaveletFileList�� 4������ �ϳ��� �ǳ� �ٸ鼭 ������ �̸��� �־��ش�.
			// �׸��� ���� ����(nLines)�� �ƴ� ������ ������ g_unNumOfWavelet�� For Loop�� ���鼭 ����Ѵ�.
			for (index = 0; index < nLines; g_unNumOfWavelet++, index+=4 )
			{
				// g_ppScrollList�� g_ppScrollList_Length�� �޸𸮸� �Ҵ�
				g_ppScrollList[g_unNumOfWavelet] = (BYTE *)SKY_Calloc(LCD_COLUME_SIZE-1, sizeof(BYTE));
				g_ppScrollList_Length[g_unNumOfWavelet] = (BYTE *)SKY_Calloc(25+3, sizeof(BYTE));
				
				// �Ҵ�� �޸𸮿� ������ �̸��� ����
				strcpy(g_ppScrollList[g_unNumOfWavelet], ppWaveletFileList[index]);
				strcpy(g_ppScrollList_Length[g_unNumOfWavelet], ppWaveletFileList[index]);

				// ������ ũ�⸦ ����ϱ� ���� �־��� �������� ���� ���� �̸��� �������
				strcpy(tFileNameHeader, g_ppScrollList[g_unNumOfWavelet]);
				strcpy(tFileNameVideo, g_ppScrollList[g_unNumOfWavelet]);
				strcpy(tFileNameAudio, g_ppScrollList[g_unNumOfWavelet]);
				strcpy(tFileNameText, g_ppScrollList[g_unNumOfWavelet]);
					
				strcat(tFileNameHeader, ".inf");
				strcat(tFileNameVideo, ".vdo");
				strcat(tFileNameAudio, ".ado");
				strcat(tFileNameText, ".cap");

				// ������ ũ�� ���
				tSize = (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameHeader)	
										+ (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameVideo)
										+ (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameAudio)
										+ (DWORD)SKY_GetDataFileSize(SDF_WAVELET, tFileNameText);

				// ���� ������ ������(tSize)�� Kbyte������ ȯ���ϰ� charater������ �ٲپ� ��(Size)
				sprintf(Size,"%d",tSize>>10); 

				// �̸��� ���� ����Ʈ�� ũ�⸦ �����ִ� �κ��� �ٿ��� : (ex) ����ƾ(137Kbyte)
				strcat(g_ppScrollList_Length[g_unNumOfWavelet],"(");
				strcat(g_ppScrollList_Length[g_unNumOfWavelet],Size);
				strcat(g_ppScrollList_Length[g_unNumOfWavelet],"Kbyte)");
			}
			// Scroll List ����
			sWindowSize = ( g_unNumOfWavelet > 6 ) ? 6 : g_unNumOfWavelet;
			g_hListHnd = EQC_ScrollList(5, 5, 18, sWindowSize,   g_ppScrollList_Length   , g_unNumOfWavelet, g_unNumOfWavelet, 0); 
		}
		else	// Content Name�� list�� Display�� ��� 
		{		// Wavelet �������� list�� Idle �޴��󿡼� ������� �� ���
			for (index = 0; index < nLines; g_unNumOfWavelet++, index+=4 )
			{
				// g_ppScrollList�� �޸𸮸� �Ҵ�
				g_ppScrollList[g_unNumOfWavelet] = (BYTE *)SKY_Calloc(LCD_COLUME_SIZE-1, sizeof(BYTE));
				// �Ҵ�� �޸𸮿� ������ �̸��� ����
				strcpy(g_ppScrollList[g_unNumOfWavelet], ppWaveletFileList[index]);			
			}
			// Scroll List ����
			sWindowSize = ( g_unNumOfWavelet > 6 ) ? 6 : g_unNumOfWavelet;
			g_hListHnd = EQC_ScrollList(5, 5, 18, sWindowSize,   g_ppScrollList   , g_unNumOfWavelet, g_unNumOfWavelet, 0); 
		}
		
		// ��ũ�� ����Ʈ�� ������ ȭ�鿡 �� �� ���̹Ƿ� TRUE�� �����Ͽ� �θ� 
		// ������ ���ʰ� �������� ������ �Դ� ���� �ϸ鼭 ������ �ٺ��̰� ���ش�.
		EQC_SetScrollFlowMode(g_hListHnd, TRUE);

		// ��ũ�� ����Ʈ�� ��Ÿ���� ����
        EQC_SET_STYLE( g_hListHnd, EQC_GET_STYLE(g_hListHnd) |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_MAXIMIZE );
		// ��ũ�� ����Ʈ�� ��Ƽ�� 
		EQS_SetIOCActive( g_hListHnd );
		
		// ppWaveletFileList�� �޸� �ݳ�
		for (index = 0; index < SKY_MAX_NUM_FILES; index++)
		{
			SKY_Free(ppWaveletFileList[index]);
		}
    }
}
/*============================================================================= 
Function:		MM_ReadContentInfo

Description:	Download ���� Wavelet �������� ������ CONNECT_CONTENTS, OLD_CONTENTS, NEW_CONTENTS
				���� �� ������ Return ���ش� �Լ��� Wap���� call��
  
Input:			cpid	- Contents Provider ID
				ctsid	- Contenst ID
				cntName	- Contents Name

Output:			

Return value:	���õ� �������� � �ٿ�ε� Ÿ������ �˷���

Side effects:
===============================================================================*/
// 
BYTE MM_ReadContentInfo(int cpid, int ctsid, char *cntName)
{
    UINT index;

	// ���� ��������� �켱 �̸��� ���� ���̹Ƿ� �켱 ������ �̸��� �Ҵ�
	strcpy(szFileNameHeader, cntName);
    strcat(szFileNameHeader, ".inf");

	MSG_HIGH(" Read Content Info",0,0,0);
    
	if ((index = (UINT)SKY_GetNumFile(SKY_TEMPWAV_DIR)) != 0)	
	{		// Wavelet Temporary directory�� �ٿ�޴� ���� ������ �ִ� ��� �̾�ޱ�� ������ ������ Ȯ��
		// Wap���� �ٿ�ε带 ���� ������ �������� ���� �̸��� ������ ���� Ȯ��
		// �� �̸��� header�� ������ �켱 �̸��� ���� ��������
        if (SKY_OpenDataFile(SDF_TEMPWAV, (BYTE*)szFileNameHeader, &hFile_Header,SFT_READ) == SDR_OK)
        {
            // ���� Wap�� List���� ������ �������� �ӽ� ���� ������ �ٿ�޴� ���� �������� ���� ������
			// ���� Ȯ���ϱ� ���� �ӽ���������� �ִ�  ���� �̸��� ������ ��� ������ ����
			SKY_ReadDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));
			
			// Read �� Header File Close
			if (hFile_Header)
			{
				SKY_CloseDataFile(hFile_Header);
				hFile_Header = NULL;
			}

			// ������� ���� ������ Centents Provider ID�� Content ID�� ���Ͽ� ������ 
			// ���� �������� �Ǵ��Ͽ� �̾�ޱ��� ����� �Ǵ� ������ ���� CONNECT_CONTENTS�� �����Ͽ� �˷���
			if ((gWaveletHeader.cpid == g_wcpid)&&(gWaveletHeader.ctsid == g_wctsid)) 
            {
				return CONNECT_CONTENTS;
            }
        }
	}
    else	// Wavelet Temporary directory�� �ٿ�޴� ���� �������� ���� ��� ���ιޱ�� ������ ������ Ȯ��
	{
		// Wap���� �ٿ�ε带 ���� ������ �������� ���� �̸��� ������ ���� Ȯ��
		// �� �̸��� header�� ������ �켱 �̸��� ���� ��������
		if (SKY_OpenDataFile(SDF_WAVELET, (BYTE*)szFileNameHeader, &hFile_Header, SFT_READ) == SDR_OK)
        {
            // ���� Wap�� List���� ������ �������� Wavelet ���� ������ �̹� �ٿ� ���� �������� ���� ������
			// Ȯ���ϱ� ���� Wavelet ���� ������ �ִ� ���� �̸��� ������ ��� ������ ����
            SKY_ReadDataBlock(hFile_Header, &gWaveletHeader, (DWORD)sizeof(gWaveletHeader));

			// Read �� Header File Close
			if (hFile_Header)
			{
				SKY_CloseDataFile(hFile_Header);
				hFile_Header = NULL;
			}

			// ������� ���� ������ Centents Provider ID�� Content ID�� ���Ͽ� ������ 
			// ���� �������� �Ǵ��Ͽ� ���ιޱ��� ����� �Ǵ� ���� OLD_CONTENTS�� �����Ͽ� �˷���
			if ((gWaveletHeader.cpid == g_wcpid)&&(gWaveletHeader.ctsid == g_wctsid)) 
			{
				return OLD_CONTENTS; 
			}
	    }
	}

	// Open Error������ ��ó ������ ���ߴ��� Ȯ���Ͽ� Header File Close
	if (hFile_Header)	
	{
		SKY_CloseDataFile(hFile_Header);		
		hFile_Header = NULL;
	}

	// ���� ��찡 �ƴ϶�� ó�� �ٿ���� ���������� NEW_CONTENTS�� �����Ͽ� �˷���
	return NEW_CONTENTS;
}

void MM_TimerDo(void) // ���� ������ ������ timer�� �� �ʷ��� ���� ���� ������ Rex Timer�� ����
{
	MSG_HIGH(" Rex Timer! ",0,0,0);
	(ti[EQS_GENERAL_TIMER]).func(EQS_GENERAL_TIMER+TIMER_ID_CHANGE+WAV_GENERAL_TIMER,(ti[EQS_GENERAL_TIMER]).user);
}