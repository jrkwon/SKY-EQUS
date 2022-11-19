/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_scal.c
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
** 01-01-02 rosa     Created.
**
** ===========================================================================
*/  

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SCHL_DAYS_AWEEK         7
#define SCHL_MONTHS_AYEAR       12
#define	MAX_COUNT_LUNAR_DAY	    483
#define	MAX_COUNT_KOR_HOLIDAY   12
#define	LEAP_MONTH			    100
#define MAX_NUM_HOLIDAY_TITLE   11//휴일명 최대 10bytes(5글자)까지 표시가능함
#define X_POS_CAL_TITLE         5//15

// for drawing calendar
#define sDAY_BAR_WIDTH          120//122//118
#define sDAY_BAR_HEIGHT         11//14
#define sCALENDAR_HEIGHT        102//(13*5)+1 // = 65

#define sDAY_BAR_COLUMN_WIDTH   16
#define sDAY_BAR_COLUMN_HEIGHT  14    
#define sDAY_BAR_COLUMN_SPACE   1//4

#define sFONT_WIDTH             6
#define sFONT_HEIGHT            10

#define sMARGIN_X               2//3//6
#define sMARGIN_Y               3//1

#define sOFFSET_X               ((DevGetMaxWidth() - sDAY_BAR_WIDTH)/2)
#define sOFFSET_Y               3
#define MAX_SIZE_CALENDAR_MENU  7

#define NEW_COLOR_SCHEME

#define REC_LARGE_WIDTH         16
#define REC_LARGE_HEIGHT        14
#define REC_SMALL_WIDTH         14
#define REC_SMALL_HEIGHT        12

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagSCHL_CAL_CURSOR_ON_E {
	SCCO_NONE,
    SCCO_HOLIDAY,
    SCCO_EVENT,
    SCCO_EVENT_HOLIDAY,
    SCCO_TODAY,
    SCCO_TODAY_HOLIDAY,    
    SCCO_MAX
};
typedef enum tagSCHL_CAL_CURSOR_ON_E SCHL_CAL_CURSOR_ON_E;

enum tagSCHL_SEARCH_CAL_E {
    SSC_SEARCH_CAL_IMAGE,
    SSC_SEARCH_CAL_TIME,
    SSC_SEARCH_CAL_LINE,
	SSC_SEARCH_CAL_RANGE_TEXT = 0,
    SSC_MAX
};

enum tagSCHL_CAL_TYPE_E {
    SCT_LUNAR,
    SCT_SOLAR
};
typedef enum tagSCHL_CAL_TYPE_E SCHL_CAL_TYPE_E;

struct tagSCHL_LUNAR_CAL_T {
    DWORD   unSYear;
    UINT8   unSMonth, unSDay;
    UINT8   unLMonth, unLDay;
};
typedef struct tagSCHL_LUNAR_CAL_T SCHL_LUNAR_CAL_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
SKY_TOKEN paSolarCalendarMenu[MAX_SIZE_CALENDAR_MENU] =
{
    TKN_SCHEDULE_CALENDAR_MENU6, // View Lunar Calendar
    TKN_SCHEDULE_CALENDAR_MENU1, // Calendar Search
    TKN_SCHEDULE_CALENDAR_MENU3, // My Holiday
    TKN_SCHEDULE_CALENDAR_MENU2, // List View
    TKN_SCHEDULE_CALENDAR_MENU5, // Delete Some
    TKN_SCHEDULE_CALENDAR_MENU4, // Alert Bell Setting
    TKN_SCHEDULE_CALENDAR_MENU8  // Anniversary Alarm Bell Setting
};

SKY_TOKEN paLunarCalendarMenu[MAX_SIZE_CALENDAR_MENU] =
{
    TKN_SCHEDULE_CALENDAR_MENU7, // View Solar Calendar
    TKN_SCHEDULE_CALENDAR_MENU1, // Calendar Search
    TKN_SCHEDULE_CALENDAR_MENU3, // My Holiday
    TKN_SCHEDULE_CALENDAR_MENU2, // List View    
    TKN_SCHEDULE_CALENDAR_MENU5, // Delete Some
    TKN_SCHEDULE_CALENDAR_MENU4, // Alert Bell Setting
    TKN_SCHEDULE_CALENDAR_MENU8  // Anniversary Alarm Bell Setting
};
/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
DWORD           l_dwSetTime;
SCHL_TIME_SET_T l_SetTime;          // current time
UINT8           l_unLunarMonth = 0, l_unLunarDay=0;
clk_julian_type l_cjToday;

LOCAL COORD   cursor_X = 0, cursor_Y = 0;
LOCAL COORD   cursor_X1 = 0, cursor_Y1 = 0;

LOCAL COORD   BmpCursorOn_X = 0, BmpCursorOn_Y = 0;
//LOCAL COORD   BmpEvent_X = 0, BmpEvent_Y = 0;
LOCAL COORD   BmpToday_X = 0, BmpToday_Y = 0;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
const static SCHL_LUNAR_CAL_T l_LunarDayT[MAX_COUNT_LUNAR_DAY] = 
{
    #include "LunarTable.sky"
};

const static SCHL_HOLIDAY_CAL_T l_KorHolidayT[MAX_COUNT_KOR_HOLIDAY] = 
{
    #include "HolidayTable.sky"
};

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
static VOID SKY_API sSKA_ViewSolarCalendar( SKY_EVENT_T* pEvent );
static VOID SKY_API sSKA_ViewLunarCalendar( SKY_EVENT_T* pEvent );
static VOID SKY_API sSKA_SearchCalendar(SKY_EVENT_T *pEvent);

static VOID sDrawSolarCalendar(VOID);
static VOID sDrawLunarCalendar(VOID);

static VOID sGetLunarDay(UINT8* unNewLunarMonth, UINT8* unNewLunarDay);


/*
** ***************************************************************************

                                    SKA_MainCalendar()
                                        |
                                        {child}
                                        |
                                    sSKA_ViewSolarCalendar()
                                        |
          -----------------------------------------------------------------
          |                     |                                         |
        {child}              {child}                                   {child}
          |                     |                                         |
   SKA_ViewADay() sSKA_EditAdvancedAlarm()  - {sibling} -  sSKA_ChooseAlarm()

** ***************************************************************************
*/
#ifdef NEW_COLOR_SCHEME
UINT8           l_nIdxColorFore, l_nIdxColorBack;
#endif

VOID SKY_API SKA_MainCalendar(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);

        /* set calendar time */
        l_dwSetTime = pEvent->dwParam;
        SetCalendarTime(l_dwSetTime);
        clk_secs_to_julian(l_dwSetTime, &l_cjToday);

#ifdef NEW_COLOR_SCHEME
        /* ColorScheme */
        l_nIdxColorFore = SKY_GET_COLOR_SCHEME();
        l_nIdxColorBack = CSB_WHITE;
        if(l_nIdxColorFore >= COLOR_SCHEME_COUNT)
        {
            l_nIdxColorBack = CSB_BLACK;
        }
#endif

        /* view calendar */
        EQS_START_CHILD(sSKA_ViewSolarCalendar);
        break;

    case SE_APP_END:
		FreeScheduleBlock();
        EQS_ReleaseDC();
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC( pEvent );
        break;
    }    
}



static VOID SKY_API sSKA_ViewSolarCalendar( SKY_EVENT_T* pEvent )
{
    static  BYTE         aszTitle[SCHL_MAX_SIZE_DAY_TITLE];    
    static  UINT8        unLunarMonth, unLunarDay;
	BYTE    szMainTitle[CAL_MAX_SIZE_TITLE];
    BYTE    szSubTitle[CAL_MAX_SIZE_TITLE];
	INT8    nIndex;

    static BOOL fDraw;
    UINT8  nSaveMonth, nSaveYear;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC( TRUE );

        /* set window */
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y,
                      SKY_DEF_WIN_DX,
                      SKY_DEF_WIN_DY, 
                      CL_MENU_CONFIRM_NEWSCHED,
                      //NULL, 
                      SKY_GET_TOKEN(TKN_BLANK),
                      WA_SHOW_CML | WA_SHOW_HEADER);

        /* draw calendar table */
        fDraw = TRUE;
        break;

    case SE_APP_DRAW:
        if(fDraw)
        {
            cursor_X = cursor_Y = 0;
            BmpCursorOn_X = BmpCursorOn_Y = 0;
            BmpToday_X = BmpToday_Y = 0;
            EQS_ClearRegion(sOFFSET_X, sOFFSET_Y, sOFFSET_X+sDAY_BAR_WIDTH, sOFFSET_Y+sCALENDAR_HEIGHT);
            EQS_PutBitmap(sOFFSET_X, sOFFSET_Y, &g_BmpCalendar);
            fDraw = FALSE;
        }
        /* draw day */
        sDrawSolarCalendar();

        /* get the lunar day of view day */
        sGetLunarDay(&unLunarMonth, &unLunarDay );
        if(unLunarMonth > LEAP_MONTH) /* leap month */
        {
            sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_SOLAR_LEAP_CAL_TITLE), 
                    l_SetTime.wYear, l_SetTime.nMonth, (unLunarMonth-LEAP_MONTH), unLunarDay );  
        }
        else
        {
            sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_SOLAR_CAL_TITLE), 
                    l_SetTime.wYear, l_SetTime.nMonth, unLunarMonth, unLunarDay );  

        }
        EQS_SetSWindowHeaderStr(aszTitle);
        break;

    case SE_APP_RESUME:

        fDraw = TRUE;

        if( EQS_GET_CHILD() == SKA_MainSchedulerList )
        {
            if( pEvent->dwParam == SCHL_HAS_NO_EVENT )
            {
                EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_NO_EVENT), 
                                MESSAGE_DEFAULT_TIME, 
                                MBT_NONE, 
                                MBT_HELP );
            }
        }
        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX *)paSolarCalendarMenu, MAX_SIZE_CALENDAR_MENU, 0);
        break;

    case SE_SOFTKEY_CONFIRM: // it searchs the schedule of that selected day.
		EQS_START_CHILD_EVENT( SKA_ViewADay, SE_APP_START, SMET_VIEW_LIST, GetCalendarTime() );
        break;

    case SE_SOFTKEY_ADD:
        nIndex = GetNewIdxOfEventT(l_SetTime.nDay - 1);
        if(nIndex != SCHL_ERROR)
        {						
			if( GetNewIdxOfSchlT() != SCHL_ERROR  &&\
			    (GetMyEventNumOfBlockT(l_SetTime.nDay-1)< SCHL_MAX_MYEVNTS_ADAY) ) //최대 100개를 넘지 않았을 때 
			{
				EQS_START_CHILD_EVENT( SKA_ViewADay, SE_APP_START, SMET_NONE, GetCalendarTime() );
				break;
			}
        }

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_FULL_EVENT_DAY),
                       MESSAGE_DEFAULT_TIME,
                       MBT_NONE, 
                       MBT_HELP);
		
        break;

    case SE_IO_POPUP_CANCEL:
    case SE_IO_MESGBOX_END:
    case SE_IO_EXMSG_END:
        fDraw = TRUE;
        break;

    case SE_IO_POPUP_SELECT:
        switch( pEvent->dwParam )
        {
        case 0: // view lunar calendar
            EQS_START_SIBLING(sSKA_ViewLunarCalendar, FALSE);
            break;

        case 1: /* Calendar Searching */
            EQS_START_CHILD(sSKA_SearchCalendar);
            break;

        case 2: /* My Holiday */
            EQS_START_CHILD(SKA_MainMyHoliday);
            break;

        case 3: /* Event List */
            EQS_START_CHILD_EVENT(SKA_MainSchedulerList, SE_APP_START, EQS_NA, GetCalendarTime());
            break;

        case 4: /* Delete Some */
            EQS_START_CHILD(SKA_DeleteSomeEvent);
            break;

        case 5: /* Alarm Bell Setting */
            EQS_START_CHILD(SKA_MainAlertBell);
            break;

        case 6: /* Anniversary Alarm Setting */
            EQS_START_CHILD(SKA_SetAnnivAlarm);
            break;
        }
        break;

    case SE_KEY_LEFT:   // it decrease a day.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_DecreaseADay();
        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_RIGHT:  // it increase a day.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_IncreaseADay();
        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_UP:     // it decrease a week.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_DecreaseAWeek();

        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_DOWN:   // it increase a week.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_IncreaseAWeek();
        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_STAR:
        Calinder_DecreaseAMonth();
        fDraw = TRUE;
        EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_KEY_POUND:
        Calinder_IncreaseAMonth();
        fDraw = TRUE;
        EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_KEY_RECORD: /* 음력 보기 */
        fDraw = TRUE;
        EQS_START_SIBLING(sSKA_ViewLunarCalendar, FALSE);
        break;

	case SE_KEY_MANNERS:  // 에티켓 설정을 하지 않는다.
		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

static VOID SKY_API sSKA_ViewLunarCalendar(SKY_EVENT_T* pEvent)
{
    static  BYTE    aszTitle[SCHL_MAX_SIZE_DAY_TITLE];  // day selected
	INT8    nIndex;

    static BOOL fDraw;
    static UINT8    nSaveMonth;


    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC( TRUE );

        /* display the window */
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y,
                        SKY_DEF_WIN_DX, SKY_DEF_WIN_DY, 
                        CL_MENU_CONFIRM_NEWSCHED, 
                        SKY_GET_TOKEN(TKN_BLANK),
                        WA_SHOW_CML | WA_SHOW_HEADER);

        /* draw calendar bar */
        fDraw = TRUE;
        break;

    case SE_APP_DRAW:
        if(fDraw)
        {
            cursor_X = cursor_Y = 0;
            BmpCursorOn_X = BmpCursorOn_Y = 0;
            BmpToday_X = BmpToday_Y = 0;
            EQS_ClearRegion(sOFFSET_X, sOFFSET_Y, sOFFSET_X+sDAY_BAR_WIDTH, sOFFSET_Y+sCALENDAR_HEIGHT);
            EQS_PutBitmap(sOFFSET_X, sOFFSET_Y, &g_BmpCalendar);
            fDraw = FALSE;
        }
        sDrawLunarCalendar();

        if( l_unLunarMonth > LEAP_MONTH ) /* leap month */
        {
            sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_LULAR_LEAP_CAL_TITLE), 
                    l_SetTime.wYear, (l_unLunarMonth - LEAP_MONTH), l_SetTime.nMonth, l_SetTime.nDay );            
        }
        else if( l_unLunarMonth > l_SetTime.nMonth ) // last year
        {
            sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_LUNAR_CAL_TITLE), 
                    (l_SetTime.wYear-1), l_unLunarMonth, l_SetTime.nMonth, l_SetTime.nDay );            
        }
        else // this year
        {
            sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_LUNAR_CAL_TITLE), 
                    l_SetTime.wYear, l_unLunarMonth, l_SetTime.nMonth, l_SetTime.nDay );            
        }

        EQS_SetSWindowHeaderStr(aszTitle);
        break;

    case SE_APP_RESUME:
        fDraw = TRUE;
        if( EQS_GET_CHILD() == SKA_MainSchedulerList )
        {
            if( pEvent->dwParam == SCHL_HAS_NO_EVENT )
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_NO_EVENT), 
                               MESSAGE_DEFAULT_TIME, 
                               MBT_NONE, 
                               MBT_HELP);
            }
        }
        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX *)paLunarCalendarMenu, MAX_SIZE_CALENDAR_MENU, 0);
        break;

    case SE_SOFTKEY_CONFIRM: // it searchs the schedule of that selected day.
		EQS_START_CHILD_EVENT( SKA_ViewADay, SE_APP_START, SMET_VIEW_LIST, GetCalendarTime() );
        break;

    case SE_SOFTKEY_ADD:        
        nIndex = GetNewIdxOfEventT(l_SetTime.nDay - 1);
        if(nIndex != SCHL_ERROR)
        {		
			if( GetNewIdxOfSchlT() != SCHL_ERROR &&\
				(GetMyEventNumOfBlockT(l_SetTime.nDay-1)< SCHL_MAX_MYEVNTS_ADAY) ) //최대 100개를 넘지 않았을 때 
			{
				EQS_START_CHILD_EVENT( SKA_ViewADay, SE_APP_START, SMET_NONE, GetCalendarTime() );
				break;
			}
        }

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_FULL_EVENT_DAY),
                       MESSAGE_DEFAULT_TIME,
                       MBT_NONE, 
                       MBT_HELP);
		
        break;

    case SE_IO_POPUP_CANCEL:
    case SE_IO_MESGBOX_END:
    case SE_IO_EXMSG_END:
        fDraw = TRUE;
        break;

    case SE_IO_POPUP_SELECT:
        switch( pEvent->dwParam )
        {
        case 0: // view lunar calendar
            EQS_START_SIBLING(sSKA_ViewSolarCalendar, FALSE);
            break;

        case 1: // calendar searching
            EQS_START_CHILD(sSKA_SearchCalendar);
            break;

        case 2: // My Holiday
            EQS_START_CHILD(SKA_MainMyHoliday);
            break;
        
        case 3: // Event List
            EQS_START_CHILD_EVENT( SKA_MainSchedulerList, SE_APP_START, EQS_NA, GetCalendarTime() );
            break;

        case 4: // Delete
            EQS_START_CHILD(SKA_DeleteSomeEvent);
            break;

        case 5: // alarm bell type
            EQS_START_CHILD(SKA_MainAlertBell);
            break;

        case 6: /* Anniversary Alarm Setting */
            EQS_START_CHILD(SKA_SetAnnivAlarm);
            break;
        }
        break;

    case SE_KEY_LEFT:   // it decrease a day.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_DecreaseADay();
        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_RIGHT:  // it increase a day.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_IncreaseADay();
        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_UP:     // it decrease a week.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_DecreaseAWeek();
        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_DOWN:   // it increase a week.
        nSaveMonth = l_SetTime.nMonth;
        Calinder_IncreaseAWeek();
        if(nSaveMonth != l_SetTime.nMonth)
        {
            fDraw = TRUE;
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);
            EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        break;

    case SE_KEY_STAR:
        Calinder_DecreaseAMonth();
        fDraw = TRUE;
        EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

	case SE_KEY_MANNERS:  // 에티켓 설정을 하지 않는다.
		break;

    case SE_KEY_POUND:
        Calinder_IncreaseAMonth();
        fDraw = TRUE;
        EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_KEY_RECORD: /* 양력 보기 */
        fDraw = TRUE;
        EQS_START_SIBLING(sSKA_ViewSolarCalendar, FALSE);
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL void sDrawCalCursorOn(SCHL_CAL_CURSOR_ON_E Cursor)
{
    switch(Cursor){
    case SCCO_NONE:
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)
        {
            EQS_FillRectangle(cursor_X+1, 
                              cursor_Y+1, 
                              cursor_X+1+REC_LARGE_WIDTH-1, //16
                              cursor_Y+1+REC_LARGE_HEIGHT-1, //14
                              WHITE);
        }
        else
#endif
        {
            EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareBlack);
            EQS_SetDrawMode(DM_OR);
            EQS_SetTextColor(WHITE);
        }
        break;

    case SCCO_HOLIDAY:
        EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareRed);
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(WHITE);
        break;

    case SCCO_EVENT:
    case SCCO_EVENT_HOLIDAY:
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)        
        {
            EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareWhite);
            EQS_FillRectangle(cursor_X+2,
                              cursor_Y+2, 
                              cursor_X+2+REC_SMALL_WIDTH-1, //14
                              cursor_Y+2+REC_SMALL_HEIGHT-1, //12
                              WHITE);
        }
        else
#endif
        {
            EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareBlackOn);
            EQS_SetDrawMode(DM_OR);
            EQS_SetTextColor(WHITE);
        }        
        break;

    case SCCO_TODAY:
        EQS_PutBitmap(BmpToday_X, BmpToday_Y, &g_BmpSquareThickBlacklineOn);
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(WHITE);
        break;
        
    case SCCO_TODAY_HOLIDAY:
        EQS_PutBitmap(BmpToday_X, BmpToday_Y, &g_BmpSquareBlacredOn);
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(WHITE);
        break;
    }
    return;
}

LOCAL void sDrawCalCursorOff(SCHL_CAL_CURSOR_ON_E Cursor)
{
    switch(Cursor){
    case SCCO_NONE:
        break;

    case SCCO_HOLIDAY:
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)
            EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareBorderBlack);
        else
#endif
        {
            EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareBorder);
            EQS_FillRectangle(cursor_X+1, 
                              cursor_Y+1, 
                              cursor_X+1+REC_LARGE_WIDTH-1, //16
                              cursor_Y+1+REC_LARGE_HEIGHT-1, //14
                              WHITE);
        }
        break;

    case SCCO_EVENT:
    case SCCO_EVENT_HOLIDAY:
        EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareWhite);
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)
        {   // 가운데 이미지가 흰색이기 때문에 배경이 검정색일 경우 검정색으로 칠해줘야 한다.
            EQS_FillRectangle(cursor_X+2,
                              cursor_Y+2, 
                              cursor_X+2+REC_SMALL_WIDTH-1, //14
                              cursor_Y+2+REC_SMALL_HEIGHT-1, //12
                              BLACK);
        }
#endif
        break;    

    case SCCO_TODAY:
        EQS_PutBitmap(BmpToday_X, BmpToday_Y, &g_BmpSquareThickBlackline);
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)
        {   // 가운데 이미지가 흰색이기 때문에 배경이 검정색일 경우 검정색으로 칠해줘야 한다.
            EQS_FillRectangle(BmpToday_X+2,
                              BmpToday_Y+2, 
                              BmpToday_X+2+REC_SMALL_WIDTH-1, //14
                              BmpToday_Y+2+REC_SMALL_HEIGHT-1, //12
                              BLACK);
        }
#endif
        break;

    case SCCO_TODAY_HOLIDAY:
        EQS_PutBitmap(BmpToday_X, BmpToday_Y, &g_BmpSquareThickBlackline);
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)
        {   // 가운데 이미지가 흰색이기 때문에 배경이 검정색일 경우 검정색으로 칠해줘야 한다.
            EQS_FillRectangle(BmpToday_X+2,
                              BmpToday_Y+2, 
                              BmpToday_X+2+REC_SMALL_WIDTH-1, //14
                              BmpToday_Y+2+REC_SMALL_HEIGHT-1, //12
                              BLACK);
        }
#endif
        break;
    }
    return;
}

LOCAL void sDrawCalCursor(void)
{
    switch(EQS_GetTextColor()){
    case RED:
        /* rosa 01-08-23 --> @why, what */
        //EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpCalCursor_red_15x12x256c);
        EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareRed);
        /* <-- */
        break;
    case BLUE:
        /* rosa 01-08-23 --> @why, what */
        //EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpCalCursor_blue_15x12x256c);
        EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareWhite);
        /* <-- */
        break;
    default:
        /* rosa 01-08-23 --> @why, what */
        //EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpCalCursor2_15x12x256c);
        EQS_PutBitmap(cursor_X, cursor_Y, &g_BmpSquareGray);
        /* <-- */
        break;
    }
}


static  VOID    sDrawSolarCalendar( void )
{
    INT8    i;
    INT8    nDayOfWeek;
    UINT8   unMaxDays;    
    BOOL    fHolidayOn = FALSE;
    COORD   nPosX, nPosY, nStartX, nStartY;
    COLOR   cOldText, cOldTextBack;
    BYTE    abHolidayTitle[MAX_NUM_HOLIDAY_TITLE+1], bTextDrawMode;
    static  SCHL_FIND_HOLIDAY_T FindHoliday, IsHoliday;    

    DRAW_MODE   oldDrawMode;
    BOOL    fDrawToday = FALSE;
    BOOL    fDrawMode;

    /* rosa 01-08-08 --> @why, what */
    FONT_TYPE ftOldFontType;
    SCHL_CAL_CURSOR_ON_E CursorMode;
    /* <-- */    

    unMaxDays = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
    nDayOfWeek = (((l_SetTime.nDayOfWeek + 1) % 7)  + (8 - (l_SetTime.nDay % 7))) % 7;
    
    /* set color, font */
    cOldTextBack = EQS_GetTextBackColor();
    EQS_SetTextBackColor(EQS_GetBackColor());
    ftOldFontType = EQS_GetFontType();

    /* draw days */
    nPosX = nStartX = sOFFSET_X;// + sDAY_BAR_COLUMN_SPACE; //4+1
    nPosY = nStartY = sOFFSET_Y + sDAY_BAR_HEIGHT; //3+11

    EQS_SetFontType(FT_SMALL);

    /* draw mode */    
    bTextDrawMode = EQS_GetTextDrawMode();
    oldDrawMode = EQS_GetDrawMode();

    /* 이전 커서위치를 지운다 */
    if(BmpCursorOn_X != 0 && BmpCursorOn_Y != 0)
    {
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)
        {
            EQS_FillRectangle(BmpCursorOn_X, 
                              BmpCursorOn_Y, 
                              BmpCursorOn_X+sDAY_BAR_COLUMN_WIDTH,//-1, //16
                              BmpCursorOn_Y+sDAY_BAR_COLUMN_HEIGHT,//-1, //14
                              BLACK);
            EQS_PutBitmap(BmpCursorOn_X, BmpCursorOn_Y, &g_BmpSquareBorderBlack);
        }
        else 
#endif
        {
            EQS_FillRectangle(BmpCursorOn_X, 
                              BmpCursorOn_Y, 
                              BmpCursorOn_X+sDAY_BAR_COLUMN_WIDTH,//-1, //16
                              BmpCursorOn_Y+sDAY_BAR_COLUMN_HEIGHT,//-1, //14
                              WHITE);
            EQS_PutBitmap(BmpCursorOn_X, BmpCursorOn_Y, &g_BmpSquareBorder);
        }
    }

    fDrawMode = TRUE;
    for( i=1; i<=unMaxDays; i++ )
    {
        CursorMode = SCCO_NONE;

        memset(abHolidayTitle, ' ', MAX_NUM_HOLIDAY_TITLE);
        abHolidayTitle[MAX_NUM_HOLIDAY_TITLE] = '\0';

        if(nDayOfWeek == 7)
        {
            nDayOfWeek = nDayOfWeek % 7;
            nPosY = nPosY + (sDAY_BAR_COLUMN_HEIGHT + sDAY_BAR_COLUMN_SPACE); //14+1
        }

        if(l_SetTime.nDay == i) //current cursor
        {
            cOldText = EQS_GetTextColor();
            nPosX = nStartX + (nDayOfWeek * (sDAY_BAR_COLUMN_WIDTH + sDAY_BAR_COLUMN_SPACE)); //16+1            
            if(nDayOfWeek == 0) //Sunday?
            {
                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }
            if(fHolidayOn)
            {
                if( FindHoliday.unDays )
                    FindHoliday.unDays--;
                else
                    fHolidayOn = FALSE;

                CheckHoliday( (UINT8)i, &IsHoliday ); //Holiday?                
                if( IsHoliday.Type == SHT_KOR )
                {
                    if( IsMiddleOfMultiByte(l_KorHolidayT[IsHoliday.nIdxOfTable].pszName, MAX_NUM_HOLIDAY_TITLE) )
                        STRNCPY(abHolidayTitle, 
                                l_KorHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE-1);
                    else
                        STRNCPY(abHolidayTitle, 
                                l_KorHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE);
                }
                else if( IsHoliday.Type == SHT_MY )
                {
                    if( IsMiddleOfMultiByte(g_MyHolidayT[IsHoliday.nIdxOfTable].pszName, MAX_NUM_HOLIDAY_TITLE) )
                        STRNCPY(abHolidayTitle, 
                                g_MyHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE-1);
                    else
                        STRNCPY(abHolidayTitle, 
                                g_MyHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE);
                }
                else
                {
                    STRNCPY(abHolidayTitle, 
                            (BYTE*)SKY_GET_TOKEN(TKN_BLANK), 
                            MAX_NUM_HOLIDAY_TITLE);
                }
                abHolidayTitle[MAX_NUM_HOLIDAY_TITLE] = '\0';

                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }
            else
            {
                CheckHoliday((UINT8)i, &FindHoliday); //Holiday?
                if(FindHoliday.Type) //SHT_KOR, SHT_MY
                {
                    fHolidayOn = TRUE;
                    i = i - ((FindHoliday.unDays/2)+1);
                    // last month?
                    if( i < 0 )
                    {
                        i = 0;
                        FindHoliday.unDays = FindHoliday.unDays - ((FindHoliday.unDays/2)+1);
                    }
                    else
                    {
                        FindHoliday.unDays--;
                    }
                    nDayOfWeek = nDayOfWeek - ((FindHoliday.unDays/2)+1);
                    if(nDayOfWeek < 0)
                    {
                        fDrawMode = FALSE;
                        nDayOfWeek = 7 + nDayOfWeek;
                        nPosY = nPosY - (sDAY_BAR_COLUMN_HEIGHT + sDAY_BAR_COLUMN_SPACE);
                    }
                }
            }

            if(fDrawMode)
            {
                /* Have Event? */
                if(GetEventNumOfBlockT(i-1))
                {                    
                    if(CursorMode == SCCO_HOLIDAY)
                        CursorMode = SCCO_EVENT_HOLIDAY;
                    else
                        //EQS_SetTextColor(BLUE);
                        CursorMode = SCCO_EVENT;
                }
                /* Is TODAY?   */
                if( (l_SetTime.wYear == l_cjToday.year) &&
                    (l_SetTime.nMonth == l_cjToday.month) &&
                    (i == l_cjToday.day) )
                {
                    if(!fDrawToday)  
                    {
                        if((CursorMode == SCCO_HOLIDAY)||(CursorMode == SCCO_EVENT_HOLIDAY))
                            CursorMode = SCCO_TODAY_HOLIDAY;
                        else
                            CursorMode = SCCO_TODAY;
                        fDrawToday = TRUE;
                        BmpToday_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                        BmpToday_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;
                    }
                } 
                /* draw */
                BmpCursorOn_X = cursor_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                BmpCursorOn_Y = cursor_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;                

                /* set Cursor Image */
                sDrawCalCursorOn(CursorMode);

                /* write the day */
#ifdef NEW_COLOR_SCHEME
                if(l_nIdxColorBack == CSB_BLACK)
                {
                    EQS_SetTextBackColor(WHITE);
                    EQS_SetTextColor(BLACK);
                    EQS_TextOutStrF((nPosX+sMARGIN_X), (nPosY+sMARGIN_Y), "%2d", i);
                    EQS_SetTextColor(WHITE);
                    EQS_SetTextBackColor(BLACK);
                    
                }
                else
#endif
                {
                    EQS_TextOutStrF((nPosX+sMARGIN_X), (nPosY+sMARGIN_Y), "%2d", i);
                }
            }
            else
            {
                fDrawMode = TRUE;
            }

            EQS_SetDrawMode(oldDrawMode);
            EQS_SetTextColor(cOldText);
            EQS_SetTextDrawMode(bTextDrawMode);

            if( i > (UINT8)FALSE )
            {
                EQS_SetTextAlignHoriz(TAH_RIGHT);

                /* rosa 01-08-08 --> @why, what */
                EQS_SetFontType(ftOldFontType);
                /* <-- */

                EQS_TextOutStrF(118,
                                (sOFFSET_Y+sCALENDAR_HEIGHT+sDAY_BAR_COLUMN_SPACE), 
                                (BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_CAL_HOLIDAY_EVENT),//"%s(%2d건)", 
                                abHolidayTitle, 
                                GetEventNumOfBlockT(i-1) );

                EQS_SetFontType(FT_SMALL);

                EQS_ClearTextAlign();
            }
        }
        else // 커서가 위치하지 않은 날짜 표시
        {
            nPosX = nStartX + (nDayOfWeek * (sDAY_BAR_COLUMN_WIDTH + sDAY_BAR_COLUMN_SPACE));
            cOldText = EQS_GetTextColor();  

            if( nDayOfWeek == 0 ) //Sunday?
            {
                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }

            if( fHolidayOn )
            {
                if( FindHoliday.unDays )
                    FindHoliday.unDays--;
                else
                    fHolidayOn = FALSE;

                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }
            else
            {
                CheckHoliday( (UINT8)i, &FindHoliday ); //Holiday?
                if( FindHoliday.Type )
                {
                    fHolidayOn = TRUE;
                    // last month?
                    i = i - ((FindHoliday.unDays/2)+1);
                    if( i < 0 )
                    {
                        i = 0;
                        FindHoliday.unDays = FindHoliday.unDays - ((FindHoliday.unDays/2)+1);
                    }
                    else
                    {
                        FindHoliday.unDays--;
                    }
                    nDayOfWeek = nDayOfWeek - ((FindHoliday.unDays/2)+1);
                    if(nDayOfWeek < 0)
                    {
                        fDrawMode = FALSE;
                        nDayOfWeek = 7 + nDayOfWeek;
                        nPosY = nPosY - (sDAY_BAR_COLUMN_HEIGHT + sDAY_BAR_COLUMN_SPACE);
                    }
                }
            }

            if(fDrawMode)
            {
                /* Have Event */
                if(GetEventNumOfBlockT(i-1))
                {
                    if(CursorMode == SCCO_HOLIDAY)
                        CursorMode = SCCO_EVENT_HOLIDAY;
                    else
                        //EQS_SetTextColor(BLUE);
                        CursorMode = SCCO_EVENT;
                }
                /* Is TODAY? */
                if( (l_SetTime.wYear == l_cjToday.year) &&
                    (l_SetTime.nMonth == l_cjToday.month) &&
                    (i == l_cjToday.day) )
                {
                    if(!fDrawToday)
                    {
                        if((CursorMode == SCCO_HOLIDAY)||(CursorMode == SCCO_EVENT_HOLIDAY))
                            CursorMode = SCCO_TODAY_HOLIDAY;
                        else
                            CursorMode = SCCO_TODAY;

                        fDrawToday = TRUE;
                        BmpToday_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                        BmpToday_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;
                    }
                }
                /* draw */
                cursor_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                cursor_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;

                /* set Cursor Image */
                sDrawCalCursorOff(CursorMode);

                /* write the day */
                EQS_TextOutStrF((nPosX+sMARGIN_X), (nPosY+sMARGIN_Y), "%2d", i);
            }
            else
            {
                fDrawMode = TRUE;
            }

            EQS_SetTextColor(cOldText);
            EQS_SetTextDrawMode(bTextDrawMode);
        }
        nDayOfWeek++;
    }
    
    /* rosa 01-08-08 --> @why, what */
    //EQS_SetFontType(FT_MED); // restore
    EQS_SetFontType(ftOldFontType);
    /* <-- */    
    return;
}


static  VOID    sDrawLunarCalendar( VOID )
{
    INT16   i; // for lunar day
    UINT8   unMaxDays;
    INT8    nDayOfWeek;
    COORD   nPosX, nPosY, nStartX, nStartY;
    COORD   unHeightOfChar;
    UINT8   unLunarMonth = 0;
    INT8    nLunarDay = -1;
    UINT16  unIdxOfNextMonth = 0;
    BOOL    fHolidayOn = FALSE;
    COLOR   cOldText, cOldTextBack;
    BYTE    abHolidayTitle[MAX_NUM_HOLIDAY_TITLE+1], bTextDrawMode;

    DRAW_MODE   oldDrawMode;
    BOOL    fDrawToday = FALSE;

    BOOL    fDrawMode;
    /* rosa 01-08-08 --> @why, what */
    FONT_TYPE ftOldFontType;
    SCHL_CAL_CURSOR_ON_E CursorMode;
    /* <-- */   

    static  SCHL_FIND_HOLIDAY_T FindHoliday, IsHoliday;

    /* draw days */
    unMaxDays = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
    nDayOfWeek = (((l_SetTime.nDayOfWeek + 1) % 7)  + (8 - (l_SetTime.nDay % 7))) % 7;
   
    /* set color, font */
    cOldTextBack = EQS_GetTextBackColor();
    EQS_SetTextBackColor(EQS_GetBackColor());
    ftOldFontType = EQS_GetFontType();

    /* draw days */
    nPosX = nStartX = sOFFSET_X;// + sDAY_BAR_COLUMN_SPACE;
    nPosY = nStartY = sOFFSET_Y + sDAY_BAR_HEIGHT;

    EQS_SetFontType(FT_SMALL);

    bTextDrawMode = EQS_GetTextDrawMode();
    oldDrawMode = EQS_GetDrawMode();

    /* 이전 커서위치를 지운다 */
    if(BmpCursorOn_X != 0 && BmpCursorOn_Y != 0)
    {
#ifdef NEW_COLOR_SCHEME
        if(l_nIdxColorBack == CSB_BLACK)
        {
            EQS_FillRectangle(BmpCursorOn_X, 
                              BmpCursorOn_Y, 
                              BmpCursorOn_X+sDAY_BAR_COLUMN_WIDTH,//-1, //16
                              BmpCursorOn_Y+sDAY_BAR_COLUMN_HEIGHT,//-1, //14
                              BLACK);
            EQS_PutBitmap(BmpCursorOn_X, BmpCursorOn_Y, &g_BmpSquareBorderBlack);
        }
        else 
#endif
        {
            EQS_FillRectangle(BmpCursorOn_X, 
                              BmpCursorOn_Y, 
                              BmpCursorOn_X+sDAY_BAR_COLUMN_WIDTH,//-1, //16
                              BmpCursorOn_Y+sDAY_BAR_COLUMN_HEIGHT,//-1, //14
                              WHITE);
            EQS_PutBitmap(BmpCursorOn_X, BmpCursorOn_Y, &g_BmpSquareBorder);
        }
    }

    for( i=0; i<MAX_COUNT_LUNAR_DAY; i++ )
    {
        if( ( l_SetTime.wYear == l_LunarDayT[i].unSYear ) &&
            ( l_SetTime.nMonth == l_LunarDayT[i].unSMonth ) &&
            ( 1 == l_LunarDayT[i].unSDay ) )
        {
            unLunarMonth = l_LunarDayT[i].unLMonth;
            nLunarDay = l_LunarDayT[i].unLDay;
            unIdxOfNextMonth = i+1;
            break;
        }
    }

    fDrawMode = TRUE;
    for( i=1; i<=unMaxDays; i++ )
    {
        CursorMode = SCCO_NONE;

        memset(abHolidayTitle, ' ', MAX_NUM_HOLIDAY_TITLE);
        abHolidayTitle[MAX_NUM_HOLIDAY_TITLE] = '\0';

        if( nDayOfWeek == 7 )
        {
            nDayOfWeek = nDayOfWeek % 7;            
            nPosY = nPosY + (sDAY_BAR_COLUMN_HEIGHT + sDAY_BAR_COLUMN_SPACE);
        }

        if( ( l_SetTime.nMonth == l_LunarDayT[unIdxOfNextMonth].unSMonth ) &&
            ( i == l_LunarDayT[unIdxOfNextMonth].unSDay ) )
        {
            unLunarMonth = l_LunarDayT[unIdxOfNextMonth].unLMonth;
            nLunarDay = l_LunarDayT[unIdxOfNextMonth].unLDay;
        }

        if( l_SetTime.nDay == i ) //current cursor
        {            
            nPosX = nStartX + (nDayOfWeek * (sDAY_BAR_COLUMN_WIDTH + sDAY_BAR_COLUMN_SPACE));            

            cOldText = EQS_GetTextColor();
            if( nDayOfWeek == 0 ) //Sunday?
            {
                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }
            if( fHolidayOn )
            {
                if( FindHoliday.unDays )
                {
                    FindHoliday.unDays--;
                }
                else
                {
                    fHolidayOn = FALSE;
                }
                CheckHoliday( (UINT8)i, &IsHoliday ); //Holiday?                
                if( IsHoliday.Type == SHT_KOR )
                {
                    if( IsMiddleOfMultiByte(l_KorHolidayT[IsHoliday.nIdxOfTable].pszName, MAX_NUM_HOLIDAY_TITLE) )
                    {
                        STRNCPY(abHolidayTitle, 
                                l_KorHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE-1);
                    }
                    else
                    {
                        STRNCPY(abHolidayTitle, 
                                l_KorHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE);                        
                    }                    
                }
                else if( IsHoliday.Type == SHT_MY )
                {
                    if( IsMiddleOfMultiByte(g_MyHolidayT[IsHoliday.nIdxOfTable].pszName, MAX_NUM_HOLIDAY_TITLE) )
                    {
                        STRNCPY(abHolidayTitle, 
                                g_MyHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE-1);
                    }
                    else
                    {
                        STRNCPY(abHolidayTitle, 
                                g_MyHolidayT[IsHoliday.nIdxOfTable].pszName, 
                                MAX_NUM_HOLIDAY_TITLE);
                    }
                }
                else
                {
                    STRNCPY(abHolidayTitle, 
                            (BYTE*)SKY_GET_TOKEN(TKN_BLANK), 
                            MAX_NUM_HOLIDAY_TITLE);
                }
                abHolidayTitle[MAX_NUM_HOLIDAY_TITLE] = '\0';

                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }
            else
            {
                CheckHoliday( (UINT8)i, &FindHoliday ); //Holiday?
                if( FindHoliday.Type )
                {
                    fHolidayOn = TRUE;
                    // check last month
                    i = i - ((FindHoliday.unDays/2)+1);
                    if( i < 0 )
                    {
                        i = 0;
                        FindHoliday.unDays = FindHoliday.unDays - ((FindHoliday.unDays/2)+1);
                    }
                    else
                    {
                        FindHoliday.unDays--;
                    }
                    nDayOfWeek = nDayOfWeek - ((FindHoliday.unDays/2)+1);
                    //nDayOfWeek = (nDayOfWeek < -1) ? (7 + nDayOfWeek) : nDayOfWeek;
                    if(nDayOfWeek < 0)
                    {
                        fDrawMode = FALSE;
                        nDayOfWeek = 7 + nDayOfWeek;
                        nPosY = nPosY - unHeightOfChar;
                    }

                    nLunarDay = nLunarDay - ((FindHoliday.unDays/2)+1);
                    if(nLunarDay < 0)
                    {
                        nLunarDay = l_LunarDayT[unIdxOfNextMonth-1].unLDay
                            + (l_LunarDayT[unIdxOfNextMonth].unSDay - l_LunarDayT[unIdxOfNextMonth-1].unSDay)
                            - ((FindHoliday.unDays/2)+1);
                    }
                }
            }
            if(fDrawMode)
            {
                if(GetEventNumOfBlockT(i-1))
                {
                    if(CursorMode == SCCO_HOLIDAY)
                        CursorMode = SCCO_EVENT_HOLIDAY;
                    else
                        //EQS_SetTextColor(BLUE);
                        CursorMode = SCCO_EVENT;
                }
                /* Is TODAY?   */
                if( (l_SetTime.wYear == l_cjToday.year) &&
                    (l_SetTime.nMonth == l_cjToday.month) &&
                    (i == l_cjToday.day) )
                {
                    if(!fDrawToday)
                    {
                        if((CursorMode == SCCO_HOLIDAY)||(CursorMode == SCCO_EVENT_HOLIDAY))
                            CursorMode = SCCO_TODAY_HOLIDAY;
                        else
                            CursorMode = SCCO_TODAY;
                        fDrawToday = TRUE;
                        BmpToday_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                        BmpToday_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;
                    }
                }

                /* Draw */
                BmpCursorOn_X = cursor_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                BmpCursorOn_Y = cursor_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;                

                sDrawCalCursorOn(CursorMode);

                /* write the day */
#ifdef NEW_COLOR_SCHEME
                if(l_nIdxColorBack == CSB_BLACK)
                {
                    EQS_SetTextBackColor(WHITE);
                    EQS_SetTextColor(BLACK);
                    EQS_TextOutStrF((nPosX+sMARGIN_X), (nPosY+sMARGIN_Y), "%2d", nLunarDay);
                    EQS_SetTextColor(WHITE);
                    EQS_SetTextBackColor(BLACK);
                    
                }
                else
#endif
                {
                    EQS_TextOutStrF((nPosX+sMARGIN_X), (nPosY+sMARGIN_Y), "%2d", nLunarDay);
                }
            }
            else
            {
                fDrawMode = TRUE;
            }

            EQS_SetTextColor(cOldText);
            EQS_SetTextDrawMode(bTextDrawMode);
            EQS_SetDrawMode(oldDrawMode);

            if(i > (UINT8)FALSE)
            {
                EQS_SetTextAlignHoriz(TAH_RIGHT);
                /* rosa 01-08-08 --> @why, what */
                //EQS_SetFontType(FT_MED); // restore
                EQS_SetFontType(ftOldFontType);
                /* <-- */
                EQS_TextOutStrF(118, 
                                (sOFFSET_Y+sCALENDAR_HEIGHT+sDAY_BAR_COLUMN_SPACE),
                                (BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_CAL_HOLIDAY_EVENT),//"%s(%2d건)", 
                                abHolidayTitle, 
                                GetEventNumOfBlockT(i-1));

                EQS_SetFontType(FT_SMALL);
                EQS_ClearTextAlign();
            }
            l_unLunarMonth = unLunarMonth;
            l_unLunarDay = nLunarDay;
        }
        else
        {
            nPosX = nStartX + (nDayOfWeek * (sDAY_BAR_COLUMN_WIDTH + sDAY_BAR_COLUMN_SPACE));

            cOldText = EQS_GetTextColor();            
            if( nDayOfWeek == 0 ) //Sunday?
            {
                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }
            if( fHolidayOn )
            {
                if( FindHoliday.unDays )
                {
                    FindHoliday.unDays--;
                }
                else
                {
                    fHolidayOn = FALSE;
                }
                EQS_SetTextColor(RED);
                CursorMode = SCCO_HOLIDAY;
            }
            else
            {
                CheckHoliday( (UINT8)i, &FindHoliday ); //Holiday?
                if( FindHoliday.Type )
                {
                    fHolidayOn = TRUE;
                    // check last month
                    i = i - ((FindHoliday.unDays/2)+1);
                    if( i < 0 )
                    {
                        i = 0;
                        FindHoliday.unDays = FindHoliday.unDays - ((FindHoliday.unDays/2)+1);
                    }
                    else
                    {
                        FindHoliday.unDays--;
                    }
                    nDayOfWeek = nDayOfWeek - ((FindHoliday.unDays/2)+1);
                    //nDayOfWeek = (nDayOfWeek < -1) ? (7 + nDayOfWeek) : nDayOfWeek; 
                    if(nDayOfWeek < 0)
                    {
                        fDrawMode = FALSE;
                        nDayOfWeek = 7 + nDayOfWeek;
                        //nPosY = nPosY - unHeightOfChar;
                        nPosY = nPosY - (sDAY_BAR_COLUMN_HEIGHT + sDAY_BAR_COLUMN_SPACE);
                    }

                    nLunarDay = nLunarDay - ((FindHoliday.unDays/2)+1);
                    if(nLunarDay < 0)
                    {
                        nLunarDay = l_LunarDayT[unIdxOfNextMonth-1].unLDay
                            + (l_LunarDayT[unIdxOfNextMonth].unSDay - l_LunarDayT[unIdxOfNextMonth-1].unSDay)
                            - ((FindHoliday.unDays/2)+1);
                    }
                }
            }
            if(fDrawMode)
            {
                /* Have EVENT? */
                if(GetEventNumOfBlockT(i-1))
                {
                    if(CursorMode == SCCO_HOLIDAY)
                        CursorMode = SCCO_EVENT_HOLIDAY;
                    else
                        //EQS_SetTextColor(BLUE);
                        CursorMode = SCCO_EVENT;
                }
                /* Is TODAY? */
                if( (l_SetTime.wYear == l_cjToday.year) &&
                    (l_SetTime.nMonth == l_cjToday.month) &&
                    (i == l_cjToday.day) )
                {
                    if(!fDrawToday)
                    {
                        /*
                        cursor_X1 = nPosX+sMARGIN_X-1;
                        cursor_Y1 = nPosY-1;
                        EQS_PutBitmap(cursor_X1, cursor_Y1, &g_BmpCalCursor1_15x12x256c);
                        fDrawToday = TRUE;
                        */

                        if((CursorMode == SCCO_HOLIDAY)||(CursorMode == SCCO_EVENT_HOLIDAY))
                            CursorMode = SCCO_TODAY_HOLIDAY;
                        else
                            CursorMode = SCCO_TODAY;
                        fDrawToday = TRUE;
                        BmpToday_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                        BmpToday_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;
                    }
                }
                /* Draw */
                cursor_X = nPosX;// + sDAY_BAR_COLUMN_SPACE;
                cursor_Y = nPosY;// + sDAY_BAR_COLUMN_SPACE;

                sDrawCalCursorOff(CursorMode);

                /* Draw */
                EQS_TextOutStrF( (nPosX+sMARGIN_X), (nPosY+sMARGIN_Y), "%2d", nLunarDay );
            }
            else
            {
                fDrawMode = TRUE;
            }

            EQS_SetTextColor(cOldText);
            EQS_SetTextDrawMode(bTextDrawMode);
        }
        nLunarDay++;
        nDayOfWeek++;
    }
    /* rosa 01-08-08 --> @why, what */
    //EQS_SetFontType(FT_MED);
    EQS_SetFontType(ftOldFontType);
    /* <-- */
    return;
}

VOID    Calinder_DecreaseADay( VOID )
{
    DWORD   dwCalenderTime;

    l_SetTime.nDay = l_SetTime.nDay - 1;
    if( l_SetTime.nDayOfWeek == 0 ) // it changes a day of the week.
    {
        l_SetTime.nDayOfWeek = 7;
    }
    l_SetTime.nDayOfWeek--;
    l_SetTime.nDayOfWeek = l_SetTime.nDayOfWeek % 7;    

    if( l_SetTime.nDay < 1 )
    {
        l_SetTime.nMonth = l_SetTime.nMonth - 1;
        if( l_SetTime.nMonth < 1 )
        {
            l_SetTime.wYear = l_SetTime.wYear - 1;
            if( l_SetTime.wYear < 1996 )
            {
                l_SetTime.wYear = 1996;
                l_SetTime.nMonth = 1;
            }
            else
            {
                l_SetTime.nMonth = SCHL_MONTHS_AYEAR;
            }
        }
        l_SetTime.nDay = (INT)GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );

        dwCalenderTime = GetCalendarTime();
        MakeSchedule( dwCalenderTime );    // it makes the event table for the schedule on a month.
    }
    return;
}

VOID    Calinder_IncreaseADay( VOID )
{
    DWORD   dwCalenderTime;
    UINT8   nMaxDay;

    l_SetTime.nDay = l_SetTime.nDay + 1;

    l_SetTime.nDayOfWeek++; // it changes a day of the week.
    l_SetTime.nDayOfWeek = l_SetTime.nDayOfWeek % 7;

    nMaxDay = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
    if( l_SetTime.nDay > nMaxDay )
    {
        l_SetTime.nDay = l_SetTime.nDay - nMaxDay;
        l_SetTime.nMonth = l_SetTime.nMonth + 1;
        if( l_SetTime.nMonth > SCHL_MONTHS_AYEAR )
        {
            l_SetTime.wYear = l_SetTime.wYear + 1;
            if( l_SetTime.wYear > 2015 )
            {
                l_SetTime.wYear = 2015;
                l_SetTime.nMonth = 12;
            }
            else
            {
                l_SetTime.nMonth = l_SetTime.nMonth - SCHL_MONTHS_AYEAR;
            }            
        }

        dwCalenderTime = GetCalendarTime();
        MakeSchedule( dwCalenderTime );    // it makes the event table for the schedule on a month.
    }
    return;
}

VOID Calinder_DecreaseAWeek(VOID)
{
    DWORD   dwCalenderTime;

    l_SetTime.nDay = l_SetTime.nDay - SCHL_DAYS_AWEEK;
    if( l_SetTime.nDay < 1 )
    {
        l_SetTime.nMonth = l_SetTime.nMonth - 1;
        if( l_SetTime.nMonth < 1 )
        {
            l_SetTime.wYear = l_SetTime.wYear - 1;
            if( l_SetTime.wYear < 1996 )
            {
                l_SetTime.wYear = 1996;
                l_SetTime.nMonth = 1;
            }   
            else
            {
                l_SetTime.nMonth = SCHL_MONTHS_AYEAR;
            }            
        }
        l_SetTime.nDay = l_SetTime.nDay + (INT)GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );

        dwCalenderTime = GetCalendarTime();
        MakeSchedule( dwCalenderTime );    // it makes the event table for the schedule on a month.
    }    
    return;
}

VOID Calinder_IncreaseAWeek(VOID)
{
    DWORD   dwCalenderTime;
    UINT8   nMaxDay;

    l_SetTime.nDay = l_SetTime.nDay + SCHL_DAYS_AWEEK;
    nMaxDay = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
    if( l_SetTime.nDay > nMaxDay )
    {
        l_SetTime.nDay = l_SetTime.nDay - nMaxDay;
        l_SetTime.nMonth = l_SetTime.nMonth + 1;
        if( l_SetTime.nMonth > SCHL_MONTHS_AYEAR )
        {
            l_SetTime.wYear = l_SetTime.wYear + 1;
            if( l_SetTime.wYear > 2015 )
            {
                l_SetTime.wYear = 2015;
                l_SetTime.nMonth = SCHL_MONTHS_AYEAR;
            }
            else
            {
                l_SetTime.nMonth = l_SetTime.nMonth - SCHL_MONTHS_AYEAR;
            }            
        }

        dwCalenderTime = GetCalendarTime();
        MakeSchedule( dwCalenderTime );    // it makes the event table for the schedule on a month.
    }
    return;
}

VOID Calinder_DecreaseAMonth(VOID)
{
    DWORD   dwCalenderTime;

    if(l_SetTime.nMonth == 1)
        if(l_SetTime.wYear == 1996)
            return;
        else
        {
            l_SetTime.nMonth = 12;
            l_SetTime.wYear--;
        }
    else
        l_SetTime.nMonth--;

    dwCalenderTime = GetCalendarTime();
    MakeSchedule(dwCalenderTime);    // it makes the event table for the schedule on a month.
    return;
}

VOID Calinder_IncreaseAMonth(VOID)
{
    DWORD   dwCalenderTime;

    if(l_SetTime.nMonth == 12)
        if(l_SetTime.wYear == 2015)
            return;
        else
        {
            l_SetTime.nMonth = 1;
            l_SetTime.wYear++;
        }
    else
        l_SetTime.nMonth++;

    dwCalenderTime = GetCalendarTime();
    MakeSchedule(dwCalenderTime);    // it makes the event table for the schedule on a month.
    return;
}

DWORD   GetCalendarTime( VOID )
{
    clk_julian_type cjSetTime;

    cjSetTime.year = l_SetTime.wYear;
    cjSetTime.month = l_SetTime.nMonth;
    cjSetTime.day = l_SetTime.nDay;
    cjSetTime.day_of_week = l_SetTime.nDayOfWeek;
    cjSetTime.hour = l_SetTime.nHour;
    cjSetTime.minute = l_SetTime.nMinute;
    cjSetTime.second = l_SetTime.nSecond;

    return  clk_julian_to_secs( &cjSetTime );
    
}

VOID    SetCalendarTime( DWORD  dwOneSetTime )
{
    clk_julian_type cjSetTime;

    clk_secs_to_julian( dwOneSetTime, &cjSetTime );    
    
    // initialize the l_SetTime structure.
    l_SetTime.wYear = (WORD)cjSetTime.year;
    l_SetTime.nMonth = (INT8)cjSetTime.month;
    l_SetTime.nDay = (INT)cjSetTime.day;
    l_SetTime.nDayOfWeek = (INT8)cjSetTime.day_of_week;
    l_SetTime.nHour = (INT8)cjSetTime.hour;
    l_SetTime.nMinute = (INT8)cjSetTime.minute;
    l_SetTime.nSecond = (INT8)cjSetTime.second;
}

VOID    GetSolarDay( UINT8* punLMonth, UINT8*  punLDay, UINT16*  pnPosSearch, SCHL_FIND_SOLAR_DAY_T* pResult )
{
    UINT16  i,j;
    DWORD   dwSetTime;
    clk_julian_type cjSetTime;

    for( i=*pnPosSearch; i<MAX_COUNT_LUNAR_DAY; i++ )
    {
        if( l_LunarDayT[i].unSYear == l_SetTime.wYear )
        {
            break;
        }
    }

    if( *pnPosSearch > 0 )
    {
        for( ; i<MAX_COUNT_LUNAR_DAY; i++ )
        {
            if( l_LunarDayT[i].unLMonth == 1 )
            {
                break;
            }
        }
    }

    if( *punLMonth >= 11 )
    {
        for( j=i; j<MAX_COUNT_LUNAR_DAY; j++ )
        {
            if( l_LunarDayT[j].unLMonth == *punLMonth )
            {
                break;
            }
        }

        for( ; j<MAX_COUNT_LUNAR_DAY; j++ )
        {
            if(( l_LunarDayT[j+1].unLMonth == *punLMonth ) && ( l_LunarDayT[j+1].unLDay <= *punLDay ))
            {
                continue;
            }
            else
            {
                break;
            }
        }

	    if (j < MAX_COUNT_LUNAR_DAY - 1)
	    {
    		cjSetTime.year = (WORD)l_LunarDayT[j].unSYear;
	    	cjSetTime.month = (WORD)l_LunarDayT[j].unSMonth;
		    cjSetTime.day = (WORD)l_LunarDayT[j].unSDay;
            // dummy
    		cjSetTime.hour = 0; 
	    	cjSetTime.minute = 0;
		    cjSetTime.second = 0;
    		cjSetTime.day_of_week = 0;

    		dwSetTime = clk_julian_to_secs(&cjSetTime);
	    	dwSetTime += TIME_SECS_1_DAY*(*punLDay - l_LunarDayT[j].unLDay);

    		clk_secs_to_julian(dwSetTime, &cjSetTime);

            if( cjSetTime.year == l_SetTime.wYear )
            {
                pResult->fFind = TRUE;
                pResult->nIdxOfTable = j;
                pResult->unSMonth = (UINT8)cjSetTime.month;
                pResult->unSDay = (UINT8)cjSetTime.day;
    		    return;
            }
            else
            {
                for( ; j<MAX_COUNT_LUNAR_DAY; j++ )
                {
                    if( l_LunarDayT[j].unLMonth == 1 )
                    {
                        break;
                    }
                }
                i=j;
            }

	    }
    }


    for( ; i<MAX_COUNT_LUNAR_DAY; i++ )
    {
        if( l_LunarDayT[i].unLMonth == *punLMonth )
        {
            break;
        }
    }

    for( ; i<MAX_COUNT_LUNAR_DAY; i++ )
    {
        if(( l_LunarDayT[i+1].unLMonth == *punLMonth ) && ( l_LunarDayT[i+1].unLDay <= *punLDay ))
        {
            continue;
        }
        else
        {
            break;
        }
    }

	if ( ( l_LunarDayT[i].unSYear == l_SetTime.wYear ) && ( i < MAX_COUNT_LUNAR_DAY - 1 ) )
	{
		cjSetTime.year = (WORD)l_LunarDayT[i].unSYear;
		cjSetTime.month = (WORD)l_LunarDayT[i].unSMonth;
		cjSetTime.day = (WORD)l_LunarDayT[i].unSDay;
        // dummy
		cjSetTime.hour = 0;
		cjSetTime.minute = 0;
		cjSetTime.second = 0;
		cjSetTime.day_of_week = 0;

		dwSetTime = clk_julian_to_secs(&cjSetTime);
		dwSetTime += TIME_SECS_1_DAY*(*punLDay - l_LunarDayT[i].unLDay);

		clk_secs_to_julian(dwSetTime, &cjSetTime);

        pResult->fFind = TRUE;
        pResult->nIdxOfTable = i;
        pResult->unSMonth = (UINT8)cjSetTime.month;
        pResult->unSDay = (UINT8)cjSetTime.day;
		return;
	}
	else
	{
        pResult->fFind = FALSE;
        pResult->nIdxOfTable = SCHL_ERROR;
        pResult->unSMonth = 0;
        pResult->unSDay = 0;
		return;
	}
}


VOID    CheckHoliday( UINT8 unDay, SCHL_FIND_HOLIDAY_T* pFindHoliday )
{
    UINT8   i;
    static  UINT16      nIdxOfSearch;
    static  SCHL_FIND_SOLAR_DAY_T FindSolarDay;

    // Korean Holiday
    for( i=0; i<MAX_COUNT_KOR_HOLIDAY; i++ )
    {
        if( l_KorHolidayT[i].fLunar ) //lunar
        {
            nIdxOfSearch = 0;
            GetSolarDay( &(l_KorHolidayT[i].unMonth), &(l_KorHolidayT[i].unDay), &nIdxOfSearch, &FindSolarDay );
            while( FindSolarDay.fFind )
            {
                if(( FindSolarDay.unSMonth == l_SetTime.nMonth ) &&
                    ( FindSolarDay.unSDay == unDay ))
                {
                    pFindHoliday->Type = SHT_KOR;
                    pFindHoliday->nIdxOfTable = i;
                    if( l_KorHolidayT[i].fConsecutive )
                    {
                        pFindHoliday->unDays = 3;
                    }
                    else
                    {
                        pFindHoliday->unDays = 1;
                    }                    
                    return;
                }
                nIdxOfSearch = FindSolarDay.nIdxOfTable + 1; // next search
                GetSolarDay( &l_KorHolidayT[i].unMonth, &l_KorHolidayT[i].unDay, &nIdxOfSearch, &FindSolarDay );
            }
        }
        else //solar
        {
            if(( l_KorHolidayT[i].unMonth == l_SetTime.nMonth ) &&
                ( l_KorHolidayT[i].unDay == unDay ))
            {
                pFindHoliday->Type = SHT_KOR;
                pFindHoliday->nIdxOfTable = i;
                if( l_KorHolidayT[i].fConsecutive )
                {
                    pFindHoliday->unDays = 3;
                }
                else
                {
                    pFindHoliday->unDays = 1;
                }
                return;
            }
        }
    }

    // My Holiday
    for( i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++ )
    {
        if( g_MyHolidayT[i].Type == SHT_MY )
        {
            if( ( g_MyHolidayT[i].wYear == l_SetTime.wYear ) &&
                ( g_MyHolidayT[i].unMonth == l_SetTime.nMonth ) &&
                ( g_MyHolidayT[i].unDay == unDay ) )
            {
                pFindHoliday->Type = SHT_MY;
                pFindHoliday->nIdxOfTable = i;
                pFindHoliday->unDays = 1;
                return;
            }
        }
    }
    pFindHoliday->Type = SHT_NONE;
    pFindHoliday->nIdxOfTable = SCHL_ERROR;
    pFindHoliday->unDays = 0;
    return;
}

static  VOID    sGetLunarDay( UINT8* unNewLunarMonth, UINT8* unNewLunarDay )
{
    INT16   i; // for lunar day
    UINT8   unMaxDays;
    UINT8   unLunarMonth = 0;
    INT8    nLunarDay = -1;
    UINT16  unIdxOfNextMonth = 0;
    //static  SCHL_FIND_HOLIDAY_T FindHoliday, IsHoliday;
    //BOOL    fHolidayOn = FALSE;


    for( i=0; i<MAX_COUNT_LUNAR_DAY; i++ )
    {
        if( ( l_SetTime.wYear == l_LunarDayT[i].unSYear ) &&
            ( l_SetTime.nMonth == l_LunarDayT[i].unSMonth ) &&
            ( 1 == l_LunarDayT[i].unSDay ) )
        {
            unLunarMonth = l_LunarDayT[i].unLMonth;
            nLunarDay = l_LunarDayT[i].unLDay;
            unIdxOfNextMonth = i+1;
            break;
        }
    }

    unMaxDays = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
    for( i=1; i<=unMaxDays; i++ )
    {
        if( ( l_SetTime.nMonth == l_LunarDayT[unIdxOfNextMonth].unSMonth ) &&
            ( i == l_LunarDayT[unIdxOfNextMonth].unSDay ) )
        {
            unLunarMonth = l_LunarDayT[unIdxOfNextMonth].unLMonth;
            nLunarDay = l_LunarDayT[unIdxOfNextMonth].unLDay;
        }

        if( l_SetTime.nDay == i ) //current cursor
        {            
            *unNewLunarMonth = unLunarMonth;
            *unNewLunarDay = nLunarDay;
            return;
        }
        nLunarDay++;
    }
}

static  VOID    SKY_API sSKA_SearchCalendar(SKY_EVENT_T *pEvent)
{
    DWORD   dwCalenderTime;
    UINT8   unCtrl;
	clk_julian_type	cjDefaultTime;
	HCONTROL hControl;

    SCHL_TIME_SET_T SearchTime;
    static  UINT8   unMaxDays;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);

        // display the window
        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y,
                        SKY_DEF_WIN_DX,
                        SKY_DEF_WIN_DY, 
                        CL_NONE_NONE_CANCEL,
                        SKY_GET_TOKEN(TKN_SCHEDULE_SEARCH_CALENDAR_TITLE), 
                        WA_SHOW_CML | WA_SHOW_HEADER);
     
        /*
        ** ***************************************************************************
        ** CREATE
        ** ***************************************************************************
        */
        EQC_Image(47, 37, &g_BmpSearchCalendar); //SSC_SEARCH_CAL_IMAGE
        hControl = EQC_Time( 30, 66, 0, YEAR_MONTH_CONTROL ); //SSC_SEARCH_CAL_TIME
        EQC_Image(4, 91, &g_BmpHorizontalLine); //SSC_SEARCH_CAL_LINE
        /* 화면 중앙에 표시하기 위해 좌표에 직접 찍었음 */
		EQC_StaticText( 15, 99, TKN_SCHEDULE_SEARCH_CALENDAR_RANGE ); //SSC_SEARCH_CAL_RANGE_TEXT     
        
        /*
        ** ***************************************************************************
        ** SET THE VALUE
        ** ***************************************************************************
        */
		clk_secs_to_julian(SKY_GetLocalTime(), &cjDefaultTime);
        EQC_SetTimeYearLimit(EQS_GetNthIOC(SSC_SEARCH_CAL_TIME), SCHL_START_YEAR, SCHL_END_YEAR);
        EQC_SetTimeYear(EQS_GetNthIOC(SSC_SEARCH_CAL_TIME), cjDefaultTime.year);
        EQC_SetTimeMonth(EQS_GetNthIOC(SSC_SEARCH_CAL_TIME), cjDefaultTime.month);

        unMaxDays = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
		EQS_SetIOCActive(hControl);

        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;
	case SE_SOFTKEY_CANCEL:
		EQS_END();
		break;
    case SE_IO_TIME_SELECT:
    case SE_SOFTKEY_FIND:
        SearchTime.wYear = EQC_GetTimeYear( EQS_GetNthIOC(SSC_SEARCH_CAL_TIME) );
        SearchTime.nMonth = EQC_GetTimeMonth( EQS_GetNthIOC(SSC_SEARCH_CAL_TIME) );

        if(( SearchTime.wYear != l_SetTime.wYear ) || ( SearchTime.nMonth != l_SetTime.nMonth ))
        {
            if(( SearchTime.wYear < l_SetTime.wYear ) || 
                (( SearchTime.wYear == l_SetTime.wYear ) && ( SearchTime.nMonth < l_SetTime.nMonth ))) //past time
            {
                while(( SearchTime.wYear != l_SetTime.wYear ) || ( SearchTime.nMonth != l_SetTime.nMonth ))
                {
                    if( l_SetTime.nMonth > 1 )
                    {
                        l_SetTime.nMonth--;
                    }
                    else
                    {
                        l_SetTime.wYear--;
                        l_SetTime.nMonth = 12;
                    }
                    unMaxDays = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
                    l_SetTime.nDayOfWeek = ( l_SetTime.nDayOfWeek + ( 7 - unMaxDays%7) )%7;

                }
            }
            else // next time
            {
                while(( SearchTime.wYear != l_SetTime.wYear ) || ( SearchTime.nMonth != l_SetTime.nMonth ))
                {
                    if( l_SetTime.nMonth < 12 )
                    {
                        l_SetTime.nMonth++;
                    }
                    else
                    {
                        l_SetTime.nMonth = 1;
                        l_SetTime.wYear++;
                    }

                    l_SetTime.nDayOfWeek = ( l_SetTime.nDayOfWeek + (unMaxDays%7) )%7;           
                    unMaxDays = GetDaysOfMonth( l_SetTime.wYear, l_SetTime.nMonth );
                }
            }
            dwCalenderTime = GetCalendarTime();
            MakeSchedule( dwCalenderTime );    // it makes the event table for the schedule on a month.
        }
        EQS_END();
        break;

    case EQE_IO_TIME_CANCEL:  
		EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC( pEvent );
        break;
    }    
}

void Set_Schedule_Time(SCHL_TIME_SET_T * CurrentTime)
{
	l_SetTime = *CurrentTime;
}

void Get_Schedule_Time(SCHL_TIME_SET_T * CurrentTime)
{
	 *CurrentTime = l_SetTime;
}

