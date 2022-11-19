/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     msm4win32.h
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
** 2000-08-21 jrkwon     Created.
**
** ============================================================================
*/



#ifndef _MSM4WIN32_H_
#define _MSM4WIN32_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
//#include "common.h"

//#include "uistate.h"

typedef unsigned long qword[ 2 ];

#define WIN_KEY_TIMER        1
#define WIN_GENERAL_TIMER_0  2
#define WIN_GENERAL_TIMER_1  3
#define WIN_GENERAL_TIMER_2  4
#define WIN_GENERAL_TIMER_3  5
#define WIN_BACKLIGHT_TIMER  6
#define WIN_PACEMAKER_TIMER  7
#define WIN_BELL_TIMER       8
#define WIN_MOTER_TIMER      9
#define WIN_MOTER_OFF_TIMER  10


typedef void (*PCALLBACK_TYPE)(DWORD ms_interval );

extern  BOOL g_fRepeatTimer[11];
extern  PCALLBACK_TYPE   pfnCallBack[11];
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/


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
**  Variable Declarations
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/


extern void ui_task(DWORD dummy);
extern void hs_task(DWORD dummy);

extern void Start_UI_Task( void *ch );

extern void Start_HS_Task( void *ch );
extern void EQS_RefreshDisplayDevice();
extern void EQS_RefreshDisplayDeviceRect(BYTE* , BYTE xleft, BYTE ytop, BYTE xright, BYTE ybottom );


extern void SendKeyInformation(long EventID);
extern void  WIN_ProcessKey(long KeyCode);
// qualquam function


BOOL hw_force_offline( void );
DWORD rex_set_cpsr(void) ;

void ui_initialize_imsi_components ( BYTE nam );
void ui_initialize_imsi_t_components ( BYTE nam );
void SKY_GetSystemTime(qword uptime_ms);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _MSM4WIN32_H_ */
