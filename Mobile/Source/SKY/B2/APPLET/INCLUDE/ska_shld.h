/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_shld.h
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
** 01-01-22   rosa       Created.
**
** ============================================================================
*/

#ifndef _SKA_SHLD_H_
#define _SKA_SHLD_H_

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


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagSCHL_MAKE_TYPE_E {
    SMT_OLD,
    SMT_NEW
};
typedef enum tagSCHL_MAKE_TYPE_E    SCHL_MAKE_TYPE_E;

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
extern  SCHL_HOLIDAY_CAL_T  g_MyHolidayT[];

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern  VOID    SKY_API SKA_MainMyHoliday(SKY_EVENT_T *pEvent);
extern  VOID    BuildMyHoliday(VOID);
extern  UINT8   GetNewIdxOfMyHoliday(VOID);
extern  VOID    SaveMyHoliday(UINT8 unIdxOfMyHolidayT);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SHLD_H_ */
