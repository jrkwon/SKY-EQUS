/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_stop.c
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
** 2001-01-09 white       modified.
** 2000-12-20 redstar     Created.
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
#include <string.h>
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#if SKY_MODEL_ID == SKY_MODEL_B0
#define VIEW_STOP_WATCH_LINE   9
#else
#define VIEW_STOP_WATCH_LINE   8
#endif

#define MAX_STOP_WATCH   20

#define sMAX_LENGTH      (10)

#define STOPWATCH_LOG_X         45           
#define STOPWATCH_LOG_Y         2
#define STOPWATCH_TIME_X        45
#define STOPWATCH_TIME_Y        93
#define STOPWATCH_ANI_X         8
#define STOPWATCH_ANI_Y         26
#define STOPWATCH_SCREEN_X      8
#define STOPWATCH_SCREEN_Y      85

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/


struct tagSTOP_WATCH_T
{
    UINT sw_Min;
    UINT sw_Sec;
    UINT sw_Sec100;
};

typedef struct tagSTOP_WATCH_T  STOP_WATCH_T;


enum tagSTOP_WATCH_E
{    
    SW_LOG = 0,
    SW_MIN ,
    SW_SEC,
    SW_SEC100   
};
typedef enum tagSTOP_WATCH_E   STOP_WATCH_E;



static STOP_WATCH_T   StopWatch[MAX_STOP_WATCH];
static UINT8          nStopWatchIndex;
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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void sMakeStopWatchString(BYTE* pszString, UINT8 nIndex);

LOCAL void SKY_API sSKA_ViewStopWatch(SKY_EVENT_T *pEvent);


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainStopWatch()
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

void SKY_API SKA_MainStopWatch(SKY_EVENT_T *pInEvent)
{
    static qword old_time, new_time,tmp_time;
    static STOP_WATCH_T CurrentWatch;
    static DWORD dwTime;
	static UINT   tmp_Elapsed;
    DRAW_MODE     DrawMode;
    UINT   sw_Elapsed;
    COLOR OldColor,OldTextColor;
    FONT_TYPE OldFont;
    HCONTROL    hControl;

    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */      
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_START_NONE, 
            SKY_GET_TOKEN(TKN_STOPWATCH), 
            WA_SHOW_HEADER | WA_SHOW_CML);

       hControl = EQC_Animation( STOPWATCH_ANI_X,            
                                 STOPWATCH_ANI_Y, 
                                 &g_AniRunningCheetah,
                                 NULL);           
       EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
        //EQS_SetDrawMode(DM_OR);
        
       /* draw frame */
        EQC_Image(STOPWATCH_SCREEN_X,STOPWATCH_SCREEN_Y,(BITMAP*)&g_BmpStopWatchPannel);

		tmp_Elapsed =0;
        qw_set(old_time,0,0);
        qw_set(new_time,0,0);
        nStopWatchIndex = 0;
        CurrentWatch.sw_Min    = 0;
        CurrentWatch.sw_Sec    = 0;
        CurrentWatch.sw_Sec100 = 0;
        dwTime = 0;

        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        UI_ENABLE_SLEEP();
        SKY_SetGeneralTimer(0, FALSE);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;  
        
    case SE_APP_DRAW:
		sw_Elapsed = qw_lo( new_time ) - qw_lo( old_time );
		sw_Elapsed -=tmp_Elapsed;
				
		CurrentWatch.sw_Sec100  = sw_Elapsed/10;
		CurrentWatch.sw_Sec    = CurrentWatch.sw_Sec100 / 100 ;
        CurrentWatch.sw_Sec100   %=  100;
        CurrentWatch.sw_Min     = CurrentWatch.sw_Sec / 60 ;
        CurrentWatch.sw_Sec      %=  60;
        
        DrawMode = EQS_GetDrawMode();
        EQS_SetDrawMode(DM_OR);
        OldColor = EQS_GetTextBackColor();
        EQS_SetTextBackColor(EQS_GetBackColor());
        OldTextColor = EQS_GetTextColor();
        EQS_TextOutStr(8, STOPWATCH_LOG_Y, (BYTE*)SKY_GET_TOKEN(TKN_STOPWATCH_RECORD));
        EQS_TextOutStrF(STOPWATCH_LOG_X, STOPWATCH_LOG_Y, "%02d", nStopWatchIndex);
        OldFont = EQS_GetFontType();
        EQS_SetFontType(FT_BIGBIG);
        EQS_SetTextColor(WHITE);
        EQS_TextOutStrF(STOPWATCH_TIME_X, STOPWATCH_TIME_Y, 
                        " %02d'%02d\"%02d", CurrentWatch.sw_Min, CurrentWatch.sw_Sec, CurrentWatch.sw_Sec100);
        EQS_SetFontType(OldFont);
        EQS_SetTextBackColor(OldColor);
        EQS_SetTextColor(OldTextColor);
        EQS_SetDrawMode(DrawMode);
        break;
        
    case SE_TM_TIMER:
        UI_DISABLE_SLEEP();
        dwTime++;
        clk_uptime_ms(new_time);
	    
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);

        break;

    case SE_SOFTKEY_START:
        clk_uptime_ms(old_time);
        SKY_SetGeneralTimer(107, TRUE);
        hControl = EQS_GetNthIOC(0);
        EQC_StartAnimation(hControl, TRUE);        
        EQS_SetWindowCML(CL_NONE_SAVE_STOP);

        break;

    case SE_SOFTKEY_SAVE:
        if(nStopWatchIndex < MAX_STOP_WATCH)
        {
            StopWatch[nStopWatchIndex].sw_Min    = CurrentWatch.sw_Min;
            StopWatch[nStopWatchIndex].sw_Sec    = CurrentWatch.sw_Sec;
            if(StopWatch[nStopWatchIndex-1].sw_Sec100 == CurrentWatch.sw_Sec100)
                CurrentWatch.sw_Sec100++;
            StopWatch[nStopWatchIndex++].sw_Sec100 = CurrentWatch.sw_Sec100;

            if(nStopWatchIndex  >= MAX_STOP_WATCH)
                EQS_SetWindowCML(CL_NONE_NONE_STOP);
            
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
        }
        break;

    case SE_SOFTKEY_STOP:
		clk_uptime_ms(tmp_time);
        SKY_SetGeneralTimer(0, FALSE);
        //memcpy(old_time, new_time, sizeof(qword));
        hControl = EQS_GetNthIOC(0);
        EQC_StopAnimation(hControl);        
        if(nStopWatchIndex == 0)
            EQS_SetWindowCML(CL_NONE_CONTINUE_RESET );
        else
            EQS_SetWindowCML(CL_VIEW_CONTINUE_RESET );

        break;

    case SE_SOFTKEY_CONTINUE:
		clk_uptime_ms(new_time);
		tmp_Elapsed += (qw_lo(new_time)-qw_lo(tmp_time));
		SKY_SetGeneralTimer(107, TRUE);
        if(nStopWatchIndex  >= MAX_STOP_WATCH)
            EQS_SetWindowCML(CL_NONE_NONE_STOP);
        else
            EQS_SetWindowCML(CL_NONE_SAVE_STOP);

        hControl = EQS_GetNthIOC(0);
        EQC_StartAnimation(hControl, TRUE);        
        break;

    case SE_SOFTKEY_RESET:
        EQS_START_SIBLING(SKA_MainStopWatch, FALSE);
        break;
    
    case SE_SOFTKEY_CANCEL:   
        EQS_END();
        break;

    case SE_SOFTKEY_VIEW:
        EQS_START_CHILD(sSKA_ViewStopWatch);
        break;
    
    default :
        EQS_DEF_EVENT_PROC(pInEvent);
        break;
    }
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_ViewStopWatch()
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
LOCAL void SKY_API sSKA_ViewStopWatch(SKY_EVENT_T *pInEvent)
{
    HCONTROL hControl;
    UINT8   i;
    BYTE*   pTempList[MAX_STOP_WATCH];
    STATIC BYTE s_aszStopWatch[MAX_STOP_WATCH][sMAX_LENGTH+1];

    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_SWATCH_RECORD), 
            WA_SHOW_HEADER);

        if( nStopWatchIndex == 0 )
        {
            EQS_END();
            break;
        } 
        
        for(i = 0; i< nStopWatchIndex; i++)
        {
            sMakeStopWatchString(s_aszStopWatch[i], i);
            pTempList[i] = s_aszStopWatch[i];
        }

        hControl = EQC_ScrollList(1,1,18,VIEW_STOP_WATCH_LINE, \
                    pTempList, nStopWatchIndex, nStopWatchIndex,0);

        EQC_SET_STYLE( hControl, CS_SCROLL_LIST_MAXIMIZE|CS_SCROLL_LIST_ENUMBERATED|CS_SCROLL_LIST_NO_CML); 
        EQS_SetIOCActive(hControl);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_IO_SCROLL_LIST_CHG:
        EQC_GetScrollPhysicalIndex(pInEvent->wParam);
        break;
    
    case SE_IO_SCROLL_LIST_SELECT:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_SOFTKEY_CANCEL:
    case SE_IO_SCROLL_LIST_CANCEL:
    case SE_KEY_CLEAR:
        EQS_END();
        break;
    
    default :
        EQS_DEF_EVENT_PROC(pInEvent);
        break;
    }
}

LOCAL void sMakeStopWatchString(BYTE* pszString, UINT8 nIndex)
{
    sprintf((CHAR*)pszString," %02d'%02d\"%02d", StopWatch[nIndex].sw_Min, \
                                          StopWatch[nIndex].sw_Sec,\
                                          StopWatch[nIndex].sw_Sec100
                                          );
}



