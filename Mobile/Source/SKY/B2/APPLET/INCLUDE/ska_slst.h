/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_slst.h
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
** 01-01-09 rosa     Created.
**
** ============================================================================
*/

#ifndef _SKA_SLST_H_
#define _SKA_SLST_H_

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
extern  VOID SKY_API SKA_MainSchedulerList(SKY_EVENT_T *pEvent);

extern  VOID    SetScheduleListTime( DWORD  dwOneSetTime );
extern  DWORD   GetScheduleListTime( VOID );


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SLST_H_ */
