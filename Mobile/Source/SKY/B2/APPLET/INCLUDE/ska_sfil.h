/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_sfil.h
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
** 00-12-19   rosa     Created.
**
** ============================================================================
*/

#ifndef _SKA_SFIL_H_
#define _SKA_SFIL_H_

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
#define SCHL_MAX_RECORD_COUNT       (100+1)
#define SCHL_MAX_EVENT_DATA_LENGTH  (15*4)
#define SCHL_MAX_COUNT_MY_HOLIDAY   50
#define SCHL_IDX_ANNIV_ALARM        (SCHL_MAX_RECORD_COUNT-1)//100
#define MAX_SIZE_TITLE_LINE         20

// file path
#define FILE_NAME_SCHL              SKY_SCHL_DIR""SKY_DIR_CHAR"SCHEDULE.SKY"
#define FILE_NAME_HLDY              SKY_SCHL_DIR""SKY_DIR_CHAR"MYHOLIDAY.SKY"

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/* type */
enum tagSCHL_RECORD_TYPE_E {
    SRT_NONE,
    SRT_EVENT,
    SRT_EVENT_WITH_A,
    SRT_EVENT_WITH_R,
    SRT_EVENT_WITH_R_A,
    SRT_ANNIV,	    
    SRT_USED_EVENT_WITH_A,    
    SRT_USED_EVENT_WITH_R_A,
    SRT_USED_ANNIV,
    SRT_MAX
};
typedef enum tagSCHL_RECORD_TYPE_E SCHL_RECORD_TYPE_E;

/* Recursive Type */
enum tagSCHL_EVENT_RECURSIVE_TYPE_E {
    SERT_NONE,
    SERT_EVERY_DAY,
    SERT_EVERY_WEEK,
    SERT_EVERY_MONTH,
	SERT_EVERY_YEAR,
    SERT_MAX
};
typedef enum tagSCHL_EVENT_RECURSIVE_TYPE_E SCHL_EVENT_RECURSIVE_TYPE_E;

/* Alarm type */
enum tagSCHL_ALARM_TIME_TYPE_E {
    SATT_NONE,
	SATT_BEFORE_10M,
	SATT_BEFORE_20M,
	SATT_BEFORE_30M,
	SATT_BEFORE_1H,
	SATT_BEFORE_1D,
    SATT_SET_ANYTIME,
    SATT_MAX
};
typedef enum tagSCHL_ALARM_TIME_TYPE_E SCHL_ALARM_TIME_TYPE_E;

/* My Holiday Type */
enum tagSCHL_HOLIDAY_TYPE_E {
    SHT_NONE,
    SHT_KOR,
    SHT_MY
};
typedef enum tagSCHL_HOLIDAY_TYPE_E    SCHL_HOLIDAY_TYPE_E;

/****************************************************************************************/

/* Recursive */
struct tagSCHL_EVENT_RECURSIVE_T {
    SCHL_EVENT_RECURSIVE_TYPE_E Type;
    DWORD                       dwEndTime;
};
typedef struct tagSCHL_EVENT_RECURSIVE_T SCHL_EVENT_RECURSIVE_T;

/* Alarm */
struct tagSCHL_ALARM_TIME_T {
    SCHL_ALARM_TIME_TYPE_E  Type;
    DWORD                   dwTime;
    DWORD                   dwEndTime; /* for Recursive Alarm */
};
typedef struct  tagSCHL_ALARM_TIME_T SCHL_ALARM_TIME_T;

/* Event */
struct tagSCHL_RECORD_T {
    SCHL_RECORD_TYPE_E      Type;
    DWORD                   dwStartTime;
    DWORD                   dwEndTime;
    BYTE                    abData[SCHL_MAX_EVENT_DATA_LENGTH+1];
    SCHL_EVENT_RECURSIVE_T  Recursive;
    SCHL_ALARM_TIME_T       Alarm; 
};
typedef struct  tagSCHL_RECORD_T  SCHL_RECORD_T;

/* Pre-Scheduler Structure */
struct tagSCHL_PRE_CONFIG_T
{
    SCHL_RECORD_TYPE_E      Type;
    DWORD                   dwStartTime;
    SCHL_EVENT_RECURSIVE_T  Recursive;
    SCHL_ALARM_TIME_T       Alarm;
    //DWORD                   dwAlarmTime;
    //DWORD                   dwAlarmEndTime;

};
typedef struct tagSCHL_PRE_CONFIG_T     SCHL_PRE_CONFIG_T;

#define sGet_SCHL_PRE_CONFIG_TYPE(s)            (s).Type
#define sGet_SCHL_PRE_CONFIG_START_TIME(s)      (s).dwStartTime
#define sGet_SCHL_PRE_CONFIG_RECURSIVE_TYPE(s)  (s).Recursive.Type
#define sGet_SCHL_PRE_CONFIG_RECURSIVE_TIME(s)  (s).Recursive.dwEndTime
#define sGet_SCHL_PRE_CONFIG_ALARM_TYPE(s)      (s).Alarm.Type
#define sGet_SCHL_PRE_CONFIG_ALARM_TIME(s)      (s).Alarm.dwTime
#define sGet_SCHL_PRE_CONFIG_ALARM_END_TIME(s)  (s).Alarm.dwEndTime
//#define sGet_SCHL_PRE_CONFIG_ADDR(s)            (s).nAddr

/* My Holiday */
struct  tagSCHL_HOLIDAY_CAL_T {
    SCHL_HOLIDAY_TYPE_E Type;    
    BOOL    fLunar;
    WORD    wYear;
    UINT8   unMonth;
    UINT8   unDay;
    BOOL    fConsecutive;
    BYTE    pszName[MAX_SIZE_TITLE_LINE+1];
};
typedef struct  tagSCHL_HOLIDAY_CAL_T   SCHL_HOLIDAY_CAL_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
** ***************************************************************************
**  정보 읽기
** ***************************************************************************
*/
#define SKY_GET_SCHL_PRE_CONFIG_TYPE(i)             sGet_SCHL_PRE_CONFIG_TYPE(g_SchlPreConfig[i])
#define SKY_GET_SCHL_PRE_CONFIG_START_TIME(i)       sGet_SCHL_PRE_CONFIG_START_TIME(g_SchlPreConfig[i])
#define SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(i)   sGet_SCHL_PRE_CONFIG_RECURSIVE_TYPE(g_SchlPreConfig[i])
#define SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i)   sGet_SCHL_PRE_CONFIG_RECURSIVE_TIME(g_SchlPreConfig[i])
#define SKY_GET_SCHL_PRE_CONFIG_ALARM_TYPE(i)       sGet_SCHL_PRE_CONFIG_ALARM_TYPE(g_SchlPreConfig[i])
#define SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(i)       sGet_SCHL_PRE_CONFIG_ALARM_TIME(g_SchlPreConfig[i])
#define SKY_GET_SCHL_PRE_CONFIG_ALARM_END_TIME(i)   sGet_SCHL_PRE_CONFIG_ALARM_END_TIME(g_SchlPreConfig[i])
//이거 나중에 지워야 해.
#define SKY_GET_SCHL_PRE_CONFIG_ADDR(i)             sGet_SCHL_PRE_CONFIG_ADDR(g_SchlPreConfig[i])

/*
** ***************************************************************************
**  정보 기록
** ***************************************************************************
*/
#define SKY_SET_SCHL_PRE_CONFIG_TYPE(i, t)              SKY_GET_SCHL_PRE_CONFIG_TYPE(i) = t;
#define SKY_SET_SCHL_PRE_CONFIG_START_TIME(i, t)        SKY_GET_SCHL_PRE_CONFIG_START_TIME(i) = t;
#define SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(i, t)    SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(i) = t;
#define SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i, t)    SKY_GET_SCHL_PRE_CONFIG_RECURSIVE_TIME(i) = t;
#define SKY_SET_SCHL_PRE_CONFIG_ALARM_TYPE(i, t)        SKY_GET_SCHL_PRE_CONFIG_ALARM_TYPE(i) = t;
#define SKY_SET_SCHL_PRE_CONFIG_ALARM_TIME(i, t)        SKY_GET_SCHL_PRE_CONFIG_ALARM_TIME(i) = t;
#define SKY_SET_SCHL_PRE_CONFIG_ALARM_END_TIME(i, t)    SKY_GET_SCHL_PRE_CONFIG_ALARM_END_TIME(i) = t;
//이거 나중에 지워야 해.
#define SKY_SET_SCHL_PRE_CONFIG_ADDR(i, t)              SKY_GET_SCHL_PRE_CONFIG_TYPE(i) = t;

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern  SCHL_PRE_CONFIG_T       g_SchlPreConfig[];
extern  SCHL_ALARM_TIME_TYPE_E  g_AlarmOfAnniv;
extern  SCHL_RECORD_T           g_InitRecord;
extern  SCHL_HOLIDAY_CAL_T      g_InitHoliday;

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern VOID SKY_API SKY_InitScheduler(VOID);
extern VOID SKY_SeekReadSchlT( UINT8 unIdxOfSchl, SCHL_RECORD_T* pReadRecord );
extern VOID SKY_SeekWriteSchlT( UINT8 unIdxOfSchl, SCHL_RECORD_T* pWriteRecord );

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SFIL_H_ */
