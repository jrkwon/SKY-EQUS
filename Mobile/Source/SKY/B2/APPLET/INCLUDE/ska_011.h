/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_011.h
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
** 4333-12-15 white     Created.
**
** ============================================================================
*/

#ifndef _SKA_011_H_
#define _SKA_011_H_

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
void SKY_API SKA_SorisamConnect(SKY_EVENT_T *pEvent);
void SKY_API SKA_SorisamRegi(SKY_EVENT_T *pEvent);
void SKY_API SKA_SorisamDirectRegi(SKY_EVENT_T *pEvent);
void SKY_API SKA_SorisamCancel(SKY_EVENT_T *pEvent);
void SKY_API SKA_SorisamEditing(SKY_EVENT_T *pEvent);

void SKY_API SKA_AutoConEdit(SKY_EVENT_T *pEvent);
void SKY_API SKA_AutoConCancel(SKY_EVENT_T *pEvent);
void SKY_API SKA_AutoConReg(SKY_EVENT_T *pEvent);

void SKY_API SKA_RevCallReg(SKY_EVENT_T *pEvent);
void SKY_API SKA_RevCallCancel(SKY_EVENT_T *pEvent);
void SKY_API SKA_RevCallEdit(SKY_EVENT_T *pEvent);

void SKY_API SKA_InfoSamCon(SKY_EVENT_T *pEvent);
void SKY_API SKA_InfoSamEdit(SKY_EVENT_T *pEvent);

void SKY_API SKA_TrafficInfoCon(SKY_EVENT_T *pEvent);
void SKY_API SKA_TrafficEdit(SKY_EVENT_T *pEvent);

void SKY_API SKA_InterCallCTRL(SKY_EVENT_T *pEvent);
void SKY_API SKA_AllCallCTRL(SKY_EVENT_T *pEvent);
void SKY_API SKA_SecretCancel(SKY_EVENT_T *pEvent);
void SKY_API SKA_SecretEdit(SKY_EVENT_T *pEvent);
void SKY_API SKA_HotMenu3Edit(SKY_EVENT_T *pEvent);
void SKY_API SKA_HotMenu4Edit(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainMenuService(SKY_EVENT_T *pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_011_H_ */
