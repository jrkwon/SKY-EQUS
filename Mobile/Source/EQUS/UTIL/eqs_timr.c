/*
** ---------------------------------------------------------------------------
**
** Project: 
**     EQUS
**
** File:
**     EQS_timr.c
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-09-23   redstar    edit 
** 00-03-23   redstar    Created.
**
** ---------------------------------------------------------------------------
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"

/**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define MAX_LOCAL_TIMER            100//(MAX_IOC_NUM*2)
#define MAX_GLOBAL_TIMER            20
#define TIMER_TIME_PERIOD          100    // 100ms

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
            
/*
**---------------------------------------------------------------------------
**  General Declarations
**---------------------------------------------------------------------------
*/
enum tagTIMER_TYPE_T
{
    LOCAL_TIMER,
    GLOBAL_TIMER,
    APPLET_TIMER,
    CONTROL_TIMER,
    SWINDOW_TIMER
};

typedef enum tagTIMER_TYPE_T  TIMER_TYPE_T;

struct tagTIMER_T 
{
    HCONTROL        hControl;
    TIMER_TYPE_T    Type;
    EQS_TIMER_E     TimerID;
	BOOL            fOnUse;
	DWORD           dwPeriod;
	QWORD           qwStartTime;
	BOOL            fRepeat;
};

typedef struct tagTIMER_T           TIMER_T;

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
TIMER_T     g_aLocalTimer[MAX_LOCAL_TIMER+1];
TIMER_T     g_aGlobalTimer[MAX_GLOBAL_TIMER+1];

UINT8       g_anTimerCount[EQS_TIMER_END];//+1];
//UINT16      g_anTimerCount;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL UINT8  g_nEndTimerIndex;
LOCAL UINT8  g_nStartTimerIndex;

LOCAL UINT8  g_nGlobalIndex;



/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL HTIMER EQS_API     sAllocTimer(TIMER_TYPE_T TimerType);
LOCAL EQS_RESULT EQS_API sFreeTimer(HTIMER hTimer, TIMER_TYPE_T TimerType);
LOCAL HTIMER EQS_API     sDeserveTimer(HTIMER StartTimerA);
LOCAL EQS_RESULT EQS_API sStopTimer(HTIMER hTimer);



/*
**---------------------------------------------------------------------------
**  Function(external use only) Definition
**---------------------------------------------------------------------------
*/

LOCAL INT s_TimerCount(void)
{
	INT nTimer = 0;
	UINT8 Handle;
    
    for( Handle = 0; Handle < EQS_TIMER_END; Handle++)
	{
		nTimer += g_anTimerCount[Handle] ;
	}
    return nTimer;
}


LOCAL HTIMER EQS_API sAllocTimer(TIMER_TYPE_T TimerType)
{
    HTIMER Handle;
    if( TimerType == LOCAL_TIMER )
    {
        for( Handle=g_nStartTimerIndex; Handle<=MAX_LOCAL_TIMER; Handle++ )
        {
            if( g_aLocalTimer[Handle].fOnUse == FALSE )
            {
                g_aLocalTimer[Handle].fOnUse = TRUE;
                break;
            }
        }
        if( Handle > MAX_LOCAL_TIMER )
            return INVALID_TIMER_HANDLE;
        if( Handle >= g_nEndTimerIndex )
            g_nEndTimerIndex = Handle+1;
    }
    else if( TimerType == GLOBAL_TIMER )
    {        
        for( Handle= 1; Handle<=MAX_GLOBAL_TIMER; Handle++)
        {
            if( g_aGlobalTimer[Handle].fOnUse == FALSE )
            {
                g_aGlobalTimer[Handle].fOnUse = TRUE;
                break;
            }
        }
        if( Handle > MAX_GLOBAL_TIMER )
            return INVALID_TIMER_HANDLE;
        if( Handle > g_nGlobalIndex )
            g_nGlobalIndex = Handle;
    }
    else
        return INVALID_TIMER_HANDLE;

    return Handle;
}

LOCAL EQS_RESULT EQS_API sFreeTimer(HTIMER hTimer, TIMER_TYPE_T TimerType)
{
    if( TimerType == LOCAL_TIMER )
    {
        if( hTimer >= g_nStartTimerIndex && hTimer < g_nEndTimerIndex )
        {
            g_aLocalTimer[hTimer].fOnUse = FALSE ;
            if( hTimer == (g_nEndTimerIndex-1) )
                g_nEndTimerIndex--;
            return RESULT_OK;
        }
    }
    else if( TimerType == GLOBAL_TIMER )
    {
        if( hTimer >= 1  &&  hTimer <= MAX_GLOBAL_TIMER )
        {
            g_aGlobalTimer[hTimer].fOnUse = FALSE;
            if( hTimer == g_nGlobalIndex )
                g_nGlobalIndex--;
        }
        return RESULT_OK;
    }
    else ;

    return RESULT_ERROR;
}


LOCAL HTIMER EQS_API sStartTimer(HCONTROL hControl, TIMER_TYPE_T TimerType, \
                           EQS_TIMER_E TimerID, DWORD dwPeriod, BOOL fRepeat)
{
    HTIMER TimerHandle;
	
    if( TimerID == 0 || TimerID >= EQS_TIMER_END)
        return INVALID_TIMER_HANDLE;

    for( TimerHandle = g_nStartTimerIndex; TimerHandle < g_nEndTimerIndex; TimerHandle++)
    {
        if( g_aLocalTimer[TimerHandle].fOnUse == TRUE )
        {
            if( g_aLocalTimer[TimerHandle].hControl == hControl &&  \
                g_aLocalTimer[TimerHandle].TimerID == TimerID  )
            {
                g_aLocalTimer[TimerHandle].Type        = TimerType;   //redstar 2000.12.26 중복된 타이머는 재설정
                g_pfnGetSystemTime(g_aLocalTimer[TimerHandle].qwStartTime);
                g_aLocalTimer[TimerHandle].dwPeriod    = dwPeriod;
                g_aLocalTimer[TimerHandle].fRepeat     = fRepeat;
                return TimerHandle;                
            }
        }
    }

    if( TimerHandle >= g_nEndTimerIndex )
    {
        if( dwPeriod > 0 )
        {
            TimerHandle = sAllocTimer(LOCAL_TIMER);
            if( TimerHandle == 0 )
                return INVALID_TIMER_HANDLE;
            g_aLocalTimer[TimerHandle].hControl    = hControl;
            g_aLocalTimer[TimerHandle].Type        = TimerType;
            g_pfnGetSystemTime( g_aLocalTimer[TimerHandle].qwStartTime );
            g_aLocalTimer[TimerHandle].dwPeriod    = dwPeriod;
            g_aLocalTimer[TimerHandle].fRepeat     = fRepeat;
            g_aLocalTimer[TimerHandle].TimerID     = TimerID;
            g_anTimerCount[TimerID]++;
#if 0
            if( g_anTimerCount[TimerID] == 1 )
                g_pfnSetTimer( TimerID, TIMER_TIME_PERIOD, TRUE );
#endif 
			if( s_TimerCount() == 1 )
				g_pfnSetTimer( EQS_TIMER_1, TIMER_TIME_PERIOD, TRUE );

        }
    }
    else   // edit old timer information
    {
        if( dwPeriod != 0 )
        {
            g_pfnGetSystemTime( g_aLocalTimer[TimerHandle].qwStartTime);
            g_aLocalTimer[TimerHandle].dwPeriod  = dwPeriod;
            g_aLocalTimer[TimerHandle].fRepeat   = fRepeat;
        }
        else
        {
            sStopTimer(TimerHandle);
        }        
    }

    return TimerHandle;
}
/*
LOCAL HTIMER EQS_API sDeserveTimerEvent(EQS_EVENT_ID EventID, TIMER_TYPE_T TimerType, HTIMER StartTimerA)
{
    HTIMER Handle;
    EQS_TIMER_E  TimerID;
    HTIMER StartIndex;

    if( TimerType == APPLET_TIMER )
    {
        StartIndex = g_nStartTimerIndex;
    }
    else if( TimerType == CONTROL_TIMER )
    {
        StartIndex = StartTimerA;
    }
    else
        StartIndex = StartTimerA;

    switch( EventID )
    {
    case EQE_TM_TIMER1:
        TimerID = EQS_TIMER_1;
        break;
    case EQE_TM_TIMER2:
        TimerID = EQS_TIMER_2;
        break;
    case EQE_TM_TIMER3:
        TimerID = EQS_TIMER_3;
        break;

    default:
        TimerID = 0;
        break;        
    }
    for( Handle=StartIndex; Handle < g_nEndTimerIndex; Handle++)
    {
        if( g_aLocalTimer[Handle].fOnUse == TRUE )
        {
            if( g_aLocalTimer[Handle].TimerID == TimerID && g_aLocalTimer[Handle].Type == TimerType )
            {
                g_aLocalTimer[Handle].qwStartTime += TIMER_TIME_PERIOD;
                if( g_aLocalTimer[Handle].qwStartTime >= g_aLocalTimer[Handle].dwPeriod )
                {              
                    if( g_aLocalTimer[Handle].fRepeat == TRUE )
                    {
                        g_aLocalTimer[Handle].qwStartTime = 0;
                    }
                    else
                    {
                        sStopTimer( Handle );
                    }
                    return Handle;
                }
            }
        }
    }                       
    return INVALID_TIMER_HANDLE;           
}
*/

LOCAL HTIMER EQS_API sDeserveTimer(HTIMER hStartTimer)
{
    HTIMER Handle;
    QWORD  qwCurrentSystemTime;
    QWORD  qwElapsedTime;
       
    for( Handle= hStartTimer; Handle < g_nEndTimerIndex; Handle++)
    {
        if( g_aLocalTimer[Handle].fOnUse == TRUE )
        {            
            g_pfnGetSystemTime(qwCurrentSystemTime);
            QW_SUB(qwElapsedTime, qwCurrentSystemTime, g_aLocalTimer[Handle].qwStartTime);
            if( QW_LO(qwElapsedTime) >= g_aLocalTimer[Handle].dwPeriod )
            {              
                if( g_aLocalTimer[Handle].fRepeat == TRUE )
                {
//redstar 2001.1.2 수정필요(sleep 모드로 인하여 타이머가 계속 느져질경우 연달아 무지 느려질수 있다.
                    QW_SET( g_aLocalTimer[Handle].qwStartTime, \
                            QW_HI(qwCurrentSystemTime), \
                            QW_LO(qwCurrentSystemTime) );
                }
                else
                {
                    sStopTimer( Handle );					
                }
                return Handle;
            }            
        }
    }                       
    return INVALID_TIMER_HANDLE;           
}


HTIMER EQS_API EQS_GetFirstDeserveTimer(void)
{
    return sDeserveTimer(g_nStartTimerIndex);
}

HTIMER EQS_API EQS_GetNextDeserveTimer(HTIMER hPreviousTimer)
{
    return sDeserveTimer(hPreviousTimer+1);
}


EQS_RESULT EQS_API EQS_InitTimers(void)
{
    HTIMER Handle;

    for( Handle=0; Handle<=MAX_LOCAL_TIMER; Handle++ )
    {
        g_aLocalTimer[Handle].fOnUse = FALSE;
    }
    for( Handle=0; Handle<=MAX_GLOBAL_TIMER; Handle++)
    {
        g_aGlobalTimer[Handle].fOnUse = FALSE;
    }
    for( Handle = 0; Handle < EQS_TIMER_END; Handle++)
        g_anTimerCount[Handle] = 0;

    g_nStartTimerIndex = 0;
    g_nEndTimerIndex   = 1;

    g_nGlobalIndex = 0;

    return RESULT_OK;
}


EQS_RESULT EQS_API EQS_PurgeAllTimers(void)
{
	g_pfnSetTimer(EQS_TIMER_1, 0, FALSE);
    g_pfnSetTimer(EQS_TIMER_2, 0, FALSE);
    g_pfnSetTimer(EQS_TIMER_3, 0, FALSE);

    EQS_InitTimers();

	return RESULT_OK;
}

HTIMER EQS_API EQS_InitAppletTimer(void)
{
    g_nStartTimerIndex = g_nEndTimerIndex;

    return g_nStartTimerIndex;
}

EQS_RESULT EQS_API EQS_FreeAppletTimer(void)
{
    HTIMER Handle;

    for( Handle = g_nStartTimerIndex; Handle< g_nEndTimerIndex; Handle++)
    {
        if( g_aLocalTimer[Handle].fOnUse == TRUE )
        {
            sStopTimer( Handle );
        }
    }    
    g_nEndTimerIndex = g_nStartTimerIndex;
    g_nStartTimerIndex = g_nEndTimerIndex;

    return RESULT_OK;

}

EQS_RESULT EQS_API EQS_PopAppletTimer(void)
{    
    g_nEndTimerIndex   = g_nStartTimerIndex;
    g_nStartTimerIndex = EQS_GetAppletTimerInfo();
    return RESULT_OK;
}


HTIMER EQS_API EQS_StartTimer(EQS_TIMER_E TimerID, DWORD dwPeriod, BOOL fRepeat)
{
    HTIMER TimerHandle;

    TimerHandle = sStartTimer( INVALID_CONTROL_HANDLE , APPLET_TIMER, TimerID, dwPeriod, fRepeat );
	return TimerHandle;
}

LOCAL EQS_RESULT EQS_API sStopTimer(HTIMER hTimer)
{
    EQS_TIMER_E TimerID;

    if( hTimer < g_nStartTimerIndex || hTimer >= g_nEndTimerIndex )
        return RESULT_ERROR;

	if( g_aLocalTimer[hTimer].fOnUse == FALSE )
		return RESULT_ERROR;
	
    if( sFreeTimer( hTimer, LOCAL_TIMER ) == RESULT_OK )
    {
        TimerID = g_aLocalTimer[hTimer].TimerID;

		g_anTimerCount[TimerID]--;
#if 0
		if( g_anTimerCount[TimerID] == 0 )
		{
			g_pfnSetTimer( TimerID, 0, FALSE);
		}
#endif
		if( s_TimerCount() == 0 )
			g_pfnSetTimer( EQS_TIMER_1, 0, FALSE );

		return RESULT_OK;	
    }    
    return RESULT_ERROR;
}


EQS_RESULT EQS_API EQS_StopTimer(EQS_TIMER_E TimerID)
{
    HTIMER hTimer;

	for( hTimer = g_nStartTimerIndex; hTimer <= g_nEndTimerIndex; hTimer++)
	{
		EQS_ASSERT( hTimer >= 0 && hTimer <= MAX_LOCAL_TIMER );
		if( g_aLocalTimer[hTimer].fOnUse == TRUE )
		{
			if( g_aLocalTimer[hTimer].hControl == INVALID_CONTROL_HANDLE )
			{
				if( g_aLocalTimer[hTimer].TimerID == TimerID )
					break;
			}
		}
	}
	if( hTimer > g_nEndTimerIndex )
		return RESULT_ERROR;
	
    if( sFreeTimer( hTimer, LOCAL_TIMER ) == RESULT_OK )
    {
		g_anTimerCount[TimerID]--;
#if 0
		if( g_anTimerCount[TimerID] == 0 )
		{
			g_pfnSetTimer( TimerID, 0, FALSE);
		}     
#endif
		if( s_TimerCount() == 0 )
			g_pfnSetTimer( EQS_TIMER_1, 0, FALSE );

		return RESULT_OK;
    }    
    return RESULT_ERROR;
}

HTIMER EQS_API EQS_StartControlTimer(HCONTROL hControl, EQS_TIMER_E TimerID, 
                                     DWORD dwPeriod, BOOL fRepeat)
{
    HTIMER TimerHandle;

    if( hControl == INVALID_CONTROL_HANDLE )
        return INVALID_TIMER_HANDLE;

    TimerHandle = sStartTimer( hControl, CONTROL_TIMER, TimerID, dwPeriod, fRepeat );
	return TimerHandle;
}

EQS_RESULT EQS_API EQS_StopControlTimer(HCONTROL hControl, HTIMER hTimerHandle)
{
   if( g_aLocalTimer[hTimerHandle].hControl == hControl )
   {   
	   //if( g_aLocalTimer[hTimerHandle].fRepeat == FALSE )
		 //  return RESULT_INVALID_ARG;
		//if( g_aLocalTimer[hTimer].fOnUse == FALSE )
		//	return RESULT_ERROR;

       return sStopTimer(hTimerHandle);
   }
   else
       return RESULT_INVALID_ARG;
}


HCONTROL EQS_API GetTimerControlHandle( HTIMER Handle )
{
    return g_aLocalTimer[Handle].hControl;
}

EQS_TIMER_E  EQS_API GetTimerID(HTIMER Handle)
{
    return g_aLocalTimer[Handle].TimerID;
}


BOOL EQS_API IsEQSTimerEvent(EQS_EVENT_ID EventID)
{
    BOOL fResult;
    if( EventID == EQE_TM_TIMER1 )
    {
        fResult = TRUE;
    }
    else if( EventID == EQE_TM_TIMER2 )
    {
        fResult = TRUE;
    }
    else if( EventID == EQE_TM_TIMER3 )
    {
        fResult = TRUE;
    }
    else
    {
        fResult = FALSE;
    }
    return fResult;
}
