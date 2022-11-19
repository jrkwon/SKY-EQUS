/*
** ============================================================================
**
** Project: 
**     Athens
**
** File:
**     eqa_strt.h
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
** 2000-07-20 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKA_STRT_H_
#define _SKA_STRT_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "comdef.h" /* boolean */

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
#ifdef FEATURE_OTASP
/* below flags are needed, for the case when MC decides the phone should */
/* power down, because of wrong lock code being sent over the air.       */
// jrkwon 2001-02-14 moved to the signal handler (sky_hsig.c)
//extern boolean ui_pwrdown_complete ;
//extern boolean ui_mc_initiated_pwrdown ;
#endif

/* use these flags to treat all IMSI components as one */
extern boolean initialize_imsi[] ;
extern boolean imsi_all_zeros[] ;

#ifdef FEATURE_IS95B_TRUE_IMSI
extern boolean initialize_imsi_t[];
extern boolean imsi_t_all_zeros[];
#endif


/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API SKY_InitSystemFileAndDataDir(void);
extern void SKY_API SKY_InitApplicationFiles(void);
extern VOID SKY_API SKA_MainStartUp(SKY_EVENT_T* pEvent);
extern void SKY_API SKY_PowerOff(POWER_OFF_MODE PowerOffMode);
extern VOID SKY_API SKA_ShutDown(SKY_EVENT_T* pEvent);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_STRT_H_ */
