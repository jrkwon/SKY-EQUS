/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_dscf.h
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
** 2000-12-27 htg     Created.
**
** ============================================================================
*/

#ifndef _SKA_DSCF_H_
#define _SKA_DSCF_H_

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
#define VOICE_INCOMMING 0
#define MODEM_INCOMMING 1
#define FAX_INCOMMING   2

#define INTERNET_MAX_SPEED      0
#define INTERNET_NORMAL_SPEED   1

#define PC_SPEED_19200      0
#define PC_SPEED_115200     1
#define PC_SPEED_230400    2
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
extern void SKY_API SKA_SetVoiceMode(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_SetUmSpeed(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_SetRmSpeed(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_SetRmSpeed(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_SetDmSpeed(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_SetDmPort(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_MainDataServiceSet(SKY_EVENT_T *pEvent);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_DSCF_H_ */



