/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_time.h
**
** Description:
**      Day & Time Input Control
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-12-21   redstar       Created.
**
** ===========================================================================
*/

#ifndef _EQC_TIME_H_
#define _EQC_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* CONTROL_TYPE -- bit flag */
#define CS_TIME_NOBORDER     0x0001            /* 0000 0000 0000 0001 */
#define CS_TIME_BORDER       0x0002            /* 0000 0000 0000 0010 */

/* default control style */
#define CS_TIME_DEFAULT    EQS_DEFAULT_STYLE


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagTIME_CONTROL_E
{
    YEAR_TIME_CONTROL = 0,   //yyyy
    MON_TIME_CONTROL,        //mm
    WEEK_TIME_CONTROL,       //w
    DAY_TIME_CONTROL,        //dd
    TIME_TIME_CONTROL,       //hh:mm(AMorPM)
	TIME_TO_TIME_CONTROL,    //hh:mmAM/PM ~ hh:mmAM/PM
	TIME_MINUTE_CONTROL,     //(오전/오후)hh시mm분 
	MONTH_DAY_CONTROL,       //mm월dd일          
    INFORM_TIME_CONTROL,      //yyyy.mm.dd(양/음)
    CAL_TIME_CONTROL,         //yyyy년mm월dd일
    YEAR_MONTH_CONTROL,       //yyyy년mm월
    ALL_TIME_CONTROL,        //yyyy.mm.dd.mm:ss(AMorPM)
    ENTIRE_TIME_CONTROL     //yyyy.mm.dd.mm:ss(AMorPM)요일
};
typedef enum tagTIME_CONTROL_E  TIME_CONTROL_E;

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
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

HCONTROL EQS_API EQC_Time(COORD x, COORD y,  DWORD nInitTime, TIME_CONTROL_E WhichTime);

void EQS_API EQC_SetTime(HCONTROL hControl, WORD wYear,   \
                                            UINT8 uMonth, \
                                            UINT8 uWeek,  \
                                            UINT8 uDay,   \
                                            UINT8 uHour,  \
                                            UINT8 uMinute);

void EQS_API EQC_SetTimeYear(HCONTROL hControl, WORD wYear );
void EQS_API EQC_SetTimeMonth(HCONTROL hControl, UINT8 uMonth);
void EQS_API EQC_SetTimeWeek(HCONTROL hControl, UINT8 uWeek);
void EQS_API EQC_SetTimeDay(HCONTROL hControl, UINT8 uDay);
void EQS_API EQC_SetTimeHour(HCONTROL hControl, UINT8 uHour);
void EQS_API EQC_SetTimeMinute(HCONTROL hControl, UINT8 uMinute);
void EQS_API EQC_SetTimePM(HCONTROL hControl);
void EQS_API EQC_SetTimeAM(HCONTROL hControl);
void EQS_API EQC_SetTimeYearLimit(HCONTROL hControl, WORD wMinYear, WORD wMaxYear);
void EQS_API EQC_SetTimeLunar(HCONTROL hControl, BOOL fLunar);


void EQS_API EQC_GetTime(HCONTROL hControl, WORD*  pwYear,   \
                                            UINT8* puMonth,  \
                                            UINT8* puWeek,   \
                                            UINT8* puDay,    \
                                            UINT8* puHour,   \
                                            UINT8* puMinute);
WORD  EQS_API EQC_GetTimeYear(HCONTROL hControl);
UINT8 EQS_API EQC_GetTimeMonth(HCONTROL hControl);
UINT8 EQS_API EQC_GetTimeWeek(HCONTROL hControl);
UINT8 EQS_API EQC_GetTimeDay(HCONTROL hControl);
UINT8 EQS_API EQC_GetTimeHour(HCONTROL hControl);
UINT8 EQS_API EQC_GetTimeMinute(HCONTROL hControl);
BOOL  EQS_API EQC_GetTimeLunar(HCONTROL   hControl);

BOOL  EQS_API EQC_GetTimeEditMode(HCONTROL hControl);
void  EQS_API EQC_SetTimeEditMode(HCONTROL hControl, BOOL fEdit);

BOOL  EQS_API EQC_GetFirstTime(HCONTROL hControl, UINT8* nHour, UINT8* nMinute);
BOOL  EQS_API EQC_GetSecondTime(HCONTROL hControl, UINT8* nHour, UINT8* nMinute);

BOOL  EQS_API EQC_SetFirstTime(HCONTROL hControl, UINT8  nHour, UINT8  nMinute);
BOOL  EQS_API EQC_SetSecondTime(HCONTROL hControl, UINT8  nHour, UINT8  nMinute);

BOOL  EQS_API EQC_IsTimePM(HCONTROL hControl);
BOOL  EQS_API EQC_IsTime2PM(HCONTROL hControl);
void  EQS_API EQC_SetTime2PM(HCONTROL hControl);
void  EQS_API EQC_SetTime2AM(HCONTROL hControl);

void  EQS_API EQC_SetTimeLimit(HCONTROL hControl, BOOL fActive);
BOOL  EQS_API EQC_GetTimeLimit(HCONTROL hControl);

void  EQS_API EQC_SetTimeUpper(HCONTROL hControl, WORD wYear, UINT8 nMonth, UINT8 nDay, UINT8 nHour, UINT8 nMinute);
void  EQS_API EQC_SetTimeLower(HCONTROL hControl, WORD wYear, UINT8 nMonth, UINT8 nDay, UINT8 nHour, UINT8 nMinute);



#ifdef __cplusplus
}
#endif

#endif // _EQC_STXT_H_ 
/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/




