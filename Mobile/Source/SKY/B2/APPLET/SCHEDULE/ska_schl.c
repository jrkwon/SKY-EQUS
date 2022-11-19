/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_schl.c
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
** 00-12-07   rosa       Created.
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
#define MAX_NUM_ANNIV_ITEM_NAME             MAX_BOOK_NAME
//#define MAX_SIZE_VIEW_LINE		            20
#define MAX_SIZE_VIEW_LINE		            (EQS_GetMaxX()+1)/EQS_CharWidth((BYTE)' ') //18
#define MAX_SIZE_VIEW_EVENT_DATA            10
#define DIFF_SECS_EVENT_TIME_START_N_END    TIME_SECS_10_MIN
#define DIFF_MINS_EVENT_TIME_START_N_END    10
#define MOON_TIME                           12

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagSCHL_EVENT_DAY_TIME_E {
    SEDT_SOME_TIME,
    SEDT_ALL_DAY,
    SEDT_MAX
};
typedef enum tagSCHL_EVENT_DAY_TIME_E SCHL_EVENT_DAY_TIME_E;

enum tagSCHL_USER_DEFINED_TIME_E {
    SUDT_USER_DEFINED_TIME_TEXT,
    SUDT_USER_DEFINED_TIME,
    SUDT_MAX
};
typedef enum tagSCHL_USER_DEFINED_TIME_E SCHL_USER_DEFINED_TIME_E;

enum tagSCHL_MAKE_EVENT_E {
    SME_EVENT_TIME_ICON,
    SME_EVENT_TIME_COMBOBOX,
    SME_EVENT_TIME,
	SME_EVENT_MEMO_ICON,
    SME_EVENT_MEMO,
    SME_EVENT_RECURSIVE_ICON,
    SME_EVENT_RECURSIVE_COMBOBOX,
    SME_EVENT_RECURSIVE_TEXT,
    SME_EVENT_RECURSIVE_ENDDATE,    
    SME_EVENT_ALARM_ICON,
    SME_EVENT_ALARM_COMBOBOX,
    SME_EVENT_ALARM_SET_DATE,
    SME_EVENT_ALARM_SET_TIME, 
    SME_MAX //15
};
typedef enum tagSCHL_MAKE_EVENT_E SCHL_MAKE_EVENT_E;

enum tagSCHL_DELETE_EVENT_E {
    SDE_DATE_START,
    SDE_DATE_START_TEXT,
    SDE_DATE_END,
    SDE_DATE_END_TEXT,
    SDE_MAX
};
typedef enum tagSCHL_DELETE_EVENT_E SCHL_DELETE_EVENT_E;

enum tagSCHL_MAKE_ANNIV_ALARM_E {
    SMAA_CHECK_BOX_ICON,
    SMAA_CHECK_BOX,
    SMAA_ALARM_TIME_ICON,
	SMAA_ALARM_TIME,
    SMAA_MAX
};
typedef enum tagSCHL_MAKE_ANNIV_ALARM_E SCHL_MAKE_ANNIV_ALARM_E;

enum tagSCHL_ALARM_TYPE_ALL_DAY_E {
    SATAD_NONE,
    SATAD_SET_ANYTIME,
    SATAD_MAX
};
typedef enum tagSCHL_ALARM_TYPE_ALL_DAY_E SCHL_ALARM_TYPE_ALL_DAY_E;

enum tagSCHL_ALARM_TYPE_SOME_TIME_E {
    SATST_NONE,
	SATST_BEFORE_10M,
	SATST_BEFORE_20M,
	SATST_BEFORE_30M,
	SATST_BEFORE_1H,
	SATST_BEFORE_1D,
    SATST_MAX
};
typedef enum tagSCHL_ALARM_TYPE_SOME_TIME_E SCHL_ALARM_TYPE_SOME_TIME_E;


struct  tagSCHL_GET_SCHEDULE_T {
    DWORD   dwStartTime;
    UINT8   unAddr;
};
typedef struct  tagSCHL_GET_SCHEDULE_T SCHL_GET_SCHEDULE_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
UINT8 l_anDaysOfMonth[12] =
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* 일정 타입 */
SKY_TOKEN atDayTimeMenu[SEDT_MAX] =
{
    TKN_SCHEDULE_EVENT_SOME_TIME,
    TKN_SCHEDULE_EVENT_ALL_DAY
};

/* 반복 주기 */
SKY_TOKEN atRecursiveMenu[SERT_MAX] =
{
    TKN_SCHEDULE_EVENT_RECURSIVE_OFF,
    TKN_SCHEDULE_EVENT_RECURSIVE_EVERYDAY,
    TKN_SCHEDULE_EVENT_RECURSIVE_EVERYWEEK,
    TKN_SCHEDULE_EVENT_RECURSIVE_EVERYMONTH
};
/* 종일 일정 알람 */
SKY_TOKEN atAlarmMenu4AllDay[SATAD_MAX] =
{
    TKN_SCHEDULE_EVENT_ALARM_OFF,
    TKN_SCHEDULE_EVENT_ALARM_SET_ANYTIME
};
/* 일정 알람 */
SKY_TOKEN atAlarmMenu4SomeTime[SATST_MAX] =
{
    TKN_SCHEDULE_EVENT_ALARM_OFF,
    TKN_SCHEDULE_EVENT_ALARM_BEFORE_10M,
    TKN_SCHEDULE_EVENT_ALARM_BEFORE_20M,
    TKN_SCHEDULE_EVENT_ALARM_BEFORE_30M,
    TKN_SCHEDULE_EVENT_ALARM_BEFORE_1H,
    TKN_SCHEDULE_EVENT_ALARM_BEFORE_1D
};

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/   
static  DWORD               l_dwInitSecs;       // initial secs_time 
static  SCHL_TIME_SET_T     l_SetTime;          // current time

static  SCHL_BLOCK_RECORD_T l_BlockT[SCHL_MAX_DAYS_AMONTH];
static  SCHL_RECORD_T*      l_pEventRecordT;
static  UINT                l_unIdxOfEvent4View;

LOCAL   UINT8               l_SaveWorkAddress;         


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
static VOID SKY_API sSKA_ListEvent(SKY_EVENT_T* pEvent);
static VOID SKY_API sSKA_SetUserDefinedTime(SKY_EVENT_T* pEvent);


static VOID sBuildBlockWithAnniv( VOID );
static VOID sBuildBlockWithEvent( VOID );
static VOID sDeleteAEvent( UINT8 unOneIdxOfBlockT, UINT8 unOneIdxOfEventT );
static VOID sDeleteFile( UINT8 unIdxOfMemoT );
static BOOL sSortEvent( INT8  anSortedIdxT[] );
static VOID sSaveAEvent( UINT8  unIdxOfBlockT, INT8 nIdxOfEventT );
static BOOL sSortEventToday(SCHL_PRE_CONFIG_T pTodayEventT[], INT8 anSortedIdxT[]);

/*
** ***************************************************************************
** Functions Definitions
** ***************************************************************************
*/

/*
** ***************************************************************************

                                    SKA_MainSchedule()
                                        |
                                        -------------------------
                                        |                        |
                                        {child}               {child}
                                        |                        |  
                                    SKA_MainCalendar() sSKA_SetUserDefinedTime()
                                        |
                                        {child}
                                        |
                                    sSKA_ViewSolarCalendar()
                                        |
         -----------------------------------------------------------------
        |                     |                                         |
      {child}              {child}                                   {child}
        |                     |                                         |
   SKA_ViewADay()  - {sibling} -  
        |
        -------------------------------------------------
        |                    |                           |
      {child}              {child}                    {child}
        |                    |                           |
    SKA_MakeAEvent()       sSKA_ListEvent()      SKA_ViewBookContent
        |                    |                           
** ***************************************************************************
*/
enum tagMAKE_NEWSCHEDULE_WORK_E 
{ 	   	
    MNW_DATE_START_TIME_E =0,
	MNW_DATE_ICON_E,
	MNW_DATE_END_TIME_E,
    MNW_DURING_ICON_E,
    MNW_DURING_TEXT_E,
	MNW_DURING_TIME_E,
	MNW_TIME_ICON_E,
	MNW_TIME_TIME_E,
	MNW_MEMO_ICON_E,
	MNW_MEMO_TEXT_E,
	MNW_ALARM_ICON_E,
	MNW_ALARM_CHECK_E,
	MNW_ALARM_TIME_TEXT_E,
	MNW_ALARM_TIME_E

};

VOID SKY_API sSKA_MakeNewWork( SKY_EVENT_T* pEvent )
{
	UINT8  uIndex;
	static DWORD  s_dwIndex;
	static BYTE   szWorkTitle[20];

	HCONTROL hControl;
	int      nIndex;
	UINT8    nType, nHour, nMinute;
	DWORD    dwTime;
	clk_julian_type  cjTime, cjCurrentTime;
	WORD     wValue;
    COORD    rosa1, rosa2;


    switch( pEvent->EventID )
    {
    case SE_APP_START:
		{
			COORD    StartX, StartY;

            MARGIN_T Margin;
            COORD    xSpace, ySpace;
            COORD    xPos, yPos;	
			BYTE*    ppszBuf[2];
			BYTE     szBuf[12];
            
            EQS_StartFormatter();   
            EQS_GetControlMargin(&Margin);
            EQS_GetControlSpace(&xSpace, &ySpace);
            EQS_EndFormatter();    
			Margin.xLeft  = 2;
            /* rosa 01-08-29 --> @why, what */
            Margin.xRight = 7;
            //Margin.xRight = 2;
            /* <-- */

			EQS_SetWindow(  SKY_DEF_WIN_X, 
							SKY_DEF_WIN_Y, 
							SKY_DEF_WIN_DX, 
							200, 
							CL_NONE_SELECT_SAVE, 
							(BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_NEW_WORK_TITLE), 
							WA_SHOW_CML | WA_SHOW_HEADER);
     
			clk_secs_to_julian(SKY_GetLocalTime(), &cjTime);


			if( s_dwIndex == SCHL_MAX_EVENTS_ADAY && GetNewIdxOfSchlT() == SCHL_ERROR )
			{// 더이상 추가할 수 없는 경우 ...
				EQS_END();
				break;
			}

			/* get the idx */

		    nType     = pEvent->wParam;
			uIndex    = l_SetTime.nDay - 1;

			s_dwIndex = pEvent->dwParam;
			if( s_dwIndex != SCHL_MAX_EVENTS_ADAY )
			{
				nType =  l_pEventRecordT[s_dwIndex].Recursive.Type - SERT_NONE;
			}

			StartY = Margin.yTop ;

			switch( nType )
			{
			case 0: /* 시간 지정 */
				StartX = 0;
				StartY = 0;
                hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
				StartX += EQC_GET_DX(hControl) + xSpace;
                hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
				StartX += EQC_GET_DX(hControl) + xSpace;
				                
				hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
				StartX += EQC_GET_DX(hControl) + xSpace;

				StartY = Margin.yTop;
				break;
			case 1: /* 매일 주기 */
			case 2:	/* 매주 주기 */		
				StartX = Margin.xLeft;
				hControl = EQC_Time(StartX, StartY, 0, MONTH_DAY_CONTROL );
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl, FALSE);

				StartX += EQC_GET_DX(hControl) + xSpace-2;
 				hControl = EQC_StaticText(StartX, StartY, TKN_SCHEDULE_START_END_HYPEN);

                StartX += EQC_GET_DX(hControl) + xSpace;
				hControl = EQC_Time(StartX, StartY, 0, MONTH_DAY_CONTROL );
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

				StartY += EQC_GET_DY(hControl) + ySpace;
				break;
			case 3: /* 매월 주기 */
				StartX = Margin.xLeft+52;
				hControl = EQC_Time(StartX, StartY, 0, MON_TIME_CONTROL);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl, FALSE);

                //StartX += EQC_GET_DX(hControl) + xSpace-4;
                StartX += EQC_GET_DX(hControl) + xSpace-2;
 				hControl = EQC_StaticText(StartX, StartY, TKN_SCHEDULE_START_END_HYPEN);

                StartX += EQC_GET_DX(hControl) + xSpace;
				hControl = EQC_Time(StartX, StartY, 0, MON_TIME_CONTROL);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

				StartY += EQC_GET_DY(hControl) + ySpace;
				break;
			case 4: /* 매년 주기 */
				StartX = Margin.xLeft+24;
				hControl = EQC_Time(StartX, StartY, 0, YEAR_TIME_CONTROL);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl, FALSE);

				StartX += EQC_GET_DX(hControl) + xSpace-2;
 				hControl = EQC_StaticText(StartX, StartY, TKN_SCHEDULE_START_END_HYPEN);

                StartX += EQC_GET_DX(hControl) + xSpace;
				hControl = EQC_Time(StartX, StartY, 0, YEAR_TIME_CONTROL);

				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
				StartY += EQC_GET_DY(hControl) + ySpace;
				break;
			default:
				EQS_END();
				break;
			}

			StartX = Margin.xLeft;	
			
			hControl = EQC_StaticText(StartX, StartY, TKN_NULL);//TKN_ALARM_TIME_MONTH);			
			StartX += EQC_GET_DX(hControl) + xSpace;


			switch( nType )
			{
            case 0: //시간지정 
				hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
				StartX += EQC_GET_DX(hControl) + xSpace;
				hControl = EQC_Time(StartX, StartY, 0, CAL_TIME_CONTROL );
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl, FALSE);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
                //rosa1 = EQS_GetMaxX();
                //rosa2 = EQC_GET_DX(hControl);
                //EQC_SET_X(hControl, EQS_GetMaxX() - 2 - 98);
				break;
			case 1:// 매일 
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_DAY_SET));
				hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
				StartX += EQC_GET_DX(hControl) + xSpace;
                hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NEW_MENU1);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
				break;
			case 2://매주 
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_WEEK_SET));
				hControl = EQC_Time( StartX, StartY, 0,WEEK_TIME_CONTROL );
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl, FALSE);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

				StartX = EQC_GET_X(hControl) - EQS_CharWidth((BYTE)' ')*5;
				hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NEW_MENU2);				
				break;
			case 3://매월
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_MONTH_SET));
				hControl = EQC_Time( StartX, StartY, 0,DAY_TIME_CONTROL );
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl, FALSE);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

				StartX = EQC_GET_X(hControl) - EQS_CharWidth((BYTE)' ')*5;
				hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NEW_MENU3);
				break;
			case 4://매년 
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_YEAR_SET));

				hControl = EQC_Time(StartX, StartY, 0, MONTH_DAY_CONTROL );
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl, FALSE);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

				StartX = EQC_GET_X(hControl) - EQS_CharWidth((BYTE)' ')*5;
				hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_EVERY_YEAR);

				break;
			default:
				EQS_END();
				return;
				break;
			}

			StartY += EQC_GET_DY(hControl) + ySpace;
            
			StartX = Margin.xLeft;
			hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
			EQC_SetStaticText(hControl, SF_TIME);
			StartX += EQC_GET_DX(hControl) + xSpace;

			//hControl = EQC_Time(StartX, StartY, 0, TIME_TO_TIME_CONTROL); //01.06.19
			hControl = EQC_Time(StartX, StartY, 0, TIME_MINUTE_CONTROL);
			EQC_SetFirstTime(hControl, cjTime.hour, cjTime.minute);
            //EQC_SetSecondTime(hControl, cjTime.hour, cjTime.minute);  //01.06.19

			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
//
			StartX = Margin.xLeft;
			StartY += EQC_GET_DY(hControl) + ySpace;
			hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
		    EQC_SetStaticText(hControl, (BYTE*)SF_TITLE);
			StartX += EQC_GET_DX(hControl) + xSpace;
			hControl = EQC_TextEdit(StartX, StartY, MAX_SIZE_VIEW_LINE-5,	2, \
						            TEM_KOREAN,	SCHL_MAX_EVENT_DATA_LENGTH,\
						            (BYTE *)"",	0);	
			EQC_SET_USERDATA(hControl, nType);
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
//
			StartX = Margin.xLeft;			
			StartY += EQC_GET_DY(hControl) + ySpace;

            /* rosa 2001-10-10 --> @why, what */
            hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NOMALDAY_SET); //알람울림
			//sprintf(szBuf,"%s%s%s",SF_ALARM1_3,SF_ALARM2_3, SF_ALARM3_3); //알람울림
			//EQC_SetStaticText(hControl,szBuf);
            /* <-- */

			StartX += EQC_GET_DX(hControl) + xSpace;

			ppszBuf[0] = (BYTE*)SKY_GET_TOKEN(TKN_ALARM_UNSET);
			ppszBuf[1] = (BYTE*)SKY_GET_TOKEN(TKN_ALARM_SET);
			hControl = EQC_ScrollCombo(StartX, StartY,6,1,ppszBuf,2,2,0);   // 알람 울림 설정(해제)
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
			
			StartX = Margin.xLeft;			
			StartY += EQC_GET_DY(hControl) + ySpace;

            /* rosa 01-08-29 --> @why, what */
			hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_TIME); // 알람시간  
			//sprintf(szBuf,"%s%s%s",SF_ALARM_TIME1_3	,SF_ALARM_TIME2_3, SF_ALARM_TIME3_3	); //알람시간
			//EQC_SetStaticText(hControl,szBuf);
			StartX += EQC_GET_DX(hControl) + xSpace;            
            StartY += EQC_GET_DY(hControl) + ySpace;
            /* <-- */

			hControl = EQC_Time( StartX, StartY, 0,TIME_MINUTE_CONTROL);
			EQC_SetTimeEditMode(hControl, FALSE);
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

			EQC_SetTimeHour(hControl, cjTime.hour);
			EQC_SetTimeMinute(hControl, cjTime.minute);	
			
			
			if(  s_dwIndex == SCHL_MAX_EVENTS_ADAY )  //새로 생성
			{
				switch( nType )
				{
				case 0:
					EQC_SetTimeYear(EQS_GetNthIOC(MNW_DURING_TIME_E), l_SetTime.wYear);
					EQC_SetTimeMonth(EQS_GetNthIOC(MNW_DURING_TIME_E), l_SetTime.nMonth);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DURING_TIME_E), l_SetTime.nDay);
					break;
				case 1://매일 

					EQC_SetTimeYear( EQS_GetNthIOC(MNW_DATE_END_TIME_E), l_SetTime.wYear);
					EQC_SetTimeMonth( EQS_GetNthIOC(MNW_DATE_END_TIME_E), l_SetTime.nMonth);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DATE_END_TIME_E), l_SetTime.nDay);
					EQC_SetTimeLower(EQS_GetNthIOC(MNW_DATE_END_TIME_E),\
							         l_SetTime.wYear,l_SetTime.nMonth,l_SetTime.nDay,1,0);
					break;
				case 2://매주
					EQC_SetTimeWeek(EQS_GetNthIOC(MNW_DURING_TIME_E), l_SetTime.nDayOfWeek);

					EQC_SetTimeYear( EQS_GetNthIOC(MNW_DATE_END_TIME_E), l_SetTime.wYear);
					EQC_SetTimeMonth( EQS_GetNthIOC(MNW_DATE_END_TIME_E), 12);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DATE_END_TIME_E), 31);
					EQC_SetTimeLower(EQS_GetNthIOC(MNW_DATE_END_TIME_E),\
							         l_SetTime.wYear,l_SetTime.nMonth,l_SetTime.nDay,1,0);
					break;
				case 3://매월
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DURING_TIME_E), l_SetTime.nDay);

					EQC_SetTimeYear( EQS_GetNthIOC(MNW_DATE_END_TIME_E), l_SetTime.wYear);
					EQC_SetTimeMonth( EQS_GetNthIOC(MNW_DATE_END_TIME_E), 12);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DATE_END_TIME_E), 31);
					EQC_SetTimeLower(EQS_GetNthIOC(MNW_DATE_END_TIME_E),\
							         l_SetTime.wYear,l_SetTime.nMonth,l_SetTime.nDay,1,0);
					break;
				case 4: //매년 
					EQC_SetTimeMonth(EQS_GetNthIOC(MNW_DURING_TIME_E), l_SetTime.nMonth);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DURING_TIME_E), l_SetTime.nDay);

					EQC_SetTimeYear( EQS_GetNthIOC(MNW_DATE_END_TIME_E), SCHL_END_YEAR );
					EQC_SetTimeYearLimit(EQS_GetNthIOC(MNW_DATE_END_TIME_E), l_SetTime.wYear, SCHL_END_YEAR);

					EQC_SetTimeMonth( EQS_GetNthIOC(MNW_DATE_END_TIME_E), 12);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DATE_END_TIME_E), 31);
					EQC_SetTimeLower(EQS_GetNthIOC(MNW_DATE_END_TIME_E),\
							         l_SetTime.wYear,l_SetTime.nMonth,l_SetTime.nDay,1,0);
					break;
				default:
					break;
				}

				if( nType )
				{
					EQC_SetTimeYear( EQS_GetNthIOC(MNW_DATE_START_TIME_E), l_SetTime.wYear);
					EQC_SetTimeMonth( EQS_GetNthIOC(MNW_DATE_START_TIME_E), l_SetTime.nMonth);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DATE_START_TIME_E), l_SetTime.nDay);
				}



			}
			else // 기존 데이터 수정
			{
				if( nType )
				{
					clk_secs_to_julian(l_pEventRecordT[s_dwIndex].dwStartTime, &cjTime);
                    EQC_SetTimeYear(EQS_GetNthIOC(MNW_DATE_START_TIME_E), cjTime.year);
					EQC_SetTimeMonth(EQS_GetNthIOC(MNW_DATE_START_TIME_E), cjTime.month);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DATE_START_TIME_E), cjTime.day);

					EQC_SetTimeLower(EQS_GetNthIOC(MNW_DATE_END_TIME_E),\
						             cjTime.year,cjTime.month,cjTime.day,1,0);
					wValue = cjTime.year;

					clk_secs_to_julian(l_pEventRecordT[s_dwIndex].Recursive.dwEndTime, &cjTime);
					EQC_SetTimeYear(EQS_GetNthIOC(MNW_DATE_END_TIME_E), cjTime.year);
					EQC_SetTimeMonth(EQS_GetNthIOC(MNW_DATE_END_TIME_E), cjTime.month);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DATE_END_TIME_E), cjTime.day);

					EQC_SetTimeYearLimit(EQS_GetNthIOC(MNW_DATE_END_TIME_E), wValue, SCHL_END_YEAR);
				}


				clk_secs_to_julian(l_pEventRecordT[s_dwIndex].dwStartTime, &cjTime);	
				switch( nType )
				{
				case 0:
					EQC_SetTimeYear(EQS_GetNthIOC(MNW_DURING_TIME_E), cjTime.year);
					EQC_SetTimeMonth(EQS_GetNthIOC(MNW_DURING_TIME_E), cjTime.month);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DURING_TIME_E), cjTime.day);
					break;
				case 1:				
					break;
				case 2:
					EQC_SetTimeWeek(EQS_GetNthIOC(MNW_DURING_TIME_E), cjTime.day_of_week);
					break;
				case 3:
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DURING_TIME_E), cjTime.day);
					break;
				case 4:
					EQC_SetTimeMonth(EQS_GetNthIOC(MNW_DURING_TIME_E), cjTime.month);
					EQC_SetTimeDay(EQS_GetNthIOC(MNW_DURING_TIME_E), cjTime.day);
					break;
				default:
					break;
				}

				clk_secs_to_julian(l_pEventRecordT[s_dwIndex].dwStartTime, &cjTime);
				EQC_SetFirstTime(EQS_GetNthIOC(MNW_TIME_TIME_E), cjTime.hour, cjTime.minute);

				//clk_secs_to_julian(l_pEventRecordT[s_dwIndex].dwEndTime, &cjTime);
				//EQC_SetSecondTime(EQS_GetNthIOC(MNW_TIME_TIME_E), cjTime.hour, cjTime.minute);
				//01.06.19

				
				EQC_SetTextEditData(EQS_GetNthIOC(MNW_MEMO_TEXT_E), l_pEventRecordT[s_dwIndex].abData); 

				clk_secs_to_julian(l_pEventRecordT[s_dwIndex].Alarm.dwTime, &cjTime);
				EQC_SetTimeHour(EQS_GetNthIOC(MNW_ALARM_TIME_E), cjTime.hour);
				EQC_SetTimeMinute(EQS_GetNthIOC(MNW_ALARM_TIME_E), cjTime.minute);	
				if( l_pEventRecordT[s_dwIndex].Alarm.Type == SATT_NONE )
				{                    
					EQC_SetTimeEditMode(EQS_GetNthIOC(MNW_ALARM_TIME_E), FALSE);
				}
				else
				{
					EQC_SetScrollPhysicalIndex(EQS_GetNthIOC(MNW_ALARM_CHECK_E), 1);
					EQC_SetTimeEditMode(EQS_GetNthIOC(MNW_ALARM_TIME_E), TRUE);
				}


				for(uIndex=0; uIndex < EQS_GetIOCNum(); uIndex++)
				{			
					EQC_SET_FOCUS(EQS_GetNthIOC(uIndex), FALSE);
					EQC_SET_EDIT(EQS_GetNthIOC(uIndex), FALSE);
				}
				sprintf((char*)szWorkTitle, "%d%s%d%s %s", 
						l_SetTime.nMonth, SKY_GET_TOKEN(TKN_DAT_MONTH),\
						l_SetTime.nDay, SKY_GET_TOKEN(TKN_DAT_DAY),\
						                SKY_GET_TOKEN(TKN_SCHEDULE_WORK_TITLE));
				EQS_SetSWindowHeaderStr( szWorkTitle );	
				EQS_SetWindowCML(CL_NONE_NONE_EDIT);
				
			}
			EQS_FitCanvasSize();
		}
        break;

    case SE_APP_END:
		
        break;
	case SE_SOFTKEY_EDIT:
		hControl = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_PBOOK_CHANGE_EDIT_MODE_MSG),\
								   0, MBT_NONE,MBT_HELP); //MESSAGE_DEFAULT_TIME,
		EQC_SET_USERDATA(hControl, SE_SOFTKEY_EDIT);
	
		break;
	case SE_SOFTKEY_SAVE:
		nType = EQC_GET_USERDATA( EQS_GetNthIOC(MNW_MEMO_TEXT_E) );
		clk_secs_to_julian(SKY_GetLocalTime(), &cjTime);
		
		cjTime.year  =  l_SetTime.wYear;
		cjTime.month =  l_SetTime.nMonth;
		cjTime.day   =  l_SetTime.nDay;
									
		uIndex = l_SetTime.nDay - 1;
		if( s_dwIndex == SCHL_MAX_EVENTS_ADAY )
		{
			s_dwIndex = GetNewIdxOfEventT(uIndex);
			GetANewEvent(uIndex, s_dwIndex);
		}

		l_SaveWorkAddress = s_dwIndex;
		if( nType )
		{
			l_pEventRecordT[s_dwIndex].Type = SRT_EVENT_WITH_R_A; //SRT_EVENT_WITH_R
		}
		else
		{
			l_pEventRecordT[s_dwIndex].Type = SRT_EVENT_WITH_A; //SRT_EVENT
		}

		switch( nType )
		{ 
		case 0:
			cjTime.year  = EQC_GetTimeYear(  EQS_GetNthIOC(MNW_DURING_TIME_E) );
			cjTime.month = EQC_GetTimeMonth( EQS_GetNthIOC(MNW_DURING_TIME_E) );
			cjTime.day   = EQC_GetTimeDay(   EQS_GetNthIOC(MNW_DURING_TIME_E) );
			l_pEventRecordT[s_dwIndex].Recursive.Type = SERT_NONE;
			
			break;
		case 1: // 매일 
			cjTime.year  = EQC_GetTimeYear(EQS_GetNthIOC(MNW_DATE_START_TIME_E));
			cjTime.month = EQC_GetTimeMonth( EQS_GetNthIOC(MNW_DATE_START_TIME_E) );
			cjTime.day   = EQC_GetTimeDay(   EQS_GetNthIOC(MNW_DATE_START_TIME_E) );

			l_pEventRecordT[s_dwIndex].Recursive.Type = SERT_EVERY_DAY;

			break;
		case 2: // 매주 
			cjTime.year  = EQC_GetTimeYear(EQS_GetNthIOC(MNW_DATE_START_TIME_E));
			cjTime.month = EQC_GetTimeMonth( EQS_GetNthIOC(MNW_DATE_START_TIME_E) );
			cjTime.day   = EQC_GetTimeDay(   EQS_GetNthIOC(MNW_DATE_START_TIME_E) );
			
			cjTime.day_of_week = EQC_GetTimeWeek( EQS_GetNthIOC(MNW_DURING_TIME_E) );
			l_pEventRecordT[s_dwIndex].Recursive.Type = SERT_EVERY_WEEK;
										
			break;
		case 3: // 매월 
			cjTime.year  = EQC_GetTimeYear(EQS_GetNthIOC(MNW_DATE_START_TIME_E));
			cjTime.month = EQC_GetTimeMonth( EQS_GetNthIOC(MNW_DATE_START_TIME_E) );
			//cjTime.day   = EQC_GetTimeDay(   EQS_GetNthIOC(MNW_DATE_START_TIME_E));
			cjTime.day = EQC_GetTimeDay( EQS_GetNthIOC(MNW_DURING_TIME_E) );

			l_pEventRecordT[s_dwIndex].Recursive.Type = SERT_EVERY_MONTH;

			break;
		case 4: //매년 
			cjTime.year  = EQC_GetTimeYear(EQS_GetNthIOC(MNW_DATE_START_TIME_E));
			cjTime.month = EQC_GetTimeMonth( EQS_GetNthIOC(MNW_DURING_TIME_E) );
			cjTime.day   = EQC_GetTimeDay(   EQS_GetNthIOC(MNW_DURING_TIME_E) );
			l_pEventRecordT[s_dwIndex].Recursive.Type = SERT_EVERY_YEAR;
			break;

		}
				
		
		EQC_GetFirstTime( EQS_GetNthIOC(MNW_TIME_TIME_E), &cjTime.hour, &cjTime.minute);
        l_pEventRecordT[s_dwIndex].dwStartTime = clk_julian_to_secs(&cjTime);

		//EQC_GetSecondTime( EQS_GetNthIOC(MNW_TIME_TIME_E), &cjTime.hour, &cjTime.minute); //01.06.19
        l_pEventRecordT[s_dwIndex].dwEndTime = clk_julian_to_secs(&cjTime);			

        memset(l_pEventRecordT[s_dwIndex].abData, NULL,  sizeof(SCHL_MAX_EVENT_DATA_LENGTH+1));
		EQC_GetTextEditData(EQS_GetNthIOC(MNW_MEMO_TEXT_E), l_pEventRecordT[s_dwIndex].abData);
       
		if( nType )
		{
			clk_julian_type  cjEndTime;
			UINT8 nFromMonth, nFromDay;

			clk_secs_to_julian(SKY_GetLocalTime(), &cjEndTime);

			cjEndTime.year  = EQC_GetTimeYear(EQS_GetNthIOC(MNW_DATE_END_TIME_E));

			if( nType == 4 ) // 매년 
			{
				cjTime.month = EQC_GetTimeMonth( EQS_GetNthIOC(MNW_DURING_TIME_E) );
				cjTime.day   = EQC_GetTimeDay(   EQS_GetNthIOC(MNW_DURING_TIME_E) );
                /* rosa 01-07-27 -->  이후 날짜에 일정을 생성하면 시작날짜 값으로 설정되는 것을 막음 */
                cjEndTime.month = EQC_GetTimeMonth(EQS_GetNthIOC(MNW_DATE_END_TIME_E));
				cjEndTime.day = EQC_GetTimeDay(EQS_GetNthIOC(MNW_DATE_END_TIME_E));
                /* <-- */
			}
			else
			{
				cjEndTime.month = EQC_GetTimeMonth(EQS_GetNthIOC(MNW_DATE_END_TIME_E));
				cjEndTime.day = EQC_GetTimeDay(EQS_GetNthIOC(MNW_DATE_END_TIME_E));
			}

			if( cjEndTime.day > GetDaysOfMonth(cjEndTime.year, cjEndTime.month ) )
			{			  
				cjEndTime.day = GetDaysOfMonth(cjEndTime.year, cjEndTime.month ) ;
			}
	

			hControl = EQS_GetNthIOC(MNW_DATE_START_TIME_E);			
			nFromMonth = EQC_GetTimeMonth(hControl);
			nFromDay   = EQC_GetTimeDay(hControl);
			if( (nFromMonth > cjEndTime.month) ||\
				(nFromMonth==cjEndTime.month && nFromDay > cjEndTime.day) )
			{
				l_pEventRecordT[s_dwIndex].Recursive.dwEndTime = l_pEventRecordT[s_dwIndex].dwStartTime;
			}
			else
			{						
				l_pEventRecordT[s_dwIndex].Recursive.dwEndTime = clk_julian_to_secs(&cjEndTime);
			}			
		}
		else
		{
			l_pEventRecordT[s_dwIndex].Recursive.dwEndTime = l_pEventRecordT[s_dwIndex].dwStartTime;
		}


        if( EQC_GetScrollLogicalIndex(EQS_GetNthIOC(MNW_ALARM_CHECK_E)) )
		{    
			l_pEventRecordT[s_dwIndex].Alarm.Type = SATT_BEFORE_10M; //시간지정 
			cjTime.hour   = EQC_GetTimeHour( EQS_GetNthIOC(MNW_ALARM_TIME_E) );
			cjTime.minute = EQC_GetTimeMinute( EQS_GetNthIOC(MNW_ALARM_TIME_E) );
			l_pEventRecordT[s_dwIndex].Alarm.dwTime = clk_julian_to_secs(&cjTime);

			l_pEventRecordT[s_dwIndex].Alarm.dwEndTime = l_pEventRecordT[s_dwIndex].Recursive.dwEndTime;

			clk_secs_to_julian(SKY_GetLocalTime(), &cjCurrentTime);
			if( cjCurrentTime.hour > cjTime.hour ||\
				(cjCurrentTime.hour == cjTime.hour && cjCurrentTime.minute > cjTime.minute ) )
			{
				if( nType )
				{
					l_pEventRecordT[s_dwIndex].Type = SRT_USED_EVENT_WITH_R_A; //SRT_EVENT_WITH_R
				}
				else
				{
					l_pEventRecordT[s_dwIndex].Type = SRT_USED_EVENT_WITH_A; //SRT_EVENT
				}
			}
		}
		else
		{
			l_pEventRecordT[s_dwIndex].Alarm.Type = SATT_NONE; //시간지정 
			cjTime.hour   = EQC_GetTimeHour( EQS_GetNthIOC(MNW_ALARM_TIME_E) );
			cjTime.minute = EQC_GetTimeMinute( EQS_GetNthIOC(MNW_ALARM_TIME_E) );
			l_pEventRecordT[s_dwIndex].Alarm.dwTime = clk_julian_to_secs(&cjTime);
			l_pEventRecordT[s_dwIndex].Alarm.dwEndTime = l_pEventRecordT[s_dwIndex].Alarm.dwTime;
		}



        /* save */
        sSaveAEvent(uIndex, s_dwIndex);

		hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_PBOOK_SAVE_MSG),\
								  MESSAGE_DEFAULT_TIME,\
								  MBT_NONE, MBT_HELP);	

		if( pEvent->dwParam == 0 )  // 리스트로 돌아감 
		{
			EQC_SET_USERDATA(hControl, SE_APP_RESUME );
			
		}
		else
		{
			EQC_SET_USERDATA(hControl, pEvent->dwParam);  //process  key_end and key_clear
			//EQS_END_TO(EQS_POP_TO_ROOT);
		}

		break;

	case SE_KEY_CLEAR:          	
        if(EQS_IsControlDataChanged())
        {
    		hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO,
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hControl, SE_KEY_CLEAR);
        }
		else
		{				
			EQS_END();//EQS_END_TO(EQS_POP_TO_SUBROOT);	
		}
        break;
	case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
    		hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO,
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hControl, SE_KEY_END);
        }		
		else
		{				
			EQS_END_TO(EQS_POP_TO_ROOT);
		}
        break;

    case SE_IO_MESGBOX_YES:		
		EQS_SEND_EVENT( SE_SOFTKEY_SAVE, EQS_NA, pEvent->dwParam);
		break;
    case SE_IO_MESGBOX_NO:
		if( pEvent->dwParam == SE_KEY_END )
		{
			EQS_END_TO(EQS_POP_TO_ROOT);
		}
		else  // pEvent->dwParam == SE_KEY_CLEAR
		{
			EQS_END();
		}
		break;
	case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SE_KEY_END)
		{
            EQS_END_TO(EQS_POP_TO_ROOT);
		}
		else if( pEvent->dwParam == SE_KEY_CLEAR ||\
			     pEvent->dwParam == SE_APP_RESUME ) // 성공적으로 저장하고 리스트로 돌아감 
		{
			EQS_END_EVENT(SE_APP_RESUME, TRUE, EQS_NA);
		}
		else if( pEvent->dwParam == SE_SOFTKEY_EDIT )
		{

			for(uIndex=0; uIndex < EQS_GetIOCNum(); uIndex++)
			{			
				EQC_SET_EDIT(EQS_GetNthIOC(uIndex), TRUE);
			}
					
			EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
			nType = EQC_GET_USERDATA( EQS_GetNthIOC(MNW_MEMO_TEXT_E) );
			EQC_SET_FOCUS( EQS_GetNthIOC(MNW_TIME_TIME_E), TRUE);
			EQC_SET_FOCUS( EQS_GetNthIOC(MNW_MEMO_TEXT_E ), TRUE);
			EQC_SET_FOCUS( EQS_GetNthIOC(MNW_ALARM_CHECK_E ), TRUE);
			EQC_SET_FOCUS( EQS_GetNthIOC(MNW_ALARM_TIME_E ), TRUE);
			switch( nType )
			{
			case 0:
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_SCHEDULE_NEW_WORK_TITLE));
				break;
			case 1:// 매일 
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_DAY_SET));
				EQC_SET_FOCUS( EQS_GetNthIOC(MNW_DATE_END_TIME_E ), TRUE);
				break;
			case 2://매주 
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_WEEK_SET));
				EQC_SET_FOCUS( EQS_GetNthIOC(MNW_DATE_END_TIME_E ), TRUE);
				break;
			case 3://매월
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_MONTH_SET));
				EQC_SET_FOCUS( EQS_GetNthIOC(MNW_DATE_END_TIME_E ), TRUE);
			case 4://매년 
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_NEW_WORK_YEAR_SET));
				EQC_SET_FOCUS( EQS_GetNthIOC(MNW_DATE_END_TIME_E ), TRUE);
				break;	
			}

			EQS_SetIOCFocus( EQS_GetNthIOC(MNW_TIME_TIME_E) );
			if( nType )
			{
				EQS_SetIOCFocus( EQS_GetNthIOC(MNW_DATE_END_TIME_E) );
			}

			if( EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(MNW_ALARM_CHECK_E)) == 1 ) //알람 시간 설정 
			{
				EQC_SetTimeEditMode(EQS_GetNthIOC(MNW_ALARM_TIME_E), TRUE);
			}
			else
			{
				EQC_SetTimeEditMode(EQS_GetNthIOC(MNW_ALARM_TIME_E), FALSE);
			}
	
		}
        else
		{
		    EQS_END();
		}
		break;
    case SE_IO_SCROLL_LIST_CANCEL:
		if( pEvent->wParam == EQS_GetNthIOC(MNW_ALARM_CHECK_E) )
		{
			EQC_SetTimeEditMode(EQS_GetNthIOC(MNW_ALARM_TIME_E), EQC_GetScrollLogicalIndex(pEvent->wParam));
		}
		break;
	case SE_IO_SCROLL_LIST_CHG:
		if( pEvent->wParam == EQS_GetNthIOC(MNW_ALARM_CHECK_E) )
		{
			EQC_SetTimeEditMode(EQS_GetNthIOC(MNW_ALARM_TIME_E), pEvent->dwParam);
		}
		break;
	case EQE_IO_TIME_SELECT:
		hControl = EQS_GetNthIOC(MNW_DATE_START_TIME_E);
		if( pEvent->wParam == EQS_GetNthIOC(MNW_DATE_END_TIME_E) )
		{
			UINT8 nFromMonth, nFromDay,  nToMonth, nToDay;

			nFromMonth = EQC_GetTimeMonth(hControl);
			nFromDay   = EQC_GetTimeDay(hControl);
			nToMonth   = EQC_GetTimeMonth(pEvent->wParam);
			nToDay     = EQC_GetTimeDay(pEvent->wParam);

			if( (nFromMonth > nToMonth) || (nFromMonth==nFromDay && nFromDay > nToDay) )
			{
				EQC_SetTimeMonth(pEvent->wParam, nFromMonth);
				EQC_SetTimeDay(pEvent->wParam, nFromDay);
			}
		}
		else if( pEvent->wParam == EQS_GetNthIOC(MNW_TIME_TIME_E) )
		{
			DWORD dwFirstTime, dwSecondTime;

            clk_secs_to_julian(SKY_GetLocalTime(), &cjTime);
			cjTime.year  =  l_SetTime.wYear;
			cjTime.month =  l_SetTime.nMonth;
			cjTime.day   =  l_SetTime.nDay;

			EQC_GetSecondTime( EQS_GetNthIOC(MNW_TIME_TIME_E), &cjTime.hour, &cjTime.minute);
			dwSecondTime = clk_julian_to_secs(&cjTime);
			EQC_GetFirstTime( EQS_GetNthIOC(MNW_TIME_TIME_E), &cjTime.hour, &cjTime.minute);
			dwFirstTime = clk_julian_to_secs(&cjTime);
			if( dwFirstTime > dwSecondTime )
			{
				EQC_SetSecondTime(EQS_GetNthIOC(MNW_TIME_TIME_E), cjTime.hour, cjTime.minute);
			}

		}
		else ;

		break;        
    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }

}


VOID SKY_API SKA_MakeAEvent(SKY_EVENT_T *pEvent)
{
	BYTE*    paMakeWorkMenu[5];
	HCONTROL hControl;
	int      nIndex;

    switch( pEvent->EventID )
    {
    case SE_APP_START:

        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_SELECT_NONE, 
                        SKY_GET_TOKEN(TKN_SCHEDULE_NEW_WORK_TITLE),
                        WA_SHOW_CML | WA_SHOW_HEADER);


		/* set scroll list item */
		paMakeWorkMenu[0] = SKY_GET_TOKEN(TKN_ALARM_SET_ANYTIME);
		paMakeWorkMenu[1] = SKY_GET_TOKEN(TKN_ALARM_EVERYDAY_TITLE);
		paMakeWorkMenu[2] = SKY_GET_TOKEN(TKN_ALARM_EVERYWEEK_TITLE);
		paMakeWorkMenu[3] = SKY_GET_TOKEN(TKN_ALARM_EVERYMONTH_TITLE);
		paMakeWorkMenu[4] = SKY_GET_TOKEN(TKN_ALARM_EVERYYEAR_TITLE);


		/* create scroll list */
        hControl = EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), paMakeWorkMenu,5, 5, 0 );
        EQC_SET_STYLE(hControl, 
					(EQC_GET_STYLE(hControl)|CS_SCROLL_LIST_ENUMBERATED
										    |CS_SCROLL_LIST_MAXIMIZE) );
        EQS_SetIOCActive(hControl);
        break;

    case SE_APP_END:        
        break;

    case SE_APP_RESUME:
		if( pEvent->wParam == 0 )
		{
			EQS_SetIOCActive(EQS_GetNthIOC(0));
		}
		else
		{
			EQS_END_EVENT( SE_APP_RESUME, pEvent->wParam, pEvent->dwParam );
		}
        break;

	case SE_IO_SCROLL_LIST_SELECT: /* set */
		nIndex = pEvent->dwParam;
		EQS_START_CHILD_EVENT(sSKA_MakeNewWork, SE_APP_START,\
				              nIndex, SCHL_MAX_EVENTS_ADAY);
		break;
	case SE_IO_MESGBOX_END:
		EQS_END();
		break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if( pEvent->dwParam != EQC_DELETED)
        {
            EQS_END();
        }
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



VOID SKY_API SKA_MainSchedule(SKY_EVENT_T *pEvent)
{
    clk_julian_type  cjSetTime;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        /* get the current time */
        l_dwInitSecs = SKY_GetLocalTime(); 
        clk_secs_to_julian(l_dwInitSecs, &cjSetTime);

        /* if system time is OK */
        if(SKY_IS_VALID_LOCAL_TIME(l_dwInitSecs) &&
           (cjSetTime.year != SCHL_INIT_YEAR))
        {
            /* makes CALENDAR data table */
            MakeSchedule(l_dwInitSecs);
            /* go to CALENDAR */
            EQS_START_CHILD_EVENT(SKA_MainCalendar, SE_APP_START, EQS_NA, GetScheduleTime());
        }
        else
		{
            EQS_START_CHILD(sSKA_SetUserDefinedTime);
		}
        break;

   case SE_APP_RESUME:
        if(EQS_GET_CHILD() == sSKA_SetUserDefinedTime)
        {
            if(SKY_IS_VALID_LOCAL_TIME(l_dwInitSecs))
            {
                /* makes CALENDAR data table */
                MakeSchedule(l_dwInitSecs);
                /* go to CALENDAR */
                EQS_START_CHILD_EVENT(SKA_MainCalendar, SE_APP_START, EQS_NA, GetScheduleTime());
            }
            else
                EQS_END();
        }
        else
            EQS_END();
        break;

    case SE_APP_END:
        break;
    
    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }    
}

static  VOID    SKY_API sSKA_SetUserDefinedTime( SKY_EVENT_T* pEvent )
{
    MARGIN_T Margin;
    COORD    StartX, StartY,xSpace, ySpace;
    UINT8 unCtrl;
    clk_julian_type cjInitTime;
	HCONTROL hControl;

    switch( pEvent->EventID )
    {
    case SE_APP_START:


        
        EQS_StartFormatter();   
        EQS_GetControlMargin(&Margin);
        EQS_GetControlSpace(&xSpace, &ySpace);
        EQS_EndFormatter();  

        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX,
                      SKY_DEF_WIN_DY, 
                      CL_NONE_NONE_SET, 
                      NULL, 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        /*
        ** ***************************************************************************
        ** CREATE
        ** ***************************************************************************
        */
		StartY = Margin.yTop + ySpace;
		StartX = Margin.xLeft+ 2*xSpace;
        hControl = EQC_StaticText(StartX, StartY, TKN_SCHEDULE_USER_DEFINED_TIME_TEXT); //SUDT_USER_DEFINED_TIME_TEXT

		StartY += EQC_GET_DY(hControl) + 8*ySpace;

        EQC_Time(0, StartY, 0, ENTIRE_TIME_CONTROL); //SUDT_USER_DEFINED_TIME
        EQC_SetTimeYearLimit(EQS_GetNthIOC(SUDT_USER_DEFINED_TIME), SCHL_START_YEAR, SCHL_END_YEAR);

        EQS_FitCanvasSize();

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_NO_SYSTEM_TIME), 
                       MESSAGE_DEFAULT_TIME, 
                       MBT_NONE, 
                       MBT_HELP );

        break;

    case SE_APP_END:
   
        break;

    case SE_SOFTKEY_SET:
        /*
        ** ***************************************************************************
        ** GET THE VALUE
        ** ***************************************************************************
        */
        cjInitTime.year = (WORD)EQC_GetTimeYear( EQS_GetNthIOC(SUDT_USER_DEFINED_TIME) );
        cjInitTime.month = (WORD)EQC_GetTimeMonth( EQS_GetNthIOC(SUDT_USER_DEFINED_TIME) );
        cjInitTime.day_of_week = (WORD)EQC_GetTimeWeek( EQS_GetNthIOC(SUDT_USER_DEFINED_TIME) );
        cjInitTime.day = (WORD)EQC_GetTimeDay( EQS_GetNthIOC(SUDT_USER_DEFINED_TIME) );
        cjInitTime.hour = (WORD)EQC_GetTimeHour( EQS_GetNthIOC(SUDT_USER_DEFINED_TIME) );
        cjInitTime.minute = (WORD)EQC_GetTimeMinute( EQS_GetNthIOC(SUDT_USER_DEFINED_TIME) );
        cjInitTime.second = (WORD)0;
        l_dwInitSecs = clk_julian_to_secs(&cjInitTime);
        EQS_END();
        break;

    case SE_KEY_CLEAR:
        l_dwInitSecs = (DWORD)FALSE;
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

VOID    SKY_API SKA_ViewADay( SKY_EVENT_T* pEvent )
{   
    UINT8   i;
    DWORD   dwSetTime;
    UINT8           unIdxOfBlockT;
    INT8            nIdxOfEventT;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        /* set the CALENDAR time to the SCHEDULER time */
        dwSetTime = pEvent->dwParam;
        SetScheduleTime(dwSetTime);

        /* alloc. */
        l_pEventRecordT = (SCHL_RECORD_T*)SKY_Malloc((SIZE_T)(sizeof(SCHL_RECORD_T) * SCHL_MAX_EVENTS_ADAY));
        for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
            memset(&l_pEventRecordT[i], NULL, sizeof(SCHL_RECORD_T));

        unIdxOfBlockT = l_SetTime.nDay - 1;
        if( pEvent->wParam == SMET_VIEW_LIST ) /* for EVENT LIST */
            EQS_START_CHILD(sSKA_ListEvent);
        else /* for NEW */
        {            
#if 0
            nIdxOfEventT = GetNewIdxOfEventT(unIdxOfBlockT);
            if(nIdxOfEventT != SCHL_ERROR)
#endif
            {
                //GetANewEvent(unIdxOfBlockT, nIdxOfEventT);
                //EQS_START_CHILD_EVENT(SKA_MakeAEvent, SE_APP_START, unIdxOfBlockT, nIdxOfEventT);
				EQS_START_CHILD(SKA_MakeAEvent);
            }
#if 0
            else
			{
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_FULL_EVENT_DAY),
                               MESSAGE_DEFAULT_TIME,
                               MBT_NONE, 
                               MBT_HELP);
			}
#endif
        }
        break;

    case SE_APP_RESUME:
        if(EQS_GET_CHILD() == SKA_MakeAEvent)
        {
			unIdxOfBlockT = l_SetTime.nDay - 1;
            if(GetEventNumOfBlockT(unIdxOfBlockT))/* if SOME EVENTS, go to LIST */
                EQS_START_CHILD( sSKA_ListEvent );
            else /* if NO EVENTS, go to CALENDAR */
                EQS_END();
        }
        else if(EQS_GET_CHILD() == sSKA_ListEvent)/* return from other applets */
        {
			if(pEvent->wParam == TRUE)/* Recursive Event is deleted */
            {
                /* rebuild */
                MakeSchedule(GetScheduleTime());
				EQS_END();
            }
			else
				EQS_END();
        }
        else if((EQS_GET_CHILD() == SKA_ViewBookContent) ||
           (EQS_GET_CHILD() == SKA_EditBookContent))
        {
            if(pEvent->wParam == TRUE) /* edited */
            {
                /* rebuild */
                MakeSchedule(GetScheduleTime());
				EQS_END();
            }
			else
				EQS_START_CHILD(sSKA_ListEvent);
        }
        break;

    case SE_APP_END:
        /* dealloc.*/
        SKY_Free( l_pEventRecordT );
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

static  VOID SKY_API sSKA_ListEvent( SKY_EVENT_T* pEvent )
{
    UINT8                   i, unIdx;
    BYTE    			    abAnnivItemName[MAX_NUM_ANNIV_ITEM_NAME+1], abStartTime[8];
    HCONTROL                hMessageBox, hControl;
    pb_anniv_info_type      AnnivInfo;
    clk_julian_type         cjStartTime;
	UINT8                   unIdxOfBlockT;
	INT8                    nIdxOfEventT;
    static  BOOL            fRebuildData = FALSE;
    static  UINT8           hList;
    static  INT8            anSortedIdxT[SCHL_MAX_EVENTS_ADAY];
    static  BYTE*           pbEventList[SCHL_MAX_EVENTS_ADAY];
	static  UINT8           unNumOfEvents;
    BYTE                    aszTitle[SCHL_MAX_SIZE_DAY_TITLE];
	BYTE*                   pszStr;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        /* set window */
        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y,
                        SKY_DEF_WIN_DX,
                        SKY_DEF_WIN_DY, 
                        NULL, 
                        NULL, 
                        WA_SHOW_CML);

		l_SaveWorkAddress = SCHL_MAX_EVENTS_ADAY;
        fRebuildData = FALSE;
        unIdxOfBlockT = l_SetTime.nDay - 1;
        unNumOfEvents = GetEventNumOfBlockT(unIdxOfBlockT);

		/* init the text of the scroll list */
		for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
            pbEventList[i] = (BYTE *)SKY_Malloc((SIZE_T)(MAX_SIZE_VIEW_LINE+1));


        /* read the all events */
        if(unNumOfEvents)
        {
            for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++) /* 10건 */
            {                   
                //if(l_BlockT[unIdxOfBlockT].pEvents[i].Type == SRT_ANNIV)/* ANNIVERSARY */                    
                if(GetEventTypeOfBlockT(unIdxOfBlockT, i) == SRT_ANNIV)/* ANNIVERSARY */
                {
                    SKY_SeekReadSchlT(SCHL_IDX_ANNIV_ALARM, &l_pEventRecordT[i]);
 
                    AnnivInfo.address = GetEventAddrOfBlockT(unIdxOfBlockT, i);
                    ui_get_pb(PB_ANNIV_INFO_I, &AnnivInfo);
					if( IsValid( PB_BOOK_INFO_I, AnnivInfo.book_address) )
					{
                        l_pEventRecordT[i].Type = SRT_ANNIV;
						//l_pEventRecordT[i].dwStartTime
						memset(abAnnivItemName, NULL, MAX_NUM_ANNIV_ITEM_NAME+1);
						if(IsMiddleOfMultiByte((BYTE*)g_szPersonName[AnnivInfo.book_address-1], MAX_NUM_ANNIV_ITEM_NAME))
							STRNCPY((BYTE*)abAnnivItemName, (BYTE*)g_szPersonName[AnnivInfo.book_address-1], MAX_NUM_ANNIV_ITEM_NAME-1);
						else
							STRNCPY((BYTE*)abAnnivItemName, (BYTE*)g_szPersonName[AnnivInfo.book_address-1], MAX_NUM_ANNIV_ITEM_NAME);

						if(AnnivInfo.type == 1)
							sprintf((char*)l_pEventRecordT[i].abData, SF_CAKE"%s", abAnnivItemName);
						else if(AnnivInfo.type == 2)
							sprintf((char*)l_pEventRecordT[i].abData, SF_COCKTAIL"%s", abAnnivItemName);
						else if(AnnivInfo.type == 3)
							sprintf((char*)l_pEventRecordT[i].abData, SF_LOVE"%s", abAnnivItemName);
						else if(AnnivInfo.type == 4)
							sprintf((char*)l_pEventRecordT[i].abData, SF_100TH_DAY_ANNIVERSARY"%s", abAnnivItemName);
						else
							sprintf((char*)l_pEventRecordT[i].abData, SF_ANNIVERSARY_TEXT"%s", abAnnivItemName);
					}
					else ;
                }                
                else if(GetEventTypeOfBlockT(unIdxOfBlockT, i) != SRT_NONE)
                    SKY_SeekReadSchlT((UINT8)GetEventAddrOfBlockT(unIdxOfBlockT, i), &l_pEventRecordT[i]);
            }
        }
        else
			; //EQS_END_PARAM((WORD)fRebuildData, 0);

        /* link the sorted event record */
        unIdx = 0;
        hList = CreateList(SCHL_MAX_EVENTS_ADAY);
        if(sSortEvent(anSortedIdxT))
        {
            for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
            {
                if(anSortedIdxT[i] >= 0)
                {
					if(l_pEventRecordT[anSortedIdxT[i]].Type == SRT_ANNIV)
					{
						if(IsMiddleOfMultiByte((BYTE*)l_pEventRecordT[anSortedIdxT[i]].abData, MAX_SIZE_VIEW_LINE))
						{
							STRNCPY(pbEventList[unIdx], l_pEventRecordT[anSortedIdxT[i]].abData, MAX_SIZE_VIEW_LINE-1);
							pbEventList[unIdx][MAX_SIZE_VIEW_LINE-1] = NULL;
						}
						else
						{
							STRNCPY(pbEventList[unIdx], l_pEventRecordT[anSortedIdxT[i]].abData, MAX_SIZE_VIEW_LINE);
							pbEventList[unIdx][MAX_SIZE_VIEW_LINE] = NULL;
						} 
					}
					else
					{
                        /* icon */
#if 0
						if(l_pEventRecordT[anSortedIdxT[i]].Type == SRT_EVENT)
							STRCPY(pbEventList[unIdx], SF_SCHEDULE); //2 bytes
                        else if(l_pEventRecordT[anSortedIdxT[i]].Type == SRT_EVENT_WITH_A)
                            STRCPY(pbEventList[unIdx], SF_SCHEDULE_ALARM); //2 bytes
                        else if(l_pEventRecordT[anSortedIdxT[i]].Type == SRT_EVENT_WITH_R)
                            STRCPY(pbEventList[unIdx], SF_SCHEDULE); //2 bytes
                        else if(l_pEventRecordT[anSortedIdxT[i]].Type == SRT_EVENT_WITH_R_A)
                            STRCPY(pbEventList[unIdx], SF_SCHEDULE_ALARM); //2 bytes
#endif
                        /* time */
                        clk_secs_to_julian(l_pEventRecordT[anSortedIdxT[i]].dwStartTime, &cjStartTime);
                        if(cjStartTime.hour < MOON_TIME)
                            sprintf((char*)abStartTime, "%02d:%02dAM-", cjStartTime.hour, cjStartTime.minute);
                        else
                            sprintf((char*)abStartTime, "%02d:%02dPM-", 
                                    (cjStartTime.hour==MOON_TIME)?cjStartTime.hour:cjStartTime.hour-12, cjStartTime.minute);
                        STRCPY(pbEventList[unIdx], abStartTime);
                        /* text */
						if(IsMiddleOfMultiByte((BYTE*)l_pEventRecordT[anSortedIdxT[i]].abData, MAX_SIZE_VIEW_EVENT_DATA))
						{
							if( l_pEventRecordT[anSortedIdxT[i]].abData[0] )
							{
								STRNCAT(pbEventList[unIdx], l_pEventRecordT[anSortedIdxT[i]].abData, MAX_SIZE_VIEW_EVENT_DATA-1);
							}
							else
							{
								STRCAT(pbEventList[unIdx], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
							}
							pbEventList[unIdx][MAX_SIZE_VIEW_LINE-1] = NULL;
						}
						else
						{

							if( l_pEventRecordT[anSortedIdxT[i]].abData[0] )
							{
								STRNCAT(pbEventList[unIdx], l_pEventRecordT[anSortedIdxT[i]].abData, MAX_SIZE_VIEW_EVENT_DATA);
							}
							else
							{
								STRCAT(pbEventList[unIdx], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
							}
							pbEventList[unIdx][MAX_SIZE_VIEW_LINE] = NULL;
						} 
					}
                    AddList(hList, (INT16)anSortedIdxT[i], FALSE);
                    unIdx++;
                }
            }

        }

        /*
        ** ***************************************************************************
        ** CREATE
        ** ***************************************************************************
        */
        hControl = EQC_ScrollList(EQS_NA, 
								  EQS_NA,
								  EQS_NA,
								  EQC_GetScrollDisplayLine(),
								  pbEventList, 
								  unNumOfEvents,//unIdx, 
								  unNumOfEvents,//unIdx, 
								  0); // unIdx == num of the all data.
        EQC_SET_STYLE(hControl, //EQC_GET_STYLE(hControl) |
                                        CS_SCROLL_LIST_MAXIMIZE         |\
										CS_SCROLL_LIST_NO_CML           |\
										CS_SCROLL_LIST_ENUMBERATED);
        EQC_SetScrollListHandle(hControl, hList);
		/* set the title */
		sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_DAY_TITLE), 
				l_SetTime.nMonth, l_SetTime.nDay, unNumOfEvents );

		EQC_SetScrollTitle(hControl, aszTitle);	
		EQC_ShowScrollTitle(hControl, TRUE);
		//EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);
        EQS_SetIOCActive(hControl);

		if( GetListSize(hList) )
		{
			nIdxOfEventT = (UINT8)GetListValue(hList, 0); /* 0th item */
			if(GetEventTypeOfBlockT(unIdxOfBlockT, nIdxOfEventT) == SRT_ANNIV)
				EQS_SetWindowCML(CL_ALARM_CONFIRM_NEWSCHED);
			else
				EQS_SetWindowCML(CL_DELETE_CONFIRM_NEWSCHED);            
		}
		else
		{
            EQS_SetWindowCML(CL_NONE_NONE_NEWSCHED);
		}
		break;
    case SE_APP_END:
        for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
            SKY_Free(pbEventList[i]);
        break;

    case SE_APP_RESUME:
		hControl = EQS_GetNthIOC(0);		
        if( EQS_GET_CHILD() == sSKA_MakeNewWork || EQS_GET_CHILD() == SKA_MakeAEvent )
		{
			unIdxOfBlockT = l_SetTime.nDay - 1;
			if(l_SaveWorkAddress != SCHL_MAX_EVENTS_ADAY ) 
			{
				unIdx = EQC_GetScrollLogicalIndex(hControl);
				if( GetListSize(hList) > 0 && GetListValue(hList, unIdx) == l_SaveWorkAddress ) //  기존 데이터 수정 
				{
					if( l_BlockT[unIdxOfBlockT].pEvents[l_SaveWorkAddress].Type != SRT_NONE )
					{
						/* time */
						clk_secs_to_julian(l_pEventRecordT[l_SaveWorkAddress].dwStartTime, &cjStartTime);
						if(cjStartTime.hour < MOON_TIME)
							sprintf((char*)abStartTime, "%02d:%02dAM-", cjStartTime.hour, cjStartTime.minute);
						else
							sprintf((char*)abStartTime, "%02d:%02dPM-", 
									(cjStartTime.hour==MOON_TIME)?cjStartTime.hour:cjStartTime.hour-12, cjStartTime.minute);
						STRCPY(pbEventList[unIdx], abStartTime);
						/* text */
						if(IsMiddleOfMultiByte((BYTE*)l_pEventRecordT[l_SaveWorkAddress].abData, MAX_SIZE_VIEW_EVENT_DATA))
						{
							if( l_pEventRecordT[l_SaveWorkAddress].abData[0] )
							{
								STRNCAT(pbEventList[unIdx], l_pEventRecordT[l_SaveWorkAddress].abData, MAX_SIZE_VIEW_EVENT_DATA-1);
							}
							else
							{
								STRCAT(pbEventList[unIdx], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
							}
							pbEventList[unIdx][MAX_SIZE_VIEW_LINE-1] = NULL;
						}
						else
						{
							if( l_pEventRecordT[l_SaveWorkAddress].abData[0] )
							{
								STRNCAT(pbEventList[unIdx], l_pEventRecordT[l_SaveWorkAddress].abData, MAX_SIZE_VIEW_EVENT_DATA);
							}
							else
							{
								STRCAT(pbEventList[unIdx], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
							}
							pbEventList[unIdx][MAX_SIZE_VIEW_LINE] = NULL;
						} 
					}

				}
				else  // 새 데이터 추가 
				{
					if( l_BlockT[unIdxOfBlockT].pEvents[l_SaveWorkAddress].Type != SRT_NONE )
					{
						/* time */
						clk_secs_to_julian(l_pEventRecordT[l_SaveWorkAddress].dwStartTime, &cjStartTime);
						if(cjStartTime.hour < MOON_TIME)
							sprintf((char*)abStartTime, "%02d:%02dAM-", cjStartTime.hour, cjStartTime.minute);
						else
							sprintf((char*)abStartTime, "%02d:%02dPM-", 
									(cjStartTime.hour==MOON_TIME)?cjStartTime.hour:cjStartTime.hour-12, cjStartTime.minute);
						STRCPY(pbEventList[unNumOfEvents], abStartTime);
						/* text */
						if(IsMiddleOfMultiByte((BYTE*)l_pEventRecordT[l_SaveWorkAddress].abData, MAX_SIZE_VIEW_EVENT_DATA))
						{
							if( l_pEventRecordT[l_SaveWorkAddress].abData[0] )
							{
								STRNCAT(pbEventList[unNumOfEvents], l_pEventRecordT[l_SaveWorkAddress].abData, MAX_SIZE_VIEW_EVENT_DATA-1);
							}
							else
							{
								STRCAT(pbEventList[unNumOfEvents], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
							}
							pbEventList[unNumOfEvents][MAX_SIZE_VIEW_LINE-1] = NULL;
						}
						else
						{
							if( l_pEventRecordT[l_SaveWorkAddress].abData[0] )
							{
								STRNCAT(pbEventList[unNumOfEvents], l_pEventRecordT[l_SaveWorkAddress].abData, MAX_SIZE_VIEW_EVENT_DATA);
							}
							else
							{
								STRCAT(pbEventList[unNumOfEvents], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
							}
							pbEventList[unNumOfEvents][MAX_SIZE_VIEW_LINE] = NULL;
						} 
						EQC_AddScrollData(hControl, unNumOfEvents, pbEventList[unNumOfEvents] ,FALSE);
						AddList(hList, l_SaveWorkAddress, EQS_NA);
						unNumOfEvents++;
						sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_DAY_TITLE), 
								l_SetTime.nMonth, l_SetTime.nDay, unNumOfEvents );
						EQC_SetScrollTitle(hControl, aszTitle);	
						EQS_SetWindowCML(CL_DELETE_CONFIRM_NEWSCHED); 
					}
				}
			}
		}
		else if( EQS_GET_CHILD() == SKA_ViewBookContent )
		{

			MakeSchedule(GetScheduleTime());
			for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
			{
				memset(&l_pEventRecordT[i], NULL, sizeof(SCHL_RECORD_T));
			}
			EQS_START_SIBLING(sSKA_ListEvent,FALSE);
			//EQS_END_PARAM(TRUE, 0);  //fRebuildData
			break;

		}
		else if( EQS_GET_CHILD() == SKA_EditBookContent )
		{
			if( pEvent->dwParam )
			{
				MakeSchedule(GetScheduleTime());
				for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
				{
					memset(&l_pEventRecordT[i], NULL, sizeof(SCHL_RECORD_T));
				}
				EQS_START_SIBLING(sSKA_ListEvent,FALSE);
				//EQS_END_PARAM(TRUE, 0);  //fRebuildData
				break;
			}
		}
		else
			;
		l_SaveWorkAddress = SCHL_MAX_EVENTS_ADAY;
		EQS_SetIOCActive(hControl);

		break;
    case SE_SOFTKEY_ADD:
		unIdxOfBlockT = l_SetTime.nDay - 1;
        nIdxOfEventT = GetNewIdxOfEventT( unIdxOfBlockT );
        if(nIdxOfEventT != SCHL_ERROR)
        {
            //GetANewEvent(unIdxOfBlockT, nIdxOfEventT);
            //EQS_START_SIBLING_EVENT(SKA_MakeAEvent, SE_APP_START, unIdxOfBlockT, nIdxOfEventT, FALSE);
			if( GetNewIdxOfSchlT() != SCHL_ERROR &&\
				(GetMyEventNumOfBlockT(l_SetTime.nDay-1)< SCHL_MAX_MYEVNTS_ADAY) )//100개를 넘지않고 하루에 10개를 넘지 않는 경우 생성 
			{
				EQS_START_CHILD(SKA_MakeAEvent);
				break;
			}
			
        }

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_FULL_EVENT_DAY), 
                       MESSAGE_DEFAULT_TIME, 
                       MBT_NONE, 
                       MBT_HELP);

        break;
#if 0
	case SE_IO_SCROLL_LIST_LEFT:
		Set_Schedule_Time(&l_SetTime);
		Calinder_DecreaseADay();
		Get_Schedule_Time(&l_SetTime);
		EQS_START_SIBLING( sSKA_ListEvent, FALSE);
		break;

	case SE_IO_SCROLL_LIST_RIGHT:
		Set_Schedule_Time(&l_SetTime);
		Calinder_IncreaseADay();
		Get_Schedule_Time(&l_SetTime);
		EQS_START_SIBLING( sSKA_ListEvent, FALSE);
		break;
#endif

    case SE_IO_SCROLL_LIST_CHG:
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));            
        nIdxOfEventT = (UINT8)GetListValue( hList, unIdx );
        if(l_pEventRecordT[nIdxOfEventT].Type == SRT_ANNIV)
            EQS_SetWindowCML(CL_ALARM_CONFIRM_NEWSCHED);
        else
            EQS_SetWindowCML(CL_DELETE_CONFIRM_NEWSCHED);
        break;

    case SE_IO_SCROLL_LIST_SELECT:

		unIdxOfBlockT = l_SetTime.nDay - 1;
        l_unIdxOfEvent4View = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0)) + 1;
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));            
        nIdxOfEventT = (UINT8)GetListValue( hList, unIdx ); /* get the address of EventT */

        if( l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type == SRT_ANNIV )
        {
            AnnivInfo.address = l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr;
            if( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) > 0 )
            {
				if( IsValid(PB_BOOK_INFO_I, AnnivInfo.book_address) )
				{
					if(!SKY_IS_PHONE_BOOK_LOCK())
						EQS_START_CHILD_EVENT( SKA_ViewBookContent, SE_APP_START, AnnivInfo.book_address, BFT_EDIT_PBOOK_E);
					else
						EQS_START_CHILD_EVENT( SKA_MainPbookCode, SE_APP_START,  BOOKCODE_IN_SCHEDULE, AnnivInfo.book_address);
				}
				else
					break;
            }                
        }
        else
        {
            EQS_START_CHILD_EVENT( sSKA_MakeNewWork, SE_APP_START, unIdxOfBlockT, nIdxOfEventT);
        }

        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)/* if you just clear the scroll list */
            EQS_END_PARAM((WORD)fRebuildData, 0);
        break;
        
    case SE_SOFTKEY_ALARM:
		unIdxOfBlockT = l_SetTime.nDay - 1;
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        nIdxOfEventT = (UINT8)GetListValue(hList, unIdx); /* get the address of EventT */
        EQS_START_CHILD_EVENT(SKA_SetAnnivAlarm, SE_APP_START, unIdxOfBlockT, nIdxOfEventT);
        break;

    case SE_SOFTKEY_DELETE:
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        nIdxOfEventT = (UINT8)GetListValue(hList, unIdx);
        if((l_pEventRecordT[nIdxOfEventT].Type == SRT_EVENT_WITH_R) ||
           (l_pEventRecordT[nIdxOfEventT].Type == SRT_EVENT_WITH_R_A) ||
		   (l_pEventRecordT[nIdxOfEventT].Type == SRT_USED_EVENT_WITH_R_A))
        {
           EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_DELETE_RECURSIVE), 
                        MESSAGE_DEFAULT_TIME,
                        MBT_YESNO, 
                        MBT_CONFIRM);
        }
        else
        {
           EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_DELETE), 
                        MESSAGE_DEFAULT_TIME,
                        MBT_YESNO, 
                        MBT_CONFIRM);
        }
        break;

        /* DELETE */
    case SE_IO_MESGBOX_YES: 
		hControl = EQS_GetNthIOC(0);
		unIdxOfBlockT = l_SetTime.nDay - 1;
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(hControl);
        nIdxOfEventT = (UINT8)GetListValue(hList, unIdx);
        if((l_pEventRecordT[nIdxOfEventT].Type == SRT_EVENT_WITH_R) ||
           (l_pEventRecordT[nIdxOfEventT].Type == SRT_EVENT_WITH_R_A)||
		   (l_pEventRecordT[nIdxOfEventT].Type == SRT_USED_EVENT_WITH_R_A))
            fRebuildData = TRUE;
        /* delete FILE, Calendar data table */
        sDeleteAEvent(unIdxOfBlockT, nIdxOfEventT);
        /* delete a event of SCROLL LIST */
        CopyRecord(&g_InitRecord, &l_pEventRecordT[nIdxOfEventT]);
        EQC_DeleteScrollData(hControl, unIdx);
        RemoveListItem(hList, unIdx);

		pszStr = pbEventList[unIdx]; // 인덱스 제조정 
		for( i=unIdx; i< SCHL_MAX_EVENTS_ADAY-1; i++)
		{
			pbEventList[i] = pbEventList[i+1];
		}
		pbEventList[i] = pszStr;

        /* set the title */
		unNumOfEvents--;
		sprintf((char*)aszTitle, SKY_GET_TOKEN(TKN_SCHEDULE_DAY_TITLE), 
				l_SetTime.nMonth, l_SetTime.nDay, unNumOfEvents );
		EQC_SetScrollTitle(hControl, aszTitle);	
		
		if( unNumOfEvents == 0 )
		{
			EQS_SetWindowCML(CL_NONE_NONE_NEWSCHED);
		}
		else
		{
			unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));            
			nIdxOfEventT = (UINT8)GetListValue( hList, unIdx );
			if(l_pEventRecordT[nIdxOfEventT].Type == SRT_ANNIV)
				EQS_SetWindowCML(CL_ALARM_CONFIRM_NEWSCHED);
			else
				EQS_SetWindowCML(CL_DELETE_CONFIRM_NEWSCHED);
		}

        hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_DELETE), 
                                     MESSAGE_DEFAULT_TIME,
                                     MBT_NONE,
                                     MBT_SUCCESS);        
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_IO_MESGBOX_END:            
		EQS_SetIOCActive(EQS_GetNthIOC(0));           
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

VOID SKY_API SKA_SetAnnivAlarm(SKY_EVENT_T* pEvent)
{
    UINT8       unCtrl;
    HCONTROL    hMessageBox;
    static  UINT8           unIdxOfBlockT;
    static  INT8            nIdxOfEventT;
    const   BYTE*           pbVaildTitle[1];
    static  clk_julian_type cjAlarmTime;

	SCHL_RECORD_T   *pWriteRecord;

    switch( pEvent->EventID )
    {
    case SE_APP_START:


        /* get the info */
        unIdxOfBlockT = (UINT8)pEvent->wParam;
        nIdxOfEventT = (INT8)pEvent->dwParam;

		/* set window */
        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_SET_SAVE, 
                        (BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_ANNIV_ALARM_TITLE),
                        WA_SHOW_CML | WA_SHOW_HEADER);

        pbVaildTitle[0] = (BYTE *)SKY_GET_TOKEN(TKN_ALARM_MY_HOLIDAY_TEXT);

        /*
        ** ***************************************************************************
        ** CREATE
        ** ***************************************************************************
        */
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SCHEDULE_ANNIV_ALARM_CHECK); //SMAA_CHECK_BOX_ICON        
        EQC_CheckBox(EQS_NA, EQS_NA, (BYTE **)pbVaildTitle, 1); //SMAA_CHECK_BOX
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SCHEDULE_ANNIV_ALARM_TIME); //SMAA_ALARM_TIME_ICON
        EQC_Time(EQS_NA, EQS_NA, 0, TIME_MINUTE_CONTROL); //SMAA_ALARM_TIME

        /*
        ** ***************************************************************************
        ** SET THE VALUE
        ** ***************************************************************************
        */
        if(SKY_GET_SCHL_PRE_CONFIG_ALARM_TYPE(SCHL_IDX_ANNIV_ALARM))
        {
            clk_secs_to_julian(SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(SCHL_IDX_ANNIV_ALARM), &cjAlarmTime);
            EQC_SetCheckBox(EQS_GetNthIOC(SMAA_CHECK_BOX), 0);
            EQC_SetTimeHour(EQS_GetNthIOC(SMAA_ALARM_TIME), (UINT8)cjAlarmTime.hour);
            EQC_SetTimeMinute(EQS_GetNthIOC(SMAA_ALARM_TIME), (UINT8)cjAlarmTime.minute);
        }
        else
        {
            EQC_SetTimeEditMode(EQS_GetNthIOC(SMAA_ALARM_TIME), FALSE);
        }
        //EQS_SetIOCActive(EQS_GetNthIOC(SMAA_CHECK_BOX));

        // Formatting..
        EQS_StartFormatter();        
        for(unCtrl=0; unCtrl<SMAA_MAX; unCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(unCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            if(unCtrl%2 == 1)
                EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();

        break;

    case SE_APP_END:

        break;

    case SE_IO_FOCUS_CHG:
		if( pEvent->wParam == EQS_GetNthIOC(SMAA_CHECK_BOX) )
		{
			if( EQC_GetCheckBox( pEvent->wParam, 0 ) )
			{
				EQS_SetWindowCML(CL_NONE_DISSOLVE_SAVE);
			}
			else
			{
				EQS_SetWindowCML(CL_NONE_SET_SAVE);
			}
		}
		else
		{
			EQS_SetWindowCML(CL_NONE_NONE_SAVE);
		}
		break;
    case SE_IO_CHECK_BOX_CHECK:
        if(pEvent->wParam == EQS_GetNthIOC(SMAA_CHECK_BOX))
        {
            clk_secs_to_julian(SKY_GetLocalTime(), &cjAlarmTime);
            EQC_SetTimeEditMode(EQS_GetNthIOC(SMAA_ALARM_TIME), TRUE);
            EQC_SetTimeHour(EQS_GetNthIOC(SMAA_ALARM_TIME), (UINT8)cjAlarmTime.hour );
            EQC_SetTimeMinute(EQS_GetNthIOC(SMAA_ALARM_TIME), (UINT8)cjAlarmTime.minute );
			EQS_SetWindowCML(CL_NONE_DISSOLVE_SAVE);
        }
        break;

    case SE_IO_CHECK_BOX_UNCHECK:
        if(pEvent->wParam == EQS_GetNthIOC(SMAA_CHECK_BOX))
        {
            EQC_SetTimeEditMode(EQS_GetNthIOC(SMAA_ALARM_TIME), FALSE);
			EQS_SetWindowCML(CL_NONE_SET_SAVE);
        }
        break;
    
    case SE_SOFTKEY_SAVE:        		
        g_AlarmOfAnniv = (SCHL_ALARM_TIME_TYPE_E)EQC_GetCheckBox(EQS_GetNthIOC(SMAA_CHECK_BOX), 0);
        cjAlarmTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SMAA_ALARM_TIME));
        cjAlarmTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SMAA_ALARM_TIME));
        SKY_SET_SCHL_PRE_CONFIG_ALARM_TYPE(SCHL_IDX_ANNIV_ALARM, (SCHL_ALARM_TIME_TYPE_E)g_AlarmOfAnniv);
        SKY_SET_SCHL_PRE_CONFIG_ALARM_TIME(SCHL_IDX_ANNIV_ALARM, clk_julian_to_secs(&cjAlarmTime));

        pWriteRecord = (SCHL_RECORD_T*)SKY_Malloc((SIZE_T)(sizeof(SCHL_RECORD_T)));
        SKY_SeekReadSchlT(SCHL_IDX_ANNIV_ALARM, pWriteRecord);
        pWriteRecord->Alarm.Type = (SCHL_ALARM_TIME_TYPE_E)g_AlarmOfAnniv;
        pWriteRecord->Alarm.dwTime = clk_julian_to_secs(&cjAlarmTime);
        SKY_SeekWriteSchlT(SCHL_IDX_ANNIV_ALARM, pWriteRecord);
        SKY_Free(pWriteRecord);

		hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_SET_ALARM_ANNIV),
				                     MESSAGE_DEFAULT_TIME, 
                                     MBT_NONE, 
                                     MBT_SUCCESS);
        EQC_SET_USERDATA(hMessageBox, pEvent->dwParam);  	
        break;

	case EQE_IO_CHECK_BOX_CANCEL:
    case SE_IO_TIME_CANCEL:
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
    		hMessageBox = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO,
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hMessageBox, SE_KEY_CLEAR);
        }
		else
			EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
			hMessageBox = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO, 
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hMessageBox, SE_KEY_END);
        }
		else
			EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SE_KEY_CLEAR)
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, SE_KEY_CLEAR);
        else if(pEvent->dwParam == SE_KEY_END)
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, SE_KEY_END);
        else
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SE_KEY_END)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else
            EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



VOID    SKY_API SKA_DeleteSomeEvent(SKY_EVENT_T *pEvent)
{
    UINT8       i, DeleteNum, unCtrl, unPosX, unPosY;
    DWORD       dwTempTime;
    MARGIN_T    Margin;
    COORD       xSpace, ySpace;
    BOOL        fDeleted;

    static  BYTE            aszDeleteNum[34];
    static  DWORD           dwStartTime, dwEndTime;
    static  SCHL_TIME_SET_T StartTime, EndTime;
    static  HCONTROL        hEndMessage;


    switch( pEvent->EventID )
    {
    case SE_APP_START:


        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_DELETE_NONE_NONE, 
                      SKY_GET_TOKEN(TKN_SCHEDULE_DELETE_EVENT_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        /*
        ** ***************************************************************************
        ** CREATE
        ** ***************************************************************************
        */
        EQS_StartFormatter();
        EQS_GetControlMargin(&Margin);
        EQS_GetControlSpace(&xSpace, &ySpace);
        EQS_EndFormatter();

        unPosX = Margin.xLeft;  /* (1,25) */
        unPosY = 40; //25;
        EQC_Time(unPosX, unPosY, 0, CAL_TIME_CONTROL ); //SDE_DATE_START
        unPosX = unPosX + EQC_GET_DX(EQS_GetNthIOC(SDE_DATE_START)) + xSpace;
        EQC_StaticText(unPosX, unPosY, TKN_SCHEDULE_DELETE_EVENT_START_TEXT ); //SDE_DATE_START_TEXT
        unPosX = Margin.xLeft;
        unPosY = unPosY + EQC_GET_DY(EQS_GetNthIOC(SDE_DATE_START_TEXT)) + ySpace;
        EQC_Time(unPosX, unPosY, 0, CAL_TIME_CONTROL ); //SDE_DATE_END
        unPosX = unPosX + EQC_GET_DX(EQS_GetNthIOC(SDE_DATE_END)) + xSpace;
        EQC_StaticText(unPosX, unPosY, TKN_SCHEDULE_DELETE_EVENT_END_TEXT ); //SDE_DATE_END_TEXT

        /*
        ** ***************************************************************************
        ** SET THE VALUE
        ** ***************************************************************************
        */
        //SDE_DATE_START
        EQC_SetTimeYear( EQS_GetNthIOC(SDE_DATE_START), (WORD)SCHL_START_YEAR );
        EQC_SetTimeMonth( EQS_GetNthIOC(SDE_DATE_START), (UINT8)1 );
        EQC_SetTimeDay( EQS_GetNthIOC(SDE_DATE_START), (UINT8)1 );

		EQC_SetTimeLower(EQS_GetNthIOC(SDE_DATE_START),SCHL_START_YEAR,\
				                           1,\
										   1,\
										   EQS_NA, EQS_NA);
		EQC_SetTimeUpper(EQS_GetNthIOC(SDE_DATE_START), l_SetTime.wYear,\
				                           l_SetTime.nMonth,\
										   l_SetTime.nDay,\
										   EQS_NA, EQS_NA);	
        //SDE_DATE_END
        EQC_SetTimeYear( EQS_GetNthIOC(SDE_DATE_END), l_SetTime.wYear );
        EQC_SetTimeMonth( EQS_GetNthIOC(SDE_DATE_END), (UINT8)l_SetTime.nMonth );
        EQC_SetTimeDay( EQS_GetNthIOC(SDE_DATE_END), (UINT8)l_SetTime.nDay );

		EQC_SetTimeLower(EQS_GetNthIOC(SDE_DATE_END),SCHL_START_YEAR,\
				                           1,\
										   1,\
										   EQS_NA, EQS_NA);
		EQC_SetTimeUpper(EQS_GetNthIOC(SDE_DATE_END), SCHL_END_YEAR,\
				                           12,\
										   31,\
										   EQS_NA, EQS_NA);	

        /* set the limit of the year */
        //EQC_SetTimeYearLimit(EQS_GetNthIOC(SDE_DATE_START), SCHL_START_YEAR, SCHL_END_YEAR);
        //EQC_SetTimeYearLimit(EQS_GetNthIOC(SDE_DATE_END), SCHL_START_YEAR, SCHL_END_YEAR);

	

        break;

    case SE_APP_END:

        break;

    case SE_SOFTKEY_DELETE:        
        /* set the value */
        //SDE_DATE_START
        StartTime.wYear = EQC_GetTimeYear( EQS_GetNthIOC(SDE_DATE_START) );
        StartTime.nMonth = EQC_GetTimeMonth( EQS_GetNthIOC(SDE_DATE_START) );
        StartTime.nDay = EQC_GetTimeDay( EQS_GetNthIOC(SDE_DATE_START) );

        //SDE_DATE_END
        EndTime.wYear = EQC_GetTimeYear( EQS_GetNthIOC(SDE_DATE_END) );
        EndTime.nMonth = EQC_GetTimeMonth( EQS_GetNthIOC(SDE_DATE_END) );
        EndTime.nDay = EQC_GetTimeDay( EQS_GetNthIOC(SDE_DATE_END) );

        DeleteNum = 0;
        dwStartTime = GetSecsTime( &StartTime ); // get 
        dwEndTime = GetSecsTime( &EndTime );
		dwEndTime = dwEndTime+TIME_SECS_1_DAY;
        for( i=0; i<SCHL_IDX_ANNIV_ALARM; i++ ) // 100 except Anniversary Alarm
        { 
            if( SKY_GET_SCHL_PRE_CONFIG_TYPE(i) )
            {
                dwTempTime = SKY_GET_SCHL_PRE_CONFIG_START_TIME(i);
                if(( dwTempTime >= dwStartTime ) && 
                    ( dwTempTime <= dwEndTime ))
                {
                    DeleteNum++;
                }
            }
        }
        if( DeleteNum )
        {
            sprintf((char*)aszDeleteNum, SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_DELETE_SOME_EVENT), DeleteNum );
            EQC_MessageBox( (BYTE *)aszDeleteNum, MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM );
        }
        else
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_NO_EVENT_DELETE), 
                           MESSAGE_DEFAULT_TIME,
                           MBT_NONE,
                           MBT_HELP);
        }
        break;

    case SE_IO_MESGBOX_YES: // delete

        fDeleted = FALSE;

        // get the time
        dwStartTime = GetSecsTime( &StartTime );
        dwEndTime = GetSecsTime( &EndTime );
        dwEndTime = dwEndTime + TIME_SECS_1_DAY;

        for( i=0; i<SCHL_IDX_ANNIV_ALARM; i++ ) // 100 except Anniversary Alarm
        {
            if( SKY_GET_SCHL_PRE_CONFIG_TYPE(i) )
            {
                if(( SKY_GET_SCHL_PRE_CONFIG_START_TIME(i) >= dwStartTime ) &&
                    (SKY_GET_SCHL_PRE_CONFIG_START_TIME(i) <= dwEndTime ))
                {
                    sDeleteFile( i );
                    fDeleted = TRUE;
                }
            }
        }
        if(fDeleted)
        {
            hEndMessage = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_DELETE),
                                         MESSAGE_DEFAULT_TIME, 
                                         MBT_NONE,
                                         MBT_SUCCESS);
        }
        break;

    case SE_IO_MESGBOX_NO: // cancel
        break;

    case SE_IO_MESGBOX_END:
        if( pEvent->wParam == hEndMessage )
        {
            l_dwInitSecs = SKY_GetLocalTime(); // get the current time.
            MakeSchedule( l_dwInitSecs );    // it makes the data table for the schedule on a month.
            SetCalendarTime( l_dwInitSecs );
            EQS_END();
        }
        break;

    case SE_IO_TIME_SELECT:			
		if( pEvent->wParam == EQS_GetNthIOC(SDE_DATE_START) ) // 시작 날짜 
		{
			EQC_SetTimeLower(EQS_GetNthIOC(SDE_DATE_END), EQC_GetTimeYear(pEvent->wParam),\
				                               EQC_GetTimeMonth(pEvent->wParam),\
											   EQC_GetTimeDay(pEvent->wParam),\
											   EQS_NA, EQS_NA);
		}
		else
		{
			EQC_SetTimeUpper(EQS_GetNthIOC(SDE_DATE_START), EQC_GetTimeYear(pEvent->wParam),\
				                               EQC_GetTimeMonth(pEvent->wParam),\
											   EQC_GetTimeDay(pEvent->wParam),\
											   EQS_NA, EQS_NA);
		}

		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }    
}

/*
** ***************************************************************************
** static Funcrions
** ***************************************************************************
*/

VOID CopyRecord( SCHL_RECORD_T* pSrcRecord, SCHL_RECORD_T*  pDestRecord )
{
    pDestRecord->Type = pSrcRecord->Type;
    pDestRecord->dwStartTime = pSrcRecord->dwStartTime;
    pDestRecord->dwEndTime = pSrcRecord->dwEndTime;
    STRCPY( pDestRecord->abData, pSrcRecord->abData);
    pDestRecord->Recursive.Type = pSrcRecord->Recursive.Type;
    pDestRecord->Recursive.dwEndTime = pSrcRecord->Recursive.dwEndTime;
    pDestRecord->Alarm.Type = pSrcRecord->Alarm.Type;
    pDestRecord->Alarm.dwTime = pSrcRecord->Alarm.dwTime;

    return;
}

VOID MakeSchedule( DWORD dwOneSetTime ) // build Event table/A Block Record
{
    /* set time */
    SetScheduleTime(dwOneSetTime);
    SetCalendarTime(dwOneSetTime);

    /* make table */
    FreeScheduleBlock();
    sBuildBlockWithAnniv(); //From PBook
    sBuildBlockWithEvent();
    BuildMyHoliday();    
    return;
}

BOOL HasAEvent( UINT8 unOneIdxOfBlockT )
{
    if( l_BlockT[unOneIdxOfBlockT].Type )
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

/*
static BOOL sAddAlarmControls(VOID)
{
    return TRUE;
}
*/
static VOID sSaveAEvent(UINT8  unIdxOfBlockT, INT8 nIdxOfEventT)
{
    INT8                    nIdxOfSchlT;
    SCHL_RECORD_TYPE_E      RecordType;
    static  SCHL_RECORD_T   *pWriteRecord;

    // if it set alarm time
    if( l_pEventRecordT[nIdxOfEventT].Type == SRT_ANNIV )
    {
        nIdxOfSchlT = SCHL_IDX_ANNIV_ALARM;
    }
    else
    {
        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = l_pEventRecordT[nIdxOfEventT].Type;
        nIdxOfSchlT = (UINT8)l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr;
    }

    RecordType = (SCHL_RECORD_TYPE_E)SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(nIdxOfSchlT);
    /* set PreConfig table */
    SKY_SET_SCHL_PRE_CONFIG_TYPE(nIdxOfSchlT, l_pEventRecordT[nIdxOfEventT].Type);
    SKY_SET_SCHL_PRE_CONFIG_START_TIME(nIdxOfSchlT, l_pEventRecordT[nIdxOfEventT].dwStartTime);
    SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(nIdxOfSchlT, l_pEventRecordT[nIdxOfEventT].Recursive.Type)
    SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TIME(nIdxOfSchlT, l_pEventRecordT[nIdxOfEventT].Recursive.dwEndTime)
    SKY_SET_SCHL_PRE_CONFIG_ALARM_TYPE(nIdxOfSchlT, l_pEventRecordT[nIdxOfEventT].Alarm.Type);
    SKY_SET_SCHL_PRE_CONFIG_ALARM_TIME(nIdxOfSchlT, l_pEventRecordT[nIdxOfEventT].Alarm.dwTime);
    SKY_SET_SCHL_PRE_CONFIG_ALARM_END_TIME(nIdxOfSchlT, l_pEventRecordT[nIdxOfEventT].Alarm.dwEndTime);
    
    /* save a event */
    pWriteRecord = (SCHL_RECORD_T*)SKY_Malloc((SIZE_T)(sizeof(SCHL_RECORD_T)));
    CopyRecord(&l_pEventRecordT[nIdxOfEventT], pWriteRecord);
    SKY_SeekWriteSchlT(nIdxOfSchlT, pWriteRecord);
    SKY_Free(pWriteRecord);
#if 0
    /* if recursive change, rebuild the calendar table */
    if(RecordType != l_pEventRecordT[nIdxOfEventT].Recursive.Type)
        MakeSchedule(GetScheduleTime());
#endif
	MakeSchedule(GetScheduleTime());


    return;
}

static  BOOL    sSortEvent( INT8  anSortedIdxT[] )
{
    BOOL    fChecked[SCHL_MAX_EVENTS_ADAY], fFind, fSorted;
    UINT8   i,j,k,l;
    DWORD   dwStartTime[SCHL_MAX_EVENTS_ADAY];
    clk_julian_type cjStartTime;
    static  UINT8   unIdxOfMinValue;
    static  DWORD   dwMinValue;

    fSorted = FALSE;
    for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
    {
        if(l_pEventRecordT[i].Type > SRT_NONE)
        {
            fChecked[i] = FALSE;

			if( l_pEventRecordT[i].Type == SRT_ANNIV )
			{
				//dwStartTime[i] = 0xffffffff;
				dwStartTime[i] = 0;
			}
			else
			{
				clk_secs_to_julian(l_pEventRecordT[i].dwStartTime, &cjStartTime);            
				dwStartTime[i] = (cjStartTime.hour*TIME_SECS_1_HOUR) +
					             (cjStartTime.minute*TIME_SECS_1_MIN) +
						         cjStartTime.second;
			}
        }
        else
        {
            fChecked[i] = TRUE;
            dwStartTime[i] = 0;
        }        
    }

    k=0;
    for(i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
    {
        //find the initial small value.
        for(l=0; l<SCHL_MAX_EVENTS_ADAY; l++)
        {
            if(fChecked[l] == FALSE)
            {
                //dwMinValue = l_pEventRecordT[l].dwStartTime;
                dwMinValue = dwStartTime[l];
                unIdxOfMinValue = l;
                fSorted = TRUE;
                break;
            }
        }
        if(l == SCHL_MAX_EVENTS_ADAY)
        {
            break;
        }

        //find the smallest value.
        fFind = FALSE;
        for(j=0; j<SCHL_MAX_EVENTS_ADAY; j++)
        {
            if(fChecked[j] == FALSE)
            {
                //if(l_pEventRecordT[j].dwStartTime < dwMinValue)
                if(dwStartTime[j] < dwMinValue)
                {
                    dwMinValue = dwStartTime[j];
                    unIdxOfMinValue = j;
                    fFind = TRUE;
                    //fChecked[j] = TRUE; //이거 버그로 막음
                }
            }
        }
        if(fFind)
        {
            anSortedIdxT[k] = unIdxOfMinValue;
            fChecked[unIdxOfMinValue] = TRUE;
        }
        else
        {
            anSortedIdxT[k] = unIdxOfMinValue;
            fChecked[unIdxOfMinValue] = TRUE;
        }
        k++;
    }

    if(fSorted)
    {
        for( i=k; i<SCHL_MAX_EVENTS_ADAY; i++ )
        {
            anSortedIdxT[i] = SCHL_ERROR;
        }
    }

    return  fSorted;


}


static VOID sDeleteAEvent( UINT8 unOneIdxOfBlockT, UINT8 unOneIdxOfEventT )
{
    // 1. process the event of day table.
    l_BlockT[unOneIdxOfBlockT].unNumOfEvents--;
    if(l_BlockT[unOneIdxOfBlockT].unNumOfEvents)
    {
        l_BlockT[unOneIdxOfBlockT].pEvents[unOneIdxOfEventT].Type = SRT_NONE;    
        // 1.1 process the event of memo table.
        sDeleteFile((UINT8)l_BlockT[unOneIdxOfBlockT].pEvents[unOneIdxOfEventT].nAddr);
        l_BlockT[unOneIdxOfBlockT].pEvents[unOneIdxOfEventT].nAddr = SCHL_ERROR; //-1
    }
    else // no events
    {
        l_BlockT[unOneIdxOfBlockT].Type = SBRT_NONE;
        // 1.2 process the event of memo table.
        sDeleteFile((UINT8)l_BlockT[unOneIdxOfBlockT].pEvents[unOneIdxOfEventT].nAddr);
        SKY_Free(l_BlockT[unOneIdxOfBlockT].pEvents);
        l_BlockT[unOneIdxOfBlockT].pEvents = NULL;
    }

    return;
}

static VOID sDeleteFile( UINT8 unOneIdxOfSchlT )
{
    static SCHL_RECORD_T *pWriteRecord;

    pWriteRecord = (SCHL_RECORD_T*)SKY_Malloc( (SIZE_T)(sizeof(SCHL_RECORD_T)) );

    // init a record.
    CopyRecord( &g_InitRecord, pWriteRecord );

    /*
    ** ***************************************************************************
    ** PRE-CONFIG TABLE
    ** ***************************************************************************
    */
    SKY_SET_SCHL_PRE_CONFIG_TYPE(unOneIdxOfSchlT, pWriteRecord->Type);
    SKY_SET_SCHL_PRE_CONFIG_START_TIME(unOneIdxOfSchlT, pWriteRecord->dwStartTime);
    SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(unOneIdxOfSchlT, pWriteRecord->Recursive.Type);
    SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TIME(unOneIdxOfSchlT, pWriteRecord->Recursive.dwEndTime);
    SKY_SET_SCHL_PRE_CONFIG_ALARM_TYPE(unOneIdxOfSchlT, pWriteRecord->Alarm.Type);
    SKY_SET_SCHL_PRE_CONFIG_ALARM_TIME(unOneIdxOfSchlT, pWriteRecord->Alarm.dwTime);
    SKY_SET_SCHL_PRE_CONFIG_ALARM_END_TIME(unOneIdxOfSchlT, pWriteRecord->Alarm.dwEndTime);

    /*
    ** ***************************************************************************
    ** DATA TABLE
    ** ***************************************************************************
    */
    SKY_SeekWriteSchlT( unOneIdxOfSchlT, pWriteRecord );

    SKY_Free( pWriteRecord );
    return;
}


VOID Init_Schedule_Block(void)
{
    INT16 i;

    /* init a block */
    for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ ) // 31 days
    {
        l_BlockT[i].Type = SBRT_NONE;
        l_BlockT[i].unNumOfEvents = 0;
		l_BlockT[i].unNumOfAnniv  = 0;
        l_BlockT[i].pEvents = NULL;
    }
}


VOID FreeScheduleBlock( VOID )
{
    INT16 i;
    /* init a block */
    for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ ) // 31 days
    {
		if( l_BlockT[i].pEvents != NULL )
		{
			l_BlockT[i].Type = SBRT_NONE;
			l_BlockT[i].unNumOfEvents = 0;
			l_BlockT[i].unNumOfAnniv  = 0;
			SKY_Free(l_BlockT[i].pEvents);
			l_BlockT[i].pEvents = NULL;
		}
    }
}

static  VOID    sBuildBlockWithAnniv( VOID )
{
    UINT8               i, j, nCount;
    UINT8               unIdxOfBlockT;
    INT8                nIdxOfEventT;

    INT8                nAnnivMonth;
    UINT16              nIdxOfSearch;
    pb_anniv_info_type  AnnivInfo;
    SCHL_FIND_SOLAR_DAY_T FindSolarDay;

	BOOL                faCheckAnniv[MAX_ANNIV_NUMBER];
	INT16               nIndex;

    for(nIndex=0; nIndex< MAX_ANNIV_NUMBER; nIndex++)
	{
		faCheckAnniv[nIndex] = FALSE;
	}
	
    nAnnivMonth = l_SetTime.nMonth;
    nAnnivMonth = nAnnivMonth - SCHL_DIFF_LUNAL_CAL;

    if( nAnnivMonth < 1 )
    {
        //nAnnivMonth = nAnnivMonth + 12;
		nAnnivMonth = nAnnivMonth + 12 + 1;
    }
    
    for(i=0; i<=SCHL_DIFF_LUNAL_CAL; i++ )
    {
        if( nAnnivMonth > 12 )
        {
            nAnnivMonth -= 12;
        }

        AnnivInfo.address = active_list.anniv_info[nAnnivMonth-1]; //solar

        while( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) > 0 && faCheckAnniv[AnnivInfo.address-1] == FALSE )
        {
            if( AnnivInfo.fLunar ) //lunar
            {
                nIdxOfSearch = 0;
                GetSolarDay( &AnnivInfo.month, &AnnivInfo.day, &nIdxOfSearch, &FindSolarDay );
				nCount = 0;
                while( FindSolarDay.fFind && nCount < 31)
                {
					nCount++;
                    if( FindSolarDay.unSMonth == l_SetTime.nMonth )
                    {
						faCheckAnniv[AnnivInfo.address-1] = TRUE; // find and check

                        unIdxOfBlockT = FindSolarDay.unSDay - 1;
						if( l_BlockT[unIdxOfBlockT].unNumOfAnniv >= SCHL_MAX_ANNIV_ADAY  ) //redstar 01.07.01
							break;
                        nIdxOfEventT = GetNewIdxOfEventT(unIdxOfBlockT);
                        if(nIdxOfEventT != SCHL_ERROR)
                        {
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = SRT_ANNIV;
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = AnnivInfo.address;
                            l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                            l_BlockT[unIdxOfBlockT].unNumOfAnniv++;
                            l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
							break;
#if 0
                            /* 반복된 기념일 데이터인지 확인한다 */
                            for(j=0; j<SCHL_MAX_EVENTS_ADAY; j++)
                            {
                                if(AnnivInfo.address == l_BlockT[unIdxOfBlockT].pEvents[j].nAddr)
                                    break;
                            }
                            if(j == SCHL_MAX_EVENTS_ADAY)
                            {
                                l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = SRT_ANNIV;
                                l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = AnnivInfo.address;
                                l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                                l_BlockT[unIdxOfBlockT].unNumOfAnniv++;
                                l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                            }
                            else /* error */
                                break;
#endif
                        }
                        else
                        {
                            //SKY_ASSERT( nIdxOfEventT != SCHL_ERROR );
                            break;
                        }
                    }
                    nIdxOfSearch = FindSolarDay.nIdxOfTable + 1; // next search
                    GetSolarDay( &AnnivInfo.month, &AnnivInfo.day, &nIdxOfSearch, &FindSolarDay );
                }
            }
            else //solar
            {
                if( AnnivInfo.month == l_SetTime.nMonth )
                {
					faCheckAnniv[AnnivInfo.address-1] = TRUE; // find and check

                    unIdxOfBlockT = AnnivInfo.day - 1;
					if( l_BlockT[unIdxOfBlockT].unNumOfAnniv >= SCHL_MAX_ANNIV_ADAY  ) //redstar 01.07.01
						break;
                    nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                    if(nIdxOfEventT != SCHL_ERROR)
                    {

                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = SRT_ANNIV; 
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = AnnivInfo.address;
                        l_BlockT[unIdxOfBlockT].unNumOfEvents++;
						l_BlockT[unIdxOfBlockT].unNumOfAnniv++;
                        l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
#if 0
                        /* 반복된 기념일 데이터인지 확인한다 */
                        for(j=0; j<SCHL_MAX_EVENTS_ADAY; j++)
                        {
                            if(AnnivInfo.address == l_BlockT[unIdxOfBlockT].pEvents[j].nAddr)
                                break;
                        }
                        if(j == SCHL_MAX_EVENTS_ADAY)
                        {
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = SRT_ANNIV; 
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = AnnivInfo.address;
                            l_BlockT[unIdxOfBlockT].unNumOfEvents++;
							l_BlockT[unIdxOfBlockT].unNumOfAnniv++;
                            l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                        }
                        else/* error */
                            break;
#endif
                    }
                    else
                    {
                        //SKY_ASSERT( nIdxOfEventT != SCHL_ERROR );
                        break;
                    }
                }                
            }
            AnnivInfo.address = AnnivInfo.link;
        } // while
        nAnnivMonth++;
    } // for
    return;
}

static  VOID    sBuildBlockWithEvent( VOID )
{
    INT8                nIdxOfEventT, nDayOfWeek;
    UINT8               i, j, unIdxOfBlockT;
   
    clk_julian_type     cjStartTime, cjRecursiveDate;
    SCHL_RECORD_TYPE_E  TypeOfRecord;
    SCHL_EVENT_RECURSIVE_TYPE_E RecursiveType;

    for(i=0; i<SCHL_MAX_RECORD_COUNT; i++) // 101
    {
        TypeOfRecord = SKY_GET_SCHL_PRE_CONFIG_TYPE(i);
        switch( TypeOfRecord )
        {
        case SRT_EVENT:
        case SRT_EVENT_WITH_A:
		case SRT_USED_EVENT_WITH_A:
            clk_secs_to_julian( SKY_GET_SCHL_PRE_CONFIG_START_TIME(i), &cjStartTime );
            /* if it's same month */
            if((cjStartTime.year == l_SetTime.wYear) && (cjStartTime.month == l_SetTime.nMonth)) 
            {
                unIdxOfBlockT = cjStartTime.day - 1;                
                nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                if( nIdxOfEventT != SCHL_ERROR )
                {
                    l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                    l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                    l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                    l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                }
                else
                {
                    //SKY_ASSERT( nIdxOfEventT != SCHL_ERROR );
                    break;
                }
            }
            break;
        case SRT_EVENT_WITH_R:
        case SRT_EVENT_WITH_R_A:
		case SRT_USED_EVENT_WITH_R_A:
            RecursiveType = SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(i);
            clk_secs_to_julian(SKY_GET_SCHL_PRE_CONFIG_START_TIME(i), &cjStartTime);
            clk_secs_to_julian(SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i), &cjRecursiveDate);

			if( RecursiveType == SERT_EVERY_YEAR )
			{
				if( l_SetTime.wYear >=cjStartTime.year && l_SetTime.wYear <= cjRecursiveDate.year &&\
					l_SetTime.nMonth == cjStartTime.month )
					//l_SetTime.nDay   == cjStartTime.day )			       
				{
                    unIdxOfBlockT = cjStartTime.day - 1;                
                    nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                    if( nIdxOfEventT != SCHL_ERROR )
                    {
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                        l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                        l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                    }
					break;
				}
				else
					break;
			}

            /* case 1. in */
            if( ((l_SetTime.wYear == cjStartTime.year) &&
                 (l_SetTime.nMonth == cjStartTime.month)) 
                 &&
                ((l_SetTime.wYear == cjRecursiveDate.year) &&
                 (l_SetTime.nMonth == cjRecursiveDate.month)) )
            {
                while(cjStartTime.day <= cjRecursiveDate.day)
                {
                    unIdxOfBlockT = cjStartTime.day - 1;                
                    nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                    if( nIdxOfEventT != SCHL_ERROR )
                    {
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                        l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                        l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                    }
                    if(RecursiveType == SERT_EVERY_DAY)
                        cjStartTime.day++;
                    else if(RecursiveType == SERT_EVERY_WEEK)
                        cjStartTime.day = cjStartTime.day + 7;
                    else if(RecursiveType == SERT_EVERY_MONTH)
                        break;
					else 
						break;
                }
            }
            /* case 2.overlap after */
            else if( ((cjStartTime.year == l_SetTime.wYear) &&
                      (cjStartTime.month == l_SetTime.nMonth))
                      &&
                     (((cjRecursiveDate.year == l_SetTime.wYear) &&
                       (cjRecursiveDate.month > l_SetTime.nMonth)) ||
                      (cjRecursiveDate.year > l_SetTime.wYear)) )
            {
                while(cjStartTime.day <= GetDaysOfMonth(l_SetTime.wYear, l_SetTime.nMonth)) /* to 31 */
                {
                    unIdxOfBlockT = cjStartTime.day - 1;                
                    nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                    if( nIdxOfEventT != SCHL_ERROR )
                    {
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                        l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                        l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                    }
                    if(RecursiveType == SERT_EVERY_DAY)
                        cjStartTime.day++;
                    else if(RecursiveType == SERT_EVERY_WEEK)
                        cjStartTime.day = cjStartTime.day + 7;
                    else if(RecursiveType == SERT_EVERY_MONTH)
                        break;
                }

            }
            /* case 3.overlap before */
            else if( (((cjStartTime.year == l_SetTime.wYear) &&                      
                       (cjStartTime.month < l_SetTime.nMonth)) ||
                      (cjStartTime.year < l_SetTime.wYear))
                      &&
                      ((cjRecursiveDate.year == l_SetTime.wYear) &&
                       (cjRecursiveDate.month == l_SetTime.nMonth)) )                  
            {

                if(RecursiveType == SERT_EVERY_DAY)
                {
                    cjStartTime.day = 1;
                    while(cjStartTime.day <= cjRecursiveDate.day)
                    {
                        unIdxOfBlockT = cjStartTime.day - 1;                
                        nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                        if( nIdxOfEventT != SCHL_ERROR )
                        {
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                            l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                            l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                        }
                        cjStartTime.day++;
                    }
                }
                else if(RecursiveType == SERT_EVERY_WEEK)
                {
                    //nDayOfWeek = (((l_SetTime.nDayOfWeek + 1) % 7)  + (8 - (l_SetTime.nDay % 7))) % 7;
                    nDayOfWeek = ((l_SetTime.nDayOfWeek % 7)  + (8 - (l_SetTime.nDay % 7))) % 7;
                    for(j=1; j<=GetDaysOfMonth(l_SetTime.wYear, l_SetTime.nMonth); j++)
                    {
                        if(nDayOfWeek != cjStartTime.day_of_week)
                        {
                            nDayOfWeek++;
                            if(nDayOfWeek == 7)
                                nDayOfWeek = 0;
                        }
                        else
                        {
                            cjStartTime.day = j;
                            break;
                        }
                    }
                    while(cjStartTime.day <= cjRecursiveDate.day)
                    {
                        unIdxOfBlockT = cjStartTime.day - 1;                
                        nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                        if( nIdxOfEventT != SCHL_ERROR )
                        {
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                            l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                            l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                        }
                        cjStartTime.day = cjStartTime.day + 7;
                    }
                }
                else if(RecursiveType == SERT_EVERY_MONTH)
                {
                    while(cjStartTime.day <= cjRecursiveDate.day)
                    {
                        unIdxOfBlockT = cjStartTime.day - 1;                
                        nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                        if( nIdxOfEventT != SCHL_ERROR )
                        {
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                            l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                            l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                        }
                        break;
                    }
                    break;
                }
            }
            /* case 4.contained */
            else if( (((cjStartTime.year == l_SetTime.wYear) &&                      
                       (cjStartTime.month < l_SetTime.nMonth)) ||
                      (cjStartTime.year < l_SetTime.wYear))
                      &&
                     (((cjRecursiveDate.year == l_SetTime.wYear) &&
                       (cjRecursiveDate.month > l_SetTime.nMonth)) ||
                      (cjRecursiveDate.year > l_SetTime.wYear)) )
            {
                if(RecursiveType == SERT_EVERY_DAY)
                {
                    cjStartTime.day = 1;
                    while(cjStartTime.day <= GetDaysOfMonth(l_SetTime.wYear, l_SetTime.nMonth))
                    {
                        unIdxOfBlockT = cjStartTime.day - 1;                
                        nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                        if( nIdxOfEventT != SCHL_ERROR )
                        {
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                            l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                            l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                        }
                        cjStartTime.day++;
                    }
                    cjStartTime.day++;
                }
                else if(RecursiveType == SERT_EVERY_WEEK)
                {
                    //nDayOfWeek = (((l_SetTime.nDayOfWeek + 1) % 7)  + (8 - (l_SetTime.nDay % 7))) % 7;
                    nDayOfWeek = ((l_SetTime.nDayOfWeek % 7)  + (8 - (l_SetTime.nDay % 7))) % 7;
                    for(j=1; j<=GetDaysOfMonth(l_SetTime.wYear, l_SetTime.nMonth); j++)
                    {
                        if(nDayOfWeek != cjStartTime.day_of_week)
                        {
                            nDayOfWeek++;
                            if(nDayOfWeek == 7)
                                nDayOfWeek = 0;
                        }
                        else
                        {
                            cjStartTime.day = j;
                            break;
                        }
                    }
                    while(cjStartTime.day <= GetDaysOfMonth(l_SetTime.wYear, l_SetTime.nMonth))
                    {
                        unIdxOfBlockT = cjStartTime.day - 1;                
                        nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                        if( nIdxOfEventT != SCHL_ERROR )
                        {
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                            l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                            l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                            l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                        }
                        cjStartTime.day = cjStartTime.day + 7;
                    }
                }
                else if(RecursiveType == SERT_EVERY_MONTH)
                {
                    unIdxOfBlockT = cjStartTime.day - 1;                
                    nIdxOfEventT = GetNewIdxOfEventT( (UINT8)unIdxOfBlockT );
                    if( nIdxOfEventT != SCHL_ERROR )
                    {
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = TypeOfRecord;
                        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = i;
                        l_BlockT[unIdxOfBlockT].unNumOfEvents++;
                        l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
                    }
                    break;
                }
            }
            break;
        }
    }
    return;
}


INT8 GetNewIdxOfEventT( UINT8 unIdxOfBlockT )
{
    INT8    i;

    if( l_BlockT[unIdxOfBlockT].unNumOfEvents )
    {
        for( i=0; i<SCHL_MAX_EVENTS_ADAY; i++)
        {
            // we search the empty record of event table.
            if( l_BlockT[unIdxOfBlockT].pEvents[i].Type == SRT_NONE )
            {
                return i;
            }
        }
        //if i == 50, it's error. it's too many events here.
        
        return SCHL_ERROR; //-1
    }
    else
    {
        l_BlockT[unIdxOfBlockT].pEvents = 
                    (SCHL_EVENT_RECORD_T*)SKY_Malloc((SIZE_T)(sizeof(SCHL_EVENT_RECORD_T)*SCHL_MAX_EVENTS_ADAY));
        for( i=0; i<SCHL_MAX_EVENTS_ADAY; i++ )
        {
            l_BlockT[unIdxOfBlockT].pEvents[i].Type = SRT_NONE;
            l_BlockT[unIdxOfBlockT].pEvents[i].nAddr = SCHL_ERROR;
        }
        return (INT8)0;
    }
}


INT8 GetNewIdxOfSchlT( VOID )
{
    INT8 i;
    //SCHL_EVENTS_TYPE_E SchlEventType;
 
    for( i=0; i<SCHL_MAX_RECORD_COUNT; i++ ) //SCHL_MAX_RECORD_COUNT = 101
    {
        if( SKY_GET_SCHL_PRE_CONFIG_TYPE(i) == SRT_NONE )
        {
            return i;
        }
    }    
    return SCHL_ERROR; // -1
}

VOID GetANewEvent( UINT8  unIdxOfBlockT, INT8 nIdxOfEventT )
{

    INT8            nIdxOfSchlT;
    DWORD           dwSetTime;
    clk_julian_type cjSetTime;

    // 1) create the DB, global table.
    nIdxOfSchlT = GetNewIdxOfSchlT();  // idx of DB.
    if( nIdxOfSchlT != SCHL_ERROR )
    {
        dwSetTime = SKY_GetLocalTime();
        clk_secs_to_julian( dwSetTime, &cjSetTime );

        /* set date to view day */
        cjSetTime.year = l_SetTime.wYear;
        cjSetTime.month = l_SetTime.nMonth;
        cjSetTime.day_of_week = l_SetTime.nDayOfWeek;
        cjSetTime.day = l_SetTime.nDay;
        dwSetTime = clk_julian_to_secs( &cjSetTime );

        /* make a new local event record */
        CopyRecord(&g_InitRecord, &l_pEventRecordT[nIdxOfEventT]);

        l_pEventRecordT[nIdxOfEventT].dwStartTime = dwSetTime;
        l_pEventRecordT[nIdxOfEventT].dwEndTime = dwSetTime + DIFF_SECS_EVENT_TIME_START_N_END; /* set after 10min */
        l_pEventRecordT[nIdxOfEventT].Recursive.dwEndTime = dwSetTime;
        l_pEventRecordT[nIdxOfEventT].Alarm.dwTime = dwSetTime;
        l_pEventRecordT[nIdxOfEventT].Alarm.dwEndTime = dwSetTime;

        /* make a new local block record */
        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].Type = l_pEventRecordT[nIdxOfEventT].Type; 
        l_BlockT[unIdxOfBlockT].pEvents[nIdxOfEventT].nAddr = nIdxOfSchlT;
        /* after inserting the new event */
        l_BlockT[unIdxOfBlockT].unNumOfEvents++;        
        l_BlockT[unIdxOfBlockT].Type = SBRT_EVENT;
    }
    else //error
    {
        EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_FULL_EVENT_ALL),MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP );
    }

    return;
}

DWORD   GetScheduleTime( VOID )
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

VOID    SetScheduleTime( DWORD  dwOneSetTime )
{
    clk_julian_type cjSetTime;

    clk_secs_to_julian( dwOneSetTime, &cjSetTime );    
    /* set l_SetTime */
    l_SetTime.wYear = (WORD)cjSetTime.year;
    l_SetTime.nMonth = (INT8)cjSetTime.month;
    l_SetTime.nDay = (INT)cjSetTime.day;
    l_SetTime.nDayOfWeek = (INT8)cjSetTime.day_of_week;
    l_SetTime.nHour = (INT8)cjSetTime.hour;
    l_SetTime.nMinute = (INT8)cjSetTime.minute;
    l_SetTime.nSecond = (INT8)cjSetTime.second;
}

SCHL_BLOCK_RECORD_TYPE_E GetTypeOfBlockT(UINT8 unIdxOfDay)
{
    return  l_BlockT[unIdxOfDay].Type;
}

UINT8 GetEventNumOfBlockT(UINT8 unIdxOfDay)
{
    return l_BlockT[unIdxOfDay].unNumOfEvents;
}

SCHL_RECORD_TYPE_E GetEventTypeOfBlockT(UINT8 unIdxOfDay, UINT8 unIdxOfEvent)
{
    return  l_BlockT[unIdxOfDay].pEvents[unIdxOfEvent].Type;
}

INT16 GetEventAddrOfBlockT(UINT8 unIdxOfDay, UINT8 unIdxOfEvent)
{
    return  l_BlockT[unIdxOfDay].pEvents[unIdxOfEvent].nAddr;
}

UINT8 GetMyEventNumOfBlockT(UINT8 unIdxOfDay)
{
    UINT8   i, NumOfEvents;

    NumOfEvents = 0;
    for(i=0; i<SCHL_MAX_EVENTS_ADAY ; i++) // 10건
    {
        /* if not SRT_NONE & SRT_ANNIV */
        if(l_BlockT[unIdxOfDay].pEvents != NULL)
        {
            if((l_BlockT[unIdxOfDay].pEvents[i].Type != SRT_NONE) &&
               (l_BlockT[unIdxOfDay].pEvents[i].Type != SRT_ANNIV))
                NumOfEvents++;
        }
    }
    return NumOfEvents;
}

BOOL HaveAnnivOfBlockT(UINT8 unIdxOfDay)
{
    if(GetEventNumOfBlockT(unIdxOfDay) != GetMyEventNumOfBlockT(unIdxOfDay))
        return TRUE;
    else
        return FALSE;
}

BOOL HaveMyEventOfBlockT(UINT8  unIdxOfDay)
{
	if(GetMyEventNumOfBlockT(unIdxOfDay))
        return TRUE;
    else
        return FALSE;
}



DWORD   GetSecsTime( SCHL_TIME_SET_T* pSearchDay )
{
    UINT16			unDaySum;    
	SCHL_TIME_SET_T	CompTime;
	clk_julian_type	cjGetTime;
	static  DWORD   dwSetTime;
    
    //dwSetTime = GetScheduleTime();
    cjGetTime.year = l_SetTime.wYear;
    cjGetTime.month = l_SetTime.nMonth;
    cjGetTime.day = l_SetTime.nDay;
    cjGetTime.day_of_week = l_SetTime.nDayOfWeek;
    cjGetTime.hour = 0;
    cjGetTime.minute = 0;
    cjGetTime.second = 0;
	dwSetTime = clk_julian_to_secs(&cjGetTime);

    CompTime.wYear = l_SetTime.wYear;
    CompTime.nMonth = l_SetTime.nMonth;
    CompTime.nDay = l_SetTime.nDay;
    //CompTime.nDayOfWeek = l_SetTime.nDayOfWeek;
    //CompTime.nHour = l_SetTime.nHour;
    //CompTime.nMinute = l_SetTime.nMinute;
    //CompTime.nSecond = l_SetTime.nSecond;

    if( (CompTime.wYear != pSearchDay->wYear) || (CompTime.nMonth != pSearchDay->nMonth) || (CompTime.nDay != pSearchDay->nDay) )
    {
        /* past */
        if(( CompTime.wYear > pSearchDay->wYear ) ||
            (( CompTime.wYear == pSearchDay->wYear ) && ( CompTime.nMonth > pSearchDay->nMonth )) ||
            (( CompTime.wYear == pSearchDay->wYear ) && ( CompTime.nMonth == pSearchDay->nMonth ) && ( CompTime.nDay > pSearchDay->nDay )))
        {
            unDaySum = CompTime.nDay;
            while( (CompTime.wYear != pSearchDay->wYear) || (CompTime.nMonth != pSearchDay->nMonth) )
            {
                if( CompTime.nMonth > 1)
                {
                    CompTime.nMonth--;
                }
                else
                {
                    CompTime.wYear--;
                    CompTime.nMonth = 12;
                }
                unDaySum = unDaySum + GetDaysOfMonth( CompTime.wYear, CompTime.nMonth );
            }
            unDaySum = unDaySum - pSearchDay->nDay;
            dwSetTime = dwSetTime - unDaySum * TIME_SECS_1_DAY;
        }        
        else/* future */
        {
            unDaySum = GetDaysOfMonth( CompTime.wYear, CompTime.nMonth );
            while( (CompTime.wYear != pSearchDay->wYear) || (CompTime.nMonth != pSearchDay->nMonth) )
            {
                if( CompTime.nMonth < 12 )
                {
                    CompTime.nMonth++;
                }
                else
                {
                    CompTime.wYear++;
                    CompTime.nMonth = 1;
                }
                unDaySum = unDaySum + GetDaysOfMonth( CompTime.wYear, CompTime.nMonth );
            }
            unDaySum = unDaySum - 
                ( ( GetDaysOfMonth( CompTime.wYear, CompTime.nMonth ) - pSearchDay->nDay ) + CompTime.nDay );
            dwSetTime = dwSetTime + unDaySum * TIME_SECS_1_DAY;
        }
    }

    return  dwSetTime;
}

UINT8 GetSchedule( DWORD dwToday, SCHL_RECORD_T* pEventRecordT, UINT8 unIdxOfView )
{
	UINT8           i, unIdx;
    clk_julian_type cjToday, cjEvent;
    UINT8           aunIdxT[SCHL_MAX_EVENTS_ADAY];
    static  SCHL_PRE_CONFIG_T   TodayEventT[SCHL_MAX_EVENTS_ADAY];
    static  INT8                anSortedIdxT[SCHL_MAX_EVENTS_ADAY];

    clk_secs_to_julian( dwToday, &cjToday );

    /* alloc. */
    //*pEventRecordT = (SCHL_RECORD_T*)SKY_Malloc( (SIZE_T)(sizeof(SCHL_RECORD_T) * SCHL_MAX_EVENTS_ADAY) );
    //pTempRecordT = *pEventRecordT;
    //for( i=0; i<SCHL_MAX_EVENTS_ADAY; i++ )
    //{
    //    memset( &pTempRecordT[i], 0, sizeof(SCHL_RECORD_T) );
    //}

    unIdx = 0;
	/* search the events of today */
    for( i=0; i<SCHL_MAX_RECORD_COUNT; i++ ) // 101
    {
        if( SKY_GET_SCHL_PRE_CONFIG_TYPE( i ) )
        {
            clk_secs_to_julian( SKY_GET_SCHL_PRE_CONFIG_START_TIME(i), &cjEvent );

			if( unIdx < SCHL_MAX_EVENTS_ADAY )
			{
				/* only today event */
				if( ( cjEvent.year == cjToday.year ) &&
					( cjEvent.month == cjToday.month ) &&
					( cjEvent.day == cjToday.day ) ) 
				{
					TodayEventT[unIdx].Type = SKY_GET_SCHL_PRE_CONFIG_TYPE(i);
					TodayEventT[unIdx].dwStartTime = SKY_GET_SCHL_PRE_CONFIG_START_TIME(i);
                    TodayEventT[unIdx].Recursive.Type = SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(i);
                    TodayEventT[unIdx].Recursive.dwEndTime = SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i);
                    TodayEventT[unIdx].Alarm.Type = SKY_GET_SCHL_PRE_CONFIG_ALARM_TYPE(i);
					TodayEventT[unIdx].Alarm.dwTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(i);
                    TodayEventT[unIdx].Alarm.dwEndTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_END_TIME(i);

					//TodayEventT[unIdx].nAddr = SKY_GET_SCHL_PRE_CONFIG_ADDR(i);
                    aunIdxT[unIdx] = i;
					unIdx++;
				}
			}
        }
    }
    /* sort */
    if(sSortEventToday(TodayEventT, anSortedIdxT))
    {
        //SKY_SeekReadSchlT( (UINT8)TodayEventT[anSortedIdxT[unIdxOfView]].nAddr, pEventRecordT );
        SKY_SeekReadSchlT( (UINT8)aunIdxT[anSortedIdxT[unIdxOfView]], pEventRecordT );
        return unIdx;
    }
    else
    {
        return (UINT8)0;
    }
}

static BOOL sSortEventToday(SCHL_PRE_CONFIG_T pTodayEventT[], INT8 anSortedIdxT[])
{
    BOOL    fChecked[SCHL_MAX_EVENTS_ADAY], fFind, fSorted;
    UINT8   i,j,k,l;
    static  UINT8   unIdxOfMinValue;
    static  DWORD   dwMinValue;

    fSorted = FALSE;
    for( i=0; i<SCHL_MAX_EVENTS_ADAY; i++ )
    {
        if( pTodayEventT[i].Type > SRT_NONE )
        {
            fChecked[i] = FALSE;
        }
        else
        {
            fChecked[i] = TRUE;
        }
    }

    k=0;
    for( i=0; i<SCHL_MAX_EVENTS_ADAY; i++ )
    {
        //find the initial small value.
        for( l=0; l<SCHL_MAX_EVENTS_ADAY; l++ )
        {
            if( fChecked[l] == FALSE )
            {
                dwMinValue = pTodayEventT[l].dwStartTime;
                unIdxOfMinValue = l;
                fSorted = TRUE;
                break;
            }
        }
        if( l == SCHL_MAX_EVENTS_ADAY )
        {
            break;
        }

        //find the smallest value.
        fFind = FALSE;
        for( j=0; j<SCHL_MAX_EVENTS_ADAY; j++ )
        {
            if( fChecked[j] == FALSE )
            {
                if( pTodayEventT[j].dwStartTime < dwMinValue )
                {
                    dwMinValue = pTodayEventT[j].dwStartTime;
                    unIdxOfMinValue = j;
                    fFind = TRUE;
                    fChecked[j] = TRUE;
                }
            }
        }
        if( fFind )
        {
            anSortedIdxT[k] = unIdxOfMinValue;
        }
        else
        {
            anSortedIdxT[k] = unIdxOfMinValue;
            fChecked[unIdxOfMinValue] = TRUE;
        }
        k++;
    }

    if( fSorted )
    {
        for( i=k; i<SCHL_MAX_EVENTS_ADAY; i++ )
        {
            anSortedIdxT[i] = SCHL_ERROR;
        }
    }

    return  fSorted;
}

UINT8 Get12HourTime(UINT8 un24HourTime)
{
    if(un24HourTime > MOON_TIME)
        return (un24HourTime - MOON_TIME);
    else
        return un24HourTime;
}

UINT8 Get24HourTime(UINT8 un12HourTime, BOOL f24Hour)
{
    if(f24Hour)
        return un12HourTime;
    else
        return (un12HourTime + MOON_TIME);        
}

BOOL IsTimePM(UINT8 un24HourTime)
{
    if(un24HourTime > MOON_TIME)
        return TRUE;
    else
        return FALSE;
}


#if 0 /* 000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 */
#endif /* 000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 */

