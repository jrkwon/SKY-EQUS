#ifdef FEATURE_WAP  
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
** 2001-07-16   ytchoi     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "target.h"
#include "comdef.h"
#include "task.h"
#include "rex.h" 
#include "queue.h"
#include "err.h"
#include "msg.h"
#include "wapadapter.h"
#include "wapconnector.h"
#include "tapicmmn.h"
#include "capiclnt.h"
#include "capimmi.h"
#include "capimem.h"
#include "capiudp.h"
#include "msg.h"
#include <memory.h>
#include "dog.h"
#include "dsm.h"
#include "clnt.h"
#include "errcodes.h"
#include "aapiclnt.h"
#include "mmi_func.h"
#include "sky.h"
#include "ska.h"
#include "wapcommon.h"
#include "GenDsp.h"
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/



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
void displayloading(int catagory);
void WAP_DrawLoadingBar(int nAniTextMode, BOOL bAniBarDirection);

#ifdef FEATURE_WAP_REDIRECT
BOOL save_redirecturl(void);
#endif //FEATURE_WAP_REDIRECT
/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
q_type           wap_cmd_q;      /* WAP command queue              */
q_type           wap_cmd_free_q; /* WAP command free queue         */
ip4a wapip;   /* Our IP address */
/* Allocate a buffer to hold the queue elements  */
wap_cmd_type wap_buffers[ WAP_CMD_BUFFERS ];
//#define temp_forwapapp2

boolean start_disp = FALSE;
UINT8 g_nAniBarCount = 0;
clk_cb_type wapbar_timer;

UINT32 g_nTimeStamp = 0; // initalze_MMI()에서 다시 초기화!!
int g_nAniTextMode = 0;


/*
**---------------------------------------------------------------------------
**  External variables
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Function(External use only) Declarations
**---------------------------------------------------------------------------
*/



/*
**---------------------------------------------------------------------------
**  Function(Internal use only) Declarations
**---------------------------------------------------------------------------
*/

void wap_display_exit_message(void)
{
	char temp[21];

	//g_bTerminatedWAP = TRUE;

	memset(temp, NULL, 21);
	//sprintf(temp, "%s%s%s%s을",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP

	//====================================================================
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "NATE를", WAP_TEXT_ALIGN_CENTER);//WAP_SetTextAtLine(2, temp, WAP_TEXT_ALIGN_CENTER);
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

#ifdef FEATURE_GVM
void wap_display_exit_for_gvm(void)
{
	WAP_ClearScreen();
	/* rosa 01-04-06 --> @why, what */
	WAP_SetTextAtLine(2, "\x82\xEE\x82\xEF\x82\xF0\x82\xF1 GVM",WAP_TEXT_ALIGN_CENTER); /* n.TOP GVM */
	/* <-- */
	WAP_SetTextAtLine(3, "연결중",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}
#endif //#ifdef FEATURE_GVM

/*===========================================================================
FUNCTION GET_LENTH
===========================================================================*/
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

void send_cmd2wapHandler( wap_cmd_type *cmd_ptr )
{
  (void) q_link( cmd_ptr, &cmd_ptr->hdr.link );   /* init link */
  //q_put( &wap_cmd_q, &cmd_ptr->hdr.link );        /* and queue it */
  q_put( &ui_cmd_q, &cmd_ptr->hdr.link );        /* and queue it */
  (void) SKY_Set_Signal( &ui_tcb, WAP_CMD_Q_SIG );  /* signal the UI task */  
}


void SKY_API WAP_HandleCommands(wap_cmd_type *pCommand)
{
    switch (pCommand->hdr.cmd) 
    {   
	    case WAP_START_NETWORK:
	    {
            MSG_HIGH("Right SIGNAL!!!", 0, 0, 0);		    
		    break;
	    }

        default:
            break;
    }
}

/* Initialize queue , clock timer, browser polling timer, wapip*/
void WAP_InitQnCLK(void)
{
    int i;
  /* Initialize queues */
    (void) q_init( &wap_cmd_q );
    (void) q_init( &wap_cmd_free_q );

  /* Initialize each element of the free queue and build the free queue */
    for (i=0; i < WAP_CMD_BUFFERS; i++) 
    {
        wap_buffers[i].hdr.done_q_ptr = &wap_cmd_free_q;
        q_put( &wap_cmd_free_q,
               q_link( &wap_buffers[i],
                       &wap_buffers[i].hdr.link ));
    }
// No use 
    //rex_def_timer( &wap_control_timer, &ui_tcb, WAP_SIG );
    clk_def(&clk_timer);
    wapip = 0;
}

void bar_ani_start(int type, int time)
{	
	(void) rex_set_timer( &bar_ani_timer, time );
	displayloading( type );
}

void bar_ani_stop(void)
{
	(void) rex_clr_sigs( &ui_tcb, BAR_ANI_SIG );
	(void) rex_clr_timer( &bar_ani_timer);
	g_nAniBarCount=0;
    g_bDrawBar=FALSE;
}

void process_draw_bar(void)
{
	int MODE =0;

#ifdef FEATURE_WAP_REDIRECT
//if(redirect.bCorrect)
    if(gredirectloadingbar)
        MODE = WAP_ANI_TEXT_RESETUP;
    else
#endif//#ifdef FEATURE_WAP_REDIRECT
	if(start_disp ||g_pCard == NULL)    
	{
		MODE = WAP_ANI_TEXT_CONNECTING;//접속중
		WAP_DrawLoadingBar(MODE, WAP_ANI_BAR_FROM_LEFT);		
		return;
	}
	else if(g_nAniTextMode == WAP_ANI_TEXT_SECURITY)
    {
        MODE = WAP_ANI_TEXT_SECURITY;
    }
	else
    {
        MODE = WAP_ANI_TEXT_RECEIVING;	
    }

	WAP_DrawLoadingBar(MODE, WAP_ANI_BAR_FROM_LEFT);		

}

void displayloading(int catagory)
{
	//int nAniTextMode = 0; --> 전역으로 !!
	BOOL bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
	
	switch(catagory)
	{
		case 0://연결중
			start_disp = TRUE;
			g_nAniTextMode = WAP_ANI_TEXT_CONNECTING;//접속중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		case 1://데이타 수신중
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_RECEIVING;//수신중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		case 2://보안 요구중
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		case 3://보안 종료중 
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		
		case 5://재수신중
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_RECEIVING;//수신중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		case 6://보안요구중
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		case 8://Key교환중
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_SECURITY;//보안중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;

		case 11://즐겨찾기추가중
		{
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_ADDBOOKMARK;//추가중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
			break;
		}
#ifdef FEATURE_WAP_REDIRECT
		case 12://Redirect
		{
			start_disp = FALSE;
			g_nAniTextMode = WAP_ANI_TEXT_RESETUP;//추가중
			bAniBarDirection = WAP_ANI_BAR_FROM_LEFT;
		}
#endif//#ifdef FEATURE_WAP_REDIRECT
	}

	g_bDrawBar = TRUE;
    SKY_SetGeneralTimer(0, FALSE);	
    SKY_SetGeneralTimer(1000, TRUE); //ytchoi 700m-->1000ms수정
}


void WAP_Display_LoadingBar(int start_x, int end_x)
{
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 1, end_x, WAP_SOFTKEY_Y_POS + 1, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][5]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 2, end_x, WAP_SOFTKEY_Y_POS + 2, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][4]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 3, end_x, WAP_SOFTKEY_Y_POS + 3, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][3]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 4, end_x, WAP_SOFTKEY_Y_POS + 4, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][2]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 5, end_x, WAP_SOFTKEY_Y_POS + 5, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][1]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 6, end_x, WAP_SOFTKEY_Y_POS + 6, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][0]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 7, end_x, WAP_SOFTKEY_Y_POS + 7, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][0]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 8, end_x, WAP_SOFTKEY_Y_POS + 8, TRUE, WHITE);//g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][3]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 9, end_x, WAP_SOFTKEY_Y_POS + 9, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][0]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 10, end_x, WAP_SOFTKEY_Y_POS + 10, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][0]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 11, end_x, WAP_SOFTKEY_Y_POS + 11, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][1]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 12, end_x, WAP_SOFTKEY_Y_POS + 12, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][2]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 13, end_x, WAP_SOFTKEY_Y_POS + 13, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][3]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 14, end_x, WAP_SOFTKEY_Y_POS + 14, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][4]);
	WAP_Display_Rectangle(start_x, WAP_SOFTKEY_Y_POS + 15, end_x, WAP_SOFTKEY_Y_POS + 15, TRUE, g_WapLoadingBarColor[SKY_GET_COLOR_SCHEME()][5]);
}

void WAP_DrawLoadingBarToBackBuff(int nAniTextMode, BOOL bAniBarDirection)
{

	UINT8 nBarCount;
	//-------------------------------
	//skip drawing
	if(!g_bDrawBar) return;

    //-------------------------------
    //clear
    WAP_Display_Rectangle(0, WAP_SOFTKEY_Y_POS, 119, WAP_SOFTKEY_Y_POS+15,TRUE, WAP_BASIC_BACK_COLOR);//WHITE);

    //-------------------------------
	//text
	//g_nAniBarCount = (g_nAniBarCount + 1) % 9;
	nBarCount = g_nAniBarCount % 9;

	if(nBarCount < 7)//normal
	{
		switch(nAniTextMode)
		{
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
		}
	}
	else//반전
	{
		switch(nAniTextMode)
		{
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
		}
	}
	//-------------------------------
	//progress bar
	if(bAniBarDirection == WAP_ANI_BAR_FROM_RIGHT)
	{
		switch(nBarCount) //++중<-취소
		{
		case 8://반전
            break;
		case 7: WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_RIGHT_CML_X-1); break;
		case 6: WAP_Display_LoadingBar(WAP_UPDOWN_CML_X+6, WAP_RIGHT_CML_X-1); break;
		case 5 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X+12, WAP_RIGHT_CML_X-1); break;
		case 4 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X+18, WAP_RIGHT_CML_X-1); break;
		case 3 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X+26, WAP_RIGHT_CML_X-1); break;
		case 2 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X+34, WAP_RIGHT_CML_X-1); break;
		case 1 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X+42, WAP_RIGHT_CML_X-1); break;
		}
	}
	else
	{
		switch(nBarCount) //++중->취소
		{
		case 8://반전
             break;
		case 7: WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_RIGHT_CML_X-1); break;
		case 6: WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_UPDOWN_CML_X+38); break;
		case 5 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_UPDOWN_CML_X+30); break;
		case 4 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_UPDOWN_CML_X+22); break;
		case 3 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_UPDOWN_CML_X+15); break;
		case 2 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_UPDOWN_CML_X+9); break;
		case 1 : WAP_Display_LoadingBar(WAP_UPDOWN_CML_X, WAP_UPDOWN_CML_X+3); break;
		}
	}
 	//---------------------------------------------------
	// Loading 을 종료하기 위한 취소 키를 오른쪽에 넣어준다
	WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_CANCEL, WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);           


}

void WAP_DrawLoadingBar(int nAniTextMode, BOOL bAniBarDirection)
{
	// WAP_DrawLoadingBarToBackBuff()를 바로 call하는 경우가 있어 bar가 그려지는 time이 일정하지 않음 -- yjahn96 01.11.09
	g_nAniBarCount = (g_nAniBarCount + 1) % 9;
    //display at LCD
	WAP_DrawLoadingBarToBackBuff(nAniTextMode, bAniBarDirection);
	SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), 0, WAP_HEAD_HEIGHT + WAP_SOFTKEY_Y_POS, WAP_LCD_TOTAL_ADDRESS_X-1, WAP_LCD_TOTAL_ADDRESS_Y-1);//WAP_Send_LCDBuffer_ToHS();
}

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

#ifdef FEATURE_STI_DS
 	tmp_buf[0] = '\0';
	db_get(DB_SID, &dbi );
	sprintf(tmp_buf,";%d",dbi.sid);
	strcat(result, tmp_buf);
	length += strlen(tmp_buf);
#endif//#ifdef FEATURE_STI_DS
	
	return length;
} 

void BartimerTick(int4  interval)
{                        
    process_draw_bar();
}

#ifdef FEATURE_SKVM
int GoToSKVM( char* msdURL, char* retURL, int scvOption)
{
    if( msdURL == NULL || retURL == NULL ||
		scvOption > 1  || scvOption < 0 )
	{
		return 1;
		ERR("Invalid Parameter",0,0,0);		
	}	

    /* copy the data of script */
	memset((char*)g_pszmsdURL, 0x00, MSDURL_MAX);
	strncpy((char*)g_pszmsdURL, (const char*)msdURL, 100);

    memset((char*)g_pszretURL, 0x00, MSDURL_MAX);
	strncpy((char*)g_pszretURL, (const char*)retURL, 100);

	g_scvOption = scvOption ;
	
	/* in order to go to netgame menu after exit */
    SKY_SET_TO_SKVM(TRUE);

    EQS_SEND_EVENT(SE_GOTO_SKVM, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
		
	return 0;
}
#endif //FEATURE_SKVM

void wap_display_error_message(int kindOfError, int kindOfType)
{
    HCONTROL hMSGControl;
#ifdef FEATURE_SKT_EIF//EIF_WAP    //yjahn96 01.02.24 --- eif for wap
    extern INT   g_nEIFWapstatus;
	extern BOOL g_EIF_WAP_WAPCON;

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
            hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_INVALID_FORMAT_ERROR), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);
            EQC_SET_STYLE(hMSGControl, EQC_GET_STYLE(hMSGControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
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
            hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_NO_DOCUMENT_ERROR), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);
            EQC_SET_STYLE(hMSGControl, EQC_GET_STYLE(hMSGControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
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
            hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_NO_SERVER_ANSWER_ERROR), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);        
            EQC_SET_STYLE(hMSGControl, EQC_GET_STYLE(hMSGControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
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
            if(g_nEIFWapstatus == EIF_WAP_WAPCON || g_EIF_WAP_WAPCON == TRUE)
            {
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;    
				g_EIF_WAP_WAPCON = FALSE;
				eif_error_command(EIF_ERR_WAPCON_CONNECT_FAIL);	
				CLNTa_log(1,0, "WAPCON: Connection Failed");
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
            hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_WRONG_URL_ERROR), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);            
            EQC_SET_STYLE(hMSGControl, EQC_GET_STYLE(hMSGControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
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
			if(g_nEIFWapstatus == EIF_WAP_WAPCON || g_EIF_WAP_WAPCON == TRUE)
            {
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;    
				g_EIF_WAP_WAPCON = FALSE;
				eif_error_command(EIF_ERR_WAPCON_CONNECT_FAIL);	
				CLNTa_log(1,0, "WAPCON: Connection Failed");
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
#ifdef FEATURE_WAP_REDIRECT
			if(redirect.bCorrect)//redirect수행 
			{
				if(save_redirecturl())
				{			
					process_draw_bar(); //재설정 
                    EQS_SEND_EVENT(SE_WAP_REDIRECT, EQS_NA, EQS_NA);
					break;
				}				
			}
            else
#endif //#ifdef FEATURE_WAP_REDIRECT
            if(g_pCard == NULL)
            {
                g_bWAPinitAccessFail = TRUE;
                EQS_SEND_EVENT(SE_WAP_INITACCEESSFAIL, EQS_NA, EQS_NA);
				break;
            }
            else
            {
                hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_DEFAULT_ERROR), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);            
                EQC_SET_STYLE(hMSGControl, EQC_GET_STYLE(hMSGControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                break;
            }
		}
		default:
		{
            hMSGControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAP_DEFAULT_ERROR), MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_NOTITLE);            
            EQC_SET_STYLE(hMSGControl, EQC_GET_STYLE(hMSGControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
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
			if(g_nEIFWapstatus == EIF_WAP_WAPCON || g_EIF_WAP_WAPCON == TRUE)
            {
				extern void eif_error_command(int);
                g_nEIFWapstatus = EIF_WAP_NONE;    
				g_EIF_WAP_WAPCON = FALSE;
				eif_error_command(EIF_ERR_WAPCON_CONNECT_FAIL);	
				CLNTa_log(1,0, "WAPCON: Connection Failed");
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

#ifdef FEATURE_WAP_REDIRECT
BOOL save_redirecturl(void)
{	
	if(redirect.Address == NULL)
	{
		return FALSE;
	}
	memset(gWAPproxy_url,'\x00',130);
	if(redirect.Addresslen > 130) return FALSE;
	else
		memcpy(gWAPproxy_url,redirect.Address,redirect.Addresslen);


	if(redirect.bPerm_or_not)
	{		 
		 SKY_SET_WAP_PROXY_URL(gWAPproxy_url); //(void)ui_put_nv( NV_PROXY_I, ( nv_item_type * )proxy_url);
	}	
	CLNTa_log(1,0,"proxy_url[%s] ,addresslen=%d,redirect url [%s]",gWAPproxy_url,redirect.Addresslen,redirect.Address);

	if(redirect.Address !=NULL)
	{
		wip_free(redirect.Address);
		redirect.Address = NULL;
	}

	return TRUE;
}
#endif //FEATURE_WAP_REDIRECT

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     WAP_Time_Stmap()
**
** Description: 
**     wap 사용시간을 set하는 함수 
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
void WAP_Time_Stmap(BOOL bStart)
{
	extern void ntop_used_time ( dword new_time );
	if(bStart)
	{
		g_nTimeStamp = ELAPSED_SECONDS + SKY_GetLocalTime();//종료	
		//CLNTa_log(1, 0, "시작시각 %d", g_nTimeStamp);
	}
	else
	{
		UINT32 nUsedTime;
		if(g_nTimeStamp == 0)   return; //<= 0) return;
		nUsedTime = (ELAPSED_SECONDS + SKY_GetLocalTime()) - g_nTimeStamp;
		//CLNTa_log(1, 0, "끝 (%d초 사용)", nUsedTime);
		ntop_used_time(nUsedTime);
		g_nTimeStamp = 0;
	}

}

void ntop_used_time ( dword new_time )
{
	/* Save the last call time */
	SKY_SET_WAP_LAST_USING_TIME(new_time);
	/* Save counters and timers */
	SKY_SET_WAP_CUMULATION_USING_TIME(SKY_GET_WAP_CUMULATION_USING_TIME()+ new_time);
	/* Save Life  timers */
	SKY_SET_WAP_TOTAL_USING_TIME(SKY_GET_WAP_TOTAL_USING_TIME()+ new_time);	
}

#endif //FEATURE_WAP