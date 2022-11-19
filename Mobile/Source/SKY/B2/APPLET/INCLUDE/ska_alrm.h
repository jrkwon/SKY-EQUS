/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_alrm.h
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
** ============================================================================
*/

#ifndef _SKA_ALRM_H_
#define _SKA_ALRM_H_

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
//#define MAX_ALARM_TITLE				20


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagALARM_DISP_TYPE_E {
    ADT_NONE,
    ADT_ANNIV,
    ADT_EVENT,
    ADT_ALARM,
    ADT_MAX
};
typedef enum tagALARM_DISP_TYPE_E    ALARM_DISP_TYPE_E;

struct tagALARM_DISP_TYPE_T {
    ALARM_DISP_TYPE_E   Type;
    INT8                nAddr;
};
typedef struct  tagALARM_DISP_TYPE_T  ALARM_DISP_TYPE_T;
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
extern VOID SKY_API SKA_MainAlarm(SKY_EVENT_T *pEvent);
extern VOID SKY_API sSKA_PlayAlarm(SKY_EVENT_T* pEvent);

extern VOID sFindAlarmOn( clk_julian_type *cjCurrentTime, ALARM_DISP_TYPE_T* AlarmType );

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_ALRM_H_ */
