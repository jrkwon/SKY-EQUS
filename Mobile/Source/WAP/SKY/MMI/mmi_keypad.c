
#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
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

//////////////////////////////////////////////////
#include "ds.h"
#include "psi.h"
#include "waptask.h"


//-----------------------------------------------------------------------------------
//
// MMI �� Data structure
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
//////////////////////////////////////////////////
#include "wapconnector.h"

#ifndef WAP_USE_SECURE
	BOOL wtls_session = FALSE;
	extern BOOL Wtlshello; 
#endif//#ifndef WAP_USE_SECURE

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

//#if defined(WAP_USE_DORM) || defined(WAP_USE_SKT_NEW) 
#include "Gendsp.h"
	extern storeWML normal_use;
//#endif//#ifdef WAP_USE_DORM


extern boolean Check_Play_SIS_Animation(void);

void WAP_Repaint(void);
////////////////////////////////////////////////////////////////////

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*					WAP Key Event ó���� Main Handler					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_MainHandle_Event(int Event)
{
    //------------------------------------
    // SMS ���� �̺�Ʈ ó��
    //------------------------------------
    //if(Event == SE_WAP_PUSH || Event == SE_WAP_MAIL)
    {
    //    WAP_Handle_SMS(Event);
    //    return;
    }
    //------------------------------------
    // Dialog Mode�϶� �̺�Ʈ ó��
    //------------------------------------
    if(g_nDlgMode != WAP_DLG_MODE_NONE) 
    {
         WAP_Handle_DIALOG(Event);
         return;
    }
    //------------------------------------
    // Main Menu Mode(n.Top�޴�)�϶� �̺�Ʈ ó��
    //------------------------------------
    if(g_nMenuMode != WAP_MENU_MODE_NONE) 
    {
        if(g_nMenuMode == WAP_MENU_MODE_MAIN) 
            WAP_Handle_MENU_MAIN(Event);
        return;
    }
    //------------------------------------
    // �Ϲ����� WML�϶� �̺�Ʈ ó��
    //------------------------------------
    if(g_nViewMode == WAP_VIEW_MODE_WML) 
    {
         WAP_Handle_WML(Event);
         return;
    }
    //------------------------------------
    // WML���� Option ȭ���϶� �̺�Ʈ ó��
    //------------------------------------
    if(g_nViewMode == WAP_VIEW_MODE_WML_OPTION) 
    {
         WAP_Handle_WML_OPTION(Event);
         return;
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*					    SMS���� �̺�Ʈ ó�� �Լ�    					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_SMS(int Event)
{
        // �׸�ģ�� ȭ�鿡�� wap push/mail������ ����� ���� 
        CancelAnimationTimer(); //stop sis animation
#ifdef WAP_NOWRAP
       	CancelNowrapTimer(); //stop nowrap
#endif//#ifdef WAP_NOWRAP

//    if(Event == SE_WAP_PUSH)
    {
/*		nRetry =0;
#ifdef WAP_USE_DORM
		wap_clk_key = FALSE;
#endif//#ifdef WAP_USE_DORM
		if(InWapMailorPush == 2)//��card�� ���� 
		{
			WAP_Repaint();
        }
		else//�ʱ� ���� �׸�
		{
			PlayNTopConntImage();
		}
		push_acc = 1;				
		SKY_loadURL(1, g_pszPushUrl, TRUE);
		receiveOK = TRUE;
		bWtlsgoback = FALSE;
		displayloading(1,TRUE);
		save_min_state = WAPLOADING_S;

		//EQS_START_SIBLING(SKA_WAPLOADING,TRUE);
*/    }
//    else if(Event == SE_WAP_MAIL)
    {
/*		nRetry =0;
#ifdef WAP_USE_DORM
		wap_clk_key = FALSE;
#endif//#ifdef WAP_USE_DORM
		push_acc = 2;
		if(InWapMailorPush == 2)//��card�� ���� 
		{
			WAP_Repaint();
        }
		else//�ʱ� ���� �׸�
		{
			PlayNTopConntImage();
		}				
		SKY_loadURL(1, EmailUrl, TRUE);
		
		receiveOK = TRUE;
		bWtlsgoback = FALSE;
		displayloading(1,TRUE);
		save_min_state = WAPLOADING_S;

		//EQS_START_SIBLING(SKA_WAPLOADING,TRUE);
*/    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*      		    Dialog Mode�� �̺�Ʈ ó�� �Լ�    					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_DIALOG(int Event)
{
    if(g_nDlgMode == WAP_DLG_MODE_PASSWORD)
    {
        //sSKA_WAPDIALOG_ID()�� �̵�
        s_towhereminor = WAPGOMINOR_DI_ID;
        EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);    
    }
    else if(g_nDlgMode == WAP_DLG_MODE_PROMPT)
    {
        //sSKA_WAPDIALOG_PROMPT()�� �̵�
        s_towhereminor = WAPGOMINOR_DI_PROMPT;
        EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);
    }
    else if(g_nDlgMode == WAP_DLG_MODE_ALERT)
    {
        switch(Event)
        {
        case SE_KEY_UP:     WAP_Scrolling_Message(FALSE); break;
        case SE_KEY_DOWN:   WAP_Scrolling_Message(TRUE); break;
        case SE_KEY_CENTER: 
            {
                if(g_nDlgMode == WAP_DLG_MODE_ALERT)    //[Ȯ��]
			        WAP_Alert_Dialog();
		        else if(g_nDlgMode == WAP_DLG_MODE_CONFIRM) //[��]
			        WAP_OK_Dialog();
#ifdef FEATURE_MUSIC_BELL
		        musicbell_check();
#endif  //#ifdef FEATURE_MUSIC_BELL
            }
            break;
        case SE_KEY_F2:  
            {
		        if(g_nDlgMode == WAP_DLG_MODE_CONFIRM) //[�ƴϿ�]
			        WAP_Cancel_Dialog();
#ifdef FEATURE_MUSIC_BELL
		        musicbell_check();
#endif  //#ifdef FEATURE_MUSIC_BELL
            }
            break;
        default:    break;
        }
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*  			    n.TOP�޴� �� �̺�Ʈ ó�� �Լ�    					*/
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
		    //1.���ã��, 2.���ã�� �߰�, 3.���ΰ�ġ��
            nCurMenu = WAP_Handle_CenterKey_MainMenu();
            if(nCurMenu == 4)   //4. �� URL ����
            {
                //sSKA_WAPNewURL()�� �̵�
                s_towhereminor = WAPGOMINOR_NEWURL;
                EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);             
            }
            else if(nCurMenu ==5)   //5.Phone���
            {
                //SKA_BookInCalling()�� �̵�
                s_towhereminor = WAPGOMINOR_PHONEBOOK;
                EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);            
            }

        }
        break;

    case SE_KEY_1:  WAP_Handle_NumKey_MainMenu(1);  break;      //1.���ã��
    case SE_KEY_2:  WAP_Handle_NumKey_MainMenu(2);  break;      //2.���ã�� �߰�
    case SE_KEY_3:  WAP_Handle_NumKey_MainMenu(3);  break;      //3.���ΰ�ġ��
    case SE_KEY_4:  WAP_Handle_NumKey_MainMenu(4);              //4. �� URL ����
                    s_towhereminor = WAPGOMINOR_NEWURL;    //sSKA_WAPNewURL()�� �̵�
                    EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);            break;
    case SE_KEY_5:  WAP_Handle_NumKey_MainMenu(5);              //5.Phone���
                    s_towhereminor = WAPGOMINOR_PHONEBOOK;     //SKA_BookInCalling()�� �̵�
                    EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);            break;

    case SE_KEY_RECORD:     //Top URL ����
    case SE_KEY_F2_LONG:    
    	{
		    g_nMenuMode = WAP_MENU_MODE_NONE;
		    SKY_loadURL(g_pCard->viewId,temp_url, FALSE);
            normal_use.viewid  = 1;
		    normal_use.id      = 0;
		    normal_use.wmlkind = 6;

		    //firstsite = TRUE;
    	}
        break;

    case SE_KEY_F2:         //[���]Ű
    case SE_KEY_CLEAR:
        {
		    if(entertain)
		    {
			    entertain = FALSE;//AddBook = FALSE;
		    }

            g_nMenuMode = WAP_MENU_MODE_NONE;

            //-------------------------------
            // Musicbell ��� ��.... 
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
            //-------------------------------
    		//  �̹��� �̸������ [���]Ű �����ϸ� animation timer start��Ŵ
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
/*  	    �Ϲ����� WML���� ȭ�鿡�� �̺�Ʈ ó�� �Լ� 					*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
void WAP_Handle_WML(int Event)
{
    switch(Event)
    {
    case SE_KEY_STAR:   WAP_Scrolling_TableRow(FALSE);  break;
    case SE_KEY_POUND:  WAP_Scrolling_TableRow(TRUE);   break;
    case SE_KEY_UP:     WAP_Scrolling(FALSE);           break;
    case SE_KEY_DOWN:   WAP_Scrolling(TRUE);            break;
    case SE_KEY_CENTER: 
        {
            WAP_Handle_CenterKey();         
            if(g_nViewMode == WAP_VIEW_MODE_WML_INPUT)
            {
                s_towhereminor = WAPGOMINOR_INPUT;    //sSKA_WAPNewURL()�� �̵�
                EQS_SEND_EVENT(SE_WAP_GOMINOR,  EQS_NA, EQS_NA);                
            }
        }
        break;
    case SE_KEY_F1:     
		//------------------------------------------------
		// n.Top�޴� ��Ȳ�� �ƴ� ��� //	->	n.Top�޴� ��Ȳ���� ����
		//------------------------------------------------
		if(g_nDlgMode == WAP_DLG_MODE_NONE && g_nMenuMode == WAP_MENU_MODE_NONE)
        {
#ifdef FEATURE_MUSIC_BELL
			bReplayMusic = FALSE;

			if(g_fMusicbellOn) 		bReplayMusic = TRUE;

			musicbell_check();
#endif

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
        if(WAP_Handle_RightKey())    //[����]Ű ������ --> ������ [���]Ű�� ���� ����
        {
            break;
        }
    case SE_KEY_CLEAR:
        {
		    extern boolean bStopKey;
		    extern BOOL g_bBuildingNewCard;

		    //�̹��� �̸������ [���]Ű �����ϸ� animation timer start��Ŵ
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

		    if(!g_bBlocked && !bStopKey && !g_bBuildingNewCard) 
		    {
#ifdef WAP_USE_GOBACK_ERROR			
			    if(!g_pCard->isBookmarkable)
#endif//#ifdef WAP_USE_GOBACK_ERROR
			    {
				    SKY_goBack(1);
#ifdef WAP_USE_SECURE
					    Portdetect = FALSE;	
#endif//#ifdef WAP_USE_SECURE
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
    case SE_KEY_RECORD:     //Top URL ����
    case SE_KEY_F2_LONG:
        {
            CancelAnimationTimer(); // for Animation timer stop
#ifdef WAP_NOWRAP	
			CancelNowrapTimer();    //stop nowrap
#endif//#ifdef WAP_NOWRAP
		    SKY_loadURL(g_pCard->viewId,temp_url, FALSE);
            normal_use.viewid  = 1;
		    normal_use.id      = 0;
		    normal_use.wmlkind = 6;
        }
        break;
    default:
        if((SE_KEY_0 <= Event && Event <= SE_KEY_9))   // SK Telecom Do element(Ű)�� ���� Ű ó��
	    {
		    int nKeyForSK;
            nKeyForSK = Event - SE_KEY_0;//����
		    
		    if(WAP_Handle_NumKey_ForDo(nKeyForSK))
		    {
#ifndef WAP_USE_SECURE
			    if(Wtlshello)
			    {
				    //return(WAP_WTLS_SESSION_START);
			    }
#endif //#ifndef WAP_USE_SECURE
            }
	    }
        break;
    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*		    WML���� Optionȭ�鿡���� �̺�Ʈ ó�� �Լ�  					*/
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
#ifndef WAP_USE_SECURE
		    CLNTa_log(1,0,"HERE FOR_MS REGION1");
		    if(wtls_session) 
		    {
			    CLNTa_log(1,0,"HERE FOR_MS REGION2");
			    wtls_session =FALSE;
			    //return WAP_WTLS_SESSION_START;
		    }
#endif//#ifndef WAP_USE_SECURE        
        }
        break;
    case SE_KEY_F1:     WAP_Handle_LeftKey_Options();   break;     
    case SE_KEY_F2:     WAP_Handle_RightKey_Options();  break;
    case SE_KEY_CLEAR:  
        {
		    g_nViewMode = WAP_VIEW_MODE_WML;    // WML ȭ��(ȭ�� ��ȯ)
		    g_nCurFocusOption = 0;

		    WAP_Display();
        }
        break;

    case SE_KEY_RECORD:     //Top URL ����
    case SE_KEY_F2_LONG:    
    	{
    		SKY_loadURL(g_pCard->viewId,"http://wap.n-top.com", FALSE);//SKY_loadURL(g_pCard->viewId,temp_url, FALSE);
            normal_use.viewid  = 1;
		    normal_use.id      = 0;
		    normal_use.wmlkind = 6;

		    //firstsite = TRUE;
    	}
        break;

    }
}

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
/*																		*/
/*					WAP Repaint Routine									*/
/*																		*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*/
//extern void WAP_Display_Dialog(void);
//extern void WAP_Display_Menu(void);

void WAP_Repaint(void)
{
	extern void report_event(wap_name_type name);
	//------------------------------------------------
	//stop animation bar
	//ui_wappic_set(0, FALSE);
	report_event(WAP_DRAW_STOP_EVENT);

	//------------------------------------------------
	// ��ȭ����
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
	// WML ���� ȭ��
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


