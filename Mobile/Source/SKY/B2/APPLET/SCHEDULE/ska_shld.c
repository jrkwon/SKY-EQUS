/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_shld.c
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
** 01-01-22   rosa       Created.
**
** ===========================================================================
*/

/*   
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
#include <stdio.h>

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define MYHOLIDAY_VIEW_LINE  20 //10 + 9 + 1

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum    tagSCHL_MAKE_HOLIDAY_E {
    SMH_TIME_TEXT,
    SMH_TIME,
    SMH_TITLE_TEXT,
    SMH_TITLE,
    SMH_MAX
};
typedef enum tagSCHL_MAKE_HOLIDAY_E    SCHL_MAKE_HOLIDAY_E;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
SKY_TOKEN paHolidayMenu[1] =
{
    TKN_SCHEDULE_MY_HOLIDAY_MENU1 // Delete
};

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
SCHL_HOLIDAY_CAL_T  g_MyHolidayT[SCHL_MAX_COUNT_MY_HOLIDAY];

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
static VOID SKY_API sSKA_ListMyHoliday(SKY_EVENT_T *pEvent);
static VOID SKY_API sSKA_MakeMyHoliday(SKY_EVENT_T *pEvent);

static BOOL sSortMyHoliday( INT8    anSortedIdxT[] );
static DWORD sGetSecsTimeOfMyHoliday( UINT8  unIdxOfMyHolidayT );
static BOOL sSameToMyHoliday(UINT8 unIdx, WORD wYear, UINT8 unMonth, UINT8 unDay);

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_MainMyHoliday(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);

        EQS_START_CHILD(sSKA_ListMyHoliday);
        break;

    case SE_APP_END:
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

static VOID SKY_API sSKA_ListMyHoliday(SKY_EVENT_T *pEvent)
{
    UINT8   i, unIdx, unIdxOfMyHolidayT;     
    BYTE    abDateTime[11];
    static  UINT8       hList, unTotalMyHoliday;
    static  BYTE*       pabHolidayList[SCHL_MAX_COUNT_MY_HOLIDAY];
    static  INT8        anSortedIdxT[SCHL_MAX_COUNT_MY_HOLIDAY];
    static  HCONTROL    hNoMyHoliday;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);

        /* display the window */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX,
                      SKY_DEF_WIN_DY, 
                      CL_DELETE_CONFIRM_NEWHOLIDAY, 
                      SKY_GET_TOKEN(TKN_SCHEDULE_MY_HOLIDAY_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        /* sort */
        if(sSortMyHoliday(anSortedIdxT))
        {
            unIdx = 0;
            hList = CreateList( SCHL_MAX_COUNT_MY_HOLIDAY );
            for( i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++ ) // 50
            {
                if(anSortedIdxT[i] > SCHL_ERROR) // more than idx 0
                {
                    pabHolidayList[unIdx] = (BYTE *)SKY_Malloc( (SIZE_T)(MYHOLIDAY_VIEW_LINE) );
                    sprintf((char*)abDateTime, "%4d.%02d.%02d ",\
						    g_MyHolidayT[anSortedIdxT[i]].wYear,\
							g_MyHolidayT[anSortedIdxT[i]].unMonth,\
							g_MyHolidayT[anSortedIdxT[i]].unDay );
                    STRCPY(pabHolidayList[unIdx], abDateTime);
                    if(IsMiddleOfMultiByte(g_MyHolidayT[anSortedIdxT[i]].pszName, 9))
                        STRNCAT(pabHolidayList[unIdx], g_MyHolidayT[anSortedIdxT[i]].pszName, 8);
                    else
                        STRNCAT(pabHolidayList[unIdx], g_MyHolidayT[anSortedIdxT[i]].pszName, 9);
                    
                    AddList(hList, (INT16)anSortedIdxT[i], FALSE);
                    unIdx++;
                }
            }
            unTotalMyHoliday = unIdx;

            /*
            ** ***************************************************************************
            ** CREATE
            ** ***************************************************************************
            */
            EQC_ScrollList(EQS_NA,
                           EQS_NA, 
                           EQS_NA, 
                           EQC_GetScrollDisplayLine(), 
                           pabHolidayList, 
                           unTotalMyHoliday, 
                           unTotalMyHoliday, 
                           0);
            EQC_SetScrollListHandle(EQS_GetNthIOC(0), hList);
            EQC_SET_STYLE(EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) |
                                            CS_SCROLL_LIST_MAXIMIZE | 
                                            CS_SCROLL_LIST_ENUMBERATED |
                                            CS_SCROLL_LIST_NO_CML));
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        }
        else
        {
            EQS_SetWindowCML(CL_NONE_NONE_NEWHOLIDAY);
        }
        break;

    case SE_APP_END:
        /* dealloc. */
        for(i=0; i<unTotalMyHoliday; i++)
        {
            SKY_Free(pabHolidayList[i]);
        }
		if( GetListSize(hList) > 0 )
		{
			DeleteList(hList);
		}
        EQS_ReleaseDC();
        break;

    case SE_SOFTKEY_ADD:
        unIdxOfMyHolidayT = GetNewIdxOfMyHoliday();
        if(unIdxOfMyHolidayT < SCHL_MAX_COUNT_MY_HOLIDAY)
		{
            EQS_START_SIBLING_EVENT(sSKA_MakeMyHoliday, SE_APP_START, (WORD)SMT_NEW, (DWORD)unIdxOfMyHolidayT, FALSE);
		}
        else
		{
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_FULL_MY_HOLIDAY), 
                           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
		}
        break;

    case SE_IO_SCROLL_LIST_SELECT: // view
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        unIdxOfMyHolidayT = (UINT8)GetListValue(hList, unIdx);
        EQS_START_SIBLING_EVENT(sSKA_MakeMyHoliday, SE_APP_START, (WORD)SMT_OLD, (DWORD)unIdxOfMyHolidayT, FALSE);
        break;

	case SE_SOFTKEY_DELETE:
		EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_Q_DELETE),
                       MESSAGE_DEFAULT_TIME, 
                       MBT_YESNO, 
                       MBT_CONFIRM);
		break;

    case SE_IO_MESGBOX_YES:
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        unIdxOfMyHolidayT = (UINT8)GetListValue(hList, unIdx); //get the address of EventT

        g_MyHolidayT[unIdxOfMyHolidayT].Type = g_InitHoliday.Type;
        g_MyHolidayT[unIdxOfMyHolidayT].fLunar = g_InitHoliday.fLunar;
        g_MyHolidayT[unIdxOfMyHolidayT].wYear = g_InitHoliday.wYear;
        g_MyHolidayT[unIdxOfMyHolidayT].unMonth = g_InitHoliday.unMonth;
        g_MyHolidayT[unIdxOfMyHolidayT].unDay = g_InitHoliday.unDay;
        g_MyHolidayT[unIdxOfMyHolidayT].fConsecutive = g_InitHoliday.fConsecutive;
        STRCPY(g_MyHolidayT[unIdxOfMyHolidayT].pszName, g_InitHoliday.pszName);

        SaveMyHoliday(unIdxOfMyHolidayT);

        EQC_DeleteScrollData(EQS_GetNthIOC(0), unIdx);
        RemoveListItem(hList, unIdx);
        unTotalMyHoliday = unTotalMyHoliday - 1;
        if(unTotalMyHoliday == 0)
        {
            EQS_SetWindowCML(CL_NONE_NONE_NEWHOLIDAY);
            EQS_DeleteControl(EQS_GetNthIOC(0));
        }

		EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_DELETE),
                       MESSAGE_DEFAULT_TIME, 
                       MBT_NONE, 
                       MBT_SUCCESS);  
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

    case SE_IO_MESGBOX_END:
		if(EQS_GetIOCNum()) /* Control exist? */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
        else
            EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }    
}

static  VOID    SKY_API sSKA_MakeMyHoliday(SKY_EVENT_T *pEvent)
{
    UINT8               i, unCtrl;
    HCONTROL            hMessageBox;
    SCHL_MAKE_TYPE_E    MakeType;
    clk_julian_type     cjSetTime;
	HCONTROL            hControl;
    static HCONTROL     hError;
    static UINT8        unIdxOfMyHolidayT;


    switch( pEvent->EventID )
    {
    case SE_APP_START:
		{

			COORD    StartX, StartY;

            MARGIN_T Margin;
            COORD    xSpace, ySpace;

        
			/* display the window */
			EQS_SetWindow(  SKY_DEF_WIN_X,
							SKY_DEF_WIN_Y,
							SKY_DEF_WIN_DX, 
							SKY_DEF_WIN_DY, 
							CL_NONE_NONE_SAVE, 
							SKY_GET_TOKEN(TKN_SCHEDULE_MY_HOLIDAY_TITLE), 
							WA_SHOW_CML | WA_SHOW_HEADER);

			MakeType = pEvent->wParam;
			unIdxOfMyHolidayT = (UINT8)pEvent->dwParam;

            
            EQS_StartFormatter();   
            EQS_GetControlMargin(&Margin);
            EQS_GetControlSpace(&xSpace, &ySpace);
            EQS_EndFormatter();    
			/*
			** ***************************************************************************
			** CREATE
			** ***************************************************************************
			*/
			StartX = Margin.xLeft + xSpace;
            StartY = Margin.yTop  + ySpace;
			hControl = EQC_StaticText(StartX, StartY, TKN_SCHEDULE_MY_HOLIDAY_TIME_TEXT ); //SMH_TIME_TEXT

			StartX += EQC_GET_DX(hControl) + xSpace;
			hControl = EQC_Time(StartX, StartY, 0, CAL_TIME_CONTROL ); //SMH_TIME
			EQC_SetTimeYearLimit(hControl, SCHL_START_YEAR, SCHL_END_YEAR);
			
			StartX = Margin.xLeft + xSpace;
			StartY += EQC_GET_DY(hControl) + ySpace;
			hControl = EQC_StaticText(StartX, StartY, TKN_SCHEDULE_MY_HOLIDAY_TITLE_TEXT ); //SMH_TITLE_TEXT
			StartX += EQC_GET_DX(hControl) + xSpace;
			EQC_TextEdit(StartX, StartY, 14, 2, TEM_KOREAN, MAX_SIZE_TITLE_LINE, "", 0 ); //SMH_TITLE
			//EQC_SET_STYLE(EQS_GetNthIOC(SMH_TITLE), EQC_GET_STYLE(EQS_GetNthIOC(SMH_TITLE)) | CS_TEXT_EDIT_VSCROLL); 

			/*
			** ***************************************************************************
			** SET THE VALUE
			** ***************************************************************************
			*/
			if(MakeType) //SMT_NEW
			{
				clk_secs_to_julian(SKY_GetLocalTime(), &cjSetTime);
				EQC_SetTimeYear(EQS_GetNthIOC(SMH_TIME), (WORD)cjSetTime.year);
				EQC_SetTimeMonth(EQS_GetNthIOC(SMH_TIME), (UINT8)cjSetTime.month);
				EQC_SetTimeDay(EQS_GetNthIOC(SMH_TIME), (UINT8)cjSetTime.day);
			}
			else //SMT_OLD
			{            
				EQC_SetTimeYear(EQS_GetNthIOC(SMH_TIME), g_MyHolidayT[unIdxOfMyHolidayT].wYear);
				EQC_SetTimeMonth(EQS_GetNthIOC(SMH_TIME), g_MyHolidayT[unIdxOfMyHolidayT].unMonth);
				EQC_SetTimeDay(EQS_GetNthIOC(SMH_TIME), g_MyHolidayT[unIdxOfMyHolidayT].unDay);            
				EQC_SetTextEditData(EQS_GetNthIOC(SMH_TITLE), g_MyHolidayT[unIdxOfMyHolidayT].pszName);
			}
			EQS_FitCanvasSize();
		}
        break;

    case SE_APP_END:
        break;

    case SE_SOFTKEY_SAVE:
        EQC_GetTextEditData(EQS_GetNthIOC(SMH_TITLE), g_MyHolidayT[unIdxOfMyHolidayT].pszName);
        if(STRLEN(g_MyHolidayT[unIdxOfMyHolidayT].pszName))
        {
            if(sSameToMyHoliday(unIdxOfMyHolidayT,
                                EQC_GetTimeYear(EQS_GetNthIOC(SMH_TIME)), 
                                EQC_GetTimeMonth(EQS_GetNthIOC(SMH_TIME)), 
                                EQC_GetTimeDay(EQS_GetNthIOC(SMH_TIME))))
            {
                hError = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_OVERLAP_MY_HOLIDAY),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE, 
                                        MBT_FAIL );
            }
            else
            {
                g_MyHolidayT[unIdxOfMyHolidayT].Type = SHT_MY;
                g_MyHolidayT[unIdxOfMyHolidayT].fConsecutive = FALSE;
                g_MyHolidayT[unIdxOfMyHolidayT].fLunar = FALSE; //solar
                g_MyHolidayT[unIdxOfMyHolidayT].wYear = EQC_GetTimeYear( EQS_GetNthIOC(SMH_TIME) );
                g_MyHolidayT[unIdxOfMyHolidayT].unMonth = EQC_GetTimeMonth( EQS_GetNthIOC(SMH_TIME) );
                g_MyHolidayT[unIdxOfMyHolidayT].unDay = EQC_GetTimeDay( EQS_GetNthIOC(SMH_TIME) );

                SaveMyHoliday(unIdxOfMyHolidayT);
                hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_CONFIRM_SAVE), 
                                             MESSAGE_DEFAULT_TIME, 
                                             MBT_NONE, 
                                             MBT_SUCCESS);
                EQC_SET_USERDATA(hMessageBox, pEvent->dwParam);
            }                
        }
        else
        {
            hError = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_INPUT_TITLE_MY_HOLIDAY),
                                    MESSAGE_DEFAULT_TIME, 
                                    MBT_NONE, 
                                    MBT_HELP);
        }        
        break;

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
			EQS_START_SIBLING(sSKA_ListMyHoliday, FALSE);
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
        if(pEvent->dwParam == SE_KEY_END)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else
            EQS_START_SIBLING(sSKA_ListMyHoliday, FALSE);
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam != hError)
            if(pEvent->dwParam == SE_KEY_END)
                EQS_END_TO(EQS_POP_TO_ROOT);
            else
                EQS_START_SIBLING(sSKA_ListMyHoliday, FALSE);
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }    
}

VOID BuildMyHoliday(VOID)
{
    UINT8   i;
    HFILE   hFile;
    DWORD   dwCount;

    hFile = SKY_OpenFile(FILE_NAME_HLDY, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    for( i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++ )
    {
        dwCount = SKY_ReadFile(hFile, &(g_MyHolidayT[i]), sizeof(SCHL_HOLIDAY_CAL_T));
    }
    SKY_CloseFile( hFile );   
}

UINT8 GetNewIdxOfMyHoliday(VOID)
{
    UINT8   i;

    for(i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++)
    {
        if(g_MyHolidayT[i].Type == SHT_NONE)
            break;
    }
    return i;
}

VOID    SaveMyHoliday( UINT8  unIdxOfMyHolidayT )
{
    HFILE hFile;
    DWORD dwCount;

    hFile = SKY_OpenFile(FILE_NAME_HLDY, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        hFile = SKY_OpenFile(FILE_NAME_HLDY, FS_AM_CREATE);
    }

    dwCount = SKY_SeekWriteFile( 
                hFile, FS_SO_SET, unIdxOfMyHolidayT*sizeof(SCHL_HOLIDAY_CAL_T), 
                (SCHL_HOLIDAY_CAL_T *)&g_MyHolidayT[unIdxOfMyHolidayT], sizeof(SCHL_HOLIDAY_CAL_T) );
    SKY_CloseFile( hFile );

}

static  BOOL    sSortMyHoliday( INT8  anSortedIdxT[] )
{

    BOOL    fChecked[SCHL_MAX_COUNT_MY_HOLIDAY], fFind, fSorted;
    UINT8   i,j,k,l;
    static  UINT8   unIdxOfMinValue;
    static  DWORD   dwMinValue;

    fSorted = FALSE;
    for( i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++ )
    {
        if( g_MyHolidayT[i].Type ) //SHT_MY
        {
            fChecked[i] = FALSE;
        }
        else
        {
            fChecked[i] = TRUE;
        }
    }

    k=0;
    for( i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++ )
    {
        //find the initial small value.
        for( l=0; l<SCHL_MAX_COUNT_MY_HOLIDAY; l++ )
        {
            if( fChecked[l] == FALSE )
            {
                dwMinValue = sGetSecsTimeOfMyHoliday(l);
                unIdxOfMinValue = l;
                fSorted = TRUE;
                break;
            }
        }
        if( l == SCHL_MAX_COUNT_MY_HOLIDAY )
        {
            break;
        }

        //find the smallest value.
        fFind = FALSE;
        for( j=0; j<SCHL_MAX_COUNT_MY_HOLIDAY; j++ )
        {
            if( fChecked[j] == FALSE )
            {
                if( sGetSecsTimeOfMyHoliday(j) < dwMinValue )
                {
                    dwMinValue = sGetSecsTimeOfMyHoliday(j);
                    unIdxOfMinValue = j;
                    fFind = TRUE;
                    //fChecked[j] = TRUE;
                }
            }
        }
        /*
        if( fFind )
        {
            anSortedIdxT[k] = unIdxOfMinValue;
            fChecked[unIdxOfMinValue] = TRUE;
        }
        else
        {
            anSortedIdxT[k] = unIdxOfMinValue;
            fChecked[unIdxOfMinValue] = TRUE;
        }
        */
        anSortedIdxT[k] = unIdxOfMinValue;
        fChecked[unIdxOfMinValue] = TRUE;
        k++;
    }

    if( fSorted )
    {
        for( i=k; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++ )
        {
            anSortedIdxT[i] = SCHL_ERROR;
        }
    }

    return  fSorted;
}

static  DWORD   sGetSecsTimeOfMyHoliday( UINT8  unIdxOfMyHolidayT )
{
    static  clk_julian_type cjSetTime;

    cjSetTime.year = g_MyHolidayT[unIdxOfMyHolidayT].wYear;
    cjSetTime.month = g_MyHolidayT[unIdxOfMyHolidayT].unMonth;
    cjSetTime.day = g_MyHolidayT[unIdxOfMyHolidayT].unDay;
    cjSetTime.day_of_week = 0;
    cjSetTime.hour = 0;
    cjSetTime.minute = 0;
    cjSetTime.second = 0;

    return  clk_julian_to_secs( &cjSetTime );
}

static BOOL sSameToMyHoliday(UINT8 unIdx, WORD wYear, UINT8 unMonth, UINT8 unDay)
{
    UINT8   i;

    for(i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++)
    {
        if(i!=unIdx)
            if( g_MyHolidayT[i].Type == SHT_MY )
                if( ( g_MyHolidayT[i].wYear == wYear ) &&
                    ( g_MyHolidayT[i].unMonth == unMonth ) &&
                    ( g_MyHolidayT[i].unDay == unDay ) )
                    return  TRUE;
    }
    return  FALSE;
}