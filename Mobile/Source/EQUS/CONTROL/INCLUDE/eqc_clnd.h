/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_clnd.h
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
** 2000-12-22 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQC_CLND_H_
#define _EQC_CLND_H_

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
/* CONTROL_STYLE : bit flag */
#define CS_CALENDAR_BORDER           0x0001   /* 0000 0000 0000 0001 */
#define CS_CALENDAR_SOLAR            0x0002   /* 0000 0000 0000 0010 */
#define CS_CALENDAR_LUNAR            0x0004   /* 0000 0000 0000 0100 */
#define CS_CALANDAR_DISPLAY_YEAR     0x0008   /* 0000 0000 0000 1000 */

#define CS_CALENDAR_DEFAULT         CS_CALENDAR_BORDER | CS_CALENDAR_SOLAR | CS_CALANDAR_DISPLAY_YEAR

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
HCONTROL    EQS_API EQC_Calendar(COORD x, COORD y/* <-- Add here --> */);
//void        EQS_API EQC_GoToday(void);
/* <-- Add here --> get/set functions */
//void        EQS_API EQC_SetXxxYyyZzz(HCONTROL hControl/* <-- Add here --> */);
///* <-- Add here --> */ EQS_API EQC_GetXxxYyyZzz(HCONTROL hControl);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_CLND_H_ */
