/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_alrm.c
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
** 00-12-29   rosa       Created.
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
#define ALARM_ERROR             -1
//#define ALARM_SIZE_VIEW_LINE    20+1 
#define ALARM_SIZE_VIEW_LINE    (EQS_GetMaxX()+1)/EQS_CharWidth((BYTE)' ') + 1//18
#define ALARM_SIZE_10M	        10*60
#define ALARM_SIZE_20M	        20*60
#define ALARM_SIZE_30M	        30*60
#define ALARM_SIZE_1H	        1*60*60
#define ONE_DAY_TIME            24*60*60

/* rosa 01-08-21 --> @why, what */
#define ALARM_ANNIV_ANI_X       25
#define ALARM_ANNIV_ANI_Y       35

#define ALARM_GENERAL_ANI_X     27
#define ALARM_GENERAL_ANI_Y     27

#define ALARM_TITLE1_Y          103
#define ALARM_TITLE2_Y          123
#define ALARM_TITLE3_Y          143
/* <-- */


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagALARM_MAKE_TYPE_E
{
    AMT_NEW,
    AMT_OLD,
    AMT_MAX
};
typedef enum tagALARM_MAKE_TYPE_E ALARM_MAKE_TYPE_E;

enum tagALARM_SIMPLE_EDIT_MODE_E {
	ASEM_TIME_DATE,
	ASEM_TIME,
	ASEM_MAX
};



enum tagALARM_MENU_ITEM_E {
	AMI_AFTER_10M,
	AMI_AFTER_20M,
	AMI_AFTER_30M,
	AMI_AFTER_1H,
	AMI_SET_ANYTIME,
    AMI_SET_EVERYDAY,
	AMI_SET_EVERYWEEK,
	AMI_SET_EVERYMONTH,
	AMI_MAX
};

struct tagALARM_RECORD_T{
    BYTE*   pbTitle;
    UINT8   unAddr;
};
typedef struct tagALARM_RECORD_T ALARM_RECORD_T;

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
LOCAL BOOL  l_fFirstAlarmCheck = TRUE;
LOCAL BOOL  l_fResetRecursiveAlarm = TRUE;
LOCAL UINT8 l_SaveAlarmAddress ;

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
static VOID SKY_API sSKA_ListAlarm(SKY_EVENT_T *pEvent);
static VOID SKY_API sSKA_ChooseAlarm(SKY_EVENT_T *pEvent);
static VOID SKY_API sSKA_EditAdvancedAlarm(SKY_EVENT_T *pEvent);


static BOOL sSortAlarm(INT8 anSortedIdxT[]);
static BOOL sSetAlarmTime(UINT8 unAlarmMenuItem);
static BOOL sDeleteAlarmTime(UINT8 unIdxOfAlarm);
static UINT8 sGetNewIdx(VOID);

static void sMakeUsedEvent(INT16 nAddress, SCHL_RECORD_TYPE_E RecordType);

/*
** ***************************************************************************

                                    SKA_MainAlarm()
                                        |
                                        {child}
                                        |
                                    sSKA_ListAlarm()
                                        |
          -----------------------------------------------------------------
          |                     |                                         |
                             {child}                                   {child}
                                |                                         |
                         sSKA_EditAdvancedAlarm()  - {sibling} -  sSKA_ChooseAlarm()

** ***************************************************************************
*/

VOID SKY_API SKA_MainAlarm(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_START_CHILD( sSKA_ListAlarm );
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

static VOID SKY_API sSKA_ListAlarm(SKY_EVENT_T *pEvent)
{   
    static  HCONTROL    hNoAlarm; 
    static  BYTE*       pabAlarmList[MAX_ALARM_COUNT];
    static  UINT8       hList, unTotalAlarm;
    static  INT8        anSortedIdxT[MAX_ALARM_COUNT];

	
	HCONTROL  hControl, hScrollList;
	UINT8     i, unIdx, unAddrOfAlarm;

    switch( pEvent->EventID )
    {
    case SE_APP_START:

		/* set window */
        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_MENU_CONFIRM_NEWALARM, 
                        SKY_GET_TOKEN(TKN_ALARM_APPLET_TITLE), 
                        WA_SHOW_CML | WA_SHOW_HEADER);


		l_SaveAlarmAddress = MAX_ALARM_COUNT;

		for( i = 0; i < MAX_ALARM_COUNT;  i++ )
		{
            /* alloc. */
            pabAlarmList[i] = (BYTE *)SKY_Malloc((SIZE_T)(ALARM_SIZE_VIEW_LINE));
		}

        unIdx = 0;
            
		hList = CreateList(MAX_ALARM_COUNT);
        /* sort */
        if(sSortAlarm(anSortedIdxT))
        {   
            for(i=0; i<MAX_ALARM_COUNT; i++)
            {
                if( anSortedIdxT[i] >= 0 )
                {
                    if( STRLEN(SKY_GET_ALARM_TITLE(anSortedIdxT[i])) == 0 )
					{
						STRCPY( pabAlarmList[unIdx], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
					}
					else
					{				
						STRCPY( pabAlarmList[unIdx], SKY_GET_ALARM_TITLE(anSortedIdxT[i]) );
					}					
					/* link the idx */
                    AddList(hList, (INT16)anSortedIdxT[i], FALSE);
                    unIdx++;
                }
            }            
            unTotalAlarm = unIdx;
        }
        else
        {  
			unTotalAlarm = unIdx;		
			EQS_SetWindowCML(CL_NONE_NONE_NEWALARM);

        }
		/* create */
        hScrollList = EQC_ScrollList( EQS_NA, 
									  EQS_NA,
									  EQS_NA,
									  EQC_GetScrollDisplayLine(), 
									  pabAlarmList, 
									  unTotalAlarm, 
									  unTotalAlarm, 
									  0 );
        EQC_SetScrollListHandle( EQS_GetNthIOC(0), hList );
        EQC_SET_STYLE( EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) |\
				                          CS_SCROLL_LIST_MAXIMIZE         |\
										  CS_SCROLL_LIST_NO_CML           |\
										  CS_SCROLL_LIST_ENUMBERATED) );
        EQS_SetIOCActive(EQS_GetNthIOC(0));

		EQS_PinUp();
        break;

    case SE_APP_END:
		/* dealloc. */
        for( i=0; i<MAX_ALARM_COUNT; i++ )
        {
            SKY_Free( pabAlarmList[i] );
        }        
        DeleteList(hList);
        break;
    case SE_APP_RESUME:
		hControl = EQS_GetNthIOC(0);
		if( l_SaveAlarmAddress  != MAX_ALARM_COUNT )
		{			
			unIdx = EQC_GetScrollLogicalIndex(hControl);
			if( GetListSize(hList) > 0 && GetListValue(hList, unIdx) == l_SaveAlarmAddress ) //  기존 데이터 수정 
			{
				if(SKY_GET_ALARM_TIME_TYPE(l_SaveAlarmAddress) != ATT_NONE)
				{
					if( STRLEN(SKY_GET_ALARM_TITLE(l_SaveAlarmAddress)) == 0 )
					{
						STRCPY( pabAlarmList[unIdx], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
					}
					else
					{				
						STRCPY( pabAlarmList[unIdx], SKY_GET_ALARM_TITLE(l_SaveAlarmAddress) );
					}
				}
			}
			else  // 새 데이터 추가 
			{
				if(SKY_GET_ALARM_TIME_TYPE(l_SaveAlarmAddress) != ATT_NONE)
				{
					if( STRLEN(SKY_GET_ALARM_TITLE(l_SaveAlarmAddress)) == 0 )
					{
						STRCPY( pabAlarmList[unTotalAlarm], SKY_GET_TOKEN(TKN_ALARM_NO_TITLE) );
					}
					else
					{				
						STRCPY( pabAlarmList[unTotalAlarm], SKY_GET_ALARM_TITLE(l_SaveAlarmAddress) );
					}
					EQC_AddScrollData(hControl, unTotalAlarm,pabAlarmList[unTotalAlarm]  ,FALSE);
					AddList(hList, l_SaveAlarmAddress, EQS_NA);
					unTotalAlarm++;
				}
			}
			EQS_SetWindowCML(CL_MENU_CONFIRM_NEWALARM);
		}
		l_SaveAlarmAddress = MAX_ALARM_COUNT;
		EQS_SetIOCActive(hControl);

        break;

    case SE_SOFTKEY_MENU:
		{
			SKY_TOKEN paAlarmListMenu[2] =
			{
				TKN_ALARM_LIST_MENU1,   // delete
				TKN_ALARM_LIST_MENU2,   // alarm bell
				//TKN_ALARM_LIST_MENU3    // ignore MyHoliday
			};
			BOOL faActive[2] = { TRUE, TRUE};

			if( unTotalAlarm  == 0 )
			{
				faActive[0] = FALSE;
			}

			hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paAlarmListMenu, 2, 0, faActive);
			EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
		}
        break;

    case SE_SOFTKEY_NEWALARM: /* for NEW */
		if(unTotalAlarm < MAX_ALARM_COUNT)
		{
			EQS_START_CHILD(sSKA_ChooseAlarm);
		}
		else/* FULL */
		{			
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_ALARM_MESSG_FULL), 
							MESSAGE_DEFAULT_TIME, 
							MBT_NONE, 
							MBT_FAIL);
		}
        break;

    case SE_IO_SCROLL_LIST_SELECT: /* OLD */
		if( GetListSize(hList) > 0 )
		{
			unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
			unAddrOfAlarm = (UINT8)GetListValue(hList, unIdx); //get the idx

			EQS_START_CHILD_EVENT(sSKA_EditAdvancedAlarm, SE_APP_START,\
								  SKY_GET_ALARM_TIME_TYPE(unAddrOfAlarm), unAddrOfAlarm+1);
		}
		else
		{
			EQS_SetIOCActive(EQS_GetNthIOC(0));
		}

        break;

    case SE_IO_POPUP_SELECT:

        switch( pEvent->dwParam )
        {
        case TKN_ALARM_LIST_MENU1: /* delete */
            unIdx = EQC_GetScrollDataLine( EQS_GetNthIOC(0) );
            if( unIdx > 0 )
            {
				BYTE* pszStr;

                unIdx = (UINT8)EQC_GetScrollLogicalIndex( EQS_GetNthIOC(0) );
                unAddrOfAlarm = (UINT8)GetListValue( hList, unIdx ); //get the address of EventT

                sDeleteAlarmTime(unAddrOfAlarm);

                EQC_DeleteScrollData( EQS_GetNthIOC(0), unIdx );
                RemoveListItem( hList, unIdx );

				pszStr = pabAlarmList[unIdx]; // 인덱스 제조정 
				for( i=unIdx; i< MAX_ALARM_COUNT-1; i++)
				{
					pabAlarmList[i] = pabAlarmList[i+1];
				}
				pabAlarmList[i] = pszStr;

                unTotalAlarm = unTotalAlarm - 1;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_ALRRM_MESSG_CONFIRM_DELETE),
                               MESSAGE_DEFAULT_TIME,
                               MBT_NONE, 
                               MBT_SUCCESS);
            }
            else
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_ALARM_MESSG_NO_DELETE), 
                               MESSAGE_DEFAULT_TIME, 
                               MBT_NONE, 
                               MBT_SUCCESS);
            }
            break;

        case TKN_ALARM_LIST_MENU2: // alarm bell
            EQS_START_CHILD( SKA_MainAlarmBellSetting );
            break;

        default:
            break;
        }

        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if( pEvent->dwParam != EQC_DELETED)
        {
            EQS_END();
        }        
        break;

    case EQE_IO_POPUP_CANCEL:
        EQS_END();
        break;

    case SE_IO_MESGBOX_END:
        if( pEvent->wParam == hNoAlarm )
		{
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_ADD, EQS_DRAW_LEVEL_ALL, EQS_NA);
		}
        else
        {
			EQS_SetIOCActive(EQS_GetNthIOC(0));
        }

	    if( unTotalAlarm == 0 )
		{
			EQS_SetWindowCML(CL_NONE_NONE_NEWALARM);
		}
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }     
}

static VOID SKY_API sSKA_ChooseAlarm(SKY_EVENT_T *pEvent)
{
	UINT8 unIdx;
	BYTE* pabAlarmMenu[AMI_MAX];

    switch( pEvent->EventID )
    {
    case SE_APP_START:        
		/* set window */
        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_SELECT_NONE, 
                        SKY_GET_TOKEN(TKN_ALARM_NEW_TITLE), 
                        WA_SHOW_CML | WA_SHOW_HEADER);

		/* set scroll list item */
		pabAlarmMenu[0] = SKY_GET_TOKEN(TKN_ALARM_AFTER_10M);
		pabAlarmMenu[1] = SKY_GET_TOKEN(TKN_ALARM_AFTER_20M);
		pabAlarmMenu[2] = SKY_GET_TOKEN(TKN_ALARM_AFTER_30M);
		pabAlarmMenu[3] = SKY_GET_TOKEN(TKN_ALARM_AFTER_1H);
		pabAlarmMenu[4] = SKY_GET_TOKEN(TKN_ALARM_SET_ANYTIME);
		pabAlarmMenu[5] = SKY_GET_TOKEN(TKN_ALARM_EVERYDAY_TITLE);
		pabAlarmMenu[6] = SKY_GET_TOKEN(TKN_ALARM_EVERYWEEK_TITLE);
		pabAlarmMenu[7] = SKY_GET_TOKEN(TKN_ALARM_EVERYMONTH_TITLE);


		/* create scroll list */
        EQC_ScrollList( EQS_NA, EQS_NA, EQS_NA, EQC_GetScrollDisplayLine(), pabAlarmMenu,AMI_MAX, AMI_MAX, 0 );
        EQC_SET_STYLE(EQS_GetNthIOC(0), 
					(EQC_GET_STYLE(EQS_GetNthIOC(0))|CS_SCROLL_LIST_ENUMBERATED
													|CS_SCROLL_LIST_MAXIMIZE) );
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_APP_END:
        break;

    case SE_APP_RESUME:
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

	case SE_IO_SCROLL_LIST_SELECT: /* set */
        unIdx = pEvent->dwParam;
		switch( unIdx )
		{
		case AMI_AFTER_10M:       
		    sSetAlarmTime(AMI_AFTER_10M);
			EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_ALARM_MESSG_SET), 
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS );
			break;
        
		case AMI_AFTER_20M:        
		    sSetAlarmTime(AMI_AFTER_20M);
			EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_ALARM_MESSG_SET), 
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS );
			break;
        
        case AMI_AFTER_30M:        
		    sSetAlarmTime(AMI_AFTER_30M);
			EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_ALARM_MESSG_SET), 
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS );
			break;
        
		case AMI_AFTER_1H:        
		    sSetAlarmTime(AMI_AFTER_1H);
			EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_ALARM_MESSG_SET), 
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS );
			break;        
        case AMI_SET_ANYTIME:
		case AMI_SET_EVERYDAY:
		case AMI_SET_EVERYWEEK:
		case AMI_SET_EVERYMONTH:			
			EQS_START_CHILD_EVENT(sSKA_EditAdvancedAlarm, SE_APP_START,\
				                   (ALARM_TIME_TYPE_E)(unIdx+1), EQS_NA);
			break;
		default:
			break;
		}
		
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

enum tagALARM_ADVANCED_EDIT_MODE_E 
{
    AAEM_NORMAL_ALARM_TEXT_E,
    AAEM_NORMAL_ALRAM_COMBO_E,
    AAEM_HOLIDAY_ALARM_TEXT_E,
	AAEM_HOLIDAY_ALRAM_COMBO_E,
    AAEM_DAY_TEXT_E,
	AAEM_DAY_INFO_TEXT_E,
    AAEM_DAY_TIME_E,
    AAEM_TIME_TEXT_E,
	AAEM_TIME_TIME_E,
	AAEM_ALRAM_CONTENT_TEXT_E,
	AAEM_ALARM_CONTENT_TEDIT_E,
	AAEM_MAX_E
};

VOID SKY_API sSKA_EditAdvancedAlarm(SKY_EVENT_T *pEvent)
{
	UINT8                   nDayOfWeek;
    UINT8                   unCtrl;  
    BYTE                    abAlarmTitle[MAX_ALARM_TITLE+1];
    ALARM_MAKE_TYPE_E       EditType;
	DWORD                   dwSaveTime;

	UINT8            unIdx;
	clk_julian_type  cjAlarmTime;
	const  BYTE*            pbVaildTitle[1];	

	HCONTROL  hControl;


    switch( pEvent->EventID )
    {
    case SE_APP_START:
		/* set window */
		{
			BYTE*     ppszBuf[2];
			BYTE      szBuf[12];
			COORD     StartX, StartY;

            MARGIN_T Margin;
            COORD    xSpace, ySpace;
            COORD    xPos, yPos;
            
            EQS_StartFormatter();   
            EQS_GetControlMargin(&Margin);
            EQS_GetControlSpace(&xSpace, &ySpace);
            EQS_EndFormatter();

			EQS_SetWindow(  SKY_DEF_WIN_X, 
							SKY_DEF_WIN_Y, 
							SKY_DEF_WIN_DX, 
							SKY_DEF_WIN_DY, 
							CL_NONE_SELECT_SAVE, 
							(BYTE*)SKY_GET_TOKEN(TKN_SCH_ALARM_SET), 
							WA_SHOW_CML | WA_SHOW_HEADER);
     
			/* get the idx */
			EditType = (ALARM_MAKE_TYPE_E)pEvent->wParam;
			unIdx = (UINT8)pEvent->dwParam;

			StartX = Margin.xLeft;
			StartY = 1;//Margin.yTop + ySpace;

			ppszBuf[0] = (BYTE*)SKY_GET_TOKEN(TKN_ALARM_UNSET);
			ppszBuf[1] = (BYTE*)SKY_GET_TOKEN(TKN_ALARM_SET);

            /* rosa 2001-10-10 --> @why, what */
            hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NOMALDAY_SET); // 알람울림
			//sprintf(szBuf,"%s%s%s",SF_ALARM1_3,SF_ALARM2_3, SF_ALARM3_3); //알람울림
			//EQC_SetStaticText(hControl,szBuf);
            /* <-- */

			EQC_SET_USERDATA(hControl, EditType);
			StartX += EQC_GET_DX(hControl) + xSpace;
			
			hControl = EQC_ScrollCombo(StartX, StartY,6,1,ppszBuf,2,2,1);
			EQC_SET_USERDATA(hControl, unIdx);
			
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
			StartX = Margin.xLeft;
			StartY +=  EQC_GET_DY(hControl) + ySpace;

            /* rosa 2001-10-10 --> @why, what */
            hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_HOLIDAY_SET); // 휴일울림 
            //StartY +=  EQC_GET_DY(hControl) + ySpace;
			//sprintf(szBuf,"%s%s%s%s",SF_HOLLYDAY_ALARM1_4,SF_HOLLYDAY_ALARM2_4, SF_HOLLYDAY_ALARM3_4, SF_HOLLYDAY_ALARM4_4); //휴일울림
			//EQC_SetStaticText(hControl,szBuf);
            /* <-- */

			StartX += EQC_GET_DX(hControl) + xSpace;
			
			hControl = EQC_ScrollCombo(StartX, StartY,6,1,ppszBuf,2,2,1);
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));


			StartX = Margin.xLeft;
			StartY +=  EQC_GET_DY(hControl) + ySpace;

			hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
			EQC_SetStaticText(hControl, SF_DATE);
			StartX += EQC_GET_DX(hControl);
			switch( EditType )
			{
            case ATT_AFTER_10M:
			case ATT_AFTER_20M:
			case ATT_AFTER_30M:
			case ATT_AFTER_1H:
			case ATT_SET_ANYTIME:
				hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
				StartX += EQC_GET_DX(hControl) + xSpace;
				hControl = EQC_Time( StartX, StartY,0, CAL_TIME_CONTROL );//MONTH_DAY_CONTROL );
				EQC_SetTimeLimit(hControl,TRUE);
				EQC_SetTimeUpper(hControl,2015,12,31,EQS_NA,EQS_NA);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
				break;
			case ATT_EVERY_DAY:
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_SCH_DAY_ALARM_SET));
				hControl = EQC_StaticText(StartX, StartY, TKN_NULL);
				StartX += EQC_GET_DX(hControl) + xSpace;
                hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NEW_MENU1);
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
				break;
			case ATT_EVERY_WEEK:
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_SCH_WEEK_ALARM_SET));
				hControl = EQC_Time( StartX, StartY, 0,WEEK_TIME_CONTROL );
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

				StartX = EQC_GET_X(hControl) - EQS_CharWidth((BYTE)' ')*5;
				hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NEW_MENU2);				
				break;
			case ATT_EVERY_MONTH:			
				EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_SCH_MONTH_ALARM_SET));
				hControl = EQC_Time( StartX, StartY, 0,DAY_TIME_CONTROL );
				EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));

				StartX = EQC_GET_X(hControl) - EQS_CharWidth((BYTE)' ')*5;
				hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_NEW_MENU3);
				break;
			default:
				break;
			}

			StartX = Margin.xLeft;
			StartY += EQC_GET_DY(hControl) + ySpace;
			hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_TIME);
			EQC_SetStaticText(hControl, SF_TIME);

			StartX += EQC_GET_DX(hControl) + xSpace;
			hControl = EQC_Time( StartX, StartY, 0,TIME_MINUTE_CONTROL);
			EQC_SET_X(hControl, EQS_GetMaxX() - Margin.xRight - EQC_GET_DX(hControl));
						
			StartX = Margin.xLeft;
			StartY += EQC_GET_DY(hControl) + ySpace;
			        
			if( EditType == ATT_SET_ANYTIME ||\
				EditType == ATT_EVERY_DAY ||\
				EditType == ATT_EVERY_WEEK ||\
				EditType == ATT_EVERY_MONTH )
			{
				hControl = EQC_StaticText(StartX, StartY, TKN_ALARM_TIME_TITLE);
				EQC_SetStaticText(hControl, SF_TITLE);
				StartX += EQC_GET_DX(hControl) + xSpace;
				EQC_TextEdit(StartX, StartY, ALARM_SIZE_VIEW_LINE-5, 2, TEM_KOREAN, MAX_ALARM_TITLE, (BYTE *)"", 0);
			}
               
			if( unIdx > 0 )
			{ //기존 알람 수정
				DWORD dwAlarmTime;

				unIdx -= 1;				
				dwAlarmTime = SKY_GET_ALARM_TIME(unIdx);
				clk_secs_to_julian(dwAlarmTime, &cjAlarmTime);

				EQC_SetScrollLogicalIndex( EQS_GetNthIOC(AAEM_NORMAL_ALRAM_COMBO_E),\
					                       SKY_IS_ALARM_TIME_ACTIVE(unIdx));

				EQC_SetScrollLogicalIndex( EQS_GetNthIOC(AAEM_HOLIDAY_ALRAM_COMBO_E),\
					                       SKY_IS_ALARM_HOLIDAY_ACTIVE(unIdx));

				switch( EditType )
				{
				case ATT_AFTER_10M:
				case ATT_AFTER_20M:
				case ATT_AFTER_30M:
				case ATT_AFTER_1H:				
				case ATT_SET_ANYTIME:
					EQC_SetTimeYear(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.year);
					EQC_SetTimeMonth(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.month);
					EQC_SetTimeDay(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.day);
					break;
				case ATT_EVERY_DAY:					
					break;
				case ATT_EVERY_WEEK:
					EQC_SetTimeWeek(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.day_of_week);				
					break;
				case ATT_EVERY_MONTH:
					EQC_SetTimeDay(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.day);
					break;
				default:
					break;
				}				
				EQC_SetTimeHour(EQS_GetNthIOC(AAEM_TIME_TIME_E), cjAlarmTime.hour);
				EQC_SetTimeMinute(EQS_GetNthIOC(AAEM_TIME_TIME_E), cjAlarmTime.minute);	
				if( EditType == ATT_SET_ANYTIME ||\
					EditType == ATT_EVERY_DAY ||\
					EditType == ATT_EVERY_WEEK ||\
					EditType == ATT_EVERY_MONTH )
				{					
					EQC_SetTextEditData(EQS_GetNthIOC(AAEM_ALARM_CONTENT_TEDIT_E), SKY_GET_ALARM_TITLE(unIdx)); 
				}

				for(unCtrl = 0; unCtrl < EQS_GetIOCNum(); unCtrl++)         // for only view not edit
				{
					hControl = EQS_GetNthIOC(unCtrl);
					EQC_SET_EDIT(hControl,FALSE);	
					EQC_SET_FOCUS(hControl, FALSE);
				}

				EQS_SetWindowCML(CL_NONE_NONE_EDIT);
			}
			else
			{   //새 알람 생성 

				clk_secs_to_julian(SKY_GetLocalTime(), &cjAlarmTime);
				switch( EditType )
				{
				case ATT_AFTER_10M:
				case ATT_AFTER_20M:
				case ATT_AFTER_30M:
				case ATT_AFTER_1H:
					return ;
					break;
				case ATT_SET_ANYTIME:
					EQC_SetTimeYear(EQS_GetNthIOC(AAEM_DAY_TIME_E),  cjAlarmTime.year);
					EQC_SetTimeMonth(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.month);
					EQC_SetTimeDay(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.day);
					break;
				case ATT_EVERY_DAY:
					break;
				case ATT_EVERY_WEEK:
					EQC_SetTimeWeek(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.day_of_week);				
					break;
				case ATT_EVERY_MONTH:
					EQC_SetTimeDay(EQS_GetNthIOC(AAEM_DAY_TIME_E), cjAlarmTime.day);
					break;
				default:
					break;
				}					
				EQC_SetTimeHour(EQS_GetNthIOC(AAEM_TIME_TIME_E), cjAlarmTime.hour);
				EQC_SetTimeMinute(EQS_GetNthIOC(AAEM_TIME_TIME_E), cjAlarmTime.minute);	
			}
			//EQS_FitCanvasSize();
		}		        
        break;

    case SE_APP_END:

        break;
	case SE_SOFTKEY_EDIT:

		for(unCtrl = 0; unCtrl < EQS_GetIOCNum(); unCtrl++)         // for only view not edit
		{
			hControl = EQS_GetNthIOC(unCtrl);				
			EQC_SET_EDIT(hControl,TRUE);		
		}	
		EQS_SetWindowCML(CL_NONE_SELECT_SAVE);		
	
		EQC_SET_FOCUS(EQS_GetNthIOC(AAEM_NORMAL_ALRAM_COMBO_E),TRUE);
		EQS_SetIOCFocus(EQS_GetNthIOC(AAEM_NORMAL_ALRAM_COMBO_E));
		EQC_SET_FOCUS(EQS_GetNthIOC(AAEM_HOLIDAY_ALRAM_COMBO_E),TRUE);
		EQC_SET_FOCUS(EQS_GetNthIOC(AAEM_TIME_TIME_E),TRUE);

		EditType = EQC_GET_USERDATA( EQS_GetNthIOC(AAEM_NORMAL_ALARM_TEXT_E) );	
				
		switch( EditType )
		{
		case ATT_AFTER_10M:
		case ATT_AFTER_20M:
		case ATT_AFTER_30M:
		case ATT_AFTER_1H:
			return ;
			break;
		case ATT_SET_ANYTIME:
			EQC_SET_FOCUS(EQS_GetNthIOC(AAEM_DAY_TIME_E),TRUE);
			break;
		case ATT_EVERY_DAY:
			break;
		case ATT_EVERY_WEEK:
			EQC_SET_FOCUS(EQS_GetNthIOC(AAEM_DAY_TIME_E),TRUE);
			break;
		case ATT_EVERY_MONTH:
			EQC_SET_FOCUS(EQS_GetNthIOC(AAEM_DAY_TIME_E),TRUE);
			break;
		default:
			break;
		}
		EQC_SET_FOCUS(EQS_GetNthIOC(AAEM_ALARM_CONTENT_TEDIT_E),TRUE);	
		
		break;       
	case SE_SOFTKEY_SAVE:  
		EditType = EQC_GET_USERDATA( EQS_GetNthIOC(AAEM_NORMAL_ALARM_TEXT_E) );	
		unIdx    = EQC_GET_USERDATA( EQS_GetNthIOC(AAEM_NORMAL_ALRAM_COMBO_E) );

		if( EditType == ATT_SET_ANYTIME || EditType == ATT_AFTER_10M ||\
			EditType == ATT_AFTER_20M   || EditType == ATT_AFTER_30M ||\
			EditType == ATT_AFTER_1H    )
		{
			clk_julian_type  cjCurrentTime;
			DWORD CurrentTime;

			CurrentTime = SKY_GetLocalTime();
			clk_secs_to_julian(CurrentTime, &cjCurrentTime);
			cjAlarmTime = cjCurrentTime;

			cjAlarmTime.year  = EQC_GetTimeYear(EQS_GetNthIOC(AAEM_DAY_TIME_E));
			cjAlarmTime.month = EQC_GetTimeMonth(EQS_GetNthIOC(AAEM_DAY_TIME_E));
			cjAlarmTime.day = EQC_GetTimeDay(EQS_GetNthIOC(AAEM_DAY_TIME_E));			
			cjAlarmTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(AAEM_TIME_TIME_E));
			cjAlarmTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(AAEM_TIME_TIME_E));
		
			if( cjAlarmTime.year < cjCurrentTime.year )
			{
				hControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_INVALID_ALARM_SAVE_MSG),
								           MESSAGE_DEFAULT_TIME, 
										   MBT_NONE, 
										   MBT_FAIL);
				EQC_SET_USERDATA(hControl, pEvent->dwParam);
				break;
			}
			else if( cjAlarmTime.year == cjCurrentTime.year )
			{
				if( (cjAlarmTime.month < cjCurrentTime.month) ||\
					((cjAlarmTime.month == cjCurrentTime.month) && (cjAlarmTime.day < cjCurrentTime.day)) ||\
					((cjAlarmTime.month == cjCurrentTime.month) && (cjAlarmTime.day == cjCurrentTime.day) && (CurrentTime > clk_julian_to_secs(&cjAlarmTime))))
				{
					hControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_INVALID_ALARM_SAVE_MSG),
											   MESSAGE_DEFAULT_TIME, 
											   MBT_NONE, 
											   MBT_FAIL);
					EQC_SET_USERDATA(hControl, pEvent->dwParam);
					break;
				}
			}
			else
				;
			
		}		

		if( unIdx == 0 )
		{
			unIdx = sGetNewIdx();
		}
		else
		{
			unIdx--;
		}
        
		l_SaveAlarmAddress = unIdx;  // save alarm address 

		SKY_SET_ALARM_TIME_TYPE(unIdx, EditType);
		SKY_SET_ALARM_TIME_ACTIVE(unIdx, \
									 EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AAEM_NORMAL_ALRAM_COMBO_E)));		
		SKY_SET_ALARM_HOLIDAY_ACTIVE(unIdx,\
			                         EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AAEM_HOLIDAY_ALRAM_COMBO_E)));

			
		clk_secs_to_julian(SKY_GetLocalTime(), &cjAlarmTime);

		switch( EditType )
		{
		case ATT_SET_ANYTIME:
			cjAlarmTime.year  = EQC_GetTimeYear(EQS_GetNthIOC(AAEM_DAY_TIME_E));
			cjAlarmTime.month = EQC_GetTimeMonth(EQS_GetNthIOC(AAEM_DAY_TIME_E));
			cjAlarmTime.day = EQC_GetTimeDay(EQS_GetNthIOC(AAEM_DAY_TIME_E));
			if( cjAlarmTime.day > GetDaysOfMonth(cjAlarmTime.year, cjAlarmTime.month ) )
			{			  
				cjAlarmTime.day = GetDaysOfMonth(cjAlarmTime.year, cjAlarmTime.month ) ;
			}
			break;
		case ATT_EVERY_DAY:
			break;
		case ATT_EVERY_WEEK:
			dwSaveTime = clk_julian_to_secs(&cjAlarmTime);

			nDayOfWeek = EQC_GetTimeWeek(EQS_GetNthIOC(AAEM_DAY_TIME_E));
			dwSaveTime += (nDayOfWeek - cjAlarmTime.day_of_week ) * ALARM_SIZE_1H * 24;
			clk_secs_to_julian( dwSaveTime, &cjAlarmTime);
			cjAlarmTime.day_of_week = nDayOfWeek;
			break;
		case ATT_EVERY_MONTH:
			cjAlarmTime.day = EQC_GetTimeDay(EQS_GetNthIOC(AAEM_DAY_TIME_E));
			break;
		default:
			break;
		}
		cjAlarmTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(AAEM_TIME_TIME_E));
		cjAlarmTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(AAEM_TIME_TIME_E));
		SKY_SET_ALARM_TIME(unIdx, clk_julian_to_secs(&cjAlarmTime));
		
		SKY_SET_ALARM_TIME_USED(unIdx, FALSE);
		

		if( EditType == ATT_SET_ANYTIME ||\
			EditType == ATT_EVERY_DAY ||\
			EditType == ATT_EVERY_WEEK ||\
			EditType == ATT_EVERY_MONTH )
		{		        
			EQC_GetTextEditData(EQS_GetNthIOC(AAEM_ALARM_CONTENT_TEDIT_E), abAlarmTitle); 
			SKY_SET_ALARM_TITLE(unIdx, abAlarmTitle);
			if( EditType != ATT_SET_ANYTIME )
			{	
				clk_julian_type	 cjCurrentTime;

				clk_secs_to_julian(SKY_GetLocalTime(), &cjCurrentTime);

														
				if(  cjCurrentTime.hour > cjAlarmTime.hour )
				{
					SKY_SET_ALARM_TIME_USED(unIdx, TRUE);
				}
				else if( cjCurrentTime.hour == cjAlarmTime.hour )
				{
					if( cjCurrentTime.minute >= cjAlarmTime.minute )
					{
						SKY_SET_ALARM_TIME_USED(unIdx, TRUE);
					}
				}
				else ;

				if( EditType == ATT_EVERY_WEEK  )
				{
					if( cjCurrentTime.day_of_week != cjAlarmTime.day_of_week )
					{
						SKY_SET_ALARM_TIME_USED(unIdx, FALSE);
					}
				}
			}
		}
		else
		{
			if( cjAlarmTime.hour < 12)
			{	
				sprintf((char*)abAlarmTitle, "%02d:%02dAM", cjAlarmTime.hour, cjAlarmTime.minute);
			}
			else
			{
				sprintf((char*)abAlarmTitle, "%02d:%02dPM", 
						(cjAlarmTime.hour==12)?cjAlarmTime.hour:cjAlarmTime.hour-12, cjAlarmTime.minute);
			}
			SKY_SET_ALARM_TITLE(unIdx, abAlarmTitle);
		}
		SKY_WRITE_CONFIG(SC_APP);

		hControl = EQC_MessageBox( (BYTE *)SKY_GET_TOKEN(TKN_ALARM_MESSG_SET), 
									MESSAGE_DEFAULT_TIME, 
									MBT_NONE, 
									MBT_SUCCESS );
		if( pEvent->dwParam == EQS_NA )
			pEvent->dwParam = SE_KEY_CLEAR;
		EQC_SET_USERDATA(hControl , pEvent->dwParam );

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
		EQS_SEND_EVENT( SE_SOFTKEY_SAVE, EQS_NA, pEvent->dwParam );
		break;
    case SE_IO_MESGBOX_NO:
		EQS_END_TO(EQS_POP_TO_SUBROOT);
		break;
	case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SE_KEY_END)
		{
            EQS_END_TO(EQS_POP_TO_ROOT);
		}
        else if( pEvent->dwParam == SE_KEY_CLEAR )
		{
		    EQS_END_TO(EQS_POP_TO_SUBROOT);
		}
		else 
			;
		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


VOID SKY_API sSKA_PlayAlarm(SKY_EVENT_T* pEvent)
{    
    DWORD               dwStartTime;
    clk_julian_type     cjStartTime, cjCurrentTime;
	HCONTROL            hControl;
	BYTE                szBuf[40];

    static BOOL                 fKeyInput;
    static BYTE                 aszTitle[MAX_ALARM_TITLE+1];
    static BYTE                 aszContent[MAX_ALARM_TITLE+1];
    static BYTE                 aszContent2[MAX_ALARM_TITLE+1];
    static HCONTROL             hAniAlarm;
    static COLOR                cOldTextBack;    
    static TEXT_ALIGN           TextAlign;
    static ALARM_DISP_TYPE_T    AlarmType;
    static SCHL_RECORD_T		EventRecord;
    static pb_anniv_info_type   AnnivInfo;

#ifdef FEATURE_SKY_SUBLCD
    BYTE szAlarmTitle[MAX_ALARM_TITLE];
#endif
    
    switch(pEvent->EventID)
    {
    case SE_APP_START:    
		EQS_GetDC(TRUE);
		fKeyInput = FALSE;
        EQS_GetTextAlign(&TextAlign);
        //EQS_SetTextAlignHoriz(EHA_CENTER);
        //EQS_SetTextAlignVert(EVA_CENTER);
        cOldTextBack = EQS_GetTextBackColor();
        EQS_SetTextBackColor(EQS_GetBackColor());

        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_SELECT_NONE, 
                        NULL,
                        WA_NONE);//WA_SHOW_CML);

	    SKY_SET_IN_ALARM(TRUE);
        /* get the info */
        AlarmType.Type = (ALARM_DISP_TYPE_E)pEvent->wParam;
        AlarmType.nAddr = (INT8)pEvent->dwParam;
        
		memset(aszTitle, NULL, MAX_ALARM_TITLE+1);
		memset(aszContent, NULL, MAX_ALARM_TITLE+1);
        memset(aszContent2, NULL, MAX_ALARM_TITLE+1);

        if(AlarmType.Type == ADT_ANNIV) /* anniversary */
        {
			BYTE  szBuf[20];
			//hAniAlarm = EQC_Animation(16, 9, &g_AniSpecialalrm74x43x256c, NULL);
            /* rosa 01-08-21 --> @why, what */
            //hAniAlarm = EQC_Animation(23, 9, &g_AniSpecialalrm74x43x256c, NULL);
            hAniAlarm = EQC_Animation(ALARM_ANNIV_ANI_X, ALARM_ANNIV_ANI_Y, &g_AniAnniversary, NULL);
            /* <-- */
			EQC_SET_STYLE(hAniAlarm, CS_ANIMATION_REPEAT);
			EQC_StartAnimation(hAniAlarm, TRUE);

            AnnivInfo.address = AlarmType.nAddr;
            if( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) )
            {
				sprintf((char*)aszTitle, "[%s]", AnnivInfo.content);
                if(IsMiddleOfMultiByte((BYTE*)g_szPersonName[AnnivInfo.book_address-1], MAX_ALARM_TITLE))
                    strncpy((char*)aszContent, (char*)g_szPersonName[AnnivInfo.book_address-1], MAX_ALARM_TITLE-1);
                else
                    strncpy((char*)aszContent, (char*)g_szPersonName[AnnivInfo.book_address-1], MAX_ALARM_TITLE);
            }
			else
			{
				STRCPY(aszTitle, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ITEM_ANNIV));
				STRCPY(aszContent," ");
			}

#ifdef FEATURE_SKY_SUBLCD
            sprintf((char*)szBuf,"[%s]", AnnivInfo.content);
            MakeShortString(szBuf, SUB_CHAR_NUM);
            SKY_SubLCDTextOutStr(LN_UPPER, szBuf);//TKN_ALARM);//(BYTE*)"알람");
            /* SUB_CHAR_NUM만큼만 디스플레이 한다 */

            STRCPY(szBuf, aszContent);
            MakeShortString(szBuf, SUB_CHAR_NUM);
            SKY_SubLCDTextOutStr(LN_LOWER, szBuf);

#endif//#ifdef FEATURE_SKY_SUBLCD

			/* Playing Alarm */
			SKY_StopMusic();
			SKY_PlaySchedulerBell();
        }
        else if( AlarmType.Type == ADT_EVENT ) /* scheduler */
        {
			clk_secs_to_julian(SKY_GetLocalTime(), &cjCurrentTime);
			SKY_SeekReadSchlT(AlarmType.nAddr, (SCHL_RECORD_T*)&EventRecord);
			dwStartTime = EventRecord.dwStartTime;
            clk_secs_to_julian( dwStartTime, &cjStartTime );
    
            /* TITLE */
			if(IsMiddleOfMultiByte((BYTE*)EventRecord.abData, MAX_ALARM_TITLE))
				STRNCPY((BYTE*)aszTitle, (BYTE*)EventRecord.abData, MAX_ALARM_TITLE-1);
			else
				STRNCPY((BYTE*)aszTitle, (BYTE*)EventRecord.abData, MAX_ALARM_TITLE);
            /* EVENT DATE */
		    sprintf((char*)aszContent, 
				    SKY_GET_TOKEN(TKN_SCHEDULE_EVENT_ALARM_DAY_TITLE),
				    cjCurrentTime.year,
				    cjCurrentTime.month,
				    cjCurrentTime.day);
            /* EVENT START TIME */
		    sprintf((char*)aszContent2, 
				    SKY_GET_TOKEN(TKN_ALARM_TIME_FORMAT),
				    (cjStartTime.hour<12)?SKY_GET_TOKEN(TKN_DAT_AM):SKY_GET_TOKEN(TKN_DAT_PM),
				    (cjStartTime.hour<=12)?cjStartTime.hour:cjStartTime.hour-12,
				    cjStartTime.minute);

			//hAniAlarm = EQC_Animation(16, 9, &g_AniAlrm57x49x256c, NULL);
            /* rosa 01-08-21 --> @why, what */
			//hAniAlarm = EQC_Animation(33, 8, &g_AniAlrm57x49x256c, NULL);
            hAniAlarm = EQC_Animation(ALARM_GENERAL_ANI_X, ALARM_GENERAL_ANI_Y, &g_AniGeneralAlarm, NULL);
            /* <-- */
			EQC_SET_STYLE(hAniAlarm, CS_ANIMATION_REPEAT);
			EQC_StartAnimation(hAniAlarm, TRUE);

#ifdef FEATURE_SKY_SUBLCD
            if(aszTitle[0] == NULL) /* no content : TKN_ALARM_NO_TITLE */
            {
                STRCPY(aszTitle,SKY_GET_TOKEN(TKN_ALARM_NO_TITLE));
            }
			MakeShortString(aszTitle, SUB_CHAR_NUM);
            SKY_SubLCDTextOutStr(LN_UPPER, aszTitle);
            /* SUB_CHAR_NUM만큼만 디스플레이 한다 */
            STRCPY(szAlarmTitle, aszContent);
            MakeShortString(szAlarmTitle, SUB_CHAR_NUM);
            SKY_SubLCDTextOutStr(LN_LOWER, szAlarmTitle);
#endif//#ifdef FEATURE_SKY_SUBLCD

			/* Playing Alarm */
			SKY_StopMusic();
			SKY_PlaySchedulerBell();

        }
        else if( AlarmType.Type == ADT_ALARM ) /* PIM alarm */
        {
            dwStartTime = SKY_GET_ALARM_TIME( AlarmType.nAddr );
            clk_secs_to_julian( dwStartTime, &cjStartTime );

			/* set the title */
			if( SKY_GET_ALARM_TIME_TYPE(AlarmType.nAddr) ==  ATT_AFTER_10M ||
				SKY_GET_ALARM_TIME_TYPE(AlarmType.nAddr) ==  ATT_AFTER_20M ||
				SKY_GET_ALARM_TIME_TYPE(AlarmType.nAddr) ==  ATT_AFTER_30M ||
				SKY_GET_ALARM_TIME_TYPE(AlarmType.nAddr) ==  ATT_AFTER_1H  )
			{
			//	aszTitle[0] = 0;
				if(STRLEN(SKY_GET_ALARM_TITLE(AlarmType.nAddr)))
				{
					if( IsMiddleOfMultiByte((BYTE*)SKY_GET_ALARM_TITLE(AlarmType.nAddr), MAX_ALARM_TITLE) )
						STRNCPY((BYTE*)aszTitle, (BYTE*)SKY_GET_ALARM_TITLE(AlarmType.nAddr), MAX_ALARM_TITLE-1);
					else
						STRNCPY((BYTE*)aszTitle, (BYTE*)SKY_GET_ALARM_TITLE(AlarmType.nAddr), MAX_ALARM_TITLE);
				}
			}
			else
			{
				if(STRLEN(SKY_GET_ALARM_TITLE(AlarmType.nAddr)))
				{
					if( IsMiddleOfMultiByte((BYTE*)SKY_GET_ALARM_TITLE(AlarmType.nAddr), MAX_ALARM_TITLE) )
						STRNCPY((BYTE*)aszTitle, (BYTE*)SKY_GET_ALARM_TITLE(AlarmType.nAddr), MAX_ALARM_TITLE-1);
					else
						STRNCPY((BYTE*)aszTitle, (BYTE*)SKY_GET_ALARM_TITLE(AlarmType.nAddr), MAX_ALARM_TITLE);
				}
				else
				{
					STRNCPY((BYTE*)aszTitle, (BYTE*)SKY_GET_TOKEN(TKN_ALARM_NO_TITLE), STRLEN(SKY_GET_TOKEN(TKN_ALARM_NO_TITLE)));
				}
			}
#if 0
            if((SKY_GET_ALARM_TIME_TYPE(AlarmType.nAddr) == ATT_SET_ANYTIME) ||
			   (SKY_GET_ALARM_TIME_TYPE(AlarmType.nAddr) == ATT_EVERY_DAY))
#endif 
            {
				/* set the time */
		        sprintf((char*)aszContent, 
				        SKY_GET_TOKEN(TKN_ALARM_TIME_FORMAT),
				        (cjStartTime.hour<12)?SKY_GET_TOKEN(TKN_DAT_AM):SKY_GET_TOKEN(TKN_DAT_PM),
				        (cjStartTime.hour<=12)?cjStartTime.hour:cjStartTime.hour-12,
				        cjStartTime.minute);	

            }
            /* play animation */
			//hAniAlarm = EQC_Animation(16, 15, &g_AniAlrm57x49x256c, NULL);			
            /* rosa 01-08-21 --> @why, what */
            //hAniAlarm = EQC_Animation(33, 8, &g_AniAlrm57x49x256c, NULL);
            hAniAlarm = EQC_Animation(ALARM_GENERAL_ANI_X, ALARM_GENERAL_ANI_Y, &g_AniGeneralAlarm, NULL);   
            /* <-- */                     
			EQC_SET_STYLE(hAniAlarm, CS_ANIMATION_REPEAT);
			EQC_StartAnimation(hAniAlarm, TRUE);
            /* rosa 01-08-21 --> @why, what */
            //hControl = EQC_StaticText(10,93, TKN_BLANK);
            hControl = EQC_StaticText(4, ALARM_TITLE3_Y, TKN_BLANK);            
            /* <-- */
			clk_secs_to_julian(SKY_GetLocalTime(), &cjCurrentTime);
		    sprintf((char*)szBuf, 
				    SKY_GET_TOKEN(TKN_ALARM_PLAYTIME_FORMAT),\
					SKY_GET_TOKEN(TKN_ALARM_PLAY_CURRENT_TIME),\
				    (cjCurrentTime.hour<12)?SKY_GET_TOKEN(TKN_DAT_AM):SKY_GET_TOKEN(TKN_DAT_PM),
				    (cjCurrentTime.hour<=12)?cjCurrentTime.hour:cjCurrentTime.hour-12,
				    cjCurrentTime.minute);

			EQC_SetStaticText(hControl, szBuf);

#ifdef FEATURE_SKY_SUBLCD
            SKY_SubLCDTextOut(LN_UPPER, TKN_ALARM_LIST_TITLE);
            /* SUB_CHAR_NUM만큼만 디스플레이 한다 */
            STRCPY(szAlarmTitle, aszTitle);
            MakeShortString(szAlarmTitle, SUB_CHAR_NUM);
            SKY_SubLCDTextOutStr(LN_LOWER, szAlarmTitle);
#endif//#ifdef FEATURE_SKY_SUBLCD

            /* Playing Alarm */
			SKY_StopMusic();
			SKY_PlayAlarmBell();

        }
		EQS_StartTimer(EQS_TIMER_1, 60*1000, FALSE);
        SKY_SNI(HS_SNI_GENERAL_ALARM, TRUE);
        /* rosa 2001-10-09 --> Backlight 켜기 */
        SKY_SubBacklightOn(AB_ALWAYS);

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
		SKY_BacklightOn(AB_ALWAYS);
        /* <-- */
        break;

    case SE_APP_DRAW:
        /* draw text */
        /* rosa 01-08-21 --> @why, what */
        //EQS_TextOutStr(SKY_SCREEN_WIDTH/2-STRLEN(aszTitle)*EQS_CharWidth(' ')/2, 65, aszTitle);
        EQS_TextOutStr(SKY_SCREEN_WIDTH/2-STRLEN(aszTitle)*EQS_CharWidth(' ')/2, ALARM_TITLE1_Y, aszTitle);
        if(STRLEN(aszContent))
            //EQS_TextOutStr(SKY_SCREEN_WIDTH/2-STRLEN(aszContent)*EQS_CharWidth(' ')/2, 80, aszContent);
            EQS_TextOutStr(SKY_SCREEN_WIDTH/2-STRLEN(aszContent)*EQS_CharWidth(' ')/2, ALARM_TITLE2_Y, aszContent);
        if( AlarmType.Type == ADT_EVENT ) /* scheduler */
        {
            //EQS_TextOutStr(SKY_SCREEN_WIDTH/2-STRLEN(aszContent2)*EQS_CharWidth(' ')/2, 95, aszContent2);            
            EQS_TextOutStr(SKY_SCREEN_WIDTH/2-STRLEN(aszContent2)*EQS_CharWidth(' ')/2, ALARM_TITLE3_Y, aszContent2);
        }
        /* <-- */
        break;

    case SE_APP_END:
	    SKY_SET_IN_ALARM(FALSE);

        if( AlarmType.Type == ADT_ALARM ) /* PIM alarm */
        {
            /* delete alarm after playing */
			switch( SKY_GET_ALARM_TIME_TYPE(AlarmType.nAddr) )
			{
			case ATT_AFTER_10M:
			case ATT_AFTER_20M:
			case ATT_AFTER_30M:
			case ATT_AFTER_1H:
			case ATT_SET_ANYTIME:
				sDeleteAlarmTime(AlarmType.nAddr);
				break;
			case ATT_EVERY_DAY:
				SKY_SET_ALARM_TIME_USED(AlarmType.nAddr, TRUE);
				break;
			case ATT_EVERY_WEEK:
				SKY_SET_ALARM_TIME_USED(AlarmType.nAddr, TRUE);
				break;
			case ATT_EVERY_MONTH:
				SKY_SET_ALARM_TIME_USED(AlarmType.nAddr, TRUE);
				break;
			default:
				break;
			}  
			SKY_WRITE_CONFIG(SC_APP);
        }
		
        EQS_SetTextAlign(&TextAlign);
        EQS_SetTextBackColor(cOldTextBack);
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDDefault();
        //SKY_SubLCDEndText(LN_UPPER);
        //SKY_SubLCDEndText(LN_LOWER);
#endif//#ifdef FEATURE_SKY_SUBLCD
		SKY_SNI(HS_SNI_GENERAL_ALARM, FALSE);

        /* rosa 2001-10-09 --> Backlight 켜기 */
        SKY_SubBacklightOff();
        SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());

	    SKY_SET_NO_OFF_BACKLIGHT(FALSE);
	    SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        /* <-- */

		SKY_MotorStop();
		SKY_StopMusic();
		SKY_MusicPowerOff();
		EQS_ReleaseDC();
        break;

    case SE_SOFTKEY_SELECT:
    case SE_KEY_SEND:
    case SE_KEY_POUND:
    case SE_KEY_STAR:
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
    case SE_KEY_UP:
    case SE_KEY_DOWN:
    case SE_KEY_LEFT:
	case SE_KEY_RIGHT:
	case SE_KEY_END:
	case SE_KEY_CLEAR:
	case SE_KEY_PHONE_CLOSE:

	case SE_KEY_RECORD:
	case SE_KEY_AUTO_ANSWER: 

#if 0
		EQS_StopTimer(EQS_TIMER_1);
		SKY_StopMusic();
		SKY_MotorStop();
        EQS_END();
#endif
		if( fKeyInput == FALSE )
		{

			fKeyInput = TRUE;
			EQS_StopTimer(EQS_TIMER_1);
			EQS_StartTimer(EQS_TIMER_2, 1000, FALSE);		
		}

        break;
    case SE_TM_TIMER1:
#ifdef UPDATE_GVM_ALARM
	if(SKY_IS_FROM_GVM())
	{
		SKY_SET_FROM_GVM(FALSE);
		EQS_START_SIBLING(SKA_RunStandAloneGame, FALSE);
	}
	else
#endif //UPDATE_GVM_ALARM
	{
	        EQS_END();
	}
		break;
	case SE_TM_TIMER2:
		SKY_StopMusic();
		SKY_MotorStop();
#ifdef UPDATE_GVM_ALARM
	if(SKY_IS_FROM_GVM())
	{
		SKY_SET_FROM_GVM(FALSE);
		EQS_START_SIBLING(SKA_RunStandAloneGame, FALSE);
	}
	else
#endif //UPDATE_GVM_ALARM
	{
	        EQS_END();
	}
		break;
    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }    
}

static BOOL sSetAlarmTime(UINT8 unAlarmMenuItem)
{
	DWORD dwNewAlarmTime;
	UINT8 unIdx;
	clk_julian_type cjNewAlarmTime;
	BYTE abAlarmTitle[MAX_ALARM_TITLE+1];

	unIdx = sGetNewIdx();
	if(unIdx < MAX_ALARM_COUNT)
	{
		dwNewAlarmTime = SKY_GetLocalTime();
        /* type */
		switch(unAlarmMenuItem)
		{
		case AMI_AFTER_10M:			
			SKY_SET_ALARM_TIME_TYPE(unIdx, ATT_AFTER_10M);
            dwNewAlarmTime += ALARM_SIZE_10M;
			break;
		case AMI_AFTER_20M:
			SKY_SET_ALARM_TIME_TYPE(unIdx, ATT_AFTER_20M);
            dwNewAlarmTime += ALARM_SIZE_20M;
			break;
		case AMI_AFTER_30M:
			SKY_SET_ALARM_TIME_TYPE(unIdx, ATT_AFTER_30M);
            dwNewAlarmTime += ALARM_SIZE_30M;
			break;
		case AMI_AFTER_1H:
			SKY_SET_ALARM_TIME_TYPE(unIdx, ATT_AFTER_1H);
            dwNewAlarmTime += ALARM_SIZE_1H;
			break;
		}
		l_SaveAlarmAddress = unIdx;
		SKY_SET_ALARM_TIME_ACTIVE(unIdx, TRUE);
		SKY_SET_ALARM_HOLIDAY_ACTIVE(unIdx, TRUE);
		SKY_SET_ALARM_TIME_USED(unIdx, FALSE);
		
        /* time */
        SKY_SET_ALARM_TIME(unIdx, dwNewAlarmTime);
        /* title */
	    memset(abAlarmTitle, NULL, MAX_ALARM_TITLE+1);
        clk_secs_to_julian(dwNewAlarmTime, &cjNewAlarmTime);
#if 0
		sprintf((char*)abAlarmTitle, 
				SKY_GET_TOKEN(TKN_ALARM_TIME_FORMAT),
				(cjNewAlarmTime.hour<=12)?SKY_GET_TOKEN(TKN_DAT_AM):SKY_GET_TOKEN(TKN_DAT_PM),
				(cjNewAlarmTime.hour<=12)?cjNewAlarmTime.hour:cjNewAlarmTime.hour-12,
				cjNewAlarmTime.minute);		
#endif
        if( cjNewAlarmTime.hour < 12)
		{	
            sprintf((char*)abAlarmTitle, "%02d:%02dAM", cjNewAlarmTime.hour, cjNewAlarmTime.minute);
		}
        else
		{
            sprintf((char*)abAlarmTitle, "%02d:%02dPM", 
                    (cjNewAlarmTime.hour==12)?cjNewAlarmTime.hour:cjNewAlarmTime.hour-12, cjNewAlarmTime.minute);
		}


		SKY_SET_ALARM_TITLE(unIdx, abAlarmTitle);
		SKY_WRITE_CONFIG(SC_APP);

		
		return TRUE;
	}
	else /* error */
	{
		return FALSE;
	}
}

static BOOL sDeleteAlarmTime(UINT8 unIdxOfAlarm)
{
    if(unIdxOfAlarm < MAX_ALARM_COUNT)
    {
        SKY_SET_ALARM_TIME_TYPE(unIdxOfAlarm, INIT_ALARM_TIME_TYPE);
        SKY_SET_ALARM_TIME(unIdxOfAlarm, INIT_ALARM_TIME);
        SKY_SET_ALARM_TITLE(unIdxOfAlarm, INIT_ALARM_TITLE);		
		SKY_SET_ALARM_TIME_ACTIVE(unIdxOfAlarm,	INIT_ALARM_TIME_ACTIVE);
		SKY_SET_ALARM_HOLIDAY_ACTIVE(unIdxOfAlarm, INIT_ALARM_HOLIDAY_ACTIVE);
		SKY_SET_ALARM_TIME_USED(unIdxOfAlarm, INIT_ALARM_TIME_USED);
		SKY_WRITE_CONFIG(SC_APP);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static UINT8 sGetNewIdx(VOID)
{
	UINT8 i;
    for(i=0; i<MAX_ALARM_COUNT; i++)
    {
        /* get new idx */
        if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_NONE)
        {
			break;
        }
    }
	return i;
}

static  BOOL    sSortAlarm( INT8  anSortedIdxT[] )
{

    BOOL    fChecked[MAX_ALARM_COUNT], fFind, fSorted;
    UINT8   i,j,k,l;
    static  UINT8   unIdxOfMinValue;
    static  DWORD   dwMinValue;

    fSorted = FALSE;
    for( i=0; i<MAX_ALARM_COUNT; i++ )
    {
        if( SKY_GET_ALARM_TIME_TYPE(i) != ATT_NONE )
        {
            fChecked[i] = FALSE;
        }
        else
        {
            fChecked[i] = TRUE;
        }
    }

    k=0;
    for( i=0; i<MAX_ALARM_COUNT; i++ )
    {
        //find the initial small value.
        for( l=0; l<MAX_ALARM_COUNT; l++ )
        {
            if( fChecked[l] == FALSE )
            {
                dwMinValue = SKY_GET_ALARM_TIME(l);
                unIdxOfMinValue = l;
                fSorted = TRUE;
                break;
            }
        }
        if( l == MAX_ALARM_COUNT )
        {
            break;
        }

        //find the smallest value.
        fFind = FALSE;
        for( j=0; j<MAX_ALARM_COUNT; j++ )
        {
            if( fChecked[j] == FALSE )
            {
                if( SKY_GET_ALARM_TIME(j) < dwMinValue )
                {
                    dwMinValue = SKY_GET_ALARM_TIME(j);
                    unIdxOfMinValue = j;
                    fFind = TRUE;
                    //fChecked[j] = TRUE;
                }
            }
        }
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
        k++;
    }

    if( fSorted )
    {
        for( i=k; i<MAX_ALARM_COUNT; i++ )
        {
            anSortedIdxT[i] = ALARM_ERROR;
        }
    }

    return  fSorted;
}

VOID sFindAlarmOn( clk_julian_type *cjCurrentTime, ALARM_DISP_TYPE_T* AlarmResult )
{
	static              WORD  wCheckWorkIndex = 0;	
	static              WORD  wCheckAnnivIndex= 0;
	static              UINT8 uCheckAlarmIndex= 0;
	static              UINT8 PreviousMinute  = 61;

	WORD                nAnnivCount;
    UINT8               i;
    DWORD               dwSetTime, dwRecursiveDate, dwStartDate, dwAlarmTime, dwAlarmEndTime;
	DWORD               dwPlayAlarmTime;
    clk_julian_type     cjSetTime, cjRecursiveDate, cjStartDate, cjAlarmTime, cjAlarmEndTime;
    SCHL_ALARM_TIME_TYPE_E  AlarmType;
    SCHL_RECORD_TYPE_E  EventType;
    SCHL_EVENT_RECURSIVE_TYPE_E RecursiveType;

    INT8        nAnnivMonth;
    pb_anniv_info_type  AnnivInfo;
    UINT16      nIdxOfSearch;
    SCHL_FIND_SOLAR_DAY_T FindSloarDay;
    SCHL_FIND_HOLIDAY_T FindHoliday;


	if( clk_julian_to_secs( cjCurrentTime ) == SKY_INVALID_SYS_TIME )
	{
		AlarmResult->Type = ADT_NONE;
		AlarmResult->nAddr = -1;
		return;
	}

	if( cjCurrentTime->year <= 2000 )
	{
		AlarmResult->Type = ADT_NONE;
		AlarmResult->nAddr = -1;
		return;
	}


    if( l_fFirstAlarmCheck == TRUE && cjCurrentTime->hour > 0 && cjCurrentTime->minute > 0)
	{
		l_fFirstAlarmCheck = FALSE;
		l_fResetRecursiveAlarm = FALSE;

		for( i=0; i<MAX_ALARM_COUNT; i++ )
		{
			if( SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_DAY   ||\
				SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_WEEK  ||\
				SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_MONTH   )
			{
				dwSetTime = SKY_GET_ALARM_TIME( i );
				clk_secs_to_julian( dwSetTime, &cjSetTime );
				if( cjSetTime.hour > cjCurrentTime->hour|| 
					( cjSetTime.hour == cjCurrentTime->hour && cjSetTime.minute > cjCurrentTime->minute ) )
				{
					SKY_SET_ALARM_TIME_USED(i,FALSE);
				}
			}
		}
		for( i=0; i<SCHL_MAX_RECORD_COUNT; i++ )
		{
			EventType = SKY_GET_SCHL_PRE_CONFIG_TYPE(i);
            if( EventType == SRT_USED_EVENT_WITH_R_A )
			{
				dwStartDate = SKY_GET_SCHL_PRE_CONFIG_START_TIME(i);
				dwRecursiveDate = SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i);
				dwAlarmTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(i);

				clk_secs_to_julian(dwAlarmTime, &cjAlarmTime);
				clk_secs_to_julian(dwStartDate, &cjStartDate);
				clk_secs_to_julian(dwRecursiveDate, &cjRecursiveDate);
			
				dwSetTime = cjCurrentTime->year * 12 * 31 + cjCurrentTime->month * 31 + cjCurrentTime->day;
				dwStartDate = cjStartDate.year * 12 * 31 + cjStartDate.month * 31 + cjStartDate.day;
				dwRecursiveDate = cjRecursiveDate.year * 12 * 31 + cjRecursiveDate.month * 31 + cjRecursiveDate.day;
				
				if( dwSetTime >= dwStartDate && dwSetTime <= dwRecursiveDate )
				{
					clk_secs_to_julian(dwAlarmTime, &cjSetTime);
					if( cjCurrentTime->hour < cjSetTime.hour || 
						( cjCurrentTime->hour == cjSetTime.hour && cjCurrentTime->minute < cjSetTime.minute) )
					{
						sMakeUsedEvent(i, SRT_EVENT_WITH_R_A);
					}
				}
			}
		}			
	}

	if( cjCurrentTime->hour == 0 &&  PreviousMinute != 61 && l_fResetRecursiveAlarm == TRUE )
	{
		l_fFirstAlarmCheck = FALSE;
		l_fResetRecursiveAlarm = FALSE;

		for( i=0; i<MAX_ALARM_COUNT; i++ ) // 알람에 대해서 reset 
		{
			if( SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_DAY   ||\
				SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_WEEK  ||\
				SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_MONTH   )
			{					
				dwSetTime = SKY_GET_ALARM_TIME(i);
				clk_secs_to_julian( dwSetTime, &cjSetTime );
				if( cjCurrentTime->hour < cjSetTime.hour || 
					( cjCurrentTime->hour == cjSetTime.hour && cjCurrentTime->minute < cjSetTime.minute) )
				{
					SKY_SET_ALARM_TIME_USED(i, FALSE);
				}
			}
		}
		for( i=0; i<SCHL_MAX_RECORD_COUNT; i++ )
		{
			EventType = SKY_GET_SCHL_PRE_CONFIG_TYPE(i);
             if( EventType == SRT_USED_EVENT_WITH_R_A )
			{
				dwStartDate = SKY_GET_SCHL_PRE_CONFIG_START_TIME(i);
				dwRecursiveDate = SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i);
				dwAlarmTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(i);

				clk_secs_to_julian(dwAlarmTime, &cjAlarmTime);
				clk_secs_to_julian(dwStartDate, &cjStartDate);
				clk_secs_to_julian(dwRecursiveDate, &cjRecursiveDate);
			
				dwSetTime = cjCurrentTime->year * 12 * 31 + cjCurrentTime->month * 31 + cjCurrentTime->day;
				dwStartDate = cjStartDate.year * 12 * 31 + cjStartDate.month * 31 + cjStartDate.day;
				dwRecursiveDate = cjRecursiveDate.year * 12 * 31 + cjRecursiveDate.month * 31 + cjRecursiveDate.day;
				
				if( dwSetTime >= dwStartDate && dwSetTime <= dwRecursiveDate )
				{
					clk_secs_to_julian(dwAlarmTime, &cjSetTime);
					if( cjCurrentTime->hour < cjSetTime.hour || 
						( cjCurrentTime->hour == cjSetTime.hour && cjCurrentTime->minute < cjSetTime.minute) )
					{
						sMakeUsedEvent(i, SRT_EVENT_WITH_R_A);
					}
				}
			}
		}	
	}

	if( cjCurrentTime->hour > 0 )
	{
		l_fResetRecursiveAlarm = TRUE;
	}

	if( cjCurrentTime->minute != PreviousMinute )
	{
		wCheckAnnivIndex = 0;
		wCheckWorkIndex  = 0;
		uCheckAlarmIndex = 0;
	}
	
    // schedular anniv
    /* ----- add here ----- */
    /*
    1. 오늘 해당하는 기념일이 있니? : 양력/음력 검색 다
    2. Yes : 시간 비교한다. -> 바로 이시간! : 알람 디스플레이 화면으로 넘어간다. 
    3. No : 그냥 빠져나간다.
    */
	nAnnivCount = 0;
    if ( g_AlarmOfAnniv )
    {
        dwSetTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(SCHL_IDX_ANNIV_ALARM);
        clk_secs_to_julian( dwSetTime, &cjSetTime );
        if(( cjCurrentTime->hour == cjSetTime.hour ) &&
            ( cjCurrentTime->minute == cjSetTime.minute ))
        {
            nAnnivMonth = cjCurrentTime->month;
            nAnnivMonth = nAnnivMonth - SCHL_DIFF_LUNAL_CAL;
            if( nAnnivMonth < 1 )
            {
                nAnnivMonth = nAnnivMonth + 12;
            }
    
            for(i=0; i<=SCHL_DIFF_LUNAL_CAL; i++ )
            {
				nAnnivCount++;
				if( nAnnivCount > wCheckAnnivIndex )
				{
					if( nAnnivMonth > 12 )
					{
						nAnnivMonth = 1;
					}
					AnnivInfo.address = active_list.anniv_info[nAnnivMonth-1]; //solar
					while( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) > 0 )
					{
						if( AnnivInfo.fLunar ) //lunar
						{
							nIdxOfSearch = 0;
							GetSolarDay( &AnnivInfo.month, &AnnivInfo.day, &nIdxOfSearch, &FindSloarDay );
							while( FindSloarDay.fFind )
							{
								if(( FindSloarDay.unSMonth == cjCurrentTime->month ) && 
									( FindSloarDay.unSDay == cjCurrentTime->day ))
								{
									PreviousMinute = cjCurrentTime->minute;
									AlarmResult->Type = ADT_ANNIV;
									AlarmResult->nAddr = AnnivInfo.address;
									wCheckAnnivIndex = nAnnivCount;
									return;
								}
								nIdxOfSearch = FindSloarDay.nIdxOfTable + 1; // next search
								GetSolarDay( &AnnivInfo.month, &AnnivInfo.day, &nIdxOfSearch, &FindSloarDay );
							}
						}
						else //solar
						{
							if(( AnnivInfo.month == cjCurrentTime->month ) && 
								( AnnivInfo.day == cjCurrentTime->day ))
							{
								 PreviousMinute = cjCurrentTime->minute;
								 AlarmResult->Type = ADT_ANNIV;
								 AlarmResult->nAddr = AnnivInfo.address;
								 wCheckAnnivIndex = nAnnivCount;
								 return;
							}
						}
						AnnivInfo.address = AnnivInfo.link;
					}
					nAnnivMonth++;
				}
            }
        }
    }
	
    // schedular event
    for( i=wCheckWorkIndex; i<SCHL_MAX_RECORD_COUNT; i++ )
    {        
		if( SKY_GET_SCHL_PRE_CONFIG_ALARM_TYPE(i) != SATT_NONE )
		{
			EventType = SKY_GET_SCHL_PRE_CONFIG_TYPE(i);
			if(EventType == SRT_EVENT_WITH_A)
			{
				dwAlarmTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(i);
				clk_secs_to_julian(dwAlarmTime, &cjAlarmTime);

				if( (cjCurrentTime->year == cjAlarmTime.year) &&
					(cjCurrentTime->month == cjAlarmTime.month) &&
					(cjCurrentTime->day == cjAlarmTime.day) )
				{
					if( cjCurrentTime->hour > cjAlarmTime.hour ||
						( cjCurrentTime->hour == cjAlarmTime.hour && cjCurrentTime->minute >= cjAlarmTime.minute ))
					{					
						PreviousMinute = cjCurrentTime->minute;
						AlarmResult->Type = ADT_EVENT;
						AlarmResult->nAddr = i;
						wCheckWorkIndex = i+1;

						sMakeUsedEvent(i, SRT_USED_EVENT_WITH_A);
						return;
					}
				}
			}
			else if(EventType == SRT_EVENT_WITH_R_A)
			{      			
				dwStartDate = SKY_GET_SCHL_PRE_CONFIG_START_TIME(i);
				dwRecursiveDate = SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i);
				dwAlarmTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(i);
				//dwAlarmEndTime = SKY_GET_SCHL_PRE_CONFIG_ALARM_END_TIME(i);
				clk_secs_to_julian(dwAlarmTime, &cjAlarmTime);
				clk_secs_to_julian(dwStartDate, &cjStartDate);
				clk_secs_to_julian(dwRecursiveDate, &cjRecursiveDate);
			
				dwSetTime = cjCurrentTime->year * 12 * 31 + cjCurrentTime->month * 31 + cjCurrentTime->day;
				dwStartDate = cjStartDate.year * 12 * 31 + cjStartDate.month * 31 + cjStartDate.day;
				dwRecursiveDate = cjRecursiveDate.year * 12 * 31 + cjRecursiveDate.month * 31 + cjRecursiveDate.day;
				
				if( dwSetTime >= dwStartDate && dwSetTime <= dwRecursiveDate )
				{
					clk_secs_to_julian(dwAlarmTime, &cjAlarmTime);
					RecursiveType = SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(i);
					switch( RecursiveType )
					{
	/*
					case SERT_NONE:
						if( (cjCurrentTime->year == cjAlarmTime.year) &&
							(cjCurrentTime->month == cjAlarmTime.month) &&
							(cjCurrentTime->day == cjAlarmTime.day) &&
							(cjCurrentTime->hour == cjAlarmTime.hour) &&
							(cjCurrentTime->minute == cjAlarmTime.minute) )
						{
							AlarmResult->Type = ADT_EVENT;
							AlarmResult->nAddr = i;
							return;
						}
						break;
	*/
					case SERT_EVERY_DAY:
						if( cjCurrentTime->hour > cjAlarmTime.hour ||
							( cjCurrentTime->hour == cjAlarmTime.hour && cjCurrentTime->minute >= cjAlarmTime.minute ))
						{
							PreviousMinute = cjCurrentTime->minute;
							AlarmResult->Type = ADT_EVENT;
							AlarmResult->nAddr = i;
							wCheckWorkIndex = i+1;	
							
							sMakeUsedEvent(i, SRT_USED_EVENT_WITH_R_A);
							return;
						}
						break;
					case SERT_EVERY_WEEK:
						if( (cjCurrentTime->day_of_week == cjAlarmTime.day_of_week) &&
						    (cjCurrentTime->hour > cjAlarmTime.hour ||
							 ( cjCurrentTime->hour == cjAlarmTime.hour && cjCurrentTime->minute >= cjAlarmTime.minute )))
						{
							PreviousMinute = cjCurrentTime->minute;
							AlarmResult->Type = ADT_EVENT;
							AlarmResult->nAddr = i;
							wCheckWorkIndex = i+1;

							sMakeUsedEvent(i, SRT_USED_EVENT_WITH_R_A);
							return;
						}
						break;
					case SERT_EVERY_MONTH:
						if( (cjCurrentTime->day == cjAlarmTime.day) &&
						    (cjCurrentTime->hour > cjAlarmTime.hour ||
							 (cjCurrentTime->hour == cjAlarmTime.hour && cjCurrentTime->minute >= cjAlarmTime.minute )))
						{
							PreviousMinute = cjCurrentTime->minute;
							AlarmResult->Type = ADT_EVENT;
							AlarmResult->nAddr = i;
							wCheckWorkIndex = i+1;

							sMakeUsedEvent(i, SRT_USED_EVENT_WITH_R_A);
							return;
						}
						break;
					case SERT_EVERY_YEAR:
						if( (cjCurrentTime->month == cjAlarmTime.month) &&
							(cjCurrentTime->day == cjAlarmTime.day) &&
						    ( cjCurrentTime->hour > cjAlarmTime.hour ||
							  (cjCurrentTime->hour == cjAlarmTime.hour && cjCurrentTime->minute >= cjAlarmTime.minute )))
						{
							PreviousMinute = cjCurrentTime->minute;
							AlarmResult->Type = ADT_EVENT;
							AlarmResult->nAddr = i;
							wCheckWorkIndex = i+1;

							sMakeUsedEvent(i, SRT_USED_EVENT_WITH_R_A);
							return;
						}
						break;
					default:
						break;
					}
				}
			}
		}
    }


    // PIM alarm
    CheckHoliday( cjCurrentTime->day, &FindHoliday );

    AlarmResult->Type = ADT_NONE;
    AlarmResult->nAddr = -1;

	if( FindHoliday.Type == SHT_NONE && cjCurrentTime->day_of_week != 6 )
	{
		INT  k;
		BOOL fCheck;

		for( i=uCheckAlarmIndex; i<MAX_ALARM_COUNT; i++ )
		{
			k = SKY_GET_ALARM_TIME_TYPE(i);
			fCheck = SKY_IS_ALARM_TIME_ACTIVE(i);
			fCheck = SKY_IS_ALARM_TIME_USED(i);

			if( SKY_GET_ALARM_TIME_TYPE(i)           &&\
				SKY_IS_ALARM_TIME_ACTIVE(i) == TRUE  &&\
				SKY_IS_ALARM_TIME_USED(i)   == FALSE )  // != ATT_NONE
            {
				dwSetTime = SKY_GET_ALARM_TIME( i );
				clk_secs_to_julian( dwSetTime, &cjSetTime );			
				switch( SKY_GET_ALARM_TIME_TYPE(i) )
				{
				case ATT_AFTER_10M:     			  /* 10분후 */
				case ATT_AFTER_20M:                   /* 20분후 */
				case ATT_AFTER_30M:                   /* 30분후 */
				case ATT_AFTER_1H:                    /* 1시간후 */
				case ATT_SET_ANYTIME:                 /* 시간지정 */
					dwAlarmTime =  clk_julian_to_secs(cjCurrentTime);
					if( (dwAlarmTime / 60) >= (dwSetTime/60) )
					{
						AlarmResult->Type = ADT_ALARM;
						AlarmResult->nAddr = i;
						uCheckAlarmIndex = i+1;
						return;						
					}
					break;							
				case ATT_EVERY_DAY:                   /* 매일 */
					if( cjSetTime.hour == cjCurrentTime->hour )
					{
						if( cjSetTime.minute <= cjCurrentTime->minute )
						{
							AlarmResult->Type = ADT_ALARM;
							AlarmResult->nAddr = i;
							//SKY_SET_ALARM_TIME_USED(i, TRUE);
							//SKY_WRITE_CONFIG(SC_APP);
							uCheckAlarmIndex = i+1;
							return ;							
						}
					}
					else if( cjSetTime.hour  < cjCurrentTime->hour )
					{
						AlarmResult->Type = ADT_ALARM;
						AlarmResult->nAddr = i;
						//SKY_SET_ALARM_TIME_USED(i, TRUE);
						//SKY_WRITE_CONFIG(SC_APP);
						uCheckAlarmIndex = i+1;
						return ;					
					}
					else ;

					break;
				case ATT_EVERY_WEEK:                  /* 매주 */
					if( cjSetTime.day_of_week == cjCurrentTime->day_of_week )
					{
						if( cjSetTime.hour == cjCurrentTime->hour )
						{
							if( cjSetTime.minute <= cjCurrentTime->minute )
							{
								AlarmResult->Type = ADT_ALARM;
								AlarmResult->nAddr = i;
								//SKY_SET_ALARM_TIME_USED(i, TRUE);
								//SKY_WRITE_CONFIG(SC_APP);
								uCheckAlarmIndex = i+1;
								return ;
							}
						}
						else if( cjSetTime.hour  < cjCurrentTime->hour )
						{
							AlarmResult->Type = ADT_ALARM;
							AlarmResult->nAddr = i;
							//SKY_SET_ALARM_TIME_USED(i, TRUE);
							//SKY_WRITE_CONFIG(SC_APP);
							uCheckAlarmIndex = i+1;
							return ;
						}
						else ;
					}
					break;
				case ATT_EVERY_MONTH:                  /* 매달 */
					if( cjSetTime.day == cjCurrentTime->day )
					{
						if( cjSetTime.hour == cjCurrentTime->hour )
						{
							if( cjSetTime.minute <= cjCurrentTime->minute )
							{
								AlarmResult->Type = ADT_ALARM;
								AlarmResult->nAddr = i;
								//SKY_SET_ALARM_TIME_USED(i, TRUE);
								//SKY_WRITE_CONFIG(SC_APP);
								uCheckAlarmIndex = i+1;
								return ;
							
							}
						}
						else if( cjSetTime.hour  < cjCurrentTime->hour )
						{
							AlarmResult->Type = ADT_ALARM;
							AlarmResult->nAddr = i;
							//SKY_SET_ALARM_TIME_USED(i, TRUE);
							//SKY_WRITE_CONFIG(SC_APP);
							uCheckAlarmIndex = i+1;
							return;								
						}
						else ;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	else
	{
		for( i=uCheckAlarmIndex; i<MAX_ALARM_COUNT; i++ )
		{
			int  s;
			s = SKY_IS_ALARM_HOLIDAY_ACTIVE(i);
			s = SKY_IS_ALARM_TIME_USED(i);
			if( SKY_GET_ALARM_TIME_TYPE(i)           &&\
				SKY_IS_ALARM_HOLIDAY_ACTIVE(i)   == TRUE  &&\
				SKY_IS_ALARM_TIME_USED(i)        == FALSE )  // != ATT_NONE
            {
				dwSetTime = SKY_GET_ALARM_TIME( i );
				clk_secs_to_julian( dwSetTime, &cjSetTime );
				switch( SKY_GET_ALARM_TIME_TYPE(i) )
				{
				case ATT_AFTER_10M:     			  /* 10분후 */
				case ATT_AFTER_20M:                   /* 20분후 */
				case ATT_AFTER_30M:                   /* 30분후 */
				case ATT_AFTER_1H:                    /* 1시간후 */
				case ATT_SET_ANYTIME:                 /* 시간지정 */
					dwAlarmTime =  clk_julian_to_secs(cjCurrentTime);
					if( (dwAlarmTime / 60) >= (dwSetTime/60) )
					{
						AlarmResult->Type = ADT_ALARM;
						AlarmResult->nAddr = i;
						uCheckAlarmIndex = i+1;
						return;
					}
					break;							
				case ATT_EVERY_DAY:                   /* 매일 */
					if( cjSetTime.hour == cjCurrentTime->hour )
					{
						if( cjSetTime.minute <= cjCurrentTime->minute )
						{
							AlarmResult->Type = ADT_ALARM;
							AlarmResult->nAddr = i;
							//SKY_SET_ALARM_TIME_USED(i, TRUE);
							//SKY_WRITE_CONFIG(SC_APP);
							uCheckAlarmIndex = i+1;
							return;
						}
					}
					else if( cjSetTime.hour  < cjCurrentTime->hour )
					{
							AlarmResult->Type = ADT_ALARM;
							AlarmResult->nAddr = i;
							//SKY_SET_ALARM_TIME_USED(i, TRUE);
							//SKY_WRITE_CONFIG(SC_APP);
							uCheckAlarmIndex = i+1;
							return ;
					}
					else ;

					break;
				case ATT_EVERY_WEEK:                  /* 매주 */
					if( cjSetTime.day_of_week == cjCurrentTime->day_of_week )
					{
						if( cjSetTime.hour == cjCurrentTime->hour )
						{
							if( cjSetTime.minute <= cjCurrentTime->minute )
							{
								AlarmResult->Type = ADT_ALARM;
								AlarmResult->nAddr = i;
								//SKY_SET_ALARM_TIME_USED(i, TRUE);
								//SKY_WRITE_CONFIG(SC_APP);
								uCheckAlarmIndex = i+1;
								return ;
							}
						}
						else if( cjSetTime.hour  < cjCurrentTime->hour )
						{

							AlarmResult->Type = ADT_ALARM;
							AlarmResult->nAddr = i;
							//SKY_SET_ALARM_TIME_USED(i, TRUE);
							//SKY_WRITE_CONFIG(SC_APP);
							uCheckAlarmIndex = i+1;
							return ;
						}
						else ;
					}
					break;
				case ATT_EVERY_MONTH:                  /* 매달 */
					if( cjSetTime.day == cjCurrentTime->day )
					{
						if( cjSetTime.hour == cjCurrentTime->hour )
						{
							if( cjSetTime.minute <= cjCurrentTime->minute )
							{
								AlarmResult->Type = ADT_ALARM;
								AlarmResult->nAddr = i;
								//SKY_SET_ALARM_TIME_USED(i, TRUE);
								//SKY_WRITE_CONFIG(SC_APP);
								uCheckAlarmIndex = i+1;
								return ;								
							}
						}
						else if( cjSetTime.hour  < cjCurrentTime->hour )
						{

								AlarmResult->Type = ADT_ALARM;
								AlarmResult->nAddr = i;
								//SKY_SET_ALARM_TIME_USED(i, TRUE);
								//SKY_WRITE_CONFIG(SC_APP);	
								uCheckAlarmIndex = i+1;
								return;
						}
						else ;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	if( cjCurrentTime->minute != PreviousMinute )
	{
		PreviousMinute = cjCurrentTime->minute;
	}

    SKY_WRITE_CONFIG(SC_APP);
    AlarmResult->Type = ADT_NONE;
    AlarmResult->nAddr = -1;
    return;
}


static void sMakeUsedEvent(INT16 nAddress, SCHL_RECORD_TYPE_E RecordType)
{
	SCHL_RECORD_T EventRecord;

	SKY_SeekReadSchlT(nAddress, (SCHL_RECORD_T*)&EventRecord);
	SKY_SET_SCHL_PRE_CONFIG_TYPE(nAddress, RecordType);	
	EventRecord.Type = RecordType;
	SKY_SeekWriteSchlT(nAddress, &EventRecord);
}
