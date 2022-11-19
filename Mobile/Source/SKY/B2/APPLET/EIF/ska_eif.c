/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     ska_eif.c
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
** 2001-02-05 redstar     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#include "dsi.h"
#include "dsatcop.h"
#include "parm.h"
#include <ctype.h>
#include "ska.h"

//yjahn96 01.02.24 --- eif for wap
#include "eifwap.h"
#include "wapadapter.h"



#include "dsskt.h"
#include "dsi.h"
#include "dsatcop.h"
#include "sdevmap.h"
#include "sio.h"
#include "siors232.h"
#include "hw.h"
#include "diag.h"

#include "caii.h"
#include "srchi.h"
#include "mccdma.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define	BCD_2_ASCII(n)			(((n) >> 4)*10 + ((n) & 0x0F))

#define MAX_EIF_COMMAND_LEN     15

#define BEEP_INTERVAL  500   // delay 500m(0.5) sec

#define MAX_INPUT_SIZE 256

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

extern  q_type            mc_cmd_free_q;

//-------------------------------
//yjahn96 01.03.12 - for EIF WAP
extern char EIF_url[192];	
extern char EIF_proxyurl[192];
extern boolean g_NKey_UOPEN;
extern boolean g_Link_Text_UOPEN;

extern boolean g_EIF_SEND_EXIT;


struct	tagEIF_TABLE_T	
{
	BYTE	*pszCommand;	             // ASCII Command
	BYTE    (*pCommandFunction)(BYTE*);  // ASCII Parsing function
};

typedef struct tagEIF_TABLE_T    EIF_TABLE_T;
extern unsigned long destinationIP;


/* 단말 상태 설정및 확인을 위한 명령*/
BYTE EIF_Reset(BYTE* pszCommand);          // 단말기 초기화
BYTE EIF_Ping(BYTE* pszCommand);           // 핑퐁 신호
BYTE EIF_SetPing(BYTE* pszCommand);        // 핑 신호 설정
BYTE EIF_Dial(BYTE* pszCommand);            // 전화번호 확인
BYTE EIF_Level(BYTE* pszCommand);           // 무선망 수신정보 조회
BYTE EIF_Location(BYTE* pszCommand);        // 위치정보 조회
BYTE EIF_Ver(BYTE* pszCommand);             // 버전 조회
/* 음성통화 */
BYTE EIF_Ori(BYTE* pszCommand);             // 전화걸기
BYTE EIF_Rel(BYTE* pszCommand);             // 전화끊기
BYTE EIF_Atan(BYTE* pszCommand);            // 자동 착신 기능 

/* 단분 발신 */
BYTE EIF_MOReq(BYTE* pszCommand);           // 단문 발신
BYTE EIF_EmMO(BYTE* pszCommand);            // 비상용 MO 등록
BYTE EIF_DelMO(BYTE* pszCommand);           // 송신한 단문 삭제
BYTE EIF_DelAllMO(BYTE* pszCommand);           // 송신한 단문 전체 삭제

/* 단문 수신*/
BYTE EIF_MTCnt(BYTE* pszCommand);           // 수신된 단문 갯수 확인
BYTE EIF_ReadMT(BYTE* pszCommand);          // 수신된 단문 읽기
BYTE EIF_ReadTI(BYTE* pszCommand);          // 수신된 단문 익기(특정 TI)
BYTE EIF_DelMT(BYTE* pszCommand);           // 수신된 단문 삭제
BYTE EIF_DelAllMT(BYTE* pszCommand);        // 수신된 단문 전체 삭제
BYTE EIF_NewMsg(BYTE* pszCommand);          // 단문 메시지 수신 즉시 알림 설정 해제 및 확인
BYTE EIF_MTAck(BYTE* pszCommand);            //process  new msg ack

 /* LCD 제어 */
BYTE EIF_SetLCD(BYTE* pszCommand);          // LCD 제어권 설정
BYTE EIF_ClrLCD(BYTE* pszCommand);          // LCD 지우기
BYTE EIF_OutLCD(BYTE* pszCommand);          // LCD 에 문자 출력

/* KeyPad 제어*/
BYTE EIF_SetKey(BYTE* pszCommand);          // 키패드 관련 제어 상태 선택 
BYTE EIF_KeyEmul(BYTE* pszCommand);         // Keypad Emulation
BOOL EIF_KeyScan(INT KeyID);                //Keypad Scan

/* 부저 제어 */
BYTE EIF_Beep(BYTE* pszCommand);            // 부저
BYTE EIF_Bell(BYTE* pszCommand);            // 부저 착신음

/* WAP 단말 기능 */
BYTE EIF_IWFCon(BYTE* pszCommand);          // IWF 접속 상태 확인 기능
BYTE EIF_WAPCon(BYTE* pszCommand);			// WAP 접속 상태 확인 기능
BYTE EIF_Input(BYTE* pszCommand);           // 입력상자에 입력하기
BYTE EIF_Select(BYTE* pszCommand);          // Option 선택

/* URL 접속 관련 기능 */
BYTE EIF_DelCache(BYTE* pszCommand);        // Cache Memory Clear
BYTE EIF_UPing(BYTE* pszCommand);           // URL Ping
BYTE EIF_UOpen(BYTE* pszCommand);           // URL Open
BYTE EIF_NewPage(BYTE* pszCommand);         // 새로운 페이지 수신시 즉시 알림
BYTE EIF_GetURL(BYTE* pszCommand);          // URL  정보 가져오기 기능
BYTE EIF_Gvminfo(BYTE* pszCommand);          // GVM 파일 수신정보 알림 기능

/* E-mail 관련 기능 */
BYTE EIF_DelAllMail(BYTE* pszCommand);      //  메일 삭제

/* Mobile Wallet 관련 기능 */
BYTE EIF_GetWalletInfo(BYTE* pszCommand);        // Get Mobile Wallet Information
BYTE EIF_SetPIN(BYTE* pszCommand);           // Set PIN
BYTE EIF_SetMWData(BYTE* pszCommand);           // Set Mobile Wallet Data

BOOL EIF_SendNewMsg(UINT8 nRcvNumber);
BOOL EIF_SendReMsg(UINT8 nRcvNumber);
void EIF_Inititialize(void);


BYTE	SKT_DM_OFF_command(BYTE*);
BYTE	SKT_DM_1_command(BYTE*);
BYTE	SKT_DM_2_command(BYTE*);

BYTE    SKT_tibn_command(BYTE *src);
BYTE	SKT_dr_command(BYTE *src);

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

const	EIF_TABLE_T  ParseTable[] = {	// ASCII AT SKT Module Table
                                        /* 단말 상태 설정및 확인을 위한 명령*/
    "*RESET",       EIF_Reset,          // 단말기 초기화
    "*PING",        EIF_Ping,           // 핑퐁 신호
    "*SETPING",     EIF_SetPing,        // 핑 신호 설정
    "*DIAL",        EIF_Dial,           // 전화번호 확인
    "*LEVEL=",      EIF_Level,          // 무선망 수신정보 조회
    "*LOCATION",    EIF_Location,       // 위치정보 조회
    "*VER",         EIF_Ver,            // 버전 조회

                                        /* 음성통화 */
    "*ORI=",        EIF_Ori,            // 전화걸기
    "*REL",         EIF_Rel,            // 전화끊기
    "*ATAN",        EIF_Atan,           // 자동 착신 기능 

    /* 단분 발신 */
    "*MOREQ=",      EIF_MOReq,          // 단문 발신
    "*EMMO",        EIF_EmMO,           // 비상용 MO 등록
    "*DELMO=",      EIF_DelMO,          // 송신한 단문 삭제
    "*DELALLMO",    EIF_DelAllMO,          // 송신한 단문 전체 삭제
    
    /* 단문 수신*/
    "*MTCNT",       EIF_MTCnt,          // 수신된 단문 갯수 확인
    "*READMT=",     EIF_ReadMT,         // 수신된 단문 읽기
    "*READTI=",     EIF_ReadTI,         // 수신된 단문 익기(특정 TI)
    "*DELMT=",      EIF_DelMT,          // 수신된 단문 삭제
    "*DELALLMT",    EIF_DelAllMT,       // 수신된 단문 전체 삭제
    "*NEWMSG",      EIF_NewMsg,         // 단문 메시지 수신 즉시 알림 설정 해제 및 확인       
    "*MTACK:",      EIF_MTAck,          // process ack from TE2 for newmsg
    /* LCD 제어 */
    "*SETLCD",      EIF_SetLCD,         // LCD 제어권 설정
    "*CLRLCD",      EIF_ClrLCD,         // LCD 지우기
    "*OUTLCD=",     EIF_OutLCD,         // LCD 에 문자 출력
    
    /* KeyPad 제어*/
    "*SETKEY",      EIF_SetKey,         // 키패드 관련 제어 상태 선택 
    "*KEYEMUL=",    EIF_KeyEmul,        // Keypad Emulation    

    /* 부저 제어 */
    "*BEEP=",       EIF_Beep,           // 부저
    "*BELL",        EIF_Bell,           // 부저 착신음
    

    /* WAP 단말 기능 */
    "*IWFCON",      EIF_IWFCon,         // IWF 접속 상태 확인 기능
	"*WAPCON:",		EIF_WAPCon,			// WAP 접속 상태 확인 기능
    "*INPUT=",      EIF_Input,          // 입력상자에 입력하기
    "*SELECT=",     EIF_Select,         // Option 선택
    
    /* URL 접속 관련 기능 */
    "*DELCACHE",    EIF_DelCache,       // Cache Memory Clear
    "*UPING=",      EIF_UPing,          // URL Ping
    "*UOPEN=",      EIF_UOpen,          // URL Open
    "*NEWPAGE=",    EIF_NewPage,        // 새로운 페이지 수신시 즉시 알림
    "*GETURL=",     EIF_GetURL,         // URL  정보 가져오기 기능
	"*GVMINFO=",    EIF_Gvminfo,        // GVM 파일 수신정보 알림 기능

    /* E-mail 관련 기능 */
    "*DELALLMAIL",  EIF_DelAllMail,     //  메일 삭제

	/* Mobile Wallet 관련 기능 */
	"*GETWALLETINFO",	EIF_GetWalletInfo,    // Get Mobile Wallet Information
	"*SETPIN:",			EIF_SetPIN,           // Set PIN
	"*SETMWDATA:",		EIF_SetMWData,        // Set Mobile Wallet Data
                                        /*  old command */
	"*DMOFF",       SKT_DM_OFF_command,
	"*DM1",         SKT_DM_1_command,
	"*DM2",         SKT_DM_2_command,
	"*TIBN",		SKT_tibn_command,	
	"*DR",			SKT_dr_command,

    "",             NULL
};
clk_cb_type Ping_timer;
clk_cb_type BeepTimer;
clk_cb_type	PingTimeOutTimer;
clk_cb_type NewMsgTimer;                // for newmsg 
clk_cb_type LevelTimer;

BOOL  g_fEmmo = FALSE;
BOOL  g_fEIFAutoReceive = FALSE;
BOOL  g_fLevel = FALSE;
BOOL  g_fSetLCD   = FALSE;
UINT8 g_nOutX, g_nOutY;
BYTE  g_szOutBuffer[80];
UINT8 g_nBeepNumber;
UINT8 g_nReadTI;
SKY_EVENT_E EIF_Event;

BOOL  g_fNewMsgTimerAlive = FALSE;
BOOL  g_fSMSNewMsg;  // for sms new message

UINT8 nIteration = 0; // index create

//-------------------------------
//yjahn96 01.02.24 --- eif for wap
INT   g_nEIFWapstatus = EIF_WAP_NONE;
char selectBuf[16];				// select_command function 수행을 위한 버퍼 
unsigned char		PingCnt=0;
int nTimeout = 0;	


#define MAX_NEW_MSG_TI     40
UINT8   g_nTIMsgSize;
INT     g_nTIMsg[MAX_NEW_MSG_TI];
UINT8   g_nNewMsgTry;

BYTE  l_Dial[21];


static BOOL  g_fBell;
static INT   g_nTI;
static UINT8 g_nOriMsgNumber;

static UINT8 g_nSetPing  = 0;
static INT   g_nPingTurn = 0;
static BYTE  g_szTermNumber[15];       // for emmo
static BYTE  g_szCallBackNumber[15];
static INT   g_nEMTI;
static BYTE  g_szEMMsg[81];

/*
static INT16 l_nTimeOut, l_nTI;
static BYTE  l_szSendNumber[17], l_szRcvNumber[17], l_szMsg[81];
*/

UINT8 g_nKeyFlag = 0;

LOCAL UINT8 skt_TiBellNotify;


LOCAL void   CheckPingTimeOutTimer(void);
LOCAL void PingCallBackCommand(int4 ms_interval);


LOCAL  void	Put_NV_DM_PORT ( byte nPort )
{
	NV_ITEM_U nv_data;

	if(nPort == 0)
	{
		nv_data.sio_dev_map_menu_item = SDM__U1_DIAG__U2_NULL;
	}
	else if(nPort == 1)
	{
		nv_data.sio_dev_map_menu_item = SDM__U1_DS____U2_DIAG;
	}
	else
	{
		nv_data.sio_dev_map_menu_item = SDM__U1_DS____U2_NULL;
	}

    SKY_PutNV(NV_SIO_DEV_MAP_MENU_ITEM_I, &nv_data);                 
	hw_reset();
}


BOOL SendEIFCommand(SKY_EVENT_E Event)
{
    ui_cmd_type	*cmd_ptr;	// Pointer to UI command

    EIF_Event = Event;
	if ((cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) != NULL) 
	{
		cmd_ptr->hdr.cmd        = UI_EIF_CMD_F;
		cmd_ptr->hdr.task_ptr   = NULL;
		cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		ui_cmd(cmd_ptr);       
	}
    else 
        return FALSE;

    return TRUE;
}


/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL INT	at_atoi(byte **command)
{
	byte	*src;
	INT    number = 0;

	src = *command;

    while( 1 )
    {
        if( *src == 0 )
            break;
        else if( *src >= '0' && *src <='9' )
        {
            number = number*10 + (*src - '0');
            src++;
        }
        else if( *src == ' ')
        {            
            while( *src ==' ' )
            {
                if( *src == 0 || *src == ',' || isalpha(*src) || isdigit(*src) )
                    break;
                src++;
            }
        }
        else if( *src == ',' )
        {
            break;
        }
        else if( *src == 13  || *src == 10 )
            break;
        else if( isalpha(*src) )
            break;
        else
            return -1;       
    }     
    *command = src;

    return number;
}


BYTE	at_eif_command(BYTE **command)
{
    BYTE*  pszSrc;
    BYTE   Buf[MAX_EIF_COMMAND_LEN+1];
    INT16  i, nLen;

    pszSrc = *command;
	
	for (i = 0; i <MAX_EIF_COMMAND_LEN; i++)
	{
        if( pszSrc[i] == 0 )
            break;
        if( isalpha(pszSrc[i]))
            Buf[i] = toupper(pszSrc[i]);
        else
            Buf[i] = pszSrc[i];
    }
    Buf[i] = 0;
   
	for (i = 0; ParseTable[i].pCommandFunction != NULL; i++) 
    {
        nLen = STRLEN(ParseTable[i].pszCommand);
		if (!STRNCMP(Buf, ParseTable[i].pszCommand, nLen)) 
        {
			pszSrc += nLen;           // 명령어까지 인식 
			break;
		}
	}

    if( ParseTable[i].pCommandFunction == NULL )
        return 4;
    else
        return ParseTable[i].pCommandFunction(pszSrc);				
}



/* 단말 상태 설정및 확인을 위한 명령*/
BYTE EIF_Reset(BYTE* pszCommand)          // 단말기 초기화
{
    mc_msg_type *mc_ptr; // for RESET command

	mc_ptr = (mc_msg_type *) q_get( &mc_cmd_free_q );

    if( mc_ptr == NULL ) 
    {      
        at_send_extended_text("*SKT*RESET:0");      
    } 
    else 
    {
        /* Set up queue to place on when done, and */
        /* specify no task signalled when done     */
        mc_ptr->hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
        mc_ptr->hdr.cmd_hdr.task_ptr = NULL;
        mc_ptr->hdr.cmd = MC_RESET_F;
        mc_cmd(mc_ptr);
        at_send_extended_text("*SKT*RESET:1");   
    }

    return 0xFE;
}
BYTE EIF_Ping(BYTE* pszCommand)           // 핑퐁 신호
{
    BYTE   szString[80];
    UINT8  nPhoneState;
    DWORD  curr_time;

	clk_julian_type Time;  /* julian time conversion */

    
	
	// 단말기의 현재 상태를 체크한다.
    if( SKY_IS_ORIGINATE() )
    {
        if( SKY_IsInCall() )
            nPhoneState = 3;  //Traffic State
        else
            nPhoneState = 1;  //Access  State
    }
    else if( SKY_IS_ALERT() )
        nPhoneState = 2;      //Paging  State
    else if( SKY_IsInCall() )
        nPhoneState = 3;      //Traffic State
    else
        nPhoneState = 0;      //Idle    State

	curr_time = SKY_GetLocalTime();           // get current time

	clk_secs_to_julian (curr_time, &Time);
		
	sprintf(szString, "*SKT*PONG: %d, %04d%02d%02d%02d%02d%02d",\
		    (int)nPhoneState, Time.year, Time.month, Time.day, Time.hour, Time.minute, Time.second);
	
	at_send_extended_text(szString);

    //CheckPingTimeOutTimer(); 	// Reset ping time
    if( g_fEmmo && g_nSetPing == 0 )
	{
		clk_dereg(&PingTimeOutTimer);
		CheckPingTimeOutTimer();
	}

    return 0xFE;
}

void   PingTimeoutCallBack(int4 ms_interval)
{
	g_UIMOBuffer.nPriority = 0;
	g_UIMOBuffer.nSendNumberSize = STRLEN(g_szCallBackNumber);
	STRCPY(g_UIMOBuffer.pszSendNumber, g_szCallBackNumber);
	g_UIMOBuffer.nRcvNumberSize = STRLEN(g_szTermNumber);
	STRCPY(g_UIMOBuffer.pszRcvNumber, g_szTermNumber);
	g_UIMOBuffer.nMsgSize = STRLEN(g_szEMMsg);
	STRCPY(g_UIMOBuffer.pszMsg, g_szEMMsg);

    if( g_fEmmo )
	{
		SendEIFCommand(SE_EM_REQ);
	}

}

void PingCallBackCommand(int4 ms_interval)
{
    BYTE   szString[80];
    UINT8  nPhoneState;
    DWORD  curr_time;

	clk_julian_type Time;  /* julian time conversion */
	
	// 단말기의 현재 상태를 체크한다.
    if( SKY_IS_ORIGINATE() )
    {
        if( SKY_IsInCall() )
            nPhoneState = 3;  //Traffic State
        else
            nPhoneState = 1;  //Access  State
    }
    else if( SKY_IS_ALERT() )
        nPhoneState = 2;      //Paging  State
    else if( SKY_IsInCall() )
        nPhoneState = 3;      //Traffic State
    else
        nPhoneState = 0;      //Idle    State

	curr_time = SKY_GetLocalTime();           // get current time

	clk_secs_to_julian (curr_time, &Time);
		
	sprintf(szString, "*SKT*PONG: %d, %04d%02d%02d%02d%02d%02d",\
		    (int)nPhoneState, Time.year, Time.month, Time.day, Time.hour, Time.minute, Time.second);
	
	at_send_extended_text(szString);

}


LOCAL void   CheckPingTimeOutTimer()
{
#if 0
    {
		clk_dereg(&PingTimeOutTimer);
	}
#endif
	// Re - Set timer
	if ( g_nPingTurn > 0) 
    {
		clk_reg(&PingTimeOutTimer, PingTimeoutCallBack,
			    (g_nPingTurn)*1000,
			    (g_nPingTurn)*1000, TRUE); 
	}
	
}

BYTE EIF_SetPing(BYTE* pszCommand)        // 핑 신호 설정
{
	static BOOL fEmmoActive = TRUE;
    int  nSec;
    char szString[20];	

	switch (*pszCommand)	
    {
	case '=':
		pszCommand++;					
	    nSec = at_atoi(&pszCommand);
        if( nSec < 0 || nSec > 255 )
            return 4;
        //g_nPingTurn = nSec;

		g_nSetPing = (UINT8) nSec;

        //CheckPingTimeOutTimer(); 	// Reset ping time		
		clk_dereg(&Ping_timer);
		if (nSec > 0) {
            if( g_fEmmo == TRUE && fEmmoActive == TRUE)
			{
				fEmmoActive = FALSE;
				clk_dereg(&PingTimeOutTimer);
			}
			clk_def(&Ping_timer);
			clk_reg(&Ping_timer, PingCallBackCommand, nSec*1000, nSec*1000, TRUE);
		}
		else
		{
			if( g_fEmmo == TRUE )
			{
				fEmmoActive = TRUE;
				CheckPingTimeOutTimer();
			}
		}

		break;

	case '?':
		pszCommand++;
		nSec = g_nSetPing;
		break;
	default	:
		return 4;							// Return ERROR
	}
	sprintf(szString, "*SKT*SETPING:%d", nSec);
	at_send_extended_text(szString);
    return 0xFE;
}

BYTE EIF_Dial(BYTE* pszCommand)            // 전화번호 확인
{   
   char temp[21];
   int  i, nLen;
   char String[80];   

   nv_dir_number_type     nv_dir_num; 
   nv_dir_num.nam = SKY_GET_CURRENT_NAM();     // 자기정보 저장    

   memset(&nv_dir_num, 0, sizeof(nv_dir_number_type));
   dsi_get_nv_item(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );   

   nLen = strlen(nv_dir_num.dir_number);

   memset(l_Dial,0,21);
   memset(String,0,80);

	if( nv_dir_num.dir_number[0] != '0' ) // 4자리 국번 011-1234-5678
	{ 
		l_Dial[0] = '0';
		for( i = 0; i < nLen; i++)
		{
			if( i >= 20 || (nv_dir_num.dir_number[i] < '0' || nv_dir_num.dir_number[i] > '9') )
				break;
			l_Dial[i+1] = nv_dir_num.dir_number[i];
		}
		l_Dial[i+1] = 0;
	} 
	else 
	{
		for( i = 0; i < nLen; i++)
		{
			if( i >= 20 || (nv_dir_num.dir_number[i] < '0' || nv_dir_num.dir_number[i] > '9')  )
				break;
			l_Dial[i] = nv_dir_num.dir_number[i];
		}
		l_Dial[i] = 0;
        //strcpy(dial, nv_dir_num.dir_number);
	}	
	sprintf(String, "*SKT*DIAL: %s", l_Dial);
	at_send_extended_text(String);

    return 0xFE;
}

void   LevelCallBack(int4 ms_interval)
{
    int  temp_rx, temp_tx, temp_adj;
    int   rx_power, ecio_log, tx_power;
    char  szString[100],rssi_txt[5],adj_txt[5],txstate_txt[5],ecio_txt[5];

    dword badFr, totFr, fer;

#ifndef WIN32   

    db_items_value_type dbi_ch;      /* DB items temp buffer */   
    db_items_value_type dbi_pn;      /* DB items temp buffer */
    db_items_value_type dbi_ecio;    /* DB items temp buffer */    
    parm_fer_type   fer_data;         /* data for FER parameters */


	if( g_fLevel == FALSE )
	{
		clk_dereg(&LevelTimer);
	}

    db_get( DB_CHANNEL, &dbi_ch ); // show channel 

    temp_rx = srch_get_rx_agc();   // receive rx power
    rx_power = ( 6325 - ((int1)temp_rx * 33) ) /100;   
    rssi_txt[0] = '-';
			
    if(rx_power >99)
    {
		rx_power=-100;
		rssi_txt[1] = '1';
        rssi_txt[2] = rx_power/10 +'0';
		rssi_txt[3] = rx_power%10 +'0';
		
		rssi_txt[4] = '\0';
			
    }
	else
	{
		rssi_txt[1] = ' ';
        rssi_txt[2] = rx_power/10 + '0';
		rssi_txt[3] = rx_power%10 + '0';		
		rssi_txt[4] = '\0';
	}

    temp_tx = srch_get_tx_adj(); // receive tx power
	temp_tx = (-temp_tx) / 2;

	if( temp_tx < -64 ) 
    { 
        temp_tx = -64; 
    }
	else if( temp_tx > 64 ) 
    { 
        temp_tx = 64; 
    }
    else ;

		
    temp_adj = temp_tx; // receive adjust

	if( temp_adj < 0 )
	{
		temp_adj = -temp_adj;
		adj_txt[0] = '-';
		adj_txt[1] = temp_adj/10 +'0';
		adj_txt[2] = temp_adj%10 +'0';
		adj_txt[3] = '\0';
	}
	else
	{
		adj_txt[0] = ' ';
		adj_txt[1] = temp_adj/10 + '0';
		adj_txt[2] = temp_adj%10 + '0';
		adj_txt[3] = '\0';
	}		
		
    if(rx_power > 0)
    {
	    rx_power = -rx_power;
    }

    tx_power =  -73 - rx_power + temp_tx;

    if( SKY_IsInCall() == FALSE)   
    {
	    tx_power = -99;
    } 
    if( tx_power >= 0 )
    {
	    txstate_txt[0] = ' ';
	    txstate_txt[1] = tx_power/10 + '0';  //ex
	    txstate_txt[2] = tx_power%10 + '0';  //ex
	    txstate_txt[3] = '\0';
    }

    else
    {
	    tx_power = -tx_power;
	    txstate_txt[0] = '-';
	    txstate_txt[1] = tx_power/10 + '0';
	    txstate_txt[2] = tx_power%10 + '0';
	    txstate_txt[3] = '\0';
    }

	
    parm_get_fer ( &fer_data );       // Get parameter FER data 
	badFr = fer_data.bad_frames;
	totFr =	fer_data.total_frames;
	if(totFr == 0)
	{
		fer = 0;
	}
	else
	{
		fer = 100 * badFr / totFr;
	}

	db_get( DB_PILOT_PN_OFF, &dbi_pn ); // Show PN Offset

	db_get(DB_ECIO, &dbi_ecio);       // Get an EcIo value
		
    ecio_log = dbi_ecio.ecio / 2;
	ecio_txt[0] = '-';
	ecio_txt[1] = ecio_log/10 +'0';
	ecio_txt[2] = ecio_log%10 +'0';
	ecio_txt[3] = '\0';
	
	sprintf((char*)szString, "*SKT*LEVEL: CH %d,RX %s,Adjust %s,TX %s,FER %ld,PN %d,EcIo %s",\
					  dbi_ch.channel, rssi_txt, adj_txt, txstate_txt, fer,\
                      dbi_pn.pilot_pn_off, ecio_txt );
					
	at_send_extended_text(szString);  // Display current values
#endif // WIN32
    //return 0xFE;
}

BYTE EIF_Level(BYTE* pszCommand)           // 무선망 수신정보 조회
{
    int nLevelIter;
    
    if( isdigit(pszCommand[0]) )
    {
        nLevelIter = at_atoi(&pszCommand);
        if( nLevelIter > 255 )
			nLevelIter = 255;
        if( nLevelIter )
        {     
            g_fLevel = TRUE;
            clk_reg(&LevelTimer, LevelCallBack,1000,
			        nLevelIter*1000, TRUE); 
        }
        else
        {
			if( g_fLevel == TRUE )
			{
				g_fLevel = FALSE;
				clk_dereg(&LevelTimer);
			}
			nLevelIter = 1;
            clk_reg(&LevelTimer, LevelCallBack,1000,100000, FALSE); 
            //LevelCallBack(100);
        }
        return 0xFE;
    }
    else
        return 4;

}

extern srch_sect_struct_type  srch_aset[];    /* Active set */
extern srch_sect_struct_type  srch_cset[];    /* Candidate set */  
extern srch_sect_struct_type  srch_nset[];    /* Neighbor set */
extern UINT16  srch_aset_cnt;    /* Active set size */
extern UINT16  srch_cset_cnt;    /* Candidate set size */
extern UINT16  srch_nset_cnt;    /* Neighbor set size */
extern UINT16  srch_ref_inx;

BYTE EIF_Location(BYTE* pszCommand)        // 위치정보 조회
{
    char szBuf[100];

	caii_sysparm_type *cap_ptr;

	if ((cap_ptr = (caii_sysparm_type *) mcc_get_msg( CAI_SYS_PARM_MSG )) != NULL)
	{
		sprintf((char*)szBuf, "LOCATION: SID %d NID %d, Reg Zone %d, Base ID %d, Pilot PN %d",\
				cap_ptr->sid, cap_ptr->nid, cap_ptr->reg_zone, cap_ptr->base_id,\
				(UINT16)srch_aset[srch_ref_inx].pilot);

	}
	else
	{
		sprintf((char*)szBuf, "LOCATION: SID %d NID %d, Reg Zone %d, Base ID %d, Pilot PN %d",
				0, 0, 0, 0, 0);

	}

	at_send_extended_text(szBuf);
	return 0xFE;
}

BYTE EIF_Ver(BYTE* pszCommand)             // 버전 조회
{
    at_send_extended_text("*SKT*VER:2.2");      // EIF Version 2.0
    return 0xFE;
}

/* 음성통화 */
BYTE EIF_Ori(BYTE* pszCommand)             // 전화걸기
{
    UINT8 nLen, nIndex;

    nLen = STRLEN(pszCommand);
    nIndex = 0;

    while( pszCommand[nIndex] != 0 )
    {
        if( !isdigit(pszCommand[nIndex]))
           return 4;
        else
            nIndex++;
    }
               
    if( (EQS_GET_APPLET() == SKA_MainIdleScreen || EQS_GET_APPLET() == SKA_IdleDebugScreen) &&\
		SKY_IsInCall()     == FALSE         &&\
        SKY_IS_AUTO_LOCK() == FALSE )    
    {
        if(nIndex > 0)
        {
            SET_NUMEDIT_SECRET(FALSE);
            SET_NUMEDIT_POS(nIndex);
            SET_NUMEDIT_BUFFER(pszCommand, nIndex);
            SendEIFCommand(SE_ORI_CALL);    
            at_send_extended_text("*SKT*ORI: 1");
        }
    }
    else
    {      
        at_send_extended_text("*SKT*ORI: 0");
    }

#if 0
	int i, len;
	ui.n.pos = 0;
	memset(ui.n.buf, 0, sizeof(ui.n.buf)); // initializing, 1999. 11. 23

    if( EQS_GET_APPLET() == 
	if(ui.idling == 1       // IDLE 상태이고,		
	  && ui.call == 0       // 통화중이 아니며, 
	  && ui.lock == 0)      // LOCK이 안걸려 있을 때 전화를 건다.
	{		
		len = strlen((char*)src);
	
		for(i = 0; i < len; i++)
		{
			if(DIGIT(*src) || *src == '*' || *src == '#')
			{
				ui.n.buf[ui.n.pos++] = *src++;
			}
			else
			{
				return 4;
			}
		}
									
		ui.n.buf[ui.n.pos] = '\0';

		ui.n.act = TRUE;
		ui.tog_flag = FALSE;
		ui.ed.act = FALSE;   // 있으나 마나 한 코딩이다.

		ui_add_event(HS_SEND_K);

		at_send_extended_text("*SKT*ORI: 1");
	}
	else 
		at_send_extended_text("*SKT*ORI: 0");
#endif
    return 0xFE;
}
BYTE EIF_Rel(BYTE* pszCommand)             // 전화끊기
{

    if( SKY_IsInCall() )
    {        
        if( SendEIFCommand(SE_KEY_END) )
        {
            at_send_extended_text("*SKT*REL: 1");
	    }
        else
            at_send_extended_text("*SKT*REL: 0");
        
    }
    else
        at_send_extended_text("*SKT*REL: 0");

    return 0xFE;
}

BYTE EIF_Atan(BYTE* pszCommand)            // 자동 착신 기능 
{

    char  szString[80];
   	
	switch (*pszCommand)	
    {
	case '=':		
        pszCommand++;
	    if( *pszCommand == '0' )
        {
			g_fEIFAutoReceive = FALSE;
            //SKY_SET_HANDS_FREE_AUTO_RECEIPT(FALSE);
			//SKY_SET_EAR_PHONE_AUTO_RECEIPT(FALSE);
        }
        else if( *pszCommand == '1')
        {
			g_fEIFAutoReceive = TRUE;
            //SKY_SET_HANDS_FREE_AUTO_RECEIPT(TRUE);
			//SKY_SET_EAR_PHONE_AUTO_RECEIPT(TRUE);
        }
        else 
        {
            return 4;
        }
        break;
	case '?':
        pszCommand++;
		break;
	default	:
		return 4;					// Return ERROR
	}

	if( g_fEIFAutoReceive )
	{
		at_send_extended_text("*SKT*ATAN: 1");
	}
	else
	{
		at_send_extended_text("*SKT*ATAN: 0");
	}
	//sprintf(szString, "*SKT*ATAN: %d", (int)SKY_IS_HANDS_FREE_AUTO_RECEIPT());
	//at_send_extended_text(szString);

    return 0xFE;
}

/* 단분 발신 */
BYTE EIF_MOReq(BYTE* pszCommand)           // 단문 발신
{
    BYTE* pszSrc = pszCommand;
    BYTE  szString[20];
    INT   i, nComma;    
    
    if( !isdigit( *pszSrc ) )
        return 4;

    i = at_atoi(&pszSrc);

    if( i < 0 || i > 254 )
        g_nOriMsgNumber = 255;
    else
        g_nOriMsgNumber = i;

    if( *pszSrc !=',')
        return 4;

    pszSrc++;
    i = 0;
    while( pszSrc[i] !=',' )
    {
        if( pszSrc[i] == 0 )
            return 4;
        i++;
    }
    STRNCPY(g_UIMOBuffer.pszRcvNumber, pszSrc, i);
    g_UIMOBuffer.nRcvNumberSize = i;
    g_UIMOBuffer.pszRcvNumber[i] = 0;

    pszSrc += i+1;
    nComma  = 0;
    i = 0;
    while( pszSrc[i] != 0 )
    {
        if( pszSrc[i++] == ',' )
            nComma++;
    }
    if( nComma == 1 )
    {
        g_nTI = at_atoi(&pszSrc);
        pszSrc++;
    }
    else if( nComma == 2 )
    {
        i = 0;
        while( pszSrc[i] !=',' )
        {
            if( pszSrc[i] == 0 )
                return 4;
            i++;
        }
        STRNCPY(g_UIMOBuffer.pszSendNumber, pszSrc, i);
        g_UIMOBuffer.pszSendNumber[i] = 0;
        g_UIMOBuffer.nSendNumberSize = i;
        pszSrc += i+1;
        g_nTI = at_atoi(&pszSrc);
        pszSrc++;
    }
    else
        return 4;

    if( STRLEN(pszSrc) > MAX_SMS_TEXT)
    {
        STRNCPY(g_UIMOBuffer.pszMsg,pszSrc,MAX_SMS_TEXT);
        g_UIMOBuffer.pszMsg[MAX_SMS_TEXT] = 0;
    }
    else
        STRCPY(g_UIMOBuffer.pszMsg,pszSrc);

    g_UIMOBuffer.nMsgSize  = STRLEN( g_UIMOBuffer.pszMsg );
    g_UIMOBuffer.nPriority = 0;
    g_UIMOBuffer.nID = g_nOriMsgNumber;

    sprintf(szString,"*SKT*MOREQ:%d", g_nOriMsgNumber);
    at_send_extended_text(szString);

    if( SendEIFCommand(SE_MO_REQ) == FALSE )
    {
        sprintf(szString,"*SKT*MOREQ: %d, 0",g_nOriMsgNumber);
        at_send_extended_text(szString);
    }    
    return 0xFE;
}

BYTE EIF_EmMO(BYTE* pszCommand)            // 비상용 MO 등록
{

    BYTE* pszSrc = pszCommand;
    INT   i, nComma;

    INT   nTimeOut, nTI;
    BYTE  szSendNumber[17], szRcvNumber[17], szMsg[81];
        
	if( pszSrc[0] == '=' )
	{
        szRcvNumber[0] = 0;
		pszSrc++;
		if( !isdigit( *pszSrc ) )
		{
			at_send_extended_text("*SKT*EMMO:0");
			return 0xFE;
		}	
		nTimeOut = at_atoi(&pszSrc);

		if( nTimeOut == 256 )
		{
			g_nPingTurn = 0;	
			if( g_fEmmo )
			{
				g_fEmmo = FALSE;
				clk_dereg(&PingTimeOutTimer);
			}
			at_send_extended_text("*SKT*EMMO:1");	
			return 0xFE;
		}
		else if( nTimeOut < 256 || nTimeOut > 4095 || g_nSetPing != 0)
		{
			at_send_extended_text("*SKT*EMMO:0");
			return 0xFE;
		}
		else ;

		if( *pszSrc !=',')
		{
			at_send_extended_text("*SKT*EMMO:0");
			return 0xFE;
		}

		pszSrc++;
		i = 0;
		while( pszSrc[i] !=',' )
		{
			if( pszSrc[i] == 0 )
			{
				at_send_extended_text("*SKT*EMMO:0");
				return 0xFE;
			}			
			i++;
		}
		STRNCPY(szSendNumber, pszSrc, i);
		szSendNumber[i] = 0;
 
  
		pszSrc += i+1;
		nComma  = 0;
		i = 0;
		while( pszSrc[i] != 0 )
		{
			if( pszSrc[i++] == ',' )
				nComma++;
		}
		if( nComma == 1 )
		{
			nTI = at_atoi(&pszSrc);
			pszSrc++;
		}
		else if( nComma == 2 )
		{
			i = 0;
			while( pszSrc[i] !=',' )
			{
				if( pszSrc[i] == 0 )
				{
					at_send_extended_text("*SKT*EMMO:0");
					return 0xFE;
				}			
				i++;
			}
			STRNCPY(szRcvNumber, pszSrc, i);
			szRcvNumber[i] = 0;

			pszSrc += i+1;
			nTI = at_atoi(&pszSrc);
			pszSrc++;
		}
		else
		{
			at_send_extended_text("*SKT*EMMO:0");
			return 0xFE;
		}	

		if( STRLEN(pszSrc) > MAX_SMS_TEXT)
		{
			STRNCPY(szMsg,pszSrc,MAX_SMS_TEXT);
			szMsg[MAX_SMS_TEXT] = 0;
		}
		else
		{
			STRCPY(szMsg,pszSrc);
		}

		g_nPingTurn = nTimeOut;
		STRCPY(g_szTermNumber, szSendNumber);
		STRCPY(g_szCallBackNumber, szRcvNumber);
		STRCPY(g_szEMMsg, szMsg);
		g_nEMTI = nTI;
		g_fEmmo = TRUE;
		CheckPingTimeOutTimer();
  
/*	
	    l_nTimeOut = nTimeOut;
		l_nTI = nTI;
		STRCPY(l_szSendNumber, szSendNumber);
		STRCPY(l_szRcvNumber, szRcvNumber);
		STRCPY(l_szMsg, szMsg);
*/
		at_send_extended_text("*SKT*EMMO:1");		
	}
	else if( pszSrc[0] == '?')
	{
		BYTE szOutMsg[120];
		if( g_fEmmo == FALSE || g_nPingTurn == 0 )// || szRcvNumber[0] == 0 )
		{
			STRCPY(szOutMsg,(BYTE*)"*SKT*EMMO: 0");
		}
		else
		{
			sprintf((char*)szOutMsg,"*SKT*EMMO: %d, %s, %s, %d, %s",\
			        g_nPingTurn, g_szTermNumber, g_szCallBackNumber, g_nEMTI, g_szEMMsg);
		}
		at_send_extended_text((char*) szOutMsg);
	}
	else
	{
		at_send_extended_text("*SKT*EMMO:0");
		return 0xFE;
	}

	return 0xFE;
		    
}


BYTE EIF_DelMO(BYTE* pszCommand)           // 송신한 단문 삭제
{
    BYTE    szBuffer[15];
	INT		nIndex;
	UINT8   nSize;
    
    if( !isdigit(*pszCommand) )
        return 4;

    nIndex = at_atoi(&pszCommand);

	GetMOSendSize(&nSize);
    if( nIndex < nSize )  //g_nMOSentSize )
    {
        //DeleteSMSMOSentMsg(nIndex);
		DeleteSMSMOSendMsg(nIndex);	
        sprintf((char*)szBuffer,"*SKT*DELMO: %d",nIndex);
        at_send_extended_text(szBuffer); 
    }
    else
        at_send_extended_text("*SKT*DELMO: 255"); 

    return 0xFE;
}
BYTE EIF_DelAllMO(BYTE* pszCommand)           // 송신한 단문 전체 삭제
{
    UINT8  nSize;

/*
    nSize = g_nMOSentSize;

    if( g_nMOSentSize == 0 )
    {
        at_send_extended_text("*SKT*DELALLMO: 0");
    }
    else
    {
        while( g_nMOSentSize )
        {    
            DeleteSMSMOSentMsg(0); //보낸 메시지 삭제 
        }

		//DeleteAllSendMsg();        //보낼 메시지 삭제 
        at_send_extended_text("*SKT*DELALLMO: 1");
    }
*/
	GetMOSendSize(&nSize);
    if( nSize == 0 )
    {
        at_send_extended_text("*SKT*DELALLMO: 0");
    }
    else
    {
        while( nSize )
        {   
			nSize--;
            DeleteSMSMOSendMsg(0); //보낼 메시지 삭제 
        }	
        at_send_extended_text("*SKT*DELALLMO: 1");
    }
    return 0xFE;
}

/* 단문 수신*/
BYTE EIF_MTCnt(BYTE* pszCommand)           // 수신된 단문 갯수 확인
{
    BYTE szString[40];
	sprintf((char*)szString, "*SKT*MTCNT: %d, %d",	
		ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt       //
	  + ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt   // 수신된 단문 개수
	  + ui_sms_list[UI_SMS_TXT_OLD].cnt ,             //		
		ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt		 //
	  + ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt); // 읽지 않은 단문 개수

	at_send_extended_text(szString);
    return 0xFE;
}

INT  sReadMT(UINT8 nIndex, BYTE* pszResult)
{
    nv_sms_type		nv_sms;
    smsi_msg_type	nv_smsi;

	BYTE    callback[33];
    BYTE    szBuffer[81];
	WORD	year;
	UINT8	month, day, hour, min, sec;
	UINT8   nv_addr;
	UINT8   nRcv;
	int		i;


	nRcv = nIndex;
	// Out of range
	if ( nIndex >= ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt + ui_sms_list[UI_SMS_TXT_OLD].cnt)
		return -1;

	// Find location
	if ( nIndex < ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt) 
    {		// New message
		i = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - nIndex - 1;
		nv_addr = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].nv_idx[i];
	}
	else 
    {															// Old message
		nIndex -= ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].cnt;
		i = ui_sms_list[UI_SMS_TXT_OLD].cnt - nIndex - 1;
		nv_addr = ui_sms_list[UI_SMS_TXT_OLD].nv_idx[i];
	}

	// Read MT from NV
	nv_sms.address = nv_addr;
	if (dsi_get_nv_item(NV_SMS_I, (nv_item_type*)&nv_sms) != NV_DONE_S)
		return -1;

	// Unpack SMS
	ui_sms_nv_data(FALSE, &nv_sms, &nv_smsi);	


	//DebugOut("\tTI=%d", nv_smsi.tele_srv); original

	// Date
	year  = BCD_2_ASCII(nv_smsi.msg_data.mc_time.yr);
	if (year < 99)
		year += 2000;
	else
		year += 1900;
	month =	BCD_2_ASCII(nv_smsi.msg_data.mc_time.mon);
	day   = BCD_2_ASCII(nv_smsi.msg_data.mc_time.day);
	hour  = BCD_2_ASCII(nv_smsi.msg_data.mc_time.hrs);
	min   = BCD_2_ASCII(nv_smsi.msg_data.mc_time.min);
	sec   = BCD_2_ASCII(nv_smsi.msg_data.mc_time.sec);
	
	memset(callback, 0, sizeof(callback));
	memcpy(callback, (void *)nv_smsi.msg_data.call_back.chari,\
           nv_smsi.msg_data.call_back.num_fields);
	memset(szBuffer, 0, sizeof(szBuffer));
	memcpy(szBuffer, (void *)nv_smsi.msg_data.user_data.chari,\
           nv_smsi.msg_data.user_data.num_fields);

	sprintf((char*)pszResult, "%d, %4d%02d%02d%02d%02d, %s, %d, %s",
		    nRcv, year, month, day, hour, min, callback,\
            uinuasms_convert_to_internal_ti(nv_smsi.tele_srv),szBuffer);//nv_smsi.tele_srv, szBuffer);

    return nv_smsi.tele_srv;
}


BYTE EIF_ReadMT(BYTE* pszCommand)          // 수신된 단문 읽기
{
    BYTE  szString[120];
    INT   nIndex;

    if( !isdigit(*pszCommand) )
        return 4;
    nIndex = at_atoi(&pszCommand);
    STRCPY(szString,"*SKT*READMT:");
    if( sReadMT(nIndex,szString+STRLEN(szString)) >= 0)
    {        
        at_send_extended_text(szString);
    }
    else
        at_send_extended_text("*SKT*READMT: 255,0,0,0,0");

    return 0xFE;
}


BOOL  sReadTI(INT nAddress, INT nTI, BYTE* pszBuffer)
{
    nv_sms_type		nv_sms;
    smsi_msg_type	nv_smsi;
	INT		MTIndex = 0;
	WORD    year;
	UINT8	month, day, hour, min, sec;
	BYTE	callback[16], szMsg[81];   
   

	// Read MT from NV
	nv_sms.address = nAddress;
	if (dsi_get_nv_item(NV_SMS_I, (nv_item_type*)&nv_sms) != NV_DONE_S)
		return FALSE;

	// Unpack SMS
	ui_sms_nv_data(FALSE, &nv_sms, &nv_smsi);	

	//DebugOut("\t%d TI=%d", i, nv_smsi.tele_srv);	original

	if( uinuasms_convert_to_internal_ti(nv_smsi.tele_srv) != nTI )
        return FALSE;

	year  = BCD_2_ASCII(nv_smsi.msg_data.mc_time.yr);
	if (year < 99)
		year += 2000;
	else
		year += 1900;
	month =	BCD_2_ASCII(nv_smsi.msg_data.mc_time.mon);
	day   = BCD_2_ASCII(nv_smsi.msg_data.mc_time.day);
	hour  = BCD_2_ASCII(nv_smsi.msg_data.mc_time.hrs);
	min   = BCD_2_ASCII(nv_smsi.msg_data.mc_time.min);
	sec   = BCD_2_ASCII(nv_smsi.msg_data.mc_time.sec);        

	memset(callback, 0, sizeof(callback));
	memcpy(callback, nv_smsi.msg_data.call_back.chari, nv_smsi.msg_data.call_back.num_fields);
	memset(szMsg, 0, sizeof(szMsg));
	memcpy(szMsg, nv_smsi.msg_data.user_data.chari, nv_smsi.msg_data.user_data.num_fields);

   
    
	sprintf(pszBuffer, "*SKT*READTI: %d, %4d%02d%02d%02d%02d, %s, %d, %s",
		               g_nReadTI, year, month, day, hour, min, callback,\
		               uinuasms_convert_to_internal_ti(nv_smsi.tele_srv), szMsg);

	g_nReadTI++;

    return TRUE;

}

BYTE EIF_ReadTI(BYTE* pszCommand)          // 수신된 단문 익기(특정 TI)
{

	INT		nTI;
	INT		MTIndex = 0;
	INT		MTListNum; // Total MT Messages
	UINT8	nv_addr;
	BYTE	szBuffer[120];   
	BYTE	i, j, k;  


    if( !isdigit(*pszCommand) )
        return 4;
	g_nReadTI = 0;

    nTI = at_atoi(&pszCommand);

	MTListNum = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt \
                + ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt\
                + ui_sms_list[UI_SMS_TXT_OLD].cnt;

	for (i = 0;  i < ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt
				     + ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt
		             + ui_sms_list[UI_SMS_TXT_OLD].cnt; i++) 
    {

		MTIndex = i;
		/* SISI Modified on 00/03/28 */
		// Find location
		if (MTIndex < MTListNum - ui_sms_list[UI_SMS_TXT_OLD].cnt) 
        { // New message
			if(ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0) 
            {
				if(ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0) 
                {
					if(MTIndex >= ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) 
                    {						
						j = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - \
                            (MTIndex - ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) - 1;
						nv_addr = ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].nv_idx [j];				
					}
					else 
                    {
						j = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - MTIndex - 1;
						nv_addr = ui_sms_list [UI_SMS_TXT_NEW_URGENT].nv_idx [j]; 				
					}
				}
				else 
                {
					j = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - MTIndex - 1;
					nv_addr = ui_sms_list [UI_SMS_TXT_NEW_URGENT].nv_idx [j]; 
				}
			}
			else 
            {				
				j = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - MTIndex - 1;
				nv_addr = ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].nv_idx [j];			
			}
		}
		else 
        {													  // Old messages
			j = MTIndex;
			j -= ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt \
                + ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt;
			k = ui_sms_list[UI_SMS_TXT_OLD].cnt - j - 1;
			nv_addr = ui_sms_list[UI_SMS_TXT_OLD].nv_idx[k];
		}

        if( sReadTI(nv_addr, nTI, szBuffer) == TRUE )
        {
            at_send_extended_text(szBuffer);
        }

	}

	// Update icon
	ui_sms_update_icons();

    return 0xFE;
}

BOOL sDeleteMT(UINT8 MTIndex)
{
    nv_sms_type		nv_sms;    
	int		list, nIndex, MTListNum;
	int		i;

	MTListNum = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt +\
                ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt +\
                ui_sms_list[UI_SMS_TXT_OLD].cnt;

	// Out of range
	if (MTIndex >= MTListNum ) //|| MTIndex)
		return FALSE;
    

	/* SISI Modified on 00/03/28 */
	// Find location
	if (MTIndex < MTListNum - ui_sms_list[UI_SMS_TXT_OLD].cnt) 
    { // New message
		if(ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0) 
        {
			if(ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0) 
            {
				if(MTIndex >= ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) 
                {					
					nIndex = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt -\
                             (MTIndex - ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) - 1;
					list = UI_SMS_TXT_NEW_NOT_URGENT;
				}
				else 
                {
					nIndex = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt \
                             - MTIndex - 1;
					list = UI_SMS_TXT_NEW_URGENT;
				}
			}
			else 
            {
				nIndex = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - MTIndex - 1;
				list = UI_SMS_TXT_NEW_URGENT;
			}
		}
		else 
        {			
			nIndex = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - MTIndex - 1;
			list = UI_SMS_TXT_NEW_NOT_URGENT;
		}
	}
	else 
    {													  // Old messages		
		MTIndex -= ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt \
                   + ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt;
		nIndex = ui_sms_list[UI_SMS_TXT_OLD].cnt - MTIndex - 1;
		list = UI_SMS_TXT_OLD;
	}

	// Free NV
	nv_sms.address = ui_sms_list[list].nv_idx [nIndex];

	// Free NV memory
	if (nv_sms.address <= MAX_NV_MSGS)	// Check valid address
		//ui_free_nv(NV_SMS_I, (nv_item_type*)&nv_sms);
        SKY_FreeNV(NV_SMS_I, (nv_item_type*)&nv_sms);

	// Move token to avail list
	ui_sms_list[UI_SMS_AVAIL].nv_idx[ui_sms_list[UI_SMS_AVAIL].cnt] = ui_sms_list [list].nv_idx[nIndex];
	ui_sms_list[UI_SMS_AVAIL].cnt++;

	// Repack
	for (i = nIndex + 1; i < ui_sms_list[list].cnt; i++)
		ui_sms_list[list].nv_idx[i - 1] = ui_sms_list [list].nv_idx[i];
	ui_sms_list[list].cnt--;

    return TRUE;
}

BYTE EIF_DelMT(BYTE* pszCommand)           // 수신된 단문 삭제
{
    INT  nIndex;

	if( !isdigit(*pszCommand) )
        return 4;

	nIndex = at_atoi(&pszCommand);

	if( sDeleteMT(nIndex) )
	{
		at_send_extended_text("*SKT*DELMT:1");		
		ui_sms_update_icons();  // Update icon
	}
	else
	{
		at_send_extended_text("*SKT*DELMT:0");
	}

		
    ui_sms_update_icons();  // Update icon	
	
    return 0xFE;
}
BYTE EIF_DelAllMT(BYTE* pszCommand)        // 수신된 단문 전체 삭제
{

	while (ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt + ui_sms_list[UI_SMS_TXT_OLD].cnt > 0)
		sDeleteMT(0);

	at_send_extended_text("*SKT*DELALLMT: 1");
	
	ui_sms_update_icons(); // Update icon

    return 0xFE;
}

BYTE EIF_NewMsg(BYTE* pszCommand)          // 단문 메시지 수신 즉시 알림 설정 해제 및 확인
{
	int	   i, nTI;
	BYTE   *pSrc = pszCommand;
    BYTE   szString[200];

	switch (*pSrc)	
    {
	case	'=':
		pSrc++;
		g_nTIMsgSize = 0;
		while ( g_nTIMsgSize < MAX_NEW_MSG_TI )
        {
			nTI = at_atoi(&pSrc);
			if (nTI == 0) 
            {
				g_nTIMsgSize= 0;
				break;
			}
			g_nTIMsg[g_nTIMsgSize++] = nTI;
			if ( *pSrc != ',')
				break;
            else if( *pSrc == 0 )
                break;
            else
			    pSrc++;
		}
		break;				
  
	case	'?':
		pSrc++;
		break;

	default	:
		return 4;					// Return ERROR
	}

	if ( g_nTIMsgSize == 0) 
    { 
        g_fSMSNewMsg = FALSE;
		at_send_extended_text("*SKT*NEWMSG: 0");
	}
	else 
    {
        g_fSMSNewMsg = TRUE;
		STRCPY(szString,"*SKT*NEWMSG:");       
		for (i = 0; i < g_nTIMsgSize; i++) 
        {
            pSrc = szString + STRLEN(szString);
			sprintf(pSrc, "%d", g_nTIMsg[i]);
			if ( i < g_nTIMsgSize - 1 )
            {
                pSrc = szString + STRLEN(szString);
				sprintf(pSrc, ", ");
            }
		}
        at_send_extended_text((char*)szString);
	}
	
	return 0xFE;
}

BYTE EIF_MTAck(BYTE* pszCommand) 
{
    INT nRcvNumber;

    nRcvNumber = at_atoi(&pszCommand);

    if ( g_fNewMsgTimerAlive == FALSE)
		return 4;

	if ( g_fNewMsgTimerAlive == TRUE) 
    {
		clk_dereg(&NewMsgTimer);
		g_fNewMsgTimerAlive = FALSE;
	}
	//skt_readmt(0);

	//if (nv_smsi.tele_srv == SMSI_GRAPHIC)
	//	skt_delmt(0);
	return 0xFE;

}


 /* LCD 제어 */
BYTE EIF_SetLCD(BYTE* pszCommand)          // LCD 제어권 설정
{

    UINT8 nCol, nRow;
    char  szString[20];
    BYTE* szSrc = pszCommand;
    int   nNumber;
	

	switch (*szSrc)	{

	case  '=':
			szSrc++;
			nNumber = at_atoi(&szSrc);
            if( nNumber == 0 )
            {
                g_fSetLCD = FALSE;              
                SendEIFCommand(SE_RESET_LCD);
            }
            else if( nNumber == 1 )
            {
                g_fSetLCD = TRUE;
                SendEIFCommand(SE_SET_LCD);                      
            }
            else 
                return 4;

            break;

	case '?':			
			break;

	default	:
			return 4;					// Return ERROR
	}

    if( g_fSetLCD )
        nNumber = 1;
    else
        nNumber = 2;
    nCol = EQS_GetMaxWidth()  / EQS_CharWidth((BYTE)' ');
    nRow = EQS_GetMaxHeight() / EQS_CharHeight((BYTE)' ');
	nRow -= 1;
	sprintf(szString, "*SKT*SETLCD:%d, %d, %d", nNumber,nCol,nRow);
	at_send_extended_text(szString);
    return 0xFE;
}

BYTE EIF_ClrLCD(BYTE* pszCommand)          // LCD 지우기
{

    if( g_fSetLCD == TRUE )
    {      
        EIF_Event = SE_CLEAR_LCD;
        if( SendEIFCommand(SE_CLEAR_LCD) )
        {
            at_send_extended_text("*SKT*CLRLCD: 1");
	    }
        else
            at_send_extended_text("*SKT*CLRLCD: 0");        
    }
    else
        at_send_extended_text("*SKT*CLRLCD: 0");
    return 0xFE;
}

BYTE EIF_OutLCD(BYTE* pszCommand)          // LCD 에 문자 출력
{
    UINT8 nCol, nRow;
    BYTE* pszSrc = pszCommand;

    g_nOutX = at_atoi(&pszSrc);
    if( *pszSrc != ',')
        return 4;
    pszSrc++;
    g_nOutY = at_atoi(&pszSrc);
    if( *pszSrc != ',')
        return 4;
    pszSrc++;
    STRCPY( g_szOutBuffer, pszSrc);
    nCol = EQS_GetMaxWidth()  / EQS_CharWidth((BYTE)' ');
    nRow = EQS_GetMaxHeight() / EQS_CharHeight((BYTE)' ');
    if( g_nOutX >= nCol || g_nOutY >= nRow  )
    {
        g_nOutX = 0;
        g_nOutY = 0;
        g_szOutBuffer[0] = 0;
        at_send_extended_text("*SKT*OUTLCD: 0");
    }
    else
    {        
        EIF_Event = SE_DRAW_LCD;
        if( SendEIFCommand(SE_DRAW_LCD) )
        {
            at_send_extended_text("*SKT*OUTLCD: 1");
        }
        else
            at_send_extended_text("*SKT*OUTLCD: 0");

    }

    return 0xFE;
}


/* KeyPad 제어*/
BYTE EIF_SetKey(BYTE* pszCommand)          // 키패드 관련 제어 상태 선택 
{

    BYTE* pszSrc = pszCommand;
    int   nNumber;
    BYTE  szString[20];
	
	switch (*pszSrc)	
    {
	case '=':
		pszSrc++;
		nNumber = at_atoi(&pszSrc);
        
		if (nNumber != 0 && nNumber != 1 && nNumber != 2)  //  0:해제, 1: key emulation , 2: key scan
			return 4;				// Return ERROR
        g_nKeyFlag = nNumber;
		break;

	case '?':		
		break;

	default	:
		return 4;					// Return ERROR
	}

	sprintf(szString, "*SKT*SETKEY: %d", g_nKeyFlag);
	at_send_extended_text(szString);

    return 0xFE;
}
BYTE EIF_KeyEmul(BYTE* pszCommand)         // Keypad Emulation
{
    BYTE* pszSrc = pszCommand;
	int   k,j;
    
    if( g_nKeyFlag != 1 )
    {
        at_send_extended_text("KEYEMUL: 0");
        return  0xFE;
    }

    while( *pszSrc != 0 )
    {                   
        switch( toupper(*pszSrc) )
        {
        case '(':
            SendEIFCommand(SE_KEY_PHONE_OPEN);
            break;
        case ')':        
            SendEIFCommand(SE_KEY_PHONE_CLOSE);
            break; 

        case 'U':
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_UP);
                SendEIFCommand(SE_KEY_UP_LONG);
                pszSrc++;
            }            
            else
                SendEIFCommand(SE_KEY_UP);
            break;
        case 'D':
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_DOWN);
                SendEIFCommand(SE_KEY_DOWN_LONG);
                pszSrc++;
            }            
            else
                SendEIFCommand(SE_KEY_DOWN);
            break;
        case 'B':  			
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_LEFT);
                SendEIFCommand(SE_KEY_LEFT_LONG);
                pszSrc++;
            }            
            else
                SendEIFCommand(SE_KEY_LEFT);
            break;
        case 'F':
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_RIGHT);
                SendEIFCommand(SE_KEY_RIGHT_LONG);
                pszSrc++;
            }            
            else
                SendEIFCommand(SE_KEY_RIGHT);
            break;
        case 'W': // N.TOP KEY
            if( pszSrc[1] == 'L')	//nate접속이 long키로 변경됨.
            {
                //IDLE 상태 일 경우, n.Top접속 - 종료시 'EXIT'보냄
                if(EQS_GET_APPLET() == SKA_MainIdleScreen || EQS_GET_APPLET() == SKA_IdleDebugScreen)
                {
                    g_EIF_SEND_EXIT = TRUE;
                }

                SendEIFCommand(SE_KEY_F2_LONG);
                pszSrc++;
            }            
            else
            {
                SendEIFCommand(SE_KEY_F2);
            }
            break;
        case 'H':
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_F1);
                SendEIFCommand(SE_KEY_F1_LONG);
                pszSrc++;
            }            
            else
                SendEIFCommand(SE_KEY_F1);
            break;
        case '#':   
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_POUND);
                SendEIFCommand(SE_KEY_POUND_LONG);
                pszSrc++;
            }            
            else
                SendEIFCommand(SE_KEY_POUND);
            break;        
        case '*':        
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_STAR);
                SendEIFCommand(SE_KEY_STAR_LONG);
				SendEIFCommand(SE_KEY_MANNERS);
                pszSrc++;
            }
            else
                SendEIFCommand(SE_KEY_STAR);
            break;               
        case 'S':        

            SendEIFCommand(SE_KEY_SEND);
            break;        
        case 'E':        
            if( toupper(pszSrc[1]) == 'L')
            {
				SendEIFCommand(SE_KEY_END);
                SendEIFCommand(SE_KEY_END_LONG);
                pszSrc++;
            }
            else
                SendEIFCommand(SE_KEY_END);
            break;
        
        case 'C':        
            if( toupper(pszSrc[1]) == 'L' )
            {
				SendEIFCommand(SE_KEY_CLEAR); 
                SendEIFCommand(SE_KEY_CLEAR_LONG);
                pszSrc++;
            }
            else
                SendEIFCommand(SE_KEY_CLEAR);        
            break;
        case 'P':
            if( toupper(pszSrc[1]) == 'L' )
            {
				SendEIFCommand(SE_KEY_PLAY); 
                SendEIFCommand(SE_KEY_PLAY_LONG);
                pszSrc++;
            }
            else
                SendEIFCommand(SE_KEY_PLAY);   
            break;
        case 'A':
            if( toupper(pszSrc[1]) == 'L' )
            {
                SendEIFCommand(SE_KEY_AUTO_ANSWER_LONG);
                pszSrc++;
            }
            else
                SendEIFCommand(SE_KEY_AUTO_ANSWER);
            break;
        case 'R':
            if( toupper(pszSrc[1]) == 'L' )
            {		
#if 0
				SendEIFCommand(SE_KEY_RECORD);
				j = 10;
				for(k=0; k<100000; k++)
					j = j*j;
				SendEIFCommand(SE_APP_DRAW);
				j = 10;
				for(k=0; k<100000; k++)
					j = j*j;
#endif

                SendEIFCommand(SE_KEY_RECORD_LONG);
                pszSrc++;
            }
            else
                SendEIFCommand(SE_KEY_RECORD);   
            break;
		case 'N':
            if( toupper(pszSrc[1]) == 'L' )
            {
				SendEIFCommand(SE_KEY_CENTER);
                SendEIFCommand(SE_KEY_CENTER_LONG);
                pszSrc++;
            }
			else if(pszSrc[1] == '\0')//'N'만 들어왔을때...
			{
                SendEIFCommand(SE_KEY_CENTER);
			}
			else
			{
				if (g_NKey_UOPEN == TRUE)
				{
					extern void NKey_uopen_command(int whatKey);
					switch(pszSrc[1]) {
						case '1':
							NKey_uopen_command(1);
							break;
						case '2':
							NKey_uopen_command(2);
							break;
						case '3':
							NKey_uopen_command(3);
							break;
						case '4':
							NKey_uopen_command(4);
							break;
						case '5':
							NKey_uopen_command(5);
							break;
						default:
							at_send_extended_text("Bad Command");
							return 4;
					}
		            pszSrc++;
				}
				else
				{
					at_send_extended_text("Bad Command");
					return 4;
				}
			}
			break;
		case 'T':
			{
				extern void LinkText_open_command(int whatNumber);
				int link_txt_num = 0;

				if( isdigit(pszSrc[1]))
				{
					link_txt_num = pszSrc[1] - '0';
					if (g_Link_Text_UOPEN == TRUE)
					{
						LinkText_open_command(link_txt_num);
						pszSrc++;
					}
					else
					{
						at_send_extended_text("Bad Command");
						return 4;
					}
				}
				else
				{
					at_send_extended_text("Bad Command");
					return 4;
				}
			}
			break;
        default:
             
            if( isdigit(*pszSrc) )
            {
                if( toupper(pszSrc[1]) == 'L')
                {
                    //SendEIFCommand((SKY_EVENT_E)(SE_KEY_0_LONG + *pszSrc -'0'));
					SendEIFCommand((SKY_EVENT_E)(SE_KEY_0 + *pszSrc -'0'));
					SendEIFCommand((SKY_EVENT_E)(SE_KEY_0_LONG + *pszSrc -'0'));
					SendEIFCommand((SKY_EVENT_E)(SE_KEY_DIGIT_LONG));
                    pszSrc +=1;
                }
                else
                {
                    SendEIFCommand((SKY_EVENT_E)(SE_KEY_0 + *pszSrc -'0'));
                }
            }
            else 
                return 4;
        }
        pszSrc++;
    }
    at_send_extended_text("*SKT*KEYEMUL: 1");

    return 0xFE;
}

BOOL EIF_KeyScan(INT KeyID)         // Keypad Scan
{

    SKY_EVENT_E EventID;

    EventID = (SKY_EVENT_E) KeyID;
    switch( EventID )
    {
    case SE_KEY_PHONE_OPEN:
        at_send_extended_text("*SKT*KEYSCAN:(");
        break;
    case SE_KEY_PHONE_CLOSE:
        at_send_extended_text("*SKT*KEYSCAN:)");
        break;
    case SE_KEY_F1:
        at_send_extended_text("*SKT*KEYSCAN:H");
        break;
    case SE_KEY_F2:   
        at_send_extended_text("*SKT*KEYSCAN:W"); // ntop 
        break;        
    case SE_KEY_UP:
        at_send_extended_text("*SKT*KEYSCAN:U");
        break;
    case SE_KEY_DOWN:
        at_send_extended_text("*SKT*KEYSCAN:D");
        break;
    case SE_KEY_LEFT:
        at_send_extended_text("*SKT*KEYSCAN:B");
        break;
    case SE_KEY_RIGHT:
        at_send_extended_text("*SKT*KEYSCAN:F");
        break;
    case SE_KEY_CENTER:
        at_send_extended_text("*SKT*KEYSCAN:N");
        break;
    case SE_KEY_0:
        at_send_extended_text("*SKT*KEYSCAN:0");
        break;
    case SE_KEY_1:
        at_send_extended_text("*SKT*KEYSCAN:1");
        break;
    case SE_KEY_2:
        at_send_extended_text("*SKT*KEYSCAN:2");
        break;
    case SE_KEY_3:
        at_send_extended_text("*SKT*KEYSCAN:3");
        break;
    case SE_KEY_4:
        at_send_extended_text("*SKT*KEYSCAN:4");
        break;
    case SE_KEY_5:
        at_send_extended_text("*SKT*KEYSCAN:5");
        break;
    case SE_KEY_6:
        at_send_extended_text("*SKT*KEYSCAN:6");
        break;
    case SE_KEY_7:
        at_send_extended_text("*SKT*KEYSCAN:7");
        break;
    case SE_KEY_8:
        at_send_extended_text("*SKT*KEYSCAN:8");
        break;
    case SE_KEY_9:
        at_send_extended_text("*SKT*KEYSCAN:9");
        break;
    case SE_KEY_POUND:
        at_send_extended_text("*SKT*KEYSCAN:#");
        break;
    case SE_KEY_STAR:   
        at_send_extended_text("*SKT*KEYSCAN:*");
        break;
    case SE_KEY_SEND:
        at_send_extended_text("*SKT*KEYSCAN:S");
        break;
    case SE_KEY_END:
        at_send_extended_text("*SKT*KEYSCAN:E");
        break;
    case SE_KEY_CLEAR:
        at_send_extended_text("*SKT*KEYSCAN:C");
        break;
    case SE_KEY_PLAY:
        at_send_extended_text("*SKT*KEYSCAN:P");
        break;
    case SE_KEY_AUTO_ANSWER:
        at_send_extended_text("*SKT*KEYSCAN:A");
        break;
    case SE_KEY_RECORD:
        at_send_extended_text("*SKT*KEYSCAN:R");
        break;

    case SE_KEY_F1_LONG:
        at_send_extended_text("*SKT*KEYSCAN:HL");
        break;
    case SE_KEY_F2_LONG:    
        at_send_extended_text("*SKT*KEYSCAN:WL");
        break;
    case SE_KEY_UP_LONG:
        at_send_extended_text("*SKT*KEYSCAN:UL");
        break;
    case SE_KEY_DOWN_LONG:
        at_send_extended_text("*SKT*KEYSCAN:DL");
        break;
    case SE_KEY_LEFT_LONG:
        at_send_extended_text("*SKT*KEYSCAN:BL");
        break;
    case SE_KEY_RIGHT_LONG:
        at_send_extended_text("*SKT*KEYSCAN:FL");
        break;
    case SE_KEY_CENTER_LONG:
        at_send_extended_text("*SKT*KEYSCAN:NL");
        break;
    case SE_KEY_0_LONG:
        at_send_extended_text("*SKT*KEYSCAN:0L");
        break;
    case SE_KEY_1_LONG:
        at_send_extended_text("*SKT*KEYSCAN:1L");
        break;
    case SE_KEY_2_LONG:
        at_send_extended_text("*SKT*KEYSCAN:2L");
        break;
    case SE_KEY_3_LONG:
        at_send_extended_text("*SKT*KEYSCAN:3L");
        break;
    case SE_KEY_4_LONG:
        at_send_extended_text("*SKT*KEYSCAN:4L");
        break;
    case SE_KEY_5_LONG:
        at_send_extended_text("*SKT*KEYSCAN:5L");
        break;
    case SE_KEY_6_LONG:
        at_send_extended_text("*SKT*KEYSCAN:6L");
        break;
    case SE_KEY_7_LONG:
        at_send_extended_text("*SKT*KEYSCAN:7L");
        break;
    case SE_KEY_8_LONG:
        at_send_extended_text("*SKT*KEYSCAN:8L");
        break;
    case SE_KEY_9_LONG:
        at_send_extended_text("*SKT*KEYSCAN:9L");
        break;
    case SE_KEY_POUND_LONG:
        at_send_extended_text("*SKT*KEYSCAN:#L");
        break;
    case SE_KEY_STAR_LONG:
        at_send_extended_text("*SKT*KEYSCAN:*L");
        break;
    case SE_KEY_END_LONG:
        at_send_extended_text("*SKT*KEYSCAN:EL");
        break;
    case SE_KEY_CLEAR_LONG:
        at_send_extended_text("*SKT*KEYSCAN:CL");
        break;
    case SE_KEY_PLAY_LONG:
        at_send_extended_text("*SKT*KEYSCAN:PL");
        break;
    case SE_KEY_AUTO_ANSWER_LONG:
        at_send_extended_text("*SKT*KEYSCAN:AL");
        break;
    case SE_KEY_RECORD_LONG:
        at_send_extended_text("*SKT*KEYSCAN:RL");
        break;
    default:
        return FALSE;        
    }
    at_send_extended_text("OK");
    return TRUE;

}

void BeepTimerCallBack(int4 ms_interval)
{		
	if (nIteration >= g_nBeepNumber)
	{
		clk_dereg(&BeepTimer);
		nIteration = 0; // Initialize
		return;
	}
	//uisnd_snd(UI_BEEP_SND, SND_SVC_CHANGE); // BEEP~!!
	//SendEIFCommand( SE_EIF_PLAY_BEEP );	
    //SKY_PlayTone(UI_BEEP_SND,SND_WARN,300);

	SKY_PlayKeySound(SE_KEY_0,200);
	//SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5,300,4);
	nIteration++;
}

/* 부저 제어 */
BYTE EIF_Beep(BYTE* pszCommand)            // 부저
{
    INT  nNumber;
          
    nNumber =  at_atoi(&pszCommand);
    if( nNumber < 256 && nNumber >=0 )
    {
        g_nBeepNumber = nNumber ;
		//clk_def(&BeepTimer);
		// BEEP_INTERVAL = 0.5 sec
        if( SKY_IS_ETIQUETTE() == FALSE )
		{
			nIteration = 0;
		    clk_reg(&BeepTimer, BeepTimerCallBack, BEEP_INTERVAL, BEEP_INTERVAL, TRUE);
		}
        at_send_extended_text("*SKT*BEEP: 1");
    }
    else
        at_send_extended_text("*SKT*BEEP: 0");

    return 0xFE;
}

BYTE EIF_Bell(BYTE* pszCommand)            // 부저 착신음
{
    BYTE*  pszSrc = pszCommand;
    BYTE   szString[20];
    UINT8  nNumber;
    
    switch( *pszSrc )
    {
    case '=':
        pszSrc++;
        nNumber = at_atoi(&pszSrc);
        if( nNumber  == 1 )
        {
            g_fBell = TRUE;
        }
        else if( nNumber == 0 )
        {
            g_fBell = FALSE;
        }
        else 
            return 4;

        if( SKY_IS_ETIQUETTE() == FALSE )
        {
            if( g_fBell )
				SendEIFCommand( SE_EIF_PLAY_BELL );                
            else
				SendEIFCommand( SE_EIF_STOP_BELL );
        }               
        break;
    case '?':
        break;
    default:
        return 4;
    }
	sprintf(szString, "*SKT*BELL: %d", g_fBell);
	at_send_extended_text(szString);

    return 0xFE;
}


/* WAP 단말 기능 */
BYTE EIF_IWFCon(BYTE* pszCommand)          // IWF 접속 상태 확인 기능
{
    if(SKY_IS_IN_WAP())//SKY_IS_IN_USE())    //WAP 접속상태일 경우 
    {
		CLNTa_log(1,0, "WAP 상태: iwfcon_command() is called!!");
        iwfcon_command();        
    }
    else if(EQS_GET_APPLET() == SKA_MainIdleScreen || EQS_GET_APPLET() == SKA_IdleDebugScreen )    //IDLE 상태일 경우
    {
        g_nEIFWapstatus = EIF_WAP_IWFCON;
        
        CLNTa_log(1,0, "IDLE 상태: iwfcon_command() is called!!");
        SendEIFCommand(SE_KEY_F2_LONG);//SE_KEY_F2);//EQS_START_CHILD( SKA_MainWAP );        
    }
    else
    {
        eif_error_command(EIF_ERR_IWFCON_NOT_IDLE);    
    }

    return 0xFE;
}

BYTE EIF_WAPCon(BYTE* pszCommand)			// WAP 접속 상태 확인 기능
{
    BYTE* pszSrc = pszCommand;
	INT		i;
	char wapconBuf[128];

	memset((void*)EIF_proxyurl, NULL, sizeof(EIF_proxyurl)); // initializing
	memset((void*)EIF_url, NULL, sizeof(EIF_url)); // initializing
	memset((void*)wapconBuf, 0, sizeof(wapconBuf)); // initializing

	while(1) {
		
		i = 0;
		while(pszSrc[i] != ',')		
		{
			i++;
		}

		strncpy((char *) EIF_proxyurl,(char *)pszSrc, i); 
		pszSrc = &pszSrc[i];

		// 1st Comma Separation
		if (*pszSrc != ',')
			break;
		pszSrc++;

		// Destination(URL or IP Address)
		i = strlen((char *)pszSrc);
		if(i <= 0) break;

		strcpy((char*)wapconBuf, (char*)pszSrc);

		if(SKY_IS_IN_WAP())//SKY_IS_IN_USE())    //WAP 접속상태일 경우 
		{
			g_nEIFWapstatus = EIF_WAP_WAPCON;
			strncpy(EIF_url, wapconBuf, sizeof(EIF_url));

			//CLNTa_log(1,0, "WAP 상태: wapcon_command() is called!!");
               
		}
	/*    else if(EQS_GET_APPLET() == SKA_MainIdleScreen || EQS_GET_APPLET() == SKA_IdleDebugScreen )    //IDLE 상태일 경우
		{
			g_nEIFWapstatus = EIF_WAP_IWFCON;
        
			CLNTa_log(1,0, "IDLE 상태: iwfcon_command() is called!!");
			SendEIFCommand(SE_KEY_F2);//EQS_START_CHILD( SKA_MainWAP );        
		}
	*/    else
		{
			eif_error_command(EIF_ERR_WAPCON_NOT_IN_WAP);    
		}

		return 0xFE;

	} // End of WHILE - statement
	
	at_send_extended_text("ERROR : Abnormal Argument");

    return 0xFE;
}

BYTE EIF_Input(BYTE* pszCommand)           // 입력상자에 입력하기
{
	static word input_text_size;
	char inputBuf[MAX_INPUT_SIZE];

	memset((void*)inputBuf, 0, sizeof(inputBuf)); // initializing

	input_text_size = strlen((char*)pszCommand);

	if(input_text_size <= MAX_INPUT_SIZE)
	{
		memcpy((void*)inputBuf, (void*)pszCommand, input_text_size);		

		input_command((char*)inputBuf);
	}
	else
	{
		at_send_extended_text("INPUT buffer overflow");
	}

    return 0xFE;
}

BYTE EIF_Select(BYTE* pszCommand)          // Option 선택
{

	memset((void*)selectBuf, 0, sizeof(selectBuf)); // initializing
	strcpy((char*)selectBuf, (char*)pszCommand);	

    SendEIFCommand(SE_WAP_EIF_SELECT);
    return 0xFE;
}

/* URL 접속 관련 기능 */
BYTE EIF_DelCache(BYTE* pszCommand)        // Cache Memory Clear
{    
	// extern variable and functions for Cache clear!!
#ifdef FEATURE_WAP408
	extern void MEMc_initCache (UINT32 cacheSize);
#else
    extern void MEMc_initCache (memSizeType cacheSize, memSizeType restoredSize);
#endif //#ifdef FEATURE_WAP408
	extern void initCache(void);
    extern const    memSizeType   cCacheSize;
    extern const    memSizeType   crestoredSize;
	
    if(destinationIP != 0)//if((wapip != 0))//&&(SKY_IsInCall()))  //if ((wapip != 0)&&(ui.call))
    {
#ifdef FEATURE_WAP408
		MEMc_initCache(cCacheSize);	// browser cache clear
#else
        MEMc_initCache(cCacheSize, crestoredSize);	// browser cache clear
#endif //#ifdef FEATURE_WAP408
		ProcessEvents();

		initCache();

		at_send_extended_text("*SKT*DELCACHE:1");
	}
	else
	{
		at_send_extended_text("*SKT*DELCACHE:0");
	}

    return 0xFE;
}
BYTE EIF_UPing(BYTE* pszCommand)           // URL Ping
{
    BYTE* pszSrc = pszCommand;
	INT		i;
	char upingBuf[128];

	memset((void*)upingBuf, 0, sizeof(upingBuf)); // initializing
	memset((void*)EIF_url, NULL, sizeof(EIF_url)); // initializing

	while(1) {
		
		if( !isdigit( *pszSrc ) )
				break;

		// Ping Count
		i = PingCnt;
		PingCnt = at_atoi( &pszSrc );
		if(PingCnt > 255)//if(PingCnt < 0|| PingCnt > 255)  
			break;
		if(i == 255) //무한 uping샐행 중이면 Count값을 0으로 입력하여 무한 uping종료
			PingCnt = 0;

		// 1st Comma Separation
		if (*pszSrc != ',')
			break;
		pszSrc++;

		// Timeout value
		nTimeout = at_atoi( &pszSrc );
		if(nTimeout < 0 || nTimeout > 32*1000)//if(nTimeout > 32*1000)
			break;

		// 2nd Comma Separation
		if (*pszSrc != ',')
			break;
		pszSrc++;

		// Skip "32," - Packetsize
		i = at_atoi( &pszSrc );
		if(i < 0) break;

		if (*pszSrc != ',')
			break;	
		pszSrc++;		

		// Destination(URL or IP Address)
		i = strlen((char *)pszSrc);
		if(i <= 0) break;

		strcpy((char*)upingBuf, (char*)pszSrc);

		//EIF_WAP_start(3, upingBuf);
		if(SKY_IS_IN_WAP())//SKY_IS_IN_USE())    //WAP 접속상태일 경우 
		{
			g_nEIFWapstatus = EIF_WAP_UPING;

			strncpy(EIF_url, upingBuf, sizeof(EIF_url));		
			CLNTa_log(1,0, "WAP 상태: uping_command() is called!!");
		}
		else if(EQS_GET_APPLET() == SKA_MainIdleScreen || EQS_GET_APPLET() == SKA_IdleDebugScreen)    //IDLE 상태일 경우
		{
			g_nEIFWapstatus = EIF_WAP_UPING;
			strncpy(EIF_url, upingBuf, sizeof(EIF_url));		
		
			CLNTa_log(1,0, "IDLE 상태: uping_command() is called!!");
			SendEIFCommand(SE_KEY_F2_LONG);//SE_KEY_F2);//EQS_START_CHILD( SKA_MainWAP );        
		}
		else
		{
			CLNTa_log(1,0, "ERROR: uping_command() is called!!");
			return 4;			    
		}

		return 0xFE;

	} // End of WHILE - statement
	
	at_send_extended_text("ERROR : Abnormal Argument");

    return 0xFE;
}
BYTE EIF_UOpen(BYTE* pszCommand)           // URL Open
{
	char urlBuf[128];
	memset((void*)urlBuf, 0, sizeof(urlBuf)); // initializing

	strcpy((char*)urlBuf, (char*)pszCommand);	

	//ui_backlight( TRUE, UI_BKLT_ON ); /* turn on backlighting */

	if(SKY_IS_IN_WAP())//SKY_IS_IN_USE())    //WAP 접속상태일 경우 
	{
    	if(destinationIP != 0)//if ((wapip != 0))//&&(SKY_IsInCall()))//if ((wapip != 0)&&(ui.call))	// WAP connection OK!!
		{
		    g_nEIFWapstatus = EIF_WAP_NONE;
		    strncpy(EIF_url, urlBuf, sizeof(EIF_url));
		    CLNTa_log(1, 0, "[%s] in EIF_WAP_start to UOPEN", EIF_url);
			uopen_command(EIF_url);
		}
        else
            at_send_extended_text("*SKT*UOPEN:0");
	}
	else if(EQS_GET_APPLET() == SKA_MainIdleScreen || EQS_GET_APPLET() == SKA_IdleDebugScreen)    //IDLE 상태일 경우
	{
		g_nEIFWapstatus = EIF_WAP_UOPEN;
        g_EIF_SEND_EXIT = TRUE;
		strncpy(EIF_url, urlBuf, sizeof(EIF_url));
		CLNTa_log(1, 0, "[%s] in EIF_WAP_start to UOPEN", EIF_url);

		SendEIFCommand(SE_KEY_F2_LONG);//SE_KEY_F2);//EQS_START_CHILD( SKA_MainWAP );        
	}
	else
	{
		at_send_extended_text("*SKT*UOPEN:0");
		CLNTa_log(1,0, "ERROR: uopen_command() is called!!");
		return 4;			    
	}

    return 0xFE;
}

BYTE EIF_NewPage(BYTE* pszCommand)         // 새로운 페이지 수신시 즉시 알림
{
	if (*pszCommand == '1')
	{
    	CLNTa_log(1,0, "[NEWURL] : 1");
		newpage_command(1);
		at_send_extended_text("*SKT*NEWPAGE:1");
	}
	else if (*pszCommand == '0')
	{
    	CLNTa_log(1,0, "[NEWURL] : 1");
		newpage_command(0);
		at_send_extended_text("*SKT*NEWPAGE:1");
	}
	else 
	{		
		return 4; // Return Error
	}

    return 0xFE;
}
BYTE EIF_GetURL(BYTE* pszCommand)          // URL  정보 가져오기 기능
{
	INT nNumber;
	
	nNumber = at_atoi(&pszCommand);

    CLNTa_log(1,0, "[GETURL] : %d",nNumber);
	geturl_command(nNumber);

    return 0xFE;
}

BYTE EIF_Gvminfo(BYTE* pszCommand)          // GVM 파일 수신정보 알림 기능
{
	INT nNumber;
	
	nNumber = at_atoi(&pszCommand);

    CLNTa_log(1,0, "[GVMINFO] : %d",nNumber);
	if(nNumber == 0 || nNumber == 1)
	{
		gvminfo_command(nNumber);
	}
	else
	{
		return 4; // Return Error
	}

    return 0xFE;
}

/* E-mail 관련 기능 */
BYTE EIF_DelAllMail(BYTE* pszCommand)      //  메일 삭제
{

    return 0xFE;
}


/* Mobile Wallet 관련 기능 */
BYTE EIF_GetWalletInfo(BYTE* pszCommand)        // Get Mobile Wallet Information
{

	return 0xFE;
}

BYTE EIF_SetPIN(BYTE* pszCommand)           // Set PIN
{

	return 0xFE;
}

BYTE EIF_SetMWData(BYTE* pszCommand)           // Set Mobile Wallet Data
{
	return 0xFE;
}


void EIF_Inititialize(void)
{
	
	g_fLevel = FALSE;
	g_fEIFAutoReceive = FALSE;    
    
    g_fSetLCD   = FALSE;
    g_nOutX = 0;
    g_nOutY = 0;

    g_nBeepNumber = 0;    
    g_nKeyFlag = 0;

    EIF_Event = SE_NONE;

    g_fNewMsgTimerAlive = FALSE;
    g_fSMSNewMsg = FALSE;

    g_nTIMsgSize     = 0;
    g_nNewMsgTry     = 0;
	skt_TiBellNotify = 0;
	nIteration       = 0;
	if( g_fEmmo == TRUE )
	{
		clk_dereg(&PingTimeOutTimer);
	}
	g_fEmmo  = FALSE;
	g_nPingTurn = 0;
	g_nSetPing  = 0;
	g_nEMTI = 0;
	memset(g_szTermNumber, 0, 15);
	memset(g_szCallBackNumber,0, 15);
	memset(g_szEMMsg,0, 81);

    SendEIFCommand(SE_RESET_LCD);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:

**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API SKA_MainEif(SKY_EVENT_T *pEvent)
{
    UINT8 nWidth, nHeight;
    BOOL  fWrap;
	static BOOL fOwnLcd;
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // Show annonciator
            WA_SHOW_HEADER);
		fOwnLcd = FALSE;

        break;
        
    case SE_APP_END :   /* The very last event */ 
        g_fSetLCD = FALSE;
		EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
#ifndef WIN32
        if( EQS_GET_CHILD() == SKA_SMSSendMsg )  //SMS_SEND_FAIL or MS_SEND_SUCCESS
        {
	        ds_cmd_type	ds_cmd_item;				// item to hold cmd to dsmgr
	        extern ds_sigs_type dsi_requested_sig_mask;

            if( pEvent->dwParam )
	            ds_cmd_item.cmd_name = DS_EIF_MO_OK;            
            else
                ds_cmd_item.cmd_name = DS_EIF_MO_NG;
            ds_cmd_item.parms.srv_opt = g_nOriMsgNumber;
	        ds_cmd(&ds_cmd_item);
	        dsi_requested_sig_mask |= DS_CMD_Q_SIG;          
        }
#endif
        EQS_END();
        break;
	case SE_APP_DRAW:
/*
		if( fOwnLcd )
		{
			fOwnLcd = FALSE;
			nWidth  =  EQS_CharWidth((BYTE)' ');
			nHeight =  EQS_CharHeight((BYTE)' ');        
			fWrap = EQS_GetTextWrap();
			EQS_SetTextWrap(TRUE);
			EQS_TextOutStr(g_nOutX*nWidth, (g_nOutY+1)*nHeight, g_szOutBuffer);
			EQS_SetTextWrap(fWrap);
		}
*/
		break;
    case SE_KEY_SEND:
            break;
    case SE_KEY_1:        
        break;
    case SE_KEY_2:       
        break;
    case SE_KEY_3:
        break;
    case SE_SET_LCD:    // ignore
        break;
    case SE_RESET_LCD:
        EQS_END();
        break;
    case SE_CLEAR_LCD:
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
        break;
    case SE_DRAW_LCD:
		fOwnLcd = TRUE;
		nWidth  =  EQS_CharWidth((BYTE)' ');
		nHeight =  EQS_CharHeight((BYTE)' ');        
		fWrap = EQS_GetTextWrap();
		EQS_SetTextWrap(TRUE);
		EQS_TextOutStr(g_nOutX*nWidth, EQS_SWINDOW_HEADER_HEIGHT + g_nOutY*nHeight, g_szOutBuffer);
		EQS_SetTextWrap(fWrap);

		EQS_UpdateDisplay();
		//EQS_SEND_EVENT( SE_APP_DRAW, EQS_NA, EQS_NA );

        break;
    case SE_MO_REQ:
#ifndef WIN32
        EQS_START_CHILD_EVENT(SKA_SMSSendMsg , SE_APP_START, EQS_NA, g_nTI);
#endif
        break;
    case SE_EM_REQ:        
#ifndef WIN32
        EQS_START_CHILD_EVENT(SKA_SMSSendMsg , SE_APP_START, EQS_NA, g_nEMTI);
#endif
        break;

    case SE_ORI:

        break;
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void CheckNewMsg(void)
{
	ds_cmd_type	ds_cmd_item;				// item to hold cmd to dsmgr
	ds_sigs_type dsi_requested_sig_mask;

	if (g_nTIMsgSize == 0)
		return;

	ds_cmd_item.cmd_name = DS_EIF_MT_NEW;
	ds_cmd(&ds_cmd_item);
	dsi_requested_sig_mask |= DS_CMD_Q_SIG;
}

void NewMsgTimeout(int4 ms_interval)
{
	CheckNewMsg();
}

BYTE  EIF_NewMT(void)
{
    BYTE szString[120], szMsg[120];
	BOOL fMatch = FALSE;
 
    INT  i, TI_Type;

	if ( g_nNewMsgTry > 0) 
    {
        TI_Type = sReadMT(0, szMsg);
		if ( TI_Type < 0 )
			return 4;


		for (i = 0; i < g_nTIMsgSize; i++) 
        {
			if( uinuasms_convert_to_internal_ti(TI_Type) ==  g_nTIMsg[i] )
            {
				fMatch = TRUE;
				break;
			}
		}
		if ( fMatch == FALSE)
			return 4;


		if (g_nNewMsgTry == 3)
			sprintf((char*)szString, "*SKT*NEWMSG: %s", (char*)szMsg);
		else
			sprintf((char*)szString, "*SKT*REMSG: %s", (char*)szMsg);

		at_send_extended_text((char*)szString);
		g_nNewMsgTry--;

/*
		if (g_fNewMsgTimerAlive == FALSE)
			clk_def(&NewMsgTimer);
*/

		clk_reg(&NewMsgTimer, NewMsgTimeout, 3000, 3000, FALSE);
		g_fNewMsgTimerAlive = TRUE;
	}
	else 
    {
		if (g_fNewMsgTimerAlive == TRUE)
        {
			clk_dereg(&NewMsgTimer);
			g_fNewMsgTimerAlive = FALSE;
		}
	}

	return 0xFE;
}


BYTE SKT_DM_OFF_command(BYTE *src)
{
	Put_NV_DM_PORT ( 0xff );
	return 0xFE;
}

BYTE SKT_DM_1_command(BYTE *src)
{
	Put_NV_DM_PORT ( 0 );
	return 0xFE;
}

BYTE SKT_DM_2_command(BYTE *src)
{
	Put_NV_DM_PORT ( 1 );
	return 0xFE;
}

BYTE SKT_tibn_command(BYTE *src)
{
	int	no;
	BYTE szBuf[80];

	switch (*src)	{

	case	'=':
		src++;	
		no = at_atoi(&src);
		if (no != 0 && no != 1)
			return 4;				// Return ERROR

		skt_TiBellNotify = no;
		break;
	
	case	'?':
		break;

	default	:
		return 4;					// Return ERROR
	}

	sprintf((char*)szBuf, "*SKT*TIBN: %d", skt_TiBellNotify);
	at_send_extended_text((char*)szBuf);
	return 0xFE;
}

BYTE	SKT_dr_command(BYTE *src)
{
	int	no;
	extern	void displayDataBanner(boolean); 

	if (*src == '=')
	{
		src++;
		if (*src == '?')
		{			
			at_send_extended_text("*SKT*DR: (0 - 2)");
			return	0xFE;
		}
		else if (*src >= '0' && *src <= '2')
		{
			dsAtSktDrVal = *src - '0';

#ifdef	FEATURE_SKT_DISPBANNER_DS	//	khekim 00/09/18
			displayDataBanner(check_dtr()); 
#endif	//	#ifdef	FEATURE_SKT_DISPBANNER_DS

//			displayDataBanner(check_dtr()); 

		    ds_nv_item.data_rev_mode = dsAtSktDrVal;
			SKY_PutNV(NV_DATA_REV_MODE_I, &ds_nv_item);
		}
		else 
		{		
			no = at_atoi(&src);
			if (no < 0 || no > 2)
				return 4; // Return Error
		}
	}
	else if (*src=='?')
	{
		src++;

		switch(dsAtSktDrVal)
		{
		case 0:
			at_send_extended_text("*SKT*DR: 0");
			break;

		case 1:
			at_send_extended_text("*SKT*DR: 1");
			break;

		case 2:
			at_send_extended_text("*SKT*DR: 2");
			break;

		default:
			break;
		}
	}
	else if (*src=='\0')
	{
		dsAtSktDrVal = 0;

#ifdef	FEATURE_SKT_DISPBANNER_DS	//	khekim 00/09/18
		displayDataBanner(check_dtr()); 
#endif	//	#ifdef	FEATURE_SKT_DISPBANNER_DS

//		displayDataBanner(check_dtr()); 

	    ds_nv_item.data_rev_mode = dsAtSktDrVal;
		SKY_PutNV(NV_DATA_REV_MODE_I, &ds_nv_item);
	}
	else
	{
		return 4;							// Return ERROR
	}

	return 0xFE;
}



BOOL  SKY_API IsEIF_AutoReceive(void)
{
	return g_fEIFAutoReceive;

}