/*========================================================================================================

                        EDIT HISTORY FOR MODULE

		$Header:    wapstate.c_v   1.23   21 Jun 2000 16:38:38   tliou  $

when       who     what, where, why
--------   ---     --------------------------------------------------------------------------------------
12/05/00  zeus(yjchoi)  EIF WAP(External Interface for WAP)에서 사용하는 Error Message 첨가 
12/05/00  zeus(yjchoi)  EIF WAP(External Interface for WAP)의 iwfcon, uopen, uping command를 위해 HANDLE_KEY_S 와 EXIT_S 에 필요한 
                   함수 및 procedure 첨가함
				   HANDLE_KEY_S 에서는 iwfcon, uopen 의 시작과정을 첨가 또한 uping 시작 루틴도 포함시켰으며 upong 이 올 경우 처리해주는
				   루틴도 첨가. EXIT_S 에서는 uping에서 사용하는 버퍼를 다음번에 사용하기 위해 clear 한다. 
09/00/30   jjkyg   Out of memory시 SW_RESET걸리게 함
09/00/30   jjkyg   CALL fade시 tx/rx activiation이 있더라도 그냥 Dormant로 들어가게 함
09/00/21   jjkyg   UI_FADE_F처리 루틴 첨가		
09/19/00   jjkyg   ENTER state에서 초기 n-top시작가능 여부 checking하는 것->막음 
09/18/00   jjkyg   Dormant시 보안 처리 문제 해결
09/06/00   jjkyg   disconnect msg에 의한 reorigination invoke를 막기 위햐 Terminatvar에 wap_dorm_mode = NOT_REORIG
				   으로 셋팅
09/05/00   jjkyg   reorigination enable함수인 void enable_reorigin(void)첨가 
08/31/00   jjkyg   Dormant mode가 active인지를 검사하는 wap_dorm_active()첨가 
08/30/00   jjkyg   WAP의 dormant 시 reorigination을 위하여 void wap_reorigination(void)을 첨가함 
08/28/00   jjkyg   WAP의 dormant 기능을 위하여 LOCAL wap_dorm_type wap_dorm_mode,wap_set_dorm_mode()
                   를 첨가함 
========================================================================================================*/
#include <string.h>
#include <ctype.h>
#include <stdio.h>
/////////////////////////////////////////////////////////////

#include "psi.h"
#include "waptask.h"
#include "wapconnector.h"
#include "pppipcp.h"
#include "ds.h"
#include "dsi.h"
//jjkyg 11/3
#include "comdef.h"
#include "db.h"
#include "msg.h"
#include "errcodes.h"
#include "clnt.h"

#include "GenDsp.h"
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "const.h"

#ifdef FEATURE_SKT_EIF //yjahn96 01.02.24 --- eif for wap
#include "ska_eif.h"
#endif  //#ifdef FEATURE_SKT_EIF

#ifdef FOR_MS
#include "cwsplib.h"
#include "cssls.h"
#include "clk.h"
#include "cFunc.h"
#endif
#include "nv.h"
#if (MODEL_ID != MODEL_B1)  && (MODEL_ID != MODEL_B2)
#include "..\MMI\SK\email\dns.h"
#endif //#if (MODEL_ID != MODEL_B1)
#include "netuser.h"
#if (MODEL_ID != MODEL_B1)  && (MODEL_ID != MODEL_B2)
#include "..\MMI\SK\email\dssock.h"
#endif //#if (MODEL_ID != MODEL_B1)
#include "errcodes.h"

#include "queue.h"
#include "dsm.h"

#ifdef FEATURE_NETGAME
// defined in midd.c
extern char svNetGameRetURL[];
#endif//netgame

//for wip_malloc
#include "confvars.h"
#include "aapiclnt.h"
#include "netuser.h"
#include "capiclnt.h"

#if MMI_TYPE == SKY_UI  //add_htg_00_xmas : To adapt for EQUS
#include "sky.h"
#include "ska_wap.h"
#endif

#ifndef DIRECT_RESPONSE
#include "clntapp.h"
#endif//#ifndef DIRECT_RESPONSE

#include "err.h"                /* Error logging macro                     */

#include "mccdma.h"

// tasik 00.11.01
#ifdef FEATURE_SMART_BATTERY
#include "smb.h"

extern void SmbEvent(SmbStatusType);
#endif
// tasik end

extern int atoi( const char *string );      //white

#ifdef WAP_USE_DORM
 //변수
	LOCAL   wap_dorm_type wap_dorm_mode = WAP_MODE_NOT_DORM;
	extern  boolean dsi_release_dormancy;
	extern  boolean wap_flow_cont;
	LOCAL   boolean wap_clk_key = FALSE;
	int     wap_count = 0;
#ifdef PORTING_B0_WAP
	dsi_orig_chk_enum_type wap_orig_chk;
	int    dorm_dispkind = 0;
#else
	extern dsi_orig_chk_enum_type wap_orig_chk;
	extern int    dorm_dispkind;
#endif //PORTING_B0_WAP
	
	extern boolean wap_rcv_page;	
	extern unsigned short  wap_duration ;

//함수
	void    wap_set_dorm_mode(wap_dorm_type wap_dorm);
#ifdef PORTING_B0_WAP
	LOCAL   void Do_cancel(boolean send);
#endif //PORTING_B0_WAP
	int     wap_reorigination(void);
	void enable_reorigin(void);	
#endif//#ifdef WAP_USE_DORM

extern void dsmgr_cancel_timer(ds_sigs_type sig);
//-----------------------------------------------------------------------------
#ifdef PORTING_B0_WAP
UINT32 g_nTimeStamp = 0;
#else
extern UINT32 g_nTimeStamp;
#endif //PORTING_B0_WAP

/* MMI의 입력 Element 용(for 알파에디터 연결) */
#ifdef PORTING_B0_WAP
extern byte no_alpha_lines;
extern byte allow_empty;
extern byte from_wap;
extern char format_string[200];
extern byte is_delimeter[200];
#endif //PORTING_B0_WAP

extern WAP_Content_Input *g_pCurInput;

/* MMI 다이알로그용 */
extern int   g_nDlgId;
extern char *g_pDlgMessage;
extern char *g_pDlgMessage2;

///////////////////////////////////////////////////////////////////////////////
//	WAP CLIENT BEGIN
///////////////////////////////////////////////////////////////////////////////
#ifdef PORTING_B0_WAP
LOCAL  void browser_start(void);
#else //PORTING_B0_WAP
void browser_start(void);
#endif //PORTING_B0_WAPLOCAL  void browser_terminate(void);

void wap_confirm_error(void);
void wap_check_error(boolean isMajorState);
void wap_manage_error(boolean isMajorState);
void wap_display_error_message(int kindOfError, int kindOfType);

void wap_display_error(void);
void wap_display_exit_message(void);
void wap_display_exit_message2(void);
void wap_display_exit_message3(void);
void wap_display_exit_for_send_call(void);
void wap_display_exit_for_receive_call(void);
void wap_display_failure_message(void);
void wap_display_unknown_message(void);	
void wap_handle_earpiece_down(void);
void wap_do_abnormal_exit(void);
void wap_do_abnormal_exit2(void);
void wap_handle_memory_shortage(void);

/*
** Flags for start, close control
*/
boolean resume_is_ok = TRUE;
boolean close_view_is_ok = FALSE;

/*
** Flags for error handling
*/
boolean isFirstDisplay = FALSE;
boolean isFailureLoadURL = FALSE;
boolean isFromWap = FALSE;
boolean isFirstWapToIdle = FALSE;
boolean isUnknownContent = FALSE;
boolean isNotGoBackHistory = FALSE;
boolean isOutOfMemory = FALSE;
boolean isFirstLoadFailure = FALSE;
boolean isUDPErrorInd = FALSE;
boolean isBadURL= FALSE;
boolean isIssuePowerOffEvent = FALSE;
boolean isFirstReceived = TRUE;

#ifdef WAP_USE_DORM
boolean isFailOrig = FALSE;
#endif//#ifdef WAP_USE_DORM

extern boolean isErrorState;

#ifdef FEATURE_MUSIC_BELL
boolean ui_musicbell_default = FALSE;
#ifdef PORTING_B0_WAP
extern void musicbell_check(void);
extern void musicbell_nvwrite(void);
#endif //PORTING_B0_WAP
#endif //FEATURE_MUSIC_BELL

int errorNumber;
int errorType;
///////////////////////////////////////////////////////////////////////////////
//	WAP CLIENT END
///////////////////////////////////////////////////////////////////////////////

//jjkyg 11/4

LOCAL void Process_Loading(int nKey);
LOCAL void Process_After_HandleKeys(int nResult);
LOCAL void Process_Input_Element(void);
LOCAL void Process_PasswordDialog_For_Input(BOOL bRequestId);
LOCAL void Process_PromptDialog_For_Input(void);
LOCAL void Process_For_Exit(void);
#ifdef PORTING_B0_WAP
LOCAL void Process_For_Cancel_Loading(void);
#else
void Process_For_Cancel_Loading(void);
#endif //PORTING_B0_WAP


extern void WAP_Repaint(void);

BOOL makecall   = FALSE;
UINT8 ui_errortype =0;
BOOL toneback_on =FALSE;

#ifdef PORTING_B0_WAP
BOOL rcvcall  = FALSE;
#else
extern rcvcall;
#endif //PORTING_B0_WAP

BOOL makeagent = FALSE;

//jjkyg add 00/09/29
#ifdef PORTING_B0_WAP
LOCAL void processExitOnly(void);
#else
void processExitOnly(void);
#endif //PORTING_B0_WAP

#ifndef QUALCOM30_WAP
extern boolean dsi_hold_orig_enabled;
#endif //not QUALCOM30_WAP

#ifdef PORTING_B0_WAP
static boolean disponly = FALSE;
#endif //PORTING_B0_WAP

// ========================================================================
// zeus(yjchoi), 00.12.05
// EIF WAP(External Interface for WAP)을 위한 각종 global 변수들 
#ifdef EIF_WAP						
	static int ping_cnt = 0;
	static int PongCnt = 0;
	static boolean continue_ping = FALSE;				// ping_cnt 가 255 들어오면 TRUE로 만들어 continuous ping 실시 
	static boolean pinging = TRUE;

	extern rex_timer_type ui_uping_timer;

	extern char EIF_url[];
	extern ip4a Dest_url_IP;

	extern	UB	PingCnt;
	extern	UB	nTimeout;	

	extern char resultBuf[2048];					

	extern struct icmp * eif_icmp;
	extern void at_send_extended_text(char*);
	extern void uping_command(int, ip4a);
	extern int uping_reply_command(struct icmp *);
	extern void select_command(char*);
	extern char selectBuf[16];

	extern dword ping_RTT;
	extern dword ping_MIN;
	extern dword ping_MAX;

	extern dword ping_seq;
	extern dword ping_timeStamp;

	extern int After_WAP_start_what_call;
	extern BOOL EIF_dns_query;
//	extern ip4a Dest_IP;
	extern char Dest_IP[];
#endif // EIF_WAP
// ========================================================================


/************************************************************************************/
/*																					*/
/*							WAP States 관련 변수									*/
/*																					*/
/************************************************************************************/
typedef enum 
{
	NONE_S,                         /* no state							*/
	IRDA_CHECK_S,
	ENTER_S,                        /* entering this state				*/
	
	LOADING_S,						/* Loading..						*/
	HANDLE_KEY_S,					/* handle key						*/

	AFTER_PHONEBOOK_S,				/* after phonebook state			*/		
	AFTER_ALPHAEDIT_S,				/* after a-edit state				*/		
	AFTER_ALPHAEDIT_FOR_NEWURL_S,	/* after a-edit state for WAP-Menu	*/
	AFTER_ALPHAEDIT_FOR_DIALOG_PROMPT_S,
	AFTER_ALPHAEDIT_FOR_DIALOG_ID_S,
	AFTER_ALPHAEDIT_FOR_DIALOG_PASSWORD_S,

#ifdef FOR_MS
	WTLS_SESSION_START_S,
	WTLS_CLIENT_HELLO_S,
	WTLS_CHANGE_KEY_S,	
#endif

#ifdef USE_DNS
	ACQ_DNS_S,
#endif

	EXIT_S,							/* exiting							*/
	ERROR_S,						/* Error 발생						*/
	NEW_CARD_S						/* newCard from Timer/Refresh		*/
#ifdef WAP_USE_DORM
	,WAP_RET_CALL_S
#endif//#ifdef WAP_USE_DORM
} min_type;

static min_type		state = IRDA_CHECK_S;	/* entry state */
static min_type		new_state;			/* new state if any */
#ifdef PORTING_B0_WAP
static ui_maj_type	ret_state;				/* return state */
#endif //PORTING_B0_WAP
static mc_msg_type  *mc_ptr;
extern boolean DataFst;

#ifdef PORTING_B0_WAP
static boolean      ps_start = FALSE;
#endif //PORTING_B0_WAP
int   wap_mode =0;/*0: CL_WSP 1: CL_WTLS 2: CL_WTA 3: CO_WSP 4: CO_WTLS 5: CO_WTA*/

/*
0: Connecting...
1: Reloading...(when 15second timer setting..)
*/
#ifdef PORTING_B0_WAP
static db_items_value_type dbi;   
#else
db_items_value_type dbi;   
#endif //PORTING_B0_WAP

static wap_cmd_type*       wap_ptr;
static ds_cmd_type ui_ds_buf;
#ifdef PORTING_B0_WAP
static void    displayterminate(void);
#endif //PORTING_B0_WAP
static int     retry =0;

#ifdef PORTING_B0_WAP
boolean  load_data = FALSE;
#else
extern boolean  load_data ;
#endif //PORTING_B0_WAP

nv_item_type wap_nvi; /* NV items */
//static  char proxy_url[130] ;  

/*=====================================================================
				     PUSH 관련 변수 
        -SMS로 내려 올 수 있는 데이타는 최대 80byte임
		-일단, Email url도 fix하여 사용함
 =====================================================================*/
#ifdef PORTING_B0_WAP
static char EmailUrl[100];
char  PushUrl[100];
#else
extern char EmailUrl[100];
#endif //PORTING_B0_WAP
extern char  temp_url[200];

#ifdef PORTING_B0_WAP
LOCAL void do_makecall(void);
#else
void do_makecall(void);
#endif //PORTING_B0_WAP
void process_common(void);
void process_release(void);

uc		ucVerifyDataOpen[1500];
ul		ulVerifyDataLenOpen;


#ifdef FOR_MS
/*====================================================================
				GLOBAL VARIABLE.....
 =====================================================================*/
#ifndef WAP_USE_SECURE
	const  int wtlsMode   = E2E_MODE;
	const  uc  SktCipher  = T_SEED;
#endif//#ifndef WAP_USE_SECURE

#ifdef PORTING_B0_WAP
CHAR SaveWtls[1000];
BOOL wtls_enc_start = FALSE;
BOOL Wtlshello      = FALSE; 
boolean bStopKey    = FALSE;
boolean bWtlsgoback = FALSE;
/*=====================================================================
       Nego가 끝났을 경우 서로의 키값이 생성되므로 이것을 이용함
 =====================================================================*/
boolean bSetKey  = FALSE;
#else
extern BOOL wtls_enc_start ;
extern boolean bStopKey    ;
#ifdef WAP_USE_SECURE
extern CHAR SaveWtls[1000];
extern BOOL Wtlshello      ; 
extern boolean bWtlsgoback ;
/*=====================================================================
       Nego가 끝났을 경우 서로의 키값이 생성되므로 이것을 이용함
 =====================================================================*/
extern boolean bSetKey  ;
#endif //WAP_USE_SECURE
#endif //PORTING_B0_WAP
int  wtls_match     = 0;//TRUE
BOOL isFailureWTLSA = FALSE;
BOOL isFailureWTLSB = FALSE;


int    nLinkType = 0;
int    nLinkId   = 0;
/*===================================================================
                  WTLS END process를 위한 것
 ===================================================================*/
BOOL bWstart = FALSE;

//====================================================================
//          LOCAL VARIABLE.....
//====================================================================
LOCAL int wtlserr  = 0;
LOCAL int return_ssls;


//====================================================================
//          EXTERN VARIABLE.....
//====================================================================
extern SSLS*  spSKT;
extern stPaq  ui_stpaq;
extern storeWML enc_use;

//====================================================================
//          FUNCTION.....
//====================================================================
extern int ssls_open_send(stPaq *stpPaq,int *npErr,char *cpUrl, int nMode,uc ucCipher);
extern int ssls_recv_hello(stPaq *stpPaq, int *npErr, uc *ucRecBuf, int nRecBufSize,char *cpUrl, int nMode);
extern void clear_ssls(void);
extern void close_wtls_session(void);
extern void wsp_send(unsigned char *ucpSendData, unsigned  int nSendLen);

LOCAL void send_usecontent(CHAR* url);
LOCAL void wap_display_wtls(int k);
#ifdef PORTING_B0_WAP
LOCAL void go_storeWml(boolean del);
#else
void go_storeWml(boolean del);
#endif //PORTING_B0_WAP

int clnt_wsp_send(unsigned char *ucpSendData, unsigned int nSendLen,char *cpUrl,BOOL state);
void close_wtls_not_air(void);
//===============new function for WTLS====================
void processcommonWTLS(int nKey);

#endif//#ifdef FOR_MS

#ifdef WAP_USE_DORM
	extern storeWML normal_use;
	#ifndef WAP_USE_SECURE
		LOCAL void go_storeWml_normal(BOOL delete);
	#else		
		void go_storeWml_normal(BOOL delete);	    
	#endif//#ifndef WAP_USE_SECURE
#endif//#ifdef WAP_USE_DORM
#ifdef PORTING_B0_WAP
int  push_acc =0;
#else
extern int  push_acc ;
#endif //PORTING_B0_WAP


BOOL receiveOK =FALSE;
LOCAL void displayloading(int catagory,boolean setting);
boolean from_ds =FALSE;

BOOL initialaccess =FALSE;

#ifdef USE_DNS
BOOL dnsaccess = FALSE;
#endif
extern void *wip_malloc (UINT32 size);
extern void wip_free (void* mem);


//-------------------------------------------------------------------------
// animation 관련
//-------------------------------------------------------------------------
#define WAP_ANIMATION_TIMER_INTERVAL	100

#define WAP_ANI_TEXT_CONNECTING			1	//접속중
#define WAP_ANI_TEXT_RECEIVING			2	//수신중
#define WAP_ANI_TEXT_SECURITY			3	//보안중
#define WAP_ANI_TEXT_ADDBOOKMARK		4	//추가중
#define WAP_ANI_TEXT_RESETUP			5	//재설정
#define WAP_ANI_BAR_FROM_LEFT			FALSE
#define WAP_ANI_BAR_FROM_RIGHT			TRUE

BOOL g_bDrawBar = TRUE;
//PORTING_B0_WAP static --> global//
UINT8 g_nAniBarCount = 0;
void WAP_DrawLoadingBar(int nAniTextMode, BOOL bAniBarDirection);
BOOL g_bExistConntAnimImage = FALSE;
/************************************************************************************/
/*																					*/
/*							WAP State Machine										*/
/*																					*/
/************************************************************************************/
//LOCAL int PrevKey         = UI_NONE_F;
#ifdef PORTING_B0_WAP
	  BOOL firstsite      = FALSE ;
#endif //PORTING_B0_WAP

LOCAL void terminate_timer(void);
#ifdef PORTING_B0_WAP
LOCAL void WAP_Initialize_MMI(void);
#else
void WAP_Initialize_MMI(void);
#endif //PORTING_B0_WAP

LOCAL BOOL newurl_input = FALSE;
LOCAL BOOL gobackward   = FALSE;
LOCAL char new_url[WAP_MAX_INPUT];
LOCAL void TerminateVar(void);
LOCAL int process_handle_event(int event);
#ifdef PORTING_B0_WAP
LOCAL void resetalldata(void);
#else
void resetalldata(void);
#endif //PORTING_B0_WAP
#ifdef PORTING_B0_WAP
void   WAP_Time_Stmap(BOOL bStart);
#endif //PORTING_B0_WAP
extern BOOL savefirst;
#ifdef PORTING_B0_WAP
extern int first_flag_tid;
#endif //PORTING_B0_WAP
#ifdef PORTING_B0_WAP
BOOL entertain = FALSE;
#else
extern BOOL entertain ;
#endif//PORTING_B0_WAP



/*======================================================
				Terminate 관련 변수
 =======================================================*/
boolean browserTerComp     = TRUE;
boolean WapTraffic_Term    = TRUE;

boolean bReceiveData       = FALSE;//uinstate.c와 Net_arrived_f가 도착할 경우 됨..
int     iGoorNew           = 0;
/*===========================================================
 초기 접속시, END key를 누룰 경우 죽는 것을 해결하기 위한 변수
 일단, 특정 EVENT발생시까지 wait event에 END_K, EARDOWN_K,
 HS_PF2_K,CLR_K등을 저장하여 놓았다가 Event 발생시 사용한다.
 ===========================================================*/
//int    WaitKey = UI_NONE_F;
BOOL   bCreateReq = FALSE;
extern boolean g_bGettingContent;//picture mate/music bell
extern boolean g_bSavingContent;//for Saving Picture Mate/Music Bell

/*=========================================================
               2000/03/15  FOR idLE TIMER 
	           When 3 min pass, go to bye..
 =========================================================*/
BOOL  bByetimerSet = FALSE;
BOOL  bReceiveRelease = FALSE;

#define CACHETEST//->Error발생시 cache를 clear한다.

/*===========================================
		WAP중 push혹은 mail의 사용을 위함
 ============================================*/
void process_wap_push_mail(void);
#ifdef PORTING_B0_WAP
LOCAL int InWapMailorPush = 0;//1: initial url connecting 2:일단 card는 하나 받았음
#endif //PORTING_B0_WAP

#ifdef PORTING_B0_WAP
static boolean bsetonlyone = TRUE;
#endif //PORTING_B0_WAP
//static int     save_event = UI_NONE_F;

#define INIT_TEST 
/*============================================
		For close Port scheme
 ============================================*/
#ifdef BROWSER_UPDATE
void close_udp_port(void);
boolean bchoose = FALSE;
#endif//BROWSER_UPDATE
/*===========================================*/

extern uint16 SAport;

#ifdef BROWSER_UPDATE
	#define REDIRECT
#endif

#ifdef  REDIRECT
/*===============================================
	   Redirect에 사용하기 위한 변수 
 ================================================*/
redirect_config redirect;
void redirect_terminate(void);
BOOL save_redirecturl(void);
void start_redirect(void);
CHAR RedirectUrl[1500];
CHAR prevRedirect[1500];
BOOL setPrev = FALSE;
#endif//#ifdef  REDIRECT

void wap_display_content_error(void);	

void PlayNTopConntImage(void);//WAP 초기접속

LOCAL int plength = 0 ;
boolean start_disp = FALSE;

#ifdef FEATURE_NETGAME
void wap_display_exit_message_for_netgame(void);
#endif//#ifdef FEATURE_NETGAME

#define WAP_USE_COMMAND
void report_event(wap_name_type name);

#ifdef PORTING_B0_WAP
extern byte	ui_bIrDAPortClosed; // connection with pc
extern void	ChangeIrDAMode(boolean);
#endif //PORTING_B0_WAP

#ifdef WAP_USE_PZID2
void wap_change_pzid(void);    
#endif//#ifdef WAP_USE_PZID2

#ifdef WAP_USE_SKT_NEW
	extern int ReleaseErrorKind;
#ifdef PORTING_B0_WAP
    wap_release_state_enum_type wap_rel_state = REL_NULL;
#else 
	extern wap_release_state_enum_type wap_rel_state;
#endif //PORTING_B0_WAP
    min_type save_min_state = NONE_S;
#endif//#ifdef WAP_USE_SKT_NEW

void process_stop_event(void);
#ifdef PORTING_B0_WAP
LOCAL void SendRequest(void);
LOCAL void SendRelReq(void);
LOCAL void EnterDormMode(void);
LOCAL void SendNormalRequest(void);
#else
void SendRequest(void);
#endif //PORTING_B0_WAP
LOCAL void SendExitDormReq(void);
LOCAL void CheckCAIrev(void);
#ifdef PORTING_B0_WAP
LOCAL void ClearRelVar(void);
#endif //PORTING_B0_WAP

extern void dsmgr_cancel_timer(ds_sigs_type sig);
void process_draw_stop(void);
void process_draw_bar(void);
boolean bb_setclk = FALSE;

#ifdef PORTING_B0_WAP
LOCAL void ProcessBye(void);
#else //PORTING_B0_WAP
void ProcessBye(void);
#endif //PORTING_B0_WAP
boolean CheckTraffic(void);
void process_rcv_call(void);
void process_makecall(void);
void process_after_release(void);
void process_exit_event(void);
void process_hold_orig(void);
void CheckBrowerDown(void);
void ShowTheBar(void);
void ReturtoErrorState(void);
void processWtlsResult(void);
void Dormantrcvcall(void);
#ifdef PORTING_B0_WAP
void process_rcv_sms(void);
#endif //PORTING_B0_WAP
void nosvc_indicator(void);


//다른 State에서 release를 받았을 경우를 처리하기 위함
int save_release = 0;

#ifdef SW_RESET_ENABLE
	void SW_Reset(void);
#endif//#ifdef SW_RESET_ENABLE

#ifdef WAP_USE_SECURE
	extern boolean bInDealloc;
	extern void ClearWTLSdata(void);
	extern wap_handshake_state handShakestate;
	extern BOOL Portdetect;	
#endif//#ifdef WAP_USE_SECURE

#ifdef PORTING_B0_WAP
ui_maj_type uistate_wap( void )
{
	int nResultHandleKey = 0;

	extern BOOL wtls_enc_start;
	extern BOOL Wtlshello;
	extern CHAR SaveWtls[1000];
	extern boolean bStopKey;
	extern boolean bSetKey;
	extern BOOL isFailureWTLSA;
    extern BOOL	isFailureWTLSB;
	extern BOOL dnsaccess;
	extern void ui_waptimer_set (int4  interval,boolean repeat);


	ret_state = UI_NOSTATE_S;       /* don't assume a return state */
	wap_check_error(TRUE);

	if(new_state != EXIT_S)	new_state = state;	    
/*=============================================
		For test CHATTING(2000/05/012)
 =============================================*/

	if(ui.event == UI_CHAT_F)
	{
		if(state !=HANDLE_KEY_S) ui.event = UI_NONE_F;
	}


	while( new_state != NONE_S ) 
	{
		wap_check_error(FALSE);
		state = new_state;	
	    new_state = NONE_S;

		switch ( state ) 
		{
			case IRDA_CHECK_S://초기화
			{
				// IrDA shared SRAM with WAP
				// , so we have to close IrDA port before starting WAP
				MSG_MED("--------[IRDA_CHECK_S]--------",0,0,0);

				if(ui.event == UI_TIMER_F)	
				{
					new_state = ENTER_S;
					break;
				}

				if(ui_bIrDAPortClosed)
					new_state = ENTER_S;
				else
				{
					ChangeIrDAMode ( FALSE );
					ui_timer_set((int4)200, FALSE);
				}


				break;
			}	

			case ERROR_S:
			{
			#ifndef DIRECT_RESPONSE
				extern boolean isIntegerResponse;extern BOOL isStringResponse;
			#endif//#ifndef DIRECT_RESPONSE
			#ifdef CACHETEST	
				extern int s_initialized ;
			#endif//#ifdef CACHETEST	

				//2000/05/19 jjkyg add
			if(wtls_enc_start) 
			{
				if(load_data) SKY_stop(1);
				wtls_enc_start = FALSE;
			}

	#ifdef WAP_USE_DORM
			if(!ui.wap_dorm_mode)
	#endif//#ifdef WAP_USE_DORM
			{
				Wtlshello = FALSE;					
			}
	#ifdef WAP_USE_DORM
			else
			{
				CLNTa_log(1,0,"dormant mode시 ERROR로 옴");
			}
	#endif//#ifdef WAP_USE_DORM

	
	#ifdef WAP_USE_DORM
			if(!ui.wap_dorm_mode)
	#endif//#ifdef WAP_USE_DORM
			{
				CLNTa_log(1,0,"storewml--1");
				
            	go_storeWml(TRUE);memset((void *)SaveWtls,0x00,1000);
			}

			bStopKey = FALSE;bSetKey = FALSE;close_wtls_not_air();resetalldata();
					
	#ifndef DIRECT_RESPONSE
			if(isIntegerResponse||isStringResponse)
	#endif//#ifndef DIRECT_RESPONSE
			{
				CLNTa_log(1,0,"여기타찌...");
	#ifdef CONNECT_SESSION
                close_udp_port();
	#endif//#ifdef CONNECT_SESSION
				receiveOK = FALSE;
			}		

			if(entertain)
			{

				if(!ui.wap_dorm_mode) entertain = FALSE; //AddBook =FALSE;

			}

			ui_waptimer_set(0,FALSE);


	#ifdef WAP_USE_DORM
			if(isFailOrig)
			{
				CLNTa_log(1,0,"dorm일시는 항상");
                isFailOrig = FALSE;	wap_display_error();
			}
	#endif//#ifdef WAP_USE_DORM

	#ifdef WAP_USE_SKT_NEW

			if(ui.wap_release && (ReleaseErrorKind == 1))
			{
				CLNTa_log(1,0,"Cache not clear");
			}
			else

	#endif//#ifdef WAP_USE_SKT_NEW
			{
				s_initialized = 0;
			}
	
			if (isFirstDisplay) 
			{
				wap_display_error();isFirstDisplay = FALSE;
			}

			if (isFailureLoadURL) 
			{
				wap_display_error();isFailureLoadURL = FALSE;
			}

			if(isFailureWTLSA)//내부 error
			{
				isFailureWTLSA = FALSE;wap_display_error();					
			}
			else if(isFailureWTLSB)//TImeout
			{
				isFailureWTLSB = FALSE;	
				if(isUDPErrorInd) isUDPErrorInd = FALSE;

                wap_display_error();														
			}
			wap_confirm_error();
			break;
		}//end of case

#ifdef WAP_USE_DORM
		case WAP_RET_CALL_S:
		{
			CLNTa_log(1,0,"WAP_RET_CLL_S",0,0);

			ui_get_keys(FALSE);

			ui.clrnum_flag = TRUE;ui.tog_flag = FALSE;uiscrn_deact(UI_NUMEDIT_F);

			//uiscrn_SetPageTypeEX(PAGE_NORMAL, WT_WHITE, WMT_ALL, DMETHOD_BACKGROUND);
			//uiscrn_SetPageTypeEX(PAGE_CURRENT, WT_NTOP, WMT_CURRENT, DMETHOD_BACKGROUND);
			WAP_Initialize_MMI();WAP_Repaint();

#ifdef WAP_USE_SKT_NEW

			if(ui.wap_release)
				new_state = save_min_state;
			else

#endif//#ifdef WAP_USE_SKT_NEW
				new_state = HANDLE_KEY_S;

			ui.event  = UI_NONE_F;

			break;
		}
#endif//#ifdef WAP_USE_DORM

		case ENTER_S://초기화
		{
			int go_exit;				


    		ChangeIrDAMode ( FALSE );

#ifdef FEATURE_SMART_BATTERY
			SmbEvent(SMB_START_TALK_S);
#endif

#ifdef WAP_USE_DORM

			ui.wap_dorm_mode = FALSE;

#endif//#ifdef WAP_USE_DORM
            if(g_bBlocked) g_bBlocked = FALSE;
			//젤 첨 한번 Trigger함
			bsetonlyone = TRUE;disponly    = FALSE;

			ui.clrnum_flag = TRUE;ui.tog_flag    = FALSE;uiscrn_deact(UI_NUMEDIT_F);

			
#ifdef FEATURE_MUSIC_BELL

			ui.musicbell_on = FALSE;ui_musicbell_default = FALSE;

#endif

			memset(SaveWtls,0x00,1000);

#ifdef REDIRECT
			memset(RedirectUrl,0x00,1500);memset(prevRedirect,0x00,1500);
			setPrev = FALSE;
#endif

			bReceiveData = FALSE;ui_get_keys(TRUE);
			go_exit = process_handle_event(ui.event);


			if(go_exit == -1)	break;

			db_get (DB_NO_SVC, &dbi );


           	//ui_dispmsgtimer_set(0);
			WAP_Initialize_MMI();

			//=====================================================
			// 초기 접속 화면 처리
			//=====================================================
			{
				//WAP 초기접속					
				PlayNTopConntImage();
			}

			MSG_MED("--------[ENTER_S]--------",0,0,0);

			first_flag_tid  = 0;
			ui_get_keys( FALSE ); /* ignore keys */
			ui.wap    = TRUE;makeagent = TRUE;

			if (resume_is_ok) 
			{
				resume_is_ok = FALSE;
			}					

			if(ui.event == UI_DOALERT_F)
			{
				terminate_timer();
					
				report_event(WAP_DRAW_STOP_EVENT);
				//정상종료 된것처럼 처리
				WapTraffic_Term = TRUE;browserTerComp  = TRUE;
			
				wap_display_exit_for_receive_call();

				new_state = EXIT_S;
				rcvcall =TRUE;
				ui.event = UI_NONE_F;ui_dispmsgtimer_set(1500);
				MSG_MED("HERE receive call",0,0,0);
				break;
			}


			if(!ps_start)
			{
				do_ds_upenter();
				ui_dispmsgtimer_set(20000);

			}
			makecall =FALSE;

			//first, get proxy url
			(void) ui_get_nv( NV_PROXY_I,&wap_nvi ); 
			plength = get_length((byte *)wap_nvi.proxy.proxy,NV_PROXY);
			memset(proxy_url,'\x00',130);
			MSG_MED("Plength %d",plength,0,0);

			if(plength < 130) memcpy(proxy_url,(const void *)wap_nvi.proxy.proxy,plength);
			else
			{
				new_state = ERROR_S;isFailureLoadURL = TRUE;
			}

			//second, get initial url
			(void) ui_get_nv( NV_INIT_URL_I,&wap_nvi ); 
			plength = get_length((byte *)wap_nvi.initurl.init_url,NV_URL);
			memset(temp_url,'\x00',200);
			//remove_error
			if(plength < 200) memcpy(temp_url,(const void *)wap_nvi.initurl.init_url,plength);
			else
			{
				new_state = ERROR_S;isFailureLoadURL = TRUE;
			}

			//third, get Email url
			(void) ui_get_nv( NV_INITEMAIL_I,&wap_nvi ); 
			//remove_error
			plength = get_length((byte *)wap_nvi.initemail.initemail,NV_INITEMAIL);
			memset(EmailUrl,'\x00',100);
			if(plength < 100) memcpy(EmailUrl,(const void *)wap_nvi.initemail.initemail,plength);
			else
			{
				new_state = ERROR_S;isFailureLoadURL = TRUE;
			}

			//MakeMin
			memset((void *)SaveSelfMin,0x00,8);
			MakeMin_UI();

		    retry =0;
		 	new_state = ACQ_DNS_S;

			displayloading(0,TRUE);dnsaccess  = TRUE;

			initialaccess = TRUE;ui.event = UI_NONE_F;

			break;
		}//end of ENTER_S

		/*===========================================================================
						DNS ACCESS STATE
		  1. DNS request
		  2. Resend DNS request when not receive proper n-top URL IP address
		  3. Go to Loading_s with UI_NETCONNECTED_F	flag and set timer to 8000 sec..
		 ===========================================================================*/
			case ACQ_DNS_S : 
			{

				ui_get_keys( TRUE );

			switch(ui.event)
			{
				case UI_DISPMSGTIMER_F:
				{
					if(!ui.call)
					{
						CLNTa_log(1,0,"Bye -- 1");
				#ifdef WAP_USE_SKT_NEW
						if(ui.wap_release)
							process_release();
						else
				#endif//#ifdef WAP_USE_SKT_NEW						
							ProcessBye();						
					}
					break;
				}     
				
				case UI_NETCONNECTED_F:
				{
					CLNTa_log(1, 0, "UI_NETCONNECTED_F");
				#ifdef WAP_USE_DORM
					wap_flow_cont = FALSE;
				#endif//#ifdef WAP_USE_DORM
					receiveOK = TRUE;retry =0;
					memset((BYTE*)&wapip,0x00,sizeof(wapip));

					ui_waptimer_set(4000,FALSE);//DNS request limit time


					CLNTa_log(1,0,"length of proxy_url %d proxy %s",strlen(proxy_url),proxy_url);

					process_dns(proxy_url);		
					ui.event = UI_NONE_F;
					break;
				}

				case UI_WAPTIME_F:
				{
					if(retry ==0)
					{
						retry++;
						memset(&wapip,0x00,sizeof(wapip));
						process_dns(proxy_url);		

						ui_waptimer_set(4000,FALSE);//DNS request limit time

					}
					else if(retry==1)
					{
						retry =0;
						memset(&wapip,0x00,sizeof(wapip));

						ui_waptimer_set(0,FALSE);

						new_state = ERROR_S;
						isFailureLoadURL = TRUE;
					}
					ui.event = UI_NONE_F;
					break;
				}

				case UI_DNSANS_F:
				{
					if(wapip!=0)// ==> G/W address acquire..
					{
						CLNTa_log(1, 0, "UI_DNSANS_F");

						ui_waptimer_set(0,FALSE); dnsaccess = FALSE;

							
				  #ifdef WAP_USE_SKT_NEW
                        if(ui.wap_release)
                        {
                            new_state = save_min_state;
                            if(new_state == WTLS_SESSION_START_S)
                            {
            					close_wtls_not_air();                                
            					bStopKey = FALSE;bSetKey = FALSE;	
			            		wtls_match = 1;bWstart = FALSE;                                
                                ui.wap_release = FALSE;wap_rel_state  = REL_NULL;								
                            }
                            else if(new_state == HANDLE_KEY_S)
                            {
                                ui.wap_release = FALSE;wap_rel_state  = REL_NULL;
                                process_stop_event();
                                receiveOK = FALSE;
								break;
                            }
                            else
							{
								SendRelReq();
							}

                            receiveOK = TRUE;
                        }
                        else
                    #endif//#ifdef WAP_USE_SKT_NEW
                        {
                            if(redirect.bCorrect) displayloading(12,TRUE);								
							else displayloading(1,TRUE);

							new_state = LOADING_S;receiveOK = TRUE;
                            SendNormalRequest();
                        }
					}
					else
					{
						new_state = ERROR_S;ui.event = UI_NONE_F;isFailureLoadURL = TRUE;
					}
					break;
				}

				case UI_CONNECTED_F :
				{
					ui_dispmsgtimer_set(0);
					WapTraffic_Term = FALSE;
						
					//time stamp
					WAP_Time_Stmap(TRUE);                   

					uiscrn_ann( HS_AN_INUSE, HS_AN_INUSE );
					dbi.in_use = TRUE;
					db_put( DB_IN_USE, &dbi );

					ui.beep_updown = TRUE;
					ui.call = TRUE;
					CLNTa_log(1,0,"--------[UI_CONNECTED_F]--------");
					ui.event = UI_NONE_F;
					break;
				}//end of UI_CONNECTED_F
			}

			process_common();
			break;
		}

		case LOADING_S :
		{
			//CLNTa_log(1,0,"--------[LOADING_S]--------");

			/*===================================================================
			  현재 Key를 저장하고 있다가 일정 EVENT가 올시 이것을 실행한다.(2/9)
			 ====================================================================*/
		 	if(bCreateReq && initialaccess)
			{

				if((ui.event==HS_END_K)||(ui.event==HS_CLR_K)||(ui.event==HS_EAR_DOWN_K)||(ui.event==HS_PF2_K))
				{
					WaitKey  = ui.event;
					ui.event = UI_NONE_F;
					break;
				}

			}
			
			//==================================================
			//LOADING_S 중, Content를 받고 있는 중에는 모든 키를 무시한다
			if(g_bGettingContent)//picture mate/music bell
			{
				//-------------------------------------
				//UI_RELEASE_F는 처리한다
				//if(ui.event == UI_RELEASE_F)

				if(ui.event == UI_RELEASE_F || ui.event == UI_FADE_F)
						;
				//저장 중이 아닐 경우, end키/clr키 처리한다
				else if(g_bSavingContent == FALSE
					&& (ui.event == HS_END_K || ui.event == HS_EAR_DOWN_K || ui.event == HS_CLR_K))
						;
				//-------------------------------------
				else
				{
					ui.event = UI_NONE_F;
						break;
				}

			}
			//==================================================

			//11/15 jjkyg

			ui_get_keys( TRUE );     /*  keys, thank you */
			Process_Loading(ui.event);

			break;
			
		}//end of LOADING_S

		case NEW_CARD_S:
		{

			ui_get_keys( TRUE );     /*  keys, thank you */

			CLNTa_log(1,0,"====NEW CARD STATE====");

			switch(ui.event)
			{
				case UI_NETDIS_F://showCard
				{
					#ifdef REDIRECT//jjkyg/06/26
						extern void init_buffer(redirect_config* buf);
						if(redirect.bCorrect) init_buffer(&redirect);
					#endif//#ifdef REDIRECT
					ui.event  = UI_NONE_F;
					new_state = HANDLE_KEY_S;
					break;
				}

				//------------ Key 처리 ----------------
				case HS_END_K://end
				case HS_EAR_DOWN_K://back to idle
				{
					ui.event = UI_NONE_F;
#ifdef WAP_USE_SKT_NEW
                    if(ui.wap_release) 
                    {
                       CLNTa_log(1,0,"wap release clear 1");
                       ClearRelVar();
                    }
#endif//#ifdef WAP_USE_SKT_NEW
					if(g_bGettingContent) break;//picture mate/music bell

					Process_After_HandleKeys(WAP_HANDLE_KEY_RESULT_EXIT);
					break;
				}
					
				//jjkyg 09/21
				case UI_FADE_F:
				case UI_RELEASE_F://end
				{			
					ui.event = UI_NONE_F;

					//WAP_HANDLE_RECEIVE_RELEASE
					Process_After_HandleKeys(WAP_HANDLE_RECEIVE_RELEASE);
					break;
				}

				case HS_WAP_K://Top URL 들어가기
				{
					ui.event = UI_NONE_F;
					if(g_bGettingContent) break;//picture mate/music bell

					if(!Wtlshello)
					{
						if(load_data)
						{
							SKY_stop(g_pCard->viewId);
						#ifdef CONNECT_SESSION
						    close_udp_port();
						#endif//##ifdef CONNECT_SESSION
						}
					}

					SKY_loadURL(g_pCard->viewId, temp_url, FALSE);

                    normal_use.viewid  = 1;
					normal_use.id      = 0;
					normal_use.wmlkind = 6;


					//firstsite = TRUE;

					//WAP_HANDLE_KEY_RESULT_MAYBE_LOADING
					Process_After_HandleKeys(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
					break;
				}
			}

			break;
		}//end of NEW_CARD_S

		case HANDLE_KEY_S :
		{
			extern BOOL g_bBuildingNewCard;
			MSG_ERROR("====HANDLE KEY STATE====",0,0,0);
#ifdef WAP_USE_DORM
			/*-------------------------------------------------------------
				  일단 Dormant시 incomming call을 받을 경우에 대한 처리
			 --------------------------------------------------------------*/

			if(ui.event == UI_DOALERT_F)
			{
				Dormantrcvcall();
				break;
			}

			if(ui.event == UI_WANT_RECONNECT_F)
			{
				CLNTa_log(1,0,"Reconnect Want Flag received in handle");
				new_state  = ERROR_S;isFailOrig = TRUE;
				ui.event   = UI_NONE_F;
				break;
			}

			if(ui.event == UI_WAP_DORM_F)
			{
				CLNTa_log(1,0,"WAP go to DORMANT MODE %d in handle",ui.wap_dorm_mode);
				EnterDormMode();
                ui.event = UI_NONE_F;
				break;
			}

			if(ui.event == UI_WAKEUP_F)
			{
				CLNTa_log(1,0,"Receive WAKEUp so sucess reorig in handle");
				if(ui.wap_dorm_mode)
				{
				#ifdef WAP_USE_PZID2
					if(!ui.wap_pzid)
				#endif//#ifdef WAP_USE_PZID2
					{
						CLNTa_log(1,0," SUCCESS REORIGINATION in handle");
						dbi.in_use = TRUE;
						db_put( DB_IN_USE, &dbi );					
						uiscrn_ann( HS_AN_INUSE, HS_AN_INUSE );						
					}
				#ifdef WAP_USE_PZID2
					else
					{
						CLNTa_log(1,0,"Pzid false");
                        ui.wap_pzid = FALSE;						
					}
				#endif//#ifdef WAP_USE_PZID2
					ui.wap_dorm_mode = FALSE;				
					WapTraffic_Term = FALSE;
					ui.beep_updown = TRUE;//일단 막아놈 
					ui.call = TRUE;

				#ifdef FOR_MS//Check하여야 함 		
					if(wtls_enc_start)	bStopKey  = FALSE;
				#endif//#ifdef FOR_MS			
					wap_clk_key   = FALSE;
					wap_rcv_page  = FALSE;
					ui.event = UI_NONE_F;
				}
				break;
			}	

#endif//#ifdef WAP_USE_DORM

			//---------------------------------------------
			// Content를 받고 있는 중이면, LOADING_S로 되돌려 보낸다
			//if(g_bGettingContent)//picture mate/music bell

			if(ui.event == UI_DOWNLOAD_CONTENT_F)
			{
				CLNTa_log(1, 0, "----go go go----");
				ui.event  = UI_NONE_F;
				new_state = LOADING_S;
				break;
			}


			/*==============================================
			          WAP중 PUSH의 사용 
			 ==============================================*/
			if(ui.event == UI_SMS_MSG_F)
			{
				process_rcv_sms();
				break;
			}

			/*==============================================
			    Earpiece가 close시 사용하는 루틴
 		     ==============================================*/

#if MMI_TYPE != MMI_A10     //white 
#ifdef GIZMO_A7M
				if(!ui.earpieceup//earpiece is close
				&& bStartEarPieceUp)//nTop시작시 open되었던 상태
#else// GIZMO_A7M
				if(!ui.earpieceup)//earpiece is close
#endif// GIZMO_A7M
				{
					toneback_on = FALSE;
					initialaccess = FALSE;
					bCreateReq  = FALSE;
					ProcesBye();
					break;
				}
#endif  //MMI_TYPE != MMI_A10

#ifdef GIZMO_A7M
				bStartEarPieceUp = ui.earpieceup;
#endif// GIZMO_A7M
					
			  /*==========================================================
			      Handle key에서 아무런 입력이 3분동안 입력되지 않을 경우
			 	                  사용하는 루틴임
			   ==========================================================*/
				if(initialaccess)
				{

					ui_waptimer_set(60000*5,FALSE);

					bByetimerSet = TRUE;
				}

			   	if(
				    (HS_1_K <= ui.event && ui.event <= HS_9_K)|| 
					(ui.event == HS_REC_K   || ui.event == HS_PLAY_K || ui.event == HS_STAR_K || 
					 ui.event == HS_POUND_K || ui.event == HS_UP_K   || ui.event == HS_DOWN_K ||
					 ui.event == HS_PF1_K   || ui.event == HS_PF2_K  || ui.event == HS_PF3_K  ||
                     ui.event == HS_CLR_K   || ui.event == HS_WAP_K  )
                   )
				{
					if(bByetimerSet)
					{
						CLNTa_log(1,0,"BYE timer not setting");

						bByetimerSet = FALSE;ui_waptimer_set(0,FALSE);

					}
				}
				else 
				{
					if(!bByetimerSet)
					{
						CLNTa_log(1,0,"BYE timer setting");

						ui_waptimer_set(60000*5,FALSE);bByetimerSet = TRUE;

					}

					if((ui.event == UI_UPDISP_F)||(ui.event == UI_ONEMINUTE_F))
					{
						ui.event = UI_NONE_F;
					}

				}
				if(ui.event == UI_WAPTIME_F)
				{
					CLNTa_log(1,0,"Bye -- 3");
					bByetimerSet = FALSE;ui_waptimer_set(0,FALSE);					
					ProcessBye();
				}

  
/*
				else if((ui.event == UI_NONE_F)||(ui.event == UI_UPDISP_F) ||(ui.event == UI_ONEMINUTE_F))
				{
					if(!bByetimerSet)
					{
						ui_waptimer_set(60000*3,FALSE);
						bByetimerSet = TRUE;
					}
					if((ui.event == UI_UPDISP_F)||(ui.event == UI_ONEMINUTE_F))
					{
						ui.event = UI_NONE_F;
					}
				}
				else//not initialaccess & ui.event is not UI_NONE_F
				{
					if(bByetimerSet)
					{
						bByetimerSet = FALSE;
						ui_waptimer_set(0,FALSE);
					}
				}
*/

   /*==================================================================================================*/

				if((WaitKey==HS_END_K)||(WaitKey==HS_CLR_K)||(WaitKey==HS_EAR_DOWN_K)||(WaitKey==HS_PF2_K))
				{
					MSG_ERROR("============UI_WAPREQ_F==============",0,0,0);
					bCreateReq = FALSE;	new_state  = state ;

					ui.event   = WaitKey;WaitKey    = UI_NONE_F;

					//jjkyg add 00/10/30
#ifdef WAP_USE_SKT_NEW

                    if(ui.wap_release) 
                    {
                       CLNTa_log(1,0,"wap release clear 1");
                       ui.wap_release = FALSE;
  		               bsetonlyone = TRUE;
			           disponly    = FALSE;
                    }

#endif//#ifdef WAP_USE_SKT_NEW
					ProcessBye();
					break;
				}
				bReceiveData = FALSE;
				g_bGettingContent = FALSE;//picture mate/music bell

				PrevKey = UI_NONE_F;
			
				if(toneback_on)
				{
					toneback_on =FALSE;

					//tone..

					uisnd_tone( UI_MSG_SND, SND_MSG_WAITING, 100 );
					// Backlight
					if (ui.backval != UI_BKLT_OFF)
					ui_backlight( TRUE, ui.backval ); 

				}

				//-----------------------------------------------
				// [newCard] from Timer/Refresh, so wait for [showCard]

				if(ui.event == UI_CHAT_F || ui.event == UI_NETDATA_ARRIVED_F || g_bBuildingNewCard)
				{
					if(ui.event == UI_CHAT_F)
					{
						CLNTa_log(1,0,"RECEIVE CHAT_F");
					}

					ui.event = UI_NONE_F;
					new_state = NEW_CARD_S;
					break;
				}

#ifdef EIF_WAP	//===========================================================================================================================					
				// zeus(yjchoi) 00.12.05   EIF WAP(External Interface for WAP) 을 위한 routine
				// EIF_WAP select command 수행 
				if(ui.event == UI_EIF_SELECT_F)
				{
					CLNTa_log(1,0,"UI_EIF_SELECT_F cmd getted.....");
					select_command(selectBuf);
					break;
				}


				///////////////////////////////////////////////////////////
				// icmp.c 에서 upong을 받고 signal에 따른 event를 처리 
				if(ui.event == UI_UPONG_F)				
				{
					if(uping_reply_command(eif_icmp))
						PongCnt++;


					if ( continue_ping && pinging )					// continue ping!!!
					{
						rex_clr_timer( &ui_uping_timer );
						rex_clr_sigs( rex_self(), UI_UPING_TIMER_SIG );
						rex_clr_sigs( rex_self(), UI_UPONG_SIG );
						ping_cnt++;
						uping_command(ping_cnt, Dest_url_IP); 
						rex_set_timer( &ui_uping_timer, nTimeout*1000);


						CLNTa_log(1,0, "<<<<< CONTI_UPONG [%d] received!!!", PongCnt);

					} 
					else								// this is not continue ping 
					{
						if ((ping_cnt < PingCnt) && (continue_ping == FALSE))		// 사용자 지정 ping 회수보다 적게 ping을 보낸 경우 계속 ping을 보낸다. 
						{
							rex_clr_timer( &ui_uping_timer );
							rex_clr_sigs( rex_self(), UI_UPING_TIMER_SIG );
							rex_clr_sigs( rex_self(), UI_UPONG_SIG );
							ping_cnt++;
							uping_command(ping_cnt, Dest_url_IP); 
							rex_set_timer( &ui_uping_timer, nTimeout*1000);


							CLNTa_log(1,0, "UPONG received..... from icmp...");

						}
						else if ((ping_cnt >= PingCnt) && (continue_ping == FALSE))	// 사용자 지정 ping 회수(PingCnt) 보다 보내진 ping 회수(ping_cnt - 1)가 많은 경우 ping 종료 
						{
							memset(resultBuf, 0x00, sizeof(resultBuf));				// resultBuf init

							rex_clr_sigs( rex_self(), UI_UPONG_SIG );				// UI_UPONG_SIG clear
							rex_clr_sigs( rex_self(), UI_UPING_TIMER_SIG );			// UI_UPING_TIMER_SIG clear
							rex_clr_timer( &ui_uping_timer );

							if (PongCnt != 0)
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%ldms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt,
										ping_MIN, ping_MAX, (ping_RTT/PongCnt));
							}
							else
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%dms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt, 
										ping_MIN, ping_MAX, 0);
							}

							at_send_extended_text(resultBuf);						// result send

							Dest_IP[0] = 0; Dest_IP[1] = 0; Dest_IP[2] = 0; Dest_IP[3] = 0;
							memset((char*)&Dest_url_IP, 0x00, sizeof(Dest_url_IP));

							ui.event = UI_NONE_F;
							bCreateReq = FALSE;
							new_state  = state ;
							ui.event   = HS_END_K;
							WaitKey    = UI_NONE_F;					
							ProcessBye();						// wap 종료!!
							break;

							CLNTa_log(1,0, "UPONG received..... from icmp...");
						}
						else if ((continue_ping == TRUE) && (pinging == FALSE))		// continuous ping 종료 
						{
							memset(resultBuf, 0x00, sizeof(resultBuf));				// resultBuf init

							continue_ping = FALSE;
							pinging = TRUE;

							rex_clr_sigs( rex_self(), UI_UPONG_SIG );				// UI_UPONG_SIG clear
							rex_clr_sigs( rex_self(), UI_UPING_TIMER_SIG );			// UI_UPING_TIMER_SIG clear
							rex_clr_timer( &ui_uping_timer );

							if (PongCnt != 0)
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%ldms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt,
										ping_MIN, ping_MAX, (ping_RTT/PongCnt));
							}
							else
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%dms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt, 
										ping_MIN, ping_MAX, 0);
							}

							at_send_extended_text(resultBuf);						// result send

							Dest_IP[0] = 0; Dest_IP[1] = 0; Dest_IP[2] = 0; Dest_IP[3] = 0;
							memset((char*)&Dest_url_IP, 0x00, sizeof(Dest_url_IP));

							ui.event = UI_NONE_F;
							bCreateReq = FALSE;
							new_state  = state ;
							ui.event   = HS_END_K;
							WaitKey    = UI_NONE_F;					
							ProcessBye();						// wap 종료!!
							break;

							CLNTa_log(1,0, "<<<<< CONTINUE PING END EVENT GET >>>>>");

						}

					}	// end of if (continue_ping)

					ui.event = UI_NONE_F;
					break;
				}


				////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// uping 명령 수행 중 timer expired signal(UI_UPING_TIMER_SIG)에 따른 UI_TIMER_EXPIRED_F 이벤트를 받았을 때 
				if(ui.event == UI_TIMER_EXPIRED_F)		
				{
					if ( continue_ping &&  pinging )					// continue ping!!!
					{
						rex_clr_sigs( rex_self(), UI_UPONG_SIG );
						ping_cnt++;					
						uping_command(ping_cnt, Dest_url_IP);
						rex_set_timer( &ui_uping_timer, nTimeout*1000);


						CLNTa_log(1,0, "<<<<< CONTI_UPONG TIMER expired!!!");

					}
					else									// this is not continue ping
					{
						if ((ping_cnt < PingCnt) && (continue_ping == FALSE))
						{
							rex_clr_sigs( rex_self(), UI_UPONG_SIG );
							ping_cnt++;		
							uping_command(ping_cnt, Dest_url_IP);
							rex_set_timer( &ui_uping_timer, nTimeout*1000);
							CLNTa_log(1,0, "UPING TIMER EXPIRED......!!!");
						}
						else if ((ping_cnt >= PingCnt) && (continue_ping == FALSE))
						{
							memset(resultBuf, 0x00, sizeof(resultBuf));				// resultBuf init

							if (PongCnt != 0)
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%ldms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt,
										ping_MIN, ping_MAX, (ping_RTT/PongCnt));
							}
							else
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%dms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt, 
										ping_MIN, ping_MAX, 0);
							}

							at_send_extended_text(resultBuf);						// result send

							Dest_IP[0] = 0; Dest_IP[1] = 0; Dest_IP[2] = 0; Dest_IP[3] = 0;
							memset((char*)&Dest_url_IP, 0x00, sizeof(Dest_url_IP));

							ui.event = UI_NONE_F;
							bCreateReq = FALSE;
							new_state  = state ;
							ui.event   = HS_END_K;
							WaitKey    = UI_NONE_F;					
							ProcessBye();						// wap 종료!!
							break;

							CLNTa_log(1,0, "UPING TIMER EXPIRED......!!!");

						}
						else if ((continue_ping == TRUE) && (pinging == FALSE))		// continuous ping 종료 
						{
							memset(resultBuf, 0x00, sizeof(resultBuf));				// resultBuf init

							continue_ping = FALSE;
							pinging = TRUE;

							rex_clr_sigs( rex_self(), UI_UPONG_SIG );				// UI_UPONG_SIG clear
							rex_clr_sigs( rex_self(), UI_UPING_TIMER_SIG );			// UI_UPING_TIMER_SIG clear
							rex_clr_timer( &ui_uping_timer );

							if (PongCnt != 0)
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%ldms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt,
										ping_MIN, ping_MAX, (ping_RTT/PongCnt));
							}
							else
							{
								sprintf(resultBuf, "UPONG:%s[%d.%d.%d.%d],32 Bytes,[Packets:S=%d,R=%d,L=%d],[RTT:MIN=%ldms,MAX=%ldms,AVG=%dms]",EIF_url,
										Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3],ping_cnt,PongCnt,ping_cnt-PongCnt, 
										ping_MIN, ping_MAX, 0);
							}

							at_send_extended_text(resultBuf);						// result send

							Dest_IP[0] = 0; Dest_IP[1] = 0; Dest_IP[2] = 0; Dest_IP[3] = 0;
							memset((char*)&Dest_url_IP, 0x00, sizeof(Dest_url_IP));

							ui.event = UI_NONE_F;
							bCreateReq = FALSE;
							new_state  = state ;
							ui.event   = HS_END_K;
							WaitKey    = UI_NONE_F;					
							ProcessBye();						// wap 종료!!
							break;

							CLNTa_log(1,0, "<<<<< CONTINUE PING END EVENT GET >>>>>");

						}
					}	// end of if (continue_ping)

					ui.event = UI_NONE_F;
					break;
				}

				/////////////////////////////////////////////////////
				// EIF_WAP 관련 IWFCON, UOPEN, UPING 처리 
				if (After_WAP_start_what_call == 1)
				{
					extern void iwfcon_command(void);
					After_WAP_start_what_call = 0;

					CLNTa_log(1, 0, "=== IWFCON function called ===");

					iwfcon_command();	

					bCreateReq = FALSE;
					new_state  = state ;
					ui.event   = HS_END_K;
					WaitKey    = UI_NONE_F;					
					ProcessBye();						// wap 종료!!
					break;

				} 
/*				
				if (After_WAP_start_what_call == 2)
				{
					extern void uopen_command(char*);
					After_WAP_start_what_call = 0;

					CLNTa_log(1, 0, "=== UOPEN function called [%s] ===", EIF_url);

					uopen_command(EIF_url);
					
				}
*/				
				if (After_WAP_start_what_call == 3)				// EIF_url로 dns query를 한다. 
				{
					extern BOOL EIF_dns_query;
					
					boolean ip_setted = FALSE;
					char eachValue[2];
					char OptionValue = 0;
					int i = 0;
					char * url_or_ip = EIF_url;

					After_WAP_start_what_call = 0;

					CLNTa_log(1,0,"[%s] url_or_ip", url_or_ip);

					if ((PingCnt == 0) && (continue_ping == TRUE))				// user input Ping count is 0 and continue pinging then continue ping -> end
					{
						pinging = FALSE;										// current pinging end 시킨다.
						break;
					}

					// 여기서 dns query 를 해야할지 아닐지를 결정 
					if ((*url_or_ip < 0x30) || (0x39 < *url_or_ip))				// 숫자가 아니면 ......
					{
						EIF_dns_query = TRUE;
						dnsaccess = TRUE;
						receiveOK = TRUE;
						retry =0;
	//					memset((BYTE*)&Dest_IP,0x00,sizeof(Dest_IP));

						ui_waptimer_set(4000,FALSE);				//DNS request limit time


						CLNTa_log(1,0,"length of DNS query EIF_url %d",strlen(EIF_url));
						CLNTa_log(1, 0, "===== process_dns start =====");

						process_dns(EIF_url);		

					}
					else
					{
						while (*url_or_ip != NULL)
						{
							memset((char*)eachValue, '\0', 2);

							if ((0x30 <= *url_or_ip) && (*url_or_ip <= 0x39))		// 숫자이면.....
							{								
								ip_setted = TRUE;

								eachValue[0] = *url_or_ip;
								OptionValue *= 10;
								OptionValue = OptionValue + atoi((char*)eachValue);

								CLNTa_log(1,0,"OptionValue[%d],*url_or_ip[%c]",OptionValue, *url_or_ip);

								url_or_ip++;
							}
							if (*url_or_ip == '.')
							{
								Dest_IP[i] = OptionValue;
								OptionValue = 0;

								CLNTa_log(1,0,"Dest_IP[%d] - [%d]", i, Dest_IP[i]);

								i++;
								url_or_ip++;
								
							}
							if (*url_or_ip == NULL)
							{
								Dest_IP[i] = OptionValue;

								CLNTa_log(1,0,"ip_addr[%d] - [%d]", i, Dest_IP[i]);

								OptionValue = 0;
							}	

						}

						CLNTa_log(1,0, "[[[ EIF WAP DNS query.......!!! ]]]");

						EIF_dns_query = TRUE;

					}
					
				}

				if ((EIF_dns_query) && (Dest_IP[0] != 0))						// 최초 uping 보내는 경우 
				{
					int i;
					EIF_dns_query = FALSE;

					memset((char*)&Dest_url_IP, 0x00, sizeof(Dest_url_IP));
					for (i = 0 ; i < 4 ; i++)
					{
					    ((char*)&Dest_url_IP)[4 - i - 1] = Dest_IP[i];
					}

					ping_cnt = 0;				// ping 보낸 횟수를 count
					PongCnt = 0;				// pong 받은 횟수를 count
					ping_MIN = 0;
					ping_MAX = 0;
					ping_RTT = 0;

					ping_seq = 0;
					ping_timeStamp =0;


					CLNTa_log(1,0, "Dest_IP[0]-[%d],Dest_IP[1]-[%d]\nDest_IP[2]-[%d],Dest_IP[3]-[%d]",Dest_IP[0],Dest_IP[1],Dest_IP[2],Dest_IP[3]);


					if((nTimeout > 0) && (PingCnt > 0) && (PingCnt <255))				// timeout 이 0보다 크고 PingCnt가 0과 255 사이인 경우 
					{
						continue_ping = FALSE;		
						ping_cnt++;
						uping_command(ping_cnt, Dest_url_IP);

						rex_set_timer( &ui_uping_timer, nTimeout*1000 );				// 최초 uping 보내고 Timer set 

					}
					else if ((nTimeout > 0) && (PingCnt == 255))						// timeout 이 0보다 크고 PingCnt가 255 즉 Continuous ping 인 경우 
					{	
						continue_ping = TRUE;
						ping_cnt++;
						uping_command(ping_cnt, Dest_url_IP);
						rex_set_timer( &ui_uping_timer, nTimeout*1000 );				// 최초 uping 보내고 Timer set 
						CLNTa_log(1,0, "<<<<< CONTI_UPONG start!!!!! >>>>>");
					}
					break;		

				} // end of if ((EIF_dns_query) && (Dest_IP[0] != 0))

#endif	// #ifdef EIF_WAP

				//-----------------------------------------------
				//다이알로그 처리 - 패스워드
				if(ui.event == UI_WAP_PASSWORD_DLG_F)
				{
					Process_PasswordDialog_For_Input(TRUE);
					break;
				}
				//-----------------------------------------------
				//다이알로그 처리 - 프롬프트
				if(ui.event == UI_WAP_PROMPT_DLG_F)
				{
					Process_PromptDialog_For_Input();
					break;
				}

				close_view_is_ok = TRUE;

				initialaccess = FALSE;

				ui_get_keys( TRUE );     /*  keys, thank you */

						
				//==================================================
				// Key 처리 루틴
				//==================================================
				//for goback..
				if(firstsite && (g_nViewMode == WAP_VIEW_MODE_WML))
				{
					CLNTa_log(1,0,"first site 에 있음");

					if(((ui.event == HS_CLR_K)||(ui.event == HS_WAP_K)||
						(ui.event == HS_PF2_K))&&(g_nMenuMode == WAP_MENU_MODE_MAIN))
					{
						MSG_ERROR("Break1..",0,0,0);
						ui.event = HS_PF2_K;
					}
					else if((ui.event == HS_CLR_K)||(ui.event == HS_WAP_K) || (ui.event == HS_PF2_K))
					{
						MSG_ERROR("Break2..",0,0,0);
						ui.event = HS_NONE_K;
					}

				}

				PrevKey = ui.event;
				nResultHandleKey = WAP_HandleKeys(ui.event);

				ui.event = UI_NONE_F;

				Process_After_HandleKeys(nResultHandleKey);
				break;

			}//end of HANDLE_KEY_S
			
			case AFTER_PHONEBOOK_S:
			{

#ifdef PHONE
				ui.r_IdleAniFlag = old_idleimage;
#endif

				//----------------------------
				// 전화부 -> WAP
				//----------------------------

				isFromWap = FALSE;

				MSG_MED("--------[AFTER_PHONEBOOK_S]--------",0,0,0);

				//--------------------------------------------------
				// WAP 상황에서는 menu timer 를 죽인다

				ui_menutimer_set( ( int4 )0 ); /* clear our timer */

				//--------------------------------------------------
				
				WAP_Repaint();

				//////////////////////////////////////////////
				// Handle earpiece down case
				//////////////////////////////////////////////
				//if(ui.event == HS_EAR_DOWN_K || ui.event == UI_TOIDLE_F || ui.event == HS_END_K)

				if(ui.event == HS_EAR_DOWN_K || ui.event == HS_END_K)
				{	
					wap_handle_earpiece_down();
				}
				//////////////////////////////////////////////
				else
				{
					ui.event = UI_NONE_F;
					new_state = HANDLE_KEY_S;
				}

				break;

			}//end of AFTER_PHONEBOOK_S

			case AFTER_ALPHAEDIT_S:
			{
				isFromWap = FALSE;

				//----------------------------
				// a-Edit -> WAP
				//----------------------------

				MSG_MED("--------[AFTER_ALPHAEDIT_S]--------",0,0,0);

				//--------------------------------------------------
				// WAP 상황에서는 menu timer 를 죽인다

				ui_menutimer_set( ( int4 )0 ); /* clear our timer */

				//--------------------------------------------------
				
				//--------------------------------------------------

				ui.smsalpha = FALSE;//기호
				
				//--------------------------------------------------
				if(ui.event == HS_PF2_K)//취소키로 빠져나옴
				{	
					g_pCurInput = NULL;//init
	
					g_nViewMode = WAP_VIEW_MODE_WML;
					WAP_Display();
				
					ui.event = UI_NONE_F;
					new_state = HANDLE_KEY_S;
					break;
				}

				//--------------------------------------------------
				//얻어온 값을 현재 input 에 넣어준다
				if(g_pCurInput != NULL)//입력
				{

					int length = get_length(ui.ed.buf, WAP_MAX_INPUT);


					memset(g_pCurInput->text, '\0', WAP_MAX_INPUT+1);
#ifdef DEBUG_MMI
					if(g_pCurInput->isPassword)
						CLNTa_log(1, 0, "pass>ui.ed.buf : %s", ui.ed.buf);
					else
						CLNTa_log(1, 0, "no>ui.ed.buf : %s", ui.ed.buf);
#endif
					if(strlen(g_pCurInput->format) == 0)
					{

						memcpy(g_pCurInput->text, ui.ed.buf, length);

					}
					else
					{
						int nIndexDel = 0;//delimeter
						int nIndexSrc = 0;//source
						int nIndexDest = 0;//destination

						int nKorean = 0;

						for(nIndexSrc=0; nIndexSrc<length; nIndexSrc++)
						{
							if(ui.ed.buf[nIndexSrc] & 0x80)//한글
							{
								nKorean++;
								nIndexSrc++;
							}
						}


						nIndexSrc = 0;

						for(nIndexDel=0; nIndexDel<length-nKorean; nIndexDel++)
						{

							if(is_delimeter[nIndexDel] == 0x00)//copy
							{
								if(ui.ed.buf[nIndexSrc] & 0x80)//한글(2회 copy)
								{
									g_pCurInput->text[nIndexDest] = ui.ed.buf[nIndexSrc];
									nIndexDest++;
									nIndexSrc++;
								}
								g_pCurInput->text[nIndexDest] = ui.ed.buf[nIndexSrc];
								nIndexDest++;
								nIndexSrc++;
							}
							else//skip
							{
								nIndexSrc++;
							}
						}//end of for

					}
					g_pCurInput = NULL;//init
				}
		
				//--------------------------------------------------
				g_nViewMode = WAP_VIEW_MODE_WML;
				WAP_Formatting();
				WAP_Display();

				//////////////////////////////////////////////
				// Handle earpiece down case
				//////////////////////////////////////////////
				//if(ui.event == HS_EAR_DOWN_K || ui.event == UI_TOIDLE_F || ui.event == HS_END_K)

				if(ui.event == HS_EAR_DOWN_K  || ui.event == HS_END_K)
				{	
					wap_handle_earpiece_down();
				}
				//////////////////////////////////////////////
				else
				{
					ui.event = UI_NONE_F;
					new_state = HANDLE_KEY_S;
				}


				break;

			}//end of AFTER_ALPHAEDIT_S

			case AFTER_ALPHAEDIT_FOR_NEWURL_S:
			{
				int length;

				isFromWap = FALSE;

				//----------------------------
				// a-Edit -> WAP
				//----------------------------

				MSG_MED("--------[AFTER_ALPHAEDIT_FOR_NEWURL_S]--------",0,0,0);

				//--------------------------------------------------
				// WAP 상황에서는 menu timer 를 죽인다

				ui_menutimer_set( ( int4 )0 ); /* clear our timer */

				//--------------------------------------------------

				//--------------------------------------------------

				ui.smsalpha = FALSE;//기호

				WAP_Repaint();

				//////////////////////////////////////////////
				// Handle earpiece down case
				//////////////////////////////////////////////
				//if(ui.event == HS_EAR_DOWN_K || ui.event == UI_TOIDLE_F || ui.event == HS_END_K)

				if(ui.event == HS_EAR_DOWN_K|| ui.event == HS_END_K)
				{	
					wap_handle_earpiece_down();
					break;
				}
				//--------------------------------------------------
				if(ui.event == HS_PF2_K)//취소키로 빠져나옴
				{	
					new_state = HANDLE_KEY_S;
					ui.event = UI_NONE_F;
					break;
				}

				//--------------------------------------------------
				length = get_length(ui.ed.buf, WAP_MAX_INPUT);

				if(length > 0)
				{
					//char new_url[WAP_MAX_INPUT];
					memset(new_url, '\0', sizeof(new_url));
					{

						memcpy(new_url, ui.ed.buf, length);

					}
					//--------------------------------------------------
					//사용자의 Key 입력을 막는다

					ui_get_keys( FALSE );retry =0;


					new_state = LOADING_S;
					receiveOK = TRUE;
            #ifdef WAP_USE_DORM
                    wap_clk_key = FALSE;
            #endif//#ifdef WAP_USE_DORM
					displayloading(1,TRUE);
					newurl_input = TRUE;
					//--------------------------------------------------
					MSG_MED("new url %s",new_url,0,0);			

					//똑같은 것을 재시도 할 수 있으므로 FALSE로 둔다.
					SKY_loadURL(g_pCard->viewId, new_url, FALSE);
					CLNTa_log(1,0," NEW URL %s",new_url);

					//--------------------------------------------------
					g_nMenuMode = WAP_MENU_MODE_NONE;
					//--------------------------------------------------
				}
				//////////////////////////////////////////////
				else//URL을 입력하지 않은 경우
				{
					new_state = HANDLE_KEY_S;

					ui.event = UI_NONE_F;

				}
												
				break;

			}//end of AFTER_ALPHAEDIT_FOR_NEWURL_S

			case AFTER_ALPHAEDIT_FOR_DIALOG_PROMPT_S:
			{
				int length;

				isFromWap = FALSE;

				//----------------------------
				// a-Edit -> WAP
				g_nDlgMode = WAP_DLG_MODE_NONE;
				WAP_Repaint();
				//----------------------------

				MSG_MED("--------[AFTER_ALPHAEDIT_FOR_DIALOG_PROMPT_S]--------",0,0,0);

				//--------------------------------------------------
				// WAP 상황에서는 menu timer 를 죽인다

				ui_menutimer_set( ( int4 )0 ); /* clear our timer */

				//--------------------------------------------------

				//--------------------------------------------------

				ui.smsalpha = FALSE;//기호

				
				//--------------------------------------------------
				if(ui.event == HS_PF2_K)//취소키로 빠져나옴
				{	
					//이전 값을 다시 넣어준다
					length = strlen(g_pDlgMessage2);

					if(length > 0)
						memcpy(ui.ed.buf, g_pDlgMessage2, length+1);
					else
						ui.ed.buf[0] = '\0';
				}

				//--------------------------------------------------
				//얻어온 값을 AUR에게 돌려준다

				length = get_length(ui.ed.buf, WAP_MAX_INPUT);

				if(length > 0)
				{
					WCHAR *answer;

					char temp[WAP_MAX_INPUT];
					memset(temp, '\0', WAP_MAX_INPUT);

					memcpy(temp, ui.ed.buf, length);

					//char -> WCHAR
					length = UniLenOfKSCStr((char *)temp);
					answer = (WCHAR *)wip_malloc(sizeof(WCHAR)*(length+1));
					KSC2UniString((unsigned char *)temp, (WCHAR *)answer);
										
					SKY_promptDialogResponse(1, g_nDlgId, answer);					
				}
				else
				{
					SKY_promptDialogResponse(1, g_nDlgId, NULL);
				}
							
				//////////////////////////////////////////////
				// Handle earpiece down case
				//////////////////////////////////////////////
				//if(ui.event == HS_EAR_DOWN_K || ui.event == UI_TOIDLE_F || ui.event == HS_END_K)

				if(ui.event == HS_EAR_DOWN_K || ui.event == HS_END_K)
				{	
					wap_handle_earpiece_down();
				}
				//////////////////////////////////////////////
				else
				{
					ui.event = UI_NONE_F;
					new_state = HANDLE_KEY_S;
				}

				break;

			}//end of AFTER_ALPHAEDIT_FOR_DIALOG_PROMPT_S

			case AFTER_ALPHAEDIT_FOR_DIALOG_ID_S:
			{
				int length;

				isFromWap = FALSE;

				//----------------------------
				// a-Edit -> WAP
				//----------------------------

				MSG_MED("--------[PASSWORD_DIALOG_ID_S]--------",0,0,0);

				//--------------------------------------------------
				// WAP 상황에서는 menu timer 를 죽인다

				ui_menutimer_set( ( int4 )0 ); /* clear our timer */

				//--------------------------------------------------

				//--------------------------------------------------

				ui.smsalpha = FALSE;//기호

				
				//--------------------------------------------------
				if(ui.event == HS_PF2_K)//취소키로 빠져나옴
				{	
					SKY_passwordDialogResponse(g_pCard->viewId, g_nDlgId, NULL, NULL);

					g_pDlgMessage = NULL;
					g_pDlgMessage2 = NULL;

					ui.event = UI_NONE_F;
					new_state = HANDLE_KEY_S;
					break;
				}

				//--------------------------------------------------
				//얻어온 값을 AUR에게 돌려준다

				length = get_length(ui.ed.buf, WAP_MAX_INPUT);

				if(length > 0)
				{
					if(g_pDlgMessage) wip_free((void *)g_pDlgMessage);
					g_pDlgMessage = (char *)wip_malloc(sizeof(char)*(length+1));

					memset(g_pDlgMessage, '\0', length);

					memcpy(g_pDlgMessage, ui.ed.buf, length);

#ifdef DEBUG_MMI
					CLNTa_log(1, 0, "id) %s", g_pDlgMessage);
#endif
					//--------------------------------------------------

					ui.event = UI_NONE_F;


					//패스워드를 입력받는다
					Process_PasswordDialog_For_Input(FALSE);
					break;
				}
				//////////////////////////////////////////////
				// Handle earpiece down case
				//////////////////////////////////////////////
				//if(ui.event == HS_EAR_DOWN_K || ui.event == UI_TOIDLE_F || ui.event == HS_END_K)

				if(ui.event == HS_EAR_DOWN_K  || ui.event == HS_END_K)
				{	
					wap_handle_earpiece_down();
				}
				//////////////////////////////////////////////
				else
				{
					//ID를 재입력받는다
					Process_PasswordDialog_For_Input(TRUE);
				}

				break;

			}//end of AFTER_ALPHAEDIT_FOR_DIALOG_ID_S

			case AFTER_ALPHAEDIT_FOR_DIALOG_PASSWORD_S:
			{
				int length;

				isFromWap = FALSE;

				//----------------------------
				// a-Edit -> WAP
				g_nDlgMode = WAP_DLG_MODE_NONE;
				WAP_Repaint();
				//----------------------------

				MSG_MED("--------[PASSWORD_DIALOG_PASSWORD_S]--------",0,0,0);

				//--------------------------------------------------
				// WAP 상황에서는 menu timer 를 죽인다

				ui_menutimer_set( ( int4 )0 ); /* clear menu timer */

				//--------------------------------------------------

				//--------------------------------------------------

				ui.smsalpha = FALSE;//기호
				
				//--------------------------------------------------
				if(ui.event == HS_PF2_K)//취소키로 빠져나옴
				{	
					SKY_passwordDialogResponse(g_pCard->viewId, g_nDlgId, NULL, NULL);

					g_pDlgMessage = NULL;
					g_pDlgMessage2 = NULL;

					ui.event = UI_NONE_F;
					new_state = HANDLE_KEY_S;
					break;
				}

				//--------------------------------------------------
				//얻어온 값을 AUR에게 돌려준다

				length = get_length(ui.ed.buf, WAP_MAX_INPUT);

				if(length > 0)
				{
					if(g_pDlgMessage2) wip_free((void *)g_pDlgMessage2);
					g_pDlgMessage2 = (char *)wip_malloc(sizeof(char)*(length+1));

					memset(g_pDlgMessage2, '\0', length);

					memcpy(g_pDlgMessage2, ui.ed.buf, length);


					//--------------------------------------------------
					//ID와 패스워드를 AUR에게 돌려준다
					SKY_passwordDialogResponse(g_pCard->viewId, g_nDlgId,
						g_pDlgMessage, g_pDlgMessage2);

					g_pDlgMessage = NULL;
					g_pDlgMessage2 = NULL;

					//--------------------------------------------------

					ui.event = UI_NONE_F;

					new_state = HANDLE_KEY_S;

					break;
				}
				//////////////////////////////////////////////
				// Handle earpiece down case
				//////////////////////////////////////////////
				//if(ui.event == HS_EAR_DOWN_K || ui.event == UI_TOIDLE_F || ui.event == HS_END_K)

				if(ui.event == HS_EAR_DOWN_K || ui.event == HS_END_K)
				{	
					wap_handle_earpiece_down();
				}
				//////////////////////////////////////////////
				else
				{
					//패스워드를 재입력받는다
					Process_PasswordDialog_For_Input(FALSE);
				}

				break;

			}//end of AFTER_ALPHAEDIT_FOR_DIALOG_PASSWORD_S

#ifdef FOR_MS
#ifndef WAP_USE_SECURE
			case WTLS_SESSION_START_S:
			{

				ui_get_keys(TRUE);

				bStopKey = TRUE;
				wtls_enc_start = FALSE;
				displayloading(6,TRUE);

				new_state = WTLS_CLIENT_HELLO_S;

				ui.event = UI_NONE_F;


				memset(ucVerifyDataOpen,0x00,1500);ulVerifyDataLenOpen = 0;

				bWstart = TRUE;

				processcommonWTLS(ui.event);

				break;
			}//end of case WTLS_SESSION_START_S

			case WTLS_CLIENT_HELLO_S:
			//실제로 client hello를 행한후, Loadurl을 행하는 부분임 !!Method = Post로 해야 함
			{

				ui_get_keys(TRUE);


				CLNTa_log(1,0,"=====WTLS_CLIENT_HELLO_S=====");

				//g_bDrawBar = TRUE;

				if(bWstart)
				{
					bWstart = FALSE;
					memset(&ui_stpaq,0, sizeof(stPaq));

					bStopKey  =  TRUE;
				/*------------------------------------------------------------------------
					- Dorm mode에 있을 시는 WTLS session을 열지 않고 Traffic을 일단 열어준다.
					- KEY select시 SaveWtls에 url이 저장되므로 아무런 상관없이 Session을 
					  열어줄 수 있다.
				  ------------------------------------------------------------------------*/
				#ifdef WAP_USE_DORM

					if(ui.wap_dorm_mode)
					{
						extern void wap_reorigination_attempt(void);
						CLNTa_log(1,0,"wap dorm reorigination in WTLS mode");						
						return_ssls =1;
						CLNTa_log(1,0,"500");
						wap_reorigination_attempt();
						break;
					}
					else
					{
						CLNTa_log(1,0,"Normal WTLS processing");
						return_ssls = 0;
						return_ssls = ssls_open_send(&ui_stpaq,&wtlserr,(char*)SaveWtls,wtlsMode,SktCipher);						
					}

				#else//#ifdef WAP_USE_DORM
					return_ssls = ssls_open_send(&ui_stpaq,&wtlserr,(char*)SaveWtls,wtlsMode,SktCipher);						
				#endif//#ifdef WAP_USE_DORM
					CLNTa_log(1,0,"여기 in WLTS");
					//jjkyg 08/21 add

					ui_waptimer_set(7000,FALSE);

				}

				if(return_ssls == 0)
				{
					//Check ERROR.. npErr check할 것.. Close connection..wtlser 채크할 것 
					//jjkyg 08/21 add

					ui_waptimer_set(0,FALSE);new_state = ERROR_S;

					isFailureWTLSA = TRUE;ui.event = UI_NONE_F;

					CLNTa_log(1,0,"ERROR in CLIENT Hello");				
					//jjkyg 08/21 add
					break;
				}

				if(ui.event == UI_CHNKEY_F)
				{
					bStopKey  = TRUE;
					if(spSKT !=NULL)
					{
						CLNTa_log(1,0,"receive Change key in clinet hello state");
						new_state = WTLS_CHANGE_KEY_S;
						ui.event  = UI_SEVERHELLO_F;
						clear_ssls();
						retry = 0;

					   	ui_waptimer_set(0,FALSE);

						CLNTa_log(1,0,"이리로1??");
					}
					else
					{

						ui_waptimer_set(0,FALSE);

						bStopKey = FALSE;
						new_state = ERROR_S;
						isFailureWTLSA = TRUE;
						ui.event = UI_NONE_F;
						CLNTa_log(1,0,"이리로2??");
					}
					CLNTa_log(1,0,"go Change Key state");
					break;
				}

				processcommonWTLS(ui.event);

				break;
			}

			case WTLS_CHANGE_KEY_S:
			{
				int nRet = -1;

				ui_get_keys(TRUE);


				CLNTa_log(1,0,"======WTLS_CHANGE_KEY_S=====");

				switch(ui.event)
				{
					case UI_SEVERHELLO_F://parsing the received message
					{
						if(spSKT->ucState == ST_WAIT)
						{
							extern char contentBuffer[MAX_CONTENT_SIZE];

						//jjkyg added 2000/05/20==========================
							if(ui.wtlslength==0)
							{
								go_storeWml(TRUE);
							
								if(wtls_enc_start) wtls_enc_start = FALSE;

								isFailureWTLSB = TRUE;
								new_state = ERROR_S;
								ui.event = UI_NONE_F;

								CLNTa_log(1,0,"LENGTH =0");
								break;							
							}
						//jjkyg added 2000/05/20==========================

							bStopKey = TRUE;

							nRet = ssls_recv_hello(&ui_stpaq,&wtlserr,(uc*)contentBuffer,(UINT16)ui.wtlslength,(char*)SaveWtls,SktCipher);
							//jjkyg 08/21 add

							ui_waptimer_set(7000,FALSE);


							
							CLNTa_log(1,0,"Send Length %d",ui.wtlslength);
							
							ui.wtlslength = 0;
																												
							if(nRet < 0)
							{
								//jjkyg 08/21 add

								ui_waptimer_set(0,FALSE);


								go_storeWml(TRUE);
								if(wtls_enc_start) wtls_enc_start = FALSE;

								isFailureWTLSB = TRUE;
								new_state = ERROR_S;
								ui.event = UI_NONE_F;

								CLNTa_log(1,0,"SERVERHELLO FAIL");
								
								break;
							}
							else if(spSKT->ucState == ST_OPEN)
							{
								retry=0;

								ui_waptimer_set(0,FALSE);


								//Handshake process end...
								wtls_enc_start = TRUE;
								clear_ssls();

								//2000/03/11 for secure jjkyg
								bStopKey  = TRUE;

								go_storeWml(FALSE);									
								Wtlshello = FALSE;
															
								new_state = LOADING_S;
								//ui.event  = UI_NETCONNECTED_F;
                                receiveOK = TRUE;

								#ifdef debug
									CLNTa_log(1,0,"NEGO finished");
								#endif
								break;
							}
							//#ifdef debug
							CLNTa_log(1,0,"SEVER HELLO SUCCESS");
							//#endif//#ifdef debug
						}
						ui.event = UI_NONE_F;
						break;
					}
	
					case UI_CHNKEY_F:
					//FINISHED 메세지가 들어왔다는 것
					{
						retry = 0;

						ui_waptimer_set(0,FALSE);


						if(spSKT->ucState == ST_EXKEY)
						{
							extern char contentBuffer[MAX_CONTENT_SIZE];
					
							//===jjkyg 2000/05/20============
							if(ui.wtlslength ==0)
							{
								go_storeWml(TRUE);
						
								if(wtls_enc_start) wtls_enc_start = FALSE;

								isFailureWTLSB = TRUE;
								new_state = ERROR_S;
								ui.event = UI_NONE_F;

								CLNTa_log(1,0,"LENGTH =0");
								break;							
							}
							//===jjkyg 2000/05/20============

							bStopKey = TRUE;

							CLNTa_log(1,0,"UI_CHNKEY_F received in wapstate.c");

							nRet = ssls_recv_hello(&ui_stpaq,&wtlserr,(uc*)contentBuffer,(UINT16)ui.wtlslength,(char*)SaveWtls,SktCipher);

							//jjkyg 08/21 add

							ui_waptimer_set(7000,FALSE);


							ui.wtlslength = 0;
							if(nRet < 0)
							{	
								//jjkyg 08/21 add

								ui_waptimer_set(0,FALSE);

								go_storeWml(TRUE);
								if(wtls_enc_start)	wtls_enc_start = FALSE;
								isFailureWTLSB = TRUE;
								new_state = ERROR_S;
								ui.event = UI_NONE_F;

								CLNTa_log(1,0,"NEGO FAIL");
							
								ui.event = UI_NONE_F;
							}
							else if (spSKT->ucState ==ST_OPEN)
							{
								//jjkyg 08/21 add

								ui_waptimer_set(0,FALSE);


								//Handshake process end...
								wtls_enc_start = TRUE;
								clear_ssls();

								//2000/03/11 for secure jjkyg
								bStopKey  = TRUE;

								CLNTa_log(1,0,"storewml--7");
								go_storeWml(FALSE);									
								Wtlshello = FALSE;
								
								new_state = LOADING_S;
                                receiveOK = TRUE;
								//ui.event  = UI_NETCONNECTED_F;

								CLNTa_log(1,0,"NEGO finished");

								//Nego finished
								bSetKey = TRUE;								
								break;
							}
						}
						ui.event = UI_NONE_F;
						break;
					}
				}//end of switch
				processcommonWTLS(ui.event);

				break;
			}		
#endif//#ifndef WAP_USE_SECURE
#endif//#ifdef FOR_MS

			case EXIT_S:
			{
#ifndef QUALCOM30_WAP
			    extern boolean dsi_hold_orig_enabled;	
#endif //#ifndef QUALCOM30_WAP
				extern void init_buffer(redirect_config* buf);
				if(redirect.bCorrect)
				{
					init_buffer(&redirect);
					CLNTa_log(1,0,"redirect.bCorrect =TRUE");
				}

// ===============================================================================			
// zeus(yjchoi) 00.12.05  EIF WAP(External Interface for WAP)
// uping 종료시 다음번 uping을 위해 초기화시켜준다.
#ifdef EIF_WAP		
				Dest_IP[0] = 0; Dest_IP[1] = 0; Dest_IP[2] = 0; Dest_IP[3] = 0;
				memset((char*)&Dest_url_IP, 0x00, sizeof(Dest_url_IP));
#endif	// EIF_WAP
// ===============================================================================			


            #ifdef FEATURE_SMART_BATTERY
				SmbEvent(SMB_END_TALK_S);
            #endif//#ifdef FEATURE_SMART_BATTERY

				WAP_Time_Stmap(FALSE);ui_get_keys(TRUE);			


                if(dsi_hold_orig_enabled)
                {
                   process_hold_orig();
                }
                
                if(bsetonlyone)
				{
				    bsetonlyone = FALSE;                    
					processExitOnly();						
				}

				if(!browserTerComp && !ui.wap_release)	
				{
					CheckBrowerDown();
					break;
				}
                else
                {
                    if(save_event == UI_NETDISCONNECTED_F)
                    {
                        CLNTa_log(1,0," Receive save_event");
                        ui.event = UI_NETDISCONNECTED_F;
                        save_event = UI_NONE_F;
                    }
                }

				process_exit_event();

  			    ui.clrnum_flag = FALSE;	ui.tog_flag    = TRUE;


			#ifdef WAP_USE_SKT_NEW

				if(ui.wap_release || makecall) 
				{
					CLNTa_log(1,0,"SKIP remain part in EXIT_S");
					break;
				}

			#endif//#ifdef WAP_USE_SKT_NEW

				if(ui.from_where == 2)
				{
					ui.event  = HS_PF2_K;
				}
				else  ui.event = HS_END_K;


      			break;
			}// end of EXIT_S
		}// end of switch
	}// end of while
 return ret_state;

}//end of uistate_wap()
#endif //PORTING_B0_WAP

/************************************************************************************/
/*																					*/
/*						WML 문서 Loading/Terminate 관련								*/
/*																					*/
/************************************************************************************/
void WAP_Initialize_MMI(void)
{
	g_nAniBarCount = 0;g_bDrawBar = FALSE;

#ifdef WAP_USE_DORM
    if(SKY_IS_WAP_DORM() || SKY_IS_WAP_RELEASE())//if(ui.wap_dorm_mode || ui.wap_release)
	{
		CLNTa_log(1,0," Not initialzie MMI");
	}else
#endif//#ifdef WAP_USE_DORM
	{
		if(g_pDlgMessage) wip_free((void *)g_pDlgMessage);
		g_pDlgMessage = NULL;g_nDlgMode = WAP_DLG_MODE_NONE;
		g_nTimeStamp = 0;
	}

	g_bGettingContent = FALSE;g_bTerminatedWAP = FALSE;isErrorState = FALSE;			
}

LOCAL void Process_Loading(int nKey)
{
	switch(nKey)
	{
#ifdef PORTING_B0_WAP
	    case UI_WAPPIC_F:
		{
			(void) rex_set_sigs( &wap_tcb, WAP_BAR_SIG );		
			ui.event = UI_NONE_F;
			break;
		}

#ifdef WAP_USE_PZID2
        case UI_CONNECTED_F:
        {
            if(ui.wap_pzid)
            {
                uiscrn_ann( HS_AN_INUSE, HS_AN_INUSE );
			    dbi.in_use = TRUE;
			    db_put( DB_IN_USE, &dbi );

			    ui.beep_updown = TRUE;
			    ui.call = TRUE;
    		    ui.event = UI_NONE_F;
            }
            break;
        }
#endif//#Ifdef WAP_USE_PZID2

#ifdef WAP_USE_DORM
		case UI_WANT_RECONNECT_F:
		{
			CLNTa_log(1,0,"Reconnect Want Flag received");
			new_state  = ERROR_S;isFailOrig = TRUE;
			ui.event   = UI_NONE_F;
			break;
		}
		case UI_WAP_DORM_F:
		{
			CLNTa_log(1,0,"WAP go to DORMANT MODE %d",ui.wap_dorm_mode);
			EnterDormMode();
			ui.event = UI_NONE_F;
			break;
		}

		case UI_WAKEUP_F:
		{
			CLNTa_log(1,0,"Receive WAKEUp so sucess reorig");
			/*==========================================
				WAP reorigination 성공시 이리로 옴 
			 ===========================================*/
			if(ui.wap_dorm_mode)
			{
				CLNTa_log(1,0," SUCCESS REORIGINATION");
			#ifdef WAP_USE_PZID2
				if(!ui.wap_pzid)
			#endif//#ifdef WAP_USE_PZID2
				{	
					dbi.in_use = TRUE;
					db_put( DB_IN_USE, &dbi );					
					uiscrn_ann( HS_AN_INUSE, HS_AN_INUSE );
				}
			#ifdef WAP_USE_PZID2
				else
				{
					CLNTa_log(1,0,"pzid false--2");
                    ui.wap_pzid = FALSE;						
				}
			#endif//#ifdef WAP_USE_PZID2
				
				ui.wap_dorm_mode = FALSE;				
				WapTraffic_Term = FALSE;
				
				ui.beep_updown = TRUE;
				ui.call = TRUE;

			#ifdef FOR_MS//Check하여야 함 		
				if(wtls_enc_start)	bStopKey  = FALSE;
			#endif//#ifdef FOR_MS			
	
				if(!wap_clk_key)
				{
					if(!wap_rcv_page) SendRequest();
					else wap_rcv_page  = FALSE;
				}
				else
				{
					wap_clk_key   = FALSE;
					wap_rcv_page  = FALSE;
					Do_cancel(FALSE);
				}
				ui.event = UI_NONE_F;
			}
			break;
		}
#endif//#ifdef WAP_USE_DORM
		case UI_WAPMNG_F:
		{
			if (isBadURL)
			{
				isBadURL = FALSE;
				bCreateReq = FALSE;	new_state  = state ;
				ui.event   = UI_WAPREQ_F;
				break;
			}
			else
			{
				g_nAniBarCount = 0;	new_state = HANDLE_KEY_S;
				ui.event = UI_NONE_F;
				break;
			}
		}
		
	/*============================================================
		       잠깐동안 END를 못하게 하기 위한 루틴임(2/9)
			   밑의 flag을 받으면 EXIT하자...
	 =============================================================*/
		case UI_WAPREQ_F:
		{
			MSG_ERROR("===UI_WAPREQ_F======",0,0,0);
			bCreateReq = FALSE;	new_state  = state ;
			ui.event = WaitKey;WaitKey = UI_NONE_F;
			break;
		}

		case UI_DOALERT_F:
		{
			entertain = FALSE;newurl_input = FALSE;
			CLNTa_log(1,0,"receive alert --loading_s");
	#ifdef WAP_USE_DORM						
			if(!ui.wap_dorm_mode)
	#endif//#ifdef WAP_USE_DORM						
			{
				if(load_data)
				{
					if(!Wtlshello)
					{
						SKY_stop(g_pCard->viewId);
		#ifdef CONNECT_SESSION
						close_udp_port();
		#endif//#ifdef CONNECT_SESSION
					}	
				}
			}
			terminate_timer();
			report_event(WAP_DRAW_STOP_EVENT);
	#ifdef WAP_USE_DORM						
			if(!ui.wap_dorm_mode)
	#endif//#ifdef WAP_USE_DORM						
			{
				wap_display_exit_for_receive_call();

				new_state = EXIT_S;rcvcall =TRUE;
				ui_dispmsgtimer_set(1500);
				ui.event = UI_NONE_F;
			}
	#ifdef WAP_USE_DORM
			else
			{
				isFirstWapToIdle = TRUE;ui.tog_flag  = TRUE;displayterminate();
				state = WAP_RET_CALL_S;ui.event = UI_DOALERT_F;						
			}	
	#endif//#ifdef WAP_USE_DORM
			break;
		}

		case UI_MAKECALL_F:
		{
			MSG_MED("--------[UI_MAKECALL_F]--------",0,0,0);
			do_makecall();
			break;
		}

#ifdef WAP_USE_DORM
   		case UI_WAPTIME_F :
		{
			ui_waptimer_set(0,FALSE);
			ui.event = UI_NONE_F;

			if(wap_dorm_active() == TRUE && wap_count == 0)
			{
				//flow control이 TRUE일 경우에만 이 루틴을 수행함
				extern void wap_reorigination_attempt(void);		
				if((dsi_srvc_mode == DS_BROWSER_PKT_SRVC_MODE) && (wap_duration !=0))
				{
					CLNTa_log(1,0,"WAP reorigination in wapstate.c");
					CLNTa_log(1,0,"600");
					wap_reorigination_attempt();
				}		
				wap_count++;
			}
			else
			{
				wap_count =0;isFailureLoadURL = TRUE;new_state = ERROR_S;
			}
			break;
		}
#endif//#ifdef WAP_USE_DORM

	case UI_NETCONNECTED_F : //PPP setup 완료
	{
#ifdef WAP_USE_PZID2
       if (ui.wap_pzid)
       {
           ui.wap_pzid = FALSE;
            if(!wap_clk_key)
	        {
		        if(!wap_rcv_page) SendRequest();
		        else wap_rcv_page  = FALSE;
	        }
	        else
	        {
	    	    wap_clk_key = FALSE;wap_rcv_page = FALSE;
                Do_cancel(FALSE);
	        }
       }
#endif//#ifdef WAP_USE_PZID
		ui.event = UI_NONE_F;		
		break;
	}//end of UI_NETCONNECTED_F

	case UI_DISPMSGTIMER_F:
	{
		if((bReceiveData)||(iGoorNew==1))//goback from HANDLE_KEY_S 
		{
			new_state   = state;
			ui.event    = UI_NETDIS_F;
			if(iGoorNew == 1)
				isNotGoBackHistory = TRUE;
		}
		break;
	}

	case UI_MMIWAIT_F://from readcache....
	case UI_INNCARD_F://udpa_sendrequest에서 셋팅(Air로 나간다는 이야기..)
	{
		ui_dispmsgtimer_set(0);		
		ui.event    = UI_NONE_F;
		break;
	}
		
	case UI_NETDIS_F://from showcard....
	{
	#ifdef REDIRECT//jjkyg/06/26
		extern void init_buffer(redirect_config* buf);
		if(redirect.bCorrect) init_buffer(&redirect);
	#endif//#ifdef REDIRECT

		if(g_bExistConntAnimImage)
		{
			g_bExistConntAnimImage = FALSE;
		}
		ui_waptimer_set(0,FALSE);
	#ifdef WAP_USE_DORM
		go_storeWml_normal(TRUE);
	#endif//#ifdef WAP_USE_DORM
		ui_dispmsgtimer_set(0);


		if(firstsite)//어떠한 Action이 있어야만 이리로 오므로 이렇게 해도 됨 
		{
			if((PrevKey!=HS_WAP_K)&&(PrevKey!=UI_NONE_F)&&(PrevKey!=HS_CLR_K))
			{
				MSG_ERROR("firstsite to FALSE",0,0,0);
				firstsite = FALSE;PrevKey = UI_NONE_F;
			}
		}
		report_event(WAP_DRAW_STOP_EVENT);
		newurl_input = FALSE;entertain = FALSE;push_acc = 0;
		toneback_on = TRUE;g_nAniBarCount = 0;
	#ifdef FEATURE_NETGAME
		ui.to_wap = FALSE;
		svNetGameRetURL[0]  = '\0' ;
		CLNTa_log(1,0,"ui.to_wap = FALSE");
	#endif //netgame	
			
#ifdef FOR_MS			
		if(wtls_enc_start)
		{
			wtls_enc_start = FALSE;	bStopKey = FALSE;bWtlsgoback = TRUE;				
			go_storeWml(TRUE);						
		}
#endif//#ifdef FOR_MS			
		new_state = HANDLE_KEY_S;
		ui.event = UI_NONE_F;
		break;
	}

	case UI_NETDATA_ARRIVED_F:
	{
		if(g_bExistConntAnimImage)
		{
			g_bExistConntAnimImage = FALSE;
		}
			//--------------------------
		bReceiveData = TRUE;//newcard가 불렸음을 의미...
		ui_dispmsgtimer_set(0);
		ui.event = UI_NONE_F;
		break;
	}//end of UI_NETDATA_ARRIVED_F

    case HS_CLR_K://[취소] 키
	case HS_PF2_K://right
	{
#ifdef WAP_USE_COMMAND
		if(g_bBlocked || g_bGettingContent || bStopKey) 
		{
			ui.event = UI_NONE_F;
			break;
		}

		receiveOK = FALSE;
		report_event(WAP_DRAW_STOP_EVENT);

		if(g_pCard != NULL)
		{
			wap_clk_key = TRUE;
			new_state = HANDLE_KEY_S;				
			report_event(WAP_STOP_EVENT);
		}
		else//이전 카드가 없는 경우 WAP을 종료한다
		{
			ProcessBye();
		}			
#endif//#ifdef WAP_USE_COMMAND						
		ui.event = UI_NONE_F;
		break;
	}

 /*============================================================================
		    WAP중 PUSH나 Email을 받았을 경우의 처리
  ============================================================================*/
	case UI_SMS_MSG_F:
	{
		if(bStopKey ||g_bBlocked ) 
		{
			ui.event = UI_NONE_F;
			break;
		}
		entertain = FALSE;newurl_input = FALSE;

		report_event(WAP_DRAW_STOP_EVENT);
		process_wap_push_mail();
		break;
	}
 /*============================================================================*/
#ifdef WAP_USE_DORM
		case UI_DORM_EXIT_F:
		{
			 CLNTa_log(1,0,"Receive UI_DORM_EXIT_REQ_F");
			 CLNTa_log(1,0,"Bye -- 5");
             SendExitDormReq();
             ui.event = UI_NONE_F;
			 break;
		}
#endif//#ifdef WAP_USE_DORM

		case HS_EAR_DOWN_K:
		case HS_END_K ://end
		{
			if(bStopKey)
			{
				ui.event = UI_NONE_F;
				break;
			}
			CLNTa_log(1,0,"Bye -- 6");
#ifdef WAP_USE_SKT_NEW
            if(ui.wap_release) 
            {
               CLNTa_log(1,0,"wap release clear 3");
               ClearRelVar();
            }
#endif//#ifdef WAP_USE_SKT_NEW
			ProcessBye();
			break;
		}//end of HS_END_K

		//jjkyg 09/21
		case UI_FADE_F:
		case UI_RELEASE_F ://end
		{
			bStopKey = FALSE;
			process_release();
			break;
		}//end of RELEASE_F
#endif //PORTING_B0_WAP
	}//end of switch
}

#ifdef WAP_USE_COMMAND
LOCAL void report_event(wap_name_type name)
{
	wap_cmd_type* wap_event_ptr;

	if(name == WAP_TEMINATE_EVENT)
    {
        CLNTa_log(1,0,"When end_k, clear ui.wap_release");
		SKY_SET_WAP_RELEASE(FALSE);        
    }
	
	if(name == WAP_DRAW_EVENT)
	{
		if(!bb_setclk)
		{
#ifdef PORTING_B0_WAP
			ui_wappic_set(130,TRUE);
#endif //PORTING_B0_WAP
			bb_setclk = TRUE;
			(void) rex_set_sigs( &wap_tcb, WAP_BAR_SIG );
		}					
	}
	else if(name == WAP_DRAW_STOP_EVENT)
	{
		if(bb_setclk)
		{
			bb_setclk = FALSE;process_draw_stop();
		}		
	}
	else
	{
		if(( wap_event_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
		{
			wap_event_ptr->hdr.task_ptr   = NULL;
			wap_event_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
		}
		else {
			ERR_FATAL( "report_event: Free queue is EMPTY!", 0, 0, 0 );
		}
		wap_event_ptr->hdr.cmd = name;
		send_cmd_other2wap(wap_event_ptr);
	}
}

void process_draw_bar(void)
{
#ifdef WAP_USE_SECURE
	//extern boolean dispWtls;
#endif//#ifdef WAP_USE_SECURE
	int MODE =0;
	
	if(start_disp)
	{
		MODE = WAP_ANI_TEXT_CONNECTING;//접속중
		WAP_DrawLoadingBar(MODE, WAP_ANI_BAR_FROM_LEFT);		
		return;
	}
	else if(entertain) MODE = WAP_ANI_TEXT_ADDBOOKMARK;
  #ifdef REDIRECT//jjkyg 06/26
	else if(redirect.bCorrect) MODE = WAP_ANI_TEXT_RESETUP;
  #endif//#ifdef REDIRECT
#ifdef WAP_USE_SECURE
	else if(Portdetect) MODE = WAP_ANI_TEXT_SECURITY;
#else
	else if(Wtlshello || wtls_enc_start) MODE = WAP_ANI_TEXT_SECURITY;
#endif//#ifdef WAP_USE_SECURE
	else MODE = WAP_ANI_TEXT_RECEIVING;	

	WAP_DrawLoadingBar(MODE, WAP_ANI_BAR_FROM_RIGHT);		

}

void process_draw_stop(void)
{
	g_nAniBarCount=0;g_bDrawBar=FALSE;
#ifdef PORTING_B0_WAP
	ui_wappic_set(0,FALSE);
#endif //PORTING_B0_WAP
}

void process_goback_event(void)
{
	retry =0;
	if(bWtlsgoback)
	{
		close_wtls_not_air();memset(SaveWtls,0,1000);
		Wtlshello = FALSE;wtls_enc_start = FALSE;
        bWtlsgoback =FALSE;
#ifdef WAP_USE_SECURE
		ClearWTLSdata();
#endif//#ifdef WAP_USE_SECURE
		bSetKey = FALSE;
	}	
}

void process_stop_event(void)
{
    receiveOK = FALSE;
	report_event(WAP_DRAW_STOP_EVENT);

    entertain = FALSE;newurl_input = FALSE;
	Process_For_Cancel_Loading();		
}

void process_terminate_event(void)
{
#ifdef WAP_USE_LEAK
   extern int isReStartComleted;
   extern BOOL bsetonlyone;
#endif //WAP_USE_LEAK
   //save_event = UI_NONE_F;
   wap_do_abnormal_exit();

#ifdef FEATURE_MUSIC_BELL
	MSG_HIGH("ProcessBye",0,0,0);
    musicbell_check();
#endif //#ifdef FEATURE_MUSIC_BELL

    entertain = FALSE;newurl_input = FALSE;
	go_storeWml(TRUE);receiveOK  = FALSE;

	if(!makecall)
	{
	#ifdef FEATURE_NETGAME
        /* rosa 01-03-01 --> @why, what */
        // 여긴 EQUS코드로 바꿔야 할 부분.
		//if(ui.to_netgame)
		//	wap_display_exit_message_for_netgame();
		//else
        /* <-- */
	#endif//netgmae
	}
#ifdef WAP_USE_LEAK
	if(isReStartComleted !=3)
	{
		CLNTa_log(1,0," Setting isReStartComleted = 0");
		isReStartComleted = 0;
	}
	else //isReStartComleted ==3
	{
		CLNTa_log(1,0,"ALready setting all variable");
		bsetonlyone = FALSE;
	}
#endif//#ifdef WAP_USE_LEAK

}
#endif//#ifdef WAP_USE_COMMAND

#ifdef PORTING_B0_WAP
LOCAL void browser_start(void)
#else
void browser_start(void)
#endif //PORTING_B0_WAP
{
	if (( wap_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
	{
		wap_ptr->hdr.task_ptr   = NULL;
		wap_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
	}
	else {
		ERR_FATAL( "browser_start: Free queue is EMPTY!", 0, 0, 0 );
	}
	wap_ptr->hdr.cmd = WAP_BROWSER_START;
	
	send_cmd_other2wap(wap_ptr);
	CLNTa_log(1,0,"Browser start");
}

BOOL bTerminatewap = FALSE;
#ifdef PORTING_B0_WAP
LOCAL void browser_terminate(void)
#else
void browser_terminate(void)
#endif //PORTING_B0_WAP
{
#ifdef WAP_USE_DORM
	wap_dorm_mode    = WAP_MODE_NOT_DORM;
	wap_flow_cont    = FALSE;
#endif//#ifdef WAP_USE_DORM

	if (( wap_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
	{
		wap_ptr->hdr.task_ptr   = NULL;
		wap_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
	}
	else {
		ERR_FATAL( "browser_terminate: Free queue is EMPTY!", 0, 0, 0 );
	}
	
#ifndef WAP_USE_INIT
	bTerminatewap = TRUE;
	wap_ptr->hdr.cmd = MEM_PREPARE_CACHE;	
	wap_ptr->prepare_cache.availablePersistentMemory = MAX_CACHE_SIZE;
#else
	wap_ptr->hdr.cmd = MMI_CLOSE_VIEW;	
	wap_ptr->close_view.viewId = 1;		
#endif//#ifndef WAP_USE_INIT
	send_cmd_other2wap(wap_ptr);
	CLNTa_log(1,0,"AUR TEMINATE");
}

#ifdef PORTING_B0_WAP
static void displayterminate(void)
{
	ui_waptimer_set(0,FALSE);
	dbi.in_use = FALSE;db_put( DB_IN_USE, &dbi );

	CLNTa_log(1,0,"DISPLAY TERMINaTE");
	ui.call = FALSE;ui.datacall = FALSE;
	uiscrn_ann( HS_AN_INUSE, 0 );
#ifdef FEATURE_MUSIC_BELL
	CLNTa_log(1, 0, "UI_MUSICBELL_DEF[%d]",ui_musicbell_default);
	if(ui_musicbell_default)
	{
		musicbell_nvwrite();
		ui_musicbell_default = FALSE;
	}
#endif

#ifdef WAP_USE_SKT_NEW  
    if(!ui.wap_release) 
#endif//#ifdef WAP_USE_SKT_NEW   
    {
      state = IRDA_CHECK_S;
//      ret_state = UI_POP_S;
    }
#ifdef WAP_USE_SKT_NEW
    else
    {
        //if(wap_rel_state == REL_NULL) 
		if(wap_rel_state == REL_NULL || wap_rel_state == REL_INPROG) 
        {
            CLNTa_log(1,0," Enable reorigination when relese 2G");
            wap_rel_state = REL_ENABLE_ORIG;
        }
    }
	save_release = 0;
#endif//WAP_USE_SKT_NEW
}
#endif //PORTING_B0_WAP

/************************************************************************************/
/*																					*/
/*							WAP Animation 관련										*/
/*																					*/
/************************************************************************************/
extern void WAP_DrawLoadingBarToBackBuff(int nAniTextMode, BOOL bAniBarDirection);
void WAP_DrawLoadingBar(int nAniTextMode, BOOL bAniBarDirection)
{
	//-------------------------------
	//WAP 초기접속 - Animation 중이면 Bar는 Animation Timer로 그린다
	if(g_bExistConntAnimImage) return;

	//display at LCD
	WAP_DrawLoadingBarToBackBuff(nAniTextMode, bAniBarDirection);
	WAP_Send_LCDBuffer_ToHS();
}

void WAP_Display_LoadingBar(int start_x, int end_x)
{
//    WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS, end_x, WAP_SOFTKEY_Y_POS, TRUE, RGB332( 32, 32,128));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 1, end_x, WAP_SOFTKEY_Y_POS + 1, TRUE, 10);//RGB332( 32, 32,128));//RGB332(  0,  0,255));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 2, end_x, WAP_SOFTKEY_Y_POS + 2, TRUE, 15);//RGB332(  0,  0,255));//RGB332( 32, 64,192));//RGB332(  0,  0,255));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 3, end_x, WAP_SOFTKEY_Y_POS + 3, TRUE, 27);//RGB332(  0,192,192));//RGB332(  0,128,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 4, end_x, WAP_SOFTKEY_Y_POS + 4, TRUE, 95);//RGB332(  0,224,192));//RGB332(  0,192,192));//RGB332( 32, 64,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 5, end_x, WAP_SOFTKEY_Y_POS + 5, TRUE, 159);//RGB332(128,224,192));//RGB332(  0,224,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 6, end_x, WAP_SOFTKEY_Y_POS + 6, TRUE, 223);//RGB332(128,224,192));//RGB332(  0,128,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 7, end_x, WAP_SOFTKEY_Y_POS + 7, TRUE, 223);//RGB332(128,224,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 8, end_x, WAP_SOFTKEY_Y_POS + 8, TRUE, 159);//RGB332(128,224,192));//RGB332(  0,192,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 9, end_x, WAP_SOFTKEY_Y_POS + 9, TRUE, 95);//RGB332(  0,224,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 10, end_x, WAP_SOFTKEY_Y_POS + 10, TRUE, 27);//RGB332(  0,192,192));//RGB332(  0,224,192));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 11, end_x, WAP_SOFTKEY_Y_POS + 11, TRUE, 15);//RGB332(  0,  0,255));
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 12, end_x, WAP_SOFTKEY_Y_POS + 12, TRUE, 10);//RGB332( 32, 32,128));//RGB332(128,224,192));
}

void WAP_DrawLoadingBarToBackBuff(int nAniTextMode, BOOL bAniBarDirection)
{
#ifdef PORTING_B0_WAP
	//yjahn96 00.12.28 -------------------
	extern byte* /*EQS_API*/ GetScreenBuffer(void);
	byte* LCDWordAniBuffer = (byte* )GetScreenBuffer();
#endif //PORTING_B0_WAP

	UINT8 nBarCount;

#if ! ((defined WAP_B1) || (defined WAP_B2)) 
	char temp[20];
#endif 
	
	//-------------------------------
	//skip drawing
	if(!g_bDrawBar) return;

	//-------------------------------
	//clear
#if !((defined WAP_B1) || (defined WAP_B2))  
	strcpy(temp, "                    ");
	WAP_Display_TextOut(0, WAP_SOFTKEY_Y_POS, temp,	WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT, 1, 0);
#else //WAP_B1
	//WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
//    WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//WAP_Display_Image_CML(WAP_OK_CML/*WAP_UPDOWN_CML*/, WAP_CML_OKEMPTY/*WAP_CML_UPDOWNEMPTY*/ ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
//    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OKEMPTY, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);
//    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CETEREMPTY , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
    //WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_RIGHTEMPTY ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Rectangle(0, WAP_SOFTKEY_Y_POS, 119, WAP_SOFTKEY_Y_POS+12, TRUE, WAP_BASIC_BACK_COLOR);

#endif 

	//-------------------------------
	// LCD update time 때문에 초기접속 그림을 보이지 못하는 경우가 발생하므로
	// 초기접속 그림은 [접속중]시 계속 update하도록 한다 (2000.8.21)
#ifdef PORTING_B0_WAP
//	if(nAniTextMode == WAP_ANI_TEXT_CONNECTING && ui.ani_option[ANI_TYPE_NTOP].Option == ANI_OPTION_NONE)
	{
		int y;
		const word WAP_Init_Image[2000] = {//3072//WAP_LCD_PIXELS
			#include	".\REX\MMI\WAP_Init.dhf"
		};

		for(y = WAP_HEAD_HEIGHT; y < 104/*80*/; y++)
//			memcpy( &LCDWordAniBuffer[y][0], (byte*)&WAP_Init_Image[y*(16-1)], (16-1)*2);
#if !((defined WAP_B1) || (defined WAP_B2))
			memcpy( LCDWordAniBuffer+ y*WAP_LCD_TOTAL_ADDRESS_X, (byte*)&WAP_Init_Image[(y-WAP_HEAD_HEIGHT)*(16-1)], (16-1)*2);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
			memcpy( LCDWordAniBuffer+ y*WAP_LCD_TOTAL_ADDRESS_X, (byte*)&WAP_Init_Image[(y-WAP_HEAD_HEIGHT)*(16-1)], 120);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
#endif //PORTING_B0_WAP

	//-------------------------------
	//text
	g_nAniBarCount = (g_nAniBarCount + 1) % 17;
	nBarCount = g_nAniBarCount % 17;

	if(nBarCount < 13)//normal
	{
		switch(nAniTextMode)
		{
#if !((defined WAP_B1) || (defined WAP_B2))
			case WAP_ANI_TEXT_CONNECTING://접속중{0x89ca, 0x89cb}//반전
				strcpy(temp, "\211\312\211\313"); break;
			case WAP_ANI_TEXT_SECURITY	://보안중{0x89cc, 0x89cd}
				strcpy(temp, "\211\314\211\315"); break;
			case WAP_ANI_TEXT_RECEIVING	://수신중{0x89ea, 0x89eb}
				strcpy(temp, "\211\352\211\353"); break;
			case WAP_ANI_TEXT_ADDBOOKMARK://추가중{0x89ec, 0x89ed}
				strcpy(temp, "\211\354\211\355"); break;
			case WAP_ANI_TEXT_RESETUP	://재설정{0x89ee, 0x89ef}
				strcpy(temp, "\211\356\211\357"); break;
#else   //#ifndef WAP_B1
			case WAP_ANI_TEXT_CONNECTING://접속중{0x88ce, 0x88cf}//기본
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_CONNECTING1, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_SECURITY	://보안중{0x88f5, 0x88f6}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_SECURITY1, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_RECEIVING	://수신중{0x84d4, 0x84d5}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_RECEIVING1, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_ADDBOOKMARK://추가중{0x89be, 0x89bf}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_ADDBOOKMARK1, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_RESETUP	://재설정{0x89ce, 0x89cf}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_RESETUP1, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
#endif  //#ifndef WAP_B1
		}
	}
	else//반전
	{
		switch(nAniTextMode)
		{
#if !((defined WAP_B1) || (defined WAP_B2))
			case WAP_ANI_TEXT_CONNECTING://접속중{0x88ce, 0x88cf}//기본
				strcpy(temp, "\210\316\210\317"); break;
			case WAP_ANI_TEXT_SECURITY	://보안중{0x88f5, 0x88f6}
				strcpy(temp, "\210\365\210\366"); break;
			case WAP_ANI_TEXT_RECEIVING	://수신중{0x84d4, 0x84d5}
				strcpy(temp, "\204\324\204\325"); break;
			case WAP_ANI_TEXT_ADDBOOKMARK://추가중{0x89be, 0x89bf}
				strcpy(temp, "\211\276\211\277"); break;
			case WAP_ANI_TEXT_RESETUP	://재설정{0x89ce, 0x89cf}
				strcpy(temp, "\211\316\211\317"); break;
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
			case WAP_ANI_TEXT_CONNECTING://접속중{0x89ca, 0x89cb}//반전
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_CONNECTING2, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_SECURITY	://보안중{0x89cc, 0x89cd}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_SECURITY2, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_RECEIVING	://수신중{0x89ea, 0x89eb}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_RECEIVING2, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_ADDBOOKMARK://추가중{0x89ec, 0x89ed}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_ADDBOOKMARK2, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
			case WAP_ANI_TEXT_RESETUP	://재설정{0x89ee, 0x89ef}
				WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_RESETUP2, WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
				break;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
		}
	}
#if !((defined WAP_B1) || (defined WAP_B2)) 
	WAP_Display_TextOut(0, WAP_SOFTKEY_Y_POS, temp,	WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT, 1, 0);
#endif 
	//-------------------------------
	//progress bar
	if(bAniBarDirection == WAP_ANI_BAR_FROM_RIGHT)
	{
		switch(nBarCount) //얇은것<-굵은것
		{
		case 16://반전
		case 15://반전
		case 14://반전
#if !((defined WAP_B1) || (defined WAP_B2))
		case 13: WAP_Display_Rectangle(24, WAP_SOFTKEY_Y_POS, 25+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 12: WAP_Display_Rectangle(27, WAP_SOFTKEY_Y_POS, 28+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 11: WAP_Display_Rectangle(30, WAP_SOFTKEY_Y_POS, 32+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 10: WAP_Display_Rectangle(34, WAP_SOFTKEY_Y_POS, 36+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 9 : WAP_Display_Rectangle(38, WAP_SOFTKEY_Y_POS, 40+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 8 : WAP_Display_Rectangle(42, WAP_SOFTKEY_Y_POS, 45+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 7 : WAP_Display_Rectangle(47, WAP_SOFTKEY_Y_POS, 50+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 6 : WAP_Display_Rectangle(52, WAP_SOFTKEY_Y_POS, 55+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 5 : WAP_Display_Rectangle(57, WAP_SOFTKEY_Y_POS, 60+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 4 : WAP_Display_Rectangle(62, WAP_SOFTKEY_Y_POS, 67+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 3 : WAP_Display_Rectangle(69, WAP_SOFTKEY_Y_POS, 74+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 2 : WAP_Display_Rectangle(76, WAP_SOFTKEY_Y_POS, 81+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 1 : WAP_Display_Rectangle(83, WAP_SOFTKEY_Y_POS, 88+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);

#else   //#if !((defined WAP_B1) || (defined WAP_B2)) 
/*		case 13: WAP_Display_Rectangle(36, WAP_SOFTKEY_Y_POS, 38-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 32,128));
		case 12: WAP_Display_Rectangle(38, WAP_SOFTKEY_Y_POS, 41-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 32,128));
		case 11: WAP_Display_Rectangle(40, WAP_SOFTKEY_Y_POS, 44-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 32,128));//RGB332(  0,  0,255));
		case 10: WAP_Display_Rectangle(44, WAP_SOFTKEY_Y_POS, 48-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,  0,255));
		case 9 : WAP_Display_Rectangle(48, WAP_SOFTKEY_Y_POS, 52-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,  0,255));//RGB332( 32, 64,192));
		case 8 : WAP_Display_Rectangle(52, WAP_SOFTKEY_Y_POS, 56-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 64,192));
		case 7 : WAP_Display_Rectangle(56, WAP_SOFTKEY_Y_POS, 60-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 64,192));//RGB332(  0,128,192));
		case 6 : WAP_Display_Rectangle(60, WAP_SOFTKEY_Y_POS, 64-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,128,192));
		case 5 : WAP_Display_Rectangle(64, WAP_SOFTKEY_Y_POS, 68-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,128,192));//RGB332(  0,192,192));
		case 4 : WAP_Display_Rectangle(68, WAP_SOFTKEY_Y_POS, 72-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,192,192));
		case 3 : WAP_Display_Rectangle(72, WAP_SOFTKEY_Y_POS, 76-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,192,192));//RGB332(  0,224,192));
		case 2 : WAP_Display_Rectangle(76, WAP_SOFTKEY_Y_POS, 80-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,224,192));
		case 1 : WAP_Display_Rectangle(80, WAP_SOFTKEY_Y_POS, 84-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,224,192));//RGB332(128,224,192));
*/
		case 13: WAP_Display_LoadingBar(40, 44-1);
		case 12: WAP_Display_LoadingBar(44, 47-1);
		case 11: WAP_Display_LoadingBar(47, 50-1);
		case 10: WAP_Display_LoadingBar(50, 53-1);
		case 9 : WAP_Display_LoadingBar(53, 56-1);
		case 8 : WAP_Display_LoadingBar(56, 59-1);
		case 7 : WAP_Display_LoadingBar(59, 62-1);
		case 6 : WAP_Display_LoadingBar(62, 65-1);
		case 5 : WAP_Display_LoadingBar(65, 68-1);
		case 4 : WAP_Display_LoadingBar(68, 71-1);
		case 3 : WAP_Display_LoadingBar(71, 74-1);
		case 2 : WAP_Display_LoadingBar(74, 77-1);
		case 1 : WAP_Display_LoadingBar(77, 80-1);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
		}
	}
	else
	{
		switch(nBarCount) //굵은것->얇은것
		{
		case 16://반전
		case 15://반전
		case 14://반전
#if !((defined WAP_B1) || (defined WAP_B2))
		case 13: WAP_Display_Rectangle(89, WAP_SOFTKEY_Y_POS, 89+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 12: WAP_Display_Rectangle(85, WAP_SOFTKEY_Y_POS, 87+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 11: WAP_Display_Rectangle(81, WAP_SOFTKEY_Y_POS, 83+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 10: WAP_Display_Rectangle(77, WAP_SOFTKEY_Y_POS, 79+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 9 : WAP_Display_Rectangle(73, WAP_SOFTKEY_Y_POS, 75+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 8 : WAP_Display_Rectangle(68, WAP_SOFTKEY_Y_POS, 71+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 7 : WAP_Display_Rectangle(63, WAP_SOFTKEY_Y_POS, 66+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACKGRAY);
		case 6 : WAP_Display_Rectangle(58, WAP_SOFTKEY_Y_POS, 61+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 5 : WAP_Display_Rectangle(53, WAP_SOFTKEY_Y_POS, 56+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 4 : WAP_Display_Rectangle(46, WAP_SOFTKEY_Y_POS, 51+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 3 : WAP_Display_Rectangle(39, WAP_SOFTKEY_Y_POS, 44+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 2 : WAP_Display_Rectangle(32, WAP_SOFTKEY_Y_POS, 37+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
		case 1 : WAP_Display_Rectangle(25, WAP_SOFTKEY_Y_POS, 30+1, WAP_SOFTKEY_Y_POS+WAP_HAN_FONT_HEIGHT, TRUE, WAP_BLACK);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
/*		case 13: WAP_Display_Rectangle(81, WAP_SOFTKEY_Y_POS, 84-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 32,128));
		case 12: WAP_Display_Rectangle(79, WAP_SOFTKEY_Y_POS, 81-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 32,128));
		case 11: WAP_Display_Rectangle(76, WAP_SOFTKEY_Y_POS, 79-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 32,128));//RGB332(  0,  0,255));
		case 10: WAP_Display_Rectangle(72, WAP_SOFTKEY_Y_POS, 76-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,  0,255));
		case 9 : WAP_Display_Rectangle(68, WAP_SOFTKEY_Y_POS, 72-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,  0,255));//RGB332( 32, 64,192));
		case 8 : WAP_Display_Rectangle(64, WAP_SOFTKEY_Y_POS, 68-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 64,192));
		case 7 : WAP_Display_Rectangle(60, WAP_SOFTKEY_Y_POS, 64-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332( 32, 64,192));//RGB332(  0,128,192));
		case 6 : WAP_Display_Rectangle(56, WAP_SOFTKEY_Y_POS, 60-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,128,192));
		case 5 : WAP_Display_Rectangle(52, WAP_SOFTKEY_Y_POS, 56-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,128,192));//RGB332(  0,192,192));
		case 4 : WAP_Display_Rectangle(48, WAP_SOFTKEY_Y_POS, 52-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,192,192));
		case 3 : WAP_Display_Rectangle(44, WAP_SOFTKEY_Y_POS, 48-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,192,192));//RGB332(  0,224,192));
		case 2 : WAP_Display_Rectangle(40, WAP_SOFTKEY_Y_POS, 44-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,224,192));
		case 1 : WAP_Display_Rectangle(36, WAP_SOFTKEY_Y_POS, 40-1, WAP_SOFTKEY_Y_POS+WAP_SOFTKEY_HEIGHT-1, TRUE, RGB332(  0,224,192));//RGB332(128,224,192));
*/
		case 13: WAP_Display_LoadingBar(76, 80-1);
		case 12: WAP_Display_LoadingBar(73, 76-1);
		case 11: WAP_Display_LoadingBar(70, 73-1);
		case 10: WAP_Display_LoadingBar(67, 70-1);
		case 9 : WAP_Display_LoadingBar(64, 67-1);
		case 8 : WAP_Display_LoadingBar(61, 64-1);
		case 7 : WAP_Display_LoadingBar(58, 61-1);
		case 6 : WAP_Display_LoadingBar(55, 58-1);
		case 5 : WAP_Display_LoadingBar(52, 55-1);
		case 4 : WAP_Display_LoadingBar(49, 52-1);
		case 3 : WAP_Display_LoadingBar(46, 49-1);
		case 2 : WAP_Display_LoadingBar(43, 46-1);
		case 1 : WAP_Display_LoadingBar(40, 43-1);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
		}
	}

	//---------------------------------------------------
	// Loading 을 종료하기 위한 취소 키를 오른쪽에 넣어준다
#if !((defined WAP_B1) || (defined WAP_B2)) 
	strcpy(temp, "\206\266\206\267\006");//"취소>"
	WAP_Display_TextOut(90*2, WAP_SOFTKEY_Y_POS, temp, WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT, 1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_CANCEL, WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

/************************************************************************************/
/*																					*/
/*						 Key 처리 결과												*/
/*																					*/
/************************************************************************************/


LOCAL void Process_After_HandleKeys(int nResult)
{
#ifdef PORTING_B0_WAP
	extern storeWML normal_use;

	if(nResult == WAP_HANDLE_KEY_RESULT_EXIT)
	{
		CLNTa_log(1,0,"Bye -- 7");
#ifdef WAP_USE_SKT_NEW
		if(ui.wap_release) 
		{
			CLNTa_log(1,0," WAP_RElEASE CLEAR");
			ClearRelVar();
		}
#endif//#ifdef WAP_USE_SKT_NEW
		ProcessBye();
	}//end of WAP_HANDLE_KEY_RESULT_EXIT
	else if(nResult == WAP_HANDLE_RECEIVE_RELEASE)
	{
        process_release();
	}
	else if(nResult == WAP_HANDLE_KEY_RESULT_PHONE)
	{
#ifdef FEATURE_MUSIC_BELL
		MSG_HIGH("Process_After_HandleKeys",0,0,0);
        musicbell_check();
#endif// MUSICBELL
		isFromWap = TRUE;
		ui_get_keys( FALSE );
		ui.event= UI_NONE_F;
		ui.txdtmf = FALSE;
		ui.n.act = FALSE;
		ui.a.act = FALSE;
		ui.beep_updown = TRUE;

#ifdef GIZMO_A7M
		if(ui.earpieceup)
		{
			//전화부의 Root로 들어간다
			ui_activate_hotbook_menu();
			ret_state = UI_MENU_S;
		}
		else
		{
			//전화부 중 Speed찾기로 들어간다
			ui.n.act = FALSE;
			ui.a.act = FALSE;
			ret_state = UI_RCL_S;
			ui.book_menu = 1;
		}
#else//GIZMO_A7M
		ui_activate_hotbook_menu();
//		ret_state = UI_MENU_S;
#endif//GIZMO_A7M

		state = AFTER_PHONEBOOK_S;

	}//end of WAP_HANDLE_KEY_RESULT_PHONE
	else if(nResult == WAP_HANDLE_KEY_RESULT_INPUT)
	{
		MSG_MED("WAP_HANDLE_KEY_RESULT_INPUT",0,0,0);

		isFromWap = TRUE;

		Process_Input_Element();

	}//end of WAP_HANDLE_KEY_RESULT_INPUT
	else if(nResult == WAP_HANDLE_KEY_RESULT_NEWURL)
	{
		MSG_MED("WAP_HANDLE_KEY_RESULT_NEWURL",0,0,0);

		isFromWap = TRUE;

		//--------------------------------------------------
		//현재 URL을 기본값으로 ui.ed.buf 에 넣어준다
		memset(ui.ed.buf, '\0', sizeof(ui.ed.buf));
		memcpy(ui.ed.buf, "www.", 4);
		//--------------------------------------------------
		ui.ed.act = TRUE;
		ui.ed.mode = 1;//영문 소문자
		ui.ed.pos = 4;//get_length(ui.ed.buf, WAP_MAX_INPUT);
		ui.clr_exit = TRUE;//취소키로 빠져나옴
		ui.email_edit = TRUE;//기본값도 지움
		ui.smsalpha = TRUE;//기호 건너뛰기
		no_alpha_lines = 8;//Alpha editor의 입력 line 결정 : 8줄 (128자)
				
		uiscrn_bind_field(UI_ATITLE_F, "  새 URL 입력   0");
		uiscrn_act(UI_ATITLE_F);
		state = AFTER_ALPHAEDIT_FOR_NEWURL_S;
//		ret_state = UI_ALPHAEDIT_S;
		from_wap = TRUE;          // raptor: alpha for wap
	
	}//end of WAP_HANDLE_KEY_RESULT_NEWURL
	else if((nResult == WAP_HANDLE_KEY_RESULT_MAYBE_LOADING)
		     ||(nResult== WAP_HANDLE_KEY_RESULT_MAYBE_ENTERTAIN))
	{
#ifdef WAP_USE_DORM
        wap_clk_key = FALSE;
#endif//#ifdef WAP_USE_DORM
		CLNTa_log(1,0,"MAYBE loading");

#ifdef FEATURE_MUSIC_BELL
		MSG_HIGH("Process_After_HandleKeys",0,0,0);
		musicbell_check();
#endif
		retry =0;new_state =  LOADING_S;receiveOK = TRUE;

		if(bWtlsgoback)
		{
			close_wtls_not_air();
			memset(SaveWtls,0,1000);
			Wtlshello = FALSE;wtls_enc_start = FALSE;bWtlsgoback = FALSE;
		}

		if(nResult==WAP_HANDLE_KEY_RESULT_MAYBE_ENTERTAIN)
		{
			entertain = TRUE;
			displayloading(11,TRUE);
		}
		else
		{
			displayloading(1,TRUE);
		}
	}//end of WAP_HANDLE_KEY_RESULT_MAYBE_LOADING
	else if(nResult == WAP_HANDLE_KEY_RESULT_GOBACK)
	{
		
		CLNTa_log(1,0,"--------[WAP_HANDLE_KEY_RES1ULT_GOBACK]--------");
#ifdef WAP_USE_DORM
        wap_clk_key = FALSE;
#endif//#ifdef WAP_USE_DORM

#ifdef FEATURE_MUSIC_BELL
		musicbell_check();
#endif
		normal_use.viewid  = 1;normal_use.id  = 0;normal_use.wmlkind = 7;

        receiveOK = TRUE;new_state=LOADING_S;
		displayloading(1,TRUE);    
		report_event(WAP_GOBACK_EVENT);

	}//end of WAP_HANDLE_KEY_RESULT_GOBACK
	else if(nResult == WAP_HANDLE_MAKECALL)
	{
		MSG_MED("MAKE call TRUE",0,0,0);
#ifdef FEATURE_MUSIC_BELL
		musicbell_check();
#endif
		do_makecall();
	}
#ifndef WAP_USE_SECURE
	else if(nResult == WAP_WTLS_SESSION_START)
	{
		ui_get_keys( FALSE );
#ifdef WAP_USE_DORM
        wap_clk_key = FALSE;
#endif//#ifdef WAP_USE_DORM
		processWtlsResult();
        receiveOK = TRUE;bWtlsgoback =FALSE;displayloading(6,TRUE);
		ui.event =UI_NONE_F;
	}
#endif//#ifndef WAP_USE_SECURE
	else if(nResult == WAP_PUSH_IN_WAP)
	{
		ui_get_keys( FALSE );
		retry =0;
#ifdef WAP_USE_DORM
        wap_clk_key = FALSE;
#endif//#ifdef WAP_USE_DORM

		if(InWapMailorPush == 2)//전card가 있음 
		{
			WAP_Repaint();
		}
		else//초기 접속 그림
		{
			PlayNTopConntImage();
		}
	
		//if(push_acc == 1)  SKY_loadURL(1, PushUrl, TRUE);		   
		if(push_acc == 1)  SKY_loadURL(1, g_pszPushUrl, TRUE);		   
		else if(push_acc == 2)  SKY_loadURL(1, EmailUrl, TRUE);
		else//push_acc = other value
		{
			if(InWapMailorPush == 1)
            {
				SKY_loadURL(1,temp_url,FALSE);
                normal_use.viewid  = 1;
    			normal_use.id      = 0;
				normal_use.wmlkind = 6;
            }
			else//
				return;
		}
		new_state =  LOADING_S;
        receiveOK = TRUE;
		bWtlsgoback = FALSE;
		displayloading(1,TRUE);
	}
#ifdef WAP_USE_SKT_NEW
    if(ui.wap_release) save_min_state = new_state;
#endif//#ifdef WAP_USE_SKT_NEW
#endif //PORTING_B0_WAP
}		

LOCAL void Process_Input_Element(void)
{
#ifdef PORTING_B0_WAP
	if(g_pCurInput == NULL)//Error
	{
		WAP_Repaint();
		new_state = HANDLE_KEY_S;
	}
	else
	{
		int length;
		memset(ui.ed.buf, 0x00, WAP_MAX_INPUT);
		//--------------------------------------------------
		//현재 값을 기본값으로 ui.ed.buf 에 넣어준다
		length = strlen(g_pCurInput->text);

		if(length > 0)
		{
			ui.ed.act = TRUE;
			memcpy(ui.ed.buf, g_pCurInput->text, length+1);
#ifdef DEBUG_MMI
			CLNTa_log(1, 0, ">> g_pCurInput->text :%s(%d)", g_pCurInput->text, length);
			CLNTa_log(1, 0, ">> ui.ed.buf :%s(%d)", ui.ed.buf, length);
#endif
		}
		else
		{
			ui.ed.act = FALSE;
			ui.ed.buf[0] = '\0';
#ifdef DEBUG_MMI
			CLNTa_log(1, 0, ">> ui.ed.buf == 널");
#endif
		}

		//--------------------------------------------------
		ui.smsalpha = TRUE;//기호 건너뛰기
		ui.clr_exit = TRUE;//취소키로 빠져나옴
		ui.ed.pos = get_length(ui.ed.buf, WAP_MAX_INPUT);
		no_alpha_lines = 8;//Alpha editor의 입력 line 결정 : 8줄 (128자)

		if(g_pCurInput->emptyOk)
		{
			allow_empty = 1;//확인키로 빠져나옴
		}
		else
		{
			allow_empty = 0;
		}
		if(g_pCurInput->isPassword)
		{
			ui.ed.mode = 1;//영문 소문자
			ui.ed.secret = TRUE;
			ui.email_edit = TRUE;
		}

		//--------------------------------------------------
		//input의 format 처리
		memset(format_string, '\0', 200);//init
		if(strncmp(g_pCurInput->format, "*M", 2) == 0)
		{
			strcpy(g_pCurInput->format, "*Z");
		}
		
		if(strncmp(g_pCurInput->format, "*", strlen("*") ) == 0
		&& strlen(g_pCurInput->format) == 2
		&& g_pCurInput->nChars > 0)
		{
			//*->nChar
			char newFormat[20];
			memset(newFormat, '\0', 20);//init
			sprintf(newFormat, "%d%c", g_pCurInput->nChars, g_pCurInput->format[1]);
			memcpy(format_string, newFormat, strlen(newFormat));

			if(g_pCurInput->isPassword) {
				if(g_pCurInput->nChars > 0) {
					ui.edit_leng = g_pCurInput->nChars;//maxLength
				}
			}
		}
		else
		{
			if(g_pCurInput->nChars > 0)
				ui.edit_leng = g_pCurInput->nChars;//maxLength
			memcpy(format_string, g_pCurInput->format, strlen(g_pCurInput->format));
		}
#ifdef DEBUG_MMI		

		if(g_pCurInput->isPassword)
			CLNTa_log(1, 0, "pass)%s(max:%d)->%s", g_pCurInput->format, g_pCurInput->nChars, format_string);
		else
			CLNTa_log(1, 0, "no pass)%s(max:%d)->%s", g_pCurInput->format, g_pCurInput->nChars, format_string);
#endif
		//--------------------------------------------------
		//알파에디터의 Title 결정
		{
			char temp[17];
			memset(temp, ' ', 17);

			length = strlen(g_pCurInput->title);

			if(g_pCurInput->title == NULL)
			{
				memcpy(temp, "   입력하세요   0", 17);
			}
			else if(length > 17)//17자이상
			{
				char temp2[255];
				memcpy(temp2, g_pCurInput->title, length);

//    			TruncateString(temp2, UI_WIDE);
//				memcpy(temp, temp2, UI_WIDE);
			}
			else
			{
				extern void MakeCenterAligned(byte *center_str, byte buff_length);

				memcpy(temp, g_pCurInput->title, length);
				MakeCenterAligned((byte *)temp, UI_WIDE);
			}
			uiscrn_bind_field(UI_ATITLE_F, temp);
		}
		uiscrn_act(UI_ATITLE_F);
	
		state = AFTER_ALPHAEDIT_S;
		ret_state = UI_ALPHAEDIT_S;
		from_wap = TRUE;          // raptor: alpha for wap
	}
#endif //PORTING_B0_WAP
}

LOCAL void Process_PasswordDialog_For_Input(BOOL bRequestId)
{
#ifdef PORTING_B0_WAP
	if(g_nDlgMode == WAP_DLG_MODE_PASSWORD)
	{
		memset(ui.ed.buf, '\0', sizeof(ui.ed.buf));//init

		ui.ed.act = FALSE;
		ui.ed.buf[0] = '\0';

		//--------------------------------------------------
		ui.ed.mode = 1;//영문 소문자
		ui.smsalpha = TRUE;//기호 건너뛰기
		//ui.edit_pos = TRUE;//취소키로 빠져나옴
		ui.ed.pos = get_length(ui.ed.buf, WAP_MAX_INPUT); 
		no_alpha_lines = 8;//Alpha editor의 입력 line 결정 : 8줄 (128자)
		if(bRequestId)
		{
			uiscrn_bind_field(UI_ATITLE_F, "    ID 입력     0");
			state = AFTER_ALPHAEDIT_FOR_DIALOG_ID_S;
		}
		else
		{
			ui.ed.secret = TRUE;
			uiscrn_bind_field(UI_ATITLE_F, "  패스워드 입력 0");
			state = AFTER_ALPHAEDIT_FOR_DIALOG_PASSWORD_S;
		}
		uiscrn_act(UI_ATITLE_F);
		ret_state = UI_ALPHAEDIT_S;
		from_wap = TRUE;          // raptor: alpha for wap
	}
#endif //PORTING_B0_WAP
}

LOCAL void Process_PromptDialog_For_Input(void)
{
#ifdef PORTING_B0_WAP
	if(g_nDlgMode == WAP_DLG_MODE_PROMPT)
	{
		int length;
		memset(ui.ed.buf, '\0', sizeof(ui.ed.buf));//init
		//--------------------------------------------------
		//현재 값을 기본값으로 ui.ed.buf 에 넣어준다
		length = strlen(g_pDlgMessage2);

		if(length > 0)
		{
			ui.ed.act = TRUE;
			memcpy(ui.ed.buf, g_pDlgMessage2, length+1);
		}
		else
		{
			ui.ed.act = FALSE;
			ui.ed.buf[0] = '\0';
		}

		//--------------------------------------------------
		ui.smsalpha = TRUE;//기호 건너뛰기
		//ui.edit_pos = TRUE;//취소키로 빠져나옴
		ui.ed.pos = get_length(ui.ed.buf, WAP_MAX_INPUT);
		no_alpha_lines = 8;//Alpha editor의 입력 line 결정 : 8줄 (128자)
		//uiscrn_bind_field(UI_ATITLE_F, "** 입력하세요 **0");
		{
			char temp[17];
			memset(temp, ' ', 17);

			length = strlen(g_pDlgMessage);

			if(g_pDlgMessage == NULL)
			{
				memcpy(temp, "   입력하세요   0", 17);
			}
			else if(length > 17)//17자이상
			{
				char temp2[255];
				memcpy(temp2, g_pDlgMessage, length);

    			TruncateString(temp2, UI_WIDE);
				memcpy(temp, temp2, UI_WIDE);
			}
			else
			{
				extern void MakeCenterAligned(byte *center_str, byte buff_length);
				memcpy(temp, g_pDlgMessage, length);
				MakeCenterAligned((byte *)temp, UI_WIDE);
			}
			uiscrn_bind_field(UI_ATITLE_F, temp);
		}
		uiscrn_act(UI_ATITLE_F);
		state = AFTER_ALPHAEDIT_FOR_DIALOG_PROMPT_S;
		ret_state = UI_ALPHAEDIT_S;
		from_wap = TRUE;          // raptor: alpha for wap
	}
#endif //PORTING_B0_WAP
}

#ifdef PORTING_B0_WAP
LOCAL void Process_For_Cancel_Loading()
#else
void Process_For_Cancel_Loading()
#endif
{

#ifdef FEATURE_MUSIC_BELL
	 musicbell_check();
#endif //#ifdef FEATURE_MUSIC_BELL

#ifndef WAP_USE_SECURE
	if(Wtlshello)//wtlshello일 경우에만, close session을 시도...
	{
		if(wtls_match ==1)	close_wtls_not_air();//key가없음 		
	}
#else
	if(Wtlshello)//wtlshello일 경우에만, close session을 시도...
	{
		if(!bSetKey) close_wtls_not_air();//key가없음 		
	}
#endif//#ifndef WAP_USE_SECURE
	if(load_data)
	{
		if(!Wtlshello)
		{
			SKY_stop(g_pCard->viewId);

			#ifdef BROWSER_UPDATE
			   close_udp_port();
			#endif//#ifdef BROWSER_UPDATE						
			CLNTa_log(1,0,"bbbb");
		}
	}

	wtls_enc_start = FALSE;	Wtlshello = FALSE;
	resetalldata();

	if(g_pCard != NULL)	WAP_Repaint();
}

/************************************************************************************/
/*																					*/
/*							Error 관련												*/
/*																					*/
/************************************************************************************/

void wap_check_error(boolean isMajorState)
{
#ifdef PORTING_B0_WAP
	if(ui.event == UI_WAP_HANDLE_ERROR_F) 
	{
		wap_manage_error(isMajorState);
	}
	if((ui.event == HS_PWR_OFF_K) && (isFirstReceived))
	{
		CLNTa_log(1,0,"Bye -- 9");
		
		ProcessBye();
		isFirstReceived = FALSE;
		isIssuePowerOffEvent = TRUE;
	}
#endif //PORTING_B0_WAP
}


void wap_manage_error(boolean isMajorState)
{
//PORTING_B0_WAP --> sky_hcmd에서 처리
//	errorNumber = ui.wap_error.number;
//	errorType   = ui.wap_error.type;
//PORTING_B0_WAP

	if ((errorNumber == ERR_WAE_OUT_OF_MEMORY) || (errorNumber == ERR_OUT_OF_MEMORY))
	{
		isOutOfMemory = TRUE;
	}
	else if (errorNumber == ERR_WTP_INVALIDTID)
	{
#ifdef FOR_MS		
		isFailureWTLSB = TRUE; 
#endif//#ifdef FOR_MS		
		isUDPErrorInd = TRUE;
	}
#ifdef PORTING_B0_WAP
	ui.event = UI_NONE_F;ui.wap_error.number = 0;ui.wap_error.type   = 0;
#endif //PORTING_B0_WAP
	isFirstDisplay = TRUE;
		 
	if (isMajorState) state = ERROR_S;
	else new_state = ERROR_S;
}

void wap_display_error(void)
{	
	/* Initialize bar */
#ifdef PORTING_B0_WAP
	report_event(WAP_DRAW_STOP_EVENT);ui_dispmsgtimer_set(0);

#ifdef WAP_USE_DORM
	if(!ui.wap_dorm_mode)
#endif//#ifdef WAP_USE_DORM
	{
		ui_dispmsgtimer_set(1500);
		CLNTa_log(1,0,"이리로 오면 않됨-->error");
	}

	if (isOutOfMemory)
	{
		ui_dispmsgtimer_set(0);wap_handle_memory_shortage();
		isOutOfMemory = FALSE;
	}
#ifdef WAP_USE_DORM
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
	else if(SKY_IS_WAP_DORM())
#else
    else if(ui.wap_dorm_mode)
#endif //#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
	{
		ui_dispmsgtimer_set(0); 
		if(dorm_dispkind == 1 || dorm_dispkind == 3)
		{
			WAP_ClearScreen();
			WAP_SetTextAtLine(2, "  연결실패  ",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(3, "다시연결",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(4, "하겠습니까?",WAP_TEXT_ALIGN_CENTER);
			//WAP_SetTextAtLine(4, "     \210\277\205\276\205\277\205\274\205\275\6 ",WAP_TEXT_ALIGN_LEFT);		
			WAP_DisplayMessage_Error();	

			//command line
			WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
			WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//WAP_Display_Image_CML(WAP_OK_CML/*WAP_UPDOWN_CML*/, WAP_CML_OKEMPTY/*WAP_CML_UPDOWNEMPTY*/ ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
			WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS); //ok
			WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_YES , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//예
			WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_NO ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);

			//------------------------------------------------------------------
			//handset task에 display를 요청한다
			WAP_Send_LCDBuffer_ToHS();
		}
		else
		{
			dorm_dispkind = 0;
			ui_dispmsgtimer_set(1500);nosvc_indicator();
			//WAP_ClearScreen();WAP_SetTextAtLine(0, "서비스가지원",WAP_TEXT_ALIGN_CENTER);
			//WAP_SetTextAtLine(1, "되지않습니다",WAP_TEXT_ALIGN_CENTER);WAP_SetTextAtLine(4, "\206\276\206\277",WAP_TEXT_ALIGN_CENTER);								 
		}		
	}
#endif//#ifdef WAP_USE_DORM
#ifdef WAP_USE_SKT_NEW
	else if(ui.wap_release && (ReleaseErrorKind == 1))
	{
		nosvc_indicator();
	}
#endif//#ifdef WAP_USE_SKT_NEW
	else
	{
		wap_display_error_message(errorNumber, errorType);	
	}
#endif //PORTING_B0_WAP
}

#define UI_WIDE 20//10
void WAP_DisplayMessage_Error(void)
{
	extern int g_nAlign[8];

	extern char g_strMessage0[UI_WIDE];
	extern char g_strMessage1[UI_WIDE];
	extern char g_strMessage2[UI_WIDE];
	extern char g_strMessage3[UI_WIDE];
	extern char g_strMessage4[UI_WIDE];
	extern char g_strMessage5[UI_WIDE];
	extern char g_strMessage6[UI_WIDE];
	extern char g_strMessage7[UI_WIDE];

	int nLine;
	char temp[UI_WIDE];

	for(nLine=0; nLine<8; nLine++)
	{
		memset(temp, '\0', UI_WIDE);
		switch(nLine)
		{
			case 0: strcpy(temp, g_strMessage0); break;
			case 1: strcpy(temp, g_strMessage1); break;
			case 2: strcpy(temp, g_strMessage2); break;
			case 3: strcpy(temp, g_strMessage3); break;
			case 4: strcpy(temp, g_strMessage4); break;
			case 5: strcpy(temp, g_strMessage5); break;
			case 6: strcpy(temp, g_strMessage6); break;
			case 7: strcpy(temp, g_strMessage7); break;
		}
#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(g_nAlign[nLine]*2,//for double buffering
							nLine * WAP_ASCII_FONT_HEIGHT,
							temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(g_nAlign[nLine],
							nLine * WAP_ASCII_FONT_HEIGHT,
							temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
}


#ifdef ABORT_IN_CONTENT
void wap_display_content_error(void)
{
	WAP_Node *pCurNode = g_pCard->pHeadContent;
	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_TEXT)
		{
			WAP_Content_Text* pContent = (WAP_Content_Text *) pCurNode->pContent;

			char temp[50];
			int length;
			
			strcpy(temp, "저장을 실패하였습니다.");
			length = strlen(temp);

			//이전 데이타를 지운다
			if(pContent->text != NULL) wip_free((void *)pContent->text);

			//새로운 데이타를 만든다
			pContent->text = (char *) wip_malloc(sizeof(char)*(length+1));
			memset(pContent->text, 0x00, length+1);
			strcpy(pContent->text, temp);

			break;
		}
		pCurNode = pCurNode->pNextNode;
	}
	//Formatting
	WAP_Formatting();
}
#endif//#ifdef ABORT_IN_CONTENT

void wap_confirm_error(void)
{
	CLNTa_log(1,0,"--------[wap_confirm_error]--------");
#ifdef PORTING_B0_WAP
	switch(ui.event)
	{
#ifdef WAP_USE_DORM
		case HS_PF2_K:
			 //if(ui.wap_dorm_mode && dorm_dispkind == 1) 
			 if(
				 (ui.wap_dorm_mode && dorm_dispkind == 1) || (ui.wap_pzid && dorm_dispkind == 3)
			   )
			 {
		  	    //extern void dsi_end_protocol_operation(void);
			
				dorm_dispkind = 0;ui.wap_dorm_mode = FALSE;
		
				if(entertain) entertain = FALSE;

				wap_dorm_mode = WAP_MODE_NOT_DORM;
				wap_clk_key   = FALSE;wap_flow_cont = FALSE;				
				isFailOrig	  = FALSE;new_state     = EXIT_S;
				go_storeWml_normal(TRUE);

			#ifdef FOR_MS
				if(Wtlshello) 
				{
					Wtlshello = FALSE;go_storeWml(TRUE);wtls_match = 0;
					memset(SaveWtls,0x00,1000);
				}
			#endif//#ifdef FOR_MS

				wap_display_exit_message();

			#ifdef WAP_USE_PZID2
				if(ui.wap_pzid)
				{
					extern ds_wap_pzid_enum_type ds_wap_pzid_state ;
					ui.wap_pzid = FALSE;
					ds_wap_pzid_state = DS_PZID_NULL;
				}
				//else
			#endif//#ifdef WAP_USE_PZID2
				//	dsi_end_protocol_operation();				

				ProcessBye();
			 }
			 ui.event = UI_NONE_F;
			 break;

//================================================================================================
		case HS_PF1_K:
#ifdef WAP_USE_PZID2
			if(ui.wap_dorm_mode || ui.wap_pzid )
#else
			if(ui.wap_dorm_mode)
#endif//#ifdef WAP_USE_PZID
			{
				extern void dsi_set_callstate(dsi_callstate_enum_type new_callstate);
				extern void	wap_set_error(unsigned char isError);

				CLNTa_log(1,0,"다시 origination을 시작하자");
				ui_dispmsgtimer_set(0);
				//enable reorigination====================================
				dsmgr_cancel_timer(DS_STATE_TIMER_SIG);//dummy사실 필요없음

				wap_flow_cont = FALSE;wap_clk_key   = FALSE;
				wap_set_dorm_mode(WAP_MODE_REORIG);								
				dsi_set_callstate(DS_IDLE_CSTATE);		
				//enable reorigination=====================================

				wap_set_error(FALSE);isFailOrig = FALSE;
				WAP_Repaint();

		#ifdef FOR_MS
				//일단 보안일시는 이렇게 하면 안될것으로 봄
				if(wtls_enc_start) wtls_enc_start = FALSE;
		#endif//#ifdef FOR_MS	
	
				CLNTa_log(1,0,"Dorm mode %d",dorm_dispkind);
				if(dorm_dispkind==1)
				{
					int orig_success  = 0;

					dorm_dispkind = 0;
					orig_success = wap_reorigination();

					if(orig_success == -1)
					{
						ReturtoErrorState();
						dorm_dispkind =  1;
						break;	
					}
					else if(orig_success ==1)
					{
						ReturtoErrorState();
						dorm_dispkind = 2;
						break;	
					}

			#ifdef FOR_MS
					if(Wtlshello)//다시 해야함
					{
						new_state = WTLS_SESSION_START_S;close_wtls_not_air();
						wtls_match = 1;
						CLNTa_log(1,0,"WTLS에서 Reconnect실패시..");
						
					}else
			#endif//#ifdef FOR_MS
					{
						new_state = LOADING_S;
                        receiveOK = TRUE;
						CLNTa_log(1,0,"정상상태에서 Reconnect실패시..");
					}
					ShowTheBar();					
				}//if(dorm_dispkind==1)
		    #ifdef WAP_USE_PZID2
				else if(dorm_dispkind == 3)
				{
					CLNTa_log(1,0," PACKET zoNE FAIL");
					dorm_dispkind = 0;
					new_state = LOADING_S;
                    receiveOK = TRUE;
					wap_change_pzid();
					ui.event = UI_NONE_F;
					ShowTheBar();									
				}				
			#endif//#ifdef WAP_USE_PZID
                else 
                {
					dorm_dispkind = 0;new_state = HANDLE_KEY_S;ui.event  = UI_NONE_F;					
				}				
				break;				
			}
			ui.event  = UI_NONE_F;
			break;
//================================================================================================
#endif//#ifdef WAP_USE_DORM
		
		case HS_CLR_K:
		case UI_DISPMSGTIMER_F:
			ui_dispmsgtimer_set(0);			
            Wtlshello = FALSE;wtls_enc_start = FALSE;

			if(g_pCard == NULL)
			{
				ProcessBye();
				break;
			}
			else
			{
				extern void wap_set_error(boolean isError);
			#ifdef ABORT_IN_CONTENT  
				extern BOOL bCall_content;
				
#ifndef DIRECT_RESPONSE
				extern UINT16 ContentURLid;
				extern void clnt_make_string_response(unsigned short invokeId, char *pStringResult);
#endif//#ifndef DIRECT_RESPONSE
			#endif//#ifdef ABORT_IN_CONTENT  
			
				wap_set_error(FALSE);
			#ifdef ABORT_IN_CONTENT  
				if(bCall_content)
				{
					extern BOOL isStringResponse;
				#ifndef DIRECT_RESPONSE
					extern boolean isIntegerResponse;
				
					if (isStringResponse)
					{
						isStringResponse = FALSE;
						clnt_make_string_response(ContentURLid, NULL);
					}
					else if (isIntegerResponse)
					{
						isIntegerResponse = FALSE;
						clnt_make_integer_response(ContentURLid, 1004);
					}
				#else
					isStringResponse = FALSE;
				#endif//#ifndef DIRECT_RESPONSE
					wap_display_content_error();
					bCall_content = FALSE;
				}				
			#endif//#ifdef ABORT_IN_CONTENT  
				WAP_Repaint();

				CLNTa_log(1,0,"여기를 타서 handle key로");
				ui.event  = UI_NONE_F;new_state = HANDLE_KEY_S;				
            #ifdef WAP_USE_SKT_NEW
                if(ui.wap_release) 
				{
					if(ReleaseErrorKind == 1)
					{
						CLNTa_log(1,0," Origination시 실패");
						new_state = EXIT_S;ReleaseErrorKind =0;
					}
					else
						ui.event = UI_RELEASE_F;
				}
            #endif//#ifdef WAP_USE_SKT_NEW
			}
			break;
		
		case HS_EAR_DOWN_K:
		case HS_END_K: 
			CLNTa_log(1,0,"Bye -- 12");
#ifdef WAP_USE_SKT_NEW
            if(ui.wap_release) 
            {
               CLNTa_log(1,0,"wap release clear 4");
               ClearRelVar();
            }
#endif//#ifdef WAP_USE_SKT_NEW

			ProcessBye();
			break;

#ifdef REDIRECT
		case UI_WAPPIC_F: 
			rex_set_sigs(&wap_tcb,WAP_BAR_SIG);
			ui.event = UI_NONE_F;
			break;
			
		case UI_COMPLETE_F:
		{
			ui.event = UI_NONE_F;
			CLNTa_log(1,0,"RECIVE UI_COMPLETE_F");
			start_redirect();
			new_state = ACQ_DNS_S;
			ui.event  = UI_NETCONNECTED_F;			
			break;
		}
#endif//#ifdef REDIRECT

		default:
			CLNTa_log(1,0,"이리로 또 오는가?");
			state = ERROR_S;
			ui.event = UI_NONE_F;
			break;

	} //end of switch
#endif //PORTING_B0_WAP
}

void wap_do_abnormal_exit(void)
{
	ui_ds_buf.cmd_name = DS_EXIT_BROWSER_MODE_CMD;
	ds_cmd( &ui_ds_buf ); /* send command to DS */
}

void wap_do_abnormal_exit2(void)
{
#ifdef PORTING_B0_WAP
	mc_ptr = ui_get_mc_buf();
#endif //PORTING_B0_WAP

	if( mc_ptr == NULL ) 
	{
		MSG_HIGH( " ... no END_F sent", 0, 0, 0 );
	}
	mc_ptr->hdr.cmd = MC_END_F;
	mc_cmd( mc_ptr );	
}
void wap_display_error_message(int kindOfError, int kindOfType)
{
#ifdef FEATURE_SKT_EIF//EIF_WAP    //yjahn96 01.02.24 --- eif for wap
    extern INT   g_nEIFWapstatus;

    // ============================================================
	// zeus(yjchoi) 2000.12.05
	// EIF WAP(External Interface for WAP) Error Message 
	extern BOOL g_EIF_WAP_UOPEN;
#ifdef EIF_WAP
	extern int After_WAP_start_what_call;
	int error_type;
#endif	//EIF_WAP
#endif // FEATURE_SKT_EIF

	switch(kindOfError)
	{
		case ERR_MMI_UNKNOWN_CONTENT: 
		{
			WAP_ClearScreen();
			WAP_SetTextAtLine(2, "정해진 형식에",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(3, "맞지않는",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(4, "데이터입니다",WAP_TEXT_ALIGN_CENTER);
			WAP_DisplayMessage();


#ifdef FEATURE_SKT_EIF//EIF_WAP					
			// ========================================================
			// zeus(yjchoi) 2000.12.05
			// EIF WAP(External Interface for WAP) Error Message
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				eif_error_command(EIF_ERR_UOPEN_UNKONWN_CONTENT);
				CLNTa_log(1, 0, "UOPEN:0 [Unknown Content]");
			}
#endif  // EIF_WAP


			break;
		}
		case HTTPFileNotFound: 
		{
			WAP_ClearScreen();
			WAP_SetTextAtLine(2, "요청하신",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(3, "문서가없습니다",WAP_TEXT_ALIGN_CENTER);
			WAP_DisplayMessage();


#ifdef FEATURE_SKT_EIF//EIF_WAP					
			// ========================================================
			// zeus(yjchoi) 2000.12.05
			// EIF WAP(External Interface for WAP) Error Message
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				eif_error_command(EIF_ERR_UOPEN_FILE_NOT_FOUND);
				CLNTa_log(1, 0, "UOPEN:0 [File not found]");
			}
#endif  // EIF_WAP


			break;
		}

		case HTTPForbidden: 
		case HTTPInternalServerError: 
		case ERR_WAE_UA_URL_TIMEOUT: 
		{
			WAP_ClearScreen();
			WAP_SetTextAtLine(2, "서버로부터",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(3, "응답이없습니다",WAP_TEXT_ALIGN_CENTER);
			WAP_DisplayMessage();

#ifdef FEATURE_SKT_EIF  //yjahn96 01.02.24 --- eif for wap
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				eif_error_command(EIF_ERR_UOPEN_NOANSWER_SERVER);
				CLNTa_log(1, 0, "UOPEN:0 [No answer from server]");
			}
            if(g_nEIFWapstatus == EIF_WAP_IWFCON)
            {
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;            
				eif_error_command(EIF_ERR_IWFCON_NOANSWER_SERVER);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [No answer from server]");
            }
			if (g_nEIFWapstatus == EIF_WAP_UPING)
			{
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;   
				eif_error_command(EIF_ERR_UPING_CONNECT_FAIL);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}
#endif  //#ifdef FEATURE_SKT_EIF
            
#ifdef EIF_WAP					
			// ========================================================
			// zeus(yjchoi) 2000.12.05
			// EIF WAP(External Interface for WAP) Error Message
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				error_type = 3;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UOPEN:0 [No answer from server]");
			}
			if (After_WAP_start_what_call == 1)			// iwfcon_command 명령 실행시 에러이면 
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 6;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [No answer from server]");
			}
			if (After_WAP_start_what_call == 3)
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 8;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}
#endif  // EIF_WAP


			break;
		}
		case ERR_WAE_UA_URL_INVALID: 
		{
			WAP_ClearScreen();
			WAP_SetTextAtLine(2, "잘못된",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(3, "URL입니다",WAP_TEXT_ALIGN_CENTER);
			WAP_DisplayMessage();


#ifdef FEATURE_SKT_EIF//EIF_WAP					
			// ========================================================
			// zeus(yjchoi) 2000.12.05
			// EIF WAP(External Interface for WAP) Error Message
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				eif_error_command(EIF_ERR_UOPEN_INVALID_URL);
				CLNTa_log(1, 0, "UOPEN:0 [Invalid URL]");
			}
#endif  // EIF_WAP


			break;
		}
#ifdef BLOCK
		case ERR_PPP_SETUP_FAILURE: 
		{
			WAP_ClearScreen();
			WAP_SetTextAtLine(2, "네트워크가",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(3, "응답하지",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(4, "않습니다",WAP_TEXT_ALIGN_CENTER);
			WAP_DisplayMessage();

            
#ifdef FEATURE_SKT_EIF            //yjahn96 01.02.24 --- eif for wap
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				eif_error_command(EIF_ERR_UOPEN_CONNECT_FAIL);
				CLNTa_log(1, 0, "UOPEN:0 [Network connecting failure]");
			}
            if(g_nEIFWapstatus == EIF_WAP_IWFCON)
            {
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;            
				eif_error_command(EIF_ERR_IWFCON_CONNECT_FAIL);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [Network connecting failure]");
            }
			if (g_nEIFWapstatus == EIF_WAP_UPING)
			{
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;   
				eif_error_command(EIF_ERR_UPING_CONNECT_FAIL);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}

#endif  //#ifdef FEATURE_SKT_EIF

#ifdef EIF_WAP					
			// ========================================================
			// zeus(yjchoi) 2000.12.05
			// EIF WAP(External Interface for WAP) Error Message
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				error_type = 5;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UOPEN:0 [Network connecting failure]");
			}
			if (After_WAP_start_what_call == 1)			// iwfcon_command 명령 실행시 에러이면 
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 7;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [Network connecting failure]");
			}
			if (After_WAP_start_what_call == 3)
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 8;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}
#endif  // EIF_WAP


			break;
		}
#endif //#ifdef BLOCK

#ifdef  REDIRECT
		case ERR_WAE_UA_SESSION_NOT_CONNECTED : 
		{
          
#ifdef FEATURE_SKT_EIF            //yjahn96 01.02.24 --- eif for wap
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				eif_error_command(EIF_ERR_UOPEN_NOANSWER_SERVER);
				CLNTa_log(1, 0, "UOPEN:0 [No answer from server]");
			}
            if(g_nEIFWapstatus == EIF_WAP_IWFCON)
            {
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;            
				eif_error_command(EIF_ERR_IWFCON_NOANSWER_SERVER);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [No answer from server]");
            }
			if (g_nEIFWapstatus == EIF_WAP_UPING)
			{
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;   
				eif_error_command(EIF_ERR_UPING_CONNECT_FAIL);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}
#endif  //#ifdef FEATURE_SKT_EIF

#ifdef EIF_WAP					
			// ========================================================
			// zeus(yjchoi) 2000.12.05
			// EIF WAP(External Interface for WAP) Error Message
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				error_type = 3;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UOPEN:0 [No answer from server]");
			}
			if (After_WAP_start_what_call == 1)			// iwfcon_command 명령 실행시 에러이면 
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 6;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [No answer from server]");
			}
			if (After_WAP_start_what_call == 3)
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 8;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}
#endif  // EIF_WAP


			CLNTa_log(1,0," WSP session not connected for redirect");
		  /*=================================================================================
										Redirect 기능 첨가

			1. redirect.bCorrect = TRUE일 경우에만 처리 아니면 SKIP
			2. 1이 TRUE이면,browser_terminate을 행하고,UI는 ENTER_S로 간다.
			3. Enter_s에서 CLNTa_terminate의 수행이 완료되면, 그 다음은 정상적으로 동작한다.
			4. Permanent일 경우에는 NV에 저장하며 아닐 경우에는 temp한다.
			5. ENTER_S에서 WapTraffic_Term = TRUE가 아니면 계속 무한 loop을 돌므로 이것을 방지
			   하기 위하여, 억지로 TRUE로 만듬 (실제로 Traffic이 끊어진것은 아님)
		   =================================================================================*/
			if(redirect.bCorrect)//redirect수행 
			{
#ifdef PORTING_B0_WAP
				ui_dispmsgtimer_set(0);
#endif //PORTING_B0_WAP

				if(save_redirecturl())
				{			
					redirect_terminate();browser_terminate();					
					displayloading(12,TRUE);
					break;
				}				
			}
		}
#endif//#ifdef  REDIRECT
		default:
		{
			WAP_ClearScreen();
			WAP_SetTextAtLine(2, "정보제공자로부터",WAP_TEXT_ALIGN_CENTER);
			WAP_SetTextAtLine(3, "응답이없습니다",WAP_TEXT_ALIGN_CENTER);
			WAP_DisplayMessage();

#ifdef FEATURE_SKT_EIF            //yjahn96 01.02.24 --- eif for wap
			if (g_EIF_WAP_UOPEN)
			{
                extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				eif_error_command(EIF_ERR_UOPEN_NOANSWER_SERVER);
				CLNTa_log(1, 0, "UOPEN:0 [No answer from server]");
			}
            if(g_nEIFWapstatus == EIF_WAP_IWFCON)
            {
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;            
				eif_error_command(EIF_ERR_IWFCON_NOANSWER_SERVER);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [No answer from server]");
            }
			if (g_nEIFWapstatus == EIF_WAP_UPING)
			{
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;   
				eif_error_command(EIF_ERR_UPING_CONNECT_FAIL);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}
#endif  //#ifdef FEATURE_SKT_EIF

#ifdef EIF_WAP					
			// ========================================================
			// zeus(yjchoi) 2000.12.05
			// EIF WAP(External Interface for WAP) Error Message
			if (g_EIF_WAP_UOPEN)
			{
				extern void eif_error_command(int);
				g_EIF_WAP_UOPEN = FALSE;
				error_type = 3;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UOPEN:0 [No answer from server]");
			}
			if (After_WAP_start_what_call == 1)			// iwfcon_command 명령 실행시 에러이면 
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 6;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "IWFCON:Connection Failed [No answer from server]");
			}
			if (After_WAP_start_what_call == 3)
			{
				extern void eif_error_command(int);
				After_WAP_start_what_call = 0;
				error_type = 8;
				eif_error_command(error_type);
				CLNTa_log(1, 0, "UPING:Connection Failed");
			}
#endif  // EIF_WAP


			break;
		}		
	}
}
void wap_handle_earpiece_down(void)
{
#ifdef PORTING_B0_WAP
#ifdef WAP_USE_SKT_NEW
    if(ui.wap_release) 
    {
         CLNTa_log(1,0,"wap release clear 7");
         ClearRelVar();
    }
#endif//#ifdef WAP_USE_SKT_NEW
    ProcessBye();
#endif //PORTING_B0_WAP
}

void wap_handle_memory_shortage(void)
{
	report_event(WAP_DRAW_STOP_EVENT);
	wap_display_exit_message3();
#ifdef SW_RESET_ENABLE
	SW_Reset();
#endif//#ifdef SW_RESET_ENABLE
#ifdef PORTING_B0_WAP
	ui_dispmsgtimer_set(3000);wap_do_abnormal_exit();

	ui.event = UI_NONE_F;new_state = EXIT_S;
#endif //PORTING_B0_WAP

}
#ifdef PORTING_B0_WAP
void WAP_Time_Stmap(BOOL bStart)
{

	extern UINT32  CLNTa_currentTime (VOID);
	extern void ntop_used_time ( dword new_time );
	if(bStart)
	{
		g_nTimeStamp = CLNTa_currentTime();//종료	
		//CLNTa_log(1, 0, "시작시각 %d", g_nTimeStamp);
	}
	else
	{
		UINT32 nUsedTime;
		if(g_nTimeStamp == 0)   return; //<= 0) return;
		nUsedTime = CLNTa_currentTime() - g_nTimeStamp;
		//CLNTa_log(1, 0, "끝 (%d초 사용)", nUsedTime);
		ntop_used_time(nUsedTime);
		g_nTimeStamp = 0;
	}

}
#endif //PORTING_B0_WAP
#ifdef FEATURE_NETGAME
void wap_display_exit_message_for_netgame(void)
{
	char temp[21];

	g_bTerminatedWAP = TRUE;

	memset(temp, NULL, 21);
	sprintf(temp, "%s%s%s%s GVM",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP
	//====================================================================
	WAP_ClearScreen();
//	WAP_SetTextAtLine(1, "\207\364\207\365\207\366\207\367게임방",WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(2, temp, WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "다운로드 연결중 ",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}
#endif //FEATURE_NETGAME

void wap_display_exit_message(void)
{
	char temp[21];

	//g_bTerminatedWAP = TRUE;

	memset(temp, NULL, 21);
	sprintf(temp, "%s%s%s%s을",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP

	//====================================================================
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, temp, WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "종료합니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

void wap_display_exit_message2(void)
{
	char temp[21];

	//g_bTerminatedWAP = TRUE;

	memset(temp, NULL, 21);
	sprintf(temp, "%s%s%s%s연결이",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP

	WAP_ClearScreen();
	WAP_SetTextAtLine(2, temp, WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "종료되었습니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

void wap_display_exit_message3(void)
{
	char temp[21];

	//g_bTerminatedWAP = TRUE;

	memset(temp, NULL, 21);
	sprintf(temp, "%s%s%s%s연결이",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP

	WAP_ClearScreen();
#ifdef WAP_USE_LEAK
	WAP_SetTextAtLine(0, "현재 연결이 종료",WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(1, "되었습니다.",WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(2, "재연결할까요?",WAP_TEXT_ALIGN_CENTER);
#else
	WAP_SetTextAtLine(1, "이용자가 많아",WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(2, "\207\364\207\365\207\366\207\367연결이",WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "종료되었습니다",WAP_TEXT_ALIGN_CENTER);
#endif//#ifdef WAP_USE_LEAK
	WAP_DisplayMessage();
}

void wap_display_exit_for_receive_call(void)
{
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "전화가", WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "왔습니다", WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

void wap_display_exit_for_send_call(void)
{
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "전화를",WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "연결합니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

void wap_display_exit_for_gvm(void)
{
	WAP_ClearScreen();
	/* rosa 01-04-06 --> @why, what */
	WAP_SetTextAtLine(2, "\x82\xEE\x82\xEF\x82\xF0\x82\xF1 GVM",WAP_TEXT_ALIGN_CENTER); /* n.TOP GVM */
	/* <-- */
	WAP_SetTextAtLine(3, "연결중",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

void wap_display_exit_for_pm(void)
{
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "포토메일을",WAP_TEXT_ALIGN_CENTER); 	
	WAP_SetTextAtLine(3, "종료합니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

#ifdef FEATURE_SKVM
void wap_display_exit_for_skvm(void)
{
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "SKVM을",WAP_TEXT_ALIGN_CENTER); 	
	WAP_SetTextAtLine(3, "시작합니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}
#endif //FEATURE_SKVM


void do_makecall(void)
{
	makecall =TRUE;
	entertain = FALSE;newurl_input = FALSE;
#ifdef PORTING_B0_WAP
	ClearRelVar();
	wap_display_exit_for_send_call();
	terminate_timer();ui.event = UI_NONE_F;
#endif //PORTING_B0_WAP
	CLNTa_log(1,0,"Terminate WAP");
	ProcessBye();
}


#ifdef PORTING_B0_WAP 
#ifdef WAP_USE_SECURE
void displayloading(int catagory,boolean setting)
#else
LOCAL void displayloading(int catagory,boolean setting)
#endif//#ifdef WAP_USE_SECURE
#else //PORTING_B0_WAP
void displayloading(int catagory,boolean setting)
#endif //PORTING_B0_WAP
{
	int nAniTextMode = 0;
	BOOL bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
	
	switch(catagory)
	{
		case 0://연결중
			start_disp = TRUE;
			nAniTextMode = WAP_ANI_TEXT_CONNECTING;//접속중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		case 1://데이타 수신중
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_RECEIVING;//수신중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
			break;

		case 2://보안 요구중
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
			break;

		case 3://보안 종료중 
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
			break;

		
		case 5://재수신중
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_RECEIVING;//수신중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
			break;

		case 6://보안요구중
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
			break;

		case 8://Key교환중
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
			break;

		case 11://즐겨찾기추가중
		{
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_ADDBOOKMARK;//추가중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
			break;
		}
#ifdef REDIRECT
		case 12://Redirect
		{
			start_disp = FALSE;
			nAniTextMode = WAP_ANI_TEXT_RESETUP;//추가중
			bAniBarDirection = WAP_ANI_BAR_FROM_RIGHT;
		}
#endif//#ifdef REDIRECT
	}

	g_bDrawBar = TRUE;
#ifdef PORTING_B0_WAP
	if(setting) report_event(WAP_DRAW_EVENT);
#endif //PORTING_B0_WAP
}

void process_common(void)
{
#ifdef PORTING_B0_WAP
	switch(ui.event)
	{
/*==========일단 막어둔다.=========================
		case UI_SMS_MSG_F:
		{
			if(entertain)
			{
				entertain = FALSE;
			}
					 
			if(newurl_input)
					newurl_input = FALSE;

			g_nAniBarCount = 0;
			ui_wappic_set(0, FALSE);
						
			process_wap_push_mail();
			break;
		}
 ==========일단 막어둔다.=========================*/
		case UI_WAPPIC_F:
		{
			rex_set_sigs(&wap_tcb,WAP_BAR_SIG);
			ui.event = UI_NONE_F;
			break;
		}

		case HS_EAR_DOWN_K:
		case HS_END_K:
			 CLNTa_log(1,0,"Bye -- 12");
#ifdef WAP_USE_SKT_NEW
            if(ui.wap_release) 
            {
               CLNTa_log(1,0,"wap release clear 5");
               ClearRelVar();
            }
#endif//#ifdef WAP_USE_SKT_NEW
			ProcessBye();
			break;
		case UI_FADE_F:	
			CLNTa_log(1,0,"Bye -- 13 -1");
		case UI_NETDISCONNECTED_F:
			CLNTa_log(1,0,"Bye -- 13 -2");
		case UI_RELEASE_F:
			CLNTa_log(1,0,"Bye -- 13");
			process_release();
			break;

		case UI_DOALERT_F:
		{
			entertain = FALSE;newurl_input = FALSE;

			if(load_data)
			{
				if(!Wtlshello) SKY_stop(g_pCard->viewId);
			}
			terminate_timer();
			report_event(WAP_DRAW_STOP_EVENT);

			wap_display_exit_for_receive_call();
			new_state = EXIT_S;

			rcvcall =TRUE;
			ui_dispmsgtimer_set(1500);
			ui.event = UI_NONE_F;
			break;
		}
		case HS_CLR_K:
		case HS_PF2_K://right
		{
			if(g_bBlocked) 
			{
				ui.event = UI_NONE_F;
				break;
			}
            
            if(g_bDrawBar)
            {
                g_bDrawBar = FALSE;
                if(g_pCard != NULL)
				{
					wap_clk_key = TRUE;
                   #ifdef WAP_USE_SKT_NEW
                    if(ui.wap_release)
                    {
                        CLNTa_log(1,0," ACQ_DNS CLR_K when wap_relase");
                        save_min_state = HANDLE_KEY_S;
                    }
                    else
                   #endif//#ifdef WAP_USE_SKT_NEW                        
                        new_state = HANDLE_KEY_S;				
					if(!ui.wap_release) report_event(WAP_STOP_EVENT);
                    else process_stop_event();
				}
				else ProcessBye();
            #if MMI_TYPE == MMI_A10                    
                ui.from_where = 2;
            #endif
                ui.event = UI_NONE_F;
				break;
            }
		}

        default:
        {
#ifdef WAP_USE_SKT_NEW
            int nResultHandleKey = 0;
        	if(
                (HS_1_K <= ui.event && ui.event <= HS_9_K)|| 
                (
                  ui.event == HS_REC_K   || ui.event == HS_PLAY_K || ui.event == HS_STAR_K || 
                  ui.event == HS_POUND_K || ui.event == HS_UP_K   || ui.event == HS_DOWN_K ||
                  ui.event == HS_PF1_K   || ui.event == HS_PF2_K  || ui.event == HS_PF3_K  ||
                  ui.event == HS_CLR_K   || ui.event == HS_WAP_K
                )
              )
            {
                if(ui.wap_release)
                {
                    CLNTa_log(11,0,"Key processing in ACQ_S");
                    nResultHandleKey = WAP_HandleKeys(ui.event);
		            Process_After_HandleKeys(nResultHandleKey); 
                    ui_wappic_set(130,TRUE);new_state = ACQ_DNS_S;
                }
            }
#endif//#ifdef WAP_USE_SKT_NEW
            ui.event = UI_NONE_F;
            break;
        }
	}//end of switch
#endif //#ifdef PORTING_B0_WAP
}

void processcommonWTLS(int nKey)
{
#ifdef PORTING_B0_WAP
	switch(nKey)
	{
#ifdef WAP_USE_DORM
		case UI_DORM_EXIT_F:
		{
			 CLNTa_log(1,0,"Receive UI_DORM_EXIT_REQ_F in secure");
			 SendExitDormReq();
             ui.event = UI_NONE_F;
			 break;
		}
#endif//#ifdef WAP_USE_DORM

#ifdef WAP_USE_DORM
		case UI_WANT_RECONNECT_F:
		{
			CLNTa_log(1,0,"Reconnect Want Flag received in WTLS");
			new_state  = ERROR_S;isFailOrig = TRUE;
			ui.event   = UI_NONE_F;
			break;
		}
		case UI_WAP_DORM_F:
		{
			CLNTa_log(1,0,"WAP go to DORMANT MODE %d in WTLS",ui.wap_dorm_mode);
            if(Wtlshello)  Wtlshello = FALSE;

            EnterDormMode();
			ui.event = UI_NONE_F;
			break;
		}
#ifdef WAP_USE_PZID2
        case UI_CONNECTED_F:
        {
            if(ui.wap_pzid)
            {
                uiscrn_ann( HS_AN_INUSE, HS_AN_INUSE );
			    dbi.in_use = TRUE;
			    db_put( DB_IN_USE, &dbi );

			    ui.beep_updown = TRUE;
			    ui.call = TRUE;
            }
            ui.event = UI_NONE_F;
            break;
        }
#endif//#Ifdef WAP_USE_PZID2
		case UI_WAKEUP_F:
		{
			CLNTa_log(1,0,"Receive WAKEUp so sucess reorig in WTLS");
			/*==========================================
				WAP reorigination 성공시 이리로 옴 
			 ===========================================*/
			if(ui.wap_dorm_mode)
			{
				CLNTa_log(1,0," SUCCESS REORIGINATION in WTLS");
					
			#ifdef WAP_USE_PZID2
				if(!ui.wap_pzid)
			#endif//#ifdef WAP_USE_PZID2
				{
					dbi.in_use = TRUE;
					db_put( DB_IN_USE, &dbi );					
					uiscrn_ann( HS_AN_INUSE, HS_AN_INUSE );
				}
			#ifdef WAP_USE_PZID2
				else
				{
					ui.wap_pzid = FALSE;						
				}
			#endif//#ifdef WAP_USE_PZID2

				ui.wap_dorm_mode = FALSE;				
				WapTraffic_Term = FALSE;
				ui.beep_updown = TRUE;
				ui.call = TRUE;

			#ifdef FOR_MS//Check하여야 함 		
				if(wtls_enc_start)	bStopKey  = FALSE;
			#endif//#ifdef FOR_MS			
	
				if(!wap_clk_key)
				{
                    receiveOK = TRUE;
					//보안 초기화=========================================
					close_wtls_not_air();
					bStopKey = FALSE;bSetKey = FALSE;	
					wtls_match = 1;bWstart = FALSE;
					//보안 초기화=========================================					
					new_state = WTLS_SESSION_START_S;
				}
				else
				{
					wap_clk_key = FALSE;
					memset(SaveWtls,0x00,1000);
					Do_cancel(FALSE);
				}
				wap_rcv_page = FALSE;
				ui.event     = UI_NONE_F;
			}
			break;
		}
#endif//#ifdef WAP_USE_DORM

#ifdef REDIRECT	
		case UI_REDIRECT_F:
		{
			CLNTa_log(1,0,"Receive Redirect flag");
			browser_terminate();
			displayloading(12,FALSE);
			ui.event = UI_NONE_F;
			break;
		}

		case UI_COMPLETE_F:
		{
			ui.event = UI_NONE_F;
			CLNTa_log(1,0,"RECIVE UI_COMPLETE_F in secure");
			start_redirect();
			new_state = ACQ_DNS_S;
			ui.event  = UI_NETCONNECTED_F;			
			break;
		}
#endif//#ifdef REDIRECT	
		case UI_WTLS_F:
		{
			bStopKey  = TRUE;receiveOK = TRUE;

			if(clnt_wsp_send(spSKT->ucpBuffer,spSKT->ulBufSize,SaveWtls,TRUE) ==-1) 
			{
				isFailureWTLSB = TRUE;
				new_state = ERROR_S;
				ui.event  = UI_NONE_F;
				break;
			}
			ui.event = UI_NONE_F;

			//reaccess setting
#ifndef BROWSER_UPDATE
			retry = 0;ui_waptimer_set(15000,FALSE);
#endif//#ifndef BROWSER_UPDATE
			break;
		}

		case UI_WAPTIME_F:
		{
			//2000/03/11 for secure jjkyg
			CLNTa_log(1,0,"WLTS WAPTIME FLAG");
			bStopKey  = FALSE;
			break;
		}
		case HS_EAR_DOWN_K:
		case HS_END_K:
		{	
			//2000/03/11 for secure jjkyg
			if(bStopKey)
			{
			  ui.event = UI_NONE_F;
			  break;
			}
#ifdef WAP_USE_SKT_NEW
            if(ui.wap_release) 
            {
               CLNTa_log(1,0,"wap release clear 6");
               ClearRelVar();
            }
#endif//#ifdef WAP_USE_SKT_NEW
			ProcessBye();
			break;
		}

		//jjkyg 09/21
		case UI_FADE_F:		
		case UI_RELEASE_F:
		{
			//2000/03/11 for secure jjkyg
			bStopKey  = FALSE;

			process_release();
			ui.event = UI_NONE_F;
			break;
		}

		case HS_CLR_K:
		case HS_PF2_K://right
		{
			if(g_bBlocked || bStopKey) 
			{
				ui.event = UI_NONE_F;
				return;
			}
			go_storeWml(TRUE);
			receiveOK = FALSE;report_event(WAP_DRAW_STOP_EVENT);

			if(wtls_match == 1) memset(SaveWtls,0x00,1000);
			if(g_pCard != NULL)
			{
				wap_clk_key = TRUE;
				new_state = HANDLE_KEY_S;				
				report_event(WAP_STOP_EVENT);
			}
			else//이전 카드가 없는 경우 WAP을 종료한다
			{
				ProcessBye();
			}			

			if(g_bGettingContent)//picture mate/music bell
				g_bGettingContent = FALSE;
			ui.event = UI_NONE_F;
			break;
		}
 /*============================================================================
		    WAP중 PUSH나 Email을 받았을 경우의 처리
  ============================================================================*/
		case UI_SMS_MSG_F:
		{
			if(bStopKey)
			{
			   ui.event = UI_NONE_F;
			   break;
			}
			report_event(WAP_DRAW_STOP_EVENT);
			ui_waptimer_set(0,FALSE);

			if(wtls_match == 1)//key가 없음 
				memset(SaveWtls,0x00,1000);

			if(g_bGettingContent)//picture mate/music bell
				g_bGettingContent = FALSE;

			process_wap_push_mail();
			break;
		}
 /*============================================================================*/
		case UI_WAPPIC_F:
		{
			rex_set_sigs(&wap_tcb,WAP_BAR_SIG);
			ui.event = UI_NONE_F;
			break;
		}
		default:
		{
			ui.event = UI_NONE_F;
			break;
		}
	}//end of switch
#endif //PORTING_B0_WAP
}

#ifndef WAP_USE_SECURE
LOCAL void send_usecontent(CHAR* url)
{
	wap_cmd_type msg_ptr;
	
	msg_ptr.hdr.cmd = CLNT_GET_CONTENT;
	msg_ptr.get_content.url = url;	
	msg_ptr.get_content.urlId = 1;
	msg_ptr.get_content.isOTAallowed = TRUE;
#ifdef debug
	CLNTa_log(1,0," usecontent url %s",url);
#endif//#ifdef debug->여기까지 왔음
	make_cmd_other2wap( &msg_ptr );	
}

//===============================================================================
//						 ST_NULL,ST_WAIT만 사용함 
//===============================================================================
int clnt_wsp_send(unsigned char *ucpSendData, unsigned int nSendLen,char *cpUrl,BOOL state)
{
	/*=========================================================================
	    state = TRUE  : - Postdata를 update해야 한다는 의미
		                - Nego message의 경우, 처음으로 망으로 전송시 사용
		state = FALSE : - Postdata를 update하지 말라는 의미
					    - Nego message의 경우, 망으로 재전송시 사용
	 =========================================================================*/
	if(state)
	{
		if(nSendLen > 1500) return -1;
		CLNTa_log(1,0,"Send data WTLS!!!!");
		wsp_send(ucpSendData,nSendLen);
	}

	send_usecontent(cpUrl);
	return 0;	
}
#endif//#ifndef WAP_USE_SECURE

/*******************************************************************
					close_wtls_not_air()
********************************************************************/
void close_wtls_not_air()
{
	close_wtls_session();
}
/******************************************************************
				      go_storeWml()
********************************************************************/
#ifdef PORTING_B0_WAP
LOCAL void go_storeWml(boolean del)
#else
void go_storeWml(boolean del)
#endif //PORTING_B0_WAP
{
	//1: KEY 2: OPTION 3: LINK
	if(del){
		enc_use.viewid  = 0;
		enc_use.id      = 0;
		enc_use.wmlkind = 0;
		return;
	}
	switch(enc_use.wmlkind)
	{
		case 1:
			SKY_keySelected (1,enc_use.id);
			break;

		case 2:
			SKY_optionSelected (1,enc_use.id);			
			break;

		case 3:
			SKY_textSelected (1,enc_use.id);
    		break;
	}
}

#ifdef WAP_USE_DORM
extern char urladd[500];
#ifndef WAP_USE_SECURE
	LOCAL void go_storeWml_normal(BOOL del)
#else
	void go_storeWml_normal(BOOL del)
#endif//#ifndef WAP_USE_SECURE
{
    extern char g_NewURL[200];

	if(del)
	{
		normal_use.viewid  = 0;
		normal_use.id      = 0;
		normal_use.wmlkind = 0;
		return;
	}
	//1: KEY 2: OPTION 3: LINK
	switch(normal_use.wmlkind)
	{
		case 1:
		{
			CLNTa_log(1,0,"SKY_keySelected[%d]",normal_use.id);
			SKY_keySelected (normal_use.viewid,normal_use.id);
			break;
		}

		case 2:
			SKY_optionSelected (normal_use.viewid,normal_use.id);			
			break;

		case 3:
			SKY_textSelected (normal_use.viewid,normal_use.id);
			break;

		case 4://즐겨찾기 추가...
			SKY_loadURL(normal_use.viewid,urladd,TRUE); 
			break;

		case 5://즐겨찾기
			SKY_loadURL(normal_use.viewid, "http://wap.n-top.com/cgi/get_service.cgi?SERVICE_ID=100011710000007",TRUE);
			break;
        case 6:
            SKY_loadURL(1,temp_url,FALSE);
            break;
		case 7:
			SKY_goBack(1);
			break;

        case 8:
			SKY_reload(normal_use.viewid);
			break;

        case 9:
			SKY_loadURL(normal_use.viewid,g_NewURL,FALSE);
			break;

	}
}
#endif//#ifdef WAP_USE_DORM
/*******************************************************************
				jjip_addr(char* cp)
********************************************************************/
#define DUMMY_INADDR_NONE				0xffffffff

unsigned long jjip_addr(char* cp)
{
	int base, n;
	char c;
	unsigned int parts[4], val;
	unsigned int *pp = parts;

	c = *cp;
	for (;;) { /* 0x=hex, 0=octal, isdigit=decimal */
		if (!isdigit((int)c))
			return DUMMY_INADDR_NONE;
		val = 0; base = 10;
		if (c == '0') {
			c = *++cp;
			if (c == 'x' || c == 'X') {
				base = 16; c = *++cp;
			} else
				base = 8;
		}
		for (;;) {
			if ((c < 0x7f) && isdigit((int)c)) {
				val = (val * base) + (c - '0');
			} else if (base == 16 && (c < 0x7f) && isxdigit((int)c)) {
				val = (val << 4) |
					(10 + c - (islower((int)c) ? 'a' : 'A'));
			} else
				break;
			c = *++cp;
		}
		if (c == ':') {
			if (pp >= parts + 3)
				return 0;
			*pp++ = val;
			c = *++cp;
		} else
			break;
	}
	if (c != '\0' && (!(c < 0x7f) || !isspace((int)c)))
		return DUMMY_INADDR_NONE;

	n = pp - parts + 1;
	switch (n) {
		case 1:				/* a -- 32 bits */
			break;

		case 2:				/* a.b -- 8.24 bits */
			if (val > 0xffffff)
				return DUMMY_INADDR_NONE;
			val |= parts[0] << 24;
			break;

		case 3:				/* a.b.c -- 8.8.16 bits */
			if (val > 0xffff)
				return DUMMY_INADDR_NONE;
			val |= (parts[0] << 24) | (parts[1] << 16);
			break;

		case 4:				/* a.b.c.d -- 8.8.8.8 bits */
			if (val > 0xff)
				return DUMMY_INADDR_NONE;
			val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
			break;
	}
	return htonl(val);
}//end of jjip_addr();


LOCAL void terminate_timer(void)
{
	g_nAniBarCount = 0;
#ifdef PORTING_B0_WAP
	ui_wappic_set(0, FALSE);
	ui_waptimer_set(0, FALSE);
	ui_dispmsgtimer_set(0);
#endif //PORTING_B0_WAP
}

void process_release(void)
{
    report_event(WAP_DRAW_STOP_EVENT);
#ifdef WAP_USE_DORM
	if(cdma.p_rev_in_use >= 6 && wap_duration >0)
	{
		if(
			((dsi_srvc_mode == DS_BROWSER_PKT_SRVC_MODE)&&(dsi_protocol_state == DS_PROTOCOL_OPEN))
#ifdef WAP_USE_PZID2
            ||(SKY_IS_WAP_PZID())
#endif//#Ifdef WAP_USE_PZID2
		)
		{
			CLNTa_log(1,0,"Dormant 시 release.. -->loading");
			return;
		}
        if(SKY_IS_WAP_RELEASE())
        {
            CLNTa_log(1,0,"2G-->2.5G in wapstate.c");
            CheckCAIrev();
        }
	}
	else
	{
#ifdef WAP_USE_SKT_NEW
		CLNTa_log(1,0,"2G dormnat");
        CheckCAIrev();		
#endif//#ifdef WAP_USE_SKT_NEW
        CLNTa_log(1,0,"wap_release not setting process release");
	}	
#endif//#ifdef WAP_USE_DORM
    bReceiveRelease = TRUE;
//  new_state = EXIT_S; --> sibling WAPEXIT로 대체!!
#ifdef WAP_USE_SKT_NEW
	if(!SKY_IS_WAP_RELEASE())
#endif//#ifdef WAP_USE_SKT_NEW
	{
	    report_event(WAP_TEMINATE_EVENT);
	}
#ifdef WAP_USE_SKT_NEW
	else
	{
		CLNTa_log(1,0,"wap_release TRUE & terminate");
		process_terminate_event();
	}
#endif//#ifdef WAP_USE_SKT_NEW
	EQS_START_SIBLING(SKA_WAPEXIT,FALSE);
}

LOCAL void TerminateVar(void)
{
	extern BOOL      ps_start;
#ifdef WAP_USE_SECURE
	extern wap_handshake_state handShakestate;
#endif//#ifdef WAP_USE_SECURE

#ifndef DIRECT_RESPONSE
	extern boolean isIntegerResponse;
#endif//#ifndef DIRECT_RESPONSE

#ifdef ABORT_IN_CONTENT  
	extern BOOL isStringResponse;
	extern BOOL bCall_content;
#endif//#ifdef ABORT_IN_CONTENT  

#ifdef WAP_USE_SECURE
	handShakestate = HAND_NULL_STATE; 
#endif//WAP_USE_SECURE

#ifdef WAP_USE_SKT_NEW
    save_min_state = NONE_S;
#endif//#ifdef WAP_USE_SKT_NEW

#ifdef WAP_USE_DORM
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
	SKY_SET_WAP_DORM(FALSE);
#else
	ui.wap_dorm_mode = FALSE;
#endif //#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
	wap_dorm_mode    = WAP_MODE_NOT_DORM;
	wap_clk_key      = FALSE;wap_flow_cont    = FALSE;wap_rcv_page     = FALSE;
//    wap_reorig_process= FALSE;
#endif//#ifdef WAP_USE_DORM

#ifndef DIRECT_RESPONSE
	isIntegerResponse = FALSE;	
#endif//#ifndef DIRECT_RESPONSE

#ifdef ABORT_IN_CONTENT  
	isStringResponse  = FALSE;bCall_content = FALSE;
#endif//#ifdef ABORT_IN_CONTENT  
    ps_start  = FALSE;

#ifdef FOR_MS
	Wtlshello      =  FALSE;wtls_enc_start =  FALSE;
	wtls_match     =  0;    bStopKey       =  FALSE;
	bWtlsgoback    =  FALSE;bSetKey        =  FALSE;
#endif//#ifdef FOR_MS

#ifdef REDIRECT	
	if(!redirect.bCorrect)
#endif//#ifdef REDIRECT	
	{
		push_acc =0;
	}

	check_browser_init = TRUE; 
	DataFst = FALSE;receiveOK = FALSE;savefirst = TRUE;
	bReceiveData = FALSE;

	start_disp = FALSE;

    if(g_bBlocked)
    {
        CLNTa_log(1,0,"g_bBlocked not setting in TerminateVar");
        //g_bBlocked = FALSE;
    }
	SKY_SET_WAP_PZID(FALSE); //ui.wap_pzid = FALSE; 

}

LOCAL int process_handle_event(int event)
{
#ifdef PORTING_B0_WAP
	switch(event)
	{
		case HS_EAR_DOWN_K:
		case HS_END_K:
		case HS_PF2_K:
		case HS_CLR_K:
		{
			if(g_bBlocked) 
			{
				ui.event = UI_NONE_F;
				break;
			}			
			report_event(WAP_DRAW_STOP_EVENT);

#ifdef FEATURE_NETGAME
			if(ui.to_netgame)
				wap_display_exit_message_for_netgame();
			else
#endif//netgame
				wap_display_exit_message();

			terminate_timer();
			WapTraffic_Term = TRUE;browserTerComp  = TRUE;
            ui_dispmsgtimer_set(1500);	
	
            ui.event = UI_NONE_F;new_state = EXIT_S;
			return -1;
		}
		case UI_FADE_F:		
		case UI_RELEASE_F:
			process_release();
			return -1;
		
		case UI_DISPMSGTIMER_F:
		{
			browserTerComp =TRUE;
			return 0;
		}

		default:
			ui.event = UI_NONE_F;
			return 0;
	}	
#endif //PORTING_B0_WAP
	return 0;
}		

byte ret_passwd()
{
	return(g_pCurInput->isPassword);
}

#ifdef PORTING_B0_WAP
LOCAL void resetalldata(void)
#else
void resetalldata(void)
#endif //PORTING_B0_WAP
{
  dsm_item_type *item_ptr;

  while(( item_ptr = (dsm_item_type *)q_get(&psi_udp_q)) != NULL)
  {
   	(void)dsm_free_packet(&item_ptr);
  }
  while(( item_ptr = (dsm_item_type *)q_get(&psi_udp_out_q)) != NULL)
  {
    (void)dsm_free_packet(&item_ptr);
  }
}

#if (defined WAP_B1) || (defined WAP_B2)
#define gen_setbit(byte,bitno,color)  \
			(byte |= ((color << 7) >> bitno))
#define gen_getbit(byte,bitno)   \
		  ( ((byte)&(1<<(bitno)))? 1 : 0 )
#endif  //#if (defined WAP_B1) || (defined WAP_B2)

void PlayNTopConntImage(void)//WAP 초기접속
{
	//yjahn96 00.12.28 -------------------
	extern byte* /*EQS_API*/ GetScreenBuffer(void);
	byte* LCDWordAniBuffer = (byte* )GetScreenBuffer();

#ifdef PORTING_B0_WAP
	if(ui.ani_option[ANI_TYPE_NTOP].Option == ANI_OPTION_NONE)
#endif //PORTING_B0_WAP
	{
#if (defined WAP_B1) || (defined WAP_B2)
        byte temp[15*2];
        byte color;
        int i, j, k ,x;
#endif  //#if (defined WAP_B1) || (defined WAP_B2)
        int y;
		const word WAP_Init_Image[2000] = {//3072//WAP_LCD_PIXELS
			#include	".\REX\MMI\WAP_Init.dhf"
		};
        //COLOR OldColor;
        //extern VOID EQS_ClearRegion(COORD xLeft, COORD yTop, COORD xRight, COORD yBottom);
	
		//정지 이미지
		g_bExistConntAnimImage = FALSE;//need to draw bar

        //OldBackColor = EQS_GetBackColor();
        //EQS_SetBackColor(BLACK);
        //EQS_ClearRegion(0, 0/*16*/, 119 , 119);
        //EQS_SetBackColor(OldBackColor);
        EQS_FillRectangle(0, WAP_HEAD_HEIGHT, 119, 106, BLACK);
		//WAP_Clear_LCDDataBuffer();

#if !((defined WAP_B1) || (defined WAP_B2))
		for(y = WAP_HEAD_HEIGHT; y < 104/*80*/; y++)
			//yjahn96 00.12.28 -------------------
			//memcpy( &LCDWordAniBuffer[y][0], (byte*)&WAP_Init_Image[y*(16-1)], (16-1)*2);
			memcpy( LCDWordAniBuffer+ y*WAP_LCD_TOTAL_ADDRESS_X, (byte*)&WAP_Init_Image[(y-WAP_HEAD_HEIGHT)*(16-1)], (16-1)*2);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		for(y = WAP_HEAD_HEIGHT; y < 104/*80*/; y++)
        {
            memcpy( temp, (byte*)&WAP_Init_Image[(y-WAP_HEAD_HEIGHT)*(16-1)], (16-1)*2);

            x= 0;
            for(i = 0; i < 15*2; i++)
            {
                for(j = 0; j < 8; j++)
                {
                    color = gen_getbit(temp[i], 7-j);

                    for(k=0; k<4; k++)
                    {
                        if(j%2 == 0)
                            gen_setbit(*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+x+j/2), k, color);	
                        else if(j%2 == 1)
                            gen_setbit(*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+x+j/2), k+4, color);	
                    }
                }
                x += 4;
            }
        }
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Send_LCDBuffer_ToHS();

		return;
	}
#ifdef PORTING_B0_WAP
	if(ui.ani_option[ANI_TYPE_NTOP].Option == ANI_OPTION_IMG_SKY
	|| ui.ani_option[ANI_TYPE_NTOP].Option == ANI_OPTION_IMG_SPEED011
	|| ui.ani_option[ANI_TYPE_NTOP].Option == ANI_OPTION_IMG_TTL
	|| ui.ani_option[ANI_TYPE_NTOP].Option == ANI_OPTION_IMG_NTOP2)
	{
		//정지 이미지
		g_bExistConntAnimImage = FALSE;//need to draw bar
	}
	else
	{
		//애니매이션 이미지	(사용자 지정은 정지 이미지 포함됨)
		g_bDrawBar = TRUE;
		g_bExistConntAnimImage = TRUE;//no need to draw bar
	}

	uiscrn_SetPageTypeEX(PAGE_CURRENT, WT_NTOP, WMT_CURRENT, DMETHOD_BACKGROUND);
#endif //PORTING_B0_WAP
}


void process_wap_push_mail(void)
{
	if(Wtlshello)//wtlshello일 경우에만, close session을 시도...
	{
		if(wtls_match ==1)
		{
			MSG_ERROR("Perform clear WTLS",0,0,0);
			close_wtls_not_air();//key가없음 
		}
	}
    Wtlshello = FALSE;wtls_enc_start = FALSE;	
	
	if(load_data)
	{
		if(!Wtlshello)
		{
			SKY_stop(g_pCard->viewId);
#ifdef CONNECT_SESSION
			close_udp_port();
#endif//#ifdef CONNECT_SESSION
			CLNTa_log(1,0,"rrrr");
		}
	}
#ifdef PORTING_B0_WAP
	process_rcv_sms();
	ui_waptimer_set(0,FALSE);
#endif //PORTING_B0_WAP
}        

#ifdef BROWSER_UPDATE
void close_udp_port(void)
{
	if(bchoose)
	{
		CLNTa_log(1,0," Close port %d ",SAport);

		if (( wap_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
		{
			wap_ptr->hdr.task_ptr   = NULL;
			wap_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
		}
		else {
			ERR_FATAL( "close_udp_port: Free queue is EMPTY!", 0, 0, 0 );
		}
		wap_ptr->hdr.cmd = CLNT_CLOSE_PORT;
		wap_ptr->close_port.port = (UINT16)SAport;
		send_cmd_other2wap(wap_ptr);
		bchoose = FALSE;
	}
#ifdef WAP_USE_PZID2//살펴볼것...
    if(SKY_IS_WAP_PZID()) enable_reorigin();
#endif//#ifdef WAP_USE_PZID2
}
#endif//#ifdef BROWSER_UPDATE


#ifdef REDIRECT
void redirect_terminate(void)
{
#ifdef FEATURE_MUSIC_BELL
    musicbell_check();
#endif //#ifdef FEATURE_MUSIC_BELL
    entertain = FALSE;newurl_input = FALSE;
	if(load_data)
	{
		if(!Wtlshello)
		{
			SKY_stop(g_pCard->viewId);
#ifdef CONNECT_SESSION
			close_udp_port();			
#endif//#ifdef CONNECT_SESSION
		}
	}
#ifdef WAP_USE_SECURE
    if(SaveWtls[0]!=0)	memset(SaveWtls,0x00,1000);		
#endif //WAP_USE_SECURE

	close_wtls_not_air();CLNTa_log(1,0,"storewml--14");
	go_storeWml(TRUE);
	
	terminate_timer();
	resetalldata();
	WAP_Terminate_WML();
	TerminateVar();
}

/*======================================================
				SAVE_REDIRECTURL()

		1. Permanent일 경우에는 NV에 저장
		2. 공통적으로 proxy_url이 copy한다.
 ======================================================*/
BOOL save_redirecturl(void)
{
	extern char proxy_url[130];

	if(redirect.Address == NULL)
	{
		return FALSE;
	}
	memset(proxy_url,'\x00',130);
	if(redirect.Addresslen > 130) return FALSE;
	else
		memcpy(proxy_url,redirect.Address,redirect.Addresslen);


	if(redirect.bPerm_or_not)
	{		 
		 SKY_SET_WAP_PROXY_URL(proxy_url); //(void)ui_put_nv( NV_PROXY_I, ( nv_item_type * )proxy_url);
	}	
	CLNTa_log(1,0,"proxy_url[%s] ,addresslen=%d,redirect url [%s]",proxy_url,redirect.Addresslen,redirect.Address);

	if(redirect.Address !=NULL)
	{
		wip_free(redirect.Address);
		redirect.Address = NULL;
	}

	return TRUE;
}

void start_redirect(void)
{
	extern boolean isErrorState;
	extern int first_flag_tid;

	isErrorState = FALSE;
	bReceiveData = FALSE;
	first_flag_tid  = 0;

	retry=0; dnsaccess=TRUE; initialaccess=TRUE;
    browser_start();	
}
#endif//#ifdef REDIRECT
#ifdef WAP_USE_DORM
void wap_set_dorm_mode(wap_dorm_type wap_dorm)
{
	CLNTa_log(1,0,"dorm mode setting %d",wap_dorm);
	INTLOCK();	
	wap_dorm_mode = wap_dorm;  
    INTFREE();
}

int  wap_reorigination(void)
{
  db_items_value_type dbi1;   

  if (wap_flow_cont == TRUE)
  {
#ifdef FOR_MS
     if(Wtlshello)
	 {
		 close_wtls_not_air();
		 bStopKey = FALSE;
		 CLNTa_log(1,0,"CLEAR WTLS when reorigination");
	 }
	 else
#endif//#ifdef FOR_MS
     {
         CLNTa_log(1,0,"stop막음");		
         //SKY_stop(1);
     }

	 CLNTa_log(1,0,"WAP Flow Control!!! when origination");
 	 return -1; 
  }

  if (wap_dorm_mode == WAP_MODE_REORIG)
  {
     dsmgr_cancel_timer(DS_STATE_TIMER_SIG);
	 
#ifdef FOR_MS
     if(Wtlshello)
	 {
		 CLNTa_log(1,0,"Wtlshello TRUE");	
		 bStopKey = FALSE;
	 }
	 else
#endif//#ifdef FOR_MS
	 {
		//CLNTa_log(1,0,"Wtlshello FALSE");
        //CLNTa_log(1,0,"Stop막음--2");
        //SKY_stop(1);
	 }
#ifdef BLOCK
	 db_get (DB_NO_SVC, &dbi1 );     
	 CLNTa_log(1,0,"dbi1.no_svc[%d]in reorig",dbi1.no_svc);
	 if(dbi1.no_svc)
#endif //BLOCK
	 CLNTa_log(1,0,"SKY_IS_NOSERVICE[%d]in reorig",SKY_IS_NOSERVICE());
	 if(SKY_IS_NOSERVICE())
	 {
		 wap_dorm_mode = WAP_MODE_REORIG;
		 CLNTa_log(1,0,"WAP to be able to reorign");    
		 return 1;
	 }

	/*----------------------------------------------------------------------
       Set mode to Transient mode so we don't re-originate again 
     ----------------------------------------------------------------------*/
     wap_set_dorm_mode(WAP_MODE_TRANSIENT);

     /*----------------------------------------------------------------------
       Send command to dsmgr to bring up PPP and the traffic channel.
     ----------------------------------------------------------------------*/
	 dbi.in_use = FALSE;
	 db_put( DB_IN_USE, &dbi );					

     CLNTa_log(1,0,"Sending DS_REORIG_WAP_CMD");
     ui_ds_buf.cmd_name = DS_REORIG_WAP_CMD;
     ds_cmd(&ui_ds_buf);
  }
  else if ( wap_dorm_mode == WAP_MODE_TRANSIENT)
  {
	 db_get (DB_NO_SVC, &dbi1 );     
	 if(dbi1.no_svc)
	 {
		 wap_dorm_mode = WAP_MODE_REORIG;
		 CLNTa_log(1,0,"WAP to be able to reorign");    
		 return 1;
	 }
	 else
		CLNTa_log(1,0,"WAP in processing reorigination");    

	 return 0;
  }
  return 0;
}

#ifdef WAP_USE_PZID2
void wap_change_pzid(void)
{
	ds_cmd_type ds_buf;	
	ds_buf.cmd_name = DS_PZID_WAP_CMD;
    ds_cmd(&ds_buf);
}
#endif//#ifdef WAP_USE_PZID

boolean wap_dorm_active(void)
{
	if(
		(wap_dorm_mode ==WAP_MODE_REORIG) || (wap_dorm_mode == WAP_MODE_TRANSIENT) || (wap_flow_cont == TRUE)
	  )
	{
		CLNTa_log(1,0," wap_dorm_mode %d  wap_flow_cont %d",wap_dorm_mode,wap_flow_cont);
		return(TRUE);
	}	
	else return(FALSE);
}

#ifdef PORTING_B0_WAP //-->to wap applet
LOCAL void Do_cancel(boolean send)
{	
    entertain = FALSE;newurl_input = FALSE;
    report_event(WAP_DRAW_STOP_EVENT);
	if(g_pCard != NULL)
	{
		new_state = HANDLE_KEY_S;				
        if(send) report_event(WAP_STOP_EVENT);
        else process_stop_event();
	}       
	else
	{
		ProcessBye();
	}	
}
#endif //PORTING_B0_WAP

void enable_reorigin(void)
{
	extern void dsi_set_callstate(dsi_callstate_enum_type new_callstate);
	wap_flow_cont = FALSE;
	wap_set_dorm_mode(WAP_MODE_REORIG);
	dsi_set_callstate(DS_IDLE_CSTATE);
}
#endif//#ifdef WAP_USE_DORM

// Not yet
#ifdef PORTING_B0_WAP
LOCAL void processExitOnly(void)
#else
void processExitOnly(void)
#endif //PORTING_B0_WAP
{
	makeagent = FALSE;TerminateVar();
	
#ifdef FEATURE_MUSIC_BELL
	musicbell_check();
#endif
	close_wtls_not_air();close_udp_port();
#ifdef WAP_USE_SECURE
	ClearWTLSdata();
#endif//#ifdef WAP_USE_SECURE

	if(SaveWtls[0]!=0) memset(SaveWtls,0x00,1000);
	resetalldata();	
}
#ifdef PORTING_B0_WAP
LOCAL void SendRequest(void)
#else
void SendRequest(void)
#endif //PORTING_B0_WAP
{
    CLNTa_log(1,0,"SendRequest again");
    receiveOK = TRUE;
    //if(push_acc == 1) SKY_loadURL(1, PushUrl, TRUE);								
	if(push_acc == 1)  SKY_loadURL(1, (CHAR*)g_pszPushUrl, TRUE);		   
	else if(push_acc == 2)	SKY_loadURL(1, EmailUrl, TRUE);
#if (defined WAP_B1) || (defined WAP_B2)//for b0m build tmp 
	else
		go_storeWml_normal(FALSE);
#endif //WAP_B1 
}
#ifdef PORTING_B0_WAP
LOCAL void SendNormalRequest(void)
{
	extern storeWML normal_use;
	receiveOK =TRUE;retry = 0;
	if(entertain) displayloading(11,TRUE);
	if(!ps_start)
	{
   	    browser_start();browserTerComp = FALSE; bCreateReq = TRUE;

		//if(push_acc == 1) SKY_loadURL(1, PushUrl,  TRUE);
		if(push_acc == 1)  SKY_loadURL(1, g_pszPushUrl, TRUE);		   
		else if(push_acc == 2)	SKY_loadURL(1, EmailUrl, TRUE);

		// ====================================================================
		// zeus(yjchoi) 00.12.13	EIF WAP(External Interface for WAP) UOPEN command opens URL directly!!
#ifdef EIF_WAP
		else if (After_WAP_start_what_call == 2)
		{
			extern void uopen_command(char*);
			After_WAP_start_what_call = 0;

			CLNTa_log(1, 0, "=== UOPEN function called [%s] ===", EIF_url);

			uopen_command(EIF_url);	
		}
#endif  

#ifdef FEATURE_NETGAME
		else if(ui.to_wap && svNetGameRetURL[0] ) SKY_loadURL(1, svNetGameRetURL, FALSE);					
#endif//netgame
		else if(redirect.bCorrect)
		{
		    if(setPrev)
			{
				setPrev = FALSE;SKY_loadURL(1, prevRedirect, FALSE);
			}
			else if(RedirectUrl[0] !=0x00)	SKY_loadURL(1, RedirectUrl, FALSE);
			else
            {  
                SKY_loadURL(1, temp_url, FALSE);
                normal_use.viewid  = 1;normal_use.id = 0;normal_use.wmlkind = 6;
             }
		}
        else
        {
            SKY_loadURL(1, temp_url, FALSE);
            normal_use.viewid  = 1;normal_use.id = 0;normal_use.wmlkind = 6;
        }
		ps_start = TRUE;
		if((push_acc!=1)&&(push_acc!=2))
		{
			if(!redirect.bCorrect)
			{
				firstsite = TRUE;PrevKey = UI_NONE_F;
			}
		}//if((push_acc!=1)&&(push_acc!=2))
	}//if(!ps_start)

	if(wtls_enc_start)	bStopKey  = FALSE;
}
#endif //PORTING_B0_WAP

LOCAL void SendExitDormReq(void)
{
#ifdef PORTING_B0_WAP
#ifdef WAP_USE_SKT_NEW
    bsetonlyone = TRUE;
	disponly = FALSE;
	dnsaccess = TRUE;
    new_state = ACQ_DNS_S;
    ui_dispmsgtimer_set(20000);
#else             
    ProcessBye();
#endif//#ifdef WAP_USE_SKT_NEW
#endif //PORTING_B0_WAP
}

#ifdef PORTING_B0_WAP
LOCAL void SendRelReq(void)
{
    if(SKY_IS_WAP_RELEASE())
    {           
		SKY_SET_WAP_RELEASE(FALSE);
		wap_rel_state  = REL_NULL;

        if(!wap_clk_key)
        {
            if(!wap_rcv_page) SendRequest();
            else wap_rcv_page  = FALSE;
        }
        else
        {
            wap_clk_key = FALSE;wap_rcv_page = FALSE;
            Do_cancel(FALSE);
        }
    }
}
#endif //PORTING_B0_WAP

#ifdef PORTING_B0_WAP			    
LOCAL void EnterDormMode(void)
{
    resetalldata();
    dbi.in_use = FALSE;
    db_put( DB_IN_USE, &dbi );								

    if(ui.call) ui.call = FALSE;
    if(ui.datacall)	ui.datacall = FALSE;
    uiscrn_ann( HS_AN_INUSE, 0 );
    if(state!=HANDLE_KEY_S) Do_cancel(TRUE);
}
#endif //PORTING_B0_WAP

void CheckCAIrev(void)
{
#ifdef WAP_USE_LEAK
    extern int isReStartComleted;
#endif //WAP_USE_LEAK
    if(g_pCard!=NULL && load_data)
	{
		CLNTa_log(1,0," wap release set to TRUE");
        SKY_SET_WAP_RELEASE(TRUE);
		wap_rel_state = REL_NULL;
		report_event(WAP_STOP_EVENT);
	}
#ifdef WAP_USE_LEAK
	else if(isReStartComleted == 3)
	{
		CLNTa_log(1,0,"OutMem시 release");
		SKY_SET_WAP_RELEASE(TRUE);
        wap_rel_state = REL_NULL;
	}
#endif//#ifdef WAP_USE_LEAK
}

#ifdef PORTING_B0_WAP
LOCAL void ClearRelVar(void)
{
    if(!makecall) ui.wap_release = FALSE;
	bsetonlyone = TRUE;disponly = FALSE;
}
#endif //PORTING_B0_WAP

#ifdef PORTING_B0_WAP
LOCAL void ProcessBye(void)
#else
void ProcessBye(void)
#endif //PORTING_B0_WAP
{
#ifdef WAP_USE_LEAK
    extern int isReStartComleted;
	isReStartComleted = 0;
#endif//#ifdef WAP_USE_LEAK

#ifdef WAP_USE_SECURE
	Portdetect = FALSE;
#endif//#ifdef WAP_USE_SECURE
	report_event(WAP_DRAW_STOP_EVENT);
	report_event(WAP_TEMINATE_EVENT);
#ifdef WAP_USE_DORM
	wap_duration = 0;
#endif //WAP_USE_DORM
	if(bReceiveRelease)  bReceiveRelease = FALSE;
//wap applet 에서 sibling한다. 
//	new_state = EXIT_S;
//	ui.event = UI_NONE_F;
}

void process_rcv_call(void)
{
#ifdef PORTING_B0_WAP
	isFirstWapToIdle = TRUE;
	WapTraffic_Term = TRUE;browserTerComp  = TRUE;
	ui.event = UI_DOALERT_F;rcvcall  = FALSE;
	ui.tog_flag    = TRUE;
	disponly = FALSE;
	displayterminate();
#endif //PORTING_B0_WAP
}

void process_makecall(void)
{
#ifdef PORTING_B0_WAP
	extern CHAR WTAnumBuf[15];
	extern int  WTAlength;

	ui.so = CAI_SO_NULL;ui.datacall = FALSE;
	ui.n.pos = MIN( WTAlength,NV_MAX_DIAL_DIGITS + 1);
	ui.n.secret = FALSE;ui.n.act = TRUE;
	ui.tog_flag = FALSE;ui.canlookup = TRUE;
	ui.ed.act = FALSE;isFirstWapToIdle = TRUE;
							
	memcpy(ui.n.buf,WTAnumBuf,WTAlength);
	displayterminate();
	ui.event = HS_SEND_K;
#endif //PORTING_B0_WAP
}

void process_after_release(void)
{
#ifdef PORTING_B0_WAP
	CLNTa_log(1,0,"RELEASE TO IDLEsssss");
	bReceiveRelease = FALSE;isFirstWapToIdle = TRUE;
	close_wtls_not_air();
	if(SaveWtls[0]!=0) memset(SaveWtls,0x00,1000);
	ui.clrnum_flag = FALSE;	ui.tog_flag    = TRUE;
	ui.event = UI_NONE_F;disponly = FALSE;
	displayterminate();
#endif //PORTING_B0_WAP
}

boolean CheckTraffic(void)
{
#ifdef PORTING_B0_WAP
  if(
		(!WapTraffic_Term && !bReceiveRelease) ||//여전히 Traffic일 경우..
		((dsi_callstate != DS_IDLE_CSTATE)&&(dsi_callstate != DS_END_CALL_CSTATE))
	)
	{
		CLNTa_log(1,0,"RELEASE TO IDLE1");
		disponly = TRUE;ui_dispmsgtimer_set(3000);
		wap_do_abnormal_exit2();
		return FALSE;
	}
	else
	{
		isFirstWapToIdle = TRUE;disponly = FALSE;
		displayterminate();		
		return TRUE;
	}
#endif //PORTING_B0_WAP
	return TRUE; //Dummy porting B0
}

#ifdef PORTING_B0_WAP
void process_rcv_sms(void)
{
	if(g_pCard != NULL)
	{
		CLNTa_log(1, 0, "이전 카드 있음");InWapMailorPush = 2;
	}
	else//카드가 없음 = 초기 URL 접속중이었다는 의미
	{
		CLNTa_log(1, 0, "이전 카드 없음 ");	InWapMailorPush = 1;
	}
	state = HANDLE_KEY_S;ret_state = UI_SMS_S;ui.event  = UI_SMS_MSG_F;					
	CLNTa_log(1,0,"GO SMS state");
}
#endif //PORTING_B0_WAP

void process_exit_event(void)
{
#ifdef PORTING_B0_WAP
 int nResultHandleKey = 0;

 switch(ui.event)
 {
	case UI_SMS_MSG_F:
	{
		if(ui.wap_release) process_rcv_sms();
		else ui.event = UI_NONE_F;
		break;
	}
	case UI_DOALERT_F:
	{
		Dormantrcvcall();
		break;
	}    
	case UI_DISPMSGTIMER_F:
    {
        ui_dispmsgtimer_set(0);	
		if(disponly)
		{
		   disponly = FALSE;ui_dispmsgtimer_set(3000);
		}
		CLNTa_log(1,0,"UI_DISPMSGTIMER_F");

		if(makecall)
		{
			ui.wap_release = FALSE;process_makecall();
			break;
		}

		if(rcvcall)//check해 볼것....
		{
		    process_rcv_call();
			break;
		}
		if(bReceiveRelease)
		{
			process_after_release();
            break;
        }
		(void)CheckTraffic();			
        break;
    }
    case UI_NETDISCONNECTED_F:
    {
		ui_dispmsgtimer_set(0);	
        CLNTa_log(1,0,"UI_NETDISCONNECTED_F");

		if(CheckTraffic() == FALSE) break;

		if(bReceiveRelease)
		{
			process_after_release();
			break;
		}
		if(makecall)
		{
			process_makecall();
			break;
		}

		isFirstWapToIdle = TRUE;displayterminate();
        break;
    }

	case HS_END_K:
	case HS_EAR_DOWN_K:
		if(!ui.wap_release)
		{
			if(WapTraffic_Term && browserTerComp) displayterminate();			
			ui.event = UI_NONE_F;
			break;
		}
    default:
    {
#ifdef WAP_USE_SKT_NEW
        if(ui.wap_release)
        {
            CLNTa_log(11,0,"Key processing in EXIT_S");
            nResultHandleKey = WAP_HandleKeys(ui.event);
		    Process_After_HandleKeys(nResultHandleKey); 
        }
#endif//#ifdef WAP_USE_SKT_NEW
        ui.event = UI_NONE_F;
        break;
    }
 }//End of switch
#endif //PORTING_B0_WAP
}

void process_hold_orig(void)
{
#ifndef QUALCOM30_WAP
	dsi_hold_orig_enabled = FALSE; 
#endif //#ifndef QUALCOM30_WAP
	dsmgr_cancel_timer(DS_STATE_TIMER_SIG);
    CLNTa_log(1,0," cancel timer in exit_s");
}

void CheckBrowerDown(void)
{
	CLNTa_log(1,0,"여기를 탄다.");
#ifdef PORTING_B0_WAP
	if(ui.event == UI_DISPMSGTIMER_F)
	{
		CLNTa_log(1,0,"여기를 탄다.-2");
		ui_dispmsgtimer_set(0);
		browserTerComp = TRUE;
		ui_dispmsgtimer_set(3000);
	}

	else if(ui.event == UI_NETDISCONNECTED_F)
    {
        save_event = UI_NETDISCONNECTED_F;
        CLNTa_log(1,0,"REceive NETDISCONNETED_F");
    }
#endif //PORTING_B0_WAP
}						

void ShowTheBar(void)
{
	if(entertain) displayloading(11,TRUE);
	else if(Wtlshello) displayloading(2,TRUE);				
	else if(redirect.bCorrect) displayloading(12,TRUE);
	else displayloading(1,TRUE);		
}		

void ReturtoErrorState(void)
{
#ifdef PORTING_B0_WAP
	new_state  = ERROR_S;isFailOrig = TRUE;ui.event   = UI_NONE_F;			
#endif //PORTING_B0_WAP
}

void processWtlsResult(void)
{
#ifndef WAP_USE_SECURE
	if(wtls_match == 1)
	{
		CLNTa_log(1,0,"UINSWAP wtls_match");
		close_wtls_not_air();new_state = WTLS_SESSION_START_S;receiveOK = TRUE;			
	}
	else if(wtls_match == 2)
	{
		if(!bSetKey)
		{
			CLNTa_log(1,0,"UINSWAP wtls_match2");
			close_wtls_not_air();new_state = WTLS_SESSION_START_S;
		}
		else
		{
			wtls_enc_start = TRUE;Wtlshello      = FALSE;
			clear_ssls();
			CLNTa_log(1,0,"storewml--10");
			go_storeWml(FALSE);new_state = LOADING_S;

            if(wtls_enc_start) bStopKey = FALSE;
		}
	}
#endif//#ifndef WAP_USE_SECURE
}

void Dormantdispterminate(void)
{
#ifdef PORTING_B0_WAP
	dbi.in_use = FALSE;db_put( DB_IN_USE, &dbi );
	ui.call = FALSE;ui.datacall = FALSE;
	uiscrn_ann( HS_AN_INUSE, 0 );
    //ret_state = UI_POP_S;
	ret_state = UI_CALL_S;
#endif //PORTING_B0_WAP
}

void Dormantrcvcall(void)
{
#ifdef PORTING_B0_WAP
	entertain = FALSE;newurl_input = FALSE;
	CLNTa_log(1,0,"receive alert --Handlekey");

	terminate_timer();	

	//Traffic 일시는 않오므로 이렇게 함
	isFirstWapToIdle = TRUE;
	ui.tog_flag  = TRUE;
	Dormantdispterminate();

	save_min_state = state;
	state    = WAP_RET_CALL_S;
	ui.event = UI_DOALERT_F;					
#endif //PORTING_B0_WAP
}

void nosvc_indicator(void)
{
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "서비스가지원",WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "되지않습니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();	
}

//PORTING_B0_WAP uicommon -->옮김 
byte get_length( byte *chk_str, byte buff_length)
{
	byte i;
	byte ret_length = 0;

	for(i=0; i < buff_length ; i++ ) {
		/* rosa */
		if( *chk_str == '\0' )
		{
			return( ret_length );
		}
		/* <-- */
		if( (*chk_str != ' ') && *chk_str )
			ret_length = i + 1;
		chk_str++;
	}

	return( ret_length );
}

//////////////////////////////
// Terminal library
//////////////////////////////
int get_position(char *result) {
	db_items_value_type dbi;
	char tmp_buf[20];
	int  length = 0;

	*result = '\0';
	tmp_buf[0] = '\0';

	db_get(DB_NID,&dbi);
	sprintf(tmp_buf,";%d",dbi.nid);
	strcat(result, tmp_buf);
	length += strlen(tmp_buf);
	tmp_buf[0] = '\0';

	db_get(DB_REG_ZONE,&dbi);
	sprintf(tmp_buf,";%d",dbi.reg_zone);
	strcat(result, tmp_buf);
	length += strlen(tmp_buf);
	tmp_buf[0] = '\0';

	db_get(DB_BASE_ID,&dbi);
	sprintf(tmp_buf,";%d",dbi.base_id);

	strcat(result, tmp_buf);
	length += strlen(tmp_buf);
	tmp_buf[0] = '\0';

	db_get (DB_PILOT_PN_OFF, &dbi );
	sprintf(tmp_buf,";%d",dbi.pilot_pn_off);
	strcat(result, tmp_buf);
	length += strlen(tmp_buf);
	
	return length;
} 
//PORTING_B0_WAP

