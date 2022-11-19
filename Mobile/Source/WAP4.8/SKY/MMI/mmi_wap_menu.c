
#include <stdio.h>
#include <string.h>
#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"
#include "cmmnrsrc.h"

#if MMI_TYPE == SKY_UI  //for EQUS
#include "sky.h"
#endif

//General Display Library
#include "GenDsp.h"

//-----------------------------------------------------------------------------------
//
// MMI 의 Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "clnt.h"
#include "wapconnector.h"


//PORTING_B0_WAP
//#define MAX_WAP_LINES					3
#define MAX_WAP_MENU_ITEMS				5
#define MAX_WAP_LINES					7//MAX_WAP_MENU_ITEMS

//------------------------------------------------------------------
void WAP_MainMenu_Add_Bookmark(void);//즐겨찾기추가

int nCurStartMenu;
int nCurFocusMenu;

//------------------------------------------------------------------
void WAP_Build_Menu(void)
{
	//------------------------------------------------------------------
	//Init
	nCurStartMenu = 1;//init
	nCurFocusMenu = 1;//init

	//------------------------------------------------------------------
	//Display
	WAP_Display_Menu();
}

void WAP_Display_Menu(void)
{
	int i;
	char temp[21];

	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();
	memset(temp, NULL, 21);

	//-----------------------------------------------------------
	// 타이틀 쓰기
	strcpy(temp, "       NATE       ");//sprintf(temp, "      %s%s%s%s      ",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP menu tite
	WAP_Display_TextOut_Title(0, 0, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);

	//------------------------------------------------------------------
	//각 메뉴를 그려준다
	for(i=0; i<MAX_WAP_MENU_ITEMS; i++)//	for(i=0; i<MAX_WAP_LINES; i++)
    {
		WAP_Draw_Menu_Item(i);
    }

	//------------------------------------------------------------------
	//소프트 키를 그려준다
    WAP_Display_Rectangle(0, WAP_DOC_HEIGHT, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, TRUE, WHITE);//WAP_BASIC_BACK_COLOR);//clear
	WAP_DisplayCML_Menu();
	
	//------------------------------------------------------------------
	//handset task에 display를 요청한다
	WAP_Send_LCDBuffer_ToHS();
}

void WAP_Draw_Menu_Item(int nIndex)
{
	char temp[22];
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	
	//-----------------------------------------------------------
	int nCurMenu = nIndex + nCurStartMenu;

	if(nCurMenu > MAX_WAP_MENU_ITEMS)
		nCurMenu -= MAX_WAP_MENU_ITEMS;

	//-----------------------------------------------------------
	if(nCurMenu == nCurFocusMenu)
		nDrawMode = WAP_DRAW_REVERSE;//reserve

	//-----------------------------------------------------------
	switch(nCurMenu)
	{
		case 1: sprintf(temp, "%s즐겨찾기        ",SF_BLACK_NUM1);break;
		case 2: sprintf(temp, "%s즐겨찾기 추가   ",SF_BLACK_NUM2);break;
		case 3: sprintf(temp, "%s새로 고치기     ",SF_BLACK_NUM3);break;
		case 4: sprintf(temp, "%s새URL 열기      ",SF_BLACK_NUM4);break;
		case 5: sprintf(temp, "%sPhone기능       ",SF_BLACK_NUM5);break;
	}

    WAP_Display_TextOut_Focus(0,//4,
						WAP_HAN_FONT_HEIGHT + nIndex*WAP_HAN_FONT_HEIGHT, //title
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
}

//-----------------------------------------------------
// yjahn96 01.03.05
void WAP_DisplayCML_Menu(void)
{
	//ok선택 : {0x88bf, 0x84fe, 0x84ff}
	//A선택  : {0x8ac0, 0x84fe, 0x84ff}
	//취소)  : {0x86b6, 0x86b7, 6}

	//===========================================================================
	// 화면아래 라인
	WAP_Display_Line(1, WAP_DOC_HEIGHT+3 , WAP_LCD_WIDTH - 2, WAP_DOC_HEIGHT+3 , BLACK);

    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);//clear
    WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWN,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//clear
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);//ok
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CSELECT , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//선택
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_CANCEL ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);//취소
}

void WAP_Display_Menu_Partial(BOOL bDown)
{
    int x1, y1, x2, y2;
    int nFirstMenu, nSecondMenu;

    if(bDown)
    {
        if(nCurFocusMenu == 1) //5. -> 1.
        {
            nFirstMenu = MAX_WAP_MENU_ITEMS;
            nSecondMenu = nCurFocusMenu;
            
            //1.
            WAP_Draw_Menu_Item(nFirstMenu - 1);  //이 함수에 들어가는 index는 0부터 시작

            //update
            x1 = 0;
            y1 = (nFirstMenu*WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
            x2 = WAP_LCD_TOTAL_ADDRESS_X-1;
            y2 = (y1 + WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
		    //handset task에 display를 요청한다
		    SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x1, y1, x2, y2);//WAP_Send_LCDBuffer_ToHS();
            
            //2.
            WAP_Draw_Menu_Item(nSecondMenu - 1);

            //update
            x1 = 0;
            y1 = (nSecondMenu*WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
            x2 = WAP_LCD_TOTAL_ADDRESS_X-1;
            y2 = (y1 + WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
		    //handset task에 display를 요청한다
		    SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x1, y1, x2, y2);//WAP_Send_LCDBuffer_ToHS();



        }
        else
        {
            nFirstMenu = nCurFocusMenu -1;
            nSecondMenu = nCurFocusMenu;

            WAP_Draw_Menu_Item(nFirstMenu - 1);
            WAP_Draw_Menu_Item(nSecondMenu - 1);

            //update
            x1 = 0;
            y1 = (nFirstMenu*WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT; //title
            x2 = WAP_LCD_TOTAL_ADDRESS_X-1;
            y2 = (nSecondMenu*WAP_HAN_FONT_HEIGHT + WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
		    //handset task에 display를 요청한다
		    SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x1, y1, x2, y2);//WAP_Send_LCDBuffer_ToHS();

        }
    }
    else
    {
        if(nCurFocusMenu == MAX_WAP_MENU_ITEMS)//1. -> 5.
        {
            nFirstMenu = nCurFocusMenu;
            nSecondMenu = 1;        
        
            //1.
            WAP_Draw_Menu_Item(nFirstMenu - 1);
            
            //update
            x1 = 0;
            y1 = (nFirstMenu*WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
            x2 = WAP_LCD_TOTAL_ADDRESS_X-1;
            y2 = (y1 + WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
		    //handset task에 display를 요청한다
		    SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x1, y1, x2, y2);//WAP_Send_LCDBuffer_ToHS();

            //2.
            WAP_Draw_Menu_Item(nSecondMenu - 1);

            //update
            x1 = 0;
            y1 = (nSecondMenu*WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
            x2 = WAP_LCD_TOTAL_ADDRESS_X-1;
            y2 = (y1 + WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
		    //handset task에 display를 요청한다
		    SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x1, y1, x2, y2);//WAP_Send_LCDBuffer_ToHS();

        }
        else
        {
            nFirstMenu = nCurFocusMenu;
            nSecondMenu = nCurFocusMenu + 1;

            WAP_Draw_Menu_Item(nFirstMenu - 1);
            WAP_Draw_Menu_Item(nSecondMenu - 1);     
            
            //update
            x1 = 0;
            y1 = (nFirstMenu*WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT; //title
            x2 = WAP_LCD_TOTAL_ADDRESS_X-1;
            y2 = (nSecondMenu*WAP_HAN_FONT_HEIGHT + WAP_HAN_FONT_HEIGHT) + WAP_HEAD_HEIGHT;
		    //handset task에 display를 요청한다
		    SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x1, y1, x2, y2);//WAP_Send_LCDBuffer_ToHS();
        }
    }


}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
/*																						*/
/*								WAP Menu Action Handlers								*/
/*																						*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/

void WAP_MainMenu_Bookmark(void)//즐겨찾기
{
#ifdef DEBUG_MMI
	CLNTa_log(1, 0, "즐겨찾기");
#endif
	
    SKY_loadURL(g_pCard->viewId, "http://wap.nate.com/cgi/get_service.cgi?SERVICE_ID=100011710000007",TRUE);	
}

void WAP_MainMenu_Refresh(void)//새로고치기
{
#ifdef DEBUG_MMI
	CLNTa_log(1, 0, "새로고치기");
#endif
    MSG_HIGH("",0,0,0);
	SKY_reload(g_pCard->viewId);
}

void WAP_MainMenu_New_URL(void)//새 URL 열기 
{
#ifdef DEBUG_MMI
	CLNTa_log(1, 0, "새 URL 열기");
#endif
	//go [alpha-edit mode]
}

void WAP_MainMenu_PhoneMode(void)//Phone기능
{
#ifdef DEBUG_MMI
	CLNTa_log(1, 0, "Phone기능");
#endif
	//go [Phone mode]
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
/*																						*/
/*								Key Handlers											*/
/*																						*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
void WAP_Scrolling_Menu(BOOL bDown)
{
	//--------------------------------------------------------
	// Down
	//--------------------------------------------------------
	if(bDown)
	{
		if(nCurStartMenu + (MAX_WAP_LINES-1) == nCurFocusMenu)
//		|| nCurStartMenu + (MAX_WAP_LINES-1) == nCurFocusMenu + MAX_WAP_MENU_ITEMS )
            nCurStartMenu++;
		nCurFocusMenu++;
	}
	//--------------------------------------------------------
	// Up
	//--------------------------------------------------------
	else
	{
//		if(nCurStartMenu == nCurFocusMenu) 
//			nCurStartMenu--;
		nCurFocusMenu--;
	}

	//--------------------------------------------------------
	// 값 조정 for Rotate
	if(nCurStartMenu < 1)   nCurStartMenu = MAX_WAP_MENU_ITEMS;
	else if(nCurStartMenu > MAX_WAP_MENU_ITEMS) nCurStartMenu = 1;
	
	if(nCurFocusMenu < 1)   nCurFocusMenu = MAX_WAP_MENU_ITEMS;
	else if(nCurFocusMenu > MAX_WAP_MENU_ITEMS) nCurFocusMenu = 1;
	
	//--------------------------------------------------------
	// Display
	WAP_Display_Menu_Partial(bDown);//WAP_Display_Menu();
}

int WAP_Handle_CenterKey_MainMenu(void)
{
	switch(nCurFocusMenu)
	{
		case 1: //즐겨찾기    
			WAP_MainMenu_Bookmark();
			break;
		case 2: //즐겨찾기추가
			WAP_MainMenu_Add_Bookmark();
			break;
		case 3: //새로고치기
            //보안site 진입후 새로고치기시 이상현상 때문에 추가
            //ytchoi 01/05/22
			WAP_MainMenu_Refresh();
			break;
		case 4: //새 URL 열기 
			WAP_MainMenu_New_URL();
			break;
		case 5: //Phone기능   
			WAP_MainMenu_PhoneMode();
			break;
	}

	return(nCurFocusMenu);
}

void WAP_Handle_NumKey_MainMenu(int nKey)
{
	//---------------------------------------------------------
	// yjahn96 01.03.19
	// 'OK'키 선택시 StartMenu가 달라지는 현상 수정  
	nCurStartMenu = 1; nCurFocusMenu = nKey;	//nCurStartMenu = nCurFocusMenu = nKey;
	if(nCurFocusMenu < 4)
	{
		WAP_Display_Menu();//화면 Update
	} 

	WAP_Handle_CenterKey_MainMenu();
}


extern BYTE *b_EscapeURLString (const BYTE* pbString);
extern BYTE *b_EscapeString (const BYTE* pbString);
int   url_length;
char  urladd[500];
#define fix_size   83
char*  addurl;
char*  tempurl   = NULL;
char*  temptitle = NULL;
void WAP_MainMenu_Add_Bookmark(void)//즐겨찾기추가
{
	
	char* pSendurl   = NULL;
	char* pTempurl   = NULL;
	char* pSendTitle = NULL;
	char* ptemptitle = NULL;

	url_length =0;
			
	if(g_pCard->title!=NULL)
	{
		ptemptitle = (char*)wip_malloc(strlen(g_pCard->title)+1);
		memset(ptemptitle,0x00, strlen(g_pCard->title)+1 );
		memcpy(ptemptitle,g_pCard->title,strlen(g_pCard->title));

	  /*===============jjkyg 2000/03/25=========================
		if(
			((ptempclear = strstr(ptemptitle,"(")) !=NULL) ||
			((ptempclear = strstr(ptemptitle,"[")) !=NULL)
		  )
		{
			*ptempclear = 0x00;
		}
	  === =============jjkyg 2000/03/25==========================*/

		pSendTitle = (char *)b_EscapeString((const BYTE *)ptemptitle);
		//pSendTitle = b_EscapeTitleString((const BYTE*)ptemptitle);
		temptitle  = (char *)wip_malloc(strlen(pSendTitle)+1);
		strcpy(temptitle,pSendTitle);
	}

	if((pTempurl = strstr(g_pCard->URL,"#"))!=NULL)
		*pTempurl = 0x00;
	
	if(strstr(g_pCard->URL,"https")!=NULL)
	{
		if(g_pCard->title==NULL)
		{
#ifdef DEBUG_MMI
			CLNTa_log(0,0,"Title not exist");
#endif
		}
		tempurl = (char*)wip_malloc(strlen(g_pCard->URL)+1-8);
		memset(tempurl,0x00,strlen(g_pCard->URL)+1-8);
		memcpy(tempurl,g_pCard->URL+8,strlen(g_pCard->URL)-8);

		if(g_pCard->title!=NULL)
			url_length = fix_size + strlen(tempurl)+strlen(temptitle)+1;
		else
			url_length = fix_size + strlen(tempurl)+1;
	}
	else if(strstr(g_pCard->URL,"http")!=NULL)
	{
		if(g_pCard->title==NULL)
		{
#ifdef DEBUG_MMI
			CLNTa_log(0,0,"Title not exist");
#endif
		}
		tempurl = (char*)wip_malloc(strlen(g_pCard->URL)+1-7);
		memset(tempurl,0x00,strlen(g_pCard->URL)+1-7);
		memcpy(tempurl,g_pCard->URL+7,strlen(g_pCard->URL)-7);

		if(g_pCard->title!=NULL)
			url_length = fix_size + strlen(tempurl)+strlen(temptitle)+1;
		else
			url_length = fix_size + strlen(tempurl)+1;
	}

	addurl = (char *)wip_malloc(url_length*sizeof(CHAR));

	memset(addurl,0x00,url_length);

	if(g_pCard->title==NULL)
		//sprintf(addurl,"http://wap.n-top.com/cgi/get_service.cgi?SERVICE_ID=100011710000008&v_url=%s",tempurl);
        sprintf(addurl,"http://wap.nate.com/cgi/get_service.cgi?SERVICE_ID=100011710000008&v_url=%s",tempurl);
	else
		//sprintf(addurl,"http://wap.n-top.com/cgi/get_service.cgi?SERVICE_ID=100011710000008&v_url=%s&v_title=%s",tempurl,temptitle);
        sprintf(addurl,"http://wap.nate.com/cgi/get_service.cgi?SERVICE_ID=100011710000008&v_url=%s&v_title=%s",tempurl,temptitle);

#ifdef DEBUG_MMI
    if(temptitle!=NULL)
		CLNTa_log(0,0," title %s",temptitle);
	else
		CLNTa_log(0,0," temptitle NULL");

	CLNTa_log(0,0," Surl %s",tempurl);
#endif	

	memset(urladd,0x00,500);
	pSendurl = (char *)b_EscapeURLString((const BYTE *)addurl);
	strcpy(urladd,pSendurl);
    
	SKY_loadURL(g_pCard->viewId, (const CHAR*)urladd, TRUE);//WIPFREE되는지 알아볼것 

	if(g_pCard->title!=NULL)
	{
		wip_free(temptitle);
		wip_free(ptemptitle);
		DEALLOC(&pSendTitle);
	}
	wip_free(tempurl);
	wip_free(addurl);
	DEALLOC(&pSendurl);
	//for test addbookmark
}