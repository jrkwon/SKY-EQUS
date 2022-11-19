/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     EQS__timr.h
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
** 00-03-23    redstar       Created.
**
** ============================================================================
*/

#ifndef _EQS_TIMR_H_
#define _EQS_TIMR_H_

#include "eqs_evnt.h"

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#ifdef __cplusplus
extern "C" {
#endif
    
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

    
/*
#define EQS_TIMER_1 1
#define EQS_TIMER_2 2
#define EQS_TIMER_3 3
#define EQS_TIMER_4 4
#define EQS_TIMER_5 5
*/
#define INVALID_TIMER_HANDLE        0

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagEQS_TIMER_E
{
    EQS_GENERAL_TIMER = 0,
    EQS_TIMER_1,
    EQS_TIMER_2,
    EQS_TIMER_3,
	EQS_TIMER_4,
	EQS_TIMER_5,
    EQS_TIMER_END
};
typedef enum tagEQS_TIMER_E     EQS_TIMER_E;

/*
**---------------------------------------------------------------------------
**  General Declarations
**---------------------------------------------------------------------------
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
extern EQS_RESULT   EQS_API EQS_InitTimers(void);
extern EQS_RESULT   EQS_API EQS_PurgeAllTimers(void);


extern HTIMER       EQS_API EQS_InitAppletTimer(void);
extern EQS_RESULT   EQS_API EQS_FreeAppletTimer(void);
extern EQS_RESULT   EQS_API EQS_PopAppletTimer(void);

extern HCONTROL     EQS_API GetTimerControlHandle(HTIMER hTimer);
extern EQS_TIMER_E  EQS_API GetTimerID(HTIMER Handle);
extern HTIMER EQS_API EQS_GetFirstDeserveTimer(void);
extern HTIMER EQS_API EQS_GetNextDeserveTimer(HTIMER hPreviousTimer);


extern BOOL         EQS_API IsEQSTimerEvent(EQS_EVENT_ID EventID);

extern HTIMER       EQS_API EQS_StartTimer(EQS_TIMER_E TimerID, DWORD dwPeriod, 
                                         BOOL bRepeat);
extern EQS_RESULT   EQS_API EQS_StopTimer(EQS_TIMER_E TimerID);
//extern HTIMER NIOC_StartTimer(HCONTROL CHandle, UINT8 uTimerID, DWORD dwPeriod, BOOL bRepeat);
//extern EQS_RESULT NIOC_StopTimer(HTIMER uTimerHandle);
extern HTIMER       EQS_API EQS_StartControlTimer(HCONTROL hControl, 
                                                EQS_TIMER_E TimerID, DWORD dwPeriod, 
                                                BOOL fRepeat);

extern EQS_RESULT   EQS_API EQS_StopControlTimer(HCONTROL hControl, HTIMER hTimerHandle);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_TIMER_H_ */
