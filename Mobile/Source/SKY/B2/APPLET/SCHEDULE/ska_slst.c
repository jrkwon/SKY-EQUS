/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_slst.c
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
** 01-01-09   rosa       Created.
**
** ===========================================================================
*/
 
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
//#include <stdio.h>

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SCHL_MAX_VIEW_SIZE_RECORD   20 + 1

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
struct tagSCHL_LIST_CHECK_EVENT_T {
    BOOL    fPreEvent;
    BOOL    fPostEvent;
};
typedef struct tagSCHL_LIST_CHECK_EVENT_T SCHL_LIST_CHECK_EVENT_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

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
static  DWORD           l_dwSetTime;
static  clk_julian_type l_cjSetTime;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
static VOID sCheckEvent( DWORD   pdwOneMonthTime, SCHL_LIST_CHECK_EVENT_T* pCheckEventR );
static BOOL sHavePreMonthEvent( DWORD   pdwOneMonthTime );
static BOOL sHavePostMonthEvent( DWORD   pdwOneMonthTime );

UINT8  sGetScheduleListDirect( UINT8  hList , INT8 nDirect);
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
/*
VOID    SKY_API SKA_MainSchedulerList(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_START_CHILD( sSKA_ListSchedulerList );
        break;

    case SE_APP_END:
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
*/

VOID SKY_API SKA_MainSchedulerList(SKY_EVENT_T *pEvent)
{
    UINT8           i, unIdxOfSchlList, unIdx;
    INT8            nIdxOfBlockT;
    clk_julian_type cjSetTime;
	HCONTROL        hControl;
	BYTE            abHeader[SCHL_MAX_VIEW_SIZE_RECORD];
	UINT8           TitleDirect;
	SCHL_LIST_CHECK_EVENT_T CheckEventR;
	DWORD           dwCurCalinderTime;

    static  UINT8                   hList, unNumOfEvent;        
	static  BYTE                    aszSchlList[SCHL_MAX_DAYS_AMONTH][SCHL_MAX_VIEW_SIZE_RECORD];
	static  BOOL                    fReBuild = FALSE;

	BYTE*                            pabSchlList[SCHL_MAX_DAYS_AMONTH];
	BYTE                             szBuf[SCHL_MAX_VIEW_SIZE_RECORD];

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);

        // display the window
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX,
                        SKY_DEF_WIN_DY, 
                        CL_NONE_CONFIRM_NONE,
                        NULL, 
                        WA_SHOW_CML);// | WA_SHOW_HEADER);	

        l_dwSetTime = pEvent->dwParam;
        SetScheduleListTime( l_dwSetTime );

        unIdxOfSchlList=0;
        hList = CreateList(SCHL_MAX_DAYS_AMONTH);
        for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ ) // 31
        {
            if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
            {
                sprintf((char*)szBuf, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_DATA), i+1, GetEventNumOfBlockT(i) );
                pabSchlList[unIdxOfSchlList] = aszSchlList[unIdxOfSchlList];
                STRCPY( pabSchlList[unIdxOfSchlList], szBuf );
                AddList( hList, (INT16)i, FALSE );
                unIdxOfSchlList++;
            }
        }
        unNumOfEvent = unIdxOfSchlList;

        if( unNumOfEvent > 0 )
        {
            hControl = EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), 
                pabSchlList, unNumOfEvent, unNumOfEvent, 0 );
            EQC_SetScrollListHandle( EQS_GetNthIOC( 0 ), hList );
            //EQC_SET_STYLE(hControl, CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_FIXED | CS_SCROLL_LIST_MAXIMIZE );
			EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );
            EQS_SetIOCActive( hControl );

            // set the title
            sprintf((char*)abHeader, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_TITLE), l_cjSetTime.year, l_cjSetTime.month );
            abHeader[SCHL_MAX_VIEW_SIZE_RECORD-1] = '\0';
            //EQS_SetSWindowHeaderStr( abHeader );
			hControl = EQS_GetNthIOC(0);
			EQC_SetScrollTitle(hControl, abHeader);			
			EQC_ShowScrollTitle(hControl, TRUE);
			EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);

        }
        else // find evnet of other months
        {
            // for quick search 
            sCheckEvent( GetScheduleListTime(), &CheckEventR );

            if( CheckEventR.fPostEvent == TRUE )
            {
                sHavePostMonthEvent( GetScheduleListTime() );

                for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
                {
                    if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
                    {
                        sprintf((char*)szBuf, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_DATA), i+1, GetEventNumOfBlockT(i) );
                        pabSchlList[unIdxOfSchlList] = aszSchlList[unIdxOfSchlList];
                        STRCPY( pabSchlList[unIdxOfSchlList], szBuf );
                        AddList( hList, (INT16)i, FALSE );
                        unIdxOfSchlList++;
                    }
                }
                unNumOfEvent = unIdxOfSchlList;
                EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), 
                        pabSchlList, unNumOfEvent, unNumOfEvent, 0 );
                EQC_SetScrollListHandle( EQS_GetNthIOC( 0 ), hList );
                //EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_FIXED | CS_SCROLL_LIST_MAXIMIZE );				
				EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );
                EQS_SetIOCActive( EQS_GetNthIOC( 0 ) );

                sprintf((char*)abHeader, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_TITLE), l_cjSetTime.year, l_cjSetTime.month );
                abHeader[SCHL_MAX_VIEW_SIZE_RECORD-1] = '\0';
                //EQS_SetSWindowHeaderStr( abHeader );
				hControl = EQS_GetNthIOC(0);
				EQC_SetScrollTitle(hControl, abHeader);				
				EQC_ShowScrollTitle(hControl, TRUE);
				EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);

            }
            else if(( CheckEventR.fPostEvent == FALSE ) && (CheckEventR.fPreEvent == TRUE ))
            {
                sHavePreMonthEvent( GetScheduleListTime() );

                for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
                {
                    if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
                    {
                        sprintf((char*)szBuf, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_DATA), i+1, GetEventNumOfBlockT(i) );
                        pabSchlList[unIdxOfSchlList] = aszSchlList[unIdxOfSchlList];
                        STRCPY( pabSchlList[unIdxOfSchlList], szBuf );
                        AddList( hList, (INT16)i, FALSE );
                        unIdxOfSchlList++;
                    }
                }
                unNumOfEvent = unIdxOfSchlList;
                EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), 
                        pabSchlList, unNumOfEvent, unNumOfEvent, 0 ); // unIdxOfSchlList == num of the all data.
                EQC_SetScrollListHandle( EQS_GetNthIOC( 0 ), hList );
                //EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_FIXED | CS_SCROLL_LIST_MAXIMIZE );
				EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );
                EQS_SetIOCActive( EQS_GetNthIOC( 0 ) );

                sprintf((char*)abHeader, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_TITLE), l_cjSetTime.year, l_cjSetTime.month );
                abHeader[SCHL_MAX_VIEW_SIZE_RECORD-1] = '\0';
                //EQS_SetSWindowHeaderStr( abHeader );
				hControl = EQS_GetNthIOC(0);
				EQC_SetScrollTitle(hControl, abHeader);
				EQC_ShowScrollTitle(hControl, TRUE);						
				EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);
            }
            else
            {
				if( pEvent->wParam )
				{
					EQS_END();
				}
				else
				{
					EQS_END_TO_EVENT( EQS_POP_TO_PARENT, SE_APP_RESUME, EQS_NA, SCHL_HAS_NO_EVENT );
				}
            }
         }

		if( GetListSize(hList) == 0 )
		{
			if( fReBuild == TRUE )
			{
				EQS_END();
			}
			else
			{
				EQS_END_TO_EVENT( EQS_POP_TO_PARENT, SE_APP_RESUME, EQS_NA, SCHL_HAS_NO_EVENT );
			}
		}
		fReBuild = FALSE;


        break;

    case SE_APP_END:
        SetCalendarTime( GetScheduleListTime() );
        MakeSchedule( GetScheduleListTime() );    // it makes the event table for the schedule on a month.		
        EQS_ReleaseDC();
        break;

    case SE_APP_RESUME:
        //EQS_SetIOCActive( EQS_GetNthIOC( 0 ) );
		fReBuild = TRUE;
		EQS_START_SIBLING_EVENT( SKA_MainSchedulerList, SE_APP_START, TRUE, l_dwSetTime, FALSE);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        unIdx = (UINT8)EQC_GetScrollLogicalIndex( EQS_GetNthIOC( 0 ) );            
        nIdxOfBlockT = (UINT8)GetListValue( hList, unIdx ); //get the address of EventT

        l_cjSetTime.day = nIdxOfBlockT + 1;

        EQS_START_CHILD_EVENT( SKA_ViewADay, SE_APP_START, SMET_VIEW_LIST, clk_julian_to_secs( &l_cjSetTime ) );
        break;

    //case SE_IO_SCROLL_LIST_TOP:
	case SE_IO_SCROLL_LIST_LEFT:		
        hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SCHEDULE_IN_SEARCH_MSG), 0, MBT_NONE,MBT_HELP);
		EQC_SET_USERDATA(hControl,SE_IO_SCROLL_LIST_LEFT);
#if 0
		dwCurCalinderTime = GetScheduleListTime();			
		if( sHavePreMonthEvent(GetScheduleListTime()) )
		{   
			EQS_DeleteControl( EQS_GetNthIOC( 0 ) );

			unIdxOfSchlList = 0;
			hList = CreateList( SCHL_MAX_DAYS_AMONTH );
			for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
			{
				if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
				{
					sprintf((char*)szBuf, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_DATA), i+1, GetEventNumOfBlockT(i) );
					pabSchlList[unIdxOfSchlList] = aszSchlList[unIdxOfSchlList];
					STRCPY( pabSchlList[unIdxOfSchlList], szBuf );
					AddList( hList, (INT16)i, FALSE );
					unIdxOfSchlList++;
				}
			}
			unNumOfEvent = unIdxOfSchlList;
			EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), 
					pabSchlList, unNumOfEvent, unNumOfEvent, 0 ); // unIdxOfSchlList == num of the all data.
			EQC_SetScrollListHandle( EQS_GetNthIOC( 0 ), hList );
			//EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_FIXED | CS_SCROLL_LIST_MAXIMIZE );
			EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );
			EQC_SetScrollPhysicalIndex(EQS_GetNthIOC(0), unNumOfEvent-1);
			EQS_SetIOCActive(EQS_GetNthIOC(0));        
        
			sprintf((char*)abHeader, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_TITLE), l_cjSetTime.year, l_cjSetTime.month );
			abHeader[SCHL_MAX_VIEW_SIZE_RECORD-1] = '\0';
			hControl = EQS_GetNthIOC(0);
			EQC_SetScrollTitle(hControl, abHeader);			
			EQC_ShowScrollTitle(hControl, TRUE);
			EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);
		}
		else
		{
			SetScheduleListTime(dwCurCalinderTime);
			MakeSchedule(dwCurCalinderTime);    // it makes the event table for the schedule on a month.
			EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_NO_PRE_EVENT),
						   MESSAGE_DEFAULT_TIME, 
						   MBT_NONE, 
						   MBT_CONFIRM);
		}
#endif
		break;
    //case SE_IO_SCROLL_LIST_BOTTOM:
	case SE_IO_SCROLL_LIST_RIGHT:        
        hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SCHEDULE_IN_SEARCH_MSG), 0, MBT_NONE,MBT_HELP);
		EQC_SET_USERDATA(hControl,SE_IO_SCROLL_LIST_RIGHT);
#if 0
		dwCurCalinderTime = GetScheduleListTime();
      	
		if( sHavePostMonthEvent(GetScheduleListTime()) )
		{          
			EQS_DeleteControl( EQS_GetNthIOC( 0 ) );

			unIdxOfSchlList = 0;
			hList = CreateList( SCHL_MAX_DAYS_AMONTH );            
			for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
			{
				if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
				{
					sprintf((char*)szBuf, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_DATA), i+1, GetEventNumOfBlockT(i) );
					pabSchlList[unIdxOfSchlList] = aszSchlList[unIdxOfSchlList];
					STRCPY( pabSchlList[unIdxOfSchlList], szBuf );
					AddList( hList, (INT16)i, FALSE );
					unIdxOfSchlList++;
				}
			}
			unNumOfEvent = unIdxOfSchlList;
			EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), 
					pabSchlList, unNumOfEvent, unNumOfEvent, 0 ); // unIdxOfSchlList == num of the all data.
			EQC_SetScrollListHandle( EQS_GetNthIOC( 0 ), hList );
			//EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_FIXED | CS_SCROLL_LIST_MAXIMIZE );
			EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );

			EQS_SetIOCActive( EQS_GetNthIOC( 0 ) );

			sprintf((char*)abHeader, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_TITLE), l_cjSetTime.year, l_cjSetTime.month );
			abHeader[SCHL_MAX_VIEW_SIZE_RECORD-1] = '\0';
			//EQS_SetSWindowHeaderStr( abHeader );
			hControl = EQS_GetNthIOC(0);
			EQC_SetScrollTitle(hControl, abHeader);			
			EQC_ShowScrollTitle(hControl, TRUE);		
			EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);

		}
		else
		{
			SetScheduleListTime(dwCurCalinderTime);
			MakeSchedule(dwCurCalinderTime);    // it makes the event table for the schedule on a month.
			EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_NO_POST_EVENT),
					         MESSAGE_DEFAULT_TIME, 
							 MBT_NONE, MBT_CONFIRM);
		}
#endif
        break;

    //case SE_SOFTKEY_CANCEL:
    case SE_IO_SCROLL_LIST_CANCEL:
        if( pEvent->dwParam != EQC_DELETED )//if you just clear the scroll list
        {
            EQS_END();
        }
        break;
    
	case SE_IO_MESGBOX_END:
		if( pEvent->dwParam == SE_IO_SCROLL_LIST_LEFT)
		{
			dwCurCalinderTime = GetScheduleListTime();			
			if( sHavePreMonthEvent(GetScheduleListTime()) )
			{      
				EQS_DeleteControl( EQS_GetNthIOC( 0 ) );
				unIdxOfSchlList = 0;
				hList = CreateList(SCHL_MAX_DAYS_AMONTH);
				for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
				{
					if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
					{
						sprintf((char*)szBuf, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_DATA), i+1, GetEventNumOfBlockT(i) );
						pabSchlList[unIdxOfSchlList] = aszSchlList[unIdxOfSchlList];
						STRCPY( pabSchlList[unIdxOfSchlList], szBuf );
						AddList( hList, (INT16)i, FALSE );
						unIdxOfSchlList++;
					}
				}
				unNumOfEvent = unIdxOfSchlList;
				EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), 
						pabSchlList, unNumOfEvent, unNumOfEvent, 0 ); // unIdxOfSchlList == num of the all data.
				EQC_SetScrollListHandle( EQS_GetNthIOC( 0 ), hList );
				//EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_FIXED | CS_SCROLL_LIST_MAXIMIZE );
				EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );
				//EQC_SetScrollPhysicalIndex(EQS_GetNthIOC(0), unNumOfEvent-1);
				EQS_SetIOCActive(EQS_GetNthIOC(0));        
            
				sprintf((char*)abHeader, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_TITLE), l_cjSetTime.year, l_cjSetTime.month );
				abHeader[SCHL_MAX_VIEW_SIZE_RECORD-1] = '\0';
				hControl = EQS_GetNthIOC(0);
				EQC_SetScrollTitle(hControl, abHeader);			
				EQC_ShowScrollTitle(hControl, TRUE);
				EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);
			}
			else
			{
				SetScheduleListTime(dwCurCalinderTime);
				MakeSchedule(dwCurCalinderTime);    // it makes the event table for the schedule on a month.
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_NO_PRE_EVENT),
							   MESSAGE_DEFAULT_TIME, 
							   MBT_NONE, 
							   MBT_CONFIRM);
			}

		}
		else if( pEvent->dwParam == SE_IO_SCROLL_LIST_RIGHT )
		{
			dwCurCalinderTime = GetScheduleListTime();
      		
			if( sHavePostMonthEvent(GetScheduleListTime()) )
			{          
				EQS_DeleteControl( EQS_GetNthIOC( 0 ) );

				unIdxOfSchlList = 0;
				hList = CreateList(SCHL_MAX_DAYS_AMONTH);            
				for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
				{
					if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
					{
						sprintf((char*)szBuf, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_DATA), i+1, GetEventNumOfBlockT(i) );
						pabSchlList[unIdxOfSchlList] = aszSchlList[unIdxOfSchlList];
						STRCPY( pabSchlList[unIdxOfSchlList], szBuf );
						AddList( hList, (INT16)i, FALSE );
						unIdxOfSchlList++;
					}
				}
				unNumOfEvent = unIdxOfSchlList;
				EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), 
						pabSchlList, unNumOfEvent, unNumOfEvent, 0 ); // unIdxOfSchlList == num of the all data.
				EQC_SetScrollListHandle( EQS_GetNthIOC( 0 ), hList );
				//EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_FIXED | CS_SCROLL_LIST_MAXIMIZE );
				EQC_SET_STYLE( EQS_GetNthIOC( 0 ), CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );

				EQS_SetIOCActive( EQS_GetNthIOC( 0 ) );

				sprintf((char*)abHeader, SKY_GET_TOKEN(TKN_SCHEDULE_LIST_TITLE), l_cjSetTime.year, l_cjSetTime.month );
				abHeader[SCHL_MAX_VIEW_SIZE_RECORD-1] = '\0';
				//EQS_SetSWindowHeaderStr( abHeader );
				hControl = EQS_GetNthIOC(0);
				EQC_SetScrollTitle(hControl, abHeader);			
				EQC_ShowScrollTitle(hControl, TRUE);		
				EQC_SetScrollTitleStyle(hControl,CS_SCROLL_LIST_BOTH_ARROW_TITLE);

			}
			else
			{
				SetScheduleListTime(dwCurCalinderTime);
				MakeSchedule(dwCurCalinderTime);    // it makes the event table for the schedule on a month.
				EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_SCHEDULE_MESSG_NO_POST_EVENT),
					             MESSAGE_DEFAULT_TIME, 
								 MBT_NONE, MBT_CONFIRM);
			}
		}
		else 
			;

		break;
    default:
        EQS_DEF_EVENT_PROC( pEvent );
        break;
    }    
}



static  VOID    sCheckEvent( DWORD   pdwOneMonthTime, SCHL_LIST_CHECK_EVENT_T* pCheckEventR )
{
    UINT8   i;
	clk_julian_type     cjStartTime, cjEndTime,cjCurTime;

	pCheckEventR->fPostEvent = FALSE;
	pCheckEventR->fPreEvent  = FALSE;
	clk_secs_to_julian( pdwOneMonthTime, &cjCurTime );


    memset( pCheckEventR, NULL, (SIZE_T)sizeof(SCHL_LIST_CHECK_EVENT_T) ); 
    for( i=0; i<(SCHL_MAX_RECORD_COUNT-1); i++ )
    {
        if( SKY_GET_SCHL_PRE_CONFIG_TYPE( i ) )// have some event
        {
			clk_secs_to_julian( SKY_GET_SCHL_PRE_CONFIG_START_TIME(i), &cjStartTime );
			clk_secs_to_julian( SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i), &cjEndTime );
            if((pCheckEventR->fPreEvent == FALSE) && 
                ((pdwOneMonthTime > SKY_GET_SCHL_PRE_CONFIG_START_TIME(i)) || // pre event
                 (pdwOneMonthTime > SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i)))) // pre event

            {
                pCheckEventR->fPreEvent = TRUE;
            }
            if((pCheckEventR->fPostEvent == FALSE) && 
                ((pdwOneMonthTime < SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i)) ||
                 (pdwOneMonthTime < SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i)))) // post event
            {
                pCheckEventR->fPostEvent = TRUE;
            }
        }
    }
    return;
}

static  BOOL    sHavePreMonthEvent( DWORD   pdwOneMonthTime )
{
    UINT8   i,unDays;
    BOOL    fHavePreEvent;
    DWORD   dwViewTime;
    clk_julian_type cjViewTime;
    
    fHavePreEvent = FALSE;
    dwViewTime = GetScheduleListTime();
    do
    {
        clk_secs_to_julian( dwViewTime, &cjViewTime );
        if( cjViewTime.month > 1 )
        {
            cjViewTime.month--;
        }
        else
        {
			return fHavePreEvent;
#if 0
            if( cjViewTime.year > SCHL_START_YEAR ) //1995
            {
                cjViewTime.year--;
                cjViewTime.month = 12;
            }
            else
            {   
                break;
            }
#endif
        }
        unDays = GetDaysOfMonth( cjViewTime.year, (INT8)cjViewTime.month );
        dwViewTime = dwViewTime - ( unDays*TIME_SECS_1_DAY );

        MakeSchedule( dwViewTime );    // it makes the event table for the schedule on a month.

        for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
        {
            if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
            {
                fHavePreEvent = TRUE;
                SetScheduleListTime( dwViewTime );
                break;
            }
        }

    }while( fHavePreEvent == FALSE );

    return fHavePreEvent;
}

static  BOOL    sHavePostMonthEvent( DWORD   pdwOneMonthTime )
{
    UINT8   i,unDays;  
    BOOL    fHavePostEvent;
    DWORD   dwViewTime;
    clk_julian_type cjViewTime;

    fHavePostEvent = FALSE;
    dwViewTime = GetScheduleListTime();
    do
    {
        clk_secs_to_julian( dwViewTime, &cjViewTime );
        unDays = GetDaysOfMonth( cjViewTime.year, (INT8)cjViewTime.month );        

        if( cjViewTime.month < 12 )
        {
            cjViewTime.month++;
        }
        else // cjViewTime.month == 12
        {
			return fHavePostEvent;
#if 0
            if( cjViewTime.year < SCHL_END_YEAR ) //2015
            {
                cjViewTime.year++;
                cjViewTime.month = 1;
            }
            else
            {   
                break;
            }
#endif
        }
        dwViewTime = dwViewTime + ( unDays*TIME_SECS_1_DAY );

        MakeSchedule( dwViewTime );    // it makes the event table for the schedule on a month.

        for( i=0; i<SCHL_MAX_DAYS_AMONTH; i++ )
        {
            if( GetTypeOfBlockT( i ) ) //SBRT_EVENT
            {
                fHavePostEvent = TRUE;
                SetScheduleListTime( dwViewTime );
                break;
            }
        }

    }while( fHavePostEvent == FALSE );

    return fHavePostEvent;
}

VOID    SetScheduleListTime( DWORD  dwOneViewTime )
{
    l_dwSetTime = dwOneViewTime;
    clk_secs_to_julian( l_dwSetTime, &l_cjSetTime );
    return;

}

DWORD    GetScheduleListTime( VOID )		
{
    l_dwSetTime = clk_julian_to_secs( &l_cjSetTime );
    return  l_dwSetTime;
}


UINT8  sGetScheduleListDirect( UINT8  hList, INT8 nDirect)
{

    INT8            nIdxOfBlockT;
    clk_julian_type cjSetTime;
	UINT8  TitleDirect;
	SCHL_LIST_CHECK_EVENT_T CheckEventR;



	if( nDirect > 0 )
	{
		memset(&cjSetTime, NULL, (SIZE_T)(sizeof(clk_julian_type)));
		cjSetTime.year = l_cjSetTime.year;
		cjSetTime.month = (l_cjSetTime.month + 1);
		cjSetTime.day = 1; 
		if(cjSetTime.month > 12)
		{
			cjSetTime.year++;
			cjSetTime.month = 1;            
			cjSetTime.day = 1;
		}      
		// for quick search 
		sCheckEvent(clk_julian_to_secs(&cjSetTime), &CheckEventR);

		if(CheckEventR.fPostEvent == TRUE)
		{
			TitleDirect = CS_SCROLL_LIST_RIGHT_ARROW_TITLE;
		}
		else
		{
			TitleDirect = CS_SCROLL_LIST_NO_ARROW_TITLE;
		}
	}
	else
	{
		nIdxOfBlockT = (UINT8)GetListValue( hList, 0 );
		memset( &cjSetTime, NULL, (SIZE_T)(sizeof(clk_julian_type)) );
		cjSetTime.year = l_cjSetTime.year;
		cjSetTime.month = l_cjSetTime.month;
		cjSetTime.day = (nIdxOfBlockT + 1);

		// for quick search 
		sCheckEvent( clk_julian_to_secs( &cjSetTime ), &CheckEventR );

		if(CheckEventR.fPostEvent == TRUE)
		{
			TitleDirect = CS_SCROLL_LIST_LEFT_ARROW_TITLE;
		}
		else
		{
			TitleDirect = CS_SCROLL_LIST_NO_ARROW_TITLE;
		}
	}

	return TitleDirect;

}
