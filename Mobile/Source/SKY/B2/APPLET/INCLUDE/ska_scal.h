/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_scal.h
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
** 01-01-02 rosa     Created.
**
** ============================================================================
*/

#ifndef _SKA_SCAL_H_
#define _SKA_SCAL_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "ska_sfil.h"
#include "ska_schl.h"


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define CAL_MAX_SIZE_TITLE      10

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/


struct tagSCHL_FIND_SOLAR_DAY_T {
    BOOL    fFind;
    INT16   nIdxOfTable;
    UINT8   unSMonth;
    UINT8   unSDay;
};
typedef struct  tagSCHL_FIND_SOLAR_DAY_T    SCHL_FIND_SOLAR_DAY_T;

struct tagSCHL_FIND_HOLIDAY_T {
    SCHL_HOLIDAY_TYPE_E Type;
    INT8                nIdxOfTable;
    UINT8               unDays;
};
typedef struct  tagSCHL_FIND_HOLIDAY_T    SCHL_FIND_HOLIDAY_T;



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
extern  VOID SKY_API SKA_MainCalendar(SKY_EVENT_T *pEvent);

extern  VOID    SetCalendarTime( DWORD  dwOneSetTime );
extern  DWORD   GetCalendarTime( VOID );
extern  VOID    GetSolarDay( UINT8* punLMonth, UINT8*  punLDay, UINT16*  pnPosSearch, SCHL_FIND_SOLAR_DAY_T* pResult );

extern  VOID    CheckHoliday( UINT8 unDay, SCHL_FIND_HOLIDAY_T* pFindHoliday );


void Set_Schedule_Time(SCHL_TIME_SET_T* CurrentTime);
void Get_Schedule_Time(SCHL_TIME_SET_T * CurrentTime);

extern VOID Calinder_DecreaseADay(VOID);
extern VOID Calinder_IncreaseADay(VOID);
extern VOID Calinder_DecreaseAWeek(VOID);
extern VOID Calinder_IncreaseAWeek(VOID);
extern VOID Calinder_DecreaseAMonth(VOID);
extern VOID Calinder_IncreaseAMonth(VOID);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SCAL_H_ */
