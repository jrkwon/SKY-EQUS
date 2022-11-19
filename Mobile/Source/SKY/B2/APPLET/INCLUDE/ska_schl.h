/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_schl.h
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
** 00-12-07   rosa     Created.
**
** ============================================================================
*/

#ifndef _SKA_SCHL_H_
#define _SKA_SCHL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include    "ska_sfil.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SCHL_MAX_DAYS_AMONTH    31
#define SCHL_MAX_EVENTS_ADAY    15     //total view event
#define SCHL_MAX_MYEVNTS_ADAY   10     //schedule event
#define SCHL_MAX_ANNIV_ADAY      5     //maxd anniv event 

#define SCHL_INIT_YEAR			1980
#define SCHL_START_YEAR         1996
#define SCHL_END_YEAR           2015

#define SCHL_HAS_NO_EVENT       1
#define SCHL_ERROR              -1

#define SCHL_DIFF_LUNAL_CAL     3
#define SCHL_MAX_SIZE_DAY_TITLE 19//18//17

#define MAX_NUM_ALARM_TYPE		6
#define TIME_SECS_10_MIN		10*60
#define TIME_SECS_20_MIN        20*60
#define TIME_SECS_30_MIN        30*60
#define TIME_SECS_1_MIN         1*60
#define TIME_SECS_1_HOUR        1*60*60
#define TIME_SECS_1_DAY         24*60*60

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagSCHL_BLOCK_RECORD_TYPE_E {
    SBRT_NONE,
    SBRT_EVENT
};
typedef enum tagSCHL_BLOCK_RECORD_TYPE_E    SCHL_BLOCK_RECORD_TYPE_E;

enum tagSCHL_MAKE_EVENT_TYPE_E {
    SMET_NONE,
    SMET_VIEW_LIST
};
typedef enum tagSCHL_MAKE_EVENT_TYPE_E    SCHL_MAKE_EVENT_TYPE_E;

/* ------------------------------------------------------------------------- */

/* DayEvent Structure */
struct tagSCHL_EVENT_RECORD_T {
    SCHL_RECORD_TYPE_E  Type;
    INT16               nAddr; // It's the address of Event
};
typedef struct  tagSCHL_EVENT_RECORD_T  SCHL_EVENT_RECORD_T;


struct tagSCHL_BLOCK_RECORD_T {
    SCHL_BLOCK_RECORD_TYPE_E    Type;
    UINT8                       unNumOfEvents;   // it's the number of the all events of the day.
	UINT8                       unNumOfAnniv;
    SCHL_EVENT_RECORD_T         *pEvents;    
};
typedef struct  tagSCHL_BLOCK_RECORD_T    SCHL_BLOCK_RECORD_T;


struct tagSCHL_TIME_SET_T{
    WORD    wYear;
    INT8    nMonth;
    INT     nDay;
    INT8    nHour;
    INT8    nMinute;
    INT8    nSecond;
    INT8    nDayOfWeek;
};
typedef struct  tagSCHL_TIME_SET_T SCHL_TIME_SET_T;

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
extern  VOID    SKY_API Init_Schedule_Block(void);
extern  VOID    SKY_API FreeScheduleBlock( VOID );
extern  VOID    SKY_API SKA_MainSchedule(SKY_EVENT_T *pEvent);
extern  VOID    SKY_API SKA_ViewADay( SKY_EVENT_T* pEvent );
extern  VOID    SKY_API SKA_DeleteSomeEvent(SKY_EVENT_T *pEvent);
extern  VOID    SKY_API SKA_MakeAEvent( SKY_EVENT_T* pEvent );
extern  VOID    SKY_API SKA_SetAnnivAlarm(SKY_EVENT_T* pEvent);

extern  DWORD   GetScheduleTime( VOID );
extern  VOID    SetScheduleTime( DWORD  dwOneSetTime );
extern  DWORD   GetSecsTime( SCHL_TIME_SET_T* pSearchDay );
extern  VOID    MakeSchedule( DWORD dwOneSetTime );
extern  VOID    GetANewEvent( UINT8   unIdxOfBlockT, INT8 nIdxOfEventT );
extern  UINT8   GetDaysOfMonth( WORD wOneYear, INT8 nOneMonth );
extern  BOOL    HasAEvent( UINT8 unOneIdxOfBlockT );
extern  VOID    CopyRecord( SCHL_RECORD_T* pSrcRecord, SCHL_RECORD_T*  pDestRecord );
extern  INT8    GetNewIdxOfEventT( UINT8 unIdxOfBlockT );
extern  UINT8	GetSchedule( DWORD dwToday, SCHL_RECORD_T* pEventRecordT, UINT8 unIdx ); /* use in ska_idle.c */

extern  INT8    GetNewIdxOfSchlT( VOID );


extern  SCHL_BLOCK_RECORD_TYPE_E    GetTypeOfBlockT( UINT8  unIdxOfDay );
extern  UINT8                       GetEventNumOfBlockT( UINT8  unIdxOfDay );
extern  UINT8                       GetMyEventNumOfBlockT( UINT8  unIdxOfDay );
extern  BOOL                        HaveMyEventOfBlockT( UINT8  unIdxOfDay );
extern  BOOL                        HaveAnnivOfBlockT( UINT8  unIdxOfDay );
extern  SCHL_RECORD_TYPE_E          GetEventTypeOfBlockT( UINT8 unIdxOfDay, UINT8 unIdxOfEvent );
extern  INT16                       GetEventAddrOfBlockT( UINT8 unIdxOfDay, UINT8 unIdxOfEvent );

extern  UINT8   Get12HourTime(UINT8 un24HourTime);
extern  UINT8   Get24HourTime(UINT8 un12HourTime, BOOL f24Hour);
extern  BOOL    IsTimePM(UINT8 un24HourTime);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SCHL_H_ */
