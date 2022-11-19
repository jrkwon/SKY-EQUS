
#include "customer.h"
#include <stdio.h>
//////////////////////////////////////////////////

#include <string.h>

#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"

//////////////////////////////////////////////////
#include "ds.h"
#include "psi.h"


//-----------------------------------------------------------------------------------
//
// MMI 의 Data structure
//
//-----------------------------------------------------------------------------------
#include "sky.h"
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "ska_WAPAPP.h"


//=========================================================================================================
//  B2 WAP
//=========================================================================================================
#ifdef FEATURE_WAP  
//////////////////////////////////////////////////
#include "wapconnector.h"

	BOOL wtls_session = FALSE;
	extern BOOL Wtlshello; 

extern CHAR  temp_url[200];

extern char *url_pophistory(void); // url_history
extern char from_goback; //url_history

//extern BOOL entertain;
BOOL entertain = FALSE;
boolean bStopKey    = FALSE;

#ifdef FEATURE_MUSIC_BELL
boolean bReplayMusic = FALSE;
extern  boolean		g_fMusicbellOn;
extern void musicbell_check(void);
#endif //		#ifdef FEATURE_MUSIC_BELL

extern UINT8 g_nAniBarCount;
extern BOOL g_bDrawBar;
extern BOOL g_bImageLoading; //yjahn96 01.11.09 ---- 이미지 로딩중 스크롤시 화면 깨지는 문제

#ifdef FEATURE_CHARACTER_BELL
boolean bReplayCharacbell = FALSE;
extern  boolean         g_fCharacbellPreview;
extern unsigned char    *CBMelodyData;
#endif // FEATURE_CHARACTER_BELL

//#if defined(WAP_USE_DORM) || defined(WAP_USE_SKT_NEW) 
#include "Gendsp.h"
	extern storeWML normal_use;
//#endif//#ifdef WAP_USE_DORM


extern boolean Check_Play_SIS_Animation(void);

void WAP_Repaint(void);
////////////////////////////////////////////////////////////////////

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*					WAP Key Event 처리의 Main Handler					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_MainHandle_Event(int Event)
{
    //------------------------------------
    // SMS 관련 이벤트 처리
    //------------------------------------
    if(Event == SE_WAP_PUSH || Event == SE_WAP_MAIL)
    {
        WAP_Handle_SMS(Event);
        return;
    }
    //------------------------------------
    // Dialog Mode일때 이벤트 처리
    //------------------------------------
    if(g_nDlgMode != WAP_DLG_MODE_NONE) 
    {
         WAP_Handle_DIALOG(Event);
         return;
    }
    //------------------------------------
    // Main Menu Mode(n.Top메뉴)일때 이벤트 처리
    //------------------------------------
    if(g_nMenuMode != WAP_MENU_MODE_NONE) 
    {
        if(g_nMenuMode == WAP_MENU_MODE_MAIN) 
            WAP_Handle_MENU_MAIN(Event);
        return;
    }
    //------------------------------------
    // 일반적인 WML일때 이벤트 처리
    //------------------------------------
    if(g_nViewMode == WAP_VIEW_MODE_WML) 
    {
         WAP_Handle_WML(Event);
         return;
    }
    //------------------------------------
    // WML에서 Option 화면일때 이벤트 처리
    //------------------------------------
    if(g_nViewMode == WAP_VIEW_MODE_WML_OPTION) 
    {
         WAP_Handle_WML_OPTION(Event);
         return;
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*					    SMS관련 이벤트 처리 함수    					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_SMS(int Event)
{
        // 그림친구 화면에서 wap push/mail수신후 연결시 문제 
    CancelAnimationTimer(); //stop sis animation
#ifdef WAP_NOWRAP
    CancelNowrapTimer(); //stop nowrap
#endif//#ifdef WAP_NOWRAP

    if(Event == SE_WAP_PUSH)
    {
		if(InWapMailorPush == 2)//전card가 있음 
		{
			WAP_Repaint();
        }
		else//초기 접속 그림
		{
			PlayNTopMainWapImage();
		}
		SKY_loadURL(1, (const CHAR*)g_pszPushUrl, TRUE);
    }
    else if(Event == SE_WAP_MAIL)
    {
		if(InWapMailorPush == 2)//전card가 있음 
		{
			WAP_Repaint();
        }
		else//초기 접속 그림
		{
			PlayNTopMainWapImage();
		}				
		SKY_loadURL(1, gWAPEmail_Url, TRUE);
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*      		    Dialog Mode시 이벤트 처리 함수    					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_DIALOG(int Event)
{
    if(g_nDlgMode == WAP_DLG_MODE_PASSWORD)
    {
        //sSKA_WAPDIALOG_ID()로 이동
        s_towhereminor = WAPGOMINOR_DI_ID;
        EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);    
    }
    else if(g_nDlgMode == WAP_DLG_MODE_PROMPT)
    {
        //sSKA_WAPDIALOG_PROMPT()로 이동
        s_towhereminor = WAPGOMINOR_DI_PROMPT;
        EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);
    }
    else if(g_nDlgMode == WAP_DLG_MODE_ALERT ||	g_nDlgMode == WAP_DLG_MODE_CONFIRM)
    {
        switch(Event)
        {
        case SE_KEY_UP:     WAP_Scrolling_Message(FALSE); break;
        case SE_KEY_DOWN:   WAP_Scrolling_Message(TRUE); break;
        case SE_KEY_CENTER: 
            {
                if(g_nDlgMode == WAP_DLG_MODE_ALERT)    //[확인]
			        WAP_Alert_Dialog();
		        else if(g_nDlgMode == WAP_DLG_MODE_CONFIRM) //[예]
			        WAP_OK_Dialog();
#ifdef FEATURE_MUSIC_BELL
		        musicbell_check();
#endif  //#ifdef FEATURE_MUSIC_BELL
                
#ifdef FEATURE_CHARACTER_BELL
                if (g_fCharacbellPreview) {
	                SKY_StopMusic();
                    g_fCharacbellPreview = FALSE;
	            }
#endif // FEATURE_CHARACTER_BELL
            }
            break;
        case SE_KEY_F2:  
            {
		        if(g_nDlgMode == WAP_DLG_MODE_CONFIRM) //[아니오]
			        WAP_Cancel_Dialog();
#ifdef FEATURE_MUSIC_BELL
		        musicbell_check();
#endif  //#ifdef FEATURE_MUSIC_BELL

#ifdef FEATURE_CHARACTER_BELL
                if (g_fCharacbellPreview) {
	                SKY_StopMusic();
                    g_fCharacbellPreview = FALSE;
	            }
#endif // FEATURE_CHARACTER_BELL
            }
            break;
        default:    break;
        }
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*  			    n.TOP메뉴 시 이벤트 처리 함수    					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_MENU_MAIN(int Event)
{
    int nCurMenu;
    switch(Event)
    {
    case SE_KEY_UP:     WAP_Scrolling_Menu(FALSE);  break;
    case SE_KEY_DOWN:   WAP_Scrolling_Menu(TRUE);  break;
    case SE_KEY_CENTER:
        {
		    //1.즐겨찾기, 2.즐겨찾기 추가, 3.새로고치기
            nCurMenu = WAP_Handle_CenterKey_MainMenu();
            if(nCurMenu == 4)   //4. 새 URL 열기
            {
                //sSKA_WAPNewURL()로 이동
                s_towhereminor = WAPGOMINOR_NEWURL;
                EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);             
            }
            else if(nCurMenu ==5)   //5.Phone기능
            {
                //SKA_BookInCalling()로 이동
                s_towhereminor = WAPGOMINOR_PHONEBOOK;
                EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);            
            }

        }
        break;

    case SE_KEY_1:  WAP_Handle_NumKey_MainMenu(1);  break;      //1.즐겨찾기
    case SE_KEY_2:  WAP_Handle_NumKey_MainMenu(2);  break;      //2.즐겨찾기 추가
    case SE_KEY_3:  WAP_Handle_NumKey_MainMenu(3);  break;      //3.새로고치기
    case SE_KEY_4:  WAP_Handle_NumKey_MainMenu(4);              //4. 새 URL 열기
                    s_towhereminor = WAPGOMINOR_NEWURL;    //sSKA_WAPNewURL()로 이동
                    EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);            break;
    case SE_KEY_5:  WAP_Handle_NumKey_MainMenu(5);              //5.Phone기능
                    s_towhereminor = WAPGOMINOR_PHONEBOOK;     //SKA_BookInCalling()로 이동
                    EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);            break;

    case SE_KEY_RECORD:     //Top URL 들어가기
    case SE_KEY_F2_LONG:    
    	{
		    g_nMenuMode = WAP_MENU_MODE_NONE;
		    SKY_loadURL(g_pCard->viewId,gWAPinit_url, FALSE);
            normal_use.viewid  = 1;
		    normal_use.id      = 0;
		    normal_use.wmlkind = 6;

		    //firstsite = TRUE;
    	}
        break;

    case SE_KEY_F2:         //[취소]키
    case SE_KEY_CLEAR:
        {
		    if(entertain)
		    {
			    entertain = FALSE;//AddBook = FALSE;
		    }

   		    g_nMenuMode = WAP_MENU_MODE_NONE;

            //-------------------------------
            // Musicbell 듣던 중.... 
            //-------------------------------
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

#ifdef FEATURE_CHARACTER_BELL
		    if(bReplayCharacbell)//g_fCharacbellPreview
		    {
                g_fCharacbellPreview = TRUE;
                SKY_PlayMusicBufferEx((byte*)CBMelodyData, SKY_GET_BELL_VOLUME(), 0);     //play music in buffer
			    bReplayCharacbell = FALSE;
		    }
#endif // FEATURE_CHARACTER_BELL
            //-------------------------------
    		//  이미지 미리보기시 [취소]키 선택하면 animation timer start시킴
            //-------------------------------
		    if(Check_Play_SIS_Animation() == TRUE)
		    {
			    RestartAnimationTimer(600);    //		ui_sistimer_set(200, TRUE);
		    } 
#ifdef WAP_NOWRAP	//yjahn96
			//set timer for no wrap
			SetFocusNode(g_pCurFocusNode);
#endif//#ifdef WAP_NOWRAP
		    WAP_Repaint();
        }
        break;
    default:
        break;
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*  	    일반적인 WML문서 화면에서 이벤트 처리 함수 					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_WML(int Event)
{
    extern unsigned int WAEStatus ;
	extern BOOL g_bBuildingNewCard;

    switch(Event)
    {
    case SE_KEY_STAR: 
		{
			if(g_bImageLoading) break;	//yjahn96 01.11.09
			WAP_Scrolling_TableRow(FALSE);  
		}
		break;
    case SE_KEY_POUND:
		{
			if(g_bImageLoading) break;
			WAP_Scrolling_TableRow(TRUE);   
		}
		break;
    case SE_KEY_UP:
		{
			if(g_bImageLoading) break;
			WAP_Scrolling(FALSE);           
		}
		break;
    case SE_KEY_DOWN:
		{
			if(g_bImageLoading) break;
			WAP_Scrolling(TRUE);            
		}break;
    case SE_KEY_CENTER: 
        {
            // 2001/11/29 ytchoi loading상태에서 input등에 상태로 가면 화면이 깨진다.
            if((WAEStatus == ContentIsDone || 
                WAEStatus == ImageIsDone   || 
                WAEStatus == ScriptIsDone  ||
                WAEStatus == LoadingDataDone ||
                WAEStatus == ReadFromNetworkDone))
            {            
                WAP_Handle_CenterKey();         
                if(g_nViewMode == WAP_VIEW_MODE_WML_INPUT)
                {
                    s_towhereminor = WAPGOMINOR_INPUT;    //sSKA_WAPNewURL()로 이동
                    EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);                
                }
            }
        }
        break;
    case SE_KEY_F1:     
		//------------------------------------------------
		// n.Top메뉴 상황이 아닌 경우 //	->	n.Top메뉴 상황으로 들어간다
		//------------------------------------------------
		if(g_nDlgMode == WAP_DLG_MODE_NONE && g_nMenuMode == WAP_MENU_MODE_NONE)
        {
			if(g_pCard == NULL)	//yjahn96 01.10.24
				return; 

#ifdef FEATURE_MUSIC_BELL
			bReplayMusic = FALSE;

			if(g_fMusicbellOn) 		bReplayMusic = TRUE;

			musicbell_check();
#endif
#ifdef FEATURE_CHARACTER_BELL
            bReplayCharacbell = FALSE;
            if(g_fCharacbellPreview) {
                bReplayCharacbell = TRUE;
                SKY_StopMusic();
                g_fCharacbellPreview = FALSE;
            }
#endif // FEATURE_CHARACTER_BELL
			CancelAnimationTimer(); // for Animation timer stop
#ifdef WAP_NOWRAP	
			CancelNowrapTimer();    //stop nowrap
#endif//#ifdef WAP_NOWRAP

  			g_nMenuMode = WAP_MENU_MODE_MAIN;
			WAP_Build_Menu();

        }
        break;
    case SE_KEY_F2:
		CancelAnimationTimer(); // for Animation timer stop
#ifdef WAP_NOWRAP	
		CancelNowrapTimer();    //stop nowrap
#endif//#ifdef WAP_NOWRAP
        if(g_pCard == NULL)
        {
            EQS_SEND_EVENT( SE_KEY_END, EQS_NA, EQS_NA );
            break;
        }
        else if( g_bBlocked) //yjahn96 01.11.09
		{			
			break;
		}
        else if(!(WAEStatus == ContentIsDone || 
                    WAEStatus == ImageIsDone   || 
                    WAEStatus == ScriptIsDone  ||
                    WAEStatus == LoadingDataDone ||
                    WAEStatus == ReadFromNetworkDone))
        {
            //resetalldata();
            SKY_SetGeneralTimer(0, FALSE);
			g_nAniBarCount=0;
			g_bDrawBar=FALSE;
            MMIc_stop(1);
            ProcessEvents();
            SKY_DrawAnnunciators();
            WAP_Repaint();
            break;
        }
        else if(WAP_Handle_RightKey())    //[상위]키 있을때 --> 없으면 [취소]키와 같이 동작
        {
            break;
        }

    case SE_KEY_CLEAR:
        {
		    extern boolean bStopKey;

		    //이미지 미리보기시 [취소]키 선택하면 animation timer start시킴
		    if(Check_Play_SIS_Animation() == TRUE)
		    {
			    RestartAnimationTimer(600);    
		    } 
#ifdef WAP_NOWRAP	//yjahn96 01.03.21
		    CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
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

		    if(g_pCard == NULL)
            {
                EQS_SEND_EVENT( SE_KEY_END, EQS_NA, EQS_NA );
            }
			else if(g_bBlocked) //yjahn96 01.11.09
			{			
				break;
			}
            else if(!(WAEStatus == ContentIsDone || 
                    WAEStatus == ImageIsDone   || 
                    WAEStatus == ScriptIsDone  ||
                    WAEStatus == LoadingDataDone ||
                    WAEStatus == ReadFromNetworkDone))
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
            else if(!g_bBlocked && !bStopKey && !g_bBuildingNewCard) 
		    {
#ifdef WAP_USE_GOBACK_ERROR			
			    if(!g_pCard->isBookmarkable)
#endif//#ifdef WAP_USE_GOBACK_ERROR
			    {
				    SKY_goBack(1);
					    //receiveOK = TRUE;
					    normal_use.viewid  = 1;normal_use.id  = 0;normal_use.wmlkind = 7;
					    //process_goback_event();		
			    }
#ifdef WAP_USE_GOBACK_ERROR
			    else
			    {
					    isFailuregoBack = 1;
					    EQS_START_SIBLING(sSKA_WAPERROR,FALSE);
			    }
#endif//#ifdef WAP_USE_GOBACK_ERROR
		    }
        }
        break;
    case SE_KEY_RECORD:     //Top URL 들어가기
    case SE_KEY_F2_LONG:
        {
            CancelAnimationTimer(); // for Animation timer stop
#ifdef WAP_NOWRAP	
			CancelNowrapTimer();    //stop nowrap
#endif//#ifdef WAP_NOWRAP
		    SKY_loadURL(g_pCard->viewId,gWAPinit_url, FALSE);
            normal_use.viewid  = 1;
		    normal_use.id      = 0;
		    normal_use.wmlkind = 6;
        }
        break;
    default:
        if((SE_KEY_0 <= Event && Event <= SE_KEY_9))   // SK Telecom Do element(키)를 위한 키 처리
	    {
		    int nKeyForSK;
            nKeyForSK = Event - SE_KEY_0;//숫자
		    
		    if(WAP_Handle_NumKey_ForDo(nKeyForSK))
		    {

            }
	    }
        break;
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*		    WML에서 Option화면에서의 이벤트 처리 함수  					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_WML_OPTION(int Event)
{
    switch(Event)
    {
    case SE_KEY_UP:     WAP_Scrolling_Options(FALSE);   break;
    case SE_KEY_DOWN:   WAP_Scrolling_Options(TRUE);    break;
    case SE_KEY_CENTER:
        {
		    WAP_Handle_CenterKey_Options();
        }
        break;
    case SE_KEY_F1:     WAP_Handle_LeftKey_Options();   break;     
    case SE_KEY_F2:     WAP_Handle_RightKey_Options();  break;
    case SE_KEY_CLEAR:  
        {
		    g_nViewMode = WAP_VIEW_MODE_WML;    // WML 화면(화면 전환)
		    g_nCurFocusOption = 0;

		    WAP_Display();
        }
        break;

    case SE_KEY_RECORD:     //Top URL 들어가기
    case SE_KEY_F2_LONG:    
    	{
    		SKY_loadURL(g_pCard->viewId,gWAPinit_url, FALSE);//SKY_loadURL(g_pCard->viewId,temp_url, FALSE);
            normal_use.viewid  = 1;
		    normal_use.id      = 0;
		    normal_use.wmlkind = 6;

		    //firstsite = TRUE;
    	}
        break;

    }
}
#endif   //#ifdef FEATURE_WAP
//=========================================================================================================

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*					WAP Repaint Routine									*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
//extern void WAP_Display_Dialog(void);
//extern void WAP_Display_Menu(void);

void WAP_Repaint(void)
{
//	extern void report_event(wap_name_type name);
	//------------------------------------------------
	//stop animation bar
	//ui_wappic_set(0, FALSE);
//	report_event(WAP_DRAW_STOP_EVENT);

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


