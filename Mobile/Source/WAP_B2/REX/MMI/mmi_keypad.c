
#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
//#include "custtrex.h"
#include "customer.h"
#include <stdio.h>
//////////////////////////////////////////////////

#include <string.h>

#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"

#if MMI_TYPE == SKY_UI  //add_htg_00_xmas : To adapt for EQUS
#include "sky.h"
#else
#include "hs.h"
#include "uiuint.h"
#endif



//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
//#include "uinstate.h"
//#include "uinint.h"

//////////////////////////////////////////////////
#include "ds.h"
#include "psi.h"
#include "waptask.h"


//-----------------------------------------------------------------------------------
//
// MMI 의 Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
//#include ".\girin\Common\uiCommon.h"

//////////////////////////////////////////////////
#include "wapconnector.h"

//===============================================
// 각 화면 모드별 키 처리 함수
LOCAL int WAP_HandleKeys_Graphics(int nKey);
LOCAL int WAP_HandleKeys_Dialog(int nKey);
LOCAL int WAP_HandleKeys_Menu_Main(int nKey);
LOCAL int WAP_HandleKeys_WML(int nKey);
LOCAL int WAP_HandleKeys_Option(int nKey);
//===============================================

//++for WAP Main Menu
extern void WAP_Build_Menu(void);
//--for WAP Main Menu

void WAP_Repaint(void);

//jjkyg 10/27

#ifndef WAP_USE_SECURE
	BOOL wtls_session = FALSE;
	extern BOOL Wtlshello; 
#endif//#ifndef WAP_USE_SECURE

extern char  temp_url[200];

extern char *url_pophistory(void); // url_history
extern char from_goback; //url_history

extern BOOL entertain;

#ifdef FEATURE_MUSIC_BELL
boolean bReplayMusic = FALSE;
extern  boolean		g_fMusicbellOn;
extern void musicbell_check(void);
#endif //		#ifdef FEATURE_MUSIC_BELL

#if defined(WAP_USE_DORM) || defined(WAP_USE_SKT_NEW) 
#include "Gendsp.h"
	extern storeWML normal_use;
#endif//#ifdef WAP_USE_DORM

//------------------------------------------------------
// yjahn96 01.04.03 - for Animation timer
#ifdef SIS_1
boolean g_fFirstSISImageDraw = FALSE;
#endif  //#if SIS_1
extern boolean Check_Play_SIS_Animation(void);

extern void CancelAnimationTimer(void);
extern void RestartAnimationTimer(unsigned long ms);
////////////////////////////////////////////////////////////////////


//extern BOOL firstsite;
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*																		*/
/*					WAP Key 처리의 Main Routine							*/
/*																		*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
#ifdef SOCKET_AUWAP
extern byte clnta_err_flag;
void main_HandleKeys(int nKey)
{
	int nResultHandleKey = 0;
	if (clnta_err_flag && nKey == SE_KEY_CENTER) 
	{
		clnta_err_flag = 0;
		WAP_Display();
	} else if(!clnta_err_flag)
	{
		WAP_HandleKeys(nKey);
	}
}
#endif //SOCKET_AUWAP

int WAP_HandleKeys(int nKey)
{
/*===============================================================
			 WAP 상태에서 PUSH를 사용하기 위함
 ================================================================*/
	if(nKey == SE_WAP_PUSH)
	{
		return WAP_PUSH_IN_WAP;
	}	
	if(nKey == SE_WAP_MAIL)
	{
		return WAP_MAIL_IN_WAP;
	}
#ifdef TMP
	if(nKey == SE_WAP_PUSHMAIL_CANCEL)
	{
		return WAP_PUSHMAIL_CANCEL_IN_WAP;
	}
#endif //TMP

	//------------------------------------------------
	// 대화상자
	//------------------------------------------------
	if(g_nDlgMode != WAP_DLG_MODE_NONE)
	{
		if(g_nDlgMode == WAP_DLG_MODE_CONFIRM
		|| g_nDlgMode == WAP_DLG_MODE_ALERT)
		{
			return ( WAP_HandleKeys_Dialog(nKey) );
		}
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;//exception case
	}
	
	//------------------------------------------------
	// Wap Main Menu
	//------------------------------------------------
	if(g_nMenuMode != WAP_MENU_MODE_NONE)
	{
		if(g_nMenuMode == WAP_MENU_MODE_MAIN)
		{
			return ( WAP_HandleKeys_Menu_Main(nKey) );
		}
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;//exception case
	}

	//------------------------------------------------
	// WML 문서 화면
	//------------------------------------------------
	if(g_nViewMode == WAP_VIEW_MODE_WML)
	{
		return ( WAP_HandleKeys_WML(nKey) );
	}
	if(g_nViewMode == WAP_VIEW_MODE_WML_OPTION)
	{
		return ( WAP_HandleKeys_Option(nKey) );
	}
#ifdef BLOCK // 01.02.02 ytchoi NO Use event
	if(nKey == UI_OPTSEL_F)
	{
		return WAP_HANDLE_KEY_RESULT_MAYBE_LOADING;
	}
#endif //BLOCK
	return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;//exception case
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*																		*/
/*					그래픽 화면 Key 처리 Routine						*/
/*																		*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/

LOCAL int WAP_HandleKeys_Graphics(int nKey)
{

	if(nKey == SE_KEY_CENTER)//left
	{
		WAP_Send_LCDBuffer_ToHS();
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_F2//SE_KEY_RIGHT//right
	|| nKey == SE_KEY_CLEAR)//[취소] 키
	{
		g_nGraphicMode = WAP_GRAPHIC_MODE_NONE;
		WAP_Repaint();
		return (WAP_HANDLE_KEY_RESULT_JUST_BREAK);
	}
	if(nKey == SE_KEY_END//end
	|| nKey == SE_KEY_PHONE_CLOSE)//back to idle
	{			
		g_nGraphicMode = WAP_GRAPHIC_MODE_NONE;
		return (WAP_HANDLE_KEY_RESULT_EXIT);
	}

	return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;

}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*																		*/
/*					대화상자 Key 처리 Routine							*/
/*																		*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
extern void WAP_Scrolling_Message(BOOL bDown);
extern void WAP_OK_Dialog(void);
extern void WAP_Cancel_Dialog(void);
extern void WAP_Close_Dialog(void);
extern void WAP_Alert_Dialog(void);

LOCAL int WAP_HandleKeys_Dialog(int nKey)
{

	if(nKey == SE_KEY_UP)//up
	{
		WAP_Scrolling_Message(FALSE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_DOWN)//down
	{
		WAP_Scrolling_Message(TRUE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_CENTER)//center-Alert:[확인]
						//left-Confirm:[예]
	{
		if(g_nDlgMode == WAP_DLG_MODE_ALERT)
			WAP_Alert_Dialog();
		else if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)
			WAP_OK_Dialog();

#ifdef FEATURE_MUSIC_BELL
		musicbell_check();
#endif  //#ifdef FEATURE_MUSIC_BELL

		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_F2)//SE_KEY_RIGHT)//right-Confirm:[아니요]
	{
		if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)
		{
			WAP_Cancel_Dialog();

#ifdef FEATURE_MUSIC_BELL
			musicbell_check();
#endif  //#ifdef FEATURE_MUSIC_BELL
		}
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_END//end
	|| nKey == SE_KEY_PHONE_CLOSE)//back to idle
	{			
		WAP_Close_Dialog();
		return (WAP_HANDLE_KEY_RESULT_EXIT);
	}
	
	return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;

}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*																		*/
/*					WAP Menu Key 처리 Routine							*/
/*																		*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/

/*======================================================================*/
/*					WAP Main Menu 화면									*/
/*======================================================================*/
extern void WAP_Scrolling_Menu(BOOL bDown);
extern int  WAP_Handle_CenterKey_MainMenu(void);
extern void WAP_Handle_NumKey_MainMenu(int nKey);

//extern boolean	 load_incard ; 
//extern BOOL	     loadingkind ; 

#include "..\CLNT\clnt.h"
LOCAL int WAP_HandleKeys_Menu_Main(int nKey)
{


	if(nKey == SE_KEY_UP)//up
	{
		WAP_Scrolling_Menu(FALSE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_DOWN)//down
	{
		WAP_Scrolling_Menu(TRUE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_CENTER)//center
	{
		int ret = WAP_Handle_CenterKey_MainMenu();
		//1.즐겨찾기, 2.즐겨찾기 추가, 3.새로고치기, 6.Homepage
#ifdef WAP_HOMEPAGE
		if((ret < 4) || (ret == 6))
#else //#ifdef WAP_HOMEPAGE
		if(ret < 4)
#endif //#ifdef WAP_HOMEPAGE
		{
//			loadingkind =TRUE;
			if(ret==2)
				return(WAP_HANDLE_KEY_RESULT_MAYBE_ENTERTAIN);

			return (WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
		}
		//4.새 URL 입력
		if(ret == 4)
			return (WAP_HANDLE_KEY_RESULT_NEWURL);
		//5.번호부
		if(ret == 5)
			return (WAP_HANDLE_KEY_RESULT_PHONE);

		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_F2//SE_KEY_RIGHT//right
	|| nKey == SE_KEY_CLEAR)//[취소] 키
	{
		if(entertain)
		{
			entertain = FALSE;//AddBook = FALSE;
		}

#ifdef FEATURE_MUSIC_BELL
		if(bReplayMusic)//g_fMusicbellOn)
		{
			extern char contentBuffer[];
			extern int contentLength;
			extern DownMusicbellType MusicbellInfo;
			extern char mResult[MAX_SIZE_MUSIC_RESULT];

			extern void Musicbell_play(char *Data, int Length,
										char *URL, const char* Title,
										const char* AudioType, const char* PlayType,
										char *Result);

			Musicbell_play(contentBuffer, contentLength,
							MusicbellInfo.URL, MusicbellInfo.Title,
							MusicbellInfo.AudioType, MusicbellInfo.PlayType,
							mResult);

            bReplayMusic = FALSE;
		}
#endif

#ifdef WAP_NOWRAP	//yjahn96
			//set timer for no wrap
			SetFocusNode(g_pCurFocusNode);
#endif//#ifdef WAP_NOWRAP
	
		//-------------------------------------------------------------
		// yjahn96 01.04.03
		//이미지 미리보기시 [취소]키 선택하면 animation timer start시킴
		if(Check_Play_SIS_Animation() == TRUE)
		{
			RestartAnimationTimer(600);    //		ui_sistimer_set(200, TRUE);
		} 

		g_nMenuMode = WAP_MENU_MODE_NONE;
		WAP_Repaint();
		return (WAP_HANDLE_KEY_RESULT_JUST_BREAK);
	}
	if(nKey == SE_KEY_F1)//SE_KEY_LEFT)//left
	{
		//nothing
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_RECORD || nKey == SE_KEY_F2_LONG)//SE_KEY_F2)//Top URL 들어가기
	{
		g_nMenuMode = WAP_MENU_MODE_NONE;
		SKY_loadURL(g_pCard->viewId,temp_url, FALSE);
        normal_use.viewid  = 1;
		normal_use.id      = 0;
		normal_use.wmlkind = 6;

		//firstsite = TRUE;
		return(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}

	if(nKey == SE_KEY_END//end
	|| nKey == SE_KEY_PHONE_CLOSE)//back to idle
	{			
		return (WAP_HANDLE_KEY_RESULT_EXIT);
	}

	if(nKey == SE_CALL_ENDED)//if(nKey == UI_RELEASE_F || nKey == UI_FADE_F)//end for CM
	{			
		return (WAP_HANDLE_RECEIVE_RELEASE);
	}

	if(nKey == SE_KEY_1)// 즐겨찾기
	{
		WAP_Handle_NumKey_MainMenu(1);
		return (WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
	if(nKey == SE_KEY_2)// 즐겨찾기에 추가
	{
		WAP_Handle_NumKey_MainMenu(2);
		return (WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
	if(nKey == SE_KEY_3)// 새로 고치기
	{
		WAP_Handle_NumKey_MainMenu(3);
		return (WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
	if(nKey == SE_KEY_4)// 새 URL 열기
	{
		WAP_Handle_NumKey_MainMenu(4);
		return(WAP_HANDLE_KEY_RESULT_NEWURL);
	}
	if(nKey == SE_KEY_5)// 전화부
	{
		WAP_Handle_NumKey_MainMenu(5);
		return (WAP_HANDLE_KEY_RESULT_PHONE);
	}
#ifdef WAP_HOMEPAGE
	if(nKey == SE_KEY_6)// Homepage
	{
		WAP_Handle_NumKey_MainMenu(6);
		return (WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
#endif //#ifdef WAP_HOMEPAGE

	return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;//exception case

}


/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*																		*/
/*					WAP WML 문서 Key 처리 Routine						*/
/*																		*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/

/*======================================================================*/
/*					WML 문서 화면										*/
/*======================================================================*/
extern void WAP_Scrolling_TableRow(BOOL bRight);
extern int WAP_Handle_CenterKey(void);
extern BOOL WAP_Handle_RightKey(void);
extern BOOL WAP_Handle_NumKey_ForDo(int nKey);
#ifdef WAP_NOWRAP	//yjahn96
extern void CancelNowrapTimer(void);
#endif	//#ifdef WAP_NOWRAP	

LOCAL int WAP_HandleKeys_WML(int nKey)
{

#ifdef BLOCK // 01.02.02 ytchoi NO Use event
	if(nKey == UI_OPTSEL_F)
	{
		return WAP_HANDLE_KEY_RESULT_MAYBE_LOADING;
	}
#endif //BLOCK
	//CLNTa_log(1, 0, "nKey[%x]",nKey);
	if(nKey == SE_KEY_STAR)//* (<-)
	{
		WAP_Scrolling_TableRow(FALSE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_POUND)//# (->)
	{
		WAP_Scrolling_TableRow(TRUE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_UP)//up
	{
		WAP_Scrolling(FALSE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_DOWN)//down 
	{
		WAP_Scrolling(TRUE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_CENTER)//center 
	//if(nKey == 0x19b)//center 
	{
        int ret ;
#ifdef WAP_USE_LEAK
		extern isReStartComleted;
		if(isReStartComleted == 3)
		{
			return WAP_HANDLE_ACCEPT_LEAK;
		}
#endif//#ifdef WAP_USE_LEAK
		ret =  WAP_Handle_CenterKey();

		if(ret == _ELEMENT_TYPE_INPUT)//입력 화면 전환
			return (WAP_HANDLE_KEY_RESULT_INPUT);

		if(ret == _ELEMENT_TYPE_SELECT)//옵션 화면 전환
			return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;

		if(ret == _ELEMENT_TYPE_KEY
		|| ret == _ELEMENT_TYPE_LINK_TEXT
		|| ret == _ELEMENT_TYPE_LINK_IMAGE)//키, 링크
		{
			return(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
		}

#ifndef WAP_USE_SECURE
		if(ret ==_ELEMENT_TYPE_WTLS_SESSION)
		{
			return WAP_WTLS_SESSION_START;
		}
#endif//#ifndef WAP_USE_SECURE
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_F1)//SE_KEY_LEFT)//left : wap menu 들어가기
	{
		//------------------------------------------------
		//WAP Menu 상황이 아닌 경우
		//	->	WAP Menu 상황으로 들어간다
		//------------------------------------------------
		if(g_nDlgMode == WAP_DLG_MODE_NONE
		&& g_nMenuMode == WAP_MENU_MODE_NONE)
		{
#ifdef FEATURE_MUSIC_BELL
			bReplayMusic = FALSE;

			if(g_fMusicbellOn)//ui.musicbell_on)
			{
				bReplayMusic = TRUE;
			}

			musicbell_check();
#endif

#ifdef WAP_NOWRAP	//yjahn96
			CancelNowrapTimer(); //ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP

			//-------------------------------------------------------------
			// yjahn96 01.04.03 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

			g_nMenuMode = WAP_MENU_MODE_MAIN;
			WAP_Build_Menu();
		}
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_F2)//)SE_KEY_RIGHT)//right : 상위
	//if(nKey == 0x13e)//right : 상위
	{
#ifdef WAP_USE_LEAK
		extern isReStartComleted;
		if(isReStartComleted == 3)
		{
			return WAP_HANDLE_NOT_ACCEPT_LEAK;
		}
#endif//#ifdef WAP_USE_LEAK	
		//-------------------------------------------------------------
		// yjahn96 01.04.03 - for Animation timer stop
		CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96 01.03.21
			CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP
		if(WAP_Handle_RightKey())
			return WAP_HANDLE_KEY_RESULT_MAYBE_LOADING;
		
		//[상위] 키가 없는 경우 [취소] 처리를 한다
		nKey = SE_KEY_CLEAR;
	}
	if(nKey == SE_KEY_CLEAR)//[취소] 키
	{
		extern boolean bStopKey;
		extern BOOL g_bBuildingNewCard;

		//-------------------------------------------------------------
		// yjahn96 01.04.03
		//이미지 미리보기시 [취소]키 선택하면 animation timer start시킴
		if(Check_Play_SIS_Animation() == TRUE)
		{
			RestartAnimationTimer(600);    //		ui_sistimer_set(200, TRUE);
		} 

#ifdef WAP_NOWRAP	//yjahn96 01.03.21
		CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP
		if(!g_bBlocked && !bStopKey && !g_bBuildingNewCard) 
		{
#ifdef WAP_USE_GOBACK_ERROR			
			if(!g_pCard->isBookmarkable)
#endif//#ifdef WAP_USE_GOBACK_ERROR
			{
				SKY_goBack(1);
				return(WAP_HANDLE_KEY_RESULT_GOBACK);
			}
#ifdef WAP_USE_GOBACK_ERROR
			else//g_pCard->isBookmarkable = TRUE
			{
				return(WAP_HANDLE_KEY_RESULT_GOBACK_ERROR);
			}
#endif//#ifdef WAP_USE_GOBACK_ERROR
		}
		else  return (WAP_HANDLE_KEY_RESULT_JUST_BREAK);
	}

#ifdef GIZMO_A7M
	if((nKey == HS_WAP_K && ui.earpieceup)//Top URL 들어가기
	|| (nKey == HS_PF3_LONG_K && !ui.earpieceup))//Top URL 들어가기
#else// GIZMO_A7M
	if(nKey == SE_KEY_RECORD || nKey == SE_KEY_F2_LONG)//SE_KEY_F2)//Top URL 들어가기
#endif// GIZMO_A7M
	{
//		loadingkind = TRUE;

		//-------------------------------------------------------------
		// yjahn96 01.04.03 - for Animation timer stop
		CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96 01.03.21
			CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP
				
		SKY_loadURL(g_pCard->viewId,temp_url, FALSE);
        normal_use.viewid  = 1;
		normal_use.id      = 0;
		normal_use.wmlkind = 6;

		//firstsite = TRUE;
		return(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
	if(nKey == SE_KEY_END//end
	|| nKey == SE_KEY_PHONE_CLOSE)//back to idle
	{
		//-------------------------------------------------------------
		// yjahn96 01.04.03 - for Animation timer stop
		CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96 01.03.21
		CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP
		return WAP_HANDLE_KEY_RESULT_EXIT;
	}
	if(nKey == SE_CALL_ENDED)//if(nKey == UI_RELEASE_F || nKey == UI_FADE_F)//end for CM
	{			
		return (WAP_HANDLE_RECEIVE_RELEASE);
	}
	//11/16 for WTA makecall..
	if(nKey == SE_WAP_MAKECALL)//UI_MAKECALL_F end
	{			
		return (WAP_HANDLE_MAKECALL);
	}
	// SK Telecom Do element(키)를 위한 키 처리
	if((SE_KEY_0 <= nKey && nKey <= SE_KEY_9)
//	|| (nKey == SE_KEY_RECORD || nKey == SE_KEY_PLAY)
	)
	{
		int nKeyForSK;

		switch(nKey)
		{

			//case SE_KEY_RECORD	: nKeyForSK = WAP_KEY_SK_PREV;	break;//이전
			//case SE_KEY_PLAY	: nKeyForSK = WAP_KEY_SK_NEXT;	break;//다음
#ifdef PORTING_B0_WAP
			default			: nKeyForSK = nKey - SE_KEY_0 + 1;//숫자
#else //PORTING_B0_WAP
			default			: nKeyForSK = nKey - SE_KEY_0;//숫자
#endif //PORTING_B0_WAP
		}

		if(WAP_Handle_NumKey_ForDo(nKeyForSK))
		{
#ifndef WAP_USE_SECURE
			if(Wtlshello)
			{
				return(WAP_WTLS_SESSION_START);
			}
#endif //#ifndef WAP_USE_SECURE
			return(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
		}
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}

	return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;//exception case
}

/*======================================================================*/
/*					WML Option 화면										*/
/*======================================================================*/
extern void WAP_Handle_LeftKey_Options(void);
extern void WAP_Handle_CenterKey_Options(void);
extern void WAP_Handle_RightKey_Options(void);

LOCAL int WAP_HandleKeys_Option(int nKey)
{

#ifdef BLOCK // 01.02.02 ytchoi NO Use event
	if(nKey == UI_OPTSEL_F)
	{
		return WAP_HANDLE_KEY_RESULT_MAYBE_LOADING;
	}
#endif //BLOCK
	if(nKey == SE_KEY_UP)//up
	{
		WAP_Scrolling_Options(FALSE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_DOWN)//down
	{
		WAP_Scrolling_Options(TRUE);
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_CENTER)//center
	{
		WAP_Handle_CenterKey_Options();
#ifndef WAP_USE_SECURE
		CLNTa_log(1,0,"HERE FOR_MS REGION1");
		if(wtls_session) 
		{
			CLNTa_log(1,0,"HERE FOR_MS REGION2");
			wtls_session =FALSE;
			return WAP_WTLS_SESSION_START;
		}
#endif//#ifndef WAP_USE_SECURE
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_F2)//SE_KEY_RIGHT)//right
	{
		WAP_Handle_RightKey_Options();
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_F1)//SE_KEY_LEFT)//left
	{
		WAP_Handle_LeftKey_Options();
		return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;
	}
	if(nKey == SE_KEY_CLEAR)//[취소] 키
	{
		//-----------------------------------
		// WML 화면(화면 전환)
		g_nViewMode = WAP_VIEW_MODE_WML;
		g_nCurFocusOption = 0;

		WAP_Display();
		return (WAP_HANDLE_KEY_RESULT_JUST_BREAK);
	}
	if(nKey == SE_KEY_RECORD || nKey == SE_KEY_F2_LONG)//SE_KEY_F2)//Top URL 들어가기
	{
		SKY_loadURL(g_pCard->viewId,"http://wap.n-top.com", FALSE);
        normal_use.viewid  = 1;
		normal_use.id      = 0;
		normal_use.wmlkind = 6;

		//firstsite = TRUE;
		return(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
	if(nKey == SE_KEY_END//end
	|| nKey == SE_KEY_PHONE_CLOSE)//back to idle
	{	
		return WAP_HANDLE_KEY_RESULT_EXIT;
	}
	if(nKey == SE_CALL_ENDED)//if(nKey == UI_RELEASE_F || nKey == UI_FADE_F)//end for CM
	{			
		return (WAP_HANDLE_RECEIVE_RELEASE);
	}
	return WAP_HANDLE_KEY_RESULT_JUST_BREAK;//0;//exception case

}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*					WAP Repaint Routine									*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
extern void WAP_Display_Dialog(void);
extern void WAP_Display_Menu(void);

void WAP_Repaint(void)
{
	extern void report_event(wap_name_type name);
	//------------------------------------------------
	//stop animation bar
	//ui_wappic_set(0, FALSE);
	report_event(WAP_DRAW_STOP_EVENT);

	//------------------------------------------------
	// 대화상자
	//------------------------------------------------
	if(g_nDlgMode != WAP_DLG_MODE_NONE)
	{
		WAP_Display_Dialog();
		return;
	}

	//------------------------------------------------
	// Wap Main Menu
	//------------------------------------------------
	if(g_nMenuMode != WAP_MENU_MODE_NONE)
	{
		if(g_nMenuMode == WAP_MENU_MODE_MAIN)
		{
			WAP_Display_Menu();
			return;
		}
		return;
	}
	//------------------------------------------------
	// WML 문서 화면
	//------------------------------------------------
	if(g_nViewMode == WAP_VIEW_MODE_WML)
	{
		WAP_Display();
		return;
	}
	if(g_nViewMode == WAP_VIEW_MODE_WML_OPTION)
	{
		WAP_Display_Options();
		return;
	}
    MSG_HIGH("",0,0,0);
}		