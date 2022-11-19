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

#include "waptask.h"
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
#define WAP_ANI_TEXT_CONNECTING			1	//접속중
#define WAP_ANI_TEXT_RECEIVING			2	//수신중
#define WAP_ANI_TEXT_SECURITY			3	//보안중
#define WAP_ANI_TEXT_ADDBOOKMARK		4	//추가중
#define WAP_ANI_TEXT_RESETUP			5	//재설정
#define WAP_ANI_BAR_FROM_LEFT			FALSE
#define WAP_ANI_BAR_FROM_RIGHT			TRUE


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
#ifdef temp_forwapapp2
int g_reloaderrorTime;
UINT8 ui_errortype =0;
CHAR  temp_url[200];
#endif //temp_forwapapp2

boolean start_disp = FALSE;
UINT8 g_nAniBarCount = 0;


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
	sprintf(temp, "%s%s%s%s을",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP

	//====================================================================
	WAP_ClearScreen();
	WAP_SetTextAtLine(2, temp, WAP_TEXT_ALIGN_CENTER);
	WAP_SetTextAtLine(3, "종료합니다",WAP_TEXT_ALIGN_CENTER);
	WAP_DisplayMessage();
}

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
	
	if(start_disp)
	{
		MODE = WAP_ANI_TEXT_CONNECTING;//접속중
		WAP_DrawLoadingBar(MODE, WAP_ANI_BAR_FROM_LEFT);		
		return;
	}	
#ifdef REDIRECT//jjkyg 06/26
	else if(redirect.bCorrect) MODE = WAP_ANI_TEXT_RESETUP;
#endif//#ifdef REDIRECT
#ifdef WAP_USE_SECURE
	else if(Portdetect) MODE = WAP_ANI_TEXT_SECURITY;
#endif//#ifdef WAP_USE_SECURE
	else MODE = WAP_ANI_TEXT_RECEIVING;	

	WAP_DrawLoadingBar(MODE, WAP_ANI_BAR_FROM_RIGHT);		

}

void displayloading(int catagory)
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
}


void WAP_Display_LoadingBar(int start_x, int end_x)
{
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

	UINT8 nBarCount;
	//-------------------------------
	//skip drawing
	if(!g_bDrawBar) return;

	//-------------------------------
	//clear
    WAP_Display_Rectangle(0, WAP_SOFTKEY_Y_POS, 119, WAP_SOFTKEY_Y_POS+12, TRUE, WHITE);
	//-------------------------------
	//text
	g_nAniBarCount = (g_nAniBarCount + 1) % 17;
	nBarCount = g_nAniBarCount % 17;

	if(nBarCount < 13)//normal
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
		switch(nBarCount) //얇은것<-굵은것
		{
		case 16://반전
		case 15://반전
		case 14://반전
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
		}
	}
	else
	{
		switch(nBarCount) //굵은것->얇은것
		{
		case 16://반전
		case 15://반전
		case 14://반전
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
		}
	}

	//---------------------------------------------------
	// Loading 을 종료하기 위한 취소 키를 오른쪽에 넣어준다
	WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_CANCEL, WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);
}

void WAP_DrawLoadingBar(int nAniTextMode, BOOL bAniBarDirection)
{
//display at LCD
	WAP_DrawLoadingBarToBackBuff(nAniTextMode, bAniBarDirection);
	WAP_Send_LCDBuffer_ToHS();
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
	
	return length;
} 
#endif //FEATURE_WAP