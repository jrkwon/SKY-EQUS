#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

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

//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
//#include "uintxt.h"

//////////////////////////////////////////////////

//#define WAP_ERROR_TEST
//------------------------------------------------------------------
#ifdef WAP_ERROR_TEST
	#define MAX_WAP_MENU_ITEMS				12
#else //#ifdef WAP_ERROR_TEST
	#define MAX_WAP_MENU_ITEMS				5
#endif //#ifdef WAP_ERROR_TEST

//PORTING_B0_WAP
//#define MAX_WAP_LINES					3
#define MAX_WAP_LINES					6//MAX_WAP_MENU_ITEMS

//------------------------------------------------------------------

int nCurStartMenu;
int nCurFocusMenu;

//------------------------------------------------------------------
extern void WAP_Display_Menu(void);
extern void WAP_DisplaySoftkey_Menu(void);
extern void WAP_DisplayCML_Menu(void);
extern void WAP_Draw_Menu_Item(int nIndex);
void WAP_MainMenu_Add_Bookmark(void);

#ifdef WAP_USE_DORM
	extern storeWML normal_use;
#endif//#ifdef WAP_USE_DORM

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
	sprintf(temp, "      %s%s%s%s      ",SF_NTOP1_4,SF_NTOP2_4,SF_NTOP3_4,SF_NTOP4_4);	// n.TOP menu tite
#if !((defined WAP_B1) || (defined WAP_B2))	//yjahn96 01.03.19
	WAP_Display_TextOut(0, 0, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else	//#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut_Title(0, 0, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif	//#if !((defined WAP_B1) || (defined WAP_B2))

	//------------------------------------------------------------------
	//각 메뉴를 그려준다
//	for(i=0; i<MAX_WAP_LINES; i++)
	for(i=0; i<MAX_WAP_MENU_ITEMS; i++)
    {
		WAP_Draw_Menu_Item(i);
    }

	//------------------------------------------------------------------
	//소프트 키를 그려준다
#if (defined WAP_B1) || (defined WAP_B2)
	WAP_DisplayCML_Menu();
#else   //#if (defined WAP_B1) || (defined WAP_B2)
    WAP_DisplaySoftkey_Menu();
#endif  //#if (defined WAP_B1) || (defined WAP_B2)
	
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
		case 1: sprintf(temp, "%s즐겨찾기          ",SF_BLACK_NUM1);break;
		case 2: sprintf(temp, "%s즐겨찾기추가      ",SF_BLACK_NUM2);break;
		case 3: sprintf(temp, "%s새로고치기        ",SF_BLACK_NUM3);break;
		case 4: sprintf(temp, "%s새URL열기         ",SF_BLACK_NUM4);break;
		case 5: sprintf(temp, "%sPhone기능         ",SF_BLACK_NUM5);break;
#ifdef WAP_HOMEPAGE
		case 6: sprintf(temp, "%sSK텔레텍가기      ",SF_BLACK_NUM6);break;
#endif //#ifdef WAP_HOMEPAGE
#ifdef WAP_ERROR_TEST
		case 7: sprintf(temp, "%sFileNotFound      ",SF_BLACK_NUM7);break;
		case 8: sprintf(temp, "%sForbidden         ",SF_BLACK_NUM8);break;
		case 9: sprintf(temp, "%sInternalSvrErr    ",SF_BLACK_NUM9);break;
		case 10: sprintf(temp, "%sURLTimeOut       ",SF_BLACK_NUM10);break;
		case 11: sprintf(temp, "%sUnknownContent   ",SF_BLACK_NUM11);break;
		case 12: sprintf(temp, "%sURLInvalid       ",SF_BLACK_NUM12);break;
//		case 13: strcpy(temp, "\204\315NoSvrResponse ");break;
//		case 14: strcpy(temp, "\204\316GeneralError");break;
#endif //#ifdef WAP_ERROR_TEST
	}
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(0,//4*2,
						WAP_HAN_FONT_HEIGHT + nIndex*WAP_HAN_FONT_HEIGHT,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut_Focus(0,//4,
						WAP_HAN_FONT_HEIGHT + nIndex*WAP_HAN_FONT_HEIGHT,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
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
	WAP_Display_Line(2, WAP_DOC_HEIGHT , WAP_LCD_WIDTH - 3, WAP_DOC_HEIGHT , BLACK);

    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);//clear
    WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWN,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//clear
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);//ok
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CSELECT , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//선택
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_CANCEL ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);//취소
}

void WAP_DisplaySoftkey_Menu(void)
{
	//ok선택 : {0x88bf, 0x84fe, 0x84ff}
	//A선택  : {0x8ac0, 0x84fe, 0x84ff}
	//취소)  : {0x86b6, 0x86b7, 6}
	char temp[20];

//#if MMI_TYPE == MMI_A10
    strcpy(temp, "\210\277\204\376\204\377\206\266\206\267\6");
//#else
    if(SKY_IS_PHONE_OPEN())//ui.earpieceup
		strcpy(temp, "\210\277\204\376\204\377\206\266\206\267\6");
	else//for A7M
		strcpy(temp, "\212\300\204\376\204\377\206\266\206\267\6");
//#endif

#if !((defined WAP_B1) || (defined WAP_B2))
    WAP_Display_TextOut(48*2, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
    WAP_Display_TextOut(48, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
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
		
	//!!!!......즐겨찾기에는 cache가 필요 없다....!!!!!
	SKY_loadURL(g_pCard->viewId, "http://wap.n-top.com/cgi/get_service.cgi?SERVICE_ID=100011710000007",TRUE);

	//RETRANSMISSION을 위한 조작이 들어가야 한다.
#ifdef WAP_USE_DORM
	normal_use.viewid  = g_pCard->viewId;
	normal_use.id      = 0;
	normal_use.wmlkind = 5;					
#endif//#ifdef WAP_USE_DORM
}

void WAP_MainMenu_Refresh(void)//새로고치기
{
#ifdef DEBUG_MMI
	CLNTa_log(1, 0, "새로고치기");
#endif
    MSG_HIGH("",0,0,0);
	SKY_reload(g_pCard->viewId);

#ifdef WAP_USE_DORM
	normal_use.viewid  = g_pCard->viewId;
	normal_use.id      = 0;
	normal_use.wmlkind = 8;					
#endif//#ifdef WAP_USE_DORM
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

#ifdef WAP_HOMEPAGE

//char sktt_homepage[100] = "http://www.skteletech.co.kr/skwap/Home/WML/home.wml";
static  char home_url[200] ;  
nv_item_type waphome_nvi; /* NV items */

void WAP_MainMenu_HomePage(void)//Homepage
{
	int hlength = 0;
//#ifdef DEBUG_MMI
	CLNTa_log(1, 0, "Homepage");
//#endif
	(void) ui_get_nv( NV_INITHOME_I,&waphome_nvi ); 
	hlength = get_length(waphome_nvi.inithome.inithome,NV_INITHOME);
	memset(home_url,'\x00',200);
	memcpy(home_url,waphome_nvi.inithome.inithome,hlength);
	
	//Temporary value 
	SKY_loadURL(g_pCard->viewId, home_url,TRUE);
	//RETRANSMISSION을 위한 조작이 들어가야 한다.

#ifdef WAP_USE_DORM
	normal_use.viewid  = g_pCard->viewId;
	normal_use.id      = 0;
	normal_use.wmlkind = 6;					
#endif//#ifdef WAP_USE_DORM
}
#endif //#ifdef WAP_HOMEPAGE

#ifdef WAP_ERROR_TEST
void WAP_MainMenu_Test(int input)
{
	switch (input)
	{
	case 7:  CLNTa_error(1,HTTPFileNotFound,ERRTYPE_INFORMATION); break;
	case 8:  CLNTa_error(1,HTTPForbidden,ERRTYPE_INFORMATION); break;
	case 9:  CLNTa_error(1,HTTPInternalServerError,ERRTYPE_INFORMATION); break;
	case 10: CLNTa_error(1,ERR_WAE_UA_URL_TIMEOUT,ERRTYPE_INFORMATION); break;
	case 11: CLNTa_error(1,ERR_MMI_UNKNOWN_CONTENT,ERRTYPE_SKTT); break;
	case 12: CLNTa_error(1,ERR_WAE_UA_URL_INVALID,ERRTYPE_INFORMATION); break;
#ifdef BLOCK
	case 13: CLNTa_error(1,ERR_PPP_SETUP_FAILURE,ERRTYPE_SKTT); break;
	case 14: CLNTa_error(1,ERR_WAE_UA_URL_NONSUPPORTED_SCHEME,ERRTYPE_INFORMATION); break;
#endif //BLOCK
	}

}
#endif //#ifdef WAP_ERROR_TEST

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
	WAP_Display_Menu();
}

int WAP_Handle_CenterKey_MainMenu(void)
{

    extern Portdetect;
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
            Portdetect = FALSE; 
			WAP_MainMenu_Refresh();
			break;
		case 4: //새 URL 열기 
			WAP_MainMenu_New_URL();
			break;
		case 5: //Phone기능   
			WAP_MainMenu_PhoneMode();
			break;
#ifdef WAP_HOMEPAGE
		case 6: //Homepage
			WAP_MainMenu_HomePage();
			break;
#endif //#ifdef WAP_HOMEPAGE
#ifdef WAP_ERROR_TEST
		case 7: 
		case 8: 
		case 9: 
		case 10:
		case 11:
		case 12:
//		case 13:
//		case 14:
			WAP_MainMenu_Test(nCurFocusMenu);
			break;
#endif //#ifdef WAP_ERROR_TEST

	}

	return(nCurFocusMenu);
}

void WAP_Handle_NumKey_MainMenu(int nKey)
{
	//---------------------------------------------------------
	// yjahn96 01.03.19
	// 'OK'키 선택시 StartMenu가 달라지는 현상 수정  
	nCurStartMenu = 1; nCurFocusMenu = nKey;	//nCurStartMenu = nCurFocusMenu = nKey;

#ifdef WAP_HOMEPAGE
	if((nCurFocusMenu < 4) || (nCurFocusMenu == 6)) 
#else //#ifdef WAP_HOMEPAGE
	if(nCurFocusMenu < 4)
#endif //#ifdef WAP_HOMEPAGE
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
#ifdef WAP_USE_LEAK
		ptemptitle = (char*)WIPAlloc(strlen(g_pCard->title)+1);
		if(!ptemptitle) return;
#else //#ifdef WAP_USE_LEAK
		ptemptitle = (char*)wip_malloc(strlen(g_pCard->title)+1);
#endif //#ifdef WAP_USE_LEAK
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
#ifdef WAP_USE_LEAK
		temptitle  = (char *)WIPAlloc(strlen(pSendTitle)+1);
		if(!temptitle) return;
else //#ifdef WAP_USE_LEAK
		temptitle  = (char *)wip_malloc(strlen(pSendTitle)+1);
#endif //#ifdef WAP_USE_LEAK
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
#ifdef WAP_USE_LEAK
		tempurl = (char*)WIPAlloc(strlen(g_pCard->URL)+1-8);
		if(!tempurl) return;
#else //#ifdef WAP_USE_LEAK
		tempurl = (char*)wip_malloc(strlen(g_pCard->URL)+1-8);
#endif //#ifdef WAP_USE_LEAK

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
#ifdef WAP_USE_LEAK
		tempurl = (char*)WIPAlloc(strlen(g_pCard->URL)+1-7);
		if(!tempurl) return;
#else //#ifdef WAP_USE_LEAK
		tempurl = (char*)wip_malloc(strlen(g_pCard->URL)+1-7);
#endif //#ifdef WAP_USE_LEAK

		memset(tempurl,0x00,strlen(g_pCard->URL)+1-7);
		memcpy(tempurl,g_pCard->URL+7,strlen(g_pCard->URL)-7);

		if(g_pCard->title!=NULL)
			url_length = fix_size + strlen(tempurl)+strlen(temptitle)+1;
		else
			url_length = fix_size + strlen(tempurl)+1;
	}

#ifdef WAP_USE_LEAK
	addurl = (char *)WIPAlloc(url_length*sizeof(CHAR));
	if(!addurl) return;
#else //#ifdef WAP_USE_LEAK
	addurl = (char *)wip_malloc(url_length*sizeof(CHAR));
#endif //#ifdef WAP_USE_LEAK

	memset(addurl,0x00,url_length);

	if(g_pCard->title==NULL)
		sprintf(addurl,"http://wap.n-top.com/cgi/get_service.cgi?SERVICE_ID=100011710000008&v_url=%s",tempurl);
	else
		sprintf(addurl,"http://wap.n-top.com/cgi/get_service.cgi?SERVICE_ID=100011710000008&v_url=%s&v_title=%s",tempurl,temptitle);

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

#ifdef WAP_USE_DORM
	//RETRANSMISSION을 위한 조작이 들어가야 한다.
	normal_use.viewid  = g_pCard->viewId;
	normal_use.id      = 0;
	normal_use.wmlkind = 4;				
#endif//#ifdef WAP_USE_DORM

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