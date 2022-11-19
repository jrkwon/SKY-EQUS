
/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_hkey.h
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
** 2000-11-20 redstar     Created.
**
** ============================================================================
*/

#ifndef _SKY_HKEY_H_
#define _SKY_HKEY_H_

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
#define GET_KEY_DOWN_KEY()           g_KeyDownKey
#define SET_KEY_DOWN_KEY(Key)        g_KeyDownKey = Key

#define GET_POWER_OFF_COUNT()        g_nPowerOffCount
#define SET_POWER_OFF_COUNT(Count)   g_nPowerOffCount = Count

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
/* do not use these variables directly, use MACRO */
extern SKY_EVENT_E g_KeyDownKey;
extern INT8        g_nPowerOffCount;
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API SKY_KeyUp(void);
void SKY_API SKY_HandleKeys(void);



#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_HKEY_H_ */



