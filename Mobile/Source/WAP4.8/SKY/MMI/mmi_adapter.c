
//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
#include "customer.h" // jwpark 00.04.27

#include <string.h>
#include <stdio.h>

//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////

#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"

//for log
#include "aapiclnt.h"
#include "clnt.h"
#include "errcodes.h"
#include "msg.h"
#include "err.h"

// for Queue
#include "queue.h"
#include "wapadapter.h"

//-----------------------------------------------------------------------------------
// MMI include file
//-----------------------------------------------------------------------------------
#include "sky.h"
#include "ska.h"
#include "GenDsp.h"
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "wapcommon.h"

////////////////////////////////////////////////////////////////////
extern void CancelAnimationTimer(void);
extern void RestartAnimationTimer(unsigned long ms);
extern void CancelNowrapTimer(void);
////////////////////////////////////////////////////////////////////

static int g_nTextFontType;//for dual fonts

static int nParagraphAlign = WAP_TEXT_ALIGN_LEFT;
static BOOL bFirstParagraph = TRUE;
//-----------------------------------------------------------------------------------
//
// MMI 의 Data structure
//
//-----------------------------------------------------------------------------------

char tempurlbuffer[200];

BOOL g_bTerminatedWAP = FALSE;

#ifdef FEATURE_CHARACTER_BELL
BOOL g_fCharacbellPreview = FALSE;
extern unsigned char    *CBMelodyData;
extern UINT16           s_nDecodedImageSize;
//extern unsigned char *CBImageData;
#endif // FEATURE_CHARACTER_BELL

/*
** Flag for checking display 
*/
//boolean isDisplayError = FALSE;
static ui_cmd_type   *ui_cmd_ptr;                        /* to hold the UI cmd. */
static int g_nKeyNum_For_SKT;

//-------------------------------------------------------------
//						Table
//-------------------------------------------------------------
BOOL bTable = FALSE;//init

static int nCurTableColumn = 0;//init
LOCAL int nTotalTableColumns = 0;//init

WAP_Content_TableRow *g_pCurTableRow = NULL;//init
extern void WAP_Building_Table(int type, void *pContent, int nFontType);

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
/*																									*/
/*								SKY    M M I   Adapter Fuctions										*/
/*																									*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
extern void report_event(wap_name_type name);

void    MMIa_wait (unsigned char viewId, char isWait)
{
	//===============================================
    // MMI - SKY
    //void SKY_wait (UINT8 viewId, BOOL isWait) //SKY_wait(viewId, isWait);
	//===============================================

	CLNTa_log(1,0,"MMI WAIT called  %d",isWait);
	g_bBlocked = isWait;
}

BOOL g_bDrawBar;
extern UINT8 g_nAniBarCount;
static BOOL s_SESSION_DONE = FALSE;
unsigned int WAEStatus = 0;

BOOL g_bImageLoading = FALSE;//init for Image loding (scroll)

//To avoid redundant data from server which don't have ACK
#ifdef FEATURE_SKIPDATA
boolean g_bSkipData = FALSE;
#endif

void    MMIa_status (unsigned char viewId, unsigned char status, const char *URL)
{
    CLNTa_log(1,0,"view[%d] status[%d] URL [%s]",viewId, status, URL);

	//===============================================
    // MMI - SKY
    //void SKY_status (UINT8 viewId, UINT8 status, const CHAR *URL) ////SKY_status(viewId, status, URL);
	//===============================================

	switch(WAEStatus = status)
	{
		case ContentIsOpened:                    
		case ImageIsOpened:
		{
            SKY_SET_NO_OFF_BACKLIGHT(TRUE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

			if(WAEStatus == ImageIsOpened)
				g_bImageLoading = TRUE;	// start image loading

			//start to draw progressive bar
			CLNTa_log(1,0," ContentIsOpened/ ImageIsOpened");            
            if(s_SESSION_DONE)
            {
                displayloading(1);
            }
			break;
		}
		case ContentIsDone:
			CLNTa_log(1,0," ContentIsDONE");
            SKY_SET_NO_OFF_BACKLIGHT(FALSE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());	
			
			g_bImageLoading = FALSE;	//init

            #ifdef BLOCK
            SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
            #endif //block
            SKY_SetGeneralTimer(0, FALSE);
            g_nAniBarCount=6;
            process_draw_bar();
            g_nAniBarCount=0;
			g_bDrawBar=FALSE;
          #ifdef FEATURE_SKIPDATA
            g_bSkipData = TRUE;
          #endif
			break;

		case ImageIsDone:
			CLNTa_log(1,0," ImageIsDONE");
            SKY_SET_NO_OFF_BACKLIGHT(FALSE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

			g_bImageLoading = FALSE;	//init

			//report_event(WAP_DRAW_STOP_EVENT);
            //bar_ani_stop();
            // ytchoi 2001/11/03 bar수정
            #ifdef BLOCK
            SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
            #endif //block
            SKY_SetGeneralTimer(0, FALSE);
            g_nAniBarCount=6;
            process_draw_bar();
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;
            //clk_dereg(&wapbar_timer);
          #ifdef FEATURE_SKIPDATA
            g_bSkipData = TRUE;
          #endif
			break;

		case ScriptIsRunning:
			CLNTa_log(1,0," ScriptIsRunning");
			break;

		case ScriptIsDone:
		{
			//stop to draw progressive bar
			CLNTa_log(1,0," ScriptIsDone");
            SKY_SET_NO_OFF_BACKLIGHT(FALSE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
			#ifdef BLOCK
            SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
            #endif //block
            SKY_SetGeneralTimer(0, FALSE);
            g_nAniBarCount=6;
            process_draw_bar();           
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;
			break;
		}
		
		case Redirect: 
			CLNTa_log(1,0,"Redirect");
			break;
		case ReadFromCache:
			CLNTa_log(1,0,"ReadFromCache");			
			break;

		case ReadFromNetwork: 
			CLNTa_log(1,0,"ReadFromNetwork");		
          #ifdef FEATURE_SKIPDATA
            g_bSkipData = FALSE;
          #endif
			
			break;

		case CheckForNewerContent: 
			CLNTa_log(1,0,"CheckForNewerContent");			
			break;

		case ReadFromNetworkDone: 
			CLNTa_log(1,0,"ReceivedFromNetwork");	
          #ifdef FEATURE_SKIPDATA
            g_bSkipData = TRUE;
          #endif
			
			break;

		case WSPSessionIsSetup:
			 CLNTa_log(1,0," Session setup");
          #ifdef FEATURE_SKIPDATA
            g_bSkipData = FALSE;
          #endif
			 break;

		case WSPSessionIsDone:
             s_SESSION_DONE = TRUE;
             displayloading(1);
			 CLNTa_log(1,0," Session DONE");
			 break;

		case LoadingData:             
			 CLNTa_log(1,0," LoadingData");
             //displayloading(1);
			 break;
		case LoadingDataDone:   
			CLNTa_log(1,0," LoadingDataDone");
			break;

        case SKTEncryptionIsRunning:
            displayloading(6);
            CLNTa_log(1,0," SKTEncryptionIsRunning");
            break;

        case SKTEncryptionIsRunningDone:
            #ifdef BLOCK
            SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
            #endif //block
            SKY_SetGeneralTimer(0, FALSE);
            g_nAniBarCount=6;
            process_draw_bar();
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;
            CLNTa_log(1,0," SKTEncryptionIsRunningDone");
            break;
	};//end of switch
}

void    MMIa_unknownContent (unsigned char viewId, const char *data, unsigned short length, const char *contentType, const char *URL)
{
	extern int clnt_check_mime(const char *mime);
	int cert_mime=0;
	char *pMime=0;
	//===============================================
    // MMI - SKY
    //void SKY_unknownContent (UINT8 viewId, const CHAR *data, UINT16 length, const CHAR *contentType, const CHAR *URL) //    //SKY_unknownContent(viewId, data, length, contentType, URL);
	//===============================================
	cert_mime = clnt_check_mime(contentType);

	switch(cert_mime)
	{
		case CERT_RESPONSE_TYPE:
		case HASHED_CA_CERT_RESPONSE_TYPE:
		case SIGNED_CA_CERT_RESPONSE_TYPE:
				//Save certificate mime to global buffer after checking capability
				//Stop and report error, if there is no room for saving certificate(over 4096bytes)
				if(clnt_save_cert_mime(data, (int)length))
				{
					wap_set_error(TRUE);
					wap_handle_error(ERR_MMI_UNKNOWN_CONTENT, ERRTYPE_SKTT);                    
				}
                else
                {
				     //Send event to wap for further MMI. For example "You successfully downloaded certificate.
				     EQS_SEND_EVENT(SE_MW_CERT_MIME, cert_mime, EQS_NA);
                }
				//swjung
				//Same taks ^^;
				//clnt_send_cert_mime(cert_mime); 
				break;
		case UNKNOWN_TYPE:
		default:
			wap_set_error(TRUE);
			wap_handle_error(ERR_MMI_UNKNOWN_CONTENT, ERRTYPE_SKTT);
			break;
	}
}

int g_nDlgId;
char *g_pDlgMessage = NULL;
char *g_pDlgMessage2 = NULL;
extern void WAP_Build_Dialog(void);


VOID    MMIa_passwordDialog (UINT8 viewId, UINT16 dialogId, const CHAR *realm, INT8 type)
{
	//===============================================
    // MMI - SKY
    //void SKY_passwordDialog (UINT8 viewId, UINT8 dialogId, const CHAR *realm, INT8 type)	//SKY_passwordDialog(viewId, dialogId, realm, type);
	//===============================================

	//--------------------------------------------------------
	g_bDrawBar = FALSE;
	g_nDlgId   = dialogId;
	g_nDlgMode = WAP_DLG_MODE_PASSWORD;	

	//yjahn96 01.11.28
    //sSKA_WAPDIALOG_ID()로 이동
    s_towhereminor = WAPGOMINOR_DI_ID;
    EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);
}

void ReplaceCtrlChar2Space(char *pString)
{
	int i = 0;
	int len = strlen(pString);

	while(len > i)
	{
		if(pString[i] < 0x20) pString[i] = 0x20;//space
		i++;
	}
}

void    MMIa_promptDialog (unsigned char viewId, unsigned char dialogId, const unsigned short *message, const unsigned short *defaultMessage)
{
	//===============================================
    // MMI - SKY
    //void SKY_promptDialog (UINT8 viewId, UINT8 dialogId, const WCHAR *message, const WCHAR *defaultMessage)	//SKY_promptDialog(viewId, dialogId, message, defaultMessage);
	//===============================================

	//--------------------------------------------------------
	g_bDrawBar = FALSE;
	g_nDlgId = dialogId;
	g_nDlgMode = WAP_DLG_MODE_PROMPT;
	

	if(g_pDlgMessage) wip_free((void *)g_pDlgMessage);
	if(message == NULL)	g_pDlgMessage = NULL;
	else
	{
		signed short length;
		length = KSCStrLenOfUni((WCHAR *)message);
	    g_pDlgMessage = (char *)wip_malloc(sizeof(char)*(length+1));
		memset(g_pDlgMessage, 0x00, length+1);
		Uni2KSCString((WCHAR *)message, (unsigned char *)g_pDlgMessage);

		/* Free wap core allocated memory */
		//WIPFree((void *)message);
	}
	ReplaceCtrlChar2Space(g_pDlgMessage);

	if(g_pDlgMessage2) wip_free((void *)g_pDlgMessage2);
	if(defaultMessage == NULL)	g_pDlgMessage2 = NULL;
	else
	{
		signed short length;
		length = KSCStrLenOfUni((WCHAR *)defaultMessage);

    	g_pDlgMessage2 = (char *)wip_malloc(sizeof(char)*(length+1));

		memset(g_pDlgMessage2, 0x00, length+1);
		Uni2KSCString((WCHAR *)defaultMessage, (unsigned char *)g_pDlgMessage2);

		/* Free wap core allocated memory */
		//WIPFree((void *)defaultMessage);
	}
	ReplaceCtrlChar2Space(g_pDlgMessage2);

	//yjahn96 01.11.28
    //sSKA_WAPDIALOG_PROMPT()로 이동
    s_towhereminor = WAPGOMINOR_DI_PROMPT;
    EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);
}


void    MMIa_confirmDialog (unsigned char viewId, unsigned char dialogId, const unsigned short *message, const unsigned short *ok, const unsigned short *cancel)
{
	//===============================================
    // MMI - SKY
    //void SKY_confirmDialog (UINT8 viewId, UINT8 dialogId, const WCHAR *message, const WCHAR *ok, const WCHAR *cancel)	//SKY_confirmDialog(viewId, dialogId, message, ok, cancel);
	//===============================================

	//--------------------------------------------------------
	g_bDrawBar = FALSE;
	g_nDlgId = dialogId;
	g_nDlgMode = WAP_DLG_MODE_CONFIRM;
	

	if(g_pDlgMessage != NULL) wip_free((void *)g_pDlgMessage);
	if(message == NULL)	g_pDlgMessage = NULL;
	else
	{
		signed short length;
		length = KSCStrLenOfUni((WCHAR *)message);
		g_pDlgMessage = (char *)wip_malloc(sizeof(char)*(length+1));
		memset(g_pDlgMessage, 0x00, length+1);
		Uni2KSCString((WCHAR *)message, (unsigned char *)g_pDlgMessage);

		/* Free wap core allocated memory */
		//WIPFree((void *)message);
	}
	ReplaceCtrlChar2Space(g_pDlgMessage);
	WAP_Build_Dialog();
}

void    MMIa_alertDialog (unsigned char viewId, unsigned char dialogId, const unsigned short *message)
{
	//===============================================
    // MMI - SKY
    //void SKY_alertDialog (UINT8 viewId, UINT8 dialogId, const WCHAR *message)	//SKY_alertDialog(viewId, dialogId, message);
	//===============================================

	//--------------------------------------------------------
	g_bDrawBar = FALSE;
	g_nDlgId = dialogId;
	g_nDlgMode = WAP_DLG_MODE_ALERT;
	

	if(g_pDlgMessage != NULL) wip_free((void *)g_pDlgMessage);

	if(message == NULL)	g_pDlgMessage = NULL;
	else
	{
		signed short length;
		length = KSCStrLenOfUni((WCHAR *)message);						 
		g_pDlgMessage = (char *)wip_malloc(sizeof(char)*(length+1));
		memset(g_pDlgMessage, 0x00, length+1);
		Uni2KSCString((WCHAR *)message, (unsigned char *)g_pDlgMessage);

		/* Free wap core allocated memory */
		//WIPFree((void *)message);
	}
	ReplaceCtrlChar2Space(g_pDlgMessage);	
	WAP_Build_Dialog();
}

BOOL savefirst = TRUE;
BOOL firstsite;
extern   CHAR temp_url[200];
static   char saveUrl[200];

#ifdef FEATURE_MUSIC_BELL
	//===========================
	// for Music Bell
	BOOL g_bStopMusic = FALSE;
	extern void musicbell_check(void);
	//===========================
#endif//#ifdef FEATURE_MUSIC_BELL
 
BOOL g_bBuildingNewCard;
void    MMIa_newCard (unsigned char viewId, const unsigned short *title, char isList, char isRefresh, const char *URL, char isBookmarkable, const unsigned short * const *history)
{
	signed short	lenOfTitle;
	char	*pTitle;

	signed short	lenOfURL;
	char			*pURL;

	if(title == NULL)
	{
		pTitle = NULL;
	}
	else
	{
		lenOfTitle = KSCStrLenOfUni((WCHAR *)title);
		pTitle = (char *)wip_malloc(lenOfTitle+1);
		memset(pTitle,0x00,lenOfTitle+1);
		Uni2KSCString((WCHAR *)title, (unsigned char *)pTitle);
	}
		
	//URL
	if(URL == NULL)
	{
		pURL = NULL;
	}
	else
	{
		extern char tempurlbuffer[];

		lenOfURL = strlen(URL);
		pURL = (char *)wip_malloc(lenOfURL+1);
		memset(pURL,0x00,lenOfURL+1);
		strcpy(pURL, URL);

		memset(tempurlbuffer,0x00,200);
		if(lenOfURL <200) 
		{
			strncpy(tempurlbuffer,URL,lenOfURL);
		}
		else
		{
			strncpy(tempurlbuffer,URL,200);
		}
	}

	MSG_HIGH("SKY_newCard",0,0,0);

	if(g_bTerminatedWAP) return;

	g_bBuildingNewCard = TRUE;

#ifdef FEATURE_MUSIC_BELL	
	//============================================================
	// for Music Bell (새 카드로 이동시, 음악을 멈춘다)
	g_bStopMusic = TRUE;
	musicbell_check();
#endif//#ifdef FEATURE_MUSIC_BELL

#ifdef FEATURE_CHARACTER_BELL
    g_bStopMusic = TRUE;
    if(g_fCharacbellPreview) {
        SKY_StopMusic();
        g_fCharacbellPreview = FALSE;
    }
#endif // FEATURE_CHARACTER_BELL

	CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP
	CancelNowrapTimer();    //	ui_nowrap_set(0, FALSE);//stop nowrap //yjahn96 00.10.27
#endif//#ifdef WAP_NOWRAP
	
	if(savefirst)
	{
		memset(saveUrl,0x00,200);
		if(strstr(pURL,gWAPinit_url)!=NULL)
		{
			if(strlen(pURL) < 200) strncpy(saveUrl,pURL,strlen(pURL));
			else strncpy(saveUrl,gWAPinit_url,200);

			savefirst = FALSE;         
		}		
	}
	
	if(g_pCard != NULL)
	{
		if(WAP_Terminate_WML() == FALSE) return;
	}

	g_pCard = (WAP_Card *) wip_malloc(sizeof(WAP_Card));

	g_pCard->viewId			= viewId;
    g_pCard->title			= (CHAR *)pTitle;
    g_pCard->isList			= (BOOL)isList;	 
    g_pCard->isRefresh		= (BOOL)isRefresh; 
	g_pCard->URL			= (CHAR *)pURL;
	g_pCard->isBookmarkable	= isBookmarkable;
	g_pCard->history		= NULL;//history;
    g_pCard->pHeadContent	= NULL;
    g_pCard->pHeadFocus		= NULL;

	g_pCurFocusNode			= NULL;
	g_pCurSelect			= NULL;
	
	g_nViewMode				= WAP_VIEW_MODE_WML;
	g_nMenuMode				= WAP_MENU_MODE_NONE;
	g_nDlgMode				= WAP_DLG_MODE_NONE;

	g_nLCDOffset 			= 0;// 단말기의 LCD Height

	g_nKeyNum_For_SKT		= 0;
	bFirstParagraph			= TRUE;

	g_nTextFontType = WAP_FONT_NORMAL;
	WAP_Building(_ELEMENT_TYPE_CARD, g_pCard, WAP_FONT_NORMAL);
}

//extern boolean g_bGettingContent;//picture mate/music bell
extern boolean g_bSavingContent;
extern boolean Check_Play_SIS_Animation(void);

void    MMIa_showCard (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_showCard (UINT8 viewId)	//SKY_showCard(viewId);
	//===============================================

// ==========================================================================================
// zeus(yjchoi) 00.12.04
// EIF WAP (External Interface for WAP)에서 
// uopen, newpage command 수행을 위한 플래그 변수들 
#ifdef FEATURE_SKT_EIF	//EIF_WAP
	extern BOOL g_EIF_WAP_UOPEN;
	extern BOOL g_EIF_WAP_NEWPAGE;
	extern BOOL g_NKey_UOPEN;
	extern BOOL g_Link_Text_UOPEN;
	extern int g_WAPSuccess;
	extern BOOL g_EIF_WAP_WAPCON;
#endif  //#ifdef FEATURE_SKT_EIF

// ==========================================================================================

	extern boolean isErrorState;
    extern BOOL g_fCurrentSate_WAPEXIT;

	MSG_HIGH("SKY_showCard[%d][%d][%d]",g_fCurrentSate_WAPEXIT,isErrorState,g_bSavingContent);
	//------------------------------------------------
	//stop animation
	g_bDrawBar = FALSE;
	//-------------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if( (g_pCard == NULL) ||(g_bTerminatedWAP) )
	{
		MSG_ERROR("SHOW CARD error",0,0,0);
		return; 
	}
		
	//------------------------------------------------
	//for Formatting
	WAP_Formatting();	

#ifdef WAP_NOWRAP
	SetFocusNode(g_pCurFocusNode);
	WAP_CheckKeyCard();
#endif//#ifdef WAP_NOWRAP

	g_bBuildingNewCard = FALSE;
	g_bImageLoading = FALSE;	//init
    
	if (isErrorState ||
        g_bSavingContent)//g_bGettingContent) 
        return;
	
	if(Check_Play_SIS_Animation() == TRUE)
	{
		MSG_ERROR("SIS Timer Start",0,0,0);
		RestartAnimationTimer(600);    //		ui_sistimer_set(200, TRUE);
	} 

	//--------------------------------------------------------
	// 이미지 저장 선택시, 로딩 중 End키로 wap종료시 화면 문제 해결
	if(g_fCurrentSate_WAPEXIT != TRUE)	//yjahn96 01.06.28
	{
        //ytchoi
        if(!g_WAPfirstDisplay)
        {
            g_WAPfirstDisplay = 1;
            SKY_DrawAnnunciators();
        }
	    WAP_Display();
    }

	SKY_ERROR_TONE(); //yjahn96 01.10.24

	if(strcmp(tempurlbuffer,saveUrl)==0)
	{
		firstsite = TRUE;
	}
    else
    {
        firstsite = FALSE;
    }



// ==========================================================================================
// zeus(yjchoi) modified 00.12.04
// for EIF_WAP (External Interface for WAP) 에서 newpage, uopen command 를 위해 
// 현재 AU Browser가 가지고 있는 data structure를 parsing 하고 출력한다. 

#ifdef FEATURE_SKT_EIF	//EIF_WAP
	if(g_WAPSuccess == 1)
		g_WAPSuccess = 2;

	if(g_EIF_WAP_WAPCON == TRUE)
    {
		extern void eif_error_command(int);
		extern char resultBuf[4096];
		extern char EIF_url[192];
		extern char EIF_proxyurl[192];

        g_EIF_WAP_WAPCON = FALSE;    
		memset(resultBuf, 0x00, sizeof(resultBuf));

		sprintf(resultBuf, "WAPCON:Connected(%s,%s)", EIF_proxyurl, EIF_url);
		at_send_extended_text(resultBuf);

		CLNTa_log(1,0, "WAPCON: Success!!");
    }

	g_NKey_UOPEN = FALSE;
	g_Link_Text_UOPEN = FALSE;

	// EIF 에서 TE2로부터 NEWPAGE 모드가 설정되어 있다면 
	// TE2로부터 NEWPAGE 모드 설정 해제가 있을 때까지 
	// 계속 새롭게 수신되는 WML 데이터를 TE2 로 보낸다.
	if (g_EIF_WAP_NEWPAGE)			
	{
		extern void WAP_Parsing_MMI(BOOL);
		WAP_Parsing_MMI(FALSE);
	}

	// EIF 에서 TE2로부터 UOPEN 명령어가 실행되었으면 
	// 지정된 URL을 open 하고 받은 수신 데이터를 TE2로 보낸다.
	// UOPNE은 단발성 명령어이므로 한번 수행 후 플래그 변수를 다시 FALSE로 세팅한다. 
	if (g_EIF_WAP_UOPEN)				
	{
		extern void WAP_Parsing_MMI(BOOL);
		WAP_Parsing_MMI(TRUE);
		g_EIF_WAP_UOPEN = FALSE;
	}
#endif // end of #ifdef EIF_WAP
// ==========================================================================================
#ifdef FEATURE_CHARACTER_BELL
    if (g_fCharacbellPreview) {
        SKY_PlayMusicBufferEx((byte*)CBMelodyData, SKY_GET_BELL_VOLUME(), 0);     //play music in buffer
    }
#endif // FEATURE_CHARACTER_BELL
#ifdef FEATURE_WAP_REDIRECT   
    if(gredirectloadingbar)
    {
        gredirectloadingbar = FALSE;
    }
#endif //#ifdef FEATURE_WAP_REDIRECT
    // For Stop 5분 timer  
    EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);

}

void    MMIa_cancelCard (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_cancelCard (UINT8 viewId)	//SKY_cancelCard(viewId);
	//===============================================
}

void    MMIa_newKey (unsigned char viewId, unsigned char keyId, const unsigned short *eventType, const unsigned short *label, char isOptional)
{
	//===============================================
    // MMI - SKY
    //void SKY_newKey (UINT8 viewId, UINT8 keyId, const WCHAR *eventType, const WCHAR *label, BOOL isOptional)	//SKY_newKey(viewId, keyId, eventType, label, isOptional);
	//===============================================

	WAP_Content_Key* pContent;
	int		length;
	char	*eventType_ptr, *label_ptr;

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL)
	{
		MSG_ERROR("NEW KEY Error",0,0,0);
		return; 
	}

	if(eventType == NULL) eventType_ptr = NULL;
	else
	{
		//char *p;

		length = KSCStrLenOfUni((WCHAR *)eventType);
		eventType_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(eventType_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)eventType, (unsigned char *)eventType_ptr);

		/*for(p=eventType_ptr; p < pContent->eventType+length; p++)
		{
			if(isupper(*p)) tolower(*p);
		}*/
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)eventType);
	
	if(label == NULL) label_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)label);
		label_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(label_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)label, (unsigned char *)label_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)label);		

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Key*) wip_malloc(sizeof(WAP_Content_Key));
		
	pContent->viewId	= viewId;
	pContent->keyId		= keyId;
	pContent->eventType	= eventType_ptr;
	pContent->label		= label_ptr;
	pContent->isOptional= isOptional;

	//------------------------------------------------------------------
	// SK Telecom의 Do element(키)를 위한 Numbering
	if(strncmp(pContent->eventType, "vnd.up", 6) == 0
	|| strncmp(pContent->eventType, "VND.UP", 6) == 0)
	{
		pContent->nSKT_Number = WAP_KEY_SK_UP;//상위
	}
	else if(strncmp(pContent->eventType, "vnd.prev", 8) == 0
		 || strncmp(pContent->eventType, "VND.PREV", 8) == 0)
	{
		pContent->nSKT_Number = WAP_KEY_SK_PREV;//이전
	}
	else if(strncmp(pContent->eventType, "vnd.next", 8) == 0
		 || strncmp(pContent->eventType, "VND.NEXT", 8) == 0)
	{
		pContent->nSKT_Number = WAP_KEY_SK_NEXT;//다음
	}
	else if(strncmp(pContent->eventType, "vnd.skmn", 8) == 0
		 || strncmp(pContent->eventType, "VND.SKMN", 8) == 0)
	{
		g_nKeyNum_For_SKT++;
		pContent->nSKT_Number = g_nKeyNum_For_SKT;//숫자(1-9)
	}
	else if(strncmp(pContent->eventType, "vnd.call", 8) == 0
		 || strncmp(pContent->eventType, "VND.CALL", 8) == 0)
	{
		pContent->nSKT_Number = WAP_KEY_SK_CALL;//전화
	}
	else
	{
		pContent->nSKT_Number = -1;
	}

	//------------------------------------------------------------------
	//[상위]키 처리
	if(pContent->nSKT_Number == WAP_KEY_SK_UP)
	{
		//[상위]키가 존재하지 않을 경우 이를 생성한다
		extern WAP_Content_Key *g_pKeyUp;
		if(g_pKeyUp == NULL)
		{
			g_pKeyUp = pContent;

			//WAP_Building은 하지 않는다
			return;
		}
	}
	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	g_nTextFontType = WAP_FONT_NORMAL;
	WAP_Building(_ELEMENT_TYPE_KEY, pContent, WAP_FONT_NORMAL);
}

VOID    MMIa_newText (UINT8 viewId, UINT8 textId, const WCHAR *text, BOOL isLink, const WCHAR *linkTitle, WCHAR accessKey, INT8 format)
{
	//===============================================
    // MMI - SKY
    //void SKY_newText (UINT8 viewId, UINT8 textId, const WCHAR *text, BOOL isLink, const WCHAR *linkTitle, INT8 format)    //SKY_newText(viewId, textId, text, isLink, linkTitle, format);
	//===============================================

	WAP_Content_Text* pContent;

	int		length;
	char	*text_ptr, *linkTitle_ptr;

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) 
	{
		MSG_ERROR("NEW TEXT Error",0,0,0);
		return; 
	}

	//-----------------------------------------------------------
	if(text == NULL)//node를 만들지 않는다
	{
		return;
	}
	else
	{
		length = KSCStrLenOfUni((WCHAR *)text);
		
		if(length == 0)//node를 만들지 않는다
		{
			return;
		}
		else
		{
			text_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
			memset(text_ptr, 0x00, length+1);
			Uni2KSCString((WCHAR *)text, (unsigned char *)text_ptr);
		}
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)text);		

	if(linkTitle == NULL) linkTitle_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)linkTitle);
		linkTitle_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(linkTitle_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)linkTitle, (unsigned char *)linkTitle_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)linkTitle);	

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Text*) wip_malloc(sizeof(WAP_Content_Text));
	pContent->viewId	= viewId;
	pContent->textId	= textId;
	pContent->text		= text_ptr;
	pContent->isLink	= isLink;
	pContent->linkTitle	= linkTitle_ptr;
	pContent->align		= nParagraphAlign;
	pContent->nTableColumn = 0;

	//------------------------------------------------------------------
	//font type 적용 
    // 1. TXT_NORMAL, TXT_SMALL, TXT_BIG 구별
	if(format & TXT_SMALL)
	{
		g_nTextFontType = WAP_FONT_MEDIUM;
	}
	else 
	{
		g_nTextFontType = WAP_FONT_NORMAL;
	}

	//---------------------------------------------
	// 2. TXT_UNDERLINE, TXT_EMPHASIS, TXT_STRONG 구별
	if(format & TXT_UNDERLINE)
	{
		// isLink는 Text를 그릴때 underline을 그릴것인지 구별만 하므로
		// format에 TXT_UNDERLINE이 있을 때는 이 값을 TRUE로만 해주면 된다.
		pContent->isLink	= TRUE;
	}

	pContent->format = g_nTextFontType;

	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	if(bTable)
	{
		pContent->align			= WAP_TEXT_ALIGN_LEFT;//Table은 Left Align
		pContent->nTableColumn	= nCurTableColumn++;

		if(isLink)
			WAP_Building_Table(_ELEMENT_TYPE_LINK_TEXT, pContent, g_nTextFontType);
		else
			WAP_Building_Table(_ELEMENT_TYPE_TEXT, pContent, g_nTextFontType);
	}
	else
	{
		if(isLink)
			WAP_Building(_ELEMENT_TYPE_LINK_TEXT, pContent, g_nTextFontType);
		else
			WAP_Building(_ELEMENT_TYPE_TEXT, pContent, g_nTextFontType);
	}
}

#include "SISdecoder.h"

#ifdef FEATURE_CHARACTER_BELL
#include "MMFdecoder.h"
void SKY_MMF_Preview (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align);
#endif // FEATURE_CHARACTER_BELL

void SKY_newImage_SIS (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align);
void SKY_newImage_BMP (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align);
void SKY_newImage_WBMP (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align);
void SKY_newImage_Etc (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align);

VOID    MMIa_newImage (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, WCHAR accessKey, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align)
{
	//===============================================
    // MMI - SKY
    //void SKY_newImage (UINT8 viewId, UINT8 imageId, const CHAR *imageData, 
	//			   UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, 
	//			   const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, 
	//			   INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align)
	//SKY_newImage(viewId, imageId, imageData, imageSize,	imageType, 
	//			 altText, localSrc, isLink,	linkTitle, 
	//			 vSpace, hSpace, width,	height,	isPercent, align);
	//===============================================

    MSG_HIGH("SKY_newImage [%d]",imageSize,0,0);

#ifdef FEATURE_CHARACTER_BELL
    if (imageData != NULL && !(strcmp(imageType, "application/x-cmf")))
        SKY_MMF_Preview (viewId, imageId, imageData, imageSize, imageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
    else if(imageData != NULL && imageData[0] == 'B' && imageData[1] == 'M')
#else
    if(imageData != NULL && imageData[0] == 'B' && imageData[1] == 'M')
#endif // FEATURE_CHARACTER_BELL
	//-----------------------------------------------------------
	// BMP
	//-----------------------------------------------------------
    {
		SKY_newImage_BMP (viewId, imageId, imageData, imageSize, imageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
	}
	//-----------------------------------------------------------
	// WBMP
	//-----------------------------------------------------------
	else if(imageData != NULL && imageData[0] == 0 && imageData[1] == 0)
	{
		SKY_newImage_WBMP (viewId, imageId, imageData, imageSize, imageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
	}
	//-----------------------------------------------------------
	// SIS (Animation)
	//-----------------------------------------------------------
	else if(
			(imageData != NULL) && 
			((imageData[0] == 'S' && imageData[1] == 'I' && imageData[2] == 'S') ||
			(imageData[0] == 'S' && imageData[1] == 'A' && imageData[2] == 'F'))
		  )
	{
		SKY_newImage_SIS (viewId, imageId, imageData, imageSize, imageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
	}
	//-----------------------------------------------------------
	// 그 외의 이미지
	//-----------------------------------------------------------
	else
	{
		if(imageData == NULL)
		{
			CLNTa_log(1, 0, "newImage) 안 들어옴 -> 데이타 NULL");
		}
		else
		{
			CLNTa_log(1, 0, "newImage) etc 데이타 들어옴(%s)", imageType);
		}

		SKY_newImage_Etc (viewId, imageId, imageData, imageSize, imageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);

	}
}


void SKY_newImage_SIS (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align)
{
	WAP_Content_Image* pContent;

#ifdef FEATURE_CHARACTER_BELL
    int     image_frame = 0;
	DWORD   image_size = 0;
    int     image_type = 0;
	int     image_bitcount = 0;
#endif // FEATURE_CHARACTER_BELL
	char	*altText_ptr;
	int		length, result, nCurFrame = 0;
	int		numberOfFrame, imageXSize, imageYSize, bestImage;
	int     varDelay, infoImage, imageLevel, Loop; 
    int     delay[MAX_ANIMATION_FRAME-1]={0,};

	MSG_HIGH("SKY_newImage_SIS",0,0,0);
	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) 
	{
		MSG_ERROR("NEW IAMGE Error",0,0,0);
		return; 
	}

	//-----------------------------------------------------------
	if(altText == NULL) altText_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)altText);
		altText_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(altText_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)altText, (unsigned char *)altText_ptr);
	}
	
#ifdef FEATURE_MLZ
	//-----------------------------------------------------------
	// SIS Format을 처리한다
	//-----------------------------------------------------------
	// getSISInfo()와 sisDecode()에서 넣어주는 input data의 주소를 갖게 해주어야함 -- yjahn96 01.05.15
	memset((char*)m_gImageBuf, 0, MAX_IMAGEBUF_SIZE);
	memcpy((char*)m_gImageBuf, (const char*)imageData, imageSize);

	infoImage = getSISInfo((unsigned char*) m_gImageBuf, &numberOfFrame,
		                &imageXSize, &imageYSize, &varDelay, delay, &bestImage,
						&imageLevel, &Loop);

    // 여기 4줄 추가함. 보고
    if (imageXSize == 0) 
        imageXSize = MAX_IMAGE_WIDTH;
    if (imageYSize == 0)
        imageYSize = MAX_IMAGE_HEIGHT;

	MSG_HIGH("Frame[%d], XSize[%d], YSize[%d]",numberOfFrame,imageXSize,imageYSize);
	MSG_HIGH("Dealy[%d], Speed[%d], ImageLevel[%d]",varDelay,delay[0],imageLevel);

    //--------------------------------------------------------------------
    //첫번째 frame을 먼저 decoding해 놓음

	if(infoImage != 0)
	{	
        memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);

        sisInitialize((unsigned char*)m_gObjBuf, MAX_OBJBUF_SIZE,
				  (unsigned char*)m_gCodeBuf, MAX_CODEBUF_SIZE,
					MAX_SKY_IMAGE_WIDTH, MAX_SKY_IMAGE_HEIGHT);

        do {
            infoImage = sis_prepare_images (m_gImageBuf);
        } while (infoImage == 0);

        if (infoImage > 0) {
            result = sisDecode( (unsigned char *)m_gImageBuf, 
                            (unsigned char *)m_gUncompBuf, nCurFrame, 0) ; //첫번째 frame					        
		    MSG_ERROR("SIS DECODING : [%d]",result,0,0);

		    if(result == 0)
		    {
			    imageXSize	= WAP_LCD_WIDTH-1;//95
			    imageYSize  = WAP_ASCII_FONT_HEIGHT*3;//13*3
			    MSG_ERROR("sisDecode Fail",0,0,0);

                infoImage = 0;
		    }
            //else if(result == 1)
            {
		        nCurFrame++;
            }
        }
	}
#else
    //------------------------------------------------------------
	// SIS initialize 
	//------------------------------------------------------------
    if(sisInitialize((unsigned char*)m_gObjBuf, MAX_OBJBUF_SIZE,
				  (unsigned char*)m_gCodeBuf, MAX_CODEBUF_SIZE,
					MAX_SKY_IMAGE_WIDTH, MAX_SKY_IMAGE_HEIGHT) == 0)
	{
		ERR_FATAL("sisInitialize error",0,0,0);
	}
	//-----------------------------------------------------------
	// SIS Format을 처리한다
	//-----------------------------------------------------------
	// getSISInfo()와 sisDecode()에서 넣어주는 input data의 주소를 갖게 해주어야함 -- yjahn96 01.05.15
	memset((char*)m_gImageBuf, 0, MAX_IMAGEBUF_SIZE);
	memcpy((char*)m_gImageBuf, (const char*)imageData, imageSize);

	infoImage = getSISInfo((unsigned char*) m_gImageBuf, &numberOfFrame,
		                &imageXSize, &imageYSize, &varDelay, delay, &bestImage,
						&imageLevel, &Loop);

    MSG_HIGH("Frame[%d], XSize[%d], YSize[%d]",numberOfFrame,imageXSize,imageYSize);
	MSG_HIGH("Dealy[%d], Speed[%d], ImageLevel[%d]",varDelay,delay[0],imageLevel);

    //--------------------------------------------------------------------
    //첫번째 frame을 먼저 decoding해 놓음


	if(infoImage != 0)
	{	
        memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
        
        result = sisDecode( (unsigned char *)m_gImageBuf, 
                        (unsigned char *)m_gUncompBuf, nCurFrame) ; //첫번째 frame

		MSG_ERROR("SIS DECODING : [%d]",result,0,0);

		if(result == 0)
		{
			imageXSize	= WAP_LCD_WIDTH-1;//95
			imageYSize  = WAP_ASCII_FONT_HEIGHT*3;//13*3
			MSG_ERROR("sisDecode Fail",0,0,0);

            infoImage = 0;
		}
        //else if(result == 1)
        {
		    nCurFrame++;
        }
	}
#endif // FEATURE_MLZ

	//-----------------------------------------------------------
	// 그외의 이미지는 altText로 처리한다
	//-----------------------------------------------------------
	else
	{
		imageXSize		= WAP_LCD_WIDTH-1;//119
		imageYSize	= WAP_ASCII_FONT_HEIGHT*3;//13*3
	}

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Image*) wip_malloc(sizeof(WAP_Content_Image));
	pContent->viewId	= viewId;
	pContent->imageId	= imageId;
	pContent->imageData	= (char*)m_gUncompBuf;//(char*)m_gImageBuf;
	pContent->imageSize	= imageSize;

    if(infoImage == 0)
		pContent->imageType	= WAP_IMAGE_TYPE_ETC;
	else
		pContent->imageType	= WAP_IMAGE_TYPE_SIS;

	if(imageLevel == WAP_256COLOR_BITS_PER_PIXEL)
	{
		//실제 X,Y size는 아래와 같음
		if(imageXSize == 0) imageXSize = 256;
		if(imageYSize == 0) imageYSize = 256;

		//그러나, 128이 넘으면 임의로 128로 넣어줌
		if(imageXSize > MAX_SKY_IMAGE_WIDTH) imageXSize = MAX_SKY_IMAGE_WIDTH;	//128
		if(imageYSize > MAX_SKY_IMAGE_HEIGHT) imageYSize = MAX_SKY_IMAGE_HEIGHT;	//128
	}

	pContent->altText	= altText_ptr;
	pContent->localSrc	= NULL;//localSrc;
	pContent->isLink	= isLink;
	pContent->linkTitle	= NULL;//linkTitle;
	pContent->vSpace	= vSpace;
	pContent->hSpace	= hSpace;
	pContent->width		= imageXSize;
	pContent->height	= imageYSize;
	pContent->isPercent	= isPercent;
	pContent->nTotalFrame = numberOfFrame;//SIS format
	pContent->nCurFrame = nCurFrame;//SIS format

	//for color image
	pContent->nBitCount = imageLevel;

	switch(align)
	{
		case ALIGN_LEFT		: pContent->align = WAP_TEXT_ALIGN_LEFT; break;
		case ALIGN_CENTER	: pContent->align = WAP_TEXT_ALIGN_CENTER; break;
		case ALIGN_RIGHT	: pContent->align = WAP_TEXT_ALIGN_RIGHT; break;
	}

	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	if(bTable)
	{
		pContent->align			= WAP_TEXT_ALIGN_LEFT;//Table은 Left Align
		pContent->nTableColumn	= nCurTableColumn++;

		g_nTextFontType = WAP_FONT_NORMAL;
#ifdef FEATURE_CHARACTER_BELL
        if (!strcmp(imageType, "mmf"))  
            WAP_Building_Table(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
        else if(isLink)
#else 
        if(isLink)
#endif // FEATURE_CHARACTER_BELL
			WAP_Building_Table(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building_Table(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
	else
	{
		g_nTextFontType = WAP_FONT_NORMAL;
#ifdef FEATURE_CHARACTER_BELL
        if (!strcmp(imageType, "mmf"))  
            WAP_Building(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
        else if(isLink)
#else 
        if(isLink)
#endif // FEATURE_CHARACTER_BELL
			WAP_Building(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
#ifdef FEATURE_CHARACTER_BELL
    if (!strcmp(imageType, "mmf"))
    {
        GetCurrentImageInfo(&image_frame, &image_size, &image_type, &image_bitcount);
        if(image_bitcount == 1)	// B/W
		{
			// header + frame갯수 * (120*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_BW_IMAGE_SIZE); 
		}
		else if(image_bitcount == 2)	// 4 gray
		{
			// header + frame갯수 * (120*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_4GRAY_IMAGE_SIZE); 
		}
		else if(image_bitcount == 8)	//256 color
		{
			// header + frame갯수 * (120*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_256COLOR_IMAGE_SIZE); 
		}
    }
#endif // FEATURE_CHARACTER_BELL
}

void SKY_newImage_BMP (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align)
{
	WAP_Content_Image* pContent;

#ifdef FEATURE_CHARACTER_BELL
    int     image_frame = 0;
	DWORD   image_size = 0;
    int     image_type = 0;
	int     image_bitcount = 0;
#endif // FEATURE_CHARACTER_BELL
	int		length;
	char	*imageData_ptr, *altText_ptr;
	const char	*imgDataOrigin = imageData;

	//------------------------------------------------------------------
	//#pragma pack (2)
	typedef struct {
		unsigned char   fill1[2];
		unsigned long	bfsize;
		unsigned char   fill2[4];
		unsigned long	bfoffbits;
		unsigned char   fill3[4];
		long			biWidth;
		long			biHeight;
		unsigned char   fill4[2];
		short			biBitCount;
	}  BMPHeader;

	BMPHeader head;

	MSG_HIGH("SKY_newImage_BMP",0,0,0);

	//unsigned char   fill1[2];
	head.fill1[0] = *imageData++;
	head.fill1[1] = *imageData++;
	//unsigned long	bfsize;
	head.bfsize = imageData[0] + (imageData[1]<<8) + (imageData[2]<<16) + (imageData[3]<<24);
	imageData += 4;
	//unsigned char   fill2[4];
	imageData += 4;
	//unsigned long	bfoffbits;
	head.bfoffbits = imageData[0] + (imageData[1]<<8) + (imageData[2]<<16) + (imageData[3]<<24);
	imageData += 4;
	//unsigned char   fill3[4];
	imageData += 4;
	//long			biWidth;
	head.biWidth = imageData[0] + (imageData[1]<<8) + (imageData[2]<<16) + (imageData[3]<<24);
	imageData += 4;
	//long			biHeight;
	head.biHeight = imageData[0] + (imageData[1]<<8) + (imageData[2]<<16) + (imageData[3]<<24);
	imageData += 4;
	//unsigned char   fill4[2];
	imageData += 2;
	//short				biBitCount;
	head.biBitCount = imageData[0] + (imageData[1]<<8);
	imageData += 2;

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) 
	{
		MSG_ERROR("NEW IAMGE BMP Error",0,0,0);
		return; 
	}

	//-----------------------------------------------------------
	if(altText == NULL) altText_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)altText);
		altText_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(altText_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)altText, (unsigned char *)altText_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)altText);		

	//-----------------------------------------------------------
	// 흑백, 4Gray, 256 color 의 비트맵을 처리한다
	//-----------------------------------------------------------
	if(head.fill1[0] == 'B' && head.fill1[1] == 'M'
	&& (head.biBitCount == 1 || head.biBitCount == 4 || head.biBitCount == 8))
	{
		if(imageData == NULL) imageData_ptr = NULL;
		else
		{
			imageData_ptr = (char *) wip_malloc(sizeof(char)*(imageSize-head.bfoffbits+1));
			memcpy(imageData_ptr, imgDataOrigin+head.bfoffbits, imageSize-head.bfoffbits);
		}

		/* Free wap core allocated memory */
		//WIPFree((void *)imageData);		
	}
	//-----------------------------------------------------------
	// 그외의 이미지는 altText로 처리한다
	//-----------------------------------------------------------
	else
	{
		imageData_ptr	= NULL;
		head.biWidth	= WAP_LCD_WIDTH-1;//95
		head.biHeight	= WAP_ASCII_FONT_HEIGHT*3;//13*3
	}

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Image*) wip_malloc(sizeof(WAP_Content_Image));
	pContent->viewId	= viewId;
	pContent->imageId	= imageId;
	pContent->imageData	= imageData_ptr;
	pContent->imageSize	= imageSize;
	
	if(imageData_ptr == NULL)
		pContent->imageType	= WAP_IMAGE_TYPE_ETC;
	else
		pContent->imageType	= WAP_IMAGE_TYPE_BMP;

	pContent->altText	= altText_ptr;
	pContent->localSrc	= NULL;//localSrc;
	pContent->isLink	= isLink;
	pContent->linkTitle	= NULL;//linkTitle;
	pContent->vSpace	= vSpace;
	pContent->hSpace	= hSpace;
	pContent->width		= head.biWidth;
	pContent->height	= head.biHeight;
	pContent->isPercent	= isPercent;
	
	//for color image
	pContent->nBitCount = head.biBitCount;

	switch(align)
	{
		case ALIGN_LEFT		: pContent->align = WAP_TEXT_ALIGN_LEFT; break;
		case ALIGN_CENTER	: pContent->align = WAP_TEXT_ALIGN_CENTER; break;
		case ALIGN_RIGHT	: pContent->align = WAP_TEXT_ALIGN_RIGHT; break;
	}

	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	if(bTable)
	{
		pContent->align			= WAP_TEXT_ALIGN_LEFT;//Table은 Left Align
		pContent->nTableColumn	= nCurTableColumn++;

		g_nTextFontType = WAP_FONT_NORMAL;
#ifdef FEATURE_CHARACTER_BELL
        if (!strcmp(imageType, "mmf"))  
            WAP_Building_Table(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
        else if(isLink)
#else 
        if(isLink)
#endif // FEATURE_CHARACTER_BELL
			WAP_Building_Table(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building_Table(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
	else
	{
		g_nTextFontType = WAP_FONT_NORMAL;
#ifdef FEATURE_CHARACTER_BELL
        if (!strcmp(imageType, "mmf"))
            WAP_Building(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
        else if(isLink)
#else 
        if(isLink)
#endif // FEATURE_CHARACTER_BELL
			WAP_Building(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
#ifdef FEATURE_CHARACTER_BELL
    if (!strcmp(imageType, "mmf"))
    {
        GetCurrentImageInfo(&image_frame, &image_size, &image_type, &image_bitcount);
        if(image_bitcount == 1)	//B/W
		{
			// header + frame갯수 * (120/8*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_BW_IMAGE_SIZE);        
		}
		else if(image_bitcount == 2)	//4 gray
		{
			// header + frame갯수 * (120/4*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_4GRAY_IMAGE_SIZE);        
		}
		else if(image_bitcount == 8)	//256 color
		{
			// header + frame갯수 * (120*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_256COLOR_IMAGE_SIZE);        
		}
    }
#endif // FEATURE_CHARACTER_BELL
}

void SKY_newImage_WBMP (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align)
{
	WAP_Content_Image* pContent;

#ifdef FEATURE_CHARACTER_BELL
    int     image_frame = 0;
	DWORD   image_size = 0;
    int     image_type = 0;
	int     image_bitcount = 0;
#endif // FEATURE_CHARACTER_BELL
	int		length;
	char	*imageData_ptr, *altText_ptr;
	const char	*imgDataOrigin = imageData;

	int header1			= imageData[0];
	int header2			= imageData[1];
	int image_width;
	int image_height;
	int data_index;

	MSG_HIGH("SKY_newImage_WBMP",0,0,0);

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL)
	{
		MSG_ERROR("NEW IMAGE WBMP Error",0,0,0);
		return; 
	}

	//-----------------------------------------------------------
	if(altText == NULL) altText_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)altText);
		altText_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(altText_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)altText, (unsigned char *)altText_ptr);
	}
	
	/* Free wap core allocated memory */
	//WIPFree((void *)altText);	
	
	//-----------------------------------------------------------
	// WBMP만 처리한다
	//-----------------------------------------------------------
	
	//----------------------------------
	data_index = 2;//header Byte
	//----------------------------------
	image_width = 0;
	while(imageData[data_index] > 127)
	{
		image_width += 127;
		data_index++;
	}
	image_width += imageData[data_index++];
	//----------------------------------
	image_height = 0;
	while(imageData[data_index] > 127)
	{
		image_height += 127;
		data_index++;
	}
	image_height += imageData[data_index++];
	//----------------------------------
	
	if(header1==0 && header2==0)//wbmp
	{
		if(imageData == NULL) imageData_ptr = NULL;
		else
		{
			imageData_ptr = (char *) wip_malloc(sizeof(char)*(imageSize-data_index+1));
			memcpy(imageData_ptr, imgDataOrigin+data_index, imageSize-data_index);
		}

		/* Free wap core allocated memory */
		//WIPFree((void *)imageData);		
	}
	//-----------------------------------------------------------
	// 그외의 이미지는 altText로 처리한다
	//-----------------------------------------------------------
	else
	{
		imageData_ptr	= NULL;
		image_width		= WAP_LCD_WIDTH-1;//95
		image_height	= WAP_ASCII_FONT_HEIGHT*3;//13*3
	}

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Image*) wip_malloc(sizeof(WAP_Content_Image));
	pContent->viewId	= viewId;
	pContent->imageId	= imageId;
	pContent->imageData	= imageData_ptr;
	pContent->imageSize	= imageSize;
	
	if(imageData_ptr == NULL)
		pContent->imageType	= WAP_IMAGE_TYPE_ETC;
	else
		pContent->imageType	= WAP_IMAGE_TYPE_WBMP;

	pContent->altText	= altText_ptr;
	pContent->localSrc	= NULL;//localSrc;
	pContent->isLink	= isLink;
	pContent->linkTitle	= NULL;//linkTitle;
	pContent->vSpace	= vSpace;
	pContent->hSpace	= hSpace;
	pContent->width		= image_width;
	pContent->height	= image_height;
	pContent->isPercent	= isPercent;
	switch(align)
	{
		case ALIGN_LEFT		: pContent->align = WAP_TEXT_ALIGN_LEFT; break;
		case ALIGN_CENTER	: pContent->align = WAP_TEXT_ALIGN_CENTER; break;
		case ALIGN_RIGHT	: pContent->align = WAP_TEXT_ALIGN_RIGHT; break;
	}

	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	if(bTable)
	{
		pContent->align			= WAP_TEXT_ALIGN_LEFT;//Table은 Left Align
		pContent->nTableColumn	= nCurTableColumn++;

		g_nTextFontType = WAP_FONT_NORMAL;
#ifdef FEATURE_CHARACTER_BELL
        if (!strcmp(imageType, "mmf")) 
            WAP_Building_Table(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
        else if(isLink)
#else 
        if(isLink)
#endif // FEATURE_CHARACTER_BELL
			WAP_Building_Table(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building_Table(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
	else
	{
		g_nTextFontType = WAP_FONT_NORMAL;
#ifdef FEATURE_CHARACTER_BELL
        if (!strcmp(imageType, "mmf")) 
            WAP_Building(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
        else if(isLink)
#else 
        if(isLink)
#endif // FEATURE_CHARACTER_BELL
			WAP_Building(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
#ifdef FEATURE_CHARACTER_BELL
    if (!strcmp(imageType, "mmf"))
    {
        GetCurrentImageInfo(&image_frame, &image_size, &image_type, &image_bitcount);
        if(image_bitcount == 1)	//B/W
		{
			// header + frame갯수 * (120/8*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_BW_IMAGE_SIZE);        
		}
		else if(image_bitcount == 2)	//4 gray
		{
			// header + frame갯수 * (120/4*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_4GRAY_IMAGE_SIZE);        
		}
		else if(image_bitcount == 8)	//256 color
		{
			// header + frame갯수 * (120*120)이미지 크기
    		s_nDecodedImageSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_256COLOR_IMAGE_SIZE);        
		}
    }
#endif // FEATURE_CHARACTER_BELL
}

void SKY_newImage_Etc (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align)
{
	WAP_Content_Image* pContent;

	int		length;
	char	*altText_ptr;

	MSG_HIGH("SKY_newImage_Etc",0,0,0);
	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) 
	{
		MSG_ERROR("SKY_newImage_Etc",0,0,0);
		return; 
	}

	//-----------------------------------------------------------
	if(altText == NULL) altText_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)altText);
		altText_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(altText_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)altText, (unsigned char *)altText_ptr);
	}
	
	/* Free wap core allocated memory */
	//WIPFree((void *)altText);		

	/* Free wap core allocated memory */
	//WIPFree((void *)imageData);		

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Image*) wip_malloc(sizeof(WAP_Content_Image));

	pContent->viewId	= viewId;
	pContent->imageId	= imageId;
	pContent->imageData	= NULL;
	pContent->imageSize	= imageSize;
	pContent->imageType	= WAP_IMAGE_TYPE_ETC;
	pContent->altText	= altText_ptr;
	pContent->localSrc	= NULL;//localSrc;
	pContent->isLink	= isLink;
	pContent->linkTitle	= NULL;//linkTitle;
	pContent->vSpace	= vSpace;
	pContent->hSpace	= hSpace;
	pContent->width		= WAP_LCD_WIDTH-1;//95
	pContent->height	= WAP_ASCII_FONT_HEIGHT*3;//13*3
	pContent->isPercent	= isPercent;
	pContent->align		= align;

	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	if(bTable)
	{
		pContent->align			= WAP_TEXT_ALIGN_LEFT;//Table은 Left Align
		pContent->nTableColumn	= nCurTableColumn++;

		g_nTextFontType = WAP_FONT_NORMAL;
		if(isLink)
			WAP_Building_Table(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building_Table(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
	else
	{
		g_nTextFontType = WAP_FONT_NORMAL;
		if(isLink)
			WAP_Building(_ELEMENT_TYPE_LINK_IMAGE, pContent, WAP_FONT_NORMAL);
		else
			WAP_Building(_ELEMENT_TYPE_IMAGE, pContent, WAP_FONT_NORMAL);
	}
}

#ifdef FEATURE_CHARACTER_BELL
void SKY_MMF_Preview (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align) {
    int         infoCB, resultCB, ImageType, MelodyType;
    int         titleSize, ImageSize, MelodySize, textSize;
    char        mmfImageType[5];
    extern unsigned char *CBtitle;
    extern unsigned char *CBtext;

    infoCB = getMMFInfo((OCTET* )imageData, &titleSize, &ImageSize, 
	                &MelodySize, &textSize);
    if (infoCB == 0)
        ;
    else {
        if(CBtitle){
			wip_free((void*)CBtitle);
			CBtitle =NULL;
		}
        CBtitle = (BYTE*)wip_malloc(titleSize);

        /*if(CBImageData){
			wip_free((void*)CBImageData);
			CBImageData =NULL;
		}
        CBImageData = (BYTE*)wip_malloc(ImageSize);*/

        if(CBMelodyData){
			wip_free((void*)CBMelodyData);
			CBMelodyData =NULL;
		}
        CBMelodyData = (BYTE*)wip_malloc(MelodySize);

        if(CBtext){
			wip_free((void*)CBtext);
			CBtext =NULL;
		}
        CBtext = (BYTE*)wip_malloc(textSize);

        memset((unsigned char*)CBtitle, 0, titleSize);
        //memset((unsigned char*)CBImageData, 0, ImageSize);
        memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
        memset((unsigned char*)CBMelodyData, 0, MelodySize);
        memset((unsigned char*)CBtext, 0, textSize);

        //resultCB = mmfDecode ((OCTET*)imageData, CBtitle, CBImageData, &ImageType,
        resultCB = mmfDecode ((OCTET*)imageData, CBtitle, m_gUncompBuf, &ImageType,
                                CBMelodyData, &MelodyType, CBtext);
		
        if (resultCB == 0) {
            if(CBMelodyData){
			    wip_free((void*)CBMelodyData);
			    CBMelodyData =NULL;
		    }

            /*if(CBImageData){
			    wip_free((void*)CBImageData);
			    CBImageData =NULL;
		    }*/

            if(CBtitle){
			    wip_free((void*)CBtitle);
			    CBtitle =NULL;
		    }

            if(CBtext){
			    wip_free((void*)CBtext);
			    CBtext =NULL;
		    }
        }
        else {
            if(CBtitle){
			    wip_free((void*)CBtitle);
			    CBtitle =NULL;
		    }
            if(CBtext){
			    wip_free((void*)CBtext);
			    CBtext =NULL;
		    }

            strcpy(mmfImageType, "mmf");
            g_fCharacbellPreview = TRUE;
            if (ImageType == 1)			// BMP type
                //SKY_newImage_BMP (viewId, imageId, (char *)CBImageData, ImageSize, mmfImageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
                SKY_newImage_BMP (viewId, imageId, (char *)m_gUncompBuf, ImageSize, mmfImageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
        	else if (ImageType == 3)    // WBMP type
                //SKY_newImage_WBMP (viewId, imageId, (char *)CBImageData, ImageSize, mmfImageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
                SKY_newImage_WBMP (viewId, imageId, (char *)m_gUncompBuf, ImageSize, mmfImageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
            else if (ImageType == 4)	// Animation type
                //SKY_newImage_SIS (viewId, imageId, (char *)CBImageData, ImageSize, mmfImageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
                SKY_newImage_SIS (viewId, imageId, (char *)m_gUncompBuf, ImageSize, mmfImageType, altText, localSrc, isLink, linkTitle, vSpace, hSpace, width, height, isPercent, align);
        }
    } 
} // SKY_MMF_Preview
#endif // FEATURE_CHARACTER_BELL

void    MMIa_completeImage (unsigned char viewId, unsigned char imageId, const char *imageData, unsigned short imageSize, const char *imageType)
{
	//===============================================
    // MMI - SKY
    //void SKY_completeImage (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType)	//SKY_completeImage(viewId, imageId, imageData, imageSize, imageType);
	//===============================================
}

VOID    MMIa_newParagraph (UINT8 viewId, INT8 align, BOOL wrap, BOOL preformatted)
{
	//===============================================
    // MMI - SKY
    //void SKY_newParagraph (UINT8 viewId, INT8 align, BOOL wrap)	//SKY_newParagraph(viewId, align, wrap);
	//===============================================

	//처음 Paragraph가 아닌 경우는 라인 브레이크를 삽입한다
	if(bFirstParagraph)	bFirstParagraph = FALSE;
	else				MMIa_newBreak(viewId);//SKY_newBreak(viewId);

	switch(align)
	{
		case ALIGN_LEFT		: nParagraphAlign = WAP_TEXT_ALIGN_LEFT;	break;
		case ALIGN_CENTER	: nParagraphAlign = WAP_TEXT_ALIGN_CENTER;	break;
		case ALIGN_RIGHT	: nParagraphAlign = WAP_TEXT_ALIGN_RIGHT;	break;
	}
}

void    MMIa_closeParagraph (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_closeParagraph (UINT8 viewId)	//SKY_closeParagraph(viewId);
	//===============================================

    //init
	nParagraphAlign = WAP_TEXT_ALIGN_LEFT;
}

void    MMIa_newBreak (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_newBreak (UINT8 viewId)	//SKY_newBreak(viewId);
	//===============================================

	WAP_Building(_ELEMENT_TYPE_BR, NULL, g_nTextFontType);
}

void    MMIa_newTable (unsigned char viewId, const unsigned short *title, signed char noOfColumns, const char *align)
{
	//===============================================
    // MMI - SKY
	//SKY_newTable(viewId, title, noOfColumns, align);
    //void SKY_newTable (UINT8 viewId, const WCHAR *title, INT8 noOfColumns, const CHAR *align)	//SKY_newTable(viewId, title, noOfColumns, align);
	//===============================================

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) return; 
	
	//-----------------------------------------------------------
	// Table의 최대 Column의 갯수는 4 이다 -> 한글 2자가 보여짐
	if(1 <= noOfColumns && noOfColumns <= 4)
	{
		bTable = TRUE;
		nCurTableColumn = 1;//1_based
		nTotalTableColumns = noOfColumns;
	}
}

void    MMIa_newTableData (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_newTableData (UINT8 viewId)	//SKY_newTableData(viewId);
	//===============================================

	WAP_Content_TableRow *pContent;

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) return; 

	//-----------------------------------------------------------
	// 현재 Table 상태가 아니라면, Table Row를 추가할 수 없다
	if(!bTable)
	{
		//-----------------------------------------------------------
		// Column의 수가 최대치(4)를 초과한 경우, LineBreak만 삽입한다
		if(! (1 <= nTotalTableColumns && nTotalTableColumns <= 4) )
		{ 
			MMIa_newBreak(viewId);//SKY_newBreak(viewId);
		}
		return;
	}	
	
	//-----------------------------------------------------------
	if(nCurTableColumn > nTotalTableColumns)
		nCurTableColumn -= nTotalTableColumns;

	//-----------------------------------------------------------
	// 새로운 Row의 경우에만 Table Row를 추가한다
	if(nCurTableColumn != 1) return;

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_TableRow*) wip_malloc(sizeof(WAP_Content_TableRow));
	pContent->viewId		= viewId;
	pContent->noOfColumns	= nTotalTableColumns;//from SKY_newTable
	pContent->align			= 0;//from SKY_newTable

	//----------------------------------------------------------
	// Table 화면 구성
	g_pCurTableRow = pContent;
	g_pCurTableRow->pHeadCell = NULL;
	g_pCurTableRow->pHeadFocusCell = NULL;
	g_pCurTableRow->pFocusCell = NULL;
	
	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	g_nTextFontType = WAP_FONT_NORMAL;
	WAP_Building(_ELEMENT_TYPE_TABLE_ROW, pContent, WAP_FONT_NORMAL);
}

void    MMIa_closeTable (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_closeTable (UINT8 viewId)   //SKY_closeTable(viewId);
	//===============================================

	bTable = FALSE;//init
	
	nCurTableColumn = 0;//init
	nTotalTableColumns = 0;//init
	
	g_pCurTableRow = NULL;//init

	MMIa_newBreak(viewId);//SKY_newBreak(viewId);
}

void    MMIa_newFieldSet (unsigned char viewId, const unsigned short *title)
{
	//===============================================
    // MMI - SKY
    //void SKY_newFieldSet (UINT8 viewId, const WCHAR *title)//SKY_newFieldSet(viewId, title);
	//===============================================

}

void    MMIa_closeFieldSet (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_closeFieldSet (UINT8 viewId)	//SKY_closeFieldSet(viewId);
	//===============================================

}

void    MMIa_newSelect (unsigned char viewId, const unsigned short *title, char multiSelect, signed char tabIndex)
{
	//===============================================
    // MMI - SKY
    //void SKY_newSelect (UINT8 viewId, const WCHAR *title, BOOL multiSelect, INT8 tabIndex)	//SKY_newSelect(viewId, title, multiSelect, tabIndex);
	//===============================================

	WAP_Content_Select *pContent;

	int		length;
	char	*title_ptr;

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) return; 

	if(title == NULL) title_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)title);
		title_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(title_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)title, (unsigned char *)title_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)title);		

	//-----------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Select*) wip_malloc(sizeof(WAP_Content_Select));
	pContent->viewId		= viewId;
	pContent->title			= title_ptr;
	pContent->multiSelect	= multiSelect;
	pContent->tabIndex		= tabIndex;
	
	memset(g_pCurSelect->selectedOptions, '\0', 255);//init

	//----------------------------------------------------------
	// 옵션 화면 구성
	g_pCurSelect = pContent;
	g_nCurFocusOption = 0;
	g_pCurSelect->pHeadOption = NULL;
	g_pCurSelect->pHeadFocusOption = NULL;
	
	//----------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	g_nTextFontType = WAP_FONT_NORMAL;
	WAP_Building(_ELEMENT_TYPE_SELECT, pContent, WAP_FONT_NORMAL);

	MMIa_newBreak(viewId);//SKY_newBreak(viewId);
}

void    MMIa_closeSelect (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_closeSelect (UINT8 viewId)	//SKY_closeSelect(viewId);
	//===============================================

    WAP_Node *pCurNode;
	WAP_Content_Option *pContent;

	//clear
	memset(g_pCurSelect->selectedOptions, '\0', 255);
	pCurNode = g_pCurSelect->pHeadFocusOption;
	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_OPTION)
		{
			pContent = (WAP_Content_Option *)pCurNode->pContent; 

			if(pContent->isSelected)
			{
				if(g_pCurSelect->multiSelect)
				{
					if(strlen(g_pCurSelect->selectedOptions) == 0)
						strcpy(g_pCurSelect->selectedOptions, pContent->label);
					else 
						sprintf(g_pCurSelect->selectedOptions, "%s,%s",
								g_pCurSelect->selectedOptions, pContent->label);
				}
				else
				{
					strcpy(g_pCurSelect->selectedOptions, pContent->label);
					break;
				}
			}
		}//end of if(pContent->type == _ELEMENT_TYPE_OPTION)

		pCurNode = (WAP_Node *)pCurNode->pNextFocusNode;
	
	}//end of while(pCurNode != NULL)
	//===============

	g_pCurSelect = NULL;
	g_nCurFocusOption = 0;
}

void    MMIa_newOption (unsigned char viewId, unsigned char optionId, const unsigned short *label, const unsigned short *title, char isSelected)
{
	//===============================================
    // MMI - SKY
    //void SKY_newOption (UINT8 viewId, UINT8 optionId, const WCHAR *label, const WCHAR *title, BOOL isSelected)	//SKY_newOption(viewId, optionId, label, title, isSelected);
	//===============================================

	WAP_Content_Option *pContent;

	int		length;
	char	*label_ptr, *title_ptr;

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) return; 

	//-------------------------------------------------------------
	// 현재 Select가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCurSelect == NULL) return;
	
	if(label == NULL) label_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)label);
		label_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(label_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)label, (unsigned char *)label_ptr);	
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)label);
	
	if(title == NULL) title_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)title);
		title_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(title_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)title, (unsigned char *)title_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)title);

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Option*) wip_malloc(sizeof(WAP_Content_Option));
	pContent->viewId	= viewId;
	pContent->optionId	= optionId;
	pContent->label		= label_ptr;
	pContent->title		= title_ptr;
	pContent->isSelected= isSelected;
	
	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	WAP_Building_Options(_ELEMENT_TYPE_OPTION, pContent);
}

void    MMIa_newOptionGroup (unsigned char viewId, const unsigned short *label)
{
	//===============================================
    // MMI - SKY
    //void SKY_newOptionGroup (UINT8 viewId, const WCHAR *label)	//SKY_newOptionGroup(viewId, label);
	//===============================================

    WAP_Content_OptionGroup *pContent;

	int		length;
	char	*label_ptr;

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) return; 

	//-------------------------------------------------------------
	// 현재 Select가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCurSelect == NULL) return;
	
	if(label == NULL) label_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)label);
		label_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(label_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)label, (unsigned char *)label_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)label);
	
	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_OptionGroup*) wip_malloc(sizeof(WAP_Content_OptionGroup));
	pContent->viewId	= viewId;
	pContent->label		= label_ptr;

	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	WAP_Building_Options(_ELEMENT_TYPE_OPTIONGROUP, pContent);
}

void    MMIa_closeOptionGroup (unsigned char viewId)
{
	//===============================================
    // MMI - SKY
    //void SKY_closeOptionGroup (UINT8 viewId)	//SKY_closeOptionGroup(viewId);
	//===============================================
}

VOID    MMIa_newInput (UINT8 viewId, UINT8 inputId, const WCHAR *title, const WCHAR *text, BOOL isPassword, BOOL emptyOk, const WCHAR *format, INT8 size, INT8 nChars, INT8 tabIndex, WCHAR accessKey)
{
	//===============================================
    // MMI - SKY
    //void SKY_newInput (UINT8 viewId, UINT8 inputId, const WCHAR *title, const WCHAR *text, BOOL isPassword, BOOL emptyOk, const WCHAR *format, INT8 size, INT8 nChars, INT8 tabIndex)	//SKY_newInput(viewId, inputId, title, text, isPassword, emptyOk, format, size, nchars, tabIndex);
	//===============================================

	WAP_Content_Input *pContent;

	int	length;
	char *title_ptr, *format_ptr;
	char text_ptr[WAP_MAX_INPUT+1];

	//-----------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, Content도 추가할 수 없다
	if(g_pCard == NULL) return; 

	if(title == NULL) title_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)title);
		title_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(title_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)title, (unsigned char *)title_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)title);
	
	if(text == NULL) ;//text_ptr = NULL;
	else
	{
		//static data
		memset(text_ptr, '\0', WAP_MAX_INPUT+1);//초기화
		Uni2KSCString((WCHAR *)text, (unsigned char *)text_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)text);

	if(format == NULL) format_ptr = NULL;
	else
	{
		length = KSCStrLenOfUni((WCHAR *)format);
		format_ptr = (char *) wip_malloc(sizeof(char)*(length+1));
		memset(format_ptr, 0x00, length+1);
		Uni2KSCString((WCHAR *)format, (unsigned char *)format_ptr);
	}

	/* Free wap core allocated memory */
	//WIPFree((void *)format);

	//------------------------------------------------------------------
	// 새로운 컨텐트를 하나 만든다 
	pContent = (WAP_Content_Input*) wip_malloc(sizeof(WAP_Content_Input));
	pContent->viewId	= viewId;
	pContent->inputId	= inputId;
	pContent->title		= title_ptr;

	if(text == NULL)
		memset(pContent->text, '\0', WAP_MAX_INPUT+1);//초기화
	else
		strcpy(pContent->text, text_ptr);
	
	pContent->isPassword= isPassword;
	pContent->emptyOk	= emptyOk;
	pContent->format	= format_ptr;
	pContent->size		= size;
	pContent->nChars	= nChars;
	pContent->tabIndex	= tabIndex;

	//------------------------------------------------------------------
	// 새 컨텐트를 Document에 추가한다
	g_nTextFontType = WAP_FONT_NORMAL;
	WAP_Building(_ELEMENT_TYPE_INPUT, pContent, WAP_FONT_NORMAL);

	MMIa_newBreak(viewId);//SKY_newBreak(viewId);
}

void    MMIa_getInputString (unsigned char viewId, unsigned char inputId)
{
	//===============================================
    // MMI - SKY
    //void SKY_getInputString (UINT8 viewId, UINT8 inputId)	//SKY_getInputString(viewId, inputId);
	//===============================================

	WAP_Node *pCurNode;

	//-------------------------------------------------------------
	// 현재 Card가 만들어 지지 않았다면, String을 넘겨줄 수 없다
	if(g_pCard == NULL) return;

	pCurNode = g_pCard->pHeadFocus;
	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_INPUT)
		{
			WAP_Content_Input *pInput = (WAP_Content_Input *)pCurNode->pContent; 
			
			if(pInput != NULL && pInput->inputId == inputId)
			{
				if(pInput->text != NULL)
				{
					//char -> WCHAR
					WCHAR   *input;
					int		length;

					length = UniLenOfKSCStr((char *)pInput->text);
					input = (WCHAR *)wip_malloc(sizeof(WCHAR)*(length+1));
					KSC2UniString((unsigned char *)pInput->text, (WCHAR *)input);
										
					SKY_inputString(viewId, inputId, (WCHAR *)input);

					//[input] will be deleted in BROWSER...
					//if(input != NULL) wip_free((void *) input);
				}
				else
				{
					SKY_inputString(viewId, inputId, (WCHAR *)NULL);
				}
				return;
			}

		}// end of if

		pCurNode = (WAP_Node *)pCurNode->pNextFocusNode;

	}// end of while
}

//jjkyg00/07/2
#ifdef BROWSER_UPDATE
	#define REDIRECT
#endif

void    MMIa_linkInfo (unsigned char viewId, unsigned char widgetType , unsigned char widgetId, const char *URL)
{
	//===============================================
    // MMI - SKY
    //void SKY_linkInfo (UINT8 viewId, UINT8 widgetType , UINT8 widgetId, const CHAR *URL)	//SKY_linkInfo(viewId, widgetType, widgetId, URL);
	//===============================================

// ==========================================================================================
// zeus(yjchoi) 00.12.04
// EIF WAP (External Interface for WAP)에서 
// current Card의 각 key 에 해당하는 URL을 출력해주기 위한 버퍼, 출력 함수, 플래그 변수 
#ifdef FEATURE_SKT_EIF//EIF_WAP
	extern void at_send_extended_text(char*);
	extern char resultBuf[2048];
	extern BOOL g_EIF_WAP_GETURL;
#endif // enf of #ifdef EIF_WAP
// ==========================================================================================

//jjkyg00/07/2
#ifdef REDIRECT
	/*extern*/ CHAR RedirectUrl[1500];
	/*extern*/ CHAR prevRedirect[1500];
	/*extern*/ BOOL setPrev;

	memset(prevRedirect,0x00,1500);
	memset(RedirectUrl,0x00,1500);
	if(strlen(URL) < 1500 ) strncpy(RedirectUrl,URL,strlen(URL));

#endif//#ifdef REDIRECT

// ==========================================================================================
// zeus(yjchoi) 00.12.04
// EIF WAP(External Interface for WAP)에서 
// current card의 각 key에 해당하는 url을 출력한다.
#ifdef FEATURE_SKT_EIF//EIF_WAP
	if (g_EIF_WAP_GETURL)
	{
	//	memset(resultBuf, 0x00, sizeof(resultBuf));
		sprintf(resultBuf, "%s%s",resultBuf, URL);
		g_EIF_WAP_GETURL = FALSE;
		at_send_extended_text(resultBuf);
	}
#endif // #ifdef EIF_WAP
}


VOID    MMIa_setLanguage (UINT8 viewId, const CHAR *language)
{
    CLNTa_log(1,0,"DUMMY MMIa_setLanguage");
}


/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
/*																									*/
/*								WML Script의 Graphic 관련 Adapter Fuctions										*/
/*																									*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
void SKY_graphicsStart(void)
{
	g_nGraphicMode = WAP_GRAPHIC_MODE_DRAWING;
	
	//---------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();
	//---------------------------------------
}

void SKY_graphicsEnd(void)
{
}

void SKY_graphicsPoint(int x, int y)
{
	if(g_nGraphicMode != WAP_GRAPHIC_MODE_DRAWING) return;
	WAP_Display_Point(x, y, (COLOR)TRUE);
}

void SKY_graphicsLine(int x1, int y1, int x2, int y2)
{
	if(g_nGraphicMode != WAP_GRAPHIC_MODE_DRAWING) return;
	WAP_Display_Line(x1, y1, x2, y2, (COLOR)TRUE);
}

void SKY_graphicsRectangle(int x, int y, int width, int height, BOOL isFill)
{
	if(g_nGraphicMode != WAP_GRAPHIC_MODE_DRAWING) return;
	WAP_Display_Rectangle(x, y, (x+width), (y+height), isFill, BLACK);
}

void SKY_graphicsCircle(int x, int y, int radius, BOOL isFill)
{
	if(g_nGraphicMode != WAP_GRAPHIC_MODE_DRAWING) return;
	WAP_Display_Circle(x, y, radius, isFill, (COLOR)TRUE);
}
