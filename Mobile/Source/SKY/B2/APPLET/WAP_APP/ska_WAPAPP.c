#ifdef FEATURE_WAP
/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_WAPAPP.c
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
** 2001-07-16   ytchoi     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"
#include "sky_sock.h"
#include "dsi.h"
#include "psglobal.h"
#include "wapcommon.h"
#include "mmi_func.h"
#include "tapimmi.h"
#include "clnt.h"
//For mobile commerce
#include "crypto_script.h"
#include "mwscript.h"

#ifdef FEATURE_WAP_MUXOPTION
#include"mc.h"
#endif //#ifdef FEATURE_WAP_MUXOPTION
#ifdef  FEATURE_SKT_EIF //yjahn96 01.02.24 --- eif for wap
#include "eifwap.h"
#endif  //#ifdef  FEATURE_SKT_EIF

#include "capimmi.h"






/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define NV_PROXY  130
#define NV_URL    130
#define NV_INITEMAIL 130

#ifdef FEATURE_MCOMMERCE
	#define REQUEST_CERT_CMD 1
	#define REVOKE_CERT_CMD	2
#endif //#ifdef FEATURE_MCOMMERCE

#define FEATURE_MUXOPTION_NOTI

//#define VASTEST

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/


ROM EDIT_MODE_SELECT_T  DialogMode = {
    4,     0,
    { TEM_ENGLISH_LOWER, TEM_ENGLISH_UPPER, TEM_NUMBER, TEM_KOREAN },
	TRUE
};

#ifdef FEATURE_WAPMAIL
enum tag_WAPMAIL_E {
    WEM_TEXT_TITLE,
    WEM_TEXT,    
    WEM_MAX
};
#endif //FEATURE_WAPMAIL
enum tagNTOP_MENU_LIST_E{
	NML_NTOP,
	NML_GVM,
#ifdef FEATURE_SKVM
	NML_ENV,
#endif
#ifdef FEATURE_WAVELET
	NML_WAV,
#endif //FEATURE_WAVELET
#ifdef FEATURE_CHARACTER_BELL
    NML_CHARACBELL,
#endif // FEATURE_CHARACTER_BELL
    NML_MAX
};

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

BOOL g_bWAPinitAccessFail = FALSE;

HCONTROL hNTopAnim = INVALID_CONTROL_HANDLE;	//초기 animation stop위해

#ifdef FEATURE_WAP_REDIRECT
redirect_config redirect;
CHAR gWAPRedirectUrl[1500];
BOOL gredirectloadingbar = FALSE;
#endif //#ifdef FEATURE_WAP_REDIRECT

CHAR SaveSelfMin[8];
CHAR  gWAPinit_url[130];
CHAR  gWAPproxy_url[130];
CHAR  gWAPEmail_Url[130];
CHAR  temp_url[200];
UCHAR g_pszPushUrl[URL_MAX];
int InWapMailorPush = 0;

CHAR proxy_url[130];
CHAR EmailUrl[100];
BOOL load_incard = FALSE;
#ifdef FEATURE_WAPMAIL
WAP_MAIL_TYPE g_wapmaildata;
#endif //FEATURE_WAPMAIL
UCHAR g_szGoToURL[MAX_GOTOURL_LEN];
int g_reloaderrorTime;
UINT8 ui_errortype =0;

int g_errorNumber;
int g_errorType;

#ifdef FEATURE_WAP_WAVELET
BOOL fgotoWavelet = FALSE;
int g_wcpid;
int g_wctsid;
char g_wsndIP[101] = {0};
int g_wsndPort;
int g_wMO;
int g_wSO;
int g_wsvcType;
int g_wskinType;
char g_wcntName[21] = {0};
int g_wcntProtect;
int g_wcntExpDate;
char g_wretURL[101] = {0};
STATIC BYTE s_WAVELETsaveType = NEW_CONTENTS;
extern BYTE gDownInfo;
#endif //FEATURE_WAP_WAVELET

#ifdef INCOMING_CALL_IN_WAP
BOOL g_fWAP_RET_CALL = FALSE; // incoming call관련해서 app_start 를 무시하고 resume_event를 처리하기 위해서 !!!
#endif //INCOMING_CALL_IN_WAP
BOOL g_fWAP_RET_SMS = FALSE;

#ifdef FEATURE_WAP_MUXOPTION
BOOL fMUX_Option = FALSE;
extern boolean  in_traffic;
#endif //#ifdef FEATURE_WAP_MUXOPTION
BOOL g_WAPfirstDisplay = 0;
INT g_KindofPUSH = 0;

#ifdef FEATURE_SKT_EIF//yjahn96 01.08.01 - for sending 'EXIT' to TE2
//keyemul, uopen으로 접속후, 일정시간 입력없이 wap종료될때
boolean g_EIF_SEND_EXIT = FALSE;    //keyenul, uopen으로 접속여부
static  boolean g_EIFWAPExit = FALSE;  //g_EIF_SEND_EXIT에 따라 TRUE하며, 실제 EXIT보내는 여부 판단

int ping_cnt = 0;
static int PongCnt = 0;
boolean continue_ping = FALSE;				// ping_cnt 가 255 들어오면 TRUE로 만들어 continuous ping 실시 
static boolean pinging = TRUE;
#endif//#ifdef FEATURE_SKT_EIF

/*
**---------------------------------------------------------------------------
**  External variables
**---------------------------------------------------------------------------
*/
extern unsigned long destinationIP;
extern q_type           wap_cmd_q;      /* WAP command queue              */
extern q_type           wap_cmd_free_q; /* WAP command free queue         */
extern SOCKET g_fd;
extern CHAR WTAnumBuf[15];
extern int  WTAlength;
extern unsigned int WAEStatus ;
extern HCONTROL hContentsDownloadControl;
extern UINT8 g_nAniBarCount;
extern BOOL g_bDrawBar;
extern int	first_flag_tid ;

#ifdef WAPDORMANT
extern boolean gbDormant  ;             /* Set for call drop in 95C */
extern boolean gb2GHandoff;             /* Set for 2.5G->2G H/O */
#endif//wapdormant
#ifdef FEATURE_SKIPDATA
extern boolean g_bSkipData;
#endif//feature_skipdata

#ifdef  FEATURE_SKT_EIF //yjahn96 01.10.08 - eif for wap
extern INT  g_nEIFWapstatus;
extern BOOL g_EIF_WAP_WAPCON;
extern char selectBuf[16];
extern char resultBuf[4096];

extern dword ping_RTT;
extern dword ping_MIN;
extern dword ping_MAX;

extern dword ping_seq;
extern dword ping_timeStamp;

extern unsigned char PingCnt;
extern int nTimeout;
extern char EIF_url[192];
extern char Dest_IP[4];
extern ip4a Dest_url_IP;
extern struct icmp * eif_icmp;
#endif  //#ifdef  FEATURE_SKT_EIF

#ifdef FEATURE_CHARACTER_BELL
extern boolean g_fCharacbellPreview;
#endif // FEATURE_CHARACTER_BELL

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
static s_initWAPACCESSFail = FALSE;
static  int  s_plength = 0 ; // nv의 URL/GW/MAIL 값의 valid check
LOCAL BITMAP l_BmpBuf = {
    { EQUS_IMAGE_ID, { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT}, 
        SKY_SCREEN_WIDTH, BIT_PER_PIXEL_256C, FALSE, MAX_COLOR, 0 },
    g_abBmpBuf
};
static wap_cmd_type*       wap_ptr;
wapexit_enum_type s_towhere;
wapminor_enum_type s_towhereminor;

#ifdef FEATURE_SKT_EIF
BOOL g_WAPSuccess = 0;
#endif	//#ifdef FEATURE_SKT_EIF
/*
**---------------------------------------------------------------------------
**  Function(External use only) Declarations
**---------------------------------------------------------------------------
*/
extern void wap_end_cmd(void);
extern void wap_abnormal_end_cmd(void);
extern void wap_exit_cmd(void);
extern void wap_retry_cmd(void);
#ifdef WAPDORMANT
extern void wap_2G_setup_connection_cmd(void);
#endif//wapdormant
extern BOOL WAP_Terminate_WML_Test(void);
extern void wap_set_error(boolean isError);

#ifdef FEATURE_WAP_REDIRECT
extern void Redirection_dnsquery(void);
extern void init_buffer(redirect_config* buf);
#endif //#ifdef FEATURE_WAP_REDIRECT

extern void musicbell_check(void);

extern void WAP_DrawLoadingBarToBackBuff(int nAniTextMode, BOOL bAniBarDirection);

#ifdef FEATURE_SKT_EIF	//yjahn96 01.03.15
extern void at_send_extended_text(char*);
extern void select_command(char*);
extern void uping_command(int, ip4a);
extern int uping_reply_command(struct icmp *);
extern int atoi( const char *string );      
extern int NKey_Uopen_Loading(int KeyCode);
extern int LinkText_Uopen_Loading(int textID);
#endif	//#ifdef FEATURE_SKT_EIF

#ifdef VASTEST
extern VOID SKY_API SKA_MainWAP0(SKY_EVENT_T *pEvent);
#endif//vastest

/*
**---------------------------------------------------------------------------
**  Function(Internal use only) Declarations
**---------------------------------------------------------------------------
*/

LOCAL VOID SKY_API sSKA_WAPERROR(SKY_EVENT_T *pEvent) ;//redirect, dormant처리 !!
LOCAL VOID SKY_API sSKA_WAPINPUT(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_WAPNewURL(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_WAPEXIT(SKY_EVENT_T *pEvent);

#ifdef FEATURE_WAP_WAVELET
LOCAL VOID SKY_API sSKA_WAVELETMSG_INWAP(SKY_EVENT_T *pEvent) ;
#endif //#ifdef FEATURE_WAP_WAVELET

LOCAL void SKY_API sSKA_WAPDIALOG_PASSWORD(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_WAPDIALOG_ID(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_WAPDIALOG_PROMPT(SKY_EVENT_T *pEvent);

LOCAL void MakeMin_UI(void);
#ifdef FEATURE_WAPMAIL
LOCAL BOOL ReadWAPMail(void);
LOCAL VOID OverWriteWAPMail(void);
#endif //FEATURE_WAPMAIL
VOID PlayNTopMainWapImage(void);
LOCAL void send_wapcmd(wap_name_type cmd_name);
LOCAL void WAP_Initialize_MEM(void);
#ifdef FEATURE_WAP_WAVELET
void wap_display_exit_for_wavelet(void);		
#endif//FEATURE_WAP_WAVELET

LOCAL VOID process_rcv_sms(void);
LOCAL VOID Cancell_WAPLoading(void);
void WAPUsedTime(char UsedTimeType);
LOCAL VOID InitalizeWAPvariable(void);

VOID SKY_API SKA_ChooseMenu(SKY_EVENT_T *pEvent)
{	
	UINT8  unIdx;
	static BYTE* pabNTOPMenu[NML_MAX];
    STATIC HCONTROL           hCtrlNoSrvc;
    STATIC BOOL  fFromChooseMenuCode;    

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y,
                        SKY_DEF_WIN_DX,
                        SKY_DEF_WIN_DY, 
                        NULL, 
                        SKY_GET_TOKEN(TKN_GVM_NTOP_MENU_TITLE), 
                        WA_SHOW_CML | WA_SHOW_HEADER);

		pabNTOPMenu[NML_NTOP] = (BYTE*)SKY_GET_TOKEN(TKN_GVM_NTOP_MENU_CONNECT);
#ifdef FEATURE_SKVM
		pabNTOPMenu[NML_GVM] = (BYTE*)SKY_GET_TOKEN(TKN_SKVM_DOWNLOAD_SAVE);
		pabNTOPMenu[NML_ENV] = (BYTE*)SKY_GET_TOKEN(TKN_SKVM_INPUT_ENV);
#else
		pabNTOPMenu[NML_GVM] = (BYTE*)SKY_GET_TOKEN(TKN_GVM_NTOP_MENU_GVM);
#endif

#ifdef FEATURE_WAVELET
		pabNTOPMenu[NML_WAV] = (BYTE*)SKY_GET_TOKEN(TKN_WAVELET_NTOP_FILE_MENU);
#endif //FEATURE_WAVELET

#ifdef FEATURE_CHARACTER_BELL
        pabNTOPMenu[NML_CHARACBELL] = (BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_MENU);
#endif // FEATURE_CHARACTER_BELL

		EQC_ScrollList(EQS_NA, 
                       EQS_NA, 
                       EQS_NA, 
                       EQC_GetScrollDisplayLine(), 
                       pabNTOPMenu, 
                       NML_MAX, 
                       NML_MAX, 
                       0);
        EQC_SET_STYLE(EQS_GetNthIOC(0), EQC_GET_STYLE(EQS_GetNthIOC(0)) |
                                        CS_SCROLL_LIST_MAXIMIZE |
                                        CS_SCROLL_LIST_ENUMBERATED);
        EQS_SetIOCActive(EQS_GetNthIOC(0));

        if(pEvent->wParam == NATE_CODE_CHOOSE_MENU)
        {
            fFromChooseMenuCode = TRUE;
        }
        else
        {
            fFromChooseMenuCode = FALSE;
        }
	    break;

    case SE_APP_END:
		break;

    case SE_APP_RESUME:
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
		if(unIdx == NML_NTOP)
		{
			if((SKY_IS_NOSERVICE() == FALSE) && (SKY_IS_MAINTREQ() == FALSE))
			{
			  if(SKY_IS_WAP_LOCK() && (fFromChooseMenuCode == FALSE))
				  EQS_START_SIBLING(SKA_MainNtopCode, FALSE);      //white 2001.02.26
			  else
#ifdef VASTEST
				  EQS_START_SIBLING( SKA_MainWAP0, FALSE );
#else
				  EQS_START_SIBLING( SKA_MainWAP, FALSE );        //잠금 기능때문에 SKA_MainWAP를 한번 타고 WAP으로 들어간다.
#endif//vastest
			}
			else if(SKY_IS_MAINTREQ())
			{    
			  hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
										 MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);          
			}
			else if(SKY_IS_NOSERVICE())
			{			 
			  hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
										 MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
			}     
		}
		else if(unIdx == NML_GVM)
		{
#ifdef FEATURE_SKVM
			EQS_SetDirectDraw(TRUE);
            EQS_START_SIBLING(SKA_MainSKVM,FALSE); 
#else
            EQS_START_CHILD(SKA_MainGVM); 
#endif
		}
#ifdef FEATURE_WAVELET
		else if (unIdx == NML_WAV)
		{
            EQS_START_CHILD(SKA_IdleWavelet); 
		}
#endif //FEATURE_WAVELET

#ifdef FEATURE_CHARACTER_BELL
        else if (unIdx == NML_CHARACBELL)
		{
            EQS_START_CHILD(SKA_MainCharacBell); 
		}
#endif // FEATURE_CHARACTER_BELL
        
#ifdef FEATURE_SKVM // for SKVM color selection by tortol 01.09.13
		else if(unIdx == NML_ENV)
		{
            EQS_START_CHILD(SKA_SKVMInputEnv); 
		}
#endif // SKVM end


		break;

	case SE_IO_MESGBOX_END:   
		EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainWAP()
**
** Description: 
**     idle상태에서 WAP key event시 처음invoke되는 함수
**     1. wap에서 쓰는 변수들 초기화/nv값 Read
**     2. n.Top 초기화면/ 접속바 진행
**     3. WAP 종료시에 최종적으로 불리는 main 함수 
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

VOID SKY_API SKA_MainWAP(SKY_EVENT_T *pEvent)
{	
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        MSG_HIGH("WAP Applet시작",0,0,0);         
		WAPUsedTime(INIT_WAPTIME);
        in_traffic = FALSE;
		InitalizeWAPvariable();
        
        if(pEvent->dwParam == INITURL_IS_WAPPUSH)
		{
			g_KindofPUSH = INITURL_IS_WAPPUSH;
		}
		else if(pEvent->dwParam == INITURL_IS_WAPMAIL)
		{
			g_KindofPUSH = INITURL_IS_WAPMAIL;
		}
        EQS_SetDirectDraw(TRUE);
        EQS_GetDC(TRUE);
		SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());		
		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators. 
            WA_SHOW_HEADER);  
        // for shared RAM 
   		if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
			//SKY_SET_USE_SHARED_MEMORY(FALSE);
			g_fIrDAPortForPC = FALSE;
		}

		if(g_bBlocked) g_bBlocked = FALSE;		//yjahn96 01.11.09        
		//yjahn96 01.11.07 - for 깜박임문제
		PlayNTopMainWapImage();        
        displayloading(0);
		process_draw_bar();//WAP_DrawLoadingBarToBackBuff(WAP_ANI_TEXT_CONNECTING, WAP_ANI_BAR_FROM_LEFT);//yjahn96 01.11.07 - for bar
        WAP_Initialize_MEM(); 
        // 혹시 다른 applet에서 살아있을 general timer를 죽이기위해 !!
        //SKY_SetGeneralTimer(0, FALSE);
        break;
        
    case SE_APP_DRAW:
		s_plength = get_length((byte *)gWAPproxy_url,130);		
		if(s_plength >= 130)
        {
			EQS_SendEvent((EQS_EVENT_ID)SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
            break;
		}

		//second, get initial url
        s_plength = get_length((byte *)gWAPinit_url,130);
        if(s_plength >= 130)
		{
			EQS_SendEvent((EQS_EVENT_ID)SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
            break;			
		}
		//third, get Email url	
		s_plength = get_length((byte *)gWAPEmail_Url,130);		
		if(s_plength >= 130)		
		{
			EQS_SendEvent((EQS_EVENT_ID)SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
            break;			
		}
        //send_wapcmd(WAP_BROWSER_START);         
        //wap_enter_cmd();
        EQS_SendEvent((EQS_EVENT_ID)SE_USER_1, EQS_NA, EQS_NA);//EQS_START_CHILD(SKA_WAPHANDLE_KEY);    //yjahn96 01.11.07 - for 깜박임문제
		break;

	case SE_USER_1:				//yjahn96 01.11.07 - for 깜박임문제
		wap_enter_cmd();
        EQS_START_CHILD(SKA_WAPHANDLE_KEY);
		break;

    case SE_WAP_REPORTERROR:
		if( hNTopAnim != INVALID_CONTROL_HANDLE  )
		{
			//EQC_StopAnimation(hNTopAnim);	
			EQS_DeleteControl(hNTopAnim);
			hNTopAnim = INVALID_CONTROL_HANDLE;
		}
        s_towhere = WAPGO_INITERROR;
		EQS_START_CHILD(sSKA_WAPEXIT);
		break;

   case SE_APP_END :   /* The very last event */
		MSG_HIGH("WAP Applet종료",0,0,0); 
        init_buffer(&redirect);
        g_bWAPinitAccessFail = FALSE;
        start_disp = FALSE;
        in_traffic = FALSE;
        s_initWAPACCESSFail = FALSE;
	#ifdef FEATURE_SKIPDATA
		g_bSkipData = FALSE;
	#endif//feature_skipdata
#ifdef FEATURE_WAP_REDIRECT
        gredirectloadingbar = FALSE;
#endif //#ifdef FEATURE_WAP_REDIRECT

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        //WAP_Time_Stmap(FALSE);

        EQS_SetDirectDraw(FALSE);
        EQS_ReleaseDC();
        SKY_SET_IN_WAP(FALSE);       
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */   

        switch(s_towhere)
        {            
            case WAPGO_MAKECALL:
                /*not wap terminate !!*/
                break;

            case WAPGO_INCOMINGCALL:
                /*not wap terminate !!*/
                break;

            case WAPGO_SMS:
                /*not wap terminate !!*/
                break;
#ifdef FEATURE_GVM
            case WAPGO_GVM:
                EQS_START_SIBLING(SKA_MainGVM, FALSE);
                break;
#endif //#ifdef FEATURE_GVM
#ifdef FEATURE_SKVM
            case WAPGO_SKVM:            
				EQS_SetDirectDraw(TRUE);
				if (!SKVM_IS_PLAY_BG())
			        EQS_START_SIBLING_EVENT(SKA_MainSKVM, SE_APP_START,EQS_NA,INIT_SKVM_MSDURL ,FALSE);
				else EQS_START_SIBLING_EVENT(SKA_MainSKVMBG, SE_APP_START,EQS_NA,INIT_SKVM_MSDURL ,FALSE);
                break; 
#endif //#ifdef FEATURE_SKVM
#ifdef FEATURE_WAP_WAVELET
            case WAPGO_WAVELET:                
                if(g_wsvcType == 1|| g_wsvcType == 2)
                {
#ifdef FEATURE_WAVELET
                    EQS_START_SIBLING(SKA_WaveletStreaming, FALSE);
#else
					;
#endif //#ifdef FEATURE_WAVELET
                }
                else if(g_wsvcType == 3 || g_wsvcType == 4)
                {
#ifdef FEATURE_WAVELET
                    EQS_START_SIBLING(SKA_MainWavelet, FALSE);
#else
					;
#endif //#ifdef FEATURE_WAVELET

                }
                break;
#endif //#ifdef FEATURE_WAP_WAVELET

            default :
                MSG_HIGH("WAP종료!!--> IDLE로 ",0,0,0);
                EQS_END_TO(EQS_POP_TO_ROOT);
                break;
        }
        s_towhere = WAPGO_NONE;
        break;
/* KEY 	*/
	case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END :        
        //just break!!
        break;

    case SE_CALL_INCOMING:
        //just break!!
        break;
    case SE_SMS_MESSAGE:
        //just break!!
        break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_HIGH("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

	case SE_CALL_ENDED:
		MSG_HIGH("SE_CALL_ENDED",0,0,0);
		break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
	
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_WAPHANDLE_KEY()
**
** Description: 
**    일반적으로 WAP 상태에서 User와 communication하는 상태
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

VOID SKY_API SKA_WAPHANDLE_KEY(SKY_EVENT_T *pEvent)
{
	STATIC  TELESEVICE_KIND_TYPE    s_TeleIdType;
	STATIC  BOOL fDraw = FALSE;

#ifdef FEATURE_MUXOPTION_NOTI
    STATIC  HCONTROL hMSGControl;
#endif//#ifdef FEATURE_MUXOPTION_NOTI
    
#ifdef FEATURE_MCOMMERCE
	ENUM_SCRIPT_E	script;
	int				cert;
	int				errorMMI = MMI_NO_ERROR;
#endif //#ifdef FEATURE_MCOMMERCE
    STATIC  BOOL fwaveletMUX = FALSE;

    if((SE_KEY_0 <= pEvent->EventID && pEvent->EventID <=SE_KEY_F2)||
        pEvent->EventID == SE_KEY_RECORD||pEvent->EventID == SE_KEY_AUTO_ANSWER||
        pEvent->EventID == SE_KEY_SEND || pEvent->EventID == SE_WAP_SHOWCARD ||
        pEvent->EventID == SE_WAP_IN_PIM )
        
    {  
	    EQS_StopTimer(EQS_TIMER_1);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);        
    }

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(TRUE);		  
		EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators. 
            WA_SHOW_HEADER);      
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);
        if(g_fWAP_RET_CALL || g_fWAP_RET_SMS)
        {
            g_fWAP_RET_CALL = FALSE;
            g_fWAP_RET_SMS = FALSE;
        }

		if(EQS_GET_PARENT() == SKA_MainWAP )
		{
			EQC_StopAnimation(hNTopAnim);
			if(hNTopAnim != INVALID_CONTROL_HANDLE)
			{
				PlayNTopMainWapImage();
				process_draw_bar();//WAP_DrawLoadingBarToBackBuff(WAP_ANI_TEXT_CONNECTING, WAP_ANI_BAR_FROM_LEFT);			//yjahn96 01.11.07 - for bar			
			}
		}
        break;

    case SE_TM_TIMER1: // 5분 timer auto종료 !!
        MSG_HIGH("5분 timer종료!!",0,0,0);        
#ifdef FEATURE_SKT_EIF
        if(g_EIFWAPExit)
        {
            g_EIFWAPExit = FALSE;
            at_send_extended_text("EXIT");
        }
#endif  //#ifdef FEATURE_SKT_EIF

		//------------------------------------------
		// sSKA_WAPEXIT()로 이동하기 전에 처리
        //Error후에 animation이 play되는 현상 수정
		if( hNTopAnim != INVALID_CONTROL_HANDLE  )
		{
			//EQC_StopAnimation(hNTopAnim);		
			EQS_DeleteControl(hNTopAnim);
			hNTopAnim = INVALID_CONTROL_HANDLE;
		}

		// contents 저장후 msg box 없앰. key event 발생 허용
		if( hContentsDownloadControl != INVALID_CONTROL_HANDLE  )
		{
			EQS_DeleteControl(hContentsDownloadControl);
			hContentsDownloadControl = INVALID_CONTROL_HANDLE;
		}

	    EQS_START_SIBLING(sSKA_WAPEXIT,FALSE);
        break;

    //bar ani
    // 다른 applet의 영향을 받을 수 있다 다른 applet의 general timer kill을 확인한다.
    case SE_TM_TIMER:
        (void) SKY_Set_Signal( &ui_tcb, BAR_ANI_SIG );
        //process_draw_bar();
        break;
		
#ifdef WAPDORMANT
	case SE_TM_TIMER2:
		{
			MSG_HIGH("!!!! TIMER2",0,0,0);
			wap_2G_setup_connection_cmd();
			break;
		}
#endif//wapdormant

    case SE_APP_END :   /* The very last event */ 		
        EQS_ReleaseDC();
        break;

	//yjahn96 01.10.24 - low batter시 화면 update안되는 문제 수정
    case SE_IO_EXMSG_END:	
        fDraw = TRUE;
		break;

	case SE_APP_DRAW:		
		if(fDraw)
		{
			fDraw = FALSE; 
			WAP_Repaint();
		}
		break;

#ifdef  FEATURE_SKT_EIF //yjahn96 01.02.28 --- eif for wap
    case SE_WAP_EIF_INPUT:   /* EIF의 INPUT 명령시 sSKA_WAPINPUT()호출한 것과 똑같이 동작하도록함..*/
        EQS_START_CHILD_EVENT(sSKA_WAPINPUT, SE_APP_START, (WORD)EIF_WAP_INPUT, EQS_NA);
        break;

	case SE_WAP_EIF_SELECT:
		CLNTa_log(1,0,"UI_EIF_SELECT_F cmd getted.....");
		select_command(selectBuf);
		break;

    case SE_WAP_EIF_AFTER_SELECT:
        WAP_Display();
        break;
	case SE_WAP_EIF_UPONG:
		///////////////////////////////////////////////////////////
		// icmp.c 에서 upong을 받고 signal에 따른 event를 처리 
		if(uping_reply_command(eif_icmp))
			PongCnt++;

		if ( continue_ping && pinging )					// continue ping!!!
		{
			EQS_StopTimer(EQS_TIMER_3);
			rex_clr_sigs( rex_self(), UI_UPONG_SIG );
			ping_cnt++;
			uping_command(ping_cnt, Dest_url_IP); 

            CLNTa_log(1,0, "1. uping_command [%d] !!!", ping_cnt);

			EQS_StartTimer(EQS_TIMER_3,nTimeout, FALSE);

			CLNTa_log(1,0, "<<<<< CONTI_UPONG [%d] received!!!", PongCnt);

		} 
		else								// this is not continue ping 
		{
			if ((ping_cnt < PingCnt) && (continue_ping == FALSE))		// 사용자 지정 ping 회수보다 적게 ping을 보낸 경우 계속 ping을 보낸다. 
			{
				EQS_StopTimer(EQS_TIMER_3);
				rex_clr_sigs( rex_self(), UI_UPONG_SIG );
				ping_cnt++;
				uping_command(ping_cnt, Dest_url_IP); 
                CLNTa_log(1,0, "2. uping_command [%d] !!!", ping_cnt);
				EQS_StartTimer(EQS_TIMER_3,nTimeout, FALSE);


				CLNTa_log(1,0, "UPONG received..... from icmp...");

			}
            //yjahn96 01.08.27
			else if ((ping_cnt == PingCnt) && (continue_ping == FALSE))	// 사용자 지정 ping 회수(PingCnt) 보다 보내진 ping 회수(ping_cnt - 1)가 많은 경우 ping 종료 
			//else if ((ping_cnt >= PingCnt) && (continue_ping == FALSE))	// 사용자 지정 ping 회수(PingCnt) 보다 보내진 ping 회수(ping_cnt - 1)가 많은 경우 ping 종료 
			{
				memset(resultBuf, 0x00, sizeof(resultBuf));				// resultBuf init

				rex_clr_sigs( rex_self(), UI_UPONG_SIG );				// UI_UPONG_SIG clear
				EQS_StopTimer(EQS_TIMER_3);
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
				Dest_url_IP = 0;//memset((char*)&Dest_url_IP, 0x00, sizeof(Dest_url_IP));

                //yjahn96 01.08.27
                //모두 실패일 경우, 더이상 'SE_WAP_EIF_UPONG'이벤트를 발생시키지 않도록 함.
                ping_cnt++; 

                CLNTa_log(1,0, "UPONG received..... from icmp...");
                break;
			}
			else if ((continue_ping == TRUE) && (pinging == FALSE))		// continuous ping 종료 
			{
				memset(resultBuf, 0x00, sizeof(resultBuf));				// resultBuf init

				continue_ping = FALSE;
				pinging = TRUE;

				rex_clr_sigs( rex_self(), UI_UPONG_SIG );				// UI_UPONG_SIG clear
				EQS_StopTimer(EQS_TIMER_3);

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
				Dest_url_IP = 0;

				CLNTa_log(1,0, "<<<<< CONTINUE PING END EVENT GET >>>>>");
                break;
			}
		}	// end of if (continue_ping)
		break;

	case SE_TM_TIMER3://yjahn96 01.03.15 - for UPING
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// uping 명령 수행 중 timer expired signal(UI_UPING_TIMER_SIG)에 따른 UI_TIMER_EXPIRED_F 이벤트를 받았을 때 
			if ( continue_ping &&  pinging )					// continue ping!!!
			{
				rex_clr_sigs( rex_self(), UI_UPONG_SIG );
				ping_cnt++;					
				uping_command(ping_cnt, Dest_url_IP);
                CLNTa_log(1,0, "3. uping_command [%d] !!!", ping_cnt);
                EQS_StartTimer(EQS_TIMER_3,nTimeout , FALSE);//rex_set_timer( &ui_uping_timer, nTimeout*1000);

				CLNTa_log(1,0, "<<<<< CONTI_UPONG TIMER expired!!!");
			}
			else									// this is not continue ping
			{
				if ((ping_cnt < PingCnt) && (continue_ping == FALSE))
				{
					rex_clr_sigs( rex_self(), UI_UPONG_SIG );
					ping_cnt++;		
					uping_command(ping_cnt, Dest_url_IP);
                    CLNTa_log(1,0, "4. uping_command [%d] !!!", ping_cnt);
					EQS_StartTimer(EQS_TIMER_3,nTimeout , FALSE);
					CLNTa_log(1,0, "UPING TIMER EXPIRED......!!!");
				}
                //yjahn96 01.08.27
                else if ((ping_cnt == PingCnt) && (continue_ping == FALSE))
				//else if ((ping_cnt >= PingCnt) && (continue_ping == FALSE))
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
					Dest_url_IP = 0;
                    //yjahn96 01.08.27
                    //모두 실패일 경우, 더이상 'SE_WAP_EIF_UPONG'이벤트를 발생시키지 않도록 함.
                    ping_cnt++; 

					CLNTa_log(1,0, "UPING TIMER EXPIRED......!!!");
                    break;

				}
				else if ((continue_ping == TRUE) && (pinging == FALSE))		// continuous ping 종료 
				{
					memset(resultBuf, 0x00, sizeof(resultBuf));				// resultBuf init

					continue_ping = FALSE;
					pinging = TRUE;

					rex_clr_sigs( rex_self(), UI_UPONG_SIG );				// UI_UPONG_SIG clear

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
					Dest_url_IP = 0;

					CLNTa_log(1,0, "<<<<< CONTINUE PING END EVENT GET >>>>>");
                    break;

				}
			}	// end of if (continue_ping)
		break;

	case SE_WAP_EIF_NKEY_OPEN:// N1, N2, N3 ..... 에 해당하는 KEY 들도 한방에 OPEN 할 수 있도록 한다.
		{
			extern int NKey_count;

			NKey_Uopen_Loading(NKey_count);
			NKey_count = 0;
		}
		break;
	case SE_WAP_EIF_LINKTEXT_OPEN:	// Link Text Open available
		{
			extern int NKey_count;

			LinkText_Uopen_Loading(NKey_count);
			NKey_count = 0;
		}
		break;
#endif	//#ifdef	FEATURE_SKT_EIF

    case SE_WAP_GOMINOR:
#ifdef FEATURE_MUSIC_BELL
		musicbell_check();
#endif  //#ifdef FEATURE_MUSIC_BELL
#ifdef FEATURE_CHARACTER_BELL
        if (g_fCharacbellPreview) {
            SKY_StopMusic();
            g_fCharacbellPreview = FALSE;
        }
#endif // FEATURE_CHARACTER_BELL
#ifdef FEATURE_SKIPDATA
		g_bSkipData = FALSE;
#endif//feature_skipdata

        switch(s_towhereminor)
        {
        case WAPGOMINOR_MAKECALL:
            // ppp close --> go call
            EQS_SetDirectDraw(FALSE);
            SetCallFromWAP(CALL_SKA_WAPHANDLE_KEY);
            SET_NUMEDIT_BUFFER((BYTE*)WTAnumBuf, WTAlength);
            SET_NUMEDIT_POS(WTAlength); 
            ui_orig_voice_call(GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,  
                          GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
            EQS_START_SIBLING(SKA_MainCall, TRUE);            
            break;

        case WAPGOMINOR_SMS:
            EQS_SetDirectDraw(FALSE);
            EQS_START_CHILD_EVENT(SKA_MainSMS, SE_SMS_MESSAGE, EQS_NA, EQS_NA);
            break;

        case WAPGOMINOR_INCOMINGCALL:
            EQS_SetDirectDraw(FALSE);
            EQS_START_SIBLING_EVENT( SKA_MainCall, SE_CALL_INCOMING, EQS_NA, EQS_NA, FALSE);	
            break;

        case WAPGOMINOR_DI_PROMPT:
            EQS_START_CHILD( sSKA_WAPDIALOG_PROMPT );            
            break;

        case WAPGOMINOR_DI_ID:             
			EQS_START_CHILD( sSKA_WAPDIALOG_ID );
            break;

        case WAPGOMINOR_INPUT:
            EQS_START_CHILD( sSKA_WAPINPUT );
            break;

        case WAPGOMINOR_NEWURL:
            EQS_START_CHILD( sSKA_WAPNewURL );            
            break;

        case WAPGOMINOR_PHONEBOOK:
			/* added by hjcho 01.10.17 */
			if(SKY_IS_PHONE_BOOK_LOCK())
			{
				EQS_SetDirectDraw(FALSE);
				EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START,BOOKCODE_IN_CALL,EQS_NA);
			}
            else
			{
                 EQS_START_CHILD( SKA_BookInCalling );
            }
			break;
            
        default:
            EQS_END();
            break;
        }
        s_towhereminor = WAPGOMINOR_NONE;
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;

	/* jrkwon 2001.05.03 */
	case SE_SYS_PACEMAKER: 
#ifdef	FEATURE_SKT_EIF
        if(g_nEIFWapstatus == EIF_WAP_IWFCON && destinationIP !=0 && g_WAPSuccess == 2)	 //wap접속상태일 경우..
        {
            CLNTa_log(1,0, "HANDLEKEY_S: iwfcon_command() is called!!");
            g_nEIFWapstatus = EIF_WAP_NONE;
            iwfcon_command();        
        }

        if(g_nEIFWapstatus == EIF_WAP_WAPCON && destinationIP !=0 && g_WAPSuccess == 2)	 //wap접속상태일 경우..
        {
			extern void wapcon_command(void);
            CLNTa_log(1,0, "HANDLEKEY_S: wapcon_command() is called!!");
            g_nEIFWapstatus = EIF_WAP_NONE;
            wapcon_command();        
        }

		if (g_nEIFWapstatus == EIF_WAP_UPING // EIF_url로 dns query를 한다.
			&& destinationIP !=0 && g_WAPSuccess == 2)	 //wap접속상태일 경우..
		{
			extern BOOL Eif_dnsquery(void);
			//extern BOOL EIF_dns_query;
			
			char * url_or_ip = EIF_url;
			BOOL dns_success = FALSE;

			g_nEIFWapstatus = EIF_WAP_NONE;

			CLNTa_log(1,0,"[%s] url_or_ip", url_or_ip);

			if ((PingCnt == 0) && (continue_ping == TRUE))	// user input Ping count is 0 and continue pinging then continue ping -> end
			{
				pinging = FALSE;										// current pinging end 시킨다.
				break;
			}

			CLNTa_log(1,0,"length of DNS query EIF_url %d",strlen(EIF_url));
			CLNTa_log(1, 0, "===== process_dns start =====");
			
			dns_success = Eif_dnsquery();
			if(dns_success)
			{
				//-----------------------------
				//실제 uping_command수행
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
					CLNTa_log(1,0, "5. uping_command [%d] !!!", ping_cnt);
					EQS_StartTimer(EQS_TIMER_3,nTimeout , FALSE);	

				}
				else if ((nTimeout > 0) && (PingCnt == 255))						// timeout 이 0보다 크고 PingCnt가 255 즉 Continuous ping 인 경우 
				{	
					continue_ping = TRUE;
					ping_cnt++;
					uping_command(ping_cnt, Dest_url_IP);
					CLNTa_log(1,0, "6. uping_command [%d] !!!", ping_cnt);
					EQS_StartTimer(EQS_TIMER_3,nTimeout, FALSE);
					CLNTa_log(1,0, "<<<<< CONTI_UPONG start!!!!! >>>>>");
				}
			}
			else	//dns query is FAIL!!
			{
				eif_error_command(EIF_ERR_UPING_CONNECT_FAIL);
			}
		}
#endif	//#ifdef	FEATURE_SKT_EIF
        /* draw date and time in the sub lcd */
        EQS_DEF_EVENT_PROC(pEvent);
        break;

	case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;
        
    case SE_APP_RESUME: /* child applet is ended */
		EQS_SetDirectDraw(TRUE);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);//5분 auto 종료!!
        if(g_pCard != NULL)
        {		    
            SKY_DrawAnnunciators();
            WAP_Repaint();     
        }
        else if(s_initWAPACCESSFail) //초기 접속Fail시 
        {            
            PlayNTopMainWapImage();        
            displayloading(0);
			process_draw_bar();//WAP_DrawLoadingBarToBackBuff(WAP_ANI_TEXT_CONNECTING, WAP_ANI_BAR_FROM_LEFT);			//yjahn96 01.11.07 - for bar
            EQS_SEND_EVENT(SE_TM_TIMER, EQS_NA, EQS_NA);
        }
        else
        {              
			MSG_HIGH("!!!! child applet is ended and g_pCard is Null",0,0,0);
            EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        }        
        break;

	case SE_PH_INUSE_STATE:
		if (SKY_SetThenCompareInUseState((IN_USE_STATE_E)SKY_GetInUseState()))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

	case SE_SYS_TO_IDLE: 
        break;


	case SE_WAP_REPORTERROR:
		//------------------------------------------
		// sSKA_WAPERROR()로 이동하기 전에 처리
        //Error후에 animation이 play되는 현상 수정
		if( hNTopAnim != INVALID_CONTROL_HANDLE  )
		{
			//EQC_StopAnimation(hNTopAnim);	
			EQS_DeleteControl(hNTopAnim);
			hNTopAnim = INVALID_CONTROL_HANDLE;
		}

		// contents 저장후 msg box 없앰. key event 발생 허용
		if( hContentsDownloadControl != INVALID_CONTROL_HANDLE  )
		{
			EQS_DeleteControl(hContentsDownloadControl);
			hContentsDownloadControl = INVALID_CONTROL_HANDLE;
		}

        //EQS_START_SIBLING(sSKA_WAPERROR,FALSE);
        EQS_START_CHILD(sSKA_WAPERROR);
		break;
#ifdef FEATURE_MCOMMERCE
	case SE_WAP_SCRIPT:

		script = (ENUM_SCRIPT_E)pEvent->wParam;
		
		switch(script)
		{
			case REQUEST_CERT:
			{
				//Check space for TEMP CERT is available
				//Available then get user PIN input 
				//Not then show TEMP CERT is full msg and delete previous CERT or quit this CSR
				if(pEvent->dwParam == REQUEST_CERT_CMD){
					EQS_START_CHILD(SKA_MWCheckTempCert);
				}else if(pEvent->dwParam == REVOKE_CERT_CMD){
					EQS_START_CHILD(SKA_MWDeleteCert);
				}
			}
				break;
			case ENCRYPT_TEXT:
				EQS_START_CHILD(SKA_MWEncryptText);
				break;
			case SIGN_TEXT:
				EQS_START_CHILD(SKA_MWSignText);
				break;
			case GET_MW_DATA:
				EQS_START_CHILD(SKA_process_GetMWData);
				break;
		}
		break;

	case SE_WAP_SCRIPT_RESULT:

		if(pEvent->dwParam >= UNAUTHORIZED_CERT && pEvent->dwParam <= CANCELED_BYUSER)
			errorMMI = pEvent->dwParam;

		switch(pEvent->wParam) {
			case REQUEST_CERT :
				clnt_make_string_response(CSRData.invokeId, (char *)script_certRequest(CSRData.nCommand, (const char *)CSRData.strSubject, CSRData.strPolicy, errorMMI));
				break;
			case GET_MW_DATA :
				clnt_make_string_response(getMobileWalletData.invokeId,
										  getMWData(getMobileWalletData.appendString,					
													getMobileWalletData.setStringEnc,
													getMobileWalletData.setStringSign,
													getMobileWalletData.recipientCrt,
													getMobileWalletData.keyAgreeAlg,
													getMobileWalletData.contentEncAlg,
													getMobileWalletData.symmetricKeyId, 
													getMobileWalletData.keyIdType,					
													getMobileWalletData.keyId,
													errorMMI));			
				break;
			case ENCRYPT_TEXT:
				clnt_make_string_response(EncryptTextEx.invokeId,
									script_encryptText( (const char *)userPromptData,
 														(const char *)EncryptTextEx.serverNonce,
														(const char *)EncryptTextEx.strRecipientCert,
														(int)EncryptTextEx.nKeyAgreementAlgo,
														(const char *)EncryptTextEx.strEncAlgo,
														(int)EncryptTextEx.symmetricKeyId,
														errorMMI));
				break;
			case SIGN_TEXT:
				clnt_make_string_response(SignTextEx.invokeId, (char *)script_signText((const char *)SignTextEx.strToSign, SignTextEx.nOption, SignTextEx.nKeyIdType, (const unsigned char *)SignTextEx.keyId, errorMMI));
				break;
			default :
				CLNTa_log(1,0,"Undefined Crypto Script Result!");
				break;
		}

		break;

	case SE_MW_CERT_MIME:	

		cert = (int)pEvent->wParam;

		switch(cert)
		{
		case CERT_RESPONSE_TYPE:  
			EQS_START_CHILD(SKA_MWCertResponse);  // 개인 사용자 인증서 .. 성공../저장..
			break;
		case HASHED_CA_CERT_RESPONSE_TYPE:
			EQS_START_CHILD(SKA_MWHashedCertResponse);  // 사용자로부터 30자리 입력을 받는다.
			break;
		case SIGNED_CA_CERT_RESPONSE_TYPE:  
			//Temporarily
			EQS_START_CHILD(SKA_MWSignedCertResponse);  // 성공여부 저장..
			break;
		}

		//swjung
		//MMIa_unknownContent has no MMIa_showCard, so it is necessary to repaint current screen.
		if(g_pCard != NULL)
		{			
            SKY_DrawAnnunciators();
            WAP_Repaint();				
		}
		else
		{
			EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
		}

		break;
#endif //#ifdef FEATURE_MCOMMERCE
#ifdef INCOMING_CALL_IN_WAP
	case SE_CALL_INCOMING:
        if(!SKY_IS_DATA_CALL() && (g_pCard != NULL)) //only voice call일 경우만!!
        {            
            SKY_SET_ALERT( TRUE );
            SKY_SET_ORIGINATE(FALSE);
            g_fWAP_RET_CALL = TRUE;

            SetCallFromWAP(CALL_SKA_WAPHANDLE_KEY);	    
            ui_update_cm_call_info( &(GET_EVENT_PARAM()->call_event.call_info) );
            s_towhereminor = WAPGOMINOR_INCOMINGCALL;
            EQS_SEND_EVENT(SE_WAP_GOMINOR, EQS_NA, EQS_NA);		    
        }
        break;
#endif //#ifdef INCOMING_CALL_IN_WAP

    case SE_WAP_MAKECALL:
// that's WAP to CALL flag
#ifdef INCOMING_CALL_IN_WAP
        g_fWAP_RET_CALL = TRUE;
#endif //#ifdef INCOMING_CALL_IN_WAP
        if(SKY_IsInTraffic())
        {
            SKY_EndCall();
            //wap_end_cmd();// PPP Close; Socket Close
        }
        else
        {
            EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA);
        }         
        break;

/* ================================================================== */
    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

    case SE_CALL_ENDED : // 첫초기 origination 실패시를 고려해야한다.        
		{            
			extern void clear_command_line(void);

			MSG_ERROR("CALL 종료 됨",0,0,0);
			WAPUsedTime(MID_WAPTIME);
			SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;

			//Origination Fail -> Retry Screen 
			if(g_pCard == NULL)
			{
				wap_connection_done_cmd(1,1,FALSE);
			}else
			{
			//Idle Fail -> Close socket 
		#ifdef WAPDORMANT
				if(P_REV_IS_6_OR_GREATER && !g_fWAP_RET_CALL) 
				{
					gbDormant = TRUE;
				}
		#endif//wapdormant
				wap_end_cmd();
			}
			EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
			break;
		}
		break;

    case SE_NET_CONNECTED:
		MSG_ERROR("SE_NET_CONNECTED",0,0,0);        
//		WAPUsedTime(START);
        wap_connection_done_cmd(1,1,TRUE);
#ifdef FEATURE_SKT_EIF
		g_WAPSuccess = 1;
#endif	//#ifdef FEATURE_SKT_EIF
		break;

		/* only Using Makecall*/
    case SE_NET_DISCONNECTED:
		WAPUsedTime(MID_WAPTIME);
		SKY_SetGeneralTimer(0, FALSE);
		g_nAniBarCount=0;
		g_bDrawBar=FALSE;

#ifdef FEATURE_SKT_EIF
		if(g_WAPSuccess != 0)		
		{
			g_WAPSuccess = 0;
		}
#endif	//#ifdef FEATURE_SKT_EIF

        if(g_fWAP_RET_CALL && (g_pCard != NULL))
        {
			extern VOID WTAIc_publicMakeCallResponse(UINT8 objectID, INT8 result);

            WTAIc_publicMakeCallResponse (1,0);
            ProcessEvents();
            s_towhereminor = WAPGOMINOR_MAKECALL;
            EQS_SEND_EVENT(SE_WAP_GOMINOR, EQS_NA, EQS_NA);            
			break;
        }
	#ifdef WAPDORMANT
		if(gb2GHandoff)
		{
			/*
			    Have a intermission before run setup connection
				wap_2G_setup_connection_cmd();
			*/
			EQS_StartTimer(EQS_TIMER_2, 1000, FALSE);
			break;
		}
	#endif//wapdormant		
        break;

/* ================================================================== */

	case SE_SMS_MESSAGE:
        g_fWAP_RET_SMS = TRUE;
        process_rcv_sms(); // 되돌아 왔을 때 초기 ani or 일반 wml문서인지 결정 
        s_TeleIdType = (TELESEVICE_KIND_TYPE) g_TeleId();
        if( s_TeleIdType == WAP_MAIL || s_TeleIdType == WAP_PUSH)
        {
            Cancell_WAPLoading();
            CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation
#ifdef WAP_NOWRAP	//yjahn96
            CancelNowrapTimer(); //ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP
            process_rcv_sms(); // 되돌아 왔을 때 초기 ani or 일반 wml문서인지 결정 
            s_towhereminor = WAPGOMINOR_SMS;
            EQS_SEND_EVENT(SE_WAP_GOMINOR, EQS_NA, EQS_NA);	
        }
        EQS_PASS_CURRENT_EVENT(); 
        break;

    case SE_GOTO_SKVM:
        s_towhere = WAPGO_SKVM;
        EQS_START_SIBLING(sSKA_WAPEXIT,FALSE);
        break;

    case SE_GOTO_GVM:
        s_towhere = WAPGO_GVM;
        EQS_START_SIBLING(sSKA_WAPEXIT,FALSE);
        break;

//---------------------------------------------
    case SE_IO_TO_IDLE:     //'음악준비/저장중' msg box에서 end key에 대한 처리...   eqs_aapi.c 에러 대책 - yjahn96 01.10.30
        //EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA); -- for muxoption
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hMSGControl ||
			pEvent->wParam == hContentsDownloadControl) 
        {
            if(g_pCard != NULL)
            {		    
                SKY_DrawAnnunciators();
                WAP_Repaint();     
            }
            else
            {
                ERR_FATAL("g_Pcard NULL",0,0,0);                
            }        
        }
        break;
//---------------------------------------------

#ifdef FEATURE_WAP_WAVELET    
    case SE_WAVELET_IN_WAP:
		MSG_HIGH("SE_WAVELET_IN_WAP",0,0,0);
        fwaveletMUX = FALSE;

        //2.5G단말이 2G지역에서 stream서비스를 받을 때 
        if(!P_REV_IS_6_OR_GREATER && (g_wsvcType == 1 || g_wsvcType == 2))         
        {
            hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAVELET_NOSERVICE_REGION), 2, MBT_NONE, MBT_NOTITLE);
            break;
        }
        //2G단말이 2.5G에서 stream서비스를 받을 때 --> 해당사항없음 
        //SO/MO 실패시 ? 현재 임시로 무조선 성공event!!
        if(P_REV_IS_6_OR_GREATER)
        {
			MSG_HIGH("CAI_MUX_16X_RS1_DOUBLE",0,0,0);
            in_traffic = TRUE;            
            if(g_wMO == 0x921)
            {
                SetMOinSO33(CAI_MUX_16X_RS1_DOUBLE);                
            }
            else if(g_wMO == 0x911)
            {
                SetMOinSO33(CAI_MUX_8X_RS1_DOUBLE);             
            }
            else
            {                
                MSG_HIGH("Crtical Error",0,0,0);
            }
            EQS_StartTimer(EQS_TIMER_4, 1000*5, FALSE);            
        }
        else
        {
            EQS_SEND_EVENT(SE_MUXNEGO_SUCCESS, EQS_NA, EQS_NA);
			MSG_HIGH("SERVICE NEGO 15",0,0,0);
            //service nego 15???
        }
		break;

       
    case SE_MUXNEGO_SUCCESS:
		
		MSG_HIGH("!!!! RECEIVE SE_MUXNEGO_SUCCESS",0,0,0);
		if(SKY_IS_WAP_TO_WAVELET())
		{
            fwaveletMUX = TRUE;
            in_traffic = FALSE;
            EQS_StopTimer(EQS_TIMER_4);

			s_WAVELETsaveType = MM_ReadContentInfo(g_wcpid,g_wctsid ,g_wcntName);

			if(s_WAVELETsaveType == CONNECT_CONTENTS || s_WAVELETsaveType == OLD_CONTENTS )              
			{
				EQS_START_CHILD(sSKA_WAVELETMSG_INWAP); 
			}
			else if(s_WAVELETsaveType == NEW_CONTENTS)
			{
				gDownInfo = NEW_CONTENTS;
				EQS_SEND_EVENT(SE_TOWAVELET, EQS_NA, EQS_NA);
			}
		}
#ifdef FEATURE_WAP_MUXOPTION
		if(fMUX_Option)
		{
			fMUX_Option = FALSE;			
			SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;
            // stop CORE
			//Process_For_Cancel_Loading();            
            if(g_pCard != NULL)
			{			
                SKY_DrawAnnunciators();
                WAP_Repaint();				
			}
			else
			{
				EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			}
		}
		break; 
#endif//FEATURE_WAP_MUXOPTION
        
    case SE_TM_TIMER4: // 
        MSG_HIGH("wavelet 5초 timer종료!!",0,0,0); 
        if(!fwaveletMUX)
        {
            EQS_SEND_EVENT(SE_MUXNEGO_FAIL, EQS_NA, EQS_NA);
        }
        break;

    case SE_MUXNEGO_FAIL:
		MSG_HIGH("RECEIVE SE_MUXNEGO_FAIL",0,0,0);
        in_traffic = FALSE;
        SetMOinSO33toMIN();
        SKY_SET_WAP_TO_WAVELET(FALSE);
        hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAVELET_TOOMANY_USER), 2, MBT_NONE, MBT_NOTITLE);
        break;


    case SE_TOWAVELET:
		MSG_HIGH("RECEIVE SE_TOWAVELET",0,0,0);
        s_towhere = WAPGO_WAVELET;
        EQS_START_SIBLING(sSKA_WAPEXIT,FALSE);
        break;
#endif //#ifdef FEATURE_WAP_WAVELET


	case SE_DATA_DORM_F:
		break;

    case SE_KEY_MANNERS:
        EQS_DEF_EVENT_PROC(pEvent);
        break;

    case SE_SYS_BAD_BATT:
		MSG_HIGH("SE_SYS_BAD_BATT",0,0,0);
        EQS_SEND_EVENT( SE_KEY_END, EQS_NA, EQS_NA );
		break;

		/* KEY 	*/
	case SE_KEY_PHONE_CLOSE:
	case SE_KEY_END :  
		//------------------------------------------
		// sSKA_WAPEXIT()로 이동하기 전에 처리
        //Error후에 animation이 play되는 현상 수정
		if( hNTopAnim != INVALID_CONTROL_HANDLE  )
		{
			//EQC_StopAnimation(hNTopAnim); 		
			EQS_DeleteControl(hNTopAnim);
			hNTopAnim = INVALID_CONTROL_HANDLE;
		}

		// contents 저장후 msg box 없앰. key event 발생 허용
		if( hContentsDownloadControl != INVALID_CONTROL_HANDLE  )
		{
			EQS_DeleteControl(hContentsDownloadControl);
			hContentsDownloadControl = INVALID_CONTROL_HANDLE;
		}
	#ifdef WAPDORMANT
		if(gb2GHandoff)
		{
        //	stop equs timer	& net_disconnect
			EQS_StopTimer(EQS_TIMER_2);
		}
	#endif//wapdormant
        SetMOinSO33toMIN();
        s_towhere = WAPGO_NONE;
        SKY_SET_WAP_TO_WAVELET(FALSE);
        SKY_SET_TO_SKVM(FALSE);
        SKY_SET_TO_GVM(FALSE);
        EQS_START_SIBLING(sSKA_WAPEXIT,FALSE);
		break;
		
    case SE_KEY_0:
#if 0 
		{
			extern void ds_pzid_reconnect_cb(void);

			ds_pzid_reconnect_cb();
		}
#endif

    case SE_KEY_1:
    case SE_KEY_2:
    case SE_KEY_3:
    case SE_KEY_4:
    case SE_KEY_5:
    case SE_KEY_6:
    case SE_KEY_7:
    case SE_KEY_8:
    case SE_KEY_9:
    case SE_KEY_F1:
    case SE_KEY_F2:
    case SE_KEY_UP:
    case SE_KEY_DOWN:
    case SE_KEY_STAR:
    case SE_KEY_POUND:
    case SE_KEY_CLEAR:
    case SE_KEY_RECORD:
    case SE_KEY_CENTER:
    case SE_KEY_F2_LONG:
    case SE_WAP_PUSH:
    case SE_WAP_MAIL:
        WAP_MainHandle_Event(pEvent->EventID);
        break;

	default :
		EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAPINPUT()
**
** Description: 
**     ELEMENT_TYPE_INPUT선택시 입력화면 applet
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
** Revise:
**     sSKA_WAPINPUT을 dormant에서 call applet으로 간후 돌아올 때 pop to root가 아니고
**     sSKA_WAPINPUT으로 돌아와야하기 때문에 전역함수로 수정 
** Not yet: 
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API sSKA_WAPINPUT(SKY_EVENT_T *pEvent)
{
    static HCONTROL hControl;
    STATIC BYTE *szTmpBuffer;
    STATIC BYTE *s_pText;
    STATIC BYTE *s_pTitle;
    STATIC BYTE *s_pFormat;
    STATIC int s_nLength, s_nMaxStrLeng;
    STATIC TEXT_EDIT_MODE_E s_Mode;

    if((SE_KEY_0 <= pEvent->EventID && pEvent->EventID <=SE_KEY_F2)||
        pEvent->EventID == SE_KEY_RECORD||pEvent->EventID == SE_KEY_AUTO_ANSWER||
        pEvent->EventID == SE_KEY_SEND || pEvent->EventID == SE_WAP_SHOWCARD)
        
    {  
	    EQS_StopTimer(EQS_TIMER_1);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);        
    }

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(FALSE);		
        if(pEvent->wParam == EIF_WAP_INPUT)
        {
            EQS_END();
            break;
        }

        //-------------------------------------------------------------
        //Text Edit의 Title 결정
        s_nLength = strlen(g_pCurInput->title);
        if(g_pCurInput->title == NULL)
            s_pTitle = (BYTE*)SKY_GET_TOKEN(TKN_WAP_INPUT_TITLE); 
		else if(s_nLength > WAP_HS_SCRN_WIDE-2) //16자 초과       
        {
		    TruncateString((BYTE *)g_pCurInput->title, WAP_HS_SCRN_WIDE-2);//16자
		    s_pTitle = (BYTE *)g_pCurInput->title;
        }
		else
		{
		    s_pTitle = (BYTE *)g_pCurInput->title;
		}

        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      (BYTE *)s_pTitle,//SKY_GET_TOKEN(TKN_WAP_INPUT_TITLE),
					  WA_SHOW_HEADER);

        //---------------------------------------------------------------
        //input의 format 처리 - Max num of chars
        s_pFormat = (BYTE*)SKY_Malloc(WAP_MAX_INPUT);
        memset(s_pFormat, NULL, WAP_MAX_INPUT);//init
		memcpy(s_pFormat, g_pCurInput->format, strlen(g_pCurInput->format));


        //-------------------------------------------------------------
        // 입력 문자 수 set
        // 항상 max로 보내고, 실제로 제한이 있는 경우는, EQC_SetFormatLength()를 사용 - yjahn96 01.09.01
//        if(g_pCurInput->nChars > 0) s_nMaxStrLeng = g_pCurInput->nChars;
//        else                        s_nMaxStrLeng = WAP_MAX_INPUT;
        s_nMaxStrLeng = WAP_MAX_INPUT;

        //-------------------------------------------------------------
        //현재 값을 기본값으로 set
        s_nLength = strlen(g_pCurInput->text);

        if(s_nLength > 0)   s_pText = (BYTE *)g_pCurInput->text;
        else                s_pText = (BYTE *)"";

        //---------------------------------------------------------------
        // Password 처리
        if(g_pCurInput->isPassword) //이때 입력되는 것은 '*'으로 나와야 함...
        {
            s_Mode = TEM_ENGLISH_LOWER;
             hControl = EQC_TextEdit( 8, 20, 16, 5, s_Mode, WAP_MAX_INPUT/*s_nMaxStrLeng*/,(BYTE *)s_pText, s_nLength );
            EQC_SET_STYLE(hControl, CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_PASSWORD);
        }
        else
        {
            s_Mode = TEM_KOREAN;
            hControl = EQC_TextEdit( 8, 20, 16, 5, s_Mode, WAP_MAX_INPUT/*s_nMaxStrLeng*/,(BYTE *)s_pText, s_nLength );
        }
        EQC_SetFormatString(hControl, (BYTE*)s_pFormat);
        if(g_pCurInput->nChars > 0) EQC_SetFormatLength(hControl, s_nMaxStrLeng);
        szTmpBuffer = (BYTE*)SKY_Malloc(WAP_MAX_INPUT);
        EQS_SetIOCActive(hControl);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);//5분종료!!
        break;

    
	case SE_CALL_INCOMING:  
        EQS_END_EVENT(SE_CALL_INCOMING, EQS_NA, EQS_NA);
        break;

	case SE_SMS_MESSAGE:
        EQS_END_EVENT(SE_SMS_MESSAGE, EQS_NA, EQS_NA);		
        break;

     case SE_TM_TIMER1: // 5분 timer auto종료 !!
        MSG_HIGH("5분 timer종료!!",0,0,0);        
#ifdef FEATURE_SKT_EIF
        if(g_EIFWAPExit)
        {
            g_EIFWAPExit = FALSE;
            at_send_extended_text("EXIT");
        }
#endif  //#ifdef FEATURE_SKT_EIF
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;
		
    case SE_APP_END :   /* The very last event */  
        g_nViewMode = WAP_VIEW_MODE_WML;
		SKY_Free(s_pFormat);
        SKY_Free(szTmpBuffer);       
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
        break;

    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData(hControl, szTmpBuffer);
        s_nLength = strlen((char *)szTmpBuffer);          
        if((g_pCurInput->emptyOk == FALSE) && (s_nLength == 0))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_INPUT_ALERT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);  
            break;//Msg box "입력해주세요"
        }
        else
        {
            memset(g_pCurInput->text, NULL, WAP_MAX_INPUT+1);

			memcpy(g_pCurInput->text, szTmpBuffer, s_nLength);
			g_pCurInput = NULL;//init
		}
		g_nViewMode = WAP_VIEW_MODE_WML;
        WAP_Formatting();
        EQS_END();  //'sSKA_WAPHANDLE_KEY'로 이동
        break;

    case SE_IO_TEDIT_CANCEL:
		g_pCurInput = NULL;//init
		g_nViewMode = WAP_VIEW_MODE_WML;
        EQS_END();  //'sSKA_WAPHANDLE_KEY'로 이동
        break;

	case SE_CALL_ENDED ://다른 child도 mode에 따라 바꿔준다.!!
        /*EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_NA, EQS_NA );*/
        EQS_END_CURRENT_EVENT();
		break;

   	case SE_SYS_TO_IDLE: //phone close시 call상태일때는 이 이벤트 후에 SE_KEY_PHONE_CLOSE가 옴
        break;           //다라서, wap중에서는 이 이벤트를 무시함

	case SE_IO_TO_IDLE:	//from messagebox - end key눌렀을때 
    case SE_KEY_END : /* KEY wap 종료!! */
    case SE_KEY_PHONE_CLOSE:    /* ear down */
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;

    case SE_WAP_REPORTERROR:
        EQS_END_EVENT(SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
		break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

	default:

		EQS_DEF_EVENT_PROC(pEvent);
		break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAPNewURL()
**
** Description: 
**     Wap menu에서 '새URL고치기'선택시 입력화면 applet
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
** Revise:
**     sSKA_WAPNewURL을 dormant에서 call applet으로 간후 돌아올 때 pop to root가 아니고
**     sSKA_WAPNewURL으로 돌아와야하기 때문에 전역함수로 수정
** Not yet: 
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API sSKA_WAPNewURL(SKY_EVENT_T *pEvent)
{
    static HCONTROL hControl;
    STATIC BYTE *szTmpBuffer;
    STATIC BYTE *s_pNewURL;
    STATIC int s_nLength;

    if((SE_KEY_0 <= pEvent->EventID && pEvent->EventID <=SE_KEY_F2)||
        pEvent->EventID == SE_KEY_RECORD||pEvent->EventID == SE_KEY_AUTO_ANSWER||
        pEvent->EventID == SE_KEY_SEND || pEvent->EventID == SE_WAP_SHOWCARD )
        
    {  
	    EQS_StopTimer(EQS_TIMER_1);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);        
    }

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(FALSE);		
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_WAP_NEW_URL),
					  WA_SHOW_HEADER);
        hControl = EQC_TextEdit( 8, 20, 16, 5, TEM_ENGLISH_LOWER, WAP_MAX_INPUT,(BYTE *)"www.", 4 );
        szTmpBuffer = (BYTE*)SKY_Malloc(WAP_MAX_INPUT);
        EQS_SetIOCActive(hControl);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);//5분종료!!
        break;

    
	case SE_CALL_INCOMING: 
        EQS_END_EVENT(SE_CALL_INCOMING, EQS_NA, EQS_NA);
        break;

	case SE_SMS_MESSAGE:
        EQS_END_EVENT(SE_SMS_MESSAGE, EQS_NA, EQS_NA);		
        break;

    case SE_TM_TIMER1: // 5분 timer auto종료 !!
#ifdef FEATURE_SKT_EIF
        if(g_EIFWAPExit)
        {
            g_EIFWAPExit = FALSE;
            at_send_extended_text("EXIT");
        }
#endif  //#ifdef FEATURE_SKT_EIF
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;
		
    case SE_APP_END :   /* The very last event */ 
        SKY_Free(szTmpBuffer);        
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
        break;

    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData(hControl, szTmpBuffer);
        s_nLength = strlen((char *)szTmpBuffer);          
        if(s_nLength > 4)
        {
            s_pNewURL = (BYTE*)SKY_Malloc(s_nLength+1);
            memset(s_pNewURL, NULL, s_nLength+1);
            memcpy(s_pNewURL, szTmpBuffer, s_nLength);        
		    g_nMenuMode = WAP_MENU_MODE_NONE;
			SKY_loadURL(g_pCard->viewId, (const CHAR*)s_pNewURL, FALSE);
            //EQS_END_EVENT(SE_APP_RESUME, TO_LOADING, EQS_NA);
            EQS_END();
        }
        else    //URL을 입력하지 않은 경우
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_INPUT_ALERT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);  
            break;//Msg box "입력해주세요"
   
        }
        break;

    case SE_IO_TEDIT_CANCEL:
		EQS_END();//'sSKA_WAPHANDLE_KEY'로 이동
        break;
	
    case SE_CALL_ENDED ://다른 child도 mode에 따라 바꿔준다.!!  
        /*EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_NA, EQS_NA );*/
        EQS_END_CURRENT_EVENT();
		break;

   	case SE_SYS_TO_IDLE: //phone close시 call상태일때는 이 이벤트 후에 SE_KEY_PHONE_CLOSE가 옴
        break;           //다라서, wap중에서는 이 이벤트를 무시함

	case SE_IO_TO_IDLE:	//from messagebox - end key눌렀을때 
    case SE_KEY_END : /* KEY wap 종료!! */
    case SE_KEY_PHONE_CLOSE:    /* ear down */
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;

    case SE_WAP_REPORTERROR:
        EQS_END_EVENT(SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
		break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

	default:
		EQS_DEF_EVENT_PROC(pEvent);
		break;
    }
    
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAPDIALOG_PASSWORD()
**
** Description: 
**     입력화면 applet
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
** Not yet: 
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sSKA_WAPDIALOG_PASSWORD(SKY_EVENT_T *pEvent)
{
    static HCONTROL hControl;
    STATIC BYTE *szTmpBuffer;
	STATIC int s_nLength;

    if((SE_KEY_0 <= pEvent->EventID && pEvent->EventID <=SE_KEY_F2)||
        pEvent->EventID == SE_KEY_RECORD||pEvent->EventID == SE_KEY_AUTO_ANSWER||
        pEvent->EventID == SE_KEY_SEND || pEvent->EventID == SE_WAP_SHOWCARD )
        
    {  
	    EQS_StopTimer(EQS_TIMER_1);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);        
    }

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(FALSE);		
		EQS_SetWindow(SKY_DEF_WIN_X, 
					  SKY_DEF_WIN_Y, 
					  SKY_DEF_WIN_DX, 
					  SKY_DEF_WIN_DY, 
					  CL_NONE, 
					  SKY_GET_TOKEN(TKN_WAP_DIALOG_PASSWORD_TITLE),
					  WA_SHOW_HEADER); 
		hControl = EQC_TextEdit( 8, 20, 16, 5, TEM_ENGLISH_LOWER, WAP_MAX_INPUT,(BYTE *)"", 0 );
        EQC_SET_STYLE(hControl, CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_PASSWORD);
		EQC_SetTextEditModeSequence(hControl, DialogMode);

        szTmpBuffer = (BYTE*)SKY_Malloc(WAP_MAX_INPUT);
        EQS_SetIOCActive(hControl);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);//5분종료!!
        break;

     case SE_TM_TIMER1: // 5분 timer auto종료 !!
        MSG_HIGH("5분 timer종료!!",0,0,0);
#ifdef FEATURE_SKT_EIF
        if(g_EIFWAPExit)
        {
            g_EIFWAPExit = FALSE;
            at_send_extended_text("EXIT");
        }
#endif  //#ifdef FEATURE_SKT_EIF
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;
		
    case SE_APP_END :   /* The very last event */ 
		g_nDlgMode = WAP_DLG_MODE_NONE;
        SKY_Free(szTmpBuffer);		
		EQS_SetDirectDraw(TRUE);
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
        break;

    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData(hControl, szTmpBuffer);
        s_nLength = strlen((char *)szTmpBuffer);          
        if(s_nLength == 0)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_INPUT_ALERT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);  
            break;//Msg box "입력해주세요"
        }
        else
        {
			if(g_pDlgMessage2) wip_free((void *)g_pDlgMessage2);
			g_pDlgMessage2 = (char *)wip_malloc(sizeof(char)*(s_nLength+1));

			memset(g_pDlgMessage2, '\0', s_nLength);
			memcpy(g_pDlgMessage2, szTmpBuffer, s_nLength);
			SKY_passwordDialogResponse(g_pCard->viewId, g_nDlgId,
				g_pDlgMessage, g_pDlgMessage2);
			g_pDlgMessage = NULL;
			g_pDlgMessage2 = NULL;
			g_nDlgMode = WAP_DLG_MODE_NONE;
	        EQS_END();  //'sSKA_WAPHANDLE_KEY'로 이동
			break;
		}	
        break;

    case SE_IO_TEDIT_CANCEL:
		g_nDlgMode = WAP_DLG_MODE_NONE;
		SKY_passwordDialogResponse(g_pCard->viewId, g_nDlgId, NULL, NULL);
		g_pDlgMessage = NULL;
		g_pDlgMessage2 = NULL;
        EQS_END();  //'sSKA_WAPHANDLE_KEY'로 이동
        break;

	case SE_CALL_ENDED ://UI_CALL_ENDED_E
        /*EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_NA, EQS_NA );*/
        EQS_END_CURRENT_EVENT();
		break;

   	case SE_CALL_INCOMING: // 일단 무시한다. 
        EQS_END_EVENT(SE_CALL_INCOMING, EQS_NA, EQS_NA);
        break;

    case SE_SMS_MESSAGE:
        EQS_END_EVENT(SE_SMS_MESSAGE, EQS_NA, EQS_NA);		
        break;

   	case SE_SYS_TO_IDLE: //phone close시 call상태일때는 이 이벤트 후에 SE_KEY_PHONE_CLOSE가 옴
        break;           //다라서, wap중에서는 이 이벤트를 무시함

	case SE_IO_TO_IDLE:	//from messagebox - end key눌렀을때 
    case SE_KEY_END : /* KEY wap 종료!! */
    case SE_KEY_PHONE_CLOSE:    /* ear down */
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;

    case SE_WAP_REPORTERROR:
        EQS_END_EVENT(SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
		break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

	default:
		EQS_DEF_EVENT_PROC(pEvent);
		break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAPDIALOG_ID()
**
** Description: 
**     입력화면 applet
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
** Not yet: 
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sSKA_WAPDIALOG_ID(SKY_EVENT_T *pEvent)
{
    static HCONTROL hControl;
    STATIC BYTE *szTmpBuffer;
	STATIC int s_nLength;

    if((SE_KEY_0 <= pEvent->EventID && pEvent->EventID <=SE_KEY_F2)||
        pEvent->EventID == SE_KEY_RECORD||pEvent->EventID == SE_KEY_AUTO_ANSWER||
        pEvent->EventID == SE_KEY_SEND || pEvent->EventID == SE_WAP_SHOWCARD )
        
    {  
	    EQS_StopTimer(EQS_TIMER_1);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);        
    }

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(FALSE);
		EQS_SetWindow(SKY_DEF_WIN_X, 
					  SKY_DEF_WIN_Y, 
					  SKY_DEF_WIN_DX, 
					  SKY_DEF_WIN_DY, 
					  CL_NONE, 
					  SKY_GET_TOKEN(TKN_WAP_DIALOG_ID_TITLE),
					  WA_SHOW_HEADER); 
        hControl = EQC_TextEdit( 8, 20, 16, 5, TEM_ENGLISH_LOWER, WAP_MAX_INPUT,(BYTE *)"", 0 );
		EQC_SetTextEditModeSequence(hControl, DialogMode);        
        szTmpBuffer = (BYTE*)SKY_Malloc(WAP_MAX_INPUT);
        EQS_SetIOCActive(hControl);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);//5분종료!!
        break;

     case SE_TM_TIMER1: // 5분 timer auto종료 !!
        MSG_HIGH("5분 timer종료!!",0,0,0);
#ifdef FEATURE_SKT_EIF
        if(g_EIFWAPExit)
        {
            g_EIFWAPExit = FALSE;
            at_send_extended_text("EXIT");
        }
#endif  //#ifdef FEATURE_SKT_EIF
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;
		
    case SE_APP_END :   /* The very last event */ 
        SKY_Free(szTmpBuffer);	
		EQS_SetDirectDraw(TRUE);
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
        break;

    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData(hControl, szTmpBuffer);
        s_nLength = strlen((char *)szTmpBuffer);
        if(s_nLength == 0)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_INPUT_ALERT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);  
            break;//Msg box "입력해주세요"
        }
        else
        {
			if(g_pDlgMessage) wip_free((void *)g_pDlgMessage);
			g_pDlgMessage = (char *)wip_malloc(sizeof(char)*(s_nLength+1));
			memset(g_pDlgMessage, '\0', s_nLength);
			memcpy(g_pDlgMessage, szTmpBuffer, s_nLength);
			//패스워드를 입력받는다
			EQS_START_SIBLING(sSKA_WAPDIALOG_PASSWORD, FALSE);
		}
        break;

    case SE_IO_TEDIT_CANCEL:
		g_nDlgMode = WAP_DLG_MODE_NONE;
		SKY_passwordDialogResponse(g_pCard->viewId, g_nDlgId, NULL, NULL);
		g_pDlgMessage = NULL;
		g_pDlgMessage2 = NULL;
        EQS_END();  //'sSKA_WAPHANDLE_KEY'로 이동
        break;

	case SE_CALL_ENDED ://일단 wap 종료한다.
        /*EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_NA, EQS_NA );*/
        EQS_END_CURRENT_EVENT();
		break;

   	case SE_CALL_INCOMING: // 일단 무시한다. 
        EQS_END_EVENT(SE_CALL_INCOMING, EQS_NA, EQS_NA);
        break;

    case SE_SMS_MESSAGE:
        EQS_END_EVENT(SE_SMS_MESSAGE, EQS_NA, EQS_NA);		
        break;

   	case SE_SYS_TO_IDLE: //phone close시 call상태일때는 이 이벤트 후에 SE_KEY_PHONE_CLOSE가 옴
        break;           //다라서, wap중에서는 이 이벤트를 무시함

	case SE_IO_TO_IDLE:	//from messagebox - end key눌렀을때 
    case SE_KEY_END : /* KEY wap 종료!! */
    case SE_KEY_PHONE_CLOSE:    /* ear down */
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;

    case SE_WAP_REPORTERROR:
        EQS_END_EVENT(SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
		break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

	default:
		EQS_DEF_EVENT_PROC(pEvent);
		break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAPDIALOG_PROMPT()
**
** Description: 
**     입력화면 applet
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
** Not yet: 
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sSKA_WAPDIALOG_PROMPT(SKY_EVENT_T *pEvent)
{
    static HCONTROL hControl;
    STATIC BYTE *szTmpBuffer;
    STATIC BYTE *s_pText;
    STATIC BYTE *s_pTitle;
    STATIC int s_nLength;

    if((SE_KEY_0 <= pEvent->EventID && pEvent->EventID <=SE_KEY_F2)||
        pEvent->EventID == SE_KEY_RECORD||pEvent->EventID == SE_KEY_AUTO_ANSWER||
        pEvent->EventID == SE_KEY_SEND || pEvent->EventID == SE_WAP_SHOWCARD)
        
    {  
	    EQS_StopTimer(EQS_TIMER_1);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);        
    }

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(FALSE);		
        s_nLength = strlen(g_pDlgMessage);
        if(g_pDlgMessage == NULL)  s_pTitle = (BYTE *)SKY_GET_TOKEN(TKN_WAP_INPUT_TITLE); 
		else if(s_nLength > WAP_HS_SCRN_WIDE-2) //16자 초과       
        {
		    TruncateString((BYTE *)g_pDlgMessage, WAP_HS_SCRN_WIDE-2);//16자
		    s_pTitle = (BYTE *)g_pDlgMessage;
        }
		else
		{
		    s_pTitle = (BYTE *)g_pDlgMessage;
		}

        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      (BYTE *)s_pTitle,//SKY_GET_TOKEN(TKN_WAP_INPUT_TITLE),
					  WA_SHOW_HEADER);

        s_nLength = strlen((char *)g_pDlgMessage2);
        if(s_nLength > 0)   s_pText = (BYTE *)g_pDlgMessage2;
        else                s_pText = (BYTE *)"";
        hControl = EQC_TextEdit( 8, 20, 16, 5, TEM_ENGLISH_LOWER, WAP_MAX_INPUT, (BYTE *)s_pText, s_nLength );
		EQC_SetTextEditModeSequence(hControl, DialogMode);        
        szTmpBuffer = (BYTE*)SKY_Malloc(WAP_MAX_INPUT);
        EQS_SetIOCActive(hControl);
        EQS_StartTimer(EQS_TIMER_1, 60000*5, FALSE);//5분종료!!
        break;

     case SE_TM_TIMER1: // 5분 timer auto종료 !!
        MSG_HIGH("5분 timer종료!!",0,0,0);
#ifdef FEATURE_SKT_EIF
        if(g_EIFWAPExit)
        {
            g_EIFWAPExit = FALSE;
            at_send_extended_text("EXIT");
        }
#endif  //#ifdef FEATURE_SKT_EIF
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;
		
    case SE_APP_END :   /* The very last event */ 
        SKY_Free(szTmpBuffer);		
		EQS_SetDirectDraw(TRUE);
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
        break;

    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData(hControl, szTmpBuffer);
        s_nLength = strlen((char *)szTmpBuffer);        
        if(s_nLength == 0)
        {
			SKY_promptDialogResponse(1, g_nDlgId, NULL);
        }
        else
        {
			WCHAR *answer;
			char temp[WAP_MAX_INPUT];
			memset(temp, '\0', WAP_MAX_INPUT);
			memcpy(temp, szTmpBuffer, s_nLength);
			//char -> WCHAR
			s_nLength = UniLenOfKSCStr((char *)temp);
			answer = (WCHAR *)wip_malloc(sizeof(WCHAR)*(s_nLength+1));
			KSC2UniString((unsigned char *)temp, (WCHAR *)answer);								
			SKY_promptDialogResponse(1, g_nDlgId, answer);	

		}
		g_nDlgMode = WAP_DLG_MODE_NONE;
        EQS_END();  //'sSKA_WAPHANDLE_KEY'로 이동
        break;

    case SE_IO_TEDIT_CANCEL:
		s_nLength = strlen(g_pDlgMessage2);
		if(s_nLength > 0)
		{
			WCHAR *answer;
			char temp[WAP_MAX_INPUT];
			memset(temp, '\0', WAP_MAX_INPUT);
			memcpy(temp, g_pDlgMessage2, s_nLength);
			//char -> WCHAR
			s_nLength = UniLenOfKSCStr((char *)temp);
			answer = (WCHAR *)wip_malloc(sizeof(WCHAR)*(s_nLength+1));
			KSC2UniString((unsigned char *)temp, (WCHAR *)answer);								
			SKY_promptDialogResponse(1, g_nDlgId, answer);					
		}
		else
		{
			SKY_promptDialogResponse(1, g_nDlgId, NULL);
		}
		g_nDlgMode = WAP_DLG_MODE_NONE;
        EQS_END();  //'sSKA_WAPHANDLE_KEY'로 이동
        break;

	case SE_CALL_ENDED ://UI_CALL_ENDED_E
        /*EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_NA, EQS_NA );*/
        EQS_END_CURRENT_EVENT();
		break;

   	case SE_CALL_INCOMING: // 일단 무시한다. 
        EQS_END_EVENT(SE_CALL_INCOMING, EQS_NA, EQS_NA);
        break;

    case SE_SMS_MESSAGE:
        EQS_END_EVENT(SE_SMS_MESSAGE, EQS_NA, EQS_NA);		
        break;

   	case SE_SYS_TO_IDLE: //phone close시 call상태일때는 이 이벤트 후에 SE_KEY_PHONE_CLOSE가 옴
        break;           //다라서, wap중에서는 이 이벤트를 무시함

    case SE_KEY_END : /* KEY wap 종료!! */
    case SE_KEY_PHONE_CLOSE:    /* ear down */
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;

    case SE_WAP_REPORTERROR:
        EQS_END_EVENT(SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
		break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

	default:
		EQS_DEF_EVENT_PROC(pEvent);
		break;
    }
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAPERROR()
**
** Description: 
**     WAP중 Error발생시 진입하는 applet 
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
** Not yet: 
**     1. 모든 error가 확인되지 않은 상태 
**     2. 초기 dns query time out/ dormant reconnet/no service
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sSKA_WAPERROR(SKY_EVENT_T *pEvent) //redirect, dormant처리 !!
{
    HCONTROL hControl;
    STATIC HCONTROL hMSGControl1; //redirect fail
    STATIC HCONTROL hMSGControl2; //init orign fail

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(TRUE);
        EQS_GetDC(TRUE);
		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators. 
            WA_SHOW_HEADER);         

		g_bDrawBar=FALSE;	//yjahn96 01.11.08 - 취소 후, command line 안그려지는 문제  

		CancelAnimationTimer(); //stop sis animation
	#ifdef WAP_NOWRAP
		CancelNowrapTimer(); //stop nowrap
	#endif//#ifdef WAP_NOWRAP
	#ifdef FEATURE_MUSIC_BELL
		musicbell_check();
	#endif  //#ifdef FEATURE_MUSIC_BELL
	#ifdef FEATURE_CHARACTER_BELL
		if (g_fCharacbellPreview) {
			SKY_StopMusic();
			g_fCharacbellPreview = FALSE;
		}
	#endif // FEATURE_CHARACTER_BELL
#ifdef FEATURE_WAP_REDIRECT
        gredirectloadingbar = FALSE;
#endif //#ifdef FEATURE_WAP_REDIRECT

        wap_display_error_message(g_errorNumber, g_errorType);		
#ifdef FEATURE_WAP_REDIRECT
        if(redirect.bCorrect || g_bWAPinitAccessFail)
        {
            ;
        }
        else
#endif //#ifdef FEATURE_WAP_REDIRECT
        {
		    EQS_StartTimer(EQS_TIMER_1,500, FALSE);//error display time *header에 이상현상에의해 값을줄임!!
        }
        g_nAniTextMode = WAP_ANI_TEXT_RECEIVING;//수신중
        break;


	case SE_APP_DRAW:        
		break;

    case SE_TM_TIMER1:
//		Go to ska_waphandlekeys instead of ska_wapexit		
//		if(g_pCard == NULL)
//		{			
//			EQS_START_SIBLING(sSKA_WAPEXIT,FALSE);	
//		}
//      else
        {
            EQS_END();            
         //   EQS_START_SIBLING_EVENT( SKA_WAPHANDLE_KEY, SE_APP_RESUME, EQS_NA, EQS_NA, FALSE);
        }
		break;

	case SE_SYS_TO_IDLE: 
        break;
	
/* KEY 	*/
	case SE_KEY_PHONE_CLOSE:
	case SE_KEY_END :
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        //EQS_START_SIBLING_EVENT( SKA_WAPHANDLE_KEY, SE_KEY_END, EQS_NA, EQS_NA, FALSE); 
		break;

    case SE_KEY_CLEAR:
        break;

    case SE_IO_TO_IDLE:
        EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;

    case SE_APP_END :   /* The very last event */ 
        init_buffer(&redirect);
        g_bWAPinitAccessFail = FALSE;
        wap_set_error(FALSE);
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }
        EQS_ReleaseDC();
    	break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_WAP_INITACCEESSFAIL:
		{
			clear_command_line();
			hMSGControl2= EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_INITACCESS_FAIL), MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_STYLE(hMSGControl2, EQC_GET_STYLE(hMSGControl2) |CS_MESSAGE_BOX_IGNORE_KEYS);
			break;  
		}
   
#ifdef FEATURE_WAP_REDIRECT
    case SE_WAP_REDIRECT:
        s_initWAPACCESSFail = TRUE;
        gredirectloadingbar = TRUE;
        Redirection_dnsquery();
        break;
    // Redirection Fail시 처리가 있어야???

    case SE_WAP_REDIRECTFAIL:
        hMSGControl1 = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_REDIRECT_FAIL), MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);
        EQC_SET_STYLE(hMSGControl1, EQC_GET_STYLE(hMSGControl1) |CS_MESSAGE_BOX_IGNORE_KEYS);
        break;

    case SE_IO_MESGBOX_YES:
        s_initWAPACCESSFail = TRUE;
        if(pEvent->wParam == hMSGControl1)
        {
            Redirection_dnsquery();
        }
        else if(pEvent->wParam == hMSGControl2)
        {
            wap_retry_cmd();
            displayloading(0);
			process_draw_bar();//WAP_DrawLoadingBarToBackBuff(WAP_ANI_TEXT_CONNECTING, WAP_ANI_BAR_FROM_LEFT);			//yjahn96 01.11.07 - for bar
            EQS_END();
            //EQS_START_SIBLING_EVENT( SKA_WAPHANDLE_KEY, SE_APP_RESUME, EQS_NA, EQS_NA, FALSE);
        }
        break;

    case SE_IO_MESGBOX_NO:
        s_initWAPACCESSFail = FALSE;
        if(pEvent->wParam == hMSGControl1 || pEvent->wParam == hMSGControl2)
        {
         //   EQS_START_SIBLING_EVENT( SKA_WAPHANDLE_KEY, SE_KEY_END, EQS_NA, EQS_NA, FALSE);
            EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        }
        break;
#endif //#ifdef FEATURE_WAP_REDIRECT

    case SE_CALL_ENDED:
        if(redirect.bCorrect)
        {
            wap_end_cmd();            
            s_towhere = WAPGO_ABNORMALEXIT;
            EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        }
        else
        {
            EQS_END_EVENT(SE_CALL_ENDED, EQS_NA, EQS_NA);
        }
        //EQS_START_SIBLING_EVENT( SKA_WAPHANDLE_KEY, SE_CALL_ENDED, EQS_NA, EQS_NA, FALSE);
        break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;
	    
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAPEXIT()
**
** Description: 
**     user에 의한 WAP 종료 / Network혹은 Dormant Time에의한 Dormant상태일 때 applet
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
** Not yet: 
**       Terminate Reset변수 정리!!!
** ---------------------------------------------------------------------------
*/
VOID SKY_API sSKA_WAPEXIT(SKY_EVENT_T *pEvent)		
{
	extern byte *mb_MusicBellPreBuffer;

    HCONTROL hMSGControl;
    HCONTROL hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(TRUE);
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
        SKY_DEF_WIN_Y, 
        SKY_DEF_WIN_DX, 
        SKY_DEF_WIN_DY, 
        CL_NONE, 
        NULL, // show annunciators. 
        WA_SHOW_HEADER); 
        SKY_DrawAnnunciators();

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

		MSG_HIGH("WAP Exit : SE_APP_START ",0,0,0);
    CancelAnimationTimer(); //stop sis animation
#ifdef WAP_NOWRAP
    CancelNowrapTimer(); //stop nowrap
#endif//#ifdef WAP_NOWRAP
#ifdef FEATURE_MUSIC_BELL
	musicbell_check();	//play stop
#endif  //#ifdef FEATURE_MUSIC_BELL
#ifdef FEATURE_CHARACTER_BELL
    if (g_fCharacbellPreview) {
        SKY_StopMusic();
        g_fCharacbellPreview = FALSE;
    }
#endif // FEATURE_CHARACTER_BELL

#ifdef FEATURE_SKT_EIF
		// ===============================================================================			
		// yjahn96 01.03.14  EIF WAP(External Interface for WAP)
		// uping 종료시 다음번 uping을 위해 초기화시켜준다.

		if(g_WAPSuccess != 0)
			g_WAPSuccess = 0;

		Dest_IP[0] = 0; Dest_IP[1] = 0; Dest_IP[2] = 0; Dest_IP[3] = 0;
		Dest_url_IP = 0;

        //trigger...
        if(g_EIFWAPExit)
        {
            g_EIFWAPExit = FALSE;
        }
		// ===============================================================================
#endif	// FEATURE_SKT_EIF//EIF_WAP

        switch(s_towhere)
        {            
#ifdef FEATURE_GVM
            case WAPGO_GVM: 
				if(mb_MusicBellPreBuffer)
				{
					MSG_ERROR("mb_MusicBellPreBuffer =NULL",0,0,0);
					wip_free((void*)mb_MusicBellPreBuffer);
					mb_MusicBellPreBuffer =NULL;
				}
                //wap_display_exit_for_gvm();                
                hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_TOGVM_EXIT), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);
                break;
#endif //#ifdef FEATURE_GVM
#ifdef FEATURE_SKVM
            case WAPGO_SKVM:            
		        //wap_display_exit_for_skvm();
                hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_TOSKVM_EXIT), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);
                break; 
#endif //#ifdef FEATURE_SKVM
#ifdef FEATURE_WAP_WAVELET
            case WAPGO_WAVELET:            
		        //wap_display_exit_for_wavelet();
                hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_TOWAVELET_EXIT), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);
                break; 
#endif //#ifdef FEATURE_WAP_WAVELET

            case WAPGO_INITERROR:
                hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_INITSETTING_FAIL), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);
                break;

            case WAPGO_ABNORMALEXIT:
                hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_ABNORMAL_EXIT), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);
                break;


            default :                
                //wap_display_exit_message();
                hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_NORMAL_EXIT), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);                
                break;
        }   
        EQC_SET_STYLE(hMSGControl, EQC_GET_STYLE(hMSGControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
		//To prevent Applet from escaping for safe 
        EQS_StartTimer(EQS_TIMER_1, 10000, FALSE); 
#ifdef WAPDORMANT
		{
			//when user want end, avoid dormant mode
			gbDormant = FALSE;
		}
#endif//wapdoramt
        wap_end_cmd();
		break;

    
    case SE_WAP_COMPLETE://browser Ended...
		{
			EQS_SetWindowSymbolMode(WTSM_NORMAL_MODE);
			EQS_StopTimer(EQS_TIMER_1);
			SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;
			EQS_END();  
			break;
		}

    case SE_APP_DRAW:
		break;

    case SE_TM_TIMER1: 
		{
			MSG_ERROR("나오면 안되는 Timer1에의한 종료",0,0,0);
			EQS_SetWindowSymbolMode(WTSM_NORMAL_MODE);        
			//jmpark 011019, blocking to find out bugs
			MSG_ERROR("ERR_FATAL report to jmpark",0,0,0);
			EQS_StopTimer(EQS_TIMER_1);
			EQS_StartTimer(EQS_TIMER_2, 5000, FALSE); 
			wap_abnormal_end_cmd();
			break;
		}
	case SE_TM_TIMER2:
		{
			ERR_FATAL("나오면 안되는 Timer2에 의한 종료",0,0,0);
			break;
		}

    case SE_NET_DISCONNECTED:		
		{
			MSG_HIGH("NET_DISCONNET에의한 종료",0,0,0);
			WAPUsedTime(MID_WAPTIME);
			WAPUsedTime(END_WAPTIME);
			wap_exit_cmd();        
			break;        
		}
    
	case SE_APP_RESUME: /* child applet is ended */
        break;
	
       /* jrkwon 2001.05.03 */
	case SE_SYS_PACEMAKER: 
        /* draw date and time in the sub lcd */
        EQS_DEF_EVENT_PROC(pEvent);
        break;
	
	case SE_PH_INUSE_STATE:
		if (SKY_SetThenCompareInUseState((IN_USE_STATE_E)SKY_GetInUseState()))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

	case SE_SYS_TO_IDLE: 
        break;
		
	case SE_CALL_ENDED :
		WAPUsedTime(MID_WAPTIME);
        EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
		break;

    case SE_APP_END :   /* The very last event */

        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
		EQS_ReleaseDC();		
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;

	case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/        	
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;

	case SE_CALL_INCOMING:
        break;

	case SE_SMS_MESSAGE:
        break;

	case SE_DATA_DORM_F:
		break;
    
    case SE_WAP_REPORTERROR:
		EQS_START_SIBLING(sSKA_WAPERROR,FALSE);
		break;

    case SE_CALL_COMMAND_ERROR:
        break;

    case SE_KEY_MANNERS:
        EQS_DEF_EVENT_PROC(pEvent);
        break;

    case SE_IO_TO_IDLE:        
        break;

    default :
            if(pEvent->EventID == SE_KEY_F2 ||pEvent->EventID == SE_KEY_PHONE_CLOSE ||
                pEvent->EventID == SE_KEY_END)
                break;
            else
			EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

#ifdef FEATURE_WAP_WAVELET
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WAVELETMSG_INWAP()
**
** Description: 
**     WAP중 Wavelet script를 실행해서 이어받기 인지 새로받기 인지 물어보는 applet 
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
LOCAL void SKY_API sSKA_WAVELETMSG_INWAP(SKY_EVENT_T *pEvent) //redirect, dormant처리 !!
{
    HCONTROL hControl;
    STATIC HCONTROL hMSGControla; //이어받기
    STATIC HCONTROL hMSGControlb; //새로받기

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(TRUE);
        EQS_GetDC(TRUE);
		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators. 
            WA_SHOW_HEADER);         
        // user의 응답이 없을 때 대비해서 필요한 timer!!
        if(s_WAVELETsaveType == CONNECT_CONTENTS)
        {
            hMSGControla = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAVELET_CONTINUE_DOWNLOAD), 5, MBT_YESNO, MBT_CONFIRM);        
        }
        else if(s_WAVELETsaveType == OLD_CONTENTS)
        {
            hMSGControlb = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAVELET_NEW_DOWNLOAD), 5, MBT_YESNO, MBT_CONFIRM);        
        }
        break;

	case SE_APP_DRAW:        
		break;

    case SE_TM_TIMER1:
		break;

	case SE_SYS_TO_IDLE: 
        break;
	
/* KEY 	*/
	case SE_KEY_PHONE_CLOSE:
	case SE_KEY_END :
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);        
		break;

    case SE_KEY_CLEAR:
        break;

    case SE_IO_TO_IDLE:
        EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;

    case SE_APP_END :   /* The very last event */         
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }
        EQS_ReleaseDC();
    	break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;   

    case SE_IO_MESGBOX_YES:        
        if(pEvent->wParam == hMSGControla)
        {
            MSG_HIGH("이어받음",0,0,0);  
            gDownInfo = CONNECT_CONTENTS;
            EQS_END_EVENT(SE_TOWAVELET, EQS_NA, EQS_NA);           
        }
        else if(pEvent->wParam == hMSGControlb)
        {
            MSG_HIGH("새로받음",0,0,0);  
            gDownInfo = OLD_CONTENTS;
            EQS_END_EVENT(SE_TOWAVELET, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
        SetMOinSO33toMIN();
        SKY_SET_WAP_TO_WAVELET(FALSE);
        EQS_END();               
        break;

/* 까리 까리 !!
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hMSGControl)
        {
            MSG_HIGH("새로 받음",0,0,0);            
            EQS_END_EVENT(SE_TOWAVELET, EQS_NA, EQS_NA);           
        }       
        break;*/


    case SE_CALL_ENDED:
        EQS_END_EVENT(SE_CALL_ENDED, EQS_NA, EQS_NA);
        break;

    case SE_CALL_ORIGINATED:/*초기접속시 종료시킬때 나오는 데이터 서비스연결중을 막기위해*/
        EQS_SetWindowSymbolMode(WTSM_CALL_MODE); //wap접속 상태임을 표시
		break;

    case SE_CALL_CONNECT:
		MSG_ERROR("SE_CALL_CONNECT",0,0,0);        
		WAPUsedTime(START_WAPTIME);
        break;
	    
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif //FEATURE_WAP_WAVELET

/* 
**----------------------------------------------------------------------------
**  LOCAL  Function
**---------------------------------------------------------------------------- 
*/
LOCAL void WAP_Initialize_MEM(void)
{
    extern void AddressAssign(void);
	extern void InitWipMalloc(void);
    AddressAssign();
	InitWipMalloc();	
}

LOCAL VOID Cancell_WAPLoading(void)
{
    if(!(WAEStatus == ContentIsDone || 
                    WAEStatus == ImageIsDone   || 
                    WAEStatus == ScriptIsDone  ||
                    WAEStatus == LoadingDataDone))
        {
            //resetalldata();
            SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;
            MMIc_stop(1);
            ProcessEvents();
            SKY_DrawAnnunciators();
            WAP_Repaint();         
        }
    ;
}


LOCAL void MakeMin_UI(void)
{
   nv_dir_number_type nv_min_num;

   nv_min_num.nam = SKY_GET_CURRENT_NAM();//nv_min_num.nam = ui.nam; 
   (void)SKY_GetNV( NV_DIR_NUMBER_I, (nv_item_type *)&nv_min_num );

   if(nv_min_num.dir_number[0] !='0')//4자리 국번
   {
		strncpy(SaveSelfMin,(char*)nv_min_num.dir_number+2,8);
   }
   else//3자리 국번
   { 
		SaveSelfMin[0] ='0';
		strncpy(&SaveSelfMin[1],(char*)nv_min_num.dir_number+3,7);
   }
}

#ifdef FEATURE_WAPMAIL
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_SendWAPMsg()
**
** Description: 
**     메인메뉴에서 n.Top메일 작성시 시작하는 applet
**
** Input:
**     pEvent
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
** Not yet:
**     1. sms 메세지 삭제에서 모두 or보낼 메세지 삭제에서 적용할것인지 검토      
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_SendWAPMsg(SKY_EVENT_T* pEvent)
{	
	STATIC HCONTROL hMsgControln;
	STATIC HCONTROL hMsgControlab;
	STATIC HCONTROL hMsgControlend;	
	STATIC BOOL s_fEndKey = FALSE;
	BOOL fNewDataFile;
	
    switch(pEvent->EventID)
    {
    case SE_APP_START :
		fNewDataFile = ReadWAPMail();
		EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      100, 
                      fNewDataFile ? CL_NONE_SELECT_SAVE : CL_DELETE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_NTOP_MAIL_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
		
		EQC_StaticText(EQS_NA, EQS_NA, fNewDataFile ? TKN_NTOP_MAIL_NEW : TKN_NTOP_MAIL_OLD);
		EQC_TextEdit(EQS_NA, EQS_NA, 16, 5, TEM_KOREAN, 
			WAPMAIL_SIZE-2, fNewDataFile ?(BYTE*)"" : (BYTE*)g_wapmaildata.Memo, 0);
		EQS_AutoFormatting(WEM_MAX, EHA_LEFT, EVA_CENTER);      
      break;

    case SE_APP_END :     /* The very last event */
		break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */		
        break;

	case SE_SOFTKEY_DELETE:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NTOP_MAIL_DELETE), 2, MBT_YESNO, MBT_CONFIRM);
        break;
    
    case SE_IO_MESGBOX_YES:
		if(pEvent->wParam == hMsgControlab)
		{
			hMsgControlend = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 2, MBT_NONE, MBT_SUCCESS);
            memset((void *)g_wapmaildata.Memo,' ',WAPMAIL_SIZE);
            EQC_GetTextEditData(EQS_GetNthIOC(1), g_wapmaildata.Memo);
			OverWriteWAPMail();
		}
		else
		{
			SKY_DeleteDataFile(SDF_MAIL, (BYTE*)"WAPMAIL");
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DELETE_DONE_MAIL), 2, MBT_NONE, MBT_SUCCESS);
			fNewDataFile = ReadWAPMail();
			EQS_SetWindowCML(fNewDataFile ? CL_NONE_SELECT_SAVE : CL_DELETE_SELECT_SAVE);	
			EQC_SetStaticText( EQS_GetNthIOC(0), (fNewDataFile ? SKY_GET_TOKEN(TKN_NTOP_MAIL_NEW) : SKY_GET_TOKEN(TKN_NTOP_MAIL_OLD)));
			EQC_SetTextEditData(EQS_GetNthIOC(1), fNewDataFile ?(BYTE*)"" : (BYTE*)g_wapmaildata.Memo);
			EQS_AutoFormatting(WEM_MAX, EHA_LEFT, EVA_CENTER);
		}
        break;

    case SE_IO_MESGBOX_NO:
		if(pEvent->wParam == hMsgControlab)
		{
			if(s_fEndKey)
			{
				s_fEndKey = FALSE;
				EQS_END_TO(EQS_POP_TO_ROOT);
			}else
			{
				EQS_End();
			}			
		}	
        break;

    case SE_IO_TEDIT_DONE:
		memset((void *)g_wapmaildata.Memo,' ',WAPMAIL_SIZE);
        EQC_GetTextEditData(pEvent->wParam, g_wapmaildata.Memo);
		break;
		
	case SE_SOFTKEY_CANCEL:
		EQS_End();
		break;

	case SE_SOFTKEY_SAVE:
		if(EQS_IsControlDataChanged())		
		{			
			hMsgControln = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 2, MBT_NONE, MBT_SUCCESS);
			OverWriteWAPMail();			
		}
		else
		{
			EQS_End();		
		}
		break;

	case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hMsgControln)
		{
            EQS_End();
		}
		else if(pEvent->wParam == hMsgControlend)
		{
			if(s_fEndKey)
			{
				s_fEndKey = FALSE;
				EQS_END_TO(EQS_POP_TO_ROOT);
			}else
			{
				EQS_End();
			}
		}
        break;

	case SE_KEY_PHONE_CLOSE:
	case SE_KEY_END:
        if(EQS_IsControlDataChanged())
		{            
			s_fEndKey = TRUE;
			hMsgControlab = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 2, MBT_YESNO, MBT_CONFIRM);
		}
		else
		{
			EQS_END_TO(EQS_POP_TO_ROOT);		
		}
        break;

	case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
			hMsgControlab = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 2, MBT_YESNO, MBT_CONFIRM);
        }
		else
		{
            EQS_End();
		}
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }	
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     ReadWAPMail()
**
** Description: 
**     현재 폰에 wapmail file이 있으면 global buffer에 저장하고 아니면 file이 없음을 나타내는 함수
**
** Input:
**     @ArgumentsList
**
** Output:
**     new wapmail data file을 만들면 true 아니면 false
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL BOOL ReadWAPMail(void)
{	
	HFILE hFile;
	BOOL fNewDataFile = TRUE;

	memset((void *)g_wapmaildata.Memo,' ',WAPMAIL_SIZE);
	if(!SKY_IsDataFile(SDF_MAIL, (BYTE*)"WAPMAIL"))
	{
		
		CLNTa_log(1,0,"wap mail file이 안만들어 졌네??");
		
	}
	else
	{		
		if(SKY_OpenDataFile(SDF_MAIL, (BYTE*)"WAPMAIL", &hFile, SFT_READ) == SDR_OK)
		{
			//SKY_ReadDataBlock(hFile, (BYTE*)g_wapmaildata.Memo, sizeof(g_wapmaildata.Memo));           
            SKY_ReadDataBlock(hFile, (BYTE*)g_wapmaildata.Memo, WAPMAIL_SIZE-2);
            SKY_CloseDataFile(hFile);
        }
		else
		{
			CLNTa_log(1,0,"wap mail file가 Read가 안되네??");
		}
		fNewDataFile = FALSE;
	}

	return fNewDataFile;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     OverWriteWAPMail()
**
** Description: 
**    edit가 끝나고 새wapmail file을 만들어서 저장하던지 기존에 있으면 overwrite     
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
LOCAL VOID OverWriteWAPMail(void)
{	
	HFILE hFile;

	if(!SKY_IsDataFile(SDF_MAIL, (BYTE*)"WAPMAIL"))
	{
		if(SKY_CreateDataFile(SDF_MAIL, (BYTE*)"WAPMAIL", &hFile) == SDR_OK)
		{
			SKY_WriteDataBlock(hFile, (BYTE*)g_wapmaildata.Memo, sizeof(g_wapmaildata.Memo));			
			SKY_CloseDataFile(hFile);			
		}
		else
		{
			CLNTa_log(1,0,"wap mail file이 안만들어 졌네??");
		}
	}
	else
	{	
		if(SKY_OpenDataFile(SDF_MAIL, (BYTE*)"WAPMAIL", &hFile, SFT_WRITE) == SDR_OK)
		{
			SKY_WriteDataBlock(hFile, (BYTE*)g_wapmaildata.Memo, sizeof(g_wapmaildata.Memo));			
			SKY_CloseDataFile(hFile);
		}
		else
		{//초기에 없으면 file을 생성하므로 반드시 있어야 한다.
			CLNTa_log(1,0,"wap mail file에 write가 안되네??");
		}
	}

}
#endif //FEATURE_WAPMAIL

//HCONTROL                    hNTopAnim ;	//초기 animation stop위해
VOID PlayNTopMainWapImage(void)//WAP 초기접속
{
	HFILE						hFile;

	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();

	hNTopAnim = INVALID_CONTROL_HANDLE;
	if(IS_MY_PICTURE(SKY_GET_WAP_PICTURE().wPictureID))    // 나만의 그림
	{
		if(SKY_OpenDataFile(SDF_IMAGE, SKY_GET_WAP_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
        {
			SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

			if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
			{
				SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
				SKY_CloseDataFile(hFile);
                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
				//EQC_Image(0, SKY_ANNUNCIATORS_HEIGHT, &l_BmpBuf);
                
                EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), SKY_ANNUNCIATORS_HEIGHT+GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
			}
			else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
			{
				ANIMATION_HEADER_T AniHeader;
				BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];
                COORD yImageHeight;

                SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
				SKY_CloseDataFile(hFile);
				sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_WAP_PICTURE().szFileName);
    			//hNTopAnim = EQC_Animation(0, SKY_ANNUNCIATORS_HEIGHT, NULL, abBuf);
                hNTopAnim = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), SKY_ANNUNCIATORS_HEIGHT+GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);

				//EQC_SetStartYAnimation(hNTopAnim, SKY_ANNUNCIATORS_HEIGHT);
                if(l_BmpBuf.Header.Size.yHeight > SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT))
                    yImageHeight = SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT)+1;
                else
                    yImageHeight = l_BmpBuf.Header.Size.yHeight;
				EQC_SetAnimationHeight(hNTopAnim, yImageHeight);
                if(SKY_IS_PHONE_OPEN())
					EQC_StartAnimation(hNTopAnim, TRUE);
			}
        }
	}
#ifdef FEATURE_CHARACBELL
    else if(IS_CHARAC_PICTURE(SKY_GET_WAP_PICTURE().wPictureID))    // 나만의 그림
	{
		if(SKY_OpenDataFile(SDF_CHA_IMAGE, SKY_GET_WAP_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
        {
			SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

			if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
			{
				SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
				SKY_CloseDataFile(hFile);
                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
				//EQC_Image(0, SKY_ANNUNCIATORS_HEIGHT, &l_BmpBuf);
                
                EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), SKY_ANNUNCIATORS_HEIGHT+GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
			}
			else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
			{
				ANIMATION_HEADER_T AniHeader;
				BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];
                COORD yImageHeight;

                SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
				SKY_CloseDataFile(hFile);
				sprintf((char *)abBuf, "%s%s%s", (char*)SKY_CHA_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_WAP_PICTURE().szFileName);
    			//hNTopAnim = EQC_Animation(0, SKY_ANNUNCIATORS_HEIGHT, NULL, abBuf);
                hNTopAnim = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), SKY_ANNUNCIATORS_HEIGHT+GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);

				//EQC_SetStartYAnimation(hNTopAnim, SKY_ANNUNCIATORS_HEIGHT);
                if(l_BmpBuf.Header.Size.yHeight > SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT))
                    yImageHeight = SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT)+1;
                else
                    yImageHeight = l_BmpBuf.Header.Size.yHeight;
				EQC_SetAnimationHeight(hNTopAnim, yImageHeight);
                if(SKY_IS_PHONE_OPEN())
					EQC_StartAnimation(hNTopAnim, TRUE);
			}
        }
	}
#endif // FEATURE_CHARACBELL
    else if(IS_MY_PHOTO(SKY_GET_WAP_PICTURE().wPictureID)) // 나만의 사진
	{
		BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 
        DWORD                       dwTimeInfo = 0;

		if(SKY_OpenDataFile(SDF_PHOTO, SKY_GET_WAP_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
        {
			SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
			SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
			SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, IMAGE_VIEWER_TOTAL_SIZE);   
			SKY_CloseDataFile(hFile); 

            //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
			hNTopAnim = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), SKY_ANNUNCIATORS_HEIGHT+GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
        }
	}
	else if(!IS_PICTURE_NOT_USED(SKY_GET_WAP_PICTURE().wPictureID))
	{
		hNTopAnim = EQC_Animation(0, SKY_ANNUNCIATORS_HEIGHT, g_pBuiltInAnimation[SKY_GET_WAP_PICTURE().wPictureID], NULL);	
		//EQC_SetStartYAnimation(hNTopAnim, SKY_ANNUNCIATORS_HEIGHT);
		EQC_SetAnimationHeight(hNTopAnim, SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT+1));
        if(SKY_IS_PHONE_OPEN())
    		EQC_StartAnimation(hNTopAnim, TRUE);
	}
}


LOCAL void send_wapcmd(wap_name_type cmd_name)
{
	if (( wap_ptr = (wap_cmd_type*)q_get( &ui_cmd_free_q )) != NULL) 
	{
		wap_ptr->hdr.task_ptr   = NULL;
        wap_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
	}
	wap_ptr->hdr.cmd = cmd_name;
	send_cmd2wapHandler(wap_ptr);
}

void WAP_READ_INITDATA(void)
{
    int tmp_length = 0;

    tmp_length = get_length((byte *)SKY_GET_WAP_PROXY_URL(),130);
	memset(gWAPproxy_url,'\x00',130);
	memcpy(gWAPproxy_url,(void *)SKY_GET_WAP_PROXY_URL(),tmp_length);

    //copy to global url var
    tmp_length = get_length((byte *)SKY_GET_WAP_INIT_URL(),130);
    memset(gWAPinit_url,'\x00',130);
	memcpy(gWAPinit_url,(void *)SKY_GET_WAP_INIT_URL(),tmp_length);

    //copy to global wap mail address var
    tmp_length = get_length((byte *)SKY_GET_WAP_INIT_MAIL(),130);
	memset(gWAPEmail_Url,'\x00',130); //for using PUSH
	memcpy(gWAPEmail_Url,(const void *)SKY_GET_WAP_INIT_MAIL(),tmp_length);

#ifdef FEATURE_WAPMAIL
	if(!ReadWAPMail()) //wap mail file이 있으면 내용을 global buffer에 copy
	{
        MSG_HIGH("sendMail here",0,0,0);			
	}		
#endif //FEATURE_WAPMAIL
    memset((void *)SaveSelfMin,0x00,8);
	MakeMin_UI();

}


  
    


/*
**   현재 Call이 Data Call인지 아닌지를 판별해 주는 함수 
*/
#ifdef INCOMING_CALL_IN_WAP
BOOL SKY_API SKY_IS_DATA_CALL(void)
{
    if((GET_EVENT_PARAM()->call_event).call_info.call_type == CM_CALL_TYPE_DATA)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif //#ifdef INCOMING_CALL_IN_WAP

LOCAL VOID process_rcv_sms(void)
{
	if(g_pCard != NULL)
	{
		CLNTa_log(1, 0, "이전 카드 있음");InWapMailorPush = 2;
	}
	else//카드가 없음 = 초기 URL 접속중이었다는 의미
	{
		CLNTa_log(1, 0, "이전 카드 없음 ");	InWapMailorPush = 1;
	}	
	CLNTa_log(1,0,"GO SMS state");
}
   


#ifdef FEATURE_WAP_WAVELET
void wap_display_exit_for_wavelet(void)
{
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "Wavelet을",WAP_TEXT_ALIGN_CENTER); 	
	WAP_SetTextAtLine(3, "시작합니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

void GoToWAVELET( int cpid, int ctsid, char* sndIP, int sndPort, 
                int MO, int SO, int svcType, int skinType, char* cntName,
                int cntProtect, int cntExpDate, char* retURL, char* Result )
{   

	memset(Result, NULL, sizeof(Result) );
    strcpy(Result, "0");

    if( sndIP == NULL || cntName == NULL || retURL == NULL  )		
	{
        memset(Result, NULL, sizeof(Result) );
		strcpy(Result, "1003");
		ERR("Invalid Parameter",0,0,0);
		return;
	}

    if( cpid < 0 || ctsid < 0 || sndPort < 0 || MO < 0 ||
        SO < 0 || svcType <= 0 || skinType < 0 ||
        cntProtect < 0 || cntExpDate < 0 ||
		sndPort > 0xffff || svcType > 5 ||
        skinType > 0xff || cntProtect > 0xff ||
        cntExpDate > 0xff )
	{
        memset(Result, NULL, sizeof(Result) );
		strcpy(Result, "1003");
		ERR("Invalid Parameter",0,0,0);
		return;
	} 
    /* copy the data of script */
    g_wcpid = cpid;                                         /* Content Manufacturer ID*/
    g_wctsid = ctsid;                                       /* Content ID*/

	memset((char*)g_wsndIP, 0x00, 101 );
	strncpy((char*)g_wsndIP, (const char*)sndIP, 100 );     /* Server IP Address */

    g_wsndPort = sndPort;                                   /* Server Port */

    if(P_REV_IS_6_OR_GREATER && ((MO != 0x911) &&(MO != 0x921)))
    {
        strcpy(Result, "1003");
		MSG_ERROR("Wrong Mux option Invalid Parameter",0,0,0);
		return;
    }    
    g_wMO = MO;                                             /* Mux Option */
    if(P_REV_IS_6_OR_GREATER && (SO != 33))
    {
        strcpy(Result, "1003");
		MSG_ERROR("Wrong Service option Invalid Parameter",0,0,0);
		return;
    }
    g_wSO = SO;                                             /* Service Option*/
    g_wsvcType = svcType;                                   /* Play Type */
    g_wskinType = skinType;                                 /* Skin ID */

	memset((char*)g_wcntName, 0x00, 21 );
	strncpy((char*)g_wcntName, (const char*)cntName, 20 );  /* File Name*/
    
    g_wcntProtect = cntProtect;                             /* Number of Playing */
    g_wcntExpDate = cntExpDate;                             /* Time of Using Content */

	memset((char*)g_wretURL, 0x00, 101 );
	strncpy((char*)g_wretURL, (const char*)retURL, 100 );    /* Return URL*/

	
    SKY_SET_WAP_TO_WAVELET(TRUE);
	/* in order to go to Wavelet */
    EQS_SEND_EVENT(SE_WAVELET_IN_WAP, EQS_NA, EQS_NA);
//    EQS_SEND_EVENT(SE_TOWAVELET, EQS_NA, EQS_NA);

}

#endif //#ifdef FEATURE_WAP_WAVELET


/*	Function: DevGetTime
 *	Returns the time of day.
 *
 *	Args: none
 *
 *	Returns: number of seconds since January 1 1970
 *
 */
unsigned long DevGetTimeOfDay(void)
{
	unsigned long result = 0L;

	/*
	** ELAPSED_SECONDS - 1970 Jan 1st 00:00:00 ~ 1980 Jan 6th 00:00:00
	** clk_read_secs() - 1980 Jan 6th 00:00:00 ~ very now
	*/
	result = ELAPSED_SECONDS + SKY_GetLocalTime();
	MSG_HIGH("TDO Time %d",result,0,0);
	return result;  //sec
}

/*-------------------------------------------------------------------
void WAPUsedTime(char UsedTimeType)
    Used to record the online time 

	ENTER_S : Initialize parameters releating to time record
	START: UI_CONNECTED_F
	MID  : UI_NETDISCONNECTED_F, UI_RELEASE_F, 
	       UI_MAKECALL_F, UI_PUSHMAIL_F
	END  : Record the accumulated connect time to NV and Initialize
	EXIT_S  : the time when WAPUsedTime(END) is called

--------------------------------------------------------------------*/
unsigned long gUsedTime = 0;
unsigned long gStartEnd = 0;

void WAPUsedTime(char UsedTimeType)
{
	static unsigned long gTimeStamp, gMidTime = 0 ;
#ifdef WAPDORMANT
	static boolean second_display = FALSE;
#endif//wapdormant
	extern unsigned long DevGetTimeOfDay(void);
	extern void ntop_used_time ( dword new_time );

	switch(UsedTimeType)
	{
	case INIT_WAPTIME:
		gStartEnd = gTimeStamp = gMidTime = gUsedTime = 0;
		gStartEnd = DevGetTimeOfDay();
#ifdef WAPDORMANT
		second_display = FALSE;
#endif
		break;
	case START_WAPTIME:
		gTimeStamp = DevGetTimeOfDay();
		MSG_HIGH("시작[%d]", gTimeStamp,0,0);
		break;
	case MID_WAPTIME:   
		if(gTimeStamp == 0) return;
		gMidTime  = DevGetTimeOfDay() - gTimeStamp ;
		MSG_HIGH("중간[%d]초", gMidTime,0,0);
		gUsedTime   += gMidTime;
		gTimeStamp   = gMidTime = 0;
		break;
	case END_WAPTIME:
		MSG_HIGH("사용[%d]초 접속[%d]초", gStartEnd,gUsedTime,0);
#ifdef WAPDORMANT
		if(second_display) return;
		else second_display = TRUE;
#endif//dormant
		if(gStartEnd == 0) return;
		gStartEnd = DevGetTimeOfDay() - gStartEnd;

		if(gUsedTime == 0) return;
		ntop_used_time(gUsedTime);
		break;
	}
}

LOCAL VOID InitalizeWAPvariable(void)
{
    SKY_SET_IN_WAP(TRUE); //ui.wap = TRUE;
    SetKindOfService(WAP); //for socket
#ifdef WAPDORMANT
	gbDormant   = FALSE;
	gb2GHandoff = FALSE;
#endif//wapdormant
#ifdef FEATURE_SKIPDATA
	g_bSkipData = FALSE;
#endif//feature_skipdata
    s_initWAPACCESSFail = FALSE;

#ifdef FEATURE_WAP_REDIRECT
    gredirectloadingbar = FALSE;
#endif //#ifdef FEATURE_WAP_REDIRECT

	g_bWAPinitAccessFail = FALSE;
    g_WAPfirstDisplay = 0;
	first_flag_tid = 0;
    start_disp = TRUE;
    s_towhere = WAPGO_NONE;
    s_towhereminor = WAPGOMINOR_NONE;
    g_KindofPUSH = 0;
#ifdef FEATURE_WAP_MUXOPTION    
	fMUX_Option = FALSE;
#endif //FEATURE_WAP_MUXOPTION
#ifdef FEATURE_SKT_EIF
	if(g_WAPSuccess != 0)
		g_WAPSuccess = 0;	//init

	if(g_EIF_WAP_WAPCON)
		g_EIF_WAP_WAPCON = FALSE;//init

    if(g_EIF_SEND_EXIT)
    {
        g_EIF_SEND_EXIT = FALSE;
        g_EIFWAPExit = TRUE;
    }
#endif  //#ifdef FEATURE_SKT_EIF
}


#endif //FEATURE_WAP#		