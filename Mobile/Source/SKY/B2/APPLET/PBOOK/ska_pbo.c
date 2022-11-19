#include "stdio.h"
#include "ska.h"

#define BOOK_FLOW_MODE           TRUE

#define VIEW_PHOTO_IMAGE         TRUE

#define PHOTO_START_Y            0//17
#define PHOTO_IMAGE_WIDTH        84
#define PHOTO_IMAGE_HEIGHT       64


#define VIEW_PHOTO_IMAGE_WIDTH   90
#define VIEW_PHOTO_IMAGE_HEIGHT  70

#define CONTROL_MARGINE          10
#define INITIAL_INPUT_CONTROL     2
#define INPUT_CONTROL_WIDTH      12//14 
#define PERSON_INFO_WIDTH        13

#define INPUT_CONTROL_X1          2
#define INPUT_CONTROL_X2         38

#define STAR_CHARACTER_ORIGIN  0x81
#define STAR_CHARACTER_1       0xE0


#define MAX_VIEW_GROUP_NAME_LEN   14
#define MAX_VIEW_BOOK_NAME_LEN    10

#define MAX_HYPEN_DIGIT_LENGTH     8


enum tagGROUP_EDIT_MENU_E
{  
    GEM_NON_FUNCTION = 0,
    GEM_ADD_GROUP,           
    GEM_DELETE_GROUP,
	GEM_DELETE_GROUP_NAME,
    GEM_EDIT_ORDER,
    GEM_SORT_GROUP,
    GEM_INFO_MSG
};

typedef enum tagGROUP_EDIT_MENU_E   GROUP_EDIT_MENU_E;

enum tagSCROLL_DATA_E
{
    SDE_SPEED_GROUP=0,
    SDE_EDIT_PBOOK,
    SDE_GROUP_LIST,
    SDE_GROUP_EDIT,
    SDE_GROUP_SELECT,
    SDE_GROUP_REORDER,
    SDE_EMAIL_GROUP_LIST,
	SDE_GROUP_TEL_LIST,
    SDE_IRDA_GROUP,
    SDE_IRDA_BOOK,
    SDE_BOOK_LIST,
    SDE_EMAIL_BOOK_LIST,
    SDE_EMAIL_BOOK_ALLLIST,
    SDE_SEARCH_NAME,
    SDE_SEARCH_NUMBER,
    SDE_EDIT_SDIAL,
    SDE_TITLE_NUMBER,
    SDE_SELECT_NUMBER,
	SDE_SELECT_EMAIL
};

typedef enum tagSCROLL_DATA_E  SCROLL_DATA_E;


enum tagBOOK_CONTROL_E 
{    
    ICT_NUMBER_E = 0,
    ICT_EMAIL_E,
    ICT_MEMO_E ,
    ICT_ANNIV_E,
    ICT_PICTURE_E,
    ICT_NAME_E,
    ICT_MENU_E,
    ICT_GROUP_E //= 0,  //그룹추가 기능을 없앰 

};
typedef enum tagBOOK_CONTROL_E  BOOK_CONTROL_E;


enum tagEDIT_GROUP_CONTENT_E      // CONTROL for Edit Group Control
{
    EGC_NAME_TEXT=0,
    EQC_NAME_EDIT,
    EQC_MEMBER_TEXT,
    EQC_MEMBER_INFO,
	EQC_GROUP_STATUS_TEXT,
    EQC_GROUP_STATUS,
    EQC_BELL_TEXT,
    EQC_BELL_ID,
#ifdef FEATURE_PHONE_COLOR_ID_LCD   //2001.10.05 change FEATURE_COLOR_ID_LCD into FEATURE_PHONE_COLOR_ID_LCD
    EQC_LCD_TEXT,
    EQC_LCD_ID
#endif //FEATURE_PHONE_COLOR_ID_LCD
};
typedef enum tagEDIT_GROUP_CONTENT_E  EDIT_GROUP_CONTENT_E;


enum tagINPUT_STRING_E
{
	INPUT_GROUP_NAME_E = 1,
	INPUT_NEW_GROUP_NAME_E,
	INPUT_ANNIV_NAME_E
};


ROM BYTE* phone_type[] = { 
   (BYTE*)SF_PBOOK_CELL,
   (BYTE*)SF_PBOOK_HOME,
   (BYTE*)SF_PBOOK_WORK,
   (BYTE*)SF_PBOOK_PAGER
};   


LOCAL BITMAP l_PBookBitmapInfo = {
	{ EQUS_IMAGE_ID, { PHOTO_H_SIZE, PHOTO_V_SIZE}, 
	  PHOTO_H_SIZE, SKY_COLOR_SPACE, FALSE, MAX_COLOR, 0}
};


LOCAL  BOOL   l_fFromCall = FALSE;

LOCAL  BYTE   l_szParam[40];   // 다용도
LOCAL  UINT8  l_nMyBell;

LOCAL  BYTE*  l_ppMyBellList[SKY_MAX_NUM_FILES];    // 나만의벨

LOCAL  INT16  l_nBookOverWriteAddress;


/*#ifdef FEATURE_CHARACBELL
LOCAL   UINT8   l_nChaBell;
LOCAL  BYTE*  l_ppChaBellList[SKY_MAX_NUM_FILES];    // Characbell
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
LOCAL   UINT8   l_nChaBell;
LOCAL  BYTE*  l_ppChaBellList[SKY_MAX_NUM_FILES];    // Characbell
#endif // FEATURE_CHARACTER_BELL

LOCAL BOOL sIsSameFirstName( BYTE* szName, BYTE* szCompareName );
LOCAL void sPBookDisplayScroll(UINT8 hControl, WORD wPos, BOOL fMultiLine,\
							   SCROLL_DATA_E ScrollType);
LOCAL void SKY_API sPBookSendCall(BYTE* szTel);  // 특정 번호로 전화연결
LOCAL void SKY_API sPBookMakeCall(HCONTROL hControl);   // 대표번호로 전화 연결
LOCAL BOOK_CONTROL_E sGetCurrentControlType(UINT8 nGroup, UINT8 nNumber, \
                                            UINT8 nEmail, UINT8 nMemo, \
                                            UINT8 nAnniv, UINT8 nPicture);
// 저장 & 편집 
LOCAL INT16 sSavePhoneBook(UINT8 hList,UINT8 nGroup, UINT8 nNumber, \
                          UINT8 nEmail, UINT8 nMemo, UINT8 nAnniv, DWORD dwPic);
LOCAL HCONTROL sMakeScrollList(COORD x, COORD y, UINT8 nChar, UINT8 nLines, \
                            UINT8 nInitIndex, UINT8 uData, WORD wData, \
                            SCROLL_DATA_E Type);
LOCAL CHAR* sAllocBuffer(INT16 size);
//LOCAL void sFreeBuffer(CHAR* pBuffer);
LOCAL BYTE* sIndexBuffer(BYTE* pBuffer, INT16 index);
// 편집시 항목삭제
LOCAL void sDeleteEditControl(UINT8 nDeletePosition, UINT8 nDeleteNum);
LOCAL HCONTROL sAddInputControl(UINT8 nInsertPosition, UINT8 nViewLine,\
								BOOK_CONTROL_E Type, BOOL fFocus);

LOCAL INT8 sGetScrollLine(INT8 Handle, UINT8 nLineA);
LOCAL void sMakeFormattedString(BYTE* pszBuffer, INT8 hList, \
                                WORD nIndex, SCROLL_DATA_E Type);
LOCAL HCONTROL  sInitScrollList(COORD x, COORD y, UINT8 nChar, UINT8 nLine, \
                                UINT8 nInitIndex, UINT8 HList, \
                                SCROLL_DATA_E Type);

LOCAL void SKY_API sSKA_AccessSecretGroup(SKY_EVENT_T *pEvent);

LOCAL BOOL sMake_Anniv_Icon(HCONTROL hControl, UINT8  nAnnivType );
LOCAL void sAdjustBookContent(BOOK_CONTROL_E ControlType);

LOCAL void SKY_API sSKA_GetString(SKY_EVENT_T *pEvent);

LOCAL VOID SKY_API sSKA_InitPBookRoutine(SKY_EVENT_T* pEvent);
BOOL IsValidImage(DWORD dwIndex);

LOCAL VOID SKY_API sSKA_ViewTelNumber(SKY_EVENT_T* pEvent);

LOCAL VOID SKY_API sSKA_SelectEmailID(SKY_EVENT_T* pEvent);

LOCAL void SKY_API sSKA_EditImageViewArea(SKY_EVENT_T *pEvent);

#ifdef FEATURE_CHARACTER_BELL // 착신번호 연동을 위해 필요
VOID SKY_API SKA_CBSearchNumber(SKY_EVENT_T* pEvent);
VOID SKA_CBViewBookList(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_CBViewBookContent(SKY_EVENT_T* pEvent);
#endif // FEATURE_CHARACTER_BELL

//phone book main applets

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_BookDefaultProcess()
**
** Description: 
**     @DesciptionAboutThisFileHere
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
extern BOOL IsEarpieceEnable(void);

void SKY_API SKA_BookDefaultProcess(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
 
    case SE_PIN_UP:
        EQS_END_CURRENT_EVENT();
        break;
    case SE_KEY_END:
    case SE_IO_TO_IDLE:
		if(l_fFromCall)
		{															
			EQS_END_CURRENT_EVENT();
			
		}
		else
		{
			EQS_END_TO(EQS_POP_TO_ROOT);
		}
		break;
    case SE_KEY_CLEAR:
        EQS_END();
        break;
    case SE_IO_MESGBOX_CLEAR:
		break;
    case SE_IO_MESGBOX_END:
    case SE_IO_POPUP_SELECT_CANCEL:
    case SE_IO_SCROLL_LIST_CANCEL:
		if( pEvent->dwParam != EQC_DELETED )
		{
			EQS_END();
		}
        break;

    case SE_SYS_TO_IDLE:
		//if(SKY_IsInCall() || SKY_IS_IN_WAP() )
		if( l_fFromCall )
		{
			EQS_END_EVENT( SE_KEY_PHONE_CLOSE, EQS_NA, EQS_NA );
		}		
		else
		{
			SKY_DefEventProc(pEvent);
		}
		break;
    case SE_KEY_PHONE_CLOSE:	
		if( l_fFromCall )
		{															
			EQS_END_CURRENT_EVENT();			
		}
		else
		{
			SKY_DefEventProc(pEvent);
		}	  
		break;
    case SE_KEY_MANNERS:
		break;
    case SE_KEY_EAR_LONG_CLICK:	
		if(SKY_IsInCall() || SKY_IS_IN_WAP())
			EQS_END_CURRENT_EVENT();
		else
			SKY_DefEventProc(pEvent);
		break;
	case SE_CALL_ENDED:
		EQS_END_CURRENT_EVENT();
		break;

	case SE_CALL_INCOMING:        
#ifdef FEATURE_WAP
		if( SKY_IS_IN_WAP() && !SKY_GET_DATACALL()) //WAP Dormant이면서 Only Voice call일 경우만 !!
		{
			EQS_END_CURRENT_EVENT();
		}
        else
#endif //#ifdef FEATURE_WAP
        {
            SKY_DefEventProc(pEvent);
        }
		break;

    case SE_SMS_MESSAGE:
#ifdef FEATURE_WAP
        if(SKY_IS_IN_WAP())
        {
            EQS_END_CURRENT_EVENT();        
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();
        }
#else   //#ifdef FEATURE_WAP
        EQS_PASS_CURRENT_EVENT();
#endif  //#ifdef FEATURE_WAP   
        break;

    default :
        SKY_DefEventProc(pEvent);
        break;
    }
}

BOOL SKY_API FindNumberInBook(BYTE*  pszNumber)
{
    UINT8  Handle;
    if( pszNumber == NULL )
    {
        return FALSE;
    }

    if( STRLEN(pszNumber) == 0 )
    {
        EQS_START_CHILD(SKA_SearchBookNumber);
    }    
    else
    {
        Handle = SearchNumber(pszNumber,BSM_NUMBER_PART_MATCH);
        if( Handle == 0 )  // 찾는 번호 없음 메시지 출력후 상위로 이동 
        {
            return FALSE;
        }
        else
        {
            SortBookName(Handle);
            EQS_START_CHILD_EVENT( SKA_ViewSearchNumber, SE_APP_START,\
                                   (WORD)Handle, BFT_SEARCH_BOOK_E);
        }
    }
    return TRUE;

}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_SearchBookNumber()
**
** Description: 
**     @DesciptionAboutThisFileHere
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
void SKY_API SKA_SearchBookNumber(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_START_CHILD_EVENT( SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_GET_GROUP_TEL_E);//BFT_SEARCH_BOOK_E);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
                    
    case SE_APP_RESUME: /* child applet is ended */
        EQS_END_EVENT( SE_APP_RESUME, EQS_NA, pEvent->dwParam);
        break;        
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}


void SKY_API SKA_GetSMSGroupTelNumber(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_START_CHILD_EVENT( SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_GET_GROUP_TEL_E);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
                    
    case SE_APP_RESUME: /* child applet is ended */
        EQS_END_EVENT( SE_APP_RESUME, EQS_NA, pEvent->dwParam);
        break;        
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_BookInCalling()
**
** Description: 
**     @DesciptionAboutThisFileHere
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
void SKY_API SKA_BookInCalling(SKY_EVENT_T *pEvent)
{
    HCONTROL  hControl;
    BYTE*     pToken[6];

#ifdef FEATURE_WAP
    extern BOOL g_fWAP_RET_CALL;
#endif //#ifdef FEATURE_WAP   

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		l_fFromCall = TRUE;
#ifdef FEATURE_WAP
		if( SKY_IS_IN_WAP() == FALSE || g_fWAP_RET_CALL == TRUE )
#endif //#ifdef FEATURE_WAP
		{
			EQS_SetWindowSymbolMode(WTSM_CALL_MODE);
		}
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_NONE,
                      SKY_GET_TOKEN(TKN_PHONE_BOOK_TITLE), 
                      WA_SHOW_HEADER|WA_SHOW_CML);

        pToken[0] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU1);    //SPEED찾기
        pToken[1] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU2);    //이름으로찾기
        pToken[2] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU3);    //전화번호로찾기
        pToken[3] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU4);    //전화부저장
        //pToken[4] = SKY_GET_TOKEN(TKN_PBOOK_EDIT);          //전화부편집
        pToken[4] = SKY_GET_TOKEN(TKN_PBOOK_PERSON_INFO);   //자기정보
        hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,5,5,0);
        EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
                                 CS_SCROLL_LIST_ENUMBERATED |\
                                 CS_SCROLL_LIST_MAXIMIZE);
		EQC_SetScrollAccelKey(hControl, 4, ACCEL_KEY_POUND);

		EQS_SetDirectDraw(FALSE);
        
        EQS_SetIOCActive(hControl);
        break;
        
    case SE_APP_END :   /* The very last event */ 
		l_fFromCall = FALSE;
		if( SKY_IS_IN_WAP() == FALSE )
		{
			EQS_SetWindowSymbolMode(WTSM_NORMAL_MODE);
		}
		//if( SKY_IS_IN_WAP() )
		//	EQS_SetDirectDraw(TRUE);
        break;
                
    case SE_APP_RESUME: /* child applet is ended */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        switch(pEvent->dwParam)
        {
        case 0 :  //SPEED찾기
            EQS_START_CHILD(SKA_SpeedSearch);
            break;
        case 1 :  //이름으로찾기
            EQS_START_CHILD(SKA_SearchName);
            break;
        case 2 :  //전화번호로찾기
            EQS_START_CHILD(SKA_SearchNumber);
            break;
        case 3 :  //전화부저장
            EQS_START_CHILD(SKA_BookInput);
            break;
/*
        case 4 :  //전화부편집		
            EQS_START_CHILD_EVENT(SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_EDIT_PBOOK_E);
            break;
*/
        case 4 :  //자기정보
            EQS_START_CHILD(SKA_MainSelfInfoCode4View);     //white
            break;
        default:
            break;
        }
        break;

	case SE_KEY_END:
	case SE_IO_TO_IDLE:
			if( SKY_IS_IN_WAP()  )
			{
#ifdef FEATURE_WAP
				//EQS_END_EVENT(SE_APP_RESUME, EQS_NA, WAP_EXIT_AFTER_ENDKEY);
                EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
#else//#ifdef FEATURE_WAP
				;
#endif//#ifdef FEATURE_WAP
			}
			else
			{
				EQS_END_CURRENT_EVENT();
			}				
			break;   
	case SE_KEY_PHONE_CLOSE:
		if( SKY_IS_IN_WAP()  )		  
		{
#ifdef FEATURE_WAP
			//EQS_END_EVENT(SE_APP_RESUME, EQS_NA, WAP_EXIT_AFTER_ENDKEY);
            EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
#else //FEATURE_WAP
				;
#endif //FEATURE_WAP				  
		}
		else
		{
			EQS_END_CURRENT_EVENT();
		}
/*
		else if(SKY_IsInCall() )
		{	  

		  if(SKY_IS_EAR_JACK_OFF() && SKY_IsInCall())
		  {
			EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
		  }
		  else 
		  {
			  SKY_DefEventProc(pEvent);
		  }
		  
		}
*/
		break;
#ifdef INCOMING_SMS_IN_WAP
	case SE_SMS_MESSAGE:
        if( SKY_IS_IN_WAP())
        {
            EQS_END_CURRENT_EVENT();
        }
        break;
#endif //INCOMING_SMS_IN_WAP

    case SE_IO_MESGBOX_END:
		break;
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}

void SKY_API SKA_EditBook(SKY_EVENT_T *pEvent)
{

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_START_SIBLING_EVENT(SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_EDIT_PBOOK_E, FALSE);
		break;
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}

void SKY_API SKA_ViewIRDAList(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, pEvent->wParam , BFT_VIEW_IRDA_E);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_END_EVENT(SE_APP_RESUME, EQS_NA, pEvent->dwParam);
      
        break;        
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}

void SKY_API SKA_ViewIRDAGroup(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_START_CHILD_EVENT( SKA_ViewGroupList, SE_APP_START, EQS_NA , BFT_VIEW_IRDA_GROUP_E);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_END_EVENT(SE_APP_RESUME, EQS_NA, pEvent->dwParam);
      
        break;        
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}


void SKY_API SKA_ViewEmailList(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_START_CHILD_EVENT( SKA_ViewGroupList, SE_APP_START, EQS_NA , BFT_VIEW_EMAIL_LIST_E);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_END_EVENT(SE_APP_RESUME, EQS_NA, pEvent->dwParam);
      
        break;        
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}

void SKY_API SaveOnNewBook(BYTE* szNumber, BOOL fLock)
{

    STRCPY(l_szParam,szNumber);
    if(!fLock)
    {
        EQS_START_CHILD_EVENT(SKA_BookInput, SE_APP_START,\
                            0, BFT_SAVE_NEW_DATA_E);
    }else
    {

        if(SKY_IS_PHONE_BOOK_LOCK())
            EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START,\
                                BOOKCODE_NEW_IN_LIST, 0);
        else
            EQS_START_CHILD_EVENT(SKA_BookInput, SE_APP_START,\
                                0, BFT_SAVE_NEW_DATA_E);
    }

}

void SKY_API SaveOnOldBook(BYTE* szNumber, BOOL fLock)
{
    STRCPY(l_szParam,szNumber);
    if(!fLock)
    {
        EQS_START_CHILD_EVENT(SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_SAVE_OLD_DATA_E);
    }else
    {
        if(SKY_IS_PHONE_BOOK_LOCK())
	        EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, BOOKCODE_OLD_IN_LIST, 0);
        else
	        EQS_START_CHILD_EVENT(SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_SAVE_OLD_DATA_E);
    }
/*
    EQS_START_SIBLING_EVENT(SKA_ViewGroupList, SE_APP_START,\
                            0, BFT_SAVE_OLD_DATA_E, FALSE);
*/
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_ViewPersonInfo()
**
** Description: 
**     @DesciptionAboutThisFileHere
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
enum tagEDIT_PERSON_INFO_E      // CONTROL for Edit Group Control
{
    EPI_NAME_TEXT = 0,
    EPI_NAME_EDIT,
    EPI_BIRTH_TEXT,
    EPI_BIRTH_EDIT,
    EPI_MOBILE_TEXT,
    EPI_MOBILE_EDIT,
    EPI_HOME_TEXT,
    EPI_HOME_EDIT,
	EPI_EMAIL_TEXT,
	EPI_EMAIL_EDIT,
    EPI_ADDR_TEXT,
    EPI_ADDR_EDIT,
	EPI_CAMERA_TEXT,
	EPI_CAMERA_IMAGE
};
typedef enum tagEDIT_PERSON_INFO_E  EDIT_PERSON_INFO_E;

#define MOVE_UNIT   1

LOCAL void SKY_API sSKA_EditImageViewArea(SKY_EVENT_T *pEvent)
{
	static RECT  s_ViewRect;

    HCONTROL hControl;
	BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
	HFILE hFile;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE, 
            SKY_GET_TOKEN(TKN_PBOOK_RESIZE_PICTURE),
            WA_SHOW_HEADER|WA_SHOW_CML);


		hControl = EQC_Image(GET_IMG_X_IN_IDLE(l_PBookBitmapInfo),\
			                 GET_IMG_Y_IN_IDLE(l_PBookBitmapInfo), &l_PBookBitmapInfo);

		//hControl = EQC_Image(0,\
			                 0, &l_PBookBitmapInfo);

		s_ViewRect.xLeft   = 0;
		s_ViewRect.yTop    = l_PBookBitmapInfo.Header.yStart;
		s_ViewRect.xRight  = l_PBookBitmapInfo.Header.Size.xWidth-1;
		s_ViewRect.yBottom = l_PBookBitmapInfo.Header.Size.yHeight - l_PBookBitmapInfo.Header.yStart -1;
		EQC_SetImageViewArea(hControl, &s_ViewRect);

		s_ViewRect.xLeft  = pEvent->wParam  + EQC_GET_X(hControl);
		s_ViewRect.yTop   = pEvent->dwParam + EQC_GET_Y(hControl);
		//if( EQS_GET_PARENT() == SKA_ViewPersonInfo )
		{
			s_ViewRect.xRight = s_ViewRect.xLeft + VIEW_PHOTO_IMAGE_WIDTH -1;
			s_ViewRect.yBottom = s_ViewRect.yTop + VIEW_PHOTO_IMAGE_HEIGHT-1;
		}
#if 0
		else
		{
			s_ViewRect.xRight = s_ViewRect.xLeft + PHOTO_IMAGE_WIDTH -1;
			s_ViewRect.yBottom = s_ViewRect.yTop + PHOTO_IMAGE_HEIGHT-1;
		}
#endif
		EQS_FitCanvasSize();
		
	
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
	case SE_APP_DRAW:
		{
			COLOR Color;		
			Color = EQS_GetForeColor();
			EQS_SetForeColor(1);
			EQS_Rectangle( s_ViewRect.xLeft, s_ViewRect.yTop, s_ViewRect.xRight, s_ViewRect.yBottom );
			EQS_SetForeColor(Color);
			
		}
		break;
    case SE_KEY_UP:
		hControl = EQS_GetNthIOC(0);
		if( (s_ViewRect.yTop - MOVE_UNIT) >= EQC_GET_Y(hControl) )
		{
			s_ViewRect.yTop    -= MOVE_UNIT;
			s_ViewRect.yBottom -= MOVE_UNIT;
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
	case SE_KEY_DOWN:
		hControl = EQS_GetNthIOC(0);
		if( (s_ViewRect.yBottom+MOVE_UNIT) <= EQC_GET_Y(hControl)+EQC_GET_DY(hControl) )
		{
			s_ViewRect.yTop    += MOVE_UNIT;
			s_ViewRect.yBottom += MOVE_UNIT;
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
	case SE_KEY_LEFT:
		hControl = EQS_GetNthIOC(0);
		if( s_ViewRect.xLeft - MOVE_UNIT >= EQC_GET_X(hControl) )
		{
			s_ViewRect.xLeft  -= MOVE_UNIT;
			s_ViewRect.xRight -= MOVE_UNIT;
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
	case SE_KEY_RIGHT:
		hControl = EQS_GetNthIOC(0);
		if( s_ViewRect.xRight + MOVE_UNIT <= EQC_GET_X(hControl)+EQC_GET_DX(hControl) )
		{
			s_ViewRect.xLeft  += MOVE_UNIT;
			s_ViewRect.xRight += MOVE_UNIT;
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
	case SE_KEY_CENTER:
	case SE_SOFTKEY_SELECT:

		hControl = EQS_GetNthIOC(0);
		s_ViewRect.yTop  -= EQC_GET_Y(hControl);
		s_ViewRect.xLeft -= EQC_GET_X(hControl);
		EQS_END_EVENT( SE_APP_RESUME, s_ViewRect.xLeft+1, s_ViewRect.yTop+1);
		break;
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
	

}



void SKY_API SKA_ViewPersonInfo(SKY_EVENT_T *pEvent)
{
    HCONTROL            hControl, hTemp;
    pb_info_type        PersonalInfo;
    COORD               StartX, StartY, StartX1;
	DWORD               dwPicture;
	static UINT8        XPos, YPos;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        {
            MARGIN_T Margin;
            COORD    xSpace, ySpace;
			BYTE     szBuf[5];

			XPos = YPos = 0;
            EQS_StartFormatter();   
            EQS_GetControlMargin(&Margin);
            EQS_GetControlSpace(&xSpace, &ySpace);
            EQS_EndFormatter();

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                400, 
                CL_MENU_SELECT_SAVE,
                SKY_GET_TOKEN(TKN_PBOOK_PERSON_INFO), 
                WA_SHOW_HEADER | WA_SHOW_CML);

            StartX = Margin.xLeft;
            StartY = Margin.yTop + ySpace;
			xSpace = 2;

			memset(&PersonalInfo, 0, sizeof(PersonalInfo));
            ui_get_pb(PB_INFO_I, &PersonalInfo);
 
            hControl = EQC_StaticText(StartX,StartY,TKN_PBOOK_ITEM_NAME);
			sprintf((char*)szBuf, "%s ", (char*) SF_NAME_TEXT);
			EQC_SetStaticText(hControl, szBuf);
			StartX1 = StartX + EQC_GET_DX(hControl) + xSpace;
            hControl = EQC_TextEdit(StartX1, StartY, PERSON_INFO_WIDTH, 1, \
                                    TEM_KOREAN, MAX_BOOK_NAME-2,(BYTE*)"", 0);    //이름
            EQC_SetTextEditData( hControl, PersonalInfo.Name);

            StartY += EQC_GET_DY(hControl) + ySpace;                            //생일
            hControl = EQC_StaticText(StartX,StartY,TKN_PBOOK_ANNIV_MENU1);            
			sprintf((char*)szBuf, "%s ", (char*) SF_CAKE);
			EQC_SetStaticText(hControl, szBuf);		
            hControl = EQC_Time(StartX1, StartY,  2000, INFORM_TIME_CONTROL);
			EQC_SET_STYLE(hControl,CS_TIME_BORDER);			      
            EQC_SetTime(hControl,PersonalInfo.wYear, PersonalInfo.nMonth, 0,\
                                 PersonalInfo.nDay, 0, 0);
			EQC_SetTimeLunar(hControl, PersonalInfo.fLunar);
			EQC_SET_DX(hControl, EQC_GET_DX(EQS_GetNthIOC(EPI_NAME_EDIT)) );


			StartY += EQC_GET_DY(hControl) + ySpace;                            // 이동전화 번호
            hControl = EQC_StaticText(StartX,StartY,TKN_PBOOK_ITEM_MOBILE); 			
			sprintf((char*)szBuf, "%s ", (char*) SF_CELLULAR);
			EQC_SetStaticText(hControl, szBuf);			
            hControl = EQC_NumberEdit(StartX1, StartY, PERSON_INFO_WIDTH, 1,\
                                      MAX_TEL_LEN, "" );
            EQC_SetNumberEditValue(hControl, PersonalInfo.MobileTel);
            //EQC_SET_FOCUS(hControl, FALSE);
			EQC_SET_ENABLE(hControl, FALSE);
#if 0
            hControl = EQC_TextEdit(StartX1, StartY, 14, 1,\
                                    TEM_KOREAN, MAX_TEL_LEN,(BYTE*)PersonalInfo.MobileTel, 0);   
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_TEXT_EDIT_READONLY );            
            EQC_SET_FOCUS(hControl, FALSE);
			EQC_SET_ENABLE(hControl, FALSE);
#endif
			
            StartY += EQC_GET_DY(hControl) + ySpace;       // 집전화번호 
            hControl = EQC_StaticText(StartX,StartY,TKN_PBOOK_ITEM_HOME);            
			//sprintf((char*)szBuf, "%s ", (char*) SF_HOME);
			sprintf((char*)szBuf, "%s ", (char*)SF_REPLY_NUMBER1_4);
			EQC_SetStaticText(hControl, szBuf);
            hControl = EQC_NumberEdit(StartX1, StartY, PERSON_INFO_WIDTH, 1,\
                                      MAX_TEL_LEN, "" );
            EQC_SetNumberEditValue(hControl, PersonalInfo.HomeTel);

            StartY += EQC_GET_DY(hControl) + ySpace;       // EMAIL
            hControl = EQC_StaticText(StartX,StartY,TKN_PBOOK_ITEM_HOME);            			
			sprintf((char*)szBuf, "%s ", (char*)SF_EMAIL);
			EQC_SetStaticText(hControl, szBuf);
			hControl = EQC_TextEdit(StartX1, StartY, PERSON_INFO_WIDTH, \
                                    3, TEM_ENGLISH_LOWER,PB_MAX_STRING_LENGTH,(BYTE*)"", 0);  
            EQC_SetTextEditData( hControl, PersonalInfo.Email);


            StartY += EQC_GET_DY(hControl) + ySpace;       // 주소 
            hControl = EQC_StaticText(StartX, StartY, TKN_PBOOK_ITEM_ADDR);            
			sprintf((char*)szBuf, "%s ", (char*) SF_ADDRESS);
			EQC_SetStaticText(hControl, szBuf);
            hControl = EQC_TextEdit(StartX1, StartY, PERSON_INFO_WIDTH, \
                                    4, TEM_KOREAN, MAX_ADDRESS_LEN,(BYTE*)"", 0);
            EQC_SetTextEditData( hControl, PersonalInfo.Address);

			StartY += EQC_GET_DY(hControl) + ySpace + ySpace;
			
			hControl = EQC_StaticText(StartX, StartY, TKN_PBOOK_ITEM_MEMO);
			sprintf((char*)szBuf,"%s ",(char*)SF_CAMERA);
			EQC_SetStaticText(hControl, szBuf);
			
			hControl = EQC_PushButton(StartX1, StartY,TKN_PBOOK_NO_PICTURE);
			EQC_SetPushButtonCaption( hControl, SKY_GET_TOKEN(TKN_PBOOK_NO_PICTURE),\
						              PERSON_INFO_WIDTH);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_PUSH_BUTTON_CENTER );

			if( PersonalInfo.dwPictureIndex )
			{
				BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
				HFILE hFile;         
				
				dwPicture =  PersonalInfo.dwPictureIndex;
				ConvertCameraImageFileName(szFileName, dwPicture);
 
#if VIEW_PHOTO_IMAGE
				if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
				{
					DWORD dwTimeInfo;
					RECT  ViewArea;
					int x,y;
					SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
					SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
					SKY_ReadDataBlock(hFile, &l_PBookBitmapInfo.Header, sizeof(l_PBookBitmapInfo.Header));  
					SKY_ReadDataBlock(hFile, g_abBmpBuf, GET_IMAGE_BYTE_SIZE(l_PBookBitmapInfo.Header));   
					l_PBookBitmapInfo.pbData = g_abBmpBuf;  
					SKY_CloseDataFile(hFile); 
					x = EQC_GET_X(hControl);
					y = EQC_GET_Y(hControl);
					EQS_DeleteControl(hControl);

					if( l_PBookBitmapInfo.Header.yStart != PHOTO_START_Y )  //01.06.04
						l_PBookBitmapInfo.Header.yStart =  PHOTO_START_Y;

					hControl = EQC_Image(x,y, &l_PBookBitmapInfo);					
					EQC_SET_STYLE(hControl, CS_IMAGE_BORDER );
					XPos = PersonalInfo.XPos;
					YPos = PersonalInfo.YPos;
					if( XPos + VIEW_PHOTO_IMAGE_WIDTH  > EQC_GET_DX(hControl) ||\
						YPos + VIEW_PHOTO_IMAGE_HEIGHT > EQC_GET_DY(hControl)    )
					{
						XPos = (l_PBookBitmapInfo.Header.Size.xWidth - VIEW_PHOTO_IMAGE_WIDTH)/2;
						YPos = ( l_PBookBitmapInfo.Header.Size.yHeight-l_PBookBitmapInfo.Header.yStart -\
							     VIEW_PHOTO_IMAGE_HEIGHT)/2;
					}
										
					ViewArea.xLeft = XPos;
					ViewArea.xRight= ViewArea.xLeft+VIEW_PHOTO_IMAGE_WIDTH-1;
					ViewArea.yTop  = YPos + l_PBookBitmapInfo.Header.yStart;
					ViewArea.yBottom = ViewArea.yTop+VIEW_PHOTO_IMAGE_HEIGHT-1;

					EQC_SetImageViewArea(hControl, &ViewArea);
					EQC_SET_USERDATA(hControl, dwPicture );
					EQC_SET_FOCUS(hControl,TRUE);
				}				
#else
				if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
				{
					SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
					SKY_CloseDataFile(hFile);                    					
					EQC_SetPushButtonCaption( hControl, szFileName, INPUT_CONTROL_WIDTH);
					EQC_SET_USERDATA(hControl, dwPicture );
                }
#endif
				else
				{
					PersonalInfo.dwPictureIndex = 0;
					ui_put_pb( PB_INFO_I, &PersonalInfo);
				}
			}				
			EQS_SetIOCFocus( EQS_GetNthIOC(EPI_NAME_EDIT) );
            EQS_FitCanvasSize();
        }                
        break;  
        
    case SE_APP_END :   /* The very last event */ 
        break;   
		
    case SE_APP_RESUME:
        if( EQS_GET_CHILD() == SKA_PhotoListNView )
        {        
			hControl  = EQS_GetNthIOC(EPI_CAMERA_IMAGE);
			if( pEvent->dwParam )
			{
#if VIEW_PHOTO_IMAGE
				BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
				HFILE hFile;
				dwPicture = pEvent->dwParam;
				ConvertCameraImageFileName(szFileName, dwPicture);
				if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
				{
					DWORD dwTimeInfo;
					RECT  ViewArea;
					int x,y;
					SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
					SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
					SKY_ReadDataBlock(hFile, &l_PBookBitmapInfo.Header, sizeof(l_PBookBitmapInfo.Header));  
					SKY_ReadDataBlock(hFile, g_abBmpBuf, GET_IMAGE_BYTE_SIZE(l_PBookBitmapInfo.Header));   
					l_PBookBitmapInfo.pbData = g_abBmpBuf;  
					SKY_CloseDataFile(hFile); 
					x = EQC_GET_X(hControl);
					y = EQC_GET_Y(hControl);
					EQS_DeleteControl(hControl);

					if( l_PBookBitmapInfo.Header.yStart != PHOTO_START_Y )  //01.06.04
						l_PBookBitmapInfo.Header.yStart = PHOTO_START_Y;
					hControl = EQC_Image(x,y, &l_PBookBitmapInfo);
					EQC_SET_STYLE(hControl, CS_IMAGE_BORDER );
					XPos = (l_PBookBitmapInfo.Header.Size.xWidth - VIEW_PHOTO_IMAGE_WIDTH)/2;
					YPos = ( l_PBookBitmapInfo.Header.Size.yHeight-l_PBookBitmapInfo.Header.yStart -\
						     PHOTO_IMAGE_HEIGHT)/2;

					ViewArea.xLeft = XPos;
					ViewArea.xRight= ViewArea.xLeft+VIEW_PHOTO_IMAGE_WIDTH-1;
					ViewArea.yTop  = YPos + l_PBookBitmapInfo.Header.yStart;
					ViewArea.yBottom = ViewArea.yTop+VIEW_PHOTO_IMAGE_HEIGHT-1;


					EQC_SetImageViewArea(hControl, &ViewArea);
					EQC_SET_USERDATA(hControl, dwPicture );
					EQC_SET_FOCUS(hControl,TRUE);
					EQS_FitCanvasSize();
				}
#else
				BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
				HFILE hFile;         
				dwPicture = pEvent->dwParam;
				ConvertCameraImageFileName(szFileName, dwPicture);
				if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
				{
					SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
					SKY_CloseDataFile(hFile);					
					EQC_SetPushButtonCaption( hControl, szFileName, INPUT_CONTROL_WIDTH);
					EQC_SET_USERDATA(hControl, dwPicture);					
				}
#endif
				EQC_SET_DIRTY(hControl, TRUE);
			}			
			EQS_SetIOCFocus(hControl);			
        }
		else if( EQS_GET_CHILD() == sSKA_EditImageViewArea )
		{
			if( pEvent->wParam != 0 && pEvent->dwParam != 0 )
			{
				RECT  ViewArea;

				hControl  = EQS_GetNthIOC(EPI_CAMERA_IMAGE);
				XPos = pEvent->wParam-1;
				YPos = pEvent->dwParam-1;

				ViewArea.xLeft   = XPos;
				ViewArea.xRight  = ViewArea.xLeft+VIEW_PHOTO_IMAGE_WIDTH-1;
				ViewArea.yTop    = l_PBookBitmapInfo.Header.yStart + YPos;
				ViewArea.yBottom = ViewArea.yTop+VIEW_PHOTO_IMAGE_HEIGHT-1;
				EQC_SetImageViewArea(hControl, &ViewArea);

			}
		}
		else if( EQS_GET_CHILD() == SKA_PhotoViewer )
		{
			EQS_SetIOCFocus( EQS_GetNthIOC(EPI_CAMERA_IMAGE) );
		}
		else
		{
			;
		}
		break;


    case SE_SOFTKEY_MENU:
		{
			SKY_TOKEN paMenu[4];
			BOOL      faActiveMenu[4];
			UINT8 nPaMenu = 0;

			hControl  = EQS_GetNthIOC(EPI_CAMERA_IMAGE);
			dwPicture = EQC_GET_USERDATA(hControl);

			paMenu[0] = TKN_PBOOK_ADD_PICTURE;
			paMenu[1] = TKN_PBOOK_EDIT_PICTURE;
			paMenu[2] = TKN_PBOOK_RESIZE_PICTURE;
			paMenu[3] = TKN_PBOOK_DELETE_PICTURE_ITEM;
			for( nPaMenu = 0; nPaMenu < 4; nPaMenu++)
			{
				faActiveMenu[nPaMenu] = TRUE;
			}

			if( dwPicture )
			{
				faActiveMenu[0] = FALSE;						   
			}
			else
			{
				faActiveMenu[1] = FALSE;
				faActiveMenu[2] = FALSE;
				faActiveMenu[3] = FALSE;
			}
		
/*			
			if( SKY_IS_IN_WAP() )
			{
				faActiveMenu[0] = FALSE;
				faActiveMenu[1] = FALSE;				
			}	
*/

			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 4, 0, faActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
		}        
        break;
	case SE_APP_SLIDE_MOVE:
		//EQS_SetWindowCML(CL_MENU_NONE_SAVE);
		break;
    case SE_IO_FOCUS_CHG:
		if( pEvent->wParam == EQS_GetNthIOC(EPI_MOBILE_EDIT) )
		{
			EQS_SetWindowCML(CL_MENU_NONE_SAVE);
		}
		else
		{
			EQS_SetWindowCML(CL_MENU_SELECT_SAVE);
		}
		break;
    case SE_IO_POPUP_SELECT: 
		hControl = EQS_GetNthIOC(EPI_CAMERA_IMAGE);
		EQC_SET_DIRTY(hControl, TRUE);
        switch( pEvent->dwParam )
        {
		case TKN_PBOOK_ADD_PICTURE:
			if( SKY_IS_IN_WAP() )
			{
				EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
							   MESSAGE_DEFAULT_TIME,\
							   MBT_NONE, MBT_HELP);
			}
			else
			{
				EQS_START_CHILD( SKA_PhotoListNView );
			}
			break;

		case TKN_PBOOK_EDIT_PICTURE:
			if( SKY_IS_IN_WAP() )
			{
				EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
							   MESSAGE_DEFAULT_TIME,\
							   MBT_NONE, MBT_HELP);
			}
			else
			{
				EQS_START_CHILD( SKA_PhotoListNView );
			}
			break;

		case TKN_PBOOK_RESIZE_PICTURE:
			EQS_START_CHILD_EVENT( sSKA_EditImageViewArea, SE_APP_START,\
				                   XPos, YPos );
			break;
		case TKN_PBOOK_DELETE_PICTURE_ITEM:	
#if VIEW_PHOTO_IMAGE
            
			hTemp = EQC_PushButton(EQC_GET_X(hControl), EQC_GET_Y(hControl),TKN_PBOOK_NO_PICTURE);
			EQC_SetPushButtonCaption( hTemp, SKY_GET_TOKEN(TKN_PBOOK_NO_PICTURE),\
						              INPUT_CONTROL_WIDTH);
			EQC_SET_STYLE(hTemp, EQC_GET_STYLE(hTemp) | CS_PUSH_BUTTON_CENTER );
			EQS_DeleteControl(hControl);
			EQC_SET_DIRTY(hTemp, TRUE);
			EQS_SetIOCFocus(hTemp);
			EQS_FitCanvasSize();
#else
			EQC_SET_USERDATA(hControl, EQS_NA);
			EQC_SetPushButtonCaption( hControl, SKY_GET_TOKEN(TKN_PBOOK_NO_PICTURE),\
						              INPUT_CONTROL_WIDTH);
			EQS_SetIOCFocus(hControl);
#endif
			break;
		default:
			break;
		}
		break;
    case SE_KEY_SEND:				
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;

		hControl = EQS_GetFocusedIOC();
		if( EQS_GetIOCOrder(hControl) == EPI_HOME_EDIT )
		{
			BYTE szBuf[40];
			INT8 nLen;

			EQC_GetNumberEditValue(hControl, szBuf);
			nLen = STRLEN(szBuf);
			if( nLen > 0 )
			{
				SET_NUMEDIT_BUFFER(szBuf, nLen);
				SET_NUMEDIT_POS(nLen);
				EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				//ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,\
				//	                 GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
			}
		}
		else
		{
			;//EQS_START_CHILD(SKA_BookMain);
		}
        break;
    case SE_SOFTKEY_SAVE:           //자기 정보 저장
		EQS_SEND_EVENT(SE_USER_1, EQS_NA, EQS_NA);
		break;
    case SE_USER_1:  // save information
		memset(&PersonalInfo, 0, sizeof(PersonalInfo));
        EQC_GetTextEditData(EQS_GetNthIOC(EPI_NAME_EDIT), PersonalInfo.Name);
        PersonalInfo.wYear  = EQC_GetTimeYear(EQS_GetNthIOC(EPI_BIRTH_EDIT));
        PersonalInfo.nMonth = EQC_GetTimeMonth(EQS_GetNthIOC(EPI_BIRTH_EDIT));
        PersonalInfo.nDay   = EQC_GetTimeDay(EQS_GetNthIOC(EPI_BIRTH_EDIT));
        PersonalInfo.fLunar = EQC_GetTimeLunar(EQS_GetNthIOC(EPI_BIRTH_EDIT));        
        EQC_GetNumberEditValue(EQS_GetNthIOC(EPI_MOBILE_EDIT), PersonalInfo.MobileTel);
        EQC_GetNumberEditValue(EQS_GetNthIOC(EPI_HOME_EDIT), PersonalInfo.HomeTel);
		EQC_GetTextEditData(EQS_GetNthIOC(EPI_EMAIL_EDIT)  , PersonalInfo.Email);	
        EQC_GetTextEditData(EQS_GetNthIOC(EPI_ADDR_EDIT)   , PersonalInfo.Address);		
		PersonalInfo.dwPictureIndex = EQC_GET_USERDATA(EQS_GetNthIOC(EPI_CAMERA_IMAGE));
		PersonalInfo.XPos = XPos;
		PersonalInfo.YPos = YPos;

        ui_put_pb( PB_INFO_I, &PersonalInfo);

        hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG),\
							      MESSAGE_DEFAULT_TIME,\
								  MBT_NONE, MBT_HELP);
		if( pEvent->dwParam == EQS_NA)
		{
			EQC_SET_USERDATA(hControl, SE_APP_END);
		}
		else
		{
			EQC_SET_USERDATA(hControl, pEvent->dwParam);
		}
        break;
	case SE_KEY_CLEAR:    
		if( EQS_IsControlDataChanged() == FALSE)
		{
			EQS_END();
		}
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_KEY_CLEAR);
		}
        break;		
    case SE_KEY_END:
		if( EQS_IsControlDataChanged() == FALSE)
		{
			EQS_END_CURRENT_EVENT();
		}
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_KEY_END);
		}        
		break;  
    case SE_IO_IMAGE_SELECT:
	case SE_IO_PUSH_BUTTON_SELECT:
		if( SKY_IS_IN_WAP() )
		{
			EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
						   MESSAGE_DEFAULT_TIME,\
						   MBT_NONE, MBT_HELP);
		}
		else
		{
			hControl  = pEvent->wParam;
			dwPicture = EQC_GET_USERDATA(hControl);
			if( dwPicture )
			{   // 사진 보기 
				EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, EQS_NA, dwPicture ); 
			}
			else
			{   // 사진 추가 
				EQS_START_CHILD( SKA_PhotoListNView );
			}
		}
		break;
	case SE_IO_MESGBOX_END:
    case SE_IO_MESGBOX_NO:
		if( pEvent->dwParam == SE_KEY_END )
		{
			EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
		}
		else if( pEvent->dwParam == SE_APP_END || pEvent->dwParam == SE_KEY_CLEAR )
		{
			EQS_END();
		}			
		else ;

		break;
    case SE_IO_MESGBOX_YES:
		EQS_SEND_EVENT( SE_USER_1, EQS_NA, pEvent->dwParam );
		break;
    		
	case SE_KEY_POUND:	
		break;
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}

enum tagBOOK_INPUT_MESSAGE_E
{
	PBOOKINPUT_ASK_SAVING_MSG_E = 1,
	PBOOKINPUT_SAVE_MSG_E
};

//2001.01.03 pEvent->wParam == 1 : 새이름에 저장(초기화면에서 저장을 눌렀을때 
VOID SKY_API SKA_BookInput(SKY_EVENT_T* pEvent)
{
    static INT8  nGroup, nNumber, nMemo, nAnniv,nEmail;
    static DWORD dwPicture;    
	static BOOL   fEndKey;
    static INT16  BookAddress;
	static UINT8  XPos, YPos;   

    BYTE            szString[34];
    HCONTROL        hControl;
    INT8            nIndex, nDeleteNum;
    BOOK_CONTROL_E  ControlType;    

    switch( pEvent->EventID )
    {
    case SE_APP_START:        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            400, 
            CL_MENU_SELECT_SAVE, 
            SKY_GET_TOKEN(TKN_PBOOK_SAVE), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        nGroup  = 0;
        nNumber = 0;
        nMemo   = 0;
        nAnniv  = 0;    
        nEmail  = 0;
		fEndKey = FALSE;
        dwPicture = 0;
		BookAddress = 0;
		l_nBookOverWriteAddress = 0;
		XPos = YPos = 0;

		if( IsFullPBook() == TRUE )
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_MEMORY_MSG),\
									  MESSAGE_DEFAULT_TIME,\
									  MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, SE_APP_END);
		}
		else
		{              
			nIndex = 0;
			sAddInputControl( nIndex, 1,ICT_NAME_E , TRUE);

			nIndex = INITIAL_INPUT_CONTROL;

			hControl = sAddInputControl( nIndex, 1,ICT_GROUP_E ,FALSE);
			if( pEvent->wParam )
			{
				UINT8 ListHandle;
				ListHandle = EQC_GetScrollListHandle(hControl);
				for(nIndex=0; nIndex< GetListSize(ListHandle); nIndex++)
				{
					if( pEvent->wParam == GetListValue( ListHandle, nIndex) )
					{
						EQC_SetScrollLogicalIndex( hControl, nIndex );
						break;
					}
				}
			}
			else
			{
				UINT8  nNotGroup;
				UINT8  ListHandle;

				nNotGroup = SearchGroup(SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6));
				ListHandle = EQC_GetScrollListHandle(hControl);
				for(nIndex=0; nIndex< GetListSize(ListHandle); nIndex++)
				{
					if( nNotGroup == GetListValue( ListHandle, nIndex) )
					{
						EQC_SetScrollLogicalIndex( hControl, nIndex );
						break;
					}
				}

			}
			nGroup++;

			nIndex = INITIAL_INPUT_CONTROL + nGroup*2;
			hControl = sAddInputControl( nIndex, 1, ICT_NUMBER_E ,FALSE);
			if( pEvent->dwParam == BFT_SAVE_NEW_DATA_E ) // 초기화면에서 전화번호를 넘겨받은 경우 
			{
				EQC_SetNumberEditValue(hControl, l_szParam);
				EQC_SET_DIRTY(hControl, TRUE);
			}
			nNumber++;    
			
			nIndex = INITIAL_INPUT_CONTROL + nGroup*2 + nNumber*2;
			hControl = sAddInputControl( nIndex, 1, ICT_NUMBER_E ,FALSE);
			EQC_SetScrollLogicalIndex( EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-1) , HOME - 1 );
			nNumber++;

			sAddInputControl(EQS_GetIOCNum(), 1, ICT_MENU_E ,FALSE);
		}            
        break;
    case SE_APP_END:
        break;
    case SE_APP_RESUME:
        if( EQS_GET_CHILD() == SKA_PhotoListNView && pEvent->dwParam )
        {                           
			BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
			HFILE hFile;         
            dwPicture = pEvent->dwParam;
			ConvertCameraImageFileName(szFileName, dwPicture);
#if VIEW_PHOTO_IMAGE
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				DWORD dwTimeInfo;
				RECT  ViewArea;
				int x,y;
				
                hControl = sAddInputControl(EQS_GetIOCNum()-1, 1, ICT_PICTURE_E, TRUE);

				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
				SKY_ReadDataBlock(hFile, &l_PBookBitmapInfo.Header, sizeof(l_PBookBitmapInfo.Header));  
				SKY_ReadDataBlock(hFile, g_abBmpBuf, GET_IMAGE_BYTE_SIZE(l_PBookBitmapInfo.Header));   
				l_PBookBitmapInfo.pbData = g_abBmpBuf;  
				SKY_CloseDataFile(hFile); 

				x = EQC_GET_X(hControl);
				y = EQC_GET_Y(hControl);
				EQS_DeleteControl(hControl);
				
				if( l_PBookBitmapInfo.Header.yStart != PHOTO_START_Y )  //01.06.04
					l_PBookBitmapInfo.Header.yStart = PHOTO_START_Y;

				hControl = EQC_Image(x,y, &l_PBookBitmapInfo);
				EQC_SET_STYLE(hControl, CS_IMAGE_BORDER );

				y = ( l_PBookBitmapInfo.Header.Size.yHeight-l_PBookBitmapInfo.Header.yStart -\
					  PHOTO_IMAGE_HEIGHT)/2 - l_PBookBitmapInfo.Header.yStart;
				if( y < 0 )
					y = 0;

				XPos = (l_PBookBitmapInfo.Header.Size.xWidth - PHOTO_IMAGE_WIDTH)/2;
				YPos = y;
									
				ViewArea.xLeft = XPos;
				ViewArea.xRight= ViewArea.xLeft+PHOTO_IMAGE_WIDTH-1;
				ViewArea.yTop  = l_PBookBitmapInfo.Header.yStart+YPos;
				ViewArea.yBottom = ViewArea.yTop+PHOTO_IMAGE_HEIGHT-1;

				EQC_SetImageViewArea(hControl, &ViewArea);
				EQC_SET_USERDATA(hControl, dwPicture );
				EQC_SET_FOCUS(hControl,TRUE);
				EQC_SET_Y( EQS_GetNthIOC(EQS_GetIOCNum()-1), EQC_GET_Y(hControl)+EQC_GET_DY(hControl) + 5);
				EQS_SetIOCFocus(hControl);
				EQS_FitCanvasSize();
			}				
#else
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_CloseDataFile(hFile);
				hControl = sAddInputControl(EQS_GetIOCNum()-1, 1, ICT_PICTURE_E, TRUE);
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_PUSH_BUTTON_CENTER );
				EQC_SetPushButtonCaption( hControl, szFileName, INPUT_CONTROL_WIDTH);
				EQS_SetIOCFocus(hControl);
			}
#endif
        }
		else if( EQS_GET_CHILD() == SKA_PhotoListNView )
		{
			EQS_SetIOCFocus( EQS_GetNthIOC(EQS_GetIOCNum()-1) );
		}
		else if( EQS_GET_CHILD() == SKA_PhotoViewer )
		{
			EQS_SetIOCFocus( EQS_GetNthIOC(EQS_GetIOCNum()-2) );
		}
		else if( EQS_GET_CHILD() == sSKA_EditImageViewArea )
		{
			if( pEvent->wParam != 0 && pEvent->dwParam != 0 )
			{
				RECT  ViewArea;

				hControl  = EQS_GetNthIOC(EQS_GetIOCNum()-2);
				XPos = pEvent->wParam  - 1;
				YPos = pEvent->dwParam - 1;

				ViewArea.xLeft   = XPos;
				ViewArea.xRight  = ViewArea.xLeft+PHOTO_IMAGE_WIDTH-1;
				ViewArea.yTop    = l_PBookBitmapInfo.Header.yStart + YPos;
				ViewArea.yBottom = ViewArea.yTop+PHOTO_IMAGE_HEIGHT-1;
				EQC_SetImageViewArea(hControl, &ViewArea);
			}
		}
		else if( EQS_GET_CHILD() == sSKA_GetString )
		{
			if( pEvent->wParam )
			{
				if( STRLEN( l_szParam ) > 0 )
				{
					EQC_SetScrollTitle( pEvent->dwParam, l_szParam );
					//EQC_ShowScrollTitle(pEvent->dwParam, TRUE );
					if( pEvent->wParam == INPUT_ANNIV_NAME_E )
					{
						//nIndex = EQC_GetScrollLogicalIndex( pEvent->dwParam );
						//if( nIndex == (ETC_ANNIV-1) )  // 기념일 내용 추가	
						{
							EQC_SetScrollPhysicalIndex(pEvent->dwParam, ETC_ANNIV-1);
							STRCPY( EQC_GetScrollData(pEvent->dwParam, ETC_ANNIV-1), l_szParam);							
							hControl = EQS_GetNthIOC( EQS_GetIOCOrder(pEvent->dwParam) + 1);
							sMake_Anniv_Icon( hControl, ETC_ANNIV);//nIndex+1 );
						}
					}
				}				
			}
			//EQS_SetIOCActive(EQS_GetFocusedIOC());
		}
        else
            EQS_SetIOCFocus( EQS_GetNthIOC(1) );

        break;
    case SE_SOFTKEY_MENU:
		{					
			SKY_TOKEN paMenu[10];
			BOOL      faActiveMenu[10];
			UINT8 nPopUpMenu=0;
						
			hControl = EQS_GetActiveIOC();
			if( hControl != INVALID_CONTROL_HANDLE )
			{
				if( EQC_GET_TYPE(hControl) == CT_SCROLL_LIST )
				{
					EQC_DeActiveScrollList(hControl);
				}
			}
			
			//paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_ITEM;
			for(nIndex = 0; nIndex < 10; nIndex++)
			{
				faActiveMenu[nIndex] = FALSE;
			}
		        		
			if( nNumber < PERSONAL_NUMBER_LIMIT )
			{
				if( active_list.nFreeNumber >= (nNumber+1) ) //active_list.nFreeBook )
				{
					faActiveMenu[nPopUpMenu] = TRUE;
				}				
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_NUMBER_ITEM;

			if( nEmail < PERSONAL_EMAIL_LIMIT )
			{
				if( active_list.nFreeEMail >= (nEmail+1) )//active_list.nFreeBook )
				{
					faActiveMenu[nPopUpMenu] = TRUE;
				}
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_EMAIL_ITEM;


			if( nMemo < PERSONAL_MEMO_LIMIT )
			{
				faActiveMenu[nPopUpMenu] = TRUE;				
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_MEMO_ITEM;

			if( nAnniv < PERSONAL_ANNIV_LIMIT )
			{
				if( active_list.nFreeAnniv >= (nAnniv+1) )//active_list.nFreeBook )
				{
					faActiveMenu[nPopUpMenu] = TRUE;					
				}
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_ANNIV_ITEM;


			if( dwPicture == 0 )
			{								
				faActiveMenu[nPopUpMenu] = TRUE;
				paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_PICTURE_ITEM;
				ControlType = sGetCurrentControlType(nGroup, nNumber, nEmail,\
													 nMemo, nAnniv, 0);
			}
			else				
			{
				faActiveMenu[nPopUpMenu] = TRUE;
				paMenu[nPopUpMenu++] = TKN_PBOOK_RESIZE_PICTURE;
				ControlType = sGetCurrentControlType(nGroup, nNumber, nEmail,\
													 nMemo, nAnniv, 1);
			}			
						
			if( ControlType == ICT_MENU_E )
			{
				;
			}
			else
			{
				faActiveMenu[nPopUpMenu] = TRUE;
				switch( ControlType )
				{
				case ICT_NAME_E:
					faActiveMenu[nPopUpMenu] = FALSE;
					paMenu[nPopUpMenu++] = TKN_PBOOK_NEW_GROUP;
					break;
				case ICT_GROUP_E:       
					paMenu[nPopUpMenu++] = TKN_PBOOK_NEW_GROUP;
					break;
				case ICT_NUMBER_E:            // 전화번호
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_NUMBER_ITEM;
					break;
				case ICT_EMAIL_E:             // eMail
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_EMAIL_ITEM;
					break;
				case ICT_MEMO_E:              // 메모					
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_MEMO_ITEM;
					break;
				case ICT_ANNIV_E:             // 기념일    
					if( EQC_GET_TYPE(EQS_GetFocusedIOC()) == CT_SCROLL_LIST )
					{
						paMenu[nPopUpMenu++] = TKN_PBOOK_ANNIV_MENU;
					}
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_ANNIV_ITEM;
					break;
				case ICT_PICTURE_E:            
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_PICTURE_ITEM;
					break;
				default:
					break;
				}  				
			}
			
			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, nPopUpMenu, 0, faActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
		}
		break;
    case SE_IO_POPUP_SELECT:                                         	
        switch( pEvent->dwParam )
        {
/*
		case TKN_PBOOK_ADD_ITEM:  // 항목추가 
			hControl = EQS_GetNthIOC(EQS_GetIOCNum()-1);
			EQS_SetIOCActive(hControl);
			break;
*/
		case TKN_PBOOK_ADD_NUMBER_ITEM:
			nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
			hControl = sAddInputControl( nIndex,1 ,ICT_NUMBER_E,TRUE );
#if 0
			if( nNumber < PAGER)  // < 4 
			{
				EQC_SetScrollPhysicalIndex( EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-1), nNumber);
			}
#endif
			nNumber++;
			break;
		case TKN_PBOOK_ADD_EMAIL_ITEM:
            nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail)*2;
            sAddInputControl( nIndex, 2, ICT_EMAIL_E,TRUE );
            nEmail++;
			break;
		case TKN_PBOOK_ADD_MEMO_ITEM:
            nIndex = INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nEmail+nMemo)*2;
            sAddInputControl( nIndex, 3, ICT_MEMO_E ,TRUE);
            nMemo++;
			break;
		case TKN_PBOOK_ADD_ANNIV_ITEM:
			{
                clk_julian_type     julian;
                DWORD               current_time;
                nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail+nMemo)*2 + nAnniv*4;
                hControl = sAddInputControl( nIndex,1, ICT_ANNIV_E,TRUE);
                current_time = SKY_GetLocalTime();
                clk_secs_to_julian(current_time, &julian);
                //EQC_SetTimeYear(hControl, julian.year);
				EQC_SetTimeYear(hControl, 0);
                EQC_SetTimeMonth(hControl,julian.month);
                EQC_SetTimeDay(hControl,  julian.day);
				sMake_Anniv_Icon(EQS_GetNthIOC(EQS_GetIOCOrder(hControl) - 1), 1 ); //생일 
                nAnniv++;	
				//EQS_SetIOCFocus(hControl);
				EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-2));
			}
			break;
		case TKN_PBOOK_ADD_PICTURE_ITEM:
			if( SKY_IS_IN_WAP() )
			{
				EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
							   MESSAGE_DEFAULT_TIME,\
							   MBT_NONE, MBT_HELP);
			}
			else
			{
				EQS_START_CHILD( SKA_PhotoListNView );
			}
			break;
		case TKN_PBOOK_RESIZE_PICTURE:
			EQS_START_CHILD_EVENT( sSKA_EditImageViewArea, SE_APP_START,\
				                   XPos, YPos);
			break;
		case TKN_PBOOK_NEW_GROUP:  // 새그룹 이름 
			if( IsFullGroup() == TRUE )
			{
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_GROUP_MSG),\
										  MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
			}
			else
			{
				l_szParam[0] = 0;
				EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START,\
							          INPUT_GROUP_NAME_E, EQS_GetFocusedIOC());
			}
			break;
		case TKN_PBOOK_ANNIV_MENU: // 기념일 이름 입력 
			l_szParam[0] = 0;						 
			EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START, INPUT_ANNIV_NAME_E, EQS_GetFocusedIOC());
			break;
		case TKN_PBOOK_DELETE_NUMBER_ITEM: // 번호 삭제 
		case TKN_PBOOK_DELETE_EMAIL_ITEM:  // 이메일계정 삭제
		case TKN_PBOOK_DELETE_MEMO_ITEM:   // 메모 삭제 
		case TKN_PBOOK_DELETE_ANNIV_ITEM:  // 기념일 삭제 
		case TKN_PBOOK_DELETE_PICTURE_ITEM:// 사진 삭제 
			nDeleteNum = 2;
			if( pEvent->dwParam == TKN_PBOOK_DELETE_NUMBER_ITEM )
			{
				nNumber--;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_EMAIL_ITEM )
			{
				nEmail--;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_MEMO_ITEM )
			{
				nMemo--;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_ANNIV_ITEM )
			{
				nAnniv--;
				nDeleteNum = 4;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_PICTURE_ITEM )
			{	
				dwPicture = 0;
			}
			else ;

            hControl = EQS_GetFocusedIOC();
            nIndex   = EQS_GetIOCOrder(hControl);

            if( EQC_GET_TYPE(hControl) == CT_TIME ) // 기념일 시간 
			{
				nIndex -= 2;
				hControl = EQS_GetNthIOC(nIndex);
				EQS_SetIOCFocus(hControl);		
			}

            if( nIndex < (EQS_GetIOCNum()-1) )
            {
                if( (nIndex % 2) == 0 )
                    sDeleteEditControl(nIndex,nDeleteNum);
                else
                    sDeleteEditControl(nIndex-1, nDeleteNum);
            }
            if( nIndex < (EQS_GetIOCNum()-1) )     // 삭제후 포커스 설정                
                EQS_SetIOCFocus( EQS_GetNthIOC(nIndex) );                
            else                
                EQS_SetIOCFocus( EQS_GetNthIOC( EQS_GetIOCNum()-1 ) ); //항목추가 컨트롤에 포커스를 준다.
			EQC_SET_DIRTY(EQS_GetNthIOC(0), TRUE);
			break;	
		default:
			break;
		}

		break;	    
    case SE_IO_SCROLL_LIST_CANCEL:
        break;
	case SE_IO_SCROLL_LIST_CHG:
#if 0
        hControl = pEvent->wParam;
		nIndex   = EQS_GetIOCOrder(hControl);
		if( nAnniv > 0 &&\
			nIndex >= INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2 &&\
			nIndex < (EQS_GetIOCNum() -1 ))
		{ // 기념일 
			nIndex = EQC_GetScrollLogicalIndex( hControl );
			EQC_SetScrollTitle(hControl, EQC_GetScrollData(hControl, nIndex));
			
			hControl = EQS_GetNthIOC( EQS_GetIOCOrder(hControl) + 1);
			sMake_Anniv_Icon( hControl, nIndex+1 );
			//EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
			//EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_PBOOK_ITEM_DAY));			

		}
#endif
		break;
    case SE_IO_SCROLL_LIST_SELECT:
        hControl = pEvent->wParam;
        nIndex = EQC_GetScrollPhysicalIndex( pEvent->wParam );
        if( hControl == EQS_GetNthIOC(EQS_GetIOCNum()-1) )
        { // 항목 추가 
            switch( nIndex )
            {
            case ICT_GROUP_E:             // 그룹    
                if( nGroup < PERSONAL_GROUP_LIMIT )
                {

					nIndex = INITIAL_INPUT_CONTROL + nGroup*2;
					sAddInputControl( nIndex, 1, ICT_GROUP_E ,TRUE);
					nGroup++;					
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            case ICT_NUMBER_E:             // 전화번호
                if( nNumber < PERSONAL_NUMBER_LIMIT &&\
					active_list.nFreeNumber >= (nNumber+1) )
                {										
					nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
					hControl = sAddInputControl( nIndex,1 ,ICT_NUMBER_E,TRUE );
#if 0
					if( nNumber < PAGER)  // < 4 
					{
						EQC_SetScrollPhysicalIndex( EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-1), nNumber);
					}
#endif
					nNumber++;	
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            case ICT_EMAIL_E:
                if( nEmail < PERSONAL_EMAIL_LIMIT &&\
					active_list.nFreeEMail >= (nEmail+1) )
                {
                    nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail)*2;
                    sAddInputControl( nIndex, 2, ICT_EMAIL_E,TRUE );
                    nEmail++;
                }  
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }                
                break;
            case ICT_MEMO_E:             // 메모
                if( nMemo < PERSONAL_MEMO_LIMIT )
                {
                    nIndex = INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nEmail+nMemo)*2;
                    sAddInputControl( nIndex, 3, ICT_MEMO_E ,TRUE);
                    nMemo++;
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            case ICT_ANNIV_E:             // 기념일    
                if( nAnniv < PERSONAL_ANNIV_LIMIT &&\
					active_list.nFreeAnniv >= (nAnniv+1) )
                {
                    clk_julian_type     julian;
                    DWORD               current_time;
                    nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail+nMemo)*2 + nAnniv*4;
                    hControl = sAddInputControl( nIndex,1, ICT_ANNIV_E,TRUE);
                    current_time = SKY_GetLocalTime();
                    clk_secs_to_julian(current_time, &julian);
                    //EQC_SetTimeYear(hControl, julian.year);
					EQC_SetTimeYear(hControl, 0);
                    EQC_SetTimeMonth(hControl,julian.month);
                    EQC_SetTimeDay(hControl,  julian.day);					
					sMake_Anniv_Icon(EQS_GetNthIOC(EQS_GetIOCOrder(hControl) - 1), 1 ); //생일 
					nAnniv++;					
					//EQS_SetIOCFocus(hControl);
					EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-2));

                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            case ICT_PICTURE_E:
                if( dwPicture == 0 )
                {
					if( SKY_IS_IN_WAP() )
					{
						EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
									   MESSAGE_DEFAULT_TIME,\
									   MBT_NONE, MBT_HELP);
					}
					else
					{
						EQS_START_CHILD( SKA_PhotoListNView );
					}
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }                
                    
                break;
            default:
                break;
            }                
        }
        else
        {
            if( EQS_GetIOCOrder(hControl) < INITIAL_INPUT_CONTROL + nGroup*2 )
            {
                nIndex = EQC_GetScrollLogicalIndex( hControl );
#if 0
                if( nIndex == 0 && KOREAN_STRCMP(EQC_GetScrollData(hControl,0),SKY_GET_TOKEN(TKN_PBOOK_NEW_GROUP))==0)   // 새그룹 추가 
                {
					if( IsFullGroup() == TRUE )
					{
						hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_GROUP_MSG),\
												  MESSAGE_DEFAULT_TIME,\
												  MBT_NONE, MBT_HELP);
					}
					else
					{
						l_szParam[0] = 0;
						EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START,\
							                  INPUT_GROUP_NAME_E, hControl);
					}
                }
                else
#endif
                {   
                    nIndex = EQC_GetScrollPhysicalIndex(hControl);
                    EQC_SetScrollTitle(hControl,\
                                       EQC_GetScrollData(hControl,nIndex));
                    //EQC_ShowScrollTitle(hControl,FALSE);
                }
            }
            else if( EQS_GetIOCOrder(hControl) >= \
                     INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2 )
            { // 기념일 
                nIndex = EQC_GetScrollLogicalIndex( hControl );
                if( nIndex == (ETC_ANNIV-1) )  // 기념일 내용 추가
                {
					l_szParam[0] = 0;						 
					EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START, INPUT_ANNIV_NAME_E, hControl);
                }
				else
				{
					EQC_SetScrollTitle(hControl, EQC_GetScrollData(hControl, nIndex));					
					hControl = EQS_GetNthIOC( EQS_GetIOCOrder(hControl) + 1);
                    sMake_Anniv_Icon( hControl, nIndex+1 );
					//EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_PBOOK_ITEM_DAY));					
				}
            }
            else ;           
        }
        EQS_FitCanvasSize();

        break;
    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            TRUE, SDE_GROUP_SELECT);
        break;
    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            FALSE, SDE_GROUP_SELECT);
        break;  
    case SE_IO_TEDIT_CANCEL:
		break;

    case SE_IO_TEDIT_DONE:
        break;
	case EQE_IO_IMAGE_SELECT:
    case SE_IO_PUSH_BUTTON_SELECT:
		if( SKY_IS_IN_WAP() )
		{
			EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
						   MESSAGE_DEFAULT_TIME,\
						   MBT_NONE, MBT_HELP);
		}
		else
		{		
			EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, EQS_NA, dwPicture );
		}
        break;

	case SE_IO_TIME_SELECT:
		hControl = EQS_GetNthIOC( EQS_GetIOCOrder(pEvent->wParam) - 2);
		nIndex = EQC_GetScrollLogicalIndex(hControl);
		hControl = EQS_GetNthIOC( EQS_GetIOCOrder(pEvent->wParam) - 1);
			
		sMake_Anniv_Icon( hControl, nIndex+1 );

		break;

     case SE_SOFTKEY_SAVE:  
		 {
            BYTE  szName[32];
            BYTE*  pszGroup;
			HCONTROL hGroup, hActive;
#if 0
			if( nMemo || nAnniv || nEmail || nNumber > 1)   //저장할 데이터가 있는지 체크 
			{
				;					
			}
			else if( nNumber == 1)
			{
				hControl = EQS_GetNthIOC( INITIAL_INPUT_CONTROL + nGroup*2 + 1);
				EQC_GetNumberEditValue(hControl, szString);

				if( STRLEN(szString) == 0 )
				{
					hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL -1);     //  이름 정보            
					EQC_GetTextEditData( hControl, szString );
					//if( KOREAN_STRCMP(szString, SKY_GET_TOKEN(TKN_PBOOK_NO_NAME)) == 0 )
					if( STRLEN(szString) == 0)
					{   // 정보가 없다.
						hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_NUMBER_EXIST_MSG),\
												  MESSAGE_DEFAULT_TIME,\
												  MBT_NONE, MBT_HELP);	
						//EQC_SET_USERDATA(hControl, SE_APP_END);
						break;
					}
				}
			}
			else
			{   //정보가 없다. 
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_NUMBER_EXIST_MSG),\
										  MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
				//EQC_SET_USERDATA(hControl, SE_APP_END);
				break;
			}
#endif

            hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL -1);     //  이름 정보    
            EQC_GetTextEditData( hControl, szName );

			hGroup  = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+1);     //  Main 그룹 정보
			hActive = EQS_GetActiveIOC();
			if( hActive != INVALID_CONTROL_HANDLE && EQC_GET_TYPE(hActive) == CT_SCROLL_LIST )
			{
				if( hActive == hGroup )
				{
					nIndex = EQC_GetScrollPhysicalIndex(hGroup);
					EQC_SetScrollTitle(hGroup, EQC_GetScrollData(hGroup,nIndex));
				}
				else if( EQS_GetIOCOrder(hActive) >= (INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2) &&\
					     EQS_GetIOCOrder(hActive)  < (EQS_GetIOCNum()-1 ) )
				{
					nIndex = EQC_GetScrollPhysicalIndex(hActive);
					EQC_SetScrollTitle(hActive, EQC_GetScrollData(hActive,nIndex));
				}
				else ;
			}     
            pszGroup = EQC_GetScrollTitle(hGroup);
				                       
            if( FindDuplicatedBook(SearchGroup(pszGroup), 0, szName) &&\
				KOREAN_STRCMP(szName, SKY_GET_TOKEN(TKN_PBOOK_NO_NAME))!=0 ) 
            { // 같은 이름이 존재 overwrite 여부 결정
                char szMsg[80];
	
                sprintf((char*)szMsg, (char*) SKY_GET_TOKEN(TKN_PBOOK_OVERWRITE_BOOK_MSG),(char*) szName);
                hControl = EQC_MessageBox(szMsg,10, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA( hControl, SE_USER_2); // overwrite 
            }
            else        
			{	// 저장 
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_SAVING), 0, MBT_NONE,MBT_HELP);
				EQC_SET_USERDATA( hControl, SE_USER_1); // save information
			}
			
		 }								          
		break;

    case SE_IO_MESGBOX_END:   
        if( pEvent->dwParam == SE_USER_1 )  //2001.01.14
        { // save information
			if( dwPicture )
			{
				EQC_SET_USERDATA( EQS_GetNthIOC(EQS_GetIOCNum()-3), ((DWORD)XPos * 256 + (DWORD)YPos));
			}
			BookAddress = sSavePhoneBook(EQS_NA,nGroup,nNumber,nEmail,nMemo,nAnniv,dwPicture);			
			PhoneBookUpdate();

			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG),\
									  MESSAGE_DEFAULT_TIME,\
									  MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, SE_APP_END);
        }
		else if( pEvent->dwParam == SE_APP_END )
		{
			if( fEndKey == TRUE )
			{
				EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			}
			else
			{
				if( EQS_GET_PARENT() == SKA_ViewBookList     ||\
					EQS_GET_PARENT() == SKA_ViewSearchName   ||\
					EQS_GET_PARENT() == SKA_ViewSearchNumber   )
				{
					EQS_END_EVENT( SE_APP_RESUME, l_nBookOverWriteAddress, BookAddress );
				}
				else
				{

					EQS_END();
				}
			}
		}			
		else ;

		break;

    case SE_IO_MESGBOX_NO:
		if( pEvent->dwParam == SE_USER_2 ) // overwrite
		{
			break;
		}
		else if( pEvent->dwParam == SE_SOFTKEY_SAVE )
		{
			if( fEndKey == TRUE )
			{
				EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			}
			else
				EQS_END();
		}
		else ;

		break;
    case SE_IO_MESGBOX_YES:
		if( pEvent->dwParam == SE_USER_2 ) // overwrite
		{
            BYTE  szName[32];
            BYTE* pszGroup;
            pb_group_info_type  GroupInfo;
            pb_book_info_type   BookInfo;            

            hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+1);     //   그룹정보         
            pszGroup = EQC_GetScrollTitle(hControl);
            //pszGroup = EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl));

            GroupInfo.address = SearchGroup( pszGroup );
            if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
            {        
            
                hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL -1);     //  이름 정보            
                EQC_GetTextEditData( hControl, szName );
                BookInfo.address = FindBook(pszGroup, szName);
                if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) > 0 )
                { // OverWrite 한다.
					l_nBookOverWriteAddress = BookInfo.address;
                    DeleteBook( &GroupInfo, &BookInfo, TRUE);
                    ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);                   
                }
            }
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_SAVING), 0, MBT_NONE,MBT_HELP);
			EQC_SET_USERDATA( hControl, SE_USER_1); // save information
			//EQS_SEND_EVENT(SE_USER_1, EQS_NA, EQS_NA);
		}
		else if( pEvent->dwParam == SE_SOFTKEY_SAVE ) // save
		{
			EQS_SEND_EVENT( SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
		}
		else ;
		
		break;
	case SE_KEY_CLEAR:    
		if( EQS_IsControlDataChanged() == FALSE)
			EQS_END();
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_SOFTKEY_SAVE);
		}
        break;		
    case SE_KEY_END:
		if( EQS_IsControlDataChanged() == FALSE)
		{
			EQS_END_CURRENT_EVENT();
		}
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_SOFTKEY_SAVE);
			fEndKey = TRUE;
		}        
		break;       
    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
} 

enum tagEDITBOOKCONTENT_MESSAGE_E
{
	EDIT_PBOOKCONTENT_ASK_SAVING_MSG_E = 1,
	EDIT_PBOOKCONTENT_ASK_OVERWRITE_MSG_E,
    EDIT_PBOOKCONTENT_FULL_NUMBER_MSG_E,
	EDIT_PBOOKCONTENT_SAVE_MSG_E
};


VOID SKY_API SKA_EditBookContent(SKY_EVENT_T* pEvent)
{          
    static INT8     nGroup, nNumber, nMemo, nAnniv,nEmail;
    static DWORD    dwPicture;
    static UINT8    hList;          //대표번호 설정을 위한 번호 리스트 핸들
    static UINT8    nGroupAddress;   //redstar
    static INT16    nBookAddress;    //redstar 
	static BOOL     fEndKey;	
	static UINT8    XPos, YPos;
	static UINT8    nSavedNumber, nSavedAnniv, nSavedEmail;
    HCONTROL            hControl, hTemp;  
    INT16               i, nIndex, nOrder, nDeleteNum, index;
    BOOK_CONTROL_E      ControlType;
    BYTE                szTemp[33];

    pb_book_info_type    BookInfo;
    pb_group_info_type   GroupInfo;
    pb_number_info_type  NumberInfo;
    pb_email_info_type   EMailInfo;
    pb_anniv_info_type   AnnivInfo;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        hList       = 0;
        nGroup      = 0;
        nNumber     = 0;
        nMemo       = 0;
        nAnniv      = 0;    
        nEmail      = 0;
        dwPicture   = 0;
		fEndKey     = FALSE;

        l_nBookOverWriteAddress = 0;
        
        if( pEvent->dwParam == BFT_SAVE_OLD_DATA_E )
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_MENU_SELECT_SAVE, // CML_MENU_EDIT,
                SKY_GET_TOKEN(TKN_PBOOK_SAVE_OLD_DATA),
                WA_SHOW_HEADER | WA_SHOW_CML);

        }
        else
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_MENU_SELECT_SAVE,  //CL_NONE_NONE_EDIT
                SKY_GET_TOKEN(TKN_PBOOK_EDIT), 
                WA_SHOW_HEADER | WA_SHOW_CML);
        }      
        XPos = YPos = 0;

        nIndex = 0;
        hControl = sAddInputControl( nIndex, 1, ICT_NAME_E ,FALSE);

        nBookAddress = BookInfo.address = pEvent->wParam;         //redstar

        ui_get_pb( PB_BOOK_INFO_I, &BookInfo);    
        EQC_SetTextEditData( hControl, BookInfo.name);
		EQC_SET_USERDATA(hControl, BookInfo.address);            // save book address

        /* display Group information */
        nIndex   = INITIAL_INPUT_CONTROL;
        hControl = sAddInputControl( nIndex, 1, ICT_GROUP_E ,FALSE);    
        nGroupAddress = BookInfo.group_address;
		if( STRLEN(g_szGroupName[nGroupAddress-1]) )
		{
			EQC_SetScrollTitle(hControl, g_szGroupName[nGroupAddress-1]);
		}
		else
		{
			EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6)); //지정안됨 그룹 으로 
			nGroupAddress = SearchGroup( SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) );
			BookInfo.group_address = nGroupAddress;
			g_PersonGroup[BookInfo.address-1] = nGroupAddress;
			ui_put_pb( PB_BOOK_INFO_I, &BookInfo);	
		}
        nOrder = GET_GROUP_ORDER(nGroupAddress);
        EQC_SetScrollLogicalIndex(hControl,nOrder-1);
		EQC_SET_USERDATA(hControl, BookInfo.group_address );     // save group address
        nGroup++;
#if 0
        if( IsValid( PB_GROUP_INFO_I, BookInfo.multi_group ) == TRUE ) // 멀티 그룹
        {                                              
            nIndex = INITIAL_INPUT_CONTROL + nGroup*2;
            hControl = sAddInputControl( nIndex, 1, ICT_GROUP_E ,FALSE);
            EQC_SetScrollTitle(hControl, g_szGroupName[BookInfo.multi_group-1]);
            nOrder = GET_GROUP_ORDER(nGroupAddress);
            EQC_SetScrollLogicalIndex(hControl, nOrder );
            nGroup++;                                     
        }  
#endif
        /* display number information */  
        NumberInfo.address = BookInfo.number_address;
        hList = CreateList(PERSONAL_NUMBER_LIMIT);           
        while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) == TRUE && nNumber < PERSONAL_NUMBER_LIMIT )
        {            
           if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 )
           {                                                        
                nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
                hControl = sAddInputControl( nIndex, 1,ICT_NUMBER_E,FALSE );
                EQC_SetNumberEditValue(hControl, NumberInfo.digits);
                AddList(hList,NumberInfo.address,hControl);         
				EQC_SET_USERDATA(hControl, NumberInfo.address);               // save number info address
                hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2);
                EQC_SetScrollPhysicalIndex( hControl, (UINT8) NumberInfo.number_type-1 ); 
                nNumber++;                
           }
           NumberInfo.address = NumberInfo.next_index;
        }
         
		if( pEvent->dwParam == BFT_SAVE_OLD_DATA_E )
		{
			if( nNumber >= PERSONAL_NUMBER_LIMIT )
			{
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_MEMORY_MSG),\
										  MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(hControl, SE_APP_END);
				break;
			}
		}
		// email
        EMailInfo.address = BookInfo.email_address;
        while( ui_get_pb( PB_EMAIL_INFO_I, &EMailInfo) == TRUE && nEmail < PERSONAL_EMAIL_LIMIT)
        {
            if( EMailInfo.book_address == BookInfo.address && STRLEN(EMailInfo.szMailID) > 0 )
            {
                nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail)*2;
                hControl = sAddInputControl( nIndex, \
					                         PB_MAX_STRING_LENGTH/INPUT_CONTROL_WIDTH+1,\
											 ICT_EMAIL_E ,FALSE);
                EQC_SetTextEditData( hControl, EMailInfo.szMailID );
				EQC_SET_USERDATA(hControl, EMailInfo.address);   // save memo info address 
                nEmail++;
            }
            EMailInfo.address = EMailInfo.next_index;
        }		
                        
        AnnivInfo.address = BookInfo.anniv_address;
        while( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) == TRUE && nAnniv < PERSONAL_ANNIV_LIMIT )
        {   
			if( AnnivInfo.book_address == BookInfo.address )
			{
				nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nMemo+nEmail)*2+ nAnniv*4;
				hControl = sAddInputControl( nIndex, 1, ICT_ANNIV_E,FALSE);
				EQC_SetTime(hControl,AnnivInfo.year, AnnivInfo.month, 0,AnnivInfo.day,0,0);
				EQC_SetTimeLunar(hControl,AnnivInfo.fLunar);
				hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup+nNumber+nMemo+nEmail)*2+nAnniv*4+1);
				EQC_SET_USERDATA(hControl, AnnivInfo.address);  // save anniv info address
				if( AnnivInfo.type == ETC_ANNIV )     
				{
					EQC_SetScrollTitle(hControl, AnnivInfo.content);
					EQC_SetScrollData(hControl, ETC_ANNIV-1, AnnivInfo.content);
					EQC_SetScrollPhysicalIndex(hControl,AnnivInfo.type-1);
				}
				else
				{
					EQC_SetScrollTitle(hControl, EQC_GetScrollData(hControl, AnnivInfo.type -1 ));
					EQC_SetScrollPhysicalIndex(hControl,AnnivInfo.type-1);
				}	
				hControl = EQS_GetNthIOC(EQS_GetIOCOrder(hControl) + 1);
				sMake_Anniv_Icon( hControl, AnnivInfo.type );

				nAnniv++;
			}
            
            AnnivInfo.address = AnnivInfo.next_index;
        }       
		//memo 
        if( STRLEN( BookInfo.szText ) > 0 )
        {
            nIndex = INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2;
            hControl = sAddInputControl( nIndex, PB_MAX_MEMO_LENGTH/INPUT_CONTROL_WIDTH +1,\
				                         ICT_MEMO_E,FALSE );
            EQC_SetTextEditData( hControl, BookInfo.szText );
            nMemo++;
        }

        if( BookInfo.dwImageIndex )
        {
			BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
			HFILE hFile;         
			
			dwPicture = BookInfo.dwImageIndex;  
			ConvertCameraImageFileName(szFileName, dwPicture);
#if VIEW_PHOTO_IMAGE
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				DWORD dwTimeInfo;
				RECT  ViewArea;
				int x,y;

				hControl = sAddInputControl(EQS_GetIOCNum(), 1, ICT_PICTURE_E, FALSE);

				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
				SKY_ReadDataBlock(hFile, &l_PBookBitmapInfo.Header, sizeof(l_PBookBitmapInfo.Header));  
				SKY_ReadDataBlock(hFile, g_abBmpBuf, GET_IMAGE_BYTE_SIZE(l_PBookBitmapInfo.Header));   
				l_PBookBitmapInfo.pbData = g_abBmpBuf;  
				SKY_CloseDataFile(hFile); 

				x = EQC_GET_X(hControl);
				y = EQC_GET_Y(hControl);
				EQS_DeleteControl(hControl);

				if( l_PBookBitmapInfo.Header.yStart != PHOTO_START_Y )  //01.06.04
					l_PBookBitmapInfo.Header.yStart = PHOTO_START_Y;

				hControl = EQC_Image(x,y, &l_PBookBitmapInfo);
				EQC_SET_STYLE(hControl, CS_IMAGE_BORDER );

				XPos = BookInfo.other_link[0] >> 8;
				YPos = (BookInfo.other_link[0] % 256);

				if( XPos + PHOTO_IMAGE_WIDTH  > EQC_GET_DX(hControl) ||\
					YPos + PHOTO_IMAGE_HEIGHT > EQC_GET_DY(hControl)    )
				{
					y = ( l_PBookBitmapInfo.Header.Size.yHeight-l_PBookBitmapInfo.Header.yStart -\
						  PHOTO_IMAGE_HEIGHT)/2 - l_PBookBitmapInfo.Header.yStart;
					if( y < 0 )
						y = 0;

					XPos = (l_PBookBitmapInfo.Header.Size.xWidth - PHOTO_IMAGE_WIDTH)/2;
					YPos = y;
				}
													
				ViewArea.xLeft = XPos;
				ViewArea.xRight= ViewArea.xLeft+PHOTO_IMAGE_WIDTH-1;
				ViewArea.yTop  = l_PBookBitmapInfo.Header.yStart+YPos;
				ViewArea.yBottom = ViewArea.yTop+PHOTO_IMAGE_HEIGHT-1;

				EQC_SetImageViewArea(hControl, &ViewArea);
				EQC_SET_USERDATA(hControl, dwPicture );				
				EQC_SET_FOCUS(hControl,TRUE);
				
			}				
#else
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				hControl = sAddInputControl(EQS_GetIOCNum(), 1, ICT_PICTURE_E, FALSE);
				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_CloseDataFile(hFile);				
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_PUSH_BUTTON_CENTER );
				EQC_SetPushButtonCaption( hControl, szFileName, INPUT_CONTROL_WIDTH);
			}
#endif
			else
			{
				dwPicture = BookInfo.dwImageIndex = 0;
				ui_put_pb( PB_BOOK_INFO_I, &BookInfo);
			}

        }

        if( pEvent->dwParam == BFT_SAVE_OLD_DATA_E )
        {
            if( nNumber < PERSONAL_NUMBER_LIMIT )
            {
                nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
                hControl = sAddInputControl( nIndex, 1, ICT_NUMBER_E,TRUE );
                EQC_SetNumberEditValue(hControl, l_szParam);
				EQC_SET_DIRTY(hControl, TRUE);
                AddList(hList,NumberInfo.address, hControl);         //2001.01.16
                nNumber++; 
            }
            else
            {                
                hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_NUMBER_MSG),\
									      MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(hControl, EDIT_PBOOKCONTENT_FULL_NUMBER_MSG_E );
				break;
            }
        }			
		sAddInputControl( EQS_GetIOCNum(), 1, ICT_MENU_E ,FALSE);

		nSavedNumber = nNumber;
		nSavedAnniv  = nAnniv;
		nSavedEmail  = nEmail;

        //EQS_FitCanvasSize();
        //EQS_SEND_EVENT(SE_APP_DRAW, 0,0);
 
        break;
    case SE_APP_END:
        DeleteList(hList);
        break;

    case SE_IO_FOCUS_CHG:
        break;
    case SE_APP_RESUME:
        if( EQS_GET_CHILD() == SKA_PhotoListNView && pEvent->dwParam )
        {                 

			BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
			HFILE hFile;         

            dwPicture = pEvent->dwParam;
			ConvertCameraImageFileName(szFileName, dwPicture);
#if VIEW_PHOTO_IMAGE
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				DWORD dwTimeInfo;
				RECT  ViewArea;
				int x,y;

				hControl = sAddInputControl(EQS_GetIOCNum()-1, 1, ICT_PICTURE_E, FALSE);

				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
				SKY_ReadDataBlock(hFile, &l_PBookBitmapInfo.Header, sizeof(l_PBookBitmapInfo.Header));  
				SKY_ReadDataBlock(hFile, g_abBmpBuf, GET_IMAGE_BYTE_SIZE(l_PBookBitmapInfo.Header));   
				l_PBookBitmapInfo.pbData = g_abBmpBuf;  
				SKY_CloseDataFile(hFile); 
				x = EQC_GET_X(hControl);
				y = EQC_GET_Y(hControl);
				EQS_DeleteControl(hControl);

				if( l_PBookBitmapInfo.Header.yStart != PHOTO_START_Y )  //01.06.04
					l_PBookBitmapInfo.Header.yStart = PHOTO_START_Y;

				hControl = EQC_Image(x,y, &l_PBookBitmapInfo);
				EQC_SET_STYLE(hControl, CS_IMAGE_BORDER );

				y = ( l_PBookBitmapInfo.Header.Size.yHeight-l_PBookBitmapInfo.Header.yStart -\
					  PHOTO_IMAGE_HEIGHT)/2 - l_PBookBitmapInfo.Header.yStart;
				if( y < 0 )
					y = 0;

				XPos = (l_PBookBitmapInfo.Header.Size.xWidth - PHOTO_IMAGE_WIDTH)/2;
				YPos = y;
									
				ViewArea.xLeft = XPos;
				ViewArea.xRight= ViewArea.xLeft+PHOTO_IMAGE_WIDTH-1;
				ViewArea.yTop  = l_PBookBitmapInfo.Header.yStart+YPos;
				ViewArea.yBottom = ViewArea.yTop+PHOTO_IMAGE_HEIGHT-1;

				EQC_SetImageViewArea(hControl, &ViewArea);
				EQC_SET_USERDATA(hControl, dwPicture );
				EQC_SET_FOCUS(hControl,TRUE);
				EQC_SET_DIRTY(hControl, TRUE);
				EQC_SET_Y( EQS_GetNthIOC(EQS_GetIOCNum()-1), EQC_GET_Y(hControl)+EQC_GET_DY(hControl) + 5);	
				EQS_SetIOCFocus(hControl);
				EQS_FitCanvasSize();

			}				
#else
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_CloseDataFile(hFile);
				hControl = sAddInputControl(EQS_GetIOCNum()-1, 1, ICT_PICTURE_E, FALSE);
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_PUSH_BUTTON_CENTER );
				EQC_SetPushButtonCaption( hControl, szFileName, INPUT_CONTROL_WIDTH);
				EQC_SET_DIRTY(hControl, TRUE);
				EQS_SetIOCFocus(hControl);
			}
#endif
			
        }
		else if( EQS_GET_CHILD() == SKA_PhotoListNView )
		{
			EQS_SetIOCFocus( EQS_GetNthIOC(EQS_GetIOCNum()-1) );
		}
		else if( EQS_GET_CHILD() == SKA_PhotoViewer )
		{
			EQS_SetIOCFocus( EQS_GetNthIOC(EQS_GetIOCNum()-2) );
		}
		else if( EQS_GET_CHILD() == sSKA_EditImageViewArea )
		{
			if( pEvent->wParam != 0 && pEvent->dwParam != 0 )
			{
				RECT  ViewArea;

				hControl  = EQS_GetNthIOC(EQS_GetIOCNum()-2);
				XPos = pEvent->wParam-1;
				YPos = pEvent->dwParam-1;

				ViewArea.xLeft   = XPos;
				ViewArea.xRight  = ViewArea.xLeft+PHOTO_IMAGE_WIDTH-1;
				ViewArea.yTop    = l_PBookBitmapInfo.Header.yStart + YPos;
				ViewArea.yBottom = ViewArea.yTop+PHOTO_IMAGE_HEIGHT-1;
				EQC_SetImageViewArea(hControl, &ViewArea);

			}
		}
		else if( EQS_GET_CHILD() == sSKA_GetString )
		{
			if( pEvent->wParam )
			{
				if( STRLEN( l_szParam ) > 0 )
				{
					EQC_SetScrollTitle( pEvent->dwParam, l_szParam );
					//EQC_ShowScrollTitle(pEvent->dwParam, TRUE );
					if( pEvent->wParam == INPUT_ANNIV_NAME_E )
					{
#if 0
						nIndex = EQC_GetScrollLogicalIndex( pEvent->dwParam );
						if( nIndex == (ETC_ANNIV-1) )  // 기념일 내용 추가	
#endif
						{						
							EQC_SetScrollPhysicalIndex(pEvent->dwParam, ETC_ANNIV-1);
							STRCPY( EQC_GetScrollData(pEvent->dwParam, ETC_ANNIV-1), l_szParam);
							hControl = EQS_GetNthIOC( EQS_GetIOCOrder(pEvent->dwParam) + 1);
							sMake_Anniv_Icon( hControl, ETC_ANNIV);//nIndex+1 );
						}
					}
				}
			}			
			EQS_SetIOCFocus( pEvent->dwParam );
		}
        else
            EQS_SetIOCFocus( EQS_GetNthIOC(1) );
            
        break;
    case SE_SOFTKEY_MENU:
		{
			SKY_TOKEN paMenu[10];
			BOOL      faActiveMenu[10];
			UINT8     nPopUpMenu = 0;


			hControl = EQS_GetActiveIOC();
			if( hControl != INVALID_CONTROL_HANDLE )
			{
				if( EQC_GET_TYPE(hControl) == CT_SCROLL_LIST )
				{
					EQC_DeActiveScrollList(hControl);
				}
			}

			//paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_ITEM;
			for(nIndex=0; nIndex < 10; nIndex++)
			{
				faActiveMenu[nIndex] = FALSE;
			}
        			
			if( nNumber < PERSONAL_NUMBER_LIMIT )
			{
				if( (active_list.nFreeNumber + nSavedNumber) >= (nNumber+1) )// active_list.nFreeBook )
				{
					faActiveMenu[nPopUpMenu] = TRUE;					
				}
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_NUMBER_ITEM;

			if( nEmail < PERSONAL_EMAIL_LIMIT )
			{
				if( (active_list.nFreeEMail + nSavedEmail) >= (nEmail+1) )//active_list.nFreeBook )
				{
					faActiveMenu[nPopUpMenu] = TRUE;
				}
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_EMAIL_ITEM;

			if( nMemo < PERSONAL_MEMO_LIMIT )
			{					
				faActiveMenu[nPopUpMenu] = TRUE;				
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_MEMO_ITEM;


			if( nAnniv < PERSONAL_ANNIV_LIMIT )
			{
				if( (active_list.nFreeAnniv + nSavedAnniv ) >= (nAnniv+1) )//  active_list.nFreeBook )
				{
					faActiveMenu[nPopUpMenu] = TRUE;					
				}
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_ANNIV_ITEM;


			if( dwPicture == 0 )
			{								
				faActiveMenu[nPopUpMenu] = TRUE;
				paMenu[nPopUpMenu++] = TKN_PBOOK_ADD_PICTURE_ITEM;
				ControlType = sGetCurrentControlType(nGroup, nNumber, nEmail,\
													 nMemo, nAnniv, 0);
			}
			else				
			{
				faActiveMenu[nPopUpMenu] = TRUE;
				paMenu[nPopUpMenu++] = TKN_PBOOK_RESIZE_PICTURE;
				ControlType = sGetCurrentControlType(nGroup, nNumber, nEmail,\
													 nMemo, nAnniv, 1);
			}	
#if 0			
			if( nNumber > 0 )
			{
				faActiveMenu[nPopUpMenu] = TRUE;								
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_SET_NUMBER;
#endif
			if( ControlType == ICT_MENU_E )
			{
               ;
			}
			else
			{	
				faActiveMenu[nPopUpMenu] = TRUE;
				switch( ControlType )
				{
				case ICT_NAME_E:
					faActiveMenu[nPopUpMenu] = FALSE;
					paMenu[nPopUpMenu++] = TKN_PBOOK_NEW_GROUP;
					break;
				case ICT_GROUP_E:    
					paMenu[nPopUpMenu++] = TKN_PBOOK_NEW_GROUP;
					break;
				case ICT_NUMBER_E:            // 전화번호
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_NUMBER_ITEM;
					break;
				case ICT_EMAIL_E:             // eMail
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_EMAIL_ITEM;
					break;
				case ICT_MEMO_E:              // 메모
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_MEMO_ITEM;
					break;
				case ICT_ANNIV_E:             // 기념일    
					if( EQC_GET_TYPE(EQS_GetFocusedIOC()) == CT_SCROLL_LIST )
					{
						paMenu[nPopUpMenu++] = TKN_PBOOK_ANNIV_MENU;
					}
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_ANNIV_ITEM;
					break;
				case ICT_PICTURE_E:            
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_PICTURE_ITEM;
					break;
				default:
					break;
				}  				
			}						
			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, nPopUpMenu, 0, faActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN);
		}        
        break;

    case SE_IO_POPUP_SELECT:                                         	
        switch( pEvent->dwParam )
        {
/*
		case TKN_PBOOK_ADD_ITEM:
			hControl = EQS_GetNthIOC(EQS_GetIOCNum()-1);
			EQS_SetIOCActive(hControl);
			break;
*/

		case TKN_PBOOK_ADD_NUMBER_ITEM:
			nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
			hControl = sAddInputControl( nIndex,1 ,ICT_NUMBER_E,TRUE );
#if 0
			if( nNumber < PAGER)  // < 4 
			{
				EQC_SetScrollPhysicalIndex( EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-1), nNumber);
			}
#endif
			AddList(hList,EQS_NA, hControl);
			nNumber++;
			break;
		case TKN_PBOOK_ADD_EMAIL_ITEM:
            nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail)*2;
            sAddInputControl( nIndex, 2, ICT_EMAIL_E,TRUE );
            nEmail++;
			break;
		case TKN_PBOOK_ADD_MEMO_ITEM:
            nIndex = INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nEmail+nMemo)*2;
            sAddInputControl( nIndex, 3, ICT_MEMO_E ,TRUE);
            nMemo++;
			break;
		case TKN_PBOOK_ADD_ANNIV_ITEM:
			{
                clk_julian_type     julian;
                DWORD               current_time;
                nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail+nMemo)*2 + nAnniv*4;
                hControl = sAddInputControl( nIndex,1, ICT_ANNIV_E,TRUE);
                current_time = SKY_GetLocalTime();
                clk_secs_to_julian(current_time, &julian);
                //EQC_SetTimeYear(hControl, julian.year);
				EQC_SetTimeYear(hControl, 0);
                EQC_SetTimeMonth(hControl,julian.month);
                EQC_SetTimeDay(hControl,  julian.day);
				sMake_Anniv_Icon(EQS_GetNthIOC(EQS_GetIOCOrder(hControl) - 1), 1 ); //생일 
                nAnniv++;					
				//EQS_SetIOCFocus(hControl);
				EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-2));

			}
			break;
		case TKN_PBOOK_ADD_PICTURE_ITEM:
			if( SKY_IS_IN_WAP() )
			{
				EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
							   MESSAGE_DEFAULT_TIME,\
							   MBT_NONE, MBT_HELP);
			}
			else
			{
				EQS_START_CHILD( SKA_PhotoListNView );
			}
			break;
		case TKN_PBOOK_RESIZE_PICTURE:
			EQS_START_CHILD_EVENT( sSKA_EditImageViewArea, SE_APP_START,\
				                   XPos, YPos);
			break;
		case TKN_PBOOK_NEW_GROUP: // 새그룹 이름 
			if( IsFullGroup() == TRUE )
			{
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_GROUP_MSG),\
										  MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
			}
			else
			{
				l_szParam[0] = 0;
				EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START,\
								      INPUT_GROUP_NAME_E, EQS_GetFocusedIOC());
			}
			break;
		case TKN_PBOOK_ANNIV_MENU: //새 기념일 이름 
			l_szParam[0] = 0;
			EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START, INPUT_ANNIV_NAME_E, EQS_GetFocusedIOC());
			break;
		case TKN_PBOOK_DELETE_NUMBER_ITEM: // 번호 삭제 
		case TKN_PBOOK_DELETE_EMAIL_ITEM:  // 이메일계정 삭제
		case TKN_PBOOK_DELETE_MEMO_ITEM:   // 메모 삭제 
		case TKN_PBOOK_DELETE_ANNIV_ITEM:  // 기념일 삭제 
		case TKN_PBOOK_DELETE_PICTURE_ITEM:// 사진 삭제 
			nDeleteNum = 2;
			if( pEvent->dwParam == TKN_PBOOK_DELETE_NUMBER_ITEM )
			{
				nNumber--;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_EMAIL_ITEM )
			{
				nEmail--;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_MEMO_ITEM )
			{
				nMemo--;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_ANNIV_ITEM )
			{
				nAnniv--;
				nDeleteNum = 4;
			}
			else if( pEvent->dwParam == TKN_PBOOK_DELETE_PICTURE_ITEM )
			{	
				dwPicture = 0;
			}
			else ;

			hControl = EQS_GetFocusedIOC();
			nIndex   = EQS_GetIOCOrder(hControl);

            if( EQC_GET_TYPE(hControl) == CT_TIME ) // 기념일 시간 
			{
				nIndex -= 2;
				hControl = EQS_GetNthIOC(nIndex);
				EQS_SetIOCFocus(hControl);		
			}

            if( (nIndex % 2) == 0 )
            {
                RemoveListType(hList,EQS_GetNthIOC(EQS_GetIOCOrder(hControl)+1));
                sDeleteEditControl(nIndex,nDeleteNum);
            }
            else
            {
                RemoveListType(hList,hControl);
                sDeleteEditControl(nIndex-1, nDeleteNum);
            }
            if( nIndex < (EQS_GetIOCNum()-1) )     // 삭제후 포커스 설정
            {
                EQS_SetIOCFocus( EQS_GetNthIOC(nIndex) );
            }
            else
            {   //항목추가 컨트롤에 포커스를 준다.
                EQS_SetIOCFocus( EQS_GetNthIOC( EQS_GetIOCNum()-1 ) ); 
            }
			EQC_SET_DIRTY(EQS_GetNthIOC(0), TRUE);

			break;		
#if 0
        case TKN_PBOOK_SET_NUMBER: // 대표번호 설정 
            if( nNumber > 0 )
            {
                hControl   = sMakeScrollList(10, EQS_GetSWindowOffset()+20, 17, 4, 0, \
                                             (UINT8)0,hList,SDE_TITLE_NUMBER);
                if( hControl == 0 )
                {
                    break;
                }
				EQC_SET_USERDATA(hControl, EQS_GetFocusedIOC());
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_POPUP );
                EQC_SetScrollTitle( hControl, SKY_GET_TOKEN(TKN_PBOOK_SET_NUMBER));
                EQC_ShowScrollTitle(hControl, TRUE);                
                EQS_SetIOCActive( hControl );    
                
            }
			else
			{   //대표번호를 설정할 번호정보가 없는 경우 
                hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_INVALID_TITLENUMBER_MSG),\
									      MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
			}
            break;
#endif
        default:
            break;
        }
        break; 

    case SE_IO_SCROLL_LIST_CANCEL:                    
        hControl = pEvent->wParam;
        if( pEvent->dwParam != EQC_DELETED && \
            KOREAN_STRCMP(SKY_GET_TOKEN(TKN_PBOOK_SET_NUMBER),\
                   EQC_GetScrollTitle(hControl)) == 0 )   // "대표번호 설정" 삭제 
        {     
            EQC_SetScrollListHandle(hControl,0);  // 리스트는 보존해야 한다.
			EQS_SetIOCFocus( EQC_GET_USERDATA(hControl) );
            EQS_DeleteControl( hControl );
        }       
        break;
	case SE_IO_SCROLL_LIST_CHG:
#if 0 //기념일 별자리를 없앰.
        hControl = pEvent->wParam;
		nIndex   = EQS_GetIOCOrder(hControl);
		if( nAnniv > 0 &&\
			nIndex >= INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2 &&\
			nIndex < (EQS_GetIOCNum() -1 ))
		{ // 기념일 
			nIndex = EQC_GetScrollLogicalIndex( hControl );
			EQC_SetScrollTitle(hControl, EQC_GetScrollData(hControl, nIndex));

			hControl = EQS_GetNthIOC( EQS_GetIOCOrder(hControl) + 1);
			sMake_Anniv_Icon( hControl, nIndex+1 );
			//EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_PBOOK_ITEM_DAY));
			
		}
#endif
		break;	
    case SE_IO_SCROLL_LIST_SELECT:
        hControl = pEvent->wParam;
        nIndex   = EQC_GetScrollPhysicalIndex( pEvent->wParam );

        if( hControl == EQS_GetNthIOC(EQS_GetIOCNum()-1) )    // 항목 추가 
        { 
            switch( nIndex )
            {
            case ICT_GROUP_E:             // 그룹  
                if( nGroup < PERSONAL_GROUP_LIMIT )
                {
                    nIndex = INITIAL_INPUT_CONTROL + nGroup*2;
                    hControl = sAddInputControl( nIndex, 1, ICT_GROUP_E ,TRUE);
                    nGroup++;
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            case ICT_NUMBER_E:             // 전화번호				
                if( nNumber < PERSONAL_NUMBER_LIMIT  &&\
					(active_list.nFreeNumber + nSavedNumber ) >= (nNumber+1) )//active_list.nFreeBook ))
                {
					nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
					hControl = sAddInputControl( nIndex, 1, ICT_NUMBER_E,TRUE );
#if 0
					if( nNumber < PAGER)  // < 4 
					{
						EQC_SetScrollPhysicalIndex( EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-1), nNumber);
					}
#endif
					AddList(hList,EQS_NA, hControl);
					nNumber++;
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;

            case ICT_EMAIL_E:             // 전화번호
                if( nEmail < PERSONAL_EMAIL_LIMIT &&\
					(active_list.nFreeEMail + nSavedEmail) >= (nEmail+1) )
                {
                    nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail)*2;
                    hControl = sAddInputControl( nIndex, 2, ICT_EMAIL_E,TRUE );
                    nEmail++;
                } 
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }                
                break;

            case ICT_MEMO_E:             // 메모
                if( nMemo < PERSONAL_MEMO_LIMIT)
                {
                    nIndex = INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2;
                    hControl = sAddInputControl( nIndex, 3, ICT_MEMO_E ,TRUE);
                    nMemo++;
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            case ICT_ANNIV_E:             // 기념일    
                if( nAnniv < PERSONAL_ANNIV_LIMIT &&\
					(active_list.nFreeAnniv+nSavedAnniv) >= (nAnniv+1) )
                {
                    clk_julian_type     julian;
                    DWORD               current_time;

                    nIndex = INITIAL_INPUT_CONTROL+(nGroup+nNumber+nMemo+nEmail)*2+nAnniv*4;
                    hControl = sAddInputControl( nIndex, 1, ICT_ANNIV_E,TRUE);
                    nAnniv++;
                    current_time = SKY_GetLocalTime();
                    clk_secs_to_julian(current_time, &julian);
                    //EQC_SetTimeYear(hControl, julian.year);
					EQC_SetTimeYear(hControl, 0);
                    EQC_SetTimeMonth(hControl,julian.month);
                    EQC_SetTimeDay(hControl,  julian.day);

					sMake_Anniv_Icon(EQS_GetNthIOC(EQS_GetIOCOrder(hControl) - 1), 1 ); //생일
					//EQS_SetIOCFocus(hControl);
					EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-2));
									                 
                }
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            case ICT_PICTURE_E:
                if( dwPicture == 0 )
                {
					if( SKY_IS_IN_WAP() )
					{
						EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
									   MESSAGE_DEFAULT_TIME,\
									   MBT_NONE, MBT_HELP);
					}
					else
					{
						EQS_START_CHILD(SKA_PhotoListNView);
					}
                }                
                else
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_ITEM_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                }
                break;
            default:
                break;
            }
/*            
			if( hControl != pEvent->wParam )
				EQS_SetIOCFocus(hControl);
*/

        }
        else
        {                       
            if( KOREAN_STRCMP(SKY_GET_TOKEN(TKN_PBOOK_SET_NUMBER), EQC_GetScrollTitle(hControl)) == 0 )
            {   //  대표번호 설정       
                EQC_SetScrollListHandle(hControl,0); // 리스트는 보존해야 한다.
				EQS_SetIOCFocus( EQC_GET_USERDATA(hControl) );
				EQS_DeleteControl( hControl );
                if( pEvent->dwParam >=1 )
                {
                    BYTE  szTitleNumber[30], szTempNumber[30];
                    UINT8 TitleType, TempType;
                                                      
                    nIndex    = INITIAL_INPUT_CONTROL + (nGroup+pEvent->dwParam)*2;
                    hControl  = EQS_GetNthIOC(nIndex);
                    TitleType = EQC_GetScrollPhysicalIndex(hControl);
                    hControl  = EQS_GetNthIOC(nIndex+1);
                    EQC_GetNumberEditValue(hControl,szTitleNumber);
                    for( i = pEvent->dwParam; i>0; i--)
                    {
                        nIndex   = INITIAL_INPUT_CONTROL + (nGroup+i-1)*2;
                        hControl = EQS_GetNthIOC(nIndex);
                        TempType = EQC_GetScrollPhysicalIndex(hControl);
                        hControl = EQS_GetNthIOC(nIndex+1);
                        EQC_GetNumberEditValue(hControl,szTempNumber);                    
                        hControl = EQS_GetNthIOC(nIndex+2);
                        EQC_SetScrollPhysicalIndex(hControl,TempType);
                        hControl = EQS_GetNthIOC(nIndex+3);
                        EQC_SetNumberEditValue(hControl,szTempNumber);
                    }
                    nIndex   =  INITIAL_INPUT_CONTROL + (nGroup)*2;
                    hControl = EQS_GetNthIOC(nIndex);
                    EQC_SetScrollPhysicalIndex(hControl,TitleType);
                    hControl = EQS_GetNthIOC(nIndex+1);
                    EQC_SetNumberEditValue(hControl, szTitleNumber);  
                    
                }							
                EQS_SEND_EVENT(SE_APP_DRAW,0,0);
            }
            else
            {
                if( EQS_GetIOCOrder(hControl) < INITIAL_INPUT_CONTROL + nGroup*2 )
                {
#if 0
                    nIndex = EQC_GetScrollLogicalIndex( hControl );
                    if( nIndex == 0 && KOREAN_STRCMP(EQC_GetScrollData(hControl,0),SKY_GET_TOKEN(TKN_PBOOK_NEW_GROUP))==0)   // 새그룹 추가 
                    {
						if( IsFullGroup() == TRUE )
						{
							hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_GROUP_MSG),\
													  MESSAGE_DEFAULT_TIME,\
													  MBT_NONE, MBT_HELP);
						}
						else
						{
							l_szParam[0] = 0;
							EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START,\
								                  INPUT_GROUP_NAME_E, hControl);
						}
                    }
                    else
#endif
                    {   
                        nIndex = EQC_GetScrollPhysicalIndex(hControl);
                        EQC_SetScrollTitle(hControl,\
                                           EQC_GetScrollData(hControl,nIndex));
                        //EQC_ShowScrollTitle(hControl,FALSE);
                    }
                }
                else if( EQS_GetIOCOrder(hControl) >= \
                         INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2 )
                {
                    nIndex = EQC_GetScrollLogicalIndex( hControl );
                    if( nIndex == (ETC_ANNIV-1) )  // 기념일 내용 추가
                    {
						l_szParam[0] = 0;
						EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START, INPUT_ANNIV_NAME_E, hControl);
                    }
					else
					{
						EQC_SetScrollTitle(hControl, EQC_GetScrollData(hControl, nIndex));	
						hControl = EQS_GetNthIOC( EQS_GetIOCOrder(hControl) + 1);
						sMake_Anniv_Icon( hControl, nIndex+1 );
						//EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_PBOOK_ITEM_DAY));
					}
                }
                else ;
            }
        }
        break;
   
    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:                                   // group name scroll
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            TRUE, SDE_GROUP_SELECT);
        break;
    case SE_IO_SCROLL_LIST_UP:                                      // group name scroll
    case SE_IO_SCROLL_LIST_DOWN:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            FALSE, SDE_GROUP_SELECT);
		
        break;  

    case SE_IO_TEDIT_CANCEL:
		break;        
    case SE_IO_TEDIT_DONE:
        break;
	case EQE_IO_IMAGE_SELECT:
    case SE_IO_PUSH_BUTTON_SELECT:
		if( SKY_IS_IN_WAP() )
		{
			EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
						   MESSAGE_DEFAULT_TIME,\
						   MBT_NONE, MBT_HELP);
		}
		else
		{
			EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, EQS_NA, dwPicture );
		}
        break;
	case SE_IO_TIME_SELECT:
		hControl = EQS_GetNthIOC( EQS_GetIOCOrder(pEvent->wParam) - 2);
		nIndex   = EQC_GetScrollLogicalIndex(hControl);
		hControl = EQS_GetNthIOC( EQS_GetIOCOrder(pEvent->wParam) - 1);			
		sMake_Anniv_Icon( hControl, nIndex+1 );		
		break;

    case SE_KEY_SEND:
		//if(SKY_IsInCall() || SKY_IS_IN_WAP() )
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE || nNumber == 0)
			break;

        if( dwPicture == 0 )
            ControlType = sGetCurrentControlType(nGroup, nNumber,\
                                                 nEmail, nMemo, nAnniv,0);
        else
            ControlType = sGetCurrentControlType(nGroup, nNumber, nEmail,\
                                                 nMemo, nAnniv, 1);
        if( ControlType == ICT_NUMBER_E )
        {                                        
            hControl = EQS_GetFocusedIOC();                
            nIndex   = EQS_GetIOCOrderly(hControl);
            if( nIndex % 2 == 0 )
                hControl = EQS_GetNthIOC(nIndex+1);
            else
                hControl = EQS_GetNthIOC(nIndex);
        }
        else   // 대표번호로 통화시도 
        {
            hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup+1)*2-1);
        }

		EQC_GetTextEditData(EQS_GetNthIOC(1), szTemp);
		hTemp = EQS_GetNthIOC( EQS_GetIOCOrder(hControl) -1 );
		index = EQC_GetScrollLogicalIndex(hTemp);
		STRCAT( szTemp, EQC_GetScrollData(hTemp, index));
		index = STRLEN(szTemp);
		SET_ALPHAEDIT_BUFFER(szTemp, index);
		SET_ALPHAEDIT_POS(index);

        EQC_GetNumberEditValue( hControl, szTemp);
		if( STRLEN(szTemp) > 0 )
		{
			sPBookSendCall(szTemp);
		}
        break;

    case SE_SOFTKEY_SAVE:       // 편집 저장
#if 0
		if( IsFullPBook() == TRUE )
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_MEMORY_MSG),\
									  MESSAGE_DEFAULT_TIME,\
									  MBT_NONE, MBT_HELP);
		}
		else
#endif
		{
			HCONTROL hGroup, hActive;
			UINT8    nFindGroupAddress;

			hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL -1);     //  이름 정보
			EQC_GetTextEditData( hControl, szTemp );

			hGroup  = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+1);     //  Main 그룹 정보
			hActive = EQS_GetActiveIOC();
			if( hActive != INVALID_CONTROL_HANDLE && EQC_GET_TYPE(hActive) == CT_SCROLL_LIST )
			{
				if( hActive == hGroup )
				{// 그룹 데이터 
					nIndex = EQC_GetScrollPhysicalIndex(hGroup);
					EQC_SetScrollTitle(hGroup, EQC_GetScrollData(hGroup,nIndex));
				}
				else if( EQS_GetIOCOrder(hActive) >= (INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nMemo+nEmail)*2) &&\
					     EQS_GetIOCOrder(hActive)  < (EQS_GetIOCNum()-1 ) )
				{//기념일 데이터 
					nIndex = EQC_GetScrollPhysicalIndex(hActive);
					EQC_SetScrollTitle(hActive, EQC_GetScrollData(hActive,nIndex));
				}
				else ;
			}
			nFindGroupAddress = SearchGroup(EQC_GetScrollTitle(hGroup));

			if( STRLEN(szTemp) > 0  && KOREAN_STRCMP(szTemp, SKY_GET_TOKEN(TKN_PBOOK_NO_NAME))!=0 )
			{ // 같은 이름이 존재 overwrite 여부 결정   
				char szMsg[80];
				   
				l_nBookOverWriteAddress= FindDuplicatedBook(nFindGroupAddress, nBookAddress, szTemp);
				if( l_nBookOverWriteAddress && l_nBookOverWriteAddress != nBookAddress )
				{
					sprintf( szMsg, SKY_GET_TOKEN(TKN_PBOOK_OVERWRITE_BOOK_MSG), szTemp);
					hControl = EQC_MessageBox(szMsg,10, MBT_YESNO, MBT_CONFIRM);
					EQC_SET_USERDATA( hControl, SE_USER_2); // overwrite 
					break;
				}
				else
				{ 
					l_nBookOverWriteAddress = 0;				
				}
			}
			// 저장 
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_SAVING), 0, MBT_NONE,MBT_HELP);
			EQC_SET_USERDATA( hControl, SE_USER_1); // save information
		}

        break;

    case SE_IO_MESGBOX_END:
        if( pEvent->dwParam == SE_USER_1 )  //2001.01.14
        { // save information
			GroupInfo.address = nGroupAddress;
			if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
			{        
/*
				BookInfo.address = nBookAddress;
				if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) > 0 )
				{
					NumberInfo.address = BookInfo.number_address;         // speed dial을 고려하여 존재하는 number를 그대로 유지하기 위해
					while( ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) > 0 )
					{
						if( FindList(hList,NumberInfo.address) < 0 )
						{
							DeleteNumberLink(&NumberInfo);
							free_pb_node(PB_NUMBER_INFO_I, NumberInfo.address);
						}
						NumberInfo.address = NumberInfo.next_index;                    
					}                                         
					DeleteBook( &GroupInfo, &BookInfo, FALSE); //수정을 위해서 지운다.
					ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
*/
				    if( dwPicture )
					{
						EQC_SET_USERDATA( EQS_GetNthIOC( EQS_GetIOCNum() - 3), ((DWORD)XPos*256 + (DWORD)YPos));
					}
					nBookAddress = sSavePhoneBook(hList, nGroup, nNumber, nEmail, nMemo,nAnniv, dwPicture);  // 그룹 세이브까지.....
					PhoneBookUpdate();
					hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG),\
											  MESSAGE_DEFAULT_TIME,\
											  MBT_NONE, MBT_HELP);							
					EQC_SET_USERDATA(hControl, SE_USER_3);
//				}
			}		
        }
		else if( pEvent->dwParam == SE_USER_3 ) // end after saving information
		{            
            if( fEndKey == TRUE )
			{
				EQS_END_EVENT( SE_KEY_END, EQS_NA, EQS_NA);
				break;
			}

			//*** 돌아갔을때 리스트 갱신이 필요함 
            if( IsValid(PB_BOOK_INFO_I, nBookAddress) )
            {
                nIndex   = INITIAL_INPUT_CONTROL + nGroup*2 -1;
                hControl = EQS_GetNthIOC(nIndex);
                if( KOREAN_STRCMP(g_szGroupName[nGroupAddress-1], EQC_GetScrollTitle(hControl)) )// 그룹이 변경된 경우
                {                        
					EQS_END_EVENT(SE_APP_RESUME,(WORD)l_nBookOverWriteAddress,(DWORD) nBookAddress);                                        
                }                            
                else                            
                {                        
					EQS_END_EVENT(SE_APP_RESUME, l_nBookOverWriteAddress, (DWORD)nBookAddress);  //주의
                }
            }
            else
			{
                EQS_END();
			}
		}
		else if( pEvent->dwParam == SE_APP_END )
		{
			if( fEndKey == TRUE )
			{
				EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			}
			else
			{
				EQS_END();
			}

		}
		else ;

		break;		
    case SE_IO_MESGBOX_NO:
		if( pEvent->dwParam == SE_USER_2 ) // overwrite
		{
            if( l_nBookOverWriteAddress )
            {
                l_nBookOverWriteAddress = 0;
                break ;  // 다시 입력 받는다.
            }
			break;
		}
		else if( pEvent->dwParam == SE_SOFTKEY_SAVE )
		{
			if( fEndKey == TRUE )
			{
				EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			}
			else
				EQS_END();
		}
		else ;

		break;
    case SE_IO_MESGBOX_YES:
		if( pEvent->dwParam == SE_USER_2 ) // overwrite
		{
            BookInfo.address = l_nBookOverWriteAddress;
            ui_get_pb( PB_BOOK_INFO_I, &BookInfo);
			GroupInfo.address = BookInfo.group_address;
			ui_get_pb( PB_GROUP_INFO_I, &GroupInfo);
            DeleteBook( &GroupInfo, &BookInfo, TRUE);  
			ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_SAVING), 0, MBT_NONE,MBT_HELP);
			EQC_SET_USERDATA( hControl, SE_USER_1); // save information

			//EQS_SEND_EVENT(SE_USER_1, EQS_NA, EQS_NA);
		}
		else if( pEvent->dwParam == SE_SOFTKEY_SAVE ) // save
		{
			EQS_SEND_EVENT( SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
		}
		else ;
		
		break;
	case SE_KEY_CLEAR:    
		if( EQS_IsControlDataChanged() == FALSE)
		{
			EQS_END();
		}
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_SOFTKEY_SAVE);
		}
        break;		
    case SE_KEY_END:
		if( EQS_IsControlDataChanged() == FALSE)
			EQS_END_CURRENT_EVENT();
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_SOFTKEY_SAVE);
			fEndKey = TRUE;
		}        
		break;

    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

enum tagVIEW_TELNUMBER_CONTROL_E
{         
	VIEW_TELNUMBER_NAME_TEXT_E = 0,
	VIEW_TELNUMBER_IMAGE_E,	
	VIEW_TELNUMBER_NUMBER_EDIT_E,	
};

VOID SKY_API sSKA_ViewTelNumber(SKY_EVENT_T* pEvent)
{
    HCONTROL       hControl;    
	
    pb_number_info_type  NumberInfo;
	BYTE  szBuf[80];
	UINT8  i, nLen;
	RECT   Rect;

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_NONE_CANCEL, 
                      (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_VIEW_PHONE_NUMBER),
                      WA_SHOW_HEADER| WA_SHOW_CML);
			
		EQS_GetSWindowViewAreaRect(&Rect);
		
		EQC_Image( 4, Rect.yBottom - g_BmpPBookPannel.Header.Size.yHeight-35,\
			       (BITMAP*)&g_BmpPBookPannel );

		
        NumberInfo.address = pEvent->wParam;

        if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &NumberInfo ) == 0 )
        {
            EQS_END_EVENT(SE_APP_RESUME, EQS_NA, EQS_NA);
            break;
        }

		hControl   = EQC_StaticText(3,20, TKN_PBOOK_SAVE);
        sprintf((CHAR*)szBuf,"%s%s", g_szPersonName[NumberInfo.book_address-1],\
                                      phone_type[NumberInfo.number_type-1]);
		EQC_SET_SELF_DRAW(hControl, FALSE);
		EQC_SetStaticText(hControl, szBuf);
		//EQC_SetStaticTextAttribute(hControl, (DRAW_MODE)TDM_BOLD);
		EQC_SET_USERDATA(hControl, NumberInfo.address);
		EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl));// | CS_STATIC_TEXT_ORING );
			
		hControl = EQC_NumberEdit(12,48,13,3,PB_MAX_DIGIT_LENGTH,NumberInfo.digits);
		//EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
		EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

		EQC_SET_SELF_DRAW(hControl, FALSE);
		EQC_SET_STYLE(hControl,  CS_NUMBER_EDIT_RIGHT2LEFT |\
			                     CS_NUMBER_EDIT_NO_CML     |\
								 CS_NUMBER_EDIT_ORING      );
								 //CS_NUMBER_EDIT_BORDER      );
		EQC_SetNumberEditBigFont(hControl,TRUE);	
		EQC_SET_FOCUS(hControl,FALSE);		    
		
        break;
    case SE_APP_END:
		break;

    case SE_KEY_SEND:
	case SE_SOFTKEY_CALL:
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;
//      SKY_SET_ORIGINATE(TRUE); hjcho 01.06.20
		hControl = EQS_GetNthIOC(VIEW_TELNUMBER_NUMBER_EDIT_E);
		EQC_GetNumberEditValue(hControl, szBuf);
        nLen = STRLEN(szBuf);

		if( nLen > 0 )
		{
			SET_NUMEDIT_BUFFER(szBuf, nLen);
			SET_NUMEDIT_POS(nLen);  

			hControl = EQS_GetNthIOC(VIEW_TELNUMBER_NAME_TEXT_E);
			STRCPY( szBuf, EQC_GetStaticText(hControl) );
			nLen = STRLEN(szBuf) ;
			SET_ALPHAEDIT_BUFFER(szBuf, nLen);
			SET_ALPHAEDIT_POS(nLen);
			EQS_START_SIBLING(SKA_StartOrigination, FALSE);
		}
	    //ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,\
        //                     GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);		
        break;

    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

enum tagVIEWBOOKCONTENT_MESSAGE_E
{     
    VIEW_PBOOKCONTENT_NO_SDIAL_INFO_MSG_E = 1,
	VIEW_PBOOKCONTENT_COMPLETE_DELETING_BOOK_MSG_E,
	VIEW_PBOOKCONTENT_DELETE_BOOK_MSG_E,
	VIEW_PBOOKCONTENT_DELETE_NUMBER_MSG_E,
	VIEW_PBOOKCONTENT_DELETE_EMAIL_MSG_E,
	VIEW_PBOOKCONTENT_DELETE_MEMO_MSG_E,
	VIEW_PBOOKCONTENT_DELETE_ANNIV_MSG_E,
	VIEW_PBOOKCONTENT_DELETE_PICTURE_MSG_E,
	VIEW_PBOOKCONTENT_INFO_MSG_E

};

VOID SKY_API SKA_ViewBookContent(SKY_EVENT_T* pEvent)
{          
    static INT8     nNumber, nMemo, nAnniv,nEmail;
    static UINT8    hList;          //대표번호 설정을 위한 번호 리스트 핸들	
	static DWORD    dwPicture;
	static INT16    nBookAddress;
	static UINT8    XPos, YPos;

    HCONTROL            hControl, hTemp;  
    INT16               i, nIndex, nOrder, index;
    BOOK_CONTROL_E      ControlType;
    BYTE                szTemp[33];
	BYTE*               pszBuf;

    pb_book_info_type    BookInfo;
    pb_group_info_type   GroupInfo;
    pb_number_info_type  NumberInfo;
    pb_email_info_type   EMailInfo;
    pb_anniv_info_type   AnnivInfo;

    switch( pEvent->EventID )
    {
    case SE_APP_START:                
        hList       = 0;
        nNumber     = 0;
        nMemo       = 0;
        nAnniv      = 0;    
        nEmail      = 0;
		        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_MENU_VIEW_EDIT,
            SKY_GET_TOKEN(TKN_PBOOK_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);
            

        nBookAddress     = pEvent->wParam;
		if( IsValid( PB_BOOK_INFO_I, nBookAddress ) == FALSE )
		{
			EQS_END();
			break;
		}
		if( g_szPersonName[nBookAddress-1][0] == BOOK_NOT_USE )
		{
			EQS_END();
			break;
		}
        BookInfo.address = nBookAddress;
        ui_get_pb( PB_BOOK_INFO_I, &BookInfo);    

        /* display number information */  
        NumberInfo.address = BookInfo.number_address;
        hList = CreateList(PERSONAL_NUMBER_LIMIT);           
        while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) == TRUE && nNumber < PERSONAL_NUMBER_LIMIT)
        {            
           if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 )
           {                                                        
                nIndex =  nNumber*2;//INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
                hControl = sAddInputControl( nIndex, 1, ICT_NUMBER_E,FALSE );
                EQC_SetNumberEditValue(hControl, NumberInfo.digits);
				EQC_SET_USERDATA(hControl, NumberInfo.address);
                AddList(hList,NumberInfo.address,hControl);         //2001.01.16
                hControl = EQS_GetNthIOC((nNumber)*2);
				switch( NumberInfo.number_type )
				{
                case CELL :
					sprintf((char*)szTemp," %s ",(char*)SF_CELLULAR1);
					break;
				case HOME :
					sprintf((char*)szTemp," %s ",(char*)SF_HOME1);
					break;
				case WORK :
					sprintf((char*)szTemp," %s ",(char*)SF_OFFICE1);
					break;
				case PAGER:
					sprintf((char*)szTemp," %s ",(char*)SF_PAGER1);
					break;
				case FAX  :
					sprintf((char*)szTemp," %s ",(char*)SF_FAX1);
					break;
				default:
					sprintf((char*)szTemp," %s ",(char*)SF_CELLULAR1);
					break;
				}
				szTemp[2] += nNumber;
				EQC_SetStaticText(hControl, szTemp);
				//EQC_SET_USERDATA(EQS_GetNthIOC(EQS_GetIOCOrder(hControl)+1), NumberInfo.number_type);
                nNumber++;                
           }
           NumberInfo.address = NumberInfo.next_index;
        }
		
		if( nNumber == PERSONAL_NUMBER_LIMIT )
		{
			NumberInfo.address = GetListValue( hList, nNumber-1);
			ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo);
			NumberInfo.next_index = 0;
			ui_put_pb( PB_NUMBER_INFO_I, &NumberInfo);
		}

		//email
        EMailInfo.address = BookInfo.email_address;
        while( ui_get_pb( PB_EMAIL_INFO_I, &EMailInfo) == TRUE && nEmail < PERSONAL_EMAIL_LIMIT)
        {
            if( EMailInfo.book_address == BookInfo.address && STRLEN(EMailInfo.szMailID) > 0 )
            {
                nIndex = (nNumber+nEmail)*2;
                hControl = sAddInputControl( nIndex, \
					                         STRLEN(EMailInfo.szMailID)/INPUT_CONTROL_WIDTH+1,\
					                         ICT_EMAIL_E ,FALSE);
                EQC_SetTextEditData( hControl, EMailInfo.szMailID );
				EQC_SET_USERDATA(hControl, EMailInfo.address);
				EQC_SET_ENABLE(hControl, FALSE);
                nEmail++;                
            }
            EMailInfo.address = EMailInfo.next_index;
        }		
            
        AnnivInfo.address = BookInfo.anniv_address;
        while( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) == TRUE && nAnniv < PERSONAL_ANNIV_LIMIT)
        {   
			if( AnnivInfo.book_address == BookInfo.address )
			{
				nIndex = (nNumber+nMemo+nEmail)*2+ nAnniv*4;
				hControl = sAddInputControl( nIndex, 1, ICT_ANNIV_E,FALSE);
				EQC_SetTime(hControl,AnnivInfo.year, AnnivInfo.month, 0,AnnivInfo.day,0,0);
				EQC_SetTimeLunar(hControl,AnnivInfo.fLunar);
				EQC_SET_USERDATA(hControl, AnnivInfo.address);
				EQC_SET_ENABLE(hControl, FALSE);
				
				hControl = EQS_GetNthIOC((nNumber+nMemo+nEmail)*2+nAnniv*4+1);
				EQC_SET_ENABLE(hControl, FALSE);

				if( AnnivInfo.type == ETC_ANNIV )     
				{
					EQC_SetScrollTitle(hControl, AnnivInfo.content);
					EQC_SetScrollPhysicalIndex(hControl,AnnivInfo.type-1);
				}
				else
				{
					EQC_SetScrollTitle(hControl, EQC_GetScrollData(hControl, AnnivInfo.type -1 ));
					EQC_SetScrollPhysicalIndex(hControl,AnnivInfo.type-1);
				}	
				EQC_SET_USERDATA(hControl, AnnivInfo.address);
				hControl = EQS_GetNthIOC(EQS_GetIOCOrder(hControl) + 1);
				sMake_Anniv_Icon( hControl, AnnivInfo.type );
				
				nAnniv++;
			}            
            AnnivInfo.address = AnnivInfo.next_index;
        }
       
		//memo
        if( STRLEN( BookInfo.szText ) > 0 )
        {
            nIndex = (nNumber+nMemo+nEmail)*2;
            hControl = sAddInputControl( nIndex, \
				                         STRLEN(BookInfo.szText)/INPUT_CONTROL_WIDTH+1, ICT_MEMO_E,FALSE );
            EQC_SetTextEditData( hControl, BookInfo.szText );
			EQC_SET_ENABLE(hControl, FALSE);
			//EQC_SET_FOCUS( EQS_GetNthIOC(EQS_GetIOCOrder(hControl)+1), FALSE);
            nMemo++;
        }
            
		dwPicture = BookInfo.dwImageIndex;
        if( dwPicture )
        {
			BYTE szFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
			HFILE hFile;
		                  			
			ConvertCameraImageFileName(szFileName, dwPicture);
#if VIEW_PHOTO_IMAGE
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				DWORD dwTimeInfo;
				RECT  ViewArea;
				int x,y;

				hControl = sAddInputControl(EQS_GetIOCNum(), 1, ICT_PICTURE_E, FALSE);

				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
				SKY_ReadDataBlock(hFile, &l_PBookBitmapInfo.Header, sizeof(l_PBookBitmapInfo.Header));  
				SKY_ReadDataBlock(hFile, g_abBmpBuf, GET_IMAGE_BYTE_SIZE(l_PBookBitmapInfo.Header));   
				l_PBookBitmapInfo.pbData = g_abBmpBuf;  
				SKY_CloseDataFile(hFile); 
				x = EQC_GET_X(hControl);
				y = EQC_GET_Y(hControl);
				EQS_DeleteControl(hControl);

				if( l_PBookBitmapInfo.Header.yStart != PHOTO_START_Y )  //01.06.04
					l_PBookBitmapInfo.Header.yStart = PHOTO_START_Y;

				hControl = EQC_Image(x,y, &l_PBookBitmapInfo);
				EQC_SET_STYLE(hControl, CS_IMAGE_BORDER );
				XPos = BookInfo.other_link[0] >> 8;
				YPos = (BookInfo.other_link[0] % 256);
				if( XPos + VIEW_PHOTO_IMAGE_WIDTH  > EQC_GET_DX(hControl) ||\
					YPos + VIEW_PHOTO_IMAGE_HEIGHT > EQC_GET_DY(hControl)    )
				{
					y = ( l_PBookBitmapInfo.Header.Size.yHeight-l_PBookBitmapInfo.Header.yStart -\
						  VIEW_PHOTO_IMAGE_HEIGHT)/2 - l_PBookBitmapInfo.Header.yStart;
					if( y < 0 )
						y = 0;

					XPos = (l_PBookBitmapInfo.Header.Size.xWidth - VIEW_PHOTO_IMAGE_WIDTH)/2;
					YPos = y;
				}
									
				ViewArea.xLeft = XPos;
				ViewArea.xRight= ViewArea.xLeft+VIEW_PHOTO_IMAGE_WIDTH-1;
				ViewArea.yTop  = l_PBookBitmapInfo.Header.yStart+YPos;
				ViewArea.yBottom = ViewArea.yTop+VIEW_PHOTO_IMAGE_HEIGHT-1;

				EQC_SetImageViewArea(hControl, &ViewArea);
				EQC_SET_USERDATA(hControl, dwPicture );
				EQC_SET_FOCUS(hControl,TRUE);
			}				
#else			
			if(SKY_OpenDataFile(SDF_PHOTO, szFileName, &hFile, SFT_READ) == SDR_OK)
			{
				SKY_ReadDataBlock(hFile, szFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
				SKY_CloseDataFile(hFile);
				hControl = sAddInputControl(EQS_GetIOCNum(), 1, ICT_PICTURE_E, FALSE);
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_PUSH_BUTTON_CENTER );
				EQC_SetPushButtonCaption( hControl, szFileName, INPUT_CONTROL_WIDTH);				
			}
#endif
			else
			{
				dwPicture = BookInfo.dwImageIndex = 0;
				ui_put_pb( PB_BOOK_INFO_I, &BookInfo);
			}
        }

        hControl = sAddInputControl( EQS_GetIOCNum(), 1,ICT_GROUP_E ,FALSE);
        GroupInfo.address = BookInfo.group_address;
		if( STRLEN(g_szGroupName[GroupInfo.address-1]) )
		{			
			EQC_SetScrollTitle(hControl, g_szGroupName[GroupInfo.address-1]);
		}
		else
		{
			EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6)); //지정안됨 그룹 으로 
			GroupInfo.address = SearchGroup( SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) );
			BookInfo.group_address = GroupInfo.address;
			g_PersonGroup[BookInfo.address-1] = GroupInfo.address;
			ui_put_pb( PB_BOOK_INFO_I, &BookInfo);			
		}
        nOrder = GET_GROUP_ORDER(GroupInfo.address);
        EQC_SetScrollLogicalIndex(hControl,nOrder);
		EQC_SET_USERDATA(hControl, BookInfo.address);              // book address
		EQC_SET_ENABLE(hControl, FALSE);
		//EQC_SET_FOCUS(hControl, FALSE);    

		pszBuf = (BYTE*) SKY_Malloc(sizeof(BYTE)*25);
		STRCPY( pszBuf, BookInfo.name );
		//STRCAT( pszBuf, (BYTE*) SF_BOOK );
		EQS_SetSWindowHeaderStr(pszBuf);

        for(i = 0; i < EQS_GetIOCNum(); i++)         // for only view not edit
        {
            hControl = EQS_GetNthIOC(i);
			EQC_SET_ENABLE(hControl,FALSE);
			EQC_SET_EDIT(hControl,FALSE);
			//EQC_SET_FOCUS(hControl, FALSE);
        }		
		if( nNumber == 0 )
		{
			EQS_SetWindowCML(CL_MENU_NONE_EDIT);
		}
        break;
    case SE_APP_END:   
		SKY_Free(EQS_GetCurrentHeaderStr());
        DeleteList(hList);
		PhoneBookUpdate();
        break;

    case SE_APP_RESUME:
		if( EQS_GET_CHILD() == SKA_SelectNumber )
		{
			if( pEvent->wParam == TKN_PBOOK_SET_SDIAL_SELECT_MSG )
			{
                EQS_START_CHILD_EVENT(SKA_EditSDial, SE_APP_START, (WORD)pEvent->dwParam, EQS_NA);
				break;
			}
			else if( pEvent->wParam == TKN_PBOOK_SEND_SMS_SELECT_MSG )
			{
			   pb_number_info_type numberinfo;
           
			   numberinfo.address = pEvent->dwParam;
			   ui_get_pb( PB_NUMBER_INFO_I, &numberinfo);
			   nIndex = STRLEN(numberinfo.digits);
			   memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T)); 
			   STRCPY(g_UIMOBuffer.pszRcvNumber, numberinfo.digits);
			   g_UIMOBuffer.nRcvNumberSize = nIndex;
			   EQS_START_CHILD(SKA_SMSEditMessage);
			   break;
			}
			else ;
		}	
		else if( EQS_GET_CHILD() == sSKA_SelectEmailID )
		{
			if( pEvent->dwParam )
			{
				EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, EQS_NA,\
					                  pEvent->dwParam);
			}
		}
		else if( EQS_GET_CHILD() == SKA_PhotoViewer )
		{
			EQS_SetIOCFocus( EQS_GetNthIOC(EQS_GetIOCNum()-3));
		}
		else ;

        break;
    case SE_SOFTKEY_MENU:
        {
            SKY_TOKEN paMenu[4];
			BOOL      fActiveMenu[4];
			UINT8     nPopUpMenu = 0;

			for(i=0; i< 4; i++)
			{
				fActiveMenu[i] = FALSE;
			}

			if( nNumber > 0 && l_fFromCall == FALSE )
			{
				fActiveMenu[nPopUpMenu] = TRUE;
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_SEND_SMS;

			if( nNumber > 0 )
			{
				fActiveMenu[nPopUpMenu] = TRUE;
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_SET_SDIAL;

			if( nEmail > 0 && l_fFromCall == FALSE )
			{
				fActiveMenu[nPopUpMenu] = TRUE;
			}
			paMenu[nPopUpMenu++] = TKN_PBOOK_SEND_EMAIL;


            fActiveMenu[nPopUpMenu] = TRUE;
			
            hControl = EQS_GetFocusedIOC();
			if( hControl != INVALID_CONTROL_HANDLE )
			{
				hControl = EQS_GetNthIOC( EQS_GetIOCOrder(hControl) - 1 );
				switch( EQC_GET_USERDATA(hControl) )
				{
				case ICT_NUMBER_E:
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_NUMBER_ITEM;
					break;
				case ICT_EMAIL_E:
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_EMAIL_ITEM;
					break;
				case ICT_MEMO_E:
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_MEMO_ITEM;
					break;
				case ICT_ANNIV_E:
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_ANNIV_ITEM;
					break;
				case ICT_PICTURE_E:
					paMenu[nPopUpMenu++] = TKN_PBOOK_DELETE_PICTURE_ITEM;
					break;
				default:
					paMenu[nPopUpMenu++] = TKN_PBOOK_ONE_BOOK_DELETE;
					break;    
				}
			}				
			else
			{
				paMenu[nPopUpMenu++] = TKN_PBOOK_ONE_BOOK_DELETE;
			}

            hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, nPopUpMenu, 0, fActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }  
        break;

    case SE_SOFTKEY_EDIT:
       EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_PBOOK_CHANGE_EDIT_MODE_MSG),\
		                      0, MBT_NONE,MBT_HELP); //MESSAGE_DEFAULT_TIME,	    
	   break;
	case SE_IO_FOCUS_CHG:
		if( EQC_GET_TYPE(pEvent->wParam) == CT_NUMBER_EDIT  ||\
			EQC_GET_TYPE(pEvent->wParam) == CT_PUSH_BUTTON  ||\
			EQC_GET_TYPE(pEvent->wParam) == CT_IMAGE)
		{

			EQS_SetWindowCML(CL_MENU_VIEW_EDIT);
		}
		else
		{
			EQS_SetWindowCML(CL_MENU_NONE_EDIT);
		}
		break;

	case SE_IO_MESGBOX_NO:

		//hControl = EQS_GetNthIOC(VIEW_PBOOK_CONTENT_NAME_TEXT_E);
		//EQS_SetIOCActive(hControl);	
		
		break;
	case SE_IO_MESGBOX_YES:

		hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETING_MSG), 1, MBT_NONE, MBT_HELP);
		if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_BOOK_MSG_E )
		{
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_BOOK_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_NUMBER_MSG_E )
		{
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_NUMBER_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_EMAIL_MSG_E )
		{
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_EMAIL_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_MEMO_MSG_E )
		{
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_MEMO_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_ANNIV_MSG_E )
		{
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_ANNIV_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_PICTURE_MSG_E )
		{
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_PICTURE_MSG_E);
		}
		else ;

		break;      

    case SE_IO_MESGBOX_END:
		if( pEvent->dwParam == VIEW_PBOOKCONTENT_INFO_MSG_E )
		{
			;			
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_COMPLETE_DELETING_BOOK_MSG_E )
		{
			EQS_END_EVENT(SE_APP_RESUME, EQS_NA, nBookAddress);  
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_NO_SDIAL_INFO_MSG_E )
		{
			;
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_BOOK_MSG_E )
		{			
			BookInfo.address = nBookAddress;
			if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
				break;

			GroupInfo.address = BookInfo.group_address;
			if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) == 0)
				break;    
        
			DeleteBook(&GroupInfo, &BookInfo, TRUE); //2001.01.08
			ui_put_pb(PB_GROUP_INFO_I, &GroupInfo);
			PhoneBookUpdate(); 
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), MESSAGE_DEFAULT_TIME,\
				                      MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_COMPLETE_DELETING_BOOK_MSG_E);						
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_NUMBER_MSG_E )
		{
			BookInfo.address = nBookAddress;
			if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
				break;
			DeleteNumber(&BookInfo, EQC_GET_USERDATA( EQS_GetFocusedIOC() ));
			sAdjustBookContent(ICT_NUMBER_E);
			nNumber--;
			for( index=0; index < nNumber; index++ )
			{								
				switch( g_PersonNumberType[nBookAddress-1][index] )
				{
                case CELL :
					sprintf((char*)szTemp," %s ",(char*)SF_CELLULAR1);
					break;
				case HOME :
					sprintf((char*)szTemp," %s ",(char*)SF_HOME1);
					break;
				case WORK :
					sprintf((char*)szTemp," %s ",(char*)SF_OFFICE1);
					break;
				case PAGER:
					sprintf((char*)szTemp," %s ",(char*)SF_PAGER1);
					break;
				case FAX  :
					sprintf((char*)szTemp," %s ",(char*)SF_FAX1);
					break;
				default:
					sprintf((char*)szTemp," %s ",(char*)SF_CELLULAR1);
					break;
				}
				szTemp[2] += index;
				EQC_SetStaticText(EQS_GetNthIOC(index*2), szTemp);
			}

			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), MESSAGE_DEFAULT_TIME,\
				                      MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_INFO_MSG_E);

		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_EMAIL_MSG_E )
		{
			BookInfo.address = nBookAddress;
			if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
				break;
			DeleteEmail(&BookInfo,  EQC_GET_USERDATA( EQS_GetFocusedIOC() ));
			sAdjustBookContent(ICT_EMAIL_E);
			nEmail--;
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), MESSAGE_DEFAULT_TIME,\
				                      MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_INFO_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_MEMO_MSG_E )
		{
			BookInfo.address = nBookAddress;
			if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
				break;
			BookInfo.szText[0] = 0;
			ui_put_pb( PB_BOOK_INFO_I, &BookInfo);
				
			sAdjustBookContent(ICT_MEMO_E);
			nMemo--;
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), MESSAGE_DEFAULT_TIME,\
				                      MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_INFO_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_ANNIV_MSG_E )
		{
			BookInfo.address = nBookAddress;
			if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
				break;
			DeleteAnniv(&BookInfo, EQC_GET_USERDATA( EQS_GetFocusedIOC() ) );
			sAdjustBookContent(ICT_ANNIV_E);
			nAnniv--;
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), MESSAGE_DEFAULT_TIME,\
				                      MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_INFO_MSG_E);
		}
		else if( pEvent->dwParam == VIEW_PBOOKCONTENT_DELETE_PICTURE_MSG_E )
		{
			BookInfo.address = nBookAddress;
			if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
				break;
			BookInfo.dwImageIndex = 0;
			ui_put_pb( PB_BOOK_INFO_I, &BookInfo);
			sAdjustBookContent(ICT_PICTURE_E);
			dwPicture = 0;
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), MESSAGE_DEFAULT_TIME,\
				                      MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_INFO_MSG_E);
		}
		else
		{		
			EQS_START_SIBLING_EVENT( SKA_EditBookContent, SE_APP_START,
			                     (WORD) nBookAddress,\
								 EQS_NA, FALSE);
		}
        break;

    /* This Event Comes From SKA_MainSmsCode hjcho 01.11.09 */
    case SE_SOFTKEY_SMS:
        if(pEvent->wParam == SMSCODE_IN_VIEW_BOOK_LIST_EMAIL)
        {
            hControl = EQS_GetFocusedIOC();
			if( hControl != INVALID_CONTROL_HANDLE )
			{
				if( EQC_GET_USERDATA(EQS_GetNthIOC( EQS_GetIOCOrder(hControl) - 1)) == ICT_EMAIL_E )
				{
					EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, EQS_NA,\
										  EQC_GET_USERDATA(hControl));
					break;
				}
			}

			if( nEmail == 1 )
			{
				hControl = EQS_GetNthIOC((nNumber+nEmail)*2 - 1);
				EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, EQS_NA,\
									  EQC_GET_USERDATA(hControl));
			}
			else
			{
				EQS_START_CHILD_EVENT(sSKA_SelectEmailID, SE_APP_START,\
									  nBookAddress,\
									  TKN_PBOOK_SET_SDIAL_SELECT_MSG);	
			}
        }
        break;

    case SE_IO_POPUP_SELECT:	                                         	
        switch( pEvent->dwParam )
        {
		case TKN_PBOOK_SEND_SMS:  //메시지 작성 
            if(SKY_IS_SMS_LOCK() && pEvent->wParam != SMSCODE_IN_VIEW_CONTENT)
            {
                EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_IN_VIEW_CONTENT, EQS_NA);
            }
            else
            {
                hControl = EQS_GetFocusedIOC();
			    if( hControl != INVALID_CONTROL_HANDLE )
			    {
				    if( EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT )
				    {
				       pb_number_info_type numberinfo;
       
				       numberinfo.address = EQC_GET_USERDATA(hControl);
				       ui_get_pb( PB_NUMBER_INFO_I, &numberinfo);
				       nIndex = STRLEN(numberinfo.digits);
				       memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T)); 
				       STRCPY(g_UIMOBuffer.pszRcvNumber, numberinfo.digits);
				       g_UIMOBuffer.nRcvNumberSize = nIndex;
				       EQS_START_CHILD(SKA_SMSEditMessage);
					    break;
				    }
			    }

			    if( nNumber == 1 )
			    {
			       pb_number_info_type numberinfo;
   
			       hControl = EQS_GetNthIOC(1); // 첫번째 전화번호 
			       numberinfo.address = EQC_GET_USERDATA(hControl);
			       ui_get_pb( PB_NUMBER_INFO_I, &numberinfo);
			       nIndex = STRLEN(numberinfo.digits);
			       memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T)); 
			       STRCPY(g_UIMOBuffer.pszRcvNumber, numberinfo.digits);
			       g_UIMOBuffer.nRcvNumberSize = nIndex;
			       EQS_START_CHILD(SKA_SMSEditMessage);

			    }
			    else
			    {				
				    EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
									      nBookAddress,\
									      TKN_PBOOK_SEND_SMS_SELECT_MSG);
			    }
            }
			break;
									
        case TKN_PBOOK_SET_SDIAL:  // 단축다이얼 지정
			hControl = EQS_GetFocusedIOC();
			if( hControl != INVALID_CONTROL_HANDLE )
			{
				if( EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT )
				{		
					EQS_START_CHILD_EVENT(SKA_EditSDial, SE_APP_START,\
						                 (WORD)EQC_GET_USERDATA(hControl), EQS_NA);
					break;
				}
			}

			if( nNumber == 1 )
			{
				hControl = EQS_GetNthIOC(1);
				EQS_START_CHILD_EVENT(SKA_EditSDial, SE_APP_START,\
						             (WORD)EQC_GET_USERDATA(hControl), EQS_NA);
					
			}
			else
			{
				EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
									  nBookAddress,\
									  TKN_PBOOK_SET_SDIAL_SELECT_MSG);	

			}
            break;
		case TKN_PBOOK_SEND_EMAIL:
            if(SKY_IS_SMS_LOCK() && pEvent->wParam != SMSCODE_IN_VIEW_CONTENT)
            {
                EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_IN_VIEW_BOOK_LIST_EMAIL, EQS_NA);
            }
            else
            {
			    hControl = EQS_GetFocusedIOC();
			    if( hControl != INVALID_CONTROL_HANDLE )
			    {
				    if( EQC_GET_USERDATA(EQS_GetNthIOC( EQS_GetIOCOrder(hControl) - 1)) == ICT_EMAIL_E )
				    {
					    EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, EQS_NA,\
										      EQC_GET_USERDATA(hControl));
					    break;
				    }
			    }

			    if( nEmail == 1 )
			    {
				    hControl = EQS_GetNthIOC((nNumber+nEmail)*2 - 1);
				    EQS_START_CHILD_EVENT(SKA_MainMailSend, SE_APP_START, EQS_NA,\
									      EQC_GET_USERDATA(hControl));
			    }
			    else
			    {
				    EQS_START_CHILD_EVENT(sSKA_SelectEmailID, SE_APP_START,\
									      nBookAddress,\
									      TKN_PBOOK_SET_SDIAL_SELECT_MSG);	
			    }
            }
			break;

		case TKN_PBOOK_DELETE_NUMBER_ITEM:	
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG), 10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_NUMBER_MSG_E);			
			break;

		case TKN_PBOOK_DELETE_EMAIL_ITEM:
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG), 10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_EMAIL_MSG_E);
			break;
		case TKN_PBOOK_DELETE_MEMO_ITEM:	
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG), 10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_MEMO_MSG_E);				
			break;

		case TKN_PBOOK_DELETE_ANNIV_ITEM:
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG), 10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_ANNIV_MSG_E);	
			break;
		case TKN_PBOOK_DELETE_PICTURE_ITEM:
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG), 10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_PICTURE_MSG_E);	
			break;
		case TKN_PBOOK_ONE_BOOK_DELETE:
			{//전화부 삭제 
				BYTE szBuf[100];

				STRCPY( szBuf, EQS_GetCurrentHeaderStr() );
				STRCAT( szBuf,(BYTE*)"\n");
				STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));
				hControl = EQC_MessageBox(szBuf, 10, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(hControl, VIEW_PBOOKCONTENT_DELETE_BOOK_MSG_E );
			}
            break;
        default:
            break;
        }

        break; 
/*
    case SE_IO_IMAGE_SELECT:
		hControl  = pEvent->wParam;
		dwPicture = EQC_GET_USERDATA(hControl);
		if( dwPicture )
		{   // 사진 보기 
			EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, EQS_NA, dwPicture ); 
		}
		break;
    case SE_IO_PUSH_BUTTON_SELECT:
        EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, EQS_NA, dwPicture);	
        break;
*/
    case SE_KEY_SEND:
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;

        hControl = EQS_GetFocusedIOC();
		if(  hControl != INVALID_CONTROL_HANDLE && EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT )
		{
			
			hTemp = EQS_GetNthIOC(EQS_GetIOCOrder(hControl)-1);	
			STRCPY( szTemp, EQS_GetCurrentHeaderStr());
			pszBuf = EQC_GetStaticText(hTemp);   //2001.10.04
			if( pszBuf[0] == ' ' )
			{
				pszBuf++;
			}
			STRCAT( szTemp, pszBuf);
			index = STRLEN(szTemp);
			SET_ALPHAEDIT_BUFFER(szTemp, index);
			SET_ALPHAEDIT_POS(index);

			EQC_GetNumberEditValue( hControl, szTemp);
			if( STRLEN(szTemp) > 0 )
			{
				sPBookSendCall(szTemp);
			}
		}
        break;
	case SE_KEY_1:
	case SE_KEY_2:	
	case SE_KEY_3:	
	case SE_KEY_4:	
	case SE_KEY_5:
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;
		nIndex  = (UINT8)(pEvent->EventID - SE_KEY_1 );
		if( nIndex >= nNumber )
			break;
		hControl = EQS_GetNthIOC((nIndex)*2+1);
	    hTemp = EQS_GetNthIOC((nIndex)*2);	
		STRCPY( szTemp, EQS_GetCurrentHeaderStr());

		pszBuf = EQC_GetStaticText(hTemp);  //2001.10.04
		if( pszBuf[0] == ' ' )
		{
			pszBuf++;
		}
		STRCAT( szTemp, pszBuf);
		
		index = STRLEN(szTemp);
		SET_ALPHAEDIT_BUFFER(szTemp, index);
		SET_ALPHAEDIT_POS(index);

		EQC_GetNumberEditValue( hControl, szTemp);
        sPBookSendCall(szTemp);
		break;		
	case SE_SOFTKEY_VIEW:
	case SE_KEY_CENTER:
        hControl = EQS_GetFocusedIOC();
		if( hControl != INVALID_CONTROL_HANDLE )
		{
			if( EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT )
			{					
				EQS_START_CHILD_EVENT( sSKA_ViewTelNumber, SE_APP_START, EQC_GET_USERDATA(hControl), EQS_NA);		
			}
			else if( EQC_GET_TYPE(hControl) == CT_IMAGE )
			{
				EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, EQS_NA, dwPicture ); 
			}
			else if( EQC_GET_TYPE(hControl) == CT_PUSH_BUTTON )
			{
				EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, EQS_NA, dwPicture);	
			}
			else
				;
		}
		break;
	case SE_KEY_CLEAR:		
		EQS_END(); 
		break;
    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

  
BOOL  sGetImageNumber(BYTE* pszFileName, DWORD* pNumber)
{
    INT8 nLen, nDigit;

    nLen = STRLEN(pszFileName);
    *pNumber = 0;
    if( pszFileName[nLen] == 0 )
    {     
        return FALSE;
    }
    nDigit = 1;
    while( nLen > 0 )
    {        
        *pNumber = *pNumber +(pszFileName[--nLen] - '0')*nDigit;
        nDigit *=10;
    }
    return TRUE;
      
}

BOOL IsValidImage(DWORD dwIndex)
{
    BYTE psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
    HFILE hFile;         

	if( dwIndex )
	{
		ConvertCameraImageFileName(psFileName, dwIndex); 
		if(SKY_OpenDataFile(SDF_PHOTO, psFileName, &hFile, SFT_READ) == SDR_OK)
		{
			SKY_CloseDataFile(hFile);
			return TRUE;
		}
	}
    return FALSE;
}


VOID SKY_API SKA_TestAnniv(SKY_EVENT_T* pEvent)
{
    pb_anniv_info_type AnnivInfo;
	int  i,j, count, total_count, Month;
	BYTE  szBuf[80];
	
    switch (pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_MENU_SELECT_NONE,
                      "ANNIV TEST", 
                      WA_SHOW_HEADER|WA_SHOW_CML);

        //EQC_StaticText(10,10, TKN_PBOOK_MAIN_MENU1 );
     
        break;
    case SE_APP_END:
        break;
   
	case SE_KEY_0:
	case SE_KEY_1:
	case SE_KEY_2:
	case SE_KEY_3:
	case SE_KEY_4:
	case SE_KEY_5:
	case SE_KEY_6:
	case SE_KEY_7:
	case SE_KEY_8:
	case SE_KEY_9:
	case SE_KEY_STAR:
	case SE_KEY_POUND:
		Month = pEvent->EventID - SE_KEY_0;
		total_count = MAX_ANNIV_NUMBER - active_list.nFreeAnniv;
		count = 0;
		AnnivInfo.address = active_list.anniv_info[Month];
		while( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) > 0 )
		{
			count++;
			AnnivInfo.address = AnnivInfo.link;
		}
		sprintf((CHAR*)szBuf,"%d월 : %d/%d", Month+1, count, total_count);

		EQC_MessageBox(szBuf, MESSAGE_DEFAULT_TIME,	MBT_NONE, MBT_HELP);
		break;

    case SE_SOFTKEY_MENU:
		total_count = MAX_ANNIV_NUMBER - active_list.nFreeAnniv;

		count  = 0;
		for( i =0; i< 12; i++ )
		{
			AnnivInfo.address = active_list.anniv_info[i];
			while( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) > 0 )
			{
				count++;
				AnnivInfo.address = AnnivInfo.link;
			}
		}
		sprintf((CHAR*)szBuf,"total  %d -  count %d", total_count, count);

		EQC_MessageBox(szBuf, MESSAGE_DEFAULT_TIME,	MBT_NONE, MBT_HELP);

        break;
	case SE_IO_MESGBOX_END:
		break;

    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }

}


VOID SKY_API SKA_BookMain(SKY_EVENT_T* pEvent)
{    
    static HCONTROL hHnd;
    static INT8 bCurrentLevel;
	HCONTROL hControl;
    int i;
    BYTE *PBookMenu[12];  
	//BYTE  szBuf[2][50];
	UINT8 Handle;
	CALLSTACK_T*    pCallStack;
	WORD  wData[6];
	BYTE* pText[6];
	
    
    switch( pEvent->EventID )
    {
    case SE_APP_START:

        PBookMenu[0] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU1);
        PBookMenu[1] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU2);
        PBookMenu[2] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU3);
        PBookMenu[3] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU4);
        PBookMenu[4] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU5);
        PBookMenu[5] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU6);
        PBookMenu[6] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU7);
        PBookMenu[7] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU8);
        PBookMenu[8] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU9);
        PBookMenu[9] = SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU10);
        PBookMenu[10] = (BYTE*)"화일닫기";
		PBookMenu[11] = (BYTE*)"anniv test";

        hHnd = EQC_ScrollList(10,10,13,6, PBookMenu, 12,12,0); 
        EQC_SetScrollTitle( hHnd, (BYTE*) "전화부");
        EQC_SET_STYLE( hHnd, CS_SCROLL_LIST_BORDER|\
			                 CS_SCROLL_LIST_ENUMBERATED| CS_SCROLL_LIST_MAXIMIZE|CS_SCROLL_LIST_STATUS_BAR);//\		
							 //CS_SCROLL_LIST_TAB|\
							 CS_SCROLL_LIST_COMBO); //CS_SCROLL_LIST_STATUS_BAR
		EQC_SetScrollStatusBarText(hHnd,"1234567890123456다한다");

#if 0
        EQC_SetScrollTab(hHnd, 6,3,FALSE,0);
		wData[0] = 3;
		wData[1] = 0;
		wData[2] = 3;
		wData[3] = 3;
		wData[4] = 2;
		wData[5] = 2;
		pText[0] = "클래식";
		pText[1] = "서비스";
		pText[2] = "효과음";
		pText[3] = "호호호";
		pText[4] = "하하하";
		pText[5] = "후후후";
		EQC_SetScrollTabData(hHnd, wData, &pText);
        
#endif
#if 0
		EQC_ShowScrollTitle(hHnd, TRUE );     		
		Handle = CreateList(2);
		AddList(Handle, 0, 0);
		AddList(Handle, 1, 0);
		pCallStack = GetCSKData();

		STRCPY( pCallStack->Stack[0].aDigits, (BYTE*)"0112935231");
		STRCPY( pCallStack->Stack[1].aDigits, (BYTE*)"2935230");
#endif


        i = sizeof(pb_group_info_type);
        i = sizeof(pb_book_info_type);
        i = sizeof(pb_email_info_type);
        i = sizeof(pb_anniv_info_type);
        i = sizeof(pb_number_info_type);
		i = sizeof(pb_active_list_type);
		i = sizeof(pb_sdial_type);
        //EIF_SetLCD("=1");
        //EQS_START_CHILD(SKA_MainEif);   
         //EQS_START_CHILD( SKA_ViewIRDAGroup );
         //EQS_START_CHILD(SKA_ViewIRDAList, 0,0);   

       
#if 0
        EQC_Time(10,100,0,TIME_MINUTE_CONTROL);
		
        EQC_Time(0,20,0,INFORM_TIME_CONTROL);
        hHnd = EQC_Time(0,40,0,TIME_TO_TIME_CONTROL);
		EQC_SetTimeEditMode(hHnd, FALSE);
        EQC_Time(0,0,0,TIME_TIME_CONTROL);
        EQC_Time(0,80,1,ENTIRE_TIME_CONTROL);
#endif
        //EQC_Time(0,100,1,DAY_TIME_CONTROL);
/*
        EQC_Time(0,80,4,CAL_TIME_CONTROL);
        EQC_Time(0,100,0,YEAR_MONTH_CONTROL);
*/

        //EQC_Time(0,80,0,MON_TIME_CONTROL);
        //EQC_Time(0,100,0,DAY_TIME_CONTROL);

        //EQC_Time(0,80,0,TIME_MINUTE_CONTROL);
        //EQC_SetTimeEditMode(EQC_Time(0,100,0,MONTH_DAY_CONTROL),FALSE);

       
        

        //EQC_Time(0,50,0,ALL_TIME_CONTROL);

        //SKY_MakeCameraImageFile(1234456);
        //IsValidImage(1234456);
        
/*
        EQS_START_CHILD(sSKA_ViewImagFile);
*/
        //EQS_START_CHILD_EVENT(SKA_ViewGroupList, SE_APP_START, EQS_NA, BFT_VIEW_EMAIL_LIST_E);
      
		//EQC_Time(1,80,0,TIME_TIME_CONTROL);
/*
		EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NOT_FUNCTION_IN_WAP),\
					   MESSAGE_DEFAULT_TIME,\
					   MBT_NONE, MBT_HELP);
*/
/*
		EQS_SetDirectDraw(TRUE);
		EQC_BeepSystemMessageBox((BYTE *)SKY_GET_TOKEN(TKN_CHARGING_NEEDED), 
                    2, 
                    MBT_NONE, 
                    MBT_SYSTEM);
*/



		
         break;
    case SE_KEY_F2:
		EQS_START_CHILD(SKA_BookInCalling);
		break;
	case SE_KEY_F2_LONG:
		i = EQS_GetNthIOC(1);
		break;
	case SE_TM_TIMER1:
		EQS_END();
		break;
    case SE_KEY_SEND:
		EQS_END();
		//EQS_SetWindowAttr(WA_SHOW_HEADER);
		//EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);

		//EQS_START_CHILD(SKA_ViewEmailList);
         break;
    case SE_APP_END:     
        break;     
	case SE_APP_DRAW:
		i = 10;
		break;

	case SE_IO_MESGBOX_END:
		i = 1;
		break;
/*
	case SE_KEY_1:
		EQS_START_SIBLING(sSKA_Test, TRUE);
		//EQS_StartTimer(EQS_TIMER_1, 3000, TRUE);
		break;
*/
/*
    case SE_KEY_1:
		EQS_StartTimer(EQS_TIMER_1, 3000, FALSE); // 3 sec.s			
		break;
    case SE_KEY_2:
		EQS_StopTimer(EQS_TIMER_1);
		break;
    case SE_TM_TIMER1:
		EQS_StopTimer(EQS_TIMER_1);
		break;
*/
	case SE_KEY_3:
		SKY_SetLocationTimer(5000, TRUE);
		break;
	case SE_KEY_4:
		SKY_SetLocationTimer(0, TRUE);
		break;
	case SE_KEY_5:
		EQS_START_CHILD( SKA_BookInCalling );
		break;
			
    case SE_LOCATION_TIMEOUT:
		EQS_START_CHILD(SKA_BookInCalling);
		break;
    case SE_APP_RESUME:
        EQS_SetIOCActive(EQS_GetNthIOC(0));

        break;

    case SE_IO_SPIN_CHG:
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
        break;

    case SE_IO_SCROLL_LIST_SELECT: 
		hControl = pEvent->wParam;
        bCurrentLevel = EQC_GetScrollPhysicalIndex(hControl);
        switch( bCurrentLevel )
        {
        case 0 :
           EQS_START_CHILD( SKA_BookInCalling );//SKA_SpeedSearch );         
           break;
        case 1 :
           EQS_START_CHILD( SKA_SearchName );
           break;
        case 2:
           EQS_START_CHILD( SKA_SearchNumber );           
           break;
        case 3:
           EQS_START_CHILD( SKA_BookInput );
           //EQS_START_CHILD( SKA_EditBook );
           break;
        case 4:
           EQS_START_CHILD( SKA_EditSDial );
           break;
        case 5:
            EQS_START_CHILD( SKA_EditGroup);           
           break;
        case 6:
            EQS_START_CHILD( SKA_BookTestInit );           
           break;
        case 7:
            EQS_START_CHILD( SKA_BookTestGroupSave );
           
           break;
        case 8:
            EQS_START_CHILD( SKA_BookTestMemberSave );
            break;     
        case 9:
            EQS_START_CHILD( SKA_BookTestDataSave );
            break;

        case 10:
            PBookClose();          
           break;
		case 11:
			EQS_START_CHILD( SKA_TestAnniv );
			break;
        default:
           break;
        }
    case SE_IO_SCROLL_LIST_BOTTOM:
         i = 1;
         break;
    
    case SE_IO_SCROLL_LIST_TOP:
         i = 0;
            break;
    case SE_IO_SCROLL_LIST_CHG:
         i = 2;            
		 break;
    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

VOID SKY_API SKA_ViewPhoneBookInfo(SKY_EVENT_T* pEvent)
{
	HCONTROL hControl;
	INT k, count;

    switch (pEvent->EventID)
    {
    case SE_APP_START:
		{
			MARGIN_T Margin;
			COORD    xSpace, ySpace;

			COORD    StartX, StartY;
			INT      nItem;
			UINT8    i, j;
			BYTE     szText[40];

			EQS_StartFormatter();   
			EQS_GetControlMargin(&Margin);
			EQS_GetControlSpace(&xSpace, &ySpace);
			EQS_EndFormatter();


			EQS_SetWindow(SKY_DEF_WIN_X,
				          SKY_DEF_WIN_Y, 
					      SKY_DEF_WIN_DX, 
						  400,//SKY_DEF_WIN_DY, 
						  CL_NONE_NONE_CANCEL,
						  SKY_GET_TOKEN(TKN_PBOOK_SAVE_INFO),
						  WA_SHOW_HEADER|WA_SHOW_CML);

			Margin.xLeft = 10;
			StartX = 0;//Margin.xLeft + xSpace;
			StartY = Margin.yTop  + ySpace;

			hControl = EQC_StaticText( StartX, StartY, TKN_PBOOK_USED_BOOK_INFO);
			//sprintf(szText,(char*)SKY_GET_TOKEN(TKN_PBOOK_USED_BOOK_INFO), MAX_BOOK_NUMBER-active_list.nFreeBook);
			if( active_list.nFreeBook > MAX_BOOK_NUMBER)
			{
				active_list.nFreeBook = MAX_BOOK_NUMBER;
			}		
			count = 0;
			for(k=0; k<MAX_BOOK_NUMBER; k++)
			{
				if( g_szPersonName[k][0] == 0 )
					count++;
			}
			active_list.nFreeBook = count;
			nItem = MAX_BOOK_NUMBER-active_list.nFreeBook;
			
			sprintf(szText,"저장인원:%4d/%4d", nItem, MAX_BOOK_NUMBER);
			EQC_SetStaticText(hControl, szText);
		    StartY += EQC_GET_DY(hControl) + ySpace;

			
			hControl = EQC_StaticText( StartX, StartY, TKN_PBOOK_USED_GROUP_INFO);			
			//sprintf(szText,(char*)SKY_GET_TOKEN(TKN_PBOOK_USED_GROUP_INFO), g_nGroup);
			if( g_nGroup > MAX_GROUP_NUMBER )
			{
				g_nGroup = MAX_GROUP_NUMBER;
			}

			sprintf(szText,"그룹    :%4d/%4d", g_nGroup, MAX_GROUP_NUMBER);
			EQC_SetStaticText(hControl, szText);
			//EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
		    StartY += EQC_GET_DY(hControl) + ySpace;
 
			StartX = 0;//Margin.xLeft + xSpace;
			hControl = EQC_StaticText( StartX, StartY, TKN_PBOOK_USED_NUMBER_INFO);			
			//sprintf(szText, (char*)SKY_GET_TOKEN(TKN_PBOOK_USED_NUMBER_INFO),(MAX_PHONE_NUMBER-active_list.nFreeNumber));
			if( active_list.nFreeNumber > MAX_PHONE_NUMBER )
			{
				active_list.nFreeNumber = MAX_PHONE_NUMBER;
			}
			nItem = MAX_PHONE_NUMBER - active_list.nFreeNumber;

			sprintf(szText,"전화번호:%4d/%4d", nItem, MAX_PHONE_NUMBER);
			EQC_SetStaticText(hControl, szText);
			//EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
			StartY += EQC_GET_DY(hControl) + ySpace;


			StartX   = 0;//Margin.xLeft + xSpace;
			hControl = EQC_StaticText( StartX, StartY, TKN_PBOOK_USED_EMAIL_INFO);			
			//sprintf(szText,(char*)SKY_GET_TOKEN(TKN_PBOOK_USED_EMAIL_INFO), (MAX_EMAIL_NUMBER-active_list.nFreeEMail));
			if( active_list.nFreeEMail > MAX_EMAIL_NUMBER )
			{
				active_list.nFreeEMail = MAX_EMAIL_NUMBER;
			}
			nItem = MAX_EMAIL_NUMBER - active_list.nFreeEMail;
			sprintf(szText,"메일계정:%4d/%4d", nItem, MAX_EMAIL_NUMBER);
			EQC_SetStaticText(hControl, szText);
			//EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
			StartY += EQC_GET_DY(hControl) + ySpace;


			StartX   = 0;//Margin.xLeft + xSpace;
			hControl = EQC_StaticText( StartX, StartY, TKN_PBOOK_USED_ANNIV_INFO);						
			//sprintf(szText,(char*)SKY_GET_TOKEN(TKN_PBOOK_USED_ANNIV_INFO),(MAX_ANNIV_NUMBER-active_list.nFreeAnniv));
            if( active_list.nFreeAnniv > MAX_ANNIV_NUMBER )
			{
				active_list.nFreeAnniv = MAX_ANNIV_NUMBER;
			}
			nItem = MAX_ANNIV_NUMBER - active_list.nFreeAnniv;
			sprintf(szText,"기념일  :%4d/%4d", nItem, MAX_ANNIV_NUMBER);
			EQC_SetStaticText(hControl, szText);
			//EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
			StartY += EQC_GET_DY(hControl) + ySpace;

#if 0
			StartX   = Margin.xLeft + xSpace;
			hControl = EQC_StaticText( StartX, StartY, TKN_PBOOK_USED_MEMO_INFO);
			StartX   += EQC_GET_DX(hControl) + xSpace;			
			hControl = EQC_StaticText( StartX, StartY, TKN_BLANK);
			StartY += EQC_GET_DY(hControl) + ySpace;
			sprintf(szText,"%3d/%3d",(MAX_MEMO_NUMBER-active_list.nFreeMemo), MAX_MEMO_NUMBER);
			EQC_SetStaticText(hControl, szText);
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));


			StartX   = Margin.xLeft + xSpace;
			hControl = EQC_StaticText( StartX, StartY, TKN_PBOOK_USED_PICTURE_INFO);
			StartX   += EQC_GET_DX(hControl) + xSpace;			
			hControl = EQC_StaticText( StartX, StartY, TKN_BLANK);
			StartY += EQC_GET_DY(hControl) + ySpace;
			sprintf(szText,"%3d/%3d",(MAX_BOOK_NUMBER-active_list.nFreeBook), MAX_BOOK_NUMBER);
			EQC_SetStaticText(hControl, szText);
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
#endif

			EQS_FitCanvasSize();
		
		}

     
        break;
    case SE_APP_END:
        break;
   
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

VOID SKY_API SKA_SpeedSearch(SKY_EVENT_T* pEvent)
{
	static BOOK_FUNCTION_E FunctionMode;
	static UINT8 GroupMenu;
    HCONTROL  hControl;
    UINT8     ListHandle, hList;
    int       index, nAddress, nSize, i, j;

    switch (pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_MENU_SELECT_NONE,
                      NULL, 
                      WA_SHOW_CML);
		GroupMenu = GEM_NON_FUNCTION;
		FunctionMode =  (BOOK_FUNCTION_E) pEvent->dwParam ;
		//FunctionMode = BFT_GET_GROUP_TEL_E;


        if( FunctionMode == BFT_SAVE_OLD_DATA_E )  // 기존 이름에 저장 
		{
			//if( IsFullPBook() == TRUE )
			if( active_list.nFreeNumber == 0 )
			{
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_MEMORY_MSG),
										  MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(hControl, SE_APP_END);
				return ;
			}
		}

		
        hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA,\
                                   EQC_GetScrollDisplayLine(),\
                                   0,(UINT8)0,(WORD)0,SDE_SPEED_GROUP);
        if( hControl == 0 )
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
            break;
        }

        ListHandle = EQC_GetScrollListHandle( hControl );
        if( ListHandle == 0 )
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
        }
        else
        {  
			
			switch( FunctionMode )
			{
            case  BFT_EDIT_PBOOK_E :   // 전화부 편집            
                EQC_SetScrollTitle( hControl, SKY_GET_TOKEN(TKN_PBOOK_EDIT)); 
				break;
            case  BFT_SEARCH_BOOK_E :  // 전화부 검색     
			case  BFT_GET_GROUP_TEL_E: // 동보메시지 
                EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_SEARCH));
				break;
            case BFT_EDIT_SDIAL_E :    // 단축다이얼 지정			
                EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_SET_SDIAL)); 
				EQS_SetWindowCML( CL_NONE_SELECT_NONE );
				break;
            case BFT_SAVE_OLD_DATA_E:  // 기존 이름에 저장 				
				EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_SAVE_OLD_DATA));				
				break;
            default:   // Speed Search                                  
                EQC_SetScrollTitle( hControl, SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU1));
				break;
            }    

#if 0
			if( FunctionMode == BFT_EDIT_PBOOK_E )
#endif
			{
				BYTE szBuf[21];
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
										CS_SCROLL_LIST_ENUMBERATED|\
										CS_SCROLL_LIST_MAXIMIZE );
										//CS_SCROLL_LIST_STATUS_BAR);
				sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_USAGE_MSG),\
						(MAX_BOOK_NUMBER - active_list.nFreeBook));
				EQC_SetScrollStatusBarText(hControl, szBuf);
			}
#if 0
			else
			{
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
										CS_SCROLL_LIST_ENUMBERATED|\
										CS_SCROLL_LIST_MAXIMIZE);
			}
#endif
            EQC_ShowScrollTitle(hControl, TRUE );         
            EQC_SetLogicalList(hControl,TRUE);     
            EQS_SetIOCActive( hControl );
#if BOOK_FLOW_MODE
			EQC_SetScrollFlowMode(hControl, TRUE);
#endif
			EQC_SET_USERDATA(hControl, FunctionMode);

			if( FunctionMode == BFT_GET_GROUP_TEL_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_ADD);
			}


        }        
        break;
    case SE_APP_END:
		FunctionMode = BFT_NON_FUNCTION_E;
		GroupMenu = GEM_NON_FUNCTION;
        break;
    case SE_APP_SUSPEND:
        break;
    case SE_APP_RESUME:
		hControl = EQS_GetNthIOC(0);
		FunctionMode = EQC_GET_USERDATA(hControl);

        index    = EQC_GetScrollLogicalIndex(hControl);
        ListHandle = EQC_GetScrollListHandle( hControl );
        if( EQS_GET_CHILD() ==  sSKA_AccessSecretGroup )
        {	
			if( GroupMenu == GEM_NON_FUNCTION )
			{
				if( pEvent->dwParam != 0 )
				{                
					switch( FunctionMode )
					{
					case  BFT_EDIT_PBOOK_E :   // 전화부 편집            
						EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
											   GetListValue( ListHandle, index ),\
											   BFT_EDIT_PBOOK_E);					
						break;
					case  BFT_SEARCH_BOOK_E :  // 전화부 검색             
						EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
											   GetListValue( ListHandle, index ),\
											   BFT_SEARCH_BOOK_E);
						break;
					case BFT_GET_GROUP_TEL_E:
						EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
											   GetListValue( ListHandle, index ),\
											   BFT_GET_GROUP_TEL_E);
						break;
					case BFT_EDIT_SDIAL_E :  // 단축다이얼 지정			
						EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
											   GetListValue( ListHandle, index ), \
											   BFT_EDIT_SDIAL_E);
						break;
					case BFT_SAVE_OLD_DATA_E:// 기존이름에 저장
						EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
											   GetListValue( ListHandle, index ), \
											   BFT_SAVE_OLD_DATA_E);
						break;
					default:   // Speed Search                                  
						EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
											   GetListValue( ListHandle, index ),\
											   EQS_NA);
						break;
					}   
				}
				else
				{
					EQS_SetIOCActive(hControl);
				}
			}
			else
			{
				if( pEvent->dwParam != 0 )
				{                
                     BYTE* Mesg[40];
 
                     sprintf(Mesg,SKY_GET_TOKEN(TKN_PBOOK_STRING_DELETE_ASK), \
                             EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl)));
                     EQC_MessageBox(Mesg, 10, MBT_YESNO, MBT_CONFIRM);

				}
				else
				{
					GroupMenu = GEM_NON_FUNCTION;
					EQS_SetIOCActive(hControl);
				}
			}
        }
        else
        {
			BYTE szBuf[21];
			sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_USAGE_MSG),\
					(MAX_BOOK_NUMBER - active_list.nFreeBook));
			EQC_SetScrollStatusBarText(hControl, szBuf);
            if( pEvent->dwParam != 0 )  // 전화부 검샏 ( BFT_SEARCH_BOOK_E )  or // 단축다이얼 지정(BFT_EDIT_SDIAL_E)
            {
				if( FunctionMode == BFT_SEARCH_BOOK_E ||\
					FunctionMode == BFT_EDIT_SDIAL_E  ||\
					FunctionMode == BFT_GET_GROUP_TEL_E )
				{
					EQS_END_EVENT(SE_APP_RESUME, 0, pEvent->dwParam);
				}
				break;
            }
			DeleteList(ListHandle);
            ListHandle = GetGroupList();
			EQC_SetScrollListHandle(hControl, ListHandle);
            nSize = GetListSize(ListHandle);
            for(i=0; i<nSize; i++)
            {
                if( GetNumOfGroupMember(GetListValue(ListHandle,i)) == 0 )
                {
                    RemoveListItem(ListHandle,i);
                    i--;
                    nSize--;
                }
            }
			//AddList(ListHandle, EQS_NA, EQS_NA); //전체리스트
			InsertList(ListHandle, 0, 0, EQS_NA); //<전체리스트>
			nSize++;	            
			EQC_SetScrollMaxDataLine(hControl, nSize);
			if( index < nSize )
			{
				EQC_SetScrollLogicalIndex(hControl, index);
			}
			else
			{
				EQC_SetScrollLogicalIndex(hControl, 0);
			}
			EQS_SetIOCActive(hControl);
        }		
      
        break;
	case SE_SOFTKEY_ADD:
		{
			pb_book_info_type BookInfo;
			pb_number_info_type NumberInfo;

			hControl = EQS_GetNthIOC(0);
			index    = EQC_GetScrollLogicalIndex(hControl);
			ListHandle = EQC_GetScrollListHandle( hControl );

			hList = GetBookList(GetListValue(ListHandle,index));

			for( i=GetListSize(hList)-1; i >=0;  i--)
			{
				nAddress = GetListValue(hList,i);
				for(j=0; j< PERSONAL_NUMBER_LIMIT; j++)
				{
					if( g_PersonNumberType[nAddress-1][j] == CELL )
						break;
				}
				if( j == PERSONAL_NUMBER_LIMIT )
				{
					RemoveListItem(hList,i);
				}
			}        
			//SortBookName(hList);
			ListHandle = CreateList(GetListSize(hList));
			for(i=0; i<GetListSize(hList); i++)
			{				
				BookInfo.address = GetListValue(hList,i);
				if( ui_get_pb(PB_BOOK_INFO_I, &BookInfo) > 0 )
				{
					index = 0;
					NumberInfo.address = BookInfo.number_address;
					while( index < PERSONAL_NUMBER_LIMIT )
					{						
						if( ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) > 0 )
						{	
							index++;
							if( NumberInfo.number_type == CELL )
							{
								AddList(ListHandle, NumberInfo.address, EQS_NA);
								break;
							}
							else
							{
								NumberInfo.address = NumberInfo.next_index;
							}
						}
						else
							break;
					}
				}
			}
			DeleteList(hList);
			EQS_END_EVENT(SE_APP_RESUME, EQS_NA, ListHandle);
		}
     
		break;
	case SE_SOFTKEY_MENU:
		{
			SKY_TOKEN paMenu[3] = 
			{                 
				TKN_PBOOK_GROUP_NAME_DELETE,      // 그룹이름 삭제
				TKN_PBOOK_GROUP_DELETE,			  // 그룹 삭제
				TKN_PBOOK_VIEW_SAVE_INFO          // 전화부 정보보기 
			};
			BOOL fActiveMenu[3] = { TRUE, TRUE, TRUE };

			
			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);    
			index = EQC_GetScrollLogicalIndex(hControl);
			if( index == 0 )  //전체이름 
			{
				fActiveMenu[0] = FALSE;
				fActiveMenu[1] = FALSE;
			}
			else
			{
				index = GetListValue(ListHandle, index);
				if( KOREAN_STRCMP( g_szGroupName[index-1] , SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) ) == 0 )
				{
					fActiveMenu[0] = FALSE;
					fActiveMenu[1] = FALSE;
				}
			}
			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 3, 0, fActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
		}
		
		break;
    case SE_IO_POPUP_SELECT:   
        hControl = EQS_GetNthIOC(0);
        ListHandle = EQC_GetScrollListHandle(hControl);		
        switch( pEvent->dwParam )
        {    
		case TKN_PBOOK_GROUP_NAME_DELETE:      // 그룹이름 삭제             
            index = GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl));
            if( KOREAN_STRCMP( g_szGroupName[index-1] , SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) ) == 0 )
            {     
				GroupMenu = GEM_INFO_MSG;
                EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_INVALID_DELETE_GROUP_MSG),\
                               MESSAGE_DEFAULT_TIME,\
                               MBT_NONE, MBT_HELP);
            }
			else
			{
				GroupMenu = GEM_DELETE_GROUP_NAME;
                if( IS_SECRET_GROUP(index) == TRUE )
                {
                    EQS_START_CHILD(sSKA_AccessSecretGroup);
                }
                else
                {
                     BYTE* Mesg[40];								    
					 sprintf(Mesg,SKY_GET_TOKEN(TKN_PBOOK_GROUP_NAME_DELETE_ASK), \
						     EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl)));					 
                     EQC_MessageBox(Mesg, 10, MBT_YESNO, MBT_CONFIRM);

				}
			}
			break;
        case TKN_PBOOK_GROUP_DELETE:           // 그룹 삭제               
            index = GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl));
            if( KOREAN_STRCMP( g_szGroupName[index-1] , SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) ) == 0 )
            {       
				GroupMenu = GEM_INFO_MSG;
                EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_INVALID_DELETE_GROUP_MSG),\
                               MESSAGE_DEFAULT_TIME,\
                               MBT_NONE, MBT_HELP);
            }
			else
			{
				GroupMenu = GEM_DELETE_GROUP;
                if( IS_SECRET_GROUP(index) == TRUE )
                {
                    EQS_START_CHILD(sSKA_AccessSecretGroup);
                }
                else
                {
                     BYTE* Mesg[40];						 						
					 sprintf(Mesg,SKY_GET_TOKEN(TKN_PBOOK_STRING_DELETE_ASK),\
							 EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl)));
                     hControl = EQC_MessageBox(Mesg, 10, MBT_YESNO, MBT_CONFIRM);
					 EQC_SET_USERDATA(hControl, pEvent->dwParam);
				}
			}
			break;
		case TKN_PBOOK_VIEW_SAVE_INFO:   //전화부 정보 보기 
			EQS_START_CHILD( SKA_ViewPhoneBookInfo );
			break;

		default:
			break;
		}
		break;

	case SE_IO_MESGBOX_CLEAR:
	case SE_IO_MESGBOX_CANCEL:
		EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;
    case SE_IO_MESGBOX_END:
		if( GroupMenu == GEM_NON_FUNCTION )
		{
			EQS_END();
		}
		else if( GroupMenu == GEM_INFO_MSG )
		{
			;
		}
		else
		{
            hControl = EQS_GetNthIOC(0);
            ListHandle = EQC_GetScrollListHandle(hControl);
            index =  EQC_GetScrollLogicalIndex(hControl);
            nAddress = GetListValue( ListHandle, index);
                                          
			if( GroupMenu == GEM_DELETE_GROUP ) // 그룹 삭제 
			{
				BYTE szBuf[21];

	            DeleteGroup( nAddress, TRUE );				
				sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_USAGE_MSG),\
						(MAX_BOOK_NUMBER - active_list.nFreeBook),MAX_BOOK_NUMBER);
				EQC_SetScrollStatusBarText(hControl, szBuf);

			}
			else                //GroupMenu == GEM_DELETE_GROUP_NAME 그룹 이름 삭제 
			{
				DeleteGroup( nAddress, FALSE );
			}
            PhoneBookUpdate();
            RemoveListItem( ListHandle,index);
            
            index = EQC_DeleteScrollData( hControl, EQC_GetScrollLogicalIndex( hControl));

            if( EQC_GetScrollMaxDataLine(hControl) == 0 )
            {
                EQS_END();
                break;
            }

            for( i= 0; i< EQC_GetScrollViewLine(hControl); i++)
            {
                sMakeFormattedString(EQC_GetScrollData(hControl, i),\
                                     ListHandle, index+i, SDE_SPEED_GROUP);  
            }


		}
		EQS_SetIOCActive(EQS_GetNthIOC(0));
		GroupMenu = GEM_NON_FUNCTION;
        break;
    
    case SE_IO_MESGBOX_NO:    // 그룹 삭제 NO
		GroupMenu = GEM_NON_FUNCTION;
		EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;
	case SE_IO_MESGBOX_YES:   // 그룹 삭제 OK
		if( GroupMenu )
		{               			
			hControl =  EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_DELETING),\
                                       0, MBT_NONE, MBT_HELP);            
		}
		break;
   
    case SE_IO_SCROLL_LIST_SELECT:
    
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );
		GroupMenu = GEM_NON_FUNCTION;
        		
        if( index == 0 || IS_SECRET_GROUP( GetListValue(ListHandle, index) ) == FALSE )
        {  //전체 이름이거나 비밀 그룹이 아닌 경우 
			switch( FunctionMode )
			{
			case  BFT_EDIT_PBOOK_E :   // 전화부 편집            
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
                                       GetListValue( ListHandle, index ),\
                                       BFT_EDIT_PBOOK_E);					
				break;
			case  BFT_SEARCH_BOOK_E :  // 전화부 검색             
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
                                       GetListValue( ListHandle, index ),\
                                       BFT_SEARCH_BOOK_E);
				break;
			case BFT_GET_GROUP_TEL_E:
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
                                       GetListValue( ListHandle, index ),\
                                       BFT_GET_GROUP_TEL_E);
				break;
			case BFT_EDIT_SDIAL_E :  // 단축다이얼 지정			
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
                                       GetListValue( ListHandle, index ), \
                                       BFT_EDIT_SDIAL_E);
				break;
			case BFT_SAVE_OLD_DATA_E:// 기존이름에 저장
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
                                       GetListValue( ListHandle, index ), \
                                       BFT_SAVE_OLD_DATA_E);
				break;
			default:   // Speed Search                                  
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START,\
                                       GetListValue( ListHandle, index ),\
                                       EQS_NA);
				break;
			}  
        }
        else
		{
            EQS_START_CHILD( sSKA_AccessSecretGroup);
		}
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            TRUE, SDE_SPEED_GROUP);
        break;

    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            FALSE, SDE_SPEED_GROUP);
        break;
    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

enum tagSEARCH_NAME_CONTROL_E 
{    
  SNC_IMAGE_E =0,
  SNC_TEXT_EDIT_E
};
typedef enum tagSEARCH_NAME_CONTROL_E  SEARCH_NAME_E;


VOID SKY_API SKA_SearchName(SKY_EVENT_T* pEvent)
{
    HCONTROL hControl;
    INT8     ListHandle;
    BYTE     bTemp[40];
    //BYTE     szString[40];
    INT16    size;

    switch (pEvent->EventID)
    {
    case SE_APP_START:        

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY,
            CL_NONE_CONFIRM_NONE, 
            SKY_GET_TOKEN(TKN_NAME_SEARCH), 
            WA_SHOW_HEADER|WA_SHOW_CML);

		if( active_list.nFreeBook == MAX_BOOK_NUMBER )
		{
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
		}
		else
		{      

			//EQC_Image( 40, 11, (BITMAP*)&g_BmpSeekPbook40x56x256c);		
			EQC_Image( 40, 11, (BITMAP*)&g_BmpFindData);
			hControl  = EQC_TextEdit( 14, 72, 14,1,
									 TEM_KOREAN, MAX_BOOK_NAME-2, (BYTE*)"", 0);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				                    CS_TEXT_EDIT_NO_CCML    |\
									CS_TEXT_EDIT_ALONE      );
			EQS_SetIOCActive(hControl);
		}

        break;
    case SE_APP_END:
    
        break;
    case SE_IO_TEDIT_DONE:

		EQC_GetTextEditData( pEvent->wParam, bTemp );
		if( STRLEN(bTemp) == 0 )
		{
#if 0
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_INPUT_NAME_MSG),\
									  MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
			EQC_SET_USERDATA(hControl, SE_USER_2);
#endif
			EQS_START_SIBLING_EVENT( SKA_ViewBookList, SE_APP_START,0 , EQS_NA, FALSE); //전체 이름 
		}
		else
		{          
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_SEARCHING), 0, MBT_NONE,MBT_HELP);
			EQC_SET_USERDATA(hControl, SE_USER_1);
		}
	
        break;    
    case SE_IO_TEDIT_CANCEL:
        EQS_END();
        break;
    case SE_IO_MESGBOX_END:                
        if( pEvent->dwParam == SE_USER_1 )
        {           
            hControl = EQS_GetNthIOC(SNC_TEXT_EDIT_E);
            EQC_GetTextEditData( hControl, bTemp );
            
            ListHandle = SearchName(bTemp,BSM_NAME_PART_MATCH);
            //2001.01.08
            size = GetListSize( ListHandle );  
            if( size == 0 )  // 메시지 출력후 종료
            {
                //sprintf(szString,SKY_GET_TOKEN(TKN_PBOOK_STRING_NO_DATA_MATCH), bTemp );
                //hControl = EQC_MessageBox(szString, MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);				
                hControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_PBOOK_NO_SEARCH_NAME_MSG),\
					                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hControl, SE_USER_2);                                         
            }
            else
            {
				SortBookName(ListHandle);
                EQS_START_SIBLING_EVENT( SKA_ViewSearchName, SE_APP_START,\
                                         (WORD) ListHandle, 0, FALSE );
            }
            break;
        }
        else if( pEvent->dwParam == SE_USER_2 )
            EQS_SetIOCActive(EQS_GetNthIOC(SNC_TEXT_EDIT_E));
		else
			EQS_END();

        break;

    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

enum tagSEARCH_NUMBER_CONTROL_E 
{    
  SNUMC_IMAGE_E =0,
  SNUMC_NUMBER_EDIT_E
};
typedef enum tagSEARCH_NUMBER_CONTROL_E  SEARCH_NUMBER_E;

VOID  SKA_SearchNumber(SKY_EVENT_T* pEvent)
{
    INT8 ListHandle ;
    HCONTROL hControl;
    BYTE   szNumber[40];

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_CANCEL_CONFIRM_NONE, 
            SKY_GET_TOKEN(TKN_NUMBER_SEARCH), 
            WA_SHOW_HEADER|WA_SHOW_CML);

		if( active_list.nFreeBook == MAX_BOOK_NUMBER )
		{
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
		}
		else
		{
			//EQC_Image( 40, 11, (BITMAP*)&g_BmpSeekPbook40x56x256c);
			EQC_Image( 40, 11, (BITMAP*)&g_BmpFindData);
			hControl = EQC_NumberEdit(8, 72,16,1,14,(BYTE*)"");
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl)|\
				                    CS_NUMBER_EDIT_NO_CML  |\
									CS_NUMBER_EDIT_ALONE     );
			EQS_SetIOCActive( hControl );
		}
        break;
    case SE_APP_END:
   
        break;

    case SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue( pEvent->wParam, szNumber);
        if( STRLEN(szNumber) == 0 )
        {
#if 0
            hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_INPUT_NUMBER_MSG),\
                           MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
			hControl = EQC_SET_USERDATA(hControl, SE_USER_2);  // no data
			EQC_SET_USERDATA(hControl, SE_USER_2 );
#endif
            EQS_START_SIBLING_EVENT( SKA_ViewBookList, SE_APP_START,0 , EQS_NA, FALSE); //전체 이름 

        }
        else
        {            
            hControl =  EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_SEARCHING),\
									   0, MBT_NONE,MBT_HELP);
			EQC_SET_USERDATA(hControl, SE_USER_1 );  // Searching
        }
        break;    
    case SE_IO_NEDIT_CANCEL:
        EQS_END();
        break;     
    case SE_IO_MESGBOX_END:
        if( pEvent->dwParam == SE_USER_1 )
        {            
            hControl = EQS_GetNthIOC(SNUMC_NUMBER_EDIT_E);
            EQC_GetNumberEditValue( hControl, szNumber);
            ListHandle = SearchNumber(szNumber,BSM_NUMBER_PART_MATCH);
            if( ListHandle == 0 )  // 찾는 번호 없음 메시지 출력후 상위로 이동 
            {
               hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_SEARCH_NUMBER_MSG),\
				                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			   EQC_SET_USERDATA(hControl, SE_USER_2);
               break;
            }               
            else
            {
                SortBookName(ListHandle);
                EQS_START_SIBLING_EVENT( SKA_ViewSearchNumber, SE_APP_START,\
                                         (WORD) ListHandle, 0, FALSE );
            }
            break;
        }
        else if( pEvent->dwParam == SE_USER_2 )
            EQS_SetIOCActive(EQS_GetNthIOC(SNUMC_NUMBER_EDIT_E));
		else
			EQS_END();        
        break;
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

enum tagVIEW_SEARCH_NAME_MSG_E
{     
    VIEW_SEARCH_NAME_DELETE_MSG_E = 1,
	VIEW_SEARCH_NAME_INFO_MSG_E,
	VIEW_SEARCH_NAME_ASK_DELETE_MSG_E
};


// use dynamic data list  -- possible have many data
VOID SKY_API SKA_ViewSearchName( SKY_EVENT_T* pEvent )
{
    UINT8    ListHandle;        
    HCONTROL hControl;
    INT16    i, nIndex, index, nDeletePos, nDeleteAddress;
	BYTE     szTemp[40];

    pb_group_info_type   GroupInfo;
    pb_book_info_type    BookInfo; 


    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_MENU_SELECT_NEWNAME, 
                      "",
                      WA_SHOW_CML);

        hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA,\
                                   EQC_GetScrollDisplayLine(), 0, \
                                   (UINT8) 0,pEvent->wParam,SDE_SEARCH_NAME); 
        if( hControl == 0 )
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SEARCH_FAIL_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
            break;
        }
        ListHandle = EQC_GetScrollListHandle( hControl );
        if( ListHandle != 0 )
        {
			BYTE szBuf[20];
            EQC_SetLogicalList(hControl, TRUE);    //redstar   00.12.13
            EQC_SetScrollTitle( hControl, SKY_GET_TOKEN(TKN_PBOOK_SEARCH_NAME_RESULT));
            EQC_ShowScrollTitle(hControl, TRUE );
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | \
                                    CS_SCROLL_LIST_MAXIMIZE   ); //CS_SCROLL_LIST_STATUS_BAR
			sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
				    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);
			EQC_SetScrollStatusBarText(hControl, szBuf);
            EQS_SetIOCActive( hControl );
        }
        else
        {
            EQS_END();
        }        
        break;
    case SE_APP_END:
    
        break;
    case SE_APP_RESUME:
        hControl = EQS_GetNthIOC(0);
        EQS_SetIOCActive(hControl);

        if( EQS_GET_CHILD() == SKA_EditBookContent )
        {
            ListHandle = EQC_GetScrollListHandle(hControl);
            index = EQC_GetScrollLogicalIndex(hControl);

            if( pEvent->dwParam )  // book address
            {
				WORD  nSize;
				INT16 nNewBookAddress, nFind;

				if( pEvent->wParam )
				{ // overwrite 인 경우 
					nFind = FindList( ListHandle, pEvent->wParam );
					if( nFind >= 0 )
					{
						RemoveListItem(ListHandle, nFind);
						EQC_SetScrollMaxDataLine(hControl, GetListSize(ListHandle) );
					}
				}
				else
				{
					nFind = -1;
				}

				RemoveListItem(ListHandle, index);
				nSize = GetListSize(ListHandle);
				nNewBookAddress = pEvent->dwParam;

				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
				{
					UINT8 nTab;
					EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);					
					nTab = GetKoreanCho(g_szPersonName[nNewBookAddress-1]);
					if( nFind < 0 )
					{
						EQC_SetScrollTabDataSize(hControl, nTab, EQC_GetScrollTabDataSize(hControl, nTab)+1);
					}
				}
				
				for(i=0; i<nSize; i++)
				{
					if( KOREAN_STRCMP( g_szPersonName[nNewBookAddress-1],\
									   g_szPersonName[GetListValue(ListHandle, i)-1] ) <= 0 )
					{
						InsertList(ListHandle, i, nNewBookAddress, EQS_NA);
						break;
					}
				}
				if( i == nSize )
				{
					AddList(ListHandle, nNewBookAddress, EQS_NA);
				}				
				EQC_SetScrollLogicalIndex(hControl, i);
            }
			else
			{//아이템이 변경되었을 경우 이를 화면에 반영하기 위해 
				EQC_SetScrollLogicalIndex(hControl, index);
			}
        }
		else if( EQS_GET_CHILD() == SKA_ViewBookContent )
		{

			ListHandle = EQC_GetScrollListHandle(hControl);
			index = EQC_GetScrollLogicalIndex(hControl);
			if( pEvent->dwParam )  //전화부 삭제 
			{
				RemoveListItem(ListHandle,index);                   
				if( GetListSize(ListHandle) == 0 )
				{ 
					EQS_END();  //01.03.25   for refreshing speed search information
					return ;
				}
				index = EQC_DeleteScrollData(hControl, index );

				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
				{
					EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);
					index = EQC_GetScrollLogicalIndex(hControl);
					EQC_SetScrollLogicalIndex(hControl, index);
				}
				else
				{
					sPBookDisplayScroll(hControl, index,TRUE, SDE_SEARCH_NAME);
				}
			}
			else// 부분 항목  삭제의  경우 
			{
				EQC_SetScrollLogicalIndex(hControl, index);
				EQS_SetIOCActive(hControl);
				break;            
			}			
			
		}
		else if( EQS_GET_CHILD() == SKA_BookInput )
		{

            if( pEvent->dwParam )  // book address
            {
				WORD  nSize;
				INT16 nNewBookAddress, nFind;

                ListHandle = EQC_GetScrollListHandle(hControl);
                index = EQC_GetScrollLogicalIndex(hControl);
				nNewBookAddress = pEvent->dwParam;

				if( pEvent->wParam )
				{ // overwrite 인 경우 
					nFind = FindList( ListHandle, pEvent->wParam );
					if( nFind >= 0 )
					{
						SetListValue(ListHandle, nFind, nNewBookAddress);
						EQC_SetScrollLogicalIndex(hControl, nFind);
						break;
					}
				}
                // overwrite가 아닌 경우 또는 overwrite가 여기 속하지 않는 경우 			
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
				{
					UINT8 nTab;					
					nTab = GetKoreanCho(g_szPersonName[nNewBookAddress-1]);
					EQC_SetScrollTabDataSize(hControl, nTab, EQC_GetScrollTabDataSize(hControl, nTab)+1);						
				}

				nSize = GetListSize(ListHandle);
				for(i=0; i<nSize; i++)
				{
					if( KOREAN_STRCMP( g_szPersonName[nNewBookAddress-1],\
									   g_szPersonName[GetListValue(ListHandle, i)-1] ) <= 0 )
					{
						InsertList(ListHandle, i, nNewBookAddress, EQS_NA);
						break;
					}
				}
				if( i == nSize )
				{
					AddList(ListHandle, nNewBookAddress, EQS_NA);
				}				
			    EQC_SetScrollMaxDataLine(hControl, GetListSize(ListHandle));
				EQC_SetScrollLogicalIndex(hControl, i);
            }
		}
		else if( EQS_GET_CHILD() == SKA_SelectNumber )
		{
			if( pEvent->wParam == TKN_PBOOK_SET_SDIAL_SELECT_MSG )
			{
                EQS_START_CHILD_EVENT(SKA_EditSDial, SE_APP_START, (WORD)pEvent->dwParam, EQS_NA);
				break;
			}
			else if( pEvent->wParam == TKN_PBOOK_SEND_SMS_SELECT_MSG )
			{
			   pb_number_info_type numberinfo;
           
			   numberinfo.address = pEvent->dwParam;
			   ui_get_pb( PB_NUMBER_INFO_I, &numberinfo);
			   nIndex = STRLEN(numberinfo.digits);
			   memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T)); 
			   STRCPY(g_UIMOBuffer.pszRcvNumber, numberinfo.digits);
			   g_UIMOBuffer.nRcvNumberSize = nIndex;
			   EQS_START_CHILD(SKA_SMSEditMessage);
			   break;
			}
			else ;
		}
		else
		{
			;
		}

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		{
			BYTE szBuf[20];
			sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
				   ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
			EQC_SetScrollStatusBarText(hControl, szBuf);
		}

        break;

	case SE_SOFTKEY_NEWNAME:
		EQS_START_CHILD_EVENT( SKA_BookInput, SE_APP_START, EQS_NA, EQS_NA );
		break;    

    case SE_SOFTKEY_MENU:
        {
            SKY_TOKEN paMenu[3] =
            {
				TKN_PBOOK_SEND_SMS,         //메시지 작성 
				TKN_PBOOK_SET_SDIAL,        //단축다이얼 지정
                TKN_PBOOK_ONE_BOOK_DELETE, //전화부 삭제 
                
            };
			BOOL  fActiveMenu[3];
			for( i=0; i<3; i++)
			{
				fActiveMenu[i] = TRUE;
			}
			hControl = EQS_GetNthIOC(0);
            ListHandle = EQC_GetScrollListHandle(hControl);
            BookInfo.address  = GetListValue(ListHandle, \
                                         EQC_GetScrollLogicalIndex(hControl));

			for( i=0; i< PERSONAL_NUMBER_LIMIT; i++ )
			{
				if( g_PersonNumberType[BookInfo.address-1][i] )
					break;
			}

			if( l_fFromCall )
			    fActiveMenu[0] = FALSE;

			if( i == PERSONAL_NUMBER_LIMIT )
			{
				fActiveMenu[0] = FALSE;
				fActiveMenu[1] = FALSE;
			}
			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 3, 0, fActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );	
        }  
        break;
    case SE_IO_POPUP_CANCEL:
        hControl = EQS_GetNthIOC(0);
        EQS_SetIOCActive(hControl);
        break;   
       
    case SE_IO_POPUP_SELECT:               
		hControl = EQS_GetNthIOC(0);
        switch( pEvent->dwParam )
        {
        case TKN_PBOOK_ONE_BOOK_DELETE: // 전화부 이름 삭제 
			{
				BYTE szBuf[100];
#if 0				
				hControl = EQS_GetNthIOC(0);
				index =  EQC_GetScrollPhysicalIndex(hControl);
				STRCPY( szBuf, EQC_GetScrollData(hControl, index));
				STRCAT( szBuf,(BYTE*)"\n");
				STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));
#endif
				ListHandle = EQC_GetScrollListHandle(hControl);
				BookInfo.address  = GetListValue(ListHandle, \
											 EQC_GetScrollLogicalIndex(hControl));

				STRCPY( szBuf, g_szPersonName[BookInfo.address-1]);
				STRCAT( szBuf,(BYTE*)"\n");
				STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));

				hControl = EQC_MessageBox(szBuf, 10, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(hControl, VIEW_SEARCH_NAME_ASK_DELETE_MSG_E);
			}
            break;
        case TKN_PBOOK_SET_SDIAL:  // 단축다이얼 지정
            ListHandle = EQC_GetScrollListHandle(hControl);
            BookInfo.address  = GetListValue(ListHandle, \
                                         EQC_GetScrollLogicalIndex(hControl));
			EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
								  BookInfo.address,\
								  TKN_PBOOK_SET_SDIAL_SELECT_MSG);	
            break;
		case TKN_PBOOK_SEND_SMS:  //메시지 작성 
            if(SKY_IS_SMS_LOCK() && pEvent->wParam !=SMSCODE_IN_VIEW_SEARCH_NAME )
            {
                EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_IN_VIEW_SEARCH_NAME, EQS_NA);
            }else
            {
                ListHandle = EQC_GetScrollListHandle(hControl);
                BookInfo.address  = GetListValue(ListHandle, \
                                             EQC_GetScrollLogicalIndex(hControl));
			    EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
								      BookInfo.address,\
								      TKN_PBOOK_SEND_SMS_SELECT_MSG);
            }
			break;
        default:
            break;
        }
        
        break;


	case SE_IO_MESGBOX_END:  
		hControl = EQS_GetNthIOC(0);
		if( pEvent->dwParam == VIEW_SEARCH_NAME_DELETE_MSG_E) // "삭제중 " 메시지 
		{			
            ListHandle = EQC_GetScrollListHandle(hControl);
            index =  EQC_GetScrollLogicalIndex(hControl);
                       
            BookInfo.address = GetListValue( ListHandle, index );
            if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
                break;

            GroupInfo.address = BookInfo.group_address;
            if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) == 0)
                break;    
            
            DeleteBook(&GroupInfo, &BookInfo, TRUE); //2001.01.08
			ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
			PhoneBookUpdate();
           

            RemoveListItem( ListHandle,index);            
            nDeletePos = EQC_DeleteScrollData( hControl, EQC_GetScrollLogicalIndex( hControl));            

            if( EQC_GetScrollMaxDataLine(hControl) == 0 )
            {
                EQS_END();
                break;
            }
            sPBookDisplayScroll((HCONTROL)hControl, (WORD)nDeletePos,\
                                TRUE, SDE_SEARCH_NAME);

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
			{
				BYTE szBuf[20];
				sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
					    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
				EQC_SetScrollStatusBarText(hControl, szBuf);
			}
			hControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG),\
				                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_SEARCH_NAME_INFO_MSG_E);
		}
		else  // VIEW_SEARCH_NAME_INFO_MSG_E
		{
			EQS_SetIOCActive(hControl);	
		}
		break;	

	case SE_IO_MESGBOX_NO:

		hControl = EQS_GetNthIOC(0);
		EQS_SetIOCActive(hControl);	
		
		break;
	case SE_IO_MESGBOX_YES:
		if( pEvent->dwParam == VIEW_SEARCH_NAME_ASK_DELETE_MSG_E )
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_DELETING),\
				                      0, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_SEARCH_NAME_DELETE_MSG_E);
		}
		else
		{
			hControl = EQS_GetNthIOC(0);
			EQS_SetIOCActive(hControl);	
		}
		break;


    case SE_IO_SCROLL_LIST_SELECT:
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );
   
        EQS_START_CHILD_EVENT( SKA_ViewBookContent, SE_APP_START, 
                             GetListValue( ListHandle, index ), BFT_NON_FUNCTION_E);
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            TRUE, SDE_SEARCH_NAME);
        break;
    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            FALSE, SDE_SEARCH_NAME);
        break;
    case SE_IO_KEY_1:       
    case SE_IO_KEY_2:       
    case SE_IO_KEY_3:        
    case SE_IO_KEY_4:       
    case SE_IO_KEY_5:
		//if(SKY_IsInCall() || SKY_IS_IN_WAP() )
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );   
        BookInfo.address = GetListValue(ListHandle,index);
        index  = pEvent->EventID - SE_IO_KEY_0;
        if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) )
        {
            pb_number_info_type NumberInfo;
            NumberInfo.address = BookInfo.number_address;
            while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) > 0 )
            {
				index--;
                if( index == 0 )
                {
					break;
				}
				else
				{
                    NumberInfo.address = NumberInfo.next_index;
                }

            }
			if( index > 0 )
				break;
			sprintf((CHAR*) szTemp,(CHAR*) SKY_GET_TOKEN(TKN_PBOOK_FMT_NAME_TYPE),	
                                                g_szPersonName[NumberInfo.book_address-1],\
												phone_type[NumberInfo.number_type-1]);
			index = STRLEN(szTemp) ;
			SET_ALPHAEDIT_BUFFER(szTemp, index);
			SET_ALPHAEDIT_POS(index);

            sPBookSendCall( NumberInfo.digits);
        }
		break;
	case SE_IO_KEY_POUND:
		{
			int nSize;
			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);
			nSize = EQC_GetScrollMaxDataLine(hControl);

			index = EQC_GetScrollLogicalIndex(hControl);
			for( i = index+1; i < nSize; i++ )
			{
				if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
					                 g_szPersonName[GetListValue(ListHandle, i    )-1]))
				{
					break;
				}
			}
			if( i == nSize )
			{
				for( i = 0; i < index; i++ )
				{
					if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
										 g_szPersonName[GetListValue(ListHandle, i    )-1]))
					{
						break;
					}
				}
			}

			if( i == index )
				break;
			EQC_SetScrollLogicalIndex(hControl, i);
			sPBookDisplayScroll(hControl, i, TRUE , SDE_SEARCH_NAME);
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
	case SE_IO_KEY_STAR:
		{
			int nSize;
			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);
			nSize = EQC_GetScrollMaxDataLine(hControl);

			index = EQC_GetScrollLogicalIndex(hControl);
			for( i = index-1; i >= 0; i-- )
			{
				if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
					                 g_szPersonName[GetListValue(ListHandle, i    )-1]))
				{
					break;
				}
			}
			if( i < 0 )
			{
				for( i = nSize-1; i  > index; i-- )
				{
					if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
										 g_szPersonName[GetListValue(ListHandle, i    )-1]))
					{
						break;
					}
				}
			}

			if( i == index )
				break;

			EQC_SetScrollLogicalIndex(hControl, i);                    
			sPBookDisplayScroll(hControl, i, TRUE , SDE_SEARCH_NAME);
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;		

    case SE_KEY_SEND:
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;
        sPBookMakeCall(EQS_GetNthIOC(0));   
        break;

    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }

}

enum tagVIEW_SEARCH_NUMBER_MSG_E
{     
    VIEW_SEARCH_NUMBER_DELETE_MSG_E = 1,
	VIEW_SEARCH_NUMBER_INFO_MSG_E,
	VIEW_SEARCH_NUMBER_ASK_DELETE_MSG_E
};

VOID SKY_API SKA_ViewSearchNumber(SKY_EVENT_T* pEvent)
{
    UINT8    nIndex, ListHandle;    
    HCONTROL hControl;
    INT16    i,index, nDeletePos, nDeleteAddress;
    pb_group_info_type  GroupInfo;
    pb_book_info_type   BookInfo;
	BYTE     szTemp[40];

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        if( pEvent->dwParam == BFT_SEARCH_BOOK_E )  // 전화부 검색 (sms 등에서 번호 검색을 통해 정보를 얻고자 하는 경우 
        {
			EQS_SetWindow(SKY_DEF_WIN_X,
						  SKY_DEF_WIN_Y, 
						  SKY_DEF_WIN_DX, 
						  SKY_DEF_WIN_DY, 
						  CL_NONE_SELECT_NONE,
						  SKY_GET_TOKEN(TKN_PBOOK_SEARCH),
						  WA_SHOW_HEADER|WA_SHOW_CML);
        }
        else
        {
			EQS_SetWindow(SKY_DEF_WIN_X,
						  SKY_DEF_WIN_Y, 
						  SKY_DEF_WIN_DX, 
						  SKY_DEF_WIN_DY, 
						  CL_MENU_SELECT_NEWNAME, 
						  SKY_GET_TOKEN(TKN_PBOOK_SEARCH_NUMBER_RESULT),
						  WA_SHOW_HEADER|WA_SHOW_CML);
        }

        hControl   = sMakeScrollList(EQS_NA,EQS_NA, EQS_NA, \
                                     EQC_GetScrollDisplayLine(), 0, \
                                     (UINT8)0,pEvent->wParam,SDE_SEARCH_NUMBER); 
        if( hControl == 0 )
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
            break;
        }
        ListHandle = EQC_GetScrollListHandle( hControl );
        if( ListHandle != 0 )
        {        
#if 0
			BYTE* pTabText[15] = \
			{ "AZ","ㄱ","ㄴ","ㄷ","ㄹ","ㅁ","ㅂ","ㅅ","ㅇ","ㅈ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ"};
			WORD  TabData[15] = {0,0,0,0,0, 0,0,0,0,0,  0,0,0,0,0};
#endif
			EQC_SetLogicalList(hControl, TRUE);    //redstar   00.12.13

			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				                    CS_SCROLL_LIST_MAXIMIZE ); //CS_SCROLL_LIST_STATUS_BAR
					                //CS_SCROLL_LIST_TAB|
#if 0					                
			EQC_SetScrollTab(hControl,15,7,TRUE,0);
			for(i=0; i< GetListSize(ListHandle); i++)
			{
				TabData[GetKoreanCho(g_szPersonName[GetListValue(ListHandle,i)-1])]++;
			}
			EQC_SetScrollTabData(hControl, TabData, pTabText);
#endif
			EQC_SetScrollLogicalIndex(hControl,0);           	
            EQS_SetIOCActive( hControl );
        }
        else
        {
            EQS_END();
        }
        
        break;

    case SE_APP_END:
        break;
    case SE_APP_RESUME:

		hControl = EQS_GetNthIOC(0);
		if( EQS_GET_CHILD() == SKA_SelectNumber )
		{
			if( pEvent->wParam == TKN_PBOOK_SET_SDIAL_SELECT_MSG )
			{
                EQS_START_CHILD_EVENT(SKA_EditSDial, SE_APP_START, (WORD)pEvent->dwParam, EQS_NA);
				break;
			}
			else if( pEvent->wParam == TKN_PBOOK_SEND_SMS_SELECT_MSG )
			{
			   pb_number_info_type numberinfo;
           
			   numberinfo.address = pEvent->dwParam;
			   ui_get_pb( PB_NUMBER_INFO_I, &numberinfo);
			   nIndex = STRLEN(numberinfo.digits);
			   memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T)); 
			   STRCPY(g_UIMOBuffer.pszRcvNumber, numberinfo.digits);
			   g_UIMOBuffer.nRcvNumberSize = nIndex;
			   EQS_START_CHILD(SKA_SMSEditMessage);
			   break;
			}
			else 
			{ //from SKA_SelectNumber, BFT_SEARCH_BOOK_E
				        
				if( pEvent->dwParam != 0 )  
				{
					EQS_END_EVENT(SE_APP_RESUME, EQS_NA, pEvent->dwParam);  
					break;
				}
			}
		}		
        else
        {            
			EQS_SetIOCActive(hControl);

			if( EQS_GET_CHILD() == SKA_EditBookContent )
			{
									
				ListHandle = EQC_GetScrollListHandle(hControl);					
				index = EQC_GetScrollLogicalIndex(hControl);
				if( pEvent->dwParam )  // book address
				{
					WORD  nSize;
					INT16 nNewBookAddress, nFind;

					if( pEvent->wParam )
					{ // overwrite 인 경우 
						nFind = FindList( ListHandle, pEvent->wParam );
						if( nFind >= 0 )
						{
							RemoveListItem(ListHandle, nFind);
							EQC_SetScrollMaxDataLine(hControl, GetListSize(ListHandle) );
						}
					}
					else
					{
						nFind = -1;
					}

					RemoveListItem(ListHandle, index);
					nSize = GetListSize(ListHandle);
					nNewBookAddress = pEvent->dwParam;

					if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
					{
						UINT8 nTab;
						EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);					
						nTab = GetKoreanCho(g_szPersonName[nNewBookAddress-1]);
						if( nFind < 0 )
						{
							EQC_SetScrollTabDataSize(hControl, nTab, EQC_GetScrollTabDataSize(hControl, nTab)+1);
						}
					}
					
					for(i=0; i<nSize; i++)
					{
						if( KOREAN_STRCMP( g_szPersonName[nNewBookAddress-1],\
										   g_szPersonName[GetListValue(ListHandle, i)-1] ) <= 0 )
						{
							InsertList(ListHandle, i, nNewBookAddress, EQS_NA);
							break;
						}
					}
					if( i == nSize )
					{
						AddList(ListHandle, nNewBookAddress, EQS_NA);
					}				
					EQC_SetScrollLogicalIndex(hControl, i);
				}
				else
				{//아이템이 변경되었을 경우 이를 화면에 반영하기 위해 
					EQC_SetScrollLogicalIndex(hControl, index);
				}
			}
			else if( EQS_GET_CHILD() == SKA_ViewBookContent )
			{

				ListHandle = EQC_GetScrollListHandle(hControl);
				index = EQC_GetScrollLogicalIndex(hControl);
				if( pEvent->dwParam )  //전화부 삭제 
				{
					RemoveListItem(ListHandle,index);                   
					if( GetListSize(ListHandle) == 0 )
					{ 
						EQS_END();  //01.03.25   for refreshing speed search information
						return ;
					}
					index = EQC_DeleteScrollData(hControl, index );

					if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
					{
						EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);
						index = EQC_GetScrollLogicalIndex(hControl);
						EQC_SetScrollLogicalIndex(hControl, index);
					}
					else
					{
						sPBookDisplayScroll(hControl, index,TRUE, SDE_SEARCH_NAME);
					}
				}
				else// 부분 항목  삭제의  경우 
				{
					EQC_SetScrollLogicalIndex(hControl, index);
					EQS_SetIOCActive(hControl);
					break;            
				}
				
			}
			else if( EQS_GET_CHILD() == SKA_BookInput )
			{

				if( pEvent->dwParam )  // book address
				{
					WORD  nSize;
					INT16 nNewBookAddress, nFind;

					ListHandle = EQC_GetScrollListHandle(hControl);
					index = EQC_GetScrollLogicalIndex(hControl);
					nNewBookAddress = pEvent->dwParam;

					if( pEvent->wParam )
					{ // overwrite 인 경우 
						nFind = FindList( ListHandle, pEvent->wParam );
						if( nFind >= 0 )
						{
							SetListValue(ListHandle, nFind, nNewBookAddress);
							EQC_SetScrollLogicalIndex(hControl, nFind);
							break;
						}
					}
					// overwrite가 아닌 경우 또는 overwrite가 여기 속하지 않는 경우 			
					if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
					{
						UINT8 nTab;					
						nTab = GetKoreanCho(g_szPersonName[nNewBookAddress-1]);
						EQC_SetScrollTabDataSize(hControl, nTab, EQC_GetScrollTabDataSize(hControl, nTab)+1);						
					}

					nSize = GetListSize(ListHandle);
					for(i=0; i<nSize; i++)
					{
						if( KOREAN_STRCMP( g_szPersonName[nNewBookAddress-1],\
										   g_szPersonName[GetListValue(ListHandle, i)-1] ) <= 0 )
						{
							InsertList(ListHandle, i, nNewBookAddress, EQS_NA);
							break;
						}
					}
					if( i == nSize )
					{
						AddList(ListHandle, nNewBookAddress, EQS_NA);
					}				
					EQC_SetScrollMaxDataLine(hControl, GetListSize(ListHandle));
					EQC_SetScrollLogicalIndex(hControl, i);
				}
			}

        }

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		{
			BYTE szBuf[20];
			sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
				    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
			EQC_SetScrollStatusBarText(hControl, szBuf);
		}
		EQS_SetIOCActive(hControl);
        break;
	case SE_SOFTKEY_NEWNAME:
		EQS_START_CHILD_EVENT( SKA_BookInput, SE_APP_START, EQS_NA, EQS_NA );
		break;    

    case SE_SOFTKEY_MENU:
        {
            SKY_TOKEN paMenu[3] =
            {
				TKN_PBOOK_SEND_SMS,    //메시지 작성 
				TKN_PBOOK_SET_SDIAL,        //단축다이얼 지정
                TKN_PBOOK_ONE_BOOK_DELETE, //전화부 삭제 
                
            };
			BOOL  fActiveMenu[3];
			for( i=0; i<3; i++)
			{
				fActiveMenu[i] = TRUE;
			}
			hControl = EQS_GetNthIOC(0);
            ListHandle = EQC_GetScrollListHandle(hControl);
            BookInfo.address  = GetListValue(ListHandle, \
                                         EQC_GetScrollLogicalIndex(hControl));

			for( i=0; i< PERSONAL_NUMBER_LIMIT; i++ )
			{
				if( g_PersonNumberType[BookInfo.address-1][i] )
					break;
			}

			if( l_fFromCall )
			    fActiveMenu[0] = FALSE;

			if( i == PERSONAL_NUMBER_LIMIT )
			{
				fActiveMenu[0] = FALSE;
				fActiveMenu[1] = FALSE;
			}

			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 3, 0, fActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );		
        }  
        break;
    case SE_IO_POPUP_CANCEL:
        hControl = EQS_GetNthIOC(0);
        EQS_SetIOCActive(hControl);
        break;   
       
    case SE_IO_POPUP_SELECT:
                  
		hControl = EQS_GetNthIOC(0);
        switch( pEvent->dwParam )
        {
        case TKN_PBOOK_ONE_BOOK_DELETE:  // 항목삭제
			{
				BYTE szBuf[100];	
#if 0				
				index =  EQC_GetScrollPhysicalIndex(hControl);
				STRCPY( szBuf, EQC_GetScrollData(hControl, index));
				STRCAT( szBuf,(BYTE*)"\n");
				STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));
#endif

				ListHandle = EQC_GetScrollListHandle(hControl);
				BookInfo.address  = GetListValue(ListHandle, \
											 EQC_GetScrollLogicalIndex(hControl));
				STRCPY( szBuf, g_szPersonName[BookInfo.address-1]);
				STRCAT( szBuf,(BYTE*)"\n");
				STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));
				hControl = EQC_MessageBox(szBuf, 10, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(hControl, VIEW_SEARCH_NAME_ASK_DELETE_MSG_E);
			}
            break;
        case TKN_PBOOK_SET_SDIAL:  // 단축다이얼 지정
            ListHandle = EQC_GetScrollListHandle(hControl);
            BookInfo.address  = GetListValue(ListHandle, \
                                         EQC_GetScrollLogicalIndex(hControl));
			EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
								  BookInfo.address,\
								  TKN_PBOOK_SET_SDIAL_SELECT_MSG);	
            break;
		case TKN_PBOOK_SEND_SMS:  //메시지 작성 
            if(SKY_IS_SMS_LOCK() && pEvent->wParam !=SMSCODE_IN_VIEW_SEARCH_NUM )
            {
                EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_IN_VIEW_SEARCH_NUM, EQS_NA);
            }else
            {
                ListHandle = EQC_GetScrollListHandle(hControl);
                BookInfo.address  = GetListValue(ListHandle, \
                                             EQC_GetScrollLogicalIndex(hControl));
			    EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
								      BookInfo.address,\
								      TKN_PBOOK_SEND_SMS_SELECT_MSG);
            }
			break;
        default:
            break;
        }
        
        break;

	case SE_IO_MESGBOX_END:  
		hControl = EQS_GetNthIOC(0);
		if( pEvent->dwParam == VIEW_SEARCH_NUMBER_DELETE_MSG_E) // "삭제중 " 메시지 
		{			
            ListHandle = EQC_GetScrollListHandle(hControl);
            index =  EQC_GetScrollLogicalIndex(hControl);                       
            BookInfo.address = GetListValue( ListHandle, index );
            if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
                break;
            GroupInfo.address = BookInfo.group_address;
            if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) == 0)
                break;    
            DeleteBook(&GroupInfo, &BookInfo, TRUE);  //2001.01.08
			ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
			PhoneBookUpdate();
            
            RemoveListItem( ListHandle,index);            
            nDeletePos = EQC_DeleteScrollData( hControl, \
                                        EQC_GetScrollLogicalIndex( hControl));
			
            if( EQC_GetScrollMaxDataLine(hControl) == 0 )
            {
                EQS_END();
                break;
            }

            if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
			{
				EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);
				if( EQC_GetScrollCursor(hControl) == 0 )
				{
					index = EQC_GetScrollLogicalIndex(hControl);
					if( index  == (EQC_GetScrollMaxDataLine(hControl)-1) )
						EQC_SetScrollLogicalIndex(hControl, index);
					else
						EQC_SetScrollLogicalIndex(hControl, index+1);
				}
				else
				{
					EQC_SetScrollLogicalIndex(hControl, EQC_GetScrollLogicalIndex(hControl));
				}
				hControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG),\
										   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(hControl, VIEW_SEARCH_NUMBER_INFO_MSG_E);

			}
			else
			{
				sPBookDisplayScroll((HCONTROL)hControl, (WORD)nDeletePos,\
									 TRUE, SDE_SEARCH_NAME);
			}

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
			{
				BYTE szBuf[20];
				sprintf(szBuf, SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
					    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
				EQC_SetScrollStatusBarText(hControl, szBuf);
			}

			hControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG),\
				                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_SEARCH_NUMBER_INFO_MSG_E);
		}
		else  // VIEW_SEARCH_NUMBER_INFO_MSG_E
		{
			EQS_SetIOCActive(hControl);	
		}
		break;

	case SE_IO_MESGBOX_NO:

		hControl = EQS_GetNthIOC(0);
		EQS_SetIOCActive(hControl);	
		
		break;
	case SE_IO_MESGBOX_YES:
		if( pEvent->dwParam == VIEW_SEARCH_NUMBER_ASK_DELETE_MSG_E )
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_DELETING),\
				                      0, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_SEARCH_NUMBER_DELETE_MSG_E);
		}
		else
		{
			hControl = EQS_GetNthIOC(0);
			EQS_SetIOCActive(hControl);	
		}
		break;

    case SE_IO_SCROLL_LIST_SELECT:   

        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );
        if(KOREAN_STRCMP( EQS_GetCurrentHeaderStr(), SKY_GET_TOKEN(TKN_PBOOK_SEARCH) ) == 0 )
        {
            EQS_START_CHILD_EVENT( SKA_SelectNumber, SE_APP_START, \
                                   GetListValue(ListHandle, index), EQS_NA);
        }
        else
        {
            EQS_START_CHILD_EVENT( SKA_ViewBookContent, SE_APP_START, \
                                   GetListValue(ListHandle, index),\
                                   BFT_NON_FUNCTION_E);
        }
        break;
	case SE_IO_SCROLL_TAB_CHG:
    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                    TRUE, SDE_SEARCH_NAME);
        break;

    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            FALSE, SDE_SEARCH_NAME);

        break;   
    case SE_IO_KEY_1:       
    case SE_IO_KEY_2:       
    case SE_IO_KEY_3:        
    case SE_IO_KEY_4:       
    case SE_IO_KEY_5:

        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );   
        BookInfo.address = GetListValue(ListHandle,index);
        index  = pEvent->EventID - SE_IO_KEY_0;
        if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) )
        {
            pb_number_info_type NumberInfo;
            NumberInfo.address = BookInfo.number_address;
            while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) > 0 )
            {
				index--;
                if( index == 0)
                {
                    break;
				}
				else
				{
                    NumberInfo.address = NumberInfo.next_index;
                }
            }
			if( index > 0 )
				break;
			if( EQS_GetWindowCML() == CL_NONE_SELECT_NONE )
			{
				EQS_END_EVENT( SE_APP_RESUME, EQS_NA, NumberInfo.address );
			}
			else
			{
				//if(SKY_IsInCall() || SKY_IS_IN_WAP() )
				if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
					break;

				sprintf((CHAR*) szTemp,(CHAR*) SKY_GET_TOKEN(TKN_PBOOK_FMT_NAME_TYPE),	
                                                    g_szPersonName[NumberInfo.book_address-1],\
													phone_type[NumberInfo.number_type-1]);
				index = STRLEN(szTemp) ;
				SET_ALPHAEDIT_BUFFER(szTemp, index);
				SET_ALPHAEDIT_POS(index);
				sPBookSendCall( NumberInfo.digits);
			}
        }
		break;
	case SE_IO_KEY_POUND:
		{
			int nSize;
			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);
			nSize = EQC_GetScrollMaxDataLine(hControl);

			index = EQC_GetScrollLogicalIndex(hControl);
			for( i = index+1; i < nSize; i++ )
			{
				if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
					                 g_szPersonName[GetListValue(ListHandle, i    )-1]))
				{
					break;
				}
			}
			if( i == nSize )
			{
				for( i = 0; i < index; i++ )
				{
					if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
										 g_szPersonName[GetListValue(ListHandle, i    )-1]))
					{
						break;
					}
				}
			}

			if( i == index )
				break;
			EQC_SetScrollLogicalIndex(hControl, i);
			sPBookDisplayScroll(hControl, i, TRUE , SDE_SEARCH_NAME);
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
	case SE_IO_KEY_STAR:
		{
			int nSize;
			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);
			nSize = EQC_GetScrollMaxDataLine(hControl);

			index = EQC_GetScrollLogicalIndex(hControl);
			for( i = index-1; i >= 0; i-- )
			{
				if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
					                 g_szPersonName[GetListValue(ListHandle, i    )-1]))
				{
					break;
				}
			}
			if( i < 0 )
			{
				for( i = nSize-1; i  > index; i-- )
				{
					if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
										 g_szPersonName[GetListValue(ListHandle, i    )-1]))
					{
						break;
					}
				}
			}

			if( i == index )
				break;

			EQC_SetScrollLogicalIndex(hControl, i);                    
			sPBookDisplayScroll(hControl, i, TRUE , SDE_SEARCH_NAME);
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;	
    case SE_KEY_SEND:
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;
			
        sPBookMakeCall(EQS_GetNthIOC(0));   
        break;        
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

VOID SKY_API SKA_ViewGroupList(SKY_EVENT_T* pEvent)
{

    static BOOK_FUNCTION_E    FunctionMode;
    HCONTROL                  hControl;
    INT8                      ListHandle;    
    SKY_TOKEN                 TitleToken;
    INT16                     index, GroupAddress;
        
    switch (pEvent->EventID)
    {
    case SE_APP_START:
        if(pEvent->dwParam == BFT_EDIT_SDIAL_E )
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                          SKY_DEF_WIN_Y, 
                          SKY_DEF_WIN_DX, 
                          SKY_DEF_WIN_DY, 
                          CL_NONE_SELECT_NONE, 
                          NULL,//SKY_GET_TOKEN(TKN_EDIT_SHORT_DIAL),
                          WA_SHOW_CML);
            TitleToken = TKN_EDIT_SHORT_DIAL;
        }
        else if( pEvent->dwParam == BFT_VIEW_IRDA_GROUP_E )
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                          SKY_DEF_WIN_Y, 
                          SKY_DEF_WIN_DX, 
                          SKY_DEF_WIN_DY, 
                          CL_NONE_SEND_NONE, 
                          NULL,//SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_BOOK),
                          WA_SHOW_CML);
            TitleToken = TKN_PHONE_TO_PHONE_BOOK;
        }
        else if( pEvent->dwParam == BFT_VIEW_EMAIL_LIST_E )
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                          SKY_DEF_WIN_Y, 
                          SKY_DEF_WIN_DX, 
                          SKY_DEF_WIN_DY, 
                          CL_NONE_SELECT_NONE, 
                          NULL,//SKY_GET_TOKEN(TKN_GROUP_EDIT),
                          WA_SHOW_CML);
            TitleToken = TKN_PBOOK_EMAIL_LIST_TITLE;
        }
        else
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                          SKY_DEF_WIN_Y, 
                          SKY_DEF_WIN_DX, 
                          SKY_DEF_WIN_DY, 
                          CL_MENU_NONE_NONE, 
                          NULL,//SKY_GET_TOKEN(TKN_GROUP_EDIT),
                          WA_SHOW_CML);
            TitleToken = TKN_GROUP_EDIT;
        }
        FunctionMode = (BOOK_FUNCTION_E)pEvent->dwParam ;

        if( pEvent->dwParam == BFT_VIEW_EMAIL_LIST_E )
        {
            hControl = sMakeScrollList(EQS_NA,EQS_NA, EQS_NA,\
                                       EQC_GetScrollDisplayLine(), 0, \
                                       (UINT8)0,(WORD)0,\
                                       SDE_EMAIL_GROUP_LIST); 
        }
		else if( pEvent->dwParam == BFT_VIEW_IRDA_GROUP_E )
		{
            hControl = sMakeScrollList(EQS_NA,EQS_NA, EQS_NA,\
                                       EQC_GetScrollDisplayLine(), 0, \
                                       (UINT8)0,(WORD)0,\
                                       SDE_IRDA_GROUP); 
		}
        else
        {
            hControl = sMakeScrollList(EQS_NA,EQS_NA, EQS_NA,\
                                       EQC_GetScrollDisplayLine(), 0, \
                                       (UINT8)0,(WORD)0,\
                                       SDE_GROUP_LIST); 
        }

        if( hControl == 0 )
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
            break;
        }
        EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TitleToken));
        EQC_ShowScrollTitle(hControl,TRUE);
        EQC_SetLogicalList(hControl,TRUE);
        EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
			                    CS_SCROLL_LIST_ENUMBERATED|\
                                CS_SCROLL_LIST_MAXIMIZE   );
		EQC_SET_USERDATA(hControl, FunctionMode);
        EQS_SetIOCActive( hControl );

        break;
    case SE_APP_END:
        if( FunctionMode == BFT_VIEW_EMAIL_LIST_E )
        {
            hControl   = EQS_GetNthIOC(0);
            if( hControl != INVALID_CONTROL_HANDLE )
            {
                ListHandle = EQC_GetScrollListHandle(hControl);
                DeleteList(GetListType(ListHandle,0));
            }
        }

		FunctionMode = BFT_NON_FUNCTION_E;
        break;

    case SE_APP_RESUME:
		if( EQS_GET_CHILD() == sSKA_AccessSecretGroup )
		{
            if(  pEvent->dwParam == 0 )  // 비밀번호가 맞지 않는 경우 종료한다.
            {
                EQS_END();			
            }
			else
			{
				hControl   = EQS_GetNthIOC(0);
				ListHandle = EQC_GetScrollListHandle( hControl );
				GroupAddress = GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl));
				EQS_END_EVENT( SE_APP_RESUME, EQS_NA, (DWORD) GroupAddress );
			}
			break;
		}
        else if( pEvent->dwParam != 0 )   //  from  SKA_4 , EQS_GET_PARENT() == SKA_EditSDial 
        {   // selected from IRDA List or EMail List
            EQS_END_EVENT( SE_APP_RESUME, 0, pEvent->dwParam);
        }
        else
        {
            EQS_SetIOCActive( EQS_GetNthIOC(0) );
        }
		FunctionMode = EQC_GET_USERDATA(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        index = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( pEvent->wParam );
        if( FunctionMode == BFT_SAVE_OLD_DATA_E )
        {
            EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
                                   GetListValue( ListHandle, index ), \
                                   BFT_SAVE_OLD_DATA_E);
        }
        else if( FunctionMode == BFT_VIEW_IRDA_GROUP_E )
        {
			GroupAddress = GetListValue(ListHandle, index); 
			if( IS_SECRET_GROUP(GroupAddress ) )
			{
				EQS_START_CHILD(sSKA_AccessSecretGroup);
			}
			else
			{
				EQS_END_EVENT( SE_APP_RESUME, EQS_NA, (DWORD) GroupAddress );
			}
        }
        else if( FunctionMode == BFT_VIEW_EMAIL_LIST_E  )
        {
            EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
                                   pEvent->wParam, \
                                   BFT_VIEW_EMAIL_LIST_E);
        }
        else
        {
            if( EQS_GET_PARENT() == SKA_EditSDial )
            {
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
                                       GetListValue( ListHandle, index ), \
                                       BFT_EDIT_SDIAL_E);
            }
#if 0
            else if(  EQS_GET_PARENT() == SKA_BookMain )
            {
                EQS_START_CHILD_EVENT( SKA_ViewBookList, SE_APP_START, \
                                       GetListValue( ListHandle, index ), \
                                       BFT_EDIT_GROUP_E);
            }
#endif
            else ;
        }       
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:		
        if( FunctionMode == BFT_VIEW_EMAIL_LIST_E )
        {
			sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
								TRUE, SDE_EMAIL_GROUP_LIST);
        }
		else if( FunctionMode == BFT_VIEW_IRDA_GROUP_E )
		{
			sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
								TRUE, SDE_IRDA_GROUP);
		}
        else
        {
			sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
						        TRUE, SDE_GROUP_LIST);
        }
        break;
    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        if( FunctionMode == BFT_VIEW_EMAIL_LIST_E )
        {
			sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
								FALSE, SDE_EMAIL_GROUP_LIST);
        }
		else if( FunctionMode == BFT_VIEW_IRDA_GROUP_E )
		{
			sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
								FALSE, SDE_IRDA_GROUP);
		}
        else
        {
			sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
						        FALSE, SDE_GROUP_LIST);
        }
        break;

    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}


void sMakeMyBellList(void)
{
    UINT8  index;

    l_nMyBell = (UINT8)SKY_GetFileNumberInDataFile(SDF_MELODY);

    if( l_nMyBell > 0)
    {
        // Load user file list
        for(index=0; index < l_nMyBell; index++)
            l_ppMyBellList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

        l_nMyBell = SKY_GetFileExtList(SKY_MELODY_DIR, l_ppMyBellList);
    }
}

/*#ifdef FEATURE_CHARACBELL
void sMakeChaBellList(void)
{
    UINT8  index;

    l_nChaBell = (UINT8)SKY_GetFileNumberInDataFile(SDF_CHA_MELODY);

    if( l_nChaBell > 0)
    {
        // Load user file list
        for(index=0; index < l_nChaBell; index++)
            l_ppChaBellList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

        l_nChaBell = SKY_GetFileExtList(SKY_CHA_MELODY_DIR, l_ppChaBellList);
    }
} // sMakeChaBellList
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
void sMakeChaBellList(void) {
    UINT8  nIndex, nPosition;

    l_nChaBell = SKY_GET_CB_ADDRESS();
    if(l_nChaBell > 0)
    {
        for(nIndex = 0; nIndex < l_nChaBell; nIndex ++)
            l_ppChaBellList[nIndex] = (BYTE*)SKY_Malloc(MAX_TITLE_LENGTH);
    
        for (nIndex=0, nPosition = 0; nIndex < MAX_NUM_OF_CHARACBELL; nIndex++) {
            if (SKY_GET_CB_EMPTY(nIndex)) {
                STRCPY(l_ppChaBellList[nPosition], SKY_GET_CB_TITLE(nIndex));
                nPosition++;
            }
        }
    }
} // sMakeChaBellList
#endif // FEATURE_CHARACTER_BELL

UINT8 sFindMyBellIndex(BYTE* pszBell)
{
	UINT8 nIndex;
    for(nIndex=0; nIndex < l_nMyBell; nIndex++)
    {
        if(!KOREAN_STRCMP(l_ppMyBellList[nIndex], pszBell))
            return (nIndex+1);
    }
	return 0;

}

/*#ifdef FEATURE_CHARACBELL
UINT8 sFindChaBellIndex(BYTE* pszBell)
{
	UINT8 nIndex;
    for(nIndex=0; nIndex < l_nChaBell; nIndex++)
    {
        if(!KOREAN_STRCMP(l_ppChaBellList[nIndex], pszBell))
            return (nIndex+1);
    }
	return 0;
} // sFindChaBellIndex
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
UINT8 sFindChaBellIndex(BYTE* pszBell)
{
	UINT8 nIndex;
    for(nIndex=0; nIndex < l_nChaBell; nIndex++)
    {
        if(!KOREAN_STRCMP(l_ppChaBellList[nIndex], pszBell))
            return (nIndex+1);
    }
	return 0;
} // sFindChaBellIndex
#endif // FEATURE_CHARACTER_BELL

HCONTROL sMakeBellList(COORD x, COORD y, UINT8 nBellGroup, UINT8 nBellID)
{  
    INT   nBellIndex, nIndex;
    HCONTROL hControl;

    BYTE* pList[100];
    //nBellGroup = SKY_BellFindGroup(nBellID);
    //nBellIndex = SKY_BellFindGroupIndex(nBellGroup);

    nBellIndex = 0;
    for(nIndex=0; nIndex < nBellGroup ; nIndex ++)
    {
        nBellIndex += aBell_Group[nIndex].nMelodyNumber;
    }

    for(nIndex=nBellIndex; nIndex < nBellIndex+aBell_Group[nBellGroup].nMelodyNumber; nIndex++)
    {
        pList[nIndex-nBellIndex] = SKY_GET_TOKEN(aBell_List[nIndex].BellName);
    }
    //nBellIndex = SKY_FindBellIndex(nBellID);
    hControl = EQC_ScrollCombo(x, y, 16, 3, pList, \
                               aBell_Group[nBellGroup].nMelodyNumber, \
                               aBell_Group[nBellGroup].nMelodyNumber, nBellID);


    return hControl;

}

UINT8 sBellFindGroup(UINT8 nBellId, UINT8 nMaxGroup)
{
    UINT8   nTmp, index;

	nTmp = 0;
    for(index=0; index < nMaxGroup ; index ++)
    {
        nTmp += aBell_Group[index].nMelodyNumber;

        if(nBellId < nTmp)
            return (nTmp - aBell_Group[index].nMelodyNumber);
    }

    return 0;
}


LOCAL void EQS_API sPlayBell(void)
{
	HCONTROL hControl;
    WORD nBellGroup, nGroupIndex, nBellIndex, nBellID;

	hControl = EQS_GetNthIOC(EQC_BELL_ID);
    nBellGroup = EQC_GetScrollCurrentTab(hControl);

#if 0
    if( nBellGroup == (NUM_BELL_GROUP-1) ) // MyBell
    {
		if( l_nMyBell > 0 )
		{
			nBellIndex = EQC_GetScrollPhysicalIndex(hControl);
			nBellIndex -= EQC_GetScrollTabIndex(hControl, nBellGroup);
			SKY_PlayMusicFileEx(l_ppMyBellList[nBellIndex], SKY_GET_BELL_VOLUME());
		}
    }
	else if( nBellGroup  < (NUM_BELL_GROUP-1) )	    
    {
		nBellIndex = EQC_GetScrollPhysicalIndex(hControl);
		nBellID = aBell_List[nBellIndex].wBellId;
		SKY_PlayMusicEx(nBellID, SKY_GET_BELL_VOLUME(), 0);
    }
	else 
		;
#endif

    //nBellGroup = EQC_GetScrollCurrentTab( hControl );
	nBellGroup = EQC_GetScrollTabPos(hControl, EQC_GetScrollPhysicalIndex(hControl));
    if( nBellGroup == (EQC_GetScrollTabSize(hControl)-1) ) //그룹벨  지정 안됨 
	{
        ;
	}
/*#ifdef FEATURE_CHARACBELL
    else if( nBellGroup == (NUM_BELL_GROUP-1) ) // Characbell
    {
		if( l_nChaBell > 0 )
		{
			nBellIndex= EQC_GetScrollPhysicalIndex(hControl);
			nBellIndex -= EQC_GetScrollTabIndex(hControl, nBellGroup);
            SetCharacbellSelected(TRUE);
			SKY_PlayMusicFileEx(l_ppChaBellList[nBellIndex], SKY_GET_BELL_VOLUME());
		}
    }
    else if( nBellGroup == (NUM_BELL_GROUP-2) ) // MyBell
#else
    else if( nBellGroup == (NUM_BELL_GROUP-1) ) // MyBell
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
    else if( nBellGroup == (NUM_BELL_GROUP) ) // Characbell
    {
		if( l_nChaBell > 0 )
		{
            UINT8   index, inc;
            char    temp[MAX_TITLE_LENGTH];

			nBellIndex= EQC_GetScrollPhysicalIndex(hControl);
			nBellIndex -= EQC_GetScrollTabIndex(hControl, nBellGroup);

            // 여기가 벨 연주하는 부분..
            for(index = 0, inc = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
                if (SKY_GET_CB_EMPTY(index)) {
                    if (inc == nBellIndex) break;
                    inc++;
                }
            }

            STRCPY(temp, SKY_GET_CB_MTITLE(index));
            if (SKY_GET_CB_MTYPE(index) == SKY_MY_BELL_ID)
                SKY_PlayMusicFileEx(temp, SKY_GET_BELL_VOLUME());
            else
                SKY_PlayMusicEx(SKY_GET_CB_MTYPE(index), SKY_GET_BELL_VOLUME(), 0);
		}
    }
#endif // FEATURE_CHARACTER_BELL
    else if( nBellGroup == (NUM_BELL_GROUP-1) ) // MyBell
    {
		if( l_nMyBell > 0 )
		{
			nBellIndex= EQC_GetScrollPhysicalIndex(hControl);
			nBellIndex -= EQC_GetScrollTabIndex(hControl, nBellGroup);
			SKY_PlayMusicFileEx(l_ppMyBellList[nBellIndex], SKY_GET_BELL_VOLUME());
		}
    }
	else  //일반 벨 
    {
		nBellIndex = EQC_GetScrollPhysicalIndex(hControl);
		nBellID = aBell_List[nBellIndex].wBellId;
		SKY_PlayMusicEx(nBellID, SKY_GET_BELL_VOLUME(), 0);
    }

}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Xxx()
**
** Description: 
**     @DesciptionAboutThisFileHere
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

#define GROUP_EDIT_CONTROL_LEN   16


LOCAL void SKY_API sSKA_EditGroupContent(SKY_EVENT_T *pEvent)
{
    HCONTROL   hControl;
    INT        nIndex, nBellGroup, nBellGroupSize, nSize, nBellIndex, nGroupIndex;
	UINT8      nColorindex, nMyBell,i;
    static     pb_group_info_type GroupInfo;
	static     WORD s_wColorIndex = 0;
    BYTE       szTemp[10];

/*#ifdef FEATURE_CHARACBELL
    UINT8      nChaBell;
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
    UINT8       nChaBell;
    BYTE        szTmp[30];
    BYTE        temp1[SKY_FILE_NAME_MAX_LENGTH+4];
    BYTE        temp2[SKY_FILE_NAME_MAX_LENGTH+4];
    char        *loc;
#endif // FEATURE_CHARACTER_BELL

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        if( IsValid(PB_GROUP_INFO_I, pEvent->wParam ) == FALSE )
            EQS_END();
        else
        {
            BYTE* pList[200];
			WORD  wList[15]; 

            BOOL  fBasicGroup;
            SKY_TOKEN GroupColor[MAX_ALERT_ID_LCD_NUMBER] =
            {
                TKN_RAINBOW,
                TKN_BLUE,
                TKN_GREEN,
                TKN_RED,
                TKN_VIOLET,
                TKN_BLUE_GREEN,
                TKN_ORANGE,
                TKN_WHITE,
                TKN_GROUP_LIGHT_NONE
            };
            MARGIN_T Margin;
            COORD    xSpace, ySpace;
            COORD    xPos, yPos;
            
            EQS_StartFormatter();   
            EQS_GetControlMargin(&Margin);
            EQS_GetControlSpace(&xSpace, &ySpace);
            EQS_EndFormatter();
            //Margin.xLeft +=5;
          

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                600, 
                CL_NONE_SELECT_SAVE,
                SKY_GET_TOKEN(TKN_GROUP_EDIT),               
                WA_SHOW_HEADER|WA_SHOW_CML);
            
			memset(&GroupInfo, 0, sizeof(GroupInfo));
            GroupInfo.address = pEvent->wParam;
            if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) == FALSE )
            {
                EQS_END();
                break;
            }

            if( IS_SECRET_GROUP(GroupInfo.address) == TRUE )
            {
                EQS_START_CHILD(sSKA_AccessSecretGroup);
            }

            if( KOREAN_STRCMP( g_szGroupName[GroupInfo.address-1] , SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) ) == 0 )
            {
                fBasicGroup = TRUE;
            }
            else
                fBasicGroup = FALSE;

            sMakeMyBellList();   // my bell list  초기화 
/*#ifdef FEATURE_CHARACBELL
            sMakeChaBellList(); // Characbell 초기화 
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
            sMakeChaBellList(); // Characbell 초기화 
#endif // FEATURE_CHARACTER_BELL

            yPos = Margin.yTop;          
            hControl = EQC_StaticText(Margin.xLeft,yPos, TKN_PBOOK_GROUP_NAME);

            yPos += EQC_GET_DY(hControl) + ySpace;
          
            hControl = EQC_TextEdit(Margin.xLeft, yPos, GROUP_EDIT_CONTROL_LEN, 1, \
                                    TEM_KOREAN, MAX_BOOK_NAME,\
                                    (BYTE*)GroupInfo.group_name, 0); // 그룹 일므   
            if( fBasicGroup )
			{
				EQC_SET_ENABLE(hControl, FALSE);
                EQC_SET_FOCUS(hControl, FALSE);
			}

            yPos += EQC_GET_DY(hControl) + ySpace;
            hControl = EQC_StaticText( Margin.xLeft, yPos, TKN_PBOOK_GROUP_MEMBER);
            xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;            
            hControl = EQC_StaticText( xPos, yPos, TKN_PBOOK_GROUP_MEMBER);
                  
			nSize = 0;//01.04.10
			for(nIndex=0; nIndex < MAX_BOOK_NUMBER; nIndex++)
			{
				if( g_szPersonName[nIndex][0] && g_PersonGroup[nIndex] == GroupInfo.address )
					nSize++;
			}
            sprintf(szTemp,"%d 명", nSize);//GroupInfo.wMember );
            EQC_SetStaticText(hControl, szTemp);

            yPos += EQC_GET_DY(hControl) + ySpace;
			
			hControl = EQC_StaticText(Margin.xLeft,yPos, TKN_PBOOK_GROUP_ATTRIBUTE);
			//yPos += EQC_GET_DY(hControl) + ySpace;

			pList[0] = SKY_GET_TOKEN(TKN_PBOOK_GROUP_NONSECRET);
			pList[1] = SKY_GET_TOKEN(TKN_PBOOK_GROUP_SECRET);
			hControl = EQC_ScrollCombo(Margin.xLeft+EQC_GET_DX(hControl)+6, yPos, 6, 2,pList,2,2,0);
		    if( GroupInfo.status )
			{
				EQC_SetScrollLogicalIndex(hControl, 1);
			}
            if( fBasicGroup )
			{
				EQC_SET_ENABLE(hControl, FALSE);
                EQC_SET_FOCUS(hControl, FALSE);
			}
#if 0
//비밀 그룹 or 일반 그룹 

            pList[0] = SKY_GET_TOKEN(TKN_PBOOK_GROUP_SECRET);
            hControl = EQC_CheckBox(Margin.xLeft,yPos,pList,1);
			if( GroupInfo.status )
				EQC_SetCheckBox(hControl, 0);
			else
				EQC_ResetCheckBox(hControl, 0);

            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |  CS_CHECK_BOX_BORDER );

            if( fBasicGroup )
			{
				EQC_SET_ENABLE(hControl, FALSE);
                EQC_SET_FOCUS(hControl, FALSE);
			}

//
#endif
            
            yPos += EQC_GET_DY(hControl) + ySpace;  

#ifdef FEATURE_CHARACTER_BELL
            sprintf(szTmp, "%s%s", SKY_GET_TOKEN(TKN_PBOOK_GROUP_BELL), " ");
            if (SKY_IS_CB_GROUPBELL(GroupInfo.Bell.wBellID))
            {
                STRCAT(szTmp, SF_CHARACBELL_2);
                STRCAT(szTmp, SF_CHARACBELL_3);
            }
            hControl = EQC_StaticText( Margin.xLeft,  yPos, TKN_NULL);       
            EQC_SetStaticText(hControl, szTmp);
#else
            hControl = EQC_StaticText( Margin.xLeft,  yPos, TKN_PBOOK_GROUP_BELL );        
#endif // FEATURE_CHARACTER_BELL
            			
			yPos += EQC_GET_DY(hControl) + ySpace; 
            if( l_nMyBell == 0 )
			{
                nBellGroupSize = NUM_BELL_GROUP;//nBellGroupSize = NUM_BELL_GROUP - 1;
				nMyBell = 0xFF;
			}
            else
			{
                nBellGroupSize = NUM_BELL_GROUP;
				nMyBell = BELL_GROUP_MY_BELL_ID;
			}

/*#ifdef FEATURE_CHARACBELL
            if( l_nChaBell == 0 )
			{
                nBellGroupSize = NUM_BELL_GROUP;//nBellGroupSize = NUM_BELL_GROUP - 1;
				nChaBell = 0xFF;
			}
            else
			{
                nBellGroupSize = NUM_BELL_GROUP;
				nChaBell = BELL_GROUP_CHARAC_BELL;
			}
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
            if( l_nChaBell == 0 )
			{
                nBellGroupSize = NUM_BELL_GROUP;//nBellGroupSize = NUM_BELL_GROUP - 1;
				nChaBell = 0xFF;
			}
            else
			{
                nBellGroupSize = NUM_BELL_GROUP;
				nChaBell = BELL_GROUP_MY_BELL_ID+1;
			}

            if( GroupInfo.Bell.wBellID == 0 )
			{
				nBellIndex = NUM_BELL_LIST + l_nMyBell+ + l_nChaBell;//nBellIndex = 0;
			}
#else
            if( GroupInfo.Bell.wBellID == 0 )
			{
				nBellIndex = NUM_BELL_LIST + l_nMyBell;//nBellIndex = 0;
			}
#endif // FEATURE_CHARACTER_BELL
			else if(SKY_IS_MY_BELL(GroupInfo.Bell.wBellID))
            {
				nBellIndex = sFindMyBellIndex(GroupInfo.Bell.szFileName);
				if( nBellIndex )
				{
					nBellIndex += (NUM_BELL_LIST-1);
				}
            }
/*#ifdef FEATURE_CHARACBELL
            else if(SKY_IS_CHARAC_BELL(GroupInfo.Bell.wBellID))
            {
				nBellIndex = sFindChaBellIndex(GroupInfo.Bell.szFileName);
				if( nBellIndex )
				{
					nBellIndex += (NUM_BELL_LIST + l_nMyBell - 1);
				}
            }
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
            else if(SKY_IS_CB_GROUPBELL(GroupInfo.Bell.wBellID))
            {
				nBellIndex = sFindChaBellIndex(GroupInfo.Bell.szFileName);
				if( nBellIndex )
				{
					nBellIndex += (NUM_BELL_LIST + l_nMyBell - 1);
				}
            }
#endif // FEATURE_CHARACTER_BELL
            else 
            {
                nBellIndex = SKY_FindBellIndex(GroupInfo.Bell.wBellID);
            }

            
			for(nIndex= 0; nIndex < NUM_BELL_LIST; nIndex++)
			{
				pList[nIndex] = SKY_GET_TOKEN(aBell_List[nIndex].BellName);
			}				
			for(i=nIndex; i < nIndex + l_nMyBell; i++)
			{
				pList[i] = l_ppMyBellList[i-nIndex];
			}
/*#ifdef FEATURE_CHARACBELL
            for(nIndex = i; nIndex < i + l_nChaBell; nIndex++)
			{
				pList[nIndex] = l_ppChaBellList[nIndex - i];
			}
            pList[nIndex] = SKY_GET_TOKEN(TKN_BELLVIB_GROUPBELL_UNSET);
            hControl = EQC_ScrollList(Margin.xLeft, yPos,GROUP_EDIT_CONTROL_LEN, 4, \
				                      pList, i + l_nChaBell + 1, i + l_nChaBell + 1, nBellIndex);
#else
			pList[i] = SKY_GET_TOKEN(TKN_BELLVIB_GROUPBELL_UNSET);
            hControl = EQC_ScrollList(Margin.xLeft, yPos,GROUP_EDIT_CONTROL_LEN, 4, \
				                      pList, nIndex + l_nMyBell+1, nIndex + l_nMyBell+1, nBellIndex);
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
            for(nIndex = i; nIndex < i + l_nChaBell; nIndex++)
			{
				pList[nIndex] = l_ppChaBellList[nIndex - i];
			}
            pList[nIndex] = SKY_GET_TOKEN(TKN_BELLVIB_GROUPBELL_UNSET);
            hControl = EQC_ScrollList(Margin.xLeft, yPos,GROUP_EDIT_CONTROL_LEN, 4, \
				                      pList, i + l_nChaBell + 1, i + l_nChaBell + 1, nBellIndex);
#else
			pList[i] = SKY_GET_TOKEN(TKN_BELLVIB_GROUPBELL_UNSET);
            hControl = EQC_ScrollList(Margin.xLeft, yPos,GROUP_EDIT_CONTROL_LEN, 4, \
				                      pList, nIndex + l_nMyBell+1, nIndex + l_nMyBell+1, nBellIndex);
#endif // FEATURE_CHARACTER_BELL
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				                    CS_SCROLL_LIST_COMBO |\
									CS_SCROLL_LIST_TAB );
#ifdef FEATURE_CHARACTER_BELL
            EQC_SetScrollTab(hControl,nBellGroupSize+2,3,FALSE,0);
#else
            EQC_SetScrollTab(hControl,nBellGroupSize+1,3,FALSE,0);
#endif // FEATURE_CHARACTER_BELL
            for(nIndex=0; nIndex < nBellGroupSize; nIndex ++)
            {
                pList[nIndex] = SKY_GET_TOKEN(aBell_Group[nIndex].GroupName);
				wList[nIndex] = aBell_Group[nIndex].nMelodyNumber;
            }
            wList[nIndex-1] = l_nMyBell;
/*#ifdef FEATURE_CHARACBELL
            wList[nIndex-1] = l_nChaBell;
            wList[nIndex-2] = l_nMyBell;
			pList[nIndex] = SKY_GET_TOKEN(TKN_PBOOK_NO_GROUP_BELL);
			wList[nIndex] = 1;
#else
			wList[nIndex-1] = l_nMyBell;	
			pList[nIndex] = SKY_GET_TOKEN(TKN_PBOOK_NO_GROUP_BELL);
			wList[nIndex] = 1;
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
            pList[nIndex] = SKY_GET_TOKEN(TKN_CHARACBELL_MENU);
            wList[nIndex] = l_nChaBell;
            pList[nIndex+1] = SKY_GET_TOKEN(TKN_PBOOK_NO_GROUP_BELL);
			wList[nIndex+1] = 1;
#else
			pList[nIndex] = SKY_GET_TOKEN(TKN_PBOOK_NO_GROUP_BELL);
			wList[nIndex] = 1;
#endif // FEATURE_CHARACTER_BELL

			EQC_SetScrollTabData(hControl, wList, pList);

            yPos += EQC_GET_DY(hControl) + ySpace;  
#ifdef FEATURE_PHONE_COLOR_ID_LCD
            // 그룹 LCD COLOR 설정 
            hControl = EQC_StaticText( Margin.xLeft , yPos , TKN_GROUP_BACKLIGHT_COLOR );
            xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;  
            yPos += EQC_GET_DY(hControl) + ySpace;  

            for(nIndex=0; nIndex< NUM_COLOR; nIndex++) //MAX_ALERT_ID_LCD_NUMBER; nIndex++)
			{
                pList[nIndex] = (BYTE *)SKY_GET_TOKEN(GroupColor[nIndex]);
			}
			pList[nIndex] = (BYTE*)SKY_GET_TOKEN(TKN_GROUP_LIGHT_NONE);

			nColorindex = GroupInfo.alert_color;
			if(GroupInfo.alert_color == NONE_GROUP_ALERT_LCD)
				nColorindex = 8;
			else if(BLUE_LCD_COLOR <= GroupInfo.alert_color &&  GroupInfo.alert_color <= BLUE_GREEN_LCD_COLOR)//WHITE_LCD_COLOR)
				nColorindex += 1;
			else if(GroupInfo.alert_color == RAINBOW_LCD_COLOR)
				nColorindex = 0;
			else
				nColorindex = 8;

            hControl = EQC_ScrollCombo(Margin.xLeft, yPos, GROUP_EDIT_CONTROL_LEN, 3, pList, \
                                       NUM_COLOR+1, \
                                       NUM_COLOR+1, nColorindex);  
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_ENUMBERATED);

            yPos += EQC_GET_DY(hControl) + ySpace;  
#endif //FEATURE_PHONE_COLOR_ID_LCD

			if( GroupInfo.wMember != nSize )
			{
				GroupInfo.wMember = nSize;
				ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
			}
            EQS_FitCanvasSize();
        }
        break;
        
    case SE_APP_END :   /* The very last event */ 
        if( l_nMyBell > 0 )
        {
            for( nIndex=0; nIndex < l_nMyBell; nIndex++)
                SKY_Free(l_ppMyBellList[nIndex]);

        }
/*#ifdef FEATURE_CHARACBELL
        if( l_nChaBell > 0 )
        {
            for( nIndex=0; nIndex < l_nChaBell; nIndex++)
                SKY_Free(l_ppChaBellList[nIndex]);

        }
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
        if( l_nChaBell > 0 )
        {
            for( nIndex=0; nIndex < l_nChaBell; nIndex++)
                SKY_Free(l_ppChaBellList[nIndex]);

        }
#endif // FEATURE_CHARACTER_BELL

		SKY_StopMusic();

#ifdef FEATURE_PHONE_COLOR_ID_LCD
        if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_ALWAYS || \
            (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME && SKY_IsSubBacklightOnTime()) ||\
            SKY_GET_NO_OFF_BACKLIGHT() || SKY_IS_HANDS_FREE())
        {
            if(!SKY_IS_PHONE_OPEN())
                SKY_SubBacklight_Confirm(SKY_GET_AUTO_SUBBACKLIGHT());
            else
                SKY_SubBacklight_Confirm(NONE_LCD_COLOR);
        }
        else
		{
            SKY_SubBacklight_Confirm(NONE_LCD_COLOR);
		}
#endif //FEATURE_PHONE_COLOR_ID_LCD
        break;
              
    case SE_APP_RESUME:
        if( EQS_GET_CHILD() == sSKA_AccessSecretGroup )
        {
            if(  pEvent->dwParam == 0 )  // 비밀번호가 맞지 않는 경우 종료한다.
            {
                EQS_END();               
            }			
        } 
        break;        

    case SE_SOFTKEY_SAVE:
		EQS_SEND_EVENT( SE_USER_1, EQS_NA, EQS_NA);
		break;

    case SE_USER_1:

        EQC_GetTextEditData( EQS_GetNthIOC(EQC_NAME_EDIT), GroupInfo.group_name);
        if( STRLEN(GroupInfo.group_name) == 0 )
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_INPUT_NAME_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
            break;
        }
        else
        {
			nIndex = SearchGroup(GroupInfo.group_name);
            if( nIndex > 0 )
            {
                if( nIndex != GroupInfo.address )
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAME_GROUP_EXIST_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);
                    break;
                }
            }
        }
		hControl = EQS_GetNthIOC(EQC_BELL_ID);
        //nBellGroup = EQC_GetScrollCurrentTab( hControl );
		nBellGroup = EQC_GetScrollTabPos(hControl, EQC_GetScrollPhysicalIndex(hControl));
        if( nBellGroup == (EQC_GetScrollTabSize(hControl)-1) )
		{
            GroupInfo.Bell.wBellID = 0;
		}
/*#ifdef FEATURE_CHARACBELL
        else if( nBellGroup == (NUM_BELL_GROUP-1) ) // Characbell
        {
            GroupInfo.Bell.wBellID = SKY_CHARAC_BELL_ID;
            nIndex = EQC_GetScrollPhysicalIndex(hControl);
            STRCPY(GroupInfo.Bell.szFileName,\
                   EQC_GetScrollData(EQS_GetNthIOC(EQC_BELL_ID),nIndex) );
        }
        else if( nBellGroup == (NUM_BELL_GROUP-2) ) // MyBell
        {
            GroupInfo.Bell.wBellID = SKY_MY_BELL_ID;
            nIndex = EQC_GetScrollPhysicalIndex(hControl);
            STRCPY(GroupInfo.Bell.szFileName,\
                   EQC_GetScrollData(EQS_GetNthIOC(EQC_BELL_ID),nIndex) );
        }
#else
        else if( nBellGroup == (NUM_BELL_GROUP-1) ) // MyBell
        {
            GroupInfo.Bell.wBellID = SKY_MY_BELL_ID;
            nIndex = EQC_GetScrollPhysicalIndex(hControl);
            STRCPY(GroupInfo.Bell.szFileName,\
                   EQC_GetScrollData(EQS_GetNthIOC(EQC_BELL_ID),nIndex) );
        }
#endif // FEATURE_CHARACBELL*/
#ifdef FEATURE_CHARACTER_BELL
        else if( nBellGroup == NUM_BELL_GROUP) // Characbell
        {
            GroupInfo.Bell.wBellID = SKY_CB_GROUPBELL;
            nIndex = EQC_GetScrollPhysicalIndex(hControl);
            STRCPY(GroupInfo.Bell.szFileName,\
                   EQC_GetScrollData(EQS_GetNthIOC(EQC_BELL_ID),nIndex) );
        }
#endif // FEATURE_CHARACTER_BELL
        else if( nBellGroup == (NUM_BELL_GROUP-1) ) // MyBell
        {
            GroupInfo.Bell.wBellID = SKY_MY_BELL_ID;
            nIndex = EQC_GetScrollPhysicalIndex(hControl);
            STRCPY(GroupInfo.Bell.szFileName,\
                   EQC_GetScrollData(EQS_GetNthIOC(EQC_BELL_ID),nIndex) );
        }
		else
        {
			nBellIndex = EQC_GetScrollPhysicalIndex(hControl);
            GroupInfo.Bell.wBellID = aBell_List[nBellIndex].wBellId; 
        }

#ifdef FEATURE_PHONE_COLOR_ID_LCD
        nIndex = EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(EQC_LCD_ID));

        if(nIndex == NUM_COLOR)
        {
            nIndex = NONE_GROUP_ALERT_LCD;
        }else if(nIndex == 0)
        {
            nIndex = RAINBOW_LCD_COLOR;
        }else if(nIndex <=NUM_COLOR -1 && nIndex >=1)
        {
            nIndex -= 1;
        } 
		GroupInfo.alert_color = (ID_LCD_COLOR_E)nIndex;		
#endif //FEATURE_PHONE_COLOR_ID_LCD
        //GroupInfo.status = EQC_GetCheckBox(EQS_GetNthIOC(EQC_GROUP_STATUS),0);
		GroupInfo.status = EQC_GetScrollLogicalIndex( EQS_GetNthIOC(EQC_GROUP_STATUS) );
		
        if( GroupInfo.status  ) // 비밀 그룹        
		{
            SET_SECRET_GROUP(GroupInfo.address,TRUE);
		}
        else
		{		
            SET_SECRET_GROUP(GroupInfo.address,FALSE);
		}

        STRCPY(g_szGroupName[GroupInfo.address-1], GroupInfo.group_name );
        ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);

        hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG), MESSAGE_DEFAULT_TIME,\
                                  MBT_NONE, MBT_HELP);

		if( pEvent->dwParam == EQS_NA)
			EQC_SET_USERDATA(hControl, SE_APP_END);
		else
			EQC_SET_USERDATA(hControl, pEvent->dwParam);

        break;
                
    case SE_TM_TIMER1:
        if( EQS_HaveFocus(EQS_GetNthIOC(EQC_BELL_ID))== TRUE )
        {
            sPlayBell();
        }
        break;

    case SE_TM_TIMER2: 
#ifdef FEATURE_PHONE_COLOR_ID_LCD
        s_wColorIndex %= 6;
        SKY_SubBacklight_Confirm(s_wColorIndex);
        s_wColorIndex ++;
#endif //FEATURE_PHONE_COLOR_ID_LCD
		break;

    case SE_SOFTKEY_MENU:

        break;    
	case SE_IO_FOCUS_CHG:
		SKY_StopMusic();
		EQS_StopTimer(EQS_TIMER_2);
		if( pEvent->wParam == EQS_GetNthIOC(EQC_BELL_ID) )
		{	
#ifdef FEATURE_PHONE_COLOR_ID_LCD
			EQS_StopTimer(EQS_TIMER_2);
            SKY_SubBacklight_Confirm(NONE_LCD_COLOR);
#endif //FEATURE_PHONE_COLOR_ID_LCD			
			EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
		}
#ifdef FEATURE_PHONE_COLOR_ID_LCD
		else if( pEvent->wParam == EQS_GetNthIOC(EQC_LCD_ID) )
		{			
			nColorindex = EQC_GetScrollLogicalIndex(pEvent->wParam);
			if(nColorindex == 0)
			{
				EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
				break;
			}
			else if(nColorindex == NONE_LCD_COLOR + 1)
			{
				nColorindex = SKY_GET_IDLCD_COLOR_ALERT();
				if(nColorindex == RAINBOW_LCD_COLOR)
				{
					EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
					break;
				}
				else
				{
					//nColorindex -= 1;
					SKY_SubBacklight_Confirm(nColorindex);
				}
			}
			else
			{
				nColorindex -= 1;
				SKY_SubBacklight_Confirm(nColorindex);
			}
		}
#endif // FEATURE_PHONE_COLOR_ID_LCD
		else 
		{
#ifdef FEATURE_PHONE_COLOR_ID_LCD
			EQS_StopTimer(EQS_TIMER_2);
            SKY_SubBacklight_Confirm(NONE_LCD_COLOR);
#endif //FEATURE_PHONE_COLOR_ID_LCD
			;
		}

		break;
    case SE_IO_TEDIT_DONE:
        break;  
    case SE_IO_TEDIT_CANCEL:
        break;
	case SE_IO_SCROLL_LIST_CANCEL:
		if( pEvent->wParam == EQS_GetNthIOC(EQC_BELL_ID) )
		{
#ifdef FEATURE_CHARACTER_BELL
            nBellGroup = EQC_GetScrollTabPos(pEvent->wParam, EQC_GetScrollPhysicalIndex(pEvent->wParam));
            memset(temp1, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);
            memset(temp2, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);

            //STRCPY(temp1, EQC_GetStaticText(EQC_BELL_TEXT));
            STRCPY(temp1, EQC_GetStaticText(EQS_GetFocusedIOC()-1));
            loc = STRSTR (temp1, (BYTE*)SF_CHARACBELL_2);

            if (loc == NULL)
            { // 기존에 캐릭벨이 아니었던 경우
                if (nBellGroup == NUM_BELL_GROUP) 
                { // 지금은 캐릭벨로 변경
                    sprintf(temp2, "%s%s%s", temp1, SF_CHARACBELL_2, SF_CHARACBELL_3);
                    //EQC_SetStaticText(EQC_BELL_TEXT, temp2);
                    EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
                }
            }
            else 
            { // 기존에 캐릭벨이었던 경우
                if (nBellGroup != NUM_BELL_GROUP)
                { // 지금은 캐릭벨이 아님.
                    STRNCPY(temp2, temp1, STRLEN(temp1)-4);
                    //EQC_SetStaticText(EQC_BELL_TEXT, temp2);
                    EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
                }
            }
#endif // FEATURE_CHARACTER_BELL
			EQS_StartTimer(EQS_TIMER_1, 0, FALSE);
			SKY_StopMusic();
			break;
		}
#ifdef FEATURE_PHONE_COLOR_ID_LCD
		if( pEvent->wParam == EQS_GetNthIOC(EQC_LCD_ID) )
		{
			nColorindex = EQC_GetScrollLogicalIndex(pEvent->wParam);
			if(nColorindex == 0)
			{
				EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
				break;
			}
			else if(nColorindex == NONE_LCD_COLOR + 1)
			{
				nColorindex = SKY_GET_IDLCD_COLOR_ALERT();
				if(nColorindex == RAINBOW_LCD_COLOR)
				{
					EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
					break;
				}
				else
				{
					//nColorindex -= 1;
					SKY_SubBacklight_Confirm(nColorindex);
				}
			}
			else
			{
				nColorindex -= 1;
				SKY_SubBacklight_Confirm(nColorindex);
			}
		}
#endif
		break;
    case SE_IO_SCROLL_LIST_SELECT:
		if( pEvent->wParam == EQS_GetNthIOC(EQC_BELL_ID) )
		{		
			//EQS_StartTimer(EQS_TIMER_1, 0, FALSE);
			SKY_StopMusic();	

/*#ifdef FEATURE_CHARACTER_BELL
            nBellGroup = EQC_GetScrollTabPos(pEvent->wParam, EQC_GetScrollPhysicalIndex(pEvent->wParam));
            memset(temp1, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);
            memset(temp2, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);

            //STRCPY(temp1, EQC_GetStaticText(EQC_BELL_TEXT));
            STRCPY(temp1, EQC_GetStaticText(EQS_GetFocusedIOC()-1));
            loc = STRSTR (temp1, (BYTE*)SF_CHARACBELL_2);

            if (loc == NULL)
            { // 기존에 캐릭벨이 아니었던 경우
                if (nBellGroup == NUM_BELL_GROUP) 
                { // 지금은 캐릭벨로 변경
                    sprintf(temp2, "%s%s%s", temp1, SF_CHARACBELL_2, SF_CHARACBELL_3);
                    //EQC_SetStaticText(EQC_BELL_TEXT, temp2);
                    EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
                }
            }
            else 
            { // 기존에 캐릭벨이었던 경우
                if (nBellGroup != NUM_BELL_GROUP)
                { // 지금은 캐릭벨이 아님.
                    STRNCPY(temp2, temp1, STRLEN(temp1)-4);
                    //EQC_SetStaticText(EQC_BELL_TEXT, temp2);
                    EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
                }
            }
#endif // FEATURE_CHARACTER_BELL          */  
		}

#ifdef FEATURE_PHONE_COLOR_ID_LCD
		else if( pEvent->wParam == EQS_GetNthIOC(EQC_LCD_ID) )
		{
			EQS_StopTimer(EQS_TIMER_2);

			nColorindex = EQC_GetScrollLogicalIndex(pEvent->wParam);
			if(nColorindex == 0)
			{
				EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
				break;
			}
			else if(nColorindex == NONE_LCD_COLOR + 1)
			{
				nColorindex = SKY_GET_IDLCD_COLOR_ALERT();
				if(nColorindex == RAINBOW_LCD_COLOR)
				{
					EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
					break;
				}
				else
				{
					//nColorindex -= 1;
					SKY_SubBacklight_Confirm(nColorindex);
				}
			}
			else
			{
				nColorindex -= 1;
				SKY_SubBacklight_Confirm(nColorindex);
			}

		}
#endif //FEATURE_PHONE_COLOR_ID_LCD
		else ;

        break;  
		
	case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
		if( pEvent->wParam == EQS_GetNthIOC(EQC_BELL_ID) )
		{
#ifdef FEATURE_CHARACTER_BELL
            nBellGroup = EQC_GetScrollTabPos(pEvent->wParam, EQC_GetScrollPhysicalIndex(pEvent->wParam));
            memset(temp1, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);
            memset(temp2, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);

            //STRCPY(temp1, EQC_GetStaticText(EQC_BELL_TEXT));
            STRCPY(temp1, EQC_GetStaticText(EQS_GetFocusedIOC()-1));
            loc = STRSTR (temp1, (BYTE*)SF_CHARACBELL_2);

            if (loc == NULL)
            { // 기존에 캐릭벨이 아니었던 경우
                if (nBellGroup == NUM_BELL_GROUP) 
                { // 지금은 캐릭벨로 변경
                    sprintf(temp2, "%s%s%s", temp1, SF_CHARACBELL_2, SF_CHARACBELL_3);
                    //EQC_SetStaticText(EQC_BELL_TEXT, temp2);
                    EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
                }
            }
            else 
            { // 기존에 캐릭벨이었던 경우
                if (nBellGroup != NUM_BELL_GROUP)
                { // 지금은 캐릭벨이 아님.
                    STRNCPY(temp2, temp1, STRLEN(temp1)-4);
                    //EQC_SetStaticText(EQC_BELL_TEXT, temp2);
                    EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
                }
            }
#endif // FEATURE_CHARACTER_BELL

			SKY_StopMusic();
			EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
		}
#ifdef FEATURE_PHONE_COLOR_ID_LCD
		else if( pEvent->wParam == EQS_GetNthIOC(EQC_LCD_ID) )
		{
			EQS_StopTimer(EQS_TIMER_2);
			nColorindex = EQC_GetScrollLogicalIndex(pEvent->wParam);
			if(nColorindex == 0)
			{
				EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
				break;
			}
			else if(nColorindex == NONE_LCD_COLOR + 1)
			{
				nColorindex = SKY_GET_IDLCD_COLOR_ALERT();
				if(nColorindex == RAINBOW_LCD_COLOR)
				{
					EQS_StartTimer(EQS_TIMER_2, 50, TRUE);
					break;
				}
				else
				{
					//nColorindex -= 1;
					SKY_SubBacklight_Confirm(nColorindex);
				}
			}
			else
			{
				nColorindex -= 1;
				SKY_SubBacklight_Confirm(nColorindex);
			}
		}
#endif //FEATURE_PHONE_COLOR_ID_LCD
		else
			;

		break;


	case SE_KEY_CLEAR:
		if( EQS_IsControlDataChanged() == FALSE)
		{
			EQS_END();
		}
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_KEY_CLEAR);
		}
        break;		
    case SE_KEY_END:
		if( EQS_IsControlDataChanged() == FALSE)
		{
			EQS_END_CURRENT_EVENT();
		}
		else
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG),\
				                      10, MBT_YESNO, MBT_CONFIRM);
			EQC_SET_USERDATA(hControl, SE_KEY_END);
		}        
		break;

	case SE_IO_MESGBOX_END:
    case SE_IO_MESGBOX_NO:
		if( pEvent->dwParam == SE_KEY_END )
		{
			EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
		}
		else if( pEvent->dwParam == SE_APP_END || pEvent->dwParam == SE_KEY_CLEAR )
		{
			EQS_END();
		}			
		else ;

		break;
    case SE_IO_MESGBOX_YES:
		EQS_SEND_EVENT( SE_USER_1, EQS_NA, pEvent->dwParam );
		break;

    case SE_APP_IO_MODE:
        if(EQS_GetIOCOrder((UINT8)(pEvent->wParam)) == EQC_BELL_ID )
        {
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        }
        break;
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

VOID SKY_API SKA_EditGroup(SKY_EVENT_T* pEvent)
{  
    static  INT16   nSelectedIndex;
    static  GROUP_EDIT_MENU_E  GroupMenu;
    HCONTROL        hControl;
    UINT8           nSize, ListHandle;
    INT16           Value,i, nGroupAddress;

    pb_group_info_type   CurrentInfo , PreviousInfo;
    INT16 index, nPosition;
        
    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_MENU_SELECT_NEWGROUP, 
            SKY_GET_TOKEN(TKN_GROUP_EDIT),
            WA_SHOW_CML);
            //WA_SHOW_HEADER|WA_SHOW_CML);

        nSelectedIndex = -1;
        hControl = sMakeScrollList(EQS_NA,EQS_NA, EQS_NA, \
                                   EQC_GetScrollDisplayLine(), 0, \
                                   (UINT8)0,pEvent->wParam,SDE_GROUP_EDIT);
#if BOOK_FLOW_MODE
		EQC_SetScrollFlowMode(hControl, TRUE);
#endif
        GroupMenu = GEM_NON_FUNCTION;
        if( hControl == 0 )
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_EDITGROUP_EXIST_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP) ;
            break;
        }
        ListHandle = EQC_GetScrollListHandle( hControl );
        if( ListHandle != 0 )
        {
            EQC_SetLogicalList(hControl, TRUE);
            EQC_SetScrollTitle(hControl, (BYTE*) SKY_GET_TOKEN(TKN_GROUP_EDIT));
            EQC_ShowScrollTitle(hControl,TRUE);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | \
				                    CS_SCROLL_LIST_ENUMBERATED|\
                                    CS_SCROLL_LIST_MAXIMIZE  |\
									CS_SCROLL_LIST_NO_CML);           
            nSelectedIndex = -1;
            EQS_SetIOCActive( hControl );

        }             
        else   //redstar 2000.09.29
        {
            EQS_END();
        }

        break;
    case SE_APP_END:
        PhoneBookUpdate();
        break;

    case SE_APP_RESUME:
        if( EQS_GET_CHILD() == sSKA_AccessSecretGroup )
        {
            if(  pEvent->dwParam != 0 )  // 비밀번호가 맞는 경우만 해당 작업을 한다.
            {
                switch( GroupMenu )
                {
                case GEM_DELETE_GROUP:
				case GEM_DELETE_GROUP_NAME:
                    {
                         BYTE* Mesg[40];
                         hControl = EQS_GetNthIOC(0);        
                         sprintf(Mesg,SKY_GET_TOKEN(TKN_PBOOK_STRING_DELETE_ASK), \
                                 EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl)));
                         EQC_MessageBox(Mesg, 10, MBT_YESNO, MBT_CONFIRM);
                    }
                    return;
                default:
                    break;
                }
            }
        }        
        else if( EQS_GET_CHILD() == sSKA_EditGroupContent )
        {
            hControl = EQS_GetNthIOC(0);
            index   = EQC_GetScrollLogicalIndex(hControl);
            sPBookDisplayScroll(hControl, (WORD)index, FALSE, SDE_GROUP_EDIT);
        }
		else if( EQS_GET_CHILD() == sSKA_GetString )
		{
			if( pEvent->wParam )
			{
				if( STRLEN( l_szParam ) > 0 )
				{
					// add new group - GEM_ADD_GROUP 
					if( SearchGroup(l_szParam) != 0 )   // 이미 같은 이름의 그룹 존재
					{    
						EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAME_GROUP_EXIST_MSG),\
									   MESSAGE_DEFAULT_TIME,\
									   MBT_NONE,MBT_HELP);
					}
					else
					{
						EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SECRET_GROUP_ASK_MSG), \
									   10, MBT_NOYES, MBT_CONFIRM);
					}
					break;
				}
			}
		}  
        else ;

        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_SOFTKEY_NEWGROUP:
		if( IsFullGroup() == TRUE )
		{
			GroupMenu = GEM_INFO_MSG;
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_GROUP_MSG),\
									  MESSAGE_DEFAULT_TIME,\
									  MBT_NONE, MBT_HELP);
		}
		else
		{
			GroupMenu = GEM_ADD_GROUP;
			l_szParam[0] = 0;
			EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START,\
					              INPUT_NEW_GROUP_NAME_E, EQS_NA);
		}
		break;
    case SE_SOFTKEY_MENU:
        {
            SKY_TOKEN paGroupEditMenu[5] =
            {
                //TKN_PBOOK_GROUP_ADD,              // 그룹 추가
                TKN_PBOOK_GROUP_DELETE,           // 그룹 삭제
				TKN_PBOOK_GROUP_NAME_DELETE,      // 그룹이름 삭제 
                TKN_PBOOK_GROUP_EDIT_ORDER,       // 그룹순서 변경
                TKN_PBOOK_GROUP_SORT              // 그룹 정열
            };
			BOOL fActiveMenu[5];

			for(i=0; i<5; i++)
			{
				fActiveMenu[i] = TRUE;
			}

			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);    
            index = GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl));
            if( KOREAN_STRCMP( g_szGroupName[index-1] , SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) ) == 0 )
            {
				fActiveMenu[0] = FALSE;
				fActiveMenu[1] = FALSE;
			}
			if( g_nGroup < 2 )
			{
				fActiveMenu[2] = FALSE;
				fActiveMenu[3] = FALSE;
			}
            hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paGroupEditMenu, 4, 0, fActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
        break;
    //case SE_IO_POPUP_SELECT_CANCEL:     
    //    break;
    case SE_IO_POPUP_CANCEL:
        break;
    case SE_IO_POPUP_SELECT:
        index = pEvent->dwParam;
        hControl = EQS_GetNthIOC(0);
        ListHandle = EQC_GetScrollListHandle(hControl);
        //GroupMenu = (GROUP_EDIT_MENU_E)(index+1);        
        switch( pEvent->dwParam )
        {
		case TKN_PBOOK_GROUP_ADD: // 그룹 추가
			GroupMenu = GEM_ADD_GROUP;
			if( IsFullGroup() == TRUE )
			{
				GroupMenu = GEM_INFO_MSG;
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_FULL_GROUP_MSG),\
										  MESSAGE_DEFAULT_TIME,\
										  MBT_NONE, MBT_HELP);
			}
			else
			{
				l_szParam[0] = 0;
				EQS_START_CHILD_EVENT(sSKA_GetString, SE_APP_START,\
					                  INPUT_NEW_GROUP_NAME_E, EQS_NA);
			}
            break;

        case TKN_PBOOK_GROUP_DELETE:           // 그룹 삭제   
		case TKN_PBOOK_GROUP_NAME_DELETE:      // 그룹이름 삭제 

			if( pEvent->dwParam == TKN_PBOOK_GROUP_DELETE )
				GroupMenu = GEM_DELETE_GROUP;
			else
				GroupMenu = GEM_DELETE_GROUP_NAME;
			index = GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl));
#if 0			
            if( KOREAN_STRCMP( g_szGroupName[index-1] , SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) ) == 0 )
            {
                GroupMenu = GEM_INFO_MSG;
                EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_INVALID_DELETE_GROUP_MSG),\
                               MESSAGE_DEFAULT_TIME,\
                               MBT_NONE, MBT_HELP);
            }
            else
#endif
            {
                if( IS_SECRET_GROUP(index) == TRUE )
                {
                    EQS_START_CHILD(sSKA_AccessSecretGroup);
                }
                else
                {
                     BYTE* Mesg[40];
					 if( GroupMenu == GEM_DELETE_GROUP )
					 {
						sprintf(Mesg,SKY_GET_TOKEN(TKN_PBOOK_STRING_DELETE_ASK), \
							     EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl)));
					 }
					 else
					 {  //GEM_DELETE_GROUP_NAME
					 				
					    sprintf(Mesg,SKY_GET_TOKEN(TKN_PBOOK_GROUP_NAME_DELETE_ASK), \
						        EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl)));
					 }

                     EQC_MessageBox(Mesg, 10, MBT_YESNO, MBT_CONFIRM);
                }
            }
            break;
		
        case TKN_PBOOK_GROUP_EDIT_ORDER:       // 그룹순서 변경
			{
				BYTE szTemp[40];

				GroupMenu = GEM_EDIT_ORDER;
				nSelectedIndex = EQC_GetScrollLogicalIndex( hControl);
				STRCPY(szTemp, SF_PBOOK_ARROW);
				STRCPY(szTemp+2, EQC_GetScrollData(hControl, EQC_GetScrollPhysicalIndex(hControl))+2);

				EQC_SetScrollData( hControl, EQC_GetScrollPhysicalIndex(hControl) , szTemp); 
				EQC_SET_STYLE( hControl, EQC_GET_STYLE(hControl)|CS_SCROLL_LIST_SWAP );
				EQC_SetScrollListAccelKeySetting(hControl, FALSE);
				EQS_SetIOCActive(hControl);
				EQS_SetWindowCML(CL_NONE_COMPLETE_NONE);
			}
            break;
        case TKN_PBOOK_GROUP_SORT:              // 그룹 정열
			GroupMenu = GEM_SORT_GROUP;
            if( ListHandle > 0 )
            {
                DeleteList( ListHandle );
            }             		
            ListHandle = SortGroup();
			PhoneBookUpdate();
#if 0
			nSize = GetListSize(ListHandle);
			if( nSize > EQC_GetScrollDataLine(hControl) )
			{
				nSize = EQC_GetScrollDataLine(hControl);
			}
#endif                
            for(index=0; index< EQC_GetScrollViewLine(hControl); index++)
            {
                sMakeFormattedString(EQC_GetScrollData(hControl, index), \
                                     ListHandle, index, SDE_GROUP_EDIT); 
            }
            EQC_SetScrollListHandle( hControl, ListHandle);		
            EQC_RePositionScrollList(hControl);
            
            EQS_SetIOCActive(hControl);
            break;
        default:
			GroupMenu = GEM_NON_FUNCTION;
            break;
        }
        break;              
    case SE_IO_SCROLL_LIST_SELECT:
        hControl = pEvent->wParam;
        index = pEvent->dwParam;    
        ListHandle = EQC_GetScrollListHandle( hControl );
        if( nSelectedIndex == -1 )   // Edit Group Content
        {   
            EQS_START_CHILD_EVENT(sSKA_EditGroupContent, SE_APP_START,\
                                  GetListValue(ListHandle,index), EQS_NA);          
        }
        else                                                
        {   // 그룹 순서 변경..
            if( index != nSelectedIndex)
            {
                CurrentInfo.address = GetListValue( ListHandle, nSelectedIndex );
                ui_get_pb( PB_GROUP_INFO_I, &CurrentInfo );
                             
                PreviousInfo.address = GetListValue( ListHandle, nSelectedIndex - 1); 
                if( ui_get_pb( PB_GROUP_INFO_I, &PreviousInfo ) )
                {
                    PreviousInfo.next_index = CurrentInfo.next_index;
                    ui_put_pb( PB_GROUP_INFO_I, &PreviousInfo);
            
                }            
                else
                {                               
                    active_list.group_info  = CurrentInfo.next_index;
                    ui_put_pb( PB_ACTIVE_LIST_I,&active_list);                
                }

                if( index == 0 )
                    PreviousInfo.address = 0;
                else
                {
                    if( nSelectedIndex < index )
                        PreviousInfo.address = GetListValue( ListHandle, index);
                    else
                        PreviousInfo.address = GetListValue( ListHandle, index-1);
                }

                if( ui_get_pb( PB_GROUP_INFO_I, &PreviousInfo) )  // 새로 삽입할 곳의 이전 레코드 
                {
                    CurrentInfo.next_index   =   PreviousInfo.next_index;
                    PreviousInfo.next_index  =   CurrentInfo.address;
                    ui_put_pb( PB_GROUP_INFO_I, &PreviousInfo);
                }
                else
                {
                    CurrentInfo.next_index  = active_list.group_info;                                 
                    active_list.group_info  = CurrentInfo.address;
                    ui_put_pb( PB_ACTIVE_LIST_I,&active_list);                
                }
                                
                ui_put_pb( PB_GROUP_INFO_I, &CurrentInfo);

                RemoveListItem(ListHandle, nSelectedIndex);
                InsertList(ListHandle, index, CurrentInfo.address, EQS_NA);
                nSize = GetListSize(ListHandle);
                for(i=0; i< nSize; i++)
                {
                    //g_GroupOrder[GetListValue(ListHandle,i)-1] = i+1;
					nGroupAddress = GetListValue(ListHandle,i);
					if( IS_SECRET_GROUP(nGroupAddress) )
					{
						SET_GROUP_ORDER(nGroupAddress, -(i+1));
					}
					else
					{
						SET_GROUP_ORDER(nGroupAddress, i+1);
					}
                }
            }
            sMakeFormattedString(EQC_GetScrollData(hControl, \
                                 EQC_GetScrollPhysicalIndex(hControl)),\
                                 ListHandle, index, SDE_GROUP_EDIT);   
            
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) & ~CS_SCROLL_LIST_SWAP );

            nSelectedIndex = -1;
			EQC_SetScrollListAccelKeySetting(hControl, TRUE);
            EQS_SetIOCActive(hControl);  
			EQS_SetWindowCML(CL_MENU_SELECT_NEWGROUP);
        }
        break;
    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            TRUE, SDE_GROUP_EDIT);
        break;

    case SE_IO_SCROLL_LIST_DELETE:
        hControl = pEvent->wParam;
        index = pEvent->dwParam;
        if( index == 0 )        
            EQS_END();
        break;
        index = pEvent->dwParam;
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            TRUE, SDE_GROUP_EDIT);
        break;
    case SE_IO_SCROLL_LIST_UP:
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle(hControl);
        nPosition  = EQC_GetScrollPhysicalIndex(hControl);
        if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SWAP )
        {
            if( index <= nSelectedIndex )
                index--;
            if( nPosition == (EQC_GetScrollDataLine(hControl)-1) )
                nPosition = 0;
            else
                nPosition++;
        }
        sMakeFormattedString(EQC_GetScrollData(hControl, nPosition),\
                             ListHandle, index, SDE_GROUP_EDIT);  
        break;

    case SE_IO_SCROLL_LIST_DOWN:

        hControl = pEvent->wParam;
        index = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle(hControl);
        nPosition = EQC_GetScrollPhysicalIndex(hControl);
        if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SWAP )
        {           
            if( index <= nSelectedIndex )
                index--;
            if( nPosition == 0 )
                nPosition = EQC_GetScrollDataLine(hControl) -1;
            else
                nPosition--;
        }    
        sMakeFormattedString(EQC_GetScrollData(hControl, nPosition),\
                             ListHandle, index, SDE_GROUP_EDIT);   
        break;


    case SE_IO_TEDIT_DONE:
        break;  
    case SE_IO_TEDIT_CANCEL:
        break;
	case SE_IO_MESGBOX_CLEAR:
	case SE_IO_MESGBOX_CANCEL:
		EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;
    case SE_IO_MESGBOX_END:
        if( GroupMenu == GEM_NON_FUNCTION)
		{
            EQS_END();
		}
        else
        {
            if( GroupMenu == GEM_DELETE_GROUP || GroupMenu == GEM_DELETE_GROUP_NAME )
            {
                hControl = EQS_GetNthIOC(0);
                ListHandle = EQC_GetScrollListHandle(hControl);
                index =  EQC_GetScrollLogicalIndex(hControl);
                CurrentInfo.address = GetListValue( ListHandle, index);
                                              
				if( GroupMenu == GEM_DELETE_GROUP )
				{
	                DeleteGroup( CurrentInfo.address , TRUE );
				}
				else
				{
					DeleteGroup( CurrentInfo.address , FALSE );
				}
                PhoneBookUpdate();
                RemoveListItem( ListHandle,index);
                
                Value = EQC_DeleteScrollData( hControl, EQC_GetScrollLogicalIndex( hControl));

                if( EQC_GetScrollMaxDataLine(hControl) == 0 )
                {
                    EQS_END();
                    break;
                }
#if 0
				nSize = GetListSize(ListHandle) - Value;			
				if( nSize > EQC_GetScrollDataLine(hControl) )
				{
					nSize = EQC_GetScrollDataLine(hControl);
				}
#endif

				nSize = EQC_GetScrollViewLine(hControl);
                for( i= 0; i<nSize; i++)
                {
                    sMakeFormattedString(EQC_GetScrollData(hControl, i),\
                                         ListHandle, Value+i, SDE_GROUP_EDIT);  
                }
				EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), MESSAGE_DEFAULT_TIME,\
					                         MBT_NONE, MBT_HELP);
				GroupMenu = GEM_INFO_MSG;
				break;
            }
            GroupMenu = GEM_NON_FUNCTION;
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        }
        break;
    case SE_IO_MESGBOX_YES:   // 비밀그룹설정 OK
    case SE_IO_MESGBOX_NO:    // 비밀그룹설정 NO
        switch( GroupMenu )
        {            
        case GEM_ADD_GROUP:          // 새 그룹 생성
            hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);
                     
            if( pEvent->EventID == SE_IO_MESGBOX_NO )
            {
				//nGroupAddress = AddGroup(l_szParam, FALSE);
				nGroupAddress = InsertGroup(l_szParam, g_nGroup, FALSE);
				if( nGroupAddress == 0 )
					break;
            }
            else
            {
				//nGroupAddress = AddGroup(l_szParam, TRUE);
				nGroupAddress = InsertGroup(l_szParam, g_nGroup, TRUE);
				if( nGroupAddress == 0 )
					break;
            }
			PhoneBookUpdate();

            index = GET_GROUP_ORDER(nGroupAddress) - 1;
            Value = EQC_AddScrollData( hControl, index, NULL, FALSE);
			AddList(ListHandle, nGroupAddress, EQS_NA);
			//InsertList(ListHandle, index, nGroupAddress, EQS_NA);            

            if( EQC_GetScrollDataLine(hControl) < EQC_GetScrollViewLine(hControl) )
                nSize = EQC_GetScrollDataLine(hControl);
            else
                nSize = EQC_GetScrollViewLine(hControl);
            for( i= 0; i< nSize; i++)
            {
                sMakeFormattedString(EQC_GetScrollData(hControl, i),\
                                     ListHandle, Value+i, SDE_GROUP_EDIT);  
            }
			EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG), MESSAGE_DEFAULT_TIME,\
                                         MBT_NONE, MBT_HELP);
			GroupMenu = GEM_INFO_MSG;         
   
            break;
        case GEM_DELETE_GROUP:       // 그룹 삭제
            if( pEvent->EventID == SE_IO_MESGBOX_YES )
            {
               hControl =  EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_GROUP_DELETING),\
                                          0, MBT_NONE, MBT_HELP);
               break;
            }
            else 
			{
				GroupMenu = GEM_NON_FUNCTION;
				EQS_SetIOCActive(EQS_GetNthIOC(0));
			}
			break;
		case GEM_DELETE_GROUP_NAME:  //그룹 이름 삭제 
            if( pEvent->EventID == SE_IO_MESGBOX_YES )
            {
               hControl =  EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_GROUP_NAME_DELETING),\
                                          0, MBT_NONE, MBT_HELP);
               break;
            }
            else 
			{
				GroupMenu = GEM_NON_FUNCTION;
				EQS_SetIOCActive(EQS_GetNthIOC(0));
			}
			break;            
        default:
            EQS_SetIOCActive(EQS_GetNthIOC(0));
            break;
        }
        break;    
    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

enum tagVIEW_BOOKLIST_MSG_E
{     
    VIEW_BOOKLIST_DELETE_MSG_E = 1,
	VIEW_BOOKLIST_INFO_MSG_E,
	VIEW_BOOKLIST_ASK_DELETE_MSG_E,
	VIEW_BOOKLIST_NO_DATA_MSG_E
};

VOID SKA_ViewBookList(SKY_EVENT_T *pEvent)
{      
    static BOOK_FUNCTION_E FunctionMode;
    HCONTROL            hControl;
    int					i,index;
    BOOL				fAllDraw;        
    INT16				nDeletePos,nIndex, ListHandle, nGroupAddress;   
    pb_group_info_type  GroupInfo;
    pb_book_info_type   BookInfo;
	pb_number_info_type NumberInfo;
	BYTE                szTemp[40];

    switch (pEvent->EventID)
    {
    case SE_APP_START:    
        FunctionMode = (BOOK_FUNCTION_E) pEvent->dwParam;

		if( pEvent->wParam == 0 ) // 전체리스트 
		{
			EQS_SetWindow(SKY_DEF_WIN_X,
				          SKY_DEF_WIN_Y, 
					      SKY_DEF_WIN_DX, 
						  SKY_DEF_WIN_DY, 
						  CL_MENU_SELECT_NEWNAME, 
						  SKY_GET_TOKEN(TKN_PBOOK_ENTIRE_LIST),
						  WA_SHOW_HEADER|WA_SHOW_CML);
		}
		else
		{
			EQS_SetWindow(SKY_DEF_WIN_X,
				          SKY_DEF_WIN_Y, 
					      SKY_DEF_WIN_DX, 
						  SKY_DEF_WIN_DY, 
						  CL_MENU_SELECT_NEWNAME, 
						  "",
						  WA_SHOW_CML);
		}

        if( FunctionMode == BFT_VIEW_IRDA_E )
        {
            EQS_SetWindowCML(CL_NONE_SEND_NONE);
            hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA, \
                                       EQC_GetScrollDisplayLine(), \
                                       0,(UINT8)0, pEvent->wParam, SDE_IRDA_BOOK); 
        }
        else if( FunctionMode == BFT_VIEW_EMAIL_LIST_E )
        {
            ListHandle = EQC_GetScrollListHandle(pEvent->wParam);
            index      = EQC_GetScrollLogicalIndex(pEvent->wParam);
          
            EQS_SetWindowCML(CL_NONE_SELECT_NONE);
            hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA, \
                                       EQC_GetScrollDisplayLine(),\
                                       0,(UINT8)GetListType(ListHandle,index),\
                                       GetListValue(ListHandle,index), SDE_EMAIL_BOOK_LIST);
        }
		else if( FunctionMode == BFT_GET_GROUP_TEL_E )
		{
			EQS_SetWindowCML(CL_MENU_SELECT_ADD);
            hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA, \
                                       EQC_GetScrollDisplayLine(), \
                                       0, (UINT8)0, pEvent->wParam, SDE_GROUP_TEL_LIST);
		}
        else
        {
			if( FunctionMode == BFT_SAVE_OLD_DATA_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_NONE);
			}        
			else if( FunctionMode == BFT_EDIT_SDIAL_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_NONE);
			}
			else if( FunctionMode == BFT_EDIT_SDIAL_E  || FunctionMode == BFT_SEARCH_BOOK_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_NONE);
			}							
			else ;

            hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA, \
                                       EQC_GetScrollDisplayLine(), \
                                       0, (UINT8)0, pEvent->wParam, SDE_BOOK_LIST);
        }
        if( hControl == 0 )
        {
            hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_VALID_DATA_MSG),\
									  MESSAGE_DEFAULT_TIME,\
									  MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_NO_DATA_MSG_E);
            break;
        }
        EQC_SET_USERDATA(hControl, pEvent->wParam); // save Group Address	
        ListHandle = EQC_GetScrollListHandle( hControl );
#if BOOK_FLOW_MODE  		
		EQC_SetScrollFlowMode(hControl, TRUE);
#endif
        if( ListHandle != 0 )
        {            
			BYTE szBuf[20];
            //EQC_SetScrollTitle(hControl, EQC_GetScrollTitle(hControl));
            EQC_ShowScrollTitle(hControl,TRUE);
            EQC_SetLogicalList(hControl, TRUE);    //redstar   00.12.13

			if( pEvent->wParam == 0 ) // 전체리스트 
			{
				WORD  TabData[15] = {0,0,0,0,0, 0,0,0,0,0,  0,0,0,0,0};
				BYTE* pTabText[15] = \
				{ "ㄱ","ㄴ","ㄷ","ㄹ","ㅁ","ㅂ","ㅅ","ㅇ","ㅈ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ","AZ\0"};

								

				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
					                    CS_SCROLL_LIST_TAB|\
										CS_SCROLL_LIST_NO_CML|\
					                    CS_SCROLL_LIST_MAXIMIZE ); //CS_SCROLL_LIST_STATUS_BAR
				EQC_SetScrollTab(hControl,15,7,TRUE,0);
				for(i=0; i< GetListSize(ListHandle); i++)
				{
					TabData[GetKoreanCho(g_szPersonName[GetListValue(ListHandle,i)-1])]++;
				}
				EQC_SetScrollTabData(hControl, TabData, pTabText);
				EQC_SetScrollLogicalIndex(hControl,0);

			}
			else
			{
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
					                    CS_SCROLL_LIST_NO_CML|\
					                    CS_SCROLL_LIST_MAXIMIZE ); //CS_SCROLL_LIST_STATUS_BAR
			}
			
			sprintf(szBuf, SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
				    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
			EQC_SetScrollStatusBarText(hControl, szBuf);
			
            EQS_SetIOCActive( hControl );          
        }
        else  // 메모리 할당 에러
        {
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
				                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_NO_DATA_MSG_E);
			// EQS_END();            
        }  
        break;
    case SE_APP_END:
		FunctionMode = BFT_NON_FUNCTION_E;
        break;
    case SE_APP_RESUME:       

		if( EQS_GET_CHILD() == SKA_SelectNumber ) // 단축다이얼 지정 
		{
			if( pEvent->wParam == TKN_PBOOK_SET_SDIAL_SELECT_MSG )
			{
                EQS_START_CHILD_EVENT(SKA_EditSDial, SE_APP_START, (WORD)pEvent->dwParam, EQS_NA);
				break;
			}
			else if( pEvent->wParam == TKN_PBOOK_SEND_SMS_SELECT_MSG )
			{
			   pb_number_info_type numberinfo;
           
			   numberinfo.address = pEvent->dwParam;
			   ui_get_pb( PB_NUMBER_INFO_I, &numberinfo);
			   nIndex = STRLEN(numberinfo.digits);
			   memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T)); 
			   STRCPY(g_UIMOBuffer.pszRcvNumber, numberinfo.digits);
			   g_UIMOBuffer.nRcvNumberSize = nIndex;
			   EQS_START_CHILD(SKA_SMSEditMessage);
			   break;
			}
			else ;

		}
		else if( EQS_GET_CHILD() == sSKA_SelectEmailID ) // 이메일 결정 
		{           
			if( pEvent->dwParam )
			{
#if 0
				hControl = EQS_GetNthIOC(0);
                ListHandle = EQC_GetScrollListHandle(hControl);
                index = EQC_GetScrollLogicalIndex(hControl);
				EQS_END_EVENT(SE_APP_RESUME, EQS_NA, (DWORD)GetListValue(ListHandle,index));
				break;
#endif
				EQS_END_EVENT(SE_APP_RESUME, EQS_NA, pEvent->dwParam);
				break;

			}
		}
		else ;

        hControl = EQS_GetNthIOC(0);
        if( FunctionMode == BFT_EDIT_SDIAL_E  )   //단축 다이얼 편집
		{
			if( pEvent->dwParam > 0 )
			{
				EQS_END_EVENT( SE_APP_RESUME, 0, pEvent->dwParam);				
			}
			else
			{
				EQS_SetIOCActive(hControl);
			}
		}
		else if( FunctionMode == BFT_SEARCH_BOOK_E )  //전화부 검색 
        {
			if( pEvent->dwParam > 0 )
			{
				EQS_END_EVENT( SE_APP_RESUME, 0, pEvent->dwParam);
#if 0
				ListHandle = CreateList(1);
				AddList(ListHandle, pEvent->dwParam, EQS_NA);
				EQS_END_EVENT( SE_APP_RESUME, 0, ListHandle);
#endif
			}
			else
				EQS_SetIOCActive(hControl);
        }
		else if( FunctionMode == BFT_GET_GROUP_TEL_E )
		{
			if( pEvent->dwParam )
			{
				ListHandle = CreateList(1);
				AddList(ListHandle, pEvent->dwParam, EQS_NA);
				EQS_END_EVENT( SE_APP_RESUME, 0, ListHandle);
			}
			else
			{
				EQS_SetIOCActive(hControl);
			}	
		}
        else if( FunctionMode == BFT_EDIT_GROUP_E )
        {
            EQS_SetIOCActive(hControl);
        }
        else 
            EQS_SetIOCActive(hControl);


        if( EQS_GET_CHILD() == SKA_EditBookContent )  //주의 
        {
            ListHandle = EQC_GetScrollListHandle(hControl);
            index = EQC_GetScrollLogicalIndex(hControl);
            if( pEvent->dwParam )  // book address
            {
				if( FunctionMode == BFT_SAVE_OLD_DATA_E )
				{
					EQS_END_TO( EQS_POP_TO_ROOT );
					return ;
				}
                nGroupAddress = EQC_GET_USERDATA(hControl);
				if( nGroupAddress == 0 || nGroupAddress == g_PersonGroup[pEvent->dwParam-1] )
                   // 그룹이 변경되지 않은 경우 or 전체 리스트
                {
					WORD  nSize;
					INT16 nNewBookAddress;

					nNewBookAddress = pEvent->dwParam;
					if( pEvent->wParam == 0)
					{   //overwrite 가 아닌 경우 
						RemoveListItem(ListHandle, index);
						nSize = GetListSize(ListHandle);
						if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
						{ 
							UINT8 nTab;
							EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);
							nTab = GetKoreanCho(g_szPersonName[nNewBookAddress-1]);
							EQC_SetScrollTabDataSize(hControl, nTab, EQC_GetScrollTabDataSize(hControl, nTab)+1);
						}
						
						for(i=0; i<nSize; i++)
						{
							if( KOREAN_STRCMP( g_szPersonName[nNewBookAddress-1],\
											   g_szPersonName[GetListValue(ListHandle, i)-1] ) <= 0 )
							{
								InsertList(ListHandle, i, nNewBookAddress, EQS_NA);
								break;
							}
						}
						if( i == nSize )
						{
							AddList(ListHandle, nNewBookAddress, EQS_NA);
						}
						index = i;
					}
					else
					{ // overwrite 인 경우 .. 전체리스트만 이곳으로 진입 
						RemoveListItem(ListHandle, index);
						if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
						{ 
							EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);
						}
						EQC_SetScrollMaxDataLine( hControl, GetListSize(ListHandle) );
						index = FindList(ListHandle, pEvent->wParam );
						SetListValue(ListHandle, index, nNewBookAddress);
					}

					EQC_SetScrollLogicalIndex(hControl, index);
					fAllDraw = TRUE;
                }
                else
                {   //전체 리스트가 아니고  그룹이 변경된 경우 
                    RemoveListItem(ListHandle,index);
                    if( GetListSize(ListHandle) == 0 )
                    { 
                        EQS_END();  //01.03.25   for refreshing speed search information
                        //EQS_END_TO(EQS_POP_TO_ROOT);  /그룹의 아이템이 완전 변경되어서 없는 경우이므로 초기화면으로 이동한다.
                        //EQS_END_EVENT(SE_PIN_UP, EQS_NA, EQS_NA);
                        return ;
                    }
					index = EQC_DeleteScrollData(hControl,\
						                         EQC_GetScrollLogicalIndex(hControl));
                    fAllDraw = TRUE;
                }
                if( FunctionMode == BFT_VIEW_IRDA_E  )
                {      
                    sPBookDisplayScroll(hControl, index,fAllDraw , SDE_IRDA_BOOK);
                }
                else
                {
                    sPBookDisplayScroll(hControl, index,fAllDraw, SDE_BOOK_LIST);
                }
            }
			else
			{ //아이템이 변경되었을 경우 이를 화면에 반영하기 위해 
				EQC_SetScrollLogicalIndex(hControl, index);
			}
        }
		else if( EQS_GET_CHILD() == SKA_ViewBookContent )
		{ // 전화부 삭제된 경우 
            ListHandle = EQC_GetScrollListHandle(hControl);
            index = EQC_GetScrollLogicalIndex(hControl);
			if( pEvent->dwParam )
			{
				RemoveListItem(ListHandle,index);                   
				if( GetListSize(ListHandle) == 0 )
				{ 
					EQS_END();  //01.03.25   for refreshing speed search information
					return ;
				}
				index = EQC_DeleteScrollData(hControl,\
											 EQC_GetScrollLogicalIndex(hControl));

				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
				{
					EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);
					index = EQC_GetScrollLogicalIndex(hControl);
					EQC_SetScrollLogicalIndex(hControl, index);
				}
				else
				{
					fAllDraw = TRUE;
					if( FunctionMode == BFT_VIEW_IRDA_E  )
					{      
						sPBookDisplayScroll(hControl, index,fAllDraw , SDE_IRDA_BOOK);
					}
					else
					{
						sPBookDisplayScroll(hControl, index,fAllDraw, SDE_BOOK_LIST);
					}
				}
			}
			else// 부분 항목  삭제의  경우 
			{
				EQC_SetScrollLogicalIndex(hControl, index);
				EQS_SetIOCActive(hControl);
				break;            
			}

		}
		else if( EQS_GET_CHILD() == SKA_BookInput )
		{
			if( pEvent->dwParam )
			{
				WORD  nSize;
				INT16 nNewBookAddress, Address;

			    nNewBookAddress = pEvent->dwParam;
			    nGroupAddress = EQC_GET_USERDATA(hControl);
				if( nGroupAddress == 0 || nGroupAddress == g_PersonGroup[nNewBookAddress-1] )
				{
					ListHandle = EQC_GetScrollListHandle(hControl);			
					nSize = GetListSize(ListHandle);

					if( l_nBookOverWriteAddress == 0) 
					{ // over write가 아닌 경우 
						for(i=0; i<nSize; i++)
						{
							Address = GetListValue(ListHandle,i);
							if( g_szPersonName[Address-1][0] == 0 || nNewBookAddress == Address )
							{
								UINT8 nTab;

								EQC_SetScrollMaxDataLine( hControl, nSize-1);
								nTab = GetKoreanCho(g_szPersonName[Address-1]);
								EQC_SetScrollTabDataSize(hControl, nTab, EQC_GetScrollTabDataSize(hControl, nTab)-1);
								RemoveListItem(ListHandle, i);
								break;
							}
						}
						nSize = GetListSize(ListHandle);
						for(i=0; i<nSize; i++)
						{
							if( KOREAN_STRCMP( g_szPersonName[nNewBookAddress-1],\
											   g_szPersonName[GetListValue(ListHandle, i)-1] ) <= 0 )
							{
								InsertList(ListHandle, i, nNewBookAddress, EQS_NA);
								break;
							}
						}
						if( i == nSize )
						{
							AddList(ListHandle, nNewBookAddress, EQS_NA);
						}
						index = i;
						EQC_SetScrollMaxDataLine( hControl, GetListSize(ListHandle) );

						if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
						{
							UINT8 nTab;				
							nTab = GetKoreanCho(g_szPersonName[nNewBookAddress-1]);
							EQC_SetScrollTabDataSize(hControl, nTab, EQC_GetScrollTabDataSize(hControl, nTab)+1);
						}

					}
					else
					{ //overwrite 인 경우 
						index = FindList(ListHandle, pEvent->wParam );
						SetListValue(ListHandle, index, nNewBookAddress);
					}

					EQC_SetScrollLogicalIndex(hControl, index);
					fAllDraw = TRUE;
					if( FunctionMode == BFT_VIEW_IRDA_E  )
					{      
						sPBookDisplayScroll(hControl, index,fAllDraw , SDE_IRDA_BOOK);
					}
					else
					{
						sPBookDisplayScroll(hControl, index,fAllDraw, SDE_BOOK_LIST);
					}
				}
			}

		}
		else ;

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		{
			BYTE szBuf[20];
			sprintf(szBuf, SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
				    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
			EQC_SetScrollStatusBarText(hControl, szBuf);
		}
		
        break;    
        
	case SE_SOFTKEY_ADD:    //동보 메시지 .... 추가 명령
		hControl = EQS_GetNthIOC(0);
        ListHandle = EQC_GetScrollListHandle(hControl);
		BookInfo.address = GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl));
		index = 0;
		ui_get_pb( PB_BOOK_INFO_I, &BookInfo);
		NumberInfo.address = BookInfo.number_address;
		ListHandle = CreateList(1);		
		while( index < PERSONAL_NUMBER_LIMIT )
		{
			if( ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) > 0 )
			{
				index++;
				if( NumberInfo.number_type == CELL )
				{
					AddList(ListHandle, NumberInfo.address, EQS_NA);
					EQS_END_EVENT(SE_APP_RESUME, EQS_NA, ListHandle);
					return ;
				}
				else
				{
					NumberInfo.address = NumberInfo.next_index;
				}
			}
			else
			{
				break;
			}
		}		
		EQS_END_EVENT(SE_APP_RESUME, EQS_NA, EQS_NA);
		break;
	case SE_SOFTKEY_NEWNAME:
	    nGroupAddress = EQC_GET_USERDATA(EQS_GetNthIOC(0));
		EQS_START_CHILD_EVENT( SKA_BookInput, SE_APP_START, nGroupAddress, EQS_NA );
		break;
    case SE_SOFTKEY_MENU:
        {
            SKY_TOKEN paMenu[3] =
            {
				TKN_PBOOK_SEND_SMS,    //메시지 작성 
				TKN_PBOOK_SET_SDIAL,        //단축다이얼 지정
                TKN_PBOOK_ONE_BOOK_DELETE, //전화부 삭제 
                
            };
			BOOL  fActiveMenu[3];
			for( i=0; i<3; i++)
			{
				fActiveMenu[i] = TRUE;
			}
			hControl = EQS_GetNthIOC(0);
            ListHandle = EQC_GetScrollListHandle(hControl);
            BookInfo.address  = GetListValue(ListHandle, \
                                         EQC_GetScrollLogicalIndex(hControl));

			for( i=0; i< PERSONAL_NUMBER_LIMIT; i++ )
			{
				if( g_PersonNumberType[BookInfo.address-1][i] )
					break;
			}

			if( l_fFromCall )
			    fActiveMenu[0] = FALSE;

			if( i == PERSONAL_NUMBER_LIMIT )
			{
				fActiveMenu[0] = FALSE;
				fActiveMenu[1] = FALSE;
			}

			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 3, 0, fActiveMenu);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }  
        break;
    case SE_IO_POPUP_CANCEL:
        hControl = EQS_GetNthIOC(0);
        EQS_SetIOCActive(hControl);
        break;   
       
    case SE_IO_POPUP_SELECT:
	
        switch( pEvent->dwParam )
        {
        case  TKN_PBOOK_ONE_BOOK_DELETE:  // 전화부 삭제 
			{

				BYTE szBuf[100];
#if 0				
				hControl = EQS_GetNthIOC(0);
				index =  EQC_GetScrollPhysicalIndex(hControl);
				STRCPY( szBuf, EQC_GetScrollData(hControl, index));
				STRCAT( szBuf,(BYTE*)"\n");
				STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));
#endif
				hControl = EQS_GetNthIOC(0);
				ListHandle = EQC_GetScrollListHandle(hControl);
				BookInfo.address  = GetListValue(ListHandle, \
											 EQC_GetScrollLogicalIndex(hControl));

				STRCPY( szBuf, g_szPersonName[BookInfo.address-1]);
				STRCAT( szBuf,(BYTE*)"\n");
				STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));

				hControl = EQC_MessageBox(szBuf, 10, MBT_YESNO, MBT_CONFIRM);
				EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_ASK_DELETE_MSG_E);
			}
            break;
        case TKN_PBOOK_SET_SDIAL:  // 단축다이얼 지정
			hControl = EQS_GetNthIOC(0);
            ListHandle = EQC_GetScrollListHandle(hControl);
            BookInfo.address  = GetListValue(ListHandle, \
                                         EQC_GetScrollLogicalIndex(hControl));

			EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
								  BookInfo.address,\
								  TKN_PBOOK_SET_SDIAL_SELECT_MSG);	


            break;

		case TKN_PBOOK_SEND_SMS: // 메시지 작성 
            if(SKY_IS_SMS_LOCK() && pEvent->wParam != SMSCODE_IN_VIEW_BOOK_LIST )
            {
                EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_IN_VIEW_BOOK_LIST, EQS_NA);
            }else
            {
                hControl = EQS_GetNthIOC(0);
                ListHandle = EQC_GetScrollListHandle(hControl);
                BookInfo.address  = GetListValue(ListHandle, \
                                             EQC_GetScrollLogicalIndex(hControl));
			    EQS_START_CHILD_EVENT(SKA_SelectNumber, SE_APP_START,\
								      BookInfo.address,\
								      TKN_PBOOK_SEND_SMS_SELECT_MSG);
            }

			break;
        default:
            break;
        }                
        break;
	case SE_IO_MESGBOX_CLEAR:
	case SE_IO_MESGBOX_CANCEL:
		EQS_SetIOCActive(EQS_GetNthIOC(0));

		break;	

	case SE_IO_MESGBOX_END:  
		hControl = EQS_GetNthIOC(0);
		if( pEvent->dwParam == VIEW_BOOKLIST_DELETE_MSG_E) // "삭제중 " 메시지 
		{			
			ListHandle = EQC_GetScrollListHandle(hControl);
			index =  EQC_GetScrollLogicalIndex(hControl);
                      
			BookInfo.address = GetListValue( ListHandle, index );
			if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) == 0 )
				break;

			GroupInfo.address = BookInfo.group_address;
			if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) == 0)
				break;    
        
			DeleteBook(&GroupInfo, &BookInfo, TRUE); //2001.01.08
			ui_put_pb(PB_GROUP_INFO_I, &GroupInfo);
			PhoneBookUpdate();        

			RemoveListItem( ListHandle,index);
        
			nDeletePos = EQC_DeleteScrollData( hControl, \
										EQC_GetScrollLogicalIndex( hControl));

			if( EQC_GetScrollMaxDataLine(hControl) == 0 )
			{
				EQS_END();
				break;
			}

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
			{
				EQC_SetScrollCurrentTabDataSize(hControl, EQC_GetScrollCurrentTabDataSize(hControl)-1);
				index = EQC_GetScrollLogicalIndex(hControl);
				EQC_SetScrollLogicalIndex(hControl, index);
/*
				if( EQC_GetScrollCursor(hControl) == 0 )
				{
					index = EQC_GetScrollLogicalIndex(hControl);
					if( index  == (EQC_GetScrollMaxDataLine(hControl)-1) )
						EQC_SetScrollLogicalIndex(hControl, index);
					else
						EQC_SetScrollLogicalIndex(hControl, index+1);
				}
				else
				{
					EQC_SetScrollLogicalIndex(hControl, EQC_GetScrollLogicalIndex(hControl));
				}
*/

				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG),\
										  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_INFO_MSG_E);
				break;
			}
			else
			{
				for( i= 0; i< EQC_GetScrollViewLine(hControl); i++)
				{
					sMakeFormattedString(EQC_GetScrollData(hControl, i),\
										 ListHandle, nDeletePos+i, SDE_BOOK_LIST);  
				}
			}

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
			{
				BYTE szBuf[20];
				sprintf(szBuf, SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
					    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
				EQC_SetScrollStatusBarText(hControl, szBuf);
			}

			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG),\
				                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_INFO_MSG_E);	
		}
		else if( pEvent->dwParam == VIEW_BOOKLIST_NO_DATA_MSG_E )
		{
			EQS_END();
		}
		else  // VIEW_BOOKLIST_INFO_MSG
		{
			EQS_SetIOCActive(hControl);	
		}        		
		break;

	case SE_IO_MESGBOX_NO:

		hControl = EQS_GetNthIOC(0);
		EQS_SetIOCActive(hControl);	
		
		break;
	case SE_IO_MESGBOX_YES:
		if( pEvent->dwParam == VIEW_BOOKLIST_ASK_DELETE_MSG_E )
		{
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ON_DELETING),\
				                      0, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_DELETE_MSG_E);
		}
		else
		{
			hControl = EQS_GetNthIOC(0);
			EQS_SetIOCActive(hControl);	
		}
		break;

    case SE_IO_SCROLL_LIST_SELECT:
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );        
        switch( FunctionMode )
        {
        case BFT_EDIT_SDIAL_E :
			BookInfo.address = GetListValue( ListHandle, index );
			for( index = 0; index < PERSONAL_NUMBER_LIMIT; index++)
			{
				if( g_PersonNumberType[BookInfo.address-1][index] )
					break;
			}

			if( index == PERSONAL_NUMBER_LIMIT )
			{
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_NUMBER_EXIST_MSG),\
										  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			}
			else
			{        
				EQS_START_CHILD_EVENT( SKA_SelectNumber, SE_APP_START,\
					                   BookInfo.address, EQS_NA );
			}
            break;
        
        case BFT_EDIT_GROUP_E :
        
            EQS_START_CHILD_EVENT( SKA_ViewBookContent, SE_APP_START, \
                                   GetListValue( ListHandle, index ), \
                                   BFT_EDIT_GROUP_E);
            break;
        
        case BFT_SAVE_OLD_DATA_E : //편집         
            EQS_START_CHILD_EVENT( SKA_EditBookContent, SE_APP_START, \
                                   GetListValue( ListHandle, index ), \
                                   BFT_SAVE_OLD_DATA_E);
            break;
        case BFT_EDIT_PBOOK_E : //편집 
            EQS_START_CHILD_EVENT( SKA_EditBookContent, SE_APP_START, \
                                   GetListValue( ListHandle, index ), \
                                   BFT_EDIT_PBOOK_E);
			break;
        
        case BFT_VIEW_IRDA_E :
			EQS_END_EVENT(SE_APP_RESUME, EQS_NA, (DWORD)GetListValue(ListHandle,index));
			break;

        case BFT_VIEW_EMAIL_LIST_E:        
            //EQS_END_EVENT(SE_APP_RESUME, EQS_NA, (DWORD)GetListValue(ListHandle,index));
			EQS_START_CHILD_EVENT(sSKA_SelectEmailID, SE_APP_START,\
								  GetListValue(ListHandle,index),\
								  TKN_PBOOK_SET_SDIAL_SELECT_MSG);	
            break;
        
        case BFT_SEARCH_BOOK_E :
            EQS_START_CHILD_EVENT( SKA_SelectNumber, SE_APP_START, \
                                   GetListValue(ListHandle,index), EQS_NA );
            break;
		case BFT_GET_GROUP_TEL_E:
            EQS_START_CHILD_EVENT( SKA_SelectNumber, SE_APP_START, \
                                   GetListValue(ListHandle,index), EQS_NA );//BFT_GET_GROUP_TEL_E );
			break;
                
        default:                                
            EQS_START_CHILD_EVENT( SKA_ViewBookContent, SE_APP_START, \
                                   GetListValue( ListHandle, index ), \
                                   BFT_NON_FUNCTION_E);
            break;
        }

        break;

	case SE_IO_SCROLL_TAB_CHG:	
        if( FunctionMode == BFT_VIEW_IRDA_E  )
        {      
            sPBookDisplayScroll((HCONTROL)pEvent->wParam,\
				                EQC_GetScrollTabIndex(pEvent->wParam, pEvent->dwParam),\
								TRUE, SDE_IRDA_BOOK);
        }
        else
        {
            sPBookDisplayScroll((HCONTROL)pEvent->wParam,\
				                EQC_GetScrollTabIndex(pEvent->wParam, pEvent->dwParam),\
								TRUE, SDE_BOOK_LIST);
        }

		if( EQC_GetScrollTabDataSize(pEvent->wParam, pEvent->dwParam) == 0 )
		{
			EQS_SetWindowCML(CL_NONE_SELECT_NONE);
		}
		else
		{
			if( FunctionMode == BFT_VIEW_IRDA_E )
			{
				EQS_SetWindowCML(CL_NONE_SEND_NONE);
			}
			else if( FunctionMode == BFT_VIEW_EMAIL_LIST_E )
			{          
				EQS_SetWindowCML(CL_NONE_SELECT_NONE);
			}
			else if( FunctionMode == BFT_SAVE_OLD_DATA_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_NONE);
			}        
			else if( FunctionMode == BFT_EDIT_SDIAL_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_NONE);
			}
			else if( FunctionMode == BFT_EDIT_SDIAL_E  || FunctionMode == BFT_SEARCH_BOOK_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_NONE);
			}
			else if( FunctionMode == BFT_GET_GROUP_TEL_E )
			{
				EQS_SetWindowCML(CL_NONE_SELECT_ADD);
			}
			else 
			{
				EQS_SetWindowCML(CL_MENU_SELECT_NEWNAME);
			}
		}
		break;
    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        if( FunctionMode == BFT_VIEW_IRDA_E  )
        {      
            sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                        TRUE, SDE_IRDA_BOOK);
        }
        else
        {
            sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                        TRUE, SDE_BOOK_LIST);
        }

        break;

    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        if( FunctionMode == BFT_VIEW_IRDA_E  )
        {      
            sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                        FALSE, SDE_IRDA_BOOK);
        }
        else
        {
            sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                        FALSE, SDE_BOOK_LIST);
        }
        break;

    case SE_IO_KEY_1:       
    case SE_IO_KEY_2:       
    case SE_IO_KEY_3:        
    case SE_IO_KEY_4:       
    case SE_IO_KEY_5:
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );   
        BookInfo.address = GetListValue(ListHandle,index);
        index  = pEvent->EventID - SE_IO_KEY_0;
        if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) )
        {
            pb_number_info_type NumberInfo;
            NumberInfo.address = BookInfo.number_address;
            while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) > 0 )
            {
				index--;
                if( index == 0 )
                {
					break;
				}
				else
				{                    
                    NumberInfo.address = NumberInfo.next_index;
                }                                
            }
			if( index > 0 )
				break;

			if( FunctionMode == BFT_GET_GROUP_TEL_E )
			{
				ListHandle = CreateList(1);	
				AddList(ListHandle, NumberInfo.address, EQS_NA);
				EQS_END_EVENT(SE_APP_RESUME, EQS_NA, ListHandle);

			}
			else if( FunctionMode == BFT_EDIT_SDIAL_E  || FunctionMode == BFT_SEARCH_BOOK_E )
			{
				EQS_END_EVENT( SE_APP_RESUME, EQS_NA, NumberInfo.address );
			}
			else 
			{
				//if(SKY_IsInCall() || SKY_IS_IN_WAP() )
				if( l_fFromCall )
					break;	
				sprintf((CHAR*) szTemp,(CHAR*) SKY_GET_TOKEN(TKN_PBOOK_FMT_NAME_TYPE),
                                                    g_szPersonName[NumberInfo.book_address-1],\
													phone_type[NumberInfo.number_type-1]);
				index = STRLEN(szTemp);
				SET_ALPHAEDIT_BUFFER(szTemp, index);
				SET_ALPHAEDIT_POS(index);
				sPBookSendCall( NumberInfo.digits);
			}
        }
		break;
	case SE_IO_KEY_POUND:
		{
			int nSize;
			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);
			nSize = EQC_GetScrollMaxDataLine(hControl);

			index = EQC_GetScrollLogicalIndex(hControl);
			if( (EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB) && EQC_GetScrollCurrentTabDataSize(hControl) == 0 )
			{
				i = index;
			}
			else
			{
				for( i = index+1; i < nSize; i++ )
				{
					if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
										 g_szPersonName[GetListValue(ListHandle, i    )-1]))
					{
						break;
					}
				}
				if( i == nSize )
				{
					for( i = 0; i < index; i++ )
					{
						if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
											 g_szPersonName[GetListValue(ListHandle, i    )-1]))
						{
							break;
						}
					}
				}			
				if( i == index )
					break;
			}

			EQC_SetScrollLogicalIndex(hControl, i);

            if( FunctionMode == BFT_VIEW_IRDA_E  )
            {      
                sPBookDisplayScroll(hControl, i, TRUE , SDE_IRDA_BOOK);
            }
            else
            {
                sPBookDisplayScroll(hControl, i, TRUE, SDE_BOOK_LIST);
            }
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
	case SE_IO_KEY_STAR:
		{
			int nSize;
			hControl = EQS_GetNthIOC(0);
			ListHandle = EQC_GetScrollListHandle(hControl);
			nSize = EQC_GetScrollMaxDataLine(hControl);

			index = EQC_GetScrollLogicalIndex(hControl);
			if( (EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB) && EQC_GetScrollCurrentTabDataSize(hControl) == 0 )
			{
				if( index == 0 )
				{
					i = nSize -1;
				}
				else
				{
					i = index - 1;
				}			
			}
			else
			{
				for( i = index-1; i >= 0; i-- )
				{
					if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
										 g_szPersonName[GetListValue(ListHandle, i    )-1]))
					{
						break;
					}
				}
				if( i < 0 )
				{
					for( i = nSize-1; i  > index; i-- )
					{
						if(!sIsSameFirstName(g_szPersonName[GetListValue(ListHandle, index)-1],\
											 g_szPersonName[GetListValue(ListHandle, i    )-1]))
						{
							break;
						}
					}
				}

				if( i == index )
					break;
			}

			EQC_SetScrollLogicalIndex(hControl, i);
            if( FunctionMode == BFT_VIEW_IRDA_E  )
            {      
                sPBookDisplayScroll(hControl, i, TRUE , SDE_IRDA_BOOK);
            }
            else
            {
                sPBookDisplayScroll(hControl, i, TRUE, SDE_BOOK_LIST);
            }
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
		break;
    case SE_KEY_SEND:
		if( l_fFromCall || EQS_GetModal() != INVALID_CONTROL_HANDLE )
			break;
        sPBookMakeCall(EQS_GetNthIOC(0));

		//SKY_SetGeneralTimer(50,TRUE);
		//EQS_StartTimer( EQS_TIMER_3, 100, TRUE);
        break;
	case SE_TM_TIMER:
		//EQS_SEND_EVENT(SE_KEY_DOWN, EQS_NA, EQS_NA );
		break;
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    } 
}



enum tagEDIT_SDIAL_CONTROL_E
{     
    EDIT_SDIAL_MAINCONTROL_E = 0
};

enum tagEDIT_SDIAL_MESSAGE_E
{     
    EDIT_SDIAL_SAVE_INFO_MSG_E = 1
};

// use dynamic data list -- have fixed data size
VOID SKY_API SKA_EditSDial(SKY_EVENT_T* pEvent)
{    
    UINT8     ListHandle;
    HCONTROL  hControl;
    INT       index;
	BOOK_FUNCTION_E FunctionMode;
      
    pb_sdial_type         sdial;
    pb_number_info_type   snumber_info;

    switch (pEvent->EventID)
    {
    case SE_APP_START:
		FunctionMode = (BOOK_FUNCTION_E) pEvent->dwParam;
	
		EQS_SetWindow(SKY_DEF_WIN_X,\
					  SKY_DEF_WIN_Y,\
					  SKY_DEF_WIN_DX,\
					  SKY_DEF_WIN_DY,\
					  CL_DELETE_SELECT_MODIFY,\
					  SKY_GET_TOKEN(TKN_EDIT_SHORT_DIAL),\
					  WA_SHOW_CML);

		hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA,\
								   EQC_GetScrollDisplayLine(), 0, \
								   (UINT8) 0, EQS_NA,SDE_EDIT_SDIAL);
		
		ListHandle = EQC_GetScrollListHandle( hControl );

		if( ListHandle !=  0 )
		{        
			EQC_SetLogicalList(hControl, TRUE);			     
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl)  | \
									CS_SCROLL_LIST_MAXIMIZE  | \
									CS_SCROLL_LIST_NO_CML    | \
									CS_SCROLL_LIST_ENUMBERATED								
									);
			EQC_SetScrollListAccelKeySetting(hControl, FALSE);
			EQC_SET_USERDATA(hControl, pEvent->wParam);
			
			ListHandle = EQC_GetScrollListHandle( hControl );

			if( EQC_GET_USERDATA(hControl) )
			{
				EQC_SetScrollTitle(hControl,SKY_GET_TOKEN(TKN_PBOOK_SET_SDIAL));
				EQS_SetWindowCML( CL_NONE_SELECT_NONE);//CL_NONE_SELECT_ASSIGN );
			}
			else
			{
				EQC_SetScrollTitle(hControl,SKY_GET_TOKEN(TKN_EDIT_SHORT_DIAL));
				if( GetListValue(ListHandle, 0) )
					EQS_SetWindowCML( CL_DELETE_SELECT_MODIFY );
				else
					EQS_SetWindowCML( CL_NONE_SELECT_NONE );//CL_NONE_SELECT_ASSIGN );
			}
			EQC_ShowScrollTitle(hControl, TRUE);
#if BOOK_FLOW_MODE
			EQC_SetScrollFlowMode(hControl, TRUE);
#endif
			EQS_SetIOCActive( hControl );
		}
		else  // 메모리 할당 에러
		{
			EQS_END();
			break;
		}
        
        break;
    case SE_APP_END:
		FunctionMode = BFT_NON_FUNCTION_E;
        PhoneBookUpdate();
        break;
    case SE_APP_RESUME:
        hControl = EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E);
        index = EQC_GetScrollLogicalIndex( hControl );
        ListHandle = EQC_GetScrollListHandle( hControl );
        
        if( EQS_GET_CHILD() == SKA_SpeedSearch || EQS_GET_CHILD() == SKA_SelectNumber)
        {    
			BYTE szBuf[100];
			memset(szBuf, 0, 100);
            snumber_info.address = pEvent->dwParam;
            if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &snumber_info ) == 0 )
            {
                EQS_SetIOCActive( hControl );
                break;
            }     
			active_list.nFreeSdial--;     //2001.01.22
			sdial.address      = index+1;   //  from  1 to 99  ( scroll index is from 0  to ...
			sdial.dial_address = snumber_info.address;
        
			if( ui_put_pb( PB_SDIAL_I, ( pb_item_type * ) &sdial ) > 0 )
			{
				int nPos, CodePos, HypenPos;

				g_SDialNumber[sdial.address-1] = sdial.dial_address;
				SetListValue(ListHandle, index, snumber_info.address);
				sMakeFormattedString(EQC_GetScrollData(hControl, \
									 EQC_GetScrollPhysicalIndex(hControl)),\
									 ListHandle, index, SDE_EDIT_SDIAL);

				ui_get_pb( PB_NUMBER_INFO_I, &snumber_info);
				sprintf((CHAR*)szBuf,"[%d%d]%s\n%s", sdial.address/10, sdial.address%10,\
					                                  g_szPersonName[snumber_info.book_address-1],\
												      phone_type[snumber_info.number_type-1]);
				nPos = STRLEN(szBuf);
				CodePos  = FindCodeSet(snumber_info.digits);
				HypenPos = FindHypen(snumber_info.digits,CodePos,MAX_HYPEN_DIGIT_LENGTH);
				MakeHypenedNumber(snumber_info.digits, szBuf+nPos,\
					              STRLEN(snumber_info.digits)+1,CodePos,HypenPos);

				STRCAT(szBuf, "\n");
				STRCAT(szBuf, SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG));
				hControl = EQC_MessageBox(szBuf, MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);					
				EQS_SetWindowCML( CL_DELETE_SELECT_MODIFY );				
			}	
			break;                
        }
        else if( EQS_GET_CHILD() == SKA_ViewNumber )
        {
			if( pEvent->dwParam > 0 && pEvent->wParam)
			{
				BYTE szBuf[100];
				int nPos, CodePos, HypenPos;

				active_list.nFreeSdial--;
				sdial.address      = pEvent->wParam;   
				sdial.dial_address = pEvent->dwParam;
    
				if( ui_put_pb( PB_SDIAL_I, ( pb_item_type * ) &sdial ) > 0 )
				{
					g_SDialNumber[sdial.address-1] = sdial.dial_address;

					SetListValue(ListHandle, sdial.address-1, sdial.dial_address);
					EQC_SetScrollLogicalIndex( hControl, sdial.address-1);
					index = EQC_GetScrollViewLine(hControl);
					if( sdial.address <= index )
						sPBookDisplayScroll(hControl, 0, TRUE, SDE_EDIT_SDIAL );
					else
						sPBookDisplayScroll(hControl, sdial.address - index , TRUE, SDE_EDIT_SDIAL );

					snumber_info.address = sdial.dial_address;
					ui_get_pb( PB_NUMBER_INFO_I, &snumber_info);
					sprintf((CHAR*)szBuf,"[%d%d]%s\n%s", sdial.address/10, sdial.address%10,\
														g_szPersonName[snumber_info.book_address-1],\
														phone_type[snumber_info.number_type-1]);
					nPos = STRLEN(szBuf);
					CodePos  = FindCodeSet(snumber_info.digits);
					HypenPos = FindHypen(snumber_info.digits,CodePos,MAX_HYPEN_DIGIT_LENGTH);
					MakeHypenedNumber(snumber_info.digits, szBuf+nPos,\
						              STRLEN(snumber_info.digits)+1,CodePos,HypenPos);					

					STRCAT(szBuf, "\n");
					STRCAT(szBuf, SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG));
					hControl = EQC_MessageBox(szBuf, MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
					EQS_SetWindowCML( CL_DELETE_SELECT_MODIFY );
					break;   
				}				

			}
			else if( pEvent->wParam )
			{   // 단축다이얼을 삭제한 경우 
				EQC_SetScrollLogicalIndex( hControl, pEvent->wParam-1);
				index = EQC_GetScrollViewLine(hControl);
				if( pEvent->wParam <= index )
					sPBookDisplayScroll(hControl, 0, TRUE, SDE_EDIT_SDIAL );
				else
					sPBookDisplayScroll(hControl, pEvent->wParam - index , TRUE, SDE_EDIT_SDIAL );
			}
			else ;
			
        }
        else        
            ;
		EQS_SetIOCActive( hControl );

        break;

    case SE_SOFTKEY_DELETE:    
		{
			BYTE szBuf[80];
			hControl = EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E);
			index    = EQC_GetScrollPhysicalIndex(hControl);

			STRCPY( szBuf, EQC_GetScrollData(hControl, index) );
			STRCAT( szBuf,(BYTE*)"\n");
			STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));
			hControl = EQC_MessageBox(szBuf, 10, MBT_YESNO, MBT_CONFIRM);			
		}
        break;
               
    case SE_SOFTKEY_MODIFY:    // 새 단축다이얼 지정
	case SE_SOFTKEY_ASSIGN:
		hControl = EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E);
		if( EQC_GET_USERDATA(hControl) )    //편집중 단축다이얼 지정을 눌렸을 경우
		{    			
			BYTE szBuf[100];
			int nPos, CodePos, HypenPos;

			hControl = EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E);
			index    = EQC_GetScrollPhysicalIndex(hControl);
			ListHandle = EQC_GetScrollListHandle( hControl );


			active_list.nFreeSdial--;
			sdial.address      = index + 1;
			sdial.dial_address = EQC_GET_USERDATA( hControl );
			ui_put_pb( PB_SDIAL_I, ( pb_item_type * ) &sdial );
			g_SDialNumber[sdial.address-1] = sdial.dial_address;

			SetListValue(ListHandle, sdial.address-1, sdial.dial_address);
			EQC_SetScrollLogicalIndex( hControl, sdial.address-1);
			index = EQC_GetScrollViewLine(hControl);
			if( sdial.address <= index )
				sPBookDisplayScroll(hControl, 0, TRUE, SDE_EDIT_SDIAL );
			else
				sPBookDisplayScroll(hControl, sdial.address - index , TRUE, SDE_EDIT_SDIAL );

			snumber_info.address = sdial.dial_address;
			ui_get_pb( PB_NUMBER_INFO_I, &snumber_info);
			sprintf((CHAR*)szBuf,"[%d%d]%s\n%s", sdial.address/10, sdial.address%10,\
												g_szPersonName[snumber_info.book_address-1],\
												phone_type[snumber_info.number_type-1]);
			//STRCAT(szBuf, snumber_info.digits);
			nPos = STRLEN(szBuf);
			CodePos  = FindCodeSet(snumber_info.digits);
			HypenPos = FindHypen(snumber_info.digits,CodePos,MAX_HYPEN_DIGIT_LENGTH);
			MakeHypenedNumber(snumber_info.digits, szBuf+nPos,\
				              STRLEN(snumber_info.digits)+1,CodePos,HypenPos);	
			STRCAT(szBuf, "\n");
			STRCAT(szBuf, SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG));
			hControl = EQC_MessageBox(szBuf, MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			break;
		}
		else
		{
			EQS_START_CHILD_EVENT(SKA_SpeedSearch, SE_APP_START, \
								  EQS_NA, (DWORD)BFT_EDIT_SDIAL_E );
		}
        break;
    case SE_SOFTKEY_SELECT:
		break;	
    case SE_IO_SCROLL_LIST_SELECT:   // 단축다이얼 지정 혹은 정보 보기
        hControl = pEvent->wParam;

		index = pEvent->dwParam;
		ListHandle = EQC_GetScrollListHandle( hControl );

		if( EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E) == hControl )  //단축다이얼 선택 스크롤 리스트 
		{
			if( EQC_GET_USERDATA(hControl))    //편집중 단축다이얼 지정을 눌렸을 경우
			{         
				BYTE szBuf[100];
				int nPos, CodePos, HypenPos;

				active_list.nFreeSdial--;
				sdial.address      = index+1;
				sdial.dial_address = EQC_GET_USERDATA( hControl );
				ui_put_pb( PB_SDIAL_I, ( pb_item_type * ) &sdial );
				g_SDialNumber[sdial.address-1] = sdial.dial_address;
    
				SetListValue(ListHandle, sdial.address-1, sdial.dial_address);
				EQC_SetScrollLogicalIndex( hControl, sdial.address-1);
				index = EQC_GetScrollViewLine(hControl);
				if( sdial.address <= index )
					sPBookDisplayScroll(hControl, 0, TRUE, SDE_EDIT_SDIAL );
				else
					sPBookDisplayScroll(hControl, sdial.address - index , TRUE, SDE_EDIT_SDIAL );

				snumber_info.address = sdial.dial_address;
				ui_get_pb( PB_NUMBER_INFO_I, &snumber_info);
				sprintf((CHAR*)szBuf,"[%d%d]%s %s\n", sdial.address/10, sdial.address%10,\
													g_szPersonName[snumber_info.book_address-1],\
													phone_type[snumber_info.number_type-1]);
				//STRCAT(szBuf, snumber_info.digits);
				nPos = STRLEN(szBuf);
				CodePos  = FindCodeSet(snumber_info.digits);
				HypenPos = FindHypen(snumber_info.digits,CodePos,MAX_HYPEN_DIGIT_LENGTH);
				MakeHypenedNumber(snumber_info.digits, szBuf+nPos,\
					              STRLEN(snumber_info.digits)+1,CodePos,HypenPos);	
				STRCAT(szBuf, "\n");
				STRCAT(szBuf, SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG));
				hControl = EQC_MessageBox(szBuf, MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				break;					
			}
			else
			{
				if( GetListValue( ListHandle, index ) > 0 )  // 번호 정보가 들어있다.
				{
					EQS_START_CHILD_EVENT( SKA_ViewNumber, SE_APP_START,\
										   index+1, ListHandle);
				}
				else
				{
					EQS_START_CHILD_EVENT(SKA_SpeedSearch, SE_APP_START, \
										  EQS_NA, (DWORD)BFT_EDIT_SDIAL_E );				
				}
			}
		}
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
		hControl = pEvent->wParam;
        sPBookDisplayScroll(hControl, (WORD)pEvent->dwParam,\
                    TRUE, SDE_EDIT_SDIAL);
		ListHandle = EQC_GetScrollListHandle(hControl);

		if( EQC_GET_USERDATA(hControl) )
		{

			EQS_SetWindowCML( CL_NONE_SELECT_NONE );//CL_NONE_SELECT_ASSIGN );
		}
		else
		{
			if( GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl)) )
			{				
				EQS_SetWindowCML( CL_DELETE_SELECT_MODIFY );
			}			
			else
			{				
				EQS_SetWindowCML( CL_NONE_SELECT_NONE ); //CL_NONE_SELECT_ASSIGN );
			}
		}
        break;

    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
		hControl = pEvent->wParam;
        sPBookDisplayScroll(hControl, (WORD)pEvent->dwParam,\
                            FALSE, SDE_EDIT_SDIAL);
		ListHandle = EQC_GetScrollListHandle(hControl);
		if( EQC_GET_USERDATA(hControl) )
		{

			EQS_SetWindowCML( CL_NONE_SELECT_NONE );//CL_NONE_SELECT_ASSIGN );
		}
		else
		{
			if( GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl)) )
			{				
				EQS_SetWindowCML( CL_DELETE_SELECT_MODIFY );
			}			
			else
			{				
				EQS_SetWindowCML( CL_NONE_SELECT_NONE );//CL_NONE_SELECT_ASSIGN );
			}
		}
        break;
    case SE_IO_SCROLL_LIST_CHG:
		hControl = pEvent->wParam;
		ListHandle = EQC_GetScrollListHandle(hControl);
		if( EQC_GET_USERDATA(hControl) )
		{
			EQS_SetWindowCML( CL_NONE_SELECT_NONE );//CL_NONE_SELECT_ASSIGN );
		}
		else
		{
			if( GetListValue(ListHandle, EQC_GetScrollLogicalIndex(hControl)) )
			{				
				EQS_SetWindowCML( CL_DELETE_SELECT_MODIFY );
			}			
			else
			{				
				EQS_SetWindowCML( CL_NONE_SELECT_NONE );//CL_NONE_SELECT_ASSIGN );
			}

		}
		
		break;
		
    case SE_IO_MESGBOX_END:
		if( pEvent->dwParam == EDIT_SDIAL_SAVE_INFO_MSG_E )
		{
			EQS_END();
		}
		else
		{
			hControl = EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E);
			if( EQC_GET_USERDATA(hControl) )
			{
				EQS_END();
			}
			else
			{
				EQS_SetIOCActive( EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E) );
			}
		}    
        break;
	case SE_IO_MESGBOX_YES:
        hControl = EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E);
        index    = EQC_GetScrollLogicalIndex(hControl);
        ListHandle = EQC_GetScrollListHandle( hControl );

        sdial.address      = index+1;   //  from  1 to 99  ( scroll index is from 0  to ...
        sdial.dial_address = EQS_NA;
        active_list.nFreeSdial++;         //2001.01.22
        //sdial.type         = EQS_NA;
        if( ui_put_pb( PB_SDIAL_I, ( pb_item_type * ) &sdial ) > 0 )
        {
			g_SDialNumber[sdial.address-1] = sdial.dial_address;
            SetListValue( ListHandle, index, EQS_NA);
            sMakeFormattedString(EQC_GetScrollData(hControl, \
                                 EQC_GetScrollPhysicalIndex(hControl)),\
                                 ListHandle, index, SDE_EDIT_SDIAL);
			EQS_SetWindowCML(CL_NONE_SELECT_NONE);
        }		
		hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), \
								  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);	
		break;
	case SE_IO_MESGBOX_CLEAR:
	case SE_IO_MESGBOX_NO:
		break;
	case SE_IO_KEY_0:
		EQC_SetScrollLogicalIndex(pEvent->wParam, 0);
		break;
	case SE_IO_KEY_1:
	case SE_IO_KEY_2:
	case SE_IO_KEY_3:
	case SE_IO_KEY_4:
	case SE_IO_KEY_5:
	case SE_IO_KEY_6:
	case SE_IO_KEY_7:
	case SE_IO_KEY_8:
	case SE_IO_KEY_9:
		hControl = (HCONTROL)pEvent->wParam;		
		index    = (INT16)(pEvent->EventID - SE_IO_KEY_0)*10-1;
		EQC_SetScrollLogicalIndex(hControl, index);
        sPBookDisplayScroll(hControl, index, TRUE, SDE_EDIT_SDIAL);
		EQS_SEND_EVENT( SE_APP_DRAW, hControl, EQS_NA);
		break;

	case SE_KEY_SEND:
		if( l_fFromCall )
			break;

		hControl   = EQS_GetNthIOC(EDIT_SDIAL_MAINCONTROL_E);
        index      = EQC_GetScrollLogicalIndex(hControl);
        ListHandle = EQC_GetScrollListHandle( hControl );
		
		snumber_info.address = GetListValue( ListHandle, index );
		if( ui_get_pb( PB_NUMBER_INFO_I, &snumber_info) > 0 )
		{					
			index = STRLEN(snumber_info.digits);
			if( index && IsValid( PB_BOOK_INFO_I, snumber_info.book_address) )
			{
//				SKY_SET_ORIGINATE(TRUE);  hjcho 01.06.20
				SET_NUMEDIT_BUFFER(snumber_info.digits, index);
				SET_NUMEDIT_POS(index);

				index = STRLEN( g_szPersonName[snumber_info.book_address-1] );
				SET_ALPHAEDIT_BUFFER(g_szPersonName[snumber_info.book_address-1], index);
				SET_ALPHAEDIT_POS(index);
                EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				//ui_orig_voice_call( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,\
				//					GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
			}
		}		
		break;

    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

enum tagVIEW_NUMBER_CONTROL_E
{     
	VIEW_NUMBER_NAME_CONTROL_E = 0,	
	VIEW_NUMBER_IMAGE_CONTROL_E,
	VIEW_NUMBER_NUMBER_CONTROL_E
	
};

VOID SKY_API SKA_ViewNumber(SKY_EVENT_T* pEvent)
{
    HCONTROL       hControl;    
	pb_sdial_type  sdial;
    pb_number_info_type  snumber;
	BYTE  szBuf[80];
	BYTE  szName[20];
	static UINT8 hList, nCurrentNo;
	UINT8  i, nLen, nSize;
	RECT   Rect;

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_DELETE_NONE_MODIFY, 
                      SKY_GET_TOKEN(TKN_PBOOK_VIEW_SDIAL),
                      WA_SHOW_HEADER|WA_SHOW_CML);
			
		EQS_GetSWindowViewAreaRect(&Rect);
				
		EQC_Image( 4, Rect.yBottom - g_BmpPBookPannel.Header.Size.yHeight-35,\
			       (BITMAP*)&g_BmpPBookPannel );


		hControl   = EQC_StaticText(3,20, TKN_PBOOK_SAVE);
        nCurrentNo = pEvent->wParam;
		hList      = pEvent->dwParam;
        snumber.address = GetListValue(hList, nCurrentNo-1);
        if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &snumber ) == 0 )
        {
            EQS_END_EVENT(SE_APP_RESUME, EQS_NA, TRUE);
            break;
        }

        strcpy(szName, g_szPersonName[snumber.book_address-1]);
		if( STRLEN(szName) > (MAX_VIEW_BOOK_NAME_LEN+2)  )
		{
			if( IsMiddleOfMultiByte(szName, MAX_VIEW_BOOK_NAME_LEN) )
			{
				STRNCPY(szName,  g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN-1);
				STRNCPY(szName+MAX_VIEW_BOOK_NAME_LEN-1, (BYTE*)SF_ETC_SYMBOL,2);
				szName[MAX_VIEW_BOOK_NAME_LEN+1] = ' ';
				
			}
			else
			{
				STRNCPY(szName, g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN);
				STRNCPY(szName+ MAX_VIEW_BOOK_NAME_LEN, (BYTE*)SF_ETC_SYMBOL,2);
			}
			szName[MAX_VIEW_BOOK_NAME_LEN+2] = 0;
		}        
        memset(szBuf,' ',18);
		szBuf[18] = 0;

        sprintf((CHAR*)szBuf,"[%d%d]%s%s", nCurrentNo/10, nCurrentNo%10,\
			                                szName,\
                                            phone_type[snumber.number_type-1]);
		nLen = STRLEN(szBuf);
		if( nLen < 18 )
			szBuf[nLen] = ' ';
		EQC_SET_SELF_DRAW(hControl, FALSE);
		EQC_SetStaticText(hControl, szBuf);
		//EQC_SetStaticTextAttribute(hControl, (DRAW_MODE)TDM_BOLD);
		EQC_SET_USERDATA(hControl, snumber.book_address);
		EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl));


		//hControl = EQC_NumberEdit(6,52,15,2,PB_MAX_DIGIT_LENGTH,snumber.digits);
		hControl = EQC_NumberEdit(12,48,13,3,PB_MAX_DIGIT_LENGTH,snumber.digits);
		//EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);	
		EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

		EQC_SET_SELF_DRAW(hControl, FALSE);
		EQC_SET_STYLE(hControl,  CS_NUMBER_EDIT_RIGHT2LEFT |\
			                     CS_NUMBER_EDIT_NO_CML     |\
								 CS_NUMBER_EDIT_ORING );
								 //CS_NUMBER_EDIT_BORDER       );
		EQC_SetNumberEditBigFont(hControl,TRUE);
		EQC_SET_USERDATA(hControl, snumber.number_type);
		EQC_SET_FOCUS(hControl,FALSE);	
		
		nSize = 0;
		for( i= 0; i < MAX_SDIAL_NUMBER; i++)
		{
			if( GetListValue(hList, i) > 0 )
			{
				nSize++;
			}				
		}
		
		if( nSize > 1 )
		{
			EQS_SetWindowCML(CL_DELETE_MOVE_MODIFY);
		}

		
        break;
    case SE_APP_END:
		break;
	case SE_APP_RESUME:
		if( EQS_GET_CHILD() == SKA_SpeedSearch )
		{
			if( pEvent->dwParam )
			{
				EQS_END_EVENT( SE_APP_RESUME, nCurrentNo, pEvent->dwParam );
			}
			else ;
		}
		break;
    case SE_KEY_CLEAR:
		EQS_END_EVENT( SE_APP_RESUME, nCurrentNo, EQS_NA);
		break;
	case SE_KEY_UP:
		for( i= nCurrentNo-1; i > 0; i--)
		{
			if( GetListValue(hList, i-1) > 0 )
				break;
		}
		if( i == 0 )
		{
			for( i = MAX_SDIAL_NUMBER; i > nCurrentNo; i-- )
			{
				if( GetListValue(hList, i-1) > 0 )
					break;
			}
			if( i == nCurrentNo )
				break;
		}
		nCurrentNo = i;

		snumber.address = GetListValue(hList, nCurrentNo-1);
        if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &snumber ) == 0 )
			break;
        memset(szBuf,' ',18);
		szBuf[18] = 0;

        strcpy(szName, g_szPersonName[snumber.book_address-1]);
		if( STRLEN(szName) > (MAX_VIEW_BOOK_NAME_LEN+2)  )
		{
			if( IsMiddleOfMultiByte(szName, MAX_VIEW_BOOK_NAME_LEN) )
			{
				STRNCPY(szName,  g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN-1);
				STRNCPY(szName+MAX_VIEW_BOOK_NAME_LEN-1, (BYTE*)SF_ETC_SYMBOL,2);
				szName[MAX_VIEW_BOOK_NAME_LEN+1] = ' ';
				
			}
			else
			{
				STRNCPY(szName, g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN);
				STRNCPY(szName+ MAX_VIEW_BOOK_NAME_LEN, (BYTE*)SF_ETC_SYMBOL,2);
			}
			szName[MAX_VIEW_BOOK_NAME_LEN+2] = 0;
		}

        sprintf((CHAR*)szBuf,"[%d%d]%s%s", nCurrentNo/10, nCurrentNo%10,\
			                               szName,\
                                           phone_type[snumber.number_type-1]);
		nLen = STRLEN(szBuf);
		if( nLen < 18 )
			szBuf[nLen] = ' ';

		hControl = EQS_GetNthIOC(VIEW_NUMBER_NAME_CONTROL_E);   // 이름 		
		EQC_SetStaticText(hControl, szBuf);
		hControl = EQS_GetNthIOC(VIEW_NUMBER_NUMBER_CONTROL_E);   // 번호 		
		EQC_SetNumberEditValue(hControl, snumber.digits);
		EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_APP , EQS_NA);
		break;
    case SE_KEY_DOWN:

        for( i = nCurrentNo+1; i <= MAX_SDIAL_NUMBER; i++)
		{
			if( GetListValue(hList, i-1) > 0 )
				break;
		}
		if( i > MAX_SDIAL_NUMBER )
		{
			for( i = 1; i < nCurrentNo;  i++)
			{
				if( GetListValue(hList,i-1) > 0 )
					break;
			}
		}
		if( i == nCurrentNo )
			break;

		nCurrentNo = i;

		snumber.address = GetListValue(hList, nCurrentNo-1);
        if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &snumber ) == 0 )
			break;
		        
		memset(szBuf,' ',18);
		szBuf[18] = 0;

        strcpy(szName, g_szPersonName[snumber.book_address-1]);
		if( STRLEN(szName) > (MAX_VIEW_BOOK_NAME_LEN+2)  )
		{
			if( IsMiddleOfMultiByte(szName, MAX_VIEW_BOOK_NAME_LEN) )
			{
				STRNCPY(szName,  g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN-1);
				STRNCPY(szName+MAX_VIEW_BOOK_NAME_LEN-1, (BYTE*)SF_ETC_SYMBOL,2);
				szName[MAX_VIEW_BOOK_NAME_LEN+1] = ' ';
				
			}
			else
			{
				STRNCPY(szName, g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN);
				STRNCPY(szName+ MAX_VIEW_BOOK_NAME_LEN, (BYTE*)SF_ETC_SYMBOL,2);
			}
			szName[MAX_VIEW_BOOK_NAME_LEN+2] = 0;
		}

        sprintf((CHAR*)szBuf,"[%d%d]%s%s", nCurrentNo/10, nCurrentNo%10,\
			                               szName,\
                                           phone_type[snumber.number_type-1]);
		nLen = STRLEN(szBuf);
		if( nLen < 18 )
			szBuf[nLen] = ' ';

		hControl = EQS_GetNthIOC(VIEW_NUMBER_NAME_CONTROL_E);   // 이름 
		EQC_SetStaticText(hControl, szBuf);
		hControl = EQS_GetNthIOC(VIEW_NUMBER_NUMBER_CONTROL_E); // 번호 
		EQC_SetNumberEditValue(hControl, snumber.digits);
		EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_APP , EQS_NA);
		
		break;

    case SE_KEY_SEND:
		if( l_fFromCall )
			break;
//      SKY_SET_ORIGINATE(TRUE);  hjcho 01.06.20
		snumber.address = GetListValue(hList, nCurrentNo-1);
        if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &snumber ) == 0 )
			break;
        STRCPY( szBuf, snumber.digits );
		if( STRLEN(snumber.digits) == 0 )
			break;		
        i = STRLEN(szBuf);
        SET_NUMEDIT_BUFFER(szBuf, i);
        SET_NUMEDIT_POS(i);		

        strcpy(szName, g_szPersonName[snumber.book_address-1]);
		if( STRLEN(szName) > (MAX_VIEW_BOOK_NAME_LEN+2)  )
		{
			if( IsMiddleOfMultiByte(szName, MAX_VIEW_BOOK_NAME_LEN) )
			{
				STRNCPY(szName,  g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN-1);
				STRNCPY(szName+MAX_VIEW_BOOK_NAME_LEN-1, (BYTE*)SF_ETC_SYMBOL,2);
				szName[MAX_VIEW_BOOK_NAME_LEN+1] = ' ';
				
			}
			else
			{
				STRNCPY(szName, g_szPersonName[snumber.book_address-1], MAX_VIEW_BOOK_NAME_LEN);
				STRNCPY(szName+ MAX_VIEW_BOOK_NAME_LEN, (BYTE*)SF_ETC_SYMBOL,2);
			}
			szName[MAX_VIEW_BOOK_NAME_LEN+2] = 0;
		}

		sprintf((CHAR*) szBuf,(CHAR*) SKY_GET_TOKEN(TKN_PBOOK_FMT_NAME_TYPE),
                                        szName,\
                                        phone_type[snumber.number_type-1]);
		nLen = STRLEN(szBuf);
		SET_ALPHAEDIT_BUFFER(szBuf, nLen);
		SET_ALPHAEDIT_POS(nLen);
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
		//ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,\
        //                     GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);		
        break;

    case SE_SOFTKEY_DELETE:
		hControl = EQS_GetNthIOC(VIEW_NUMBER_NAME_CONTROL_E);
		STRCPY( szBuf, EQC_GetStaticText(hControl) );
		STRCAT( szBuf,(BYTE*)"\n");
		STRCAT( szBuf, SKY_GET_TOKEN(TKN_PBOOK_ASK_DELETE_MSG));
		hControl = EQC_MessageBox(szBuf, 10, MBT_YESNO, MBT_CONFIRM);
		break;
    case SE_SOFTKEY_MODIFY:
		if( EQS_GET_ANCESTOR(2) == SKA_ViewBookContent ||\
			EQS_GET_ANCESTOR(2) == SKA_ViewBookList    ||\
			EQS_GET_ANCESTOR(2) == SKA_ViewSearchNumber||\
			EQS_GET_ANCESTOR(2) == SKA_ViewSearchName)
		{
			EQS_END_EVENT( SE_APP_RESUME, nCurrentNo, EQS_NA);
		}
		else
		{
			EQS_START_CHILD_EVENT(SKA_SpeedSearch, SE_APP_START, \
								  EQS_NA, (DWORD)BFT_EDIT_SDIAL_E );
		}
		break;

	case SE_IO_MESGBOX_NO:
		break;
	case SE_IO_MESGBOX_YES:
        sdial.address      = nCurrentNo;
        sdial.dial_address = EQS_NA;
        active_list.nFreeSdial++;         //2001.01.22       
        if( ui_put_pb( PB_SDIAL_I, ( pb_item_type * ) &sdial ) > 0 )
        {
			g_SDialNumber[sdial.address-1] = sdial.dial_address;
            SetListValue( hList, nCurrentNo-1, EQS_NA);
        }
		hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_DELETE_MSG), \
			                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA( hControl, nCurrentNo );
		break;
    case SE_IO_MESGBOX_END:
		EQS_END_EVENT( SE_APP_RESUME, pEvent->dwParam, EQS_NA);
		break;

    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

VOID SKY_API SKA_SelectNumber(SKY_EVENT_T* pEvent)
{
    INT8         ListHandle  ;
    HCONTROL     hControl    ;   
    INT16        index, nCount;
    RECT         WindowRect;
    pb_book_info_type        sBookInfo;
    pb_number_info_type      sNumberInfo;
	static DWORD TokenID;


    switch (pEvent->EventID)
    {
    case SE_APP_START:
        
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_NONE, 
                      "",
                      WA_SHOW_CML);
        sBookInfo.address = pEvent->wParam;
		TokenID = pEvent->dwParam;
        if( ui_get_pb( PB_BOOK_INFO_I, &sBookInfo) > 0 )
        {
            ListHandle = CreateList(PERSONAL_NUMBER_LIMIT);
            sNumberInfo.address = sBookInfo.number_address;
			nCount = 0;
            while( ui_get_pb( PB_NUMBER_INFO_I, &sNumberInfo) > 0 && nCount < PERSONAL_NUMBER_LIMIT)
            {
				nCount++;
				if( pEvent->dwParam == BFT_GET_GROUP_TEL_E ) // 동보 메시지 기능 
				{
					if( sNumberInfo.number_type == CELL )
					{
						AddList(ListHandle,sNumberInfo.address, EQS_NA);
					}
				}
				else
				{
					AddList(ListHandle,sNumberInfo.address, EQS_NA);
				}
                sNumberInfo.address = sNumberInfo.next_index;
            }

            if( GetListSize(ListHandle) == 0 )
            {
                DeleteList(ListHandle);
                // 메시지 출력
                EQS_END();
                break;
            }                     
            hControl   = sMakeScrollList(EQS_NA, EQS_NA, EQS_NA, PERSONAL_NUMBER_LIMIT+1, 0, \
                                         (UINT8)0,ListHandle, SDE_SELECT_NUMBER); 
            EQC_SetScrollListHandle(hControl, ListHandle);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_MAXIMIZE);
			if( pEvent->dwParam == 0 )
			{
				EQC_SetScrollTitle(hControl, sBookInfo.name );
			}
			else
			{
				EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TokenID));
			}
            EQC_ShowScrollTitle(hControl,TRUE);
            //EQS_SetWindowCML(CML_CANCEL_SELECT);


            EQS_GetSWindowViewAreaRect(&WindowRect);
/*
            hControl = EQC_StaticText(0,  90, TKN_PBOOK_NO_EDITGROUP_EXIST_MSG);
            EQC_SET_STYLE(hControl, CS_STATIC_TEXT_FLOW );
*/

            EQS_SetIOCActive(hControl);
            
        }
        else
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_NUMBER_EXIST_MSG),\
                           MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
        }
     
        break;
    case SE_APP_END:
        break;
        
    case SE_IO_SCROLL_LIST_SELECT: 
        // 선택 메시지를 보여주는 코드를 추가해야 함
        hControl   = pEvent->wParam;
        index      = pEvent->dwParam;   
        ListHandle = EQC_GetScrollListHandle(hControl);
        index      = GetListValue( ListHandle , index );        
        EQS_END_EVENT( SE_APP_RESUME ,(WORD) TokenID, index);
        break;

    case SE_IO_MESGBOX_END:                      
        EQS_END();
        break;
        
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

LOCAL VOID SKY_API sSKA_SelectEmailID(SKY_EVENT_T* pEvent)
{
    INT8         ListHandle  ;
    HCONTROL     hControl    ;   
    INT16        index, nCount;
    pb_book_info_type        sBookInfo;
    pb_email_info_type       sEmailInfo;

    switch (pEvent->EventID)
    {
    case SE_APP_START:
        
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_NONE, 
                      "",
                      WA_SHOW_CML);
        sBookInfo.address = pEvent->wParam;

        if( ui_get_pb( PB_BOOK_INFO_I, &sBookInfo) > 0 )
        {
            ListHandle = CreateList(PERSONAL_EMAIL_LIMIT);
            sEmailInfo.address = sBookInfo.email_address;
			nCount = 0;
            while( ui_get_pb( PB_EMAIL_INFO_I, &sEmailInfo) > 0 && nCount < PERSONAL_EMAIL_LIMIT )
            {
				nCount++;
                AddList(ListHandle,sEmailInfo.address, EQS_NA);
                sEmailInfo.address = sEmailInfo.next_index;
            }
            if( GetListSize(ListHandle) == 0 )
            {
                DeleteList(ListHandle);
                // 메시지 출력
                EQS_END();
                break;
            }                     
            hControl   = sMakeScrollList(EQS_NA, EQS_NA, EQS_NA, PERSONAL_EMAIL_LIMIT+1, 0, \
                                         (UINT8)0,ListHandle, SDE_SELECT_EMAIL); 
            EQC_SetScrollListHandle(hControl, ListHandle);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_MAXIMIZE);

		    EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_SELECT_EMAIL_ID));
            EQC_ShowScrollTitle(hControl,TRUE);
            EQS_SetIOCActive(hControl);
            
        }
        else
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NOT_EXIST_EMAIL_ID_MSG),\
                           MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
        }
     
        break;
    case SE_APP_END:
        break;
        
    case SE_IO_SCROLL_LIST_SELECT: 
        // 선택 메시지를 보여주는 코드를 추가해야 함
        hControl   = pEvent->wParam;
        index      = pEvent->dwParam;   
        ListHandle = EQC_GetScrollListHandle(hControl);
        index      = GetListValue( ListHandle , index );        
        EQS_END_EVENT( SE_APP_RESUME ,EQS_NA, index);
        break;

    case SE_IO_MESGBOX_END:                      
        EQS_END();
        break;
        
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}



enum INIT_BOOK_DATA_APPLET_e
{
	IN_INITIALIZING_DATA_E = 1,
	COMPLETE_INIT_PBOOK_DATA_E
};
/* Question Password */
VOID SKY_API SKA_InitPBookData(SKY_EVENT_T* pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
       
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_PBOOK_DATA_INIT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_Image( 
		   IMAGE_X, 
		   IMAGE_Y, 
		   //(BITMAP*)&g_BmpSecret120x93x256c
           (BITMAP*)&g_BmpLock
		  ); 

        //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
        sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
        //EQC_SetNumberEditBackColor(sNumHandle,(int) NUMBER_EDIT_BACK_COLOR);

        EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
		EQS_SetIOCActive(sNumHandle);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
    
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            EQS_START_SIBLING(sSKA_InitPBookRoutine, FALSE);
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL VOID SKY_API sSKA_InitPBookRoutine(SKY_EVENT_T* pEvent)
{
	HCONTROL hControl;
	DWORD    User;
    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_PBOOK_DATA_INIT), 
            WA_SHOW_HEADER);

			EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_INIT_DATA_MSG),\
						   10, MBT_NOYES, MBT_CONFIRM);
		//EQS_START_CHILD(sSKA_AccessSecretGroup);
        break;
    case SE_APP_END:
        break;
		
	case SE_APP_RESUME:
#if 0
		if( EQS_GET_CHILD() == sSKA_AccessSecretGroup ) 
		{
			if( pEvent->dwParam )
			{
				EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_ASK_INIT_DATA_MSG),\
							   10, MBT_NOYES, MBT_CONFIRM);
				break;
			}
		}
		EQS_END();
#endif
		break;
	case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_NO:
        EQS_END();
        break;
	case SE_IO_MESGBOX_END:
		if( pEvent->dwParam == IN_INITIALIZING_DATA_E )
		{
			BookCleanUp();
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_COMPLETE_DATA_INIT_MSG),\
				                      MESSAGE_DEFAULT_TIME,\
									  MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, COMPLETE_INIT_PBOOK_DATA_E );	
		}
		else if( pEvent->dwParam == COMPLETE_INIT_PBOOK_DATA_E )
		{
			EQS_END();
		}
		else
		{
			EQS_END();
		}
		break;
    case SE_IO_MESGBOX_YES:
        hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_IN_INITIALIZING_MSG), 0,\
								  MBT_NONE, MBT_HELP);
		EQC_SET_USERDATA(hControl, IN_INITIALIZING_DATA_E );
		User = EQC_GET_USERDATA(hControl);

		break;
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }

}


VOID SKY_API SKA_BookTestInit(SKY_EVENT_T* pEvent)
{

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQC_StaticText(0,0, TKN_PBOOK_TEST_INIT);
        EQS_SEND_EVENT( SE_SOFTKEY_SAVE, 0,0);

        break;
    case SE_APP_END:
        break;
   
    case SE_IO_MESGBOX_END:                  
        EQS_END();
        break;

    case SE_SOFTKEY_SAVE:
        BookCleanUp();
        EQC_MessageBox((BYTE*)"전화부 초기화 완료", MESSAGE_DEFAULT_TIME,\
                       MBT_NONE, MBT_HELP);

        break;

    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }

}

void sSaveData(UINT8 Handle, pb_group_info_type* pGroupInfo, INT16 num, INT16 iteration, UINT8 nOption)
{
   
    int k;
    pb_book_info_type  sBookInfo;
    pb_anniv_info_type sAnnivInfo;

    init_pb_node(PB_BOOK_INFO_I, &sBookInfo);
	init_pb_node(PB_ANNIV_INFO_I, &sAnnivInfo);
       
    //for(i=0;i<27;i++)
    {
        sprintf((CHAR*)sBookInfo.name,"%d %c",num,'A'+iteration);
        k=AddMultiBook(pGroupInfo,&sBookInfo, Handle);
		//k = AddBook(pGroupInfo, &sBookInfo);
        if( nOption > 0 )
        {
            ReverseAddNumber(&sBookInfo,"0164952341",CELL);         
            ReverseAddNumber(&sBookInfo,"0316116624",HOME);
            ReverseAddNumber(&sBookInfo,"0112935230",CELL);
            ReverseAddNumber(&sBookInfo,"0428695566",WORK);
        }
        if( nOption > 1 )
        {
            AddEmail(&sBookInfo,"경기도 평택"); 
        }
        if( nOption > 2 )
        {
            sAnnivInfo.year   = 2001;
            sAnnivInfo.month  = (iteration % 12 + 1);
            sAnnivInfo.day    = (iteration % 31 + 1);
            sAnnivInfo.fLunar = FALSE;
            sAnnivInfo.type   = 1;
            AddAnnivInfo(&sBookInfo, &sAnnivInfo);
        }
        ui_put_pb(PB_BOOK_INFO_I, &sBookInfo);
    }
}

VOID SKY_API SKA_BookTestDataSave(SKY_EVENT_T* pEvent)
{
    static INT16  nData ;
    static INT16  Number;
    static INT16  Iteration;
    static UINT8  nOption, Handle;
    
    
    static pb_group_info_type GroupInfo;
    HCONTROL    hControl;
    int i,j;
    BYTE   szNumber[10];

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        GroupInfo.address = active_list.group_info;
        Number     = 0;
        Iteration  = 0;
        nData      = 4;
        nOption    = 0;

        EQC_StaticText(0,0, TKN_PBOOK_TEST_DATASAVE);
        hControl = EQC_NumberEdit(1, 20, 10, 1 , 34,"" );
        EQS_SetIOCActive(hControl);
		nData = 6;
        EQS_SetWindowCML(CL_MENU_NONE_SAVE);   
        
        break;
    case SE_APP_END:
		PhoneBookUpdate();
        break;

    case SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue(pEvent->wParam, szNumber);
        i = 0;
        nData = 0;
        while( szNumber[i] != 0 )
        {
            nData = nData*10 + (INT16)(szNumber[i] - (BYTE)'0');
            i++;
        }
        if( nData == 0 )
            nData = 4;

        break;
   
    case SE_IO_MESGBOX_END:                  
        EQS_END();
        break;
    case SE_KEY_0:
        //2001.01.08  //test_all
        EQC_MessageBox((BYTE*)"테스트 데이터 저장 완료", MESSAGE_DEFAULT_TIME,\
                       MBT_NONE, MBT_HELP);
        break;
    case SE_KEY_1:
        nOption = 1;   // add number info
        break;
    case SE_KEY_2:     // add memo info
        nOption = 2;
        break;
    case SE_KEY_3:     // add anniv info
        nOption = 3;
        break;
    case SE_SOFTKEY_MENU:
	case SE_SOFTKEY_SAVE:
        //test_all();             
/*
        if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
        {
            sSaveData(&GroupInfo, Number);
            GroupInfo.address = GroupInfo.next_index;
            hControl = EQS_GetNthIOC(0);
            sprintf(szNumber,"%d 저장",Number);
            EQC_SetStaticText(hControl,szNumber);
            EQS_SEND_EVENT(SE_APP_DRAW,0,0);

            EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
        }
*/
		nOption = 1;
		Number  = 0;
		for(j=0; j< 6; j++)
		{
			if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
			{
				//EQS_StartTimer(EQS_TIMER_2, 100, FALSE);
				Handle = GetBookList(GroupInfo.address);
				SortBookName(Handle);  				
				for(i=0; i<30; i++)
				{
					sSaveData(Handle,&GroupInfo, Number,Iteration++, nOption);
				}			 
				ui_put_pb(PB_GROUP_INFO_I, &GroupInfo);
				GroupInfo.address = GroupInfo.next_index;
				//EQS_SEND_EVENT(SE_TM_TIMER2,0,0);
			}
			Iteration = 0;
			Number++;
		}
        EQC_MessageBox((BYTE*)"테스트 데이터 저장 완료", MESSAGE_DEFAULT_TIME,\
                       MBT_NONE, MBT_HELP);
        break;
    case SE_KEY_STAR:
    case SE_TM_TIMER1:
        Number++;
        if( Number >= nData )
        {                       
            DeleteList(Handle);
            //PBookClose();
            //PBookOpen();
            EQS_END();
            break;
        }
/*
        if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
        {
            sSaveData(&GroupInfo, Number);
            GroupInfo.address = GroupInfo.next_index;
            sprintf(szNumber,"%d저장",Number);
            hControl = EQS_GetNthIOC(0);
            EQC_SetStaticText(hControl,szNumber);
            EQS_SEND_EVENT(SE_APP_DRAW,0,0);
            EQS_StartTimer(EQS_TIMER_1, 300, FALSE);            
        }
*/
        GroupInfo.address = GroupInfo.next_index;
        if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
        {
            //EQS_StartTimer(EQS_TIMER_2,500,FALSE);
            DeleteList(Handle);
            Handle = GetBookList(GroupInfo.address);
            SortBookName(Handle);            
            EQS_SEND_EVENT(SE_TM_TIMER1,0,0);
        }
        break;

    case SE_KEY_F2:
    case SE_TM_TIMER2:
        Iteration++;
        if( Iteration < 30 )
        {
            sSaveData(Handle,&GroupInfo, Number,Iteration, nOption);
            sprintf(szNumber,"%d-%d저장",Number,Iteration);
            hControl = EQS_GetNthIOC(0);
            EQC_SetStaticText(hControl,szNumber);
            //EQS_StartTimer(EQS_TIMER_2, 500, FALSE);
            EQS_SEND_EVENT(SE_APP_DRAW,0,0);
            EQS_SEND_EVENT(SE_TM_TIMER2,0,0);
        }
        else
        {   
            ui_put_pb(PB_GROUP_INFO_I, &GroupInfo);
            Iteration = 0;
            //EQS_StartTimer(EQS_TIMER_1, 500, FALSE);
            EQS_SEND_EVENT(SE_TM_TIMER1,0,0);
            
        }

       break;
/*
    case SE_KEY_F2:
    case SE_SOFTKEY_SAVE:
        
        //2001.01.08
        EQC_MessageBox((BYTE*)"테스트 데이터 저장 완료", MESSAGE_DEFAULT_TIME,\
                       MBT_NONE, MBT_HELP);
*/

        break;
   
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }

}


VOID SKY_API SKA_BookTestGroupSave(SKY_EVENT_T* pEvent)
{

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQC_StaticText(0,0, TKN_PBOOK_TEST_GROUPSAVE);
     
        EQS_SEND_EVENT( SE_SOFTKEY_SAVE, 0,0);
        break;
    case SE_APP_END:
        break;
   
    case SE_IO_MESGBOX_END:                  
        EQS_END();
        break;


    case SE_SOFTKEY_SAVE:    
        AddGroup("1-그룹", FALSE);
        AddGroup("2-그룹", FALSE);
        AddGroup("3-그룹", FALSE);
        AddGroup("4-그룹", FALSE);
        AddGroup("5-그룹", FALSE);
        AddGroup("6-그룹", FALSE);
        AddGroup("7-그룹", FALSE);
        AddGroup("8-그룹", FALSE);
        AddGroup("9-그룹", FALSE);
        AddGroup("학생"  , FALSE);
        PhoneBookUpdate();
        EQC_MessageBox((BYTE*)"테스트 그룹 저장 완료", MESSAGE_DEFAULT_TIME,\
            MBT_NONE, MBT_HELP);

        break;

    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }


}

VOID SKY_API SKA_BookTestMemberSave(SKY_EVENT_T* pEvent)
{

    HCONTROL             hControl;
    INT16                index, ListHandle, i,k;

    pb_group_info_type   sGroupInfo;
    pb_book_info_type    sBookInfo;
    pb_anniv_info_type   sAnnivInfo;

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_MENU_SELECT_SAVE,
                      SKY_GET_TOKEN(TKN_PBOOK_TEST_MEMBERSAVE), 
                      WA_SHOW_CML);
       
        break;
    case SE_APP_END:
        break;
   
    case SE_IO_MESGBOX_END:                  
        EQS_END();
        break;

    case SE_SOFTKEY_SAVE:
        
		init_pb_node( PB_GROUP_INFO_I,  &sGroupInfo);
		sGroupInfo.address = AddGroup((BYTE*)"테스트그룹", FALSE);
		ui_get_pb( PB_GROUP_INFO_I, &sGroupInfo);

		for( i=0; i<1200; i++)
		{
			init_pb_node( PB_BOOK_INFO_I,   &sBookInfo);
			sprintf((char*)sBookInfo.name,"%4d", i+1 );
			k=AddBook(&sGroupInfo,&sBookInfo);                
			ReverseAddNumber(&sBookInfo,"0164952341",CELL);
			//ReverseAddNumber(&sBookInfo,"0164952342",CELL);
			//ReverseAddNumber(&sBookInfo,"0428693566",WORK);
			//ReverseAddNumber(&sBookInfo,"0428695566",WORK);
			ui_put_pb(PB_BOOK_INFO_I, &sBookInfo);
		}


        PhoneBookUpdate();
                 
        EQC_MessageBox((BYTE*)"테스트 데이터 저장 완료",\
                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);

        break;
    case SE_SOFTKEY_MENU:
		init_pb_node( PB_GROUP_INFO_I,  &sGroupInfo);
		sGroupInfo.address = AddGroup((BYTE*)"테스트그룹", FALSE);
		ui_get_pb( PB_GROUP_INFO_I, &sGroupInfo);

		for( i=0; i<600; i++)
		{
			init_pb_node( PB_BOOK_INFO_I,   &sBookInfo);
			sprintf((char*)sBookInfo.name,"%4d", i+1 );
			k=AddBook(&sGroupInfo,&sBookInfo);                
			ReverseAddNumber(&sBookInfo,"0164952341",CELL);
			ReverseAddNumber(&sBookInfo,"0112935230",CELL);
			//ReverseAddNumber(&sBookInfo,"0428693566",WORK);
			//ReverseAddNumber(&sBookInfo,"0428695566",WORK);
			ui_put_pb(PB_BOOK_INFO_I, &sBookInfo);
		}


        PhoneBookUpdate();
                 
        EQC_MessageBox((BYTE*)"테스트 데이터 저장 완료",\
                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);

        break;
		break;
    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }
}

VOID SKY_API SKA_BookTestApplet(SKY_EVENT_T* pEvent)
{

    switch (pEvent->EventID)
    {
    case SE_APP_START:

        EQC_StaticText(0,0, TKN_PBOOK_TEST_GROUPSAVE);

        sAddInputControl( 1,1,ICT_GROUP_E ,FALSE);

        EQS_SetWindowCML(CL_MENU_NONE_SAVE);
       
        break;
    case SE_APP_END:
        break;
   
    case SE_IO_MESGBOX_END:                  
        EQS_END();
        break;

    case SE_SOFTKEY_SAVE:

        EQC_MessageBox((BYTE*)"테스트 데이터 저장 완료",MESSAGE_DEFAULT_TIME,\
                        MBT_NONE, MBT_HELP);

        break;

    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    }

}


LOCAL void sPBookDisplayScroll(UINT8 hControl, WORD wPos, BOOL fMultiLine ,SCROLL_DATA_E ScrollType)
{
    INT16  ListHandle, nIndex, nSize;
    
    ListHandle = EQC_GetScrollListHandle(hControl);
    
    if( fMultiLine == TRUE )// SE_IO_SCROLL_LIST_TOP, SE_IO_SCROLL_LIST_BOTTOM
    {
        nSize = EQC_GetScrollViewLine(hControl);
        for(nIndex = 0; nIndex < nSize; nIndex++)
        {
            sMakeFormattedString(EQC_GetScrollData(hControl, nIndex),\
                                 ListHandle, wPos+nIndex, ScrollType);
        }
    }
    else// SE_IO_SCROLL_LIST_UP,    SE_IO_SCROLL_LIST_DOWN
    {
        nIndex = EQC_GetScrollPhysicalIndex(hControl);
        sMakeFormattedString(EQC_GetScrollData(hControl, nIndex),\
                             ListHandle, wPos, ScrollType);
    } 

	if( EQS_IsActive(hControl) == FALSE && EQC_IsShowScrollTitle(hControl) && EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO)
	{
		EQC_SetScrollTitle(hControl, EQC_GetScrollData(hControl,EQC_GetScrollPhysicalIndex(hControl)));
	}

}

LOCAL void SKY_API sPBookSendCall(BYTE* szTel)  
{
    INT16  nLen;

//  SKY_SET_ORIGINATE(TRUE);  hjcho 01.06.20
    nLen = STRLEN(szTel);
    SET_NUMEDIT_BUFFER(szTel, nLen);
    SET_NUMEDIT_POS(nLen);
    EQS_START_SIBLING(SKA_StartOrigination, FALSE);
//    ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,\
//                         GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
}

LOCAL void SKY_API sPBookMakeCall(HCONTROL hControl)   // 대표번호로 전화 연결
{
    UINT8 ListHandle;
    INT16 index;
    pb_book_info_type   BookInfo;
    pb_number_info_type NumberInfo;
	BYTE    szBuf[40];
   
    ListHandle = EQC_GetScrollListHandle(hControl);
    index = EQC_GetScrollLogicalIndex(hControl);
    BookInfo.address = GetListValue(ListHandle, index);
    ui_get_pb(PB_BOOK_INFO_I, &BookInfo);
    NumberInfo.address = BookInfo.number_address;
    if( ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) > 0 )
    {
//        SKY_SET_ORIGINATE(TRUE); hjcho 01.06.20
		if( STRLEN(NumberInfo.digits) > 0 )
		{
			sprintf((CHAR*) szBuf,(CHAR*) SKY_GET_TOKEN(TKN_PBOOK_FMT_NAME_TYPE),	
                                            g_szPersonName[NumberInfo.book_address-1],\
											phone_type[NumberInfo.number_type-1]);
			index = STRLEN(szBuf) ;
			SET_ALPHAEDIT_BUFFER(szBuf, index);
			SET_ALPHAEDIT_POS(index);

			index = STRLEN(NumberInfo.digits);
					
			SET_NUMEDIT_BUFFER(NumberInfo.digits, index);
			SET_NUMEDIT_POS(index);   
			EQS_START_SIBLING(SKA_StartOrigination, FALSE);
		}
		



//        ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,\
//                             GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
        //EQS_START_SIBLING(SKA_MainCall, FALSE);
    }
}

LOCAL BOOK_CONTROL_E sGetCurrentControlType(UINT8 nGroup, UINT8 nNumber, \
                                            UINT8 nEmail, UINT8 nMemo, \
                                            UINT8 nAnniv, UINT8 nPicture)
{
     HCONTROL hControl;
     INT16    nPosition;
     INT16    nIndex;

     hControl  = EQS_GetFocusedIOC();
     nPosition = EQS_GetIOCOrderly(hControl);


     nIndex    =  INITIAL_INPUT_CONTROL + nGroup*2;
	 if( nPosition < INITIAL_INPUT_CONTROL )
		 return ICT_NAME_E;

     if( nPosition < nIndex )
         return ICT_GROUP_E;
     
     nIndex += nNumber*2;
     if( nPosition < nIndex )
         return ICT_NUMBER_E;

     nIndex += nEmail*2;
     if( nPosition < nIndex )
         return ICT_EMAIL_E;
     nIndex += nMemo*2;     
     if( nPosition < nIndex)
         return ICT_MEMO_E;

     nIndex += nAnniv*4;    
     if( nPosition < nIndex )
         return ICT_ANNIV_E;

     nIndex += nPicture*2;

     if( nPosition < nIndex )
         return ICT_PICTURE_E;
     else
        return ICT_MENU_E;
  
}


// 저장 & 편집 
LOCAL INT16 sSavePhoneBook(UINT8 hList,UINT8 nGroup, UINT8 nNumber,\
                          UINT8 nEmail, UINT8 nMemo, UINT8 nAnniv, DWORD dwPic)
{

    HCONTROL  hControl;
    UINT8     ListHandle, nCount;
    INT16     index, address, i, number_address, anniv_address;
    pb_book_info_type   sBookInfo;
    pb_group_info_type  sGroupInfo;
    pb_number_info_type sNumberInfo;  
    pb_anniv_info_type  sAnnivInfo;
	pb_email_info_type   sEMailInfo;
    BYTE  szStr[80];

   
	init_pb_node( PB_BOOK_INFO_I,  &sBookInfo);

    hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL -1);     //  이름 정보
	address  = EQC_GET_USERDATA(hControl);
	if( address == 0 )
	{			
		sBookInfo.address = 0;
	}
	else
	{
		sBookInfo.address = EQC_GET_USERDATA(hControl);
		ui_get_pb( PB_BOOK_INFO_I, &sBookInfo );
	}

    EQC_GetTextEditData( hControl, sBookInfo.name );

    if( STRLEN( sBookInfo.name )  == 0 )                    
    {       
        STRCPY( sBookInfo.name, SKY_GET_TOKEN(TKN_PBOOK_NO_NAME));  // "이름없음" 항목으로 저장
    }	
    
    
	hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+1);              // 그룹 정보 
	address  = EQC_GET_USERDATA(hControl);
	init_pb_node( PB_GROUP_INFO_I, &sGroupInfo);		
	sGroupInfo.address = SearchGroup(EQC_GetScrollTitle(hControl));
	if( sGroupInfo.address != 0 )    // 기존 그룹에 추가
	{
		ListHandle = EQC_GetScrollListHandle( hControl );      
		if( ui_get_pb( PB_GROUP_INFO_I, &sGroupInfo ) == 0 )
		{
			hControl=EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_GROUP_INFO_ERR),\
									MESSAGE_DEFAULT_TIME,\
									MBT_NONE, MBT_HELP);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl)|CS_MESSAGE_BOX_TITLE);
			return 0;
		}
	}
	else    // 새그룹 ---
	{
		ListHandle = EQC_GetScrollListHandle( hControl );
		index = GetListSize( ListHandle );
        
		sGroupInfo.address = InsertGroup( EQC_GetScrollTitle(hControl), g_nGroup, FALSE);
		//sGroupInfo.address = AddGroup(EQC_GetScrollTitle(hControl), FALSE);
			    
		if( ui_get_pb( PB_GROUP_INFO_I, &sGroupInfo) == FALSE)
		{
			hControl=EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_CREATE_GROUP_ERR),\
									MESSAGE_DEFAULT_TIME, \
									MBT_NONE, MBT_HELP);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl)|CS_MESSAGE_BOX_TITLE );
			return 0;
		}
	}

	if( sBookInfo.address == 0 )
	{	// 새로 생성 
		AddBook(&sGroupInfo,&sBookInfo);
	}
	else
	{
		if( address != sGroupInfo.address ) // 그룹이 변경되었을 경우 
		{
			pb_group_info_type GroupInfo;
			GroupInfo.address = address;
			ui_get_pb(PB_GROUP_INFO_I, &GroupInfo);
			GroupInfo.wMember--;
			ui_put_pb(PB_GROUP_INFO_I, &GroupInfo);

			sGroupInfo.wMember++;
			ui_put_pb( PB_GROUP_INFO_I, &sGroupInfo);
			g_PersonGroup[sBookInfo.address-1] = sGroupInfo.address;
			sBookInfo.group_address = sGroupInfo.address;
		}
	}
	STRCPY( g_szPersonName[sBookInfo.address-1], sBookInfo.name );
	sBookInfo.dwImageIndex = dwPic;
	if( dwPic )
	{
		sBookInfo.other_link[0] = EQC_GET_USERDATA( EQS_GetNthIOC(EQS_GetIOCNum()-3) );
	}
    
    if( nMemo > 0 )     //memo
    {
        hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+ (nGroup+nNumber+nEmail+nMemo-1)*2 + 1 );
        EQC_GetTextEditData( hControl, sBookInfo.szText );    
    }
	else
	{
		sBookInfo.szText[0] = 0;
	}
		
	sNumberInfo.address = sBookInfo.number_address;
	nCount = 0;
	while( ui_get_pb(PB_NUMBER_INFO_I, &sNumberInfo) > 0 && nCount < PERSONAL_NUMBER_LIMIT )
	{		
		nCount++;
		number_address = sNumberInfo.next_index;
		for(i=0; i<nNumber; i++)
		{			
			hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup + i)*2 + 1 );
			if( EQC_GET_USERDATA(hControl) == sNumberInfo.address )
			{
				break;
			}
		}
		if( i == nNumber  )
		{				
			DeleteNumberInfo(&sNumberInfo);
		}		
		sNumberInfo.address = number_address;
	}

	sBookInfo.number_address = 0;
	for(i=0; i< PERSONAL_NUMBER_LIMIT; i++)
	{
		g_PersonNumberType[sBookInfo.address-1][i] = BOOK_NOT_USE;
	}
        
	index = 0;                    // for type 
    for( i=nNumber-1; i>=0; i--)  // 번호 정보 
    {
        hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup + i)*2 + 1 );
        EQC_GetNumberEditValue( hControl, szStr );
		
        if( STRLEN(szStr) > 0 )
        {
			init_pb_node( PB_NUMBER_INFO_I, &sNumberInfo);
			sNumberInfo.address = EQC_GET_USERDATA(hControl);
			if( sNumberInfo.address == 0 )
			{
				sNumberInfo.address = alloc_pb_node(PB_NUMBER_INFO_I);
			}				

			if( IsValid( PB_NUMBER_INFO_I, sNumberInfo.address )  )
			{
				hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup +i)*2 );
				
				STRCPY( sNumberInfo.digits, szStr);
				sNumberInfo.number_type  = EQC_GetScrollPhysicalIndex(hControl)+1;
				sNumberInfo.next_index   = sBookInfo.number_address;
				sNumberInfo.book_address = sBookInfo.address;
				ui_put_pb(PB_NUMBER_INFO_I, &sNumberInfo);
				sBookInfo.number_address = sNumberInfo.address;
				g_PersonNumberType[sBookInfo.address-1][i] = sNumberInfo.number_type;
			}
        }
		else
		{
			sNumberInfo.address = EQC_GET_USERDATA(hControl);
			free_pb_node( PB_NUMBER_INFO_I, sNumberInfo.address );
		}	
    }       
	// email
	sEMailInfo.address = sBookInfo.email_address;
	nCount = 0;
	while( ui_get_pb( PB_EMAIL_INFO_I, &sEMailInfo ) > 0  && nCount < PERSONAL_EMAIL_LIMIT )
	{	nCount++;
		free_pb_node( PB_EMAIL_INFO_I, sEMailInfo.address);
		sEMailInfo.address = sEMailInfo.next_index;
	}
	sBookInfo.email_address = 0;
				
    for( i=nEmail-1; i>=0; i--)
    {	
		hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup+nNumber+i)*2 + 1);
        EQC_GetTextEditData( hControl, szStr);
        if( STRLEN( szStr ) > 0 )
        {
            AddEmail( &sBookInfo, szStr );
        }       
    }
	sAnnivInfo.address = sBookInfo.anniv_address;
	nCount = 0;
	while( ui_get_pb( PB_ANNIV_INFO_I, &sAnnivInfo) > 0 && nCount < PERSONAL_ANNIV_LIMIT)
	{
		nCount++;
		anniv_address = sAnnivInfo.next_index;
        DeleteAnnivInfo(&sAnnivInfo);
		sAnnivInfo.address = anniv_address;
	}
	sBookInfo.anniv_address = 0;

    for( i=nAnniv-1; i>=0; i--)
    {
		init_pb_node(PB_ANNIV_INFO_I, &sAnnivInfo);
        hControl = EQS_GetNthIOC( INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail+nMemo)*2 + i*4 +1);

		sAnnivInfo.type = EQC_GetScrollLogicalIndex(hControl) + 1;
#if 0
		if( EQC_GET_USERDATA(hControl) == ETC_ANNIV )
		{
			sAnnivInfo.type = ETC_ANNIV;
		}
		else
		{
			sAnnivInfo.type = EQC_GetScrollLogicalIndex(hControl) + 1;
		}
#endif
        
        STRCPY( sAnnivInfo.content, EQC_GetScrollTitle(hControl));     
        hControl = EQS_GetNthIOC(INITIAL_INPUT_CONTROL+(nGroup+nNumber+nEmail+nMemo)*2 + i*4 +3);
        sAnnivInfo.year   = EQC_GetTimeYear(hControl);
        sAnnivInfo.month  = EQC_GetTimeMonth(hControl);
        sAnnivInfo.day    = EQC_GetTimeDay(hControl);
        sAnnivInfo.fLunar = EQC_GetTimeLunar(hControl);
        AddAnnivInfo( &sBookInfo, &sAnnivInfo );
    }

    ui_put_pb( PB_BOOK_INFO_I,  &sBookInfo);
    //ui_put_pb( PB_GROUP_INFO_I, &sGroupInfo);
    PhoneBookUpdate();

    return sBookInfo.address;
}

LOCAL CHAR* sAllocBuffer(INT16 size)
{
    CHAR* pBuffer;

    pBuffer = (CHAR*) SKY_Malloc( sizeof(CHAR)*(PB_MAX_STRING_LENGTH+1)* size);

    return pBuffer;
}
/*
LOCAL void sFreeBuffer(CHAR* pBuffer)
{
    SKY_Free( pBuffer);
}
*/
LOCAL BYTE* sIndexBuffer(BYTE* pBuffer, INT16 index)
{
    return (pBuffer + index*PB_MAX_STRING_LENGTH);
}

LOCAL void sDeleteEditControl(UINT8 nDeletePosition, UINT8 nDeleteNum)  // 편집시 항목삭제
{
    INT8 nIndex,nControl, nHeight;
    HCONTROL hControl;
    
    MARGIN_T Margin;
    COORD    xSpace, ySpace;

    EQS_StartFormatter();   
    EQS_GetControlMargin(&Margin);
    EQS_GetControlSpace(&xSpace, &ySpace);
    EQS_EndFormatter();
    nControl = EQS_GetIOCNum();

	nHeight  = EQC_GET_DY(EQS_GetNthIOC(nDeletePosition+1))+ySpace;
	if( nDeleteNum == 4 )
		nHeight = nHeight*2 + ySpace;
        
    for( nIndex = nControl-1; nIndex >=nDeletePosition+nDeleteNum; nIndex--)
    {
        hControl = EQS_GetNthIOC(nIndex);            
        EQC_SET_Y(hControl, EQC_GET_Y(hControl)-nHeight -ySpace +2);
    }    
    for( nIndex = 0; nIndex< nDeleteNum; nIndex++)
    {
        hControl = EQS_GetNthIOC(nDeletePosition);
        EQS_DeleteControl(hControl);
    }
	EQS_FitCanvasSize();
}

LOCAL HCONTROL sAddInputControl(UINT8 nInsertPosition, UINT8 nViewLine, BOOK_CONTROL_E Type ,BOOL fFocus)
{
    INT8 nIndex, nControl, nAdd, nHeight;
    HCONTROL hControl, hTemp;
    COORD yPos, xPos;
    BYTE* paszItemType[10];
    BYTE* pList[6];
    BYTE* pszString;
    BYTE  szBuf[10];
    MARGIN_T Margin;
    COORD    xSpace, ySpace;

	COORD StartX1, StartX2;
	UINT8 Width1,  Width2, ControlViewChar;

	ControlViewChar = INPUT_CONTROL_WIDTH;
#ifdef CONFIG_EQS_COLOR_64K
	StartX1 = 1;
	Width1  = 21;
	StartX2 = 34;
	if( EQS_GET_APPLET() == SKA_ViewBookContent )
	{
		ControlViewChar++;
		StartX2 = 28;
	}
#ifdef FEATURE_CHARACTER_BELL
    if( EQS_GET_APPLET() == SKA_CBViewBookContent )
	{
		ControlViewChar++;
		StartX2 = 28;
	}
#endif // FEATURE_CHARACTER_BELL
	Width2  = ControlViewChar* EQS_CharWidth((BYTE)' ');

#elif
	StartX1 = 1;
	Width1  = 21;
	StartX2 = 28;
	Width2  = 84;
#endif
	

    EQS_StartFormatter();   
    EQS_GetControlMargin(&Margin);
    EQS_GetControlSpace(&xSpace, &ySpace);
    EQS_EndFormatter();
    nControl = EQS_GetIOCNum();
    nAdd     = 0;
    ySpace   +=2;

	if( EQS_GET_APPLET() == SKA_ViewBookContent )
	{
		Margin.xLeft = 0;
	}
#ifdef FEATURE_CHARACTER_BELL
    if( EQS_GET_APPLET() == SKA_CBViewBookContent )
	{
		Margin.xLeft = 0;
	}
#endif // FEATURE_CHARACTER_BELL

    for( nIndex = nInsertPosition; nIndex < nControl; nIndex++ )
    {
        hControl = EQS_GetNthIOC( nIndex );
        EQC_SET_Y( hControl , EQC_GET_Y(hControl) + 60);
    }    
    if( nInsertPosition > 0 )
    {
        hControl = EQS_GetNthIOC( nInsertPosition -1);
        yPos = EQC_GET_Y(hControl) + EQC_GET_DY(hControl) + ySpace;
    }
    else
    {
        yPos = Margin.yTop; // + ySpace;
    }

    switch( Type )
    {
    case ICT_NAME_E  :
/*
        hControl = EQC_StaticText(Margin.xLeft,yPos,TKN_PBOOK_ITEM_NAME);
		sprintf((char*)szBuf," %s ",(char*)SF_NAME_TEXT);
		EQC_SetStaticText(hControl, szBuf);	
        xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace ;

        hControl = EQC_TextEdit(xPos, yPos, INPUT_CONTROL_WIDTH, nViewLine, \
                                TEM_KOREAN, MAX_BOOK_NAME,(BYTE*)"", 0);
*/

        hControl = EQC_StaticText(StartX1,yPos,TKN_PBOOK_ITEM_NAME);
		sprintf((char*)szBuf," %s ",(char*)SF_NAME_TEXT);
		EQC_SetStaticText(hControl, szBuf);	
		EQC_SET_USERDATA(hControl, ICT_NAME_E);
        EQC_SET_DX(hControl, Width1);

        hControl = EQC_TextEdit(StartX2, yPos, ControlViewChar, nViewLine, \
                                TEM_KOREAN, MAX_BOOK_NAME,(BYTE*)"", 0);
		EQC_SET_DX(hControl, Width2);

        nAdd    = 2;
        nHeight = EQC_GET_DY(hControl) + ySpace;
        break;
    case ICT_GROUP_E :  
/*
        hControl = EQC_StaticText(Margin.xLeft, yPos, TKN_PBOOK_ITEM_GROUP);
		sprintf((char*)szBuf," %s ",(char*)SF_GROUP_TEXT);
		EQC_SetStaticText(hControl, szBuf);	
        xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;
        hControl = sMakeScrollList(xPos, yPos, INPUT_CONTROL_WIDTH,\
                                     4, 0,(UINT8)0,(WORD)0,SDE_GROUP_SELECT);
        EQC_SET_STYLE( hControl, CS_SCROLL_LIST_COMBO      | \
                                 CS_SCROLL_LIST_BORDER     | \
                                 CS_SCROLL_LIST_SCROLL_BAR ); // 콤보박스 
        EQC_SET_DY(hControl,EQS_CharHeight((BYTE)' '));
        EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ADD_GROUP));
        EQC_SetLogicalList(hControl,TRUE);
        EQC_ShowScrollTitle(hControl,TRUE);   
*/

        hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_GROUP);
		sprintf((char*)szBuf," %s ",(char*)SF_GROUP_TEXT);
		EQC_SetStaticText(hControl, szBuf);	
		EQC_SET_USERDATA(hControl, ICT_GROUP_E);
		EQC_SET_DX(hControl, Width1);
        
        hControl = sMakeScrollList(StartX2, yPos, ControlViewChar,\
                                     5, 0,(UINT8)0,(WORD)0,SDE_GROUP_SELECT);
#if BOOK_FLOW_MODE
		EQC_SetScrollFlowMode(hControl, TRUE);
#endif
        EQC_SET_STYLE( hControl, CS_SCROLL_LIST_COMBO      | \
                                 CS_SCROLL_LIST_BORDER     |                                 
								 CS_SCROLL_LIST_ENUMBERATED   ); // 콤보박스 
		                         //CS_SCROLL_LIST_SCROLL_BAR
        EQC_SET_DY(hControl,EQS_CharHeight((BYTE)' '));
		EQC_SET_DX(hControl, Width2);
        EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ADD_GROUP));
        EQC_SetLogicalList(hControl,TRUE);
        EQC_ShowScrollTitle(hControl,TRUE);  
		EQC_SetShowScrollBar(hControl, FALSE);

        nAdd    = 2;
        nHeight = EQC_GET_DY(hControl) + ySpace;
        
        break;
    case ICT_NUMBER_E:
        
/*
		if( EQS_GET_APPLET() == SKA_ViewBookContent )
		{
			hControl = EQC_StaticText(Margin.xLeft, yPos, TKN_PBOOK_ITEM_EMAIL);
		}
		else
		{
			pList[0] = phone_type[0];
			pList[1] = phone_type[1];
			pList[2] = phone_type[2];
			pList[3] = phone_type[3];
			hControl =  EQC_ScrollCombo(Margin.xLeft,yPos,4, 3, pList, 4, 4,0);
			EQC_SET_STYLE( hControl, EQC_GET_STYLE(hControl)    | \
									 CS_SCROLL_LIST_BORDER      | \
									 CS_SCROLL_LIST_SCROLL_BAR  );			
		}
		xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;		        
        hControl = EQC_NumberEdit(xPos, yPos, 
                                  INPUT_CONTROL_WIDTH, nViewLine, PB_MAX_DIGIT_LENGTH, "" );
*/

		if( EQS_GET_APPLET() == SKA_ViewBookContent )
		{
			hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_EMAIL);
		}
#ifdef FEATURE_CHARACTER_BELL
        else if( EQS_GET_APPLET() == SKA_CBViewBookContent )
		{
			hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_EMAIL);
		}
#endif // FEATURE_CHARACTER_BELL
		else
		{
			pList[0] = phone_type[0];
			pList[1] = phone_type[1];
			pList[2] = phone_type[2];
			pList[3] = phone_type[3];
			hControl =  EQC_ScrollCombo(StartX1,yPos,4, 3, pList, 4, 4,0);
			EQC_SET_USERDATA(hControl, ICT_NUMBER_E);
			EQC_SET_STYLE( hControl, //EQC_GET_STYLE(hControl)    | \
									 //CS_SCROLL_LIST_BORDER    |
									 //CS_SCROLL_LIST_CENTER_ALIGN|
                                     CS_SCROLL_LIST_BORDER  |\
									 CS_SCROLL_LIST_ENUMBERATED|\
									 CS_SCROLL_LIST_COMBO);
									 //CS_SCROLL_LIST_SCROLL_BAR);
			//EQC_SET_ENABLE(hControl, FALSE);
		}
		//EQC_SET_DX(hControl, Width1);
			        
        hControl = EQC_NumberEdit(StartX2, yPos, 
                                  ControlViewChar, nViewLine, PB_MAX_DIGIT_LENGTH, "" );
		EQC_SET_DX(hControl, Width2);


        nAdd    = 2;
        nHeight = EQC_GET_DY(hControl) + ySpace;
              
        break;
    case ICT_EMAIL_E:   
/*
        hControl = EQC_StaticText(Margin.xLeft, yPos, TKN_PBOOK_ITEM_EMAIL);
		sprintf((char*)szBuf," %s ",(char*)SF_EMAIL);

		EQC_SetStaticText(hControl, szBuf);
		xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;
        hControl = EQC_TextEdit(xPos, yPos, INPUT_CONTROL_WIDTH, \
                                nViewLine, TEM_ENGLISH_LOWER,PB_MAX_STRING_LENGTH,(BYTE*)"", 0);  
*/
        hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_EMAIL);
		sprintf((char*)szBuf," %s ",(char*)SF_EMAIL);
		EQC_SetStaticText(hControl, szBuf);
		EQC_SET_USERDATA(hControl, ICT_EMAIL_E);
		EQC_SET_DX(hControl, Width1);
		
        hControl = EQC_TextEdit(StartX2, yPos, ControlViewChar, \
                                nViewLine, TEM_ENGLISH_LOWER,PB_MAX_STRING_LENGTH,(BYTE*)"", 0);  
		EQC_SET_DX(hControl, Width2);

        nAdd    = 2;
        nHeight = EQC_GET_DY(hControl) + ySpace;		

        break;
    case ICT_MEMO_E:  
/*		
        hControl = EQC_StaticText(Margin.xLeft, yPos, TKN_PBOOK_ITEM_MEMO);  
		//sprintf((char*)szBuf," %s ",(char*)SF_MEMO);
		sprintf((char*)szBuf," %s ",(char*)SF_MESSAGE_CONTENT1_5);		

		EQC_SetStaticText(hControl, szBuf);
		xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) +xSpace;
        hControl = EQC_TextEdit(xPos, yPos, INPUT_CONTROL_WIDTH, \
                                nViewLine, TEM_KOREAN, PB_MAX_MEMO_LENGTH, (BYTE*)"", 0);
*/
        hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_MEMO);  
		//sprintf((char*)szBuf," %s ",(char*)SF_MEMO);
		sprintf((char*)szBuf," %s ",(char*)SF_MESSAGE_CONTENT1_5);
		EQC_SetStaticText(hControl, szBuf);
		EQC_SET_USERDATA(hControl, ICT_MEMO_E);
		EQC_SET_DX(hControl, Width1);
		
        hControl = EQC_TextEdit(StartX2, yPos, ControlViewChar, \
                                nViewLine, TEM_KOREAN, PB_MAX_MEMO_LENGTH, (BYTE*)"", 0);
		EQC_SET_DX(hControl, Width2);

        nAdd    = 2;
        nHeight = EQC_GET_DY(hControl) + ySpace;

        break;
    case ICT_ANNIV_E:
/*        
        pszString = sAllocBuffer(5);  
       
        for(nIndex=0; nIndex<5; nIndex++)
        {
            pList[nIndex] = sIndexBuffer( pszString, nIndex );
        }
       
        STRCPY( pList[0] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU1));       
        STRCPY( pList[1] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU2));
        STRCPY( pList[2] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU3));
       // STRCPY( pList[3] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU));

        hControl = EQC_StaticText(Margin.xLeft, yPos, TKN_PBOOK_ITEM_ANNIV_TEXT);
		sprintf((char*)szBuf," %s ",(char*)SF_ANNIVERSARY_TEXT);
		EQC_SetStaticText(hControl, szBuf);	
        xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;


        hControl = EQC_ScrollCombo(xPos, yPos,INPUT_CONTROL_WIDTH, 4, pList, 3, 3,0);  //00.12.27 
        EQC_FreeScrollMemory(hControl,TRUE);
        EQC_SET_STYLE( hControl, EQC_GET_STYLE(hControl)  |\
                                 CS_SCROLL_LIST_BORDER    |\
                                 CS_SCROLL_LIST_SCROLL_BAR);
		EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU1));
		EQC_ShowScrollTitle( hControl, TRUE );

        yPos += EQC_GET_DY(hControl) + ySpace;
        hControl = EQC_StaticText(Margin.xLeft, yPos, TKN_PBOOK_ITEM_DAY);

        xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;
                          
        hControl = EQC_Time(xPos, yPos,  2000, INFORM_TIME_CONTROL);
        EQC_SET_STYLE(hControl,CS_TIME_BORDER);
        EQC_SetTime(hControl,2000,12,3,26,4,30);
*/

        pszString = sAllocBuffer(5);  
       
        for(nIndex=0; nIndex<5; nIndex++)
        {
            pList[nIndex] = sIndexBuffer( pszString, nIndex );
        }
       
        STRCPY( pList[0] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU1));       
        STRCPY( pList[1] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU2));
        STRCPY( pList[2] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU3));
        //STRCPY( pList[3] , SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU));
		STRCPY( pList[3] , SKY_GET_TOKEN(TKN_PBOOK_ITEM_ANNIV));
		

        hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_ANNIV_TEXT);
		sprintf((char*)szBuf," %s ",(char*)SF_ANNIVERSARY_TEXT);
		EQC_SetStaticText(hControl, szBuf);	
		EQC_SET_USERDATA(hControl, ICT_ANNIV_E);
        EQC_SET_DX(hControl, Width1);


        hControl = EQC_ScrollCombo(StartX2, yPos, ControlViewChar, 5, pList, 4, 4,0);  //00.12.27 
        EQC_FreeScrollMemory(hControl,TRUE);
        EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER    |\
			                     CS_SCROLL_LIST_COMBO     |\
								 CS_SCROLL_LIST_ENUMBERATED);
                                 //CS_SCROLL_LIST_SCROLL_BAR|\
								 
		EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU1));
		EQC_ShowScrollTitle( hControl, TRUE );
		EQC_SetShowScrollBar(hControl, FALSE);
		EQC_SET_DX(hControl, Width2);
#if BOOK_FLOW_MODE  		
		EQC_SetScrollFlowMode(hControl, TRUE);
#endif

        yPos += EQC_GET_DY(hControl) + ySpace;
        hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_DAY);  // 기념일 종류
		STRCPY(szBuf,(BYTE*)"    ");
		EQC_SetStaticText(hControl, szBuf);	

		EQC_SET_USERDATA(hControl, ICT_ANNIV_E);
        EQC_SET_DX(hControl, Width1);
                          
        hControl = EQC_Time(StartX2, yPos,  2000, INFORM_TIME_CONTROL);
        EQC_SET_STYLE(hControl,CS_TIME_BORDER);
        EQC_SetTime(hControl,2000,12,3,26,4,30);
		EQC_SET_DX(hControl, Width2);

        nAdd    = 4;
        nHeight = (EQC_GET_DY(hControl) + ySpace)*2;
      
        break;
    case ICT_PICTURE_E:
/*
        hControl = EQC_StaticText(Margin.xLeft, yPos, TKN_PBOOK_ITEM_MEMO);
		sprintf((char*)szBuf," %s ",(char*)SF_CAMERA);
		EQC_SetStaticText(hControl, szBuf);
		xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;
		hControl = EQC_PushButton(xPos,yPos,TKN_PBOOK_PICTURE_VIEW);
*/

        hControl = EQC_StaticText(StartX1, yPos, TKN_PBOOK_ITEM_MEMO);
		sprintf((char*)szBuf," %s ",(char*)SF_CAMERA);
		EQC_SetStaticText(hControl, szBuf);
		EQC_SET_USERDATA(hControl, ICT_PICTURE_E);
		EQC_SET_DX(hControl, Width1);
		
		hControl = EQC_PushButton(StartX2,yPos,TKN_PBOOK_PICTURE_VIEW);
		EQC_SET_DX(hControl, Width2);
        nAdd    = 2;
        nHeight = EQC_GET_DY(hControl) + ySpace;
		
        break;
    case ICT_MENU_E:
/*
//        paszItemType[ICT_GROUP_E] = SKY_GET_TOKEN(TKN_PBOOK_ITEM_GROUP); //그룹추가 기능을 없앰
        paszItemType[ICT_NUMBER_E] = SKY_GET_TOKEN(TKN_PBOOK_ADD_NUMBER_ITEM);
        paszItemType[ICT_EMAIL_E] = SKY_GET_TOKEN(TKN_PBOOK_ADD_EMAIL_ITEM);
        paszItemType[ICT_MEMO_E] = SKY_GET_TOKEN(TKN_PBOOK_ADD_MEMO_ITEM);
        paszItemType[ICT_ANNIV_E] = SKY_GET_TOKEN(TKN_PBOOK_ADD_ANNIV_ITEM);
        paszItemType[ICT_PICTURE_E] = SKY_GET_TOKEN(TKN_PBOOK_ADD_PICTURE_ITEM);
      
        hControl = EQS_GetNthIOC(EQS_GetIOCNum()-2);
        if( EQC_GET_X(hControl) == Margin.xLeft )
            xPos = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + xSpace;
        else
            xPos = EQC_GET_X(hControl);

        hControl = EQC_ScrollCombo(xPos, yPos, INPUT_CONTROL_WIDTH,\
                                   4, paszItemType, 5, 5,0);
        EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER    |\
                                 CS_SCROLL_LIST_SCROLL_BAR|\
								 CS_SCROLL_LIST_COMBO_LIST);
        EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_ADD_DATA) );
        EQC_ShowScrollTitle(hControl,TRUE);
*/
        paszItemType[ICT_NUMBER_E] = SKY_GET_TOKEN(TKN_PBOOK_ITEM_NUMBER);//TKN_PBOOK_ADD_NUMBER_ITEM);
        paszItemType[ICT_EMAIL_E] = SKY_GET_TOKEN(TKN_PBOOK_ITEM_EMAIL);//TKN_PBOOK_ADD_EMAIL_ITEM);
        paszItemType[ICT_MEMO_E] = SKY_GET_TOKEN(TKN_PBOOK_ITEM_MEMO);//TKN_PBOOK_ADD_MEMO_ITEM);
        paszItemType[ICT_ANNIV_E] = SKY_GET_TOKEN(TKN_PBOOK_ITEM_ANNIV);//TKN_PBOOK_ADD_ANNIV_ITEM);
        paszItemType[ICT_PICTURE_E] = SKY_GET_TOKEN(TKN_PBOOK_ITEM_PICTURE);//TKN_PBOOK_ADD_PICTURE_ITEM);
      
        hControl = EQC_ScrollCombo(StartX2, yPos, ControlViewChar,\
                                   5, paszItemType, 5, 5,0);
        EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER    |\
								 CS_SCROLL_LIST_COMBO_LIST|\
								 CS_SCROLL_LIST_ENUMBERATED);
		                         //CS_SCROLL_LIST_SCROLL_BAR
        EQC_SetScrollTitle(hControl, SKY_GET_TOKEN(TKN_PBOOK_ADD_DATA) );
        EQC_ShowScrollTitle(hControl,TRUE);
		EQC_SetShowScrollBar(hControl, FALSE);
		EQC_SET_USERDATA(hControl, ICT_MENU_E);
		EQC_SET_DX(hControl, Width2);

        nAdd    = 1;
        nHeight = EQC_GET_DY(hControl) + ySpace;

        break;
    default:    
        return INVALID_CONTROL_HANDLE;
    
    break;
    }

    for( nIndex = nInsertPosition+nAdd; nIndex < nControl+nAdd; nIndex++ )
    {
        hTemp = EQS_GetNthIOC( nIndex );
        if( hTemp != INVALID_CONTROL_HANDLE )
            EQC_SET_Y( hTemp , EQC_GET_Y(hTemp) - (60 -nHeight) );
    }  
   
	EQS_FitCanvasSize();
    if( fFocus == TRUE )
        EQS_SetIOCFocus( hControl );
        
    return hControl;    
}



LOCAL INT8 sGetScrollLine(INT8 Handle, UINT8 nLineA)
{
    INT16 nLine;

    nLine = GetListSize( Handle );
    if( nLine <= 0 )
        return 0;
    if( nLine < nLineA )
    {
        return nLine;
    }
    return nLineA;
}


LOCAL void sMakeFormattedString(BYTE* pszBuffer, INT8 hList, WORD nIndex, SCROLL_DATA_E Type)
{   
    pb_number_info_type  NumberInfo;	
    INT                  Value, i;
    UINT8                CodePos, HypenPos;

    HCONTROL hControl, hType;
    INT8     nPos;
    BYTE     szTemp[32];


	if( nIndex >= GetListSize(hList) )			
		return;

    switch( Type )
    {
    case SDE_EMAIL_GROUP_LIST:
    case SDE_IRDA_GROUP:
    case SDE_SPEED_GROUP:
    case SDE_GROUP_LIST:
    case SDE_GROUP_EDIT:
    case SDE_GROUP_SELECT:
    case SDE_GROUP_REORDER:

        Value = GetListValue( hList, nIndex);
        if( Value != 0 )
        {			
			nPos = STRLEN( g_szGroupName[Value-1] );
            if( Type == SDE_SPEED_GROUP || Type == SDE_GROUP_LIST ||\
				Type == SDE_IRDA_GROUP  || Type == SDE_EMAIL_GROUP_LIST )
            {
                sprintf((BYTE*)pszBuffer,"%s%s",SF_GROUP,(char*)g_szGroupName[Value-1]);
#if 0                				
				if( nPos > MAX_VIEW_GROUP_NAME_LEN  )
				{
					if( IsMiddleOfMultiByte( pszBuffer, MAX_VIEW_GROUP_NAME_LEN+2) )
					{
						sprintf((CHAR*)(pszBuffer+MAX_VIEW_GROUP_NAME_LEN),"%s ", (CHAR*) SF_ETC_SYMBOL);
						//STRCPY(pszBuffer+MAX_VIEW_GROUP_NAME_LEN, (BYTE*) SF_ETC_SYMBOL );
					}
					else
					{
						STRCPY(pszBuffer+MAX_VIEW_GROUP_NAME_LEN+2, (BYTE*) SF_ETC_SYMBOL );
					}
				}
#endif
            }
            else if( Type == SDE_GROUP_EDIT)
            {
                sprintf((BYTE*)pszBuffer,"%s%s",SF_GROUP,(char*)g_szGroupName[Value-1]);
#if 0
				if( nPos > MAX_VIEW_GROUP_NAME_LEN  )
				{
					if( IsMiddleOfMultiByte( pszBuffer, MAX_VIEW_GROUP_NAME_LEN+2) )
					{
						sprintf((CHAR*)(pszBuffer+MAX_VIEW_GROUP_NAME_LEN),"%s ", (CHAR*) SF_ETC_SYMBOL);
						//STRCPY(pszBuffer+MAX_VIEW_GROUP_NAME_LEN, (BYTE*) SF_ETC_SYMBOL );
					}
					else
					{
						STRCPY(pszBuffer+MAX_VIEW_GROUP_NAME_LEN+2, (BYTE*) SF_ETC_SYMBOL );
					}
				}
#endif
            }
            else
            {
                sprintf((BYTE*)pszBuffer,"%s", (char*)g_szGroupName[Value-1]);
            }

        }
        else if( Type == SDE_GROUP_SELECT )
        {
            STRCPY(pszBuffer, SKY_GET_TOKEN(TKN_PBOOK_NEW_GROUP));

        }
        else
        {
            STRCPY(pszBuffer,SKY_GET_TOKEN(TKN_PBOOK_ENTIRE_LIST));
        }

        break;
    case SDE_IRDA_BOOK:
      //  Value =  GetListValue( hList, nIndex );
      //  STRCPY(pszBuffer, g_szPersonName[Value-1]);
      //  break;

	case SDE_GROUP_TEL_LIST:
    case SDE_EMAIL_BOOK_LIST:
    case SDE_EMAIL_BOOK_ALLLIST:
    case SDE_BOOK_LIST:
    case SDE_SEARCH_NAME:   
    case SDE_SEARCH_NUMBER:  
		{		
			BYTE* phone_image[] = { 
			   SF_CELLULAR1,
			   SF_CELLULAR2,
			   SF_CELLULAR3,
			   SF_CELLULAR4,
			   SF_CELLULAR5,
			   SF_HOME1,
			   SF_HOME2,
			   SF_HOME3,
			   SF_HOME4,
			   SF_HOME5,
			   SF_OFFICE1,
			   SF_OFFICE2,
			   SF_OFFICE3,
			   SF_OFFICE4,
			   SF_OFFICE5,
			   SF_PAGER1,
			   SF_PAGER2,
			   SF_PAGER3,
			   SF_PAGER4,
			   SF_PAGER5,
			   SF_FAX1,
			   SF_FAX2,
			   SF_FAX3,
			   SF_FAX4,
			   SF_FAX5
			}; 
			UINT8 NumberType[PERSONAL_NUMBER_LIMIT];
			
#if BOOK_FLOW_MODE
		    Value = GetListValue( hList, nIndex );
			if( STRLEN(g_szPersonName[Value-1]) )
			{
				if( KOREAN_STRCMP( g_szPersonName[Value-1], SKY_GET_TOKEN(TKN_PBOOK_NO_NAME)) == 0 )
				{
					sprintf(pszBuffer,"(%s)",g_szPersonName[Value-1]);	
				}
				else
				{
					STRCPY(pszBuffer,g_szPersonName[Value-1]);
				}

				nPos = 0;
				for( i=0; i< PERSONAL_NUMBER_LIMIT; i++)
				{
					if( g_PersonNumberType[Value-1][i] != BOOK_NOT_USE )
					{
						NumberType[nPos++] = g_PersonNumberType[Value-1][i];
					}
				}				
				for(i = 0; i< PERSONAL_NUMBER_LIMIT; i++)
				{
					if( i < nPos )
					{
						g_PersonNumberType[Value-1][i] = NumberType[i];
						STRCAT(pszBuffer,phone_image[(NumberType[i]-1)*PERSONAL_NUMBER_LIMIT+i] );
					}
					else
					{
						g_PersonNumberType[Value-1][i] = BOOK_NOT_USE;
					}
				}
			}
			else
			{
				pszBuffer[0] = 0;
			}
#else
			STRCPY(pszBuffer,"                  ");    // space is 16
			Value =  GetListValue( hList, nIndex );        

			nPos = STRLEN(g_szPersonName[Value-1]);
			if( KOREAN_STRCMP( g_szPersonName[Value-1], SKY_GET_TOKEN(TKN_PBOOK_NO_NAME)) == 0 )
			{
				pszBuffer[0] = '(';
				STRNCPY( pszBuffer+1, g_szPersonName[Value-1], nPos );
				pszBuffer[nPos+1] = ')';		
			}
			else if( nPos == MAX_VIEW_BOOK_NAME_LEN )
			{
				if( IsMiddleOfMultiByte(g_szPersonName[Value-1],MAX_VIEW_BOOK_NAME_LEN ) )
					STRNCPY(pszBuffer, g_szPersonName[Value-1], MAX_VIEW_BOOK_NAME_LEN -1);
				else            
					STRNCPY(pszBuffer, g_szPersonName[Value-1], MAX_VIEW_BOOK_NAME_LEN);

			}
			else if( nPos > MAX_VIEW_BOOK_NAME_LEN )
			{
				if( IsMiddleOfMultiByte(g_szPersonName[Value-1],MAX_VIEW_BOOK_NAME_LEN-2) )
				{
					STRNCPY(pszBuffer, g_szPersonName[Value-1], MAX_VIEW_BOOK_NAME_LEN -3);
					STRNCPY(pszBuffer+MAX_VIEW_BOOK_NAME_LEN-3, (BYTE*)SF_ETC_SYMBOL,2);
				}
				else
				{
					STRNCPY(pszBuffer, g_szPersonName[Value-1], MAX_VIEW_BOOK_NAME_LEN -2);
					STRNCPY(pszBuffer+MAX_VIEW_BOOK_NAME_LEN-2, (BYTE*)SF_ETC_SYMBOL,2);
				}				
			}
			else
			{
				STRNCPY(pszBuffer, g_szPersonName[Value-1], nPos);				
			}
			pszBuffer[MAX_VIEW_BOOK_NAME_LEN] = 0;
			          
			nPos = 0;
			for( i=0; i< PERSONAL_NUMBER_LIMIT; i++)
			{
				if( g_PersonNumberType[Value-1][i] != BOOK_NOT_USE )
				{
					NumberType[nPos++] = g_PersonNumberType[Value-1][i];
				}
			}				
			for(i = 0; i< PERSONAL_NUMBER_LIMIT; i++)
			{
				if( i < nPos )
				{
					g_PersonNumberType[Value-1][i] = NumberType[i];
					STRCAT(pszBuffer,phone_image[(NumberType[i]-1)*PERSONAL_NUMBER_LIMIT+i] );
				}
				else
				{
					g_PersonNumberType[Value-1][i] = BOOK_NOT_USE;
				}
			}
#endif			
		}

        break;                 
    case SDE_EDIT_SDIAL:        		   
		NumberInfo.address = GetListValue(hList,nIndex);
		if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &NumberInfo ) == TRUE )                    
		{              

			STRCPY(pszBuffer, g_szPersonName[NumberInfo.book_address-1]);
			STRCAT(pszBuffer, phone_type[NumberInfo.number_type-1]);
		}
		else
		{				
			STRCPY(pszBuffer,"");			
		}

#if 0
		if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &NumberInfo ) == TRUE )                    
		{              
			if( (nIndex+1) < MAX_LIMIT_DIGIT )
			{
				sprintf((char*)pszBuffer,"%c%c%s%s",SPECIAL_DIGIT,SPECIAL_DIGIT_1+nIndex,\
				                                    g_szPersonName[NumberInfo.book_address-1],\
												    phone_type[NumberInfo.number_type-1]);
			}				                                   
			else
			{
				sprintf((char*)pszBuffer,"%c%c%s%s",SPECIAL_DIGIT_NEXT_BASE,\
								                    SPECIAL_DIGIT_96+nIndex+1-MAX_LIMIT_DIGIT,\
				                                    g_szPersonName[NumberInfo.book_address-1],\
												    phone_type[NumberInfo.number_type-1]);
			}
		}
		else
		{
			if( (nIndex+1) < MAX_LIMIT_DIGIT )
			{
				sprintf((char*)pszBuffer,"%c%c",SPECIAL_DIGIT,SPECIAL_DIGIT_1+nIndex);
			}				                                   
			else
			{
				sprintf((char*)pszBuffer,"%c%c",SPECIAL_DIGIT_NEXT_BASE,\
								                      SPECIAL_DIGIT_96+nIndex+1-MAX_LIMIT_DIGIT);

			}			
		}
#endif
        break;

    case SDE_TITLE_NUMBER:
        hControl = (HCONTROL)GetListType(hList, nIndex);
        if( hControl > 0 )
        {
            hType = EQS_GetNthIOC(EQS_GetIOCOrder(hControl) -1 );
            STRCPY(pszBuffer, phone_type[EQC_GetScrollPhysicalIndex(hType)]);
            STRCAT(pszBuffer,(BYTE*)"  ");
            nPos = STRLEN(pszBuffer);
            EQC_GetNumberEditValue(hControl, szTemp);
            CodePos  = FindCodeSet(szTemp);
            HypenPos = FindHypen(szTemp,CodePos,MAX_HYPEN_DIGIT_LENGTH);
            MakeHypenedNumber(szTemp, pszBuffer+nPos,20,CodePos,HypenPos);
			pszBuffer[nPos+20] = 0;
        }
        break;
    case SDE_SELECT_NUMBER:
        NumberInfo.address = GetListValue(hList,nIndex);
        if( ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) > 0 )
        {
            STRCPY(pszBuffer, phone_type[NumberInfo.number_type-1]);
            STRCAT(pszBuffer, (BYTE*)"  ");
            nPos = STRLEN(pszBuffer);
            CodePos  = FindCodeSet(NumberInfo.digits);
            HypenPos = FindHypen(NumberInfo.digits,CodePos,MAX_HYPEN_DIGIT_LENGTH);
            MakeHypenedNumber(NumberInfo.digits, pszBuffer+nPos,20,CodePos,HypenPos);
			pszBuffer[nPos+20] = 0;
        }
		else
		{
			pszBuffer[0] = 0;
		}
        break;    
	case SDE_SELECT_EMAIL:
		{
			pb_email_info_type EmailInfo;

			EmailInfo.address = GetListValue(hList,nIndex);
			if( ui_get_pb(PB_EMAIL_INFO_I, &EmailInfo) > 0 )
			{
				STRCPY(pszBuffer, EmailInfo.szMailID);
			}
			else
			{
				pszBuffer[0] = 0;
			}
			break;  
		}
	
    default:
        break;
    }       
}

LOCAL HCONTROL  sInitScrollList(COORD x, COORD y, UINT8 nChar, UINT8 nLine, \
                                UINT8 nInitIndex, UINT8 HList, SCROLL_DATA_E Type)
{
    HCONTROL  HControl;
    BYTE*     pScrollBuffer;
    BYTE*     ppList[12];
    UINT8     i;
    INT16     nSize, nInitData;


    pScrollBuffer = sAllocBuffer(nLine+1);
    for(i=0; i<=nLine; i++)
    {
        ppList[i] = sIndexBuffer( pScrollBuffer, i );
    }
    nSize = GetListSize(HList);
    
    if( nSize > nLine )
    {
        nInitData = nLine+1;
    }
    else
    {
        nInitData = nSize;
    }
    for(i=0; i< nInitData ; i++)
    {        
        sMakeFormattedString(ppList[i], HList, i, Type);
    }
    for(i=nInitData; i<= nLine ; i++)
    {        
        STRCPY(ppList[i],(BYTE*)"");     
    }
           
    HControl = EQC_ScrollList(x,y,nChar,nLine, ppList,  \
                              nLine+1,nSize,nInitIndex);   
    return HControl;

}

LOCAL HCONTROL sMakeScrollList(COORD x, COORD y, UINT8 nChar, UINT8 nLines, \
                               UINT8 nInitIndex, UINT8 uData, WORD wData, SCROLL_DATA_E Type)
{
    HCONTROL  Handle;
    UINT8 hList, nViewLine;
    INT16 i, j, nSize, nAddress;
    BYTE  szTitle[80];
            
    pb_sdial_type        SDial;
	pb_number_info_type  NumberInfo;

    hList = 0;
    STRCPY(szTitle,"");
    nViewLine = nLines;

    switch( Type )
    {
	case SDE_IRDA_GROUP:		
    case SDE_SPEED_GROUP:
    case SDE_GROUP_LIST:
    case SDE_GROUP_EDIT:
    case SDE_GROUP_SELECT:
    case SDE_GROUP_REORDER:    

        hList = GetGroupList();                        
        if( Type == SDE_GROUP_SELECT )
        {
            ;//InsertList( hList,0, 0, EQS_NA);  // 그룹입력 
        }
        else if( Type == SDE_SPEED_GROUP || Type == SDE_IRDA_GROUP )
        {
            nSize = GetListSize(hList);
            for(i=0; i<nSize; i++)
            {
                if( GetNumOfGroupMember(GetListValue(hList,i)) == 0 )
                {
                    RemoveListItem(hList,i);
                    i--;
                    nSize--;
                }
            }
            if( nSize == 0 )
            {
                DeleteList(hList);
                return 0;   // 데이터가 하나도 없음.
            }

			if( Type == SDE_SPEED_GROUP )
			{				
				InsertList(hList,0, 0, EQS_NA); //<전체리스>를 맨 앞에 넣는다.
			}
        }
#if 0
        else if( Type == SDE_GROUP_EDIT )  // [지정안됨]은 편집에서 제외 
        {
            RemoveListItem(hList,GetListSize(hList)-1);
        }
#endif

        else ;

        nSize = GetListSize(hList);
        if( nSize == 0 )
        {
            DeleteList(hList);
            return 0;
        }
        
        if( sGetScrollLine( hList, nLines) == 0 )
        {
            DeleteList(hList);
            return 0;     
        }
        break;

    case SDE_EMAIL_GROUP_LIST:
        hList = GetEmailList();
        if( hList == 0 )
            return 0;

        break;

    case SDE_EMAIL_BOOK_ALLLIST:
    case SDE_EMAIL_BOOK_LIST:        
        nSize = GetListSize(uData);
        hList = CreateList(nSize);		
        for(i=0; i<nSize; i++)
        {
            if( wData == 0 || GetListType(uData,i) == wData )
            {
                AddList(hList, GetListValue(uData,i) , EQS_NA);
            }
        }
        if( GetListSize(hList) == 0 )
        {
            DeleteList(hList);
            return 0;
        }
        if( wData > 0 )        
            sprintf( (CHAR*)szTitle,"%s%s",SF_GROUP, g_szGroupName[wData-1]);
               
        break;
    
    case SDE_BOOK_LIST:  
    case SDE_IRDA_BOOK:

        hList = GetBookList(wData);
        SortBookName(hList);
        if( sGetScrollLine( hList, nLines) == 0 )
            return 0;
        if( Type == SDE_IRDA_BOOK )
            sprintf( (CHAR*)szTitle, SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_BOOK) );
        else if( wData == 0 )
            sprintf( (CHAR*)szTitle,SKY_GET_TOKEN(TKN_PBOOK_STRING_ENTIRE_LIST));
        else
            sprintf( (CHAR*)szTitle,"%s%s",SF_GROUP, g_szGroupName[wData-1]);
                               
        break;

	case SDE_GROUP_TEL_LIST:
        hList = GetBookList(wData);

		for( i=GetListSize(hList)-1; i >=0;  i--)
		{
			nAddress = GetListValue(hList,i);
			for(j=0; j< PERSONAL_NUMBER_LIMIT; j++)
			{
				if( g_PersonNumberType[nAddress-1][j] == CELL )
					break;
			}
			if( j == PERSONAL_NUMBER_LIMIT )
			{
				RemoveListItem(hList,i);
			}
		}        
        if( sGetScrollLine( hList, nLines) == 0 )
            return 0;

		SortBookName(hList);

        if( wData == 0 )
            sprintf( (CHAR*)szTitle,SKY_GET_TOKEN(TKN_PBOOK_STRING_ENTIRE_LIST));
        else
            sprintf( (CHAR*)szTitle,"%s%s",SF_GROUP, g_szGroupName[wData-1]);
		break;
    case SDE_SEARCH_NAME:       
    case SDE_SEARCH_NUMBER:        
        hList = wData;
        if( sGetScrollLine( hList, nLines) == 0 )
            return 0;

        break;
    case SDE_EDIT_SDIAL:
        hList = CreateList(MAX_SDIAL_NUMBER+1);
        for(i=1; i<=MAX_SDIAL_NUMBER; i++)
        {
            SDial.address = i;
            ui_get_pb( PB_SDIAL_I, &SDial);                            
			NumberInfo.address = SDial.dial_address;
			if( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &NumberInfo ) == TRUE ) 
			{
				if( NumberInfo.digits[0] != 0 )
					AddList( hList, NumberInfo.address, EQS_NA);
				else
					AddList( hList,EQS_NA, EQS_NA);
			}
			else
				AddList( hList,EQS_NA, EQS_NA);										
        }
		STRCPY( szTitle, SKY_GET_TOKEN(TKN_EDIT_SHORT_DIAL));
        break;
    case SDE_TITLE_NUMBER:     // 대표번호 설정 
    case SDE_SELECT_NUMBER:    // 번호 선택
	case SDE_SELECT_EMAIL:
        if( wData > 0 )
        {
            hList = wData;
            if( sGetScrollLine( hList, nLines) == 0 )
                return 0;               
        }
        else
            return 0;
        break;
       
    default:
        return 0;
        
    }
    if( hList > 0 && GetListSize( hList ) == 0 )
        return 0;
    Handle = sInitScrollList(x,y,nChar,nViewLine,nInitIndex,hList,Type);
    EQC_SetScrollTitle( Handle, szTitle);
    EQC_FreeScrollMemory(Handle,TRUE);
    EQC_SetScrollListHandle(Handle, hList);

    return Handle;

}


LOCAL void SKY_API sSKA_AccessSecretGroup(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                //SKY_GET_TOKEN(TKN_PHONE_PASSWORD), 
				SKY_GET_TOKEN(TKN_PBOOK_SECRET_GROUP_MSG),
                WA_SHOW_HEADER| WA_SHOW_CML);
            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, (int)NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			//EQS_FitCanvasSize();
            EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!KOREAN_STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_END_EVENT(SE_APP_RESUME, EQS_NA, (DWORD)1);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        break;

	case SE_KEY_CLEAR:
		EQS_END();
		break;
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;    
        
    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;

    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}


LOCAL void SKY_API sSKA_GetString(SKY_EVENT_T *pEvent)
{
    HCONTROL hControl;
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            "", 
            WA_SHOW_HEADER);


		if( EQS_GET_PARENT() == SKA_EditBookContent )
		{
			EQS_SetSWindowHeaderStr( SKY_GET_TOKEN(TKN_PBOOK_EDIT) );			
		}
		else if( EQS_GET_PARENT() == SKA_BookInput )
		{
			EQS_SetSWindowHeaderStr( SKY_GET_TOKEN(TKN_PHONE_BOOK_SAVE) );
		}
		else if( EQS_GET_PARENT() == SKA_EditGroup )
		{   //그룹편집
			EQS_SetSWindowHeaderStr( SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU6) );
		}
		else ;

	    if( pEvent->wParam == INPUT_ANNIV_NAME_E )
		{
			hControl = EQC_StaticText(5, 10, TKN_PBOOK_INPUT_ANNIV_NAME_MSG);
			
		}
		else if(pEvent->wParam == INPUT_GROUP_NAME_E )
		{
			hControl = EQC_StaticText(5, 10, TKN_PBOOK_INPUT_GROUP_NAME_MSG);
		}
		else if(pEvent->wParam == INPUT_NEW_GROUP_NAME_E )
		{
			hControl = EQC_StaticText(5, 10, TKN_PBOOK_INPUT_NEW_GROUP_NAME_MSG);
		}
		else 
		{
			hControl = EQC_StaticText(5, 10, TKN_BLANK);
		}
		EQC_SET_USERDATA(hControl, pEvent->wParam);
		        
		hControl  = EQC_TextEdit(10 , 30, 16,1,\
								 TEM_KOREAN, MAX_BOOK_NAME, l_szParam, 0);
		EQC_SET_USERDATA(hControl, pEvent->dwParam);
	    if( pEvent->wParam == INPUT_ANNIV_NAME_E )
		{
			EQC_SetTextEditData( hControl,\
				                 EQC_GetScrollData(pEvent->dwParam, ETC_ANNIV-1));
		}

		EQS_SetIOCActive(hControl);
		

        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;

    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData( pEvent->wParam, l_szParam);
		hControl = EQS_GetNthIOC(0);
		EQS_END_EVENT(SE_APP_RESUME, EQC_GET_USERDATA(hControl),\
			                         EQC_GET_USERDATA(pEvent->wParam));
        break;  	
    case SE_IO_TEDIT_CANCEL:
	case SE_KEY_CLEAR:
		hControl = EQS_GetNthIOC(1);
        EQS_END_EVENT(SE_APP_RESUME, EQS_NA, EQC_GET_USERDATA(hControl));
		break;             
    case SE_IO_MESGBOX_END:
        break;
    default :
        SKA_BookDefaultProcess(pEvent);
        break;
    }
}

LOCAL BOOL sIsSameFirstName( BYTE* szName, BYTE* szCompareName )
{
    if( szName[0] & 0x80 )
	{
		if( szName[0] == szCompareName[0] && szName[1] == szCompareName[1] )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if( szName[0] == szCompareName[0] )
			return TRUE;
		else
			return FALSE;
	}

}

LOCAL BOOL sMake_Anniv_Icon(HCONTROL hControl, UINT8  nAnnivType )
{
	BYTE szBuf[5];
#if 0
	switch( nAnnivType )
	{

	case BIRTH_ANNIV:            //1  
		sprintf((char*)szBuf," %s ",(char*) SF_CAKE );
		break;
	case WEDDING_ANNIV:            //2
		sprintf((char*)szBuf," %s ",(char*) SF_RING );
		break;
	case MEET_ANNIV:               //3
		sprintf((char*)szBuf," %s ",(char*) SF_LOVE );
		break;
    case ETC_ANNIV:                //4
		sprintf((char*)szBuf," %s ",(char*) SF_COCKTAIL);
		break;
	default:
		return FALSE;
	}		
	EQC_SetStaticText(hControl, szBuf);
#endif
	return TRUE;
}

void sAdjustBookContent(BOOK_CONTROL_E ControlType)
{
	HCONTROL hControl, hFocused, hIcon;
	COORD    XPos, YPos, ySpace;
	UINT8    nIndex, nFocusOrder;

	hFocused = EQS_GetFocusedIOC();
	nFocusOrder = EQS_GetIOCOrder(hFocused);
	hIcon    = EQS_GetNthIOC(nFocusOrder-1);
	
    if( ControlType == ICT_ANNIV_E &&  EQC_GET_TYPE(hFocused) == CT_TIME )
	{
		nFocusOrder -=2;
		hFocused = EQS_GetNthIOC(nFocusOrder);
	}
		
	ySpace = EQC_GET_Y(EQS_GetNthIOC(nFocusOrder+1)) - EQC_GET_Y(hFocused) -EQC_GET_DY(hFocused) + 1;
	for( nIndex = EQS_GetIOCNum()-1; nIndex > nFocusOrder; nIndex -=2 )
	{
		YPos = EQC_GET_Y( EQS_GetNthIOC(nIndex-2) );
		EQC_SET_Y( EQS_GetNthIOC(nIndex), YPos);
		EQC_SET_Y( EQS_GetNthIOC(nIndex-1), YPos);
	}

    if( ControlType == ICT_ANNIV_E )
	{
		EQS_DeleteControl(hFocused);
		EQS_DeleteControl(hIcon);

		for( nIndex = EQS_GetIOCNum()-1; nIndex > nFocusOrder; nIndex -=2 )
		{
			YPos = EQC_GET_Y( EQS_GetNthIOC(nIndex-2) );
			EQC_SET_Y( EQS_GetNthIOC(nIndex), YPos);
			EQC_SET_Y( EQS_GetNthIOC(nIndex-1), YPos);
		}

		EQS_DeleteControl(EQS_GetNthIOC(nFocusOrder));
		EQS_DeleteControl(EQS_GetNthIOC(nFocusOrder-1));

		EQS_SetIOCFocus(EQS_GetNthIOC(nFocusOrder));

	}
	else
	{
		EQS_DeleteControl(hFocused);
		EQS_DeleteControl(hIcon);
		EQS_SetIOCFocus(EQS_GetNthIOC(nFocusOrder));
	}
	for( nIndex = nFocusOrder+1; nIndex < EQS_GetIOCNum(); nIndex+=2 )
	{
		hControl = EQS_GetNthIOC(nIndex-1);
		EQC_SET_Y(EQS_GetNthIOC(nIndex), EQC_GET_Y(hControl) + EQC_GET_DY(hControl) + ySpace);
		EQC_SET_Y(EQS_GetNthIOC(nIndex+1), EQC_GET_Y(hControl) + EQC_GET_DY(hControl) + ySpace);
	}
	EQS_FitCanvasSize();
}

#ifdef FEATURE_CHARACTER_BELL
VOID SKY_API SKA_CBSearchNumber(SKY_EVENT_T* pEvent) {
	HCONTROL  hControl;
    UINT8     ListHandle;
    int       index, nSize, i;

    switch (pEvent->EventID) 
    {
    case SE_APP_START:
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_NONE,
                      NULL, 
                      WA_SHOW_CML);

        hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA,\
                                   EQC_GetScrollDisplayLine(),\
                                   0,(UINT8)0,(WORD)0,SDE_SPEED_GROUP);
        if(hControl == 0 )
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);
        }
        else
        {
            ListHandle = EQC_GetScrollListHandle( hControl );
            if( ListHandle == 0 )
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
                               MESSAGE_DEFAULT_TIME,\
                               MBT_NONE, MBT_HELP);
            }
            else
            {
                // Speed Search                                  
                EQC_SetScrollTitle( hControl, SKY_GET_TOKEN(TKN_PBOOK_MAIN_MENU1));
                {
                    BYTE szBuf[21];
				    EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
										    CS_SCROLL_LIST_ENUMBERATED|\
										    CS_SCROLL_LIST_MAXIMIZE);
										    //CS_SCROLL_LIST_STATUS_BAR);
				    sprintf(szBuf, SKY_GET_TOKEN(TKN_PBOOK_USAGE_MSG),\
						    (MAX_BOOK_NUMBER - active_list.nFreeBook));
				    EQC_SetScrollStatusBarText(hControl, szBuf);
                }
                EQC_ShowScrollTitle(hControl, TRUE );         
                EQC_SetLogicalList(hControl,TRUE);     
                EQS_SetIOCActive( hControl );
			    //EQC_SET_USERDATA(hControl, 0);
            }
        }
        break;

    case SE_APP_END:
        break;
    
    case SE_APP_SUSPEND:
        break;

    case SE_APP_RESUME:
        if( EQS_GET_CHILD() == sSKA_AccessSecretGroup )
        {	
            hControl = EQS_GetNthIOC(0);
			if( pEvent->dwParam != 0 )
			{       
                index    = EQC_GetScrollLogicalIndex(hControl);
                ListHandle = EQC_GetScrollListHandle( hControl );

                // Speed Search                                  
				EQS_START_CHILD_EVENT( SKA_CBViewBookList, SE_APP_START,\
                                       GetListValue( ListHandle, index ),\
                                       EQS_NA);
			}
			else
			{
				EQS_SetIOCActive(hControl);
			}
        }
        else if (pEvent->dwParam) 
        {// 값을 선택하고 돌아오는 경우
            EQS_END_PARAM(EQS_NA, TRUE);
        }
        else 
        {
            hControl = EQS_GetNthIOC(0);
            index    = EQC_GetScrollLogicalIndex(hControl);
            ListHandle = EQC_GetScrollListHandle( hControl );
            {
			    BYTE szBuf[21];
			    sprintf(szBuf,SKY_GET_TOKEN(TKN_PBOOK_USAGE_MSG),\
					    (MAX_BOOK_NUMBER - active_list.nFreeBook));
			    EQC_SetScrollStatusBarText(hControl, szBuf);

			    DeleteList(ListHandle);
                ListHandle = GetGroupList();
			    EQC_SetScrollListHandle(hControl, ListHandle);
                nSize = GetListSize(ListHandle);
                for(i=0; i<nSize; i++)
                {
                    if( GetNumOfGroupMember(GetListValue(ListHandle,i)) == 0 )
                    {
                        RemoveListItem(ListHandle,i);
                        i--;
                        nSize--;
                    }
                }
			    //AddList(ListHandle, EQS_NA, EQS_NA); //전체리스트
			    InsertList(ListHandle, 0, 0, EQS_NA); //<전체리스트>
			    nSize++;	            
			    EQC_SetScrollMaxDataLine(hControl, nSize);
			    if( index < nSize )
			    {
				    EQC_SetScrollLogicalIndex(hControl, index);
			    }
			    else
			    {
				    EQC_SetScrollLogicalIndex(hControl, 0);
			    }
			    EQS_SetIOCActive(hControl);
            }	
        }
        break;

    case EQE_KEY_CLEAR:
        //EQS_End();
        EQS_END_PARAM(EQS_NA, FALSE);
        break;
    
    case SE_KEY_END :   // The very last event
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_END :
        EQS_END();
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        //EQS_End();
        EQS_END_PARAM(EQS_NA, FALSE);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );

        // Speed Search                                  
        if( index == 0 || IS_SECRET_GROUP( GetListValue(ListHandle, index) ) == FALSE )
        {  //전체 이름이거나 비밀 그룹이 아닌 경우 
            EQS_START_CHILD_EVENT( SKA_CBViewBookList, SE_APP_START,\
                                   GetListValue( ListHandle, index ),\
                                   EQS_NA);
        }
        else
		{
            EQS_START_CHILD( sSKA_AccessSecretGroup);
		}
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            TRUE, SDE_SPEED_GROUP);
        break;

    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                            FALSE, SDE_SPEED_GROUP);
        break;

    default:
        break;
    }
} // SKA_CBSearchNumber

VOID SKA_CBViewBookList(SKY_EVENT_T *pEvent)
{      
    HCONTROL            hControl;
    int					i,index;
    INT16				ListHandle;   

    switch (pEvent->EventID)
    {
    case SE_APP_START:    
		if( pEvent->wParam == 0 ) // 전체리스트 
		{
			EQS_SetWindow(SKY_DEF_WIN_X,
				          SKY_DEF_WIN_Y, 
					      SKY_DEF_WIN_DX, 
						  SKY_DEF_WIN_DY, 
						  CL_NONE_SELECT_NONE, 
						  SKY_GET_TOKEN(TKN_PBOOK_ENTIRE_LIST),
						  WA_SHOW_HEADER|WA_SHOW_CML);
		}
		else
		{
			EQS_SetWindow(SKY_DEF_WIN_X,
				          SKY_DEF_WIN_Y, 
					      SKY_DEF_WIN_DX, 
						  SKY_DEF_WIN_DY, 
						  CL_NONE_SELECT_NONE, 
						  "",
						  WA_SHOW_CML);
		}

        hControl = sMakeScrollList(EQS_NA,EQS_NA,EQS_NA, \
                                       EQC_GetScrollDisplayLine(), \
                                       0, (UINT8)0, pEvent->wParam, SDE_BOOK_LIST);

        if( hControl == 0 )
        {
            hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_VALID_DATA_MSG),\
									  MESSAGE_DEFAULT_TIME,\
									  MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_NO_DATA_MSG_E);
        }
        EQC_SET_USERDATA(hControl, pEvent->wParam); // save Group Address	
        ListHandle = EQC_GetScrollListHandle( hControl );

        if( ListHandle != 0 )
        {            
			BYTE szBuf[20];
            //EQC_SetScrollTitle(hControl, EQC_GetScrollTitle(hControl));
            EQC_ShowScrollTitle(hControl,TRUE);
            EQC_SetLogicalList(hControl, TRUE);    //redstar   00.12.13

			if( pEvent->wParam == 0 ) // 전체리스트 
			{
				WORD  TabData[15] = {0,0,0,0,0, 0,0,0,0,0,  0,0,0,0,0};
				BYTE* pTabText[15] = \
				{ "ㄱ","ㄴ","ㄷ","ㄹ","ㅁ","ㅂ","ㅅ","ㅇ","ㅈ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ","AZ\0"};

							
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
					                    CS_SCROLL_LIST_TAB|\
					                    CS_SCROLL_LIST_MAXIMIZE ); //CS_SCROLL_LIST_STATUS_BAR
				EQC_SetScrollTab(hControl,15,7,TRUE,0);
				for(i=0; i< GetListSize(ListHandle); i++)
				{
					TabData[GetKoreanCho(g_szPersonName[GetListValue(ListHandle,i)-1])]++;
				}
				EQC_SetScrollTabData(hControl, TabData, pTabText);
				EQC_SetScrollLogicalIndex(hControl,0);

			}
			else
			{
				EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
					                    CS_SCROLL_LIST_MAXIMIZE ); //CS_SCROLL_LIST_STATUS_BAR
			}
			
			sprintf(szBuf, SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
				    ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
			EQC_SetScrollStatusBarText(hControl, szBuf);
			
            EQS_SetIOCActive( hControl );          
        }
        else  // 메모리 할당 에러
        {
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_NO_DATA_MSG),\
				                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			EQC_SET_USERDATA(hControl, VIEW_BOOKLIST_NO_DATA_MSG_E);
			// EQS_END();            
        }  
        break;

    case SE_APP_END:
        break;

    case SE_APP_RESUME:   
        if (pEvent->dwParam) // 값을 선택하고 돌아오는 경우
            EQS_END_PARAM(EQS_NA, TRUE);
        else { // 취소키로 되돌아오는 경우
            hControl = EQS_GetNthIOC(0);
            EQS_SetIOCActive(hControl);

		    if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		    {
			    BYTE szBuf[20];
			    sprintf(szBuf, SKY_GET_TOKEN(TKN_PBOOK_STATUS_MSG),\
				        ((MAX_BOOK_NUMBER - active_list.nFreeBook)*100)/MAX_BOOK_NUMBER);			
			    EQC_SetScrollStatusBarText(hControl, szBuf);
		    }
        }
        break;    

    case SE_IO_SCROLL_LIST_CANCEL:
        //EQS_End();
        EQS_END_PARAM(EQS_NA, FALSE);
        break;
        
    case SE_IO_SCROLL_LIST_SELECT:
        hControl = pEvent->wParam;
        index    = pEvent->dwParam;
        ListHandle = EQC_GetScrollListHandle( hControl );        
                               
        EQS_START_CHILD_EVENT( SKA_CBViewBookContent, SE_APP_START, \
                               GetListValue( ListHandle, index ), \
                               BFT_NON_FUNCTION_E);
        break;

	case SE_IO_SCROLL_TAB_CHG:	
        sPBookDisplayScroll((HCONTROL)pEvent->wParam,\
				            EQC_GetScrollTabIndex(pEvent->wParam, pEvent->dwParam),\
							TRUE, SDE_BOOK_LIST);
        break;
    
    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                        TRUE, SDE_BOOK_LIST);
        break;

    case SE_IO_SCROLL_LIST_UP:
    case SE_IO_SCROLL_LIST_DOWN:
        sPBookDisplayScroll((HCONTROL)pEvent->wParam, (WORD)pEvent->dwParam,\
                        FALSE, SDE_BOOK_LIST);
        break;

    default:    
        SKA_BookDefaultProcess( pEvent );
        break;
    } 
} // SKA_CBViewBookList

VOID SKY_API SKA_CBViewBookContent(SKY_EVENT_T* pEvent)
{          
    static INT8     nNumber;
    static UINT8    hList;          //대표번호 설정을 위한 번호 리스트 핸들	
	static INT16    nBookAddress;

    HCONTROL            hControl;  
    INT16               i, nIndex;
    BYTE                szTemp[33];
	BYTE*               pszBuf;

    pb_book_info_type    BookInfo;
    pb_number_info_type  NumberInfo;

    switch( pEvent->EventID )
    {
    case SE_APP_START:                
        hList       = 0;
        nNumber     = 0;
		        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,
            SKY_GET_TOKEN(TKN_PBOOK_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);
            

        nBookAddress     = pEvent->wParam;
        BookInfo.address = nBookAddress;
        ui_get_pb( PB_BOOK_INFO_I, &BookInfo);    

        /* display number information */  
        NumberInfo.address = BookInfo.number_address;
        hList = CreateList(PERSONAL_NUMBER_LIMIT);           
        while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) == TRUE && nNumber < PERSONAL_NUMBER_LIMIT)
        {            
           if( NumberInfo.book_address == BookInfo.address && STRLEN(NumberInfo.digits) != 0 )
           {                                                        
                nIndex =  nNumber*2;//INITIAL_INPUT_CONTROL+(nGroup+nNumber)*2;
                hControl = sAddInputControl( nIndex, 1, ICT_NUMBER_E,FALSE );
                EQC_SetNumberEditValue(hControl, NumberInfo.digits);
				EQC_SET_USERDATA(hControl, NumberInfo.address);
                AddList(hList,NumberInfo.address,hControl);         //2001.01.16
                hControl = EQS_GetNthIOC((nNumber)*2);
				switch( NumberInfo.number_type )
				{
                case CELL :
					sprintf((char*)szTemp," %s ",(char*)SF_CELLULAR1);
					break;
				case HOME :
					sprintf((char*)szTemp," %s ",(char*)SF_HOME1);
					break;
				case WORK :
					sprintf((char*)szTemp," %s ",(char*)SF_OFFICE1);
					break;
				case PAGER:
					sprintf((char*)szTemp," %s ",(char*)SF_PAGER1);
					break;
				case FAX  :
					sprintf((char*)szTemp," %s ",(char*)SF_FAX1);
					break;
				default:
					sprintf((char*)szTemp," %s ",(char*)SF_CELLULAR1);
					break;
				}
				szTemp[2] += nNumber;
				EQC_SetStaticText(hControl, szTemp);
				//EQC_SET_USERDATA(EQS_GetNthIOC(EQS_GetIOCOrder(hControl)+1), NumberInfo.number_type);
                nNumber++;                
           }
           NumberInfo.address = NumberInfo.next_index;
        }
		
		if( nNumber == PERSONAL_NUMBER_LIMIT )
		{
			NumberInfo.address = GetListValue( hList, nNumber-1);
			ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo);
			NumberInfo.next_index = 0;
			ui_put_pb( PB_NUMBER_INFO_I, &NumberInfo);
		}

		pszBuf = (BYTE*) SKY_Malloc(sizeof(BYTE)*25);
		STRCPY( pszBuf, BookInfo.name );
		//STRCAT( pszBuf, (BYTE*) SF_BOOK );
		EQS_SetSWindowHeaderStr(pszBuf);

        for(i = 0; i < EQS_GetIOCNum(); i++)         // for only view not edit
        {
            hControl = EQS_GetNthIOC(i);
			EQC_SET_ENABLE(hControl,FALSE);
			EQC_SET_EDIT(hControl,FALSE);
			//EQC_SET_FOCUS(hControl, FALSE);
        }		
		if( nNumber == 0 )
		{
			EQS_SetWindowCML(CL_MENU_NONE_EDIT);
		}
        break;

    case SE_APP_END:   
		SKY_Free(EQS_GetCurrentHeaderStr());
        DeleteList(hList);
        break;

    case SE_APP_RESUME:
        hControl = EQS_GetNthIOC(0);
		break;

    case SE_KEY_CLEAR:
        EQS_END_PARAM(EQS_NA,FALSE);
        //EQS_End();
        break;

    case SE_KEY_END:
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_KEY_CENTER:
    case SE_SOFTKEY_SELECT:
        {
            //extern char    sGetPhoneNumber[MAX_TITLE_LENGTH];

            hControl = EQS_GetFocusedIOC();
            if( EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT ) {
                EQC_GetNumberEditValue(hControl, sGetPhoneNumber);	
            }
            EQS_END_PARAM(EQS_NA,TRUE);
        }
        break;      
    
    default:
        SKA_BookDefaultProcess( pEvent );
        break;
    }
} // SKA_CBViewBookContent
#endif // FEATURE_CHARACTER_BELL
