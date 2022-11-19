/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     msm4win32.cpp
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
** 2000-08-19 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "stdafx.h"
#include <time.h>
#include "SkySimul.h"
#include "SkySimulDlg.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "msm4win32.h"
#include "rex.h"
#include "clk.h"
//#include "hsig.h"
#include "sky_rex.h"
#include "common.h"
//#include "uistate.h"

/* hsig.h */
#define HS_GPIO_TIMER_SIG       0x0080  /* GPIO 'key' poll timer signal   */

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SKY_KEY_TIMER_SIG     0x0001 /* 0000 0000 0000 0001 - key down timer  */
#define SKY_GEN_TIMER_SIG     0x0010 /* 0000 0000 0001 0000 - genernal timer  */
#define SKY_EQS_TIMER1_SIG    0x0020 /* 0000 0000 0010 0000 - equs timer 1    */
#define SKY_EQS_TIMER2_SIG    0x0040 /* 0000 0000 0100 0000 - equs timer 2    */
#define SKY_EQS_TIMER3_SIG    0x0080 /* 0000 0000 1000 0000 - equs timer 3    */
#define SKY_BKLT_TIMER_SIG    0x0100 /* 0000 0001 0000 0000 - backlight timer */
#define SKY_PACEMAKER_TIMER_SIG 0x0200
#define SKY_BELL_TIMER_SIG      0x0400
#define SKY_MOTER_TIMER_SIG     0x0800
#define SKY_MOTER_OFF_TIMER_SIG  0x2000 





/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

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
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

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


rex_tcb_type    hs_tcb;      
BOOL g_fRepeatTimer[11] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,FALSE,FALSE };

long WinKey;

PCALLBACK_TYPE  pfnCallBack[11];



void SKY_StopForwardBurstDTMF(void)
{

}


void SendKeyInformation(long EventID)
{
    WinKey = EventID;
    SKY_Set_Signal(&hs_tcb,HS_GPIO_TIMER_SIG );
}


void Start_UI_Task( void *ch )
{
    ui_task(100);
}

void Start_HS_Task( void *ch )
{
    hs_task(100);
}


void EQS_RefreshDisplayDevice()
{
    CWnd* pMainWnd = AfxGetMainWnd();
    ((CSkySimulDlg*)pMainWnd)->DrawEQUSBuffer();
}

void EQS_RefreshDisplayDeviceRect(BYTE* pBuffer, BYTE xleft, BYTE ytop, BYTE xright, BYTE ybottom)
{
    CWnd* pMainWnd = AfxGetMainWnd();
    ((CSkySimulDlg*)pMainWnd)->DrawEQUSBufferRect(xleft, ytop, xright, ybottom);
}


BOOL SetWinTimer( PCALLBACK_TYPE pfn, WORD uTimerSignal,  INT  interval,    BOOL repeat )
{

    BOOL fResult = TRUE;
    CWnd* pMainWnd = AfxGetMainWnd();
    UINT  check;

    switch( uTimerSignal )
    {
    case SKY_KEY_TIMER_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_KEY_TIMER);
         else
             check= pMainWnd->SetTimer(WIN_KEY_TIMER, interval, NULL);
         g_fRepeatTimer[0] = repeat;
         pfnCallBack[0] = pfn;
         break;

    case SKY_GEN_TIMER_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_GENERAL_TIMER_0);
         else
             check= pMainWnd->SetTimer(WIN_GENERAL_TIMER_0, interval, NULL);
         g_fRepeatTimer[1] = repeat;
         pfnCallBack[1] = pfn;
         break;
    case SKY_EQS_TIMER1_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_GENERAL_TIMER_1);
         else
             check= pMainWnd->SetTimer(WIN_GENERAL_TIMER_1, interval, NULL);
         g_fRepeatTimer[2] = repeat;
         pfnCallBack[2] = pfn;
         break;
    case SKY_EQS_TIMER2_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_GENERAL_TIMER_2);
         else
             check= pMainWnd->SetTimer(WIN_GENERAL_TIMER_2, interval, NULL);
         g_fRepeatTimer[3] = repeat;
         pfnCallBack[3] = pfn;
         break;

    case SKY_EQS_TIMER3_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_GENERAL_TIMER_3);
         else
             check= pMainWnd->SetTimer(WIN_GENERAL_TIMER_3, interval, NULL);
         g_fRepeatTimer[4] = repeat;
         pfnCallBack[4] = pfn;
         break;

    case SKY_BKLT_TIMER_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_BACKLIGHT_TIMER);
         else
             check= pMainWnd->SetTimer(WIN_BACKLIGHT_TIMER, interval, NULL);
         g_fRepeatTimer[5] = repeat;
         pfnCallBack[5] = pfn;
         break;

    case SKY_PACEMAKER_TIMER_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_PACEMAKER_TIMER);
         else
             check= pMainWnd->SetTimer(WIN_PACEMAKER_TIMER, interval, NULL);
         g_fRepeatTimer[6] = repeat;
         pfnCallBack[6] = pfn;
         break;

    case SKY_BELL_TIMER_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_BELL_TIMER);
         else
             check= pMainWnd->SetTimer(WIN_BELL_TIMER, interval, NULL);
         g_fRepeatTimer[7] = repeat;
         pfnCallBack[7] = pfn;
         break;

    case SKY_MOTER_TIMER_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_MOTER_TIMER);
         else
             check= pMainWnd->SetTimer(WIN_MOTER_TIMER, interval, NULL);
         g_fRepeatTimer[8] = repeat;
         pfnCallBack[8] = pfn;
         break;

    case SKY_MOTER_OFF_TIMER_SIG:
         if(interval == 0)
             pMainWnd->KillTimer(WIN_MOTER_OFF_TIMER);
         else
             check= pMainWnd->SetTimer(WIN_MOTER_OFF_TIMER, interval, NULL);
         g_fRepeatTimer[9] = repeat;
         pfnCallBack[9] = pfn;
         break;

    default:
        fResult = FALSE;
        break;

    }

    return fResult;

}

// qualquam function

void ui_initialize_imsi_components ( BYTE nam )
{
}

void ui_initialize_imsi_t_components ( BYTE nam )
{
}

BOOL hw_force_offline( void )
{
    return TRUE;
}

dword ui_get_local_time ( void ) 
{
    CTime theTime;

    theTime = CTime::GetCurrentTime();

    return static_cast<unsigned long>(theTime.GetTime());
}


unsigned char ibatt_ref[4];
unsigned char tbatt_ref[4];

int vbatt_read(void)
{
    return 10; // dummy
}


BOOL read_superblock(void) 
{
    return TRUE;
}

SKA_Put256Bitmap() {}

#ifdef __cplusplus
}
#endif
