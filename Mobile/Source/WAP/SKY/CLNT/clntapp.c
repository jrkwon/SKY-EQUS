/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*



           Wireless Application Protocol   Client Function

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/21/00   swjung  
===========================================================================*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
#endif //#ifdef WAP_NEO

#include "logcodes.h"
#include "errcodes.h"
#include "confvars.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "si_libs.h"

#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif

#include "GenDsp.h"
#include "mmi_data.h"
#include "mmi_func.h"

#include "queue.h"


#include "customer.h"

#include "clnt.h"
#include "clntapp.h"
#include "wapconnector.h"
#include "waptask.h"

#if MMI_TYPE == SKY_UI  //add_htg_00_xmas : To adapt for EQUS
#include "sky.h"
#include "ska.h"
#endif

#include "msg.h"
#include "err.h"
#ifdef WAP95C_TEST
#include"mc.h"
#include "dsi.h"
#endif //WAP95C_TEST

///////////////////////////////////////////////////////////////////////////////////////////////////
// Global variables for setImage. Extra WMLScript Library Multimedia
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// Block showcard function in case of error, 
// by doing this, LCD prevent from flip-floping between error message and normal message
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FEATURE_PIM_INVALIDERROR
BOOL g_PIM = FALSE; //다시 Reset 
#endif //FEATURE_PIM_INVALIDERROR
BOOL isIntegerResponse = FALSE;

nv_item_type		item;

boolean	isErrorState = FALSE;
static	ui_cmd_type   *ui_cmd_ptr;
int		typeOfContent;
setImageType	imageInfo;

#ifdef FEATURE_MUSIC_BELL
	DownMusicbellType   MusicbellInfo;
#endif //FEATURE_MUSIC_BELL

/*====================================================================
	Music bell download시 저장이 Clnta_content가 불리지 않을 경우에만 
	사용
 ====================================================================*/
BOOL isStringResponse = FALSE;

/*====================================================================
    Define the added number of application funtions 
 =====================================================================*/
#ifdef FEATURE_MCOMMERCE
	#define NUM_MCOMMERCE 1
#else
	#define NUM_MCOMMERCE 0
#endif//mcommerce
/*====================================================================*/

#ifdef USE_PROPRIETARY_WMLS_LIBS 

const ExtraLibInfo			arrayExtraScriptLib[3+NUM_MCOMMERCE][20] =
{
	/* Multimedia 32000/0 */	
#ifdef FEATURE_MUSIC_BELL
	{	{Call_Multimedia_setImage, 4},		{Call_Musicbell_Play, 4},
		{Call_Musicbell_Stop, 0},			{Call_Musicbell_DownLoad, 4},
		{Call_Musicbell_GetListTitle, 0},	{Call_Musicbell_MemPlay, 1},
		{Call_Musicbell_DeleteList, 1},		{NULL, 0}, 
#else //#ifdef FEATURE_MUSIC_BELL
	{	{Call_Multimedia_setImage, 4},		{NULL, 0},
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
#endif //#ifdef FEATURE_MUSIC_BELL
//#ifdef FEATURE_GVM
//		{Call_Netgame_Gamerun, 8},			{NULL, 0}, 
//#else //FEATURE_GVM
        {NULL, 0},							{NULL, 0}, 
//#endif //FEATURE_GVM
#ifdef FEATURE_SK_VM
		{Call_SKVM_SetURL, 3},				{NULL, 0}, 
#else  //FEATURE_SK_VM
        {NULL, 0},							{NULL, 0}, 
#endif //FEATURE_SK_VM
#ifdef FEATURE_WAP_WAVELET
		{Call_Wavelet, 12},					{NULL, 0}, 
#else
        {NULL, 0},							{NULL, 0}, 
#endif //FEATURE_WAP_WAVELET
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0} 
	},

	/* Terminal 32001/0 */
#if defined(WAP_SETURL)||defined(WAP95C_TEST)
	{	{Call_Terminal_setURL, 2},			{Call_Terminal_sendMail, 0}, 
		{Call_Terminal_setDormantTime, 1},	{Call_Terminal_muxOption, 1}, 
#else //WAP_SETURL
	{	{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
#endif //(WAP_SETURL)||(WAP95C_TEST)
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0} 
	},

	/* Phonebook 32002/0 */
#ifdef NEW_VERSION
	{	{Call_Terminal_isModifyAll, 2},		{Call_Terminal_isExist, 1},
#else //#ifdef NEW_VERSION
	{	{Call_Terminal_isModifyAll, 0},		{Call_Terminal_isExist, 1},
#endif //#ifdef NEW_VERSION
		{Call_Terminal_isGroup, 1},			{Call_Terminal_getMaxEntry, 0},
		//{Call_Terminal_isGroup, 1},			{Call_Terminal_sendMail, 0},
		{Call_Terminal_getMaxGroup, 0},		{Call_Terminal_getMaxShortKey, 0},
#ifdef NEW_VERSION
		{Call_Terminal_getPhonebook, 0},	{Call_Terminal_getDeletedEntry, 2},
#else //#ifdef NEW_VERSION
		{Call_Terminal_getPhonebook, 0},	{Call_Terminal_getDeletedEntry, 0},
#endif //#ifdef NEW_VERSION
		{Call_Terminal_getGroup, 0},		{Call_Terminal_setGroup, 2},		
		{Call_Terminal_getEntry, 1},		{Call_Terminal_deleteEntry, 1},	
		{Call_Terminal_setEntry, 2},		{Call_Terminal_getVerID, 0},
#ifdef FEATURE_PHONEBOOK_VER12
		{Call_Terminal_getEntryList, 4},	{NULL, 0}, 
#else //#ifdef FEATURE_PHONEBOOK_VER12
		{NULL, 0},							{NULL, 0}, 
#endif //#ifdef FEATURE_PHONEBOOK_VER12
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0} 
	}
	
#ifdef FEATURE_MCOMMERCE
	,
	/* M-commerce 32003/0 */
	{	{Call_Mcommerce_testScriptA, 1},	{Call_Mcommerce_testScriptB, 2}, 
		{Call_Mcommerce_testScriptC, 2}, 	{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0}, 
		{NULL, 0},							{NULL, 0} 
	}
#endif //#ifdef FEATURE_MCOMMERCE
};


const ExtraLibArgInfo			arrayExtraScriptLibArg[3+NUM_MCOMMERCE][20] =
{
	/* Multimedia 32000/1 */
#ifdef FEATURE_MUSIC_BELL
	{	{ 2, 2, 0, 0,-1,-1,-1,-1},			{ 2, 2, 2, 2,-1,-1,-1,-1},
		//No argument
		{-1,-1,-1,-1,-1,-1,-1,-1},			{ 2, 2, 2, 2,-1,-1,-1,-1},
		//No argument
		{-1,-1,-1,-1,-1,-1,-1,-1},			{ 2,-1,-1,-1,-1,-1,-1,-1},
		{ 2,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
#else //#ifdef FEATURE_MUSIC_BELL
	{	{ 2, 2, 0, 0,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
#endif //#ifdef FEATURE_MUSIC_BELL
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1}
	},

	/* Terminal 32001/0 */
#ifdef WAP_SETURL
	{	{ 2, 0,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
#else //WAP_SETURL

	{	{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
#endif //WAP_SETURL
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1}
	},

	/* Phonebook */
	{	{ 0, 0,-1,-1,-1,-1,-1,-1},			{ 2,-1,-1,-1,-1,-1,-1,-1},
		{ 2,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{ 0, 0,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{ 0, 2,-1,-1,-1,-1,-1,-1},
		{ 0,-1,-1,-1,-1,-1,-1,-1},			{ 0,-1,-1,-1,-1,-1,-1,-1},
		{ 0, 2,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},

		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1}
	}
	
#ifdef FEATURE_MCOMMERECE
	,
	/* M-commerce */
	{	{ 1,-1,-1,-1,-1,-1,-1,-1},			{ 1,-1,-1,-1,-1,-1,-1,-1},
		{ 2, 0,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1},			{-1,-1,-1,-1,-1,-1,-1,-1}
	}
#endif //#ifdef FEATURE_MCOMMERECE
};

#endif //#ifdef USE_PROPRIETARY_WMLS_LIBS



///////////////////////////////////////////////////////////////////////////////////////////////////
// External global variables
///////////////////////////////////////////////////////////////////////////////////////////////////
extern char	*cszErrorCode;
extern int	maxLength;
extern int	maxCount;
extern BOOL g_bGettingContent;//init for Picture Mate/Music Bell
extern BOOL CheckDiskSpaceForSetImage(void);
extern BOOL CheckDiskSpaceForSetAudio(void);


#define ELAPSED_SECONDS  315964800


uint32 tid_list;
int first_flag_tid = 0;



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions for adapter functions
///////////////////////////////////////////////////////////////////////////////////////////////////

void timerInsert(uint32 ticks)
{
	tid_list = ticks;
}

void timerDelete(void)
{
	tid_list = 0; 
}

void timerTick(int4  interval)
{                        
	if( tid_list ) 
	{
		tid_list = tid_list - 1;

		if(tid_list == 0) 
		{
			extern void ProcessEvents();
            CLNTc_timerExpired();
            ProcessEvents();
		}
	}
}

void timerTerminate(void)
{
    tid_list = 0;
}



void get_content(char* url, unsigned short invokeId)
{
	//extern void CancelAnimationTimer(void);

	wap_cmd_type msg_ptr;
	
	MSG_HIGH("-- GET_CONTENT --",0,0,0);
	CLNTa_log(1, 0, "[get_content] url[%s]\n",url);

	msg_ptr.hdr.cmd = CLNT_GET_CONTENT;
	msg_ptr.get_content.url = url;
	msg_ptr.get_content.urlId = invokeId;
	msg_ptr.get_content.isOTAallowed = TRUE;

	make_cmd_other2wap( &msg_ptr );	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Contents getting method by decoding wml vendor type 
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef BLOCK
	void wap_call_get_content(int kindOfContent, int index)
	{
		switch(typeOfContent=kindOfContent)
		{
			case IMAGE_BMP:
			case IMAGE_WBMP:
			case IMAGE_ANIMATION:
			case MYBELL:
				get_content(&mybellTable[index-1][0]); break;
			default:
				break;
		}
	#ifdef DEBUG_CLNT
		CLNTa_log(1, 0, "[wap_call_connector] kindOfContent:[%d], index:[%d]\n",kindOfContent,index);
	#endif //#ifdef DEBUG_CLNT
	}
#endif //BLOCK



///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions for extra WMLScript libraries
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FEATURE_WAPMAIL
extern WAP_MAIL_TYPE g_wapmaildata;
extern BOOL g_bDrawBar;

void sendMail(char *result)
{

	CLNTa_log(1, 0, "sendMail] [%s]", g_wapmaildata.Memo);
	//MAX_SMS_TEXT=85
	memcpy((byte *)result, (void *)g_wapmaildata.Memo,WAPMAIL_SIZE-1); //size는 다시 check
}
#endif //FEATURE_WAPMAIL

#ifdef WAP_B1 //bom for tmp
extern unsigned short  wap_duration;
#endif //WAP_B1

extern nv_stat_enum_type wap_process_nv_item
(
  nv_items_enum_type  item_code,       /* Item to get                      */
  nv_item_type        *item_ptr,        /* Pointer where to put the item    */
  boolean              read			   /* read = TRUE; READ_F, read = FALSE;WRITE_F */
);

int setDormantTime(unsigned short invokeId, int dormantTime)
{
#ifndef FEATURE_WAP
#ifdef WAP_B1 //bom for tmp
	wap_duration = dormantTime;
#endif //#ifdef WAP_B1 //bom for tmp
	SKY_SET_WAP_DORMANT_TIME(dormantTime);    
	CLNTa_log(1, 0, "dormant time[%d]\n",SKY_GET_WAP_DORMANT_TIME());
    dsi_cancel_timer( DS_RLP_IDLE_TIMEOUT_SIG);
    /* ytchoi 01/06/14 script실행후 time이 바로 update가 되지 않아서 수정*/
    if(wap_duration !=0)
    {
        dsmgr_start_timer( DS_RLP_IDLE_TIMEOUT_SIG,(wap_duration * 1000));
    }    
#endif //#ifndef FEATURE_WAP
	return 0;//Success
}

#ifdef WAP95C_TEST
int muxOption(unsigned short invokeId, int Option)
{
	mc_msg_type	*msg_ptr;
	word p_rev_index;
	extern word ds_socfg_get_p_rev_index(void);

	p_rev_index = ds_socfg_get_p_rev_index();
	if (p_rev_index>=DS_BS_PREV_6)
	{
		CLNTa_log(1, 0, "muxOption[%d]\n",Option);

		if(( ui_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
		{
		 	MSG_ERROR("No UI cmd buffers",0,0,0);
		}
		else		
		{
			ui_cmd_ptr->hdr.cmd        = UI_WAP_GETCONTENT_F;//go to LOADING_S
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
		}
		switch ( Option )
		{			
			case 905:
				{
					CLNTa_log(1,0,"0x905 select");
					if(( msg_ptr = (mc_msg_type *) q_get( &mc_cmd_free_q )) == NULL ) 
					{
						CLNTa_log(1,0," MC buffer");
					} 
					else 
					{
						msg_ptr->hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
						msg_ptr->hdr.cmd_hdr.task_ptr = NULL;
					}
					msg_ptr->hdr.cmd = MC_WAP_F;

					msg_ptr->waptest.select = 1;			
					mc_cmd(msg_ptr);
					break;
				}
			case 909:
				{

					CLNTa_log(1,0,"0x909 select");
					if(( msg_ptr = (mc_msg_type *) q_get( &mc_cmd_free_q )) == NULL ) 
					{
						CLNTa_log(1,0," MC buffer");
					} 
					else 
					{
						msg_ptr->hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
						msg_ptr->hdr.cmd_hdr.task_ptr = NULL;
					}
					msg_ptr->hdr.cmd = MC_WAP_F;
					
					msg_ptr->waptest.select = 2;
					mc_cmd(msg_ptr);
					break;
				}
			case 911:
				{
					CLNTa_log(1,0,"0x911 select");
					if(( msg_ptr = (mc_msg_type *) q_get( &mc_cmd_free_q )) == NULL ) 
					{
						CLNTa_log(1,0," MC buffer");
					} 
					else 
					{
						msg_ptr->hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
						msg_ptr->hdr.cmd_hdr.task_ptr = NULL;
					}
					msg_ptr->hdr.cmd = MC_WAP_F;
					
					msg_ptr->waptest.select = 3;
					mc_cmd(msg_ptr);
					break;
				}
			case 921:
				{			
					CLNTa_log(1,0,"0x921 select");
					if(( msg_ptr = (mc_msg_type *) q_get( &mc_cmd_free_q )) == NULL ) 
					{
						CLNTa_log(1,0," MC buffer");
					} 
					else 
					{
						msg_ptr->hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
						msg_ptr->hdr.cmd_hdr.task_ptr = NULL;
					}
					msg_ptr->hdr.cmd = MC_WAP_F;
					
					msg_ptr->waptest.select = 4;
					mc_cmd(msg_ptr);
					break;
				}
			default: //9.6kbps
				{
					CLNTa_log(1,0,"원상태");
					if(( msg_ptr = (mc_msg_type *) q_get( &mc_cmd_free_q )) == NULL ) 
					{
						CLNTa_log(1,0," MC buffer");
					} 
					else 
					{
						msg_ptr->hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
						msg_ptr->hdr.cmd_hdr.task_ptr = NULL;
					}
					msg_ptr->hdr.cmd = MC_WAP_F;
					
					msg_ptr->waptest.select = 5;
					mc_cmd(msg_ptr);
					break;
				}
		}
	return 0;//Success
	}
	else // not support
	{
		return 1;//Fail
	}	
}
#endif //WAP95C_TEST
void setImage(unsigned short invokeId, char* url, char* title, int kindOfImage, int imageType)
{
	extern void CancelAnimationTimer(void);

	MSG_ERROR("setImage Is called",0,0,0);
	MSG_ERROR("kindOfImage[%] imageType[%d]",kindOfImage,imageType,0);

	if (!CheckDiskSpaceForSetImage())
	{
		MSG_ERROR("[setImage]Not enough memory",0,0,0);
	}

	g_bGettingContent = TRUE;
	
	CancelAnimationTimer(); // 010309. Do not animate more when downloading

	//========================================================
	//Message for user
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, "이미지를", WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "저장중입니다", WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(4, "기다려주세요", WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
	//========================================================

	strcpy(imageInfo.url, url);

	get_content(imageInfo.url, invokeId);

	if(title == NULL)	imageInfo.title[0] = '\0';	
	else	strcpy(imageInfo.title, title);
	imageInfo.kindOfImage = kindOfImage;
	imageInfo.imageType = typeOfContent = imageType;
}

#ifdef WAP_SETURL// ytchoi Uicommon.c함수
int setURL(char *URL, int kindOfURL)
{
	
	byte tURL[MAX_WAPMAIL_LENGTH+1]; //131 for null temp URL save

	MSG_HIGH("!!!setURL is called!!!",0,0,0);
	// Check URL validation
	if(!strlen(URL)) return INVALID_URL;

	switch (kindOfURL)
	{
	case INITIAL_URL:
		MSG_HIGH("초기URL 변경",0,0,0);
		memset((void *) tURL, 0, (MAX_WAPMAIL_LENGTH+1));
		memcpy( (void*)tURL, URL, strlen(URL));
		//wap_process_nv_item(NV_INIT_URL_I,(nv_item_type *)&nvi,FALSE);
		SKY_SET_WAP_INIT_URL(tURL);
		break;
	
	case GATEWAY_ADDR:
		MSG_HIGH("G/W URL 변경",0,0,0);
		memset((void *) tURL, 0, (MAX_WAPMAIL_LENGTH+1));
		memcpy( (void*)tURL, URL, strlen(URL));
		SKY_SET_WAP_PROXY_URL(tURL);
		//wap_process_nv_item(NV_PROXY_I,(nv_item_type *)&nvi,FALSE);
		break;

	case NTOP_EMAIL:
		MSG_HIGH("NTOP EMAIl URL 변경",0,0,0);
		memset((void *) tURL, 0, (MAX_WAPMAIL_LENGTH+1));
		memcpy( (void*)tURL, URL, strlen(URL));
		//wap_process_nv_item(NV_INITEMAIL_I,(nv_item_type *)&nvi,FALSE);
		SKY_SET_WAP_INIT_MAIL(tURL);
		break;

	default: 
		return INVALID_PARAMETER; 
	}

	return SUCCESS; 
}
///////////////////////////////////////////////////////////////
// Terminal library end
///////////////////////////////////////////////////////////////
#endif //WAP_SETURL


#ifdef FEATURE_MUSIC_BELL
void Musicbell_play_tmp(unsigned short invokeId, char* URL,char * Title, char* AudioType, char* PlayType)
{
	extern BOOL g_bStopMusic;

	//CLNTa_log(1,0,"Musicbell_play_tmp is called");
	g_bStopMusic = FALSE;

	//Content를 받기 시작한다
	g_bGettingContent = TRUE;
#ifndef FEATURE_WAP
	//========================================================
	if(( ui_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
	{
		//ERR_FATAL( "No UI cmd buffers",0,0,0);
		MSG_ERROR("No UI cmd buffers",0,0,0);
	}
	else		
	{
		ui_cmd_ptr->hdr.cmd        = UI_WAP_GETCONTENT_F;//go to LOADING_S
		ui_cmd_ptr->hdr.task_ptr   = NULL;
		ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		ui_cmd( ui_cmd_ptr);
	}
#endif //#ifndef FEATURE_WAP
	//========================================================
	//Message for user
	WAP_ClearScreen();
	WAP_SetTextAtLine(3, "연주 준비중입니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
	//========================================================

	strcpy(MusicbellInfo.URL, URL);

	get_content(MusicbellInfo.URL, invokeId);

	strcpy(MusicbellInfo.Title, Title);
	strcpy(MusicbellInfo.PlayType, PlayType);
	strcpy(MusicbellInfo.AudioType, AudioType);
	typeOfContent = MUSICBELL_TYPE_PLAY;
}


void Musicbell_download_tmp(unsigned short invokeId, char* URL, char* Title, char* AudioType, char* Attribute, char* Result)
{
	//Content를 받기 시작한다
	//g_bGettingContent = TRUE;
	if (CheckDiskSpaceForSetAudio())
	{
		g_bGettingContent = TRUE;
#ifndef FEATURE_WAP
		if(( ui_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
		{
			//ERR_FATAL( "No UI cmd buffers",0,0,0);
			MSG_ERROR("No UI cmd buffers",0,0,0);
		}
		else		
		{
			ui_cmd_ptr->hdr.cmd        = UI_WAP_GETCONTENT_F;//go to LOADING_S
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
		}
#endif //#ifndef FEATURE_WAP
		//========================================================
		//Message for user
		WAP_ClearScreen();
		WAP_SetTextAtLine(2, "뮤직벨을", WAP_TEXT_ALIGN_CENTER);
		WAP_SetTextAtLine(3, "저장중입니다", WAP_TEXT_ALIGN_CENTER);
		WAP_SetTextAtLine(4, "기다려주세요", WAP_TEXT_ALIGN_CENTER);
		WAP_DisplayMessage();
		//========================================================

		strcpy(MusicbellInfo.URL, URL);

		get_content(MusicbellInfo.URL, invokeId);
		
		//strcpy(MusicbellInfo.URL, URL);
		strcpy(MusicbellInfo.Title, Title);
		strcpy(MusicbellInfo.AudioType, AudioType);
		strcpy(MusicbellInfo.Attribute, Attribute);
		typeOfContent = MUSICBELL_TYPE_DOWN;

#ifdef DIRECT_RESPONSE
		strcpy(Result, MUSICBELL_SUCCESS);
#endif //#ifdef DIRECT_RESPONSE
	}
	else
	{
		CLNTa_log(1, 0, "[Musicbell_download_tmp]Not enough memory\n");	
		strcpy(Result, BUFFER_FULL);
	}
}

#endif //FEATURE_MUSIC_BELL

#ifdef FEATURE_MCOMMERCE

#ifdef BLOCK
void testScriptA(char *dn, char** Result)
{	
	CLNTa_log(1, 0, "[testScriptA] Before call createCSR");	

	*Result = createCSR((const char *) dn);

	CLNTa_log(1, 0, "[testScriptA] After  call createCSR");	
}
#endif //#ifdef BLOCK

void testScriptA(void)
{
#ifndef FEATURE_WAP
	extern int testEcc(void);

	CLNTa_log(1, 0, "[testScriptA] Before call ECC Test");	

	testEcc();

	CLNTa_log(1, 0, "[testScriptA] After  call ECC Test");	
#endif //#ifndef FEATURE_WAP
}


void testScriptB(char *input, char *cert, char** Result)
{
	CLNTa_log(1, 0, "[testScriptB] Before call pkEncryptData");	

//	*Result = pkEncryptData((const char *) input, (const char *) cert);

	CLNTa_log(1, 0, "[testScriptB] After  call pkEncryptData");	
}


void testScriptC(char *toBeSigned, int algo, char** Result)
{
	CLNTa_log(1, 0, "[testScriptC] Before call pkSignData");	

//	*Result = pkSignData(toBeSigned, algo);

	CLNTa_log(1, 0, "[testScriptC] After  call pkSignData");	
}
#endif //#ifdef FEATURE_MCOMMERCE
///////////////////////////////////////////////////////////////////////////// 


#ifdef BLOCK
int clnt_convert_string2integer(char *pInputString)
{
	int i, order;
	int result = 0;

	//Get an integer order of string
	order = strlen(pInputString);

	for(i = 0; i < order; i++)
	{
		result = atoi(pInputString[(order-1)-i]);
		if ((result < 48) || (result > 57))
		{
			return -1;
		}
	}
	
	for(i = 0, result = 0; i < order; i++)
	{
		result += (atoi(pInputString[(order-1)-i]) * pow(10, i));
	}

	return result;
}
#endif //#ifdef BLOCK


// !!!!!! NOTICE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Memory addressed by pStringResult should be allocated or 
// prepared by application before calling clnt_make_string_response
// Free of this memory should be processed after function clnt_make_string_response
// !!!!!! NOTICE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void clnt_convert_string_response(char *pStringResult, WMLSvar *pResult)
{
	if (pResult->type == TYPE_STRING)
	{
		CLNTa_log(1,0,"clnt_convert_string_response] pStringResult[%s]",pStringResult);
		pResult->stringVal = pStringResult;
		//CLNTa_log(1,0,"clnt_convert_string_response] pResult->stringVal[%s]",pResult->stringVal);
		pResult->stringLengthInBytes = strlen(pStringResult);
		//CLNTa_log(1,0,"clnt_convert_string_response] pResult->stringLengthInBytes [%d]",pResult->stringLengthInBytes );
		pResult->stringIANAcharset = KS_C_5601_1987;
	}
}

#ifdef BLOCK
void clnt_convert_string_response_no_convert(char *pStringResult, WMLSvar *pResult)
{
	if (pResult->type == TYPE_STRING)
	{
		CLNTa_log(1,0,"clnt_convert_string_response] pStringResult[%s]",pStringResult);
		pResult->stringVal = pStringResult;
		//CLNTa_log(1,0,"clnt_convert_string_response] pResult->stringVal[%s]",pResult->stringVal);
		pResult->stringLengthInBytes = strlen(pStringResult);
		//CLNTa_log(1,0,"clnt_convert_string_response] pResult->stringLengthInBytes [%d]",pResult->stringLengthInBytes );
		pResult->stringIANAcharset = 777;
	}
}
#endif //#ifdef BLOCK


WCHAR wide_result_buffer[100] = {0};

void clnt_convert_wstring_response(char *pStringResult, WMLSvar *pResult)
{
	if (pResult->type == TYPE_STRING)
	{
		KSC2UniString((unsigned char*)pStringResult, wide_result_buffer);

		CLNTa_log(1,0,"clnt_convert_wstring_response] pStringResult[%s]",pStringResult);
		pResult->stringVal = (char *)wide_result_buffer;
		CLNTa_log(1,0,"clnt_convert_wstring_response] pResult->stringVal[%s]",pResult->stringVal);
		pResult->stringLengthInBytes = strlen(pStringResult)*2;
		CLNTa_log(1,0,"clnt_convert_wstring_response] pResult->stringLengthInBytes [%d]",pResult->stringLengthInBytes );
		pResult->stringIANAcharset = KS_C_5601_1987;
	}
}

void clnt_make_string_response(unsigned short invokeId, char *pStringResult)
{
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	//wap_cmd_type msg_ptr;

	pResult->type = TYPE_STRING;

	CLNTa_log(1,0,"clnt_make_string_response] pStringResult[%s]",pStringResult);

	clnt_convert_string_response(pStringResult, pResult);

	//pResult->stringVal = "0";
	CLNTc_WMLSLibFuncResponse (invokeId, pResult);

	//msg_ptr.hdr.cmd = CLNT_WMLS_RESPONSE;
	//msg_ptr.wmls_response.invokeId = invokeId;	
	//msg_ptr.wmls_response.pWMLSvar = pResult;	

	//make_cmd_other2wap( &msg_ptr );	

	WIPFree(pResult);
}

void clnt_make_integer_response(unsigned short invokeId, int integerResult)
{
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	pResult->type = TYPE_INTEGER;

	//CLNTa_log(1,0,"clnt_make_integer_response] integerResult[%d]",integerResult);

	pResult->intVal = integerResult;

	CLNTc_WMLSLibFuncResponse (invokeId, pResult);

	WIPFree(pResult);
}

void clnt_init_return_value(WMLSvar * pVariable)
{
	pVariable->type = TYPE_INVALID;
#ifdef BLOCK
	pVariable->intVal = TYPE_INVALID;
	pVariable->boolVal = TYPE_INVALID;
	pVariable->floatVal = TYPE_INVALID;
	pVariable->stringIANAcharset = TYPE_INVALID;
	pVariable->stringLengthInBytes = TYPE_INVALID;
#endif //#ifdef BLOCK
	pVariable->stringVal = NULL;
}



void clnt_alloc_string(const WMLSvar * const *params, char **argString, UINT8 nbrOfArgs)
{
	int	i, j;

	for ( i=0 ; i < nbrOfArgs ; i++)
		argString[i] = (char *)NULL;

	for ( i=0, j=0 ; i < nbrOfArgs ; i++)
	{
		if(params[i]->type == typeString)
		{
			unsigned short sizeOfString;

			sizeOfString = KSCStrLenOfUni((WCHAR *)params[i]->stringVal);

			CLNTa_log(1,0,"clnt_alloc_string] size[%d]", sizeOfString);

			//argString[j] = (char *)WIPAlloc(sizeOfString+1);
			//Uni2KSCString((WCHAR*)params[i]->stringVal, (unsigned char *)argString[j]);

			if(sizeOfString)
			{
				argString[j] = (char *)WIPAlloc(sizeOfString+2);

				Uni2KSCString((WCHAR*)params[i]->stringVal, (unsigned char *)argString[j]);
			}

			CLNTa_log(1,0,"clnt_alloc_string] string[%s] \n size[%d] index:j[%d]", argString[j], sizeOfString,j);

			j++;
		}		
	}
	argString[j] = NULL;
}

#ifdef BLOCK
void clnt_alloc_string_no_convert(const WMLSvar * const *params, char **argString, UINT8 nbrOfArgs)
{
	int	i, j;

	for ( i=0 ; i < nbrOfArgs ; i++)
		argString[i] = (char *)NULL;

	for ( i=0, j=0 ; i < nbrOfArgs ; i++)
	{
		if(params[i]->type == typeString)
		{
			unsigned short sizeOfString;

			sizeOfString = strlen(params[i]->stringVal);

			CLNTa_log(1,0,"clnt_alloc_string] size[%d]", sizeOfString);

			//argString[j] = (char *)WIPAlloc(sizeOfString+1);
			//Uni2KSCString((WCHAR*)params[i]->stringVal, (unsigned char *)argString[j]);

			if(sizeOfString)
			{
				argString[j] = (char *)WIPAlloc(sizeOfString+1);

				strcpy(argString[j], params[i]->stringVal);
			}

			CLNTa_log(1,0,"clnt_alloc_string] string[%s] \n size[%d] index:j[%d]", argString[j], sizeOfString,j);

			j++;
		}		
	}
	argString[j] = NULL;
}
#endif //#ifdef BLOCK

boolean clnt_alloc_string2(const WMLSvar * const *params, char **argString)
{

	if(params[0]->type == typeString)
	{
		unsigned short sizeOfString;

		sizeOfString = KSCStrLenOfUni((WCHAR *)params[0]->stringVal);

		CLNTa_log(1,0,"clnt_alloc_string] size[%d]", sizeOfString);

		//argString[0] = (char *)WIPAlloc(sizeOfString+1);
		//Uni2KSCString((WCHAR*)params[0]->stringVal, (unsigned char *)argString[0]);

		if(sizeOfString)
		{
			argString[0] = (char *)WIPAlloc(sizeOfString+2);

			Uni2KSCString((WCHAR*)params[0]->stringVal, (unsigned char *)argString[0]);
		}

		CLNTa_log(1,0,"clnt_alloc_string2] string[%s] \n size[%d]", argString[0], sizeOfString);

		argString[1] = NULL;

		return TRUE;
	}
	else
	{
		CLNTa_log(1,0,"clnt_alloc_string2]Integer case");
		return FALSE;
	}
}

//jjkyg add ==========================================================================================
int clnt_alloc_string3(const WMLSvar * const *params, char **argString, INT32* argint, UINT8 nbrOfArgs)
{
	int	i, j, k;
	int ret = 0;	// setImage(string, string, int, int)

	CLNTa_log(1,0, "clnt_alloc_string] nbrOfArgs [%d]", nbrOfArgs);

	for ( i=0 ; i < nbrOfArgs ; i++)
		argString[i] = (char *)NULL;

	for ( i=0, j=0,k=0; i < nbrOfArgs ; i++)
	{
		if(params[i]->type == typeString)
		{
			unsigned short sizeOfString;

			sizeOfString = KSCStrLenOfUni((WCHAR *)params[i]->stringVal);

			if (sizeOfString != 0)
			{
				argString[j] = (char *)WIPAlloc(sizeOfString+2);
				Uni2KSCString((WCHAR*)params[i]->stringVal, (unsigned char *)argString[j]);
			}

			//else
			//{
			//	argString[j] = (char *)WIPAlloc(11);
			//	argString[j] = "제목 없음";
			//}

			CLNTa_log(1,0,"clnt_alloc_string] string[%s] \n size[%d] index:j[%d]", argString[j], sizeOfString,j);

			j++;
		}
		//jjkyg add..
		else if(params[i]->type == typeInteger)
		{
			ret = 1;
			argint[k] = params[i]->intVal; k++;		
		}
	}
	
	argString[j] = NULL;

	return ret;
}
//yjahn96 add ==========================================================================================
int clnt_alloc_string4(const WMLSvar * const *params, char **argString, INT32* argint, UINT8 nbrOfArgs)
{
	int	i, j, k;
	int ret = 0;	// setImage(string, string, int, int)

	CLNTa_log(1,0, "clnt_alloc_string] nbrOfArgs [%d]", nbrOfArgs);

	for ( i=0 ; i < nbrOfArgs ; i++)
		argString[i] = (char *)NULL;

	for ( i=0, j=0,k=0; i < nbrOfArgs ; i++)
	{
		if(params[i]->type == typeString)
		{
			unsigned short sizeOfString;

			sizeOfString = KSCStrLenOfUni((WCHAR *)params[i]->stringVal);

			if (sizeOfString != 0)
			{
				argString[j] = (char *)WIPAlloc(sizeOfString+2);
				Uni2KSCString((WCHAR*)params[i]->stringVal, (unsigned char *)argString[j]);
			}

			//else
			//{
			//	argString[j] = (char *)WIPAlloc(11);
			//	argString[j] = "제목 없음";
			//}

			CLNTa_log(1,0,"clnt_alloc_string] string[%s] \n size[%d] index:j[%d]", argString[j], sizeOfString,j);

			j++;
		}
		//jjkyg add..
		else if(params[i]->type == typeInteger)
		{
			//ret = 1;
			argint[k] = params[i]->intVal; k++;		
		}
	}

    if(k ==2) ret = 1;
    else if(k == 1) ret = 2;
	
	argString[j] = NULL;

	return ret;
}
//jjkyg add ==========================================================================================

void clnt_free_string(char **argString, UINT8 nbrOfArgs)
{
	int i;

	for	(i=0; i< nbrOfArgs; i++)
	{
		if(argString[i] != NULL) WIPFree((char*) argString[i] );
	}
	argString[nbrOfArgs] = NULL;
}

WMLSvar*  clnt_decide_library_response(BOOL isSeparateResponse, BOOL *pSeparateResponse, WMLSvar *pResult, char *pResultString)
{
	
	if (isSeparateResponse)
	{
		//CLNTa_log(1,0,"clnt_decide_library_response] SeparateResponse");

		*pSeparateResponse = TRUE;

		return NULL;
	} 
	else 
	{
		//CLNTa_log(1,0,"clnt_decide_library_response] DirectResponse");

		*pSeparateResponse = FALSE;
	
		clnt_convert_string_response(pResultString, pResult);

		return pResult;
	}
}

#ifdef BLOCK
WMLSvar*  clnt_decide_library_response_no_convert(BOOL isSeparateResponse, BOOL *pSeparateResponse, WMLSvar *pResult, char *pResultString)
{
	
	if (isSeparateResponse)
	{
		//CLNTa_log(1,0,"clnt_decide_library_response] SeparateResponse");

		*pSeparateResponse = TRUE;

		return NULL;
	} 
	else 
	{
		//CLNTa_log(1,0,"clnt_decide_library_response] DirectResponse");

		*pSeparateResponse = FALSE;
	
		clnt_convert_string_response_no_convert(pResultString, pResult);

		return pResult;
	}
}
#endif //#ifdef BLOCK

BOOL clnt_check_lib_range(UINT16 libNbr, UINT8 funcNbr)
{
	if((EXTRA_LIB_START < libNbr ) && (libNbr < EXTRA_LIB_END))
	{
		return TRUE;
	}
	return FALSE;
}

void clnt_adjust_index(UINT16* libNbr, UINT8* funcNbr)
{
	//Subtract MUSICBELL_FUNC_OFFSET(100) function index for musicbell functions
	if ((*libNbr == EXTRA_LIB_MULTIMEDIA)&&(*funcNbr >= MUSICBELL_FUNC_MIN)&&(*funcNbr <= MUSICBELL_FUNC_MAX))
	{
		*funcNbr -= MUSICBELL_FUNC_OFFSET; //0 ~ 6
	}
#ifdef BLOCK
    else if((*libNbr == EXTRA_LIB_MULTIMEDIA)&&(*funcNbr == FUNC_NUM_GVM_GAMERUM))
    {
        *funcNbr = FUNC_NUM_GVM_INDEX; //8
    }
#endif //BLOCK
    else if((*libNbr == EXTRA_LIB_MULTIMEDIA)&&(*funcNbr == FUNC_NUM_SKVM_SETURL))
    {
        *funcNbr = FUNC_NUM_SKVM_SETURL_INDEX; //10
    }
    else if((*libNbr == EXTRA_LIB_MULTIMEDIA)&&(*funcNbr == FUNC_NUM_WAVELET))
    {
        *funcNbr = FUNC_NUM_WAVELET_INDEX; //12
    }

	//Subtract EXTRA_LIB_MULTIMEDIA(32000) from original library number
	*libNbr -= EXTRA_LIB_MULTIMEDIA;
#ifdef FEATURE_PIM_INVALIDERROR
    if(*libNbr == 2) //this is PIM Library
    {
        g_PIM = TRUE;
    }
#endif //FEATURE_PIM_INVALIDERROR
}




///////////////////////////////////////////////////////////////////////////// 
// FUNCTION : wap_handle_error
// DESCRIPTION : send error message to ui task 
/////////////////////////////////////////////////////////////////////////////

void wap_handle_error(int errorNumber, int errorType)
{
#ifdef FEATURE_WAP    
    EQS_SEND_EVENT(SE_WAP_REPORTERROR, EQS_NA, EQS_NA);
#else
    ui_cmd_type  *error_cmd_ptr;                       
	
	if(( error_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
	{
		//ERR_FATAL( "No UI cmd buffers",0,0,0);
		CLNTa_log(1,0,"No UI cmd buffers");
	} 
	else	
	{
		error_cmd_ptr->hdr.cmd			= UI_WAP_REPORT_ERROR_F;
		error_cmd_ptr->hdr.task_ptr     = NULL;
		error_cmd_ptr->hdr.done_q_ptr   = &ui_cmd_free_q;
		error_cmd_ptr->wap_error.number = errorNumber;
		error_cmd_ptr->wap_error.type   = errorType;
		ui_cmd( error_cmd_ptr);
	}
#endif //#ifndef FEATURE_WAP
}

/////////////////////////////////////////////////////////////////////////////
// Block showcard function in case of error, by doing this,
// LCD prevent from flip-floping between error message and normal message
/////////////////////////////////////////////////////////////////////////////

void wap_set_error(boolean isError)
{
	if(isError) {
		isErrorState = TRUE;
	} else {
		isErrorState = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Classify error to display on LCD or not 
/////////////////////////////////////////////////////////////////////////////

int wap_check_error_range(INT16 errorNo)
{
	if((errorNo == ERR_WAE_UA_VIEWID_INVALID ) 
		|| (errorNo == ERR_WAE_WML_UNKNOWN_TOKEN ) 
		|| (errorNo == ERR_WAE_WMLS_NONE ) 
		|| (errorNo == ERR_WAE_UA_MAX_EXCEEDED )
        || (errorNo == ERR_WAE_UA_WMLDECK_ACCESS_DENIED ) )	   
	{
		CLNTa_log(1, 0, "----[Ignore this][%s]----\n",cszErrorCode);
		return ERR_RESULT_IS_IGNORE;
	}
	else 
	{
		CLNTa_log(1, 0, "----[Handle this][errno:%d][%s]----\n",errorNo,cszErrorCode);
		return ERR_RESULT_IS_REPAINT_OR_TERMINATE;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//  CLNTa_getFile()
///////////////////////////////////////////////////////////////////////////////////////////////////

int wap_compare_name(const char *pSource, const char *pTarget)
{
	return	strncmp(pSource, pTarget, strlen(pTarget));
}

int wap_check_library(const char *pSource, const char *pTarget)
{
	return	strcmp(pSource, pTarget);
}

void wap_get_item(char *pSource, char *pTarget)
{
	while (((*pSource != SPACE) && 
		   (*pSource != NEW_LINE) &&
		   (*pSource != SEMI_COLON) && 
		   (*pSource != HORIZONTAL_TAB) &&
		   (*pSource != '\0') &&
		   (*pSource != '.') &&
		   (*pSource != CARRIAGE_RETURN)) && maxCount < maxLength )
	{
		*pTarget = *pSource;
		pTarget++; 
		pSource++;
		maxCount++;
	}
	
	if (*pSource == PERIOD)
	{
		pSource++;	
		maxCount++;
	}	
}


void wap_make_point(char *pSource)
{
	char	xString[4] = {0};
	char	yString[4] = {0};
		
	wap_get_item(pSource,xString); pSource += strlen(xString)+1; 
	wap_get_item(pSource,yString); pSource += strlen(yString)+1;

	SKY_graphicsPoint(atoi(xString), atoi(yString));
}

void wap_make_line(char *pSource)
{
	char	xOneString[4]  = {0};
	char	xTwoString[4]  = {0};
	char	yOneString[4]  = {0};
	char	yTwoString[4]  = {0};
	
	wap_get_item(pSource,xOneString); pSource += strlen(xOneString)+1; 
	wap_get_item(pSource,yOneString); pSource += strlen(yOneString)+1; 
	wap_get_item(pSource,xTwoString); pSource += strlen(xTwoString)+1;
	wap_get_item(pSource,yTwoString); pSource += strlen(yTwoString)+1; 
	
	SKY_graphicsLine(atoi(xOneString), atoi(yOneString),
					 atoi(xTwoString), atoi(yTwoString));
}

void wap_make_rectangle(char *pSource)
{
	char	xOneString[4]  = {0};
	char	xTwoString[4]  = {0};
	char	yOneString[4]  = {0};
	char	yTwoString[4]  = {0};
	char	isFill[4]	   = {0};

	wap_get_item(pSource,xOneString); pSource += strlen(xOneString)+1; 
	wap_get_item(pSource,yOneString); pSource += strlen(yOneString)+1;
	wap_get_item(pSource,xTwoString); pSource += strlen(xTwoString)+1; 
	wap_get_item(pSource,yTwoString); pSource += strlen(yTwoString)+1; 
	wap_get_item(pSource,isFill);	  pSource += strlen(isFill)+1;
	
	SKY_graphicsRectangle(atoi(xOneString), atoi(yOneString),
					      atoi(xTwoString), atoi(yTwoString), (boolean)atoi(isFill));
}

void wap_make_circle(char *pSource)
{
	char	xOneString[4] = {0};
	char	yOneString[4] = {0};
	char	radius[4] = {0};
	char	isFill[4] = {0};
	
	wap_get_item(pSource,xOneString); pSource += strlen(xOneString)+1; 
	wap_get_item(pSource,yOneString); pSource += strlen(yOneString)+1;
	wap_get_item(pSource,radius);     pSource += strlen(radius)+1; 
	wap_get_item(pSource,isFill);     pSource += strlen(isFill)+1;
	
	SKY_graphicsCircle(atoi(xOneString), atoi(yOneString), 
		               atoi(radius), (boolean)atoi(isFill));

}





///////////////////////////////////////////////////////////////////////////////////////////////////
// SKT Extra WMLScript library functions
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef USE_PROPRIETARY_WMLS_LIBS
WMLSvar* Call_Multimedia_setImage(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8 nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	int     ret = -1;

	//jjkyg add..
	INT32 argInt[2] ={0,}; 

	clnt_init_return_value(pResult);
	ret = clnt_alloc_string4(params, argString, argInt,nbrOfArgs);

	CLNTa_log(1,0,"NUMBER of arq  %d",nbrOfArgs);

	pResult->type = TYPE_INTEGER;

#ifdef BLOCK
	setImage(invokeId, argString[0], argString[1], atoi(argString[2]), params[3]->intVal); 
	clnt_free_string(argString, nbrOfArgs);

#endif //#ifdef BLOCK
	if(ret == 1)
	{
		setImage(invokeId, argString[0], argString[1], argInt[0],argInt[1]); 
		CLNTa_log(1,0,"Setimage int arg1 = %d,arg2=%d",argInt[0],argInt[1]);
		clnt_free_string(argString, nbrOfArgs - 2);
	}
	else if(ret == 2)
	{
		setImage(invokeId, argString[0], argString[1], atoi(argString[2]),argInt[0]); 
		CLNTa_log(1,0,"Setimage int arg1 = %d",argInt[0]);
		clnt_free_string(argString, nbrOfArgs - 1);
	}
	else
	{
		setImage(invokeId, argString[0], argString[1], atoi(argString[2]),atoi(argString[3])); 		
		clnt_free_string(argString, nbrOfArgs);
	}


	DEALLOC( &argString );

	return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
}

#ifdef FEATURE_MUSIC_BELL//=====================================================================================
#ifdef DIRECT_RESPONSE
WMLSvar* Call_Musicbell_Play(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	
	CLNTa_log(1,0,"Direct Call_Musicbell_Play is called");

	clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;
	
	Musicbell_play_tmp(invokeId, argString[0], argString[1], argString[2], argString[3]); 

	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#else //#ifdef DIRECT_RESPONSE

WMLSvar* Call_Musicbell_Play(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	
	CLNTa_log(1,0,"Separate Call_Musicbell_Play is called");

	clnt_alloc_string(params, argString, nbrOfArgs);

	//pResultString = (char*)WIPAlloc(2048+1);
	//pResult->type = TYPE_STRING;
	
	Musicbell_play_tmp(invokeId, argString[0], argString[1], argString[2], argString[3]); 

	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
}
#endif //#ifdef DIRECT_RESPONSE

WMLSvar* Call_Musicbell_Stop(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	
	CLNTa_log(1,0,"Call_Musicbell_Stop] is called");

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(MAX_SIZE_MUSIC_RESULT+2);
	pResult->type = TYPE_STRING;

    Musicbell_stop(pResultString);
	
	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

#ifdef DIRECT_RESPONSE
extern boolean attributeInvalid;

WMLSvar* Call_Musicbell_DownLoad(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString;
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	/*====================================================================
		Music bell download시 저장이 Clnta_content가 불리지 않을 경우에만 
		사용
	 ====================================================================*/
	isStringResponse = TRUE;
	if (attributeInvalid)
	{
		argString = NEWARRAY( char*, nbrOfArgs );

		clnt_alloc_string(params, argString, nbrOfArgs-1);

		pResultString = (char*)WIPAlloc(100+2);

		pResult->type = TYPE_STRING;

		Musicbell_download_tmp(invokeId, argString[0], argString[1], argString[2], "1", pResultString);
		
		clnt_free_string(argString, nbrOfArgs-1);
		attributeInvalid = FALSE;
	}
	else
	{
		argString = NEWARRAY( char*, nbrOfArgs + 1 );

		clnt_alloc_string(params, argString, nbrOfArgs);

		pResultString = (char*)WIPAlloc(100+2);

		pResult->type = TYPE_STRING;

		Musicbell_download_tmp(invokeId, argString[0], argString[1], argString[2], argString[3], pResultString);

		clnt_free_string(argString, nbrOfArgs);
	}
	DEALLOC( &argString );
	
	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}


#else //#ifdef DIRECT_RESPONSE
WMLSvar* Call_Musicbell_DownLoad(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);


	clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(100+2);

	pResult->type = TYPE_STRING;
	
	Musicbell_download_tmp(invokeId, argString[0], argString[1], argString[2], argString[3], pResultString);
	
	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );
	
	if(strcmp(pResultString, BUFFER_FULL)==0)
	{
		CLNTa_log(1,0,"Bufferfull-----------------------------");
		return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
	}
	else
	{
		WIPFree((char*)pResultString);
		return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
	}
}

#endif //#ifdef DIRECT_RESPONSE


WMLSvar* Call_Musicbell_GetListTitle(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	CLNTa_log(1,0,"Call_Musicbell_GetListTitle is called");

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;

    Musicbell_getlistTitle(pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Musicbell_MemPlay(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	CLNTa_log(1,0,"Call_Musicbell_MemPlay is called");

	clnt_alloc_string(params, argString, 1);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;

    Musicbell_memplay(argString[0], pResultString); 

	clnt_free_string(argString, 1);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}


extern void Musicbell_getTitle(char *Result, int fileIndex);


#ifdef BLOCK
WMLSvar* Call_Musicbell_DeleteList(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	CLNTa_log(1,0,"Call_Musicbell_DeleteList is called");

	clnt_alloc_string(params, argString, 1);

	pResultString = (char*)WIPAlloc(200+2);
	pResult->type = TYPE_STRING;

	Musicbell_deletelist(argString[0], pResultString); 

	clnt_free_string(argString, 1);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#endif //#ifdef BLOCK

extern char result_buffer[1500];

WMLSvar* Call_Musicbell_DeleteList(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	CLNTa_log(1,0,"Call_Musicbell_DeleteList is called");

	if(params[0]->type == typeString)
	{
		Uni2KSCString((WCHAR*)params[0]->stringVal, (unsigned char *)result_buffer);

		CLNTa_log(1,0,"DeleteList] string[%s] \n", result_buffer);
	}
	//clnt_alloc_string(params, argString, 1);

	pResultString = (char*)WIPAlloc(200+2);
	pResult->type = TYPE_STRING;

    Musicbell_deletelist(result_buffer, pResultString); 

	//clnt_free_string(argString, 1);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#endif //#ifdef FEATURE_MUSIC_BELL=====================================================================================


#ifdef WAP_SETURL
WMLSvar* Call_Terminal_setURL(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);


	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, 1);

	pResult->type = TYPE_INTEGER;

	CLNTa_log(1,0,"Call_Terminal_setURL is called [%s][%d]",argString[0], params[1]->intVal);
	
	//pResult->intVal = setURL(argString[0], atoi(argString[1])); 
	pResult->intVal = setURL(argString[0], params[1]->intVal); 

	clnt_free_string(argString, 1);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#else //#ifdef WAP_SETURL
//To be implemented !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

WMLSvar* Call_Terminal_getNID(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;
	

	clnt_free_string(argString, nbrOfArgs);

	return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getZone(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;
	

	clnt_free_string(argString, nbrOfArgs);

	return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
}


WMLSvar* Call_Terminal_getZone(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;
	

	clnt_free_string(argString, nbrOfArgs);

	return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
}


WMLSvar* Call_Terminal_getPilot(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;
	

	clnt_free_string(argString, nbrOfArgs);

	return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
}
#endif //#ifdef WAP_SETURL

//To be implemented !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
WMLSvar* Call_Terminal_isModifyAll(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;
	
	CLNTa_log(1,0,"Call_Terminal_isModifyAll is called [%d][%d]", params[0]->intVal, params[1]->intVal);
	isModifyAll(params[0]->intVal, params[1]->intVal, pResultString); 

	CLNTa_log(1,0,"Call_Terminal_isModifyAll] ResultString [%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_isExist(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
#ifdef BLOCK
	char	*argString;
	char	*pResultString = NULL;
	unsigned short sizeOfString;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	sizeOfString = KSCStrLenOfUni((WCHAR *)params[0]->stringVal);

	argString = (char *)WIPAlloc(sizeOfString+2);

	Uni2KSCString((WCHAR*)params[0]->stringVal, (unsigned char *)argString);

	CLNTa_log(1,0,"string[%s],size[%d]", argString, sizeOfString);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;

	isExist(argString, pResultString); 

	WIPFree((char*) argString);

	CLNTa_log(1,0,"Call_Terminal_isExist] pResultString[%s]",pResultString);

	pResult->stringVal = pResultString;
	
	pResult->stringLengthInBytes = strlen(pResultString);
	
	pResult->stringIANAcharset = KS_C_5601_1987;
	
	return pResult;

#endif //#ifdef BLOCK

	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_alloc_string(params, argString, 1);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;

	CLNTa_log(1,0,"Call_Terminal_isExist is called[%s]", argString[0]);
	isExist(argString[0], pResultString); 

	clnt_free_string(argString, 1);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_isGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_alloc_string(params, argString, 1);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;
	isGroup(argString[0], pResultString); 

	CLNTa_log(1,0,"Call_Terminal_isGroup is called[%s] [%s]", argString[0], pResultString);

	clnt_free_string(argString, 1);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getMaxEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;

	getMaxEntry(pResultString); 

	CLNTa_log(1,0,"Call_Terminal_getMaxEntry is called [%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getMaxGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;

	getMaxGroup(pResultString); 

	CLNTa_log(1,0,"Call_Terminal_getMaxGroup is called][%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getMaxShortKey(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;
	
	getMaxShortKey(pResultString); 

	CLNTa_log(1,0,"Call_Terminal_getMaxShortKey is called][%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getPhonebook(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;
	getPhonebook(pResultString); 

	CLNTa_log(1,0,"Call_Terminal_getPhonebook is called][%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getDeletedEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//CLNTa_log(1,0,"Call_Terminal_getDeletedEntry is called");

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;
	
	//CLNTa_log(1,0,"Call_Terminal_getDeletedEntry is called][%d][%d]", params[0]->intVal, params[1]->intVal);
	getDeletedEntry(params[0]->intVal, params[1]->intVal, pResultString); 

	CLNTa_log(1,0,"Call_Terminal_getDeletedEntry is called][%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(2048+2);
	pResult->type = TYPE_STRING;
	getGroup(pResultString); 

	CLNTa_log(1,0,"Call_Terminal_getGroup is called][%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

//Type check OK
WMLSvar* Call_Terminal_setGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, 2);

	pResult->type = TYPE_INTEGER;


	CLNTa_log(1,0,"Call_Terminal_setGroup is called][%d][%s]", params[0]->intVal, argString[0]);
	//CLNTa_log(1,0,"Call_Terminal_setGroup is called][%s][%s]", argString[0], argString[1]);
	pResult->intVal = setGroup(params[0]->intVal,  argString[0]); 
	//pResult->intVal = setGroup(atoi(argString[0]),  argString[2]); 

	CLNTa_log(1,0,"Call_Terminal_setGroup is called][%d]", pResult->intVal);

	clnt_free_string(argString, 1);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

//Type check is OK
WMLSvar* Call_Terminal_getEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	boolean	isArgString = FALSE;

	isArgString = clnt_alloc_string2(params, argString);
	
	pResultString = (char*)WIPAlloc(300+2);
	pResult->type = TYPE_STRING;
	
	if (isArgString)
	{
		CLNTa_log(1,0,"Call_Terminal_getEntry is called][%s]", argString[0]);
		getEntry(atoi(argString[0]),  pResultString); 
		
		clnt_free_string(argString, 1);
	}
	else
	{
		CLNTa_log(1,0,"Call_Terminal_getEntry is called][%d]", params[0]->intVal);
		getEntry(params[0]->intVal,  pResultString); 
	}
	
	CLNTa_log(1,0,"Call_Terminal_getEntry is called][%s]", pResultString);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_deleteEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResult->type = TYPE_INTEGER;
	
	CLNTa_log(1,0,"Call_Terminal_deleteEntry is called][%d]", params[0]->intVal);
	pResult->intVal = deleteEntry(params[0]->intVal); 

	CLNTa_log(1,0,"Call_Terminal_deleteEntry is called][%d]", pResult->intVal);
	
	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_setEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, 2);

	pResult->type = TYPE_INTEGER;

	CLNTa_log(1,0,"Call_Terminal_setEntry is called][%d][%s]", params[0]->intVal, argString[0]);
	//CLNTa_log(1,0,"Call_Terminal_setEntry is called][%s][%s]", argString[0], argString[1]);
	pResult->intVal = setEntry(params[0]->intVal,  argString[0]); 
	//pResult->intVal = setEntry(atoi(argString[0]),  argString[1]); 

	CLNTa_log(1,0,"Call_Terminal_setEntry is called][%d]", pResult->intVal);
	
	clnt_free_string(argString, 1);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_getVerID(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	//char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	//clnt_alloc_string(params, argString, nbrOfArgs);

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;
	
	CLNTa_log(1,0,"Call_Terminal_getVerID is called]");
	getVerID(pResultString); 

	CLNTa_log(1,0,"Call_Terminal_getVerID is called][%s]", pResultString);

	//clnt_free_string(argString, nbrOfArgs);

	//DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

#ifdef FEATURE_PHONEBOOK_VER12
#define MAX_GET_ENTRY_LIST 30

WMLSvar* Call_Terminal_getEntryList(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	//INT32	argInt[4] ={0,0,0,0}; 
	unsigned int nbrOfItems=0;
	unsigned int sizeOfString=0;
	
	CLNTa_log(1,0,"getEntryList]NUMBER of arg  %d",nbrOfArgs);

	if(!params[2]->intVal)
		nbrOfItems = MAX_GET_ENTRY_LIST;
	else
		nbrOfItems = params[2]->intVal;

	sizeOfString = 300*nbrOfItems+2;

	pResult->type = TYPE_STRING;

	pResultString = (char*)WIPAlloc(sizeOfString);

	getEntryList(params[0]->intVal, params[1]->intVal, params[2]->intVal, params[3]->intVal, pResultString, sizeOfString);

	CLNTa_log(1,0,"Call_Terminal_getEntryList is called][%s]", pResultString);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#endif //#ifdef FEATURE_PHONEBOOK_VER12

WMLSvar* Call_Terminal_sendMail(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8 nbrOfArgs)
{
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	
	CLNTa_log(1,0,"Call_Terminal_sendMail] is called");

	pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;
	
	sendMail(pResultString);
	
	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}



WMLSvar* Call_Terminal_setDormantTime(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8 nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	int     ret = -1;

	INT32	argInt[2] ={0,}; 

	clnt_init_return_value(pResult);

	ret = clnt_alloc_string3(params, argString, argInt, nbrOfArgs);

	CLNTa_log(1,0,"setDormantTime] NUMBER of arg  %d",nbrOfArgs);

	pResult->type = TYPE_INTEGER;

	if(ret == 1)
	{
		pResult->intVal = setDormantTime(invokeId, params[0]->intVal); 

		CLNTa_log(1,0,"setDormantTime int arg1 = %d",argInt[0]);

		clnt_free_string(argString, nbrOfArgs - 1);
	}
	else
	{
		pResult->intVal = setDormantTime(invokeId, atoi(argString[0])); 		

		CLNTa_log(1,0,"setDormantTime string arg1 = %s",argString[0]);

		clnt_free_string(argString, nbrOfArgs);
	}

	DEALLOC( &argString );

	//return clnt_decide_library_response(TRUE, pSeparateResponse, pResult, pResultString);
	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Terminal_muxOption(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, nbrOfArgs + 1 );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);
	int     ret = -1;

	INT32	argInt[2] ={0,}; 

	clnt_init_return_value(pResult);

	ret = clnt_alloc_string3(params, argString, argInt, nbrOfArgs);

	CLNTa_log(1,0,"muxOption] NUMBER of arg  %d",nbrOfArgs);

	pResult->type = TYPE_INTEGER;

	if(ret == 1)
	{
#ifdef WAP95C_TEST
		pResult->intVal = muxOption(invokeId, params[0]->intVal); 
#endif //WAP95C_TEST

		CLNTa_log(1,0,"muxOption int arg1 = %d",argInt[0]);

		clnt_free_string(argString, nbrOfArgs - 1);
	}
	else
	{
#ifdef WAP95C_TEST
		pResult->intVal = muxOption(invokeId, atoi(argString[0])); 		
#endif //#ifdef WAP95C_TEST

		CLNTa_log(1,0,"muxOption string arg1 = %s",argString[0]);

		clnt_free_string(argString, nbrOfArgs);
	}

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

#ifdef FEATURE_MCOMMERCE
#ifdef BLOCK
WMLSvar* Call_Mcommerce_testScriptA(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + nbrOfArgs );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, nbrOfArgs);
	
	//CLNTa_log(1,0,"Call_Mcommerce_testScriptA is called [%s]",argString[0]);

	pResult->type = TYPE_STRING;

	//pResultString = (char*)WIPAlloc(1000+1);
	
	testScriptA(argString[0], &pResultString);
	
	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#endif //#ifdef BLOCK

WMLSvar* Call_Mcommerce_testScriptA(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + nbrOfArgs );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, nbrOfArgs);
	
	//CLNTa_log(1,0,"Call_Mcommerce_testScriptA is called [%s]",argString[0]);

	pResult->type = TYPE_STRING;

	//pResultString = (char*)WIPAlloc(1000+1);
	
	testScriptA();
	
	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}


WMLSvar* Call_Mcommerce_testScriptB(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + nbrOfArgs );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, nbrOfArgs);

	//CLNTa_log(1,0,"Call_Mcommerce_testScriptB is called [%s][%s]",argString[0], argString[1]);

	pResult->type = TYPE_STRING;

	//pResultString = (char*)WIPAlloc(1000+1);

	testScriptB(argString[0], argString[1], &pResultString); 

	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}

WMLSvar* Call_Mcommerce_testScriptC(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + nbrOfArgs );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, nbrOfArgs);

	CLNTa_log(1,0,"Call_Mcommerce_testScriptC is called [%s][%d]",argString[0], params[1]->intVal);

	pResult->type = TYPE_STRING;

	//pResultString = (char*)WIPAlloc(1000+1);

	//strcpy(pResultString, "hello test3");
	
	testScriptC(argString[0], params[1]->intVal, &pResultString); 

	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#endif //#ifdef FEATURE_MCOMMERCE



#ifdef FEATURE_GVM
#ifdef BLOCK
WMLSvar* Call_Netgame_Gamerun(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + nbrOfArgs );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, nbrOfArgs);

    pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;

    CLNTa_log(1,0,"++Netgame_Gamerun is called++\n");

	GoToGVM(argString[0],params[1]->intVal, params[2]->intVal,
		                 argString[1], params[4]->intVal, 
						 argString[2], params[6]->intVal,
						 argString[3],
						 pResultString); 

	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#endif //BLOCK
#endif //netgame

#ifdef FEATURE_SK_VM

extern int GoToSKVM( char* msdURL, char* retURL, int scvOption);
WMLSvar* Call_SKVM_SetURL(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
    char	**argString = NEWARRAY( char*, 1 + nbrOfArgs );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);
	clnt_alloc_string(params, argString, nbrOfArgs);
    pResult->type = TYPE_INTEGER;

	pResult->intVal = GoToSKVM(argString[0],argString[1], params[2]->intVal);	
        
	CLNTa_log(1,0,"++SK_VM is complete++\n");

	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);
}
#endif //FEATURE_SK_VM

#ifdef FEATURE_WAP_WAVELET
extern void GoToWAVELET( int cpid, int ctsid, char* sndIP, int sndPort, 
                         int MO, int SO, int svcType, int skinType, char* cntName,
                         int cntProtect, int cntExpDate, char* retURL, char* Result );

WMLSvar* Call_Wavelet(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs)
{
	char	**argString = NEWARRAY( char*, 1 + nbrOfArgs );
	char	*pResultString = NULL;
	WMLSvar *pResult = NEWARRAY(WMLSvar,1);

	clnt_init_return_value(pResult);

	clnt_alloc_string(params, argString, nbrOfArgs);

    pResultString = (char*)WIPAlloc(100+2);
	pResult->type = TYPE_STRING;   

	GoToWAVELET(params[0]->intVal,params[1]->intVal,
                                  argString[0], params[3]->intVal,
                                  params[4]->intVal,params[5]->intVal,
                                  params[6]->intVal,params[7]->intVal,
                                  argString[1],
                                  params[10]->intVal,params[11]->intVal,
                                  argString[2],
						          pResultString); 

    CLNTa_log(1,0,"++Watelet is called++\n");

	clnt_free_string(argString, nbrOfArgs);

	DEALLOC( &argString );

	return clnt_decide_library_response(FALSE, pSeparateResponse, pResult, pResultString);		   
}
#endif//FEATURE_WAP_WAVELET


#endif	//#ifdef USE_PROPRIETARY_WMLS_LIBS